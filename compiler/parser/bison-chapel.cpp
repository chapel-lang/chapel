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
#define YYLAST   17829

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  171
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  139
/* YYNRULES -- Number of rules.  */
#define YYNRULES  602
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1080

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
    1027,  1029,  1031,  1033,  1035,  1038,  1040,  1043,  1046,  1047,
    1050,  1053,  1058,  1063,  1065,  1069,  1073,  1077,  1081,  1085,
    1089,  1093,  1096,  1098,  1100,  1104,  1109,  1114,  1117,  1122,
    1123,  1126,  1129,  1131,  1133,  1135,  1138,  1140,  1145,  1149,
    1151,  1155,  1159,  1165,  1167,  1169,  1173,  1175,  1178,  1182,
    1183,  1186,  1189,  1193,  1196,  1201,  1205,  1209,  1214,  1218,
    1219,  1222,  1225,  1228,  1230,  1231,  1234,  1237,  1240,  1242,
    1247,  1252,  1259,  1263,  1264,  1266,  1268,  1272,  1277,  1281,
    1286,  1293,  1294,  1297,  1300,  1303,  1306,  1308,  1310,  1314,
    1318,  1320,  1324,  1326,  1328,  1330,  1334,  1338,  1339,  1341,
    1343,  1347,  1351,  1355,  1357,  1359,  1361,  1363,  1365,  1368,
    1370,  1372,  1374,  1376,  1378,  1380,  1383,  1388,  1393,  1398,
    1404,  1407,  1410,  1412,  1415,  1417,  1420,  1422,  1425,  1427,
    1430,  1432,  1434,  1436,  1443,  1450,  1455,  1465,  1475,  1483,
    1490,  1497,  1502,  1512,  1522,  1530,  1535,  1542,  1549,  1559,
    1569,  1576,  1578,  1580,  1582,  1584,  1586,  1588,  1590,  1592,
    1596,  1597,  1599,  1604,  1606,  1610,  1615,  1617,  1621,  1626,
    1630,  1634,  1636,  1638,  1641,  1643,  1646,  1648,  1650,  1654,
    1657,  1662,  1664,  1666,  1668,  1670,  1672,  1674,  1676,  1678,
    1683,  1687,  1691,  1694,  1697,  1699,  1700,  1702,  1705,  1708,
    1710,  1712,  1714,  1716,  1718,  1720,  1723,  1726,  1728,  1733,
    1738,  1743,  1747,  1751,  1755,  1759,  1765,  1769,  1774,  1778,
    1783,  1785,  1787,  1789,  1791,  1793,  1795,  1797,  1799,  1801,
    1803,  1807,  1812,  1816,  1821,  1825,  1830,  1834,  1840,  1844,
    1848,  1852,  1856,  1860,  1864,  1868,  1872,  1876,  1880,  1884,
    1888,  1892,  1896,  1900,  1904,  1908,  1912,  1916,  1920,  1924,
    1928,  1932,  1935,  1938,  1941,  1944,  1947,  1950,  1953,  1957,
    1961,  1965,  1969,  1973,  1977,  1981,  1985,  1987,  1989,  1991,
    1993,  1995,  1997
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     172,     0,    -1,   173,    -1,    -1,   173,   174,    -1,   176,
      -1,   175,   176,    -1,    72,     7,    -1,   175,    72,     7,
      -1,   178,    -1,   181,    -1,   187,    -1,   188,    -1,   199,
      -1,   189,    -1,   203,    -1,   206,    -1,   204,    -1,   213,
      -1,   207,    -1,   208,    -1,   212,    -1,   198,    -1,   283,
     156,    -1,    13,   176,    -1,    14,   284,   176,    -1,    15,
     190,   156,    -1,    22,   284,   181,    -1,    27,   190,   156,
      -1,    29,   271,   156,    -1,    51,   192,   176,    -1,    55,
     294,   197,    -1,    55,   197,    -1,    64,   294,   197,    -1,
      86,   294,   197,    -1,    86,   197,    -1,    92,   176,    -1,
     112,   294,   156,    -1,     1,   156,    -1,   179,   180,    58,
     192,    -1,   177,   161,   162,    -1,   177,   161,   182,   162,
      -1,   177,   161,     1,   162,    -1,    -1,    77,    -1,    74,
      -1,    -1,    76,    -1,   161,   162,    -1,   161,   182,   162,
      -1,   161,     1,   162,    -1,   174,    -1,   182,   174,    -1,
     294,    -1,   294,    12,   294,    -1,   183,   137,   294,    -1,
     183,   137,   294,    12,   294,    -1,    -1,   183,    -1,   159,
      -1,   183,    -1,    -1,    77,    -1,    74,    -1,   186,   105,
     270,   156,    -1,   186,   105,   294,    35,   185,   156,    -1,
     186,   105,   294,    65,   184,   156,    -1,    82,   270,   156,
      -1,   297,   234,   296,   156,    -1,   297,   160,   296,   156,
      -1,   297,   128,   296,   156,    -1,   297,   122,   296,   156,
      -1,   297,   123,   296,   156,    -1,    -1,     3,    -1,     3,
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
      83,   156,    -1,    83,   296,   156,    -1,   156,    -1,   200,
      -1,    77,   200,    -1,    74,   200,    -1,   228,    -1,   257,
      -1,   220,    -1,   253,    -1,   216,    -1,   201,    -1,   202,
      -1,    41,   294,   156,    -1,    41,   294,    35,   185,   156,
      -1,    41,   294,    65,   184,   156,    -1,    41,   257,    -1,
      37,    79,   192,   218,   161,   219,   162,    -1,    37,     7,
      79,   192,   218,   161,   219,   162,    -1,    36,    79,   192,
     218,   161,   219,   162,    -1,    36,     7,    79,   192,   218,
     161,   219,   162,    -1,    37,   295,   228,    -1,    36,   295,
     228,    -1,    37,   295,   256,   258,   156,    -1,    36,   295,
     256,   258,   156,    -1,    37,    10,    -1,    31,   176,   110,
     294,   156,    -1,   110,   294,   197,    -1,    23,   294,    44,
     294,   284,   197,    -1,    23,   294,    44,   205,   284,   197,
      -1,    23,   294,   284,   197,    -1,    39,   294,    44,   294,
     197,    -1,    39,   294,    44,   205,   197,    -1,    39,   294,
     197,    -1,    39,   205,   197,    -1,    39,    70,   192,    44,
     294,   197,    -1,    40,   294,    44,   294,   197,    -1,    40,
     294,    44,   294,   287,   197,    -1,    40,   294,    44,   205,
     197,    -1,    40,   294,    44,   205,   287,   197,    -1,    40,
     294,   197,    -1,    40,   294,   287,   197,    -1,    40,   205,
     197,    -1,    40,   205,   287,   197,    -1,   165,   270,    44,
     294,   166,   176,    -1,   165,   270,    44,   294,   287,   166,
     176,    -1,   165,   270,    44,   205,   166,   176,    -1,   165,
     270,    44,   205,   287,   166,   176,    -1,   165,   270,   166,
     176,    -1,   165,   270,   287,   166,   176,    -1,   165,   205,
     166,   176,    -1,   165,   205,   287,   166,   176,    -1,   113,
     163,   270,   164,    -1,    42,   294,    93,   176,    -1,    42,
     294,   181,    -1,    42,   294,    93,   176,    33,   176,    -1,
      42,   294,   181,    33,   176,    -1,    42,   294,   234,   294,
      93,   176,    -1,    42,   294,   234,   294,   181,    -1,    42,
     294,   234,   294,    93,   176,    33,   176,    -1,    42,   294,
     234,   294,   181,    33,   176,    -1,    28,   176,    -1,    98,
     294,   156,    -1,    99,   294,   156,    -1,    98,   189,    -1,
      99,   189,    -1,    98,   181,   209,    -1,    99,   181,   209,
      -1,    -1,   209,   210,    -1,    20,   181,    -1,    20,   211,
     181,    -1,    20,   163,   211,   164,   181,    -1,   192,    -1,
     192,   136,   294,    -1,    95,   294,   156,    -1,    85,   294,
     161,   214,   162,    -1,    85,   294,   161,     1,   162,    -1,
      -1,   214,   215,    -1,   108,   270,   197,    -1,    66,   176,
      -1,    66,    31,   176,    -1,   217,   192,   218,   161,   219,
     162,    -1,   217,   192,   218,   161,     1,   162,    -1,    21,
      -1,    79,    -1,   103,    -1,    -1,   136,   270,    -1,    -1,
     219,   199,    -1,   219,   175,   199,    -1,   221,   192,   161,
     222,   162,    -1,   221,   192,   161,     1,   162,    -1,    34,
      -1,   223,    -1,   222,   137,    -1,   222,   137,   223,    -1,
     192,    -1,   192,   116,   294,    -1,    -1,    -1,    52,   225,
     236,   226,   243,   265,   249,   246,    -1,    -1,    46,    -1,
      68,    -1,    -1,    -1,   227,   242,   229,   231,   230,   243,
     264,   244,   249,   245,    -1,   241,   233,   235,    -1,   241,
     234,   235,    -1,   241,   232,   139,   233,   235,    -1,   241,
     232,   139,   234,   235,    -1,   241,     1,   235,    -1,   277,
      -1,   163,   294,   164,    -1,   191,    -1,   133,   192,    -1,
     132,    -1,   134,    -1,   135,    -1,   133,    -1,   142,    -1,
     152,    -1,   149,    -1,   145,    -1,   148,    -1,   144,    -1,
     154,    -1,   150,    -1,   159,    -1,   138,    -1,   157,    -1,
     158,    -1,   151,    -1,   143,    -1,   131,    -1,    18,    -1,
     146,    -1,    11,    -1,   160,    -1,   147,    -1,    50,    -1,
     192,   131,    -1,   116,    -1,   127,    -1,   124,    -1,   126,
      -1,   120,    -1,   125,    -1,   121,    -1,   117,    -1,   118,
      -1,   119,    -1,   130,    -1,   129,    -1,    -1,   163,   237,
     164,    -1,   163,   237,   164,    -1,    -1,   238,    -1,   237,
     137,   238,    -1,   239,   192,   269,   262,    -1,   175,   239,
     192,   269,   262,    -1,   239,   192,   269,   248,    -1,   239,
     163,   261,   164,   269,   262,    -1,   239,   163,   261,   164,
     269,   248,    -1,    -1,   240,    -1,    44,    -1,    47,    -1,
      67,    -1,    26,    -1,    26,    44,    -1,    26,    81,    -1,
      70,    -1,    81,    -1,   100,    -1,    -1,    70,    -1,    81,
      -1,    26,    81,    -1,    26,    -1,   100,    -1,    75,    -1,
      49,    -1,    -1,    26,    -1,    26,    81,    -1,    81,    -1,
      70,    -1,   100,    -1,    -1,    96,    -1,   156,    -1,   246,
      -1,   181,    -1,   198,    -1,   155,   192,    -1,   141,    -1,
     141,   294,    -1,   141,   247,    -1,    -1,   109,   294,    -1,
      54,   250,    -1,   109,   294,    54,   250,    -1,    54,   250,
     109,   294,    -1,   251,    -1,   250,   137,   251,    -1,   252,
     116,   252,    -1,   252,   148,   252,    -1,   252,   149,   252,
      -1,   252,   142,   252,    -1,   252,   144,   252,    -1,   252,
     145,   252,    -1,    83,   252,    -1,     3,    -1,    94,    -1,
     100,   254,   156,    -1,    25,   100,   254,   156,    -1,    37,
     100,   254,   156,    -1,   192,   255,    -1,   192,   255,   137,
     254,    -1,    -1,   116,   278,    -1,   116,   266,    -1,    70,
      -1,    26,    -1,    81,    -1,    26,    81,    -1,   106,    -1,
      25,   256,   258,   156,    -1,   256,   258,   156,    -1,   259,
      -1,   258,   137,   259,    -1,   192,   265,   262,    -1,   163,
     261,   164,   265,   262,    -1,   102,    -1,   192,    -1,   163,
     261,   164,    -1,   260,    -1,   260,   137,    -1,   260,   137,
     261,    -1,    -1,   116,    61,    -1,   116,   296,    -1,   165,
     166,   278,    -1,   165,   166,    -1,   165,   270,   166,   278,
      -1,   165,   270,   166,    -1,   165,   166,   263,    -1,   165,
     270,   166,   263,    -1,   165,     1,   166,    -1,    -1,   136,
     278,    -1,   136,   263,    -1,   136,   196,    -1,     1,    -1,
      -1,   136,   278,    -1,   136,   266,    -1,   136,   196,    -1,
       1,    -1,   165,   270,   166,   278,    -1,   165,   270,   166,
     266,    -1,   165,   270,    44,   294,   166,   278,    -1,   165,
       1,   166,    -1,    -1,   278,    -1,   247,    -1,   165,   166,
     267,    -1,   165,   270,   166,   267,    -1,   165,   166,   268,
      -1,   165,   270,   166,   268,    -1,   165,   270,    44,   294,
     166,   267,    -1,    -1,   136,   278,    -1,   136,   247,    -1,
     136,   196,    -1,   136,   268,    -1,   294,    -1,   247,    -1,
     270,   137,   294,    -1,   270,   137,   247,    -1,   294,    -1,
     271,   137,   294,    -1,   102,    -1,   296,    -1,   247,    -1,
     272,   137,   272,    -1,   273,   137,   272,    -1,    -1,   275,
      -1,   276,    -1,   275,   137,   276,    -1,   193,   116,   247,
      -1,   193,   116,   296,    -1,   247,    -1,   296,    -1,   193,
      -1,   195,    -1,   279,    -1,   279,   155,    -1,   155,    -1,
     282,    -1,   297,    -1,   281,    -1,   306,    -1,   305,    -1,
      88,   294,    -1,    45,   163,   274,   164,    -1,    32,   163,
     274,   164,    -1,    91,   163,   274,   164,    -1,    89,    91,
     163,   274,   164,    -1,    13,   294,    -1,    92,   294,    -1,
      69,    -1,    69,   294,    -1,   104,    -1,   104,   294,    -1,
      87,    -1,    87,   294,    -1,    17,    -1,    17,   294,    -1,
      21,    -1,    79,    -1,    34,    -1,    39,   294,    44,   294,
      31,   294,    -1,    39,   294,    44,   205,    31,   294,    -1,
      39,   294,    31,   294,    -1,    39,   294,    44,   294,    31,
      42,   294,    93,   294,    -1,    39,   294,    44,   205,    31,
      42,   294,    93,   294,    -1,    39,   294,    31,    42,   294,
      93,   294,    -1,    40,   294,    44,   294,    31,   294,    -1,
      40,   294,    44,   205,    31,   294,    -1,    40,   294,    31,
     294,    -1,    40,   294,    44,   294,    31,    42,   294,    93,
     294,    -1,    40,   294,    44,   205,    31,    42,   294,    93,
     294,    -1,    40,   294,    31,    42,   294,    93,   294,    -1,
     165,   270,   166,   294,    -1,   165,   270,    44,   294,   166,
     294,    -1,   165,   270,    44,   205,   166,   294,    -1,   165,
     270,    44,   294,   166,    42,   294,    93,   294,    -1,   165,
     270,    44,   205,   166,    42,   294,    93,   294,    -1,    42,
     294,    93,   294,    33,   294,    -1,    60,    -1,   282,    -1,
     277,    -1,   300,    -1,   299,    -1,   224,    -1,   292,    -1,
     293,    -1,   291,   147,   294,    -1,    -1,   285,    -1,   111,
     163,   286,   164,    -1,   289,    -1,   286,   137,   289,    -1,
     111,   163,   288,   164,    -1,   289,    -1,   288,   137,   289,
      -1,   290,   277,   265,   262,    -1,   309,    80,   277,    -1,
     294,    80,   277,    -1,    26,    -1,    44,    -1,    26,    44,
      -1,    81,    -1,    26,    81,    -1,   106,    -1,   297,    -1,
     291,   147,   294,    -1,    59,   294,    -1,    53,   258,    44,
     294,    -1,   303,    -1,   278,    -1,   280,    -1,   307,    -1,
     308,    -1,   224,    -1,   292,    -1,   293,    -1,   163,   141,
     294,   164,    -1,   294,   136,   294,    -1,   294,   140,   294,
      -1,   294,   140,    -1,   140,   294,    -1,   140,    -1,    -1,
     294,    -1,    98,   294,    -1,    99,   294,    -1,   294,    -1,
     277,    -1,   299,    -1,   300,    -1,   301,    -1,   297,    -1,
     294,   131,    -1,   279,   155,    -1,   224,    -1,   298,   163,
     274,   164,    -1,   298,   165,   274,   166,    -1,    73,   163,
     274,   164,    -1,   294,   139,   193,    -1,   294,   139,   100,
      -1,   294,   139,    32,    -1,   294,   139,    56,    -1,   294,
     139,    19,   163,   164,    -1,   163,   272,   164,    -1,   163,
     272,   137,   164,    -1,   163,   273,   164,    -1,   163,   273,
     137,   164,    -1,    38,    -1,    97,    -1,   302,    -1,     4,
      -1,     5,    -1,     6,    -1,     7,    -1,     8,    -1,     9,
      -1,    62,    -1,   161,   270,   162,    -1,   161,   270,   137,
     162,    -1,   165,   270,   166,    -1,   165,   270,   137,   166,
      -1,   165,   304,   166,    -1,   165,   304,   137,   166,    -1,
     294,   114,   294,    -1,   304,   137,   294,   114,   294,    -1,
     294,   154,   294,    -1,   294,   150,   294,    -1,   294,   159,
     294,    -1,   294,   138,   294,    -1,   294,   157,   294,    -1,
     294,   158,   294,    -1,   294,   151,   294,    -1,   294,   142,
     294,    -1,   294,   152,   294,    -1,   294,   149,   294,    -1,
     294,   145,   294,    -1,   294,   148,   294,    -1,   294,   144,
     294,    -1,   294,   132,   294,    -1,   294,   134,   294,    -1,
     294,   135,   294,    -1,   294,   115,   294,    -1,   294,   153,
     294,    -1,   294,   143,   294,    -1,   294,    18,   294,    -1,
     294,    11,   294,    -1,   294,   146,   294,    -1,   294,    30,
     294,    -1,   154,   294,    -1,   150,   294,    -1,    57,   294,
      -1,    71,   294,    -1,   131,   294,    -1,   294,   131,    -1,
     133,   294,    -1,   294,    80,   294,    -1,   294,    80,   205,
      -1,   309,    80,   294,    -1,   309,    80,   205,    -1,   294,
      84,   294,    -1,   294,    84,   205,    -1,   309,    84,   294,
      -1,   309,    84,   205,    -1,   154,    -1,   159,    -1,   115,
      -1,   153,    -1,   132,    -1,   134,    -1,   135,    -1
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
     801,   802,   806,   816,   826,   836,   847,   851,   856,   862,
     871,   878,   879,   880,   881,   882,   883,   884,   885,   886,
     887,   888,   889,   890,   891,   892,   893,   894,   895,   896,
     902,   908,   914,   920,   927,   934,   938,   945,   949,   950,
     951,   952,   953,   955,   957,   959,   964,   967,   968,   969,
     970,   971,   972,   976,   977,   981,   982,   983,   987,   988,
     992,   995,   997,  1002,  1003,  1007,  1009,  1011,  1018,  1028,
    1042,  1047,  1052,  1060,  1061,  1066,  1067,  1069,  1074,  1090,
    1097,  1106,  1114,  1118,  1125,  1126,  1131,  1136,  1130,  1163,
    1169,  1176,  1187,  1193,  1186,  1225,  1229,  1234,  1238,  1243,
    1250,  1251,  1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,
    1263,  1264,  1265,  1266,  1267,  1268,  1269,  1270,  1271,  1272,
    1273,  1274,  1275,  1276,  1277,  1278,  1279,  1280,  1281,  1282,
    1286,  1287,  1288,  1289,  1290,  1291,  1292,  1293,  1294,  1295,
    1296,  1297,  1301,  1302,  1306,  1310,  1311,  1312,  1316,  1318,
    1320,  1322,  1324,  1329,  1330,  1334,  1335,  1336,  1337,  1338,
    1339,  1340,  1341,  1342,  1346,  1347,  1348,  1349,  1350,  1351,
    1355,  1356,  1360,  1361,  1362,  1363,  1364,  1365,  1369,  1370,
    1373,  1374,  1378,  1379,  1383,  1388,  1389,  1390,  1398,  1399,
    1401,  1403,  1405,  1410,  1412,  1417,  1418,  1419,  1420,  1421,
    1422,  1423,  1427,  1429,  1434,  1436,  1438,  1443,  1456,  1473,
    1474,  1476,  1481,  1482,  1483,  1484,  1485,  1489,  1495,  1503,
    1504,  1512,  1514,  1519,  1521,  1523,  1528,  1530,  1532,  1539,
    1540,  1541,  1546,  1548,  1550,  1554,  1558,  1560,  1564,  1572,
    1573,  1574,  1575,  1576,  1581,  1582,  1583,  1584,  1585,  1605,
    1609,  1613,  1621,  1628,  1629,  1630,  1634,  1636,  1642,  1644,
    1646,  1651,  1652,  1653,  1654,  1655,  1661,  1662,  1663,  1664,
    1668,  1669,  1673,  1674,  1675,  1679,  1680,  1684,  1685,  1689,
    1690,  1694,  1695,  1696,  1697,  1701,  1702,  1713,  1715,  1717,
    1723,  1724,  1725,  1726,  1727,  1728,  1730,  1732,  1734,  1736,
    1738,  1740,  1743,  1745,  1747,  1749,  1751,  1753,  1755,  1757,
    1760,  1762,  1764,  1769,  1771,  1773,  1775,  1777,  1779,  1781,
    1783,  1785,  1787,  1789,  1791,  1793,  1800,  1806,  1812,  1818,
    1827,  1837,  1845,  1846,  1847,  1848,  1849,  1850,  1851,  1852,
    1857,  1858,  1862,  1866,  1867,  1871,  1875,  1876,  1880,  1884,
    1888,  1895,  1896,  1897,  1898,  1899,  1900,  1904,  1905,  1910,
    1915,  1923,  1924,  1925,  1926,  1927,  1928,  1929,  1930,  1931,
    1933,  1935,  1937,  1939,  1941,  1946,  1947,  1951,  1952,  1953,
    1957,  1958,  1959,  1960,  1971,  1972,  1973,  1974,  1978,  1979,
    1980,  1984,  1985,  1986,  1987,  1988,  1996,  1997,  1998,  1999,
    2003,  2004,  2008,  2009,  2010,  2011,  2012,  2013,  2014,  2015,
    2016,  2017,  2018,  2019,  2020,  2024,  2032,  2033,  2037,  2038,
    2039,  2040,  2041,  2042,  2043,  2044,  2045,  2046,  2047,  2048,
    2049,  2050,  2051,  2052,  2053,  2054,  2055,  2056,  2057,  2058,
    2059,  2063,  2064,  2065,  2066,  2067,  2068,  2069,  2073,  2074,
    2075,  2076,  2080,  2081,  2082,  2083,  2088,  2089,  2090,  2091,
    2092,  2093,  2094
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
  "new_expr", "let_expr", "expr", "opt_expr", "opt_try_expr", "lhs_expr",
  "fun_expr", "call_expr", "dot_expr", "parenthesized_expr",
  "bool_literal", "literal_expr", "assoc_expr_ls", "binary_op_expr",
  "unary_op_expr", "reduce_expr", "scan_expr", "reduce_scan_op_expr", YY_NULL
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
     201,   201,   202,   202,   202,   202,   202,   202,   202,   202,
     203,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   205,   206,   206,
     206,   206,   206,   206,   206,   206,   207,   208,   208,   208,
     208,   208,   208,   209,   209,   210,   210,   210,   211,   211,
     212,   213,   213,   214,   214,   215,   215,   215,   216,   216,
     217,   217,   217,   218,   218,   219,   219,   219,   220,   220,
     221,   222,   222,   222,   223,   223,   225,   226,   224,   227,
     227,   227,   229,   230,   228,   231,   231,   231,   231,   231,
     232,   232,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   235,   235,   236,   237,   237,   237,   238,   238,
     238,   238,   238,   239,   239,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   241,   241,   241,   241,   241,   241,
     242,   242,   243,   243,   243,   243,   243,   243,   244,   244,
     245,   245,   246,   246,   247,   248,   248,   248,   249,   249,
     249,   249,   249,   250,   250,   251,   251,   251,   251,   251,
     251,   251,   252,   252,   253,   253,   253,   254,   254,   255,
     255,   255,   256,   256,   256,   256,   256,   257,   257,   258,
     258,   259,   259,   260,   260,   260,   261,   261,   261,   262,
     262,   262,   263,   263,   263,   263,   263,   263,   263,   264,
     264,   264,   264,   264,   265,   265,   265,   265,   265,   266,
     266,   266,   266,   267,   267,   267,   268,   268,   268,   268,
     268,   269,   269,   269,   269,   269,   270,   270,   270,   270,
     271,   271,   272,   272,   272,   273,   273,   274,   274,   275,
     275,   276,   276,   276,   276,   277,   277,   278,   278,   278,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     281,   282,   283,   283,   283,   283,   283,   283,   283,   283,
     284,   284,   285,   286,   286,   287,   288,   288,   289,   289,
     289,   290,   290,   290,   290,   290,   290,   291,   291,   292,
     293,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   295,   295,   296,   296,   296,
     297,   297,   297,   297,   298,   298,   298,   298,   299,   299,
     299,   300,   300,   300,   300,   300,   301,   301,   301,   301,
     302,   302,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   304,   304,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   306,   306,   306,   306,   306,   306,   306,   307,   307,
     307,   307,   308,   308,   308,   308,   309,   309,   309,   309,
     309,   309,   309
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
       1,     1,     1,     1,     2,     1,     2,     2,     0,     2,
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
       1,     1,     1,     6,     6,     4,     9,     9,     7,     6,
       6,     4,     9,     9,     7,     4,     6,     6,     9,     9,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     1,     4,     1,     3,     4,     1,     3,     4,     3,
       3,     1,     1,     2,     1,     2,     1,     1,     3,     2,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       3,     3,     2,     2,     1,     0,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     4,     4,
       4,     3,     3,     3,     3,     5,     3,     4,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     4,     3,     4,     3,     4,     3,     5,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    87,   543,   544,   545,   546,
     547,   548,     0,   480,    73,   108,   448,   114,   450,   480,
       0,   113,     0,   353,    73,     0,     0,     0,     0,   452,
     515,   515,   540,     0,     0,     0,     0,   112,     0,   230,
     109,     0,   226,     0,     0,   116,     0,     0,   471,   549,
     117,     0,   231,   442,   352,     0,     0,     0,    45,    44,
     111,   451,   354,     0,     0,     0,     0,   446,     0,     0,
     115,     0,     0,    88,     0,   541,     0,     0,     0,   110,
     212,   444,   356,   118,     0,     0,   598,     0,   600,     0,
     601,   602,   514,     0,   599,   596,   429,   127,   597,     0,
       0,     0,     4,     0,     5,     0,     9,    46,    10,     0,
      11,    12,    14,   425,   426,    22,    13,   128,   136,   137,
      15,    17,    16,    19,    20,    21,    18,   135,     0,   133,
       0,   506,     0,   131,   134,     0,   132,   520,   502,   427,
     503,   432,   430,     0,     0,   507,   508,     0,   431,     0,
     521,   522,   523,   542,   501,   434,   433,   504,   505,     0,
      38,    24,   440,     0,     0,   481,    74,     0,     0,   450,
     452,     0,     0,     0,   451,     0,     0,     0,   506,   520,
     430,   507,   508,   449,   431,   521,   522,     0,   480,     0,
       0,   355,     0,   186,     0,   410,     0,   417,   546,   451,
     516,   229,   546,   150,   451,     0,   229,     0,     0,     0,
       0,     0,     0,     0,   141,     0,     0,   417,    81,    89,
     101,    95,    94,   103,    84,    93,   104,    90,   105,    82,
     106,    99,    92,   100,    98,    96,    97,    83,    85,    91,
     102,   107,     0,    86,     0,     0,     0,     0,   359,     0,
     124,    32,     0,   583,   499,     0,   443,   584,     7,   417,
     210,   220,   515,   211,   130,   129,   429,   407,     0,   406,
       0,     0,   125,   519,     0,     0,    35,     0,   447,   435,
       0,   417,    36,   441,     0,   193,   189,     0,   431,   193,
     190,     0,   349,     0,   445,     0,     0,   585,   587,   513,
     582,   581,     0,    48,    51,     0,     0,   412,     0,   414,
       0,     0,   413,     0,     0,   406,     0,     0,     6,     0,
      47,     0,     0,   213,     0,   311,   310,   232,     0,   428,
      23,     0,     0,     0,     0,     0,     0,     0,   586,     0,
       0,     0,     0,     0,     0,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   270,   277,   278,   279,   274,   276,     0,     0,   272,
     275,   273,   271,     0,   281,   280,     0,     0,   417,   417,
       0,     0,     0,    25,    26,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,    28,     0,    29,     0,   425,
     423,     0,   418,   419,   424,     0,   213,   147,     0,     0,
     213,     0,   146,     0,     0,     0,   159,     0,     0,   158,
       0,   167,     0,     0,     0,   165,     0,     0,    57,   138,
       0,   179,     0,     0,    30,   293,   227,   363,     0,   364,
     366,     0,   388,     0,   369,     0,     0,   123,    31,    33,
       0,   324,     0,    67,   517,   518,   126,     0,    34,   417,
       0,   200,   191,   187,   192,   188,     0,   347,   344,   152,
      37,    50,    49,    52,     0,   550,     0,     0,   536,     0,
     538,     0,     0,     0,     0,     0,     0,     0,     0,   554,
       8,     0,    40,     0,     0,     0,   406,     0,     0,     0,
     304,   358,   479,   578,   577,   580,   589,   588,   593,   592,
     574,   571,   572,   573,   510,   561,     0,   533,   534,   532,
     531,   511,   565,   576,   570,   568,   579,   569,   567,   559,
     564,   566,   575,   558,   562,   563,   560,     0,     0,     0,
       0,     0,     0,     0,   591,   590,   595,   594,   491,   492,
     494,   496,     0,   483,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   552,   480,   480,   155,   345,   357,   411,
       0,     0,   437,     0,   213,     0,     0,   213,     0,   346,
       0,     0,     0,     0,   455,     0,     0,     0,   168,     0,
     461,     0,     0,   166,   597,    60,     0,    53,    58,     0,
     178,     0,     0,     0,   436,   298,   295,   296,   297,   301,
     302,   303,   293,     0,   286,     0,   294,   312,     0,   367,
       0,   121,   122,   120,   119,     0,   387,   386,   502,     0,
     361,   500,   360,   530,   409,   408,     0,     0,     0,   438,
       0,   194,   351,   502,     0,   551,   509,   537,   415,   539,
     416,   175,     0,     0,     0,   553,   173,   465,     0,   556,
     555,     0,    42,    41,    39,    64,     0,    57,   214,     0,
       0,   224,     0,   221,   308,   305,   306,   309,   233,     0,
       0,    71,    72,    70,    69,    68,   528,   529,   493,   495,
       0,   482,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   151,   421,   422,   420,     0,
     215,   149,     0,   215,   148,     0,   177,     0,     0,   157,
       0,   156,     0,   486,     0,     0,   163,     0,     0,   161,
       0,     0,   139,     0,   140,     0,     0,   181,     0,   183,
     299,   300,     0,   293,   284,     0,   401,   313,   316,   315,
     317,     0,   365,   368,   369,     0,     0,   370,   371,   202,
       0,     0,   201,   204,   439,     0,   195,   198,     0,   348,
     176,     0,     0,     0,     0,   174,     0,     0,     0,     0,
     229,   219,     0,   222,   218,   307,   312,   282,    75,   265,
      89,   263,    95,    94,    78,    93,    90,   268,   105,    76,
     106,    92,    96,    77,    79,    91,   107,   262,   244,   247,
     245,   246,   257,   248,   261,   253,   251,   264,   267,   252,
     250,   255,   260,   249,   254,   258,   259,   256,   266,     0,
     242,     0,    80,     0,   282,   282,   240,   535,   484,   369,
     520,   520,     0,     0,     0,     0,     0,     0,     0,     0,
     154,   153,   215,   229,   215,   229,   160,     0,     0,   454,
       0,   453,     0,   485,     0,     0,   460,   164,     0,   459,
     162,    55,    54,   180,   470,   182,     0,   401,   287,     0,
       0,   369,   314,   328,   362,   392,     0,   552,     0,   206,
       0,     0,     0,   196,     0,   171,   467,     0,     0,   169,
     466,     0,   557,    65,    66,   209,   229,   229,   208,   229,
     216,   225,   223,     0,   293,   239,   243,     0,   269,     0,
     235,   236,   488,     0,     0,     0,     0,     0,     0,     0,
       0,   229,   144,   229,   142,   458,     0,     0,   487,   464,
       0,     0,     0,     0,   185,   369,   401,     0,   404,   403,
     405,   502,   325,   290,   288,     0,     0,     0,     0,   390,
     502,   207,   205,     0,   199,     0,   172,     0,   170,   217,
     383,     0,   318,     0,   241,    75,    77,   282,   282,     0,
       0,     0,     0,     0,     0,   145,   143,     0,     0,     0,
       0,    56,   184,   289,   369,   393,     0,   327,   326,   342,
       0,   343,   330,   333,     0,   329,   322,   323,   228,     0,
     197,     0,     0,     0,   382,   381,   502,   319,   328,   283,
     237,   238,     0,     0,     0,     0,     0,     0,   457,   456,
     463,   462,   292,   291,   395,   396,   398,   502,     0,   552,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     502,   469,   468,     0,   373,     0,     0,     0,   397,   399,
     332,   334,   335,   338,   339,   340,   336,   337,   331,   378,
     376,   502,   552,   320,   234,   321,   393,   377,   502,   400
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   304,   103,   600,   105,   106,   107,   321,
     108,   305,   595,   599,   596,   109,   110,   111,   112,   167,
     830,   246,   113,   243,   114,   626,   251,   115,   116,   117,
     118,   119,   120,   121,   701,   122,   123,   124,   462,   641,
     768,   125,   126,   637,   763,   127,   128,   498,   780,   129,
     130,   672,   673,   178,   244,   617,   132,   133,   500,   786,
     678,   833,   834,   432,   915,   436,   613,   614,   615,   616,
     679,   327,   751,  1018,  1074,  1008,   267,   953,   957,  1002,
    1003,  1004,   134,   293,   467,   135,   136,   247,   248,   440,
     441,   630,  1015,   972,   444,   627,  1035,   950,   881,   306,
     194,   310,   311,   401,   402,   403,   179,   138,   139,   140,
     141,   180,   143,   164,   165,   552,   422,   722,   553,   554,
     144,   181,   182,   147,   206,   404,   184,   149,   185,   186,
     152,   153,   154,   316,   155,   156,   157,   158,   159
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -940
static const yytype_int16 yypact[] =
{
    -940,    74,  3038,  -940,   -42,  -940,  -940,  -940,  -940,  -940,
    -940,  -940,  4508,    14,   280,  -940, 13303,  -940, 15181,    14,
    9554,  -940,   201,   211,   280,  4508,  9554,  4508,   140, 16589,
    9717,  7934,  -940,  8097,  8739,  6793,  9554,  -940,   153,  -940,
    -940, 17665,  -940,  2726,  8902,  -940,  9554,  9554,  -940,  -940,
    -940,  9554,  -940, 13303,  -940,  9554,   367,   194,  2731,  8583,
    -940, 17722,  -940,  9880,  7608,  9554,  8902, 13303,  9554,   269,
    -940,   222,  4508,  -940,  9554,  -940, 10043, 10043, 17665,  -940,
    -940, 13303,  -940,  -940,  9554,  9554,  -940,  9554,  -940,  9554,
    -940,  -940, 12814,  9554,  -940,  9554,  -940,  -940,  -940,  3530,
    6956,  9065,  -940,  4345,  -940,   275,  -940,   363,  -940,   337,
    -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,
    -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940, 17665,  -940,
   17665,   354,   101,  -940,  -940,  2726,  -940,   298,  -940,   360,
    -940,  -940,   303,   364,   376,   374,   381, 17281, 13300,    78,
     382,   384,  -940,  -940,  -940,  -940,  -940,  -940,  -940,   397,
    -940,  -940, 17281,   372,  4508,  -940,  -940,   398,  9554,  -940,
    -940,  9554,  9554,  9554,  -940,  9554,  9880,  9880,   149,  -940,
    -940,  -940,  -940,   -55,   150,  -940,  -940,   392, 15254, 17665,
    2726,  -940,   402,  -940,   -18, 17281,   450,  8260,   483, 17665,
   17281,   205,   484,  -940, 17665, 17665,   205, 17665,   401,    18,
   14788,    51, 14756,   196,  -940, 15197, 14004,  8260,  -940,  -940,
    -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,
    -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,
    -940,  -940,  4508,  -940,   404,  1083,    93,    15,  -940,  4508,
    -940,  -940, 14932,  1129,   431, 14932,   -55,  1129,  -940,  8260,
    -940,  -940,  9228,  -940,  -940,  -940, 17665,  -940,    82, 17281,
    9554,  9554,  -940, 17281,   416, 15165,  -940, 14932,   -55, 17281,
     410,  8260,  -940, 17281, 15341,  -940,  -940, 15606,  2142,  -940,
    -940, 15655,   460,   428,   -55, 14932, 15750,   278,   278,  1570,
    1129,  1129,   118,  -940,  -940,  3693,   142,  -940,  9554,  -940,
     -29,    37,  -940,   -48,     6, 15799,   -17,   576,  -940,  3856,
    -940,   527,  9880,   451,   435,  -940,  -940,  -940,   144,   317,
    -940,  9554,  9554,  9554,  9554,  8739,  8739,  9554,   353,  9554,
    9554,  9554,  9554,  9554,   596, 12814,  9554,  9554,  9554,  9554,
    9554,  9554,  9554,  9554,  9554,  9554,  9554,  9554,  9554,  9554,
    9554,  -940,  -940,  -940,  -940,  -940,  -940,  8423,  8423,  -940,
    -940,  -940,  -940,  8423,  -940,  -940,  8423,  8423,  8260,  8260,
    8739,  8739,  7119,  -940,  -940, 15398, 15430, 15831,    47,  4019,
    -940,  8739,    18,   436,   257,  -940,  9554,  -940,  9554,   481,
    -940,   434,   463,  -940,  -940, 17665,   451,  -940,  2726, 17665,
     451,   447,  -940,  2726,   560,  9880,  -940,  4671,  8739,  -940,
     445,  -940,    18,  4834,  8739,  -940,    18, 10206,  9554,  -940,
    4508,   573,  9554,   446,  -940,   195,  -940,  -940,  1083,  -940,
     477,   454,  -940, 10369,   500,  9554,  2726,  -940,  -940,  -940,
     455,  -940,  9880,  -940, 17281, 17281,  -940,    36,  -940,  8260,
     457,  -940,   605,  -940,   605,  -940, 10532,   498,  -940,  -940,
    -940,  -940,  -940,  -940,  9391,  -940, 13860,  7282,  -940,  7445,
    -940,  4508,   470,  8739,  6301,  3367,   471,  9554,  6465,  -940,
    -940,   272,  -940,  4182, 17665,   290, 15574,  9880,   480, 16413,
     443,  -940, 15975, 17468, 17468,   366,  -940,   366,  -940,   366,
   12969,    97,  1735,   991,   -55,   278,   475,  -940,  -940,  -940,
    -940,  1570,  1401,   366,   904,   904, 17468,   904,   904,   657,
     278,  1401, 17521,   657,  1129,  1129,   278,   487,   489,   497,
     506,   507,   494,   473,  -940,   366,  -940,   366,    66,  -940,
    -940,  -940,   123,  -940,  1929, 17357,   427, 10695,  8739, 10858,
    8739,  9554,  8739, 13567,    14, 16020,  -940,  -940,  -940, 17281,
   16061,  8260,  -940,  8260,   451,   504,   293,   451,   510,  -940,
     304,  9554,   124,  9554, 17281,    54, 14989,  7119,  -940,  9554,
   17281,    64, 14845,  -940,   511,   531,   516, 16172,   531,   520,
     644, 16212,  4508, 15021,  -940,    90,  -940,  -940,  -940,  -940,
    -940,  -940,   603,   141,  -940,  2863,  -940,   452,   515,  1083,
      93,   140,   153,  9554,  9554,  6138,  -940,  -940,   375,  7771,
    -940, 17281,  -940,  -940,  -940, 17281,   530,   129,   525,  -940,
    2316,  -940,  -940,   350, 17665,  -940,  -940,  -940,  -940,  -940,
    -940,  -940,  4508,   -15,  2352,  -940,  -940, 17281,  4508, 17281,
    -940, 16253,  -940,  -940,  -940,  -940, 10206,  9554,   557,   706,
     536,   583,   147,  -940,   620,  -940,  -940,  -940,  -940,  8578,
     541,  -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,
    7119,  -940,    32,  8739,  8739,  9554,   678, 16397,  9554,   679,
   16429,   545,  2843,    18,    18,  -940,  -940,  -940,  -940,   553,
    -940,  -940,   554,  -940,  -940, 14932,  -940, 14080,  4997,  -940,
    5160,  -940,   189,  -940, 14156,  5323,  -940,    18,  5486,  -940,
      18,  9554,  -940,  9554,  -940,  4508,  9554,  -940,  4508,   684,
    -940,  -940, 17665,   686,  -940,  1083,   582,   639,  -940,  -940,
    -940,    57,  -940,  -940,   500,   556,    79,  -940,  -940,  -940,
    5649,  9880,  -940,  -940,  -940, 17665,  -940,   589,   392,  -940,
    -940,  5812,   562,  5975,   568,  -940,  9554,   579,   580,   577,
    1360,  -940,  9554, 17665,  -940,  -940,   452,   585,   232,  -940,
     606,  -940,   607,   611,   626,   615,   621,  -940,   624,   631,
     629,   630,   634,   241,   646,   636,   643,  -940,  -940, 17665,
    -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,
    -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,  9554,
    -940,   653,   659,   655,   585,   585,  -940,  -940,  -940,   500,
     238,   246, 16573, 11021, 11184, 16605, 11347, 11510, 11673, 11836,
    -940,  -940,  -940,  1991,  -940,  2565,  -940,  4508,  9554, 17281,
    9554, 17281,  7119,  -940,  4508,  9554, 17281,  -940,  9554, 17281,
    -940, 16662, 17281,  -940, 17281,   764,  4508,   582,  -940,   635,
   11999,   177,  -940,    44,  -940,  -940,  8739, 13435,  4508,  -940,
      30,   638,  9554,  -940,  9554,  -940, 17281,  4508,  9554,  -940,
   17281,  4508, 17281,  -940,  -940,  -940, 12854, 12854,  -940,   623,
    -940, 17281,  -940,    46,   195,  -940,  -940, 13901,  -940, 13659,
    -940,  -940,  -940,  9554,  9554,  9554,  9554,  9554,  9554,  9554,
    9554, 15986,  -940, 17492,  -940, 16212, 14232, 14308,  -940, 16212,
   14384, 14460,  9554,  4508,  -940,   500,   582,  6629,  -940,  -940,
    -940,   236,  9880,  -940,  -940,    63,  9554,   -16, 13715,  -940,
     524,   450,  -940,   392, 17281, 14536,  -940, 14612,  -940,  -940,
    -940, 12162,   702,   267,  -940,   672,   673,   585,   585, 16749,
   16825, 16901, 16977, 17053, 17129,  -940,  -940,  4508,  4508,  4508,
    4508, 17281,  -940,  -940,   177, 12325,    80,  -940, 17281,  -940,
      76,  -940,   135,  -940,   478, 17205,  -940,  -940,  -940, 11836,
    -940,  4508,  4508,  3201,  -940,  -940,   339,  -940,    44,  -940,
    -940,  -940,  9554,  9554,  9554,  9554,  9554,  9554, 16212, 16212,
   16212, 16212,  -940,  -940,  -940,  -940,  -940,   260,  8739, 13140,
    -940,  9554,    63,    76,    76,    76,    76,    76,    76,    63,
     786, 16212, 16212,   645, 12651,    87,   -35, 13803,  -940,  -940,
   17281,  -940,  -940,  -940,  -940,  -940,  -940,  -940,   676,  -940,
    -940,   342, 12977,  -940,  -940,  -940, 12488,  -940,   425,  -940
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -940,  -940,  -940,     1,  -389,  2139,  -940,  -940,  -940,  -940,
     379,   491,  -403,   151,   154,  -940,  -940,  -940,   274,   795,
    -940,  2111,   -81,  -662,  -940,  -779,  1243,  -928,  -723,   -52,
    -940,  -940,  -940,  -940,    72,  -940,  -940,  -940,   533,  -940,
      58,  -940,  -940,  -940,  -940,  -940,  -940,  -380,  -620,  -940,
    -940,  -940,    41,   895,  -940,  -940,  -940,   231,  -940,  -940,
    -940,  -940,   -94,  -147,  -790,  -940,   -88,    84,   216,  -940,
    -940,  -940,    43,  -940,  -940,  -223,     9,  -156,  -179,  -208,
    -200,   -92,  -940,  -174,  -940,    -2,   808,  -119,   403,  -940,
    -396,  -742,  -939,  -940,  -593,  -461,  -856,  -927,  -839,   -23,
    -940,    49,  -940,  -189,  -940,   271,   502,  -430,  -940,  -940,
    -940,  1191,  -940,    -8,  -940,  -940,  -210,  -940,  -577,  -940,
    -940,  1262,  1571,   -12,   817,    22,   869,  -940,  1691,  1797,
    -940,  -940,  -940,  -940,  -940,  -940,  -940,  -940,  -373
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -553
static const yytype_int16 yytable[] =
{
     162,   377,   426,   102,   183,   642,   264,   265,   188,   556,
     723,   187,   884,   628,   195,   393,   328,   832,   200,   200,
     190,   210,   212,   215,   216,   598,   575,   754,   433,  1007,
     578,   411,   252,   442,   253,   254,   643,   636,   945,   255,
     268,   256,   618,   257,   920,   921,   612,   970,    64,   249,
     483,   269,   273,   275,   277,   278,   279,   910,   442,   445,
     283,   249,   284,   420,   287,   291,   999,    64,  1036,   294,
     450,   394,   295,   296,     3,   297,   338,   298,   314,   999,
     299,   300,   249,   301,   344,   718,   274,   269,   273,   315,
     853,   562,   460,   855,   442,   725,   420,   922,   955,   839,
    -379,   948,  -203,   482,   486,   209,   211,   994,   477,   309,
     688,  -384,  1059,   838,   160,  1070,   399,   420,   481,   396,
     488,  1073,   312,   886,  1038,   163,   389,   334,  1007,  -379,
     910,   562,   910,  1077,   740,   478,   399,  -384,   397,   954,
    -384,   377,  -379,   484,  -203,   389,  1000,   689,  -384,   489,
     325,   771,   446,   956,   388,  -379,   162,  1001,   883,   385,
     386,   387,   420,   283,   269,   315,  -384,   452,   443,  -384,
    1001,   741,   485,   313,   479,   420,   326,   335,   399,   389,
     392,   336,   971,  1058,   484,   273,   969,  1020,  1021,   542,
     543,   389,  1014,   443,   709,   760,  -384,   712,  -203,   408,
     399,   480,  -379,   993,   413,   273,   400,  -379,   910,  -384,
     910,   190,   389,   563,   556,   389,   484,   484,  -384,   452,
    1079,   605,    23,   753,   484,   389,   400,    23,   338,   443,
    -384,    23,   931,   342,   933,   343,   344,   761,   453,   606,
     347,   378,   607,   379,  1041,   887,  1039,   273,   354,  -384,
     200,    39,  1033,  1072,   358,   359,   360,   832,   454,   455,
     690,   452,   608,   520,   598,   609,    54,    56,   400,   273,
     638,    54,  1042,    52,   160,    54,   610,    62,   743,   474,
     471,   446,    62,   166,   783,   938,    62,   691,   716,   576,
     400,   762,   191,   629,   580,   611,   476,   399,   399,   495,
     501,   189,    82,   197,   475,   744,   473,    82,   334,   784,
     496,    82,  -527,  -524,  -527,  -524,   217,   556,   952,   502,
     503,   504,   505,   507,   509,   510,   862,   511,   512,   513,
     514,   515,  -285,   521,   522,   523,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   879,
     286,   290,  -402,   863,   612,   273,   273,   259,   335,  -285,
     280,   273,   336,   -81,   273,   273,   273,   273,   545,   547,
     555,   -87,   -83,  -402,   258,  -490,  -394,  -402,   399,   565,
     -88,   727,   730,  -489,   569,   281,   570,   400,   400,   537,
     538,   909,   582,  -380,   446,   539,  -372,  -394,   540,   541,
    -402,  -394,  -490,   269,   743,   584,   586,   506,   508,   338,
    -489,   590,   592,   568,   342,   597,   597,   344,   601,  -385,
     603,   347,  -380,   250,  -394,  -372,   959,   452,   160,  -385,
     446,  1019,   407,   631,   662,  -380,   319,   412,  -372,   320,
     635,   446,   322,   772,   774,   250,   665,   273,  -380,   711,
     951,  -372,   544,   546,  -473,   285,   289,   960,  -385,  -472,
     714,   634,   635,   564,   909,   273,   909,   273,   400,   674,
     769,   654,   635,   657,   668,   659,   661,   380,   747,  -374,
    -526,   381,  -526,   634,  -385,   269,   309,  -350,   309,   556,
     585,  -385,   399,   634,   473,  -380,   591,   338,  -372,   312,
    -380,   312,   342,  -372,   137,   344,  -350,   694,  -374,   347,
    -476,   381,  -385,   675,   137,   329,  -525,  -527,  -525,  -527,
     330,  -374,   748,   331,   676,   612,   648,   137,   650,   137,
    -477,  -385,   835,   749,  -374,   382,  -385,  -478,  -475,  -385,
    -474,  1016,   909,   677,   909,   584,   697,   590,   700,   601,
     702,   657,   750,   389,   384,   653,   703,   704,   395,   273,
     398,   273,   405,   409,   415,  1037,   390,   435,  -389,   715,
     344,   717,   456,   459,   137,   555,   466,   724,  -389,  1050,
     706,  -374,   400,   490,   468,   494,  -374,   497,   250,   250,
     250,   250,   567,   707,  1043,   431,   499,   571,   572,     5,
     573,   137,   756,   579,   581,   137,   602,  -389,   587,  1037,
     604,   279,   283,   315,   619,   516,   629,   273,   620,   633,
    1044,   639,  1045,  1046,  1071,   640,  1047,  1048,   517,   605,
     696,   250,   699,  -389,   250,   644,   652,   658,   680,   687,
    -389,   669,  1078,   681,   260,   682,  1037,   606,    22,    23,
     607,   758,   518,   683,   597,   597,   250,   261,   686,    30,
     262,  -389,   684,   685,    35,   710,   137,   -59,   731,    39,
     608,   713,   732,   609,   250,   317,   734,   735,   555,   752,
    -389,   507,   545,   842,   610,  -389,   845,   334,  -389,   764,
      73,    52,   759,    54,   452,   317,   519,   906,   781,   782,
     907,   785,   263,   611,    62,   837,   859,   779,   861,   843,
     846,   848,   605,   866,   852,   854,   869,   876,   880,   871,
     882,   872,   885,    78,   874,   892,    80,  -215,   897,    82,
     606,  -215,  -215,   607,   901,   903,   904,   335,   890,   905,
    -215,   336,  -215,  -215,   137,  -108,  -114,  -215,   914,   269,
    -113,   137,  -215,   608,  -112,  -215,   609,   -84,    56,   896,
    -109,   900,   -82,  -116,   902,   506,   544,   610,  -117,  -111,
     911,   250,   978,  -115,  -215,  -110,  -215,   -85,  -215,    97,
    -215,  -215,  -118,  -215,   918,  -215,   611,  -215,   338,   339,
     -86,   340,   341,   342,   919,   343,   344,   943,  1017,   946,
     347,   250,   963,   -81,   -83,   250,  -215,   137,   354,  -215,
     493,  1069,  -215,  1042,   358,   359,   360,   917,   778,   192,
     777,   137,   464,   891,   912,   977,   973,   878,   742,   913,
    -391,   859,   861,  1075,   866,   869,   896,   900,  1032,  1056,
    -391,  1068,  1061,   214,   708,   935,   936,   201,   937,   632,
     555,     0,   939,   940,   264,   265,   941,     0,     0,     0,
       0,     0,  -215,     0,     0,     0,     0,     0,  -215,  -391,
       0,   148,     0,     0,   958,   657,     0,     0,     0,     0,
     964,   148,   965,     0,     0,     0,   967,     0,     0,   949,
       0,   137,     0,     0,   148,  -391,   148,   131,     0,     0,
       0,     0,  -391,     0,     0,     0,     0,   131,  1040,     0,
       0,   935,   979,   980,   939,   981,   982,   983,   984,   137,
     131,     0,   131,  -391,   996,   137,     0,     0,     0,     0,
     991,     0,   137,     0,   334,   315,     0,     0,     0,     0,
     998,   148,  -391,     0,  1005,   288,   288,  -391,     0,     0,
    -391,  1062,  1063,  1064,  1065,  1066,  1067,     0,     0,     0,
       0,   997,     0,     0,   250,   250,     0,   131,   148,     0,
     250,   250,   148,     0,     0,  1028,  1029,  1030,  1031,     0,
       0,     0,   739,   137,   335,     0,     0,   137,   336,     0,
    1055,     0,     0,     0,   131,   137,     0,   900,   131,  1051,
    1052,   315,     0,     0,  1034,     0,     0,     0,     0,     0,
    1028,  1029,  1030,  1031,  1051,  1052,     0,     0,     0,   766,
       0,   334,     0,     0,     0,     0,  1057,   657,     0,  1060,
       0,     0,     0,   148,     0,   338,   339,     0,   340,   341,
     342,     0,   343,   344,   345,     0,     0,   347,  1034,     0,
       0,     0,     0,     0,   353,   354,   692,     0,   357,   131,
     657,   358,   359,   360,   900,     0,     0,     0,     0,     0,
       0,   335,     0,     0,     0,   336,     0,     0,     0,     0,
       0,     0,   250,   250,     0,  1034,   218,     0,     0,     0,
       0,     0,     0,     0,   250,     0,   431,     0,     0,   219,
     220,     0,   221,   431,   137,     0,   250,   222,     0,   250,
       0,   148,     0,     0,     0,   223,     0,     0,   148,     0,
       0,   224,   338,   339,     0,     0,   225,   342,   226,   343,
     344,   227,     0,     0,   347,     0,     0,   131,     0,   228,
       0,     0,   354,     0,   131,   229,   230,   893,   358,   359,
     360,     0,   231,     0,   137,     0,     0,     0,     0,   334,
     137,   232,     0,     0,     0,     0,     0,     0,     0,     0,
     233,   234,     0,   235,   148,   236,     0,   237,     0,     0,
     238,   836,     0,     0,   239,   437,     0,   240,   148,     0,
     241,     0,     0,   142,     0,   840,   841,     0,     0,     0,
     131,     0,     0,   142,     0,     0,     0,     0,     0,   335,
       0,     0,     0,   336,   131,     0,   142,     0,   142,     0,
     137,     0,   137,     0,     0,     0,     0,   137,     0,     0,
     137,     0,     0,     0,     0,     0,     0,   137,     0,     0,
     137,     0,     0,     0,     0,     0,   438,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   148,     0,
     338,     0,   137,   142,   145,   342,     0,   343,   344,   250,
       0,     0,   347,   137,   145,   137,     0,     0,     0,     0,
     354,     0,     0,     0,   131,     0,   148,   145,   360,   145,
     142,     0,   148,     0,   142,     0,     0,     0,     0,   148,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   276,
       0,     0,   131,     0,     0,   431,   431,     0,   131,   431,
     431,     0,     0,     0,     0,   131,     0,     0,     0,     0,
       0,     0,     0,     0,   145,     0,  1006,     0,     0,     0,
       0,     0,  1010,     0,   431,     0,   431,     0,     0,     0,
     148,     0,     0,     0,   148,   142,     0,     0,     0,   137,
       0,   145,   148,     0,     0,   145,   137,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   131,     0,   137,     0,
     131,   260,     0,     0,     0,    22,    23,     0,   131,     0,
     137,     0,     0,     0,   261,     0,    30,   262,     0,   137,
       0,    35,     0,   137,     0,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   145,     0,    52,     0,
      54,   334,    56,   142,   906,  1006,     0,   907,     0,   263,
     142,    62,     0,     0,     0,   137,     0,     0,     0,     0,
       0,     0,   416,   419,   421,   425,     0,     0,     0,     0,
      78,     0,     0,    80,     0,     0,    82,     0,     0,     0,
       0,   148,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   335,     0,     0,     0,   336,     0,     0,     0,   137,
     137,   137,   137,     0,     0,   448,   142,   131,   449,     0,
       0,     0,     0,     0,   145,     0,     0,     0,     0,     0,
     142,   145,     0,   137,   137,     0,    97,     0,     0,     0,
     458,   148,   908,     0,     0,     0,     0,   148,     0,     0,
       0,     0,   338,   339,     0,   340,   341,   342,   469,   343,
     344,   345,     0,     0,   347,   348,   349,   131,     0,   351,
     352,   353,   354,   131,     0,   357,     0,     0,   358,   359,
     360,     0,     0,     0,     0,     0,     0,   145,     0,     0,
       0,     0,     0,   146,     0,     0,     0,     0,     0,     0,
     142,   145,     0,   146,     0,     0,     0,   148,     0,   148,
       0,     0,     0,     0,   148,     0,   146,   148,   146,     0,
     334,     0,     0,     0,   148,     0,     0,   148,   142,     0,
       0,     0,     0,   131,   142,   131,     0,     0,     0,     0,
     131,   142,     0,   131,     0,     0,     0,     0,     0,   148,
     131,     0,     0,   131,     0,   566,     0,     0,     0,     0,
     148,     0,   148,   146,     0,     0,     0,     0,     0,     0,
     335,   145,     0,     0,   336,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   588,   131,     0,   131,   593,
     146,     0,   142,     0,   146,     0,   142,     0,     0,   145,
       0,     0,     0,     0,   142,   145,     0,     0,     0,     0,
       0,     0,   145,   150,     0,     0,     0,     0,     0,     0,
       0,   338,   339,   150,   340,   341,   342,     0,   343,   344,
       0,     0,     0,   347,     0,     0,   150,     0,   150,     0,
     353,   354,     0,     0,   357,     0,   148,   358,   359,   360,
       0,     0,     0,   148,     0,   146,     0,     0,     0,     0,
       0,     0,     0,   145,     0,   148,     0,   145,     0,     0,
       0,     0,   131,     0,     0,   145,     0,   148,     0,   131,
       0,     0,     0,   150,     0,   334,   148,     0,     0,     0,
     148,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   131,     0,     0,     0,     0,     0,     0,
     150,     0,   131,   142,   150,     0,   131,     0,     0,   151,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   151,
       0,     0,   148,   146,     0,   335,     0,     0,     0,   336,
     146,     0,   151,     0,   151,     0,     0,     0,   719,   721,
       0,     0,     0,     0,   726,   729,     0,     0,   131,     0,
       0,     0,     0,   142,     0,     0,     0,     0,     0,   142,
       0,     0,     0,     0,     0,   150,   148,   148,   148,   148,
       0,     0,     0,     0,   145,     0,   338,   339,     0,   151,
     341,   342,     0,   343,   344,     0,   146,     0,   347,     0,
     148,   148,   131,   131,   131,   131,   354,     0,     0,     0,
     146,     0,   358,   359,   360,     0,   151,     0,     0,     0,
     151,     0,     0,     0,     0,     0,   131,   131,     0,   142,
       0,   142,     0,     0,   145,     0,   142,     0,     0,   142,
     145,     0,     0,     0,     0,     0,   142,     0,     0,   142,
       0,     0,     5,   150,     0,     0,     0,     0,     0,     0,
     150,     0,     0,     0,     0,    15,   850,   851,    17,     0,
       0,   142,     0,    21,     0,     0,     0,     0,   856,     0,
     146,   151,   142,     0,   142,     0,     0,     0,     0,     0,
     867,     0,    37,   870,     0,     0,     0,    40,     0,     0,
     145,     0,   145,     0,     0,    45,     0,   145,   146,     0,
     145,     0,    50,     0,   146,     0,   150,   145,     0,     0,
     145,   146,     0,     0,     0,     0,     0,    60,     0,     0,
     150,     0,   260,     0,     0,     0,    22,    23,     0,    70,
       0,     0,   145,    73,     0,   261,     0,    30,   262,     0,
      79,     0,    35,   145,     0,   145,    83,    39,     0,   151,
       0,     0,     0,     0,     0,     0,   151,     0,   142,     0,
       0,     0,   146,     0,     0,   142,   146,     0,     0,    52,
       0,    54,     0,    56,   146,   906,     0,   142,   907,     0,
     263,     0,    62,     0,     0,     0,     0,     0,     0,   142,
     150,     0,     0,     0,     0,     0,     0,     0,   142,     0,
       0,    78,   142,     0,    80,     0,     0,    82,     0,     0,
       0,     0,   151,     0,     0,     0,     0,     0,   150,     0,
       0,     0,     0,     0,   150,     0,   151,     0,     0,   145,
       0,   150,     0,     0,     0,     0,   145,     0,     0,     0,
       0,     0,     0,   962,   142,     0,     0,     0,   145,     0,
       0,   104,     0,     0,     0,     0,     0,    97,     0,     0,
     145,   161,   242,   932,     0,     0,     0,     0,     0,   145,
       0,     0,     0,   145,   193,     0,   196,     0,     0,     0,
       0,     0,   150,   146,     0,     0,   150,     0,   142,   142,
     142,   142,     0,     0,   150,     0,   151,     0,     0,   292,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   142,   142,     0,   145,     0,     0,     0,     0,
       0,   282,     0,     0,   151,     0,     0,     0,     0,     0,
     151,     0,     0,   146,     0,     0,     0,   151,     0,   146,
       0,     0,     0,     0,     0,     0,     0,     0,   104,   323,
       0,   324,   318,     0,     0,     0,     0,     0,     0,   145,
     145,   145,   145,     0,     0,     0,     0,     0,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   145,   145,     0,     0,     0,   151,     0,
       0,     0,   151,     0,     0,     0,     0,     0,     0,   146,
     151,   146,     0,   150,     0,     0,   146,     0,     0,   146,
     292,     0,   376,   383,     0,  -524,   146,  -524,     0,   146,
     406,     0,     0,     0,     0,   410,   292,     0,   414,   218,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   146,   219,   220,     0,   221,     0,     0,     0,     0,
     222,     0,   146,   150,   146,     0,     0,     0,   223,   150,
       0,     0,     0,     0,   224,     0,   439,     0,     0,   225,
       0,   226,     0,   332,   227,     0,     0,     0,     0,     0,
     333,     0,   228,     0,     0,     0,     0,   451,   229,   230,
       0,   434,   334,     0,     0,   231,     0,     0,   447,     0,
       0,     0,     0,     0,   232,     0,     0,     0,     0,   151,
       0,     0,     0,   233,   234,     0,   235,     0,   236,   150,
     237,   150,     0,   238,     0,     0,   150,   239,     0,   150,
     240,     0,     0,   241,     0,     0,   150,     0,   146,   150,
       0,     0,   335,     0,     0,   146,   336,     0,     0,     0,
       0,     0,     0,     0,   104,     0,     0,   146,     0,   151,
       0,   150,     0,     0,     0,   151,     0,     0,   104,   146,
       0,     0,   150,   420,   150,     0,     0,   337,   146,     0,
       0,     0,   146,     0,     0,     0,     0,   389,     0,   765,
       0,     0,     0,   338,   339,     0,   340,   341,   342,     0,
     343,   344,   345,     0,   346,   347,   348,   349,   350,     0,
     351,   352,   353,   354,   355,   356,   357,     0,     0,   358,
     359,   360,     0,     0,   146,   151,   574,   151,   773,     0,
     577,     0,   151,     0,     0,   151,     0,     0,   104,     0,
       0,     0,   151,     0,     0,   151,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   150,   439,
       0,     0,     0,     0,     0,   150,   447,   151,   146,   146,
     146,   146,   447,     0,     0,     0,     0,   150,   151,     0,
     151,     0,     0,     0,     0,     0,     0,     0,     0,   150,
       0,     0,   146,   146,     0,     0,   260,     0,   150,     0,
      22,    23,   150,     0,     0,     0,     0,     0,     0,   261,
       0,    30,   262,     0,     0,   664,    35,     0,     0,     0,
     671,    39,     0,     0,     0,     0,     0,     0,     0,     0,
     651,     0,     0,     0,   656,     0,     0,     0,     0,     0,
       0,     0,   104,    52,   150,    54,     0,    56,     0,   906,
       0,     0,   907,     0,   263,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   151,     0,     0,     0,     0,     0,
       0,   151,     0,     0,     0,    78,     0,     0,    80,     0,
       0,    82,     0,   151,     0,     0,     0,     0,   150,   150,
     150,   150,     0,     0,     0,   151,     0,     0,     0,     0,
       0,     0,     0,     0,   151,     0,     0,     0,   151,     0,
       0,     0,   150,   150,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,     0,     0,   746,   934,     0,   218,
     439,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     151,   737,   219,   220,     0,   221,     0,     0,     0,     0,
     222,   767,   260,     0,     0,   292,    22,    23,   223,     0,
       0,     0,     0,     0,   224,   261,     0,    30,   262,   225,
       0,   226,    35,     0,   227,     0,     0,    39,     0,     0,
    -229,     0,   228,     0,   151,   151,   151,   151,   229,   230,
     831,   770,     0,     0,     0,   231,     0,   775,     0,    52,
       0,    54,     0,     0,   232,     0,  -229,     0,   151,   151,
     263,     0,    62,   233,   234,     0,   235,     0,   236,     0,
     237,     0,     0,   238,     0,     0,     0,   239,     0,     0,
     240,    78,     0,   241,    80,     0,   -63,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   877,   332,     0,   439,   447,     0,   447,
       0,   333,     0,     0,   447,     0,   218,   447,     0,     0,
       0,     0,     0,   334,   873,     0,   767,   875,     0,   219,
     220,     0,   221,     0,     0,     0,     0,   222,     0,   245,
       0,     0,     0,     0,   671,   223,     0,     0,     0,   889,
       0,   224,     0,     0,     0,     0,   225,     0,   226,     0,
     895,   227,   899,     0,     0,     0,     0,     0,     0,   228,
     916,     0,     0,   335,     0,   229,   230,   336,     0,     0,
       0,     0,   231,     0,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,     0,     0,     0,     0,
     233,   234,     0,   235,     0,   236,     0,   237,   337,     0,
     238,     0,     0,     0,   239,     0,     0,   240,     0,     0,
     241,     0,     0,     0,   338,   339,     0,   340,   341,   342,
       0,   343,   344,   345,     0,   346,   347,   348,   349,   350,
       0,   351,   352,   353,   354,   355,   356,   357,     0,     0,
     358,   359,   360,     0,     0,     0,     0,     0,     0,   849,
       0,     0,     0,     0,     0,   944,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   745,   961,     0,     0,
     831,     0,     0,     0,     0,     0,   966,     0,    -2,     4,
     968,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,   992,    38,    39,     0,    40,  -229,     0,    41,
      42,    43,     0,    44,    45,    46,   -43,    47,    48,     0,
      49,    50,    51,     0,     0,     0,    52,    53,    54,    55,
      56,    57,    58,  -229,   -43,    59,    60,    61,     0,    62,
      63,    64,     0,    65,    66,    67,    68,    69,    70,    71,
      72,     0,    73,    74,     0,    75,    76,    77,    78,    79,
       0,    80,    81,   -61,    82,    83,     0,     0,    84,     0,
      85,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,    97,     0,     0,    98,     0,    99,
       0,   100,  1053,   101,     5,     6,     7,     8,     9,    10,
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
      98,     0,   176,     0,   100,     0,   177,  1054,     4,     0,
       5,     6,     7,     8,     9,    10,    11,     0,  -552,     0,
      12,    13,    14,    15,    16,  -552,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,  -552,    27,    28,
    -552,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,    39,     0,    40,  -229,     0,    41,    42,
      43,     0,    44,    45,    46,   -43,    47,    48,     0,    49,
      50,    51,     0,     0,     0,    52,    53,    54,    55,     0,
      57,    58,  -229,   -43,    59,    60,    61,  -552,    62,    63,
      64,  -552,    65,    66,    67,    68,    69,    70,    71,    72,
       0,    73,    74,     0,    75,    76,    77,    78,    79,     0,
      80,    81,   -61,    82,    83,     0,     0,    84,     0,    85,
       0,     0,  -552,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -552,  -552,
      89,  -552,  -552,  -552,  -552,  -552,  -552,  -552,     0,  -552,
    -552,  -552,  -552,  -552,     0,  -552,  -552,  -552,  -552,  -552,
    -552,  -552,  -552,    97,  -552,  -552,  -552,     0,    99,  -552,
     100,   302,   101,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,     0,    38,    39,     0,    40,  -229,
       0,    41,    42,    43,     0,    44,    45,    46,   -43,    47,
      48,     0,    49,    50,    51,     0,     0,     0,    52,    53,
      54,    55,    56,    57,    58,  -229,   -43,    59,    60,    61,
       0,    62,    63,    64,     0,    65,    66,    67,    68,    69,
      70,    71,    72,     0,    73,    74,     0,    75,    76,    77,
      78,    79,     0,    80,    81,   -61,    82,    83,     0,     0,
      84,     0,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,   266,    97,     0,     0,    98,
       0,    99,   303,   100,     4,   101,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,     0,    38,    39,
       0,    40,  -229,     0,    41,    42,    43,     0,    44,    45,
      46,   -43,    47,    48,     0,    49,    50,    51,     0,     0,
       0,    52,    53,    54,    55,    56,    57,    58,  -229,   -43,
      59,    60,    61,     0,    62,    63,    64,     0,    65,    66,
      67,    68,    69,    70,    71,    72,     0,    73,    74,     0,
      75,    76,    77,    78,    79,     0,    80,    81,   -61,    82,
      83,     0,     0,    84,     0,    85,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,    97,
       0,     0,    98,     0,    99,   472,   100,   491,   101,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
       0,    38,    39,     0,    40,  -229,     0,    41,    42,    43,
       0,    44,    45,    46,   -43,    47,    48,     0,    49,    50,
      51,     0,     0,     0,    52,    53,    54,    55,    56,    57,
      58,  -229,   -43,    59,    60,    61,     0,    62,    63,    64,
       0,    65,    66,    67,    68,    69,    70,    71,    72,     0,
      73,    74,     0,    75,    76,    77,    78,    79,     0,    80,
      81,   -61,    82,    83,     0,     0,    84,     0,    85,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,    97,     0,     0,    98,     0,    99,   492,   100,
     302,   101,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,     0,    38,    39,     0,    40,  -229,     0,
      41,    42,    43,     0,    44,    45,    46,   -43,    47,    48,
       0,    49,    50,    51,     0,     0,     0,    52,    53,    54,
      55,    56,    57,    58,  -229,   -43,    59,    60,    61,     0,
      62,    63,    64,     0,    65,    66,    67,    68,    69,    70,
      71,    72,     0,    73,    74,     0,    75,    76,    77,    78,
      79,     0,    80,    81,   -61,    82,    83,     0,     0,    84,
       0,    85,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,    97,     0,     0,    98,     0,
      99,   303,   100,     4,   101,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,     0,    38,    39,     0,
      40,  -229,     0,    41,    42,    43,     0,    44,    45,    46,
     -43,    47,    48,     0,    49,    50,    51,     0,     0,     0,
      52,    53,    54,    55,    56,    57,    58,  -229,   -43,    59,
      60,    61,     0,    62,    63,    64,     0,    65,    66,    67,
      68,    69,    70,    71,    72,     0,    73,    74,     0,    75,
      76,    77,    78,    79,     0,    80,    81,   -61,    82,    83,
       0,     0,    84,     0,    85,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,    97,     0,
       0,    98,     0,    99,   663,   100,     4,   101,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,     0,
      38,    39,     0,    40,  -229,     0,    41,    42,    43,     0,
      44,    45,    46,   -43,    47,    48,     0,    49,    50,    51,
       0,     0,     0,    52,    53,    54,    55,   317,    57,    58,
    -229,   -43,    59,    60,    61,     0,    62,    63,    64,     0,
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
      36,    37,     0,    38,    39,     0,    40,  -229,     0,    41,
      42,    43,     0,    44,    45,    46,   -43,    47,    48,     0,
      49,    50,    51,     0,     0,     0,    52,    53,    54,    55,
       0,    57,    58,  -229,   -43,    59,    60,    61,     0,    62,
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
      33,    34,    35,   583,    37,     0,    38,    39,     0,    40,
    -229,     0,    41,    42,    43,     0,    44,    45,    46,   -43,
      47,    48,     0,    49,    50,    51,     0,     0,     0,    52,
      53,    54,    55,     0,    57,    58,  -229,   -43,    59,    60,
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
      30,    31,    32,    33,    34,    35,   589,    37,     0,    38,
      39,     0,    40,  -229,     0,    41,    42,    43,     0,    44,
      45,    46,   -43,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,     0,    57,    58,  -229,
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
       0,    29,     0,    30,    31,    32,    33,    34,    35,   858,
      37,     0,    38,    39,     0,    40,  -229,     0,    41,    42,
      43,     0,    44,    45,    46,   -43,    47,    48,     0,    49,
      50,    51,     0,     0,     0,    52,    53,    54,    55,     0,
      57,    58,  -229,   -43,    59,    60,    61,     0,    62,    63,
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
      34,    35,   860,    37,     0,    38,    39,     0,    40,  -229,
       0,    41,    42,    43,     0,    44,    45,    46,   -43,    47,
      48,     0,    49,    50,    51,     0,     0,     0,    52,    53,
      54,    55,     0,    57,    58,  -229,   -43,    59,    60,    61,
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
      31,    32,    33,    34,    35,   865,    37,     0,    38,    39,
       0,    40,  -229,     0,    41,    42,    43,     0,    44,    45,
      46,   -43,    47,    48,     0,    49,    50,    51,     0,     0,
       0,    52,    53,    54,    55,     0,    57,    58,  -229,   -43,
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
      29,     0,    30,    31,    32,    33,    34,    35,   868,    37,
       0,    38,    39,     0,    40,  -229,     0,    41,    42,    43,
       0,    44,    45,    46,   -43,    47,    48,     0,    49,    50,
      51,     0,     0,     0,    52,    53,    54,    55,     0,    57,
      58,  -229,   -43,    59,    60,    61,     0,    62,    63,    64,
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
     888,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,     0,    38,    39,     0,    40,  -229,     0,
      41,    42,    43,     0,    44,    45,    46,   -43,    47,    48,
       0,    49,    50,    51,     0,     0,     0,    52,    53,    54,
      55,     0,    57,    58,  -229,   -43,    59,    60,    61,     0,
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
      32,    33,    34,    35,   894,    37,     0,    38,    39,     0,
      40,  -229,     0,    41,    42,    43,     0,    44,    45,    46,
     -43,    47,    48,     0,    49,    50,    51,     0,     0,     0,
      52,    53,    54,    55,     0,    57,    58,  -229,   -43,    59,
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
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,   898,    37,     0,
      38,    39,     0,    40,  -229,     0,    41,    42,    43,     0,
      44,    45,    46,   -43,    47,    48,     0,    49,    50,    51,
       0,     0,     0,    52,    53,    54,    55,     0,    57,    58,
    -229,   -43,    59,    60,    61,     0,    62,    63,    64,     0,
      65,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      74,     0,    75,    76,    77,    78,    79,     0,    80,    81,
     -61,    82,    83,     0,     0,    84,     0,    85,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,    97,     0,     0,    98,     0,    99,     0,   100,   755,
     101,     5,     6,     7,     8,     9,    10,    11,     0,     0,
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
       0,    94,    95,   266,     0,     0,     0,    98,     0,   176,
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
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,   266,     0,     0,     0,
      98,     0,   176,     0,   100,     0,   177,   655,     5,     6,
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
      96,     0,     0,     0,    98,     0,   176,     0,   100,     0,
     177,   660,     5,     6,     7,     8,     9,    10,    11,     0,
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
       0,     0,    94,    95,   266,     0,     0,     0,    98,     0,
     176,     0,   100,     0,   177,   995,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   168,     0,     0,    15,
      16,     0,    17,     0,   169,     0,     0,    21,   213,    23,
       0,     0,     0,     0,     0,    28,     0,   170,     0,     0,
       0,    32,   171,   172,     0,   173,    37,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,    54,    55,     0,    57,     0,     0,     0,
       0,    60,   174,     0,    62,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   175,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,    82,
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
      73,     0,     0,    75,   270,   271,     0,    79,   307,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,   308,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,   266,     0,     0,     0,    98,     0,   176,     0,   100,
       0,   177,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   168,     0,     0,    15,    16,     0,    17,     0,
     169,     0,     0,    21,     0,   548,     0,     0,     0,     0,
       0,    28,     0,   170,     0,     0,     0,    32,   171,   172,
       0,   173,    37,   549,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   174,     0,
     550,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   175,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,   551,    83,     0,     0,     0,
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
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,   266,     0,     0,
       0,    98,     0,   176,     0,   100,   647,   177,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   168,     0,
       0,    15,    16,     0,    17,     0,   169,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   170,
       0,     0,     0,    32,   171,   172,     0,   173,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   174,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   175,     0,    73,
       0,     0,    75,   270,   271,     0,    79,   307,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
     266,     0,     0,     0,    98,     0,   176,     0,   100,   649,
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
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,   272,     0,     0,    98,     0,   176,
       0,   100,     0,   177,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   168,     0,     0,    15,    16,     0,
      17,     0,   169,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   170,     0,     0,     0,    32,
     171,   172,     0,   173,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,   757,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     174,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   175,     0,    73,     0,     0,    75,   270,
     271,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,   176,     0,   100,     0,   177,     5,     6,     7,
       8,   202,    10,    11,   203,     0,     0,   168,     0,     0,
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
      50,     0,     0,     0,     0,     0,    53,   207,    55,     0,
      57,     0,     0,     0,     0,    60,   174,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   175,
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
     208,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   176,     0,
     100,     0,   177,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   168,     0,     0,    15,    16,     0,    17,
       0,   169,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   170,     0,     0,     0,    32,   171,
     172,     0,   173,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   174,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   175,     0,    73,     0,     0,    75,   270,   271,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,   266,     0,     0,     0,    98,
       0,   176,     0,   100,     0,   177,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   168,     0,     0,    15,
      16,     0,    17,     0,   169,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   170,     0,     0,
       0,    32,   171,   172,     0,   173,    37,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   174,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   175,     0,    73,     0,     0,
      75,   270,   271,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,   787,
       0,   788,    98,     0,   176,     0,   100,     0,   177,   789,
       0,     0,     0,     0,   790,   220,   791,   792,     0,     0,
       0,     0,   793,     0,   260,     0,     0,     0,    22,    23,
     223,     0,     0,     0,     0,     0,   794,   261,     0,    30,
     262,   795,     0,   226,    35,     0,   796,     0,   797,    39,
       0,     0,  -229,     0,   798,     0,     0,     0,     0,     0,
     799,   800,     0,     0,     0,     0,     0,   231,     0,     0,
       0,    52,     0,    54,     0,     0,   801,     0,  -229,     0,
       0,     0,   263,     0,    62,   233,   234,     0,   802,     0,
     236,     0,   803,     0,     0,   804,     0,     0,     0,   805,
       0,     0,   240,    78,     0,   806,    80,     0,   -62,    82,
       0,     0,     0,     0,   361,   362,   363,   364,   365,   366,
       0,     0,   369,   370,   371,   372,     0,   374,   375,   807,
     808,   809,   810,   811,     0,     0,   812,     0,     0,     0,
     813,   814,   815,   816,   817,   818,   819,   820,   821,   822,
     823,     0,   824,     0,     0,   825,   826,   827,   828,     0,
       0,   829,     5,     6,     7,     8,     9,    10,    11,     0,
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
       0,     0,   208,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     176,     0,   100,     0,   177,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   168,     0,     0,    15,    16,
       0,    17,     0,   169,     0,     0,    21,     0,     0,     0,
       0,     0,     0,   249,    28,     0,   170,     0,     0,     0,
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
       0,    98,     0,    99,     0,   100,     0,   177,     5,     6,
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
     266,     0,     0,     0,    98,     0,   176,     0,   100,     0,
     177,     5,     6,     7,     8,   202,    10,    11,     0,     0,
       0,   168,     0,     0,    15,    16,     0,    17,     0,   169,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   170,     0,     0,     0,    32,   171,   172,     0,
     173,    37,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   204,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     175,     0,    73,     0,     0,    75,     0,     0,   205,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   176,
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
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,   266,     0,     0,     0,
      98,     0,   176,   645,   100,     0,   177,     5,     6,     7,
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
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,   176,     0,   100,     0,   177,
       5,     6,     7,     8,   198,    10,    11,     0,     0,     0,
     168,     0,     0,    15,    16,     0,    17,     0,   169,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   170,     0,     0,     0,    32,   171,   172,     0,   173,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   199,     0,     0,     0,
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
      93,     0,     0,    94,    95,   266,     0,     0,     0,    98,
       0,   176,     0,   100,     0,   177,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   168,     0,     0,    15,
      16,     0,    17,     0,   169,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   170,     0,     0,
       0,    32,   171,   172,     0,   173,    37,     0,    38,     0,
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
       0,     0,    98,     0,    99,     0,   100,     0,   177,     5,
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
      95,    96,     0,     0,     0,   594,     0,   176,     0,   100,
       0,   177,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   168,     0,     0,    15,    16,     0,    17,     0,
     169,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,   621,     0,   170,     0,     0,     0,    32,   171,   172,
       0,   173,    37,     0,   622,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   174,     0,
       0,     0,     0,     0,     0,     0,    67,   623,    69,    70,
      71,   624,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     176,     0,   100,     0,   625,     5,     6,     7,     8,     9,
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
       0,    98,     0,   176,     0,   100,     0,   625,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   168,     0,
       0,    15,    16,     0,    17,     0,   169,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   170,
       0,     0,     0,    32,   171,   172,     0,   695,    37,     0,
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
      96,     0,     0,     0,    98,     0,   176,     0,   100,     0,
     177,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   168,     0,     0,    15,    16,     0,    17,     0,   169,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   170,     0,     0,     0,    32,   171,   172,     0,
     698,    37,     0,    38,     0,     0,    40,     0,     0,     0,
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
       0,   100,     0,   177,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   168,     0,     0,    15,    16,     0,
      17,     0,   169,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   170,     0,     0,     0,    32,
     171,   172,     0,   924,    37,     0,    38,     0,     0,    40,
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
       0,     0,    32,   171,   172,     0,   925,    37,     0,    38,
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
       0,   170,     0,     0,     0,    32,   171,   172,     0,   927,
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
     172,     0,   928,    37,     0,    38,     0,     0,    40,     0,
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
       0,    32,   171,   172,     0,   929,    37,     0,    38,     0,
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
     170,     0,     0,     0,    32,   171,   172,     0,   930,    37,
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
       0,   621,     0,   170,     0,     0,     0,    32,   171,   172,
       0,   173,    37,     0,   622,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   174,     0,
       0,     0,     0,     0,     0,     0,    67,   623,    69,    70,
      71,   624,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,   266,     0,     0,     0,    98,     0,
     176,     0,   100,     0,   947,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   168,     0,     0,    15,    16,
       0,    17,     0,   169,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   621,     0,   170,     0,     0,     0,
      32,   171,   172,     0,   173,    37,     0,   622,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   174,     0,     0,     0,     0,     0,     0,     0,    67,
     623,    69,    70,    71,   624,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   176,     0,   100,     0,  1013,     5,     6,
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
     947,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   168,     0,     0,    15,    16,     0,    17,     0,   169,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   170,     0,     0,     0,    32,   171,   172,     0,
     930,    37,     0,    38,     0,     0,    40,     0,     0,     0,
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
       0,    94,    95,   266,     0,     0,     0,    98,     0,   176,
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
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,     0,     0,   100,     0,  1013,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   168,     0,     0,
      15,    16,     0,    17,     0,   169,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   170,     0,
       0,     0,    32,   171,   172,     0,   173,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,   260,    49,    50,     0,    22,
      23,     0,     0,    53,     0,    55,     0,    57,   261,     0,
      30,   262,    60,   174,     0,    35,     0,     0,     0,     0,
      39,    67,    68,    69,    70,    71,   175,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,    52,     0,    54,     0,     0,     0,     0,     0,
       0,     0,     0,   263,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    78,     0,     0,    80,     0,     0,
      82,     0,     0,     0,    93,     0,     0,     0,    95,    96,
       0,     0,     0,    98,     0,   176,     0,   100,     0,   177,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     168,     0,     0,    15,    16,     0,    17,     0,   169,   334,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   170,     0,     0,     0,    32,   171,   172,     0,   173,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,  -375,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,   335,
      57,     0,     0,   336,     0,    60,   174,     0,     0,     0,
    -375,     0,     0,     0,    67,    68,    69,    70,    71,   175,
       0,    73,     0,  -375,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,  -375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     338,   339,     0,   340,   341,   342,     0,   343,   344,   345,
      89,   346,   347,   348,   349,     0,     0,   351,   352,   353,
     354,   355,     0,   357,     0,     0,   358,   359,   360,     0,
       0,     0,    96,  -375,     0,     0,     0,     0,  -375,     0,
     100,     0,  1013,     5,     6,     7,     8,     9,    10,    11,
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
       0,     0,     0,     0,     0,     0,  -393,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,  -393,     0,     0,
       0,  -393,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   266,     0,     0,     0,     0,
       0,   176,     0,   100,  -393,   947,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   168,     0,     0,    15,
      16,     0,    17,     0,   169,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   170,     0,     0,
       0,    32,   171,   172,     0,   173,    37,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   174,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   175,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,     0,     0,     0,    87,     0,    89,     0,     5,     6,
       7,     8,     9,    10,    11,     0,     0,  -497,   168,     0,
       0,    15,    16,     0,    17,     0,   169,     0,    96,    21,
     376,     0,     0,  -524,   176,  -524,   100,    28,   177,   170,
       0,     0,     0,    32,   171,   172,     0,   173,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   174,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   175,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     168,     0,     0,    15,    16,     0,    17,     0,   169,     0,
      96,    21,     0,     0,     0,     0,   176,     0,   100,    28,
     625,   170,     0,     0,     0,    32,   171,   172,     0,   173,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   174,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   175,
       0,    73,   975,     0,    75,     0,     0,     0,    79,     0,
     789,    81,     0,     0,    83,   219,   220,   791,   221,     0,
       0,     0,     0,   222,     0,     0,     0,     0,     0,     0,
       0,   223,     0,     0,     0,     0,     0,   794,     0,     0,
      89,     0,   225,     0,   226,     0,     0,   227,     0,   797,
       0,     0,     0,     0,     0,   228,     0,     0,     0,     0,
       0,   799,   230,     0,     0,     0,   332,     0,   231,     0,
     100,     0,   177,   333,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,   334,   233,   234,     0,   235,
       0,   236,     0,   976,     0,     0,   804,     0,     0,     0,
     239,     0,     0,   240,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,     0,   361,   362,   363,   364,   365,
     366,     0,     0,   369,   370,   371,   372,     0,   374,   375,
     807,   808,   809,   810,   811,   335,     0,   812,     0,   336,
       0,   813,   814,   815,   816,   817,   818,   819,   820,   821,
     822,   823,     0,   824,   332,     0,   825,   826,   827,   828,
       0,   333,     0,     0,     0,     0,     0,     0,     0,     0,
     337,     0,     0,   334,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   338,   339,     0,   340,
     341,   342,     0,   343,   344,   345,     0,   346,   347,   348,
     349,   350,     0,   351,   352,   353,   354,   355,   356,   357,
       0,   332,   358,   359,   360,     0,     0,     0,   333,     0,
       0,  1009,     0,   335,     0,     0,     0,   336,     0,     0,
     334,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   332,     0,     0,     0,     0,     0,   337,   333,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   334,     0,     0,   338,   339,     0,   340,   341,   342,
     335,   343,   344,   345,   336,   346,   347,   348,   349,   350,
       0,   351,   352,   353,   354,   355,   356,   357,     0,     0,
     358,   359,   360,     0,     0,     0,     0,     0,     0,  1076,
       0,     0,     0,     0,     0,   337,     0,     0,     0,     0,
       0,   335,     0,     0,     0,   336,     0,     0,     0,     0,
       0,   338,   339,     0,   340,   341,   342,     0,   343,   344,
     345,     0,   346,   347,   348,   349,   350,     0,   351,   352,
     353,   354,   355,   356,   357,   332,   337,   358,   359,   360,
       0,     0,   333,     0,   646,     0,     0,     0,     0,     0,
       0,     0,   338,   339,   334,   340,   341,   342,     0,   343,
     344,   345,     0,   346,   347,   348,   349,   350,     0,   351,
     352,   353,   354,   355,   356,   357,     0,     0,   358,   359,
     360,     0,     0,     0,     0,   974,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   335,     0,     0,     0,   336,     0,
       0,   332,     0,     0,     0,     0,     0,   430,   333,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     334,     0,     0,     0,     0,     0,     0,     0,     0,   337,
     361,   362,   363,   364,   365,   366,     0,     0,   369,   370,
     371,   372,     0,   374,   375,   338,   339,     0,   340,   341,
     342,     0,   343,   344,   345,     0,   346,   347,   348,   349,
     350,     0,   351,   352,   353,   354,   355,   356,   357,     0,
     335,   358,   359,   360,   336,   389,     0,   332,     0,     0,
       0,     0,     0,   857,   333,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   334,     0,     0,     0,
       0,     0,     0,     0,     0,   337,   361,   362,   363,   364,
     365,   366,     0,     0,   369,   370,   371,   372,     0,   374,
     375,   338,   339,     0,   340,   341,   342,     0,   343,   344,
     345,     0,   346,   347,   348,   349,   350,     0,   351,   352,
     353,   354,   355,   356,   357,     0,   335,   358,   359,   360,
     336,   389,     0,   332,     0,     0,     0,     0,     0,   864,
     333,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   334,     0,     0,     0,     0,     0,     0,     0,
       0,   337,   361,   362,   363,   364,   365,   366,     0,     0,
     369,   370,   371,   372,     0,   374,   375,   338,   339,     0,
     340,   341,   342,     0,   343,   344,   345,     0,   346,   347,
     348,   349,   350,     0,   351,   352,   353,   354,   355,   356,
     357,     0,   335,   358,   359,   360,   336,   389,     0,   332,
       0,     0,     0,     0,     0,   987,   333,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   334,     0,
       0,     0,     0,     0,     0,     0,     0,   337,   361,   362,
     363,   364,   365,   366,     0,     0,   369,   370,   371,   372,
       0,   374,   375,   338,   339,     0,   340,   341,   342,     0,
     343,   344,   345,     0,   346,   347,   348,   349,   350,     0,
     351,   352,   353,   354,   355,   356,   357,     0,   335,   358,
     359,   360,   336,   389,     0,   332,     0,     0,     0,     0,
       0,   988,   333,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   334,     0,     0,     0,     0,     0,
       0,     0,     0,   337,   361,   362,   363,   364,   365,   366,
       0,     0,   369,   370,   371,   372,     0,   374,   375,   338,
     339,     0,   340,   341,   342,     0,   343,   344,   345,     0,
     346,   347,   348,   349,   350,     0,   351,   352,   353,   354,
     355,   356,   357,     0,   335,   358,   359,   360,   336,   389,
       0,   332,     0,     0,     0,     0,     0,   989,   333,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     334,     0,     0,     0,     0,     0,     0,     0,     0,   337,
     361,   362,   363,   364,   365,   366,     0,     0,   369,   370,
     371,   372,     0,   374,   375,   338,   339,     0,   340,   341,
     342,     0,   343,   344,   345,     0,   346,   347,   348,   349,
     350,     0,   351,   352,   353,   354,   355,   356,   357,     0,
     335,   358,   359,   360,   336,   389,     0,   332,     0,     0,
       0,     0,     0,   990,   333,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   334,     0,     0,     0,
       0,     0,     0,     0,     0,   337,   361,   362,   363,   364,
     365,   366,     0,     0,   369,   370,   371,   372,     0,   374,
     375,   338,   339,     0,   340,   341,   342,     0,   343,   344,
     345,     0,   346,   347,   348,   349,   350,     0,   351,   352,
     353,   354,   355,   356,   357,     0,   335,   358,   359,   360,
     336,   389,     0,   332,     0,     0,     0,     0,     0,  1011,
     333,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   334,     0,     0,     0,     0,     0,     0,     0,
       0,   337,   361,   362,   363,   364,   365,   366,     0,     0,
     369,   370,   371,   372,     0,   374,   375,   338,   339,     0,
     340,   341,   342,     0,   343,   344,   345,     0,   346,   347,
     348,   349,   350,     0,   351,   352,   353,   354,   355,   356,
     357,     0,   335,   358,   359,   360,   336,   389,     0,     0,
       0,     0,     0,     0,     0,  1012,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   337,   361,   362,
     363,   364,   365,   366,     0,     0,   369,   370,   371,   372,
       0,   374,   375,   338,   339,     0,   340,   341,   342,     0,
     343,   344,   345,     0,   346,   347,   348,   349,   350,     0,
     351,   352,   353,   354,   355,   356,   357,   332,     0,   358,
     359,   360,     0,   389,   333,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   334,   423,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   332,
     424,     0,     0,     0,     0,     0,   333,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   334,   417,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   418,     0,     0,     0,   335,     0,     0,     0,
     336,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   332,     0,     0,     0,
       0,     0,     0,   333,     0,     0,     0,   420,   335,     0,
       0,   337,   336,     0,     0,   334,   728,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   338,   339,     0,
     340,   341,   342,     0,   343,   344,   345,     0,   346,   347,
     348,   349,   350,   337,   351,   352,   353,   354,   355,   356,
     357,     0,     0,   358,   359,   360,     0,   389,     0,   338,
     339,     0,   340,   341,   342,   335,   343,   344,   345,   336,
     346,   347,   348,   349,   350,     0,   351,   352,   353,   354,
     355,   356,   357,   332,     0,   358,   359,   360,     0,   389,
     333,     0,     0,     0,     0,     0,   420,     0,     0,     0,
     337,     0,   334,   249,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   338,   339,     0,   340,
     341,   342,     0,   343,   344,   345,     0,   346,   347,   348,
     349,   350,     0,   351,   352,   353,   354,   355,   356,   357,
     332,     0,   358,   359,   360,     0,   389,   333,     0,     0,
       0,     0,   335,     0,     0,     0,   336,     0,     0,   334,
     720,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   332,     0,     0,     0,     0,     0,     0,   333,
       0,     0,     0,     0,     0,     0,     0,   337,     0,     0,
       0,   334,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   338,   339,     0,   340,   341,   342,   335,
     343,   344,   345,   336,   346,   347,   348,   349,   350,     0,
     351,   352,   353,   354,   355,   356,   357,     0,     0,   358,
     359,   360,     0,   389,     0,     0,     0,     0,     0,     0,
       0,   335,     0,     0,   337,   336,     0,     0,     0,     0,
       0,     0,     0,     0,   738,     0,     0,     0,     0,     0,
     338,   339,     0,   340,   341,   342,     0,   343,   344,   345,
       0,   346,   347,   348,   349,   350,   337,   351,   352,   353,
     354,   355,   356,   357,     0,     0,   358,   359,   360,     0,
     389,     0,   338,   339,     0,   340,   341,   342,     0,   343,
     344,   345,     0,   346,   347,   348,   349,   350,     0,   351,
     352,   353,   354,   355,   356,   357,   332,     0,   358,   359,
     360,     0,   389,   333,  -210,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   334,     0,  -210,  -210,     0,
    -210,     0,     0,     0,     0,  -210,     0,     0,   332,     0,
       0,     0,     0,  -210,     0,   333,     0,     0,     0,  -210,
       0,     0,     0,     0,  -210,     0,  -210,   334,     0,  -210,
       0,     0,   427,     0,     0,     0,     0,  -210,     0,     0,
       0,     0,     0,  -210,  -210,   335,     0,     0,     0,   336,
    -210,     0,     0,     0,     0,     0,     0,     0,     0,  -210,
       0,     0,   428,     0,     0,   332,     0,     0,  -210,  -210,
       0,  -210,   333,  -210,     0,  -210,     0,   335,  -210,     0,
     337,   336,  -210,     0,   334,  -210,     0,     0,  -210,     0,
       0,     0,     0,     0,     0,     0,   338,   339,   391,   340,
     341,   342,     0,   343,   344,   345,     0,   346,   347,   348,
     349,   350,   337,   351,   352,   353,   354,   355,   356,   357,
       0,     0,   358,   359,   360,     0,   457,     0,   338,   339,
       0,   340,   341,   342,   335,   343,   344,   345,   336,   346,
     347,   348,   349,   350,     0,   351,   352,   353,   354,   355,
     356,   357,   332,   429,   358,   359,   360,     0,     0,   333,
       0,     0,     0,     0,     0,   163,     0,     0,     0,   337,
       0,   334,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   338,   339,     0,   340,   341,
     342,     0,   343,   344,   345,     0,   346,   347,   348,   349,
     350,     0,   351,   352,   353,   354,   355,   356,   357,   332,
       0,   358,   359,   360,     0,     0,   333,     0,     0,     0,
       0,   335,     0,     0,     0,   336,     0,     0,   334,   557,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   332,   558,     0,     0,     0,     0,     0,   333,     0,
       0,     0,     0,     0,     0,     0,   337,     0,     0,     0,
     334,   559,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   338,   339,   560,   340,   341,   342,   335,   343,
     344,   345,   336,   346,   347,   348,   349,   350,     0,   351,
     352,   353,   354,   355,   356,   357,     0,   461,   358,   359,
     360,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     335,     0,     0,   337,   336,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   338,
     339,     0,   340,   341,   342,     0,   343,   344,   345,     0,
     346,   347,   348,   349,   350,   337,   351,   352,   353,   354,
     355,   356,   357,     0,     0,   358,   359,   360,     0,     0,
       0,   338,   339,     0,   340,   341,   342,     0,   343,   344,
     345,     0,   346,   347,   348,   349,   350,     0,   351,   352,
     353,   354,   355,   356,   357,   332,     0,   358,   359,   360,
       0,     0,   333,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   334,     0,     0,     0,     0,   666,
       0,     0,     0,     0,     0,     0,     0,   332,     0,     0,
       0,     0,     0,     0,   333,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   334,     0,     0,   667,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   335,     0,     0,     0,   336,     0,
       0,     0,     0,     0,     0,     0,   332,     0,     0,     0,
       0,     0,     0,   333,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   334,   335,     0,     0,   337,
     336,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   338,   339,     0,   340,   341,
     342,     0,   343,   344,   345,     0,   346,   347,   348,   349,
     350,   337,   351,   352,   353,   354,   355,   356,   357,     0,
       0,   358,   359,   360,     0,   335,     0,   338,   339,   336,
     340,   341,   342,     0,   343,   344,   345,     0,   346,   347,
     348,   349,   350,     0,   351,   352,   353,   354,   355,   356,
     357,   332,   463,   358,   359,   360,     0,     0,   333,     0,
     337,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     334,     0,     0,     0,     0,     0,   338,   339,     0,   340,
     341,   342,     0,   343,   344,   345,     0,   346,   347,   348,
     349,   350,     0,   351,   352,   353,   354,   355,   356,   357,
     332,   465,   358,   359,   360,     0,     0,   333,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   334,
     335,     0,     0,     0,   336,     0,     0,     0,     0,     0,
       0,     0,   332,     0,     0,     0,     0,     0,     0,   333,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   334,     0,     0,     0,   337,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   335,
       0,   338,   339,   336,   340,   341,   342,     0,   343,   344,
     345,     0,   346,   347,   348,   349,   350,     0,   351,   352,
     353,   354,   355,   356,   357,     0,   470,   358,   359,   360,
       0,   335,     0,   487,   337,   336,     0,     0,     0,     0,
       0,     0,     0,     0,   561,     0,     0,     0,     0,     0,
     338,   339,     0,   340,   341,   342,     0,   343,   344,   345,
       0,   346,   347,   348,   349,   350,   337,   351,   352,   353,
     354,   355,   356,   357,     0,     0,   358,   359,   360,     0,
       0,     0,   338,   339,     0,   340,   341,   342,     0,   343,
     344,   345,     0,   346,   347,   348,   349,   350,     0,   351,
     352,   353,   354,   355,   356,   357,   332,     0,   358,   359,
     360,     0,     0,   333,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   334,     0,   260,     0,     0,
       0,    22,    23,     0,     0,     0,     0,     0,     0,     0,
     261,     0,    30,   262,     0,     0,     0,    35,     0,     0,
       0,   332,    39,     0,     0,     0,     0,     0,   333,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     334,     0,     0,     0,    52,   335,    54,     0,    56,   336,
     906,     0,     0,   907,     0,   263,     0,    62,     0,     0,
       0,     0,   332,     0,     0,     0,     0,     0,     0,   333,
       0,     0,     0,     0,     0,     0,    78,     0,     0,    80,
     337,   334,    82,     0,     0,     0,     0,     0,     0,     0,
     335,     0,     0,     0,   336,     0,   338,   339,     0,   340,
     341,   342,     0,   343,   344,   345,     0,   346,   347,   348,
     349,   350,  -498,   351,   352,   353,   354,   355,   356,   357,
       0,   163,   358,   359,   360,   337,     0,     0,     0,     0,
       0,   335,    97,     0,     0,   336,     0,     0,   985,     0,
       0,   338,   339,     0,   340,   341,   342,     0,   343,   344,
     345,     0,   346,   347,   348,   349,   350,     0,   351,   352,
     353,   354,   355,   356,   357,     0,   337,   358,   359,   360,
       0,     0,     0,   332,   733,     0,     0,     0,     0,     0,
     333,     0,   338,   339,     0,   340,   341,   342,     0,   343,
     344,   345,   334,   346,   347,   348,   349,   350,     0,   351,
     352,   353,   354,   355,   356,   357,     0,   705,   358,   359,
     360,     0,     0,   332,     0,     0,     0,     0,     0,     0,
     333,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   334,     0,     0,   736,     0,     0,     0,     0,
       0,     0,   335,     0,     0,     0,   336,     0,     0,     0,
       0,     0,     0,     0,   332,     0,     0,     0,     0,     0,
       0,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   334,     0,     0,     0,   337,     0,     0,
       0,     0,   335,     0,     0,     0,   336,     0,     0,     0,
       0,     0,     0,   338,   339,     0,   340,   341,   342,     0,
     343,   344,   345,     0,   346,   347,   348,   349,   350,     0,
     351,   352,   353,   354,   355,   356,   357,   337,     0,   358,
     359,   360,     0,   335,     0,     0,     0,   336,     0,     0,
       0,     0,     0,   338,   339,     0,   340,   341,   342,     0,
     343,   344,   345,     0,   346,   347,   348,   349,   350,     0,
     351,   352,   353,   354,   355,   356,   357,   776,   337,   358,
     359,   360,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   338,   339,     0,   340,   341,   342,
       0,   343,   344,   345,     0,   346,   347,   348,   349,   350,
       0,   351,   352,   353,   354,   355,   356,   357,   332,     0,
     358,   359,   360,     0,   670,   333,   218,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   334,   844,   219,
     220,     0,   221,     0,     0,     0,     0,   222,     0,     0,
     332,     0,     0,     0,     0,   223,     0,   333,     0,     0,
       0,   224,     0,     0,     0,     0,   225,     0,   226,   334,
     847,   227,     0,     0,     0,     0,     0,     0,     0,   228,
       0,     0,     0,     0,     0,   229,   230,   335,     0,     0,
       0,   336,   231,     0,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,     0,     0,     0,     0,
     233,   234,     0,   235,     0,   236,     0,   237,     0,   335,
     238,     0,   337,   336,   239,     0,     0,   240,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,   338,   339,
       0,   340,   341,   342,     0,   343,   344,   345,     0,   346,
     347,   348,   349,   350,   337,   351,   352,   353,   354,   355,
     356,   357,     0,     0,   358,   359,   360,     0,     0,     0,
     338,   339,     0,   340,   341,   342,     0,   343,   344,   345,
       0,   346,   347,   348,   349,   350,     0,   351,   352,   353,
     354,   355,   356,   357,   332,     0,   358,   359,   360,     0,
       0,   333,  -220,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   334,     0,  -220,  -220,     0,  -220,     0,
       0,     0,     0,  -220,     0,     0,   332,     0,     0,     0,
       0,  -220,     0,   333,     0,     0,     0,  -220,     0,     0,
       0,     0,  -220,     0,  -220,   334,     0,  -220,     0,     0,
       0,     0,     0,     0,     0,  -220,     0,     0,     0,     0,
       0,  -220,  -220,   335,     0,     0,     0,   336,  -220,     0,
       0,     0,     0,     0,     0,     0,   923,  -220,     0,     0,
       0,     0,     0,   332,   942,     0,  -220,  -220,     0,  -220,
     333,  -220,     0,  -220,     0,   335,  -220,     0,   337,   336,
    -220,     0,   334,  -220,     0,     0,  -220,     0,   926,     0,
       0,     0,     0,     0,   338,   339,     0,   340,   341,   342,
       0,   343,   344,   345,     0,   346,   347,   348,   349,   350,
     337,   351,   352,   353,   354,   355,   356,   357,     0,     0,
     358,   359,   360,     0,     0,     0,   338,   339,     0,   340,
     341,   342,   335,   343,   344,   345,   336,   346,   347,   348,
     349,   350,     0,   351,   352,   353,   354,   355,   356,   357,
     332,     0,   358,   359,   360,     0,     0,   333,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   337,     0,   334,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   338,   339,     0,   340,   341,   342,     0,
     343,   344,   345,     0,   346,   347,   348,   349,   350,     0,
     351,   352,   353,   354,   355,   356,   357,     0,     0,   358,
     359,   360,     0,     0,     0,     0,     0,     0,     0,   335,
       0,     0,     0,   336,     0,     0,   332,     0,     0,     0,
       0,     0,  1022,   333,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   334,     0,     0,     0,     0,
       0,     0,     0,     0,   337,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     338,   339,     0,   340,   341,   342,     0,   343,   344,   345,
       0,   346,   347,   348,   349,   350,     0,   351,   352,   353,
     354,   355,   356,   357,     0,   335,   358,   359,   360,   336,
       0,     0,   332,     0,     0,     0,     0,     0,  1023,   333,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   334,     0,     0,     0,     0,     0,     0,     0,     0,
     337,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   338,   339,     0,   340,
     341,   342,     0,   343,   344,   345,     0,   346,   347,   348,
     349,   350,     0,   351,   352,   353,   354,   355,   356,   357,
       0,   335,   358,   359,   360,   336,     0,     0,   332,     0,
       0,     0,     0,     0,  1024,   333,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   334,     0,     0,
       0,     0,     0,     0,     0,     0,   337,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   338,   339,     0,   340,   341,   342,     0,   343,
     344,   345,     0,   346,   347,   348,   349,   350,     0,   351,
     352,   353,   354,   355,   356,   357,     0,   335,   358,   359,
     360,   336,     0,     0,   332,     0,     0,     0,     0,     0,
    1025,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   334,     0,     0,     0,     0,     0,     0,
       0,     0,   337,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   338,   339,
       0,   340,   341,   342,     0,   343,   344,   345,     0,   346,
     347,   348,   349,   350,     0,   351,   352,   353,   354,   355,
     356,   357,     0,   335,   358,   359,   360,   336,     0,     0,
     332,     0,     0,     0,     0,     0,  1026,   333,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   334,
       0,     0,     0,     0,     0,     0,     0,     0,   337,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   338,   339,     0,   340,   341,   342,
       0,   343,   344,   345,     0,   346,   347,   348,   349,   350,
       0,   351,   352,   353,   354,   355,   356,   357,     0,   335,
     358,   359,   360,   336,     0,     0,   332,     0,     0,     0,
       0,     0,  1027,   333,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   334,     0,     0,     0,     0,
       0,     0,     0,     0,   337,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1049,
     338,   339,     0,   340,   341,   342,     0,   343,   344,   345,
       0,   346,   347,   348,   349,   350,     0,   351,   352,   353,
     354,   355,   356,   357,     0,   335,   358,   359,   360,   336,
       0,     0,   332,     0,     0,     0,     0,     0,     0,   333,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   334,     0,     0,     0,     0,     0,     0,     0,     0,
     337,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   338,   339,     0,   340,
     341,   342,     0,   343,   344,   345,     0,   346,   347,   348,
     349,   350,     0,   351,   352,   353,   354,   355,   356,   357,
       0,   335,   358,   359,   360,   336,     0,     0,   332,     0,
       0,     0,     0,     0,     0,   333,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   334,     0,     0,
       0,     0,     0,     0,     0,     0,   337,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   338,   339,     0,   340,   341,   342,     0,   343,
     344,   345,     0,   346,   347,   348,   349,   350,     0,   351,
     352,   353,   354,   355,   356,   357,     0,   693,   358,   359,
     360,   336,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   337,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   338,   339,
       0,   340,   341,   342,     0,   343,   344,   345,   334,   346,
     347,   348,   349,   350,     0,   351,   352,   353,   354,   355,
     356,   357,     0,   260,   358,   359,   360,    22,    23,     0,
       0,     0,     0,     0,     0,     0,   261,     0,    30,   262,
       0,     0,     0,    35,     0,     0,     0,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   335,     0,
       0,   334,   336,     0,     0,     0,     0,     0,     0,     0,
      52,     0,    54,     0,    56,     0,   906,     0,     0,   907,
       0,   263,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   337,     0,     0,     0,     0,     0,     0,
       0,     0,    78,     0,     0,    80,     0,     0,    82,   338,
     339,   335,   340,   341,   342,   336,   343,   344,   345,     0,
     346,   347,   348,   349,     0,     0,   351,   352,   353,   354,
     355,   356,   357,     0,     0,   358,   359,   360,     0,     0,
       0,     0,     0,     0,     0,     0,   337,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,     0,   338,   339,   986,   340,   341,   342,     0,   343,
     344,   345,     0,   346,   347,   348,   349,     0,   218,   351,
     352,   353,   354,   355,     0,   357,     0,     0,   358,   359,
     360,   219,   220,     0,   221,     0,     0,     0,     0,   222,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,     0,   224,     0,     0,     0,     0,   225,     0,
     226,     0,     0,   227,     0,     0,     0,     0,     0,     0,
       0,   228,     0,     0,     0,  -211,     0,   229,   230,     0,
       0,     0,     0,     0,   231,     0,     0,     0,  -211,  -211,
       0,  -211,     0,   232,     0,     0,  -211,     0,     0,     0,
       0,     0,   233,   234,  -211,   235,     0,   236,     0,   237,
    -211,     0,   238,     0,     0,  -211,   239,  -211,     0,   240,
    -211,     0,   241,     0,     0,     0,     0,     0,  -211,     0,
       0,     0,     0,     0,  -211,  -211,     0,     0,     0,     0,
       0,  -211,     0,     0,     0,     0,     0,     0,     0,     0,
    -211,     0,     0,     0,     0,     0,     0,     0,     0,  -211,
    -211,     0,  -211,     0,  -211,     0,  -211,     0,     0,  -211,
       0,     0,     0,  -211,     0,     0,  -211,     0,     0,  -211
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-940)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      12,   148,   212,     2,    16,   466,    58,    59,    20,   382,
     587,    19,   754,   443,    26,   189,   135,   679,    30,    31,
      22,    33,    34,    35,    36,   428,   406,   620,   217,   957,
     410,   205,    44,     1,    46,    47,   466,     1,   877,    51,
      63,    53,   438,    55,   834,   835,   435,     1,    83,    31,
      44,    63,    64,    65,    66,    67,    68,   780,     1,    44,
      72,    31,    74,   111,    76,    77,     3,    83,   995,    81,
     259,   190,    84,    85,     0,    87,   131,    89,   101,     3,
      92,    93,    31,    95,   139,    31,    64,    99,   100,   101,
     710,    44,   281,   713,     1,    31,   111,   839,    54,   692,
      54,   880,    66,   313,   314,    33,    34,   946,   137,   100,
      44,    54,  1039,   690,   156,  1054,   197,   111,   166,   137,
     137,   156,   100,    44,    44,   111,   161,    30,  1056,    83,
     853,    44,   855,  1072,    44,   164,   217,    44,   156,   881,
      83,   288,    96,   137,   108,   161,    83,    81,   116,   166,
      49,   166,   137,   109,   177,   109,   168,    94,   751,   171,
     172,   173,   111,   175,   176,   177,   109,   137,   136,   137,
      94,    81,   166,   101,   137,   111,    75,    80,   259,   161,
     188,    84,   136,  1039,   137,   197,   909,   977,   978,   378,
     379,   161,   971,   136,   574,    66,   164,   577,   162,   201,
     281,   164,   156,   945,   206,   217,   197,   161,   931,   116,
     933,   213,   161,   166,   587,   161,   137,   137,   161,   137,
    1076,    26,    26,   619,   137,   161,   217,    26,   131,   136,
     137,    26,   852,   136,   854,   138,   139,   108,   156,    44,
     143,   163,    47,   165,   109,   166,   166,   259,   151,   156,
     262,    46,   994,   166,   157,   158,   159,   919,   270,   271,
     137,   137,    67,   344,   667,    70,    70,    72,   259,   281,
     459,    70,   137,    68,   156,    70,    81,    81,   137,   137,
     162,   137,    81,     3,   137,   862,    81,   164,   164,   408,
     281,   162,    81,   116,   413,   100,   308,   378,   379,   322,
     156,   100,   106,   163,   162,   164,   305,   106,    30,   162,
     322,   106,   163,   163,   165,   165,   163,   690,   141,   331,
     332,   333,   334,   335,   336,   337,   137,   339,   340,   341,
     342,   343,   137,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   745,
      76,    77,   116,   164,   743,   367,   368,   163,    80,   164,
      91,   373,    84,   131,   376,   377,   378,   379,   380,   381,
     382,   139,   131,   137,     7,   137,   116,   141,   459,   391,
     139,   591,   592,   137,   396,   163,   398,   378,   379,   367,
     368,   780,   415,    54,   137,   373,    54,   137,   376,   377,
     164,   141,   164,   415,   137,   417,   418,   335,   336,   131,
     164,   423,   424,   156,   136,   427,   428,   139,   430,    44,
     432,   143,    83,    44,   164,    83,   887,   137,   156,    54,
     137,   164,   201,   445,   162,    96,   161,   206,    96,    76,
     452,   137,   105,   653,   654,    66,   156,   459,   109,   156,
     880,   109,   380,   381,   156,    76,    77,   887,    83,   156,
     156,   452,   474,   391,   853,   477,   855,   479,   459,    26,
     644,   483,   484,   485,   497,   487,   488,    80,    26,    54,
     163,    84,   165,   474,   109,   497,   477,   137,   479,   862,
     418,   116,   573,   484,   493,   156,   424,   131,   156,   477,
     161,   479,   136,   161,     2,   139,   156,    80,    83,   143,
     156,    84,   137,    70,    12,   155,   163,   163,   165,   165,
     156,    96,    70,   147,    81,   914,   477,    25,   479,    27,
     156,   156,   679,    81,   109,   163,   161,   156,   156,   164,
     156,   971,   931,   100,   933,   557,   558,   559,   560,   561,
     562,   563,   100,   161,   156,   483,   564,   565,   156,   571,
     110,   573,    79,    79,   163,   995,   187,   163,    44,   581,
     139,   583,   156,   163,    72,   587,   116,   589,    54,  1009,
     571,   156,   573,     7,   156,    58,   161,   136,   209,   210,
     211,   212,   156,   571,   116,   216,   161,   116,   164,     3,
     137,    99,   625,   156,    44,   103,    33,    83,   163,  1039,
     164,   623,   624,   625,   137,    19,   116,   629,   164,   164,
     142,   164,   144,   145,  1054,    20,   148,   149,    32,    26,
     558,   252,   560,   109,   255,   137,   166,   166,   163,   166,
     116,   161,  1072,   156,    21,   156,  1076,    44,    25,    26,
      47,   629,    56,   156,   666,   667,   277,    34,   164,    36,
      37,   137,   156,   156,    41,   161,   164,   156,   137,    46,
      67,   161,   156,    70,   295,    72,   156,    33,   690,   164,
     156,   693,   694,   695,    81,   161,   698,    30,   164,   164,
      94,    68,   162,    70,   137,    72,   100,    74,   162,   116,
      77,    81,    79,   100,    81,   164,   718,     1,   720,    31,
      31,   166,    26,   725,   161,   161,   728,    33,   136,   731,
      81,   733,   166,   100,   736,   136,   103,    21,   166,   106,
      44,    25,    26,    47,   166,   156,   156,    80,   761,   162,
      34,    84,    36,    37,   242,   139,   139,    41,   163,   761,
     139,   249,    46,    67,   139,    49,    70,   131,    72,   771,
     139,   773,   131,   139,   776,   693,   694,    81,   139,   139,
     782,   392,   919,   139,    68,   139,    70,   131,    72,   156,
      74,    75,   139,    77,   131,    79,   100,    81,   131,   132,
     131,   134,   135,   136,   139,   138,   139,    33,    96,   164,
     143,   422,   164,   131,   131,   426,   100,   305,   151,   103,
     319,   166,   106,   137,   157,   158,   159,   829,   667,    24,
     666,   319,   289,   765,   783,   919,   914,   743,   612,   786,
      44,   843,   844,  1056,   846,   847,   848,   849,   994,  1018,
      54,  1049,  1042,    35,   573,   857,   858,    30,   860,   446,
     862,    -1,   864,   865,   906,   907,   868,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,   162,    83,
      -1,     2,    -1,    -1,   886,   887,    -1,    -1,    -1,    -1,
     892,    12,   894,    -1,    -1,    -1,   898,    -1,    -1,   880,
      -1,   389,    -1,    -1,    25,   109,    27,     2,    -1,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    12,  1000,    -1,
      -1,   923,   924,   925,   926,   927,   928,   929,   930,   417,
      25,    -1,    27,   137,   947,   423,    -1,    -1,    -1,    -1,
     942,    -1,   430,    -1,    30,   947,    -1,    -1,    -1,    -1,
     952,    72,   156,    -1,   956,    76,    77,   161,    -1,    -1,
     164,  1043,  1044,  1045,  1046,  1047,  1048,    -1,    -1,    -1,
      -1,   952,    -1,    -1,   585,   586,    -1,    72,    99,    -1,
     591,   592,   103,    -1,    -1,   987,   988,   989,   990,    -1,
      -1,    -1,   603,   481,    80,    -1,    -1,   485,    84,    -1,
    1013,    -1,    -1,    -1,    99,   493,    -1,  1009,   103,  1011,
    1012,  1013,    -1,    -1,   995,    -1,    -1,    -1,    -1,    -1,
    1022,  1023,  1024,  1025,  1026,  1027,    -1,    -1,    -1,   640,
      -1,    30,    -1,    -1,    -1,    -1,  1038,  1039,    -1,  1041,
      -1,    -1,    -1,   164,    -1,   131,   132,    -1,   134,   135,
     136,    -1,   138,   139,   140,    -1,    -1,   143,  1039,    -1,
      -1,    -1,    -1,    -1,   150,   151,   554,    -1,   154,   164,
    1072,   157,   158,   159,  1076,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,   703,   704,    -1,  1076,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   715,    -1,   717,    -1,    -1,    16,
      17,    -1,    19,   724,   602,    -1,   727,    24,    -1,   730,
      -1,   242,    -1,    -1,    -1,    32,    -1,    -1,   249,    -1,
      -1,    38,   131,   132,    -1,    -1,    43,   136,    45,   138,
     139,    48,    -1,    -1,   143,    -1,    -1,   242,    -1,    56,
      -1,    -1,   151,    -1,   249,    62,    63,   768,   157,   158,
     159,    -1,    69,    -1,   652,    -1,    -1,    -1,    -1,    30,
     658,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    90,   305,    92,    -1,    94,    -1,    -1,
      97,   679,    -1,    -1,   101,   102,    -1,   104,   319,    -1,
     107,    -1,    -1,     2,    -1,   693,   694,    -1,    -1,    -1,
     305,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    84,   319,    -1,    25,    -1,    27,    -1,
     718,    -1,   720,    -1,    -1,    -1,    -1,   725,    -1,    -1,
     728,    -1,    -1,    -1,    -1,    -1,    -1,   735,    -1,    -1,
     738,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   389,    -1,
     131,    -1,   760,    72,     2,   136,    -1,   138,   139,   890,
      -1,    -1,   143,   771,    12,   773,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,   389,    -1,   417,    25,   159,    27,
      99,    -1,   423,    -1,   103,    -1,    -1,    -1,    -1,   430,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    -1,   417,    -1,    -1,   936,   937,    -1,   423,   940,
     941,    -1,    -1,    -1,    -1,   430,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    -1,   957,    -1,    -1,    -1,
      -1,    -1,   963,    -1,   965,    -1,   967,    -1,    -1,    -1,
     481,    -1,    -1,    -1,   485,   164,    -1,    -1,    -1,   857,
      -1,    99,   493,    -1,    -1,   103,   864,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   481,    -1,   876,    -1,
     485,    21,    -1,    -1,    -1,    25,    26,    -1,   493,    -1,
     888,    -1,    -1,    -1,    34,    -1,    36,    37,    -1,   897,
      -1,    41,    -1,   901,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,    68,    -1,
      70,    30,    72,   242,    74,  1056,    -1,    77,    -1,    79,
     249,    81,    -1,    -1,    -1,   943,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,   211,   212,    -1,    -1,    -1,    -1,
     100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
      -1,   602,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    -1,   987,
     988,   989,   990,    -1,    -1,   252,   305,   602,   255,    -1,
      -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,
     319,   249,    -1,  1011,  1012,    -1,   156,    -1,    -1,    -1,
     277,   652,   162,    -1,    -1,    -1,    -1,   658,    -1,    -1,
      -1,    -1,   131,   132,    -1,   134,   135,   136,   295,   138,
     139,   140,    -1,    -1,   143,   144,   145,   652,    -1,   148,
     149,   150,   151,   658,    -1,   154,    -1,    -1,   157,   158,
     159,    -1,    -1,    -1,    -1,    -1,    -1,   305,    -1,    -1,
      -1,    -1,    -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,
     389,   319,    -1,    12,    -1,    -1,    -1,   718,    -1,   720,
      -1,    -1,    -1,    -1,   725,    -1,    25,   728,    27,    -1,
      30,    -1,    -1,    -1,   735,    -1,    -1,   738,   417,    -1,
      -1,    -1,    -1,   718,   423,   720,    -1,    -1,    -1,    -1,
     725,   430,    -1,   728,    -1,    -1,    -1,    -1,    -1,   760,
     735,    -1,    -1,   738,    -1,   392,    -1,    -1,    -1,    -1,
     771,    -1,   773,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      80,   389,    -1,    -1,    84,   760,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   422,   771,    -1,   773,   426,
      99,    -1,   481,    -1,   103,    -1,   485,    -1,    -1,   417,
      -1,    -1,    -1,    -1,   493,   423,    -1,    -1,    -1,    -1,
      -1,    -1,   430,     2,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,    12,   134,   135,   136,    -1,   138,   139,
      -1,    -1,    -1,   143,    -1,    -1,    25,    -1,    27,    -1,
     150,   151,    -1,    -1,   154,    -1,   857,   157,   158,   159,
      -1,    -1,    -1,   864,    -1,   164,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   481,    -1,   876,    -1,   485,    -1,    -1,
      -1,    -1,   857,    -1,    -1,   493,    -1,   888,    -1,   864,
      -1,    -1,    -1,    72,    -1,    30,   897,    -1,    -1,    -1,
     901,   876,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   888,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   897,   602,   103,    -1,   901,    -1,    -1,     2,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,   943,   242,    -1,    80,    -1,    -1,    -1,    84,
     249,    -1,    25,    -1,    27,    -1,    -1,    -1,   585,   586,
      -1,    -1,    -1,    -1,   591,   592,    -1,    -1,   943,    -1,
      -1,    -1,    -1,   652,    -1,    -1,    -1,    -1,    -1,   658,
      -1,    -1,    -1,    -1,    -1,   164,   987,   988,   989,   990,
      -1,    -1,    -1,    -1,   602,    -1,   131,   132,    -1,    72,
     135,   136,    -1,   138,   139,    -1,   305,    -1,   143,    -1,
    1011,  1012,   987,   988,   989,   990,   151,    -1,    -1,    -1,
     319,    -1,   157,   158,   159,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,  1011,  1012,    -1,   718,
      -1,   720,    -1,    -1,   652,    -1,   725,    -1,    -1,   728,
     658,    -1,    -1,    -1,    -1,    -1,   735,    -1,    -1,   738,
      -1,    -1,     3,   242,    -1,    -1,    -1,    -1,    -1,    -1,
     249,    -1,    -1,    -1,    -1,    16,   703,   704,    19,    -1,
      -1,   760,    -1,    24,    -1,    -1,    -1,    -1,   715,    -1,
     389,   164,   771,    -1,   773,    -1,    -1,    -1,    -1,    -1,
     727,    -1,    43,   730,    -1,    -1,    -1,    48,    -1,    -1,
     718,    -1,   720,    -1,    -1,    56,    -1,   725,   417,    -1,
     728,    -1,    63,    -1,   423,    -1,   305,   735,    -1,    -1,
     738,   430,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
     319,    -1,    21,    -1,    -1,    -1,    25,    26,    -1,    90,
      -1,    -1,   760,    94,    -1,    34,    -1,    36,    37,    -1,
     101,    -1,    41,   771,    -1,   773,   107,    46,    -1,   242,
      -1,    -1,    -1,    -1,    -1,    -1,   249,    -1,   857,    -1,
      -1,    -1,   481,    -1,    -1,   864,   485,    -1,    -1,    68,
      -1,    70,    -1,    72,   493,    74,    -1,   876,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,   888,
     389,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   897,    -1,
      -1,   100,   901,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,    -1,   305,    -1,    -1,    -1,    -1,    -1,   417,    -1,
      -1,    -1,    -1,    -1,   423,    -1,   319,    -1,    -1,   857,
      -1,   430,    -1,    -1,    -1,    -1,   864,    -1,    -1,    -1,
      -1,    -1,    -1,   890,   943,    -1,    -1,    -1,   876,    -1,
      -1,     2,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     888,    12,    41,   162,    -1,    -1,    -1,    -1,    -1,   897,
      -1,    -1,    -1,   901,    25,    -1,    27,    -1,    -1,    -1,
      -1,    -1,   481,   602,    -1,    -1,   485,    -1,   987,   988,
     989,   990,    -1,    -1,   493,    -1,   389,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1011,  1012,    -1,   943,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    -1,   417,    -1,    -1,    -1,    -1,    -1,
     423,    -1,    -1,   652,    -1,    -1,    -1,   430,    -1,   658,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   128,
      -1,   130,   103,    -1,    -1,    -1,    -1,    -1,    -1,   987,
     988,   989,   990,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,  1011,  1012,    -1,    -1,    -1,   481,    -1,
      -1,    -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,   718,
     493,   720,    -1,   602,    -1,    -1,   725,    -1,    -1,   728,
     189,    -1,   160,   164,    -1,   163,   735,   165,    -1,   738,
     199,    -1,    -1,    -1,    -1,   204,   205,    -1,   207,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   760,    16,    17,    -1,    19,    -1,    -1,    -1,    -1,
      24,    -1,   771,   652,   773,    -1,    -1,    -1,    32,   658,
      -1,    -1,    -1,    -1,    38,    -1,   245,    -1,    -1,    43,
      -1,    45,    -1,    11,    48,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    56,    -1,    -1,    -1,    -1,   266,    62,    63,
      -1,   242,    30,    -1,    -1,    69,    -1,    -1,   249,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,   602,
      -1,    -1,    -1,    87,    88,    -1,    90,    -1,    92,   718,
      94,   720,    -1,    97,    -1,    -1,   725,   101,    -1,   728,
     104,    -1,    -1,   107,    -1,    -1,   735,    -1,   857,   738,
      -1,    -1,    80,    -1,    -1,   864,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   305,    -1,    -1,   876,    -1,   652,
      -1,   760,    -1,    -1,    -1,   658,    -1,    -1,   319,   888,
      -1,    -1,   771,   111,   773,    -1,    -1,   115,   897,    -1,
      -1,    -1,   901,    -1,    -1,    -1,    -1,   161,    -1,   163,
      -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,    -1,    -1,   157,
     158,   159,    -1,    -1,   943,   718,   405,   720,   166,    -1,
     409,    -1,   725,    -1,    -1,   728,    -1,    -1,   389,    -1,
      -1,    -1,   735,    -1,    -1,   738,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   857,   438,
      -1,    -1,    -1,    -1,    -1,   864,   417,   760,   987,   988,
     989,   990,   423,    -1,    -1,    -1,    -1,   876,   771,    -1,
     773,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   888,
      -1,    -1,  1011,  1012,    -1,    -1,    21,    -1,   897,    -1,
      25,    26,   901,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    37,    -1,    -1,   494,    41,    -1,    -1,    -1,
     499,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     481,    -1,    -1,    -1,   485,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   493,    68,   943,    70,    -1,    72,    -1,    74,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   857,    -1,    -1,    -1,    -1,    -1,
      -1,   864,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,
      -1,   106,    -1,   876,    -1,    -1,    -1,    -1,   987,   988,
     989,   990,    -1,    -1,    -1,   888,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   897,    -1,    -1,    -1,   901,    -1,
      -1,    -1,  1011,  1012,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,    -1,    -1,   615,   162,    -1,     3,
     619,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     943,   602,    16,    17,    -1,    19,    -1,    -1,    -1,    -1,
      24,   640,    21,    -1,    -1,   644,    25,    26,    32,    -1,
      -1,    -1,    -1,    -1,    38,    34,    -1,    36,    37,    43,
      -1,    45,    41,    -1,    48,    -1,    -1,    46,    -1,    -1,
      49,    -1,    56,    -1,   987,   988,   989,   990,    62,    63,
     679,   652,    -1,    -1,    -1,    69,    -1,   658,    -1,    68,
      -1,    70,    -1,    -1,    78,    -1,    75,    -1,  1011,  1012,
      79,    -1,    81,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,
     104,   100,    -1,   107,   103,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   742,    11,    -1,   745,   718,    -1,   720,
      -1,    18,    -1,    -1,   725,    -1,     3,   728,    -1,    -1,
      -1,    -1,    -1,    30,   735,    -1,   765,   738,    -1,    16,
      17,    -1,    19,    -1,    -1,    -1,    -1,    24,    -1,   163,
      -1,    -1,    -1,    -1,   783,    32,    -1,    -1,    -1,   760,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    45,    -1,
     771,    48,   773,    -1,    -1,    -1,    -1,    -1,    -1,    56,
     809,    -1,    -1,    80,    -1,    62,    63,    84,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,   115,    -1,
      97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
     157,   158,   159,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,   876,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163,   888,    -1,    -1,
     919,    -1,    -1,    -1,    -1,    -1,   897,    -1,     0,     1,
     901,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      -1,    13,    14,    15,    16,    17,    -1,    19,    -1,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    31,
      32,    -1,    34,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,   943,    45,    46,    -1,    48,    49,    -1,    51,
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
      -1,   163,     1,   165,     3,     4,     5,     6,     7,     8,
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
     159,    -1,   161,    -1,   163,    -1,   165,   166,     1,    -1,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    46,    -1,    48,    49,    -1,    51,    52,
      53,    -1,    55,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    -1,    -1,    -1,    68,    69,    70,    71,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,   101,    -1,
     103,   104,   105,   106,   107,    -1,    -1,   110,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,    -1,   161,   162,
     163,     1,   165,     3,     4,     5,     6,     7,     8,     9,
      -1,    -1,    -1,    13,    14,    15,    16,    17,    -1,    19,
      -1,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    31,    32,    -1,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    -1,    45,    46,    -1,    48,    49,
      -1,    51,    52,    53,    -1,    55,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    -1,    -1,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    85,    86,    87,    88,    89,
      90,    91,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,    -1,    -1,
     110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,    -1,    -1,   153,   154,   155,   156,    -1,    -1,   159,
      -1,   161,   162,   163,     1,   165,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    -1,    13,    14,    15,    16,
      17,    -1,    19,    -1,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    31,    32,    -1,    34,    -1,    36,
      37,    38,    39,    40,    41,    42,    43,    -1,    45,    46,
      -1,    48,    49,    -1,    51,    52,    53,    -1,    55,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    -1,    -1,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,   101,    -1,   103,   104,   105,   106,
     107,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,   156,
      -1,    -1,   159,    -1,   161,   162,   163,     1,   165,     3,
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
     161,    -1,   163,    -1,   165,   166,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,    16,
      17,    -1,    19,    -1,    21,    -1,    -1,    24,    25,    26,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    34,    -1,    -1,
      -1,    38,    39,    40,    -1,    42,    43,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    53,    -1,    -1,    56,
      57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    -1,
      -1,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      87,    88,    89,    90,    91,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,   106,
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
      94,    -1,    -1,    97,    98,    99,    -1,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,   140,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,   153,
     154,   155,    -1,    -1,    -1,   159,    -1,   161,    -1,   163,
      -1,   165,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    -1,    -1,    16,    17,    -1,    19,    -1,
      21,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    34,    -1,    -1,    -1,    38,    39,    40,
      -1,    42,    43,    44,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    -1,    56,    57,    -1,    59,    60,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,
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
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,    -1,   153,   154,   155,    -1,    -1,
      -1,   159,    -1,   161,    -1,   163,   164,   165,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,
      -1,    16,    17,    -1,    19,    -1,    21,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,
      -1,    56,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    71,    -1,    73,    -1,
      -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    91,    92,    -1,    94,
      -1,    -1,    97,    98,    99,    -1,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,
     155,    -1,    -1,    -1,   159,    -1,   161,    -1,   163,   164,
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
      -1,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,
      -1,   153,   154,   155,   156,    -1,    -1,   159,    -1,   161,
      -1,   163,    -1,   165,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,    -1,
      19,    -1,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    34,    -1,    -1,    -1,    38,
      39,    40,    -1,    42,    43,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    -1,    56,    57,    -1,
      59,    60,    61,    62,    63,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    71,    -1,    73,    -1,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    91,    92,    -1,    94,    -1,    -1,    97,    98,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,    -1,   153,   154,   155,    -1,    -1,    -1,
     159,    -1,   161,    -1,   163,    -1,   165,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    13,    -1,    -1,
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
      63,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
     113,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      90,    91,    92,    -1,    94,    -1,    -1,    97,    98,    99,
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
      97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,     1,
      -1,     3,   159,    -1,   161,    -1,   163,    -1,   165,    11,
      -1,    -1,    -1,    -1,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    24,    -1,    21,    -1,    -1,    -1,    25,    26,
      32,    -1,    -1,    -1,    -1,    -1,    38,    34,    -1,    36,
      37,    43,    -1,    45,    41,    -1,    48,    -1,    50,    46,
      -1,    -1,    49,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    68,    -1,    70,    -1,    -1,    78,    -1,    75,    -1,
      -1,    -1,    79,    -1,    81,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,
      -1,    -1,   104,   100,    -1,   107,   103,    -1,   105,   106,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,   121,
      -1,    -1,   124,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,    -1,    -1,   138,    -1,    -1,    -1,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,   154,    -1,    -1,   157,   158,   159,   160,    -1,
      -1,   163,     3,     4,     5,     6,     7,     8,     9,    -1,
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
      -1,    -1,   113,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,
      -1,    -1,   153,   154,   155,    -1,    -1,    -1,   159,    -1,
     161,    -1,   163,    -1,   165,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,
      -1,    19,    -1,    21,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    34,    -1,    -1,    -1,
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
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,   113,    -1,
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
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,   100,   101,
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
     159,    -1,   161,   162,   163,    -1,   165,     3,     4,     5,
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
     159,    -1,    -1,    -1,   163,    -1,   165,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,
      16,    17,    -1,    19,    -1,    21,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,    -1,
      -1,    -1,    38,    39,    40,    -1,    42,    43,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    -1,
      56,    57,    -1,    59,    60,    21,    62,    63,    -1,    25,
      26,    -1,    -1,    69,    -1,    71,    -1,    73,    34,    -1,
      36,    37,    78,    79,    -1,    41,    -1,    -1,    -1,    -1,
      46,    87,    88,    89,    90,    91,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,
      -1,   107,    68,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   150,    -1,    -1,    -1,   154,   155,
      -1,    -1,    -1,   159,    -1,   161,    -1,   163,    -1,   165,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      13,    -1,    -1,    16,    17,    -1,    19,    -1,    21,    30,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    34,    -1,    -1,    -1,    38,    39,    40,    -1,    42,
      43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      53,    54,    -1,    56,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    80,
      73,    -1,    -1,    84,    -1,    78,    79,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   101,    -1,
      -1,   104,    -1,    -1,   107,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,   132,    -1,   134,   135,   136,    -1,   138,   139,   140,
     133,   142,   143,   144,   145,    -1,    -1,   148,   149,   150,
     151,   152,    -1,   154,    -1,    -1,   157,   158,   159,    -1,
      -1,    -1,   155,   156,    -1,    -1,    -1,    -1,   161,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,   137,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,
      -1,   161,    -1,   163,   164,   165,     3,     4,     5,     6,
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
     107,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,    -1,    -1,    -1,   131,    -1,   133,    -1,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,   147,    13,    -1,
      -1,    16,    17,    -1,    19,    -1,    21,    -1,   155,    24,
     160,    -1,    -1,   163,   161,   165,   163,    32,   165,    34,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,
      -1,    56,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    71,    -1,    73,    -1,
      -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    91,    92,    -1,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      13,    -1,    -1,    16,    17,    -1,    19,    -1,    21,    -1,
     155,    24,    -1,    -1,    -1,    -1,   161,    -1,   163,    32,
     165,    34,    -1,    -1,    -1,    38,    39,    40,    -1,    42,
      43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      53,    -1,    -1,    56,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    -1,
      73,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      -1,    94,     3,    -1,    97,    -1,    -1,    -1,   101,    -1,
      11,   104,    -1,    -1,   107,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
     133,    -1,    43,    -1,    45,    -1,    -1,    48,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    62,    63,    -1,    -1,    -1,    11,    -1,    69,    -1,
     163,    -1,   165,    18,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
     101,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
     121,    -1,    -1,   124,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,    80,    -1,   138,    -1,    84,
      -1,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,   154,    11,    -1,   157,   158,   159,   160,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
      -1,    11,   157,   158,   159,    -1,    -1,    -1,    18,    -1,
      -1,   166,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,   115,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,   131,   132,    -1,   134,   135,   136,
      80,   138,   139,   140,    84,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
     157,   158,   159,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,   131,   132,    -1,   134,   135,   136,    -1,   138,   139,
     140,    -1,   142,   143,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,    11,   115,   157,   158,   159,
      -1,    -1,    18,    -1,   164,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,    30,   134,   135,   136,    -1,   138,
     139,   140,    -1,   142,   143,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,    -1,    -1,   157,   158,
     159,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    93,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
     116,   117,   118,   119,   120,   121,    -1,    -1,   124,   125,
     126,   127,    -1,   129,   130,   131,   132,    -1,   134,   135,
     136,    -1,   138,   139,   140,    -1,   142,   143,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,    -1,
      80,   157,   158,   159,    84,   161,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    93,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,
     120,   121,    -1,    -1,   124,   125,   126,   127,    -1,   129,
     130,   131,   132,    -1,   134,   135,   136,    -1,   138,   139,
     140,    -1,   142,   143,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,    -1,    80,   157,   158,   159,
      84,   161,    -1,    11,    -1,    -1,    -1,    -1,    -1,    93,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,   117,   118,   119,   120,   121,    -1,    -1,
     124,   125,   126,   127,    -1,   129,   130,   131,   132,    -1,
     134,   135,   136,    -1,   138,   139,   140,    -1,   142,   143,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,    -1,    80,   157,   158,   159,    84,   161,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    93,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,   120,   121,    -1,    -1,   124,   125,   126,   127,
      -1,   129,   130,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,    -1,    80,   157,
     158,   159,    84,   161,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    93,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,   116,   117,   118,   119,   120,   121,
      -1,    -1,   124,   125,   126,   127,    -1,   129,   130,   131,
     132,    -1,   134,   135,   136,    -1,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,    -1,    80,   157,   158,   159,    84,   161,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    93,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
     116,   117,   118,   119,   120,   121,    -1,    -1,   124,   125,
     126,   127,    -1,   129,   130,   131,   132,    -1,   134,   135,
     136,    -1,   138,   139,   140,    -1,   142,   143,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,    -1,
      80,   157,   158,   159,    84,   161,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    93,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,
     120,   121,    -1,    -1,   124,   125,   126,   127,    -1,   129,
     130,   131,   132,    -1,   134,   135,   136,    -1,   138,   139,
     140,    -1,   142,   143,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,    -1,    80,   157,   158,   159,
      84,   161,    -1,    11,    -1,    -1,    -1,    -1,    -1,    93,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,   117,   118,   119,   120,   121,    -1,    -1,
     124,   125,   126,   127,    -1,   129,   130,   131,   132,    -1,
     134,   135,   136,    -1,   138,   139,   140,    -1,   142,   143,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,    -1,    80,   157,   158,   159,    84,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,   120,   121,    -1,    -1,   124,   125,   126,   127,
      -1,   129,   130,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,    11,    -1,   157,
     158,   159,    -1,   161,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,
      44,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,   111,    80,    -1,
      -1,   115,    84,    -1,    -1,    30,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,
     134,   135,   136,    -1,   138,   139,   140,    -1,   142,   143,
     144,   145,   146,   115,   148,   149,   150,   151,   152,   153,
     154,    -1,    -1,   157,   158,   159,    -1,   161,    -1,   131,
     132,    -1,   134,   135,   136,    80,   138,   139,   140,    84,
     142,   143,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,    11,    -1,   157,   158,   159,    -1,   161,
      18,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
     115,    -1,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
      11,    -1,   157,   158,   159,    -1,   161,    18,    -1,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    30,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,    80,
     138,   139,   140,    84,   142,   143,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,    -1,    -1,   157,
     158,   159,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,   115,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
     131,   132,    -1,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,   115,   148,   149,   150,
     151,   152,   153,   154,    -1,    -1,   157,   158,   159,    -1,
     161,    -1,   131,   132,    -1,   134,   135,   136,    -1,   138,
     139,   140,    -1,   142,   143,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,    11,    -1,   157,   158,
     159,    -1,   161,    18,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    16,    17,    -1,
      19,    -1,    -1,    -1,    -1,    24,    -1,    -1,    11,    -1,
      -1,    -1,    -1,    32,    -1,    18,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    45,    30,    -1,    48,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    62,    63,    80,    -1,    -1,    -1,    84,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    65,    -1,    -1,    11,    -1,    -1,    87,    88,
      -1,    90,    18,    92,    -1,    94,    -1,    80,    97,    -1,
     115,    84,   101,    -1,    30,   104,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    44,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,   115,   148,   149,   150,   151,   152,   153,   154,
      -1,    -1,   157,   158,   159,    -1,   161,    -1,   131,   132,
      -1,   134,   135,   136,    80,   138,   139,   140,    84,   142,
     143,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,    11,   156,   157,   158,   159,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,   115,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,   135,
     136,    -1,   138,   139,   140,    -1,   142,   143,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,    11,
      -1,   157,   158,   159,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    30,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    11,    44,    -1,    -1,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,    44,   134,   135,   136,    80,   138,
     139,   140,    84,   142,   143,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    -1,    -1,   115,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,    -1,   134,   135,   136,    -1,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,   115,   148,   149,   150,   151,
     152,   153,   154,    -1,    -1,   157,   158,   159,    -1,    -1,
      -1,   131,   132,    -1,   134,   135,   136,    -1,   138,   139,
     140,    -1,   142,   143,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,    11,    -1,   157,   158,   159,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    80,    -1,    -1,   115,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,   135,
     136,    -1,   138,   139,   140,    -1,   142,   143,   144,   145,
     146,   115,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,   157,   158,   159,    -1,    80,    -1,   131,   132,    84,
     134,   135,   136,    -1,   138,   139,   140,    -1,   142,   143,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,    11,   156,   157,   158,   159,    -1,    -1,    18,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
      11,   156,   157,   158,   159,    -1,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      80,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      -1,   131,   132,    84,   134,   135,   136,    -1,   138,   139,
     140,    -1,   142,   143,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
      -1,    80,    -1,   114,   115,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
     131,   132,    -1,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,   115,   148,   149,   150,
     151,   152,   153,   154,    -1,    -1,   157,   158,   159,    -1,
      -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,   138,
     139,   140,    -1,   142,   143,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,    11,    -1,   157,   158,
     159,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    21,    -1,    -1,
      -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    37,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    11,    46,    -1,    -1,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    68,    80,    70,    -1,    72,    84,
      74,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,    -1,   103,
     115,    30,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    84,    -1,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
      -1,   111,   157,   158,   159,   115,    -1,    -1,    -1,    -1,
      -1,    80,   156,    -1,    -1,    84,    -1,    -1,   162,    -1,
      -1,   131,   132,    -1,   134,   135,   136,    -1,   138,   139,
     140,    -1,   142,   143,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,    -1,   115,   157,   158,   159,
      -1,    -1,    -1,    11,    12,    -1,    -1,    -1,    -1,    -1,
      18,    -1,   131,   132,    -1,   134,   135,   136,    -1,   138,
     139,   140,    30,   142,   143,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   115,    -1,   157,
     158,   159,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   114,   115,   157,
     158,   159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    11,    -1,
     157,   158,   159,    -1,     1,    18,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    31,    16,
      17,    -1,    19,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      11,    -1,    -1,    -1,    -1,    32,    -1,    18,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    45,    30,
      31,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    62,    63,    80,    -1,    -1,
      -1,    84,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    -1,    80,
      97,    -1,   115,    84,   101,    -1,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
      -1,   134,   135,   136,    -1,   138,   139,   140,    -1,   142,
     143,   144,   145,   146,   115,   148,   149,   150,   151,   152,
     153,   154,    -1,    -1,   157,   158,   159,    -1,    -1,    -1,
     131,   132,    -1,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,    11,    -1,   157,   158,   159,    -1,
      -1,    18,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    16,    17,    -1,    19,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    11,    -1,    -1,    -1,
      -1,    32,    -1,    18,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    45,    30,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    62,    63,    80,    -1,    -1,    -1,    84,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    78,    -1,    -1,
      -1,    -1,    -1,    11,    12,    -1,    87,    88,    -1,    90,
      18,    92,    -1,    94,    -1,    80,    97,    -1,   115,    84,
     101,    -1,    30,   104,    -1,    -1,   107,    -1,    93,    -1,
      -1,    -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
     115,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
     157,   158,   159,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    80,   138,   139,   140,    84,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
      11,    -1,   157,   158,   159,    -1,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,    -1,    -1,   157,
     158,   159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    84,    -1,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    93,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,   132,    -1,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,    -1,    80,   157,   158,   159,    84,
      -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    93,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
      -1,    80,   157,   158,   159,    84,    -1,    -1,    11,    -1,
      -1,    -1,    -1,    -1,    93,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,   138,
     139,   140,    -1,   142,   143,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,    -1,    80,   157,   158,
     159,    84,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      93,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
      -1,   134,   135,   136,    -1,   138,   139,   140,    -1,   142,
     143,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,    -1,    80,   157,   158,   159,    84,    -1,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    93,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    -1,    80,
     157,   158,   159,    84,    -1,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    93,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
     131,   132,    -1,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,    -1,    80,   157,   158,   159,    84,
      -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
      -1,    80,   157,   158,   159,    84,    -1,    -1,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,   138,
     139,   140,    -1,   142,   143,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,    -1,    80,   157,   158,
     159,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
      -1,   134,   135,   136,    -1,   138,   139,   140,    30,   142,
     143,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,    -1,    21,   157,   158,   159,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    37,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,
      -1,    30,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      68,    -1,    70,    -1,    72,    -1,    74,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,   131,
     132,    80,   134,   135,   136,    84,   138,   139,   140,    -1,
     142,   143,   144,   145,    -1,    -1,   148,   149,   150,   151,
     152,   153,   154,    -1,    -1,   157,   158,   159,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,    -1,   131,   132,   162,   134,   135,   136,    -1,   138,
     139,   140,    -1,   142,   143,   144,   145,    -1,     3,   148,
     149,   150,   151,   152,    -1,   154,    -1,    -1,   157,   158,
     159,    16,    17,    -1,    19,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    -1,     3,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    16,    17,
      -1,    19,    -1,    78,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    87,    88,    32,    90,    -1,    92,    -1,    94,
      38,    -1,    97,    -1,    -1,    43,   101,    45,    -1,   104,
      48,    -1,   107,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    62,    63,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    -1,    -1,    97,
      -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107
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
     203,   204,   206,   207,   208,   212,   213,   216,   217,   220,
     221,   224,   227,   228,   253,   256,   257,   277,   278,   279,
     280,   281,   282,   283,   291,   292,   293,   294,   297,   298,
     299,   300,   301,   302,   303,   305,   306,   307,   308,   309,
     156,   176,   294,   111,   284,   285,     3,   190,    13,    21,
      34,    39,    40,    42,    79,    92,   161,   165,   224,   277,
     282,   292,   293,   294,   297,   299,   300,   284,   294,   100,
     256,    81,   190,   176,   271,   294,   176,   163,     7,    79,
     294,   295,     7,    10,    79,   100,   295,    70,   113,   205,
     294,   205,   294,    25,   257,   294,   294,   163,     3,    16,
      17,    19,    24,    32,    38,    43,    45,    48,    56,    62,
      63,    69,    78,    87,    88,    90,    92,    94,    97,   101,
     104,   107,   192,   194,   225,   163,   192,   258,   259,    31,
     181,   197,   294,   294,   294,   294,   294,   294,     7,   163,
      21,    34,    37,    79,   200,   200,   155,   247,   270,   294,
      98,    99,   156,   294,   296,   294,   197,   294,   294,   294,
      91,   163,   176,   294,   294,   181,   189,   294,   297,   181,
     189,   294,   192,   254,   294,   294,   294,   294,   294,   294,
     294,   294,     1,   162,   174,   182,   270,   102,   141,   247,
     272,   273,   296,   205,   270,   294,   304,    72,   176,   161,
      76,   180,   105,   192,   192,    49,    75,   242,   258,   155,
     156,   147,    11,    18,    30,    80,    84,   115,   131,   132,
     134,   135,   136,   138,   139,   140,   142,   143,   144,   145,
     146,   148,   149,   150,   151,   152,   153,   154,   157,   158,
     159,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   160,   234,   163,   165,
      80,    84,   163,   176,   156,   294,   294,   294,   270,   161,
     181,    44,   284,   254,   258,   156,   137,   156,   110,   193,
     247,   274,   275,   276,   296,    79,   192,   228,   256,    79,
     192,   254,   228,   256,   192,   163,   197,    31,    44,   197,
     111,   197,   287,    31,    44,   197,   287,    35,    65,   156,
      93,   181,   234,   274,   176,   163,   236,   102,   163,   192,
     260,   261,     1,   136,   265,    44,   137,   176,   197,   197,
     274,   192,   137,   156,   294,   294,   156,   161,   197,   163,
     274,   156,   209,   156,   209,   156,   116,   255,   156,   197,
     156,   162,   162,   174,   137,   162,   294,   137,   164,   137,
     164,   166,   287,    44,   137,   166,   287,   114,   137,   166,
       7,     1,   162,   182,    58,   270,   294,   136,   218,   161,
     229,   156,   294,   294,   294,   294,   205,   294,   205,   294,
     294,   294,   294,   294,   294,   294,    19,    32,    56,   100,
     193,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   296,   296,   296,
     296,   296,   274,   274,   205,   294,   205,   294,    26,    44,
      81,   106,   286,   289,   290,   294,   309,    31,    44,    31,
      44,    93,    44,   166,   205,   294,   197,   156,   156,   294,
     294,   116,   164,   137,   192,   218,   258,   192,   218,   156,
     258,    44,   270,    42,   294,   205,   294,   163,   197,    42,
     294,   205,   294,   197,   159,   183,   185,   294,   183,   184,
     176,   294,    33,   294,   164,    26,    44,    47,    67,    70,
      81,   100,   175,   237,   238,   239,   240,   226,   261,   137,
     164,    32,    45,    88,    92,   165,   196,   266,   278,   116,
     262,   294,   259,   164,   247,   294,     1,   214,   274,   164,
      20,   210,   266,   278,   137,   162,   164,   164,   272,   164,
     272,   176,   166,   205,   294,   166,   176,   294,   166,   294,
     166,   294,   162,   162,   192,   156,    35,    65,   270,   161,
       1,   192,   222,   223,    26,    70,    81,   100,   231,   241,
     163,   156,   156,   156,   156,   156,   164,   166,    44,    81,
     137,   164,   277,    80,    80,    42,   205,   294,    42,   205,
     294,   205,   294,   284,   284,   156,   247,   296,   276,   218,
     161,   156,   218,   161,   156,   294,   164,   294,    31,   197,
      31,   197,   288,   289,   294,    31,   197,   287,    31,   197,
     287,   137,   156,    12,   156,    33,    33,   176,    93,   181,
      44,    81,   239,   137,   164,   163,   192,    26,    70,    81,
     100,   243,   164,   261,   265,     1,   270,    61,   296,   162,
      66,   108,   162,   215,   164,   163,   181,   192,   211,   254,
     176,   166,   287,   166,   287,   176,   114,   185,   184,     1,
     219,   162,   116,   137,   162,    81,   230,     1,     3,    11,
      16,    18,    19,    24,    38,    43,    48,    50,    56,    62,
      63,    78,    90,    94,    97,   101,   107,   131,   132,   133,
     134,   135,   138,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   154,   157,   158,   159,   160,   163,
     191,   192,   194,   232,   233,   234,   277,   164,   289,   265,
     277,   277,   294,    31,    31,   294,    31,    31,   166,   166,
     197,   197,   161,   219,   161,   219,   197,    93,    42,   294,
      42,   294,   137,   164,    93,    42,   294,   197,    42,   294,
     197,   294,   294,   176,   294,   176,    33,   192,   238,   261,
     136,   269,    81,   265,   262,   166,    44,   166,    31,   176,
     270,   211,   136,   181,    42,   176,   294,   166,    42,   176,
     294,   166,   294,   156,   156,   162,    74,    77,   162,   175,
     199,   294,   223,   243,   163,   235,   192,   294,   131,   139,
     235,   235,   262,    93,    42,    42,    93,    42,    42,    42,
      42,   219,   162,   219,   162,   294,   294,   294,   289,   294,
     294,   294,    12,    33,   176,   269,   164,   165,   196,   247,
     268,   278,   141,   248,   262,    54,   109,   249,   294,   266,
     278,   176,   197,   164,   294,   294,   176,   294,   176,   199,
       1,   136,   264,   237,   164,     3,    94,   233,   234,   294,
     294,   294,   294,   294,   294,   162,   162,    93,    93,    93,
      93,   294,   176,   262,   269,   166,   270,   247,   294,     3,
      83,    94,   250,   251,   252,   294,   181,   198,   246,   166,
     181,    93,    93,   165,   196,   263,   278,    96,   244,   164,
     235,   235,    93,    93,    93,    93,    93,    93,   294,   294,
     294,   294,   248,   262,   247,   267,   268,   278,    44,   166,
     252,   109,   137,   116,   142,   144,   145,   148,   149,    54,
     278,   294,   294,     1,   166,   270,   249,   294,   267,   268,
     294,   251,   252,   252,   252,   252,   252,   252,   250,   166,
     263,   278,   166,   156,   245,   246,   166,   263,   278,   267
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
#line 817 "chapel.ypp"
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
#line 827 "chapel.ypp"
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
#line 837 "chapel.ypp"
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
#line 848 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 852 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 857 "chapel.ypp"
    {
      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), context->latestComment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
      context->latestComment = NULL;
    }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 863 "chapel.ypp"
    {
      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), context->latestComment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
      context->latestComment = NULL;
    }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 872 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 878 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 879 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 880 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 881 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 882 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 883 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 884 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 885 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 886 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 887 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 888 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 889 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 890 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 891 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 892 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 893 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 894 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 895 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 897 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 903 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 909 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 915 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 921 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 928 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 935 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 939 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 945 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 949 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 950 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 951 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 952 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 953 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 955 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 957 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 959 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 964 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 967 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 968 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 969 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 970 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 971 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 972 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 976 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 977 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 981 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 982 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 983 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 987 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 988 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 992 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 996 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 998 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1002 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1003 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1008 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1010 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1012 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1019 "chapel.ypp"
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
#line 1029 "chapel.ypp"
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
#line 1042 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1047 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1052 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1060 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1061 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1066 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1068 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1070 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1075 "chapel.ypp"
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
#line 1091 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1098 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1107 "chapel.ypp"
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
#line 1115 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1119 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1125 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1126 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1131 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1136 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1141 "chapel.ypp"
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
#line 1163 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1169 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1176 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1187 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1193 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1200 "chapel.ypp"
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
#line 1226 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1230 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1235 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1239 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1244 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1251 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1255 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1256 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1257 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1258 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1259 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1260 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1261 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1262 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1263 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1264 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1265 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1266 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1267 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1268 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1269 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1270 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1271 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1272 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1273 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1276 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1277 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1286 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1288 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1312 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1317 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1319 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1321 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1323 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1325 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1329 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1330 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1335 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1336 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1337 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1338 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1340 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1341 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1342 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1346 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1347 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1348 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1349 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1350 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1351 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1356 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1363 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1364 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1365 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1370 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1379 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1384 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1388 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1389 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1390 "chapel.ypp"
    { if (DefExpr* def = toDefExpr((yyvsp[(2) - (2)].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
                         }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1398 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1400 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1402 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1404 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1406 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1411 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1413 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1417 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1418 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1419 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1420 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1421 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1422 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1423 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1428 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1430 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1435 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1437 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1439 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 347:
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

  case 348:
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

  case 349:
/* Line 1792 of yacc.c  */
#line 1473 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1481 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1490 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1496 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1505 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1541 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1555 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1559 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1561 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1565 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1572 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1573 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1574 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1575 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1576 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1582 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1583 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1584 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1606 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1610 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 391:
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

  case 392:
/* Line 1792 of yacc.c  */
#line 1622 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1629 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1630 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1635 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1637 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1643 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1645 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1647 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1651 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1652 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1653 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1654 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1661 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1662 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1663 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1664 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1668 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1669 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1673 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1674 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1679 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1680 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1684 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1690 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1694 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1695 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1696 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1697 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1701 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1729 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1731 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1733 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1735 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1737 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1739 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1741 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1744 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1746 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1748 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1750 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1752 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1754 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1756 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1758 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1761 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1763 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1765 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1770 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1772 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1776 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1778 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1780 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1782 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1784 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1786 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1788 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1790 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1792 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1794 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1801 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1807 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1813 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1819 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1828 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1837 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1853 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1857 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1862 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1866 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1867 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1871 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1875 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1876 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1881 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1885 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1889 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1895 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1896 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1897 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1898 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1899 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1900 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1906 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1911 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1916 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1932 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1934 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1936 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1938 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1940 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1942 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1946 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1947 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1951 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1952 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1953 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 1971 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 1972 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 1973 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr));}
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 1974 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 1978 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 1979 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 1980 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 1984 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 1985 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 1986 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 1987 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 1988 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 1996 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 1997 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 1998 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 1999 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 2003 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 2004 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 2009 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 2010 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 2011 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 2012 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 2013 "chapel.ypp"
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 2014 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 2015 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 2016 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 2017 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 2018 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 2019 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 2021 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 2025 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 2032 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 2033 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 2037 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 2038 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 2039 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2040 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2041 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2042 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 2043 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2044 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 2045 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 2046 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2047 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2048 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2049 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2050 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2051 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2052 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2053 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2054 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2055 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2056 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2057 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2058 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2059 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2063 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2064 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2065 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 584:
/* Line 1792 of yacc.c  */
#line 2066 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 585:
/* Line 1792 of yacc.c  */
#line 2067 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 586:
/* Line 1792 of yacc.c  */
#line 2068 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 587:
/* Line 1792 of yacc.c  */
#line 2069 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 588:
/* Line 1792 of yacc.c  */
#line 2073 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 589:
/* Line 1792 of yacc.c  */
#line 2074 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 590:
/* Line 1792 of yacc.c  */
#line 2075 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 591:
/* Line 1792 of yacc.c  */
#line 2076 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 592:
/* Line 1792 of yacc.c  */
#line 2080 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 593:
/* Line 1792 of yacc.c  */
#line 2081 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 594:
/* Line 1792 of yacc.c  */
#line 2082 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 595:
/* Line 1792 of yacc.c  */
#line 2083 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 596:
/* Line 1792 of yacc.c  */
#line 2088 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 597:
/* Line 1792 of yacc.c  */
#line 2089 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 598:
/* Line 1792 of yacc.c  */
#line 2090 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 599:
/* Line 1792 of yacc.c  */
#line 2091 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 600:
/* Line 1792 of yacc.c  */
#line 2092 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 601:
/* Line 1792 of yacc.c  */
#line 2093 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 602:
/* Line 1792 of yacc.c  */
#line 2094 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 9972 "bison-chapel.cpp"
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


