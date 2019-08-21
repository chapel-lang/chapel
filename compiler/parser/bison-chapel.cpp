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
     TBYTES = 273,
     TCATCH = 274,
     TCLASS = 275,
     TCOBEGIN = 276,
     TCOFORALL = 277,
     TCOMPLEX = 278,
     TCONFIG = 279,
     TCONST = 280,
     TCONTINUE = 281,
     TDEFER = 282,
     TDELETE = 283,
     TDMAPPED = 284,
     TDO = 285,
     TDOMAIN = 286,
     TELSE = 287,
     TENUM = 288,
     TEXCEPT = 289,
     TEXPORT = 290,
     TEXTERN = 291,
     TFALSE = 292,
     TFOR = 293,
     TFORALL = 294,
     TFORWARDING = 295,
     TIF = 296,
     TIMAG = 297,
     TIN = 298,
     TINDEX = 299,
     TINLINE = 300,
     TINOUT = 301,
     TINT = 302,
     TITER = 303,
     TINITEQUALS = 304,
     TLABEL = 305,
     TLAMBDA = 306,
     TLET = 307,
     TLIFETIME = 308,
     TLOCAL = 309,
     TLOCALE = 310,
     TMINUSMINUS = 311,
     TMODULE = 312,
     TNEW = 313,
     TNIL = 314,
     TNOINIT = 315,
     TNONE = 316,
     TNOTHING = 317,
     TON = 318,
     TONLY = 319,
     TOTHERWISE = 320,
     TOUT = 321,
     TOVERRIDE = 322,
     TOWNED = 323,
     TPARAM = 324,
     TPLUSPLUS = 325,
     TPRAGMA = 326,
     TPRIMITIVE = 327,
     TPRIVATE = 328,
     TPROC = 329,
     TPROTOTYPE = 330,
     TPUBLIC = 331,
     TREAL = 332,
     TRECORD = 333,
     TREDUCE = 334,
     TREF = 335,
     TREQUIRE = 336,
     TRETURN = 337,
     TSCAN = 338,
     TSELECT = 339,
     TSERIAL = 340,
     TSHARED = 341,
     TSINGLE = 342,
     TSPARSE = 343,
     TSTRING = 344,
     TSUBDOMAIN = 345,
     TSYNC = 346,
     TTHEN = 347,
     TTHIS = 348,
     TTHROW = 349,
     TTHROWS = 350,
     TTRUE = 351,
     TTRY = 352,
     TTRYBANG = 353,
     TTYPE = 354,
     TUINT = 355,
     TUNDERSCORE = 356,
     TUNION = 357,
     TUNMANAGED = 358,
     TUSE = 359,
     TVAR = 360,
     TVOID = 361,
     TWHEN = 362,
     TWHERE = 363,
     TWHILE = 364,
     TWITH = 365,
     TYIELD = 366,
     TZIP = 367,
     TALIAS = 368,
     TAND = 369,
     TASSIGN = 370,
     TASSIGNBAND = 371,
     TASSIGNBOR = 372,
     TASSIGNBXOR = 373,
     TASSIGNDIVIDE = 374,
     TASSIGNEXP = 375,
     TASSIGNLAND = 376,
     TASSIGNLOR = 377,
     TASSIGNMINUS = 378,
     TASSIGNMOD = 379,
     TASSIGNMULTIPLY = 380,
     TASSIGNPLUS = 381,
     TASSIGNREDUCE = 382,
     TASSIGNSL = 383,
     TASSIGNSR = 384,
     TBANG = 385,
     TBAND = 386,
     TBNOT = 387,
     TBOR = 388,
     TBXOR = 389,
     TCOLON = 390,
     TCOMMA = 391,
     TDIVIDE = 392,
     TDOT = 393,
     TDOTDOT = 394,
     TDOTDOTDOT = 395,
     TEQUAL = 396,
     TEXP = 397,
     TGREATER = 398,
     TGREATEREQUAL = 399,
     THASH = 400,
     TIO = 401,
     TLESS = 402,
     TLESSEQUAL = 403,
     TMINUS = 404,
     TMOD = 405,
     TNOTEQUAL = 406,
     TOR = 407,
     TPLUS = 408,
     TQUESTION = 409,
     TSEMI = 410,
     TSHIFTLEFT = 411,
     TSHIFTRIGHT = 412,
     TSTAR = 413,
     TSWAP = 414,
     TLCBR = 415,
     TRCBR = 416,
     TLP = 417,
     TRP = 418,
     TLSBR = 419,
     TRSBR = 420,
     TNOELSE = 421,
     TUMINUS = 422,
     TUPLUS = 423
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
#line 483 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 490 "bison-chapel.cpp"
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
#define YYLAST   17637

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  169
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  140
/* YYNRULES -- Number of rules.  */
#define YYNRULES  601
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1079

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   423

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
     165,   166,   167,   168
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
     371,   377,   380,   388,   397,   405,   414,   418,   422,   428,
     434,   437,   443,   447,   454,   461,   466,   472,   478,   482,
     486,   493,   499,   506,   512,   519,   523,   528,   532,   537,
     544,   552,   559,   567,   572,   578,   583,   589,   594,   599,
     603,   610,   616,   623,   629,   638,   646,   649,   653,   657,
     660,   663,   667,   671,   672,   675,   678,   682,   688,   690,
     694,   698,   704,   710,   711,   714,   718,   721,   725,   732,
     739,   741,   743,   745,   746,   749,   750,   753,   757,   763,
     769,   771,   773,   776,   780,   782,   786,   787,   788,   797,
     798,   800,   802,   803,   804,   815,   819,   823,   829,   835,
     839,   841,   845,   847,   850,   852,   854,   856,   858,   860,
     862,   864,   866,   868,   870,   872,   874,   876,   878,   880,
     882,   884,   886,   888,   890,   892,   894,   896,   898,   900,
     903,   905,   907,   909,   911,   913,   915,   917,   919,   921,
     923,   925,   927,   928,   932,   936,   937,   939,   943,   948,
     954,   959,   966,   973,   974,   976,   978,   980,   982,   984,
     987,   990,   992,   994,   996,   997,   999,  1001,  1004,  1006,
    1008,  1010,  1012,  1013,  1015,  1018,  1020,  1022,  1024,  1025,
    1027,  1029,  1031,  1033,  1035,  1038,  1040,  1041,  1043,  1046,
    1049,  1050,  1053,  1056,  1061,  1066,  1068,  1072,  1076,  1080,
    1084,  1088,  1092,  1096,  1099,  1101,  1103,  1107,  1112,  1117,
    1120,  1125,  1126,  1129,  1132,  1134,  1136,  1138,  1141,  1143,
    1148,  1152,  1154,  1158,  1162,  1168,  1170,  1172,  1176,  1178,
    1181,  1185,  1186,  1189,  1192,  1196,  1199,  1204,  1208,  1212,
    1217,  1221,  1222,  1225,  1228,  1231,  1233,  1234,  1237,  1240,
    1243,  1245,  1250,  1255,  1262,  1266,  1267,  1269,  1271,  1275,
    1280,  1284,  1289,  1296,  1297,  1300,  1303,  1306,  1309,  1311,
    1313,  1317,  1321,  1323,  1327,  1329,  1331,  1333,  1337,  1341,
    1342,  1344,  1346,  1350,  1354,  1358,  1360,  1362,  1364,  1366,
    1368,  1371,  1373,  1375,  1377,  1379,  1381,  1384,  1389,  1394,
    1399,  1405,  1408,  1411,  1413,  1416,  1418,  1421,  1423,  1426,
    1428,  1431,  1433,  1435,  1437,  1444,  1451,  1456,  1466,  1476,
    1484,  1491,  1498,  1503,  1513,  1523,  1531,  1536,  1543,  1550,
    1560,  1570,  1577,  1579,  1581,  1583,  1585,  1587,  1589,  1591,
    1593,  1597,  1598,  1600,  1605,  1607,  1611,  1616,  1618,  1622,
    1627,  1631,  1635,  1637,  1639,  1642,  1644,  1647,  1649,  1651,
    1655,  1658,  1663,  1665,  1667,  1669,  1671,  1673,  1675,  1677,
    1679,  1684,  1688,  1692,  1695,  1698,  1700,  1701,  1703,  1706,
    1709,  1711,  1713,  1715,  1717,  1719,  1721,  1724,  1726,  1731,
    1736,  1741,  1745,  1749,  1753,  1757,  1763,  1767,  1772,  1776,
    1781,  1783,  1785,  1787,  1789,  1791,  1793,  1795,  1797,  1799,
    1803,  1808,  1812,  1817,  1821,  1826,  1830,  1836,  1840,  1844,
    1848,  1852,  1856,  1860,  1864,  1868,  1872,  1876,  1880,  1884,
    1888,  1892,  1896,  1900,  1904,  1908,  1912,  1916,  1920,  1924,
    1928,  1931,  1934,  1937,  1940,  1943,  1946,  1949,  1953,  1957,
    1961,  1965,  1969,  1973,  1977,  1981,  1983,  1985,  1987,  1989,
    1991,  1993
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     170,     0,    -1,   171,    -1,    -1,   171,   172,    -1,   174,
      -1,   173,   174,    -1,    71,     7,    -1,   173,    71,     7,
      -1,   176,    -1,   179,    -1,   185,    -1,   186,    -1,   197,
      -1,   187,    -1,   201,    -1,   204,    -1,   202,    -1,   211,
      -1,   205,    -1,   206,    -1,   210,    -1,   196,    -1,   282,
     155,    -1,    12,   174,    -1,    13,   283,   174,    -1,    14,
     188,   155,    -1,    21,   283,   179,    -1,    26,   188,   155,
      -1,    28,   270,   155,    -1,    50,   190,   174,    -1,    54,
     293,   195,    -1,    54,   195,    -1,    63,   293,   195,    -1,
      85,   293,   195,    -1,    85,   195,    -1,    91,   174,    -1,
     111,   293,   155,    -1,     1,   155,    -1,   177,   178,    57,
     190,    -1,   175,   160,   161,    -1,   175,   160,   180,   161,
      -1,   175,   160,     1,   161,    -1,    -1,    76,    -1,    73,
      -1,    -1,    75,    -1,   160,   161,    -1,   160,   180,   161,
      -1,   160,     1,   161,    -1,   172,    -1,   180,   172,    -1,
     293,    -1,   293,    11,   293,    -1,   181,   136,   293,    -1,
     181,   136,   293,    11,   293,    -1,    -1,   181,    -1,   158,
      -1,   181,    -1,    -1,    76,    -1,    73,    -1,   184,   104,
     269,   155,    -1,   184,   104,   293,    34,   183,   155,    -1,
     184,   104,   293,    64,   182,   155,    -1,    81,   269,   155,
      -1,   296,   232,   295,   155,    -1,   296,   159,   295,   155,
      -1,   296,   127,   295,   155,    -1,   296,   121,   295,   155,
      -1,   296,   122,   295,   155,    -1,    -1,     3,    -1,     3,
      -1,    61,    -1,    93,    -1,    37,    -1,    96,    -1,   192,
      -1,     3,    -1,    61,    -1,    93,    -1,    37,    -1,    96,
      -1,   192,    -1,     3,    -1,    93,    -1,    15,    -1,    47,
      -1,   100,    -1,    77,    -1,    42,    -1,    23,    -1,    18,
      -1,    89,    -1,    91,    -1,    87,    -1,    68,    -1,    86,
      -1,    16,    -1,   103,    -1,    31,    -1,    44,    -1,    55,
      -1,    62,    -1,   106,    -1,    15,    -1,    47,    -1,   100,
      -1,    77,    -1,    42,    -1,    23,    -1,    18,    -1,    89,
      -1,    55,    -1,    62,    -1,   106,    -1,    91,    -1,    87,
      -1,    31,    -1,    44,    -1,    30,   174,    -1,   179,    -1,
      82,   155,    -1,    82,   295,   155,    -1,   155,    -1,   198,
      -1,    76,   198,    -1,    73,   198,    -1,   226,    -1,   256,
      -1,   218,    -1,   252,    -1,   214,    -1,   199,    -1,   200,
      -1,    40,   293,   155,    -1,    40,   293,    34,   183,   155,
      -1,    40,   293,    64,   182,   155,    -1,    40,   256,    -1,
      36,    78,   190,   216,   160,   217,   161,    -1,    36,     7,
      78,   190,   216,   160,   217,   161,    -1,    35,    78,   190,
     216,   160,   217,   161,    -1,    35,     7,    78,   190,   216,
     160,   217,   161,    -1,    36,   294,   226,    -1,    35,   294,
     226,    -1,    36,   294,   255,   257,   155,    -1,    35,   294,
     255,   257,   155,    -1,    36,     9,    -1,    30,   174,   109,
     293,   155,    -1,   109,   293,   195,    -1,    22,   293,    43,
     293,   283,   195,    -1,    22,   293,    43,   203,   283,   195,
      -1,    22,   293,   283,   195,    -1,    38,   293,    43,   293,
     195,    -1,    38,   293,    43,   203,   195,    -1,    38,   293,
     195,    -1,    38,   203,   195,    -1,    38,    69,   190,    43,
     293,   195,    -1,    39,   293,    43,   293,   195,    -1,    39,
     293,    43,   293,   286,   195,    -1,    39,   293,    43,   203,
     195,    -1,    39,   293,    43,   203,   286,   195,    -1,    39,
     293,   195,    -1,    39,   293,   286,   195,    -1,    39,   203,
     195,    -1,    39,   203,   286,   195,    -1,   164,   269,    43,
     293,   165,   174,    -1,   164,   269,    43,   293,   286,   165,
     174,    -1,   164,   269,    43,   203,   165,   174,    -1,   164,
     269,    43,   203,   286,   165,   174,    -1,   164,   269,   165,
     174,    -1,   164,   269,   286,   165,   174,    -1,   164,   203,
     165,   174,    -1,   164,   203,   286,   165,   174,    -1,   112,
     162,   269,   163,    -1,    41,   293,    92,   174,    -1,    41,
     293,   179,    -1,    41,   293,    92,   174,    32,   174,    -1,
      41,   293,   179,    32,   174,    -1,    41,   293,   232,   293,
      92,   174,    -1,    41,   293,   232,   293,   179,    -1,    41,
     293,   232,   293,    92,   174,    32,   174,    -1,    41,   293,
     232,   293,   179,    32,   174,    -1,    27,   174,    -1,    97,
     293,   155,    -1,    98,   293,   155,    -1,    97,   187,    -1,
      98,   187,    -1,    97,   179,   207,    -1,    98,   179,   207,
      -1,    -1,   207,   208,    -1,    19,   179,    -1,    19,   209,
     179,    -1,    19,   162,   209,   163,   179,    -1,   190,    -1,
     190,   135,   293,    -1,    94,   293,   155,    -1,    84,   293,
     160,   212,   161,    -1,    84,   293,   160,     1,   161,    -1,
      -1,   212,   213,    -1,   107,   269,   195,    -1,    65,   174,
      -1,    65,    30,   174,    -1,   215,   190,   216,   160,   217,
     161,    -1,   215,   190,   216,   160,     1,   161,    -1,    20,
      -1,    78,    -1,   102,    -1,    -1,   135,   269,    -1,    -1,
     217,   197,    -1,   217,   173,   197,    -1,   219,   190,   160,
     220,   161,    -1,   219,   190,   160,     1,   161,    -1,    33,
      -1,   221,    -1,   220,   136,    -1,   220,   136,   221,    -1,
     190,    -1,   190,   115,   293,    -1,    -1,    -1,    51,   223,
     234,   224,   241,   264,   248,   244,    -1,    -1,    45,    -1,
      67,    -1,    -1,    -1,   225,   240,   227,   229,   228,   241,
     263,   242,   248,   243,    -1,   239,   231,   233,    -1,   239,
     232,   233,    -1,   239,   230,   138,   231,   233,    -1,   239,
     230,   138,   232,   233,    -1,   239,     1,   233,    -1,   276,
      -1,   162,   293,   163,    -1,   189,    -1,   132,   190,    -1,
     131,    -1,   133,    -1,   134,    -1,   132,    -1,   141,    -1,
     151,    -1,   148,    -1,   144,    -1,   147,    -1,   143,    -1,
     153,    -1,   149,    -1,   158,    -1,   137,    -1,   156,    -1,
     157,    -1,   150,    -1,   142,    -1,   130,    -1,    17,    -1,
     145,    -1,    10,    -1,   159,    -1,   146,    -1,    49,    -1,
     190,   130,    -1,   115,    -1,   126,    -1,   123,    -1,   125,
      -1,   119,    -1,   124,    -1,   120,    -1,   116,    -1,   117,
      -1,   118,    -1,   129,    -1,   128,    -1,    -1,   162,   235,
     163,    -1,   162,   235,   163,    -1,    -1,   236,    -1,   235,
     136,   236,    -1,   237,   190,   268,   261,    -1,   173,   237,
     190,   268,   261,    -1,   237,   190,   268,   247,    -1,   237,
     162,   260,   163,   268,   261,    -1,   237,   162,   260,   163,
     268,   247,    -1,    -1,   238,    -1,    43,    -1,    46,    -1,
      66,    -1,    25,    -1,    25,    43,    -1,    25,    80,    -1,
      69,    -1,    80,    -1,    99,    -1,    -1,    69,    -1,    80,
      -1,    25,    80,    -1,    25,    -1,    99,    -1,    74,    -1,
      48,    -1,    -1,    25,    -1,    25,    80,    -1,    80,    -1,
      69,    -1,    99,    -1,    -1,    95,    -1,   155,    -1,   244,
      -1,   179,    -1,   196,    -1,   154,   190,    -1,   154,    -1,
      -1,   245,    -1,   140,   293,    -1,   140,   246,    -1,    -1,
     108,   293,    -1,    53,   249,    -1,   108,   293,    53,   249,
      -1,    53,   249,   108,   293,    -1,   250,    -1,   249,   136,
     250,    -1,   251,   115,   251,    -1,   251,   147,   251,    -1,
     251,   148,   251,    -1,   251,   141,   251,    -1,   251,   143,
     251,    -1,   251,   144,   251,    -1,    82,   251,    -1,     3,
      -1,    93,    -1,    99,   253,   155,    -1,    24,    99,   253,
     155,    -1,    36,    99,   253,   155,    -1,   190,   254,    -1,
     190,   254,   136,   253,    -1,    -1,   115,   277,    -1,   115,
     265,    -1,    69,    -1,    25,    -1,    80,    -1,    25,    80,
      -1,   105,    -1,    24,   255,   257,   155,    -1,   255,   257,
     155,    -1,   258,    -1,   257,   136,   258,    -1,   190,   264,
     261,    -1,   162,   260,   163,   264,   261,    -1,   101,    -1,
     190,    -1,   162,   260,   163,    -1,   259,    -1,   259,   136,
      -1,   259,   136,   260,    -1,    -1,   115,    60,    -1,   115,
     295,    -1,   164,   165,   277,    -1,   164,   165,    -1,   164,
     269,   165,   277,    -1,   164,   269,   165,    -1,   164,   165,
     262,    -1,   164,   269,   165,   262,    -1,   164,     1,   165,
      -1,    -1,   135,   277,    -1,   135,   262,    -1,   135,   194,
      -1,     1,    -1,    -1,   135,   277,    -1,   135,   265,    -1,
     135,   194,    -1,     1,    -1,   164,   269,   165,   277,    -1,
     164,   269,   165,   265,    -1,   164,   269,    43,   293,   165,
     277,    -1,   164,     1,   165,    -1,    -1,   277,    -1,   245,
      -1,   164,   165,   266,    -1,   164,   269,   165,   266,    -1,
     164,   165,   267,    -1,   164,   269,   165,   267,    -1,   164,
     269,    43,   293,   165,   266,    -1,    -1,   135,   277,    -1,
     135,   245,    -1,   135,   194,    -1,   135,   267,    -1,   293,
      -1,   245,    -1,   269,   136,   293,    -1,   269,   136,   245,
      -1,   293,    -1,   270,   136,   293,    -1,   101,    -1,   295,
      -1,   245,    -1,   271,   136,   271,    -1,   272,   136,   271,
      -1,    -1,   274,    -1,   275,    -1,   274,   136,   275,    -1,
     191,   115,   245,    -1,   191,   115,   295,    -1,   245,    -1,
     295,    -1,   191,    -1,   193,    -1,   278,    -1,   278,   154,
      -1,   281,    -1,   296,    -1,   280,    -1,   305,    -1,   304,
      -1,    87,   293,    -1,    44,   162,   273,   163,    -1,    31,
     162,   273,   163,    -1,    90,   162,   273,   163,    -1,    88,
      90,   162,   273,   163,    -1,    12,   293,    -1,    91,   293,
      -1,    68,    -1,    68,   293,    -1,   103,    -1,   103,   293,
      -1,    86,    -1,    86,   293,    -1,    16,    -1,    16,   293,
      -1,    20,    -1,    78,    -1,    33,    -1,    38,   293,    43,
     293,    30,   293,    -1,    38,   293,    43,   203,    30,   293,
      -1,    38,   293,    30,   293,    -1,    38,   293,    43,   293,
      30,    41,   293,    92,   293,    -1,    38,   293,    43,   203,
      30,    41,   293,    92,   293,    -1,    38,   293,    30,    41,
     293,    92,   293,    -1,    39,   293,    43,   293,    30,   293,
      -1,    39,   293,    43,   203,    30,   293,    -1,    39,   293,
      30,   293,    -1,    39,   293,    43,   293,    30,    41,   293,
      92,   293,    -1,    39,   293,    43,   203,    30,    41,   293,
      92,   293,    -1,    39,   293,    30,    41,   293,    92,   293,
      -1,   164,   269,   165,   293,    -1,   164,   269,    43,   293,
     165,   293,    -1,   164,   269,    43,   203,   165,   293,    -1,
     164,   269,    43,   293,   165,    41,   293,    92,   293,    -1,
     164,   269,    43,   203,   165,    41,   293,    92,   293,    -1,
      41,   293,    92,   293,    32,   293,    -1,    59,    -1,   281,
      -1,   276,    -1,   299,    -1,   298,    -1,   222,    -1,   291,
      -1,   292,    -1,   290,   146,   293,    -1,    -1,   284,    -1,
     110,   162,   285,   163,    -1,   288,    -1,   285,   136,   288,
      -1,   110,   162,   287,   163,    -1,   288,    -1,   287,   136,
     288,    -1,   289,   276,   264,   261,    -1,   308,    79,   276,
      -1,   293,    79,   276,    -1,    25,    -1,    43,    -1,    25,
      43,    -1,    80,    -1,    25,    80,    -1,   105,    -1,   296,
      -1,   290,   146,   293,    -1,    58,   293,    -1,    52,   257,
      43,   293,    -1,   302,    -1,   277,    -1,   279,    -1,   306,
      -1,   307,    -1,   222,    -1,   291,    -1,   292,    -1,   162,
     140,   293,   163,    -1,   293,   135,   293,    -1,   293,   139,
     293,    -1,   293,   139,    -1,   139,   293,    -1,   139,    -1,
      -1,   293,    -1,    97,   293,    -1,    98,   293,    -1,   293,
      -1,   276,    -1,   298,    -1,   299,    -1,   300,    -1,   296,
      -1,   293,   130,    -1,   222,    -1,   297,   162,   273,   163,
      -1,   297,   164,   273,   165,    -1,    72,   162,   273,   163,
      -1,   293,   138,   191,    -1,   293,   138,    99,    -1,   293,
     138,    31,    -1,   293,   138,    55,    -1,   293,   138,    18,
     162,   163,    -1,   162,   271,   163,    -1,   162,   271,   136,
     163,    -1,   162,   272,   163,    -1,   162,   272,   136,   163,
      -1,    37,    -1,    96,    -1,   301,    -1,     4,    -1,     5,
      -1,     6,    -1,     7,    -1,     8,    -1,    61,    -1,   160,
     269,   161,    -1,   160,   269,   136,   161,    -1,   164,   269,
     165,    -1,   164,   269,   136,   165,    -1,   164,   303,   165,
      -1,   164,   303,   136,   165,    -1,   293,   113,   293,    -1,
     303,   136,   293,   113,   293,    -1,   293,   153,   293,    -1,
     293,   149,   293,    -1,   293,   158,   293,    -1,   293,   137,
     293,    -1,   293,   156,   293,    -1,   293,   157,   293,    -1,
     293,   150,   293,    -1,   293,   141,   293,    -1,   293,   151,
     293,    -1,   293,   148,   293,    -1,   293,   144,   293,    -1,
     293,   147,   293,    -1,   293,   143,   293,    -1,   293,   131,
     293,    -1,   293,   133,   293,    -1,   293,   134,   293,    -1,
     293,   114,   293,    -1,   293,   152,   293,    -1,   293,   142,
     293,    -1,   293,    17,   293,    -1,   293,    10,   293,    -1,
     293,   145,   293,    -1,   293,    29,   293,    -1,   153,   293,
      -1,   149,   293,    -1,    56,   293,    -1,    70,   293,    -1,
     130,   293,    -1,   293,   130,    -1,   132,   293,    -1,   293,
      79,   293,    -1,   293,    79,   203,    -1,   308,    79,   293,
      -1,   308,    79,   203,    -1,   293,    83,   293,    -1,   293,
      83,   203,    -1,   308,    83,   293,    -1,   308,    83,   203,
      -1,   153,    -1,   158,    -1,   114,    -1,   152,    -1,   131,
      -1,   133,    -1,   134,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   492,   492,   497,   498,   504,   505,   510,   511,   516,
     517,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   539,   540,   541,   542,   543,   544,   545,   549,
     562,   567,   572,   580,   581,   582,   586,   587,   600,   601,
     602,   607,   608,   613,   618,   623,   627,   634,   639,   643,
     648,   652,   653,   654,   658,   659,   660,   664,   668,   670,
     672,   674,   676,   683,   684,   688,   689,   690,   691,   692,
     693,   696,   697,   698,   699,   700,   701,   713,   714,   725,
     726,   727,   728,   729,   730,   731,   732,   733,   734,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   747,   748,
     749,   750,   751,   752,   753,   754,   755,   756,   757,   764,
     765,   766,   767,   777,   778,   782,   783,   787,   788,   789,
     790,   794,   795,   796,   797,   798,   799,   800,   804,   805,
     806,   807,   811,   821,   831,   841,   852,   856,   861,   867,
     876,   883,   884,   885,   886,   887,   888,   889,   890,   891,
     892,   893,   894,   895,   896,   897,   898,   899,   900,   901,
     907,   913,   919,   925,   932,   939,   943,   950,   954,   955,
     956,   957,   958,   960,   962,   964,   969,   972,   973,   974,
     975,   976,   977,   981,   982,   986,   987,   988,   992,   993,
     997,  1000,  1002,  1007,  1008,  1012,  1014,  1016,  1023,  1033,
    1047,  1052,  1057,  1065,  1066,  1071,  1072,  1074,  1079,  1095,
    1102,  1111,  1119,  1123,  1130,  1131,  1136,  1141,  1135,  1168,
    1174,  1181,  1192,  1198,  1191,  1230,  1234,  1239,  1243,  1248,
    1255,  1256,  1260,  1261,  1262,  1263,  1264,  1265,  1266,  1267,
    1268,  1269,  1270,  1271,  1272,  1273,  1274,  1275,  1276,  1277,
    1278,  1279,  1280,  1281,  1282,  1283,  1284,  1285,  1286,  1287,
    1291,  1292,  1293,  1294,  1295,  1296,  1297,  1298,  1299,  1300,
    1301,  1302,  1306,  1307,  1311,  1315,  1316,  1317,  1321,  1323,
    1325,  1327,  1329,  1334,  1335,  1339,  1340,  1341,  1342,  1343,
    1344,  1345,  1346,  1347,  1351,  1352,  1353,  1354,  1355,  1356,
    1360,  1361,  1365,  1366,  1367,  1368,  1369,  1370,  1374,  1375,
    1378,  1379,  1383,  1384,  1388,  1390,  1395,  1396,  1400,  1401,
    1405,  1406,  1408,  1410,  1412,  1417,  1418,  1421,  1422,  1423,
    1424,  1425,  1426,  1427,  1430,  1431,  1434,  1436,  1438,  1443,
    1456,  1473,  1474,  1476,  1481,  1482,  1483,  1484,  1485,  1489,
    1495,  1503,  1504,  1512,  1514,  1519,  1521,  1523,  1528,  1530,
    1532,  1539,  1540,  1541,  1546,  1548,  1550,  1554,  1558,  1560,
    1564,  1572,  1573,  1574,  1575,  1576,  1581,  1582,  1583,  1584,
    1585,  1605,  1609,  1613,  1621,  1628,  1629,  1630,  1634,  1636,
    1642,  1644,  1646,  1651,  1652,  1653,  1654,  1655,  1661,  1662,
    1663,  1664,  1668,  1669,  1673,  1674,  1675,  1679,  1680,  1684,
    1685,  1689,  1690,  1694,  1695,  1696,  1697,  1701,  1702,  1713,
    1715,  1720,  1721,  1722,  1723,  1724,  1725,  1727,  1729,  1731,
    1733,  1735,  1737,  1740,  1742,  1744,  1746,  1748,  1750,  1752,
    1754,  1757,  1759,  1761,  1767,  1769,  1771,  1773,  1775,  1777,
    1779,  1781,  1783,  1785,  1787,  1789,  1791,  1798,  1804,  1810,
    1816,  1825,  1835,  1843,  1844,  1845,  1846,  1847,  1848,  1849,
    1850,  1855,  1856,  1860,  1864,  1865,  1869,  1873,  1874,  1878,
    1882,  1886,  1893,  1894,  1895,  1896,  1897,  1898,  1902,  1903,
    1908,  1913,  1921,  1922,  1923,  1924,  1925,  1926,  1927,  1928,
    1929,  1931,  1933,  1935,  1937,  1939,  1944,  1945,  1948,  1949,
    1950,  1953,  1954,  1955,  1956,  1967,  1968,  1969,  1973,  1974,
    1975,  1979,  1980,  1981,  1982,  1983,  1991,  1992,  1993,  1994,
    1998,  1999,  2003,  2004,  2005,  2006,  2007,  2008,  2009,  2010,
    2011,  2012,  2013,  2014,  2018,  2026,  2027,  2031,  2032,  2033,
    2034,  2035,  2036,  2037,  2038,  2039,  2040,  2041,  2042,  2043,
    2044,  2045,  2046,  2047,  2048,  2049,  2050,  2051,  2052,  2053,
    2057,  2058,  2059,  2060,  2061,  2062,  2063,  2067,  2068,  2069,
    2070,  2074,  2075,  2076,  2077,  2082,  2083,  2084,  2085,  2086,
    2087,  2088
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
  "TBYTES", "TCATCH", "TCLASS", "TCOBEGIN", "TCOFORALL", "TCOMPLEX",
  "TCONFIG", "TCONST", "TCONTINUE", "TDEFER", "TDELETE", "TDMAPPED", "TDO",
  "TDOMAIN", "TELSE", "TENUM", "TEXCEPT", "TEXPORT", "TEXTERN", "TFALSE",
  "TFOR", "TFORALL", "TFORWARDING", "TIF", "TIMAG", "TIN", "TINDEX",
  "TINLINE", "TINOUT", "TINT", "TITER", "TINITEQUALS", "TLABEL", "TLAMBDA",
  "TLET", "TLIFETIME", "TLOCAL", "TLOCALE", "TMINUSMINUS", "TMODULE",
  "TNEW", "TNIL", "TNOINIT", "TNONE", "TNOTHING", "TON", "TONLY",
  "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED", "TPARAM", "TPLUSPLUS",
  "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC",
  "TREAL", "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN",
  "TSELECT", "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE", "TSTRING",
  "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS", "TTHROW", "TTHROWS", "TTRUE",
  "TTRY", "TTRYBANG", "TTYPE", "TUINT", "TUNDERSCORE", "TUNION",
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
  "TRP", "TLSBR", "TRSBR", "TNOELSE", "TUMINUS", "TUPLUS", "$accept",
  "program", "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_start", "module_decl_stmt", "access_control",
  "opt_prototype", "block_stmt", "stmt_ls", "only_ls", "opt_only_ls",
  "except_ls", "use_access_control", "use_stmt", "require_stmt",
  "assignment_stmt", "opt_label_ident", "ident_fn_def", "ident_def",
  "ident_use", "internal_type_ident_def", "scalar_type",
  "reserved_type_ident_use", "do_stmt", "return_stmt", "class_level_stmt",
  "private_decl", "forwarding_stmt", "extern_export_decl_stmt",
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
     415,   416,   417,   418,   419,   420,   421,   422,   423
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   169,   170,   171,   171,   172,   172,   173,   173,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   175,
     176,   176,   176,   177,   177,   177,   178,   178,   179,   179,
     179,   180,   180,   181,   181,   181,   181,   182,   182,   183,
     183,   184,   184,   184,   185,   185,   185,   186,   187,   187,
     187,   187,   187,   188,   188,   189,   189,   189,   189,   189,
     189,   190,   190,   190,   190,   190,   190,   191,   191,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   194,
     194,   194,   194,   195,   195,   196,   196,   197,   197,   197,
     197,   198,   198,   198,   198,   198,   198,   198,   199,   199,
     199,   199,   200,   200,   200,   200,   200,   200,   200,   200,
     201,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   203,   204,   204,
     204,   204,   204,   204,   204,   204,   205,   206,   206,   206,
     206,   206,   206,   207,   207,   208,   208,   208,   209,   209,
     210,   211,   211,   212,   212,   213,   213,   213,   214,   214,
     215,   215,   215,   216,   216,   217,   217,   217,   218,   218,
     219,   220,   220,   220,   221,   221,   223,   224,   222,   225,
     225,   225,   227,   228,   226,   229,   229,   229,   229,   229,
     230,   230,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   233,   233,   234,   235,   235,   235,   236,   236,
     236,   236,   236,   237,   237,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   239,   239,   239,   239,   239,   239,
     240,   240,   241,   241,   241,   241,   241,   241,   242,   242,
     243,   243,   244,   244,   245,   245,   246,   246,   247,   247,
     248,   248,   248,   248,   248,   249,   249,   250,   250,   250,
     250,   250,   250,   250,   251,   251,   252,   252,   252,   253,
     253,   254,   254,   254,   255,   255,   255,   255,   255,   256,
     256,   257,   257,   258,   258,   259,   259,   259,   260,   260,
     260,   261,   261,   261,   262,   262,   262,   262,   262,   262,
     262,   263,   263,   263,   263,   263,   264,   264,   264,   264,
     264,   265,   265,   265,   265,   266,   266,   266,   267,   267,
     267,   267,   267,   268,   268,   268,   268,   268,   269,   269,
     269,   269,   270,   270,   271,   271,   271,   272,   272,   273,
     273,   274,   274,   275,   275,   275,   275,   276,   276,   277,
     277,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   280,   281,   282,   282,   282,   282,   282,   282,   282,
     282,   283,   283,   284,   285,   285,   286,   287,   287,   288,
     288,   288,   289,   289,   289,   289,   289,   289,   290,   290,
     291,   292,   293,   293,   293,   293,   293,   293,   293,   293,
     293,   293,   293,   293,   293,   293,   294,   294,   295,   295,
     295,   296,   296,   296,   296,   297,   297,   297,   298,   298,
     298,   299,   299,   299,   299,   299,   300,   300,   300,   300,
     301,   301,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   303,   303,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     305,   305,   305,   305,   305,   305,   305,   306,   306,   306,
     306,   307,   307,   307,   307,   308,   308,   308,   308,   308,
     308,   308
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
       5,     2,     7,     8,     7,     8,     3,     3,     5,     5,
       2,     5,     3,     6,     6,     4,     5,     5,     3,     3,
       6,     5,     6,     5,     6,     3,     4,     3,     4,     6,
       7,     6,     7,     4,     5,     4,     5,     4,     4,     3,
       6,     5,     6,     5,     8,     7,     2,     3,     3,     2,
       2,     3,     3,     0,     2,     2,     3,     5,     1,     3,
       3,     5,     5,     0,     2,     3,     2,     3,     6,     6,
       1,     1,     1,     0,     2,     0,     2,     3,     5,     5,
       1,     1,     2,     3,     1,     3,     0,     0,     8,     0,
       1,     1,     0,     0,    10,     3,     3,     5,     5,     3,
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
       3,     1,     3,     3,     5,     1,     1,     3,     1,     2,
       3,     0,     2,     2,     3,     2,     4,     3,     3,     4,
       3,     0,     2,     2,     2,     1,     0,     2,     2,     2,
       1,     4,     4,     6,     3,     0,     1,     1,     3,     4,
       3,     4,     6,     0,     2,     2,     2,     2,     1,     1,
       3,     3,     1,     3,     1,     1,     1,     3,     3,     0,
       1,     1,     3,     3,     3,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     2,     4,     4,     4,
       5,     2,     2,     1,     2,     1,     2,     1,     2,     1,
       2,     1,     1,     1,     6,     6,     4,     9,     9,     7,
       6,     6,     4,     9,     9,     7,     4,     6,     6,     9,
       9,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     1,     4,     1,     3,     4,     1,     3,     4,
       3,     3,     1,     1,     2,     1,     2,     1,     1,     3,
       2,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     3,     3,     2,     2,     1,     0,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     1,     4,     4,
       4,     3,     3,     3,     3,     5,     3,     4,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       4,     3,     4,     3,     4,     3,     5,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    87,   543,   544,   545,   546,
     547,     0,   481,    73,   108,   449,   114,   451,   481,     0,
     113,     0,   355,    73,     0,     0,     0,     0,   453,   516,
     516,   540,     0,     0,     0,     0,   112,     0,   230,   109,
       0,   226,     0,     0,   116,     0,     0,   472,   548,   117,
       0,   231,   443,   354,     0,     0,     0,    45,    44,   111,
     452,   356,     0,     0,     0,     0,   447,     0,     0,   115,
       0,     0,    88,     0,   541,     0,     0,     0,   110,   212,
     445,   358,   118,     0,     0,   597,     0,   599,     0,   600,
     601,   515,     0,   598,   595,   127,   596,     0,     0,     0,
       4,     0,     5,     0,     9,    46,    10,     0,    11,    12,
      14,   427,   428,    22,    13,   128,   136,   137,    15,    17,
      16,    19,    20,    21,    18,   135,     0,   133,     0,   507,
       0,   131,   134,     0,   132,   521,   503,   429,   504,   433,
     431,     0,     0,   508,   509,     0,   432,     0,   522,   523,
     524,   542,   502,   435,   434,   505,   506,     0,    38,    24,
     441,     0,     0,   482,    74,     0,     0,   451,   453,     0,
       0,     0,   452,     0,     0,     0,   507,   521,   431,   508,
     509,   450,   432,   522,   523,     0,   481,     0,     0,   357,
       0,   186,     0,   412,     0,   419,   546,   452,   517,   229,
     546,   150,   452,     0,   229,     0,     0,     0,     0,     0,
       0,     0,   141,     0,     0,   419,    81,    89,   101,    95,
      94,   103,    84,    93,   104,    90,   105,    82,   106,    99,
      92,   100,    98,    96,    97,    83,    85,    91,   102,   107,
       0,    86,     0,     0,     0,     0,   361,     0,   124,    32,
       0,   582,   500,     0,   444,   583,     7,   419,   210,   220,
     516,   211,   130,   129,   325,   409,     0,   408,     0,     0,
     125,   520,     0,     0,    35,     0,   448,   436,     0,   419,
      36,   442,     0,   193,   189,     0,   432,   193,   190,     0,
     351,     0,   446,     0,     0,   584,   586,   514,   581,   580,
       0,    48,    51,     0,     0,   414,     0,   416,     0,     0,
     415,     0,     0,   408,     0,     0,     6,     0,    47,     0,
       0,   213,     0,   311,   310,   232,     0,   430,    23,     0,
       0,     0,     0,     0,     0,     0,   585,     0,     0,     0,
       0,     0,     0,   513,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   270,
     277,   278,   279,   274,   276,     0,     0,   272,   275,   273,
     271,     0,   281,   280,     0,     0,   419,   419,     0,     0,
       0,    25,    26,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     0,    28,     0,    29,     0,   427,   425,     0,
     420,   421,   426,     0,   213,   147,     0,     0,   213,     0,
     146,     0,     0,     0,   159,     0,     0,   158,     0,   167,
       0,     0,     0,   165,     0,     0,    57,   138,     0,   179,
       0,     0,    30,   293,   227,   365,     0,   366,   368,     0,
     390,     0,   371,     0,     0,   123,    31,    33,     0,   324,
       0,    67,   518,   519,   126,     0,    34,   419,     0,   200,
     191,   187,   192,   188,     0,   349,   346,   152,    37,    50,
      49,    52,     0,   549,     0,     0,   536,     0,   538,     0,
       0,     0,     0,     0,     0,     0,     0,   553,     8,     0,
      40,     0,     0,     0,   408,     0,     0,     0,   304,   360,
     480,   577,   576,   579,   588,   587,   592,   591,   573,   570,
     571,   572,   511,   560,     0,   533,   534,   532,   531,   512,
     564,   575,   569,   567,   578,   568,   566,   558,   563,   565,
     574,   557,   561,   562,   559,     0,     0,     0,     0,     0,
       0,     0,   590,   589,   594,   593,   492,   493,   495,   497,
       0,   484,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   551,   481,   481,   155,   347,   359,   413,     0,     0,
     438,     0,   213,     0,     0,   213,     0,   348,     0,     0,
       0,     0,   456,     0,     0,     0,   168,     0,   462,     0,
       0,   166,   596,    60,     0,    53,    58,     0,   178,     0,
       0,     0,   437,   298,   295,   296,   297,   301,   302,   303,
     293,     0,   286,     0,   294,   312,     0,   369,     0,   121,
     122,   120,   119,     0,   389,   388,   503,     0,   363,   501,
     362,   530,   411,   410,     0,     0,     0,   439,     0,   194,
     353,   503,     0,   550,   510,   537,   417,   539,   418,   175,
       0,     0,     0,   552,   173,   466,     0,   555,   554,     0,
      42,    41,    39,    64,     0,    57,   214,     0,     0,   224,
       0,   221,   308,   305,   306,   309,   233,     0,     0,    71,
      72,    70,    69,    68,   528,   529,   494,   496,     0,   483,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   151,   423,   424,   422,     0,   215,   149,
       0,   215,   148,     0,   177,     0,     0,   157,     0,   156,
       0,   487,     0,     0,   163,     0,     0,   161,     0,     0,
     139,     0,   140,     0,     0,   181,     0,   183,   299,   300,
       0,   293,   284,     0,   403,   313,   316,   315,   317,     0,
     367,   370,   371,     0,     0,   372,   373,   202,     0,     0,
     201,   204,   440,     0,   195,   198,     0,   350,   176,     0,
       0,     0,     0,   174,     0,     0,     0,     0,   229,   219,
       0,   222,   218,   307,   312,   282,    75,   265,    89,   263,
      95,    94,    78,    93,    90,   268,   105,    76,   106,    92,
      96,    77,    79,    91,   107,   262,   244,   247,   245,   246,
     257,   248,   261,   253,   251,   264,   267,   252,   250,   255,
     260,   249,   254,   258,   259,   256,   266,     0,   242,     0,
      80,     0,   282,   282,   240,   535,   485,   371,   521,   521,
       0,     0,     0,     0,     0,     0,     0,     0,   154,   153,
     215,   229,   215,   229,   160,     0,     0,   455,     0,   454,
       0,   486,     0,     0,   461,   164,     0,   460,   162,    55,
      54,   180,   471,   182,     0,   403,   287,     0,     0,   371,
     314,   330,   364,   394,     0,   551,     0,   206,     0,     0,
       0,   196,     0,   171,   468,     0,     0,   169,   467,     0,
     556,    65,    66,   209,   229,   229,   208,   229,   216,   225,
     223,     0,   293,   239,   243,     0,   269,     0,   235,   236,
     489,     0,     0,     0,     0,     0,     0,     0,     0,   229,
     144,   229,   142,   459,     0,     0,   488,   465,     0,     0,
       0,     0,   185,   371,   403,     0,   406,   405,   407,   503,
     326,   290,   288,     0,     0,     0,     0,   392,   503,   207,
     205,     0,   199,     0,   172,     0,   170,   217,   385,     0,
     318,     0,   241,    75,    77,   282,   282,     0,     0,     0,
       0,     0,     0,   145,   143,     0,     0,     0,     0,    56,
     184,   289,   371,   395,     0,   327,   329,   328,   344,     0,
     345,   332,   335,     0,   331,   322,   323,   228,     0,   197,
       0,     0,     0,   384,   383,   503,   319,   330,   283,   237,
     238,     0,     0,     0,     0,     0,     0,   458,   457,   464,
     463,   292,   291,   397,   398,   400,   503,     0,   551,   343,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     470,   469,     0,   375,     0,     0,     0,   399,   401,   334,
     336,   337,   340,   341,   342,   338,   339,   333,   380,   378,
     503,   551,   320,   234,   321,   395,   379,   503,   402
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   302,   101,   598,   103,   104,   105,   319,
     106,   303,   593,   597,   594,   107,   108,   109,   110,   165,
     828,   244,   111,   241,   112,   624,   249,   113,   114,   115,
     116,   117,   118,   119,   699,   120,   121,   122,   460,   639,
     766,   123,   124,   635,   761,   125,   126,   496,   778,   127,
     128,   670,   671,   176,   242,   615,   130,   131,   498,   784,
     676,   831,   832,   430,   913,   434,   611,   612,   613,   614,
     677,   325,   749,  1017,  1073,  1007,   265,   996,   951,   955,
    1001,  1002,  1003,   132,   291,   465,   133,   134,   245,   246,
     438,   439,   628,  1014,   970,   442,   625,  1034,   948,   879,
     304,   192,   308,   309,   399,   400,   401,   177,   136,   137,
     138,   139,   178,   141,   162,   163,   550,   420,   720,   551,
     552,   142,   179,   180,   145,   204,   402,   182,   147,   183,
     184,   150,   151,   152,   314,   153,   154,   155,   156,   157
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -936
static const yytype_int16 yypact[] =
{
    -936,   137,  2835,  -936,    11,  -936,  -936,  -936,  -936,  -936,
    -936,  4300,    73,   108,  -936, 12846,  -936, 14943,    73,  9962,
    -936,   211,   172,   108,  4300,  9962,  4300,    47, 16175, 10124,
    8018,  -936,  8180,  8828,  6571,  9962,  -936,   123,  -936,  -936,
   16351,  -936, 13185,  8990,  -936,  9962,  9962,  -936,  -936,  -936,
    9962,  -936, 12846,  -936,  9962,   279,   149,  7377, 15327,  -936,
   17531,  -936,  9152,  7532,  9962,  8990, 12846,  9962,   203,  -936,
     191,  4300,  -936,  9962,  -936, 10286, 10286, 16351,  -936,  -936,
   12846,  -936,  -936,  9962,  9962,  -936,  9962,  -936,  9962,  -936,
    -936,  2228,  9962,  -936,  9962,  -936,  -936,  3328,  6733,  8342,
    -936,  4138,  -936,   202,  -936,   299,  -936,   271,  -936,  -936,
    -936,  -936,  -936,  -936,  -936,  -936,  -936,  -936,  -936,  -936,
    -936,  -936,  -936,  -936,  -936,  -936, 16351,  -936, 16351,   411,
     155,  -936,  -936, 13185,  -936,   229,  -936,   264,  -936,  -936,
     276,   281,   305,   313,   331, 17043,  2312,   261,   334,   341,
    -936,  -936,  -936,  -936,  -936,  -936,  -936,   194,  -936,  -936,
   17043,   283,  4300,  -936,  -936,   351,  9962,  -936,  -936,  9962,
    9962,  9962,  -936,  9962,  9152,  9152,   262,  -936,  -936,  -936,
    -936,   373,   317,  -936,  -936,   352, 15016, 16351, 13185,  -936,
     383,  -936,   146, 17043,   419,  7694,   461, 16351, 17043,   220,
     463,  -936, 16351, 16351,   220, 16351,   381,     7, 14550,    -4,
   14518,   212,  -936, 14959, 13766,  7694,  -936,  -936,  -936,  -936,
    -936,  -936,  -936,  -936,  -936,  -936,  -936,  -936,  -936,  -936,
    -936,  -936,  -936,  -936,  -936,  -936,  -936,  -936,  -936,  -936,
    4300,  -936,   389,  2502,   107,    49,  -936,  4300,  -936,  -936,
   14694,  1169,   373, 14694,   373,  1169,  -936,  7694,  -936,  -936,
    9314,  -936,  -936,  -936, 16351,  -936,   237, 17043,  9962,  9962,
    -936, 17043,   397, 14927,  -936, 14694,   373, 17043,   393,  7694,
    -936, 17043, 15103,  -936,  -936, 15368, 12711,  -936,  -936, 15417,
     448,   409,   373, 14694, 15512,   278,   278,  2634,  1169,  1169,
     -29,  -936,  -936,  3490,   -32,  -936,  9962,  -936,    55,    56,
    -936,   -44,   111, 15561,   -31,   558,  -936,  3652,  -936,   510,
    9152,   434,   423,  -936,  -936,  -936,   240,  -936,  -936,  9962,
    9962,  9962,  9962,  8828,  8828,  9962,   361,  9962,  9962,  9962,
    9962,  9962,   487,  2228,  9962,  9962,  9962,  9962,  9962,  9962,
    9962,  9962,  9962,  9962,  9962,  9962,  9962,  9962,  9962,  -936,
    -936,  -936,  -936,  -936,  -936,  8504,  8504,  -936,  -936,  -936,
    -936,  8504,  -936,  -936,  8504,  8504,  7694,  7694,  8828,  8828,
    7219,  -936,  -936, 15160, 15192, 15593,    66,  3814,  -936,  8828,
       7,   424,   285,  -936,  9962,  -936,  9962,   469,  -936,   425,
     454,  -936,  -936, 16351,   434,  -936, 13185, 16351,   434,   438,
    -936, 13185,   557,  9152,  -936,  4462,  8828,  -936,   440,  -936,
       7,  4624,  8828,  -936,     7, 10448,  9962,  -936,  4300,   571,
    9962,   442,  -936,   195,  -936,  -936,  2502,  -936,   470,   444,
    -936, 10610,   494,  9962, 13185,  -936,  -936,  -936,   450,  -936,
    9152,  -936, 17043, 17043,  -936,    28,  -936,  7694,   451,  -936,
     598,  -936,   598,  -936, 10772,   482,  -936,  -936,  -936,  -936,
    -936,  -936,  8666,  -936, 13622,  6895,  -936,  7057,  -936,  4300,
     456,  8828,  6082,  3166,   458,  9962,  6408,  -936,  -936,   153,
    -936,  3976, 16351,   302, 15336,  9152,   459, 17474,   290,  -936,
   15737, 17283, 17283,   447,  -936,   447,  -936,   447, 13432,   964,
     848,  1003,   373,   278,   466,  -936,  -936,  -936,  -936,  2634,
    2596,   447,  2053,  2053, 17283,  2053,  2053,   654,   278,  2596,
   17323,   654,  1169,  1169,   278,   477,   480,   481,   485,   488,
     462,   486,  -936,   447,  -936,   447,    27,  -936,  -936,  -936,
      88,  -936,  1398, 17119,   219, 10934,  8828, 11096,  8828,  9962,
    8828, 13106,    73, 15782,  -936,  -936,  -936, 17043, 15823,  7694,
    -936,  7694,   434,   490,   318,   434,   492,  -936,   323,  9962,
     127,  9962, 17043,    17, 14751,  7219,  -936,  9962, 17043,    15,
   14607,  -936,   491,   509,   501, 15934,   509,   503,   615, 15974,
    4300, 14783,  -936,    53,  -936,  -936,  -936,  -936,  -936,  -936,
     455,   134,  -936, 13242,  -936,   378,   497,  2502,   107,    47,
     123,  9962,  9962,  5920,  -936,  -936,   479,  7856,  -936, 17043,
    -936,  -936,  -936, 17043,   502,   -25,   499,  -936,  2597,  -936,
    -936,   340, 16351,  -936,  -936,  -936,  -936,  -936,  -936,  -936,
    4300,   -26, 13390,  -936,  -936, 17043,  4300, 17043,  -936, 16015,
    -936,  -936,  -936,  -936, 10448,  9962,   529,  1522,   506,   554,
      51,  -936,   594,  -936,  -936,  -936,  -936,  7372,   513,  -936,
    -936,  -936,  -936,  -936,  -936,  -936,  -936,  -936,  7219,  -936,
      16,  8828,  8828,  9962,   663, 16159,  9962,   665, 16191,   519,
   13478,     7,     7,  -936,  -936,  -936,  -936,   537,  -936,  -936,
     539,  -936,  -936, 14694,  -936, 13842,  4786,  -936,  4948,  -936,
     136,  -936, 13918,  5110,  -936,     7,  5272,  -936,     7,  9962,
    -936,  9962,  -936,  4300,  9962,  -936,  4300,   668,  -936,  -936,
   16351,   595,  -936,  2502,   566,   622,  -936,  -936,  -936,    41,
    -936,  -936,   494,   538,   131,  -936,  -936,  -936,  5434,  9152,
    -936,  -936,  -936, 16351,  -936,   569,   352,  -936,  -936,  5596,
     541,  5758,   543,  -936,  9962,   555,   556,   548,  1182,  -936,
    9962, 16351,  -936,  -936,   378,   551,    -8,  -936,   576,  -936,
     578,   581,   597,   586,   587,  -936,   590,   599,   600,   601,
     602,   218,   605,   603,   606,  -936,  -936, 16351,  -936,  -936,
    -936,  -936,  -936,  -936,  -936,  -936,  -936,  -936,  -936,  -936,
    -936,  -936,  -936,  -936,  -936,  -936,  -936,  9962,  -936,   616,
     619,   607,   551,   551,  -936,  -936,  -936,   494,   214,   236,
   16335, 11258, 11420, 16367, 11582, 11744, 11906, 12068,  -936,  -936,
    -936,  1846,  -936, 15748,  -936,  4300,  9962, 17043,  9962, 17043,
    7219,  -936,  4300,  9962, 17043,  -936,  9962, 17043,  -936, 16424,
   17043,  -936, 17043,   700,  4300,   566,  -936,   573,  9476,   129,
    -936,    38,  -936,  -936,  8828, 12976,  4300,  -936,    46,   588,
    9962,  -936,  9962,  -936, 17043,  4300,  9962,  -936, 17043,  4300,
   17043,  -936,  -936,  -936,  1449,  1449,  -936,  1639,  -936, 17043,
    -936,    45,   195,  -936,  -936, 13679,  -936, 13334,  -936,  -936,
    -936,  9962,  9962,  9962,  9962,  9962,  9962,  9962,  9962, 17254,
    -936, 17268,  -936, 15974, 13994, 14070,  -936, 15974, 14146, 14222,
    9962,  4300,  -936,   494,   566,  6245,  -936,  -936,  -936,   297,
    9152,  -936,  -936,   135,  9962,   -15, 13535,  -936,  1240,   419,
    -936,   352, 17043, 14298,  -936, 14374,  -936,  -936,  -936, 12230,
     655,   243,  -936,   623,   625,   551,   551, 16511, 16587, 16663,
   16739, 16815, 16891,  -936,  -936,  4300,  4300,  4300,  4300, 17043,
    -936,  -936,   129,  9638,   138,  -936,  -936, 17043,  -936,    76,
    -936,    63,  -936,   483, 16967,  -936,  -936,  -936, 12068,  -936,
    4300,  4300,  3001,  -936,  -936,   327,  -936,    38,  -936,  -936,
    -936,  9962,  9962,  9962,  9962,  9962,  9962, 15974, 15974, 15974,
   15974,  -936,  -936,  -936,  -936,  -936,   394,  8828, 12554,  -936,
    9962,   135,    76,    76,    76,    76,    76,    76,   135,  1299,
   15974, 15974,   592, 12392,   147,    13, 13576,  -936,  -936, 17043,
    -936,  -936,  -936,  -936,  -936,  -936,  -936,   626,  -936,  -936,
     347, 12716,  -936,  -936,  -936,  9800,  -936,   348,  -936
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -936,  -936,  -936,     1,  -334,  2062,  -936,  -936,  -936,  -936,
     306,   435,  -411,    94,    97,  -936,  -936,  -936,   164,   741,
    -936,    -9,   -71,  -657,  -936,  -828,  1022,  -898,  -681,   -48,
    -936,  -936,  -936,  -936,   115,  -936,  -936,  -936,   478,  -936,
       3,  -936,  -936,  -936,  -936,  -936,  -936,  -345,  -637,  -936,
    -936,  -936,   -14,  1042,  -936,  -936,  -936,   -89,  -936,  -936,
    -936,  -936,  -149,  -144,  -743,  -936,  -142,    30,   166,  -936,
    -936,  -936,   -10,  -936,  -936,  -278,   580,  -936,  -214,  -238,
    -268,  -260,  -355,  -936,  -175,  -936,    12,   748,  -127,   339,
    -936,  -392,  -736,  -818,  -936,  -588,  -457,  -910,  -935,  -826,
     -61,  -936,    52,  -936,  -179,  -936,   215,   439,  -416,  -936,
    -936,  -936,  1081,  -936,     9,  -936,  -936,  -199,  -936,  -572,
    -936,  -936,  1162,  1481,   -11,   761,    22,   646,  -936,  1685,
    1737,  -936,  -936,  -936,  -936,  -936,  -936,  -936,  -936,  -375
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -552
static const yytype_int16 yytable[] =
{
     160,   266,   375,   100,   181,   554,   326,   640,   186,   262,
     263,   424,   391,   721,   193,   596,   882,   440,   198,   198,
     830,   208,   210,   213,   214,   626,   247,   185,   409,   634,
     752,   240,   250,   188,   251,   252,   431,   247,   312,   253,
     758,   254,   440,   255,   616,   723,   968,   716,   641,   943,
     946,   267,   271,   273,   275,   276,   277,  1006,  1035,   573,
     281,   392,   282,   576,   285,   289,   418,    63,   290,   292,
     686,   851,   293,   294,   853,   295,   247,   296,   448,   998,
     297,   298,   759,   299,   418,   272,   267,   271,   313,   918,
     919,   953,   443,  -203,  -386,    63,   738,   908,  -381,   610,
     458,   920,   837,  1058,   472,   486,   418,   687,   440,   560,
     405,   164,   480,   484,   386,   410,   836,   321,   992,   322,
     310,   479,   -81,  -386,   397,   418,   158,  -381,  1057,   473,
     -87,  -386,   469,   739,   487,  -203,   760,     3,   998,   769,
    -381,  1013,   375,   952,   397,   387,   954,   207,   209,  -386,
    -386,   441,  -386,  -381,   481,   160,   387,  1006,   383,   384,
     385,   881,   281,   267,   313,  1078,   158,   387,  1072,  1000,
     908,  1040,   908,   387,   884,   387,   441,   387,   290,  -386,
     969,  1037,   450,   161,   271,   444,   397,   781,   404,  -203,
     560,   475,   477,   408,   290,   390,   412,   540,   541,  1041,
    -381,  -386,   482,   323,   271,  -381,   387,   991,   397,   195,
     554,   406,   782,   929,   311,   931,   411,   999,   476,   478,
     603,   418,  -386,   188,   688,   751,   967,   707,  1000,   324,
     710,   561,  1019,  1020,   437,  1069,    22,    22,   604,   284,
     288,   605,   441,  -386,   627,    22,   271,   482,   908,   198,
     908,   689,   189,  1076,   596,   449,  1032,   452,   453,   493,
     830,   606,  -386,   450,   607,    38,    55,   482,   271,   950,
     741,   518,   860,   378,   482,   608,   483,   379,   636,   574,
      53,    53,   394,   482,   578,   215,   256,    51,   936,    53,
     714,    61,    61,   278,   609,   474,   885,   742,   692,   861,
      61,   395,   379,  1038,   471,   397,   397,   332,   158,   494,
     187,   257,  1071,   554,   660,   672,    81,    81,   500,   501,
     502,   503,   505,   507,   508,    81,   509,   510,   511,   512,
     513,  -285,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,   534,   -83,   248,
    -491,   877,   580,   279,   271,   271,   -88,   333,  -285,   673,
     271,   334,   317,   271,   271,   271,   271,   543,   545,   553,
     674,   248,  -490,   450,   318,   320,   444,  -491,   563,   741,
    -382,   283,   287,   567,  -474,   568,   397,   535,   536,   675,
     725,   728,   451,   537,   572,   499,   538,   539,   575,  -490,
    -374,  -376,   267,   745,   582,   584,  1018,   610,   336,  -382,
     588,   590,  -404,   340,   595,   595,   342,   599,   327,   601,
     345,   444,  -382,   376,  -527,   377,  -527,   437,   957,  -374,
    -376,  -473,   629,  -404,   666,  -382,   328,  -404,   450,   633,
     566,   135,  -374,  -376,   907,   380,   271,   746,   504,   506,
     135,   329,   770,   772,   444,  -374,  -376,   663,   747,   444,
    -404,   633,   949,   135,   271,   135,   271,   767,  -478,   958,
     652,   633,   655,   709,   657,   659,  -352,   748,   712,  -525,
     603,  -525,  -382,   662,   267,   554,  -479,  -382,   669,  -476,
       5,   388,   471,   542,   544,  -352,  -475,   310,   604,   310,
     397,   605,  -374,  -376,   562,   514,   382,  -374,  -376,  -396,
     135,   342,   387,   248,   248,   248,   248,   907,   515,   907,
     429,   606,  -387,  -526,   607,  -526,   315,   646,   396,   648,
    -396,   583,  -387,   833,  -396,   608,   135,   589,   393,   403,
     135,   407,   516,   413,   582,   695,   588,   698,   599,   700,
     655,   433,   454,  1015,   609,   457,   248,  -396,   271,   248,
     271,  -387,   754,   464,   466,   488,  -477,   492,   713,   495,
     715,   701,   702,  -527,   553,  -527,   722,  1036,   610,   565,
      72,   248,   340,   497,   569,   342,   517,  -387,   570,   345,
     571,   705,  1049,   577,  -387,   907,   651,   907,  1042,   248,
     579,   135,   585,   600,   744,   602,   617,   618,   437,   627,
     277,   281,   313,   631,   637,  -387,   271,   638,   642,   667,
     603,   650,  1036,   656,  1043,   684,  1044,  1045,   678,   765,
    1046,  1047,   679,   290,  -387,   680,   681,  1070,   604,  -387,
     682,   605,  -387,   683,  1039,   729,   -59,   733,   146,   756,
     708,   685,   711,   595,   595,  1077,   730,   146,   732,  1036,
     750,   606,   762,   757,   607,   450,    55,   779,   829,   780,
     146,   694,   146,   697,   783,   608,   835,   553,   307,   135,
     505,   543,   840,   332,   846,   843,   135,  1061,  1062,  1063,
    1064,  1065,  1066,   841,   609,   844,   248,   850,   888,   852,
     874,   878,   880,   883,   890,   857,   895,   859,   899,   903,
     901,   902,   864,   912,  -108,   867,  -114,   146,   869,  -113,
     870,   286,   286,   872,  -112,  -109,   248,   -84,  -116,   -82,
     248,   875,   941,   333,   437,   -85,   944,   334,  -117,  -111,
    -115,  -110,   135,   146,  -118,   917,   916,   146,   267,   -86,
    1016,   961,   491,   -81,   765,   -83,   135,  1068,   894,   776,
     898,   775,  1041,   900,   190,   462,   889,   910,   975,   909,
     971,   876,   669,   976,   911,   398,   740,  1074,  1031,  1055,
    1067,  1060,   212,   630,   336,   337,   706,   338,   339,   340,
     199,   341,   342,     0,     0,   398,   345,     0,   914,     0,
       0,     0,     0,     0,   352,     0,   504,   542,   146,     0,
     356,   357,   358,     0,     0,     0,   915,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   135,     0,     0,     0,
     857,   859,     0,   864,   867,   894,   898,   398,     0,     0,
       0,     0,     0,     0,   933,   934,     0,   935,     0,   553,
       0,   937,   938,     0,   135,   939,   262,   263,     0,   398,
     135,     0,     0,     0,     0,     0,     0,   135,     0,     0,
       0,     0,     0,   956,   655,     0,     0,   332,     0,   962,
       0,   963,     0,     0,   994,   965,   146,     0,     0,   248,
     248,     0,     0,   146,     0,   248,   248,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   737,   829,     0,
     933,   977,   978,   937,   979,   980,   981,   982,   135,     0,
       0,     0,   135,     0,     0,     0,     0,   333,     0,   989,
     135,   334,     0,     0,   313,     0,     0,     0,     0,   997,
       0,     0,     0,  1004,   764,     0,     0,     0,     0,   146,
       0,  1054,     0,     0,     0,     0,   398,   398,     0,     0,
       0,     0,     0,   146,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1027,  1028,  1029,  1030,   336,   337,
       0,     0,   339,   340,     0,   341,   342,     0,     0,     0,
     345,   690,     0,   332,     0,     0,     0,   898,   352,  1050,
    1051,   313,     0,     0,   356,   357,   358,   248,   248,     0,
    1027,  1028,  1029,  1030,  1050,  1051,     0,     0,     0,   248,
       0,   429,     0,     0,     0,     0,  1056,   655,   429,  1059,
     632,   248,   332,   146,   248,     0,     0,   398,     0,   135,
       0,     0,     0,   333,   129,     0,     0,   334,     0,     0,
       0,     0,   632,   129,     0,   307,     0,   307,     0,     0,
     655,   146,   632,     0,   898,     0,   129,   146,   129,     0,
       0,     0,   891,     0,   146,     0,     0,     0,     0,     0,
       0,     0,   333,   140,     0,     0,   334,   274,     0,   135,
       0,     0,   140,     0,   336,   135,     0,     0,     0,   340,
       0,   341,   342,     0,     0,   140,   345,   140,     0,     0,
       0,     0,     0,   129,   352,     0,   834,     0,     0,     0,
     356,   357,   358,     0,     0,   146,     0,     0,     0,   146,
     838,   839,     0,   336,   337,     0,     0,   146,   340,   129,
     341,   342,     0,   129,     0,   345,     0,     0,     0,   704,
       0,   398,   140,   352,     0,   135,     0,   135,     0,   356,
     357,   358,   135,     0,   143,   135,     0,     0,     0,     0,
       0,     0,   135,   143,     0,   135,     0,     0,   140,     0,
       0,     0,   140,     0,     0,     0,   143,     0,   143,     0,
       0,     0,     0,     0,   248,     0,     0,   135,   332,     0,
       0,     0,   258,     0,   129,     0,    21,    22,   135,     0,
     135,     0,     0,     0,     0,   259,     0,    29,   260,     0,
       0,     0,    34,     0,     0,     0,     0,    38,     0,   414,
     417,   419,   423,   143,     0,     0,     0,     0,     0,     0,
     429,   429,     0,   140,   429,   429,   146,     0,   333,    51,
       0,    53,   334,    55,     0,   904,     0,     0,   905,   143,
     261,  1005,    61,   143,     0,     0,     0,  1009,     0,   429,
       0,   429,   446,     0,     0,   447,     0,     0,     0,     0,
       0,    77,   129,  -391,    79,     0,     0,    81,     0,   129,
       0,     0,     0,  -391,   135,     0,   146,   456,     0,   336,
       0,   135,   146,     0,   340,     0,   341,   342,     0,     0,
       0,   345,     0,   135,     0,   467,     0,     0,     0,   352,
       0,   140,  -391,     0,   143,   135,     0,   358,   140,     0,
       0,     0,     0,     0,   135,     0,     0,    95,   135,     0,
       0,     0,  -393,   906,     0,   129,     0,     0,  -391,     0,
       0,     0,  -393,     0,     0,  -391,     0,     0,     0,   129,
       0,  1005,   146,     0,   146,     0,     0,     0,     0,   146,
       0,     0,   146,     0,     0,     0,  -391,     0,     0,   146,
     135,  -393,   146,     0,   140,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -391,     0,     0,   140,     0,
    -391,     5,   143,  -391,   146,     0,     0,  -393,     0,   143,
       0,     0,   564,    14,  -393,   146,    16,   146,     0,     0,
       0,    20,     0,     0,   135,   135,   135,   135,     0,   129,
       0,     0,     0,     0,     0,  -393,     0,     0,     0,     0,
      36,     0,   586,     0,     0,    39,   591,     0,     0,   135,
     135,     0,     0,    44,  -393,     0,     0,   129,   947,  -393,
      49,     0,  -393,   129,     0,   143,     0,     0,   140,   258,
     129,     0,     0,    21,    22,    59,     0,     0,     0,   143,
       0,     0,   259,   144,    29,   260,     0,    69,     0,    34,
       0,    72,   144,     0,    38,     0,   140,     0,    78,     0,
       0,   146,   140,     0,    82,   144,     0,   144,   146,   140,
       0,     0,     0,     0,     0,     0,    51,     0,    53,     0,
     146,   129,     0,   777,     0,   129,     0,   261,     0,    61,
     995,     0,   146,   129,     0,     0,     0,     0,     0,     0,
       0,   146,  -215,     0,     0,   146,  -215,  -215,    77,   143,
       0,    79,   144,     0,    81,  -215,     0,  -215,  -215,     0,
     140,     0,  -215,     0,   140,     0,     0,  -215,     0,     0,
    -215,     0,   140,  1033,     0,     0,     0,   143,   144,     0,
       0,     0,   144,   143,     0,     0,     0,   146,     0,  -215,
     143,  -215,     0,  -215,     0,  -215,  -215,     0,  -215,     0,
    -215,     0,  -215,     0,     0,   717,   719,     0,     0,     0,
       0,   724,   727,     0,     0,     0,     0,     0,  1033,     0,
       0,  -215,     0,     0,  -215,     0,     0,  -215,     0,     0,
       0,   146,   146,   146,   146,     0,     0,     0,     0,     0,
       0,   143,   129,   144,     0,   143,     0,     0,     0,     0,
       0,     0,     0,   143,     0,  1033,   146,   146,     0,   258,
       0,     0,     0,    21,    22,     0,     0,     0,     0,     0,
       0,     0,   259,     0,    29,   260,     0,  -215,     0,    34,
       0,   140,     0,  -215,    38,     0,     0,   148,     0,     0,
       0,     0,   129,     0,     0,     0,   148,     0,   129,     0,
       0,     0,     0,     0,     0,     0,    51,     0,    53,   148,
     315,   148,   904,     0,     0,   905,     0,   261,     0,    61,
       0,   144,     0,   848,   849,     0,     0,     0,   144,     0,
       0,   140,     0,     0,     0,   854,     0,   140,    77,   149,
       0,    79,     0,     0,    81,     0,     0,   865,   149,     0,
     868,     0,     0,     0,     0,     0,   148,     0,   129,     0,
     129,   149,   143,   149,     0,   129,     0,     0,   129,     0,
       0,     0,     0,     0,     0,   129,     0,     0,   129,     0,
       0,     0,   148,     0,   144,     0,   148,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,   140,   144,   140,
     129,     0,     0,     0,   140,     0,     0,   140,   149,     0,
       0,   129,   143,   129,   140,     0,     0,   140,   143,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   149,     0,     0,     0,   149,   140,
       0,     0,     0,     0,     0,     0,     0,   148,     0,     0,
     140,     0,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   258,     0,   144,     0,
      21,    22,     0,     0,     0,     0,     0,     0,   143,   259,
     143,    29,   260,     0,     0,   143,    34,     0,   143,     0,
       0,    38,     0,     0,     0,   143,   144,   129,   143,   149,
       0,     0,   144,     0,   129,     0,     0,     0,     0,   144,
     960,     0,     0,    51,     0,    53,   129,    55,     0,   904,
     143,     0,   905,     0,   261,   148,    61,     0,   129,     0,
       0,   143,   148,   143,     0,     0,   140,   129,     0,     0,
       0,   129,     0,   140,     0,    77,     0,     0,    79,     0,
       0,    81,     0,     0,     0,   140,     0,     0,     0,     0,
     144,     0,     0,     0,   144,     0,     0,   140,     0,     0,
       0,     0,   144,     0,     0,     0,   140,   149,     0,     0,
     140,     0,     0,   129,   149,     0,     0,     0,   148,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,   148,     0,     0,     0,     0,   930,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   143,     0,     0,
       0,     0,   140,     0,   143,     0,     0,   129,   129,   129,
     129,     0,     0,     0,     0,     0,   143,     0,     0,     0,
     149,     0,     0,     0,     0,     0,     0,     0,   143,     0,
       0,     0,   129,   129,   149,     0,     0,   143,     0,     0,
       0,   143,     0,     0,   102,     0,   140,   140,   140,   140,
       0,     0,   148,   159,     0,     0,     0,     0,     0,     0,
       0,   144,   332,     0,     0,     0,   191,     0,   194,     0,
       0,   140,   140,     0,     0,     0,     0,     0,     0,     0,
     148,     0,     0,   143,     0,     0,   148,     0,     0,     0,
       0,     0,     0,   148,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   149,     0,     0,     0,     0,     0,
       0,   144,   333,   280,     0,     0,   334,   144,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   143,   143,   143,
     143,     0,   149,     0,     0,     0,     0,     0,   149,   102,
       0,     0,     0,   316,   148,   149,     0,     0,   148,     0,
       0,     0,   143,   143,     0,     0,   148,     0,     0,     0,
       0,     0,     0,   336,   337,     0,   338,   339,   340,     0,
     341,   342,   343,     0,     0,   345,     0,   144,     0,   144,
       0,     0,   351,   352,   144,     0,   355,   144,     0,   356,
     357,   358,     0,     0,   144,     0,   149,   144,     0,     0,
     149,     0,     0,     0,   381,     0,     0,     0,   149,     0,
       0,     5,     6,     7,     8,     9,    10,     0,     0,   144,
     166,     0,     0,    14,    15,     0,    16,     0,   167,     0,
     144,    20,   144,     0,     0,     0,     0,     0,     0,    27,
       0,   168,     0,     0,     0,    31,   169,   170,     0,   171,
      36,     0,    37,     0,     0,    39,     0,     0,     0,    41,
      42,     0,     0,    44,    45,   148,    46,    47,     0,    48,
      49,     0,     0,     0,     0,     0,    52,     0,    54,     0,
      56,     0,   432,     0,     0,    59,   172,     0,     0,   445,
       0,     0,     0,     0,    66,    67,    68,    69,    70,   173,
       0,    72,     0,     0,    74,     0,     0,     0,    78,     0,
       0,    80,     0,     0,    82,   148,   144,   149,     0,     0,
       0,   148,     0,   144,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   144,     0,     0,    86,    87,
      88,    89,    90,     0,     0,   102,     0,   144,     0,     0,
       0,     0,     0,     0,     0,     0,   144,    92,     0,   102,
     144,    94,     0,     0,     0,     0,    96,   149,   174,     0,
      98,     0,   175,   149,     0,     0,     0,     0,     0,     0,
       0,   148,     0,   148,     0,     0,     0,     0,   148,     0,
       0,   148,     0,     0,     0,     0,     0,     0,   148,     0,
       0,   148,   144,     0,     0,     0,     0,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,     0,   148,     0,     0,     0,     0,     0,   102,
       0,     0,     0,   149,   148,   149,   148,     0,  -498,     0,
     149,     0,     0,   149,     0,     0,   144,   144,   144,   144,
     149,   374,     0,   149,  -525,     0,  -525,   445,     0,     0,
       0,     0,     0,   445,     0,     0,     0,     0,     0,     0,
       0,   144,   144,     0,     0,   149,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   216,   149,     0,   149,     0,
       0,     0,     0,     0,     0,     0,     0,   217,   218,     0,
     219,     0,     0,     0,     0,   220,     0,     0,     0,     0,
       0,     0,     0,   221,     0,     0,     0,     0,     0,   222,
     148,   649,     0,     0,   223,   654,   224,   148,     0,   225,
       0,     0,     0,   102,     0,     0,     0,   226,     0,   148,
       0,     0,     0,   227,   228,     0,     0,     0,     0,     0,
     229,   148,     0,     0,     0,     0,     0,     0,     0,   230,
     148,     0,     0,     0,   148,     0,     0,     0,   231,   232,
       0,   233,   149,   234,     0,   235,     0,     0,   236,   149,
     216,     0,   237,   435,     0,   238,     0,     0,   239,     0,
       0,   149,   217,   218,     0,   219,     0,     0,     0,     0,
     220,     0,     0,   149,     0,   332,   148,     0,   221,     0,
       0,     0,   149,     0,   222,     0,   149,     0,     0,   223,
       0,   224,     0,     0,   225,     0,     0,     0,     0,     0,
       0,     0,   226,     0,     0,     0,     0,     0,   227,   228,
       0,     0,   735,   332,   436,   229,     0,     0,     0,     0,
     148,   148,   148,   148,   230,   333,     0,     0,   149,   334,
       0,     0,     0,   231,   232,     0,   233,     0,   234,     0,
     235,     0,     0,   236,     0,   148,   148,   237,     0,     0,
     238,     0,     0,   239,     0,     0,     0,     0,     0,     0,
       0,     0,   768,   333,     0,     0,     0,   334,   773,     0,
       0,     0,   149,   149,   149,   149,   336,   337,     0,   338,
     339,   340,     0,   341,   342,   343,     0,     0,   345,   346,
     347,     0,     0,   349,   350,   351,   352,   149,   149,   355,
       0,     0,   356,   357,   358,     0,     0,   387,     0,   763,
       0,     0,     0,     0,   336,   337,     0,   338,   339,   340,
       0,   341,   342,     0,     0,     0,   345,     0,   445,     0,
     445,     0,     0,   351,   352,   445,     0,   355,   445,     0,
     356,   357,   358,     0,     0,   871,     0,     0,   873,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     887,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   893,     0,   897,     0,    -2,     4,     0,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,    16,     0,    17,    18,    19,    20,    21,
      22,    23,    24,    25,     0,    26,    27,     0,    28,     0,
      29,    30,    31,    32,    33,    34,    35,    36,     0,    37,
      38,     0,    39,  -229,     0,    40,    41,    42,     0,    43,
      44,    45,   -43,    46,    47,     0,    48,    49,    50,     0,
       0,     0,    51,    52,    53,    54,    55,    56,    57,  -229,
     -43,    58,    59,    60,     0,    61,    62,    63,     0,    64,
      65,    66,    67,    68,    69,    70,    71,     0,    72,    73,
       0,    74,    75,    76,    77,    78,   942,    79,    80,   -61,
      81,    82,     0,     0,    83,     0,    84,     0,   959,    85,
       0,     0,     0,     0,     0,     0,     0,   964,     0,     0,
       0,   966,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,     0,
      95,     0,     0,    96,     0,    97,     0,    98,     0,    99,
       0,     0,  1052,   990,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   166,     0,     0,    14,    15,     0,    16,
       0,   167,     0,     0,    20,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   168,     0,     0,     0,    31,   169,
     170,     0,   171,    36,     0,    37,     0,     0,    39,     0,
       0,     0,    41,    42,     0,     0,    44,    45,     0,    46,
      47,     0,    48,    49,     0,     0,     0,     0,     0,    52,
       0,    54,     0,    56,     0,     0,     0,     0,    59,   172,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,    70,   173,     0,    72,     0,     0,    74,     0,     0,
       0,    78,     0,     0,    80,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,   264,     0,     0,     0,    96,
       0,   174,     0,    98,     0,   175,  1053,     4,     0,     5,
       6,     7,     8,     9,    10,     0,  -551,     0,    11,    12,
      13,    14,    15,  -551,    16,     0,    17,    18,    19,    20,
      21,    22,    23,    24,    25,  -551,    26,    27,  -551,    28,
       0,    29,    30,    31,    32,    33,    34,    35,    36,     0,
      37,    38,     0,    39,  -229,     0,    40,    41,    42,     0,
      43,    44,    45,   -43,    46,    47,     0,    48,    49,    50,
       0,     0,     0,    51,    52,    53,    54,     0,    56,    57,
    -229,   -43,    58,    59,    60,  -551,    61,    62,    63,  -551,
      64,    65,    66,    67,    68,    69,    70,    71,     0,    72,
      73,     0,    74,    75,    76,    77,    78,     0,    79,    80,
     -61,    81,    82,     0,     0,    83,     0,    84,     0,     0,
    -551,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -551,  -551,    88,  -551,
    -551,  -551,  -551,  -551,  -551,  -551,     0,  -551,  -551,  -551,
    -551,  -551,     0,  -551,  -551,  -551,  -551,  -551,  -551,  -551,
    -551,    95,  -551,  -551,  -551,     0,    97,  -551,    98,   300,
      99,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,    16,     0,    17,    18,
      19,    20,    21,    22,    23,    24,    25,     0,    26,    27,
       0,    28,     0,    29,    30,    31,    32,    33,    34,    35,
      36,     0,    37,    38,     0,    39,  -229,     0,    40,    41,
      42,     0,    43,    44,    45,   -43,    46,    47,     0,    48,
      49,    50,     0,     0,     0,    51,    52,    53,    54,    55,
      56,    57,  -229,   -43,    58,    59,    60,     0,    61,    62,
      63,     0,    64,    65,    66,    67,    68,    69,    70,    71,
       0,    72,    73,     0,    74,    75,    76,    77,    78,     0,
      79,    80,   -61,    81,    82,     0,     0,    83,     0,    84,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,   264,    95,     0,     0,    96,     0,    97,   301,
      98,     4,    99,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,    16,     0,
      17,    18,    19,    20,    21,    22,    23,    24,    25,     0,
      26,    27,     0,    28,     0,    29,    30,    31,    32,    33,
      34,    35,    36,     0,    37,    38,     0,    39,  -229,     0,
      40,    41,    42,     0,    43,    44,    45,   -43,    46,    47,
       0,    48,    49,    50,     0,     0,     0,    51,    52,    53,
      54,    55,    56,    57,  -229,   -43,    58,    59,    60,     0,
      61,    62,    63,     0,    64,    65,    66,    67,    68,    69,
      70,    71,     0,    72,    73,     0,    74,    75,    76,    77,
      78,     0,    79,    80,   -61,    81,    82,     0,     0,    83,
       0,    84,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,     0,    95,     0,     0,    96,     0,
      97,   470,    98,   489,    99,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
      16,     0,    17,    18,    19,    20,    21,    22,    23,    24,
      25,     0,    26,    27,     0,    28,     0,    29,    30,    31,
      32,    33,    34,    35,    36,     0,    37,    38,     0,    39,
    -229,     0,    40,    41,    42,     0,    43,    44,    45,   -43,
      46,    47,     0,    48,    49,    50,     0,     0,     0,    51,
      52,    53,    54,    55,    56,    57,  -229,   -43,    58,    59,
      60,     0,    61,    62,    63,     0,    64,    65,    66,    67,
      68,    69,    70,    71,     0,    72,    73,     0,    74,    75,
      76,    77,    78,     0,    79,    80,   -61,    81,    82,     0,
       0,    83,     0,    84,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,     0,    95,     0,     0,
      96,     0,    97,   490,    98,   300,    99,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,    16,     0,    17,    18,    19,    20,    21,    22,
      23,    24,    25,     0,    26,    27,     0,    28,     0,    29,
      30,    31,    32,    33,    34,    35,    36,     0,    37,    38,
       0,    39,  -229,     0,    40,    41,    42,     0,    43,    44,
      45,   -43,    46,    47,     0,    48,    49,    50,     0,     0,
       0,    51,    52,    53,    54,    55,    56,    57,  -229,   -43,
      58,    59,    60,     0,    61,    62,    63,     0,    64,    65,
      66,    67,    68,    69,    70,    71,     0,    72,    73,     0,
      74,    75,    76,    77,    78,     0,    79,    80,   -61,    81,
      82,     0,     0,    83,     0,    84,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,     0,    95,
       0,     0,    96,     0,    97,   301,    98,     4,    99,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,    16,     0,    17,    18,    19,    20,
      21,    22,    23,    24,    25,     0,    26,    27,     0,    28,
       0,    29,    30,    31,    32,    33,    34,    35,    36,     0,
      37,    38,     0,    39,  -229,     0,    40,    41,    42,     0,
      43,    44,    45,   -43,    46,    47,     0,    48,    49,    50,
       0,     0,     0,    51,    52,    53,    54,    55,    56,    57,
    -229,   -43,    58,    59,    60,     0,    61,    62,    63,     0,
      64,    65,    66,    67,    68,    69,    70,    71,     0,    72,
      73,     0,    74,    75,    76,    77,    78,     0,    79,    80,
     -61,    81,    82,     0,     0,    83,     0,    84,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,    95,     0,     0,    96,     0,    97,   661,    98,     4,
      99,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,    16,     0,    17,    18,
      19,    20,    21,    22,    23,    24,    25,     0,    26,    27,
       0,    28,     0,    29,    30,    31,    32,    33,    34,    35,
      36,     0,    37,    38,     0,    39,  -229,     0,    40,    41,
      42,     0,    43,    44,    45,   -43,    46,    47,     0,    48,
      49,    50,     0,     0,     0,    51,    52,    53,    54,   315,
      56,    57,  -229,   -43,    58,    59,    60,     0,    61,    62,
      63,     0,    64,    65,    66,    67,    68,    69,    70,    71,
       0,    72,    73,     0,    74,    75,    76,    77,    78,     0,
      79,    80,   -61,    81,    82,     0,     0,    83,     0,    84,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,     0,    95,     0,     0,    96,     0,    97,     0,
      98,     4,    99,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,    16,     0,
      17,    18,    19,    20,    21,    22,    23,    24,    25,     0,
      26,    27,     0,    28,     0,    29,    30,    31,    32,    33,
      34,    35,    36,     0,    37,    38,     0,    39,  -229,     0,
      40,    41,    42,     0,    43,    44,    45,   -43,    46,    47,
       0,    48,    49,    50,     0,     0,     0,    51,    52,    53,
      54,     0,    56,    57,  -229,   -43,    58,    59,    60,     0,
      61,    62,    63,     0,    64,    65,    66,    67,    68,    69,
      70,    71,     0,    72,    73,     0,    74,    75,    76,    77,
      78,     0,    79,    80,   -61,    81,    82,     0,     0,    83,
       0,    84,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,     0,    95,     0,     0,    96,     0,
      97,     0,    98,     4,    99,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
      16,     0,    17,    18,    19,    20,    21,    22,    23,    24,
      25,     0,    26,    27,     0,    28,     0,    29,    30,    31,
      32,    33,    34,   581,    36,     0,    37,    38,     0,    39,
    -229,     0,    40,    41,    42,     0,    43,    44,    45,   -43,
      46,    47,     0,    48,    49,    50,     0,     0,     0,    51,
      52,    53,    54,     0,    56,    57,  -229,   -43,    58,    59,
      60,     0,    61,    62,    63,     0,    64,    65,    66,    67,
      68,    69,    70,    71,     0,    72,    73,     0,    74,    75,
      76,    77,    78,     0,    79,    80,   -61,    81,    82,     0,
       0,    83,     0,    84,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,     0,    95,     0,     0,
      96,     0,    97,     0,    98,     4,    99,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,    16,     0,    17,    18,    19,    20,    21,    22,
      23,    24,    25,     0,    26,    27,     0,    28,     0,    29,
      30,    31,    32,    33,    34,   587,    36,     0,    37,    38,
       0,    39,  -229,     0,    40,    41,    42,     0,    43,    44,
      45,   -43,    46,    47,     0,    48,    49,    50,     0,     0,
       0,    51,    52,    53,    54,     0,    56,    57,  -229,   -43,
      58,    59,    60,     0,    61,    62,    63,     0,    64,    65,
      66,    67,    68,    69,    70,    71,     0,    72,    73,     0,
      74,    75,    76,    77,    78,     0,    79,    80,   -61,    81,
      82,     0,     0,    83,     0,    84,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,     0,    95,
       0,     0,    96,     0,    97,     0,    98,     4,    99,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,    16,     0,    17,    18,    19,    20,
      21,    22,    23,    24,    25,     0,    26,    27,     0,    28,
       0,    29,    30,    31,    32,    33,    34,   856,    36,     0,
      37,    38,     0,    39,  -229,     0,    40,    41,    42,     0,
      43,    44,    45,   -43,    46,    47,     0,    48,    49,    50,
       0,     0,     0,    51,    52,    53,    54,     0,    56,    57,
    -229,   -43,    58,    59,    60,     0,    61,    62,    63,     0,
      64,    65,    66,    67,    68,    69,    70,    71,     0,    72,
      73,     0,    74,    75,    76,    77,    78,     0,    79,    80,
     -61,    81,    82,     0,     0,    83,     0,    84,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,    95,     0,     0,    96,     0,    97,     0,    98,     4,
      99,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,    16,     0,    17,    18,
      19,    20,    21,    22,    23,    24,    25,     0,    26,    27,
       0,    28,     0,    29,    30,    31,    32,    33,    34,   858,
      36,     0,    37,    38,     0,    39,  -229,     0,    40,    41,
      42,     0,    43,    44,    45,   -43,    46,    47,     0,    48,
      49,    50,     0,     0,     0,    51,    52,    53,    54,     0,
      56,    57,  -229,   -43,    58,    59,    60,     0,    61,    62,
      63,     0,    64,    65,    66,    67,    68,    69,    70,    71,
       0,    72,    73,     0,    74,    75,    76,    77,    78,     0,
      79,    80,   -61,    81,    82,     0,     0,    83,     0,    84,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,     0,    95,     0,     0,    96,     0,    97,     0,
      98,     4,    99,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,    16,     0,
      17,    18,    19,    20,    21,    22,    23,    24,    25,     0,
      26,    27,     0,    28,     0,    29,    30,    31,    32,    33,
      34,   863,    36,     0,    37,    38,     0,    39,  -229,     0,
      40,    41,    42,     0,    43,    44,    45,   -43,    46,    47,
       0,    48,    49,    50,     0,     0,     0,    51,    52,    53,
      54,     0,    56,    57,  -229,   -43,    58,    59,    60,     0,
      61,    62,    63,     0,    64,    65,    66,    67,    68,    69,
      70,    71,     0,    72,    73,     0,    74,    75,    76,    77,
      78,     0,    79,    80,   -61,    81,    82,     0,     0,    83,
       0,    84,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,     0,    95,     0,     0,    96,     0,
      97,     0,    98,     4,    99,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
      16,     0,    17,    18,    19,    20,    21,    22,    23,    24,
      25,     0,    26,    27,     0,    28,     0,    29,    30,    31,
      32,    33,    34,   866,    36,     0,    37,    38,     0,    39,
    -229,     0,    40,    41,    42,     0,    43,    44,    45,   -43,
      46,    47,     0,    48,    49,    50,     0,     0,     0,    51,
      52,    53,    54,     0,    56,    57,  -229,   -43,    58,    59,
      60,     0,    61,    62,    63,     0,    64,    65,    66,    67,
      68,    69,    70,    71,     0,    72,    73,     0,    74,    75,
      76,    77,    78,     0,    79,    80,   -61,    81,    82,     0,
       0,    83,     0,    84,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,     0,    95,     0,     0,
      96,     0,    97,     0,    98,     4,    99,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,    16,     0,    17,    18,    19,    20,    21,    22,
      23,    24,    25,     0,   886,    27,     0,    28,     0,    29,
      30,    31,    32,    33,    34,    35,    36,     0,    37,    38,
       0,    39,  -229,     0,    40,    41,    42,     0,    43,    44,
      45,   -43,    46,    47,     0,    48,    49,    50,     0,     0,
       0,    51,    52,    53,    54,     0,    56,    57,  -229,   -43,
      58,    59,    60,     0,    61,    62,    63,     0,    64,    65,
      66,    67,    68,    69,    70,    71,     0,    72,    73,     0,
      74,    75,    76,    77,    78,     0,    79,    80,   -61,    81,
      82,     0,     0,    83,     0,    84,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,     0,    95,
       0,     0,    96,     0,    97,     0,    98,     4,    99,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,    16,     0,    17,    18,    19,    20,
      21,    22,    23,    24,    25,     0,    26,    27,     0,    28,
       0,    29,    30,    31,    32,    33,    34,   892,    36,     0,
      37,    38,     0,    39,  -229,     0,    40,    41,    42,     0,
      43,    44,    45,   -43,    46,    47,     0,    48,    49,    50,
       0,     0,     0,    51,    52,    53,    54,     0,    56,    57,
    -229,   -43,    58,    59,    60,     0,    61,    62,    63,     0,
      64,    65,    66,    67,    68,    69,    70,    71,     0,    72,
      73,     0,    74,    75,    76,    77,    78,     0,    79,    80,
     -61,    81,    82,     0,     0,    83,     0,    84,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,    95,     0,     0,    96,     0,    97,     0,    98,     4,
      99,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,    16,     0,    17,    18,
      19,    20,    21,    22,    23,    24,    25,     0,    26,    27,
       0,    28,     0,    29,    30,    31,    32,    33,    34,   896,
      36,     0,    37,    38,     0,    39,  -229,     0,    40,    41,
      42,     0,    43,    44,    45,   -43,    46,    47,     0,    48,
      49,    50,     0,     0,     0,    51,    52,    53,    54,     0,
      56,    57,  -229,   -43,    58,    59,    60,     0,    61,    62,
      63,     0,    64,    65,    66,    67,    68,    69,    70,    71,
       0,    72,    73,     0,    74,    75,    76,    77,    78,     0,
      79,    80,   -61,    81,    82,     0,     0,    83,     0,    84,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,     0,    95,     0,     0,    96,     0,    97,     0,
      98,   753,    99,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   166,     0,     0,    14,    15,     0,    16,     0,
     167,     0,     0,    20,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   168,     0,     0,     0,    31,   169,   170,
       0,   171,    36,     0,    37,     0,     0,    39,     0,     0,
       0,    41,    42,     0,     0,    44,    45,     0,    46,    47,
       0,    48,    49,     0,     0,     0,     0,     0,    52,     0,
      54,     0,    56,     0,     0,     0,     0,    59,   172,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
      70,   173,     0,    72,     0,     0,    74,     0,     0,     0,
      78,     0,     0,    80,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,   264,     0,     0,     0,    96,     0,
     174,     0,    98,     0,   175,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   166,     0,     0,    14,    15,     0,
      16,     0,   167,     0,     0,    20,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   168,     0,     0,     0,    31,
     169,   170,     0,   171,    36,     0,    37,     0,     0,    39,
       0,     0,     0,    41,    42,     0,     0,    44,    45,     0,
      46,    47,     0,    48,    49,     0,     0,     0,     0,     0,
      52,     0,    54,     0,    56,     0,     0,     0,     0,    59,
     172,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,    70,   173,     0,    72,     0,     0,    74,     0,
       0,     0,    78,     0,     0,    80,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,   264,     0,     0,     0,
      96,     0,   174,     0,    98,     0,   175,   653,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   166,     0,     0,
      14,    15,     0,    16,     0,   167,     0,     0,    20,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   168,     0,
       0,     0,    31,   169,   170,     0,   171,    36,     0,    37,
       0,     0,    39,     0,     0,     0,    41,    42,     0,     0,
      44,    45,     0,    46,    47,     0,    48,    49,     0,     0,
       0,     0,     0,    52,     0,    54,     0,    56,     0,     0,
       0,     0,    59,   172,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,    70,   173,     0,    72,     0,
       0,    74,     0,     0,     0,    78,     0,     0,    80,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,   264,
       0,     0,     0,    96,     0,   174,     0,    98,     0,   175,
     993,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     166,     0,     0,    14,    15,     0,    16,     0,   167,     0,
       0,    20,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   168,     0,     0,     0,    31,   169,   170,     0,   171,
      36,     0,    37,     0,     0,    39,     0,     0,     0,    41,
      42,     0,     0,    44,    45,     0,    46,    47,     0,    48,
      49,     0,     0,     0,     0,     0,    52,     0,    54,     0,
      56,     0,     0,     0,     0,    59,   172,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,    70,   173,
       0,    72,     0,     0,    74,     0,     0,     0,    78,     0,
       0,    80,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,     0,     0,     0,     0,    96,     0,   174,     0,
      98,     0,   175,   658,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   166,     0,     0,    14,    15,     0,    16,
       0,   167,     0,     0,    20,   211,    22,     0,     0,     0,
       0,     0,    27,     0,   168,     0,     0,     0,    31,   169,
     170,     0,   171,    36,     0,    37,     0,     0,    39,     0,
       0,     0,    41,    42,     0,     0,    44,    45,     0,    46,
      47,     0,    48,    49,     0,     0,     0,     0,     0,    52,
      53,    54,     0,    56,     0,     0,     0,     0,    59,   172,
       0,    61,     0,     0,     0,     0,     0,    66,    67,    68,
      69,    70,   173,     0,    72,     0,     0,    74,     0,     0,
       0,    78,     0,     0,    80,     0,    81,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,     0,     0,     0,     0,    96,
       0,   174,     0,    98,     0,   175,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   166,     0,     0,    14,    15,
       0,    16,     0,   167,     0,     0,    20,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   168,     0,     0,     0,
      31,   169,   170,     0,   171,    36,     0,    37,     0,     0,
      39,     0,     0,     0,    41,    42,     0,     0,    44,    45,
       0,    46,    47,     0,    48,    49,     0,     0,     0,     0,
       0,    52,     0,    54,     0,    56,     0,     0,     0,     0,
      59,   172,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,    70,   173,     0,    72,     0,     0,    74,
     268,   269,     0,    78,   305,     0,    80,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,   306,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,   264,     0,     0,
       0,    96,     0,   174,     0,    98,     0,   175,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   166,     0,     0,
      14,    15,     0,    16,     0,   167,     0,     0,    20,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   168,     0,
       0,     0,    31,   169,   170,     0,   171,    36,     0,    37,
       0,     0,    39,     0,     0,     0,    41,    42,     0,     0,
      44,    45,     0,    46,    47,     0,    48,    49,     0,     0,
       0,     0,     0,    52,     0,    54,     0,    56,     0,     0,
       0,     0,    59,   172,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,    70,   173,     0,    72,     0,
       0,    74,   268,   269,     0,    78,   305,     0,    80,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,   264,
       0,     0,     0,    96,     0,   174,     0,    98,   645,   175,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   166,
       0,     0,    14,    15,     0,    16,     0,   167,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     168,     0,     0,     0,    31,   169,   170,     0,   171,    36,
       0,    37,     0,     0,    39,     0,     0,     0,    41,    42,
       0,     0,    44,    45,     0,    46,    47,     0,    48,    49,
       0,     0,     0,     0,     0,    52,     0,    54,     0,    56,
       0,     0,     0,     0,    59,   172,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,    70,   173,     0,
      72,     0,     0,    74,   268,   269,     0,    78,   305,     0,
      80,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,   264,     0,     0,     0,    96,     0,   174,     0,    98,
     647,   175,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   166,     0,     0,    14,    15,     0,    16,     0,   167,
       0,     0,    20,     0,   546,     0,     0,     0,     0,     0,
      27,     0,   168,     0,     0,     0,    31,   169,   170,     0,
     171,    36,   547,    37,     0,     0,    39,     0,     0,     0,
      41,    42,     0,     0,    44,    45,     0,    46,    47,     0,
      48,    49,     0,     0,     0,     0,     0,    52,     0,    54,
       0,    56,     0,     0,     0,     0,    59,   172,     0,   548,
       0,     0,     0,     0,     0,    66,    67,    68,    69,    70,
     173,     0,    72,     0,     0,    74,     0,     0,     0,    78,
       0,     0,    80,     0,   549,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,   785,     0,   786,     0,    96,     0,   174,
       0,    98,   787,   175,     0,     0,     0,   788,   218,   789,
     790,     0,     0,     0,     0,   791,     0,   258,     0,     0,
       0,    21,    22,   221,     0,     0,     0,     0,     0,   792,
     259,     0,    29,   260,   793,     0,   224,    34,     0,   794,
       0,   795,    38,     0,     0,  -229,     0,   796,     0,     0,
       0,     0,     0,   797,   798,     0,     0,     0,     0,     0,
     229,     0,     0,     0,    51,     0,    53,     0,     0,   799,
       0,  -229,     0,     0,     0,   261,     0,    61,   231,   232,
       0,   800,     0,   234,     0,   801,     0,     0,   802,     0,
       0,     0,   803,     0,     0,   238,    77,     0,   804,    79,
       0,   -63,    81,     0,     0,     0,     0,   359,   360,   361,
     362,   363,   364,     0,     0,   367,   368,   369,   370,     0,
     372,   373,   805,   806,   807,   808,   809,     0,     0,   810,
       0,     0,     0,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,   821,     0,   822,     0,     0,   823,   824,
     825,   826,     0,     0,   827,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   166,     0,     0,    14,    15,     0,
      16,     0,   167,     0,     0,    20,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   168,     0,     0,     0,    31,
     169,   170,     0,   171,    36,     0,    37,     0,     0,    39,
       0,     0,     0,    41,    42,     0,     0,    44,    45,     0,
      46,    47,     0,    48,    49,     0,     0,     0,     0,     0,
      52,     0,    54,     0,    56,     0,     0,     0,     0,    59,
     172,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,    70,   173,     0,    72,     0,     0,    74,   268,
     269,     0,    78,     0,     0,    80,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,     0,   270,     0,     0,
      96,     0,   174,     0,    98,     0,   175,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   166,     0,     0,    14,
      15,     0,    16,     0,   167,     0,     0,    20,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   168,     0,     0,
       0,    31,   169,   170,     0,   171,    36,     0,    37,     0,
       0,    39,     0,     0,     0,    41,    42,     0,     0,    44,
      45,     0,    46,    47,     0,    48,    49,     0,     0,     0,
       0,     0,    52,     0,    54,     0,    56,     0,     0,     0,
       0,    59,   172,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,    70,   173,     0,    72,     0,     0,
      74,   268,   269,     0,    78,     0,     0,    80,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,   264,     0,
       0,     0,    96,     0,   174,     0,    98,     0,   175,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   166,     0,
       0,    14,    15,     0,    16,     0,   167,     0,     0,    20,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   168,
       0,     0,     0,    31,   169,   170,     0,   171,    36,     0,
      37,     0,     0,    39,     0,     0,     0,    41,    42,     0,
       0,    44,    45,     0,    46,    47,   755,    48,    49,     0,
       0,     0,     0,     0,    52,     0,    54,     0,    56,     0,
       0,     0,     0,    59,   172,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,    70,   173,     0,    72,
       0,     0,    74,   268,   269,     0,    78,     0,     0,    80,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,     0,     0,     0,    96,     0,   174,     0,    98,     0,
     175,     5,     6,     7,     8,   200,    10,   201,     0,     0,
     166,     0,     0,    14,    15,     0,    16,     0,   167,     0,
       0,    20,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   168,     0,     0,     0,    31,   169,   170,     0,   171,
      36,     0,    37,     0,     0,    39,     0,     0,     0,    41,
      42,     0,     0,    44,    45,     0,    46,    47,     0,    48,
      49,     0,     0,     0,     0,     0,    52,     0,    54,     0,
      56,     0,     0,     0,     0,    59,   202,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,    70,   173,
       0,    72,     0,     0,    74,     0,     0,   203,    78,     0,
       0,    80,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,     0,     0,     0,     0,    96,     0,   174,     0,
      98,     0,   175,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   166,     0,     0,    14,    15,     0,    16,     0,
     167,     0,     0,    20,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   168,     0,     0,     0,    31,   169,   170,
       0,   171,    36,     0,    37,     0,     0,    39,     0,     0,
       0,    41,    42,     0,     0,    44,    45,     0,    46,    47,
       0,    48,    49,     0,     0,     0,     0,     0,    52,   205,
      54,     0,    56,     0,     0,     0,     0,    59,   172,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
      70,   173,     0,    72,     0,     0,    74,     0,     0,     0,
      78,     0,     0,    80,     0,     0,    82,     0,     0,     0,
       0,     0,   206,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,     0,     0,     0,     0,    96,     0,
     174,     0,    98,     0,   175,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   166,     0,     0,    14,    15,     0,
      16,     0,   167,     0,     0,    20,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   168,     0,     0,     0,    31,
     169,   170,     0,   171,    36,     0,    37,     0,     0,    39,
       0,     0,     0,    41,    42,     0,     0,    44,    45,     0,
      46,    47,     0,    48,    49,     0,     0,     0,     0,     0,
      52,     0,    54,     0,    56,     0,     0,     0,     0,    59,
     172,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,    70,   173,     0,    72,     0,     0,    74,     0,
       0,     0,    78,     0,     0,    80,     0,     0,    82,     0,
       0,     0,     0,     0,   206,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,   264,     0,     0,     0,
      96,     0,   174,     0,    98,     0,   175,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   166,     0,     0,    14,
      15,     0,    16,     0,   167,     0,     0,    20,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   168,     0,     0,
       0,    31,   169,   170,     0,   171,    36,     0,    37,     0,
       0,    39,     0,     0,     0,    41,    42,     0,     0,    44,
      45,     0,    46,    47,     0,    48,    49,     0,     0,     0,
       0,     0,    52,     0,    54,     0,    56,     0,     0,     0,
       0,    59,   172,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,    70,   173,     0,    72,     0,     0,
      74,   268,   269,     0,    78,     0,     0,    80,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,     0,     0,
       0,     0,    96,     0,   174,     0,    98,     0,   175,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   166,     0,
       0,    14,    15,     0,    16,     0,   167,     0,     0,    20,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   168,
       0,     0,     0,    31,   169,   170,     0,   171,    36,     0,
      37,     0,     0,    39,     0,     0,     0,    41,    42,     0,
       0,    44,    45,     0,    46,    47,     0,    48,    49,     0,
       0,     0,     0,     0,    52,     0,    54,     0,    56,     0,
       0,     0,     0,    59,   172,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,    70,   173,     0,    72,
       0,     0,    74,     0,     0,     0,    78,     0,     0,    80,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
     264,     0,     0,     0,    96,     0,   174,   643,    98,     0,
     175,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     166,     0,     0,    14,    15,     0,    16,     0,   167,     0,
       0,    20,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   168,     0,     0,     0,    31,   169,   170,     0,   171,
      36,     0,    37,     0,     0,    39,     0,     0,     0,    41,
      42,     0,     0,    44,    45,     0,    46,    47,     0,    48,
      49,     0,     0,     0,     0,     0,    52,     0,    54,     0,
      56,     0,     0,     0,     0,    59,   172,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,    70,   173,
       0,    72,     0,     0,    74,     0,     0,     0,    78,     0,
       0,    80,     0,     0,    82,     0,     0,     0,     0,     0,
     206,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,     0,     0,     0,     0,    96,     0,   174,     0,
      98,     0,   175,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   166,     0,     0,    14,    15,     0,    16,     0,
     167,     0,     0,    20,     0,     0,     0,     0,     0,     0,
     247,    27,     0,   168,     0,     0,     0,    31,   169,   170,
       0,   171,    36,     0,    37,     0,     0,    39,     0,     0,
       0,    41,    42,     0,     0,    44,    45,     0,    46,    47,
       0,    48,    49,     0,     0,     0,     0,     0,    52,     0,
      54,     0,    56,     0,     0,     0,     0,    59,   172,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
      70,   173,     0,    72,     0,     0,    74,     0,     0,     0,
      78,     0,     0,    80,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,     0,     0,     0,     0,    96,     0,
      97,     0,    98,     0,   175,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   166,     0,     0,    14,    15,     0,
      16,     0,   167,     0,     0,    20,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   168,     0,     0,     0,    31,
     169,   170,     0,   171,    36,     0,    37,     0,     0,    39,
       0,     0,     0,    41,    42,     0,     0,    44,    45,     0,
      46,    47,     0,    48,    49,     0,     0,     0,     0,     0,
      52,     0,    54,     0,    56,     0,     0,     0,     0,    59,
     172,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,    70,   173,     0,    72,     0,     0,    74,     0,
       0,     0,    78,     0,     0,    80,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,   264,     0,     0,     0,
      96,     0,   174,     0,    98,     0,   175,     5,     6,     7,
       8,   200,    10,     0,     0,     0,   166,     0,     0,    14,
      15,     0,    16,     0,   167,     0,     0,    20,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   168,     0,     0,
       0,    31,   169,   170,     0,   171,    36,     0,    37,     0,
       0,    39,     0,     0,     0,    41,    42,     0,     0,    44,
      45,     0,    46,    47,     0,    48,    49,     0,     0,     0,
       0,     0,    52,     0,    54,     0,    56,     0,     0,     0,
       0,    59,   202,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,    70,   173,     0,    72,     0,     0,
      74,     0,     0,   203,    78,     0,     0,    80,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,     0,     0,
       0,     0,    96,     0,   174,     0,    98,     0,   175,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   166,     0,
       0,    14,    15,     0,    16,     0,   167,     0,     0,    20,
       0,     0,     0,     0,     0,     0,     0,   619,     0,   168,
       0,     0,     0,    31,   169,   170,     0,   171,    36,     0,
     620,     0,     0,    39,     0,     0,     0,    41,    42,     0,
       0,    44,    45,     0,    46,    47,     0,    48,    49,     0,
       0,     0,     0,     0,    52,     0,    54,     0,    56,     0,
       0,     0,     0,    59,   172,     0,     0,     0,     0,     0,
       0,     0,    66,   621,    68,    69,    70,   622,     0,    72,
       0,     0,    74,     0,     0,     0,    78,     0,     0,    80,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
     264,     0,     0,     0,    96,     0,   174,     0,    98,     0,
     945,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     166,     0,     0,    14,    15,     0,    16,     0,   167,     0,
       0,    20,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   168,     0,     0,     0,    31,   169,   170,     0,   171,
      36,     0,    37,     0,     0,    39,     0,     0,     0,    41,
      42,     0,     0,    44,    45,     0,    46,    47,     0,    48,
      49,     0,     0,     0,     0,     0,    52,     0,    54,     0,
      56,     0,     0,     0,     0,    59,   172,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,    70,   173,
       0,    72,     0,     0,    74,     0,     0,     0,    78,     0,
       0,    80,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,   264,     0,     0,     0,    96,     0,   174,     0,
      98,     0,   945,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   166,     0,     0,    14,    15,     0,    16,     0,
     167,     0,     0,    20,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   168,     0,     0,     0,    31,   169,   170,
       0,   928,    36,     0,    37,     0,     0,    39,     0,     0,
       0,    41,    42,     0,     0,    44,    45,     0,    46,    47,
       0,    48,    49,     0,     0,     0,     0,     0,    52,     0,
      54,     0,    56,     0,     0,     0,     0,    59,   172,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
      70,   173,     0,    72,     0,     0,    74,     0,     0,     0,
      78,     0,     0,    80,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,   264,     0,     0,     0,    96,     0,
     174,     0,    98,     0,   175,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   166,     0,     0,    14,    15,     0,
      16,     0,   167,     0,     0,    20,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   168,     0,     0,     0,    31,
     169,   170,     0,   171,    36,     0,    37,     0,     0,    39,
       0,     0,     0,    41,    42,     0,     0,    44,    45,     0,
      46,    47,     0,    48,    49,     0,     0,     0,     0,     0,
      52,     0,    54,     0,    56,     0,     0,     0,     0,    59,
     172,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,    70,   173,     0,    72,     0,     0,    74,     0,
       0,     0,    78,     0,     0,    80,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,     0,     0,     0,     0,
      96,     0,   174,     0,    98,     0,   175,     5,     6,     7,
       8,   196,    10,     0,     0,     0,   166,     0,     0,    14,
      15,     0,    16,     0,   167,     0,     0,    20,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   168,     0,     0,
       0,    31,   169,   170,     0,   171,    36,     0,    37,     0,
       0,    39,     0,     0,     0,    41,    42,     0,     0,    44,
      45,     0,    46,    47,     0,    48,    49,     0,     0,     0,
       0,     0,    52,     0,    54,     0,    56,     0,     0,     0,
       0,    59,   197,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,    70,   173,     0,    72,     0,     0,
      74,     0,     0,     0,    78,     0,     0,    80,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,     0,     0,
       0,     0,    96,     0,   174,     0,    98,     0,   175,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   166,     0,
       0,    14,    15,     0,    16,     0,   167,     0,     0,    20,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   168,
       0,     0,     0,    31,   169,   170,     0,   171,    36,     0,
      37,     0,     0,    39,     0,     0,     0,    41,    42,     0,
       0,    44,    45,     0,    46,    47,     0,    48,    49,     0,
       0,     0,     0,     0,    52,     0,    54,     0,    56,     0,
       0,     0,     0,    59,   172,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,    70,   173,     0,    72,
       0,     0,    74,     0,     0,     0,    78,     0,     0,    80,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,     0,     0,     0,    96,     0,    97,     0,    98,     0,
     175,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     166,     0,     0,    14,    15,     0,    16,     0,   167,     0,
       0,    20,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   168,     0,     0,     0,    31,   169,   170,     0,   171,
      36,     0,    37,     0,     0,    39,     0,     0,     0,    41,
      42,     0,     0,    44,    45,     0,    46,    47,     0,    48,
      49,     0,     0,     0,     0,     0,    52,     0,    54,     0,
      56,     0,     0,     0,     0,    59,   172,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,    70,   173,
       0,    72,     0,     0,    74,     0,     0,     0,    78,     0,
       0,    80,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,     0,     0,     0,     0,   592,     0,   174,     0,
      98,     0,   175,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   166,     0,     0,    14,    15,     0,    16,     0,
     167,     0,     0,    20,     0,     0,     0,     0,     0,     0,
       0,   619,     0,   168,     0,     0,     0,    31,   169,   170,
       0,   171,    36,     0,   620,     0,     0,    39,     0,     0,
       0,    41,    42,     0,     0,    44,    45,     0,    46,    47,
       0,    48,    49,     0,     0,     0,     0,     0,    52,     0,
      54,     0,    56,     0,     0,     0,     0,    59,   172,     0,
       0,     0,     0,     0,     0,     0,    66,   621,    68,    69,
      70,   622,     0,    72,     0,     0,    74,     0,     0,     0,
      78,     0,     0,    80,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,     0,     0,     0,     0,    96,     0,
     174,     0,    98,     0,   623,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   166,     0,     0,    14,    15,     0,
      16,     0,   167,     0,     0,    20,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   168,     0,     0,     0,    31,
     169,   170,     0,   171,    36,     0,    37,     0,     0,    39,
       0,     0,     0,    41,    42,     0,     0,    44,    45,     0,
      46,    47,     0,    48,    49,     0,     0,     0,     0,     0,
      52,     0,    54,     0,    56,     0,     0,     0,     0,    59,
     172,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,    70,   173,     0,    72,     0,     0,    74,     0,
       0,     0,    78,     0,     0,    80,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,     0,     0,     0,     0,
      96,     0,   174,     0,    98,     0,   623,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   166,     0,     0,    14,
      15,     0,    16,     0,   167,     0,     0,    20,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   168,     0,     0,
       0,    31,   169,   170,     0,   693,    36,     0,    37,     0,
       0,    39,     0,     0,     0,    41,    42,     0,     0,    44,
      45,     0,    46,    47,     0,    48,    49,     0,     0,     0,
       0,     0,    52,     0,    54,     0,    56,     0,     0,     0,
       0,    59,   172,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,    70,   173,     0,    72,     0,     0,
      74,     0,     0,     0,    78,     0,     0,    80,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,     0,     0,
       0,     0,    96,     0,   174,     0,    98,     0,   175,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   166,     0,
       0,    14,    15,     0,    16,     0,   167,     0,     0,    20,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   168,
       0,     0,     0,    31,   169,   170,     0,   696,    36,     0,
      37,     0,     0,    39,     0,     0,     0,    41,    42,     0,
       0,    44,    45,     0,    46,    47,     0,    48,    49,     0,
       0,     0,     0,     0,    52,     0,    54,     0,    56,     0,
       0,     0,     0,    59,   172,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,    70,   173,     0,    72,
       0,     0,    74,     0,     0,     0,    78,     0,     0,    80,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,     0,     0,     0,    96,     0,   174,     0,    98,     0,
     175,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     166,     0,     0,    14,    15,     0,    16,     0,   167,     0,
       0,    20,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   168,     0,     0,     0,    31,   169,   170,     0,   922,
      36,     0,    37,     0,     0,    39,     0,     0,     0,    41,
      42,     0,     0,    44,    45,     0,    46,    47,     0,    48,
      49,     0,     0,     0,     0,     0,    52,     0,    54,     0,
      56,     0,     0,     0,     0,    59,   172,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,    70,   173,
       0,    72,     0,     0,    74,     0,     0,     0,    78,     0,
       0,    80,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,     0,     0,     0,     0,    96,     0,   174,     0,
      98,     0,   175,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   166,     0,     0,    14,    15,     0,    16,     0,
     167,     0,     0,    20,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   168,     0,     0,     0,    31,   169,   170,
       0,   923,    36,     0,    37,     0,     0,    39,     0,     0,
       0,    41,    42,     0,     0,    44,    45,     0,    46,    47,
       0,    48,    49,     0,     0,     0,     0,     0,    52,     0,
      54,     0,    56,     0,     0,     0,     0,    59,   172,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
      70,   173,     0,    72,     0,     0,    74,     0,     0,     0,
      78,     0,     0,    80,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,     0,     0,     0,     0,    96,     0,
     174,     0,    98,     0,   175,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   166,     0,     0,    14,    15,     0,
      16,     0,   167,     0,     0,    20,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   168,     0,     0,     0,    31,
     169,   170,     0,   925,    36,     0,    37,     0,     0,    39,
       0,     0,     0,    41,    42,     0,     0,    44,    45,     0,
      46,    47,     0,    48,    49,     0,     0,     0,     0,     0,
      52,     0,    54,     0,    56,     0,     0,     0,     0,    59,
     172,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,    70,   173,     0,    72,     0,     0,    74,     0,
       0,     0,    78,     0,     0,    80,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,     0,     0,     0,     0,
      96,     0,   174,     0,    98,     0,   175,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   166,     0,     0,    14,
      15,     0,    16,     0,   167,     0,     0,    20,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   168,     0,     0,
       0,    31,   169,   170,     0,   926,    36,     0,    37,     0,
       0,    39,     0,     0,     0,    41,    42,     0,     0,    44,
      45,     0,    46,    47,     0,    48,    49,     0,     0,     0,
       0,     0,    52,     0,    54,     0,    56,     0,     0,     0,
       0,    59,   172,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,    70,   173,     0,    72,     0,     0,
      74,     0,     0,     0,    78,     0,     0,    80,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,     0,     0,
       0,     0,    96,     0,   174,     0,    98,     0,   175,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   166,     0,
       0,    14,    15,     0,    16,     0,   167,     0,     0,    20,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   168,
       0,     0,     0,    31,   169,   170,     0,   927,    36,     0,
      37,     0,     0,    39,     0,     0,     0,    41,    42,     0,
       0,    44,    45,     0,    46,    47,     0,    48,    49,     0,
       0,     0,     0,     0,    52,     0,    54,     0,    56,     0,
       0,     0,     0,    59,   172,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,    70,   173,     0,    72,
       0,     0,    74,     0,     0,     0,    78,     0,     0,    80,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,     0,     0,     0,    96,     0,   174,     0,    98,     0,
     175,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     166,     0,     0,    14,    15,     0,    16,     0,   167,     0,
       0,    20,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   168,     0,     0,     0,    31,   169,   170,     0,   928,
      36,     0,    37,     0,     0,    39,     0,     0,     0,    41,
      42,     0,     0,    44,    45,     0,    46,    47,     0,    48,
      49,     0,     0,     0,     0,     0,    52,     0,    54,     0,
      56,     0,     0,     0,     0,    59,   172,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,    70,   173,
       0,    72,     0,     0,    74,     0,     0,     0,    78,     0,
       0,    80,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,     0,     0,     0,     0,    96,     0,   174,     0,
      98,     0,   175,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   166,     0,     0,    14,    15,     0,    16,     0,
     167,     0,     0,    20,     0,     0,     0,     0,     0,     0,
       0,   619,     0,   168,     0,     0,     0,    31,   169,   170,
       0,   171,    36,     0,   620,     0,     0,    39,     0,     0,
       0,    41,    42,     0,     0,    44,    45,     0,    46,    47,
       0,    48,    49,     0,     0,     0,     0,     0,    52,     0,
      54,     0,    56,     0,     0,     0,     0,    59,   172,     0,
       0,     0,     0,     0,     0,     0,    66,   621,    68,    69,
      70,   622,     0,    72,     0,     0,    74,     0,     0,     0,
      78,     0,     0,    80,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,     0,     0,     0,     0,    96,     0,
     174,     0,    98,     0,  1012,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   166,     0,     0,    14,    15,     0,
      16,     0,   167,     0,     0,    20,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   168,     0,     0,     0,    31,
     169,   170,     0,   171,    36,     0,    37,     0,     0,    39,
       0,     0,     0,    41,    42,     0,     0,    44,    45,     0,
      46,    47,     0,    48,    49,     0,     0,     0,     0,     0,
      52,     0,    54,     0,    56,     0,     0,     0,     0,    59,
     172,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,    70,   173,     0,    72,     0,     0,    74,     0,
       0,     0,    78,     0,     0,    80,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,     0,     0,     0,     0,
      96,     0,     0,     0,    98,     0,  1012,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   166,     0,     0,    14,
      15,     0,    16,     0,   167,     0,     0,    20,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   168,     0,     0,
       0,    31,   169,   170,     0,   171,    36,     0,    37,     0,
       0,    39,     0,     0,     0,    41,    42,     0,     0,    44,
      45,     0,    46,    47,     0,    48,    49,     0,     0,     0,
       0,     0,    52,     0,    54,     0,    56,     0,     0,     0,
       0,    59,   172,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,    70,   173,     0,    72,     0,     0,
      74,     0,     0,     0,    78,     0,     0,    80,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,  -395,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
    -395,     0,     0,     0,  -395,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
       0,     0,     0,     0,   174,     0,    98,  -395,   945,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   166,     0,
       0,    14,    15,     0,    16,     0,   167,     0,     0,    20,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   168,
       0,     0,     0,    31,   169,   170,     0,   171,    36,     0,
      37,     0,     0,    39,     0,     0,     0,    41,    42,  -377,
       0,    44,    45,     0,    46,    47,     0,    48,    49,     0,
       0,     0,     0,     0,    52,     0,    54,     0,    56,     0,
       0,     0,     0,    59,   172,     0,     0,     0,  -377,     0,
       0,     0,    66,    67,    68,    69,    70,   173,     0,    72,
       0,  -377,    74,     0,     0,     0,    78,     0,     0,    80,
       0,     0,    82,     0,  -377,     0,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,     0,     0,     0,     0,     0,     0,     0,    88,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   166,     0,
       0,    14,    15,     0,    16,     0,   167,     0,     0,    20,
     374,  -377,     0,  -525,     0,  -525,  -377,    27,    98,   168,
    1012,     0,     0,    31,   169,   170,     0,   171,    36,     0,
      37,     0,     0,    39,     0,     0,     0,    41,    42,     0,
       0,    44,    45,     0,    46,    47,     0,    48,    49,     0,
       0,     0,     0,     0,    52,     0,    54,     0,    56,     0,
       0,     0,     0,    59,   172,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,    70,   173,     0,    72,
       0,     0,    74,     0,     0,     0,    78,     0,     0,    80,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   166,     0,
       0,    14,    15,     0,    16,     0,   167,     0,     0,    20,
       0,     0,     0,     0,     0,     0,   174,    27,    98,   168,
     175,     0,     0,    31,   169,   170,     0,   171,    36,     0,
      37,     0,     0,    39,     0,     0,     0,    41,    42,     0,
       0,    44,    45,     0,    46,    47,     0,    48,    49,     0,
       0,     0,     0,     0,    52,     0,    54,     0,    56,     0,
       0,     0,     0,    59,   172,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,    70,   173,     0,    72,
       0,     0,    74,     0,     0,     0,    78,     0,     0,    80,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   166,     0,
       0,    14,    15,     0,    16,     0,   167,     0,     0,    20,
       0,     0,     0,     0,     0,     0,   174,    27,    98,   168,
     623,     0,     0,    31,   169,   170,     0,   171,    36,     0,
      37,     0,     0,    39,     0,     0,     0,    41,    42,     0,
       0,    44,    45,     0,    46,    47,     0,    48,    49,     0,
       0,     0,     0,     0,    52,     0,    54,     0,    56,     0,
       0,     0,     0,    59,   172,     0,     0,     0,   216,     0,
       0,     0,    66,    67,    68,    69,    70,   173,     0,    72,
     217,   218,    74,   219,     0,     0,    78,     0,   220,    80,
       0,     0,    82,     0,     0,     0,   221,     0,     0,     0,
       0,     0,   222,     0,     0,     0,     0,   223,     0,   224,
       0,     0,   225,     0,     0,     0,     0,     0,    88,     0,
     226,     0,     0,     0,     0,   216,   227,   228,     0,     0,
       0,     0,     0,   229,     0,     0,     0,   217,   218,     0,
     219,     0,   230,     0,     0,   220,     0,     0,    98,     0,
     175,   231,   232,   221,   233,     0,   234,     0,   235,   222,
       0,   236,     0,     0,   223,   237,   224,     0,   238,   225,
       0,   239,     0,     0,     0,     0,     0,   226,     0,     0,
       0,     0,     0,   227,   228,     0,     0,     0,     0,     0,
     229,     0,     0,     0,     0,     0,     0,     0,     0,   230,
       0,     0,     0,     0,     0,     0,     0,     0,   231,   232,
       0,   233,     0,   234,     0,   235,     0,   973,   236,     0,
       0,     0,   237,     0,   787,   238,     0,   243,   239,   217,
     218,   789,   219,     0,     0,     0,     0,   220,     0,     0,
       0,     0,     0,     0,     0,   221,     0,     0,     0,     0,
       0,   792,     0,     0,     0,     0,   223,     0,   224,     0,
       0,   225,     0,   795,     0,     0,     0,     0,     0,   226,
       0,     0,     0,     0,     0,   797,   228,     0,     0,     0,
     330,     0,   229,     0,   743,     0,     0,   331,     0,     0,
       0,   230,     0,     0,     0,     0,     0,     0,     0,   332,
     231,   232,     0,   233,     0,   234,     0,   974,     0,     0,
     802,     0,     0,     0,   237,     0,     0,   238,     0,     0,
     239,     0,     0,     0,     0,     0,     0,     0,     0,   359,
     360,   361,   362,   363,   364,     0,     0,   367,   368,   369,
     370,   332,   372,   373,   805,   806,   807,   808,   809,   333,
       0,   810,     0,   334,     0,   811,   812,   813,   814,   815,
     816,   817,   818,   819,   820,   821,     0,   822,   330,     0,
     823,   824,   825,   826,     0,   331,     0,     0,     0,     0,
     418,     0,     0,     0,   335,     0,     0,   332,     0,     0,
       0,   333,     0,     0,     0,   334,     0,     0,     0,     0,
     336,   337,     0,   338,   339,   340,     0,   341,   342,   343,
       0,   344,   345,   346,   347,   348,     0,   349,   350,   351,
     352,   353,   354,   355,     0,   330,   356,   357,   358,     0,
       0,     0,   331,     0,     0,   771,     0,   333,     0,     0,
       0,   334,   336,   337,   332,   338,   339,   340,     0,   341,
     342,   343,     0,   344,   345,   346,   347,     0,     0,   349,
     350,   351,   352,   353,     0,   355,   330,     0,   356,   357,
     358,     0,   335,   331,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   332,     0,     0,   336,   337,
       0,   338,   339,   340,   333,   341,   342,   343,   334,   344,
     345,   346,   347,   348,     0,   349,   350,   351,   352,   353,
     354,   355,   330,     0,   356,   357,   358,     0,     0,   331,
       0,     0,     0,   847,     0,     0,     0,     0,     0,   335,
       0,   332,     0,     0,     0,   333,     0,     0,     0,   334,
       0,     0,     0,     0,     0,   336,   337,     0,   338,   339,
     340,     0,   341,   342,   343,     0,   344,   345,   346,   347,
     348,     0,   349,   350,   351,   352,   353,   354,   355,   330,
     335,   356,   357,   358,     0,     0,   331,     0,     0,     0,
    1008,   333,     0,     0,     0,   334,   336,   337,   332,   338,
     339,   340,     0,   341,   342,   343,     0,   344,   345,   346,
     347,   348,     0,   349,   350,   351,   352,   353,   354,   355,
       0,     0,   356,   357,   358,     0,   335,     0,     0,     0,
       0,  1075,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   336,   337,     0,   338,   339,   340,   333,   341,
     342,   343,   334,   344,   345,   346,   347,   348,     0,   349,
     350,   351,   352,   353,   354,   355,   330,     0,   356,   357,
     358,     0,     0,   331,     0,   644,     0,     0,     0,     0,
       0,     0,     0,   335,     0,   332,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   336,
     337,     0,   338,   339,   340,     0,   341,   342,   343,     0,
     344,   345,   346,   347,   348,     0,   349,   350,   351,   352,
     353,   354,   355,     0,     0,   356,   357,   358,     0,     0,
       0,     0,   972,     0,     0,   333,     0,     0,     0,   334,
       0,     0,   330,     0,     0,     0,     0,     0,   428,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   332,     0,     0,     0,     0,     0,     0,     0,     0,
     335,   359,   360,   361,   362,   363,   364,     0,     0,   367,
     368,   369,   370,     0,   372,   373,   336,   337,     0,   338,
     339,   340,     0,   341,   342,   343,     0,   344,   345,   346,
     347,   348,     0,   349,   350,   351,   352,   353,   354,   355,
       0,   333,   356,   357,   358,   334,   387,     0,   330,     0,
       0,     0,     0,     0,   855,   331,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   332,     0,     0,
       0,     0,     0,     0,     0,     0,   335,   359,   360,   361,
     362,   363,   364,     0,     0,   367,   368,   369,   370,     0,
     372,   373,   336,   337,     0,   338,   339,   340,     0,   341,
     342,   343,     0,   344,   345,   346,   347,   348,     0,   349,
     350,   351,   352,   353,   354,   355,     0,   333,   356,   357,
     358,   334,   387,     0,   330,     0,     0,     0,     0,     0,
     862,   331,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   332,     0,     0,     0,     0,     0,     0,
       0,     0,   335,   359,   360,   361,   362,   363,   364,     0,
       0,   367,   368,   369,   370,     0,   372,   373,   336,   337,
       0,   338,   339,   340,     0,   341,   342,   343,     0,   344,
     345,   346,   347,   348,     0,   349,   350,   351,   352,   353,
     354,   355,     0,   333,   356,   357,   358,   334,   387,     0,
     330,     0,     0,     0,     0,     0,   985,   331,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   332,
       0,     0,     0,     0,     0,     0,     0,     0,   335,   359,
     360,   361,   362,   363,   364,     0,     0,   367,   368,   369,
     370,     0,   372,   373,   336,   337,     0,   338,   339,   340,
       0,   341,   342,   343,     0,   344,   345,   346,   347,   348,
       0,   349,   350,   351,   352,   353,   354,   355,     0,   333,
     356,   357,   358,   334,   387,     0,   330,     0,     0,     0,
       0,     0,   986,   331,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   332,     0,     0,     0,     0,
       0,     0,     0,     0,   335,   359,   360,   361,   362,   363,
     364,     0,     0,   367,   368,   369,   370,     0,   372,   373,
     336,   337,     0,   338,   339,   340,     0,   341,   342,   343,
       0,   344,   345,   346,   347,   348,     0,   349,   350,   351,
     352,   353,   354,   355,     0,   333,   356,   357,   358,   334,
     387,     0,   330,     0,     0,     0,     0,     0,   987,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   332,     0,     0,     0,     0,     0,     0,     0,     0,
     335,   359,   360,   361,   362,   363,   364,     0,     0,   367,
     368,   369,   370,     0,   372,   373,   336,   337,     0,   338,
     339,   340,     0,   341,   342,   343,     0,   344,   345,   346,
     347,   348,     0,   349,   350,   351,   352,   353,   354,   355,
       0,   333,   356,   357,   358,   334,   387,     0,   330,     0,
       0,     0,     0,     0,   988,   331,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   332,     0,     0,
       0,     0,     0,     0,     0,     0,   335,   359,   360,   361,
     362,   363,   364,     0,     0,   367,   368,   369,   370,     0,
     372,   373,   336,   337,     0,   338,   339,   340,     0,   341,
     342,   343,     0,   344,   345,   346,   347,   348,     0,   349,
     350,   351,   352,   353,   354,   355,     0,   333,   356,   357,
     358,   334,   387,     0,   330,     0,     0,     0,     0,     0,
    1010,   331,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   332,     0,     0,     0,     0,     0,     0,
       0,     0,   335,   359,   360,   361,   362,   363,   364,     0,
       0,   367,   368,   369,   370,     0,   372,   373,   336,   337,
       0,   338,   339,   340,     0,   341,   342,   343,     0,   344,
     345,   346,   347,   348,     0,   349,   350,   351,   352,   353,
     354,   355,     0,   333,   356,   357,   358,   334,   387,     0,
       0,     0,     0,     0,     0,     0,  1011,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   335,   359,
     360,   361,   362,   363,   364,     0,     0,   367,   368,   369,
     370,     0,   372,   373,   336,   337,     0,   338,   339,   340,
       0,   341,   342,   343,     0,   344,   345,   346,   347,   348,
       0,   349,   350,   351,   352,   353,   354,   355,   330,     0,
     356,   357,   358,     0,   387,   331,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   332,   421,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     330,   422,     0,     0,     0,     0,     0,   331,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   332,
     415,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   416,     0,     0,     0,   333,     0,     0,
       0,   334,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   330,     0,     0,
       0,     0,     0,     0,   331,     0,     0,     0,   418,   333,
       0,     0,   335,   334,     0,     0,   332,   726,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   336,   337,
       0,   338,   339,   340,     0,   341,   342,   343,     0,   344,
     345,   346,   347,   348,   335,   349,   350,   351,   352,   353,
     354,   355,     0,     0,   356,   357,   358,     0,   387,     0,
     336,   337,     0,   338,   339,   340,   333,   341,   342,   343,
     334,   344,   345,   346,   347,   348,     0,   349,   350,   351,
     352,   353,   354,   355,   330,     0,   356,   357,   358,     0,
     387,   331,     0,     0,     0,     0,     0,   418,     0,     0,
       0,   335,     0,   332,   247,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   336,   337,     0,
     338,   339,   340,     0,   341,   342,   343,     0,   344,   345,
     346,   347,   348,     0,   349,   350,   351,   352,   353,   354,
     355,   330,     0,   356,   357,   358,     0,   387,   331,     0,
       0,     0,     0,   333,     0,     0,     0,   334,     0,     0,
     332,   718,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   330,     0,     0,     0,     0,     0,     0,
     331,     0,     0,     0,     0,     0,     0,     0,   335,     0,
       0,     0,   332,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   336,   337,     0,   338,   339,   340,
     333,   341,   342,   343,   334,   344,   345,   346,   347,   348,
       0,   349,   350,   351,   352,   353,   354,   355,     0,     0,
     356,   357,   358,     0,   387,     0,     0,     0,     0,     0,
       0,     0,   333,     0,     0,   335,   334,     0,     0,     0,
       0,     0,     0,     0,     0,   736,     0,     0,     0,     0,
       0,   336,   337,     0,   338,   339,   340,     0,   341,   342,
     343,     0,   344,   345,   346,   347,   348,   335,   349,   350,
     351,   352,   353,   354,   355,     0,     0,   356,   357,   358,
       0,   387,     0,   336,   337,     0,   338,   339,   340,     0,
     341,   342,   343,     0,   344,   345,   346,   347,   348,     0,
     349,   350,   351,   352,   353,   354,   355,   330,     0,   356,
     357,   358,     0,   387,   331,     0,  -210,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   332,     0,  -210,  -210,
       0,  -210,     0,     0,     0,     0,  -210,     0,     0,   330,
       0,     0,     0,     0,  -210,     0,   331,     0,     0,     0,
    -210,     0,     0,     0,     0,  -210,     0,  -210,   332,     0,
    -210,     0,     0,   425,     0,     0,     0,     0,  -210,     0,
       0,     0,     0,     0,  -210,  -210,   333,     0,     0,     0,
     334,  -210,     0,     0,     0,     0,     0,     0,     0,     0,
    -210,     0,     0,   426,     0,     0,   330,     0,     0,  -210,
    -210,     0,  -210,   331,  -210,     0,  -210,     0,   333,  -210,
       0,   335,   334,  -210,     0,   332,  -210,     0,     0,  -210,
       0,     0,     0,     0,     0,     0,     0,   336,   337,   389,
     338,   339,   340,     0,   341,   342,   343,     0,   344,   345,
     346,   347,   348,   335,   349,   350,   351,   352,   353,   354,
     355,     0,     0,   356,   357,   358,     0,   455,     0,   336,
     337,     0,   338,   339,   340,   333,   341,   342,   343,   334,
     344,   345,   346,   347,   348,     0,   349,   350,   351,   352,
     353,   354,   355,   330,   427,   356,   357,   358,     0,     0,
     331,     0,     0,     0,     0,     0,   161,     0,     0,     0,
     335,     0,   332,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   336,   337,     0,   338,
     339,   340,     0,   341,   342,   343,     0,   344,   345,   346,
     347,   348,     0,   349,   350,   351,   352,   353,   354,   355,
     330,     0,   356,   357,   358,     0,     0,   331,     0,     0,
       0,     0,   333,     0,     0,     0,   334,     0,     0,   332,
     555,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   330,   556,     0,     0,     0,     0,     0,   331,
       0,     0,     0,     0,     0,     0,     0,   335,     0,     0,
       0,   332,   557,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   336,   337,   558,   338,   339,   340,   333,
     341,   342,   343,   334,   344,   345,   346,   347,   348,     0,
     349,   350,   351,   352,   353,   354,   355,     0,   459,   356,
     357,   358,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   333,     0,     0,   335,   334,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     336,   337,     0,   338,   339,   340,     0,   341,   342,   343,
       0,   344,   345,   346,   347,   348,   335,   349,   350,   351,
     352,   353,   354,   355,     0,     0,   356,   357,   358,     0,
       0,     0,   336,   337,     0,   338,   339,   340,     0,   341,
     342,   343,     0,   344,   345,   346,   347,   348,     0,   349,
     350,   351,   352,   353,   354,   355,   330,   258,   356,   357,
     358,    21,    22,   331,     0,     0,     0,     0,     0,     0,
     259,     0,    29,   260,     0,   332,     0,    34,     0,     0,
     664,     0,    38,     0,     0,  -229,     0,     0,   330,     0,
       0,     0,     0,     0,     0,   331,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,    53,   332,     0,     0,
     665,  -229,     0,     0,     0,   261,     0,    61,     0,     0,
       0,     0,     0,     0,     0,   333,     0,     0,     0,   334,
       0,     0,     0,     0,     0,     0,    77,   330,     0,    79,
       0,   -62,    81,     0,   331,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   332,   333,     0,     0,
     335,   334,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   336,   337,     0,   338,
     339,   340,     0,   341,   342,   343,     0,   344,   345,   346,
     347,   348,   335,   349,   350,   351,   352,   353,   354,   355,
       0,     0,   356,   357,   358,     0,   333,     0,   336,   337,
     334,   338,   339,   340,     0,   341,   342,   343,     0,   344,
     345,   346,   347,   348,     0,   349,   350,   351,   352,   353,
     354,   355,   330,   461,   356,   357,   358,     0,     0,   331,
       0,   335,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   332,     0,     0,     0,     0,     0,   336,   337,     0,
     338,   339,   340,     0,   341,   342,   343,     0,   344,   345,
     346,   347,   348,     0,   349,   350,   351,   352,   353,   354,
     355,   330,   463,   356,   357,   358,     0,     0,   331,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     332,   333,     0,     0,     0,   334,     0,     0,     0,     0,
       0,     0,     0,   330,     0,     0,     0,     0,     0,     0,
     331,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   332,     0,     0,     0,   335,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     333,     0,   336,   337,   334,   338,   339,   340,     0,   341,
     342,   343,     0,   344,   345,   346,   347,   348,     0,   349,
     350,   351,   352,   353,   354,   355,     0,   468,   356,   357,
     358,     0,   333,     0,   485,   335,   334,     0,     0,     0,
       0,     0,     0,     0,     0,   559,     0,     0,     0,     0,
       0,   336,   337,     0,   338,   339,   340,     0,   341,   342,
     343,     0,   344,   345,   346,   347,   348,   335,   349,   350,
     351,   352,   353,   354,   355,     0,     0,   356,   357,   358,
       0,     0,     0,   336,   337,     0,   338,   339,   340,     0,
     341,   342,   343,     0,   344,   345,   346,   347,   348,     0,
     349,   350,   351,   352,   353,   354,   355,   330,     0,   356,
     357,   358,     0,     0,   331,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   332,     0,   258,     0,
       0,     0,    21,    22,     0,     0,     0,     0,     0,     0,
       0,   259,     0,    29,   260,     0,     0,     0,    34,     0,
       0,     0,   330,    38,     0,     0,     0,     0,     0,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   332,     0,     0,     0,    51,   333,    53,     0,    55,
     334,   904,     0,     0,   905,     0,   261,     0,    61,     0,
       0,     0,     0,   330,     0,     0,     0,     0,     0,     0,
     331,     0,     0,     0,     0,     0,     0,    77,     0,     0,
      79,   335,   332,    81,     0,     0,     0,     0,     0,     0,
       0,   333,     0,     0,     0,   334,     0,   336,   337,     0,
     338,   339,   340,     0,   341,   342,   343,     0,   344,   345,
     346,   347,   348,  -499,   349,   350,   351,   352,   353,   354,
     355,     0,   161,   356,   357,   358,   335,     0,     0,     0,
       0,     0,   333,    95,     0,     0,   334,     0,     0,   932,
       0,     0,   336,   337,     0,   338,   339,   340,     0,   341,
     342,   343,     0,   344,   345,   346,   347,   348,     0,   349,
     350,   351,   352,   353,   354,   355,     0,   335,   356,   357,
     358,     0,     0,     0,   330,   731,     0,     0,     0,     0,
       0,   331,     0,   336,   337,     0,   338,   339,   340,     0,
     341,   342,   343,   332,   344,   345,   346,   347,   348,     0,
     349,   350,   351,   352,   353,   354,   355,     0,   703,   356,
     357,   358,     0,     0,   330,     0,     0,     0,     0,     0,
       0,   331,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   332,     0,     0,   734,     0,     0,     0,
       0,     0,     0,   333,     0,     0,     0,   334,     0,     0,
       0,     0,     0,     0,     0,   330,     0,     0,     0,     0,
       0,     0,   331,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   332,     0,     0,     0,   335,     0,
       0,     0,     0,   333,     0,     0,     0,   334,     0,     0,
       0,     0,     0,     0,   336,   337,     0,   338,   339,   340,
       0,   341,   342,   343,     0,   344,   345,   346,   347,   348,
       0,   349,   350,   351,   352,   353,   354,   355,   335,     0,
     356,   357,   358,     0,   333,     0,     0,     0,   334,     0,
       0,     0,     0,     0,   336,   337,     0,   338,   339,   340,
       0,   341,   342,   343,     0,   344,   345,   346,   347,   348,
       0,   349,   350,   351,   352,   353,   354,   355,   774,   335,
     356,   357,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   336,   337,     0,   338,   339,
     340,     0,   341,   342,   343,     0,   344,   345,   346,   347,
     348,     0,   349,   350,   351,   352,   353,   354,   355,   330,
       0,   356,   357,   358,     0,     0,   331,     0,  -220,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,   842,
    -220,  -220,     0,  -220,     0,     0,     0,     0,  -220,     0,
       0,   330,     0,     0,     0,     0,  -220,     0,   331,     0,
       0,     0,  -220,     0,     0,     0,     0,  -220,     0,  -220,
     332,   845,  -220,     0,     0,     0,     0,     0,     0,     0,
    -220,     0,     0,     0,     0,     0,  -220,  -220,   333,     0,
       0,     0,   334,  -220,     0,     0,     0,     0,     0,     0,
       0,     0,  -220,     0,     0,     0,     0,     0,     0,     0,
       0,  -220,  -220,     0,  -220,     0,  -220,     0,  -220,     0,
     333,  -220,     0,   335,   334,  -220,     0,     0,  -220,     0,
       0,  -220,     0,     0,     0,     0,     0,     0,     0,   336,
     337,     0,   338,   339,   340,     0,   341,   342,   343,     0,
     344,   345,   346,   347,   348,   335,   349,   350,   351,   352,
     353,   354,   355,     0,     0,   356,   357,   358,     0,     0,
       0,   336,   337,     0,   338,   339,   340,     0,   341,   342,
     343,     0,   344,   345,   346,   347,   348,     0,   349,   350,
     351,   352,   353,   354,   355,   330,     0,   356,   357,   358,
       0,     0,   331,     0,   216,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   332,     0,   217,   218,     0,   219,
       0,     0,     0,     0,   220,     0,     0,   330,     0,     0,
       0,     0,   221,     0,   331,     0,     0,     0,   222,     0,
       0,     0,     0,   223,     0,   224,   332,     0,   225,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
       0,     0,   227,   228,   333,     0,     0,     0,   334,   229,
       0,     0,     0,     0,     0,     0,     0,   921,   230,     0,
       0,     0,     0,     0,   330,   940,     0,   231,   232,     0,
     233,   331,   234,     0,   235,     0,   333,   236,     0,   335,
     334,   237,     0,   332,   238,     0,     0,   239,     0,   924,
       0,     0,     0,     0,     0,   336,   337,     0,   338,   339,
     340,     0,   341,   342,   343,     0,   344,   345,   346,   347,
     348,   335,   349,   350,   351,   352,   353,   354,   355,     0,
       0,   356,   357,   358,     0,     0,     0,   336,   337,     0,
     338,   339,   340,   333,   341,   342,   343,   334,   344,   345,
     346,   347,   348,     0,   349,   350,   351,   352,   353,   354,
     355,   330,     0,   356,   357,   358,     0,     0,   331,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   335,     0,
     332,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   336,   337,     0,   338,   339,   340,
       0,   341,   342,   343,     0,   344,   345,   346,   347,   348,
       0,   349,   350,   351,   352,   353,   354,   355,     0,     0,
     356,   357,   358,     0,     0,     0,     0,     0,     0,     0,
     333,     0,     0,     0,   334,     0,     0,   330,     0,     0,
       0,     0,     0,  1021,   331,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   332,     0,     0,     0,
       0,     0,     0,     0,     0,   335,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   336,   337,     0,   338,   339,   340,     0,   341,   342,
     343,     0,   344,   345,   346,   347,   348,     0,   349,   350,
     351,   352,   353,   354,   355,     0,   333,   356,   357,   358,
     334,     0,     0,   330,     0,     0,     0,     0,     0,  1022,
     331,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   332,     0,     0,     0,     0,     0,     0,     0,
       0,   335,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   336,   337,     0,
     338,   339,   340,     0,   341,   342,   343,     0,   344,   345,
     346,   347,   348,     0,   349,   350,   351,   352,   353,   354,
     355,     0,   333,   356,   357,   358,   334,     0,     0,   330,
       0,     0,     0,     0,     0,  1023,   331,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,     0,
       0,     0,     0,     0,     0,     0,     0,   335,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   336,   337,     0,   338,   339,   340,     0,
     341,   342,   343,     0,   344,   345,   346,   347,   348,     0,
     349,   350,   351,   352,   353,   354,   355,     0,   333,   356,
     357,   358,   334,     0,     0,   330,     0,     0,     0,     0,
       0,  1024,   331,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   332,     0,     0,     0,     0,     0,
       0,     0,     0,   335,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   336,
     337,     0,   338,   339,   340,     0,   341,   342,   343,     0,
     344,   345,   346,   347,   348,     0,   349,   350,   351,   352,
     353,   354,   355,     0,   333,   356,   357,   358,   334,     0,
       0,   330,     0,     0,     0,     0,     0,  1025,   331,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     332,     0,     0,     0,     0,     0,     0,     0,     0,   335,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   336,   337,     0,   338,   339,
     340,     0,   341,   342,   343,     0,   344,   345,   346,   347,
     348,     0,   349,   350,   351,   352,   353,   354,   355,     0,
     333,   356,   357,   358,   334,     0,     0,   330,     0,     0,
       0,     0,     0,  1026,   331,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   332,     0,     0,     0,
       0,     0,     0,     0,     0,   335,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1048,   336,   337,     0,   338,   339,   340,     0,   341,   342,
     343,     0,   344,   345,   346,   347,   348,     0,   349,   350,
     351,   352,   353,   354,   355,     0,   333,   356,   357,   358,
     334,     0,     0,   330,     0,     0,     0,     0,     0,     0,
     331,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   332,     0,     0,     0,     0,     0,     0,     0,
       0,   335,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   336,   337,     0,
     338,   339,   340,     0,   341,   342,   343,     0,   344,   345,
     346,   347,   348,     0,   349,   350,   351,   352,   353,   354,
     355,     0,   333,   356,   357,   358,   334,     0,     0,   330,
       0,     0,     0,     0,     0,     0,   331,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,     0,
       0,     0,     0,     0,     0,     0,     0,   335,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   336,   337,     0,   338,   339,   340,     0,
     341,   342,   343,     0,   344,   345,   346,   347,   348,     0,
     349,   350,   351,   352,   353,   354,   355,     0,   691,   356,
     357,   358,   334,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   335,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   336,
     337,     0,   338,   339,   340,     0,   341,   342,   343,     0,
     344,   345,   346,   347,   348,     0,   349,   350,   351,   352,
     353,   354,   355,     0,   258,   356,   357,   358,    21,    22,
       0,     0,     0,     0,     0,     0,     0,   259,   258,    29,
     260,     0,    21,    22,    34,     0,     0,     0,     0,    38,
       0,   259,     0,    29,   260,     0,     0,     0,    34,     0,
       0,     0,   332,    38,     0,     0,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,   904,     0,     0,
     905,     0,   261,     0,    61,    51,     0,    53,     0,    55,
       0,   904,     0,     0,   905,     0,   261,     0,    61,     0,
       0,     0,   332,    77,     0,     0,    79,     0,     0,    81,
       0,     0,   333,     0,     0,     0,   334,    77,     0,     0,
      79,     0,     0,    81,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   335,     0,     0,
       0,     0,   333,     0,     0,     0,   334,     0,     0,    95,
       0,     0,     0,   336,   337,   983,   338,   339,   340,     0,
     341,   342,   343,    95,   344,   345,   346,   347,     0,   984,
     349,   350,   351,   352,   353,   354,   355,   335,     0,   356,
     357,   358,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   336,   337,     0,   338,   339,   340,     0,
     341,   342,   343,     0,   344,   345,   346,   347,     0,     0,
     349,   350,   351,   352,   353,   668,   355,   216,     0,   356,
     357,   358,     0,     0,     0,     0,     0,     0,     0,   217,
     218,     0,   219,     0,     0,     0,     0,   220,     0,     0,
       0,     0,     0,     0,     0,   221,     0,     0,     0,     0,
       0,   222,     0,     0,     0,     0,   223,     0,   224,     0,
       0,   225,     0,     0,     0,     0,     0,     0,     0,   226,
       0,     0,     0,     0,  -211,   227,   228,     0,     0,     0,
       0,     0,   229,     0,     0,     0,  -211,  -211,     0,  -211,
       0,   230,     0,     0,  -211,     0,     0,     0,     0,     0,
     231,   232,  -211,   233,     0,   234,     0,   235,  -211,     0,
     236,     0,     0,  -211,   237,  -211,     0,   238,  -211,     0,
     239,     0,     0,     0,     0,     0,  -211,     0,     0,     0,
       0,     0,  -211,  -211,     0,     0,     0,     0,     0,  -211,
       0,     0,     0,     0,     0,     0,     0,     0,  -211,     0,
       0,     0,     0,     0,     0,     0,     0,  -211,  -211,     0,
    -211,     0,  -211,     0,  -211,     0,     0,  -211,     0,     0,
       0,  -211,     0,     0,  -211,     0,     0,  -211
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-936)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,    62,   146,     2,    15,   380,   133,   464,    19,    57,
      58,   210,   187,   585,    25,   426,   752,     1,    29,    30,
     677,    32,    33,    34,    35,   441,    30,    18,   203,     1,
     618,    40,    43,    21,    45,    46,   215,    30,    99,    50,
      65,    52,     1,    54,   436,    30,     1,    30,   464,   875,
     878,    62,    63,    64,    65,    66,    67,   955,   993,   404,
      71,   188,    73,   408,    75,    76,   110,    82,    77,    80,
      43,   708,    83,    84,   711,    86,    30,    88,   257,     3,
      91,    92,   107,    94,   110,    63,    97,    98,    99,   832,
     833,    53,    43,    65,    53,    82,    43,   778,    53,   433,
     279,   837,   690,  1038,   136,   136,   110,    80,     1,    43,
     199,     3,   311,   312,   175,   204,   688,   126,   944,   128,
      98,   165,   130,    82,   195,   110,   155,    82,  1038,   161,
     138,   115,   161,    80,   165,   107,   161,     0,     3,   165,
      95,   969,   286,   879,   215,   160,   108,    32,    33,   108,
      43,   135,   136,   108,    43,   166,   160,  1055,   169,   170,
     171,   749,   173,   174,   175,  1075,   155,   160,   155,    93,
     851,   108,   853,   160,    43,   160,   135,   160,   187,   163,
     135,    43,   136,   110,   195,   136,   257,   136,   197,   161,
      43,   136,   136,   202,   203,   186,   205,   376,   377,   136,
     155,   160,   136,    48,   215,   160,   160,   943,   279,   162,
     585,   199,   161,   850,    99,   852,   204,    82,   163,   163,
      25,   110,   115,   211,   136,   617,   907,   572,    93,    74,
     575,   165,   975,   976,   243,  1053,    25,    25,    43,    75,
      76,    46,   135,   136,   115,    25,   257,   136,   929,   260,
     931,   163,    80,  1071,   665,   264,   992,   268,   269,   320,
     917,    66,   155,   136,    69,    45,    71,   136,   279,   140,
     136,   342,   136,    79,   136,    80,   165,    83,   457,   406,
      69,    69,   136,   136,   411,   162,     7,    67,   860,    69,
     163,    80,    80,    90,    99,   306,   165,   163,    79,   163,
      80,   155,    83,   165,   303,   376,   377,    29,   155,   320,
      99,   162,   165,   688,   161,    25,   105,   105,   329,   330,
     331,   332,   333,   334,   335,   105,   337,   338,   339,   340,
     341,   136,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   130,    43,
     136,   743,   413,   162,   365,   366,   138,    79,   163,    69,
     371,    83,   160,   374,   375,   376,   377,   378,   379,   380,
      80,    65,   136,   136,    75,   104,   136,   163,   389,   136,
      53,    75,    76,   394,   155,   396,   457,   365,   366,    99,
     589,   590,   155,   371,   403,   155,   374,   375,   407,   163,
      53,    53,   413,    25,   415,   416,   163,   741,   130,    82,
     421,   422,   115,   135,   425,   426,   138,   428,   154,   430,
     142,   136,    95,   162,   162,   164,   164,   436,   885,    82,
      82,   155,   443,   136,   495,   108,   155,   140,   136,   450,
     155,     2,    95,    95,   778,   162,   457,    69,   333,   334,
      11,   146,   651,   652,   136,   108,   108,   155,    80,   136,
     163,   472,   878,    24,   475,    26,   477,   642,   155,   885,
     481,   482,   483,   155,   485,   486,   136,    99,   155,   162,
      25,   164,   155,   492,   495,   860,   155,   160,   497,   155,
       3,   185,   491,   378,   379,   155,   155,   475,    43,   477,
     571,    46,   155,   155,   389,    18,   155,   160,   160,   115,
      71,   138,   160,   207,   208,   209,   210,   851,    31,   853,
     214,    66,    43,   162,    69,   164,    71,   475,   109,   477,
     136,   416,    53,   677,   140,    80,    97,   422,   155,    78,
     101,    78,    55,   162,   555,   556,   557,   558,   559,   560,
     561,   162,   155,   969,    99,   162,   250,   163,   569,   253,
     571,    82,   623,   115,   155,     7,   155,    57,   579,   135,
     581,   562,   563,   162,   585,   164,   587,   993,   912,   155,
      93,   275,   135,   160,   115,   138,    99,   108,   163,   142,
     136,   569,  1008,   155,   115,   929,   481,   931,   115,   293,
      43,   162,   162,    32,   613,   163,   136,   163,   617,   115,
     621,   622,   623,   163,   163,   136,   627,    19,   136,   160,
      25,   165,  1038,   165,   141,   163,   143,   144,   162,   638,
     147,   148,   155,   642,   155,   155,   155,  1053,    43,   160,
     155,    46,   163,   155,   999,   136,   155,    32,     2,   627,
     160,   165,   160,   664,   665,  1071,   155,    11,   155,  1075,
     163,    66,   163,   161,    69,   136,    71,   161,   677,   115,
      24,   556,    26,   558,    80,    80,   163,   688,    98,   240,
     691,   692,   693,    29,   165,   696,   247,  1042,  1043,  1044,
    1045,  1046,  1047,    30,    99,    30,   390,   160,   759,   160,
      32,   135,    80,   165,   135,   716,   165,   718,   165,   161,
     155,   155,   723,   162,   138,   726,   138,    71,   729,   138,
     731,    75,    76,   734,   138,   138,   420,   130,   138,   130,
     424,   740,    32,    79,   743,   130,   163,    83,   138,   138,
     138,   138,   303,    97,   138,   138,   130,   101,   759,   130,
      95,   163,   317,   130,   763,   130,   317,   165,   769,   665,
     771,   664,   136,   774,    23,   287,   763,   781,   917,   780,
     912,   741,   781,   917,   784,   195,   610,  1055,   992,  1017,
    1048,  1041,    34,   444,   130,   131,   571,   133,   134,   135,
      29,   137,   138,    -1,    -1,   215,   142,    -1,   807,    -1,
      -1,    -1,    -1,    -1,   150,    -1,   691,   692,   162,    -1,
     156,   157,   158,    -1,    -1,    -1,   827,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   387,    -1,    -1,    -1,
     841,   842,    -1,   844,   845,   846,   847,   257,    -1,    -1,
      -1,    -1,    -1,    -1,   855,   856,    -1,   858,    -1,   860,
      -1,   862,   863,    -1,   415,   866,   904,   905,    -1,   279,
     421,    -1,    -1,    -1,    -1,    -1,    -1,   428,    -1,    -1,
      -1,    -1,    -1,   884,   885,    -1,    -1,    29,    -1,   890,
      -1,   892,    -1,    -1,   945,   896,   240,    -1,    -1,   583,
     584,    -1,    -1,   247,    -1,   589,   590,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   601,   917,    -1,
     921,   922,   923,   924,   925,   926,   927,   928,   479,    -1,
      -1,    -1,   483,    -1,    -1,    -1,    -1,    79,    -1,   940,
     491,    83,    -1,    -1,   945,    -1,    -1,    -1,    -1,   950,
      -1,    -1,    -1,   954,   638,    -1,    -1,    -1,    -1,   303,
      -1,  1012,    -1,    -1,    -1,    -1,   376,   377,    -1,    -1,
      -1,    -1,    -1,   317,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   985,   986,   987,   988,   130,   131,
      -1,    -1,   134,   135,    -1,   137,   138,    -1,    -1,    -1,
     142,   552,    -1,    29,    -1,    -1,    -1,  1008,   150,  1010,
    1011,  1012,    -1,    -1,   156,   157,   158,   701,   702,    -1,
    1021,  1022,  1023,  1024,  1025,  1026,    -1,    -1,    -1,   713,
      -1,   715,    -1,    -1,    -1,    -1,  1037,  1038,   722,  1040,
     450,   725,    29,   387,   728,    -1,    -1,   457,    -1,   600,
      -1,    -1,    -1,    79,     2,    -1,    -1,    83,    -1,    -1,
      -1,    -1,   472,    11,    -1,   475,    -1,   477,    -1,    -1,
    1071,   415,   482,    -1,  1075,    -1,    24,   421,    26,    -1,
      -1,    -1,   766,    -1,   428,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,     2,    -1,    -1,    83,    65,    -1,   650,
      -1,    -1,    11,    -1,   130,   656,    -1,    -1,    -1,   135,
      -1,   137,   138,    -1,    -1,    24,   142,    26,    -1,    -1,
      -1,    -1,    -1,    71,   150,    -1,   677,    -1,    -1,    -1,
     156,   157,   158,    -1,    -1,   479,    -1,    -1,    -1,   483,
     691,   692,    -1,   130,   131,    -1,    -1,   491,   135,    97,
     137,   138,    -1,   101,    -1,   142,    -1,    -1,    -1,   569,
      -1,   571,    71,   150,    -1,   716,    -1,   718,    -1,   156,
     157,   158,   723,    -1,     2,   726,    -1,    -1,    -1,    -1,
      -1,    -1,   733,    11,    -1,   736,    -1,    -1,    97,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    -1,    -1,   888,    -1,    -1,   758,    29,    -1,
      -1,    -1,    20,    -1,   162,    -1,    24,    25,   769,    -1,
     771,    -1,    -1,    -1,    -1,    33,    -1,    35,    36,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    -1,   207,
     208,   209,   210,    71,    -1,    -1,    -1,    -1,    -1,    -1,
     934,   935,    -1,   162,   938,   939,   600,    -1,    79,    67,
      -1,    69,    83,    71,    -1,    73,    -1,    -1,    76,    97,
      78,   955,    80,   101,    -1,    -1,    -1,   961,    -1,   963,
      -1,   965,   250,    -1,    -1,   253,    -1,    -1,    -1,    -1,
      -1,    99,   240,    43,   102,    -1,    -1,   105,    -1,   247,
      -1,    -1,    -1,    53,   855,    -1,   650,   275,    -1,   130,
      -1,   862,   656,    -1,   135,    -1,   137,   138,    -1,    -1,
      -1,   142,    -1,   874,    -1,   293,    -1,    -1,    -1,   150,
      -1,   240,    82,    -1,   162,   886,    -1,   158,   247,    -1,
      -1,    -1,    -1,    -1,   895,    -1,    -1,   155,   899,    -1,
      -1,    -1,    43,   161,    -1,   303,    -1,    -1,   108,    -1,
      -1,    -1,    53,    -1,    -1,   115,    -1,    -1,    -1,   317,
      -1,  1055,   716,    -1,   718,    -1,    -1,    -1,    -1,   723,
      -1,    -1,   726,    -1,    -1,    -1,   136,    -1,    -1,   733,
     941,    82,   736,    -1,   303,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   317,    -1,
     160,     3,   240,   163,   758,    -1,    -1,   108,    -1,   247,
      -1,    -1,   390,    15,   115,   769,    18,   771,    -1,    -1,
      -1,    23,    -1,    -1,   985,   986,   987,   988,    -1,   387,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      42,    -1,   420,    -1,    -1,    47,   424,    -1,    -1,  1010,
    1011,    -1,    -1,    55,   155,    -1,    -1,   415,   878,   160,
      62,    -1,   163,   421,    -1,   303,    -1,    -1,   387,    20,
     428,    -1,    -1,    24,    25,    77,    -1,    -1,    -1,   317,
      -1,    -1,    33,     2,    35,    36,    -1,    89,    -1,    40,
      -1,    93,    11,    -1,    45,    -1,   415,    -1,   100,    -1,
      -1,   855,   421,    -1,   106,    24,    -1,    26,   862,   428,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,
     874,   479,    -1,     1,    -1,   483,    -1,    78,    -1,    80,
     950,    -1,   886,   491,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   895,    20,    -1,    -1,   899,    24,    25,    99,   387,
      -1,   102,    71,    -1,   105,    33,    -1,    35,    36,    -1,
     479,    -1,    40,    -1,   483,    -1,    -1,    45,    -1,    -1,
      48,    -1,   491,   993,    -1,    -1,    -1,   415,    97,    -1,
      -1,    -1,   101,   421,    -1,    -1,    -1,   941,    -1,    67,
     428,    69,    -1,    71,    -1,    73,    74,    -1,    76,    -1,
      78,    -1,    80,    -1,    -1,   583,   584,    -1,    -1,    -1,
      -1,   589,   590,    -1,    -1,    -1,    -1,    -1,  1038,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   985,   986,   987,   988,    -1,    -1,    -1,    -1,    -1,
      -1,   479,   600,   162,    -1,   483,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   491,    -1,  1075,  1010,  1011,    -1,    20,
      -1,    -1,    -1,    24,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    36,    -1,   155,    -1,    40,
      -1,   600,    -1,   161,    45,    -1,    -1,     2,    -1,    -1,
      -1,    -1,   650,    -1,    -1,    -1,    11,    -1,   656,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    24,
      71,    26,    73,    -1,    -1,    76,    -1,    78,    -1,    80,
      -1,   240,    -1,   701,   702,    -1,    -1,    -1,   247,    -1,
      -1,   650,    -1,    -1,    -1,   713,    -1,   656,    99,     2,
      -1,   102,    -1,    -1,   105,    -1,    -1,   725,    11,    -1,
     728,    -1,    -1,    -1,    -1,    -1,    71,    -1,   716,    -1,
     718,    24,   600,    26,    -1,   723,    -1,    -1,   726,    -1,
      -1,    -1,    -1,    -1,    -1,   733,    -1,    -1,   736,    -1,
      -1,    -1,    97,    -1,   303,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   716,   317,   718,
     758,    -1,    -1,    -1,   723,    -1,    -1,   726,    71,    -1,
      -1,   769,   650,   771,   733,    -1,    -1,   736,   656,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,   101,   758,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,
     769,    -1,   771,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,   387,    -1,
      24,    25,    -1,    -1,    -1,    -1,    -1,    -1,   716,    33,
     718,    35,    36,    -1,    -1,   723,    40,    -1,   726,    -1,
      -1,    45,    -1,    -1,    -1,   733,   415,   855,   736,   162,
      -1,    -1,   421,    -1,   862,    -1,    -1,    -1,    -1,   428,
     888,    -1,    -1,    67,    -1,    69,   874,    71,    -1,    73,
     758,    -1,    76,    -1,    78,   240,    80,    -1,   886,    -1,
      -1,   769,   247,   771,    -1,    -1,   855,   895,    -1,    -1,
      -1,   899,    -1,   862,    -1,    99,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   874,    -1,    -1,    -1,    -1,
     479,    -1,    -1,    -1,   483,    -1,    -1,   886,    -1,    -1,
      -1,    -1,   491,    -1,    -1,    -1,   895,   240,    -1,    -1,
     899,    -1,    -1,   941,   247,    -1,    -1,    -1,   303,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,   317,    -1,    -1,    -1,    -1,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   855,    -1,    -1,
      -1,    -1,   941,    -1,   862,    -1,    -1,   985,   986,   987,
     988,    -1,    -1,    -1,    -1,    -1,   874,    -1,    -1,    -1,
     303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   886,    -1,
      -1,    -1,  1010,  1011,   317,    -1,    -1,   895,    -1,    -1,
      -1,   899,    -1,    -1,     2,    -1,   985,   986,   987,   988,
      -1,    -1,   387,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   600,    29,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,  1010,  1011,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     415,    -1,    -1,   941,    -1,    -1,   421,    -1,    -1,    -1,
      -1,    -1,    -1,   428,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   387,    -1,    -1,    -1,    -1,    -1,
      -1,   650,    79,    71,    -1,    -1,    83,   656,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   985,   986,   987,
     988,    -1,   415,    -1,    -1,    -1,    -1,    -1,   421,    97,
      -1,    -1,    -1,   101,   479,   428,    -1,    -1,   483,    -1,
      -1,    -1,  1010,  1011,    -1,    -1,   491,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,    -1,    -1,   142,    -1,   716,    -1,   718,
      -1,    -1,   149,   150,   723,    -1,   153,   726,    -1,   156,
     157,   158,    -1,    -1,   733,    -1,   479,   736,    -1,    -1,
     483,    -1,    -1,    -1,   162,    -1,    -1,    -1,   491,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,   758,
      12,    -1,    -1,    15,    16,    -1,    18,    -1,    20,    -1,
     769,    23,   771,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    33,    -1,    -1,    -1,    37,    38,    39,    -1,    41,
      42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      52,    -1,    -1,    55,    56,   600,    58,    59,    -1,    61,
      62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,
      72,    -1,   240,    -1,    -1,    77,    78,    -1,    -1,   247,
      -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,    91,
      -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,    -1,
      -1,   103,    -1,    -1,   106,   650,   855,   600,    -1,    -1,
      -1,   656,    -1,   862,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   874,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,   303,    -1,   886,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   895,   149,    -1,   317,
     899,   153,    -1,    -1,    -1,    -1,   158,   650,   160,    -1,
     162,    -1,   164,   656,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   716,    -1,   718,    -1,    -1,    -1,    -1,   723,    -1,
      -1,   726,    -1,    -1,    -1,    -1,    -1,    -1,   733,    -1,
      -1,   736,   941,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,    -1,   758,    -1,    -1,    -1,    -1,    -1,   387,
      -1,    -1,    -1,   716,   769,   718,   771,    -1,   146,    -1,
     723,    -1,    -1,   726,    -1,    -1,   985,   986,   987,   988,
     733,   159,    -1,   736,   162,    -1,   164,   415,    -1,    -1,
      -1,    -1,    -1,   421,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1010,  1011,    -1,    -1,   758,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,   769,    -1,   771,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    16,    -1,
      18,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    37,
     855,   479,    -1,    -1,    42,   483,    44,   862,    -1,    47,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    55,    -1,   874,
      -1,    -1,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,
      68,   886,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
     895,    -1,    -1,    -1,   899,    -1,    -1,    -1,    86,    87,
      -1,    89,   855,    91,    -1,    93,    -1,    -1,    96,   862,
       3,    -1,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,   874,    15,    16,    -1,    18,    -1,    -1,    -1,    -1,
      23,    -1,    -1,   886,    -1,    29,   941,    -1,    31,    -1,
      -1,    -1,   895,    -1,    37,    -1,   899,    -1,    -1,    42,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    61,    62,
      -1,    -1,   600,    29,   162,    68,    -1,    -1,    -1,    -1,
     985,   986,   987,   988,    77,    79,    -1,    -1,   941,    83,
      -1,    -1,    -1,    86,    87,    -1,    89,    -1,    91,    -1,
      93,    -1,    -1,    96,    -1,  1010,  1011,   100,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   650,    79,    -1,    -1,    -1,    83,   656,    -1,
      -1,    -1,   985,   986,   987,   988,   130,   131,    -1,   133,
     134,   135,    -1,   137,   138,   139,    -1,    -1,   142,   143,
     144,    -1,    -1,   147,   148,   149,   150,  1010,  1011,   153,
      -1,    -1,   156,   157,   158,    -1,    -1,   160,    -1,   162,
      -1,    -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,    -1,    -1,    -1,   142,    -1,   716,    -1,
     718,    -1,    -1,   149,   150,   723,    -1,   153,   726,    -1,
     156,   157,   158,    -1,    -1,   733,    -1,    -1,   736,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     758,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   769,    -1,   771,    -1,     0,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    18,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    -1,    30,    31,    -1,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    -1,    44,
      45,    -1,    47,    48,    -1,    50,    51,    52,    -1,    54,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    -1,    80,    81,    82,    -1,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      -1,    96,    97,    98,    99,   100,   874,   102,   103,   104,
     105,   106,    -1,    -1,   109,    -1,   111,    -1,   886,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   895,    -1,    -1,
      -1,   899,    -1,    -1,    -1,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,    -1,
     155,    -1,    -1,   158,    -1,   160,    -1,   162,    -1,   164,
      -1,    -1,     1,   941,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,
      -1,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    37,    38,
      39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,
      59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,
      -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,
      89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,    -1,    -1,   152,   153,   154,    -1,    -1,    -1,   158,
      -1,   160,    -1,   162,    -1,   164,   165,     1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,    12,    13,
      14,    15,    16,    17,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      44,    45,    -1,    47,    48,    -1,    50,    51,    52,    -1,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    -1,    96,    97,    98,    99,   100,    -1,   102,   103,
     104,   105,   106,    -1,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,    -1,   141,   142,   143,
     144,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,    -1,   160,   161,   162,     1,
     164,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    18,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    30,    31,
      -1,    33,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    44,    45,    -1,    47,    48,    -1,    50,    51,
      52,    -1,    54,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    81,
      82,    -1,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    -1,    96,    97,    98,    99,   100,    -1,
     102,   103,   104,   105,   106,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,
     152,   153,   154,   155,    -1,    -1,   158,    -1,   160,   161,
     162,     1,   164,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    18,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    -1,
      30,    31,    -1,    33,    -1,    35,    36,    37,    38,    39,
      40,    41,    42,    -1,    44,    45,    -1,    47,    48,    -1,
      50,    51,    52,    -1,    54,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    -1,    -1,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    -1,
      80,    81,    82,    -1,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    -1,    96,    97,    98,    99,
     100,    -1,   102,   103,   104,   105,   106,    -1,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,    -1,   152,   153,    -1,   155,    -1,    -1,   158,    -1,
     160,   161,   162,     1,   164,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      18,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    30,    31,    -1,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    44,    45,    -1,    47,
      48,    -1,    50,    51,    52,    -1,    54,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    -1,    80,    81,    82,    -1,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    -1,    96,    97,
      98,    99,   100,    -1,   102,   103,   104,   105,   106,    -1,
      -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,    -1,    -1,   152,   153,    -1,   155,    -1,    -1,
     158,    -1,   160,   161,   162,     1,   164,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    18,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    30,    31,    -1,    33,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    -1,    44,    45,
      -1,    47,    48,    -1,    50,    51,    52,    -1,    54,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    81,    82,    -1,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    -1,
      96,    97,    98,    99,   100,    -1,   102,   103,   104,   105,
     106,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,    -1,   152,   153,    -1,   155,
      -1,    -1,   158,    -1,   160,   161,   162,     1,   164,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    30,    31,    -1,    33,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      44,    45,    -1,    47,    48,    -1,    50,    51,    52,    -1,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    -1,    80,    81,    82,    -1,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    -1,    96,    97,    98,    99,   100,    -1,   102,   103,
     104,   105,   106,    -1,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,
      -1,   155,    -1,    -1,   158,    -1,   160,   161,   162,     1,
     164,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    18,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    30,    31,
      -1,    33,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    44,    45,    -1,    47,    48,    -1,    50,    51,
      52,    -1,    54,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    81,
      82,    -1,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    -1,    96,    97,    98,    99,   100,    -1,
     102,   103,   104,   105,   106,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,
     152,   153,    -1,   155,    -1,    -1,   158,    -1,   160,    -1,
     162,     1,   164,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    18,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    -1,
      30,    31,    -1,    33,    -1,    35,    36,    37,    38,    39,
      40,    41,    42,    -1,    44,    45,    -1,    47,    48,    -1,
      50,    51,    52,    -1,    54,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    72,    73,    74,    75,    76,    77,    78,    -1,
      80,    81,    82,    -1,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    -1,    96,    97,    98,    99,
     100,    -1,   102,   103,   104,   105,   106,    -1,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,    -1,   152,   153,    -1,   155,    -1,    -1,   158,    -1,
     160,    -1,   162,     1,   164,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      18,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    30,    31,    -1,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    44,    45,    -1,    47,
      48,    -1,    50,    51,    52,    -1,    54,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    72,    73,    74,    75,    76,    77,
      78,    -1,    80,    81,    82,    -1,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    -1,    96,    97,
      98,    99,   100,    -1,   102,   103,   104,   105,   106,    -1,
      -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,    -1,    -1,   152,   153,    -1,   155,    -1,    -1,
     158,    -1,   160,    -1,   162,     1,   164,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    18,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    30,    31,    -1,    33,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    -1,    44,    45,
      -1,    47,    48,    -1,    50,    51,    52,    -1,    54,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    81,    82,    -1,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    -1,
      96,    97,    98,    99,   100,    -1,   102,   103,   104,   105,
     106,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,    -1,   152,   153,    -1,   155,
      -1,    -1,   158,    -1,   160,    -1,   162,     1,   164,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    30,    31,    -1,    33,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      44,    45,    -1,    47,    48,    -1,    50,    51,    52,    -1,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    77,    78,    -1,    80,    81,    82,    -1,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    -1,    96,    97,    98,    99,   100,    -1,   102,   103,
     104,   105,   106,    -1,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,
      -1,   155,    -1,    -1,   158,    -1,   160,    -1,   162,     1,
     164,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    18,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    30,    31,
      -1,    33,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    44,    45,    -1,    47,    48,    -1,    50,    51,
      52,    -1,    54,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    81,
      82,    -1,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    -1,    96,    97,    98,    99,   100,    -1,
     102,   103,   104,   105,   106,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,
     152,   153,    -1,   155,    -1,    -1,   158,    -1,   160,    -1,
     162,     1,   164,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    18,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    -1,
      30,    31,    -1,    33,    -1,    35,    36,    37,    38,    39,
      40,    41,    42,    -1,    44,    45,    -1,    47,    48,    -1,
      50,    51,    52,    -1,    54,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    72,    73,    74,    75,    76,    77,    78,    -1,
      80,    81,    82,    -1,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    -1,    96,    97,    98,    99,
     100,    -1,   102,   103,   104,   105,   106,    -1,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,    -1,   152,   153,    -1,   155,    -1,    -1,   158,    -1,
     160,    -1,   162,     1,   164,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      18,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    30,    31,    -1,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    44,    45,    -1,    47,
      48,    -1,    50,    51,    52,    -1,    54,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    72,    73,    74,    75,    76,    77,
      78,    -1,    80,    81,    82,    -1,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    -1,    96,    97,
      98,    99,   100,    -1,   102,   103,   104,   105,   106,    -1,
      -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,    -1,    -1,   152,   153,    -1,   155,    -1,    -1,
     158,    -1,   160,    -1,   162,     1,   164,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    18,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    30,    31,    -1,    33,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    -1,    44,    45,
      -1,    47,    48,    -1,    50,    51,    52,    -1,    54,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    81,    82,    -1,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    -1,
      96,    97,    98,    99,   100,    -1,   102,   103,   104,   105,
     106,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,    -1,   152,   153,    -1,   155,
      -1,    -1,   158,    -1,   160,    -1,   162,     1,   164,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    30,    31,    -1,    33,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      44,    45,    -1,    47,    48,    -1,    50,    51,    52,    -1,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    77,    78,    -1,    80,    81,    82,    -1,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    -1,    96,    97,    98,    99,   100,    -1,   102,   103,
     104,   105,   106,    -1,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,
      -1,   155,    -1,    -1,   158,    -1,   160,    -1,   162,     1,
     164,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    18,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    30,    31,
      -1,    33,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    44,    45,    -1,    47,    48,    -1,    50,    51,
      52,    -1,    54,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    81,
      82,    -1,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    -1,    96,    97,    98,    99,   100,    -1,
     102,   103,   104,   105,   106,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,
     152,   153,    -1,   155,    -1,    -1,   158,    -1,   160,    -1,
     162,     1,   164,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,
      20,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    33,    -1,    -1,    -1,    37,    38,    39,
      -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,    59,
      -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,
      70,    -1,    72,    -1,    -1,    -1,    -1,    77,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,
      90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,
     100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,    -1,   152,   153,   154,    -1,    -1,    -1,   158,    -1,
     160,    -1,   162,    -1,   164,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      18,    -1,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    37,
      38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,
      58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,
      68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      88,    89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,
      -1,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,    -1,    -1,   152,   153,   154,    -1,    -1,    -1,
     158,    -1,   160,    -1,   162,    -1,   164,   165,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    18,    -1,    20,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    33,    -1,
      -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,
      55,    56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,
      -1,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    93,    -1,
      -1,    96,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,   154,
      -1,    -1,    -1,   158,    -1,   160,    -1,   162,    -1,   164,
     165,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    18,    -1,    20,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    33,    -1,    -1,    -1,    37,    38,    39,    -1,    41,
      42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,    61,
      62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,
      72,    -1,    -1,    -1,    -1,    77,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,    91,
      -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,
     152,   153,    -1,    -1,    -1,    -1,   158,    -1,   160,    -1,
     162,    -1,   164,   165,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,
      -1,    20,    -1,    -1,    23,    24,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    37,    38,
      39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,
      59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,
      69,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,    78,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,
      89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,   100,    -1,    -1,   103,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,    -1,    -1,   152,   153,    -1,    -1,    -1,    -1,   158,
      -1,   160,    -1,   162,    -1,   164,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    18,    -1,    20,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,
      37,    38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,
      -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,
      77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    90,    91,    -1,    93,    -1,    -1,    96,
      97,    98,    -1,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,    -1,   152,   153,   154,    -1,    -1,
      -1,   158,    -1,   160,    -1,   162,    -1,   164,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    18,    -1,    20,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    33,    -1,
      -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,
      55,    56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,
      -1,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    93,    -1,
      -1,    96,    97,    98,    -1,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,   154,
      -1,    -1,    -1,   158,    -1,   160,    -1,   162,   163,   164,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    18,    -1,    20,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      33,    -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,
      -1,    -1,    55,    56,    -1,    58,    59,    -1,    61,    62,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,
      -1,    -1,    -1,    -1,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      93,    -1,    -1,    96,    97,    98,    -1,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,
     153,   154,    -1,    -1,    -1,   158,    -1,   160,    -1,   162,
     163,   164,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,    20,
      -1,    -1,    23,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    33,    -1,    -1,    -1,    37,    38,    39,    -1,
      41,    42,    43,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,
      61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,
      -1,    72,    -1,    -1,    -1,    -1,    77,    78,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,
      -1,    -1,   103,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,
      -1,   152,   153,     1,    -1,     3,    -1,   158,    -1,   160,
      -1,   162,    10,   164,    -1,    -1,    -1,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    23,    -1,    20,    -1,    -1,
      -1,    24,    25,    31,    -1,    -1,    -1,    -1,    -1,    37,
      33,    -1,    35,    36,    42,    -1,    44,    40,    -1,    47,
      -1,    49,    45,    -1,    -1,    48,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    67,    -1,    69,    -1,    -1,    77,
      -1,    74,    -1,    -1,    -1,    78,    -1,    80,    86,    87,
      -1,    89,    -1,    91,    -1,    93,    -1,    -1,    96,    -1,
      -1,    -1,   100,    -1,    -1,   103,    99,    -1,   106,   102,
      -1,   104,   105,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,   133,   134,    -1,    -1,   137,
      -1,    -1,    -1,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,    -1,   153,    -1,    -1,   156,   157,
     158,   159,    -1,    -1,   162,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      18,    -1,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    37,
      38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,
      58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,
      68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      88,    89,    90,    91,    -1,    93,    -1,    -1,    96,    97,
      98,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,    -1,    -1,   152,   153,    -1,   155,    -1,    -1,
     158,    -1,   160,    -1,   162,    -1,   164,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    18,    -1,    20,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,
      -1,    37,    38,    39,    -1,    41,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,
      56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,
      -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    88,    89,    90,    91,    -1,    93,    -1,    -1,
      96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,    -1,   152,   153,   154,    -1,
      -1,    -1,   158,    -1,   160,    -1,   162,    -1,   164,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    18,    -1,    20,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    33,
      -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,
      -1,    55,    56,    -1,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,
      -1,    -1,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    88,    89,    90,    91,    -1,    93,
      -1,    -1,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,
      -1,    -1,    -1,    -1,   158,    -1,   160,    -1,   162,    -1,
     164,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    18,    -1,    20,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    33,    -1,    -1,    -1,    37,    38,    39,    -1,    41,
      42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,    61,
      62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,
      72,    -1,    -1,    -1,    -1,    77,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,    91,
      -1,    93,    -1,    -1,    96,    -1,    -1,    99,   100,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,
     152,   153,    -1,    -1,    -1,    -1,   158,    -1,   160,    -1,
     162,    -1,   164,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,
      20,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    33,    -1,    -1,    -1,    37,    38,    39,
      -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,    59,
      -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,    69,
      70,    -1,    72,    -1,    -1,    -1,    -1,    77,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,
      90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,
     100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,   112,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,    -1,   152,   153,    -1,    -1,    -1,    -1,   158,    -1,
     160,    -1,   162,    -1,   164,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      18,    -1,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    37,
      38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,
      58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,
      68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      88,    89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,
      -1,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,    -1,    -1,   152,   153,   154,    -1,    -1,    -1,
     158,    -1,   160,    -1,   162,    -1,   164,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    18,    -1,    20,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,
      -1,    37,    38,    39,    -1,    41,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,
      56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,
      -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    88,    89,    90,    91,    -1,    93,    -1,    -1,
      96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,    -1,   152,   153,    -1,    -1,
      -1,    -1,   158,    -1,   160,    -1,   162,    -1,   164,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    18,    -1,    20,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    33,
      -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,
      -1,    55,    56,    -1,    58,    59,    -1,    61,    62,    -1,
      -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,
      -1,    -1,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    88,    89,    90,    91,    -1,    93,
      -1,    -1,    96,    -1,    -1,    -1,   100,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,
     154,    -1,    -1,    -1,   158,    -1,   160,   161,   162,    -1,
     164,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    18,    -1,    20,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    33,    -1,    -1,    -1,    37,    38,    39,    -1,    41,
      42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,    61,
      62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,
      72,    -1,    -1,    -1,    -1,    77,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,    91,
      -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
     112,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,
     152,   153,    -1,    -1,    -1,    -1,   158,    -1,   160,    -1,
     162,    -1,   164,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,
      20,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    31,    -1,    33,    -1,    -1,    -1,    37,    38,    39,
      -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,    59,
      -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,
      70,    -1,    72,    -1,    -1,    -1,    -1,    77,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,
      90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,
     100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,    -1,   152,   153,    -1,    -1,    -1,    -1,   158,    -1,
     160,    -1,   162,    -1,   164,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      18,    -1,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    37,
      38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,
      58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,
      68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      88,    89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,
      -1,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,    -1,    -1,   152,   153,   154,    -1,    -1,    -1,
     158,    -1,   160,    -1,   162,    -1,   164,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    18,    -1,    20,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,
      -1,    37,    38,    39,    -1,    41,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,
      56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,
      -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    88,    89,    90,    91,    -1,    93,    -1,    -1,
      96,    -1,    -1,    99,   100,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,    -1,   152,   153,    -1,    -1,
      -1,    -1,   158,    -1,   160,    -1,   162,    -1,   164,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    18,    -1,    20,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    33,
      -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,
      -1,    55,    56,    -1,    58,    59,    -1,    61,    62,    -1,
      -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,
      -1,    -1,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    88,    89,    90,    91,    -1,    93,
      -1,    -1,    96,    -1,    -1,    -1,   100,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,
     154,    -1,    -1,    -1,   158,    -1,   160,    -1,   162,    -1,
     164,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    18,    -1,    20,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    33,    -1,    -1,    -1,    37,    38,    39,    -1,    41,
      42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,    61,
      62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,
      72,    -1,    -1,    -1,    -1,    77,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,    91,
      -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,
     152,   153,   154,    -1,    -1,    -1,   158,    -1,   160,    -1,
     162,    -1,   164,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,
      20,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    33,    -1,    -1,    -1,    37,    38,    39,
      -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,    59,
      -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,
      70,    -1,    72,    -1,    -1,    -1,    -1,    77,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,
      90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,
     100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,    -1,   152,   153,   154,    -1,    -1,    -1,   158,    -1,
     160,    -1,   162,    -1,   164,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      18,    -1,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    37,
      38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,
      58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,
      68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      88,    89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,
      -1,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,    -1,    -1,   152,   153,    -1,    -1,    -1,    -1,
     158,    -1,   160,    -1,   162,    -1,   164,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    18,    -1,    20,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,
      -1,    37,    38,    39,    -1,    41,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,
      56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,
      -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    88,    89,    90,    91,    -1,    93,    -1,    -1,
      96,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,    -1,   152,   153,    -1,    -1,
      -1,    -1,   158,    -1,   160,    -1,   162,    -1,   164,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    18,    -1,    20,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    33,
      -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,
      -1,    55,    56,    -1,    58,    59,    -1,    61,    62,    -1,
      -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,
      -1,    -1,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    88,    89,    90,    91,    -1,    93,
      -1,    -1,    96,    -1,    -1,    -1,   100,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,
      -1,    -1,    -1,    -1,   158,    -1,   160,    -1,   162,    -1,
     164,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    18,    -1,    20,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    33,    -1,    -1,    -1,    37,    38,    39,    -1,    41,
      42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,    61,
      62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,
      72,    -1,    -1,    -1,    -1,    77,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,    91,
      -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,
     152,   153,    -1,    -1,    -1,    -1,   158,    -1,   160,    -1,
     162,    -1,   164,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,
      20,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    33,    -1,    -1,    -1,    37,    38,    39,
      -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,    59,
      -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,
      70,    -1,    72,    -1,    -1,    -1,    -1,    77,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,
      90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,
     100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,    -1,   152,   153,    -1,    -1,    -1,    -1,   158,    -1,
     160,    -1,   162,    -1,   164,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      18,    -1,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    37,
      38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,
      58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,
      68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      88,    89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,
      -1,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,    -1,    -1,   152,   153,    -1,    -1,    -1,    -1,
     158,    -1,   160,    -1,   162,    -1,   164,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    18,    -1,    20,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,
      -1,    37,    38,    39,    -1,    41,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,
      56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,
      -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    88,    89,    90,    91,    -1,    93,    -1,    -1,
      96,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,    -1,   152,   153,    -1,    -1,
      -1,    -1,   158,    -1,   160,    -1,   162,    -1,   164,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    18,    -1,    20,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    33,
      -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,
      -1,    55,    56,    -1,    58,    59,    -1,    61,    62,    -1,
      -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,
      -1,    -1,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    88,    89,    90,    91,    -1,    93,
      -1,    -1,    96,    -1,    -1,    -1,   100,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,
      -1,    -1,    -1,    -1,   158,    -1,   160,    -1,   162,    -1,
     164,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    18,    -1,    20,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    33,    -1,    -1,    -1,    37,    38,    39,    -1,    41,
      42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,    61,
      62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,
      72,    -1,    -1,    -1,    -1,    77,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,    91,
      -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,
     152,   153,    -1,    -1,    -1,    -1,   158,    -1,   160,    -1,
     162,    -1,   164,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,
      20,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    33,    -1,    -1,    -1,    37,    38,    39,
      -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,    59,
      -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,
      70,    -1,    72,    -1,    -1,    -1,    -1,    77,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,
      90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,
     100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,    -1,   152,   153,    -1,    -1,    -1,    -1,   158,    -1,
     160,    -1,   162,    -1,   164,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      18,    -1,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    37,
      38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,
      58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,
      68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      88,    89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,
      -1,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,    -1,    -1,   152,   153,    -1,    -1,    -1,    -1,
     158,    -1,   160,    -1,   162,    -1,   164,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    18,    -1,    20,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,
      -1,    37,    38,    39,    -1,    41,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,
      56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,
      -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    88,    89,    90,    91,    -1,    93,    -1,    -1,
      96,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,    -1,   152,   153,    -1,    -1,
      -1,    -1,   158,    -1,   160,    -1,   162,    -1,   164,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    18,    -1,    20,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    33,
      -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,
      -1,    55,    56,    -1,    58,    59,    -1,    61,    62,    -1,
      -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,
      -1,    -1,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    88,    89,    90,    91,    -1,    93,
      -1,    -1,    96,    -1,    -1,    -1,   100,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,
      -1,    -1,    -1,    -1,   158,    -1,   160,    -1,   162,    -1,
     164,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    18,    -1,    20,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    33,    -1,    -1,    -1,    37,    38,    39,    -1,    41,
      42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,    61,
      62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,
      72,    -1,    -1,    -1,    -1,    77,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,    91,
      -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,
     152,   153,    -1,    -1,    -1,    -1,   158,    -1,   160,    -1,
     162,    -1,   164,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,
      20,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    33,    -1,    -1,    -1,    37,    38,    39,
      -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,    59,
      -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,
      70,    -1,    72,    -1,    -1,    -1,    -1,    77,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,
      90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,
     100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,    -1,   152,   153,    -1,    -1,    -1,    -1,   158,    -1,
     160,    -1,   162,    -1,   164,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      18,    -1,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    37,
      38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,
      58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,
      68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      88,    89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,
      -1,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,    -1,    -1,   152,   153,    -1,    -1,    -1,    -1,
     158,    -1,    -1,    -1,   162,    -1,   164,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    18,    -1,    20,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,
      -1,    37,    38,    39,    -1,    41,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,
      56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,
      -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    88,    89,    90,    91,    -1,    93,    -1,    -1,
      96,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
     136,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,
      -1,    -1,    -1,    -1,   160,    -1,   162,   163,   164,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    18,    -1,    20,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    33,
      -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,    53,
      -1,    55,    56,    -1,    58,    59,    -1,    61,    62,    -1,
      -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,
      -1,    -1,    -1,    77,    78,    -1,    -1,    -1,    82,    -1,
      -1,    -1,    86,    87,    88,    89,    90,    91,    -1,    93,
      -1,    95,    96,    -1,    -1,    -1,   100,    -1,    -1,   103,
      -1,    -1,   106,    -1,   108,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    18,    -1,    20,    -1,    -1,    23,
     159,   155,    -1,   162,    -1,   164,   160,    31,   162,    33,
     164,    -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,
      -1,    55,    56,    -1,    58,    59,    -1,    61,    62,    -1,
      -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,
      -1,    -1,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    88,    89,    90,    91,    -1,    93,
      -1,    -1,    96,    -1,    -1,    -1,   100,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    18,    -1,    20,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    31,   162,    33,
     164,    -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,
      -1,    55,    56,    -1,    58,    59,    -1,    61,    62,    -1,
      -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,
      -1,    -1,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    88,    89,    90,    91,    -1,    93,
      -1,    -1,    96,    -1,    -1,    -1,   100,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    18,    -1,    20,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    31,   162,    33,
     164,    -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,
      -1,    55,    56,    -1,    58,    59,    -1,    61,    62,    -1,
      -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,
      -1,    -1,    -1,    77,    78,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    86,    87,    88,    89,    90,    91,    -1,    93,
      15,    16,    96,    18,    -1,    -1,   100,    -1,    23,   103,
      -1,    -1,   106,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      55,    -1,    -1,    -1,    -1,     3,    61,    62,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    15,    16,    -1,
      18,    -1,    77,    -1,    -1,    23,    -1,    -1,   162,    -1,
     164,    86,    87,    31,    89,    -1,    91,    -1,    93,    37,
      -1,    96,    -1,    -1,    42,   100,    44,    -1,   103,    47,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      -1,    89,    -1,    91,    -1,    93,    -1,     3,    96,    -1,
      -1,    -1,   100,    -1,    10,   103,    -1,   162,   106,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    -1,    42,    -1,    44,    -1,
      -1,    47,    -1,    49,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    61,    62,    -1,    -1,    -1,
      10,    -1,    68,    -1,   162,    -1,    -1,    17,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      86,    87,    -1,    89,    -1,    91,    -1,    93,    -1,    -1,
      96,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    29,   128,   129,   130,   131,   132,   133,   134,    79,
      -1,   137,    -1,    83,    -1,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,    -1,   153,    10,    -1,
     156,   157,   158,   159,    -1,    17,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,   114,    -1,    -1,    29,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,   144,   145,    -1,   147,   148,   149,
     150,   151,   152,   153,    -1,    10,   156,   157,   158,    -1,
      -1,    -1,    17,    -1,    -1,   165,    -1,    79,    -1,    -1,
      -1,    83,   130,   131,    29,   133,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,   144,    -1,    -1,   147,
     148,   149,   150,   151,    -1,   153,    10,    -1,   156,   157,
     158,    -1,   114,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,   130,   131,
      -1,   133,   134,   135,    79,   137,   138,   139,    83,   141,
     142,   143,   144,   145,    -1,   147,   148,   149,   150,   151,
     152,   153,    10,    -1,   156,   157,   158,    -1,    -1,    17,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    29,    -1,    -1,    -1,    79,    -1,    -1,    -1,    83,
      -1,    -1,    -1,    -1,    -1,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,   152,   153,    10,
     114,   156,   157,   158,    -1,    -1,    17,    -1,    -1,    -1,
     165,    79,    -1,    -1,    -1,    83,   130,   131,    29,   133,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
     144,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
      -1,    -1,   156,   157,   158,    -1,   114,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,    -1,   133,   134,   135,    79,   137,
     138,   139,    83,   141,   142,   143,   144,   145,    -1,   147,
     148,   149,   150,   151,   152,   153,    10,    -1,   156,   157,
     158,    -1,    -1,    17,    -1,   163,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,   152,   153,    -1,    -1,   156,   157,   158,    -1,    -1,
      -1,    -1,   163,    -1,    -1,    79,    -1,    -1,    -1,    83,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    92,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,   133,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
     144,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
      -1,    79,   156,   157,   158,    83,   160,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    92,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,   144,   145,    -1,   147,
     148,   149,   150,   151,   152,   153,    -1,    79,   156,   157,
     158,    83,   160,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      92,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,   133,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,   144,   145,    -1,   147,   148,   149,   150,   151,
     152,   153,    -1,    79,   156,   157,   158,    83,   160,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    92,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,   144,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,    -1,    79,
     156,   157,   158,    83,   160,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    92,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,   144,   145,    -1,   147,   148,   149,
     150,   151,   152,   153,    -1,    79,   156,   157,   158,    83,
     160,    -1,    10,    -1,    -1,    -1,    -1,    -1,    92,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,   133,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
     144,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
      -1,    79,   156,   157,   158,    83,   160,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    92,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,   144,   145,    -1,   147,
     148,   149,   150,   151,   152,   153,    -1,    79,   156,   157,
     158,    83,   160,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      92,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,   133,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,   144,   145,    -1,   147,   148,   149,   150,   151,
     152,   153,    -1,    79,   156,   157,   158,    83,   160,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,   144,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,    10,    -1,
     156,   157,   158,    -1,   160,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    43,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,   110,    79,
      -1,    -1,   114,    83,    -1,    -1,    29,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
      -1,   133,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,   144,   145,   114,   147,   148,   149,   150,   151,
     152,   153,    -1,    -1,   156,   157,   158,    -1,   160,    -1,
     130,   131,    -1,   133,   134,   135,    79,   137,   138,   139,
      83,   141,   142,   143,   144,   145,    -1,   147,   148,   149,
     150,   151,   152,   153,    10,    -1,   156,   157,   158,    -1,
     160,    17,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,   114,    -1,    29,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,
     133,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,   152,
     153,    10,    -1,   156,   157,   158,    -1,   160,    17,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    83,    -1,    -1,
      29,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,
      79,   137,   138,   139,    83,   141,   142,   143,   144,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,    -1,    -1,
     156,   157,   158,    -1,   160,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,   114,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,   144,   145,   114,   147,   148,
     149,   150,   151,   152,   153,    -1,    -1,   156,   157,   158,
      -1,   160,    -1,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,    10,    -1,   156,
     157,   158,    -1,   160,    17,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    15,    16,
      -1,    18,    -1,    -1,    -1,    -1,    23,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    31,    -1,    17,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    42,    -1,    44,    29,    -1,
      47,    -1,    -1,    34,    -1,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    61,    62,    79,    -1,    -1,    -1,
      83,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    64,    -1,    -1,    10,    -1,    -1,    86,
      87,    -1,    89,    17,    91,    -1,    93,    -1,    79,    96,
      -1,   114,    83,   100,    -1,    29,   103,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    43,
     133,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,   144,   145,   114,   147,   148,   149,   150,   151,   152,
     153,    -1,    -1,   156,   157,   158,    -1,   160,    -1,   130,
     131,    -1,   133,   134,   135,    79,   137,   138,   139,    83,
     141,   142,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,   152,   153,    10,   155,   156,   157,   158,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
     114,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,   133,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
     144,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
      10,    -1,   156,   157,   158,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    83,    -1,    -1,    29,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    43,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    29,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,    43,   133,   134,   135,    79,
     137,   138,   139,    83,   141,   142,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    -1,   114,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,   144,   145,   114,   147,   148,   149,
     150,   151,   152,   153,    -1,    -1,   156,   157,   158,    -1,
      -1,    -1,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,   144,   145,    -1,   147,
     148,   149,   150,   151,   152,   153,    10,    20,   156,   157,
     158,    24,    25,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    36,    -1,    29,    -1,    40,    -1,    -1,
      34,    -1,    45,    -1,    -1,    48,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    69,    29,    -1,    -1,
      64,    74,    -1,    -1,    -1,    78,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    10,    -1,   102,
      -1,   104,   105,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    79,    -1,    -1,
     114,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,   133,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
     144,   145,   114,   147,   148,   149,   150,   151,   152,   153,
      -1,    -1,   156,   157,   158,    -1,    79,    -1,   130,   131,
      83,   133,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,   144,   145,    -1,   147,   148,   149,   150,   151,
     152,   153,    10,   155,   156,   157,   158,    -1,    -1,    17,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,
     133,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,   152,
     153,    10,   155,   156,   157,   158,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    79,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,   130,   131,    83,   133,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,   144,   145,    -1,   147,
     148,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,    -1,    79,    -1,   113,   114,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,   144,   145,   114,   147,   148,
     149,   150,   151,   152,   153,    -1,    -1,   156,   157,   158,
      -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,    10,    -1,   156,
     157,   158,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    20,    -1,
      -1,    -1,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    36,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    10,    45,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    67,    79,    69,    -1,    71,
      83,    73,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
     102,   114,    29,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    83,    -1,   130,   131,    -1,
     133,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,    -1,   110,   156,   157,   158,   114,    -1,    -1,    -1,
      -1,    -1,    79,   155,    -1,    -1,    83,    -1,    -1,   161,
      -1,    -1,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,   144,   145,    -1,   147,
     148,   149,   150,   151,   152,   153,    -1,   114,   156,   157,
     158,    -1,    -1,    -1,    10,    11,    -1,    -1,    -1,    -1,
      -1,    17,    -1,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,    29,   141,   142,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,   144,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,   114,    -1,
     156,   157,   158,    -1,    79,    -1,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,   144,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,   113,   114,
     156,   157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,   152,   153,    10,
      -1,   156,   157,   158,    -1,    -1,    17,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,
      15,    16,    -1,    18,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    31,    -1,    17,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    42,    -1,    44,
      29,    30,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    61,    62,    79,    -1,
      -1,    -1,    83,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    -1,    89,    -1,    91,    -1,    93,    -1,
      79,    96,    -1,   114,    83,   100,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,   144,   145,   114,   147,   148,   149,   150,
     151,   152,   153,    -1,    -1,   156,   157,   158,    -1,    -1,
      -1,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,    10,    -1,   156,   157,   158,
      -1,    -1,    17,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    15,    16,    -1,    18,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    31,    -1,    17,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    -1,    42,    -1,    44,    29,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    61,    62,    79,    -1,    -1,    -1,    83,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    77,    -1,
      -1,    -1,    -1,    -1,    10,    11,    -1,    86,    87,    -1,
      89,    17,    91,    -1,    93,    -1,    79,    96,    -1,   114,
      83,   100,    -1,    29,   103,    -1,    -1,   106,    -1,    92,
      -1,    -1,    -1,    -1,    -1,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,   144,
     145,   114,   147,   148,   149,   150,   151,   152,   153,    -1,
      -1,   156,   157,   158,    -1,    -1,    -1,   130,   131,    -1,
     133,   134,   135,    79,   137,   138,   139,    83,   141,   142,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,   152,
     153,    10,    -1,   156,   157,   158,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,   144,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,    -1,    -1,
     156,   157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    83,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    92,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,    -1,    79,   156,   157,   158,
      83,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    92,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,
     133,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,   152,
     153,    -1,    79,   156,   157,   158,    83,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    92,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,    -1,    79,   156,
     157,   158,    83,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    92,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,   152,   153,    -1,    79,   156,   157,   158,    83,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    92,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,   152,   153,    -1,
      79,   156,   157,   158,    83,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    92,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      53,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,    -1,    79,   156,   157,   158,
      83,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,
     133,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,   152,
     153,    -1,    79,   156,   157,   158,    83,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,    -1,    79,   156,
     157,   158,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,   152,   153,    -1,    20,   156,   157,   158,    24,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    20,    35,
      36,    -1,    24,    25,    40,    -1,    -1,    -1,    -1,    45,
      -1,    33,    -1,    35,    36,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    29,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    71,    -1,    73,    -1,    -1,
      76,    -1,    78,    -1,    80,    67,    -1,    69,    -1,    71,
      -1,    73,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      -1,    -1,    29,    99,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,    79,    -1,    -1,    -1,    83,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    83,    -1,    -1,   155,
      -1,    -1,    -1,   130,   131,   161,   133,   134,   135,    -1,
     137,   138,   139,   155,   141,   142,   143,   144,    -1,   161,
     147,   148,   149,   150,   151,   152,   153,   114,    -1,   156,
     157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,   144,    -1,    -1,
     147,   148,   149,   150,   151,     1,   153,     3,    -1,   156,
     157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      16,    -1,    18,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    -1,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,     3,    61,    62,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    15,    16,    -1,    18,
      -1,    77,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      86,    87,    31,    89,    -1,    91,    -1,    93,    37,    -1,
      96,    -1,    -1,    42,   100,    44,    -1,   103,    47,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,
      89,    -1,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,   100,    -1,    -1,   103,    -1,    -1,   106
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   170,   171,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    15,    16,    18,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    30,    31,    33,    35,
      36,    37,    38,    39,    40,    41,    42,    44,    45,    47,
      50,    51,    52,    54,    55,    56,    58,    59,    61,    62,
      63,    67,    68,    69,    70,    71,    72,    73,    76,    77,
      78,    80,    81,    82,    84,    85,    86,    87,    88,    89,
      90,    91,    93,    94,    96,    97,    98,    99,   100,   102,
     103,   105,   106,   109,   111,   114,   130,   131,   132,   133,
     134,   139,   149,   152,   153,   155,   158,   160,   162,   164,
     172,   173,   174,   175,   176,   177,   179,   184,   185,   186,
     187,   191,   193,   196,   197,   198,   199,   200,   201,   202,
     204,   205,   206,   210,   211,   214,   215,   218,   219,   222,
     225,   226,   252,   255,   256,   276,   277,   278,   279,   280,
     281,   282,   290,   291,   292,   293,   296,   297,   298,   299,
     300,   301,   302,   304,   305,   306,   307,   308,   155,   174,
     293,   110,   283,   284,     3,   188,    12,    20,    33,    38,
      39,    41,    78,    91,   160,   164,   222,   276,   281,   291,
     292,   293,   296,   298,   299,   283,   293,    99,   255,    80,
     188,   174,   270,   293,   174,   162,     7,    78,   293,   294,
       7,     9,    78,    99,   294,    69,   112,   203,   293,   203,
     293,    24,   256,   293,   293,   162,     3,    15,    16,    18,
      23,    31,    37,    42,    44,    47,    55,    61,    62,    68,
      77,    86,    87,    89,    91,    93,    96,   100,   103,   106,
     190,   192,   223,   162,   190,   257,   258,    30,   179,   195,
     293,   293,   293,   293,   293,   293,     7,   162,    20,    33,
      36,    78,   198,   198,   154,   245,   269,   293,    97,    98,
     155,   293,   295,   293,   195,   293,   293,   293,    90,   162,
     174,   293,   293,   179,   187,   293,   296,   179,   187,   293,
     190,   253,   293,   293,   293,   293,   293,   293,   293,   293,
       1,   161,   172,   180,   269,   101,   140,   245,   271,   272,
     295,   203,   269,   293,   303,    71,   174,   160,    75,   178,
     104,   190,   190,    48,    74,   240,   257,   154,   155,   146,
      10,    17,    29,    79,    83,   114,   130,   131,   133,   134,
     135,   137,   138,   139,   141,   142,   143,   144,   145,   147,
     148,   149,   150,   151,   152,   153,   156,   157,   158,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   159,   232,   162,   164,    79,    83,
     162,   174,   155,   293,   293,   293,   269,   160,   179,    43,
     283,   253,   257,   155,   136,   155,   109,   191,   245,   273,
     274,   275,   295,    78,   190,   226,   255,    78,   190,   253,
     226,   255,   190,   162,   195,    30,    43,   195,   110,   195,
     286,    30,    43,   195,   286,    34,    64,   155,    92,   179,
     232,   273,   174,   162,   234,   101,   162,   190,   259,   260,
       1,   135,   264,    43,   136,   174,   195,   195,   273,   190,
     136,   155,   293,   293,   155,   160,   195,   162,   273,   155,
     207,   155,   207,   155,   115,   254,   155,   195,   155,   161,
     161,   172,   136,   161,   293,   136,   163,   136,   163,   165,
     286,    43,   136,   165,   286,   113,   136,   165,     7,     1,
     161,   180,    57,   269,   293,   135,   216,   160,   227,   155,
     293,   293,   293,   293,   203,   293,   203,   293,   293,   293,
     293,   293,   293,   293,    18,    31,    55,    99,   191,   293,
     293,   293,   293,   293,   293,   293,   293,   293,   293,   293,
     293,   293,   293,   293,   293,   295,   295,   295,   295,   295,
     273,   273,   203,   293,   203,   293,    25,    43,    80,   105,
     285,   288,   289,   293,   308,    30,    43,    30,    43,    92,
      43,   165,   203,   293,   195,   155,   155,   293,   293,   115,
     163,   136,   190,   216,   257,   190,   216,   155,   257,    43,
     269,    41,   293,   203,   293,   162,   195,    41,   293,   203,
     293,   195,   158,   181,   183,   293,   181,   182,   174,   293,
      32,   293,   163,    25,    43,    46,    66,    69,    80,    99,
     173,   235,   236,   237,   238,   224,   260,   136,   163,    31,
      44,    87,    91,   164,   194,   265,   277,   115,   261,   293,
     258,   163,   245,   293,     1,   212,   273,   163,    19,   208,
     265,   277,   136,   161,   163,   163,   271,   163,   271,   174,
     165,   203,   293,   165,   174,   293,   165,   293,   165,   293,
     161,   161,   190,   155,    34,    64,   269,   160,     1,   190,
     220,   221,    25,    69,    80,    99,   229,   239,   162,   155,
     155,   155,   155,   155,   163,   165,    43,    80,   136,   163,
     276,    79,    79,    41,   203,   293,    41,   203,   293,   203,
     293,   283,   283,   155,   245,   295,   275,   216,   160,   155,
     216,   160,   155,   293,   163,   293,    30,   195,    30,   195,
     287,   288,   293,    30,   195,   286,    30,   195,   286,   136,
     155,    11,   155,    32,    32,   174,    92,   179,    43,    80,
     237,   136,   163,   162,   190,    25,    69,    80,    99,   241,
     163,   260,   264,     1,   269,    60,   295,   161,    65,   107,
     161,   213,   163,   162,   179,   190,   209,   253,   174,   165,
     286,   165,   286,   174,   113,   183,   182,     1,   217,   161,
     115,   136,   161,    80,   228,     1,     3,    10,    15,    17,
      18,    23,    37,    42,    47,    49,    55,    61,    62,    77,
      89,    93,    96,   100,   106,   130,   131,   132,   133,   134,
     137,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   153,   156,   157,   158,   159,   162,   189,   190,
     192,   230,   231,   232,   276,   163,   288,   264,   276,   276,
     293,    30,    30,   293,    30,    30,   165,   165,   195,   195,
     160,   217,   160,   217,   195,    92,    41,   293,    41,   293,
     136,   163,    92,    41,   293,   195,    41,   293,   195,   293,
     293,   174,   293,   174,    32,   190,   236,   260,   135,   268,
      80,   264,   261,   165,    43,   165,    30,   174,   269,   209,
     135,   179,    41,   174,   293,   165,    41,   174,   293,   165,
     293,   155,   155,   161,    73,    76,   161,   173,   197,   293,
     221,   241,   162,   233,   190,   293,   130,   138,   233,   233,
     261,    92,    41,    41,    92,    41,    41,    41,    41,   217,
     161,   217,   161,   293,   293,   293,   288,   293,   293,   293,
      11,    32,   174,   268,   163,   164,   194,   245,   267,   277,
     140,   247,   261,    53,   108,   248,   293,   265,   277,   174,
     195,   163,   293,   293,   174,   293,   174,   197,     1,   135,
     263,   235,   163,     3,    93,   231,   232,   293,   293,   293,
     293,   293,   293,   161,   161,    92,    92,    92,    92,   293,
     174,   261,   268,   165,   269,   245,   246,   293,     3,    82,
      93,   249,   250,   251,   293,   179,   196,   244,   165,   179,
      92,    92,   164,   194,   262,   277,    95,   242,   163,   233,
     233,    92,    92,    92,    92,    92,    92,   293,   293,   293,
     293,   247,   261,   245,   266,   267,   277,    43,   165,   251,
     108,   136,   115,   141,   143,   144,   147,   148,    53,   277,
     293,   293,     1,   165,   269,   248,   293,   266,   267,   293,
     250,   251,   251,   251,   251,   251,   251,   249,   165,   262,
     277,   165,   155,   243,   244,   165,   262,   277,   266
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
#line 492 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 497 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 498 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 511 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 530 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 532 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 533 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 534 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 535 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 540 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 542 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 543 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 544 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 545 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 550 "chapel.ypp"
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
#line 563 "chapel.ypp"
    { (yyvsp[(1) - (3)].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (3)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (3)]).prevModule;  // restore previous module name
    }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 568 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = (yyvsp[(3) - (4)].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 573 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 580 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 600 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 601 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 602 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 607 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 613 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 618 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 623 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 627 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 634 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 643 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 652 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 653 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 654 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 658 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (4)].pcallexpr), (yyvsp[(1) - (4)].b)); }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 659 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), (yyvsp[(5) - (6)].ponlylist), true, (yyvsp[(1) - (6)].b)); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 660 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), (yyvsp[(5) - (6)].ponlylist), false, (yyvsp[(1) - (6)].b)); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 664 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 669 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 671 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 673 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 675 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 677 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 683 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 684 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 690 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 698 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 713 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 714 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 725 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 726 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 727 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 728 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 729 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 730 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 731 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 732 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 733 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 734 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 737 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 738 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 739 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 740 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 742 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 743 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 748 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 749 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 764 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 765 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 766 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 767 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 777 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 778 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 782 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 783 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 787 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 789 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 790 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 804 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 805 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 806 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 807 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 812 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(3) - (7)].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[(4) - (7)].pcallexpr),
                                             (yyvsp[(6) - (7)].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[(2) - (7)]).comment));
    }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 822 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch),
                                             (yyvsp[(2) - (8)].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[(5) - (8)].pcallexpr),
                                             (yyvsp[(7) - (8)].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 832 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(3) - (7)].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[(4) - (7)].pcallexpr),
                                             (yyvsp[(6) - (7)].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[(2) - (7)]).comment));
    }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 842 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch),
                                             (yyvsp[(2) - (8)].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[(5) - (8)].pcallexpr),
                                             (yyvsp[(7) - (8)].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 853 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 857 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 862 "chapel.ypp"
    {
      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), context->latestComment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
      context->latestComment = NULL;
    }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 868 "chapel.ypp"
    {
      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), context->latestComment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
      context->latestComment = NULL;
    }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 877 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 883 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 884 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 885 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 886 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 887 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 888 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 889 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 890 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 891 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 892 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 893 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 894 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 895 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 896 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 897 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 898 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 899 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 900 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 902 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 908 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 914 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 920 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 926 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 933 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 940 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 944 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 950 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 954 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 955 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 956 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 957 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 958 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 960 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 962 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 964 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 969 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 972 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 973 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 974 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 975 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 976 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 977 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 981 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 982 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 986 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 987 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 988 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 992 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 993 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 997 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1001 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1003 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1007 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1008 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1013 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1017 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1024 "chapel.ypp"
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

  case 209:
