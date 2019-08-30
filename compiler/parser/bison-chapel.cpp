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
     TUPLUS = 424
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
#line 484 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 491 "bison-chapel.cpp"
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
#define YYLAST   18069

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  170
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  140
/* YYNRULES -- Number of rules.  */
#define YYNRULES  604
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1082

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   424

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
     165,   166,   167,   168,   169
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
    1801,  1805,  1810,  1814,  1819,  1823,  1828,  1832,  1838,  1842,
    1846,  1850,  1854,  1858,  1862,  1866,  1870,  1874,  1878,  1882,
    1886,  1890,  1894,  1898,  1902,  1906,  1910,  1914,  1918,  1922,
    1926,  1930,  1934,  1938,  1941,  1944,  1947,  1950,  1953,  1956,
    1959,  1963,  1967,  1971,  1975,  1979,  1983,  1987,  1991,  1993,
    1995,  1997,  1999,  2001,  2003
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     171,     0,    -1,   172,    -1,    -1,   172,   173,    -1,   175,
      -1,   174,   175,    -1,    72,     7,    -1,   174,    72,     7,
      -1,   177,    -1,   180,    -1,   186,    -1,   187,    -1,   198,
      -1,   188,    -1,   202,    -1,   205,    -1,   203,    -1,   212,
      -1,   206,    -1,   207,    -1,   211,    -1,   197,    -1,   283,
     156,    -1,    13,   175,    -1,    14,   284,   175,    -1,    15,
     189,   156,    -1,    22,   284,   180,    -1,    27,   189,   156,
      -1,    29,   271,   156,    -1,    51,   191,   175,    -1,    55,
     294,   196,    -1,    55,   196,    -1,    64,   294,   196,    -1,
      86,   294,   196,    -1,    86,   196,    -1,    92,   175,    -1,
     112,   294,   156,    -1,     1,   156,    -1,   178,   179,    58,
     191,    -1,   176,   161,   162,    -1,   176,   161,   181,   162,
      -1,   176,   161,     1,   162,    -1,    -1,    77,    -1,    74,
      -1,    -1,    76,    -1,   161,   162,    -1,   161,   181,   162,
      -1,   161,     1,   162,    -1,   173,    -1,   181,   173,    -1,
     294,    -1,   294,    12,   294,    -1,   182,   137,   294,    -1,
     182,   137,   294,    12,   294,    -1,    -1,   182,    -1,   159,
      -1,   182,    -1,    -1,    77,    -1,    74,    -1,   185,   105,
     270,   156,    -1,   185,   105,   294,    35,   184,   156,    -1,
     185,   105,   294,    65,   183,   156,    -1,    82,   270,   156,
      -1,   297,   233,   296,   156,    -1,   297,   160,   296,   156,
      -1,   297,   128,   296,   156,    -1,   297,   122,   296,   156,
      -1,   297,   123,   296,   156,    -1,    -1,     3,    -1,     3,
      -1,    62,    -1,    94,    -1,    38,    -1,    97,    -1,   193,
      -1,     3,    -1,    62,    -1,    94,    -1,    38,    -1,    97,
      -1,   193,    -1,     3,    -1,    94,    -1,    16,    -1,    48,
      -1,   101,    -1,    78,    -1,    43,    -1,    24,    -1,    19,
      -1,    90,    -1,    92,    -1,    88,    -1,    69,    -1,    87,
      -1,    17,    -1,   104,    -1,    32,    -1,    45,    -1,    56,
      -1,    63,    -1,   107,    -1,    16,    -1,    48,    -1,   101,
      -1,    78,    -1,    43,    -1,    24,    -1,    19,    -1,    90,
      -1,    56,    -1,    63,    -1,   107,    -1,    92,    -1,    88,
      -1,    32,    -1,    45,    -1,    31,   175,    -1,   180,    -1,
      83,   156,    -1,    83,   296,   156,    -1,   156,    -1,   199,
      -1,    77,   199,    -1,    74,   199,    -1,   227,    -1,   257,
      -1,   219,    -1,   253,    -1,   215,    -1,   200,    -1,   201,
      -1,    41,   294,   156,    -1,    41,   294,    35,   184,   156,
      -1,    41,   294,    65,   183,   156,    -1,    41,   257,    -1,
      37,    79,   191,   217,   161,   218,   162,    -1,    37,     7,
      79,   191,   217,   161,   218,   162,    -1,    36,    79,   191,
     217,   161,   218,   162,    -1,    36,     7,    79,   191,   217,
     161,   218,   162,    -1,    37,   295,   227,    -1,    36,   295,
     227,    -1,    37,   295,   256,   258,   156,    -1,    36,   295,
     256,   258,   156,    -1,    37,    10,    -1,    31,   175,   110,
     294,   156,    -1,   110,   294,   196,    -1,    23,   294,    44,
     294,   284,   196,    -1,    23,   294,    44,   204,   284,   196,
      -1,    23,   294,   284,   196,    -1,    39,   294,    44,   294,
     196,    -1,    39,   294,    44,   204,   196,    -1,    39,   294,
     196,    -1,    39,   204,   196,    -1,    39,    70,   191,    44,
     294,   196,    -1,    40,   294,    44,   294,   196,    -1,    40,
     294,    44,   294,   287,   196,    -1,    40,   294,    44,   204,
     196,    -1,    40,   294,    44,   204,   287,   196,    -1,    40,
     294,   196,    -1,    40,   294,   287,   196,    -1,    40,   204,
     196,    -1,    40,   204,   287,   196,    -1,   165,   270,    44,
     294,   166,   175,    -1,   165,   270,    44,   294,   287,   166,
     175,    -1,   165,   270,    44,   204,   166,   175,    -1,   165,
     270,    44,   204,   287,   166,   175,    -1,   165,   270,   166,
     175,    -1,   165,   270,   287,   166,   175,    -1,   165,   204,
     166,   175,    -1,   165,   204,   287,   166,   175,    -1,   113,
     163,   270,   164,    -1,    42,   294,    93,   175,    -1,    42,
     294,   180,    -1,    42,   294,    93,   175,    33,   175,    -1,
      42,   294,   180,    33,   175,    -1,    42,   294,   233,   294,
      93,   175,    -1,    42,   294,   233,   294,   180,    -1,    42,
     294,   233,   294,    93,   175,    33,   175,    -1,    42,   294,
     233,   294,   180,    33,   175,    -1,    28,   175,    -1,    98,
     294,   156,    -1,    99,   294,   156,    -1,    98,   188,    -1,
      99,   188,    -1,    98,   180,   208,    -1,    99,   180,   208,
      -1,    -1,   208,   209,    -1,    20,   180,    -1,    20,   210,
     180,    -1,    20,   163,   210,   164,   180,    -1,   191,    -1,
     191,   136,   294,    -1,    95,   294,   156,    -1,    85,   294,
     161,   213,   162,    -1,    85,   294,   161,     1,   162,    -1,
      -1,   213,   214,    -1,   108,   270,   196,    -1,    66,   175,
      -1,    66,    31,   175,    -1,   216,   191,   217,   161,   218,
     162,    -1,   216,   191,   217,   161,     1,   162,    -1,    21,
      -1,    79,    -1,   103,    -1,    -1,   136,   270,    -1,    -1,
     218,   198,    -1,   218,   174,   198,    -1,   220,   191,   161,
     221,   162,    -1,   220,   191,   161,     1,   162,    -1,    34,
      -1,   222,    -1,   221,   137,    -1,   221,   137,   222,    -1,
     191,    -1,   191,   116,   294,    -1,    -1,    -1,    52,   224,
     235,   225,   242,   265,   249,   245,    -1,    -1,    46,    -1,
      68,    -1,    -1,    -1,   226,   241,   228,   230,   229,   242,
     264,   243,   249,   244,    -1,   240,   232,   234,    -1,   240,
     233,   234,    -1,   240,   231,   139,   232,   234,    -1,   240,
     231,   139,   233,   234,    -1,   240,     1,   234,    -1,   277,
      -1,   163,   294,   164,    -1,   190,    -1,   133,   191,    -1,
     132,    -1,   134,    -1,   135,    -1,   133,    -1,   142,    -1,
     152,    -1,   149,    -1,   145,    -1,   148,    -1,   144,    -1,
     154,    -1,   150,    -1,   159,    -1,   138,    -1,   157,    -1,
     158,    -1,   151,    -1,   143,    -1,   131,    -1,    18,    -1,
     146,    -1,    11,    -1,   160,    -1,   147,    -1,    50,    -1,
     191,   131,    -1,   116,    -1,   127,    -1,   124,    -1,   126,
      -1,   120,    -1,   125,    -1,   121,    -1,   117,    -1,   118,
      -1,   119,    -1,   130,    -1,   129,    -1,    -1,   163,   236,
     164,    -1,   163,   236,   164,    -1,    -1,   237,    -1,   236,
     137,   237,    -1,   238,   191,   269,   262,    -1,   174,   238,
     191,   269,   262,    -1,   238,   191,   269,   248,    -1,   238,
     163,   261,   164,   269,   262,    -1,   238,   163,   261,   164,
     269,   248,    -1,    -1,   239,    -1,    44,    -1,    47,    -1,
      67,    -1,    26,    -1,    26,    44,    -1,    26,    81,    -1,
      70,    -1,    81,    -1,   100,    -1,    -1,    70,    -1,    81,
      -1,    26,    81,    -1,    26,    -1,   100,    -1,    75,    -1,
      49,    -1,    -1,    26,    -1,    26,    81,    -1,    81,    -1,
      70,    -1,   100,    -1,    -1,    96,    -1,   156,    -1,   245,
      -1,   180,    -1,   197,    -1,   155,   191,    -1,   155,    -1,
      -1,   246,    -1,   141,   294,    -1,   141,   247,    -1,    -1,
     109,   294,    -1,    54,   250,    -1,   109,   294,    54,   250,
      -1,    54,   250,   109,   294,    -1,   251,    -1,   250,   137,
     251,    -1,   252,   116,   252,    -1,   252,   148,   252,    -1,
     252,   149,   252,    -1,   252,   142,   252,    -1,   252,   144,
     252,    -1,   252,   145,   252,    -1,    83,   252,    -1,     3,
      -1,    94,    -1,   100,   254,   156,    -1,    25,   100,   254,
     156,    -1,    37,   100,   254,   156,    -1,   191,   255,    -1,
     191,   255,   137,   254,    -1,    -1,   116,   278,    -1,   116,
     266,    -1,    70,    -1,    26,    -1,    81,    -1,    26,    81,
      -1,   106,    -1,    25,   256,   258,   156,    -1,   256,   258,
     156,    -1,   259,    -1,   258,   137,   259,    -1,   191,   265,
     262,    -1,   163,   261,   164,   265,   262,    -1,   102,    -1,
     191,    -1,   163,   261,   164,    -1,   260,    -1,   260,   137,
      -1,   260,   137,   261,    -1,    -1,   116,    61,    -1,   116,
     296,    -1,   165,   166,   278,    -1,   165,   166,    -1,   165,
     270,   166,   278,    -1,   165,   270,   166,    -1,   165,   166,
     263,    -1,   165,   270,   166,   263,    -1,   165,     1,   166,
      -1,    -1,   136,   278,    -1,   136,   263,    -1,   136,   195,
      -1,     1,    -1,    -1,   136,   278,    -1,   136,   266,    -1,
     136,   195,    -1,     1,    -1,   165,   270,   166,   278,    -1,
     165,   270,   166,   266,    -1,   165,   270,    44,   294,   166,
     278,    -1,   165,     1,   166,    -1,    -1,   278,    -1,   246,
      -1,   165,   166,   267,    -1,   165,   270,   166,   267,    -1,
     165,   166,   268,    -1,   165,   270,   166,   268,    -1,   165,
     270,    44,   294,   166,   267,    -1,    -1,   136,   278,    -1,
     136,   246,    -1,   136,   195,    -1,   136,   268,    -1,   294,
      -1,   246,    -1,   270,   137,   294,    -1,   270,   137,   246,
      -1,   294,    -1,   271,   137,   294,    -1,   102,    -1,   296,
      -1,   246,    -1,   272,   137,   272,    -1,   273,   137,   272,
      -1,    -1,   275,    -1,   276,    -1,   275,   137,   276,    -1,
     192,   116,   246,    -1,   192,   116,   296,    -1,   246,    -1,
     296,    -1,   192,    -1,   194,    -1,   279,    -1,   279,   155,
      -1,   282,    -1,   297,    -1,   281,    -1,   306,    -1,   305,
      -1,    88,   294,    -1,    45,   163,   274,   164,    -1,    32,
     163,   274,   164,    -1,    91,   163,   274,   164,    -1,    89,
      91,   163,   274,   164,    -1,    13,   294,    -1,    92,   294,
      -1,    69,    -1,    69,   294,    -1,   104,    -1,   104,   294,
      -1,    87,    -1,    87,   294,    -1,    17,    -1,    17,   294,
      -1,    21,    -1,    79,    -1,    34,    -1,    39,   294,    44,
     294,    31,   294,    -1,    39,   294,    44,   204,    31,   294,
      -1,    39,   294,    31,   294,    -1,    39,   294,    44,   294,
      31,    42,   294,    93,   294,    -1,    39,   294,    44,   204,
      31,    42,   294,    93,   294,    -1,    39,   294,    31,    42,
     294,    93,   294,    -1,    40,   294,    44,   294,    31,   294,
      -1,    40,   294,    44,   204,    31,   294,    -1,    40,   294,
      31,   294,    -1,    40,   294,    44,   294,    31,    42,   294,
      93,   294,    -1,    40,   294,    44,   204,    31,    42,   294,
      93,   294,    -1,    40,   294,    31,    42,   294,    93,   294,
      -1,   165,   270,   166,   294,    -1,   165,   270,    44,   294,
     166,   294,    -1,   165,   270,    44,   204,   166,   294,    -1,
     165,   270,    44,   294,   166,    42,   294,    93,   294,    -1,
     165,   270,    44,   204,   166,    42,   294,    93,   294,    -1,
      42,   294,    93,   294,    33,   294,    -1,    60,    -1,   282,
      -1,   277,    -1,   300,    -1,   299,    -1,   223,    -1,   292,
      -1,   293,    -1,   291,   147,   294,    -1,    -1,   285,    -1,
     111,   163,   286,   164,    -1,   289,    -1,   286,   137,   289,
      -1,   111,   163,   288,   164,    -1,   289,    -1,   288,   137,
     289,    -1,   290,   277,   265,   262,    -1,   309,    80,   277,
      -1,   294,    80,   277,    -1,    26,    -1,    44,    -1,    26,
      44,    -1,    81,    -1,    26,    81,    -1,   106,    -1,   297,
      -1,   291,   147,   294,    -1,    59,   294,    -1,    53,   258,
      44,   294,    -1,   303,    -1,   278,    -1,   280,    -1,   307,
      -1,   308,    -1,   223,    -1,   292,    -1,   293,    -1,   163,
     141,   294,   164,    -1,   294,   136,   294,    -1,   294,   140,
     294,    -1,   294,   140,    -1,   140,   294,    -1,   140,    -1,
      -1,   294,    -1,    98,   294,    -1,    99,   294,    -1,   294,
      -1,   277,    -1,   299,    -1,   300,    -1,   301,    -1,   297,
      -1,   294,   131,    -1,   223,    -1,   298,   163,   274,   164,
      -1,   298,   165,   274,   166,    -1,    73,   163,   274,   164,
      -1,   294,   139,   192,    -1,   294,   139,   100,    -1,   294,
     139,    32,    -1,   294,   139,    56,    -1,   294,   139,    19,
     163,   164,    -1,   163,   272,   164,    -1,   163,   272,   137,
     164,    -1,   163,   273,   164,    -1,   163,   273,   137,   164,
      -1,    38,    -1,    97,    -1,   302,    -1,     4,    -1,     5,
      -1,     6,    -1,     7,    -1,     8,    -1,     9,    -1,    62,
      -1,   161,   270,   162,    -1,   161,   270,   137,   162,    -1,
     165,   270,   166,    -1,   165,   270,   137,   166,    -1,   165,
     304,   166,    -1,   165,   304,   137,   166,    -1,   294,   114,
     294,    -1,   304,   137,   294,   114,   294,    -1,   294,   154,
     294,    -1,   294,   150,   294,    -1,   294,   159,   294,    -1,
     294,   138,   294,    -1,   294,   157,   294,    -1,   294,   158,
     294,    -1,   294,   151,   294,    -1,   294,   142,   294,    -1,
     294,   142,   155,    -1,   294,   152,   294,    -1,   294,   152,
     155,    -1,   294,   149,   294,    -1,   294,   145,   294,    -1,
     294,   148,   294,    -1,   294,   144,   294,    -1,   294,   132,
     294,    -1,   294,   134,   294,    -1,   294,   135,   294,    -1,
     294,   115,   294,    -1,   294,   153,   294,    -1,   294,   143,
     294,    -1,   294,    18,   294,    -1,   294,    11,   294,    -1,
     294,   146,   294,    -1,   294,    30,   294,    -1,   154,   294,
      -1,   150,   294,    -1,    57,   294,    -1,    71,   294,    -1,
     131,   294,    -1,   294,   131,    -1,   133,   294,    -1,   294,
      80,   294,    -1,   294,    80,   204,    -1,   309,    80,   294,
      -1,   309,    80,   204,    -1,   294,    84,   294,    -1,   294,
      84,   204,    -1,   309,    84,   294,    -1,   309,    84,   204,
      -1,   154,    -1,   159,    -1,   115,    -1,   153,    -1,   132,
      -1,   134,    -1,   135,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   491,   491,   496,   497,   503,   504,   509,   510,   515,
     516,   517,   518,   519,   520,   521,   522,   523,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,   539,   540,   541,   542,   543,   544,   548,
     561,   566,   571,   579,   580,   581,   585,   586,   599,   600,
     601,   606,   607,   612,   617,   622,   626,   633,   638,   642,
     647,   651,   652,   653,   657,   658,   659,   663,   667,   669,
     671,   673,   675,   682,   683,   687,   688,   689,   690,   691,
     692,   695,   696,   697,   698,   699,   700,   712,   713,   724,
     725,   726,   727,   728,   729,   730,   731,   732,   733,   734,
     735,   736,   737,   738,   739,   740,   741,   742,   746,   747,
     748,   749,   750,   751,   752,   753,   754,   755,   756,   763,
     764,   765,   766,   776,   777,   781,   782,   786,   787,   788,
     789,   793,   794,   795,   796,   797,   798,   799,   803,   804,
     805,   806,   810,   820,   830,   840,   851,   855,   860,   866,
     875,   882,   883,   884,   885,   886,   887,   888,   889,   890,
     891,   892,   893,   894,   895,   896,   897,   898,   899,   900,
     906,   912,   918,   924,   931,   938,   942,   949,   953,   954,
     955,   956,   957,   959,   961,   963,   968,   971,   972,   973,
     974,   975,   976,   980,   981,   985,   986,   987,   991,   992,
     996,   999,  1001,  1006,  1007,  1011,  1013,  1015,  1022,  1032,
    1046,  1051,  1056,  1064,  1065,  1070,  1071,  1073,  1078,  1094,
    1101,  1110,  1118,  1122,  1129,  1130,  1135,  1140,  1134,  1167,
    1173,  1180,  1191,  1197,  1190,  1229,  1233,  1238,  1242,  1247,
    1254,  1255,  1259,  1260,  1261,  1262,  1263,  1264,  1265,  1266,
    1267,  1268,  1269,  1270,  1271,  1272,  1273,  1274,  1275,  1276,
    1277,  1278,  1279,  1280,  1281,  1282,  1283,  1284,  1285,  1286,
    1290,  1291,  1292,  1293,  1294,  1295,  1296,  1297,  1298,  1299,
    1300,  1301,  1305,  1306,  1310,  1314,  1315,  1316,  1320,  1322,
    1324,  1326,  1328,  1333,  1334,  1338,  1339,  1340,  1341,  1342,
    1343,  1344,  1345,  1346,  1350,  1351,  1352,  1353,  1354,  1355,
    1359,  1360,  1364,  1365,  1366,  1367,  1368,  1369,  1373,  1374,
    1377,  1378,  1382,  1383,  1387,  1389,  1394,  1395,  1399,  1400,
    1408,  1409,  1411,  1413,  1415,  1420,  1421,  1424,  1425,  1426,
    1427,  1428,  1429,  1430,  1433,  1434,  1437,  1439,  1441,  1446,
    1459,  1476,  1477,  1479,  1484,  1485,  1486,  1487,  1488,  1492,
    1498,  1506,  1507,  1515,  1517,  1522,  1524,  1526,  1531,  1533,
    1535,  1542,  1543,  1544,  1549,  1551,  1553,  1557,  1561,  1563,
    1567,  1575,  1576,  1577,  1578,  1579,  1584,  1585,  1586,  1587,
    1588,  1608,  1612,  1616,  1624,  1631,  1632,  1633,  1637,  1639,
    1645,  1647,  1649,  1654,  1655,  1656,  1657,  1658,  1664,  1665,
    1666,  1667,  1671,  1672,  1676,  1677,  1678,  1682,  1683,  1687,
    1688,  1692,  1693,  1697,  1698,  1699,  1700,  1704,  1705,  1716,
    1718,  1724,  1725,  1726,  1727,  1728,  1729,  1731,  1733,  1735,
    1737,  1739,  1741,  1744,  1746,  1748,  1750,  1752,  1754,  1756,
    1758,  1761,  1763,  1765,  1771,  1773,  1775,  1777,  1779,  1781,
    1783,  1785,  1787,  1789,  1791,  1793,  1795,  1802,  1808,  1814,
    1820,  1829,  1839,  1847,  1848,  1849,  1850,  1851,  1852,  1853,
    1854,  1859,  1860,  1864,  1868,  1869,  1873,  1877,  1878,  1882,
    1886,  1890,  1897,  1898,  1899,  1900,  1901,  1902,  1906,  1907,
    1912,  1917,  1925,  1926,  1927,  1928,  1929,  1930,  1931,  1932,
    1933,  1935,  1937,  1939,  1941,  1943,  1948,  1949,  1952,  1953,
    1954,  1957,  1958,  1959,  1960,  1971,  1972,  1973,  1977,  1978,
    1979,  1983,  1984,  1985,  1986,  1987,  1995,  1996,  1997,  1998,
    2002,  2003,  2007,  2008,  2009,  2010,  2011,  2012,  2013,  2014,
    2015,  2016,  2017,  2018,  2019,  2023,  2031,  2032,  2036,  2037,
    2038,  2039,  2040,  2041,  2042,  2043,  2044,  2045,  2046,  2047,
    2048,  2049,  2050,  2051,  2052,  2053,  2054,  2055,  2056,  2057,
    2058,  2059,  2060,  2064,  2065,  2066,  2067,  2068,  2069,  2070,
    2074,  2075,  2076,  2077,  2081,  2082,  2083,  2084,  2089,  2090,
    2091,  2092,  2093,  2094,  2095
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
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   170,   171,   172,   172,   173,   173,   174,   174,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   176,
     177,   177,   177,   178,   178,   178,   179,   179,   180,   180,
     180,   181,   181,   182,   182,   182,   182,   183,   183,   184,
     184,   185,   185,   185,   186,   186,   186,   187,   188,   188,
     188,   188,   188,   189,   189,   190,   190,   190,   190,   190,
     190,   191,   191,   191,   191,   191,   191,   192,   192,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   195,
     195,   195,   195,   196,   196,   197,   197,   198,   198,   198,
     198,   199,   199,   199,   199,   199,   199,   199,   200,   200,
     200,   200,   201,   201,   201,   201,   201,   201,   201,   201,
     202,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   204,   205,   205,
     205,   205,   205,   205,   205,   205,   206,   207,   207,   207,
     207,   207,   207,   208,   208,   209,   209,   209,   210,   210,
     211,   212,   212,   213,   213,   214,   214,   214,   215,   215,
     216,   216,   216,   217,   217,   218,   218,   218,   219,   219,
     220,   221,   221,   221,   222,   222,   224,   225,   223,   226,
     226,   226,   228,   229,   227,   230,   230,   230,   230,   230,
     231,   231,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   234,   234,   235,   236,   236,   236,   237,   237,
     237,   237,   237,   238,   238,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   240,   240,   240,   240,   240,   240,
     241,   241,   242,   242,   242,   242,   242,   242,   243,   243,
     244,   244,   245,   245,   246,   246,   247,   247,   248,   248,
     249,   249,   249,   249,   249,   250,   250,   251,   251,   251,
     251,   251,   251,   251,   252,   252,   253,   253,   253,   254,
     254,   255,   255,   255,   256,   256,   256,   256,   256,   257,
     257,   258,   258,   259,   259,   260,   260,   260,   261,   261,
     261,   262,   262,   262,   263,   263,   263,   263,   263,   263,
     263,   264,   264,   264,   264,   264,   265,   265,   265,   265,
     265,   266,   266,   266,   266,   267,   267,   267,   268,   268,
     268,   268,   268,   269,   269,   269,   269,   269,   270,   270,
     270,   270,   271,   271,   272,   272,   272,   273,   273,   274,
     274,   275,   275,   276,   276,   276,   276,   277,   277,   278,
     278,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   281,   282,   283,   283,   283,   283,   283,   283,   283,
     283,   284,   284,   285,   286,   286,   287,   288,   288,   289,
     289,   289,   290,   290,   290,   290,   290,   290,   291,   291,
     292,   293,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   295,   295,   296,   296,
     296,   297,   297,   297,   297,   298,   298,   298,   299,   299,
     299,   300,   300,   300,   300,   300,   301,   301,   301,   301,
     302,   302,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   304,   304,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   306,   306,   306,   306,   306,   306,   306,
     307,   307,   307,   307,   308,   308,   308,   308,   309,   309,
     309,   309,   309,   309,   309
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     4,     3,     4,     3,     4,     3,     5,     3,     3,
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
       3,     0,     0,     1,     0,    87,   543,   544,   545,   546,
     547,   548,     0,   481,    73,   108,   449,   114,   451,   481,
       0,   113,     0,   355,    73,     0,     0,     0,     0,   453,
     516,   516,   540,     0,     0,     0,     0,   112,     0,   230,
     109,     0,   226,     0,     0,   116,     0,     0,   472,   549,
     117,     0,   231,   443,   354,     0,     0,     0,    45,    44,
     111,   452,   356,     0,     0,     0,     0,   447,     0,     0,
     115,     0,     0,    88,     0,   541,     0,     0,     0,   110,
     212,   445,   358,   118,     0,     0,   600,     0,   602,     0,
     603,   604,   515,     0,   601,   598,   127,   599,     0,     0,
       0,     4,     0,     5,     0,     9,    46,    10,     0,    11,
      12,    14,   427,   428,    22,    13,   128,   136,   137,    15,
      17,    16,    19,    20,    21,    18,   135,     0,   133,     0,
     507,     0,   131,   134,     0,   132,   521,   503,   429,   504,
     433,   431,     0,     0,   508,   509,     0,   432,     0,   522,
     523,   524,   542,   502,   435,   434,   505,   506,     0,    38,
      24,   441,     0,     0,   482,    74,     0,     0,   451,   453,
       0,     0,     0,   452,     0,     0,     0,   507,   521,   431,
     508,   509,   450,   432,   522,   523,     0,   481,     0,     0,
     357,     0,   186,     0,   412,     0,   419,   546,   452,   517,
     229,   546,   150,   452,     0,   229,     0,     0,     0,     0,
       0,     0,     0,   141,     0,     0,   419,    81,    89,   101,
      95,    94,   103,    84,    93,   104,    90,   105,    82,   106,
      99,    92,   100,    98,    96,    97,    83,    85,    91,   102,
     107,     0,    86,     0,     0,     0,     0,   361,     0,   124,
      32,     0,   585,   500,     0,   444,   586,     7,   419,   210,
     220,   516,   211,   130,   129,   325,   409,     0,   408,     0,
       0,   125,   520,     0,     0,    35,     0,   448,   436,     0,
     419,    36,   442,     0,   193,   189,     0,   432,   193,   190,
       0,   351,     0,   446,     0,     0,   587,   589,   514,   584,
     583,     0,    48,    51,     0,     0,   414,     0,   416,     0,
       0,   415,     0,     0,   408,     0,     0,     6,     0,    47,
       0,     0,   213,     0,   311,   310,   232,     0,   430,    23,
       0,     0,     0,     0,     0,     0,     0,   588,     0,     0,
       0,     0,     0,     0,   513,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     270,   277,   278,   279,   274,   276,     0,     0,   272,   275,
     273,   271,     0,   281,   280,     0,     0,   419,   419,     0,
       0,     0,    25,    26,     0,     0,     0,     0,     0,    27,
       0,     0,     0,     0,    28,     0,    29,     0,   427,   425,
       0,   420,   421,   426,     0,   213,   147,     0,     0,   213,
       0,   146,     0,     0,     0,   159,     0,     0,   158,     0,
     167,     0,     0,     0,   165,     0,     0,    57,   138,     0,
     179,     0,     0,    30,   293,   227,   365,     0,   366,   368,
       0,   390,     0,   371,     0,     0,   123,    31,    33,     0,
     324,     0,    67,   518,   519,   126,     0,    34,   419,     0,
     200,   191,   187,   192,   188,     0,   349,   346,   152,    37,
      50,    49,    52,     0,   550,     0,     0,   536,     0,   538,
       0,     0,     0,     0,     0,     0,     0,     0,   554,     8,
       0,    40,     0,     0,     0,   408,     0,     0,     0,   304,
     360,   480,   580,   579,   582,   591,   590,   595,   594,   576,
     573,   574,   575,   511,   561,     0,   533,   534,   532,   531,
     512,   566,   565,   578,   572,   570,   581,   571,   569,   559,
     564,   568,   567,   577,   558,   562,   563,   560,     0,     0,
       0,     0,     0,     0,     0,   593,   592,   597,   596,   492,
     493,   495,   497,     0,   484,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   552,   481,   481,   155,   347,   359,
     413,     0,     0,   438,     0,   213,     0,     0,   213,     0,
     348,     0,     0,     0,     0,   456,     0,     0,     0,   168,
       0,   462,     0,     0,   166,   599,    60,     0,    53,    58,
       0,   178,     0,     0,     0,   437,   298,   295,   296,   297,
     301,   302,   303,   293,     0,   286,     0,   294,   312,     0,
     369,     0,   121,   122,   120,   119,     0,   389,   388,   503,
       0,   363,   501,   362,   530,   411,   410,     0,     0,     0,
     439,     0,   194,   353,   503,     0,   551,   510,   537,   417,
     539,   418,   175,     0,     0,     0,   553,   173,   466,     0,
     556,   555,     0,    42,    41,    39,    64,     0,    57,   214,
       0,     0,   224,     0,   221,   308,   305,   306,   309,   233,
       0,     0,    71,    72,    70,    69,    68,   528,   529,   494,
     496,     0,   483,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   151,   423,   424,   422,
       0,   215,   149,     0,   215,   148,     0,   177,     0,     0,
     157,     0,   156,     0,   487,     0,     0,   163,     0,     0,
     161,     0,     0,   139,     0,   140,     0,     0,   181,     0,
     183,   299,   300,     0,   293,   284,     0,   403,   313,   316,
     315,   317,     0,   367,   370,   371,     0,     0,   372,   373,
     202,     0,     0,   201,   204,   440,     0,   195,   198,     0,
     350,   176,     0,     0,     0,     0,   174,     0,     0,     0,
       0,   229,   219,     0,   222,   218,   307,   312,   282,    75,
     265,    89,   263,    95,    94,    78,    93,    90,   268,   105,
      76,   106,    92,    96,    77,    79,    91,   107,   262,   244,
     247,   245,   246,   257,   248,   261,   253,   251,   264,   267,
     252,   250,   255,   260,   249,   254,   258,   259,   256,   266,
       0,   242,     0,    80,     0,   282,   282,   240,   535,   485,
     371,   521,   521,     0,     0,     0,     0,     0,     0,     0,
       0,   154,   153,   215,   229,   215,   229,   160,     0,     0,
     455,     0,   454,     0,   486,     0,     0,   461,   164,     0,
     460,   162,    55,    54,   180,   471,   182,     0,   403,   287,
       0,     0,   371,   314,   330,   364,   394,     0,   552,     0,
     206,     0,     0,     0,   196,     0,   171,   468,     0,     0,
     169,   467,     0,   557,    65,    66,   209,   229,   229,   208,
     229,   216,   225,   223,     0,   293,   239,   243,     0,   269,
       0,   235,   236,   489,     0,     0,     0,     0,     0,     0,
       0,     0,   229,   144,   229,   142,   459,     0,     0,   488,
     465,     0,     0,     0,     0,   185,   371,   403,     0,   406,
     405,   407,   503,   326,   290,   288,     0,     0,     0,     0,
     392,   503,   207,   205,     0,   199,     0,   172,     0,   170,
     217,   385,     0,   318,     0,   241,    75,    77,   282,   282,
       0,     0,     0,     0,     0,     0,   145,   143,     0,     0,
       0,     0,    56,   184,   289,   371,   395,     0,   327,   329,
     328,   344,     0,   345,   332,   335,     0,   331,   322,   323,
     228,     0,   197,     0,     0,     0,   384,   383,   503,   319,
     330,   283,   237,   238,     0,     0,     0,     0,     0,     0,
     458,   457,   464,   463,   292,   291,   397,   398,   400,   503,
       0,   552,   343,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   503,   470,   469,     0,   375,     0,     0,     0,
     399,   401,   334,   336,   337,   340,   341,   342,   338,   339,
     333,   380,   378,   503,   552,   320,   234,   321,   395,   379,
     503,   402
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   303,   102,   601,   104,   105,   106,   320,
     107,   304,   596,   600,   597,   108,   109,   110,   111,   166,
     831,   245,   112,   242,   113,   627,   250,   114,   115,   116,
     117,   118,   119,   120,   702,   121,   122,   123,   461,   642,
     769,   124,   125,   638,   764,   126,   127,   497,   781,   128,
     129,   673,   674,   177,   243,   618,   131,   132,   499,   787,
     679,   834,   835,   431,   916,   435,   614,   615,   616,   617,
     680,   326,   752,  1020,  1076,  1010,   266,   999,   954,   958,
    1004,  1005,  1006,   133,   292,   466,   134,   135,   246,   247,
     439,   440,   631,  1017,   973,   443,   628,  1037,   951,   882,
     305,   193,   309,   310,   400,   401,   402,   178,   137,   138,
     139,   140,   179,   142,   163,   164,   553,   421,   723,   554,
     555,   143,   180,   181,   146,   205,   403,   183,   148,   184,
     185,   151,   152,   153,   315,   154,   155,   156,   157,   158
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -981
static const yytype_int16 yypact[] =
{
    -981,    63,  2706,  -981,   -44,  -981,  -981,  -981,  -981,  -981,
    -981,  -981,  4176,   -14,   141,  -981, 13241,  -981, 15421,   -14,
   10200,  -981,   280,    96,   141,  4176, 10200,  4176,    26, 16829,
   10363,  7918,  -981,  8081,  8733,  6461, 10200,  -981,    70,  -981,
    -981, 17905,  -981, 13663,  8896,  -981, 10200, 10200,  -981,  -981,
    -981, 10200,  -981, 13241,  -981, 10200,   254,   210,  7762, 15805,
    -981, 17962,  -981,  9059,  7276, 10200,  8896, 13241, 10200,   283,
    -981,   237,  4176,  -981, 10200,  -981, 10526, 10526, 17905,  -981,
    -981, 13241,  -981,  -981, 10200, 10200,  -981, 10200,  -981, 10200,
    -981,  -981, 12808, 10200,  -981, 10200,  -981,  -981,  3198,  6624,
    8244,  -981,  4013,  -981,   124,  -981,   325,  -981,   301,  -981,
    -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,
    -981,  -981,  -981,  -981,  -981,  -981,  -981, 17905,  -981, 17905,
     138,   122,  -981,  -981, 13663,  -981,   256,  -981,   258,  -981,
    -981,   266,   291,   316,   317,   321, 17521,  2492,   -52,   326,
     330,  -981,  -981,  -981,  -981,  -981,  -981,  -981,   311,  -981,
    -981, 17521,   318,  4176,  -981,  -981,   337, 10200,  -981,  -981,
   10200, 10200, 10200,  -981, 10200,  9059,  9059,   294,  -981,  -981,
    -981,  -981,   363,   313,  -981,  -981,   344, 15494, 17905, 13663,
    -981,   352,  -981,   103, 17521,   399,  7439,   431, 17905, 17521,
     631,   432,  -981, 17905, 17905,   631, 17905,   356,     9, 15028,
      -4, 14996,    45,  -981, 15437, 14244,  7439,  -981,  -981,  -981,
    -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,
    -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,
    -981,  4176,  -981,   358,  2171,    58,   -13,  -981,  4176,  -981,
    -981, 15172,   914,   363, 15172,   363,   914,  -981,  7439,  -981,
    -981,  9222,  -981,  -981,  -981, 17905,  -981,   144, 17521, 10200,
   10200,  -981, 17521,   366, 15405,  -981, 15172,   363, 17521,   368,
    7439,  -981, 17521, 15581,  -981,  -981, 15846,  1356,  -981,  -981,
   15895,   413,   377,   363, 15172, 15990,   364,   364,  1176,   914,
     914,   108,  -981,  -981,  3361,   -59,  -981, 10200,  -981,    71,
     110,  -981,   -27,    24, 16039,    86,   528,  -981,  3524,  -981,
     478,  9059,   403,   380,  -981,  -981,  -981,   216,  -981,  -981,
   10200, 10200, 10200, 10200,  8733,  8733, 10200,   331, 10200, 10200,
   10200, 10200, 10200,   389, 12808,  9385, 10200, 10200, 10200, 10200,
   10200, 10200, 10200, 10200,  9548, 10200, 10200, 10200, 10200, 10200,
    -981,  -981,  -981,  -981,  -981,  -981,  8407,  8407,  -981,  -981,
    -981,  -981,  8407,  -981,  -981,  8407,  8407,  7439,  7439,  8733,
    8733,  7113,  -981,  -981, 15638, 15670, 16071,    65,  3687,  -981,
    8733,     9,   387,   242,  -981, 10200,  -981, 10200,   428,  -981,
     381,   417,  -981,  -981, 17905,   403,  -981, 13663, 17905,   403,
     405,  -981, 13663,   511,  9059,  -981,  4339,  8733,  -981,   401,
    -981,     9,  4502,  8733,  -981,     9, 10689, 10200,  -981,  4176,
     532, 10200,   407,  -981,   597,  -981,  -981,  2171,  -981,   430,
     409,  -981, 10852,   453, 10200, 13663,  -981,  -981,  -981,   410,
    -981,  9059,  -981, 17521, 17521,  -981,    29,  -981,  7439,   416,
    -981,   557,  -981,   557,  -981, 11015,   446,  -981,  -981,  -981,
    -981,  -981,  -981,  8570,  -981, 14100,  6787,  -981,  6950,  -981,
    4176,   418,  8733,  5969,  3035,   419, 10200,  6297,  -981,  -981,
     149,  -981,  3850, 17905,   247, 15814,  9059,   425, 16653,   289,
    -981, 16215, 17708, 17708,   297,  -981,   297,  -981,   297, 13910,
    1029,  1301,   495,   363,   364,   427,  -981,  -981,  -981,  -981,
    1176,  -981, 17034,   297,   672,   672, 17708,   672,   672,  1266,
     364,  -981, 17034, 17761,  1266,   914,   914,   364,   438,   441,
     442,   445,   451,   452,   443,  -981,   297,  -981,   297,    38,
    -981,  -981,  -981,   119,  -981,  1506, 17597,   343, 11178,  8733,
   11341,  8733, 10200,  8733, 13505,   -14, 16260,  -981,  -981,  -981,
   17521, 16301,  7439,  -981,  7439,   403,   449,   253,   403,   450,
    -981,   281, 10200,   125, 10200, 17521,    27, 15229,  7113,  -981,
   10200, 17521,     5, 15085,  -981,   459,   471,   461, 16412,   471,
     463,   587, 16452,  4176, 15261,  -981,    92,  -981,  -981,  -981,
    -981,  -981,  -981,   858,   134,  -981, 13720,  -981,   350,   458,
    2171,    58,    26,    70, 10200, 10200,  5806,  -981,  -981,   684,
    7602,  -981, 17521,  -981,  -981,  -981, 17521,   462,    34,   464,
    -981, 13584,  -981,  -981,   293, 17905,  -981,  -981,  -981,  -981,
    -981,  -981,  -981,  4176,    39, 13868,  -981,  -981, 17521,  4176,
   17521,  -981, 16493,  -981,  -981,  -981,  -981, 10689, 10200,   488,
    2523,   467,   516,   -41,  -981,   554,  -981,  -981,  -981,  -981,
    7757,   473,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,
    -981,  7113,  -981,    16,  8733,  8733, 10200,   608, 16637, 10200,
     611, 16669,   481, 13956,     9,     9,  -981,  -981,  -981,  -981,
     487,  -981,  -981,   497,  -981,  -981, 15172,  -981, 14320,  4665,
    -981,  4828,  -981,   135,  -981, 14396,  4991,  -981,     9,  5154,
    -981,     9, 10200,  -981, 10200,  -981,  4176, 10200,  -981,  4176,
     616,  -981,  -981, 17905,   882,  -981,  2171,   526,   585,  -981,
    -981,  -981,    89,  -981,  -981,   453,   502,    78,  -981,  -981,
    -981,  5317,  9059,  -981,  -981,  -981, 17905,  -981,   534,   344,
    -981,  -981,  5480,   505,  5643,   506,  -981, 10200,   517,   518,
     513,  1540,  -981, 10200, 17905,  -981,  -981,   350,   529,   312,
    -981,   537,  -981,   541,   550,   560,   556,   561,  -981,   564,
     562,   569,   572,   574,   329,   584,   577,   579,  -981,  -981,
   17905,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,
    -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,
   10200,  -981,   588,   590,   592,   529,   529,  -981,  -981,  -981,
     453,   150,   194, 16813, 11504, 11667, 16845, 11830, 11993, 12156,
   12319,  -981,  -981,  -981,  1848,  -981,  2338,  -981,  4176, 10200,
   17521, 10200, 17521,  7113,  -981,  4176, 10200, 17521,  -981, 10200,
   17521,  -981, 16902, 17521,  -981, 17521,   691,  4176,   526,  -981,
     565,  9711,   176,  -981,   121,  -981,  -981,  8733, 13373,  4176,
    -981,    17,   568, 10200,  -981, 10200,  -981, 17521,  4176, 10200,
    -981, 17521,  4176, 17521,  -981,  -981,  -981,  1950,  1950,  -981,
    1751,  -981, 17521,  -981,    73,   597,  -981,  -981, 14157,  -981,
   13812,  -981,  -981,  -981, 10200, 10200, 10200, 10200, 10200, 10200,
   10200, 10200, 16226,  -981, 17732,  -981, 16452, 14472, 14548,  -981,
   16452, 14624, 14700, 10200,  4176,  -981,   453,   526,  6133,  -981,
    -981,  -981,   101,  9059,  -981,  -981,    98, 10200,   -36, 14013,
    -981,   988,   399,  -981,   344, 17521, 14776,  -981, 14852,  -981,
    -981,  -981, 12482,   637,   211,  -981,   604,   610,   529,   529,
   16989, 17065, 17141, 17217, 17293, 17369,  -981,  -981,  4176,  4176,
    4176,  4176, 17521,  -981,  -981,   176,  9874,    85,  -981,  -981,
   17521,  -981,    46,  -981,    76,  -981,   447, 17445,  -981,  -981,
    -981, 12319,  -981,  4176,  4176,  2869,  -981,  -981,   201,  -981,
     121,  -981,  -981,  -981, 10200, 10200, 10200, 10200, 10200, 10200,
   16452, 16452, 16452, 16452,  -981,  -981,  -981,  -981,  -981,   132,
    8733, 12944,  -981, 10200,    98,    46,    46,    46,    46,    46,
      46,    98,  1034, 16452, 16452,   573, 12645,    87,   -33, 14054,
    -981,  -981, 17521,  -981,  -981,  -981,  -981,  -981,  -981,  -981,
     605,  -981,  -981,   345, 13107,  -981,  -981,  -981, 10037,  -981,
     444,  -981
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -981,  -981,  -981,     0,  -328,  1806,  -981,  -981,  -981,  -981,
     305,   429,  -414,    75,    79,  -981,  -981,  -981,   275,   721,
    -981,  1505,  -150,  -654,  -981,  -824,   931,  -920,  -689,   -49,
    -981,  -981,  -981,  -981,   100,  -981,  -981,  -981,   460,  -981,
     -17,  -981,  -981,  -981,  -981,  -981,  -981,  -372,  -635,  -981,
    -981,  -981,   -31,   638,  -981,  -981,  -981,   102,  -981,  -981,
    -981,  -981,  -169,  -146,  -742,  -981,  -161,    11,   151,  -981,
    -981,  -981,   -30,  -981,  -981,  -295,   308,  -981,  -227,  -251,
    -281,  -272,  -770,  -981,  -176,  -981,     7,   738,  -119,   332,
    -981,  -421,  -735,  -873,  -981,  -576,  -459,  -980,  -892,  -779,
     -58,  -981,   -20,  -981,  -191,  -981,   202,   440,  -351,  -981,
    -981,  -981,  1075,  -981,    -8,  -981,  -981,  -167,  -981,  -581,
    -981,  -981,  1253,  1439,   -12,   745,    21,   214,  -981,  1593,
    1652,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -378
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -553
static const yytype_int16 yytable[] =
{
     161,   376,   101,   557,   182,   267,   643,   724,   187,   263,
     264,   186,   392,   599,   194,   327,   619,   441,   199,   199,
     885,   209,   211,   214,   215,   432,   833,   248,   410,   189,
     637,   444,   251,   576,   252,   253,   726,   579,  1009,   254,
     248,   255,   313,   256,   425,   755,   398,    64,   248,  1001,
      64,   268,   272,   274,   276,   277,   278,   949,   719,   441,
     282,  1060,   283,     3,   286,   290,   398,   449,   482,   293,
     393,    23,   294,   295,   971,   296,   854,   297,   473,   856,
     298,   299,   689,   300,   419,   273,   268,   272,   314,   459,
     441,   629,   911,   921,   922,  -203,   784,   162,  1081,   946,
     761,  1001,  -386,   474,  1038,   923,   613,   419,   398,   563,
     839,   377,   159,   378,   644,    54,   419,   840,   387,   690,
     311,   785,   887,  1075,   445,   388,    62,  -381,   388,  1040,
     398,   563,  -386,   208,   210,   419,   741,  -203,  1009,   480,
    1003,   376,   762,  -386,   165,   481,   485,   955,  1016,  1061,
     419,    82,   442,  -386,   451,   161,  -381,   388,   384,   385,
     386,   483,   282,   268,   314,   911,   388,   911,   995,  -381,
     388,   324,  -386,   742,  -386,   956,   884,   190,   388,   391,
    -386,  1002,  -381,  1072,   272,  1043,   543,   544,   388,   196,
     484,  -203,  1003,   519,   442,  -386,   763,   325,  -386,   754,
     312,  1079,   483,   710,   272,   772,   713,   407,   476,   972,
     557,   994,   412,  1044,  -386,   483,   147,  -404,   932,   189,
     934,   970,   483,   487,   483,   442,   147,   398,   398,  -381,
     957,   564,  1042,   216,  -381,   477,  1022,  1023,  -404,   147,
     395,   147,  -404,   911,   888,   911,   272,   478,  -396,   199,
    -386,  1041,   488,  1074,   599,  -382,   691,   453,   454,   396,
    1035,   257,   451,   494,   159,  -404,   833,   639,   272,  -396,
     470,   744,   863,  -396,   479,  1064,  1065,  1066,  1067,  1068,
    1069,   451,   939,   692,  -382,   318,   147,  -491,   577,   717,
     287,   287,   630,   581,  -477,   475,  -396,  -382,   745,   864,
     452,  -527,   406,  -527,   472,   159,    23,   411,   398,   495,
    -382,   663,   147,   557,  -491,   675,   147,   953,   501,   502,
     503,   504,   506,   508,   509,   880,   510,   511,   512,   513,
     514,  -490,   520,   522,   523,   524,   525,   526,   527,   528,
     529,   530,   532,   533,   534,   535,   536,   537,   744,   249,
      54,   285,   289,   445,   272,   272,   583,  -382,  -490,   676,
     272,    62,  -382,   272,   272,   272,   272,   546,   548,   556,
     677,   249,   500,   258,   279,  1021,   748,   147,   566,   445,
     188,   284,   288,   570,   451,   571,    82,   538,   539,   678,
     445,   379,     5,   540,   333,   380,   541,   542,   569,  -374,
     280,   319,   268,   666,   585,   587,   321,   308,   515,   712,
     591,   593,  -474,   328,   598,   598,   613,   602,   445,   604,
     749,   516,  -473,   695,   398,   728,   731,   380,  -374,   960,
    -352,   750,   632,   341,   505,   507,   343,   715,   669,   636,
     346,  -374,   136,   -81,   334,   517,   272,   329,   335,  -352,
     751,   -87,   136,   910,  -374,   147,   649,  -527,   651,  -527,
     -83,   636,   147,   330,   272,   136,   272,   136,   -88,   770,
     655,   636,   658,  -478,   660,   662,  -525,  -479,  -525,   545,
     547,   381,  -476,    73,   268,   557,  -475,   773,   775,   518,
     565,   389,   472,   383,  -526,   337,  -526,   311,  -376,   311,
     341,  -374,   343,   343,   399,   388,  -374,   346,   394,   397,
     404,   408,   136,   249,   249,   249,   249,   586,   147,   414,
     430,   434,   455,   592,   399,   333,   910,  -376,   910,   465,
     952,   458,   147,   467,   836,   489,   493,   961,   136,   496,
    -376,   498,   136,   568,   572,   573,   585,   698,   591,   701,
     602,   703,   658,  -376,   574,   582,   249,   704,   705,   249,
     272,   580,   272,  1045,   588,   603,   399,   620,   757,   630,
     716,   605,   718,   621,   634,   334,   556,   641,   725,   335,
     640,   249,   654,   645,   653,   659,   670,   613,   399,  1046,
     681,  1047,  1048,   708,   682,  1049,  1050,   683,   684,   249,
    -376,   685,   147,   136,   910,  -376,   910,   686,   732,   688,
     711,   714,   278,   282,   314,   -59,   687,   733,   272,   735,
     736,  1018,   753,   606,   760,   451,   337,   338,   765,   782,
     147,   341,   783,   342,   343,   786,   147,   838,   346,   844,
     130,   607,   847,   147,   608,  1039,   353,   849,   853,   877,
     130,   759,   357,   358,   359,   598,   598,    23,   855,   697,
    1052,   700,   881,   130,   609,   130,   883,   610,   886,    56,
     893,   898,   902,   904,   905,   906,  -108,    39,   611,   556,
    -114,   136,   506,   546,   843,   399,   399,   846,   136,  -113,
    1039,   -84,   915,   -82,   147,  -112,   249,   612,   147,    52,
    -109,    54,   333,  -116,   891,  1073,   147,   860,  -117,   862,
     130,  -111,    62,  -115,   867,   -85,  -110,   870,  -118,   919,
     872,   -86,   873,  1080,   944,   875,   249,  1039,  -387,   947,
     249,   920,   964,  1019,  -285,   -81,   130,    82,  -387,  1071,
     130,   -83,  1044,   779,   136,   191,   778,   492,   463,   892,
     268,   978,   334,   913,   974,   879,   335,   914,   136,   635,
     897,  -285,   901,  1077,   743,   903,   399,  -387,  1034,  1058,
    1070,   912,  1063,   213,   979,   200,   709,   633,     0,     0,
       0,   635,     0,     0,   308,     0,   308,     0,     0,     0,
       0,   635,     0,  -387,   505,   545,     0,     0,     0,     0,
    -387,   130,     0,   337,   338,     0,   339,   340,   341,     0,
     342,   343,   344,     0,     0,   346,     0,   147,   918,     0,
       0,  -387,   352,   353,     0,     0,   356,     0,   136,   357,
     358,   359,   860,   862,     0,   867,   870,   897,   901,     0,
    -387,     0,     0,     0,     0,  -387,   936,   937,  -387,   938,
       0,   556,     0,   940,   941,     0,   136,   942,   263,   264,
       0,     0,   136,     0,     0,     0,     0,   147,     0,   136,
       0,     0,     0,   147,     0,   959,   658,     0,     0,   130,
     707,   965,   399,   966,   606,     0,   130,   968,     0,     0,
     997,   249,   249,     0,     0,     0,     0,   249,   249,     0,
       0,     0,   607,     0,     0,   608,     0,     0,   606,   740,
       0,     0,   936,   980,   981,   940,   982,   983,   984,   985,
     136,     0,     0,     0,   136,   609,   607,     0,   610,   608,
     316,   992,   136,   147,     0,   147,   314,     0,     0,   611,
     147,  1000,   130,   147,   333,  1007,   767,     0,     0,   609,
     147,     0,   610,   147,    56,     0,   130,  1057,   612,     0,
       0,     0,     0,   611,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   147,  1030,  1031,  1032,  1033,
       0,     0,   612,     0,     0,     0,   147,     0,   147,     0,
       0,     0,     0,     0,   334,   693,     0,   275,   335,   901,
       0,  1053,  1054,   314,     0,     0,     0,     0,     0,   249,
     249,     0,  1030,  1031,  1032,  1033,  1053,  1054,     0,     0,
       0,   249,     0,   430,     0,     0,   130,     0,  1059,   658,
     430,  1062,  -391,   249,     0,     0,   249,     0,     0,     0,
       0,     0,  -391,   136,     0,   337,     0,     0,     0,     0,
     341,     0,   342,   343,   130,     0,     0,   346,     0,   333,
     130,     0,   658,     0,     0,   353,   901,   130,     0,     0,
       0,  -391,   147,   359,   894,     0,     0,   141,  -393,   147,
       0,     0,     0,     0,     0,     0,     0,   141,  -393,     0,
       0,   147,     0,   136,     0,     0,     0,  -391,     0,   136,
     141,     0,   141,   147,  -391,     0,     0,     0,     0,   334,
       0,     0,   147,   335,     0,     0,   147,  -393,   130,     0,
     837,     0,   130,     0,     0,  -391,     0,     0,     0,     0,
     130,     0,     0,     0,   841,   842,     0,     0,     0,   415,
     418,   420,   424,  -393,  -391,     0,     0,   141,     0,  -391,
    -393,     0,  -391,     0,     0,     0,     0,     0,   147,   136,
     337,   136,     0,     0,     0,   341,   136,   342,   343,   136,
       0,  -393,   346,   141,     0,     0,   136,   141,     0,   136,
     353,     0,   447,     0,     0,   448,   357,   358,   359,   950,
    -393,     0,     0,     0,     0,  -393,   249,     0,  -393,     0,
       0,   136,   147,   147,   147,   147,   333,   457,     0,     0,
       0,     0,   136,     0,   136,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,     0,   147,   147,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   141,     0,
       0,   130,   430,   430,     0,     0,   430,   430,     0,     0,
       0,     0,     0,     0,     0,   144,   334,     0,     0,     0,
     335,   998,     0,  1008,     0,   144,     0,     0,     0,  1012,
       0,   430,     0,   430,     0,     0,     0,     0,   144,     0,
     144,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   130,     0,     0,     0,     0,   333,   130,   136,     0,
       0,     0,     0,     0,  1036,   136,     0,   337,   338,     0,
     339,   340,   341,     0,   342,   343,   141,   136,     0,   346,
       0,     0,   567,   141,     0,   144,   352,   353,     0,   136,
     356,   333,     0,   357,   358,   359,     0,     0,   136,     0,
       0,     0,   136,     0,     0,     0,   334,     0,     0,  1036,
     335,   144,   589,     0,     0,   144,   594,   130,     0,   130,
       0,     0,     0,  1008,   130,     0,     0,   130,     0,     0,
       0,     0,     0,     0,   130,     0,     0,   130,     0,   141,
       0,   334,     0,     0,   136,   335,  1036,     0,     0,     0,
       0,     0,     0,   141,     0,     0,     0,   337,   338,   130,
     339,   340,   341,     0,   342,   343,     0,     0,     0,   346,
     130,     0,   130,     0,     0,     0,   144,   353,     0,     0,
       0,     0,     0,   357,   358,   359,     0,     0,   136,   136,
     136,   136,   337,   338,     0,     0,   340,   341,     0,   342,
     343,   145,     0,     0,   346,     0,     0,     0,     0,     0,
       0,   145,   353,   136,   136,     0,     0,     0,   357,   358,
     359,     0,     0,   141,   145,     0,   145,     0,     0,     0,
       0,     0,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,     0,     0,     0,
       0,   141,     0,     0,   144,     0,   130,   141,     0,     0,
       0,   144,     0,   130,   141,     0,     0,     0,     0,     5,
       0,   145,     0,     0,     0,   130,   375,   720,   722,  -525,
       0,  -525,    15,   727,   730,    17,     0,   130,     0,     0,
      21,     0,     0,     0,     0,     0,   130,   145,     0,     0,
     130,   145,     0,     0,     0,     0,   241,     0,     0,    37,
       0,     0,     0,     0,    40,   141,     0,   144,     0,   141,
       0,   259,    45,     0,     0,    22,    23,   141,     0,    50,
       0,   144,     0,     0,   260,     0,    30,   261,     0,     0,
       0,    35,   130,   291,    60,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,   149,    70,     0,     0,     0,
      73,     0,   145,     0,     0,   149,     0,    79,    52,     0,
      54,     0,    56,    83,   907,     0,     0,   908,   149,   262,
     149,    62,     0,     0,     0,     0,   130,   130,   130,   130,
       0,     0,   322,     0,   323,   851,   852,     0,     0,     0,
      78,   144,     0,    80,     0,     0,    82,   857,     0,     0,
       0,   130,   130,     0,   150,     0,     0,     0,     0,   868,
       0,     0,   871,     0,   150,   149,     0,     0,     0,   144,
       0,     0,     0,     0,     0,   144,     0,   150,   141,   150,
     145,     0,   144,     0,     0,     0,     0,   145,     0,     0,
       0,   149,     0,   291,     0,   149,    96,     0,     0,     0,
       0,     0,   909,   405,     0,     0,     0,     0,   409,   291,
       0,   413,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   150,     0,     0,     0,   141,     0,
       0,     0,     0,   144,   141,     0,     0,   144,     0,     0,
       0,     0,     0,   145,     0,   144,     0,     0,     0,   438,
     150,     0,     0,     0,   150,     0,   149,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     450,     0,   259,     0,     0,     0,    22,    23,     0,     0,
       0,     0,     0,     0,     0,   260,     0,    30,   261,     0,
       0,     0,    35,     0,   141,     0,   141,    39,     0,     0,
       0,   141,     0,     0,   141,     0,     0,     0,   103,     0,
       0,   141,     0,     0,   141,   150,     0,     0,   160,    52,
       0,    54,   963,   316,     0,   907,     0,   145,   908,     0,
     262,   192,    62,   195,   149,     0,   141,     0,     0,     0,
       0,   149,     0,     0,     0,     0,     0,   141,     0,   141,
       0,    78,     0,     0,    80,   145,   144,    82,     0,     0,
       0,   145,     0,     0,     0,     0,     0,     0,   145,   259,
       0,     0,     0,    22,    23,     0,     0,     0,   281,     0,
       0,     0,   260,     0,    30,   261,     0,     0,     0,    35,
       0,     0,     0,   150,    39,     0,     0,   149,     0,     0,
     150,     0,     0,     0,   103,     0,   144,    96,   317,   575,
       0,   149,   144,   578,     0,     0,    52,     0,    54,   145,
      56,     0,   907,   145,     0,   908,     0,   262,     0,    62,
       0,   145,     0,   141,     0,     0,     0,     0,     0,     0,
     141,     0,   438,     0,     0,     0,     0,     0,    78,     0,
       0,    80,   141,     0,    82,     0,   150,     0,     0,     0,
       0,     0,     0,     0,   141,     0,     0,     0,     0,   382,
     150,   259,   144,   141,   144,    22,    23,   141,     0,   144,
       0,   149,   144,     0,   260,     0,    30,   261,     0,   144,
       0,    35,   144,     0,     0,     0,    39,     0,   665,     0,
       0,     0,     0,   672,    96,     0,     0,     0,     0,   149,
     933,     0,     0,     0,   144,   149,     0,     0,    52,   141,
      54,     0,   149,     0,     0,   144,     0,   144,     0,   262,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
     150,     0,   145,     0,     0,     0,     0,   433,     0,     0,
      78,     0,     0,    80,   446,     0,    82,     0,     0,     0,
       0,     0,     0,   141,   141,   141,   141,     0,   150,     0,
       0,     0,     0,   149,   150,     0,     0,   149,     0,     0,
       0,   150,     0,     0,     0,   149,     0,     0,   141,   141,
       0,     0,   145,     0,     0,     0,     0,     0,   145,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     103,   144,     0,     0,     0,     0,     0,     0,   144,     0,
       0,   747,     0,     0,   103,   438,     0,     0,     0,     0,
     144,     0,   150,     0,     0,     0,   150,     0,     0,     0,
       0,     0,   144,     0,   150,     0,   768,     0,     0,     0,
     291,   144,     0,     0,     0,   144,     0,     0,   145,     0,
     145,     0,     0,     0,     0,   145,     0,     0,   145,     0,
       0,     0,     0,     0,   217,   145,     0,     0,   145,     0,
       0,     0,     0,     0,     0,   832,     0,   218,   219,     0,
     220,     0,     0,     0,   103,   221,   149,   144,     0,     0,
     145,     0,     0,   222,     0,     0,     0,     0,     0,   223,
       0,   145,     0,   145,   224,     0,   225,     0,     0,   226,
       0,     0,   446,     0,     0,     0,     0,   227,   446,     0,
       0,     0,     0,   228,   229,     0,     0,     0,     0,     0,
     230,   144,   144,   144,   144,     0,   149,     0,   878,   231,
       0,   438,   149,     0,     0,   150,     0,     0,   232,   233,
       0,   234,     0,   235,     0,   236,   144,   144,   237,     0,
       0,   768,   238,   436,     0,   239,     0,     0,   240,     0,
       0,     0,     0,     0,     0,     0,   652,     0,     0,   672,
     657,     0,     0,     0,     0,     0,     0,   145,   103,     0,
       0,     0,     0,     0,   145,   150,     0,     0,     0,     0,
       0,   150,   149,     0,   149,   917,   145,     0,     0,   149,
       0,     0,   149,     0,     0,     0,     0,     0,   145,   149,
       0,     0,   149,     0,   437,     0,     0,   145,     0,     0,
       0,   145,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   149,     0,     0,     0,     0,   259,
       0,     0,     0,    22,    23,   149,     0,   149,     0,     0,
       0,   150,   260,   150,    30,   261,     0,     0,   150,    35,
       0,   150,     0,   145,    39,     0,     0,     0,   150,     0,
       0,   150,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,    54,   738,
      56,     0,   907,   150,     0,   908,     0,   262,     0,    62,
       0,     0,     0,     0,   150,   832,   150,   145,   145,   145,
     145,     0,     0,     0,     0,     0,     0,     0,    78,     0,
       0,    80,     0,     0,    82,     0,     0,     0,     0,     0,
       0,   149,   145,   145,     0,     0,     0,     0,   149,   771,
       0,     0,     0,     0,     0,   776,     0,     0,     0,     0,
     149,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   149,     0,     0,     0,     0,     0,     0,     0,
       0,   149,     0,     0,    96,   149,     0,     0,     0,     0,
     935,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     150,     0,     0,     0,     0,     0,     0,   150,     0,     0,
       0,     0,     0,     0,   780,   446,     0,   446,     0,   150,
       0,     0,   446,     0,     0,   446,     0,   149,     0,     0,
       0,   150,   874,     0,  -215,   876,     0,     0,  -215,  -215,
     150,     0,     0,     0,   150,     0,     0,  -215,     0,  -215,
    -215,     0,     0,     0,  -215,     0,     0,   890,     0,  -215,
       0,     0,  -215,     0,     0,     0,     0,     0,   896,     0,
     900,   149,   149,   149,   149,     0,     0,     0,     0,     0,
       0,  -215,     0,  -215,     0,  -215,   150,  -215,  -215,     0,
    -215,     0,  -215,     0,  -215,     0,   149,   149,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,  -215,     0,     0,  -215,     0,     0,  -215,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -498,
     150,   150,   150,   150,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,  -525,     0,  -525,     0,     0,
       0,     0,     0,     0,     0,   150,   150,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -215,
       0,     0,     0,   945,     0,  -215,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   962,     0,     0,     0,     0,
       0,     0,     0,     0,   967,     0,    -2,     4,   969,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
     993,    38,    39,     0,    40,  -229,     0,    41,    42,    43,
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
      95,     0,    96,     0,     0,    97,     0,    98,     0,    99,
    1055,   100,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   167,     0,     0,    15,    16,     0,    17,     0,
     168,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   169,     0,     0,     0,    32,   170,   171,
       0,   172,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   173,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   174,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,   265,     0,     0,     0,    97,     0,
     175,     0,    99,     0,   176,  1056,     4,     0,     5,     6,
       7,     8,     9,    10,    11,     0,  -552,     0,    12,    13,
      14,    15,    16,  -552,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,  -552,    27,    28,  -552,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,     0,
      38,    39,     0,    40,  -229,     0,    41,    42,    43,     0,
      44,    45,    46,   -43,    47,    48,     0,    49,    50,    51,
       0,     0,     0,    52,    53,    54,    55,     0,    57,    58,
    -229,   -43,    59,    60,    61,  -552,    62,    63,    64,  -552,
      65,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      74,     0,    75,    76,    77,    78,    79,     0,    80,    81,
     -61,    82,    83,     0,     0,    84,     0,    85,     0,     0,
    -552,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -552,  -552,    89,  -552,
    -552,  -552,  -552,  -552,  -552,  -552,     0,  -552,  -552,  -552,
    -552,  -552,     0,  -552,  -552,  -552,  -552,  -552,  -552,  -552,
    -552,    96,  -552,  -552,  -552,     0,    98,  -552,    99,   301,
     100,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,     0,    38,    39,     0,    40,  -229,     0,    41,
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
       0,    94,    95,   265,    96,     0,     0,    97,     0,    98,
     302,    99,     4,   100,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,     0,    38,    39,     0,    40,
    -229,     0,    41,    42,    43,     0,    44,    45,    46,   -43,
      47,    48,     0,    49,    50,    51,     0,     0,     0,    52,
      53,    54,    55,    56,    57,    58,  -229,   -43,    59,    60,
      61,     0,    62,    63,    64,     0,    65,    66,    67,    68,
      69,    70,    71,    72,     0,    73,    74,     0,    75,    76,
      77,    78,    79,     0,    80,    81,   -61,    82,    83,     0,
       0,    84,     0,    85,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,     0,    96,     0,     0,
      97,     0,    98,   471,    99,   490,   100,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,     0,    38,
      39,     0,    40,  -229,     0,    41,    42,    43,     0,    44,
      45,    46,   -43,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,    56,    57,    58,  -229,
     -43,    59,    60,    61,     0,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
       0,    75,    76,    77,    78,    79,     0,    80,    81,   -61,
      82,    83,     0,     0,    84,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,     0,
      96,     0,     0,    97,     0,    98,   491,    99,   301,   100,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,    39,     0,    40,  -229,     0,    41,    42,
      43,     0,    44,    45,    46,   -43,    47,    48,     0,    49,
      50,    51,     0,     0,     0,    52,    53,    54,    55,    56,
      57,    58,  -229,   -43,    59,    60,    61,     0,    62,    63,
      64,     0,    65,    66,    67,    68,    69,    70,    71,    72,
       0,    73,    74,     0,    75,    76,    77,    78,    79,     0,
      80,    81,   -61,    82,    83,     0,     0,    84,     0,    85,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,     0,    96,     0,     0,    97,     0,    98,   302,
      99,     4,   100,     5,     6,     7,     8,     9,    10,    11,
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
      93,     0,     0,    94,    95,     0,    96,     0,     0,    97,
       0,    98,   664,    99,     4,   100,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,     0,    38,    39,
       0,    40,  -229,     0,    41,    42,    43,     0,    44,    45,
      46,   -43,    47,    48,     0,    49,    50,    51,     0,     0,
       0,    52,    53,    54,    55,   316,    57,    58,  -229,   -43,
      59,    60,    61,     0,    62,    63,    64,     0,    65,    66,
      67,    68,    69,    70,    71,    72,     0,    73,    74,     0,
      75,    76,    77,    78,    79,     0,    80,    81,   -61,    82,
      83,     0,     0,    84,     0,    85,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,     0,    96,
       0,     0,    97,     0,    98,     0,    99,     4,   100,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
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
      95,     0,    96,     0,     0,    97,     0,    98,     0,    99,
       4,   100,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,   584,    37,     0,    38,    39,     0,    40,  -229,     0,
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
       0,     0,    94,    95,     0,    96,     0,     0,    97,     0,
      98,     0,    99,     4,   100,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,   590,    37,     0,    38,    39,     0,
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
       0,     0,    93,     0,     0,    94,    95,     0,    96,     0,
       0,    97,     0,    98,     0,    99,     4,   100,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,   859,    37,     0,
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
       0,    96,     0,     0,    97,     0,    98,     0,    99,     4,
     100,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
     861,    37,     0,    38,    39,     0,    40,  -229,     0,    41,
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
       0,    94,    95,     0,    96,     0,     0,    97,     0,    98,
       0,    99,     4,   100,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,   866,    37,     0,    38,    39,     0,    40,
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
       0,    93,     0,     0,    94,    95,     0,    96,     0,     0,
      97,     0,    98,     0,    99,     4,   100,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,   869,    37,     0,    38,
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
       0,     0,     0,     0,    93,     0,     0,    94,    95,     0,
      96,     0,     0,    97,     0,    98,     0,    99,     4,   100,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,   889,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
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
      94,    95,     0,    96,     0,     0,    97,     0,    98,     0,
      99,     4,   100,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,   895,    37,     0,    38,    39,     0,    40,  -229,
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
      93,     0,     0,    94,    95,     0,    96,     0,     0,    97,
       0,    98,     0,    99,     4,   100,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,   899,    37,     0,    38,    39,
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
       0,     0,     0,    93,     0,     0,    94,    95,     0,    96,
       0,     0,    97,     0,    98,     0,    99,   756,   100,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   167,
       0,     0,    15,    16,     0,    17,     0,   168,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     169,     0,     0,     0,    32,   170,   171,     0,   172,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   173,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   174,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,   265,     0,     0,     0,    97,     0,   175,     0,    99,
       0,   176,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   167,     0,     0,    15,    16,     0,    17,     0,
     168,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   169,     0,     0,     0,    32,   170,   171,
       0,   172,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   173,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   174,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,   265,     0,     0,     0,    97,     0,
     175,     0,    99,     0,   176,   656,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   167,     0,     0,    15,
      16,     0,    17,     0,   168,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   169,     0,     0,
       0,    32,   170,   171,     0,   172,    37,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   173,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   174,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,   265,     0,
       0,     0,    97,     0,   175,     0,    99,     0,   176,   996,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     167,     0,     0,    15,    16,     0,    17,     0,   168,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   169,     0,     0,     0,    32,   170,   171,     0,   172,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   173,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   174,
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,     0,     0,     0,     0,    97,     0,   175,     0,
      99,     0,   176,   661,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   167,     0,     0,    15,    16,     0,
      17,     0,   168,     0,     0,    21,   212,    23,     0,     0,
       0,     0,     0,    28,     0,   169,     0,     0,     0,    32,
     170,   171,     0,   172,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,    54,    55,     0,    57,     0,     0,     0,     0,    60,
     173,     0,    62,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   174,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,    82,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,     0,     0,     0,     0,
      97,     0,   175,     0,    99,     0,   176,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   167,     0,     0,
      15,    16,     0,    17,     0,   168,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   169,     0,
       0,     0,    32,   170,   171,     0,   172,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   173,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   174,     0,    73,     0,
       0,    75,   269,   270,     0,    79,   306,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,   307,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,   265,
       0,     0,     0,    97,     0,   175,     0,    99,     0,   176,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     167,     0,     0,    15,    16,     0,    17,     0,   168,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   169,     0,     0,     0,    32,   170,   171,     0,   172,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   173,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   174,
       0,    73,     0,     0,    75,   269,   270,     0,    79,   306,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,   265,     0,     0,     0,    97,     0,   175,     0,
      99,   648,   176,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   167,     0,     0,    15,    16,     0,    17,
       0,   168,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   169,     0,     0,     0,    32,   170,
     171,     0,   172,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   173,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   174,     0,    73,     0,     0,    75,   269,   270,
       0,    79,   306,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,   265,     0,     0,     0,    97,
       0,   175,     0,    99,   650,   176,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   167,     0,     0,    15,
      16,     0,    17,     0,   168,     0,     0,    21,     0,   549,
       0,     0,     0,     0,     0,    28,     0,   169,     0,     0,
       0,    32,   170,   171,     0,   172,    37,   550,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   173,     0,   551,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   174,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,   552,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,     0,     0,
       0,     0,    97,     0,   175,     0,    99,     0,   176,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   167,
       0,     0,    15,    16,     0,    17,     0,   168,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     169,     0,     0,     0,    32,   170,   171,     0,   172,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   173,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   174,     0,
      73,     0,     0,    75,   269,   270,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,     0,   271,     0,     0,    97,     0,   175,     0,    99,
       0,   176,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   167,     0,     0,    15,    16,     0,    17,     0,
     168,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   169,     0,     0,     0,    32,   170,   171,
       0,   172,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   173,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   174,     0,    73,     0,     0,    75,   269,   270,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,   265,     0,     0,     0,    97,     0,
     175,     0,    99,     0,   176,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   167,     0,     0,    15,    16,
       0,    17,     0,   168,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   169,     0,     0,     0,
      32,   170,   171,     0,   172,    37,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,   758,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   173,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   174,     0,    73,     0,     0,    75,
     269,   270,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,     0,   788,     0,
     789,    97,     0,   175,     0,    99,     0,   176,   790,     0,
       0,     0,     0,   791,   219,   792,   793,     0,     0,     0,
       0,   794,     0,   259,     0,     0,     0,    22,    23,   222,
       0,     0,     0,     0,     0,   795,   260,     0,    30,   261,
     796,     0,   225,    35,     0,   797,     0,   798,    39,     0,
       0,  -229,     0,   799,     0,     0,     0,     0,     0,   800,
     801,     0,     0,     0,     0,     0,   230,     0,     0,     0,
      52,     0,    54,     0,     0,   802,     0,  -229,     0,     0,
       0,   262,     0,    62,   232,   233,     0,   803,     0,   235,
       0,   804,     0,     0,   805,     0,     0,     0,   806,     0,
       0,   239,    78,     0,   807,    80,     0,   -63,    82,     0,
       0,     0,     0,   360,   361,   362,   363,   364,   365,     0,
       0,   368,   369,   370,   371,     0,   373,   374,   808,   809,
     810,   811,   812,     0,     0,   813,     0,     0,     0,   814,
     815,   816,   817,   818,   819,   820,   821,   822,   823,   824,
       0,   825,     0,     0,   826,   827,   828,   829,     0,     0,
     830,     5,     6,     7,     8,   201,    10,    11,   202,     0,
       0,   167,     0,     0,    15,    16,     0,    17,     0,   168,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   169,     0,     0,     0,    32,   170,   171,     0,
     172,    37,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   203,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     174,     0,    73,     0,     0,    75,     0,     0,   204,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,     0,     0,     0,     0,    97,     0,   175,
       0,    99,     0,   176,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   167,     0,     0,    15,    16,     0,
      17,     0,   168,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   169,     0,     0,     0,    32,
     170,   171,     0,   172,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,   206,    55,     0,    57,     0,     0,     0,     0,    60,
     173,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   174,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,   207,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,     0,     0,     0,     0,
      97,     0,   175,     0,    99,     0,   176,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   167,     0,     0,
      15,    16,     0,    17,     0,   168,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   169,     0,
       0,     0,    32,   170,   171,     0,   172,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   173,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   174,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,   207,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,   265,
       0,     0,     0,    97,     0,   175,     0,    99,     0,   176,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     167,     0,     0,    15,    16,     0,    17,     0,   168,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   169,     0,     0,     0,    32,   170,   171,     0,   172,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   173,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   174,
       0,    73,     0,     0,    75,   269,   270,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,     0,     0,     0,     0,    97,     0,   175,     0,
      99,     0,   176,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   167,     0,     0,    15,    16,     0,    17,
       0,   168,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   169,     0,     0,     0,    32,   170,
     171,     0,   172,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   173,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   174,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,   265,     0,     0,     0,    97,
       0,   175,   646,    99,     0,   176,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   167,     0,     0,    15,
      16,     0,    17,     0,   168,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   169,     0,     0,
       0,    32,   170,   171,     0,   172,    37,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   173,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   174,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,   207,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,     0,     0,
       0,     0,    97,     0,   175,     0,    99,     0,   176,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   167,
       0,     0,    15,    16,     0,    17,     0,   168,     0,     0,
      21,     0,     0,     0,     0,     0,     0,   248,    28,     0,
     169,     0,     0,     0,    32,   170,   171,     0,   172,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   173,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   174,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,     0,     0,     0,     0,    97,     0,    98,     0,    99,
       0,   176,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   167,     0,     0,    15,    16,     0,    17,     0,
     168,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   169,     0,     0,     0,    32,   170,   171,
       0,   172,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   173,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   174,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,   265,     0,     0,     0,    97,     0,
     175,     0,    99,     0,   176,     5,     6,     7,     8,   201,
      10,    11,     0,     0,     0,   167,     0,     0,    15,    16,
       0,    17,     0,   168,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   169,     0,     0,     0,
      32,   170,   171,     0,   172,    37,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   203,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   174,     0,    73,     0,     0,    75,
       0,     0,   204,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,     0,     0,     0,
       0,    97,     0,   175,     0,    99,     0,   176,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   167,     0,
       0,    15,    16,     0,    17,     0,   168,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   169,
       0,     0,     0,    32,   170,   171,     0,   172,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   173,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   174,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
     521,     0,     0,     0,    97,     0,   175,     0,    99,     0,
     176,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   167,     0,     0,    15,    16,     0,    17,     0,   168,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   169,     0,     0,     0,    32,   170,   171,     0,
     172,    37,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   173,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     174,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,   531,     0,     0,     0,    97,     0,   175,
       0,    99,     0,   176,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   167,     0,     0,    15,    16,     0,
      17,     0,   168,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   622,     0,   169,     0,     0,     0,    32,
     170,   171,     0,   172,    37,     0,   623,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     173,     0,     0,     0,     0,     0,     0,     0,    67,   624,
      69,    70,    71,   625,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,   265,     0,     0,     0,
      97,     0,   175,     0,    99,     0,   948,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   167,     0,     0,
      15,    16,     0,    17,     0,   168,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   169,     0,
       0,     0,    32,   170,   171,     0,   172,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   173,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   174,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,   265,
       0,     0,     0,    97,     0,   175,     0,    99,     0,   948,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     167,     0,     0,    15,    16,     0,    17,     0,   168,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   169,     0,     0,     0,    32,   170,   171,     0,   931,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   173,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   174,
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,   265,     0,     0,     0,    97,     0,   175,     0,
      99,     0,   176,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   167,     0,     0,    15,    16,     0,    17,
       0,   168,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   169,     0,     0,     0,    32,   170,
     171,     0,   172,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   173,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   174,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,     0,     0,     0,     0,    97,
       0,   175,     0,    99,     0,   176,     5,     6,     7,     8,
     197,    10,    11,     0,     0,     0,   167,     0,     0,    15,
      16,     0,    17,     0,   168,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   169,     0,     0,
       0,    32,   170,   171,     0,   172,    37,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   198,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   174,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,     0,     0,
       0,     0,    97,     0,   175,     0,    99,     0,   176,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   167,
       0,     0,    15,    16,     0,    17,     0,   168,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     169,     0,     0,     0,    32,   170,   171,     0,   172,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   173,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   174,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,     0,     0,     0,     0,    97,     0,    98,     0,    99,
       0,   176,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   167,     0,     0,    15,    16,     0,    17,     0,
     168,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   169,     0,     0,     0,    32,   170,   171,
       0,   172,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   173,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   174,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,     0,     0,     0,     0,   595,     0,
     175,     0,    99,     0,   176,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   167,     0,     0,    15,    16,
       0,    17,     0,   168,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   622,     0,   169,     0,     0,     0,
      32,   170,   171,     0,   172,    37,     0,   623,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   173,     0,     0,     0,     0,     0,     0,     0,    67,
     624,    69,    70,    71,   625,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,     0,     0,     0,
       0,    97,     0,   175,     0,    99,     0,   626,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   167,     0,
       0,    15,    16,     0,    17,     0,   168,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   169,
       0,     0,     0,    32,   170,   171,     0,   172,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   173,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   174,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
       0,     0,     0,     0,    97,     0,   175,     0,    99,     0,
     626,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   167,     0,     0,    15,    16,     0,    17,     0,   168,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   169,     0,     0,     0,    32,   170,   171,     0,
     696,    37,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   173,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     174,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,     0,     0,     0,     0,    97,     0,   175,
       0,    99,     0,   176,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   167,     0,     0,    15,    16,     0,
      17,     0,   168,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   169,     0,     0,     0,    32,
     170,   171,     0,   699,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     173,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   174,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,     0,     0,     0,     0,
      97,     0,   175,     0,    99,     0,   176,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   167,     0,     0,
      15,    16,     0,    17,     0,   168,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   169,     0,
       0,     0,    32,   170,   171,     0,   925,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   173,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   174,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,     0,
       0,     0,     0,    97,     0,   175,     0,    99,     0,   176,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     167,     0,     0,    15,    16,     0,    17,     0,   168,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   169,     0,     0,     0,    32,   170,   171,     0,   926,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   173,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   174,
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,     0,     0,     0,     0,    97,     0,   175,     0,
      99,     0,   176,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   167,     0,     0,    15,    16,     0,    17,
       0,   168,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   169,     0,     0,     0,    32,   170,
     171,     0,   928,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   173,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   174,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,     0,     0,     0,     0,    97,
       0,   175,     0,    99,     0,   176,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   167,     0,     0,    15,
      16,     0,    17,     0,   168,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   169,     0,     0,
       0,    32,   170,   171,     0,   929,    37,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   173,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   174,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,     0,     0,
       0,     0,    97,     0,   175,     0,    99,     0,   176,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   167,
       0,     0,    15,    16,     0,    17,     0,   168,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     169,     0,     0,     0,    32,   170,   171,     0,   930,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   173,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   174,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,     0,     0,     0,     0,    97,     0,   175,     0,    99,
       0,   176,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   167,     0,     0,    15,    16,     0,    17,     0,
     168,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   169,     0,     0,     0,    32,   170,   171,
       0,   931,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   173,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   174,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,     0,     0,     0,     0,    97,     0,
     175,     0,    99,     0,   176,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   167,     0,     0,    15,    16,
       0,    17,     0,   168,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   622,     0,   169,     0,     0,     0,
      32,   170,   171,     0,   172,    37,     0,   623,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   173,     0,     0,     0,     0,     0,     0,     0,    67,
     624,    69,    70,    71,   625,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,     0,     0,     0,
       0,    97,     0,   175,     0,    99,     0,  1015,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   167,     0,
       0,    15,    16,     0,    17,     0,   168,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   169,
       0,     0,     0,    32,   170,   171,     0,   172,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   173,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   174,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
       0,     0,     0,     0,    97,     0,     0,     0,    99,     0,
    1015,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   167,     0,     0,    15,    16,     0,    17,     0,   168,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   169,     0,     0,     0,    32,   170,   171,     0,
     172,    37,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   173,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     174,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   167,    93,     0,
      15,    16,    95,    17,     0,   168,     0,    97,    21,   175,
       0,    99,     0,   176,     0,     0,    28,     0,   169,     0,
       0,     0,    32,   170,   171,     0,   172,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   173,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   174,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,     0,
    -395,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,  -395,     0,     0,     0,  -395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   265,
       0,     0,     0,     0,     0,   175,     0,    99,  -395,   948,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     167,     0,     0,    15,    16,     0,    17,     0,   168,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   169,     0,     0,     0,    32,   170,   171,     0,   172,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,  -377,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   173,     0,     0,     0,
    -377,     0,     0,     0,    67,    68,    69,    70,    71,   174,
       0,    73,     0,  -377,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,  -377,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   167,     0,     0,    15,    16,     0,
      17,     0,   168,  -377,     0,    21,     0,     0,  -377,     0,
      99,     0,  1015,    28,     0,   169,     0,     0,     0,    32,
     170,   171,     0,   172,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     173,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   174,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   167,     0,     0,    15,
      16,     0,    17,     0,   168,     0,     0,    21,     0,     0,
       0,     0,   175,     0,    99,    28,   176,   169,     0,     0,
       0,    32,   170,   171,     0,   172,    37,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   173,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   174,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   167,     0,
       0,    15,    16,     0,    17,     0,   168,     0,     0,    21,
       0,     0,     0,     0,   175,     0,    99,    28,   626,   169,
       0,     0,     0,    32,   170,   171,     0,   172,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   173,     0,     0,   217,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   174,     0,    73,
     218,   219,    75,   220,     0,     0,    79,     0,   221,    81,
       0,     0,    83,     0,     0,     0,   222,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   224,     0,   225,
       0,     0,   226,     0,     0,     0,     0,     0,    89,     0,
     227,     0,     0,     0,     0,     0,   228,   229,     0,     0,
       0,     0,     0,   230,     0,     0,     0,     0,     0,     0,
       0,     0,   231,     0,     0,     0,   217,     0,    99,     0,
     176,   232,   233,     0,   234,     0,   235,     0,   236,   218,
     219,   237,   220,     0,     0,   238,     0,   221,   239,     0,
       0,   240,     0,     0,     0,   222,     0,     0,     0,     0,
       0,   223,     0,     0,     0,     0,   224,     0,   225,     0,
       0,   226,     0,     0,     0,     0,     0,     0,     0,   227,
       0,     0,     0,   217,     0,   228,   229,     0,     0,     0,
       0,     0,   230,     0,     0,     0,   218,   219,     0,   220,
       0,   231,     0,     0,   221,   388,     0,   766,     0,     0,
     232,   233,   222,   234,     0,   235,     0,   236,   223,     0,
     237,     0,     0,   224,   238,   225,     0,   239,   226,     0,
     240,     0,     0,     0,     0,     0,   227,     0,     0,     0,
       0,     0,   228,   229,     0,     0,     0,     0,     0,   230,
       0,     0,     0,     0,     0,     0,     0,     0,   231,     0,
       0,     0,     0,     0,     0,     0,     0,   232,   233,     0,
     234,     0,   235,     0,   236,   976,     0,   237,     0,     0,
       0,   238,     0,   790,   239,     0,   244,   240,   218,   219,
     792,   220,     0,     0,     0,     0,   221,     0,     0,     0,
       0,     0,     0,     0,   222,     0,     0,     0,     0,     0,
     795,     0,     0,     0,     0,   224,     0,   225,     0,     0,
     226,     0,   798,     0,     0,     0,     0,     0,   227,     0,
       0,     0,     0,     0,   800,   229,     0,     0,     0,   331,
       0,   230,     0,   746,     0,     0,   332,     0,     0,     0,
     231,     0,     0,     0,     0,     0,     0,     0,   333,   232,
     233,     0,   234,     0,   235,     0,   977,     0,     0,   805,
       0,     0,     0,   238,     0,     0,   239,     0,     0,   240,
       0,     0,     0,     0,     0,     0,     0,     0,   360,   361,
     362,   363,   364,   365,     0,     0,   368,   369,   370,   371,
     333,   373,   374,   808,   809,   810,   811,   812,   334,     0,
     813,     0,   335,     0,   814,   815,   816,   817,   818,   819,
     820,   821,   822,   823,   824,     0,   825,   331,     0,   826,
     827,   828,   829,     0,   332,     0,     0,     0,     0,   419,
       0,     0,     0,   336,     0,     0,   333,     0,     0,     0,
     334,     0,     0,     0,   335,     0,     0,     0,     0,   337,
     338,     0,   339,   340,   341,     0,   342,   343,   344,     0,
     345,   346,   347,   348,   349,     0,   350,   351,   352,   353,
     354,   355,   356,     0,   331,   357,   358,   359,     0,     0,
       0,   332,     0,     0,   774,     0,   334,     0,     0,     0,
     335,   337,   338,   333,   339,   340,   341,     0,   342,   343,
     344,     0,   345,   346,   347,   348,     0,     0,   350,   351,
     352,   353,   354,     0,   356,   331,     0,   357,   358,   359,
       0,   336,   332,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   333,     0,     0,   337,   338,     0,
     339,   340,   341,   334,   342,   343,   344,   335,   345,   346,
     347,   348,   349,     0,   350,   351,   352,   353,   354,   355,
     356,   331,     0,   357,   358,   359,     0,     0,   332,     0,
       0,     0,   850,     0,     0,     0,     0,     0,   336,     0,
     333,     0,     0,     0,   334,     0,     0,     0,   335,     0,
       0,     0,     0,     0,   337,   338,     0,   339,   340,   341,
       0,   342,   343,   344,     0,   345,   346,   347,   348,   349,
       0,   350,   351,   352,   353,   354,   355,   356,   331,   336,
     357,   358,   359,     0,     0,   332,     0,     0,     0,  1011,
     334,     0,     0,     0,   335,   337,   338,   333,   339,   340,
     341,     0,   342,   343,   344,     0,   345,   346,   347,   348,
     349,     0,   350,   351,   352,   353,   354,   355,   356,     0,
       0,   357,   358,   359,     0,   336,     0,     0,     0,     0,
    1078,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   337,   338,     0,   339,   340,   341,   334,   342,   343,
     344,   335,   345,   346,   347,   348,   349,     0,   350,   351,
     352,   353,   354,   355,   356,   331,     0,   357,   358,   359,
       0,     0,   332,     0,   647,     0,     0,     0,     0,     0,
       0,     0,   336,     0,   333,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   337,   338,
       0,   339,   340,   341,     0,   342,   343,   344,     0,   345,
     346,   347,   348,   349,     0,   350,   351,   352,   353,   354,
     355,   356,     0,     0,   357,   358,   359,     0,     0,     0,
       0,   975,     0,     0,   334,     0,     0,     0,   335,     0,
       0,   331,     0,     0,     0,     0,     0,   429,   332,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     333,     0,     0,     0,     0,     0,     0,     0,     0,   336,
     360,   361,   362,   363,   364,   365,     0,     0,   368,   369,
     370,   371,     0,   373,   374,   337,   338,     0,   339,   340,
     341,     0,   342,   343,   344,     0,   345,   346,   347,   348,
     349,     0,   350,   351,   352,   353,   354,   355,   356,     0,
     334,   357,   358,   359,   335,   388,     0,   331,     0,     0,
       0,     0,     0,   858,   332,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   333,     0,     0,     0,
       0,     0,     0,     0,     0,   336,   360,   361,   362,   363,
     364,   365,     0,     0,   368,   369,   370,   371,     0,   373,
     374,   337,   338,     0,   339,   340,   341,     0,   342,   343,
     344,     0,   345,   346,   347,   348,   349,     0,   350,   351,
     352,   353,   354,   355,   356,     0,   334,   357,   358,   359,
     335,   388,     0,   331,     0,     0,     0,     0,     0,   865,
     332,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   333,     0,     0,     0,     0,     0,     0,     0,
       0,   336,   360,   361,   362,   363,   364,   365,     0,     0,
     368,   369,   370,   371,     0,   373,   374,   337,   338,     0,
     339,   340,   341,     0,   342,   343,   344,     0,   345,   346,
     347,   348,   349,     0,   350,   351,   352,   353,   354,   355,
     356,     0,   334,   357,   358,   359,   335,   388,     0,   331,
       0,     0,     0,     0,     0,   988,   332,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   333,     0,
       0,     0,     0,     0,     0,     0,     0,   336,   360,   361,
     362,   363,   364,   365,     0,     0,   368,   369,   370,   371,
       0,   373,   374,   337,   338,     0,   339,   340,   341,     0,
     342,   343,   344,     0,   345,   346,   347,   348,   349,     0,
     350,   351,   352,   353,   354,   355,   356,     0,   334,   357,
     358,   359,   335,   388,     0,   331,     0,     0,     0,     0,
       0,   989,   332,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   333,     0,     0,     0,     0,     0,
       0,     0,     0,   336,   360,   361,   362,   363,   364,   365,
       0,     0,   368,   369,   370,   371,     0,   373,   374,   337,
     338,     0,   339,   340,   341,     0,   342,   343,   344,     0,
     345,   346,   347,   348,   349,     0,   350,   351,   352,   353,
     354,   355,   356,     0,   334,   357,   358,   359,   335,   388,
       0,   331,     0,     0,     0,     0,     0,   990,   332,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     333,     0,     0,     0,     0,     0,     0,     0,     0,   336,
     360,   361,   362,   363,   364,   365,     0,     0,   368,   369,
     370,   371,     0,   373,   374,   337,   338,     0,   339,   340,
     341,     0,   342,   343,   344,     0,   345,   346,   347,   348,
     349,     0,   350,   351,   352,   353,   354,   355,   356,     0,
     334,   357,   358,   359,   335,   388,     0,   331,     0,     0,
       0,     0,     0,   991,   332,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   333,     0,     0,     0,
       0,     0,     0,     0,     0,   336,   360,   361,   362,   363,
     364,   365,     0,     0,   368,   369,   370,   371,     0,   373,
     374,   337,   338,     0,   339,   340,   341,     0,   342,   343,
     344,     0,   345,   346,   347,   348,   349,     0,   350,   351,
     352,   353,   354,   355,   356,     0,   334,   357,   358,   359,
     335,   388,     0,   331,     0,     0,     0,     0,     0,  1013,
     332,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   333,     0,     0,     0,     0,     0,     0,     0,
       0,   336,   360,   361,   362,   363,   364,   365,     0,     0,
     368,   369,   370,   371,     0,   373,   374,   337,   338,     0,
     339,   340,   341,     0,   342,   343,   344,     0,   345,   346,
     347,   348,   349,     0,   350,   351,   352,   353,   354,   355,
     356,     0,   334,   357,   358,   359,   335,   388,     0,     0,
       0,     0,     0,     0,     0,  1014,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   336,   360,   361,
     362,   363,   364,   365,     0,     0,   368,   369,   370,   371,
       0,   373,   374,   337,   338,     0,   339,   340,   341,     0,
     342,   343,   344,     0,   345,   346,   347,   348,   349,     0,
     350,   351,   352,   353,   354,   355,   356,   331,     0,   357,
     358,   359,     0,   388,   332,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   333,   422,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   331,
     423,     0,     0,     0,     0,     0,   332,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   333,   416,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   417,     0,     0,     0,   334,     0,     0,     0,
     335,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   331,     0,     0,     0,
       0,     0,     0,   332,     0,     0,     0,   419,   334,     0,
       0,   336,   335,     0,     0,   333,   729,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   337,   338,     0,
     339,   340,   341,     0,   342,   343,   344,     0,   345,   346,
     347,   348,   349,   336,   350,   351,   352,   353,   354,   355,
     356,     0,     0,   357,   358,   359,     0,   388,     0,   337,
     338,     0,   339,   340,   341,   334,   342,   343,   344,   335,
     345,   346,   347,   348,   349,     0,   350,   351,   352,   353,
     354,   355,   356,   331,     0,   357,   358,   359,     0,   388,
     332,     0,     0,     0,     0,     0,   419,     0,     0,     0,
     336,     0,   333,   248,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   337,   338,     0,   339,
     340,   341,     0,   342,   343,   344,     0,   345,   346,   347,
     348,   349,     0,   350,   351,   352,   353,   354,   355,   356,
     331,     0,   357,   358,   359,     0,   388,   332,     0,     0,
       0,     0,   334,     0,     0,     0,   335,     0,     0,   333,
     721,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   331,     0,     0,     0,     0,     0,     0,   332,
       0,     0,     0,     0,     0,     0,     0,   336,     0,     0,
       0,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   337,   338,     0,   339,   340,   341,   334,
     342,   343,   344,   335,   345,   346,   347,   348,   349,     0,
     350,   351,   352,   353,   354,   355,   356,     0,     0,   357,
     358,   359,     0,   388,     0,     0,     0,     0,     0,     0,
       0,   334,     0,     0,   336,   335,     0,     0,     0,     0,
       0,     0,     0,     0,   739,     0,     0,     0,     0,     0,
     337,   338,     0,   339,   340,   341,     0,   342,   343,   344,
       0,   345,   346,   347,   348,   349,   336,   350,   351,   352,
     353,   354,   355,   356,     0,     0,   357,   358,   359,     0,
     388,     0,   337,   338,     0,   339,   340,   341,     0,   342,
     343,   344,     0,   345,   346,   347,   348,   349,     0,   350,
     351,   352,   353,   354,   355,   356,   331,     0,   357,   358,
     359,     0,   388,   332,  -210,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   333,     0,  -210,  -210,     0,
    -210,     0,     0,     0,     0,  -210,     0,     0,   331,     0,
       0,     0,     0,  -210,     0,   332,     0,     0,     0,  -210,
       0,     0,     0,     0,  -210,     0,  -210,   333,     0,  -210,
       0,     0,   426,     0,     0,     0,     0,  -210,     0,     0,
       0,     0,     0,  -210,  -210,   334,     0,     0,     0,   335,
    -210,     0,     0,     0,     0,     0,     0,     0,     0,  -210,
       0,     0,   427,     0,     0,   331,     0,     0,  -210,  -210,
       0,  -210,   332,  -210,     0,  -210,     0,   334,  -210,     0,
     336,   335,  -210,     0,   333,  -210,     0,     0,  -210,     0,
       0,     0,     0,     0,     0,     0,   337,   338,   390,   339,
     340,   341,     0,   342,   343,   344,     0,   345,   346,   347,
     348,   349,   336,   350,   351,   352,   353,   354,   355,   356,
       0,     0,   357,   358,   359,     0,   456,     0,   337,   338,
       0,   339,   340,   341,   334,   342,   343,   344,   335,   345,
     346,   347,   348,   349,     0,   350,   351,   352,   353,   354,
     355,   356,   331,   428,   357,   358,   359,     0,     0,   332,
       0,     0,     0,     0,     0,   162,     0,     0,     0,   336,
       0,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   337,   338,     0,   339,   340,
     341,     0,   342,   343,   344,     0,   345,   346,   347,   348,
     349,     0,   350,   351,   352,   353,   354,   355,   356,   331,
       0,   357,   358,   359,     0,     0,   332,     0,     0,     0,
       0,   334,     0,     0,     0,   335,     0,     0,   333,   558,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   331,   559,     0,     0,     0,     0,     0,   332,     0,
       0,     0,     0,     0,     0,     0,   336,     0,     0,     0,
     333,   560,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   337,   338,   561,   339,   340,   341,   334,   342,
     343,   344,   335,   345,   346,   347,   348,   349,     0,   350,
     351,   352,   353,   354,   355,   356,     0,   460,   357,   358,
     359,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     334,     0,     0,   336,   335,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   337,
     338,     0,   339,   340,   341,     0,   342,   343,   344,     0,
     345,   346,   347,   348,   349,   336,   350,   351,   352,   353,
     354,   355,   356,     0,     0,   357,   358,   359,     0,     0,
       0,   337,   338,     0,   339,   340,   341,     0,   342,   343,
     344,     0,   345,   346,   347,   348,   349,     0,   350,   351,
     352,   353,   354,   355,   356,   331,   259,   357,   358,   359,
      22,    23,   332,     0,     0,     0,     0,     0,     0,   260,
       0,    30,   261,     0,   333,     0,    35,     0,     0,   667,
       0,    39,     0,     0,  -229,     0,     0,   331,     0,     0,
       0,     0,     0,     0,   332,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,    54,   333,     0,     0,   668,
    -229,     0,     0,     0,   262,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   334,     0,     0,     0,   335,     0,
       0,     0,     0,     0,     0,    78,   331,     0,    80,     0,
     -62,    82,     0,   332,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   333,   334,     0,     0,   336,
     335,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   337,   338,     0,   339,   340,
     341,     0,   342,   343,   344,     0,   345,   346,   347,   348,
     349,   336,   350,   351,   352,   353,   354,   355,   356,     0,
       0,   357,   358,   359,     0,   334,     0,   337,   338,   335,
     339,   340,   341,     0,   342,   343,   344,     0,   345,   346,
     347,   348,   349,     0,   350,   351,   352,   353,   354,   355,
     356,   331,   462,   357,   358,   359,     0,     0,   332,     0,
     336,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     333,     0,     0,     0,     0,     0,   337,   338,     0,   339,
     340,   341,     0,   342,   343,   344,     0,   345,   346,   347,
     348,   349,     0,   350,   351,   352,   353,   354,   355,   356,
     331,   464,   357,   358,   359,     0,     0,   332,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   333,
     334,     0,     0,     0,   335,     0,     0,     0,     0,     0,
       0,     0,   331,     0,     0,     0,     0,     0,     0,   332,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   333,     0,     0,     0,   336,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   334,
       0,   337,   338,   335,   339,   340,   341,     0,   342,   343,
     344,     0,   345,   346,   347,   348,   349,     0,   350,   351,
     352,   353,   354,   355,   356,     0,   469,   357,   358,   359,
       0,   334,     0,   486,   336,   335,     0,     0,     0,     0,
       0,     0,     0,     0,   562,     0,     0,     0,     0,     0,
     337,   338,     0,   339,   340,   341,     0,   342,   343,   344,
       0,   345,   346,   347,   348,   349,   336,   350,   351,   352,
     353,   354,   355,   356,     0,     0,   357,   358,   359,     0,
       0,     0,   337,   338,     0,   339,   340,   341,     0,   342,
     343,   344,     0,   345,   346,   347,   348,   349,     0,   350,
     351,   352,   353,   354,   355,   356,   331,     0,   357,   358,
     359,     0,     0,   332,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   333,     0,   259,     0,     0,
       0,    22,    23,     0,     0,     0,     0,     0,     0,     0,
     260,     0,    30,   261,     0,     0,     0,    35,     0,     0,
       0,   331,    39,     0,     0,     0,     0,     0,   332,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     333,     0,     0,     0,    52,   334,    54,     0,    56,   335,
     907,     0,     0,   908,     0,   262,     0,    62,     0,     0,
       0,     0,   331,     0,     0,     0,     0,     0,     0,   332,
       0,     0,     0,     0,     0,     0,    78,     0,     0,    80,
     336,   333,    82,     0,     0,     0,     0,     0,     0,     0,
     334,     0,     0,     0,   335,     0,   337,   338,     0,   339,
     340,   341,     0,   342,   343,   344,     0,   345,   346,   347,
     348,   349,  -499,   350,   351,   352,   353,   354,   355,   356,
       0,   162,   357,   358,   359,   336,     0,     0,     0,     0,
       0,   334,    96,     0,     0,   335,     0,     0,   986,     0,
       0,   337,   338,     0,   339,   340,   341,     0,   342,   343,
     344,     0,   345,   346,   347,   348,   349,     0,   350,   351,
     352,   353,   354,   355,   356,     0,   336,   357,   358,   359,
       0,     0,     0,   331,   734,     0,     0,     0,     0,     0,
     332,     0,   337,   338,     0,   339,   340,   341,     0,   342,
     343,   344,   333,   345,   346,   347,   348,   349,     0,   350,
     351,   352,   353,   354,   355,   356,     0,   706,   357,   358,
     359,     0,     0,   331,     0,     0,     0,     0,     0,     0,
     332,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   333,     0,     0,   737,     0,     0,     0,     0,
       0,     0,   334,     0,     0,     0,   335,     0,     0,     0,
       0,     0,     0,     0,   331,     0,     0,     0,     0,     0,
       0,   332,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   333,     0,     0,     0,   336,     0,     0,
       0,     0,   334,     0,     0,     0,   335,     0,     0,     0,
       0,     0,     0,   337,   338,     0,   339,   340,   341,     0,
     342,   343,   344,     0,   345,   346,   347,   348,   349,     0,
     350,   351,   352,   353,   354,   355,   356,   336,     0,   357,
     358,   359,     0,   334,     0,     0,     0,   335,     0,     0,
       0,     0,     0,   337,   338,     0,   339,   340,   341,     0,
     342,   343,   344,     0,   345,   346,   347,   348,   349,     0,
     350,   351,   352,   353,   354,   355,   356,   777,   336,   357,
     358,   359,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   337,   338,     0,   339,   340,   341,
       0,   342,   343,   344,     0,   345,   346,   347,   348,   349,
       0,   350,   351,   352,   353,   354,   355,   356,   331,     0,
     357,   358,   359,     0,   671,   332,   217,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   333,   845,   218,
     219,     0,   220,     0,     0,     0,     0,   221,     0,     0,
     331,     0,     0,     0,     0,   222,     0,   332,     0,     0,
       0,   223,     0,     0,     0,     0,   224,     0,   225,   333,
     848,   226,     0,     0,     0,     0,     0,     0,     0,   227,
       0,     0,     0,     0,     0,   228,   229,   334,     0,     0,
       0,   335,   230,     0,     0,     0,     0,     0,     0,     0,
       0,   231,     0,     0,     0,     0,     0,     0,     0,     0,
     232,   233,     0,   234,     0,   235,     0,   236,     0,   334,
     237,     0,   336,   335,   238,     0,     0,   239,     0,     0,
     240,     0,     0,     0,     0,     0,     0,     0,   337,   338,
       0,   339,   340,   341,     0,   342,   343,   344,     0,   345,
     346,   347,   348,   349,   336,   350,   351,   352,   353,   354,
     355,   356,     0,     0,   357,   358,   359,     0,     0,     0,
     337,   338,     0,   339,   340,   341,     0,   342,   343,   344,
       0,   345,   346,   347,   348,   349,     0,   350,   351,   352,
     353,   354,   355,   356,   331,     0,   357,   358,   359,     0,
       0,   332,  -220,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   333,     0,  -220,  -220,     0,  -220,     0,
       0,     0,     0,  -220,     0,     0,   331,     0,     0,     0,
       0,  -220,     0,   332,     0,     0,     0,  -220,     0,     0,
       0,     0,  -220,     0,  -220,   333,     0,  -220,     0,     0,
       0,     0,     0,     0,     0,  -220,     0,     0,     0,     0,
       0,  -220,  -220,   334,     0,     0,     0,   335,  -220,     0,
       0,     0,     0,     0,     0,     0,   924,  -220,     0,     0,
       0,     0,     0,   331,   943,     0,  -220,  -220,     0,  -220,
     332,  -220,     0,  -220,     0,   334,  -220,     0,   336,   335,
    -220,     0,   333,  -220,     0,     0,  -220,     0,   927,     0,
       0,     0,     0,     0,   337,   338,     0,   339,   340,   341,
       0,   342,   343,   344,     0,   345,   346,   347,   348,   349,
     336,   350,   351,   352,   353,   354,   355,   356,     0,     0,
     357,   358,   359,     0,     0,     0,   337,   338,     0,   339,
     340,   341,   334,   342,   343,   344,   335,   345,   346,   347,
     348,   349,     0,   350,   351,   352,   353,   354,   355,   356,
     331,     0,   357,   358,   359,     0,     0,   332,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   336,     0,   333,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   337,   338,     0,   339,   340,   341,     0,
     342,   343,   344,     0,   345,   346,   347,   348,   349,     0,
     350,   351,   352,   353,   354,   355,   356,     0,     0,   357,
     358,   359,     0,     0,   333,     0,     0,     0,     0,   334,
       0,     0,     0,   335,     0,     0,   331,     0,     0,     0,
       0,     0,  1024,   332,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   333,     0,     0,     0,     0,
       0,     0,     0,     0,   336,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   334,     0,     0,     0,   335,     0,
     337,   338,     0,   339,   340,   341,     0,   342,   343,   344,
       0,   345,   346,   347,   348,   349,     0,   350,   351,   352,
     353,   354,   355,   356,     0,   334,   357,   358,   359,   335,
       0,     0,   331,     0,     0,     0,     0,     0,  1025,   332,
       0,     0,     0,     0,     0,   337,   338,     0,   339,   340,
     341,   333,   342,   343,   344,     0,     0,   346,   347,   348,
     336,     0,   350,   351,   352,   353,     0,     0,   356,     0,
       0,   357,   358,   359,     0,     0,   337,   338,     0,   339,
     340,   341,     0,   342,   343,   344,     0,   345,   346,   347,
     348,   349,     0,   350,   351,   352,   353,   354,   355,   356,
       0,   334,   357,   358,   359,   335,     0,     0,   331,     0,
       0,     0,     0,     0,  1026,   332,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   333,     0,     0,
       0,     0,     0,     0,     0,     0,   336,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   337,   338,     0,   339,   340,   341,     0,   342,
     343,   344,     0,   345,   346,   347,   348,   349,     0,   350,
     351,   352,   353,   354,   355,   356,     0,   334,   357,   358,
     359,   335,     0,     0,   331,     0,     0,     0,     0,     0,
    1027,   332,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   333,     0,     0,     0,     0,     0,     0,
       0,     0,   336,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   337,   338,
       0,   339,   340,   341,     0,   342,   343,   344,     0,   345,
     346,   347,   348,   349,     0,   350,   351,   352,   353,   354,
     355,   356,     0,   334,   357,   358,   359,   335,     0,     0,
     331,     0,     0,     0,     0,     0,  1028,   332,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   333,
       0,     0,     0,     0,     0,     0,     0,     0,   336,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   337,   338,     0,   339,   340,   341,
       0,   342,   343,   344,     0,   345,   346,   347,   348,   349,
       0,   350,   351,   352,   353,   354,   355,   356,     0,   334,
     357,   358,   359,   335,     0,     0,   331,     0,     0,     0,
       0,     0,  1029,   332,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   333,     0,     0,     0,     0,
       0,     0,     0,     0,   336,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1051,
     337,   338,     0,   339,   340,   341,     0,   342,   343,   344,
       0,   345,   346,   347,   348,   349,     0,   350,   351,   352,
     353,   354,   355,   356,     0,   334,   357,   358,   359,   335,
       0,     0,   331,     0,     0,     0,     0,     0,     0,   332,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   333,     0,     0,     0,     0,     0,     0,     0,     0,
     336,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   337,   338,     0,   339,
     340,   341,     0,   342,   343,   344,     0,   345,   346,   347,
     348,   349,     0,   350,   351,   352,   353,   354,   355,   356,
       0,   334,   357,   358,   359,   335,     0,     0,   331,     0,
       0,     0,     0,     0,     0,   332,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   333,     0,     0,
       0,     0,     0,     0,     0,     0,   336,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   337,   338,     0,   339,   340,   341,     0,   342,
     343,   344,     0,   345,   346,   347,   348,   349,     0,   350,
     351,   352,   353,   354,   355,   356,     0,   694,   357,   358,
     359,   335,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   336,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   337,   338,
       0,   339,   340,   341,     0,   342,   343,   344,   333,   345,
     346,   347,   348,   349,     0,   350,   351,   352,   353,   354,
     355,   356,     0,   259,   357,   358,   359,    22,    23,     0,
       0,     0,     0,     0,     0,     0,   260,     0,    30,   261,
       0,     0,     0,    35,     0,     0,     0,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   334,     0,
       0,   333,   335,     0,     0,     0,     0,     0,     0,     0,
      52,     0,    54,     0,    56,     0,   907,     0,     0,   908,
       0,   262,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   336,     0,     0,     0,     0,     0,     0,
       0,     0,    78,     0,     0,    80,     0,     0,    82,   337,
     338,   334,   339,   340,   341,   335,   342,   343,   344,     0,
     345,   346,   347,   348,     0,     0,   350,   351,   352,   353,
     354,   355,   356,     0,     0,   357,   358,   359,     0,     0,
       0,     0,     0,     0,     0,     0,   336,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,     0,   337,   338,   987,   339,   340,   341,     0,   342,
     343,   344,     0,   345,   346,   347,   348,     0,   217,   350,
     351,   352,   353,   354,     0,   356,     0,     0,   357,   358,
     359,   218,   219,     0,   220,     0,     0,     0,     0,   221,
       0,     0,     0,     0,     0,     0,     0,   222,     0,     0,
       0,     0,     0,   223,     0,     0,     0,     0,   224,     0,
     225,     0,     0,   226,     0,     0,     0,     0,     0,     0,
       0,   227,     0,     0,     0,  -211,     0,   228,   229,     0,
       0,     0,     0,     0,   230,     0,     0,     0,  -211,  -211,
       0,  -211,     0,   231,     0,     0,  -211,     0,     0,     0,
       0,     0,   232,   233,  -211,   234,     0,   235,     0,   236,
    -211,     0,   237,     0,     0,  -211,   238,  -211,     0,   239,
    -211,     0,   240,     0,     0,     0,     0,     0,  -211,     0,
       0,     0,     0,     0,  -211,  -211,     0,     0,     0,     0,
       0,  -211,     0,     0,     0,     0,     0,     0,     0,     0,
    -211,     0,     0,     0,     0,     0,     0,     0,     0,  -211,
    -211,     0,  -211,     0,  -211,     0,  -211,     0,     0,  -211,
       0,     0,     0,  -211,     0,     0,  -211,     0,     0,  -211
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-981)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      12,   147,     2,   381,    16,    63,   465,   588,    20,    58,
      59,    19,   188,   427,    26,   134,   437,     1,    30,    31,
     755,    33,    34,    35,    36,   216,   680,    31,   204,    22,
       1,    44,    44,   405,    46,    47,    31,   409,   958,    51,
      31,    53,   100,    55,   211,   621,   196,    83,    31,     3,
      83,    63,    64,    65,    66,    67,    68,   881,    31,     1,
      72,  1041,    74,     0,    76,    77,   216,   258,    44,    81,
     189,    26,    84,    85,     1,    87,   711,    89,   137,   714,
      92,    93,    44,    95,   111,    64,    98,    99,   100,   280,
       1,   442,   781,   835,   836,    66,   137,   111,  1078,   878,
      66,     3,    44,   162,   996,   840,   434,   111,   258,    44,
     691,   163,   156,   165,   465,    70,   111,   693,   176,    81,
      99,   162,    44,   156,   137,   161,    81,    54,   161,    44,
     280,    44,   116,    33,    34,   111,    44,   108,  1058,   166,
      94,   287,   108,    54,     3,   312,   313,   882,   972,  1041,
     111,   106,   136,   137,   137,   167,    83,   161,   170,   171,
     172,   137,   174,   175,   176,   854,   161,   856,   947,    96,
     161,    49,    83,    81,   116,    54,   752,    81,   161,   187,
     164,    83,   109,  1056,   196,   109,   377,   378,   161,   163,
     166,   162,    94,   343,   136,   137,   162,    75,   109,   620,
     100,  1074,   137,   575,   216,   166,   578,   200,   137,   136,
     588,   946,   205,   137,   156,   137,     2,   116,   853,   212,
     855,   910,   137,   137,   137,   136,    12,   377,   378,   156,
     109,   166,  1002,   163,   161,   164,   978,   979,   137,    25,
     137,    27,   141,   932,   166,   934,   258,   137,   116,   261,
     161,   166,   166,   166,   668,    54,   137,   269,   270,   156,
     995,     7,   137,   321,   156,   164,   920,   458,   280,   137,
     162,   137,   137,   141,   164,  1045,  1046,  1047,  1048,  1049,
    1050,   137,   863,   164,    83,   161,    72,   137,   407,   164,
      76,    77,   116,   412,   156,   307,   164,    96,   164,   164,
     156,   163,   200,   165,   304,   156,    26,   205,   458,   321,
     109,   162,    98,   691,   164,    26,   102,   141,   330,   331,
     332,   333,   334,   335,   336,   746,   338,   339,   340,   341,
     342,   137,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   137,    44,
      70,    76,    77,   137,   366,   367,   414,   156,   164,    70,
     372,    81,   161,   375,   376,   377,   378,   379,   380,   381,
      81,    66,   156,   163,    91,   164,    26,   163,   390,   137,
     100,    76,    77,   395,   137,   397,   106,   366,   367,   100,
     137,    80,     3,   372,    30,    84,   375,   376,   156,    54,
     163,    76,   414,   156,   416,   417,   105,    99,    19,   156,
     422,   423,   156,   155,   426,   427,   744,   429,   137,   431,
      70,    32,   156,    80,   574,   592,   593,    84,    83,   888,
     137,    81,   444,   136,   334,   335,   139,   156,   496,   451,
     143,    96,     2,   131,    80,    56,   458,   156,    84,   156,
     100,   139,    12,   781,   109,   241,   476,   163,   478,   165,
     131,   473,   248,   147,   476,    25,   478,    27,   139,   645,
     482,   483,   484,   156,   486,   487,   163,   156,   165,   379,
     380,   163,   156,    94,   496,   863,   156,   654,   655,   100,
     390,   186,   492,   156,   163,   131,   165,   476,    54,   478,
     136,   156,   139,   139,   196,   161,   161,   143,   156,   110,
      79,    79,    72,   208,   209,   210,   211,   417,   304,   163,
     215,   163,   156,   423,   216,    30,   854,    83,   856,   116,
     881,   163,   318,   156,   680,     7,    58,   888,    98,   136,
      96,   161,   102,   156,   116,   164,   558,   559,   560,   561,
     562,   563,   564,   109,   137,    44,   251,   565,   566,   254,
     572,   156,   574,   116,   163,    33,   258,   137,   626,   116,
     582,   164,   584,   164,   164,    80,   588,    20,   590,    84,
     164,   276,   482,   137,   166,   166,   161,   915,   280,   142,
     163,   144,   145,   572,   156,   148,   149,   156,   156,   294,
     156,   156,   388,   163,   932,   161,   934,   156,   137,   166,
     161,   161,   624,   625,   626,   156,   164,   156,   630,   156,
      33,   972,   164,    26,   162,   137,   131,   132,   164,   162,
     416,   136,   116,   138,   139,    81,   422,   164,   143,    31,
       2,    44,    31,   429,    47,   996,   151,   166,   161,    33,
      12,   630,   157,   158,   159,   667,   668,    26,   161,   559,
    1011,   561,   136,    25,    67,    27,    81,    70,   166,    72,
     136,   166,   166,   156,   156,   162,   139,    46,    81,   691,
     139,   241,   694,   695,   696,   377,   378,   699,   248,   139,
    1041,   131,   163,   131,   480,   139,   391,   100,   484,    68,
     139,    70,    30,   139,   762,  1056,   492,   719,   139,   721,
      72,   139,    81,   139,   726,   131,   139,   729,   139,   131,
     732,   131,   734,  1074,    33,   737,   421,  1078,    44,   164,
     425,   139,   164,    96,   137,   131,    98,   106,    54,   166,
     102,   131,   137,   668,   304,    24,   667,   318,   288,   766,
     762,   920,    80,   784,   915,   744,    84,   787,   318,   451,
     772,   164,   774,  1058,   613,   777,   458,    83,   995,  1020,
    1051,   783,  1044,    35,   920,    30,   574,   445,    -1,    -1,
      -1,   473,    -1,    -1,   476,    -1,   478,    -1,    -1,    -1,
      -1,   483,    -1,   109,   694,   695,    -1,    -1,    -1,    -1,
     116,   163,    -1,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,    -1,   143,    -1,   603,   830,    -1,
      -1,   137,   150,   151,    -1,    -1,   154,    -1,   388,   157,
     158,   159,   844,   845,    -1,   847,   848,   849,   850,    -1,
     156,    -1,    -1,    -1,    -1,   161,   858,   859,   164,   861,
      -1,   863,    -1,   865,   866,    -1,   416,   869,   907,   908,
      -1,    -1,   422,    -1,    -1,    -1,    -1,   653,    -1,   429,
      -1,    -1,    -1,   659,    -1,   887,   888,    -1,    -1,   241,
     572,   893,   574,   895,    26,    -1,   248,   899,    -1,    -1,
     948,   586,   587,    -1,    -1,    -1,    -1,   592,   593,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    26,   604,
      -1,    -1,   924,   925,   926,   927,   928,   929,   930,   931,
     480,    -1,    -1,    -1,   484,    67,    44,    -1,    70,    47,
      72,   943,   492,   719,    -1,   721,   948,    -1,    -1,    81,
     726,   953,   304,   729,    30,   957,   641,    -1,    -1,    67,
     736,    -1,    70,   739,    72,    -1,   318,  1015,   100,    -1,
      -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   761,   988,   989,   990,   991,
      -1,    -1,   100,    -1,    -1,    -1,   772,    -1,   774,    -1,
      -1,    -1,    -1,    -1,    80,   555,    -1,    66,    84,  1011,
      -1,  1013,  1014,  1015,    -1,    -1,    -1,    -1,    -1,   704,
     705,    -1,  1024,  1025,  1026,  1027,  1028,  1029,    -1,    -1,
      -1,   716,    -1,   718,    -1,    -1,   388,    -1,  1040,  1041,
     725,  1043,    44,   728,    -1,    -1,   731,    -1,    -1,    -1,
      -1,    -1,    54,   603,    -1,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,   416,    -1,    -1,   143,    -1,    30,
     422,    -1,  1074,    -1,    -1,   151,  1078,   429,    -1,    -1,
      -1,    83,   858,   159,   769,    -1,    -1,     2,    44,   865,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    54,    -1,
      -1,   877,    -1,   653,    -1,    -1,    -1,   109,    -1,   659,
      25,    -1,    27,   889,   116,    -1,    -1,    -1,    -1,    80,
      -1,    -1,   898,    84,    -1,    -1,   902,    83,   480,    -1,
     680,    -1,   484,    -1,    -1,   137,    -1,    -1,    -1,    -1,
     492,    -1,    -1,    -1,   694,   695,    -1,    -1,    -1,   208,
     209,   210,   211,   109,   156,    -1,    -1,    72,    -1,   161,
     116,    -1,   164,    -1,    -1,    -1,    -1,    -1,   944,   719,
     131,   721,    -1,    -1,    -1,   136,   726,   138,   139,   729,
      -1,   137,   143,    98,    -1,    -1,   736,   102,    -1,   739,
     151,    -1,   251,    -1,    -1,   254,   157,   158,   159,   881,
     156,    -1,    -1,    -1,    -1,   161,   891,    -1,   164,    -1,
      -1,   761,   988,   989,   990,   991,    30,   276,    -1,    -1,
      -1,    -1,   772,    -1,   774,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   294,    -1,  1013,  1014,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
      -1,   603,   937,   938,    -1,    -1,   941,   942,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     2,    80,    -1,    -1,    -1,
      84,   953,    -1,   958,    -1,    12,    -1,    -1,    -1,   964,
      -1,   966,    -1,   968,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   653,    -1,    -1,    -1,    -1,    30,   659,   858,    -1,
      -1,    -1,    -1,    -1,   996,   865,    -1,   131,   132,    -1,
     134,   135,   136,    -1,   138,   139,   241,   877,    -1,   143,
      -1,    -1,   391,   248,    -1,    72,   150,   151,    -1,   889,
     154,    30,    -1,   157,   158,   159,    -1,    -1,   898,    -1,
      -1,    -1,   902,    -1,    -1,    -1,    80,    -1,    -1,  1041,
      84,    98,   421,    -1,    -1,   102,   425,   719,    -1,   721,
      -1,    -1,    -1,  1058,   726,    -1,    -1,   729,    -1,    -1,
      -1,    -1,    -1,    -1,   736,    -1,    -1,   739,    -1,   304,
      -1,    80,    -1,    -1,   944,    84,  1078,    -1,    -1,    -1,
      -1,    -1,    -1,   318,    -1,    -1,    -1,   131,   132,   761,
     134,   135,   136,    -1,   138,   139,    -1,    -1,    -1,   143,
     772,    -1,   774,    -1,    -1,    -1,   163,   151,    -1,    -1,
      -1,    -1,    -1,   157,   158,   159,    -1,    -1,   988,   989,
     990,   991,   131,   132,    -1,    -1,   135,   136,    -1,   138,
     139,     2,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    12,   151,  1013,  1014,    -1,    -1,    -1,   157,   158,
     159,    -1,    -1,   388,    25,    -1,    27,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,   416,    -1,    -1,   241,    -1,   858,   422,    -1,    -1,
      -1,   248,    -1,   865,   429,    -1,    -1,    -1,    -1,     3,
      -1,    72,    -1,    -1,    -1,   877,   160,   586,   587,   163,
      -1,   165,    16,   592,   593,    19,    -1,   889,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,   898,    98,    -1,    -1,
     902,   102,    -1,    -1,    -1,    -1,    41,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    48,   480,    -1,   304,    -1,   484,
      -1,    21,    56,    -1,    -1,    25,    26,   492,    -1,    63,
      -1,   318,    -1,    -1,    34,    -1,    36,    37,    -1,    -1,
      -1,    41,   944,    78,    78,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     2,    90,    -1,    -1,    -1,
      94,    -1,   163,    -1,    -1,    12,    -1,   101,    68,    -1,
      70,    -1,    72,   107,    74,    -1,    -1,    77,    25,    79,
      27,    81,    -1,    -1,    -1,    -1,   988,   989,   990,   991,
      -1,    -1,   127,    -1,   129,   704,   705,    -1,    -1,    -1,
     100,   388,    -1,   103,    -1,    -1,   106,   716,    -1,    -1,
      -1,  1013,  1014,    -1,     2,    -1,    -1,    -1,    -1,   728,
      -1,    -1,   731,    -1,    12,    72,    -1,    -1,    -1,   416,
      -1,    -1,    -1,    -1,    -1,   422,    -1,    25,   603,    27,
     241,    -1,   429,    -1,    -1,    -1,    -1,   248,    -1,    -1,
      -1,    98,    -1,   188,    -1,   102,   156,    -1,    -1,    -1,
      -1,    -1,   162,   198,    -1,    -1,    -1,    -1,   203,   204,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,   653,    -1,
      -1,    -1,    -1,   480,   659,    -1,    -1,   484,    -1,    -1,
      -1,    -1,    -1,   304,    -1,   492,    -1,    -1,    -1,   244,
      98,    -1,    -1,    -1,   102,    -1,   163,   318,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     265,    -1,    21,    -1,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    37,    -1,
      -1,    -1,    41,    -1,   719,    -1,   721,    46,    -1,    -1,
      -1,   726,    -1,    -1,   729,    -1,    -1,    -1,     2,    -1,
      -1,   736,    -1,    -1,   739,   163,    -1,    -1,    12,    68,
      -1,    70,   891,    72,    -1,    74,    -1,   388,    77,    -1,
      79,    25,    81,    27,   241,    -1,   761,    -1,    -1,    -1,
      -1,   248,    -1,    -1,    -1,    -1,    -1,   772,    -1,   774,
      -1,   100,    -1,    -1,   103,   416,   603,   106,    -1,    -1,
      -1,   422,    -1,    -1,    -1,    -1,    -1,    -1,   429,    21,
      -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    34,    -1,    36,    37,    -1,    -1,    -1,    41,
      -1,    -1,    -1,   241,    46,    -1,    -1,   304,    -1,    -1,
     248,    -1,    -1,    -1,    98,    -1,   653,   156,   102,   404,
      -1,   318,   659,   408,    -1,    -1,    68,    -1,    70,   480,
      72,    -1,    74,   484,    -1,    77,    -1,    79,    -1,    81,
      -1,   492,    -1,   858,    -1,    -1,    -1,    -1,    -1,    -1,
     865,    -1,   437,    -1,    -1,    -1,    -1,    -1,   100,    -1,
      -1,   103,   877,    -1,   106,    -1,   304,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   889,    -1,    -1,    -1,    -1,   163,
     318,    21,   719,   898,   721,    25,    26,   902,    -1,   726,
      -1,   388,   729,    -1,    34,    -1,    36,    37,    -1,   736,
      -1,    41,   739,    -1,    -1,    -1,    46,    -1,   493,    -1,
      -1,    -1,    -1,   498,   156,    -1,    -1,    -1,    -1,   416,
     162,    -1,    -1,    -1,   761,   422,    -1,    -1,    68,   944,
      70,    -1,   429,    -1,    -1,   772,    -1,   774,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     388,    -1,   603,    -1,    -1,    -1,    -1,   241,    -1,    -1,
     100,    -1,    -1,   103,   248,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,   988,   989,   990,   991,    -1,   416,    -1,
      -1,    -1,    -1,   480,   422,    -1,    -1,   484,    -1,    -1,
      -1,   429,    -1,    -1,    -1,   492,    -1,    -1,  1013,  1014,
      -1,    -1,   653,    -1,    -1,    -1,    -1,    -1,   659,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     304,   858,    -1,    -1,    -1,    -1,    -1,    -1,   865,    -1,
      -1,   616,    -1,    -1,   318,   620,    -1,    -1,    -1,    -1,
     877,    -1,   480,    -1,    -1,    -1,   484,    -1,    -1,    -1,
      -1,    -1,   889,    -1,   492,    -1,   641,    -1,    -1,    -1,
     645,   898,    -1,    -1,    -1,   902,    -1,    -1,   719,    -1,
     721,    -1,    -1,    -1,    -1,   726,    -1,    -1,   729,    -1,
      -1,    -1,    -1,    -1,     3,   736,    -1,    -1,   739,    -1,
      -1,    -1,    -1,    -1,    -1,   680,    -1,    16,    17,    -1,
      19,    -1,    -1,    -1,   388,    24,   603,   944,    -1,    -1,
     761,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,   772,    -1,   774,    43,    -1,    45,    -1,    -1,    48,
      -1,    -1,   416,    -1,    -1,    -1,    -1,    56,   422,    -1,
      -1,    -1,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      69,   988,   989,   990,   991,    -1,   653,    -1,   743,    78,
      -1,   746,   659,    -1,    -1,   603,    -1,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,  1013,  1014,    97,    -1,
      -1,   766,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   480,    -1,    -1,   784,
     484,    -1,    -1,    -1,    -1,    -1,    -1,   858,   492,    -1,
      -1,    -1,    -1,    -1,   865,   653,    -1,    -1,    -1,    -1,
      -1,   659,   719,    -1,   721,   810,   877,    -1,    -1,   726,
      -1,    -1,   729,    -1,    -1,    -1,    -1,    -1,   889,   736,
      -1,    -1,   739,    -1,   163,    -1,    -1,   898,    -1,    -1,
      -1,   902,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   761,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    25,    26,   772,    -1,   774,    -1,    -1,
      -1,   719,    34,   721,    36,    37,    -1,    -1,   726,    41,
      -1,   729,    -1,   944,    46,    -1,    -1,    -1,   736,    -1,
      -1,   739,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,   603,
      72,    -1,    74,   761,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,   772,   920,   774,   988,   989,   990,
     991,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,   858,  1013,  1014,    -1,    -1,    -1,    -1,   865,   653,
      -1,    -1,    -1,    -1,    -1,   659,    -1,    -1,    -1,    -1,
     877,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   889,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   898,    -1,    -1,   156,   902,    -1,    -1,    -1,    -1,
     162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     858,    -1,    -1,    -1,    -1,    -1,    -1,   865,    -1,    -1,
      -1,    -1,    -1,    -1,     1,   719,    -1,   721,    -1,   877,
      -1,    -1,   726,    -1,    -1,   729,    -1,   944,    -1,    -1,
      -1,   889,   736,    -1,    21,   739,    -1,    -1,    25,    26,
     898,    -1,    -1,    -1,   902,    -1,    -1,    34,    -1,    36,
      37,    -1,    -1,    -1,    41,    -1,    -1,   761,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,   772,    -1,
     774,   988,   989,   990,   991,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    70,    -1,    72,   944,    74,    75,    -1,
      77,    -1,    79,    -1,    81,    -1,  1013,  1014,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   100,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
     988,   989,   990,   991,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1013,  1014,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,    -1,   877,    -1,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   889,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   898,    -1,     0,     1,   902,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    -1,    13,
      14,    15,    16,    17,    -1,    19,    -1,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    31,    32,    -1,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
     944,    45,    46,    -1,    48,    49,    -1,    51,    52,    53,
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
     154,    -1,   156,    -1,    -1,   159,    -1,   161,    -1,   163,
       1,   165,     3,     4,     5,     6,     7,     8,     9,    -1,
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
     161,    -1,   163,    -1,   165,   166,     1,    -1,     3,     4,
       5,     6,     7,     8,     9,    -1,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      45,    46,    -1,    48,    49,    -1,    51,    52,    53,    -1,
      55,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      -1,    -1,    -1,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,   101,    -1,   103,   104,
     105,   106,   107,    -1,    -1,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,    -1,   161,   162,   163,     1,
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
      -1,   150,    -1,    -1,   153,   154,    -1,   156,    -1,    -1,
     159,    -1,   161,   162,   163,     1,   165,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    14,    15,
      16,    17,    -1,    19,    -1,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,    31,    32,    -1,    34,    -1,
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
      -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,    -1,
     156,    -1,    -1,   159,    -1,   161,   162,   163,     1,   165,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      13,    14,    15,    16,    17,    -1,    19,    -1,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,    31,    32,
      -1,    34,    -1,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    46,    -1,    48,    49,    -1,    51,    52,
      53,    -1,    55,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    -1,    -1,    -1,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    85,    86,    87,    88,    89,    90,    91,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,   101,    -1,
     103,   104,   105,   106,   107,    -1,    -1,   110,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,
     153,   154,    -1,   156,    -1,    -1,   159,    -1,   161,   162,
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
     150,    -1,    -1,   153,   154,    -1,   156,    -1,    -1,   159,
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
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,    -1,   156,
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
     154,    -1,   156,    -1,    -1,   159,    -1,   161,    -1,   163,
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
      -1,    -1,   153,   154,    -1,   156,    -1,    -1,   159,    -1,
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
      -1,    -1,   150,    -1,    -1,   153,   154,    -1,   156,    -1,
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
      -1,   156,    -1,    -1,   159,    -1,   161,    -1,   163,     1,
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
      -1,   153,   154,    -1,   156,    -1,    -1,   159,    -1,   161,
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
      -1,   150,    -1,    -1,   153,   154,    -1,   156,    -1,    -1,
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
      -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,    -1,
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
     153,   154,    -1,   156,    -1,    -1,   159,    -1,   161,    -1,
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
     150,    -1,    -1,   153,   154,    -1,   156,    -1,    -1,   159,
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
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,    -1,   156,
      -1,    -1,   159,    -1,   161,    -1,   163,     1,   165,     3,
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
     161,    -1,   163,    -1,   165,   166,     3,     4,     5,     6,
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
      -1,    -1,   159,    -1,   161,    -1,   163,    -1,   165,   166,
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
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,    -1,
     163,    -1,   165,   166,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,    -1,
      19,    -1,    21,    -1,    -1,    24,    25,    26,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    34,    -1,    -1,    -1,    38,
      39,    40,    -1,    42,    43,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    -1,    56,    57,    -1,
      59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    -1,    -1,    78,
      79,    -1,    81,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    91,    92,    -1,    94,    -1,    -1,    97,    -1,
      -1,    -1,   101,    -1,    -1,   104,    -1,   106,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,    -1,   153,   154,    -1,    -1,    -1,    -1,
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
      -1,    97,    98,    99,    -1,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,   140,   141,    -1,    -1,    -1,    -1,
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
      -1,    94,    -1,    -1,    97,    98,    99,    -1,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,
     153,   154,   155,    -1,    -1,    -1,   159,    -1,   161,    -1,
     163,   164,   165,     3,     4,     5,     6,     7,     8,     9,
      -1,    -1,    -1,    13,    -1,    -1,    16,    17,    -1,    19,
      -1,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    34,    -1,    -1,    -1,    38,    39,
      40,    -1,    42,    43,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    53,    -1,    -1,    56,    57,    -1,    59,
      60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    71,    -1,    73,    -1,    -1,    -1,    -1,    78,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,
      90,    91,    92,    -1,    94,    -1,    -1,    97,    98,    99,
      -1,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,    -1,    -1,   153,   154,   155,    -1,    -1,    -1,   159,
      -1,   161,    -1,   163,   164,   165,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,    16,
      17,    -1,    19,    -1,    21,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    34,    -1,    -1,
      -1,    38,    39,    40,    -1,    42,    43,    44,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    53,    -1,    -1,    56,
      57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    71,    -1,    73,    -1,    -1,    -1,
      -1,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      87,    88,    89,    90,    91,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,   106,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,    -1,    -1,
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
     154,    -1,   156,    -1,    -1,   159,    -1,   161,    -1,   163,
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
      -1,    -1,   153,   154,   155,    -1,    -1,    -1,   159,    -1,
     161,    -1,   163,    -1,   165,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,
      -1,    19,    -1,    21,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    34,    -1,    -1,    -1,
      38,    39,    40,    -1,    42,    43,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    53,    -1,    -1,    56,    57,
      -1,    59,    60,    61,    62,    63,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    71,    -1,    73,    -1,    -1,    -1,    -1,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    -1,    94,    -1,    -1,    97,
      98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,    -1,   153,   154,    -1,     1,    -1,
       3,   159,    -1,   161,    -1,   163,    -1,   165,    11,    -1,
      -1,    -1,    -1,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    24,    -1,    21,    -1,    -1,    -1,    25,    26,    32,
      -1,    -1,    -1,    -1,    -1,    38,    34,    -1,    36,    37,
      43,    -1,    45,    41,    -1,    48,    -1,    50,    46,    -1,
      -1,    49,    -1,    56,    -1,    -1,    -1,    -1,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,
      68,    -1,    70,    -1,    -1,    78,    -1,    75,    -1,    -1,
      -1,    79,    -1,    81,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,
      -1,   104,   100,    -1,   107,   103,    -1,   105,   106,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,   121,    -1,
      -1,   124,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,    -1,    -1,   138,    -1,    -1,    -1,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      -1,   154,    -1,    -1,   157,   158,   159,   160,    -1,    -1,
     163,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,
      -1,   163,    -1,   165,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,    -1,
      19,    -1,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    34,    -1,    -1,    -1,    38,
      39,    40,    -1,    42,    43,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    -1,    56,    57,    -1,
      59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    91,    92,    -1,    94,    -1,    -1,    97,    -1,
      -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,   113,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,    -1,   153,   154,    -1,    -1,    -1,    -1,
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
      -1,   107,    -1,    -1,    -1,    -1,    -1,   113,    -1,   115,
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
      -1,    94,    -1,    -1,    97,    98,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,    -1,
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
      -1,   161,   162,   163,    -1,   165,     3,     4,     5,     6,
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
     107,    -1,    -1,    -1,    -1,    -1,   113,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,    -1,   163,    -1,   165,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    -1,    13,
      -1,    -1,    16,    17,    -1,    19,    -1,    21,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
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
     154,    -1,    -1,    -1,    -1,   159,    -1,   161,    -1,   163,
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
      -1,    -1,   100,   101,    -1,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,    -1,   153,   154,    -1,    -1,    -1,
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
     150,    -1,    -1,   153,   154,    -1,    -1,    -1,    -1,   159,
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
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,    -1,    -1,
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
     154,    -1,    -1,    -1,    -1,   159,    -1,   161,    -1,   163,
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
      -1,    -1,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
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
      -1,    -1,   150,    -1,    -1,   153,   154,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   159,    -1,   161,    -1,   163,    -1,
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
      -1,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,
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
      -1,   150,    -1,    -1,   153,   154,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,    -1,
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
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,    -1,
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
     150,    -1,    -1,   153,   154,    -1,    -1,    -1,    -1,   159,
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
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,    -1,    -1,
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
     154,    -1,    -1,    -1,    -1,   159,    -1,   161,    -1,   163,
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
      -1,    -1,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
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
      -1,    -1,   150,    -1,    -1,   153,   154,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,   163,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,   133,   134,   135,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,   150,    -1,
      16,    17,   154,    19,    -1,    21,    -1,   159,    24,   161,
      -1,   163,    -1,   165,    -1,    -1,    32,    -1,    34,    -1,
      -1,    -1,    38,    39,    40,    -1,    42,    43,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    -1,
      56,    57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    71,    -1,    73,    -1,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    89,    90,    91,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,   137,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,    -1,    -1,    -1,   161,    -1,   163,   164,   165,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      13,    -1,    -1,    16,    17,    -1,    19,    -1,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    34,    -1,    -1,    -1,    38,    39,    40,    -1,    42,
      43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      53,    54,    -1,    56,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    -1,
      73,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   101,    -1,
      -1,   104,    -1,    -1,   107,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,    -1,
      19,    -1,    21,   156,    -1,    24,    -1,    -1,   161,    -1,
     163,    -1,   165,    32,    -1,    34,    -1,    -1,    -1,    38,
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
      17,    -1,    19,    -1,    21,    -1,    -1,    24,    -1,    -1,
      -1,    -1,   161,    -1,   163,    32,   165,    34,    -1,    -1,
      -1,    38,    39,    40,    -1,    42,    43,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    53,    -1,    -1,    56,
      57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    71,    -1,    73,    -1,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    89,    90,    91,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,
      -1,    16,    17,    -1,    19,    -1,    21,    -1,    -1,    24,
      -1,    -1,    -1,    -1,   161,    -1,   163,    32,   165,    34,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,
      -1,    56,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    71,    -1,    73,    -1,
      -1,    -1,    -1,    78,    79,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    91,    92,    -1,    94,
      16,    17,    97,    19,    -1,    -1,   101,    -1,    24,   104,
      -1,    -1,   107,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    62,    63,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,     3,    -1,   163,    -1,
     165,    87,    88,    -1,    90,    -1,    92,    -1,    94,    16,
      17,    97,    19,    -1,    -1,   101,    -1,    24,   104,    -1,
      -1,   107,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    -1,     3,    -1,    62,    63,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    16,    17,    -1,    19,
      -1,    78,    -1,    -1,    24,   161,    -1,   163,    -1,    -1,
      87,    88,    32,    90,    -1,    92,    -1,    94,    38,    -1,
      97,    -1,    -1,    43,   101,    45,    -1,   104,    48,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,     3,    -1,    97,    -1,    -1,
      -1,   101,    -1,    11,   104,    -1,   163,   107,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    45,    -1,    -1,
      48,    -1,    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    62,    63,    -1,    -1,    -1,    11,
      -1,    69,    -1,   163,    -1,    -1,    18,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    -1,    -1,    97,
      -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,   121,    -1,    -1,   124,   125,   126,   127,
      30,   129,   130,   131,   132,   133,   134,   135,    80,    -1,
     138,    -1,    84,    -1,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,   154,    11,    -1,   157,
     158,   159,   160,    -1,    18,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,   115,    -1,    -1,    30,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,   131,
     132,    -1,   134,   135,   136,    -1,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,    -1,    11,   157,   158,   159,    -1,    -1,
      -1,    18,    -1,    -1,   166,    -1,    80,    -1,    -1,    -1,
      84,   131,   132,    30,   134,   135,   136,    -1,   138,   139,
     140,    -1,   142,   143,   144,   145,    -1,    -1,   148,   149,
     150,   151,   152,    -1,   154,    11,    -1,   157,   158,   159,
      -1,   115,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,   131,   132,    -1,
     134,   135,   136,    80,   138,   139,   140,    84,   142,   143,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,    11,    -1,   157,   158,   159,    -1,    -1,    18,    -1,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      30,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    11,   115,
     157,   158,   159,    -1,    -1,    18,    -1,    -1,    -1,   166,
      80,    -1,    -1,    -1,    84,   131,   132,    30,   134,   135,
     136,    -1,   138,   139,   140,    -1,   142,   143,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,   157,   158,   159,    -1,   115,    -1,    -1,    -1,    -1,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,    -1,   134,   135,   136,    80,   138,   139,
     140,    84,   142,   143,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,    11,    -1,   157,   158,   159,
      -1,    -1,    18,    -1,   164,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
      -1,   134,   135,   136,    -1,   138,   139,   140,    -1,   142,
     143,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,    -1,    -1,   157,   158,   159,    -1,    -1,    -1,
      -1,   164,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,
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
     150,   151,   152,   153,   154,    11,    21,   157,   158,   159,
      25,    26,    18,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    37,    -1,    30,    -1,    41,    -1,    -1,    35,
      -1,    46,    -1,    -1,    49,    -1,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    70,    30,    -1,    -1,    65,
      75,    -1,    -1,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,   100,    11,    -1,   103,    -1,
     105,   106,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,
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
     158,   159,    -1,    -1,    30,    -1,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    84,    -1,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    93,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,
     131,   132,    -1,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,    -1,    80,   157,   158,   159,    84,
      -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    93,    18,
      -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,   135,
     136,    30,   138,   139,   140,    -1,    -1,   143,   144,   145,
     115,    -1,   148,   149,   150,   151,    -1,    -1,   154,    -1,
      -1,   157,   158,   159,    -1,    -1,   131,   132,    -1,   134,
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
       0,   171,   172,     0,     1,     3,     4,     5,     6,     7,
       8,     9,    13,    14,    15,    16,    17,    19,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    31,    32,    34,
      36,    37,    38,    39,    40,    41,    42,    43,    45,    46,
      48,    51,    52,    53,    55,    56,    57,    59,    60,    62,
      63,    64,    68,    69,    70,    71,    72,    73,    74,    77,
      78,    79,    81,    82,    83,    85,    86,    87,    88,    89,
      90,    91,    92,    94,    95,    97,    98,    99,   100,   101,
     103,   104,   106,   107,   110,   112,   115,   131,   132,   133,
     134,   135,   140,   150,   153,   154,   156,   159,   161,   163,
     165,   173,   174,   175,   176,   177,   178,   180,   185,   186,
     187,   188,   192,   194,   197,   198,   199,   200,   201,   202,
     203,   205,   206,   207,   211,   212,   215,   216,   219,   220,
     223,   226,   227,   253,   256,   257,   277,   278,   279,   280,
     281,   282,   283,   291,   292,   293,   294,   297,   298,   299,
     300,   301,   302,   303,   305,   306,   307,   308,   309,   156,
     175,   294,   111,   284,   285,     3,   189,    13,    21,    34,
      39,    40,    42,    79,    92,   161,   165,   223,   277,   282,
     292,   293,   294,   297,   299,   300,   284,   294,   100,   256,
      81,   189,   175,   271,   294,   175,   163,     7,    79,   294,
     295,     7,    10,    79,   100,   295,    70,   113,   204,   294,
     204,   294,    25,   257,   294,   294,   163,     3,    16,    17,
      19,    24,    32,    38,    43,    45,    48,    56,    62,    63,
      69,    78,    87,    88,    90,    92,    94,    97,   101,   104,
     107,   191,   193,   224,   163,   191,   258,   259,    31,   180,
     196,   294,   294,   294,   294,   294,   294,     7,   163,    21,
      34,    37,    79,   199,   199,   155,   246,   270,   294,    98,
      99,   156,   294,   296,   294,   196,   294,   294,   294,    91,
     163,   175,   294,   294,   180,   188,   294,   297,   180,   188,
     294,   191,   254,   294,   294,   294,   294,   294,   294,   294,
     294,     1,   162,   173,   181,   270,   102,   141,   246,   272,
     273,   296,   204,   270,   294,   304,    72,   175,   161,    76,
     179,   105,   191,   191,    49,    75,   241,   258,   155,   156,
     147,    11,    18,    30,    80,    84,   115,   131,   132,   134,
     135,   136,   138,   139,   140,   142,   143,   144,   145,   146,
     148,   149,   150,   151,   152,   153,   154,   157,   158,   159,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   160,   233,   163,   165,    80,
      84,   163,   175,   156,   294,   294,   294,   270,   161,   180,
      44,   284,   254,   258,   156,   137,   156,   110,   192,   246,
     274,   275,   276,   296,    79,   191,   227,   256,    79,   191,
     254,   227,   256,   191,   163,   196,    31,    44,   196,   111,
     196,   287,    31,    44,   196,   287,    35,    65,   156,    93,
     180,   233,   274,   175,   163,   235,   102,   163,   191,   260,
     261,     1,   136,   265,    44,   137,   175,   196,   196,   274,
     191,   137,   156,   294,   294,   156,   161,   196,   163,   274,
     156,   208,   156,   208,   156,   116,   255,   156,   196,   156,
     162,   162,   173,   137,   162,   294,   137,   164,   137,   164,
     166,   287,    44,   137,   166,   287,   114,   137,   166,     7,
       1,   162,   181,    58,   270,   294,   136,   217,   161,   228,
     156,   294,   294,   294,   294,   204,   294,   204,   294,   294,
     294,   294,   294,   294,   294,    19,    32,    56,   100,   192,
     294,   155,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   155,   294,   294,   294,   294,   294,   294,   296,   296,
     296,   296,   296,   274,   274,   204,   294,   204,   294,    26,
      44,    81,   106,   286,   289,   290,   294,   309,    31,    44,
      31,    44,    93,    44,   166,   204,   294,   196,   156,   156,
     294,   294,   116,   164,   137,   191,   217,   258,   191,   217,
     156,   258,    44,   270,    42,   294,   204,   294,   163,   196,
      42,   294,   204,   294,   196,   159,   182,   184,   294,   182,
     183,   175,   294,    33,   294,   164,    26,    44,    47,    67,
      70,    81,   100,   174,   236,   237,   238,   239,   225,   261,
     137,   164,    32,    45,    88,    92,   165,   195,   266,   278,
     116,   262,   294,   259,   164,   246,   294,     1,   213,   274,
     164,    20,   209,   266,   278,   137,   162,   164,   164,   272,
     164,   272,   175,   166,   204,   294,   166,   175,   294,   166,
     294,   166,   294,   162,   162,   191,   156,    35,    65,   270,
     161,     1,   191,   221,   222,    26,    70,    81,   100,   230,
     240,   163,   156,   156,   156,   156,   156,   164,   166,    44,
      81,   137,   164,   277,    80,    80,    42,   204,   294,    42,
     204,   294,   204,   294,   284,   284,   156,   246,   296,   276,
     217,   161,   156,   217,   161,   156,   294,   164,   294,    31,
     196,    31,   196,   288,   289,   294,    31,   196,   287,    31,
     196,   287,   137,   156,    12,   156,    33,    33,   175,    93,
     180,    44,    81,   238,   137,   164,   163,   191,    26,    70,
      81,   100,   242,   164,   261,   265,     1,   270,    61,   296,
     162,    66,   108,   162,   214,   164,   163,   180,   191,   210,
     254,   175,   166,   287,   166,   287,   175,   114,   184,   183,
       1,   218,   162,   116,   137,   162,    81,   229,     1,     3,
      11,    16,    18,    19,    24,    38,    43,    48,    50,    56,
      62,    63,    78,    90,    94,    97,   101,   107,   131,   132,
     133,   134,   135,   138,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   154,   157,   158,   159,   160,
     163,   190,   191,   193,   231,   232,   233,   277,   164,   289,
     265,   277,   277,   294,    31,    31,   294,    31,    31,   166,
     166,   196,   196,   161,   218,   161,   218,   196,    93,    42,
     294,    42,   294,   137,   164,    93,    42,   294,   196,    42,
     294,   196,   294,   294,   175,   294,   175,    33,   191,   237,
     261,   136,   269,    81,   265,   262,   166,    44,   166,    31,
     175,   270,   210,   136,   180,    42,   175,   294,   166,    42,
     175,   294,   166,   294,   156,   156,   162,    74,    77,   162,
     174,   198,   294,   222,   242,   163,   234,   191,   294,   131,
     139,   234,   234,   262,    93,    42,    42,    93,    42,    42,
      42,    42,   218,   162,   218,   162,   294,   294,   294,   289,
     294,   294,   294,    12,    33,   175,   269,   164,   165,   195,
     246,   268,   278,   141,   248,   262,    54,   109,   249,   294,
     266,   278,   175,   196,   164,   294,   294,   175,   294,   175,
     198,     1,   136,   264,   236,   164,     3,    94,   232,   233,
     294,   294,   294,   294,   294,   294,   162,   162,    93,    93,
      93,    93,   294,   175,   262,   269,   166,   270,   246,   247,
     294,     3,    83,    94,   250,   251,   252,   294,   180,   197,
     245,   166,   180,    93,    93,   165,   195,   263,   278,    96,
     243,   164,   234,   234,    93,    93,    93,    93,    93,    93,
     294,   294,   294,   294,   248,   262,   246,   267,   268,   278,
      44,   166,   252,   109,   137,   116,   142,   144,   145,   148,
     149,    54,   278,   294,   294,     1,   166,   270,   249,   294,
     267,   268,   294,   251,   252,   252,   252,   252,   252,   252,
     250,   166,   263,   278,   166,   156,   244,   245,   166,   263,
     278,   267
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
#line 491 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 496 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 497 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 509 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 529 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 530 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 532 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 533 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 534 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 535 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 540 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 542 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 543 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 544 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 549 "chapel.ypp"
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
#line 562 "chapel.ypp"
    { (yyvsp[(1) - (3)].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (3)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (3)]).prevModule;  // restore previous module name
    }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 567 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = (yyvsp[(3) - (4)].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 572 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 579 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 580 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 585 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 599 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 600 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 601 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 607 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 617 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 622 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 626 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 633 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 651 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 652 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 653 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 657 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (4)].pcallexpr), (yyvsp[(1) - (4)].b)); }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 658 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), (yyvsp[(5) - (6)].ponlylist), true, (yyvsp[(1) - (6)].b)); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 659 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), (yyvsp[(5) - (6)].ponlylist), false, (yyvsp[(1) - (6)].b)); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 663 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 668 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 670 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 672 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 674 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 676 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 682 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 683 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 687 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 690 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 698 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 712 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 713 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 724 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 725 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 726 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 727 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 728 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 729 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 730 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 731 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 732 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 733 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 734 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 737 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 738 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 739 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 740 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 742 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 748 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 749 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 763 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 764 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 765 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 766 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 776 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 777 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 781 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 782 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 786 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 788 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 789 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 803 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 804 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 805 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 806 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 811 "chapel.ypp"
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
#line 821 "chapel.ypp"
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
#line 831 "chapel.ypp"
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
#line 841 "chapel.ypp"
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
#line 852 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 856 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 861 "chapel.ypp"
    {
      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), context->latestComment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
      context->latestComment = NULL;
    }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 867 "chapel.ypp"
    {
      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), context->latestComment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
      context->latestComment = NULL;
    }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 876 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 882 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 883 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 884 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 885 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 886 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 887 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 888 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 889 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 890 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 891 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 892 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 893 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 894 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 895 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 896 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 897 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 898 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 899 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 901 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 907 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 913 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 919 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 925 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 932 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 939 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 943 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 949 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 953 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 954 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 955 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 956 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 957 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 959 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 961 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 963 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 968 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 971 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 972 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 973 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 974 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 975 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 976 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 980 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 981 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 985 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 986 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 987 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 991 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 992 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 996 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1000 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1002 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1006 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1007 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1012 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1014 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1016 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1023 "chapel.ypp"
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
#line 1033 "chapel.ypp"
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
#line 1046 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1051 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1056 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1064 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1065 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1070 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1072 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1074 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1079 "chapel.ypp"
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
#line 1095 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1102 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1111 "chapel.ypp"
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
#line 1119 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1123 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1129 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1130 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1135 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1140 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1145 "chapel.ypp"
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
#line 1167 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1173 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1180 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1191 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1197 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1204 "chapel.ypp"
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
#line 1230 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1234 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1239 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1243 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1248 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1255 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1259 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1260 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1261 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1262 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1263 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1264 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1265 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1266 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1267 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1268 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1269 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1270 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1271 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1272 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1273 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1276 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1277 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1283 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1284 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1286 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1298 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1314 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1321 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1323 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1325 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1327 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1329 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1333 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1338 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1340 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1341 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1342 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1343 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1344 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1345 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1346 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1350 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1351 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1352 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1353 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1354 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1364 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1365 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1366 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1367 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1374 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1377 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1383 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1388 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1390 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1394 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1399 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1400 "chapel.ypp"
    { if (DefExpr* def = toDefExpr((yyvsp[(2) - (2)].pexpr))) {
                                 def->sym->addFlag(FLAG_PARAM);
                               }
                               (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
                             }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1408 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1410 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1412 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1414 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1416 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1420 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1421 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1424 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1425 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1426 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1427 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1428 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1429 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1430 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1434 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1438 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1440 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1442 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1447 "chapel.ypp"
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
#line 1460 "chapel.ypp"
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
#line 1476 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1480 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1486 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1487 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1488 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1493 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1499 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1508 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1518 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1527 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1532 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1534 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1536 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1542 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1543 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1544 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1550 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1552 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1554 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1564 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1568 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1575 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1576 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1577 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1578 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1579 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1584 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1586 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1587 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1588 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1609 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1613 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1617 "chapel.ypp"
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
#line 1625 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1631 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1633 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1638 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1640 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1646 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1648 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1650 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1654 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1656 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1657 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1658 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1664 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1665 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1666 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1667 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1671 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1672 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1676 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1677 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1678 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1682 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1683 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1687 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1692 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1693 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1697 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1698 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1699 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1704 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1705 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1719 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1730 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1732 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1734 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1736 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1738 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1740 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1742 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1745 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1747 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1749 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1751 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1753 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1755 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1757 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1759 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1762 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1764 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1766 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1772 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1776 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1778 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1780 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1782 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1784 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1786 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1788 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1790 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1792 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1794 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1796 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1803 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1809 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1815 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1821 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1830 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1839 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1855 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1859 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1864 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1868 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1869 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1873 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1877 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1878 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1883 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1887 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1891 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1897 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1898 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1899 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1900 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1901 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1902 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1908 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1913 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1918 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1934 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1936 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1938 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1940 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1942 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1944 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1948 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1949 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1952 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1953 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1954 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 1971 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 1972 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 1973 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 1977 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 1978 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 1979 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 1983 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 1984 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 1985 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 1986 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 1987 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 1995 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 1996 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 1997 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 1998 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 2002 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 2003 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 2008 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 2009 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 2010 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 2011 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 2012 "chapel.ypp"
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 2013 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 2014 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 2015 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 2016 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 2017 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 2018 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 2020 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 2024 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 2031 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 2032 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 2036 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 2037 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 2038 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2039 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2040 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2041 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 2042 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2043 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 2044 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), gUninstantiated); }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 2045 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2046 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), gUninstantiated); }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2047 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2048 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2049 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2050 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2051 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2052 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2053 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2054 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2055 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2056 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2057 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2058 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2059 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2060 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2064 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 584:
/* Line 1792 of yacc.c  */
#line 2065 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 585:
/* Line 1792 of yacc.c  */
#line 2066 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 586:
/* Line 1792 of yacc.c  */
#line 2067 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 587:
/* Line 1792 of yacc.c  */
#line 2068 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 588:
/* Line 1792 of yacc.c  */
#line 2069 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 589:
/* Line 1792 of yacc.c  */
#line 2070 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 590:
/* Line 1792 of yacc.c  */
#line 2074 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 591:
/* Line 1792 of yacc.c  */
#line 2075 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 592:
/* Line 1792 of yacc.c  */
#line 2076 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 593:
/* Line 1792 of yacc.c  */
#line 2077 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 594:
/* Line 1792 of yacc.c  */
#line 2081 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 595:
/* Line 1792 of yacc.c  */
#line 2082 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 596:
/* Line 1792 of yacc.c  */
#line 2083 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 597:
/* Line 1792 of yacc.c  */
#line 2084 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 598:
/* Line 1792 of yacc.c  */
#line 2089 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 599:
/* Line 1792 of yacc.c  */
#line 2090 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 600:
/* Line 1792 of yacc.c  */
#line 2091 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 601:
/* Line 1792 of yacc.c  */
#line 2092 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 602:
/* Line 1792 of yacc.c  */
#line 2093 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 603:
/* Line 1792 of yacc.c  */
#line 2094 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 604:
/* Line 1792 of yacc.c  */
#line 2095 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 10029 "bison-chapel.cpp"
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


