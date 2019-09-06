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
     BYTESLITERAL = 263,
     CSTRINGLITERAL = 264,
     EXTERNCODE = 265,
     TALIGN = 266,
     TAS = 267,
     TATOMIC = 268,
     TBEGIN = 269,
     TBREAK = 270,
     TBOOL = 271,
     TBORROWED = 272,
     TBY = 273,
     TBYTES = 274,
     TCATCH = 275,
     TCLASS = 276,
     TCOBEGIN = 277,
     TCOFORALL = 278,
     TCOMPLEX = 279,
     TCONFIG = 280,
     TCONST = 281,
     TCONTINUE = 282,
     TDEFER = 283,
     TDELETE = 284,
     TDMAPPED = 285,
     TDO = 286,
     TDOMAIN = 287,
     TELSE = 288,
     TENUM = 289,
     TEXCEPT = 290,
     TEXPORT = 291,
     TEXTERN = 292,
     TFALSE = 293,
     TFOR = 294,
     TFORALL = 295,
     TFORWARDING = 296,
     TIF = 297,
     TIMAG = 298,
     TIN = 299,
     TINDEX = 300,
     TINLINE = 301,
     TINOUT = 302,
     TINT = 303,
     TITER = 304,
     TINITEQUALS = 305,
     TLABEL = 306,
     TLAMBDA = 307,
     TLET = 308,
     TLIFETIME = 309,
     TLOCAL = 310,
     TLOCALE = 311,
     TMINUSMINUS = 312,
     TMODULE = 313,
     TNEW = 314,
     TNIL = 315,
     TNOINIT = 316,
     TNONE = 317,
     TNOTHING = 318,
     TON = 319,
     TONLY = 320,
     TOTHERWISE = 321,
     TOUT = 322,
     TOVERRIDE = 323,
     TOWNED = 324,
     TPARAM = 325,
     TPLUSPLUS = 326,
     TPRAGMA = 327,
     TPRIMITIVE = 328,
     TPRIVATE = 329,
     TPROC = 330,
     TPROTOTYPE = 331,
     TPUBLIC = 332,
     TREAL = 333,
     TRECORD = 334,
     TREDUCE = 335,
     TREF = 336,
     TREQUIRE = 337,
     TRETURN = 338,
     TSCAN = 339,
     TSELECT = 340,
     TSERIAL = 341,
     TSHARED = 342,
     TSINGLE = 343,
     TSPARSE = 344,
     TSTRING = 345,
     TSUBDOMAIN = 346,
     TSYNC = 347,
     TTHEN = 348,
     TTHIS = 349,
     TTHROW = 350,
     TTHROWS = 351,
     TTRUE = 352,
     TTRY = 353,
     TTRYBANG = 354,
     TTYPE = 355,
     TUINT = 356,
     TUNDERSCORE = 357,
     TUNION = 358,
     TUNMANAGED = 359,
     TUSE = 360,
     TVAR = 361,
     TVOID = 362,
     TWHEN = 363,
     TWHERE = 364,
     TWHILE = 365,
     TWITH = 366,
     TYIELD = 367,
     TZIP = 368,
     TALIAS = 369,
     TAND = 370,
     TASSIGN = 371,
     TASSIGNBAND = 372,
     TASSIGNBOR = 373,
     TASSIGNBXOR = 374,
     TASSIGNDIVIDE = 375,
     TASSIGNEXP = 376,
     TASSIGNLAND = 377,
     TASSIGNLOR = 378,
     TASSIGNMINUS = 379,
     TASSIGNMOD = 380,
     TASSIGNMULTIPLY = 381,
     TASSIGNPLUS = 382,
     TASSIGNREDUCE = 383,
     TASSIGNSL = 384,
     TASSIGNSR = 385,
     TBANG = 386,
     TBAND = 387,
     TBNOT = 388,
     TBOR = 389,
     TBXOR = 390,
     TCOLON = 391,
     TCOMMA = 392,
     TDIVIDE = 393,
     TDOT = 394,
     TDOTDOT = 395,
     TDOTDOTDOT = 396,
     TEQUAL = 397,
     TEXP = 398,
     TGREATER = 399,
     TGREATEREQUAL = 400,
     THASH = 401,
     TIO = 402,
     TLESS = 403,
     TLESSEQUAL = 404,
     TMINUS = 405,
     TMOD = 406,
     TNOTEQUAL = 407,
     TOR = 408,
     TPLUS = 409,
     TQUESTION = 410,
     TSEMI = 411,
     TSHIFTLEFT = 412,
     TSHIFTRIGHT = 413,
     TSTAR = 414,
     TSWAP = 415,
     TLCBR = 416,
     TRCBR = 417,
     TLP = 418,
     TRP = 419,
     TLSBR = 420,
     TRSBR = 421,
     TNOELSE = 422,
     TUMINUS = 423,
     TUPLUS = 424,
     TLNOT = 425
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
#line 485 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 492 "bison-chapel.cpp"
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
#line 569 "bison-chapel.cpp"

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
#define YYLAST   17813

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  171
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  143
/* YYNRULES -- Number of rules.  */
#define YYNRULES  606
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1084

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   425

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
     165,   166,   167,   168,   169,   170
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
     171,   173,   174,   176,   178,   183,   190,   197,   201,   206,
     211,   216,   221,   226,   227,   229,   231,   233,   235,   237,
     239,   241,   243,   245,   247,   249,   251,   253,   255,   257,
     259,   261,   263,   265,   267,   269,   271,   273,   275,   277,
     279,   281,   283,   285,   287,   289,   291,   293,   295,   297,
     299,   301,   303,   305,   307,   309,   311,   313,   315,   317,
     319,   321,   323,   325,   328,   330,   333,   337,   339,   341,
     344,   347,   349,   351,   353,   355,   357,   359,   361,   365,
     371,   377,   380,   381,   390,   391,   401,   402,   411,   412,
     422,   426,   430,   436,   442,   445,   451,   455,   462,   469,
     474,   480,   486,   490,   494,   501,   507,   514,   520,   527,
     531,   536,   540,   545,   552,   560,   567,   575,   580,   586,
     591,   597,   602,   607,   611,   618,   624,   631,   637,   646,
     654,   657,   661,   665,   668,   671,   675,   679,   680,   683,
     686,   690,   696,   698,   702,   706,   712,   718,   719,   722,
     726,   729,   733,   740,   747,   749,   751,   753,   754,   757,
     758,   761,   765,   771,   777,   779,   781,   784,   788,   790,
     794,   795,   796,   805,   806,   808,   810,   811,   812,   823,
     827,   831,   837,   843,   847,   849,   853,   855,   858,   860,
     862,   864,   866,   868,   870,   872,   874,   876,   878,   880,
     882,   884,   886,   888,   890,   892,   894,   896,   898,   900,
     902,   904,   906,   908,   911,   913,   915,   917,   919,   921,
     923,   925,   927,   929,   931,   933,   935,   936,   940,   944,
     945,   947,   951,   956,   962,   967,   974,   981,   982,   984,
     986,   988,   990,   992,   995,   998,  1000,  1002,  1004,  1005,
    1007,  1009,  1012,  1014,  1016,  1018,  1020,  1021,  1023,  1026,
    1028,  1030,  1032,  1033,  1035,  1037,  1039,  1041,  1043,  1046,
    1048,  1051,  1054,  1055,  1058,  1061,  1066,  1071,  1073,  1077,
    1081,  1085,  1089,  1093,  1097,  1101,  1104,  1106,  1108,  1112,
    1117,  1122,  1125,  1130,  1131,  1134,  1137,  1139,  1141,  1143,
    1146,  1148,  1153,  1157,  1159,  1163,  1167,  1173,  1175,  1177,
    1181,  1183,  1186,  1190,  1191,  1194,  1197,  1201,  1204,  1209,
    1213,  1217,  1222,  1226,  1227,  1230,  1233,  1236,  1238,  1239,
    1242,  1245,  1248,  1250,  1255,  1260,  1267,  1271,  1272,  1274,
    1276,  1280,  1285,  1289,  1294,  1301,  1302,  1305,  1308,  1311,
    1314,  1316,  1318,  1322,  1326,  1328,  1332,  1334,  1336,  1338,
    1342,  1346,  1347,  1349,  1351,  1355,  1359,  1363,  1365,  1367,
    1369,  1371,  1373,  1376,  1378,  1380,  1382,  1384,  1386,  1388,
    1391,  1396,  1401,  1406,  1412,  1415,  1418,  1420,  1423,  1425,
    1428,  1430,  1433,  1435,  1438,  1440,  1442,  1444,  1451,  1458,
    1463,  1473,  1483,  1491,  1498,  1505,  1510,  1520,  1530,  1538,
    1543,  1550,  1557,  1567,  1577,  1584,  1586,  1588,  1590,  1592,
    1594,  1596,  1598,  1600,  1604,  1605,  1607,  1612,  1614,  1618,
    1623,  1625,  1629,  1634,  1638,  1642,  1644,  1646,  1649,  1651,
    1654,  1656,  1658,  1662,  1665,  1670,  1672,  1674,  1676,  1678,
    1680,  1682,  1684,  1686,  1691,  1695,  1699,  1702,  1705,  1707,
    1708,  1710,  1713,  1716,  1718,  1720,  1722,  1724,  1726,  1728,
    1731,  1734,  1736,  1741,  1746,  1751,  1755,  1759,  1763,  1767,
    1773,  1777,  1782,  1786,  1791,  1793,  1795,  1797,  1799,  1801,
    1803,  1805,  1807,  1809,  1811,  1815,  1820,  1824,  1829,  1833,
    1838,  1842,  1848,  1852,  1856,  1860,  1864,  1868,  1872,  1876,
    1880,  1884,  1888,  1892,  1896,  1900,  1904,  1908,  1912,  1916,
    1920,  1924,  1928,  1932,  1936,  1940,  1943,  1946,  1949,  1952,
    1955,  1958,  1961,  1965,  1969,  1973,  1977,  1981,  1985,  1989,
    1993,  1995,  1997,  1999,  2001,  2003,  2005
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     172,     0,    -1,   173,    -1,    -1,   173,   174,    -1,   176,
      -1,   175,   176,    -1,    72,     7,    -1,   175,    72,     7,
      -1,   178,    -1,   181,    -1,   187,    -1,   188,    -1,   199,
      -1,   189,    -1,   207,    -1,   210,    -1,   208,    -1,   217,
      -1,   211,    -1,   212,    -1,   216,    -1,   198,    -1,   287,
     156,    -1,    13,   176,    -1,    14,   288,   176,    -1,    15,
     190,   156,    -1,    22,   288,   181,    -1,    27,   190,   156,
      -1,    29,   275,   156,    -1,    51,   192,   176,    -1,    55,
     298,   197,    -1,    55,   197,    -1,    64,   298,   197,    -1,
      86,   298,   197,    -1,    86,   197,    -1,    92,   176,    -1,
     112,   298,   156,    -1,     1,   156,    -1,   179,   180,    58,
     192,    -1,   177,   161,   162,    -1,   177,   161,   182,   162,
      -1,   177,   161,     1,   162,    -1,    -1,    77,    -1,    74,
      -1,    -1,    76,    -1,   161,   162,    -1,   161,   182,   162,
      -1,   161,     1,   162,    -1,   174,    -1,   182,   174,    -1,
     298,    -1,   298,    12,   298,    -1,   183,   137,   298,    -1,
     183,   137,   298,    12,   298,    -1,    -1,   183,    -1,   159,
      -1,   183,    -1,    -1,    77,    -1,    74,    -1,   186,   105,
     274,   156,    -1,   186,   105,   298,    35,   185,   156,    -1,
     186,   105,   298,    65,   184,   156,    -1,    82,   274,   156,
      -1,   301,   238,   300,   156,    -1,   301,   160,   300,   156,
      -1,   301,   128,   300,   156,    -1,   301,   122,   300,   156,
      -1,   301,   123,   300,   156,    -1,    -1,     3,    -1,     3,
      -1,    62,    -1,    94,    -1,    38,    -1,    97,    -1,   194,
      -1,     3,    -1,    62,    -1,    94,    -1,    38,    -1,    97,
      -1,   194,    -1,     3,    -1,    94,    -1,    16,    -1,    48,
      -1,   101,    -1,    78,    -1,    43,    -1,    24,    -1,    19,
      -1,    90,    -1,    92,    -1,    88,    -1,    69,    -1,    87,
      -1,    17,    -1,   104,    -1,    32,    -1,    45,    -1,    56,
      -1,    63,    -1,   107,    -1,    16,    -1,    48,    -1,   101,
      -1,    78,    -1,    43,    -1,    24,    -1,    19,    -1,    90,
      -1,    56,    -1,    63,    -1,   107,    -1,    92,    -1,    88,
      -1,    32,    -1,    45,    -1,    31,   176,    -1,   181,    -1,
      83,   156,    -1,    83,   300,   156,    -1,   156,    -1,   200,
      -1,    77,   200,    -1,    74,   200,    -1,   232,    -1,   261,
      -1,   224,    -1,   257,    -1,   220,    -1,   201,    -1,   202,
      -1,    41,   298,   156,    -1,    41,   298,    35,   185,   156,
      -1,    41,   298,    65,   184,   156,    -1,    41,   261,    -1,
      -1,    37,    79,   203,   192,   222,   161,   223,   162,    -1,
      -1,    37,     7,    79,   204,   192,   222,   161,   223,   162,
      -1,    -1,    36,    79,   205,   192,   222,   161,   223,   162,
      -1,    -1,    36,     7,    79,   206,   192,   222,   161,   223,
     162,    -1,    37,   299,   232,    -1,    36,   299,   232,    -1,
      37,   299,   260,   262,   156,    -1,    36,   299,   260,   262,
     156,    -1,    37,    10,    -1,    31,   176,   110,   298,   156,
      -1,   110,   298,   197,    -1,    23,   298,    44,   298,   288,
     197,    -1,    23,   298,    44,   209,   288,   197,    -1,    23,
     298,   288,   197,    -1,    39,   298,    44,   298,   197,    -1,
      39,   298,    44,   209,   197,    -1,    39,   298,   197,    -1,
      39,   209,   197,    -1,    39,    70,   192,    44,   298,   197,
      -1,    40,   298,    44,   298,   197,    -1,    40,   298,    44,
     298,   291,   197,    -1,    40,   298,    44,   209,   197,    -1,
      40,   298,    44,   209,   291,   197,    -1,    40,   298,   197,
      -1,    40,   298,   291,   197,    -1,    40,   209,   197,    -1,
      40,   209,   291,   197,    -1,   165,   274,    44,   298,   166,
     176,    -1,   165,   274,    44,   298,   291,   166,   176,    -1,
     165,   274,    44,   209,   166,   176,    -1,   165,   274,    44,
     209,   291,   166,   176,    -1,   165,   274,   166,   176,    -1,
     165,   274,   291,   166,   176,    -1,   165,   209,   166,   176,
      -1,   165,   209,   291,   166,   176,    -1,   113,   163,   274,
     164,    -1,    42,   298,    93,   176,    -1,    42,   298,   181,
      -1,    42,   298,    93,   176,    33,   176,    -1,    42,   298,
     181,    33,   176,    -1,    42,   298,   238,   298,    93,   176,
      -1,    42,   298,   238,   298,   181,    -1,    42,   298,   238,
     298,    93,   176,    33,   176,    -1,    42,   298,   238,   298,
     181,    33,   176,    -1,    28,   176,    -1,    98,   298,   156,
      -1,    99,   298,   156,    -1,    98,   189,    -1,    99,   189,
      -1,    98,   181,   213,    -1,    99,   181,   213,    -1,    -1,
     213,   214,    -1,    20,   181,    -1,    20,   215,   181,    -1,
      20,   163,   215,   164,   181,    -1,   192,    -1,   192,   136,
     298,    -1,    95,   298,   156,    -1,    85,   298,   161,   218,
     162,    -1,    85,   298,   161,     1,   162,    -1,    -1,   218,
     219,    -1,   108,   274,   197,    -1,    66,   176,    -1,    66,
      31,   176,    -1,   221,   192,   222,   161,   223,   162,    -1,
     221,   192,   222,   161,     1,   162,    -1,    21,    -1,    79,
      -1,   103,    -1,    -1,   136,   274,    -1,    -1,   223,   199,
      -1,   223,   175,   199,    -1,   225,   192,   161,   226,   162,
      -1,   225,   192,   161,     1,   162,    -1,    34,    -1,   227,
      -1,   226,   137,    -1,   226,   137,   227,    -1,   192,    -1,
     192,   116,   298,    -1,    -1,    -1,    52,   229,   240,   230,
     247,   269,   253,   250,    -1,    -1,    46,    -1,    68,    -1,
      -1,    -1,   231,   246,   233,   235,   234,   247,   268,   248,
     253,   249,    -1,   245,   237,   239,    -1,   245,   238,   239,
      -1,   245,   236,   139,   237,   239,    -1,   245,   236,   139,
     238,   239,    -1,   245,     1,   239,    -1,   281,    -1,   163,
     298,   164,    -1,   191,    -1,   133,   192,    -1,   132,    -1,
     134,    -1,   135,    -1,   133,    -1,   142,    -1,   152,    -1,
     149,    -1,   145,    -1,   148,    -1,   144,    -1,   154,    -1,
     150,    -1,   159,    -1,   138,    -1,   157,    -1,   158,    -1,
     151,    -1,   143,    -1,   131,    -1,    18,    -1,   146,    -1,
      11,    -1,   160,    -1,   147,    -1,    50,    -1,   192,   131,
      -1,   116,    -1,   127,    -1,   124,    -1,   126,    -1,   120,
      -1,   125,    -1,   121,    -1,   117,    -1,   118,    -1,   119,
      -1,   130,    -1,   129,    -1,    -1,   163,   241,   164,    -1,
     163,   241,   164,    -1,    -1,   242,    -1,   241,   137,   242,
      -1,   243,   192,   273,   266,    -1,   175,   243,   192,   273,
     266,    -1,   243,   192,   273,   252,    -1,   243,   163,   265,
     164,   273,   266,    -1,   243,   163,   265,   164,   273,   252,
      -1,    -1,   244,    -1,    44,    -1,    47,    -1,    67,    -1,
      26,    -1,    26,    44,    -1,    26,    81,    -1,    70,    -1,
      81,    -1,   100,    -1,    -1,    70,    -1,    81,    -1,    26,
      81,    -1,    26,    -1,   100,    -1,    75,    -1,    49,    -1,
      -1,    26,    -1,    26,    81,    -1,    81,    -1,    70,    -1,
     100,    -1,    -1,    96,    -1,   156,    -1,   250,    -1,   181,
      -1,   198,    -1,   155,   192,    -1,   141,    -1,   141,   298,
      -1,   141,   251,    -1,    -1,   109,   298,    -1,    54,   254,
      -1,   109,   298,    54,   254,    -1,    54,   254,   109,   298,
      -1,   255,    -1,   254,   137,   255,    -1,   256,   116,   256,
      -1,   256,   148,   256,    -1,   256,   149,   256,    -1,   256,
     142,   256,    -1,   256,   144,   256,    -1,   256,   145,   256,
      -1,    83,   256,    -1,     3,    -1,    94,    -1,   100,   258,
     156,    -1,    25,   100,   258,   156,    -1,    37,   100,   258,
     156,    -1,   192,   259,    -1,   192,   259,   137,   258,    -1,
      -1,   116,   282,    -1,   116,   270,    -1,    70,    -1,    26,
      -1,    81,    -1,    26,    81,    -1,   106,    -1,    25,   260,
     262,   156,    -1,   260,   262,   156,    -1,   263,    -1,   262,
     137,   263,    -1,   192,   269,   266,    -1,   163,   265,   164,
     269,   266,    -1,   102,    -1,   192,    -1,   163,   265,   164,
      -1,   264,    -1,   264,   137,    -1,   264,   137,   265,    -1,
      -1,   116,    61,    -1,   116,   300,    -1,   165,   166,   282,
      -1,   165,   166,    -1,   165,   274,   166,   282,    -1,   165,
     274,   166,    -1,   165,   166,   267,    -1,   165,   274,   166,
     267,    -1,   165,     1,   166,    -1,    -1,   136,   282,    -1,
     136,   267,    -1,   136,   196,    -1,     1,    -1,    -1,   136,
     282,    -1,   136,   270,    -1,   136,   196,    -1,     1,    -1,
     165,   274,   166,   282,    -1,   165,   274,   166,   270,    -1,
     165,   274,    44,   298,   166,   282,    -1,   165,     1,   166,
      -1,    -1,   282,    -1,   251,    -1,   165,   166,   271,    -1,
     165,   274,   166,   271,    -1,   165,   166,   272,    -1,   165,
     274,   166,   272,    -1,   165,   274,    44,   298,   166,   271,
      -1,    -1,   136,   282,    -1,   136,   251,    -1,   136,   196,
      -1,   136,   272,    -1,   298,    -1,   251,    -1,   274,   137,
     298,    -1,   274,   137,   251,    -1,   298,    -1,   275,   137,
     298,    -1,   102,    -1,   300,    -1,   251,    -1,   276,   137,
     276,    -1,   277,   137,   276,    -1,    -1,   279,    -1,   280,
      -1,   279,   137,   280,    -1,   193,   116,   251,    -1,   193,
     116,   300,    -1,   251,    -1,   300,    -1,   193,    -1,   195,
      -1,   283,    -1,   283,   155,    -1,   155,    -1,   286,    -1,
     301,    -1,   285,    -1,   310,    -1,   309,    -1,    88,   298,
      -1,    45,   163,   278,   164,    -1,    32,   163,   278,   164,
      -1,    91,   163,   278,   164,    -1,    89,    91,   163,   278,
     164,    -1,    13,   298,    -1,    92,   298,    -1,    69,    -1,
      69,   298,    -1,   104,    -1,   104,   298,    -1,    87,    -1,
      87,   298,    -1,    17,    -1,    17,   298,    -1,    21,    -1,
      79,    -1,    34,    -1,    39,   298,    44,   298,    31,   298,
      -1,    39,   298,    44,   209,    31,   298,    -1,    39,   298,
      31,   298,    -1,    39,   298,    44,   298,    31,    42,   298,
      93,   298,    -1,    39,   298,    44,   209,    31,    42,   298,
      93,   298,    -1,    39,   298,    31,    42,   298,    93,   298,
      -1,    40,   298,    44,   298,    31,   298,    -1,    40,   298,
      44,   209,    31,   298,    -1,    40,   298,    31,   298,    -1,
      40,   298,    44,   298,    31,    42,   298,    93,   298,    -1,
      40,   298,    44,   209,    31,    42,   298,    93,   298,    -1,
      40,   298,    31,    42,   298,    93,   298,    -1,   165,   274,
     166,   298,    -1,   165,   274,    44,   298,   166,   298,    -1,
     165,   274,    44,   209,   166,   298,    -1,   165,   274,    44,
     298,   166,    42,   298,    93,   298,    -1,   165,   274,    44,
     209,   166,    42,   298,    93,   298,    -1,    42,   298,    93,
     298,    33,   298,    -1,    60,    -1,   286,    -1,   281,    -1,
     304,    -1,   303,    -1,   228,    -1,   296,    -1,   297,    -1,
     295,   147,   298,    -1,    -1,   289,    -1,   111,   163,   290,
     164,    -1,   293,    -1,   290,   137,   293,    -1,   111,   163,
     292,   164,    -1,   293,    -1,   292,   137,   293,    -1,   294,
     281,   269,   266,    -1,   313,    80,   281,    -1,   298,    80,
     281,    -1,    26,    -1,    44,    -1,    26,    44,    -1,    81,
      -1,    26,    81,    -1,   106,    -1,   301,    -1,   295,   147,
     298,    -1,    59,   298,    -1,    53,   262,    44,   298,    -1,
     307,    -1,   282,    -1,   284,    -1,   311,    -1,   312,    -1,
     228,    -1,   296,    -1,   297,    -1,   163,   141,   298,   164,
      -1,   298,   136,   298,    -1,   298,   140,   298,    -1,   298,
     140,    -1,   140,   298,    -1,   140,    -1,    -1,   298,    -1,
      98,   298,    -1,    99,   298,    -1,   298,    -1,   281,    -1,
     303,    -1,   304,    -1,   305,    -1,   301,    -1,   298,   131,
      -1,   283,   155,    -1,   228,    -1,   302,   163,   278,   164,
      -1,   302,   165,   278,   166,    -1,    73,   163,   278,   164,
      -1,   298,   139,   193,    -1,   298,   139,   100,    -1,   298,
     139,    32,    -1,   298,   139,    56,    -1,   298,   139,    19,
     163,   164,    -1,   163,   276,   164,    -1,   163,   276,   137,
     164,    -1,   163,   277,   164,    -1,   163,   277,   137,   164,
      -1,    38,    -1,    97,    -1,   306,    -1,     4,    -1,     5,
      -1,     6,    -1,     7,    -1,     8,    -1,     9,    -1,    62,
      -1,   161,   274,   162,    -1,   161,   274,   137,   162,    -1,
     165,   274,   166,    -1,   165,   274,   137,   166,    -1,   165,
     308,   166,    -1,   165,   308,   137,   166,    -1,   298,   114,
     298,    -1,   308,   137,   298,   114,   298,    -1,   298,   154,
     298,    -1,   298,   150,   298,    -1,   298,   159,   298,    -1,
     298,   138,   298,    -1,   298,   157,   298,    -1,   298,   158,
     298,    -1,   298,   151,   298,    -1,   298,   142,   298,    -1,
     298,   152,   298,    -1,   298,   149,   298,    -1,   298,   145,
     298,    -1,   298,   148,   298,    -1,   298,   144,   298,    -1,
     298,   132,   298,    -1,   298,   134,   298,    -1,   298,   135,
     298,    -1,   298,   115,   298,    -1,   298,   153,   298,    -1,
     298,   143,   298,    -1,   298,    18,   298,    -1,   298,    11,
     298,    -1,   298,   146,   298,    -1,   298,    30,   298,    -1,
     154,   298,    -1,   150,   298,    -1,    57,   298,    -1,    71,
     298,    -1,   131,   298,    -1,   298,   131,    -1,   133,   298,
      -1,   298,    80,   298,    -1,   298,    80,   209,    -1,   313,
      80,   298,    -1,   313,    80,   209,    -1,   298,    84,   298,
      -1,   298,    84,   209,    -1,   313,    84,   298,    -1,   313,
      84,   209,    -1,   154,    -1,   159,    -1,   115,    -1,   153,
      -1,   132,    -1,   134,    -1,   135,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   493,   493,   498,   499,   505,   506,   511,   512,   517,
     518,   519,   520,   521,   522,   523,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
     538,   539,   540,   541,   542,   543,   544,   545,   546,   550,
     563,   568,   573,   581,   582,   583,   587,   588,   601,   602,
     603,   608,   609,   614,   619,   624,   628,   635,   640,   644,
     649,   653,   654,   655,   659,   660,   661,   665,   669,   671,
     673,   675,   677,   684,   685,   689,   690,   691,   692,   693,
     694,   697,   698,   699,   700,   701,   702,   714,   715,   726,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   748,   749,
     750,   751,   752,   753,   754,   755,   756,   757,   758,   765,
     766,   767,   768,   772,   773,   777,   778,   782,   783,   784,
     785,   789,   790,   791,   792,   793,   794,   795,   799,   800,
     801,   802,   807,   806,   822,   821,   838,   837,   853,   852,
     868,   872,   877,   885,   896,   903,   904,   905,   906,   907,
     908,   909,   910,   911,   912,   913,   914,   915,   916,   917,
     918,   919,   920,   921,   927,   933,   939,   945,   952,   959,
     963,   970,   974,   975,   976,   977,   978,   980,   982,   984,
     989,   992,   993,   994,   995,   996,   997,  1001,  1002,  1006,
    1007,  1008,  1012,  1013,  1017,  1020,  1022,  1027,  1028,  1032,
    1034,  1036,  1043,  1053,  1067,  1072,  1077,  1085,  1086,  1091,
    1092,  1094,  1099,  1115,  1122,  1131,  1139,  1143,  1150,  1151,
    1156,  1161,  1155,  1188,  1191,  1195,  1203,  1213,  1202,  1245,
    1249,  1254,  1258,  1263,  1270,  1271,  1275,  1276,  1277,  1278,
    1279,  1280,  1281,  1282,  1283,  1284,  1285,  1286,  1287,  1288,
    1289,  1290,  1291,  1292,  1293,  1294,  1295,  1296,  1297,  1298,
    1299,  1300,  1301,  1302,  1306,  1307,  1308,  1309,  1310,  1311,
    1312,  1313,  1314,  1315,  1316,  1317,  1321,  1322,  1326,  1330,
    1331,  1332,  1336,  1338,  1340,  1342,  1344,  1349,  1350,  1354,
    1355,  1356,  1357,  1358,  1359,  1360,  1361,  1362,  1366,  1367,
    1368,  1369,  1370,  1371,  1375,  1376,  1380,  1381,  1382,  1383,
    1384,  1385,  1389,  1390,  1393,  1394,  1398,  1399,  1403,  1408,
    1409,  1410,  1418,  1419,  1421,  1423,  1425,  1430,  1432,  1437,
    1438,  1439,  1440,  1441,  1442,  1443,  1447,  1449,  1454,  1456,
    1458,  1463,  1476,  1493,  1494,  1496,  1501,  1502,  1503,  1504,
    1505,  1509,  1515,  1523,  1524,  1532,  1534,  1539,  1541,  1543,
    1548,  1550,  1552,  1559,  1560,  1561,  1566,  1568,  1570,  1574,
    1578,  1580,  1584,  1592,  1593,  1594,  1595,  1596,  1601,  1602,
    1603,  1604,  1605,  1625,  1629,  1633,  1641,  1648,  1649,  1650,
    1654,  1656,  1662,  1664,  1666,  1671,  1672,  1673,  1674,  1675,
    1681,  1682,  1683,  1684,  1688,  1689,  1693,  1694,  1695,  1699,
    1700,  1704,  1705,  1709,  1710,  1714,  1715,  1716,  1717,  1721,
    1722,  1733,  1735,  1737,  1743,  1744,  1745,  1746,  1747,  1748,
    1750,  1752,  1754,  1756,  1758,  1760,  1763,  1765,  1767,  1769,
    1771,  1773,  1775,  1777,  1780,  1782,  1784,  1789,  1791,  1793,
    1795,  1797,  1799,  1801,  1803,  1805,  1807,  1809,  1811,  1813,
    1820,  1826,  1832,  1838,  1847,  1857,  1865,  1866,  1867,  1868,
    1869,  1870,  1871,  1872,  1877,  1878,  1882,  1886,  1887,  1891,
    1895,  1896,  1900,  1904,  1908,  1915,  1916,  1917,  1918,  1919,
    1920,  1924,  1925,  1930,  1935,  1943,  1944,  1945,  1946,  1947,
    1948,  1949,  1950,  1951,  1953,  1955,  1957,  1959,  1961,  1966,
    1967,  1971,  1972,  1973,  1977,  1978,  1979,  1980,  1991,  1992,
    1993,  1994,  1998,  1999,  2000,  2004,  2005,  2006,  2007,  2008,
    2016,  2017,  2018,  2019,  2023,  2024,  2028,  2029,  2030,  2031,
    2032,  2033,  2034,  2035,  2036,  2037,  2038,  2039,  2040,  2044,
    2052,  2053,  2057,  2058,  2059,  2060,  2061,  2062,  2063,  2064,
    2065,  2066,  2067,  2068,  2069,  2070,  2071,  2072,  2073,  2074,
    2075,  2076,  2077,  2078,  2079,  2083,  2084,  2085,  2086,  2087,
    2088,  2089,  2093,  2094,  2095,  2096,  2100,  2101,  2102,  2103,
    2108,  2109,  2110,  2111,  2112,  2113,  2114
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "INTLITERAL", "REALLITERAL",
  "IMAGLITERAL", "STRINGLITERAL", "BYTESLITERAL", "CSTRINGLITERAL",
  "EXTERNCODE", "TALIGN", "TAS", "TATOMIC", "TBEGIN", "TBREAK", "TBOOL",
  "TBORROWED", "TBY", "TBYTES", "TCATCH", "TCLASS", "TCOBEGIN",
  "TCOFORALL", "TCOMPLEX", "TCONFIG", "TCONST", "TCONTINUE", "TDEFER",
  "TDELETE", "TDMAPPED", "TDO", "TDOMAIN", "TELSE", "TENUM", "TEXCEPT",
  "TEXPORT", "TEXTERN", "TFALSE", "TFOR", "TFORALL", "TFORWARDING", "TIF",
  "TIMAG", "TIN", "TINDEX", "TINLINE", "TINOUT", "TINT", "TITER",
  "TINITEQUALS", "TLABEL", "TLAMBDA", "TLET", "TLIFETIME", "TLOCAL",
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
  "TRP", "TLSBR", "TRSBR", "TNOELSE", "TUMINUS", "TUPLUS", "TLNOT",
  "$accept", "program", "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls",
  "stmt", "module_decl_start", "module_decl_stmt", "access_control",
  "opt_prototype", "block_stmt", "stmt_ls", "only_ls", "opt_only_ls",
  "except_ls", "use_access_control", "use_stmt", "require_stmt",
  "assignment_stmt", "opt_label_ident", "ident_fn_def", "ident_def",
  "ident_use", "internal_type_ident_def", "scalar_type",
  "reserved_type_ident_use", "do_stmt", "return_stmt", "class_level_stmt",
  "private_decl", "forwarding_stmt", "extern_export_decl_stmt", "$@1",
  "$@2", "$@3", "$@4", "extern_block_stmt", "loop_stmt",
  "zippered_iterator", "if_stmt", "defer_stmt", "try_stmt",
  "catch_stmt_ls", "catch_stmt", "catch_expr", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt", "enum_header",
  "enum_ls", "enum_item", "lambda_decl_expr", "$@5", "$@6", "linkage_spec",
  "fn_decl_stmt", "$@7", "$@8", "fn_decl_stmt_inner",
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
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   171,   172,   173,   173,   174,   174,   175,   175,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   177,
     178,   178,   178,   179,   179,   179,   180,   180,   181,   181,
     181,   182,   182,   183,   183,   183,   183,   184,   184,   185,
     185,   186,   186,   186,   187,   187,   187,   188,   189,   189,
     189,   189,   189,   190,   190,   191,   191,   191,   191,   191,
     191,   192,   192,   192,   192,   192,   192,   193,   193,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   196,
     196,   196,   196,   197,   197,   198,   198,   199,   199,   199,
     199,   200,   200,   200,   200,   200,   200,   200,   201,   201,
     201,   201,   203,   202,   204,   202,   205,   202,   206,   202,
     202,   202,   202,   202,   207,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   209,   210,   210,   210,   210,   210,   210,   210,   210,
     211,   212,   212,   212,   212,   212,   212,   213,   213,   214,
     214,   214,   215,   215,   216,   217,   217,   218,   218,   219,
     219,   219,   220,   220,   221,   221,   221,   222,   222,   223,
     223,   223,   224,   224,   225,   226,   226,   226,   227,   227,
     229,   230,   228,   231,   231,   231,   233,   234,   232,   235,
     235,   235,   235,   235,   236,   236,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   239,   239,   240,   241,
     241,   241,   242,   242,   242,   242,   242,   243,   243,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   245,   245,
     245,   245,   245,   245,   246,   246,   247,   247,   247,   247,
     247,   247,   248,   248,   249,   249,   250,   250,   251,   252,
     252,   252,   253,   253,   253,   253,   253,   254,   254,   255,
     255,   255,   255,   255,   255,   255,   256,   256,   257,   257,
     257,   258,   258,   259,   259,   259,   260,   260,   260,   260,
     260,   261,   261,   262,   262,   263,   263,   264,   264,   264,
     265,   265,   265,   266,   266,   266,   267,   267,   267,   267,
     267,   267,   267,   268,   268,   268,   268,   268,   269,   269,
     269,   269,   269,   270,   270,   270,   270,   271,   271,   271,
     272,   272,   272,   272,   272,   273,   273,   273,   273,   273,
     274,   274,   274,   274,   275,   275,   276,   276,   276,   277,
     277,   278,   278,   279,   279,   280,   280,   280,   280,   281,
     281,   282,   282,   282,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   285,   286,   287,   287,   287,   287,
     287,   287,   287,   287,   288,   288,   289,   290,   290,   291,
     292,   292,   293,   293,   293,   294,   294,   294,   294,   294,
     294,   295,   295,   296,   297,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   299,
     299,   300,   300,   300,   301,   301,   301,   301,   302,   302,
     302,   302,   303,   303,   303,   304,   304,   304,   304,   304,
     305,   305,   305,   305,   306,   306,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     308,   308,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   310,   310,   310,   310,   310,
     310,   310,   311,   311,   311,   311,   312,   312,   312,   312,
     313,   313,   313,   313,   313,   313,   313
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
       1,     0,     1,     1,     4,     6,     6,     3,     4,     4,
       4,     4,     4,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     2,     3,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     3,     5,
       5,     2,     0,     8,     0,     9,     0,     8,     0,     9,
       3,     3,     5,     5,     2,     5,     3,     6,     6,     4,
       5,     5,     3,     3,     6,     5,     6,     5,     6,     3,
       4,     3,     4,     6,     7,     6,     7,     4,     5,     4,
       5,     4,     4,     3,     6,     5,     6,     5,     8,     7,
       2,     3,     3,     2,     2,     3,     3,     0,     2,     2,
       3,     5,     1,     3,     3,     5,     5,     0,     2,     3,
       2,     3,     6,     6,     1,     1,     1,     0,     2,     0,
       2,     3,     5,     5,     1,     1,     2,     3,     1,     3,
       0,     0,     8,     0,     1,     1,     0,     0,    10,     3,
       3,     5,     5,     3,     1,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     3,     3,     0,
       1,     3,     4,     5,     4,     6,     6,     0,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     0,     1,
       1,     2,     1,     1,     1,     1,     0,     1,     2,     1,
       1,     1,     0,     1,     1,     1,     1,     1,     2,     1,
       2,     2,     0,     2,     2,     4,     4,     1,     3,     3,
       3,     3,     3,     3,     3,     2,     1,     1,     3,     4,
       4,     2,     4,     0,     2,     2,     1,     1,     1,     2,
       1,     4,     3,     1,     3,     3,     5,     1,     1,     3,
       1,     2,     3,     0,     2,     2,     3,     2,     4,     3,
       3,     4,     3,     0,     2,     2,     2,     1,     0,     2,
       2,     2,     1,     4,     4,     6,     3,     0,     1,     1,
       3,     4,     3,     4,     6,     0,     2,     2,     2,     2,
       1,     1,     3,     3,     1,     3,     1,     1,     1,     3,
       3,     0,     1,     1,     3,     3,     3,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     2,
       4,     4,     4,     5,     2,     2,     1,     2,     1,     2,
       1,     2,     1,     2,     1,     1,     1,     6,     6,     4,
       9,     9,     7,     6,     6,     4,     9,     9,     7,     4,
       6,     6,     9,     9,     6,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     1,     4,     1,     3,     4,
       1,     3,     4,     3,     3,     1,     1,     2,     1,     2,
       1,     1,     3,     2,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     3,     3,     2,     2,     1,     0,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     4,     4,     4,     3,     3,     3,     3,     5,
       3,     4,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     4,     3,     4,     3,     4,
       3,     5,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    87,   547,   548,   549,   550,
     551,   552,     0,   484,    73,   108,   452,   114,   454,   484,
       0,   113,     0,   357,    73,     0,     0,     0,     0,   456,
     519,   519,   544,     0,     0,     0,     0,   112,     0,   234,
     109,     0,   230,     0,     0,   116,     0,     0,   475,   553,
     117,     0,   235,   446,   356,     0,     0,     0,    45,    44,
     111,   455,   358,     0,     0,     0,     0,   450,     0,     0,
     115,     0,     0,    88,     0,   545,     0,     0,     0,   110,
     216,   448,   360,   118,     0,     0,   602,     0,   604,     0,
     605,   606,   518,     0,   603,   600,   433,   127,   601,     0,
       0,     0,     4,     0,     5,     0,     9,    46,    10,     0,
      11,    12,    14,   429,   430,    22,    13,   128,   136,   137,
      15,    17,    16,    19,    20,    21,    18,   135,     0,   133,
       0,   510,     0,   131,   134,     0,   132,   524,   506,   431,
     507,   436,   434,     0,     0,   511,   512,     0,   435,     0,
     525,   526,   527,   546,   505,   438,   437,   508,   509,     0,
      38,    24,   444,     0,     0,   485,    74,     0,     0,   454,
     456,     0,     0,     0,   455,     0,     0,     0,   510,   524,
     434,   511,   512,   453,   435,   525,   526,     0,   484,     0,
       0,   359,     0,   190,     0,   414,     0,   421,   550,   455,
     520,   233,   550,   154,   455,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   141,     0,     0,   421,    81,    89,
     101,    95,    94,   103,    84,    93,   104,    90,   105,    82,
     106,    99,    92,   100,    98,    96,    97,    83,    85,    91,
     102,   107,     0,    86,     0,     0,     0,     0,   363,     0,
     124,    32,     0,   587,   503,     0,   447,   588,     7,   421,
     214,   224,   519,   215,   130,   129,   433,   411,     0,   410,
       0,     0,   125,   523,     0,     0,    35,     0,   451,   439,
       0,   421,    36,   445,     0,   197,   193,     0,   435,   197,
     194,     0,   353,     0,   449,     0,     0,   589,   591,   517,
     586,   585,     0,    48,    51,     0,     0,   416,     0,   418,
       0,     0,   417,     0,     0,   410,     0,     0,     6,     0,
      47,     0,     0,   217,     0,   315,   314,   236,     0,   432,
      23,     0,     0,     0,     0,     0,     0,     0,   590,     0,
       0,     0,     0,     0,     0,   516,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   274,   281,   282,   283,   278,   280,     0,     0,   276,
     279,   277,   275,     0,   285,   284,     0,     0,   421,   421,
       0,     0,     0,    25,    26,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,    28,     0,    29,     0,   429,
     427,     0,   422,   423,   428,   148,     0,   151,     0,   144,
       0,     0,   150,     0,     0,     0,   163,     0,     0,   162,
       0,   171,     0,     0,     0,   169,     0,     0,    57,   138,
       0,   183,     0,     0,    30,   297,   231,   367,     0,   368,
     370,     0,   392,     0,   373,     0,     0,   123,    31,    33,
       0,   328,     0,    67,   521,   522,   126,     0,    34,   421,
       0,   204,   195,   191,   196,   192,     0,   351,   348,   156,
      37,    50,    49,    52,     0,   554,     0,     0,   540,     0,
     542,     0,     0,     0,     0,     0,     0,     0,     0,   558,
       8,     0,    40,     0,     0,     0,   410,     0,     0,     0,
     308,   362,   483,   582,   581,   584,   593,   592,   597,   596,
     578,   575,   576,   577,   514,   565,     0,   537,   538,   536,
     535,   515,   569,   580,   574,   572,   583,   573,   571,   563,
     568,   570,   579,   562,   566,   567,   564,     0,     0,     0,
       0,     0,     0,     0,   595,   594,   599,   598,   495,   496,
     498,   500,     0,   487,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   556,   484,   484,   159,   349,   361,   415,
       0,     0,   441,     0,     0,   217,     0,     0,   217,   350,
       0,     0,     0,     0,   459,     0,     0,     0,   172,     0,
     465,     0,     0,   170,   601,    60,     0,    53,    58,     0,
     182,     0,     0,     0,   440,   302,   299,   300,   301,   305,
     306,   307,   297,     0,   290,     0,   298,   316,     0,   371,
       0,   121,   122,   120,   119,     0,   391,   390,   506,     0,
     365,   504,   364,   534,   413,   412,     0,     0,     0,   442,
       0,   198,   355,   506,     0,   555,   513,   541,   419,   543,
     420,   179,     0,     0,     0,   557,   177,   469,     0,   560,
     559,     0,    42,    41,    39,    64,     0,    57,   218,     0,
       0,   228,     0,   225,   312,   309,   310,   313,   237,     0,
       0,    71,    72,    70,    69,    68,   532,   533,   497,   499,
       0,   486,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   155,   425,   426,   424,   217,
       0,   153,   217,     0,   152,     0,   181,     0,     0,   161,
       0,   160,     0,   490,     0,     0,   167,     0,     0,   165,
       0,     0,   139,     0,   140,     0,     0,   185,     0,   187,
     303,   304,     0,   297,   288,     0,   405,   317,   320,   319,
     321,     0,   369,   372,   373,     0,     0,   374,   375,   206,
       0,     0,   205,   208,   443,     0,   199,   202,     0,   352,
     180,     0,     0,     0,     0,   178,     0,     0,     0,     0,
     233,   223,     0,   226,   222,   311,   316,   286,    75,   269,
      89,   267,    95,    94,    78,    93,    90,   272,   105,    76,
     106,    92,    96,    77,    79,    91,   107,   266,   248,   251,
     249,   250,   261,   252,   265,   257,   255,   268,   271,   256,
     254,   259,   264,   253,   258,   262,   263,   260,   270,     0,
     246,     0,    80,     0,   286,   286,   244,   539,   488,   373,
     524,   524,     0,     0,     0,     0,     0,     0,     0,     0,
     158,   157,     0,   219,     0,   219,   164,     0,     0,   458,
       0,   457,     0,   489,     0,     0,   464,   168,     0,   463,
     166,    55,    54,   184,   474,   186,     0,   405,   291,     0,
       0,   373,   318,   332,   366,   396,     0,   556,     0,   210,
       0,     0,     0,   200,     0,   175,   471,     0,     0,   173,
     470,     0,   561,    65,    66,   213,   233,   233,   212,   233,
     220,   229,   227,     0,   297,   243,   247,     0,   273,     0,
     239,   240,   492,     0,     0,     0,     0,     0,     0,     0,
       0,   219,   233,   219,   233,   462,     0,     0,   491,   468,
       0,     0,     0,     0,   189,   373,   405,     0,   408,   407,
     409,   506,   329,   294,   292,     0,     0,     0,     0,   394,
     506,   211,   209,     0,   203,     0,   176,     0,   174,   221,
     387,     0,   322,     0,   245,    75,    77,   286,   286,     0,
       0,     0,     0,     0,     0,   233,   147,   233,   143,     0,
       0,     0,     0,    56,   188,   293,   373,   397,     0,   331,
     330,   346,     0,   347,   334,   337,     0,   333,   326,   327,
     232,     0,   201,     0,     0,     0,   386,   385,   506,   323,
     332,   287,   241,   242,     0,     0,     0,     0,     0,     0,
     149,   145,   461,   460,   467,   466,   296,   295,   399,   400,
     402,   506,     0,   556,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   506,   473,   472,     0,   377,     0,
       0,     0,   401,   403,   336,   338,   339,   342,   343,   344,
     340,   341,   335,   382,   380,   506,   556,   324,   238,   325,
     397,   381,   506,   404
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   304,   103,   600,   105,   106,   107,   321,
     108,   305,   595,   599,   596,   109,   110,   111,   112,   167,
     830,   246,   113,   243,   114,   626,   251,   115,   116,   117,
     118,   119,   410,   577,   406,   574,   120,   121,   701,   122,
     123,   124,   462,   641,   768,   125,   126,   637,   763,   127,
     128,   498,   780,   129,   130,   672,   673,   178,   244,   617,
     132,   133,   500,   786,   678,   833,   834,   432,   915,   436,
     613,   614,   615,   616,   679,   327,   751,  1020,  1078,  1010,
     267,   953,   957,  1004,  1005,  1006,   134,   293,   467,   135,
     136,   247,   248,   440,   441,   630,  1017,   972,   444,   627,
    1039,   950,   881,   306,   194,   310,   311,   401,   402,   403,
     179,   138,   139,   140,   141,   180,   143,   164,   165,   552,
     422,   722,   553,   554,   144,   181,   182,   147,   206,   404,
     184,   149,   185,   186,   152,   153,   154,   316,   155,   156,
     157,   158,   159
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -951
static const yytype_int16 yypact[] =
{
    -951,    48,  2784,  -951,   -58,  -951,  -951,  -951,  -951,  -951,
    -951,  -951,  4254,   109,   148,  -951, 13049,  -951, 15104,   109,
    9300,  -951,   165,    38,   148,  4254,  9300,  4254,    69, 16512,
    9463,  7680,  -951,  7843,  8485,  6539,  9300,  -951,    86,  -951,
    -951, 17535,  -951, 13392,  8648,  -951,  9300,  9300,  -951,  -951,
    -951,  9300,  -951, 13049,  -951,  9300,   255,   142,  8329, 15488,
    -951, 17592,  -951,  9626,  7354,  9300,  8648, 13049,  9300,   202,
    -951,   145,  4254,  -951,  9300,  -951,  9789,  9789, 17535,  -951,
    -951, 13049,  -951,  -951,  9300,  9300,  -951,  9300,  -951,  9300,
    -951,  -951, 12560,  9300,  -951,  9300,  -951,  -951,  -951,  3276,
    6702,  8811,  -951,  4091,  -951,   171,  -951,   278,  -951,   269,
    -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,
    -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951, 17535,  -951,
   17535,   263,   234,  -951,  -951, 13392,  -951,   236,  -951,   259,
    -951,  -951,   268,   273,   266,   283,   295, 17204, 13046,   323,
     302,   341,  -951,  -951,  -951,  -951,  -951,  -951,  -951,   379,
    -951,  -951, 17204,   316,  4254,  -951,  -951,   348,  9300,  -951,
    -951,  9300,  9300,  9300,  -951,  9300,  9626,  9626,   360,  -951,
    -951,  -951,  -951,     3,   371,  -951,  -951,   344, 15177, 17535,
   13392,  -951,   353,  -951,   -38, 17204,   403,  8006,   440, 17649,
   17204,   362,   443,  -951, 17706, 17535,   362, 17535,   365,    47,
   14711,    15, 14679,   292,  -951, 15120, 13927,  8006,  -951,  -951,
    -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,
    -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,
    -951,  -951,  4254,  -951,   368,  2474,   101,    40,  -951,  4254,
    -951,  -951, 14855,  1017,   388, 14855,     3,  1017,  -951,  8006,
    -951,  -951,  8974,  -951,  -951,  -951, 17535,  -951,   -16, 17204,
    9300,  9300,  -951, 17204,   396, 15088,  -951, 14855,     3, 17204,
     372,  8006,  -951, 17204, 15264,  -951,  -951, 15529,  1635,  -951,
    -951, 15578,   428,   407,     3, 14855, 15673,   168,   168,  1878,
    1017,  1017,   132,  -951,  -951,  3439,   -29,  -951,  9300,  -951,
      97,   116,  -951,   -19,    57, 15722,   -22,   558,  -951,  3602,
    -951,   521,  9626,   444,   420,  -951,  -951,  -951,   216,   375,
    -951,  9300,  9300,  9300,  9300,  8485,  8485,  9300,   378,  9300,
    9300,  9300,  9300,  9300,   257, 12560,  9300,  9300,  9300,  9300,
    9300,  9300,  9300,  9300,  9300,  9300,  9300,  9300,  9300,  9300,
    9300,  -951,  -951,  -951,  -951,  -951,  -951,  8169,  8169,  -951,
    -951,  -951,  -951,  8169,  -951,  -951,  8169,  8169,  8006,  8006,
    8485,  8485,  6865,  -951,  -951, 15321, 15353, 15754,     6,  3765,
    -951,  8485,    47,   427,   253,  -951,  9300,  -951,  9300,   468,
    -951,   423,   453,  -951,  -951,  -951, 17535,  -951, 13392,  -951,
   17535,   441,  -951, 13392,   554,  9626,  -951,  4417,  8485,  -951,
     436,  -951,    47,  4580,  8485,  -951,    47,  9952,  9300,  -951,
    4254,   570,  9300,   445,  -951,   486,  -951,  -951,  2474,  -951,
     467,   446,  -951, 10115,   489,  9300, 13392,  -951,  -951,  -951,
     450,  -951,  9626,  -951, 17204, 17204,  -951,    30,  -951,  8006,
     451,  -951,   587,  -951,   587,  -951, 10278,   479,  -951,  -951,
    -951,  -951,  -951,  -951,  9137,  -951, 13783,  7028,  -951,  7191,
    -951,  4254,   452,  8485,  6047,  3113,   454,  9300,  6211,  -951,
    -951,   293,  -951,  3928, 17535,   265, 15497,  9626,   458, 16336,
     408,  -951, 15898, 17391, 17391,   364,  -951,   364,  -951,   364,
   12715,  1487,   494,   661,     3,   168,   464,  -951,  -951,  -951,
    -951,  1878,  2587,   364,  1169,  1169, 17391,  1169,  1169,  2323,
     168,  2587,  2083,  2323,  1017,  1017,   168,   465,   475,   478,
     482,   483,   471,   477,  -951,   364,  -951,   364,   -11,  -951,
    -951,  -951,   126,  -951,  1631, 17280,   418, 10441,  8485, 10604,
    8485,  9300,  8485, 13313,   109, 15943,  -951,  -951,  -951, 17204,
   15984,  8006,  -951,  8006, 17535,   444,   285, 17535,   444,  -951,
     288,  9300,   137,  9300, 17204,    54, 14912,  6865,  -951,  9300,
   17204,    28, 14768,  -951,   488,   510,   492, 16095,   510,   500,
     624, 16135,  4254, 14944,  -951,    42,  -951,  -951,  -951,  -951,
    -951,  -951,   926,   138,  -951, 13449,  -951,   410,   501,  2474,
     101,    69,    86,  9300,  9300,  5884,  -951,  -951,   243,  7517,
    -951, 17204,  -951,  -951,  -951, 17204,   496,    -8,   506,  -951,
    1347,  -951,  -951,   290, 17535,  -951,  -951,  -951,  -951,  -951,
    -951,  -951,  4254,   -17,  1449,  -951,  -951, 17204,  4254, 17204,
    -951, 16176,  -951,  -951,  -951,  -951,  9952,  9300,   534,  1105,
     513,   560,    92,  -951,   596,  -951,  -951,  -951,  -951,  8324,
     515,  -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,
    6865,  -951,    46,  8485,  8485,  9300,   653, 16320,  9300,   656,
   16352,   522, 13597,    47,    47,  -951,  -951,  -951,  -951,   444,
     531,  -951,   444,   532,  -951, 14855,  -951, 14003,  4743,  -951,
    4906,  -951,   213,  -951, 14079,  5069,  -951,    47,  5232,  -951,
      47,  9300,  -951,  9300,  -951,  4254,  9300,  -951,  4254,   665,
    -951,  -951, 17535,  1012,  -951,  2474,   563,   613,  -951,  -951,
    -951,    75,  -951,  -951,   489,   535,    59,  -951,  -951,  -951,
    5395,  9626,  -951,  -951,  -951, 17535,  -951,   566,   344,  -951,
    -951,  5558,   537,  5721,   538,  -951,  9300,   549,   556,   548,
    2074,  -951,  9300, 17535,  -951,  -951,   410,   552,   258,  -951,
     578,  -951,   579,   581,   592,   591,   593,  -951,   594,   595,
     597,   598,   600,   311,   603,   601,   605,  -951,  -951, 17535,
    -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,
    -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,  9300,
    -951,   604,   611,   607,   552,   552,  -951,  -951,  -951,   489,
     221,   223, 16496, 10767, 10930, 16528, 11093, 11256, 11419, 11582,
    -951,  -951,   586,  -951,   589,  -951,  -951,  4254,  9300, 17204,
    9300, 17204,  6865,  -951,  4254,  9300, 17204,  -951,  9300, 17204,
    -951, 16585, 17204,  -951, 17204,   715,  4254,   563,  -951,   588,
   11745,   176,  -951,    66,  -951,  -951,  8485, 13181,  4254,  -951,
      13,   590,  9300,  -951,  9300,  -951, 17204,  4254,  9300,  -951,
   17204,  4254, 17204,  -951,  -951,  -951, 12600, 12600,  -951,  1817,
    -951, 17204,  -951,    70,   486,  -951,  -951, 13829,  -951, 13541,
    -951,  -951,  -951,  9300,  9300,  9300,  9300,  9300,  9300,  9300,
    9300,  -951,  2248,  -951,  2614, 16135, 14155, 14231,  -951, 16135,
   14307, 14383,  9300,  4254,  -951,   489,   563,  6375,  -951,  -951,
    -951,    -9,  9626,  -951,  -951,   124,  9300,    29, 13685,  -951,
     485,   403,  -951,   344, 17204, 14459,  -951, 14535,  -951,  -951,
    -951, 11908,   657,   246,  -951,   627,   634,   552,   552, 16672,
   16748, 16824, 16900, 16976, 17052, 15909,  -951, 17415,  -951,  4254,
    4254,  4254,  4254, 17204,  -951,  -951,   176, 12071,    62,  -951,
   17204,  -951,    76,  -951,   -42,  -951,   524, 17128,  -951,  -951,
    -951, 11582,  -951,  4254,  4254,  2947,  -951,  -951,   381,  -951,
      66,  -951,  -951,  -951,  9300,  9300,  9300,  9300,  9300,  9300,
    -951,  -951, 16135, 16135, 16135, 16135,  -951,  -951,  -951,  -951,
    -951,   114,  8485, 12886,  -951,  9300,   124,    76,    76,    76,
      76,    76,    76,   124,   646, 16135, 16135,   602, 12397,    67,
     130, 13742,  -951,  -951, 17204,  -951,  -951,  -951,  -951,  -951,
    -951,  -951,   636,  -951,  -951,   642, 12723,  -951,  -951,  -951,
   12234,  -951,   660,  -951
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -951,  -951,  -951,     1,  -326,  1876,  -951,  -951,  -951,  -951,
     305,   447,  -401,   104,   108,  -951,  -951,  -951,   196,   751,
    -951,   186,  -177,  -663,  -951,  -823,   629,  -929,  -763,   -46,
    -951,  -951,  -951,  -951,  -951,  -951,  -951,  -951,   102,  -951,
    -951,  -951,   487,  -951,    12,  -951,  -951,  -951,  -951,  -951,
    -951,  -512,  -555,  -951,  -951,  -951,    -5,   755,  -951,  -951,
    -951,   247,  -951,  -951,  -951,  -951,  -140,  -147,  -797,  -951,
    -133,    41,   173,  -951,  -951,  -951,     0,  -951,  -951,  -273,
      22,  -208,  -231,  -263,  -255,  -807,  -951,  -160,  -951,     8,
     759,  -129,   363,  -951,  -433,  -729,  -781,  -951,  -578,  -464,
    -950,  -906,  -841,   -52,  -951,    85,  -951,  -191,  -951,   235,
     564,  -369,  -951,  -951,  -951,  1064,  -951,   -10,  -951,  -951,
    -197,  -951,  -577,  -951,  -951,  1399,  1608,   -12,   781,    93,
     964,  -951,  1715,  1769,  -951,  -951,  -951,  -951,  -951,  -951,
    -951,  -951,  -375
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -557
static const yytype_int16 yytable[] =
{
     162,   377,   642,   102,   183,   618,   328,   556,   188,   187,
     723,   268,   264,   265,   195,   426,   832,   910,   200,   200,
     399,   210,   212,   215,   216,   884,   433,   598,  1009,   393,
     190,   636,   252,   688,   253,   254,   945,   920,   921,   255,
     399,   256,   754,   257,   249,   411,   249,   442,     3,   314,
     562,   269,   273,   275,   277,   278,   279,   948,   760,   725,
     283,   394,   284,   710,   287,   291,   713,  1045,   450,   294,
     689,   970,   295,   296,   628,   297,   442,   298,   249,  1001,
     299,   300,   399,   301,   445,   718,   740,   269,   273,   315,
     460,  1040,   420,  1062,   420,  1046,  -207,   643,   160,   396,
     761,   483,   442,   886,   399,   996,  1042,  -406,   474,   612,
     922,   562,    64,   838,   839,   488,   482,   486,   397,   191,
     955,   452,   309,   741,  -383,   388,   420,  1001,  -406,  -388,
    1083,  1009,  -406,   475,   338,   209,   211,  1063,  -207,   420,
     453,   377,   344,   484,   489,  -388,   969,   481,  1016,   771,
     452,   166,   954,  -383,   762,  -406,   162,   274,  -388,   385,
     386,   387,  -388,   283,   269,   315,  -383,   520,   420,   910,
    1003,   910,   563,   883,   389,   956,   389,   446,   392,  -383,
    1022,  1023,   443,  -388,  -388,   273,   753,   542,   543,   389,
     389,    23,  -207,   312,   484,  1044,   484,   852,   334,   484,
     854,   399,   399,   313,   484,   273,   971,  1002,   389,   408,
    -388,   443,   556,    64,   413,   389,   995,  -388,  1003,   400,
     163,   190,   910,   485,   910,   887,  -383,   242,  1043,   783,
    -398,  -383,   197,  1076,   477,    54,  -388,   443,  -388,   400,
    1066,  1067,  1068,  1069,  1070,  1071,    62,   273,   335,   217,
     200,  -398,   336,   479,   784,  -398,   832,  -388,   454,   455,
       5,   478,   258,   690,   292,   189,   598,  1037,   638,   273,
     495,    82,   286,   290,   452,   743,   516,  1074,  -398,   576,
     480,   400,   399,   325,   580,   938,  1077,  -389,   160,   517,
     691,   389,   629,   280,   471,  1081,   476,  -389,   932,   338,
     934,   716,   744,   400,   342,   259,   473,   344,   281,   326,
     496,   347,   879,   518,   323,   556,   324,   952,    23,   502,
     503,   504,   505,   507,   509,   510,  -389,   511,   512,   513,
     514,   515,   319,   521,   522,   523,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   250,
     862,    73,  -389,   446,   320,   273,   273,   519,  -494,  -389,
    -493,   273,    54,   582,   273,   273,   273,   273,   545,   547,
     555,   250,   501,    62,   322,   292,   985,   863,   987,   565,
    -389,   285,   289,   743,   569,  -494,   570,  -493,    23,   -81,
     446,   292,  -477,   414,   727,   730,   399,   -87,    82,  -389,
     400,   400,   452,   269,  -389,   584,   586,  -389,    39,   568,
    1021,   590,   592,   331,   329,   597,   597,   612,   601,  -480,
     603,   665,   446,   959,  -476,   446,  -531,  -354,  -531,   330,
      52,   439,    54,   631,   674,  -384,   747,   506,   508,  -481,
     635,   711,   -83,    62,   714,   668,  -354,   273,   407,   160,
     -88,  -482,   451,   412,   909,   662,   772,   774,  -479,   380,
     537,   538,   635,   381,  -384,   273,   539,   273,    82,   540,
     541,   654,   635,   657,   634,   659,   661,  -384,   675,   382,
     748,   400,   544,   546,   769,   269,   378,   556,   379,   676,
    -384,   749,   390,   564,   473,   338,   634,  -478,   694,   309,
     342,   309,   381,   344,   384,   389,   634,   347,   677,   395,
     750,   951,   605,   398,   250,   250,   250,   250,   960,   405,
     585,   431,   409,  -531,   334,  -531,   591,   344,   415,  -393,
     606,   435,   835,   607,  -528,   459,  -528,  -384,  -530,  -393,
    -530,  -529,  -384,  -529,   466,   584,   697,   590,   700,   601,
     702,   657,   456,   608,   703,   704,   609,   250,    56,   273,
     250,   273,   648,   468,   650,   490,   137,   610,  -393,   715,
     312,   717,   312,   756,   335,   555,   137,   724,   336,   494,
     497,   499,   250,   567,   571,   653,   611,   572,   612,   137,
     573,   137,   575,   706,  -393,   400,   578,   579,   581,   587,
     250,  -393,  1018,   602,   619,   629,   909,   640,   909,   604,
     620,   279,   283,   315,   633,   639,   644,   273,   652,   669,
     658,   681,  -393,  -289,   439,   338,   339,   680,  1041,   341,
     342,   682,   343,   344,   683,   686,   137,   347,   684,   685,
    1047,  -393,  1054,   687,   -59,   354,  -393,   731,   732,  -393,
    -289,   358,   359,   360,   597,   597,   734,   735,   759,   909,
     696,   909,   699,   137,   707,   752,  1048,   137,  1049,  1050,
     764,   452,  1051,  1052,  1041,   781,   782,   785,   555,   837,
     664,   507,   545,   842,   843,   671,   845,   846,   848,  1075,
    -395,   334,   853,   855,   882,   276,  -376,   250,   876,   880,
    -395,   885,   892,   897,   901,   903,   859,  1082,   861,   890,
     905,  1041,   904,   866,  -378,   914,   869,  -108,  -114,   871,
    -113,   872,   758,   -84,   874,  -376,   -82,   250,   137,  -395,
    -112,   250,  -109,  -116,   -85,   918,  -117,  -111,  -376,  -115,
    -110,   335,   -86,  -378,  -118,   336,   919,   931,   943,   269,
     933,  -376,   946,  1019,   963,  -395,  -378,   131,   -81,   896,
     709,   900,  -395,   712,   902,   -83,   493,   131,  1073,  -378,
     911,   778,   978,  1046,   777,   192,   464,   891,   912,   977,
     131,   973,   131,  -395,   878,   742,   913,  1079,  1036,  1060,
    1072,  1065,   338,   339,   214,   506,   544,   342,  -376,   343,
     344,   746,  -395,  -376,   347,   439,   137,  -395,   708,   632,
    -395,   201,   354,   137,     0,     0,  -378,   917,   358,   359,
     360,  -378,     0,     0,     0,     0,   767,   131,     0,     0,
     292,   859,   861,     0,   866,   869,   896,   900,   416,   419,
     421,   425,     0,     0,     0,   935,   936,     0,   937,     0,
     555,     0,   939,   940,   131,     0,   941,     0,   131,     0,
     264,   265,     0,     0,     0,   831,     0,     0,     0,   137,
       0,     0,     0,     0,   958,   657,     0,     0,     0,     0,
     964,   448,   965,   137,   449,     0,   967,     0,     0,     0,
     250,   250,     0,     0,     0,   998,   250,   250,     0,     0,
       0,     0,   949,     0,     0,     0,   458,     0,   739,     0,
       0,   935,   979,   980,   939,   981,   982,   983,   984,   131,
       0,     0,     0,     0,   469,     0,     0,     0,   877,     0,
     993,   439,     0,     0,     0,   315,     0,     0,     0,     0,
    1000,     0,     0,     0,  1007,   766,     0,     0,     0,     0,
       0,   767,   605,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1059,     0,     0,   148,     0,     0,   671,
     606,     0,     0,   607,   999,     0,   148,  1032,  1033,  1034,
    1035,   137,     0,     0,     0,     0,     0,   137,     0,   148,
       0,   148,     0,   608,   137,   916,   609,   131,   317,   900,
       0,  1055,  1056,   315,   131,     0,     0,   610,   250,   250,
       0,     0,  1032,  1033,  1034,  1035,  1055,  1056,     0,  1038,
     250,   566,   431,     0,     0,     0,   611,     0,     0,   431,
    1061,   657,   250,  1064,     0,   250,   148,     0,   605,     0,
     288,   288,     0,     0,     0,   137,     0,   334,     0,   137,
       0,   588,     0,     0,     0,   593,   606,   137,     0,   607,
     131,     0,     0,   148,   657,  1038,   142,   148,   900,     0,
       0,     0,     0,   893,   131,     0,   142,     0,     0,   608,
       0,     0,   609,     0,    56,     0,     0,     0,     0,   142,
       0,   142,     0,   610,     0,     0,     0,   335,     0,     0,
       0,   336,  1038,     0,     0,   831,   779,     0,     0,     0,
       0,     0,   611,     0,     0,     0,     0,     0,   692,     0,
       0,     0,     0,     0,     0,     0,  -219,     0,   148,     0,
    -219,  -219,     0,     0,     0,     0,   142,     0,     0,  -219,
       0,  -219,  -219,     0,   131,     0,  -219,     0,   338,     0,
       0,  -219,     0,   342,  -219,   343,   344,     0,     0,     0,
     347,     0,     0,   142,     0,     0,   137,   142,   354,     0,
       0,     0,   131,  -219,     0,  -219,   360,  -219,   131,  -219,
    -219,     0,  -219,     0,  -219,   131,  -219,     0,     0,     0,
       0,     0,     0,     0,     0,   250,     0,     0,     0,   334,
       0,     0,     0,     0,     0,  -219,   148,     0,  -219,     0,
       0,  -219,     0,   148,   719,   721,   137,     0,     0,     0,
     726,   729,   137,     0,     0,     0,     0,     0,   142,     0,
       0,     0,     0,     0,     0,     0,   131,     0,     0,     0,
     131,   431,   431,   836,     0,   431,   431,     0,   131,   335,
       0,     0,     0,   336,     0,     0,     0,   840,   841,     0,
       0,  -219,  1008,     0,     0,     0,     0,  -219,  1012,   148,
     431,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   137,   148,   137,     0,     0,     0,     0,   137,
       0,     0,   137,     0,     0,     0,     0,     0,     0,   137,
     338,   339,   137,   340,   341,   342,   142,   343,   344,   345,
       0,     0,   347,   142,     0,     0,     0,     0,     0,   353,
     354,     0,     0,   357,   137,     0,   358,   359,   360,     0,
       0,     0,   850,   851,     0,   137,     0,   137,     0,     0,
       0,     0,     0,     0,   856,     0,     0,     0,     0,     0,
     218,     0,     0,   148,     0,     0,   867,   131,     0,   870,
       0,     0,     0,   219,   220,  1008,   221,     0,     0,   142,
       0,   222,     0,     0,     0,     0,     0,     0,     0,   223,
       0,   148,     0,   142,     0,   224,     0,   148,     0,     0,
     225,     0,   226,     0,   148,   227,     0,     0,     0,     0,
       0,   145,     0,   228,     0,     0,     0,   131,     0,   229,
     230,   145,     0,   131,     0,     0,   231,     0,     0,     0,
       0,   137,     0,     0,   145,   232,   145,     0,   137,     0,
       0,     0,     0,     0,   233,   234,     0,   235,     0,   236,
     137,   237,     0,     0,   238,   148,     0,     0,   239,   148,
       0,   240,   137,   142,   241,     0,     0,   148,     0,     0,
     332,   137,     0,     0,     0,   137,     0,   333,     0,     0,
       0,   145,     0,   131,     0,   131,     0,     0,     0,   334,
     131,   142,     0,   131,     0,     0,     0,   142,     0,     0,
     131,     0,     0,   131,   142,     0,     0,     0,   145,     0,
       0,     0,   145,     0,     0,     0,     0,   137,   389,     0,
     765,     0,     0,     0,     0,   131,     0,   334,     0,   962,
       0,     0,     0,     0,     0,     0,   131,     0,   131,   335,
       0,     0,     0,   336,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   142,     0,     0,     0,   142,
       0,     0,     0,   137,   137,   137,   137,   142,     0,     0,
     420,     0,     0,   145,   337,     0,   148,   335,     0,     0,
       0,   336,     0,     0,     0,     0,     0,   137,   137,     0,
     338,   339,     0,   340,   341,   342,     0,   343,   344,   345,
       0,   346,   347,   348,   349,   350,     0,   351,   352,   353,
     354,   355,   356,   357,     0,     0,   358,   359,   360,     0,
     146,     0,   131,     0,     0,   773,   148,     0,   338,   131,
     146,     0,   148,   342,     0,   343,   344,     0,     0,     0,
     347,   131,     0,   146,     5,   146,     0,     0,   354,     0,
       0,   145,     0,   131,   358,   359,   360,    15,   145,     0,
      17,     0,   131,     0,     0,    21,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   142,     0,     0,     0,
       0,     0,     0,     0,    37,     0,     0,     0,     0,    40,
     146,     0,   148,     0,   148,     0,     0,    45,     0,   148,
       0,     0,   148,     0,    50,     0,     0,     0,   131,   148,
       0,     0,   148,     0,   145,     0,     0,   146,     0,    60,
       0,   146,     0,     0,     0,     0,   142,   150,   145,     0,
       0,    70,   142,     0,   148,    73,     0,   150,     0,     0,
       0,     0,    79,     0,     0,   148,     0,   148,    83,     0,
     150,     0,   150,     0,   131,   131,   131,   131,     0,     0,
       0,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,     0,     0,   131,   131,
       0,   151,   146,     0,     0,     0,     0,     0,     0,     0,
       0,   151,   142,     0,   142,     0,     0,   150,   145,   142,
       0,     0,   142,     0,   151,   376,   151,     0,  -528,   142,
    -528,     0,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   150,     0,   145,     0,   150,     0,
       0,   148,   145,     0,   142,     0,     0,     0,   148,   145,
       0,     0,     0,     0,     0,   142,     0,   142,   260,     0,
     148,   151,    22,    23,     0,     0,     0,     0,     0,     0,
     146,   261,   148,    30,   262,     0,     0,   146,    35,     0,
       0,   148,     0,    39,     0,   148,     0,     0,   151,     0,
       0,     0,   151,     0,     0,     0,     0,     0,   104,   150,
     145,     0,     0,     0,   145,    52,     0,    54,   161,   317,
       0,   906,   145,     0,   907,     0,   263,     0,    62,     0,
       0,   193,     0,   196,     0,     0,     0,   148,   334,     0,
       0,     0,     0,   146,     0,     0,     0,    78,     0,     0,
      80,   142,     0,    82,     0,     0,     0,   146,   142,     0,
       0,     0,     0,   151,     0,     0,     0,     0,     0,     0,
     142,     0,     0,     0,     0,     0,     0,     0,   282,     0,
       0,     0,   142,   148,   148,   148,   148,   150,   335,     0,
       0,   142,   336,     0,   150,   142,     0,     0,     0,     0,
       0,     0,     0,    97,     0,   104,     0,   148,   148,   318,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   146,     0,     0,
       0,   145,     0,     0,     0,     0,     0,   142,     0,   338,
     339,   151,   340,   341,   342,     0,   343,   344,   151,     0,
     150,   347,     0,     0,     0,   146,     0,     0,   353,   354,
       0,   146,   357,     0,   150,   358,   359,   360,   146,     0,
     383,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   145,     0,   142,   142,   142,   142,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   151,     0,     0,   142,   142,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   151,   146,
       0,     0,     0,   146,     0,   260,     0,     0,     0,    22,
      23,   146,     0,     0,   150,     0,     0,     0,   261,     0,
      30,   262,     0,   334,     0,    35,     0,   145,   434,   145,
      39,     0,     0,     0,   145,   447,     0,   145,     0,     0,
       0,     0,   150,     0,   145,     0,     0,   145,   150,     0,
       0,     0,    52,     0,    54,   150,    56,     0,   906,     0,
       0,   907,     0,   263,     0,    62,     0,     0,   151,   145,
       0,     0,     0,   335,     0,     0,     0,   336,     0,     0,
     145,     0,   145,     0,    78,     0,     0,    80,     0,     0,
      82,   104,     0,     0,     0,     0,   151,     0,     0,     0,
       0,     0,   151,     0,     0,   104,   150,     0,   337,   151,
     150,     0,     0,     0,     0,     0,     0,     0,   150,     0,
     146,     0,     0,     0,   338,   339,     0,   340,   341,   342,
       0,   343,   344,   345,     0,   346,   347,   348,   349,     0,
      97,   351,   352,   353,   354,   355,   908,   357,     0,     0,
     358,   359,   360,     0,     0,     0,     0,     0,     0,     0,
     151,     0,     0,     0,   151,     0,   145,     0,     0,     0,
     146,     0,   151,   145,     0,   104,   146,     0,     0,   260,
       0,     0,     0,    22,    23,   145,     0,     0,     0,     0,
       0,     0,   261,     0,    30,   262,     0,   145,     0,    35,
       0,     0,     0,   447,    39,     0,   145,     0,     0,   447,
     145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,   150,    54,     0,
      56,     0,   906,     0,     0,   907,   146,   263,   146,    62,
       0,     0,     0,   146,     0,     0,   146,     0,     0,     0,
       0,     0,   145,   146,     0,     0,   146,     0,    78,     0,
       0,    80,     0,   334,    82,     0,     0,   651,     0,     0,
       0,   656,     0,     0,     0,     0,     0,   150,   146,   104,
       0,   151,     0,   150,     0,     0,     0,     0,     0,   146,
       0,   146,     0,     0,     0,     0,     0,     0,   145,   145,
     145,   145,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   335,    97,     0,     0,   336,     0,     0,
     986,     0,   145,   145,     0,     0,     0,     0,     0,     0,
       0,   151,     0,     0,     0,     0,     0,   151,     0,     0,
       0,     0,     0,   150,     0,   150,     0,     0,     0,     0,
     150,     0,     0,   150,     0,     0,     0,     0,     0,     0,
     150,     0,     0,   150,   338,   339,     0,   340,   341,   342,
       0,   343,   344,     0,     0,   146,   347,     0,     0,     0,
       0,     0,   146,     0,   354,   150,     0,   218,   737,     0,
     358,   359,   360,     0,   146,     0,   150,   151,   150,   151,
     219,   220,     0,   221,   151,     0,   146,   151,   222,     0,
       0,     0,     0,     0,   151,   146,   223,   151,     0,   146,
       0,     0,   224,     0,     0,     0,     0,   225,     0,   226,
       0,     0,   227,     0,     0,     0,     0,     0,   770,   151,
     228,     0,     0,     0,   775,     0,   229,   230,     0,     0,
     151,     0,   151,   231,     0,     0,     0,     0,     0,     0,
       0,   146,   232,     0,     0,     0,     0,     0,     0,     0,
       0,   233,   234,     0,   235,     0,   236,     0,   237,     0,
       0,   238,   150,     0,     0,   239,   437,     0,   240,   150,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   150,     0,     0,   447,     0,   447,   146,   146,   146,
     146,   447,     0,   150,   447,     0,     0,     0,     0,     0,
       0,   873,   150,     0,   875,     0,   150,   334,     0,     0,
       0,   146,   146,     0,     0,     0,   151,     0,     0,     0,
       0,     0,     0,   151,     0,   260,   889,   438,     0,    22,
      23,     0,     0,     0,     0,   151,     0,   895,   261,   899,
      30,   262,     0,     0,     0,    35,     0,   151,   150,     0,
      39,     0,     0,     0,     0,     0,   151,   335,     0,     0,
     151,   336,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,     0,    54,     0,    56,     0,   906,     0,
       0,   907,     0,   263,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,   150,   150,   150,   150,     0,     0,
       0,     0,   151,     0,    78,     0,     0,    80,   338,   339,
      82,   340,   341,   342,     0,   343,   344,   345,   150,   150,
     347,   348,   349,     0,     0,   351,   352,   353,   354,     0,
       0,   357,     0,     0,   358,   359,   360,     0,     0,     0,
       0,     0,   944,     0,     0,     0,     0,     0,   151,   151,
     151,   151,     0,     0,   961,     0,     0,     0,     0,     0,
      97,     0,     0,   966,     0,     0,   988,   968,     0,     0,
       0,     0,   151,   151,    -2,     4,     0,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,   994,
      30,    31,    32,    33,    34,    35,    36,    37,     0,    38,
      39,     0,    40,  -233,     0,    41,    42,    43,     0,    44,
      45,    46,   -43,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,    56,    57,    58,  -233,
     -43,    59,    60,    61,     0,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
       0,    75,    76,    77,    78,    79,     0,    80,    81,   -61,
      82,    83,     0,     0,    84,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,     0,   100,  1057,   101,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     168,     0,     0,    15,    16,     0,    17,     0,   169,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   170,     0,     0,     0,    32,   171,   172,     0,   173,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   174,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   175,
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,   266,     0,     0,     0,    98,     0,   176,     0,
     100,     0,   177,  1058,     4,     0,     5,     6,     7,     8,
       9,    10,    11,     0,  -556,     0,    12,    13,    14,    15,
      16,  -556,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,  -556,    27,    28,  -556,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,     0,    38,    39,
       0,    40,  -233,     0,    41,    42,    43,     0,    44,    45,
      46,   -43,    47,    48,     0,    49,    50,    51,     0,     0,
       0,    52,    53,    54,    55,     0,    57,    58,  -233,   -43,
      59,    60,    61,  -556,    62,    63,    64,  -556,    65,    66,
      67,    68,    69,    70,    71,    72,     0,    73,    74,     0,
      75,    76,    77,    78,    79,     0,    80,    81,   -61,    82,
      83,     0,     0,    84,     0,    85,     0,     0,  -556,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -556,  -556,    89,  -556,  -556,  -556,
    -556,  -556,  -556,  -556,     0,  -556,  -556,  -556,  -556,  -556,
       0,  -556,  -556,  -556,  -556,  -556,  -556,  -556,  -556,    97,
    -556,  -556,  -556,     0,    99,  -556,   100,   302,   101,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
       0,    38,    39,     0,    40,  -233,     0,    41,    42,    43,
       0,    44,    45,    46,   -43,    47,    48,     0,    49,    50,
      51,     0,     0,     0,    52,    53,    54,    55,    56,    57,
      58,  -233,   -43,    59,    60,    61,     0,    62,    63,    64,
       0,    65,    66,    67,    68,    69,    70,    71,    72,     0,
      73,    74,     0,    75,    76,    77,    78,    79,     0,    80,
      81,   -61,    82,    83,     0,     0,    84,     0,    85,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,   266,    97,     0,     0,    98,     0,    99,   303,   100,
       4,   101,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,     0,    38,    39,     0,    40,  -233,     0,
      41,    42,    43,     0,    44,    45,    46,   -43,    47,    48,
       0,    49,    50,    51,     0,     0,     0,    52,    53,    54,
      55,    56,    57,    58,  -233,   -43,    59,    60,    61,     0,
      62,    63,    64,     0,    65,    66,    67,    68,    69,    70,
      71,    72,     0,    73,    74,     0,    75,    76,    77,    78,
      79,     0,    80,    81,   -61,    82,    83,     0,     0,    84,
       0,    85,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,    97,     0,     0,    98,     0,
      99,   472,   100,   491,   101,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,     0,    38,    39,     0,
      40,  -233,     0,    41,    42,    43,     0,    44,    45,    46,
     -43,    47,    48,     0,    49,    50,    51,     0,     0,     0,
      52,    53,    54,    55,    56,    57,    58,  -233,   -43,    59,
      60,    61,     0,    62,    63,    64,     0,    65,    66,    67,
      68,    69,    70,    71,    72,     0,    73,    74,     0,    75,
      76,    77,    78,    79,     0,    80,    81,   -61,    82,    83,
       0,     0,    84,     0,    85,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,    97,     0,
       0,    98,     0,    99,   492,   100,   302,   101,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,     0,
      38,    39,     0,    40,  -233,     0,    41,    42,    43,     0,
      44,    45,    46,   -43,    47,    48,     0,    49,    50,    51,
       0,     0,     0,    52,    53,    54,    55,    56,    57,    58,
    -233,   -43,    59,    60,    61,     0,    62,    63,    64,     0,
      65,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      74,     0,    75,    76,    77,    78,    79,     0,    80,    81,
     -61,    82,    83,     0,     0,    84,     0,    85,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,    97,     0,     0,    98,     0,    99,   303,   100,     4,
     101,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,     0,    38,    39,     0,    40,  -233,     0,    41,
      42,    43,     0,    44,    45,    46,   -43,    47,    48,     0,
      49,    50,    51,     0,     0,     0,    52,    53,    54,    55,
      56,    57,    58,  -233,   -43,    59,    60,    61,     0,    62,
      63,    64,     0,    65,    66,    67,    68,    69,    70,    71,
      72,     0,    73,    74,     0,    75,    76,    77,    78,    79,
       0,    80,    81,   -61,    82,    83,     0,     0,    84,     0,
      85,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,    97,     0,     0,    98,     0,    99,
     663,   100,     4,   101,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,     0,    38,    39,     0,    40,
    -233,     0,    41,    42,    43,     0,    44,    45,    46,   -43,
      47,    48,     0,    49,    50,    51,     0,     0,     0,    52,
      53,    54,    55,   317,    57,    58,  -233,   -43,    59,    60,
      61,     0,    62,    63,    64,     0,    65,    66,    67,    68,
      69,    70,    71,    72,     0,    73,    74,     0,    75,    76,
      77,    78,    79,     0,    80,    81,   -61,    82,    83,     0,
       0,    84,     0,    85,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,    97,     0,     0,
      98,     0,    99,     0,   100,     4,   101,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,     0,    38,
      39,     0,    40,  -233,     0,    41,    42,    43,     0,    44,
      45,    46,   -43,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,     0,    57,    58,  -233,
     -43,    59,    60,    61,     0,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
       0,    75,    76,    77,    78,    79,     0,    80,    81,   -61,
      82,    83,     0,     0,    84,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,     0,   100,     4,   101,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,   583,
      37,     0,    38,    39,     0,    40,  -233,     0,    41,    42,
      43,     0,    44,    45,    46,   -43,    47,    48,     0,    49,
      50,    51,     0,     0,     0,    52,    53,    54,    55,     0,
      57,    58,  -233,   -43,    59,    60,    61,     0,    62,    63,
      64,     0,    65,    66,    67,    68,    69,    70,    71,    72,
       0,    73,    74,     0,    75,    76,    77,    78,    79,     0,
      80,    81,   -61,    82,    83,     0,     0,    84,     0,    85,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,    97,     0,     0,    98,     0,    99,     0,
     100,     4,   101,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,   589,    37,     0,    38,    39,     0,    40,  -233,
       0,    41,    42,    43,     0,    44,    45,    46,   -43,    47,
      48,     0,    49,    50,    51,     0,     0,     0,    52,    53,
      54,    55,     0,    57,    58,  -233,   -43,    59,    60,    61,
       0,    62,    63,    64,     0,    65,    66,    67,    68,    69,
      70,    71,    72,     0,    73,    74,     0,    75,    76,    77,
      78,    79,     0,    80,    81,   -61,    82,    83,     0,     0,
      84,     0,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,    97,     0,     0,    98,
       0,    99,     0,   100,     4,   101,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,   858,    37,     0,    38,    39,
       0,    40,  -233,     0,    41,    42,    43,     0,    44,    45,
      46,   -43,    47,    48,     0,    49,    50,    51,     0,     0,
       0,    52,    53,    54,    55,     0,    57,    58,  -233,   -43,
      59,    60,    61,     0,    62,    63,    64,     0,    65,    66,
      67,    68,    69,    70,    71,    72,     0,    73,    74,     0,
      75,    76,    77,    78,    79,     0,    80,    81,   -61,    82,
      83,     0,     0,    84,     0,    85,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,    97,
       0,     0,    98,     0,    99,     0,   100,     4,   101,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,   860,    37,
       0,    38,    39,     0,    40,  -233,     0,    41,    42,    43,
       0,    44,    45,    46,   -43,    47,    48,     0,    49,    50,
      51,     0,     0,     0,    52,    53,    54,    55,     0,    57,
      58,  -233,   -43,    59,    60,    61,     0,    62,    63,    64,
       0,    65,    66,    67,    68,    69,    70,    71,    72,     0,
      73,    74,     0,    75,    76,    77,    78,    79,     0,    80,
      81,   -61,    82,    83,     0,     0,    84,     0,    85,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,    97,     0,     0,    98,     0,    99,     0,   100,
       4,   101,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,   865,    37,     0,    38,    39,     0,    40,  -233,     0,
      41,    42,    43,     0,    44,    45,    46,   -43,    47,    48,
       0,    49,    50,    51,     0,     0,     0,    52,    53,    54,
      55,     0,    57,    58,  -233,   -43,    59,    60,    61,     0,
      62,    63,    64,     0,    65,    66,    67,    68,    69,    70,
      71,    72,     0,    73,    74,     0,    75,    76,    77,    78,
      79,     0,    80,    81,   -61,    82,    83,     0,     0,    84,
       0,    85,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,    97,     0,     0,    98,     0,
      99,     0,   100,     4,   101,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,   868,    37,     0,    38,    39,     0,
      40,  -233,     0,    41,    42,    43,     0,    44,    45,    46,
     -43,    47,    48,     0,    49,    50,    51,     0,     0,     0,
      52,    53,    54,    55,     0,    57,    58,  -233,   -43,    59,
      60,    61,     0,    62,    63,    64,     0,    65,    66,    67,
      68,    69,    70,    71,    72,     0,    73,    74,     0,    75,
      76,    77,    78,    79,     0,    80,    81,   -61,    82,    83,
       0,     0,    84,     0,    85,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,    97,     0,
       0,    98,     0,    99,     0,   100,     4,   101,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,   888,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,     0,
      38,    39,     0,    40,  -233,     0,    41,    42,    43,     0,
      44,    45,    46,   -43,    47,    48,     0,    49,    50,    51,
       0,     0,     0,    52,    53,    54,    55,     0,    57,    58,
    -233,   -43,    59,    60,    61,     0,    62,    63,    64,     0,
      65,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      74,     0,    75,    76,    77,    78,    79,     0,    80,    81,
     -61,    82,    83,     0,     0,    84,     0,    85,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,    97,     0,     0,    98,     0,    99,     0,   100,     4,
     101,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
     894,    37,     0,    38,    39,     0,    40,  -233,     0,    41,
      42,    43,     0,    44,    45,    46,   -43,    47,    48,     0,
      49,    50,    51,     0,     0,     0,    52,    53,    54,    55,
       0,    57,    58,  -233,   -43,    59,    60,    61,     0,    62,
      63,    64,     0,    65,    66,    67,    68,    69,    70,    71,
      72,     0,    73,    74,     0,    75,    76,    77,    78,    79,
       0,    80,    81,   -61,    82,    83,     0,     0,    84,     0,
      85,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,    97,     0,     0,    98,     0,    99,
       0,   100,     4,   101,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,   898,    37,     0,    38,    39,     0,    40,
    -233,     0,    41,    42,    43,     0,    44,    45,    46,   -43,
      47,    48,     0,    49,    50,    51,     0,     0,     0,    52,
      53,    54,    55,     0,    57,    58,  -233,   -43,    59,    60,
      61,     0,    62,    63,    64,     0,    65,    66,    67,    68,
      69,    70,    71,    72,     0,    73,    74,     0,    75,    76,
      77,    78,    79,     0,    80,    81,   -61,    82,    83,     0,
       0,    84,     0,    85,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,    97,     0,     0,
      98,     0,    99,     0,   100,   755,   101,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   168,     0,     0,
      15,    16,     0,    17,     0,   169,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   170,     0,
       0,     0,    32,   171,   172,     0,   173,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   174,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   175,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,   266,
       0,     0,     0,    98,     0,   176,     0,   100,     0,   177,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     168,     0,     0,    15,    16,     0,    17,     0,   169,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   170,     0,     0,     0,    32,   171,   172,     0,   173,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   174,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   175,
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,   266,     0,     0,     0,    98,     0,   176,     0,
     100,     0,   177,   655,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   168,     0,     0,    15,    16,     0,
      17,     0,   169,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   170,     0,     0,     0,    32,
     171,   172,     0,   173,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     174,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   175,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,   176,     0,   100,     0,   177,   660,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   168,     0,
       0,    15,    16,     0,    17,     0,   169,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   170,
       0,     0,     0,    32,   171,   172,     0,   173,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   174,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   175,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
     266,     0,     0,     0,    98,     0,   176,     0,   100,     0,
     177,   997,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   168,     0,     0,    15,    16,     0,    17,     0,
     169,     0,     0,    21,   213,    23,     0,     0,     0,     0,
       0,    28,     0,   170,     0,     0,     0,    32,   171,   172,
       0,   173,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,    54,
      55,     0,    57,     0,     0,     0,     0,    60,   174,     0,
      62,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   175,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,    82,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     176,     0,   100,     0,   177,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   168,     0,     0,    15,    16,
       0,    17,     0,   169,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   170,     0,     0,     0,
      32,   171,   172,     0,   173,    37,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   174,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   175,     0,    73,     0,     0,    75,
     270,   271,     0,    79,   307,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,   308,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,   266,     0,     0,
       0,    98,     0,   176,     0,   100,     0,   177,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   168,     0,
       0,    15,    16,     0,    17,     0,   169,     0,     0,    21,
       0,   548,     0,     0,     0,     0,     0,    28,     0,   170,
       0,     0,     0,    32,   171,   172,     0,   173,    37,   549,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   174,     0,   550,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   175,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,   551,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   176,     0,   100,     0,
     177,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   168,     0,     0,    15,    16,     0,    17,     0,   169,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   170,     0,     0,     0,    32,   171,   172,     0,
     173,    37,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   174,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     175,     0,    73,     0,     0,    75,   270,   271,     0,    79,
     307,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,   266,     0,     0,     0,    98,     0,   176,
       0,   100,   647,   177,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   168,     0,     0,    15,    16,     0,
      17,     0,   169,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   170,     0,     0,     0,    32,
     171,   172,     0,   173,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     174,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   175,     0,    73,     0,     0,    75,   270,
     271,     0,    79,   307,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,   266,     0,     0,     0,
      98,     0,   176,     0,   100,   649,   177,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   168,     0,     0,
      15,    16,     0,    17,     0,   169,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   170,     0,
       0,     0,    32,   171,   172,     0,   173,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   174,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   175,     0,    73,     0,
       0,    75,   270,   271,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
     272,     0,     0,    98,     0,   176,     0,   100,     0,   177,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     168,     0,     0,    15,    16,     0,    17,     0,   169,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   170,     0,     0,     0,    32,   171,   172,     0,   173,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,   757,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   174,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   175,
       0,    73,     0,     0,    75,   270,   271,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   176,     0,
     100,     0,   177,     5,     6,     7,     8,   202,    10,    11,
     203,     0,     0,   168,     0,     0,    15,    16,     0,    17,
       0,   169,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   170,     0,     0,     0,    32,   171,
     172,     0,   173,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   204,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   175,     0,    73,     0,     0,    75,     0,     0,
     205,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   176,     0,   100,     0,   177,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   168,     0,     0,    15,
      16,     0,    17,     0,   169,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   170,     0,     0,
       0,    32,   171,   172,     0,   173,    37,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,   207,    55,     0,    57,     0,     0,     0,
       0,    60,   174,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   175,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,   208,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   176,     0,   100,     0,   177,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   168,
       0,     0,    15,    16,     0,    17,     0,   169,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     170,     0,     0,     0,    32,   171,   172,     0,   173,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   174,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   175,     0,
      73,     0,     0,    75,   270,   271,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,   266,     0,     0,     0,    98,     0,   176,     0,   100,
       0,   177,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   168,     0,     0,    15,    16,     0,    17,     0,
     169,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   170,     0,     0,     0,    32,   171,   172,
       0,   173,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   174,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   175,     0,    73,     0,     0,    75,   270,   271,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,   787,     0,   788,    98,     0,
     176,     0,   100,     0,   177,   789,     0,     0,     0,     0,
     790,   220,   791,   792,     0,     0,     0,     0,   793,     0,
     260,     0,     0,     0,    22,    23,   223,     0,     0,     0,
       0,     0,   794,   261,     0,    30,   262,   795,     0,   226,
      35,     0,   796,     0,   797,    39,     0,     0,  -233,     0,
     798,     0,     0,     0,     0,     0,   799,   800,     0,     0,
       0,     0,     0,   231,     0,     0,     0,    52,     0,    54,
       0,     0,   801,     0,  -233,     0,     0,     0,   263,     0,
      62,   233,   234,     0,   802,     0,   236,     0,   803,     0,
       0,   804,     0,     0,     0,   805,     0,     0,   240,    78,
       0,   806,    80,     0,   -63,    82,     0,     0,     0,     0,
     361,   362,   363,   364,   365,   366,     0,     0,   369,   370,
     371,   372,     0,   374,   375,   807,   808,   809,   810,   811,
       0,     0,   812,     0,     0,     0,   813,   814,   815,   816,
     817,   818,   819,   820,   821,   822,   823,     0,   824,     0,
       0,   825,   826,   827,   828,     0,     0,   829,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   168,     0,
       0,    15,    16,     0,    17,     0,   169,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   170,
       0,     0,     0,    32,   171,   172,     0,   173,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   174,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   175,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,   208,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   176,     0,   100,     0,
     177,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   168,     0,     0,    15,    16,     0,    17,     0,   169,
       0,     0,    21,     0,     0,     0,     0,     0,     0,   249,
      28,     0,   170,     0,     0,     0,    32,   171,   172,     0,
     173,    37,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   174,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     175,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,    99,
       0,   100,     0,   177,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   168,     0,     0,    15,    16,     0,
      17,     0,   169,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   170,     0,     0,     0,    32,
     171,   172,     0,   173,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     174,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   175,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,   208,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,   266,     0,     0,     0,
      98,     0,   176,     0,   100,     0,   177,     5,     6,     7,
       8,   202,    10,    11,     0,     0,     0,   168,     0,     0,
      15,    16,     0,    17,     0,   169,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   170,     0,
       0,     0,    32,   171,   172,     0,   173,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   204,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   175,     0,    73,     0,
       0,    75,     0,     0,   205,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,   176,     0,   100,     0,   177,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     168,     0,     0,    15,    16,     0,    17,     0,   169,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   170,     0,     0,     0,    32,   171,   172,     0,   173,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   174,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   175,
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,   266,     0,     0,     0,    98,     0,   176,   645,
     100,     0,   177,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   168,     0,     0,    15,    16,     0,    17,
       0,   169,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   170,     0,     0,     0,    32,   171,
     172,     0,   173,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   174,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   175,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   176,     0,   100,     0,   177,     5,     6,     7,     8,
     198,    10,    11,     0,     0,     0,   168,     0,     0,    15,
      16,     0,    17,     0,   169,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   170,     0,     0,
       0,    32,   171,   172,     0,   173,    37,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   199,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   175,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   176,     0,   100,     0,   177,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   168,
       0,     0,    15,    16,     0,    17,     0,   169,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     170,     0,     0,     0,    32,   171,   172,     0,   173,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   174,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   175,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,   266,     0,     0,     0,    98,     0,   176,     0,   100,
       0,   177,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   168,     0,     0,    15,    16,     0,    17,     0,
     169,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   170,     0,     0,     0,    32,   171,   172,
       0,   173,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   174,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   175,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
      99,     0,   100,     0,   177,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   168,     0,     0,    15,    16,
       0,    17,     0,   169,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   170,     0,     0,     0,
      32,   171,   172,     0,   173,    37,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   174,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   175,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,   594,     0,   176,     0,   100,     0,   177,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   168,     0,
       0,    15,    16,     0,    17,     0,   169,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   621,     0,   170,
       0,     0,     0,    32,   171,   172,     0,   173,    37,     0,
     622,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   174,     0,     0,     0,     0,     0,
       0,     0,    67,   623,    69,    70,    71,   624,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   176,     0,   100,     0,
     625,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   168,     0,     0,    15,    16,     0,    17,     0,   169,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   170,     0,     0,     0,    32,   171,   172,     0,
     173,    37,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   174,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     175,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   176,
       0,   100,     0,   625,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   168,     0,     0,    15,    16,     0,
      17,     0,   169,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   170,     0,     0,     0,    32,
     171,   172,     0,   695,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     174,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   175,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,   176,     0,   100,     0,   177,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   168,     0,     0,
      15,    16,     0,    17,     0,   169,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   170,     0,
       0,     0,    32,   171,   172,     0,   698,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   174,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   175,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,   176,     0,   100,     0,   177,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     168,     0,     0,    15,    16,     0,    17,     0,   169,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   170,     0,     0,     0,    32,   171,   172,     0,   924,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   174,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   175,
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   176,     0,
     100,     0,   177,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   168,     0,     0,    15,    16,     0,    17,
       0,   169,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   170,     0,     0,     0,    32,   171,
     172,     0,   925,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   174,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   175,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   176,     0,   100,     0,   177,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   168,     0,     0,    15,
      16,     0,    17,     0,   169,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   170,     0,     0,
       0,    32,   171,   172,     0,   927,    37,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   174,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   175,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   176,     0,   100,     0,   177,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   168,
       0,     0,    15,    16,     0,    17,     0,   169,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     170,     0,     0,     0,    32,   171,   172,     0,   928,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   174,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   175,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,   176,     0,   100,
       0,   177,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   168,     0,     0,    15,    16,     0,    17,     0,
     169,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   170,     0,     0,     0,    32,   171,   172,
       0,   929,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   174,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   175,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     176,     0,   100,     0,   177,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   168,     0,     0,    15,    16,
       0,    17,     0,   169,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   170,     0,     0,     0,
      32,   171,   172,     0,   930,    37,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   174,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   175,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   176,     0,   100,     0,   177,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   168,     0,
       0,    15,    16,     0,    17,     0,   169,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   621,     0,   170,
       0,     0,     0,    32,   171,   172,     0,   173,    37,     0,
     622,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   174,     0,     0,     0,     0,     0,
       0,     0,    67,   623,    69,    70,    71,   624,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
     266,     0,     0,     0,    98,     0,   176,     0,   100,     0,
     947,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   168,     0,     0,    15,    16,     0,    17,     0,   169,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
     621,     0,   170,     0,     0,     0,    32,   171,   172,     0,
     173,    37,     0,   622,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   174,     0,     0,
       0,     0,     0,     0,     0,    67,   623,    69,    70,    71,
     624,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   176,
       0,   100,     0,  1015,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   168,     0,     0,    15,    16,     0,
      17,     0,   169,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   170,     0,     0,     0,    32,
     171,   172,     0,   173,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     174,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   175,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,   266,     0,     0,     0,
      98,     0,   176,     0,   100,     0,   947,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   168,     0,     0,
      15,    16,     0,    17,     0,   169,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   170,     0,
       0,     0,    32,   171,   172,     0,   930,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   174,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   175,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,   266,
       0,     0,     0,    98,     0,   176,     0,   100,     0,   177,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     168,     0,     0,    15,    16,     0,    17,     0,   169,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   170,     0,     0,     0,    32,   171,   172,     0,   173,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   174,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   175,
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,     0,     0,
     100,     0,  1015,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   168,     0,     0,    15,    16,     0,    17,
       0,   169,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   170,     0,     0,     0,    32,   171,
     172,     0,   173,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,   260,    49,    50,     0,    22,    23,     0,     0,    53,
       0,    55,     0,    57,   261,     0,    30,   262,    60,   174,
       0,    35,     0,     0,     0,     0,    39,    67,    68,    69,
      70,    71,   175,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,    52,     0,
      54,     0,     0,     0,     0,     0,     0,     0,     0,   263,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      78,     0,     0,    80,     0,     0,    82,     0,     0,     0,
      93,     0,     0,     0,    95,    96,     0,     0,     0,    98,
       0,   176,     0,   100,     0,   177,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   168,     0,     0,    15,
      16,     0,    17,     0,   169,   334,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   170,     0,     0,
       0,    32,   171,   172,     0,   173,    37,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,  -379,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,   335,    57,     0,     0,   336,
       0,    60,   174,     0,     0,     0,  -379,     0,     0,     0,
      67,    68,    69,    70,    71,   175,     0,    73,     0,  -379,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,  -379,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   338,   339,     0,   340,
     341,   342,     0,   343,   344,   345,    89,   346,   347,   348,
     349,     0,     0,   351,   352,   353,   354,   355,     0,   357,
       0,     0,   358,   359,   360,     0,     0,     0,    96,  -379,
       0,     0,     0,     0,  -379,     0,   100,     0,  1015,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   168,
       0,     0,    15,    16,     0,    17,     0,   169,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     170,     0,     0,     0,    32,   171,   172,     0,   173,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   174,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   175,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,     0,  -397,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,  -397,     0,     0,     0,  -397,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   266,     0,     0,     0,     0,     0,   176,     0,   100,
    -397,   947,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   168,     0,     0,    15,    16,     0,    17,     0,
     169,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   170,     0,     0,     0,    32,   171,   172,
       0,   173,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   174,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   175,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,     0,     0,     0,
      87,     0,    89,     0,     5,     6,     7,     8,     9,    10,
      11,     0,     0,  -501,   168,     0,     0,    15,    16,     0,
      17,     0,   169,     0,    96,    21,   376,     0,     0,  -528,
     176,  -528,   100,    28,   177,   170,     0,     0,     0,    32,
     171,   172,     0,   173,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     174,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   175,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   168,     0,     0,    15,
      16,     0,    17,     0,   169,     0,    96,    21,     0,     0,
       0,     0,   176,     0,   100,    28,   625,   170,     0,     0,
       0,    32,   171,   172,     0,   173,    37,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   174,     0,     0,   218,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   175,     0,    73,   219,   220,
      75,   221,     0,     0,    79,     0,   222,    81,     0,     0,
      83,     0,     0,     0,   223,     0,     0,     0,     0,     0,
     224,     0,     0,     0,     0,   225,     0,   226,     0,     0,
     227,     0,     0,     0,     0,     0,    89,     0,   228,     0,
       0,     0,   218,     0,   229,   230,     0,     0,     0,     0,
       0,   231,     0,     0,     0,   219,   220,     0,   221,     0,
     232,     0,     0,   222,     0,     0,   100,     0,   177,   233,
     234,   223,   235,     0,   236,     0,   237,   224,     0,   238,
       0,     0,   225,   239,   226,     0,   240,   227,     0,   241,
       0,     0,     0,     0,     0,   228,     0,     0,     0,     0,
       0,   229,   230,     0,     0,     0,     0,     0,   231,     0,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,     0,   233,   234,     0,   235,
       0,   236,     0,   237,   975,     0,   238,     0,     0,     0,
     239,     0,   789,   240,     0,   245,   241,   219,   220,   791,
     221,     0,     0,     0,     0,   222,     0,     0,     0,     0,
       0,     0,     0,   223,     0,     0,     0,     0,     0,   794,
       0,     0,     0,     0,   225,     0,   226,     0,     0,   227,
       0,   797,     0,     0,     0,     0,     0,   228,     0,     0,
       0,     0,     0,   799,   230,     0,     0,     0,   332,     0,
     231,     0,   745,     0,     0,   333,     0,     0,     0,   232,
       0,     0,     0,     0,     0,     0,     0,   334,   233,   234,
       0,   235,     0,   236,     0,   976,     0,     0,   804,     0,
       0,     0,   239,     0,     0,   240,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,     0,   361,   362,   363,
     364,   365,   366,     0,     0,   369,   370,   371,   372,     0,
     374,   375,   807,   808,   809,   810,   811,   335,     0,   812,
       0,   336,     0,   813,   814,   815,   816,   817,   818,   819,
     820,   821,   822,   823,     0,   824,   332,     0,   825,   826,
     827,   828,     0,   333,     0,     0,     0,     0,     0,     0,
       0,     0,   337,     0,     0,   334,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   338,   339,
       0,   340,   341,   342,     0,   343,   344,   345,     0,   346,
     347,   348,   349,   350,     0,   351,   352,   353,   354,   355,
     356,   357,     0,   332,   358,   359,   360,     0,     0,     0,
     333,     0,     0,   849,     0,   335,     0,     0,     0,   336,
       0,     0,   334,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   332,     0,     0,     0,     0,     0,
     337,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   334,     0,     0,   338,   339,     0,   340,
     341,   342,   335,   343,   344,   345,   336,   346,   347,   348,
     349,   350,     0,   351,   352,   353,   354,   355,   356,   357,
     332,     0,   358,   359,   360,     0,     0,   333,     0,     0,
       0,  1011,     0,     0,     0,     0,     0,   337,     0,   334,
       0,     0,     0,   335,     0,     0,     0,   336,     0,     0,
       0,     0,     0,   338,   339,     0,   340,   341,   342,     0,
     343,   344,   345,     0,   346,   347,   348,   349,   350,     0,
     351,   352,   353,   354,   355,   356,   357,     0,   337,   358,
     359,   360,     0,     0,     0,     0,     0,     0,  1080,   335,
       0,     0,     0,   336,   338,   339,     0,   340,   341,   342,
       0,   343,   344,   345,     0,   346,   347,   348,   349,   350,
       0,   351,   352,   353,   354,   355,   356,   357,   332,     0,
     358,   359,   360,     0,   337,   333,     0,   646,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   334,     0,     0,
     338,   339,     0,   340,   341,   342,     0,   343,   344,   345,
       0,   346,   347,   348,   349,   350,     0,   351,   352,   353,
     354,   355,   356,   357,     0,     0,   358,   359,   360,     0,
       0,     0,     0,   974,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   335,     0,     0,
       0,   336,     0,     0,   332,     0,     0,     0,     0,     0,
     430,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   334,     0,     0,     0,     0,     0,     0,
       0,     0,   337,   361,   362,   363,   364,   365,   366,     0,
       0,   369,   370,   371,   372,     0,   374,   375,   338,   339,
       0,   340,   341,   342,     0,   343,   344,   345,     0,   346,
     347,   348,   349,   350,     0,   351,   352,   353,   354,   355,
     356,   357,     0,   335,   358,   359,   360,   336,   389,     0,
     332,     0,     0,     0,     0,     0,   857,   333,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   334,
       0,     0,     0,     0,     0,     0,     0,     0,   337,   361,
     362,   363,   364,   365,   366,     0,     0,   369,   370,   371,
     372,     0,   374,   375,   338,   339,     0,   340,   341,   342,
       0,   343,   344,   345,     0,   346,   347,   348,   349,   350,
       0,   351,   352,   353,   354,   355,   356,   357,     0,   335,
     358,   359,   360,   336,   389,     0,   332,     0,     0,     0,
       0,     0,   864,   333,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   334,     0,     0,     0,     0,
       0,     0,     0,     0,   337,   361,   362,   363,   364,   365,
     366,     0,     0,   369,   370,   371,   372,     0,   374,   375,
     338,   339,     0,   340,   341,   342,     0,   343,   344,   345,
       0,   346,   347,   348,   349,   350,     0,   351,   352,   353,
     354,   355,   356,   357,     0,   335,   358,   359,   360,   336,
     389,     0,   332,     0,     0,     0,     0,     0,   989,   333,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   334,     0,     0,     0,     0,     0,     0,     0,     0,
     337,   361,   362,   363,   364,   365,   366,     0,     0,   369,
     370,   371,   372,     0,   374,   375,   338,   339,     0,   340,
     341,   342,     0,   343,   344,   345,     0,   346,   347,   348,
     349,   350,     0,   351,   352,   353,   354,   355,   356,   357,
       0,   335,   358,   359,   360,   336,   389,     0,   332,     0,
       0,     0,     0,     0,   990,   333,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   334,     0,     0,
       0,     0,     0,     0,     0,     0,   337,   361,   362,   363,
     364,   365,   366,     0,     0,   369,   370,   371,   372,     0,
     374,   375,   338,   339,     0,   340,   341,   342,     0,   343,
     344,   345,     0,   346,   347,   348,   349,   350,     0,   351,
     352,   353,   354,   355,   356,   357,     0,   335,   358,   359,
     360,   336,   389,     0,   332,     0,     0,     0,     0,     0,
     991,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   334,     0,     0,     0,     0,     0,     0,
       0,     0,   337,   361,   362,   363,   364,   365,   366,     0,
       0,   369,   370,   371,   372,     0,   374,   375,   338,   339,
       0,   340,   341,   342,     0,   343,   344,   345,     0,   346,
     347,   348,   349,   350,     0,   351,   352,   353,   354,   355,
     356,   357,     0,   335,   358,   359,   360,   336,   389,     0,
     332,     0,     0,     0,     0,     0,   992,   333,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   334,
       0,     0,     0,     0,     0,     0,     0,     0,   337,   361,
     362,   363,   364,   365,   366,     0,     0,   369,   370,   371,
     372,     0,   374,   375,   338,   339,     0,   340,   341,   342,
       0,   343,   344,   345,     0,   346,   347,   348,   349,   350,
       0,   351,   352,   353,   354,   355,   356,   357,     0,   335,
     358,   359,   360,   336,   389,     0,   332,     0,     0,     0,
       0,     0,  1013,   333,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   334,     0,     0,     0,     0,
       0,     0,     0,     0,   337,   361,   362,   363,   364,   365,
     366,     0,     0,   369,   370,   371,   372,     0,   374,   375,
     338,   339,     0,   340,   341,   342,     0,   343,   344,   345,
       0,   346,   347,   348,   349,   350,     0,   351,   352,   353,
     354,   355,   356,   357,     0,   335,   358,   359,   360,   336,
     389,     0,     0,     0,     0,     0,     0,     0,  1014,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     337,   361,   362,   363,   364,   365,   366,     0,     0,   369,
     370,   371,   372,     0,   374,   375,   338,   339,     0,   340,
     341,   342,     0,   343,   344,   345,     0,   346,   347,   348,
     349,   350,     0,   351,   352,   353,   354,   355,   356,   357,
     332,     0,   358,   359,   360,     0,   389,   333,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   334,
     423,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   332,   424,     0,     0,     0,     0,     0,   333,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   334,   417,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   418,     0,     0,     0,   335,
       0,     0,     0,   336,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   332,
       0,     0,     0,     0,     0,     0,   333,     0,     0,     0,
     420,   335,     0,     0,   337,   336,     0,     0,   334,   728,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     338,   339,     0,   340,   341,   342,     0,   343,   344,   345,
       0,   346,   347,   348,   349,   350,   337,   351,   352,   353,
     354,   355,   356,   357,     0,     0,   358,   359,   360,     0,
     389,     0,   338,   339,     0,   340,   341,   342,   335,   343,
     344,   345,   336,   346,   347,   348,   349,   350,     0,   351,
     352,   353,   354,   355,   356,   357,   332,     0,   358,   359,
     360,     0,   389,   333,     0,     0,     0,     0,     0,   420,
       0,     0,     0,   337,     0,   334,   249,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   338,
     339,     0,   340,   341,   342,     0,   343,   344,   345,     0,
     346,   347,   348,   349,   350,     0,   351,   352,   353,   354,
     355,   356,   357,   332,     0,   358,   359,   360,     0,   389,
     333,     0,     0,     0,     0,   335,     0,     0,     0,   336,
       0,     0,   334,   720,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   332,     0,     0,     0,     0,
       0,     0,   333,     0,     0,     0,     0,     0,     0,     0,
     337,     0,     0,     0,   334,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   338,   339,     0,   340,
     341,   342,   335,   343,   344,   345,   336,   346,   347,   348,
     349,   350,     0,   351,   352,   353,   354,   355,   356,   357,
       0,     0,   358,   359,   360,     0,   389,     0,     0,     0,
       0,     0,     0,     0,   335,     0,     0,   337,   336,     0,
       0,     0,     0,     0,     0,     0,     0,   738,     0,     0,
       0,     0,     0,   338,   339,     0,   340,   341,   342,     0,
     343,   344,   345,     0,   346,   347,   348,   349,   350,   337,
     351,   352,   353,   354,   355,   356,   357,     0,     0,   358,
     359,   360,     0,   389,     0,   338,   339,     0,   340,   341,
     342,     0,   343,   344,   345,     0,   346,   347,   348,   349,
     350,     0,   351,   352,   353,   354,   355,   356,   357,   332,
       0,   358,   359,   360,     0,   389,   333,  -214,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   334,     0,
    -214,  -214,     0,  -214,     0,     0,     0,     0,  -214,     0,
       0,   332,     0,     0,     0,     0,  -214,     0,   333,     0,
       0,     0,  -214,     0,     0,     0,     0,  -214,     0,  -214,
     334,     0,  -214,     0,     0,   427,     0,     0,     0,     0,
    -214,     0,     0,     0,     0,     0,  -214,  -214,   335,     0,
       0,     0,   336,  -214,     0,     0,     0,     0,     0,     0,
       0,     0,  -214,     0,     0,   428,     0,     0,   332,     0,
       0,  -214,  -214,     0,  -214,   333,  -214,     0,  -214,     0,
     335,  -214,     0,   337,   336,  -214,     0,   334,  -214,     0,
       0,  -214,     0,     0,     0,     0,     0,     0,     0,   338,
     339,   391,   340,   341,   342,     0,   343,   344,   345,     0,
     346,   347,   348,   349,   350,   337,   351,   352,   353,   354,
     355,   356,   357,     0,     0,   358,   359,   360,     0,   457,
       0,   338,   339,     0,   340,   341,   342,   335,   343,   344,
     345,   336,   346,   347,   348,   349,   350,     0,   351,   352,
     353,   354,   355,   356,   357,   332,   429,   358,   359,   360,
       0,     0,   333,     0,     0,     0,     0,     0,   163,     0,
       0,     0,   337,     0,   334,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   338,   339,
       0,   340,   341,   342,     0,   343,   344,   345,     0,   346,
     347,   348,   349,   350,     0,   351,   352,   353,   354,   355,
     356,   357,   332,     0,   358,   359,   360,     0,     0,   333,
       0,     0,     0,     0,   335,     0,     0,     0,   336,     0,
       0,   334,   557,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   332,   558,     0,     0,     0,     0,
       0,   333,     0,     0,     0,     0,     0,     0,     0,   337,
       0,     0,     0,   334,   559,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   338,   339,   560,   340,   341,
     342,   335,   343,   344,   345,   336,   346,   347,   348,   349,
     350,     0,   351,   352,   353,   354,   355,   356,   357,     0,
     461,   358,   359,   360,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   335,     0,     0,   337,   336,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   338,   339,     0,   340,   341,   342,     0,   343,
     344,   345,     0,   346,   347,   348,   349,   350,   337,   351,
     352,   353,   354,   355,   356,   357,     0,     0,   358,   359,
     360,     0,     0,     0,   338,   339,     0,   340,   341,   342,
       0,   343,   344,   345,     0,   346,   347,   348,   349,   350,
       0,   351,   352,   353,   354,   355,   356,   357,   332,   260,
     358,   359,   360,    22,    23,   333,     0,     0,     0,     0,
       0,     0,   261,     0,    30,   262,     0,   334,     0,    35,
       0,     0,   666,     0,    39,     0,     0,  -233,     0,     0,
     332,     0,     0,     0,     0,     0,     0,   333,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,    54,   334,
       0,     0,   667,  -233,     0,     0,     0,   263,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   335,     0,     0,
       0,   336,     0,     0,     0,     0,     0,     0,    78,   332,
       0,    80,     0,   -62,    82,     0,   333,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   334,   335,
       0,     0,   337,   336,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   338,   339,
       0,   340,   341,   342,     0,   343,   344,   345,     0,   346,
     347,   348,   349,   350,   337,   351,   352,   353,   354,   355,
     356,   357,     0,     0,   358,   359,   360,     0,   335,     0,
     338,   339,   336,   340,   341,   342,     0,   343,   344,   345,
       0,   346,   347,   348,   349,   350,     0,   351,   352,   353,
     354,   355,   356,   357,   332,   463,   358,   359,   360,     0,
       0,   333,     0,   337,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   334,     0,     0,     0,     0,     0,   338,
     339,     0,   340,   341,   342,     0,   343,   344,   345,     0,
     346,   347,   348,   349,   350,     0,   351,   352,   353,   354,
     355,   356,   357,   332,   465,   358,   359,   360,     0,     0,
     333,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   334,   335,     0,     0,     0,   336,     0,     0,
       0,     0,     0,     0,     0,   332,     0,     0,     0,     0,
       0,     0,   333,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   334,     0,     0,     0,   337,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   335,     0,   338,   339,   336,   340,   341,   342,
       0,   343,   344,   345,     0,   346,   347,   348,   349,   350,
       0,   351,   352,   353,   354,   355,   356,   357,     0,   470,
     358,   359,   360,     0,   335,     0,   487,   337,   336,     0,
       0,     0,     0,     0,     0,     0,     0,   561,     0,     0,
       0,     0,     0,   338,   339,     0,   340,   341,   342,     0,
     343,   344,   345,     0,   346,   347,   348,   349,   350,   337,
     351,   352,   353,   354,   355,   356,   357,     0,     0,   358,
     359,   360,     0,     0,     0,   338,   339,     0,   340,   341,
     342,     0,   343,   344,   345,     0,   346,   347,   348,   349,
     350,     0,   351,   352,   353,   354,   355,   356,   357,   332,
       0,   358,   359,   360,     0,     0,   333,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   334,     0,
     260,     0,     0,     0,    22,    23,     0,     0,     0,     0,
       0,     0,     0,   261,     0,    30,   262,     0,     0,     0,
      35,     0,     0,     0,   332,    39,     0,     0,     0,     0,
       0,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   334,     0,     0,     0,    52,   335,    54,
       0,    56,   336,   906,     0,     0,   907,     0,   263,     0,
      62,     0,     0,     0,     0,   332,     0,     0,     0,     0,
       0,     0,   333,     0,     0,     0,     0,     0,     0,    78,
       0,     0,    80,   337,   334,    82,     0,     0,     0,     0,
       0,     0,     0,   335,     0,     0,     0,   336,     0,   338,
     339,     0,   340,   341,   342,     0,   343,   344,   345,     0,
     346,   347,   348,   349,   350,  -502,   351,   352,   353,   354,
     355,   356,   357,     0,   163,   358,   359,   360,   337,     0,
       0,     0,     0,     0,   335,    97,     0,     0,   336,     0,
       0,  1030,     0,     0,   338,   339,     0,   340,   341,   342,
       0,   343,   344,   345,     0,   346,   347,   348,   349,   350,
       0,   351,   352,   353,   354,   355,   356,   357,     0,   337,
     358,   359,   360,     0,     0,     0,   332,   733,     0,     0,
       0,     0,     0,   333,     0,   338,   339,     0,   340,   341,
     342,     0,   343,   344,   345,   334,   346,   347,   348,   349,
     350,     0,   351,   352,   353,   354,   355,   356,   357,     0,
     705,   358,   359,   360,     0,     0,   332,     0,     0,     0,
       0,     0,     0,   333,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   334,     0,     0,   736,     0,
       0,     0,     0,     0,     0,   335,     0,     0,     0,   336,
       0,     0,     0,     0,     0,     0,     0,   332,     0,     0,
       0,     0,     0,     0,   333,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   334,     0,     0,     0,
     337,     0,     0,     0,     0,   335,     0,     0,     0,   336,
       0,     0,     0,     0,     0,     0,   338,   339,     0,   340,
     341,   342,     0,   343,   344,   345,     0,   346,   347,   348,
     349,   350,     0,   351,   352,   353,   354,   355,   356,   357,
     337,     0,   358,   359,   360,     0,   335,     0,     0,     0,
     336,     0,     0,     0,     0,     0,   338,   339,     0,   340,
     341,   342,     0,   343,   344,   345,     0,   346,   347,   348,
     349,   350,     0,   351,   352,   353,   354,   355,   356,   357,
     776,   337,   358,   359,   360,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   338,   339,     0,
     340,   341,   342,     0,   343,   344,   345,     0,   346,   347,
     348,   349,   350,     0,   351,   352,   353,   354,   355,   356,
     357,   332,     0,   358,   359,   360,     0,   670,   333,   218,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     334,   844,   219,   220,     0,   221,     0,     0,     0,     0,
     222,     0,     0,   332,     0,     0,     0,     0,   223,     0,
     333,     0,     0,     0,   224,     0,     0,     0,     0,   225,
       0,   226,   334,   847,   227,     0,     0,     0,     0,     0,
       0,     0,   228,     0,     0,     0,     0,     0,   229,   230,
     335,     0,     0,     0,   336,   231,     0,     0,     0,     0,
       0,     0,     0,     0,   232,     0,     0,     0,     0,     0,
       0,     0,     0,   233,   234,     0,   235,     0,   236,     0,
     237,     0,   335,   238,     0,   337,   336,   239,     0,     0,
     240,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,   338,   339,     0,   340,   341,   342,     0,   343,   344,
     345,     0,   346,   347,   348,   349,   350,   337,   351,   352,
     353,   354,   355,   356,   357,     0,     0,   358,   359,   360,
       0,     0,     0,   338,   339,     0,   340,   341,   342,     0,
     343,   344,   345,     0,   346,   347,   348,   349,   350,     0,
     351,   352,   353,   354,   355,   356,   357,   332,     0,   358,
     359,   360,     0,     0,   333,  -224,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   334,     0,  -224,  -224,
       0,  -224,     0,     0,     0,     0,  -224,     0,     0,   332,
       0,     0,     0,     0,  -224,     0,   333,     0,     0,     0,
    -224,     0,     0,     0,     0,  -224,     0,  -224,   334,     0,
    -224,     0,     0,     0,     0,     0,     0,     0,  -224,     0,
       0,     0,     0,     0,  -224,  -224,   335,     0,     0,     0,
     336,  -224,     0,     0,     0,     0,     0,     0,     0,   923,
    -224,     0,     0,     0,     0,     0,   332,   942,     0,  -224,
    -224,     0,  -224,   333,  -224,     0,  -224,     0,   335,  -224,
       0,   337,   336,  -224,     0,   334,  -224,     0,     0,  -224,
       0,   926,     0,     0,     0,     0,     0,   338,   339,     0,
     340,   341,   342,     0,   343,   344,   345,     0,   346,   347,
     348,   349,   350,   337,   351,   352,   353,   354,   355,   356,
     357,     0,     0,   358,   359,   360,     0,     0,     0,   338,
     339,     0,   340,   341,   342,   335,   343,   344,   345,   336,
     346,   347,   348,   349,   350,     0,   351,   352,   353,   354,
     355,   356,   357,   332,     0,   358,   359,   360,     0,     0,
     333,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     337,     0,   334,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   338,   339,     0,   340,
     341,   342,     0,   343,   344,   345,     0,   346,   347,   348,
     349,   350,     0,   351,   352,   353,   354,   355,   356,   357,
       0,     0,   358,   359,   360,     0,     0,     0,     0,     0,
       0,     0,   335,     0,     0,     0,   336,     0,     0,   332,
       0,     0,     0,     0,     0,  1024,   333,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   334,     0,
       0,     0,     0,     0,     0,     0,     0,   337,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   338,   339,     0,   340,   341,   342,     0,
     343,   344,   345,     0,   346,   347,   348,   349,   350,     0,
     351,   352,   353,   354,   355,   356,   357,     0,   335,   358,
     359,   360,   336,     0,     0,   332,     0,     0,     0,     0,
       0,  1025,   333,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   334,     0,     0,     0,     0,     0,
       0,     0,     0,   337,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   338,
     339,     0,   340,   341,   342,     0,   343,   344,   345,     0,
     346,   347,   348,   349,   350,     0,   351,   352,   353,   354,
     355,   356,   357,     0,   335,   358,   359,   360,   336,     0,
       0,   332,     0,     0,     0,     0,     0,  1026,   333,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     334,     0,     0,     0,     0,     0,     0,     0,     0,   337,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   338,   339,     0,   340,   341,
     342,     0,   343,   344,   345,     0,   346,   347,   348,   349,
     350,     0,   351,   352,   353,   354,   355,   356,   357,     0,
     335,   358,   359,   360,   336,     0,     0,   332,     0,     0,
       0,     0,     0,  1027,   333,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   334,     0,     0,     0,
       0,     0,     0,     0,     0,   337,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   338,   339,     0,   340,   341,   342,     0,   343,   344,
     345,     0,   346,   347,   348,   349,   350,     0,   351,   352,
     353,   354,   355,   356,   357,     0,   335,   358,   359,   360,
     336,     0,     0,   332,     0,     0,     0,     0,     0,  1028,
     333,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   334,     0,     0,     0,     0,     0,     0,     0,
       0,   337,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   338,   339,     0,
     340,   341,   342,     0,   343,   344,   345,     0,   346,   347,
     348,   349,   350,     0,   351,   352,   353,   354,   355,   356,
     357,     0,   335,   358,   359,   360,   336,     0,     0,   332,
       0,     0,     0,     0,     0,  1029,   333,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   334,     0,
       0,     0,     0,     0,     0,     0,     0,   337,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1053,   338,   339,     0,   340,   341,   342,     0,
     343,   344,   345,     0,   346,   347,   348,   349,   350,     0,
     351,   352,   353,   354,   355,   356,   357,     0,   335,   358,
     359,   360,   336,     0,     0,   332,     0,     0,     0,     0,
       0,     0,   333,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   334,     0,     0,     0,     0,     0,
       0,     0,     0,   337,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   338,
     339,     0,   340,   341,   342,     0,   343,   344,   345,     0,
     346,   347,   348,   349,   350,     0,   351,   352,   353,   354,
     355,   356,   357,     0,   335,   358,   359,   360,   336,     0,
       0,   332,     0,     0,     0,     0,     0,     0,   333,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     334,     0,     0,     0,     0,     0,     0,     0,     0,   337,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   338,   339,     0,   340,   341,
     342,     0,   343,   344,   345,     0,   346,   347,   348,   349,
     350,     0,   351,   352,   353,   354,   355,   356,   357,     0,
     693,   358,   359,   360,   336,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   337,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   338,   339,     0,   340,   341,   342,     0,   343,   344,
     345,   334,   346,   347,   348,   349,   350,     0,   351,   352,
     353,   354,   355,   356,   357,     0,   260,   358,   359,   360,
      22,    23,     0,     0,     0,     0,     0,     0,     0,   261,
       0,    30,   262,     0,     0,     0,    35,     0,     0,     0,
       0,    39,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   335,     0,     0,     0,   336,     0,     0,     0,     0,
       0,     0,     0,    52,     0,    54,     0,    56,     0,   906,
       0,     0,   907,     0,   263,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   337,     0,     0,     0,
       0,     0,     0,     0,     0,    78,     0,     0,    80,     0,
       0,    82,   338,   339,     0,   340,   341,   342,     0,   343,
     344,   345,     0,   346,   347,   348,   349,     0,   218,   351,
     352,   353,   354,   355,   356,   357,     0,     0,   358,   359,
     360,   219,   220,     0,   221,     0,     0,     0,     0,   222,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,    97,     0,   224,     0,     0,     0,  1031,   225,     0,
     226,     0,     0,   227,     0,     0,     0,     0,     0,     0,
       0,   228,     0,     0,     0,  -215,     0,   229,   230,     0,
       0,     0,     0,     0,   231,     0,     0,     0,  -215,  -215,
       0,  -215,     0,   232,     0,     0,  -215,     0,     0,     0,
       0,     0,   233,   234,  -215,   235,     0,   236,     0,   237,
    -215,     0,   238,     0,     0,  -215,   239,  -215,     0,   240,
    -215,     0,   241,     0,     0,     0,     0,     0,  -215,     0,
       0,     0,  -146,     0,  -215,  -215,     0,     0,     0,     0,
       0,  -215,     0,     0,     0,  -146,  -146,     0,  -146,     0,
    -215,     0,     0,  -146,     0,     0,     0,     0,     0,  -215,
    -215,  -146,  -215,     0,  -215,     0,  -215,  -146,     0,  -215,
       0,     0,  -146,  -215,  -146,     0,  -215,  -146,     0,  -215,
       0,     0,     0,     0,     0,  -146,     0,     0,     0,  -142,
       0,  -146,  -146,     0,     0,     0,     0,     0,  -146,     0,
       0,     0,  -142,  -142,     0,  -142,     0,  -146,     0,     0,
    -142,     0,     0,     0,     0,     0,  -146,  -146,  -142,  -146,
       0,  -146,     0,  -146,  -142,     0,  -146,     0,     0,  -142,
    -146,  -142,     0,  -146,  -142,     0,  -146,     0,     0,     0,
       0,     0,  -142,     0,     0,     0,     0,     0,  -142,  -142,
       0,     0,     0,     0,     0,  -142,     0,     0,     0,     0,
       0,     0,     0,     0,  -142,     0,     0,     0,     0,     0,
       0,     0,     0,  -142,  -142,     0,  -142,     0,  -142,     0,
    -142,     0,     0,  -142,     0,     0,     0,  -142,     0,     0,
    -142,     0,     0,  -142
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-951)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      12,   148,   466,     2,    16,   438,   135,   382,    20,    19,
     587,    63,    58,    59,    26,   212,   679,   780,    30,    31,
     197,    33,    34,    35,    36,   754,   217,   428,   957,   189,
      22,     1,    44,    44,    46,    47,   877,   834,   835,    51,
     217,    53,   620,    55,    31,   205,    31,     1,     0,   101,
      44,    63,    64,    65,    66,    67,    68,   880,    66,    31,
      72,   190,    74,   575,    76,    77,   578,   109,   259,    81,
      81,     1,    84,    85,   443,    87,     1,    89,    31,     3,
      92,    93,   259,    95,    44,    31,    44,    99,   100,   101,
     281,   997,   111,  1043,   111,   137,    66,   466,   156,   137,
     108,    44,     1,    44,   281,   946,    44,   116,   137,   435,
     839,    44,    83,   690,   692,   137,   313,   314,   156,    81,
      54,   137,   100,    81,    54,   177,   111,     3,   137,    54,
    1080,  1060,   141,   162,   131,    33,    34,  1043,   108,   111,
     156,   288,   139,   137,   166,    44,   909,   166,   971,   166,
     137,     3,   881,    83,   162,   164,   168,    64,    83,   171,
     172,   173,   116,   175,   176,   177,    96,   344,   111,   932,
      94,   934,   166,   751,   161,   109,   161,   137,   188,   109,
     977,   978,   136,   137,   109,   197,   619,   378,   379,   161,
     161,    26,   162,   100,   137,  1002,   137,   709,    30,   137,
     712,   378,   379,   101,   137,   217,   136,    83,   161,   201,
     164,   136,   587,    83,   206,   161,   945,   116,    94,   197,
     111,   213,   985,   166,   987,   166,   156,    41,   166,   137,
     116,   161,   163,   166,   137,    70,   161,   136,   137,   217,
    1047,  1048,  1049,  1050,  1051,  1052,    81,   259,    80,   163,
     262,   137,    84,   137,   162,   141,   919,   156,   270,   271,
       3,   164,     7,   137,    78,   100,   667,   996,   459,   281,
     322,   106,    76,    77,   137,   137,    19,  1058,   164,   408,
     164,   259,   459,    49,   413,   862,   156,    44,   156,    32,
     164,   161,   116,    91,   162,  1076,   308,    54,   853,   131,
     855,   164,   164,   281,   136,   163,   305,   139,   163,    75,
     322,   143,   745,    56,   128,   690,   130,   141,    26,   331,
     332,   333,   334,   335,   336,   337,    83,   339,   340,   341,
     342,   343,   161,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,    44,
     137,    94,   109,   137,    76,   367,   368,   100,   137,   116,
     137,   373,    70,   415,   376,   377,   378,   379,   380,   381,
     382,    66,   156,    81,   105,   189,   931,   164,   933,   391,
     137,    76,    77,   137,   396,   164,   398,   164,    26,   131,
     137,   205,   156,   207,   591,   592,   573,   139,   106,   156,
     378,   379,   137,   415,   161,   417,   418,   164,    46,   156,
     164,   423,   424,   147,   155,   427,   428,   743,   430,   156,
     432,   156,   137,   887,   156,   137,   163,   137,   165,   156,
      68,   245,    70,   445,    26,    54,    26,   335,   336,   156,
     452,   156,   131,    81,   156,   497,   156,   459,   201,   156,
     139,   156,   266,   206,   780,   162,   653,   654,   156,    80,
     367,   368,   474,    84,    83,   477,   373,   479,   106,   376,
     377,   483,   484,   485,   452,   487,   488,    96,    70,   163,
      70,   459,   380,   381,   644,   497,   163,   862,   165,    81,
     109,    81,   187,   391,   493,   131,   474,   156,    80,   477,
     136,   479,    84,   139,   156,   161,   484,   143,   100,   156,
     100,   880,    26,   110,   209,   210,   211,   212,   887,    79,
     418,   216,    79,   163,    30,   165,   424,   139,   163,    44,
      44,   163,   679,    47,   163,   163,   165,   156,   163,    54,
     165,   163,   161,   165,   116,   557,   558,   559,   560,   561,
     562,   563,   156,    67,   564,   565,    70,   252,    72,   571,
     255,   573,   477,   156,   479,     7,     2,    81,    83,   581,
     477,   583,   479,   625,    80,   587,    12,   589,    84,    58,
     136,   161,   277,   156,   116,   483,   100,   164,   914,    25,
     137,    27,   406,   571,   109,   573,   410,   156,    44,   163,
     295,   116,   971,    33,   137,   116,   932,    20,   934,   164,
     164,   623,   624,   625,   164,   164,   137,   629,   166,   161,
     166,   156,   137,   137,   438,   131,   132,   163,   997,   135,
     136,   156,   138,   139,   156,   164,    72,   143,   156,   156,
     116,   156,  1011,   166,   156,   151,   161,   137,   156,   164,
     164,   157,   158,   159,   666,   667,   156,    33,   162,   985,
     558,   987,   560,    99,   571,   164,   142,   103,   144,   145,
     164,   137,   148,   149,  1043,   162,   116,    81,   690,   164,
     494,   693,   694,   695,    31,   499,   698,    31,   166,  1058,
      44,    30,   161,   161,    81,    66,    54,   392,    33,   136,
      54,   166,   136,   166,   166,   156,   718,  1076,   720,   761,
     162,  1080,   156,   725,    54,   163,   728,   139,   139,   731,
     139,   733,   629,   131,   736,    83,   131,   422,   164,    83,
     139,   426,   139,   139,   131,   131,   139,   139,    96,   139,
     139,    80,   131,    83,   139,    84,   139,   161,    33,   761,
     161,   109,   164,    96,   164,   109,    96,     2,   131,   771,
     574,   773,   116,   577,   776,   131,   319,    12,   166,   109,
     782,   667,   919,   137,   666,    24,   289,   765,   783,   919,
      25,   914,    27,   137,   743,   612,   786,  1060,   996,  1020,
    1053,  1046,   131,   132,    35,   693,   694,   136,   156,   138,
     139,   615,   156,   161,   143,   619,   242,   161,   573,   446,
     164,    30,   151,   249,    -1,    -1,   156,   829,   157,   158,
     159,   161,    -1,    -1,    -1,    -1,   640,    72,    -1,    -1,
     644,   843,   844,    -1,   846,   847,   848,   849,   209,   210,
     211,   212,    -1,    -1,    -1,   857,   858,    -1,   860,    -1,
     862,    -1,   864,   865,    99,    -1,   868,    -1,   103,    -1,
     906,   907,    -1,    -1,    -1,   679,    -1,    -1,    -1,   305,
      -1,    -1,    -1,    -1,   886,   887,    -1,    -1,    -1,    -1,
     892,   252,   894,   319,   255,    -1,   898,    -1,    -1,    -1,
     585,   586,    -1,    -1,    -1,   947,   591,   592,    -1,    -1,
      -1,    -1,   880,    -1,    -1,    -1,   277,    -1,   603,    -1,
      -1,   923,   924,   925,   926,   927,   928,   929,   930,   164,
      -1,    -1,    -1,    -1,   295,    -1,    -1,    -1,   742,    -1,
     942,   745,    -1,    -1,    -1,   947,    -1,    -1,    -1,    -1,
     952,    -1,    -1,    -1,   956,   640,    -1,    -1,    -1,    -1,
      -1,   765,    26,   389,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1015,    -1,    -1,     2,    -1,    -1,   783,
      44,    -1,    -1,    47,   952,    -1,    12,   989,   990,   991,
     992,   417,    -1,    -1,    -1,    -1,    -1,   423,    -1,    25,
      -1,    27,    -1,    67,   430,   809,    70,   242,    72,  1011,
      -1,  1013,  1014,  1015,   249,    -1,    -1,    81,   703,   704,
      -1,    -1,  1024,  1025,  1026,  1027,  1028,  1029,    -1,   997,
     715,   392,   717,    -1,    -1,    -1,   100,    -1,    -1,   724,
    1042,  1043,   727,  1045,    -1,   730,    72,    -1,    26,    -1,
      76,    77,    -1,    -1,    -1,   481,    -1,    30,    -1,   485,
      -1,   422,    -1,    -1,    -1,   426,    44,   493,    -1,    47,
     305,    -1,    -1,    99,  1076,  1043,     2,   103,  1080,    -1,
      -1,    -1,    -1,   768,   319,    -1,    12,    -1,    -1,    67,
      -1,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    81,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    84,  1080,    -1,    -1,   919,     1,    -1,    -1,    -1,
      -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,   554,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,   164,    -1,
      25,    26,    -1,    -1,    -1,    -1,    72,    -1,    -1,    34,
      -1,    36,    37,    -1,   389,    -1,    41,    -1,   131,    -1,
      -1,    46,    -1,   136,    49,   138,   139,    -1,    -1,    -1,
     143,    -1,    -1,    99,    -1,    -1,   602,   103,   151,    -1,
      -1,    -1,   417,    68,    -1,    70,   159,    72,   423,    74,
      75,    -1,    77,    -1,    79,   430,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   890,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,   100,   242,    -1,   103,    -1,
      -1,   106,    -1,   249,   585,   586,   652,    -1,    -1,    -1,
     591,   592,   658,    -1,    -1,    -1,    -1,    -1,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   481,    -1,    -1,    -1,
     485,   936,   937,   679,    -1,   940,   941,    -1,   493,    80,
      -1,    -1,    -1,    84,    -1,    -1,    -1,   693,   694,    -1,
      -1,   156,   957,    -1,    -1,    -1,    -1,   162,   963,   305,
     965,    -1,   967,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   718,   319,   720,    -1,    -1,    -1,    -1,   725,
      -1,    -1,   728,    -1,    -1,    -1,    -1,    -1,    -1,   735,
     131,   132,   738,   134,   135,   136,   242,   138,   139,   140,
      -1,    -1,   143,   249,    -1,    -1,    -1,    -1,    -1,   150,
     151,    -1,    -1,   154,   760,    -1,   157,   158,   159,    -1,
      -1,    -1,   703,   704,    -1,   771,    -1,   773,    -1,    -1,
      -1,    -1,    -1,    -1,   715,    -1,    -1,    -1,    -1,    -1,
       3,    -1,    -1,   389,    -1,    -1,   727,   602,    -1,   730,
      -1,    -1,    -1,    16,    17,  1060,    19,    -1,    -1,   305,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,   417,    -1,   319,    -1,    38,    -1,   423,    -1,    -1,
      43,    -1,    45,    -1,   430,    48,    -1,    -1,    -1,    -1,
      -1,     2,    -1,    56,    -1,    -1,    -1,   652,    -1,    62,
      63,    12,    -1,   658,    -1,    -1,    69,    -1,    -1,    -1,
      -1,   857,    -1,    -1,    25,    78,    27,    -1,   864,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    90,    -1,    92,
     876,    94,    -1,    -1,    97,   481,    -1,    -1,   101,   485,
      -1,   104,   888,   389,   107,    -1,    -1,   493,    -1,    -1,
      11,   897,    -1,    -1,    -1,   901,    -1,    18,    -1,    -1,
      -1,    72,    -1,   718,    -1,   720,    -1,    -1,    -1,    30,
     725,   417,    -1,   728,    -1,    -1,    -1,   423,    -1,    -1,
     735,    -1,    -1,   738,   430,    -1,    -1,    -1,    99,    -1,
      -1,    -1,   103,    -1,    -1,    -1,    -1,   943,   161,    -1,
     163,    -1,    -1,    -1,    -1,   760,    -1,    30,    -1,   890,
      -1,    -1,    -1,    -1,    -1,    -1,   771,    -1,   773,    80,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   481,    -1,    -1,    -1,   485,
      -1,    -1,    -1,   989,   990,   991,   992,   493,    -1,    -1,
     111,    -1,    -1,   164,   115,    -1,   602,    80,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,  1013,  1014,    -1,
     131,   132,    -1,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,    -1,    -1,   157,   158,   159,    -1,
       2,    -1,   857,    -1,    -1,   166,   652,    -1,   131,   864,
      12,    -1,   658,   136,    -1,   138,   139,    -1,    -1,    -1,
     143,   876,    -1,    25,     3,    27,    -1,    -1,   151,    -1,
      -1,   242,    -1,   888,   157,   158,   159,    16,   249,    -1,
      19,    -1,   897,    -1,    -1,    24,   901,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   602,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    48,
      72,    -1,   718,    -1,   720,    -1,    -1,    56,    -1,   725,
      -1,    -1,   728,    -1,    63,    -1,    -1,    -1,   943,   735,
      -1,    -1,   738,    -1,   305,    -1,    -1,    99,    -1,    78,
      -1,   103,    -1,    -1,    -1,    -1,   652,     2,   319,    -1,
      -1,    90,   658,    -1,   760,    94,    -1,    12,    -1,    -1,
      -1,    -1,   101,    -1,    -1,   771,    -1,   773,   107,    -1,
      25,    -1,    27,    -1,   989,   990,   991,   992,    -1,    -1,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,  1013,  1014,
      -1,     2,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,   718,    -1,   720,    -1,    -1,    72,   389,   725,
      -1,    -1,   728,    -1,    25,   160,    27,    -1,   163,   735,
     165,    -1,   738,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   417,    -1,   103,    -1,
      -1,   857,   423,    -1,   760,    -1,    -1,    -1,   864,   430,
      -1,    -1,    -1,    -1,    -1,   771,    -1,   773,    21,    -1,
     876,    72,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
     242,    34,   888,    36,    37,    -1,    -1,   249,    41,    -1,
      -1,   897,    -1,    46,    -1,   901,    -1,    -1,    99,    -1,
      -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,     2,   164,
     481,    -1,    -1,    -1,   485,    68,    -1,    70,    12,    72,
      -1,    74,   493,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,   943,    30,    -1,
      -1,    -1,    -1,   305,    -1,    -1,    -1,   100,    -1,    -1,
     103,   857,    -1,   106,    -1,    -1,    -1,   319,   864,    -1,
      -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,
     876,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,   888,   989,   990,   991,   992,   242,    80,    -1,
      -1,   897,    84,    -1,   249,   901,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    99,    -1,  1013,  1014,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   389,    -1,    -1,
      -1,   602,    -1,    -1,    -1,    -1,    -1,   943,    -1,   131,
     132,   242,   134,   135,   136,    -1,   138,   139,   249,    -1,
     305,   143,    -1,    -1,    -1,   417,    -1,    -1,   150,   151,
      -1,   423,   154,    -1,   319,   157,   158,   159,   430,    -1,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   652,    -1,   989,   990,   991,   992,   658,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   305,    -1,    -1,  1013,  1014,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   319,   481,
      -1,    -1,    -1,   485,    -1,    21,    -1,    -1,    -1,    25,
      26,   493,    -1,    -1,   389,    -1,    -1,    -1,    34,    -1,
      36,    37,    -1,    30,    -1,    41,    -1,   718,   242,   720,
      46,    -1,    -1,    -1,   725,   249,    -1,   728,    -1,    -1,
      -1,    -1,   417,    -1,   735,    -1,    -1,   738,   423,    -1,
      -1,    -1,    68,    -1,    70,   430,    72,    -1,    74,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,   389,   760,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,
     771,    -1,   773,    -1,   100,    -1,    -1,   103,    -1,    -1,
     106,   305,    -1,    -1,    -1,    -1,   417,    -1,    -1,    -1,
      -1,    -1,   423,    -1,    -1,   319,   481,    -1,   115,   430,
     485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   493,    -1,
     602,    -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,    -1,
     156,   148,   149,   150,   151,   152,   162,   154,    -1,    -1,
     157,   158,   159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     481,    -1,    -1,    -1,   485,    -1,   857,    -1,    -1,    -1,
     652,    -1,   493,   864,    -1,   389,   658,    -1,    -1,    21,
      -1,    -1,    -1,    25,    26,   876,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    37,    -1,   888,    -1,    41,
      -1,    -1,    -1,   417,    46,    -1,   897,    -1,    -1,   423,
     901,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,   602,    70,    -1,
      72,    -1,    74,    -1,    -1,    77,   718,    79,   720,    81,
      -1,    -1,    -1,   725,    -1,    -1,   728,    -1,    -1,    -1,
      -1,    -1,   943,   735,    -1,    -1,   738,    -1,   100,    -1,
      -1,   103,    -1,    30,   106,    -1,    -1,   481,    -1,    -1,
      -1,   485,    -1,    -1,    -1,    -1,    -1,   652,   760,   493,
      -1,   602,    -1,   658,    -1,    -1,    -1,    -1,    -1,   771,
      -1,   773,    -1,    -1,    -1,    -1,    -1,    -1,   989,   990,
     991,   992,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,   156,    -1,    -1,    84,    -1,    -1,
     162,    -1,  1013,  1014,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   652,    -1,    -1,    -1,    -1,    -1,   658,    -1,    -1,
      -1,    -1,    -1,   718,    -1,   720,    -1,    -1,    -1,    -1,
     725,    -1,    -1,   728,    -1,    -1,    -1,    -1,    -1,    -1,
     735,    -1,    -1,   738,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,    -1,    -1,   857,   143,    -1,    -1,    -1,
      -1,    -1,   864,    -1,   151,   760,    -1,     3,   602,    -1,
     157,   158,   159,    -1,   876,    -1,   771,   718,   773,   720,
      16,    17,    -1,    19,   725,    -1,   888,   728,    24,    -1,
      -1,    -1,    -1,    -1,   735,   897,    32,   738,    -1,   901,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,   652,   760,
      56,    -1,    -1,    -1,   658,    -1,    62,    63,    -1,    -1,
     771,    -1,   773,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   943,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,    -1,
      -1,    97,   857,    -1,    -1,   101,   102,    -1,   104,   864,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   876,    -1,    -1,   718,    -1,   720,   989,   990,   991,
     992,   725,    -1,   888,   728,    -1,    -1,    -1,    -1,    -1,
      -1,   735,   897,    -1,   738,    -1,   901,    30,    -1,    -1,
      -1,  1013,  1014,    -1,    -1,    -1,   857,    -1,    -1,    -1,
      -1,    -1,    -1,   864,    -1,    21,   760,   163,    -1,    25,
      26,    -1,    -1,    -1,    -1,   876,    -1,   771,    34,   773,
      36,    37,    -1,    -1,    -1,    41,    -1,   888,   943,    -1,
      46,    -1,    -1,    -1,    -1,    -1,   897,    80,    -1,    -1,
     901,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   989,   990,   991,   992,    -1,    -1,
      -1,    -1,   943,    -1,   100,    -1,    -1,   103,   131,   132,
     106,   134,   135,   136,    -1,   138,   139,   140,  1013,  1014,
     143,   144,   145,    -1,    -1,   148,   149,   150,   151,    -1,
      -1,   154,    -1,    -1,   157,   158,   159,    -1,    -1,    -1,
      -1,    -1,   876,    -1,    -1,    -1,    -1,    -1,   989,   990,
     991,   992,    -1,    -1,   888,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   897,    -1,    -1,   162,   901,    -1,    -1,
      -1,    -1,  1013,  1014,     0,     1,    -1,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    14,    15,
      16,    17,    -1,    19,    -1,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,    31,    32,    -1,    34,   943,
      36,    37,    38,    39,    40,    41,    42,    43,    -1,    45,
      46,    -1,    48,    49,    -1,    51,    52,    53,    -1,    55,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    -1,
      -1,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,
     156,    -1,    -1,   159,    -1,   161,    -1,   163,     1,   165,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      13,    -1,    -1,    16,    17,    -1,    19,    -1,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    34,    -1,    -1,    -1,    38,    39,    40,    -1,    42,
      43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      53,    -1,    -1,    56,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    -1,
      73,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,
     153,   154,   155,    -1,    -1,    -1,   159,    -1,   161,    -1,
     163,    -1,   165,   166,     1,    -1,     3,     4,     5,     6,
       7,     8,     9,    -1,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    36,
      37,    38,    39,    40,    41,    42,    43,    -1,    45,    46,
      -1,    48,    49,    -1,    51,    52,    53,    -1,    55,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    -1,    -1,
      -1,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,   101,    -1,   103,   104,   105,   106,
     107,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,    -1,   161,   162,   163,     1,   165,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    -1,    13,
      14,    15,    16,    17,    -1,    19,    -1,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    31,    32,    -1,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      -1,    45,    46,    -1,    48,    49,    -1,    51,    52,    53,
      -1,    55,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    -1,    -1,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    85,    86,    87,    88,    89,    90,    91,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,   153,
     154,   155,   156,    -1,    -1,   159,    -1,   161,   162,   163,
       1,   165,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    14,    15,    16,    17,    -1,    19,    -1,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      31,    32,    -1,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    45,    46,    -1,    48,    49,    -1,
      51,    52,    53,    -1,    55,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    -1,    -1,    -1,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    85,    86,    87,    88,    89,    90,
      91,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
     101,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,
      -1,    -1,   153,   154,   155,   156,    -1,    -1,   159,    -1,
     161,   162,   163,     1,   165,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    13,    14,    15,    16,    17,
      -1,    19,    -1,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    -1,    31,    32,    -1,    34,    -1,    36,    37,
      38,    39,    40,    41,    42,    43,    -1,    45,    46,    -1,
      48,    49,    -1,    51,    52,    53,    -1,    55,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    -1,    -1,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
      -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,    -1,   153,   154,   155,   156,    -1,
      -1,   159,    -1,   161,   162,   163,     1,   165,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    13,    14,
      15,    16,    17,    -1,    19,    -1,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,    31,    32,    -1,    34,
      -1,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      45,    46,    -1,    48,    49,    -1,    51,    52,    53,    -1,
      55,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      -1,    -1,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      85,    86,    87,    88,    89,    90,    91,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,   101,    -1,   103,   104,
     105,   106,   107,    -1,    -1,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,
     155,   156,    -1,    -1,   159,    -1,   161,   162,   163,     1,
     165,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      -1,    13,    14,    15,    16,    17,    -1,    19,    -1,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    31,
      32,    -1,    34,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    -1,    45,    46,    -1,    48,    49,    -1,    51,
      52,    53,    -1,    55,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    -1,    -1,    -1,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    85,    86,    87,    88,    89,    90,    91,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,    -1,    -1,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,
      -1,   153,   154,   155,   156,    -1,    -1,   159,    -1,   161,
     162,   163,     1,   165,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    13,    14,    15,    16,    17,    -1,
      19,    -1,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    -1,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    -1,    45,    46,    -1,    48,
      49,    -1,    51,    52,    53,    -1,    55,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    -1,    -1,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    85,    86,    87,    88,
      89,    90,    91,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,   101,    -1,   103,   104,   105,   106,   107,    -1,
      -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,    -1,   153,   154,   155,   156,    -1,    -1,
     159,    -1,   161,    -1,   163,     1,   165,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    14,    15,
      16,    17,    -1,    19,    -1,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,    31,    32,    -1,    34,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    -1,    45,
      46,    -1,    48,    49,    -1,    51,    52,    53,    -1,    55,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    -1,
      -1,    -1,    68,    69,    70,    71,    -1,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,
     156,    -1,    -1,   159,    -1,   161,    -1,   163,     1,   165,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      13,    14,    15,    16,    17,    -1,    19,    -1,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,    31,    32,
      -1,    34,    -1,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    46,    -1,    48,    49,    -1,    51,    52,
      53,    -1,    55,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    -1,    -1,    -1,    68,    69,    70,    71,    -1,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    85,    86,    87,    88,    89,    90,    91,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,   101,    -1,
     103,   104,   105,   106,   107,    -1,    -1,   110,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,
     153,   154,   155,   156,    -1,    -1,   159,    -1,   161,    -1,
     163,     1,   165,     3,     4,     5,     6,     7,     8,     9,
      -1,    -1,    -1,    13,    14,    15,    16,    17,    -1,    19,
      -1,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    31,    32,    -1,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    -1,    45,    46,    -1,    48,    49,
      -1,    51,    52,    53,    -1,    55,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    -1,    -1,    -1,    68,    69,
      70,    71,    -1,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    85,    86,    87,    88,    89,
      90,    91,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,    -1,    -1,
     110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,    -1,    -1,   153,   154,   155,   156,    -1,    -1,   159,
      -1,   161,    -1,   163,     1,   165,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    -1,    13,    14,    15,    16,
      17,    -1,    19,    -1,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    31,    32,    -1,    34,    -1,    36,
      37,    38,    39,    40,    41,    42,    43,    -1,    45,    46,
      -1,    48,    49,    -1,    51,    52,    53,    -1,    55,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    -1,    -1,
      -1,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,   101,    -1,   103,   104,   105,   106,
     107,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,   156,
      -1,    -1,   159,    -1,   161,    -1,   163,     1,   165,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    -1,    13,
      14,    15,    16,    17,    -1,    19,    -1,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    31,    32,    -1,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      -1,    45,    46,    -1,    48,    49,    -1,    51,    52,    53,
      -1,    55,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    -1,    -1,    -1,    68,    69,    70,    71,    -1,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    85,    86,    87,    88,    89,    90,    91,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,   153,
     154,   155,   156,    -1,    -1,   159,    -1,   161,    -1,   163,
       1,   165,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    14,    15,    16,    17,    -1,    19,    -1,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      31,    32,    -1,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    45,    46,    -1,    48,    49,    -1,
      51,    52,    53,    -1,    55,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    -1,    -1,    -1,    68,    69,    70,
      71,    -1,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    85,    86,    87,    88,    89,    90,
      91,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
     101,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,
      -1,    -1,   153,   154,   155,   156,    -1,    -1,   159,    -1,
     161,    -1,   163,     1,   165,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    13,    14,    15,    16,    17,
      -1,    19,    -1,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    -1,    31,    32,    -1,    34,    -1,    36,    37,
      38,    39,    40,    41,    42,    43,    -1,    45,    46,    -1,
      48,    49,    -1,    51,    52,    53,    -1,    55,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    -1,    -1,    -1,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
      -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,    -1,   153,   154,   155,   156,    -1,
      -1,   159,    -1,   161,    -1,   163,     1,   165,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    13,    14,
      15,    16,    17,    -1,    19,    -1,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,    31,    32,    -1,    34,
      -1,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      45,    46,    -1,    48,    49,    -1,    51,    52,    53,    -1,
      55,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      -1,    -1,    -1,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      85,    86,    87,    88,    89,    90,    91,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,   101,    -1,   103,   104,
     105,   106,   107,    -1,    -1,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,
     155,   156,    -1,    -1,   159,    -1,   161,    -1,   163,     1,
     165,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      -1,    13,    14,    15,    16,    17,    -1,    19,    -1,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    31,
      32,    -1,    34,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    -1,    45,    46,    -1,    48,    49,    -1,    51,
      52,    53,    -1,    55,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    -1,    -1,    -1,    68,    69,    70,    71,
      -1,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    85,    86,    87,    88,    89,    90,    91,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,    -1,    -1,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,
      -1,   153,   154,   155,   156,    -1,    -1,   159,    -1,   161,
      -1,   163,     1,   165,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    13,    14,    15,    16,    17,    -1,
      19,    -1,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    -1,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    -1,    45,    46,    -1,    48,
      49,    -1,    51,    52,    53,    -1,    55,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    -1,    -1,    -1,    68,
      69,    70,    71,    -1,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    85,    86,    87,    88,
      89,    90,    91,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,   101,    -1,   103,   104,   105,   106,   107,    -1,
      -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,    -1,   153,   154,   155,   156,    -1,    -1,
     159,    -1,   161,    -1,   163,     1,   165,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,
      16,    17,    -1,    19,    -1,    21,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,    -1,
      -1,    -1,    38,    39,    40,    -1,    42,    43,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    -1,
      56,    57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    71,    -1,    73,    -1,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    89,    90,    91,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,
      -1,    -1,    -1,   159,    -1,   161,    -1,   163,    -1,   165,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      13,    -1,    -1,    16,    17,    -1,    19,    -1,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    34,    -1,    -1,    -1,    38,    39,    40,    -1,    42,
      43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      53,    -1,    -1,    56,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    -1,
      73,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,
     153,   154,   155,    -1,    -1,    -1,   159,    -1,   161,    -1,
     163,    -1,   165,   166,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,    -1,
      19,    -1,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    34,    -1,    -1,    -1,    38,
      39,    40,    -1,    42,    43,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    -1,    56,    57,    -1,
      59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    71,    -1,    73,    -1,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    91,    92,    -1,    94,    -1,    -1,    97,    -1,
      -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,    -1,   153,   154,   155,    -1,    -1,    -1,
     159,    -1,   161,    -1,   163,    -1,   165,   166,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,
      -1,    16,    17,    -1,    19,    -1,    21,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,
      -1,    56,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    71,    -1,    73,    -1,
      -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    91,    92,    -1,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,
     155,    -1,    -1,    -1,   159,    -1,   161,    -1,   163,    -1,
     165,   166,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    -1,    -1,    16,    17,    -1,    19,    -1,
      21,    -1,    -1,    24,    25,    26,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    34,    -1,    -1,    -1,    38,    39,    40,
      -1,    42,    43,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    -1,    56,    57,    -1,    59,    60,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    -1,    -1,    78,    79,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
     101,    -1,    -1,   104,    -1,   106,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,
      -1,    -1,   153,   154,   155,    -1,    -1,    -1,   159,    -1,
     161,    -1,   163,    -1,   165,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,
      -1,    19,    -1,    21,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    34,    -1,    -1,    -1,
      38,    39,    40,    -1,    42,    43,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    53,    -1,    -1,    56,    57,
      -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    71,    -1,    73,    -1,    -1,    -1,    -1,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    -1,    94,    -1,    -1,    97,
      98,    99,    -1,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,    -1,   153,   154,   155,    -1,    -1,
      -1,   159,    -1,   161,    -1,   163,    -1,   165,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,
      -1,    16,    17,    -1,    19,    -1,    21,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,    44,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,
      -1,    56,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    71,    -1,    73,    -1,
      -1,    -1,    -1,    78,    79,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    91,    92,    -1,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,   104,
      -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,
     155,    -1,    -1,    -1,   159,    -1,   161,    -1,   163,    -1,
     165,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      -1,    13,    -1,    -1,    16,    17,    -1,    19,    -1,    21,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    34,    -1,    -1,    -1,    38,    39,    40,    -1,
      42,    43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    53,    -1,    -1,    56,    57,    -1,    59,    60,    -1,
      62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,
      -1,    73,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,
      92,    -1,    94,    -1,    -1,    97,    98,    99,    -1,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,
      -1,   153,   154,   155,    -1,    -1,    -1,   159,    -1,   161,
      -1,   163,   164,   165,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,    -1,
      19,    -1,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    34,    -1,    -1,    -1,    38,
      39,    40,    -1,    42,    43,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    -1,    56,    57,    -1,
      59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    71,    -1,    73,    -1,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    91,    92,    -1,    94,    -1,    -1,    97,    98,
      99,    -1,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,    -1,   153,   154,   155,    -1,    -1,    -1,
     159,    -1,   161,    -1,   163,   164,   165,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,
      16,    17,    -1,    19,    -1,    21,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,    -1,
      -1,    -1,    38,    39,    40,    -1,    42,    43,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    -1,
      56,    57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    71,    -1,    73,    -1,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    89,    90,    91,    92,    -1,    94,    -1,
      -1,    97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,
     156,    -1,    -1,   159,    -1,   161,    -1,   163,    -1,   165,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      13,    -1,    -1,    16,    17,    -1,    19,    -1,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    34,    -1,    -1,    -1,    38,    39,    40,    -1,    42,
      43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      53,    -1,    -1,    56,    57,    -1,    59,    60,    61,    62,
      63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    -1,
      73,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      -1,    94,    -1,    -1,    97,    98,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,
     153,   154,   155,    -1,    -1,    -1,   159,    -1,   161,    -1,
     163,    -1,   165,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    13,    -1,    -1,    16,    17,    -1,    19,
      -1,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    34,    -1,    -1,    -1,    38,    39,
      40,    -1,    42,    43,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    53,    -1,    -1,    56,    57,    -1,    59,
      60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    71,    -1,    73,    -1,    -1,    -1,    -1,    78,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,
      90,    91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,
     100,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,    -1,    -1,   153,   154,   155,    -1,    -1,    -1,   159,
      -1,   161,    -1,   163,    -1,   165,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,    16,
      17,    -1,    19,    -1,    21,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    34,    -1,    -1,
      -1,    38,    39,    40,    -1,    42,    43,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    53,    -1,    -1,    56,
      57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    89,    90,    91,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,   113,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,    -1,
      -1,    -1,   159,    -1,   161,    -1,   163,    -1,   165,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    -1,    13,
      -1,    -1,    16,    17,    -1,    19,    -1,    21,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      34,    -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    -1,    56,    57,    -1,    59,    60,    -1,    62,    63,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    -1,    73,
      -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    89,    90,    91,    92,    -1,
      94,    -1,    -1,    97,    98,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,   153,
     154,   155,    -1,    -1,    -1,   159,    -1,   161,    -1,   163,
      -1,   165,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    -1,    -1,    16,    17,    -1,    19,    -1,
      21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    34,    -1,    -1,    -1,    38,    39,    40,
      -1,    42,    43,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    -1,    56,    57,    -1,    59,    60,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      71,    -1,    73,    -1,    -1,    -1,    -1,    78,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      91,    92,    -1,    94,    -1,    -1,    97,    98,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,
      -1,    -1,   153,   154,   155,     1,    -1,     3,   159,    -1,
     161,    -1,   163,    -1,   165,    11,    -1,    -1,    -1,    -1,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    24,    -1,
      21,    -1,    -1,    -1,    25,    26,    32,    -1,    -1,    -1,
      -1,    -1,    38,    34,    -1,    36,    37,    43,    -1,    45,
      41,    -1,    48,    -1,    50,    46,    -1,    -1,    49,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    62,    63,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    68,    -1,    70,
      -1,    -1,    78,    -1,    75,    -1,    -1,    -1,    79,    -1,
      81,    87,    88,    -1,    90,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,   101,    -1,    -1,   104,   100,
      -1,   107,   103,    -1,   105,   106,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,   121,    -1,    -1,   124,   125,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
      -1,    -1,   138,    -1,    -1,    -1,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,   154,    -1,
      -1,   157,   158,   159,   160,    -1,    -1,   163,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,
      -1,    16,    17,    -1,    19,    -1,    21,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,
      -1,    56,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    71,    -1,    73,    -1,
      -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    91,    92,    -1,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,   113,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,
     155,    -1,    -1,    -1,   159,    -1,   161,    -1,   163,    -1,
     165,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      -1,    13,    -1,    -1,    16,    17,    -1,    19,    -1,    21,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    34,    -1,    -1,    -1,    38,    39,    40,    -1,
      42,    43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    53,    -1,    -1,    56,    57,    -1,    59,    60,    -1,
      62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,
      -1,    73,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,
      -1,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,
      -1,   153,   154,   155,    -1,    -1,    -1,   159,    -1,   161,
      -1,   163,    -1,   165,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,    -1,
      19,    -1,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    34,    -1,    -1,    -1,    38,
      39,    40,    -1,    42,    43,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    -1,    56,    57,    -1,
      59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    71,    -1,    73,    -1,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    91,    92,    -1,    94,    -1,    -1,    97,    -1,
      -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,   113,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,    -1,   153,   154,   155,    -1,    -1,    -1,
     159,    -1,   161,    -1,   163,    -1,   165,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,
      16,    17,    -1,    19,    -1,    21,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,    -1,
      -1,    -1,    38,    39,    40,    -1,    42,    43,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    -1,
      56,    57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    71,    -1,    73,    -1,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    89,    90,    91,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,   100,   101,    -1,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,
      -1,    -1,    -1,   159,    -1,   161,    -1,   163,    -1,   165,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      13,    -1,    -1,    16,    17,    -1,    19,    -1,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    34,    -1,    -1,    -1,    38,    39,    40,    -1,    42,
      43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      53,    -1,    -1,    56,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    -1,
      73,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,
     153,   154,   155,    -1,    -1,    -1,   159,    -1,   161,   162,
     163,    -1,   165,     3,     4,     5,     6,     7,     8,     9,
      -1,    -1,    -1,    13,    -1,    -1,    16,    17,    -1,    19,
      -1,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    34,    -1,    -1,    -1,    38,    39,
      40,    -1,    42,    43,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    53,    -1,    -1,    56,    57,    -1,    59,
      60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    71,    -1,    73,    -1,    -1,    -1,    -1,    78,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,
      90,    91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,    -1,    -1,   153,   154,   155,    -1,    -1,    -1,   159,
      -1,   161,    -1,   163,    -1,   165,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,    16,
      17,    -1,    19,    -1,    21,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    34,    -1,    -1,
      -1,    38,    39,    40,    -1,    42,    43,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    53,    -1,    -1,    56,
      57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    71,    -1,    73,    -1,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    89,    90,    91,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,    -1,
      -1,    -1,   159,    -1,   161,    -1,   163,    -1,   165,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    -1,    13,
      -1,    -1,    16,    17,    -1,    19,    -1,    21,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      34,    -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    -1,    56,    57,    -1,    59,    60,    -1,    62,    63,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    -1,    73,
      -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    89,    90,    91,    92,    -1,
      94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,   153,
     154,   155,    -1,    -1,    -1,   159,    -1,   161,    -1,   163,
      -1,   165,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    -1,    -1,    16,    17,    -1,    19,    -1,
      21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    34,    -1,    -1,    -1,    38,    39,    40,
      -1,    42,    43,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    -1,    56,    57,    -1,    59,    60,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      71,    -1,    73,    -1,    -1,    -1,    -1,    78,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
     101,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,
      -1,    -1,   153,   154,   155,    -1,    -1,    -1,   159,    -1,
     161,    -1,   163,    -1,   165,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,
      -1,    19,    -1,    21,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    34,    -1,    -1,    -1,
      38,    39,    40,    -1,    42,    43,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    53,    -1,    -1,    56,    57,
      -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    71,    -1,    73,    -1,    -1,    -1,    -1,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    -1,    94,    -1,    -1,    97,
      -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,    -1,   153,   154,   155,    -1,    -1,
      -1,   159,    -1,   161,    -1,   163,    -1,   165,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,
      -1,    16,    17,    -1,    19,    -1,    21,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,
      -1,    56,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    71,    -1,    73,    -1,
      -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    91,    92,    -1,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,
     155,    -1,    -1,    -1,   159,    -1,   161,    -1,   163,    -1,
     165,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      -1,    13,    -1,    -1,    16,    17,    -1,    19,    -1,    21,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    34,    -1,    -1,    -1,    38,    39,    40,    -1,
      42,    43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    53,    -1,    -1,    56,    57,    -1,    59,    60,    -1,
      62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,
      -1,    73,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,
      -1,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,
      -1,   153,   154,   155,    -1,    -1,    -1,   159,    -1,   161,
      -1,   163,    -1,   165,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,    -1,
      19,    -1,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    34,    -1,    -1,    -1,    38,
      39,    40,    -1,    42,    43,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    -1,    56,    57,    -1,
      59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    71,    -1,    73,    -1,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    91,    92,    -1,    94,    -1,    -1,    97,    -1,
      -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,    -1,   153,   154,   155,    -1,    -1,    -1,
     159,    -1,   161,    -1,   163,    -1,   165,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,
      16,    17,    -1,    19,    -1,    21,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,    -1,
      -1,    -1,    38,    39,    40,    -1,    42,    43,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    -1,
      56,    57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    71,    -1,    73,    -1,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    89,    90,    91,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,
      -1,    -1,    -1,   159,    -1,   161,    -1,   163,    -1,   165,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      13,    -1,    -1,    16,    17,    -1,    19,    -1,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    34,    -1,    -1,    -1,    38,    39,    40,    -1,    42,
      43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      53,    -1,    -1,    56,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    -1,
      73,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,
     153,   154,   155,    -1,    -1,    -1,   159,    -1,   161,    -1,
     163,    -1,   165,     3,     4,     5,     6,     7,     8,     9,
      -1,    -1,    -1,    13,    -1,    -1,    16,    17,    -1,    19,
      -1,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    34,    -1,    -1,    -1,    38,    39,
      40,    -1,    42,    43,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    53,    -1,    -1,    56,    57,    -1,    59,
      60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    71,    -1,    73,    -1,    -1,    -1,    -1,    78,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,
      90,    91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,    -1,    -1,   153,   154,   155,    -1,    -1,    -1,   159,
      -1,   161,    -1,   163,    -1,   165,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,    16,
      17,    -1,    19,    -1,    21,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    34,    -1,    -1,
      -1,    38,    39,    40,    -1,    42,    43,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    53,    -1,    -1,    56,
      57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    71,    -1,    73,    -1,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    89,    90,    91,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,    -1,
      -1,    -1,   159,    -1,   161,    -1,   163,    -1,   165,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    -1,    13,
      -1,    -1,    16,    17,    -1,    19,    -1,    21,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      34,    -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    -1,    56,    57,    -1,    59,    60,    -1,    62,    63,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    -1,    73,
      -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    89,    90,    91,    92,    -1,
      94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,   153,
     154,   155,    -1,    -1,    -1,   159,    -1,   161,    -1,   163,
      -1,   165,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    -1,    -1,    16,    17,    -1,    19,    -1,
      21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    34,    -1,    -1,    -1,    38,    39,    40,
      -1,    42,    43,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    -1,    56,    57,    -1,    59,    60,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      71,    -1,    73,    -1,    -1,    -1,    -1,    78,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
     101,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,
      -1,    -1,   153,   154,   155,    -1,    -1,    -1,   159,    -1,
     161,    -1,   163,    -1,   165,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,
      -1,    19,    -1,    21,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    34,    -1,    -1,    -1,
      38,    39,    40,    -1,    42,    43,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    53,    -1,    -1,    56,    57,
      -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    71,    -1,    73,    -1,    -1,    -1,    -1,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    -1,    94,    -1,    -1,    97,
      -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,    -1,   153,   154,   155,    -1,    -1,
      -1,   159,    -1,   161,    -1,   163,    -1,   165,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,
      -1,    16,    17,    -1,    19,    -1,    21,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,
      -1,    56,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    71,    -1,    73,    -1,
      -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    91,    92,    -1,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,
     155,    -1,    -1,    -1,   159,    -1,   161,    -1,   163,    -1,
     165,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      -1,    13,    -1,    -1,    16,    17,    -1,    19,    -1,    21,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    34,    -1,    -1,    -1,    38,    39,    40,    -1,
      42,    43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    53,    -1,    -1,    56,    57,    -1,    59,    60,    -1,
      62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,
      -1,    73,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,
      -1,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,
      -1,   153,   154,   155,    -1,    -1,    -1,   159,    -1,   161,
      -1,   163,    -1,   165,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,    -1,
      19,    -1,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    34,    -1,    -1,    -1,    38,
      39,    40,    -1,    42,    43,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    -1,    56,    57,    -1,
      59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    71,    -1,    73,    -1,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    91,    92,    -1,    94,    -1,    -1,    97,    -1,
      -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,    -1,   153,   154,   155,    -1,    -1,    -1,
     159,    -1,   161,    -1,   163,    -1,   165,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,
      16,    17,    -1,    19,    -1,    21,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,    -1,
      -1,    -1,    38,    39,    40,    -1,    42,    43,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    -1,
      56,    57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    71,    -1,    73,    -1,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    89,    90,    91,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,
      -1,    -1,    -1,   159,    -1,   161,    -1,   163,    -1,   165,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      13,    -1,    -1,    16,    17,    -1,    19,    -1,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    34,    -1,    -1,    -1,    38,    39,    40,    -1,    42,
      43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      53,    -1,    -1,    56,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    -1,
      73,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,
     153,   154,   155,    -1,    -1,    -1,   159,    -1,    -1,    -1,
     163,    -1,   165,     3,     4,     5,     6,     7,     8,     9,
      -1,    -1,    -1,    13,    -1,    -1,    16,    17,    -1,    19,
      -1,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    34,    -1,    -1,    -1,    38,    39,
      40,    -1,    42,    43,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    53,    -1,    -1,    56,    57,    -1,    59,
      60,    21,    62,    63,    -1,    25,    26,    -1,    -1,    69,
      -1,    71,    -1,    73,    34,    -1,    36,    37,    78,    79,
      -1,    41,    -1,    -1,    -1,    -1,    46,    87,    88,    89,
      90,    91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,    68,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
     100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     150,    -1,    -1,    -1,   154,   155,    -1,    -1,    -1,   159,
      -1,   161,    -1,   163,    -1,   165,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,    16,
      17,    -1,    19,    -1,    21,    30,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    34,    -1,    -1,
      -1,    38,    39,    40,    -1,    42,    43,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    53,    54,    -1,    56,
      57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    71,    80,    73,    -1,    -1,    84,
      -1,    78,    79,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      87,    88,    89,    90,    91,    92,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,
     107,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,   133,   142,   143,   144,
     145,    -1,    -1,   148,   149,   150,   151,   152,    -1,   154,
      -1,    -1,   157,   158,   159,    -1,    -1,    -1,   155,   156,
      -1,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    -1,    13,
      -1,    -1,    16,    17,    -1,    19,    -1,    21,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      34,    -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    -1,    56,    57,    -1,    59,    60,    -1,    62,    63,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    -1,    73,
      -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    89,    90,    91,    92,    -1,
      94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,
     164,   165,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    -1,    -1,    16,    17,    -1,    19,    -1,
      21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    34,    -1,    -1,    -1,    38,    39,    40,
      -1,    42,    43,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    -1,    56,    57,    -1,    59,    60,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      71,    -1,    73,    -1,    -1,    -1,    -1,    78,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
     101,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,    -1,    -1,
     131,    -1,   133,    -1,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,   147,    13,    -1,    -1,    16,    17,    -1,
      19,    -1,    21,    -1,   155,    24,   160,    -1,    -1,   163,
     161,   165,   163,    32,   165,    34,    -1,    -1,    -1,    38,
      39,    40,    -1,    42,    43,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    -1,    56,    57,    -1,
      59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    71,    -1,    73,    -1,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    91,    92,    -1,    94,    -1,    -1,    97,    -1,
      -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,    -1,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,    16,
      17,    -1,    19,    -1,    21,    -1,   155,    24,    -1,    -1,
      -1,    -1,   161,    -1,   163,    32,   165,    34,    -1,    -1,
      -1,    38,    39,    40,    -1,    42,    43,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    53,    -1,    -1,    56,
      57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    71,    -1,    73,    -1,    -1,    -1,
      -1,    78,    79,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      87,    88,    89,    90,    91,    92,    -1,    94,    16,    17,
      97,    19,    -1,    -1,   101,    -1,    24,   104,    -1,    -1,
     107,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,   133,    -1,    56,    -1,
      -1,    -1,     3,    -1,    62,    63,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    16,    17,    -1,    19,    -1,
      78,    -1,    -1,    24,    -1,    -1,   163,    -1,   165,    87,
      88,    32,    90,    -1,    92,    -1,    94,    38,    -1,    97,
      -1,    -1,    43,   101,    45,    -1,   104,    48,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    90,
      -1,    92,    -1,    94,     3,    -1,    97,    -1,    -1,    -1,
     101,    -1,    11,   104,    -1,   163,   107,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    45,    -1,    -1,    48,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    62,    63,    -1,    -1,    -1,    11,    -1,
      69,    -1,   163,    -1,    -1,    18,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    -1,    -1,    97,    -1,
      -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,   121,    -1,    -1,   124,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,    80,    -1,   138,
      -1,    84,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,   154,    11,    -1,   157,   158,
     159,   160,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
      -1,   134,   135,   136,    -1,   138,   139,   140,    -1,   142,
     143,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,    -1,    11,   157,   158,   159,    -1,    -1,    -1,
      18,    -1,    -1,   166,    -1,    80,    -1,    -1,    -1,    84,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,
     115,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    80,   138,   139,   140,    84,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
      11,    -1,   157,   158,   159,    -1,    -1,    18,    -1,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,   115,    -1,    30,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,    -1,   115,   157,
     158,   159,    -1,    -1,    -1,    -1,    -1,    -1,   166,    80,
      -1,    -1,    -1,    84,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    11,    -1,
     157,   158,   159,    -1,   115,    18,    -1,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
     131,   132,    -1,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,    -1,    -1,   157,   158,   159,    -1,
      -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    84,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      93,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,   120,   121,    -1,
      -1,   124,   125,   126,   127,    -1,   129,   130,   131,   132,
      -1,   134,   135,   136,    -1,   138,   139,   140,    -1,   142,
     143,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,    -1,    80,   157,   158,   159,    84,   161,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    93,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
     117,   118,   119,   120,   121,    -1,    -1,   124,   125,   126,
     127,    -1,   129,   130,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    -1,    80,
     157,   158,   159,    84,   161,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    93,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,   120,
     121,    -1,    -1,   124,   125,   126,   127,    -1,   129,   130,
     131,   132,    -1,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,    -1,    80,   157,   158,   159,    84,
     161,    -1,    11,    -1,    -1,    -1,    -1,    -1,    93,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,   117,   118,   119,   120,   121,    -1,    -1,   124,
     125,   126,   127,    -1,   129,   130,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
      -1,    80,   157,   158,   159,    84,   161,    -1,    11,    -1,
      -1,    -1,    -1,    -1,    93,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,
     119,   120,   121,    -1,    -1,   124,   125,   126,   127,    -1,
     129,   130,   131,   132,    -1,   134,   135,   136,    -1,   138,
     139,   140,    -1,   142,   143,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,    -1,    80,   157,   158,
     159,    84,   161,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      93,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,   120,   121,    -1,
      -1,   124,   125,   126,   127,    -1,   129,   130,   131,   132,
      -1,   134,   135,   136,    -1,   138,   139,   140,    -1,   142,
     143,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,    -1,    80,   157,   158,   159,    84,   161,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    93,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
     117,   118,   119,   120,   121,    -1,    -1,   124,   125,   126,
     127,    -1,   129,   130,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    -1,    80,
     157,   158,   159,    84,   161,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    93,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,   120,
     121,    -1,    -1,   124,   125,   126,   127,    -1,   129,   130,
     131,   132,    -1,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,    -1,    80,   157,   158,   159,    84,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,   117,   118,   119,   120,   121,    -1,    -1,   124,
     125,   126,   127,    -1,   129,   130,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
      11,    -1,   157,   158,   159,    -1,   161,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    44,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
     111,    80,    -1,    -1,   115,    84,    -1,    -1,    30,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,   132,    -1,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,   115,   148,   149,   150,
     151,   152,   153,   154,    -1,    -1,   157,   158,   159,    -1,
     161,    -1,   131,   132,    -1,   134,   135,   136,    80,   138,
     139,   140,    84,   142,   143,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,    11,    -1,   157,   158,
     159,    -1,   161,    18,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,   115,    -1,    30,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,    -1,   134,   135,   136,    -1,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,    11,    -1,   157,   158,   159,    -1,   161,
      18,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,
      -1,    -1,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    80,   138,   139,   140,    84,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
      -1,    -1,   157,   158,   159,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,   115,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,   115,
     148,   149,   150,   151,   152,   153,   154,    -1,    -1,   157,
     158,   159,    -1,   161,    -1,   131,   132,    -1,   134,   135,
     136,    -1,   138,   139,   140,    -1,   142,   143,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,    11,
      -1,   157,   158,   159,    -1,   161,    18,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      16,    17,    -1,    19,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    11,    -1,    -1,    -1,    -1,    32,    -1,    18,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    45,
      30,    -1,    48,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    62,    63,    80,    -1,
      -1,    -1,    84,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    65,    -1,    -1,    11,    -1,
      -1,    87,    88,    -1,    90,    18,    92,    -1,    94,    -1,
      80,    97,    -1,   115,    84,   101,    -1,    30,   104,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,    44,   134,   135,   136,    -1,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,   115,   148,   149,   150,   151,
     152,   153,   154,    -1,    -1,   157,   158,   159,    -1,   161,
      -1,   131,   132,    -1,   134,   135,   136,    80,   138,   139,
     140,    84,   142,   143,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,    11,   156,   157,   158,   159,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,   115,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
      -1,   134,   135,   136,    -1,   138,   139,   140,    -1,   142,
     143,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,    11,    -1,   157,   158,   159,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,
      -1,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    11,    44,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    30,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,    44,   134,   135,
     136,    80,   138,   139,   140,    84,   142,   143,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,   115,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,   138,
     139,   140,    -1,   142,   143,   144,   145,   146,   115,   148,
     149,   150,   151,   152,   153,   154,    -1,    -1,   157,   158,
     159,    -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    11,    21,
     157,   158,   159,    25,    26,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    37,    -1,    30,    -1,    41,
      -1,    -1,    35,    -1,    46,    -1,    -1,    49,    -1,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    30,
      -1,    -1,    65,    75,    -1,    -1,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,   100,    11,
      -1,   103,    -1,   105,   106,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    80,
      -1,    -1,   115,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
      -1,   134,   135,   136,    -1,   138,   139,   140,    -1,   142,
     143,   144,   145,   146,   115,   148,   149,   150,   151,   152,
     153,   154,    -1,    -1,   157,   158,   159,    -1,    80,    -1,
     131,   132,    84,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,    11,   156,   157,   158,   159,    -1,
      -1,    18,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,   131,
     132,    -1,   134,   135,   136,    -1,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,    11,   156,   157,   158,   159,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    80,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,   131,   132,    84,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,    -1,    80,    -1,   114,   115,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,   115,
     148,   149,   150,   151,   152,   153,   154,    -1,    -1,   157,
     158,   159,    -1,    -1,    -1,   131,   132,    -1,   134,   135,
     136,    -1,   138,   139,   140,    -1,   142,   143,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,    11,
      -1,   157,   158,   159,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      21,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    37,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    11,    46,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    68,    80,    70,
      -1,    72,    84,    74,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,   100,
      -1,    -1,   103,   115,    30,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,   131,
     132,    -1,   134,   135,   136,    -1,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,    -1,   111,   157,   158,   159,   115,    -1,
      -1,    -1,    -1,    -1,    80,   156,    -1,    -1,    84,    -1,
      -1,   162,    -1,    -1,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    -1,   115,
     157,   158,   159,    -1,    -1,    -1,    11,    12,    -1,    -1,
      -1,    -1,    -1,    18,    -1,   131,   132,    -1,   134,   135,
     136,    -1,   138,   139,   140,    30,   142,   143,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,    -1,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     115,    -1,   157,   158,   159,    -1,    80,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     114,   115,   157,   158,   159,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,
     134,   135,   136,    -1,   138,   139,   140,    -1,   142,   143,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,    11,    -1,   157,   158,   159,    -1,     1,    18,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    31,    16,    17,    -1,    19,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    11,    -1,    -1,    -1,    -1,    32,    -1,
      18,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    45,    30,    31,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    62,    63,
      80,    -1,    -1,    -1,    84,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    -1,    80,    97,    -1,   115,    84,   101,    -1,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,    -1,   134,   135,   136,    -1,   138,   139,
     140,    -1,   142,   143,   144,   145,   146,   115,   148,   149,
     150,   151,   152,   153,   154,    -1,    -1,   157,   158,   159,
      -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,    11,    -1,   157,
     158,   159,    -1,    -1,    18,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    16,    17,
      -1,    19,    -1,    -1,    -1,    -1,    24,    -1,    -1,    11,
      -1,    -1,    -1,    -1,    32,    -1,    18,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    45,    30,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    62,    63,    80,    -1,    -1,    -1,
      84,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      78,    -1,    -1,    -1,    -1,    -1,    11,    12,    -1,    87,
      88,    -1,    90,    18,    92,    -1,    94,    -1,    80,    97,
      -1,   115,    84,   101,    -1,    30,   104,    -1,    -1,   107,
      -1,    93,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,
     134,   135,   136,    -1,   138,   139,   140,    -1,   142,   143,
     144,   145,   146,   115,   148,   149,   150,   151,   152,   153,
     154,    -1,    -1,   157,   158,   159,    -1,    -1,    -1,   131,
     132,    -1,   134,   135,   136,    80,   138,   139,   140,    84,
     142,   143,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,    11,    -1,   157,   158,   159,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
      -1,    -1,   157,   158,   159,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    93,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,    -1,    80,   157,
     158,   159,    84,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    93,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,    -1,   134,   135,   136,    -1,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,    -1,    80,   157,   158,   159,    84,    -1,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    93,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,   135,
     136,    -1,   138,   139,   140,    -1,   142,   143,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,    -1,
      80,   157,   158,   159,    84,    -1,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    93,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,    -1,   134,   135,   136,    -1,   138,   139,
     140,    -1,   142,   143,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,    -1,    80,   157,   158,   159,
      84,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    93,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,
     134,   135,   136,    -1,   138,   139,   140,    -1,   142,   143,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,    -1,    80,   157,   158,   159,    84,    -1,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    93,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,    -1,    80,   157,
     158,   159,    84,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,    -1,   134,   135,   136,    -1,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,    -1,    80,   157,   158,   159,    84,    -1,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,   135,
     136,    -1,   138,   139,   140,    -1,   142,   143,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,    -1,
      80,   157,   158,   159,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,    -1,   134,   135,   136,    -1,   138,   139,
     140,    30,   142,   143,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,    -1,    21,   157,   158,   159,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    37,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,    74,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,
      -1,   106,   131,   132,    -1,   134,   135,   136,    -1,   138,
     139,   140,    -1,   142,   143,   144,   145,    -1,     3,   148,
     149,   150,   151,   152,   153,   154,    -1,    -1,   157,   158,
     159,    16,    17,    -1,    19,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,   156,    -1,    38,    -1,    -1,    -1,   162,    43,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    -1,     3,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    16,    17,
      -1,    19,    -1,    78,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    87,    88,    32,    90,    -1,    92,    -1,    94,
      38,    -1,    97,    -1,    -1,    43,   101,    45,    -1,   104,
      48,    -1,   107,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    -1,     3,    -1,    62,    63,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    16,    17,    -1,    19,    -1,
      78,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    87,
      88,    32,    90,    -1,    92,    -1,    94,    38,    -1,    97,
      -1,    -1,    43,   101,    45,    -1,   104,    48,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,     3,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    16,    17,    -1,    19,    -1,    78,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    87,    88,    32,    90,
      -1,    92,    -1,    94,    38,    -1,    97,    -1,    -1,    43,
     101,    45,    -1,   104,    48,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    62,    63,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,
     104,    -1,    -1,   107
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   172,   173,     0,     1,     3,     4,     5,     6,     7,
       8,     9,    13,    14,    15,    16,    17,    19,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    31,    32,    34,
      36,    37,    38,    39,    40,    41,    42,    43,    45,    46,
      48,    51,    52,    53,    55,    56,    57,    59,    60,    62,
      63,    64,    68,    69,    70,    71,    72,    73,    74,    77,
      78,    79,    81,    82,    83,    85,    86,    87,    88,    89,
      90,    91,    92,    94,    95,    97,    98,    99,   100,   101,
     103,   104,   106,   107,   110,   112,   115,   131,   132,   133,
     134,   135,   140,   150,   153,   154,   155,   156,   159,   161,
     163,   165,   174,   175,   176,   177,   178,   179,   181,   186,
     187,   188,   189,   193,   195,   198,   199,   200,   201,   202,
     207,   208,   210,   211,   212,   216,   217,   220,   221,   224,
     225,   228,   231,   232,   257,   260,   261,   281,   282,   283,
     284,   285,   286,   287,   295,   296,   297,   298,   301,   302,
     303,   304,   305,   306,   307,   309,   310,   311,   312,   313,
     156,   176,   298,   111,   288,   289,     3,   190,    13,    21,
      34,    39,    40,    42,    79,    92,   161,   165,   228,   281,
     286,   296,   297,   298,   301,   303,   304,   288,   298,   100,
     260,    81,   190,   176,   275,   298,   176,   163,     7,    79,
     298,   299,     7,    10,    79,   100,   299,    70,   113,   209,
     298,   209,   298,    25,   261,   298,   298,   163,     3,    16,
      17,    19,    24,    32,    38,    43,    45,    48,    56,    62,
      63,    69,    78,    87,    88,    90,    92,    94,    97,   101,
     104,   107,   192,   194,   229,   163,   192,   262,   263,    31,
     181,   197,   298,   298,   298,   298,   298,   298,     7,   163,
      21,    34,    37,    79,   200,   200,   155,   251,   274,   298,
      98,    99,   156,   298,   300,   298,   197,   298,   298,   298,
      91,   163,   176,   298,   298,   181,   189,   298,   301,   181,
     189,   298,   192,   258,   298,   298,   298,   298,   298,   298,
     298,   298,     1,   162,   174,   182,   274,   102,   141,   251,
     276,   277,   300,   209,   274,   298,   308,    72,   176,   161,
      76,   180,   105,   192,   192,    49,    75,   246,   262,   155,
     156,   147,    11,    18,    30,    80,    84,   115,   131,   132,
     134,   135,   136,   138,   139,   140,   142,   143,   144,   145,
     146,   148,   149,   150,   151,   152,   153,   154,   157,   158,
     159,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   160,   238,   163,   165,
      80,    84,   163,   176,   156,   298,   298,   298,   274,   161,
     181,    44,   288,   258,   262,   156,   137,   156,   110,   193,
     251,   278,   279,   280,   300,    79,   205,   232,   260,    79,
     203,   258,   232,   260,   192,   163,   197,    31,    44,   197,
     111,   197,   291,    31,    44,   197,   291,    35,    65,   156,
      93,   181,   238,   278,   176,   163,   240,   102,   163,   192,
     264,   265,     1,   136,   269,    44,   137,   176,   197,   197,
     278,   192,   137,   156,   298,   298,   156,   161,   197,   163,
     278,   156,   213,   156,   213,   156,   116,   259,   156,   197,
     156,   162,   162,   174,   137,   162,   298,   137,   164,   137,
     164,   166,   291,    44,   137,   166,   291,   114,   137,   166,
       7,     1,   162,   182,    58,   274,   298,   136,   222,   161,
     233,   156,   298,   298,   298,   298,   209,   298,   209,   298,
     298,   298,   298,   298,   298,   298,    19,    32,    56,   100,
     193,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   300,   300,   300,
     300,   300,   278,   278,   209,   298,   209,   298,    26,    44,
      81,   106,   290,   293,   294,   298,   313,    31,    44,    31,
      44,    93,    44,   166,   209,   298,   197,   156,   156,   298,
     298,   116,   164,   137,   206,   192,   262,   204,   192,   156,
     262,    44,   274,    42,   298,   209,   298,   163,   197,    42,
     298,   209,   298,   197,   159,   183,   185,   298,   183,   184,
     176,   298,    33,   298,   164,    26,    44,    47,    67,    70,
      81,   100,   175,   241,   242,   243,   244,   230,   265,   137,
     164,    32,    45,    88,    92,   165,   196,   270,   282,   116,
     266,   298,   263,   164,   251,   298,     1,   218,   278,   164,
      20,   214,   270,   282,   137,   162,   164,   164,   276,   164,
     276,   176,   166,   209,   298,   166,   176,   298,   166,   298,
     166,   298,   162,   162,   192,   156,    35,    65,   274,   161,
       1,   192,   226,   227,    26,    70,    81,   100,   235,   245,
     163,   156,   156,   156,   156,   156,   164,   166,    44,    81,
     137,   164,   281,    80,    80,    42,   209,   298,    42,   209,
     298,   209,   298,   288,   288,   156,   251,   300,   280,   192,
     222,   156,   192,   222,   156,   298,   164,   298,    31,   197,
      31,   197,   292,   293,   298,    31,   197,   291,    31,   197,
     291,   137,   156,    12,   156,    33,    33,   176,    93,   181,
      44,    81,   243,   137,   164,   163,   192,    26,    70,    81,
     100,   247,   164,   265,   269,     1,   274,    61,   300,   162,
      66,   108,   162,   219,   164,   163,   181,   192,   215,   258,
     176,   166,   291,   166,   291,   176,   114,   185,   184,     1,
     223,   162,   116,   137,   162,    81,   234,     1,     3,    11,
      16,    18,    19,    24,    38,    43,    48,    50,    56,    62,
      63,    78,    90,    94,    97,   101,   107,   131,   132,   133,
     134,   135,   138,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   154,   157,   158,   159,   160,   163,
     191,   192,   194,   236,   237,   238,   281,   164,   293,   269,
     281,   281,   298,    31,    31,   298,    31,    31,   166,   166,
     197,   197,   222,   161,   222,   161,   197,    93,    42,   298,
      42,   298,   137,   164,    93,    42,   298,   197,    42,   298,
     197,   298,   298,   176,   298,   176,    33,   192,   242,   265,
     136,   273,    81,   269,   266,   166,    44,   166,    31,   176,
     274,   215,   136,   181,    42,   176,   298,   166,    42,   176,
     298,   166,   298,   156,   156,   162,    74,    77,   162,   175,
     199,   298,   227,   247,   163,   239,   192,   298,   131,   139,
     239,   239,   266,    93,    42,    42,    93,    42,    42,    42,
      42,   161,   223,   161,   223,   298,   298,   298,   293,   298,
     298,   298,    12,    33,   176,   273,   164,   165,   196,   251,
     272,   282,   141,   252,   266,    54,   109,   253,   298,   270,
     282,   176,   197,   164,   298,   298,   176,   298,   176,   199,
       1,   136,   268,   241,   164,     3,    94,   237,   238,   298,
     298,   298,   298,   298,   298,   223,   162,   223,   162,    93,
      93,    93,    93,   298,   176,   266,   273,   166,   274,   251,
     298,     3,    83,    94,   254,   255,   256,   298,   181,   198,
     250,   166,   181,    93,    93,   165,   196,   267,   282,    96,
     248,   164,   239,   239,    93,    93,    93,    93,    93,    93,
     162,   162,   298,   298,   298,   298,   252,   266,   251,   271,
     272,   282,    44,   166,   256,   109,   137,   116,   142,   144,
     145,   148,   149,    54,   282,   298,   298,     1,   166,   274,
     253,   298,   271,   272,   298,   255,   256,   256,   256,   256,
     256,   256,   254,   166,   267,   282,   166,   156,   249,   250,
     166,   267,   282,   271
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
#line 493 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 498 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 499 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 511 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 512 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 532 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 533 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 534 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 535 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 540 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 542 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 543 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 544 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 545 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 546 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 551 "chapel.ypp"
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
#line 564 "chapel.ypp"
    { (yyvsp[(1) - (3)].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (3)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (3)]).prevModule;  // restore previous module name
    }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 569 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = (yyvsp[(3) - (4)].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 574 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 583 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 588 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 601 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 602 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 603 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 609 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 619 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 624 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 628 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 635 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 644 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 653 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 654 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 655 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 659 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (4)].pcallexpr), (yyvsp[(1) - (4)].b)); }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 660 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), (yyvsp[(5) - (6)].ponlylist), true, (yyvsp[(1) - (6)].b)); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 661 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), (yyvsp[(5) - (6)].ponlylist), false, (yyvsp[(1) - (6)].b)); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 665 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 670 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 672 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 674 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 676 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 678 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 684 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 685 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 690 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 694 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 698 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 702 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 714 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 715 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 726 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 727 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 728 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 729 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 730 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 731 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 732 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 733 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 734 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 737 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 738 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 739 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 740 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 742 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 743 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 744 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 748 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 749 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 758 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 765 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 766 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 767 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 768 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 772 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 773 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 777 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 778 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 782 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 784 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 785 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 799 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 800 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 801 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 802 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 807 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 812 "chapel.ypp"
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

  case 144:
/* Line 1792 of yacc.c  */
#line 822 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 827 "chapel.ypp"
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

  case 146:
/* Line 1792 of yacc.c  */
#line 838 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 843 "chapel.ypp"
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

  case 148:
/* Line 1792 of yacc.c  */
#line 853 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 858 "chapel.ypp"
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

  case 150:
/* Line 1792 of yacc.c  */
#line 869 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 873 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 878 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 886 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 897 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 903 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 904 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 905 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 906 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 907 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 908 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 909 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 910 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 911 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 912 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 913 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 914 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 915 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 916 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 917 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 918 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 919 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 920 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 922 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 928 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 934 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 940 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 946 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 953 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 960 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 964 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 970 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 974 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 975 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 976 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 977 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 978 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 980 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 982 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 984 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 989 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 992 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 993 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 994 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 995 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 996 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 997 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1001 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1002 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1006 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1007 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1008 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1012 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1013 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1017 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1023 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1028 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1033 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1035 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1037 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1044 "chapel.ypp"
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

  case 213:
/* Line 1792 of yacc.c  */
#line 1054 "chapel.ypp"
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

  case 214:
/* Line 1792 of yacc.c  */
#line 1067 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1072 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1077 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1085 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1086 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1091 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1100 "chapel.ypp"
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

  case 223:
/* Line 1792 of yacc.c  */
#line 1116 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1123 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1132 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1140 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1144 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1150 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1151 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1156 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1161 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1166 "chapel.ypp"
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

  case 233:
/* Line 1792 of yacc.c  */
#line 1188 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1191 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1195 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1203 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1213 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1220 "chapel.ypp"
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

  case 239:
/* Line 1792 of yacc.c  */
#line 1246 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1250 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1255 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1259 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1264 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1271 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1276 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1277 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1283 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1284 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1286 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1288 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1298 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1307 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1308 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1309 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1312 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1313 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1314 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1317 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1321 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1322 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1326 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1330 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1331 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1332 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1337 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1341 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1343 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1345 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1349 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1350 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1354 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1356 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1357 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1358 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1366 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1367 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1370 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1375 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1376 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1380 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1381 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1382 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1383 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1384 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1389 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1390 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1393 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1399 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1404 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1408 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1409 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1410 "chapel.ypp"
    { if (DefExpr* def = toDefExpr((yyvsp[(2) - (2)].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
                         }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1418 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1420 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1422 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1424 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1426 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1431 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1437 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1438 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1439 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1440 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1441 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1442 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1443 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1448 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1450 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1455 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1457 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1459 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1464 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
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

  case 353:
/* Line 1792 of yacc.c  */
#line 1493 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1497 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1501 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1510 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1516 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1525 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1542 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1544 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1553 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1559 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1560 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1561 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1569 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1571 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1575 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1579 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1585 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1592 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1593 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1594 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1601 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1602 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1604 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1605 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1626 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1630 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1634 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1642 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1648 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1649 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1650 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1657 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1663 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1665 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1667 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1671 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1672 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1673 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1674 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1681 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1682 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1683 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1684 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1688 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1693 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1694 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1695 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1699 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1704 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1709 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1710 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1715 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1721 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1722 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1734 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1736 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1738 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1749 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1751 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1753 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1755 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1757 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1759 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1761 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1764 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1766 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1768 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1770 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1772 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1776 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1778 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1781 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1783 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1785 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1790 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1792 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1794 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1796 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1798 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1800 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1802 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1804 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1806 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1808 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1810 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1812 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1814 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1821 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1827 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1833 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1839 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1848 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1857 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1873 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1877 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1882 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1886 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1887 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1891 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1895 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1896 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1901 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1905 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1909 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1915 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1916 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1917 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1918 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1919 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1920 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1926 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1931 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1936 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1952 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1954 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1956 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1958 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1960 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1962 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1966 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1967 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 1971 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 1972 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 523:
/* Line 1792 of yacc.c  */
#line 1973 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 1991 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 1992 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 1993 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr));}
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 1994 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 1998 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 1999 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 2000 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 2004 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 2005 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 2006 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 2007 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 2008 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 2016 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 2017 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 2018 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 2019 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 2023 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 2024 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 2029 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 2030 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 2031 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 2032 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 2033 "chapel.ypp"
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 2034 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 2035 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 2036 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 2037 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 2038 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 2039 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 2041 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 2045 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 2052 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2053 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2057 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2058 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 2059 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2060 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 2061 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 2062 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2063 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2064 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2065 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2066 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2067 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2068 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2069 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2070 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2071 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2072 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2073 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2074 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2075 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2076 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2077 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2078 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 584:
/* Line 1792 of yacc.c  */
#line 2079 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 585:
/* Line 1792 of yacc.c  */
#line 2083 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 586:
/* Line 1792 of yacc.c  */
#line 2084 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 587:
/* Line 1792 of yacc.c  */
#line 2085 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 588:
/* Line 1792 of yacc.c  */
#line 2086 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 589:
/* Line 1792 of yacc.c  */
#line 2087 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 590:
/* Line 1792 of yacc.c  */
#line 2088 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 591:
/* Line 1792 of yacc.c  */
#line 2089 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 592:
/* Line 1792 of yacc.c  */
#line 2093 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 593:
/* Line 1792 of yacc.c  */
#line 2094 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 594:
/* Line 1792 of yacc.c  */
#line 2095 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 595:
/* Line 1792 of yacc.c  */
#line 2096 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 596:
/* Line 1792 of yacc.c  */
#line 2100 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 597:
/* Line 1792 of yacc.c  */
#line 2101 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 598:
/* Line 1792 of yacc.c  */
#line 2102 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 599:
/* Line 1792 of yacc.c  */
#line 2103 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 600:
/* Line 1792 of yacc.c  */
#line 2108 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 601:
/* Line 1792 of yacc.c  */
#line 2109 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 602:
/* Line 1792 of yacc.c  */
#line 2110 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 603:
/* Line 1792 of yacc.c  */
#line 2111 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 604:
/* Line 1792 of yacc.c  */
#line 2112 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 605:
/* Line 1792 of yacc.c  */
#line 2113 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 606:
/* Line 1792 of yacc.c  */
#line 2114 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 10013 "bison-chapel.cpp"
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