/* Line 1792 of yacc.c  */
#line 1034 "chapel.ypp"
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

  case 210:
/* Line 1792 of yacc.c  */
#line 1047 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1052 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1057 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1065 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1066 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1071 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1073 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1075 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1080 "chapel.ypp"
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

  case 219:
/* Line 1792 of yacc.c  */
#line 1096 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1103 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1112 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1120 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1124 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1130 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1136 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1141 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1146 "chapel.ypp"
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

  case 229:
/* Line 1792 of yacc.c  */
#line 1168 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1174 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1181 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1192 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1198 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1205 "chapel.ypp"
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

  case 235:
/* Line 1792 of yacc.c  */
#line 1231 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1235 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1240 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1244 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1249 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1256 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1260 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1261 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1262 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1263 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1264 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1265 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1266 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1267 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1268 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1269 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1270 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1271 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1272 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1273 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1276 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1277 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1283 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1284 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1286 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1298 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1307 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1317 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1322 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1324 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1326 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1328 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1330 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1335 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1340 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1341 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1342 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1343 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1344 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1345 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1346 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1347 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1351 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1352 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1353 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1354 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1356 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1365 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1366 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1367 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1370 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1374 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1375 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1378 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1384 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1389 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1391 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1395 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1400 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1401 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1405 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1407 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1409 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1411 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1413 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1417 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1418 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1421 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1422 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1423 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1424 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1425 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1426 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1427 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1430 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1431 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1435 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1437 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1439 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1444 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1457 "chapel.ypp"
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

  case 351:
/* Line 1792 of yacc.c  */
#line 1473 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1481 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1490 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1496 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1505 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1541 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1555 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1559 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1561 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1565 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1572 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1573 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1574 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1575 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1576 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1582 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1583 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1584 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1606 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1610 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1614 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1622 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1629 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1630 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1635 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1637 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1643 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1645 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1647 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1651 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1652 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1653 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1654 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1661 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1662 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1663 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1664 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1668 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1669 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1673 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1674 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1679 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1680 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1684 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1690 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1694 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1695 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1696 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1697 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1701 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1726 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1728 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1730 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1732 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1734 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1736 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1738 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1741 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1743 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1745 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1747 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1749 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1751 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1753 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1755 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1758 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1760 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1762 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1768 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1770 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1772 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1776 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1778 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1780 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1782 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1784 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1786 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1788 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1790 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1792 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1799 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1805 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1811 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1817 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1826 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1835 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1851 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1855 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1860 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1864 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1865 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1869 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1873 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1874 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1879 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1883 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1887 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1893 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1894 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1895 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1896 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1897 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1898 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1904 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1909 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1914 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1930 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1932 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1934 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1936 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1938 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1940 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1944 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1945 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1948 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1949 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1950 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 1967 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 1968 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 1969 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 1973 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 1974 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 1975 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 1979 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 1980 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 1981 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 1982 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 1983 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 1991 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 1992 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 1993 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 1994 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 1998 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 1999 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 2004 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 2005 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 2006 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 2007 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 2008 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 2009 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 2010 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 2011 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 2012 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 2013 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 2015 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 2019 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 2026 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 2027 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 2031 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 2032 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 2033 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 2034 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2035 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2036 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2037 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 2038 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2039 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 2040 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 2041 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2042 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2043 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2044 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2045 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2046 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2047 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2048 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2049 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2050 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2051 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2052 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2053 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2057 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2058 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2059 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2060 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 584:
/* Line 1792 of yacc.c  */
#line 2061 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 585:
/* Line 1792 of yacc.c  */
#line 2062 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 586:
/* Line 1792 of yacc.c  */
#line 2063 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 587:
/* Line 1792 of yacc.c  */
#line 2067 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 588:
/* Line 1792 of yacc.c  */
#line 2068 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 589:
/* Line 1792 of yacc.c  */
#line 2069 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 590:
/* Line 1792 of yacc.c  */
#line 2070 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 591:
/* Line 1792 of yacc.c  */
#line 2074 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 592:
/* Line 1792 of yacc.c  */
#line 2075 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 593:
/* Line 1792 of yacc.c  */
#line 2076 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 594:
/* Line 1792 of yacc.c  */
#line 2077 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 595:
/* Line 1792 of yacc.c  */
#line 2082 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 596:
/* Line 1792 of yacc.c  */
#line 2083 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 597:
/* Line 1792 of yacc.c  */
#line 2084 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 598:
/* Line 1792 of yacc.c  */
#line 2085 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 599:
/* Line 1792 of yacc.c  */
#line 2086 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 600:
/* Line 1792 of yacc.c  */
#line 2087 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 601:
/* Line 1792 of yacc.c  */
#line 2088 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 9918 "bison-chapel.cpp"
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


