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

  enum   ProcIter {
    ProcIter_PROC,
    ProcIter_ITER
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
    ProcIter                  procIter;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<PotentialRename*>* ponlylist;
    std::set<Flag>*           pflagset;
    WhereAndLifetime          lifetimeAndWhere;
  };

  #endif

/* Line 387 of yacc.c  */
#line 151 "chapel.ypp"

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
#line 174 "chapel.ypp"

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
#line 254 "bison-chapel.cpp"

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
     TOTHERWISE = 326,
     TOUT = 327,
     TOVERRIDE = 328,
     TOWNED = 329,
     TPARAM = 330,
     TPLUSPLUS = 331,
     TPRAGMA = 332,
     TPRIMITIVE = 333,
     TPRIVATE = 334,
     TPROC = 335,
     TPROTOTYPE = 336,
     TPUBLIC = 337,
     TREAL = 338,
     TRECORD = 339,
     TREDUCE = 340,
     TREF = 341,
     TREQUIRE = 342,
     TRETURN = 343,
     TSCAN = 344,
     TSELECT = 345,
     TSERIAL = 346,
     TSHARED = 347,
     TSINGLE = 348,
     TSPARSE = 349,
     TSTRING = 350,
     TSUBDOMAIN = 351,
     TSYNC = 352,
     TTHEN = 353,
     TTHIS = 354,
     TTHROW = 355,
     TTHROWS = 356,
     TTRUE = 357,
     TTRY = 358,
     TTRYBANG = 359,
     TTYPE = 360,
     TUINT = 361,
     TUNDERSCORE = 362,
     TUNION = 363,
     TUNMANAGED = 364,
     TUSE = 365,
     TVAR = 366,
     TVOID = 367,
     TWHEN = 368,
     TWHERE = 369,
     TWHILE = 370,
     TWITH = 371,
     TYIELD = 372,
     TZIP = 373,
     TALIAS = 374,
     TAND = 375,
     TASSIGN = 376,
     TASSIGNBAND = 377,
     TASSIGNBOR = 378,
     TASSIGNBXOR = 379,
     TASSIGNDIVIDE = 380,
     TASSIGNEXP = 381,
     TASSIGNLAND = 382,
     TASSIGNLOR = 383,
     TASSIGNMINUS = 384,
     TASSIGNMOD = 385,
     TASSIGNMULTIPLY = 386,
     TASSIGNPLUS = 387,
     TASSIGNREDUCE = 388,
     TASSIGNSL = 389,
     TASSIGNSR = 390,
     TBANG = 391,
     TBAND = 392,
     TBNOT = 393,
     TBOR = 394,
     TBXOR = 395,
     TCOLON = 396,
     TCOMMA = 397,
     TDIVIDE = 398,
     TDOT = 399,
     TDOTDOT = 400,
     TDOTDOTDOT = 401,
     TEQUAL = 402,
     TEXP = 403,
     TGREATER = 404,
     TGREATEREQUAL = 405,
     THASH = 406,
     TIO = 407,
     TLESS = 408,
     TLESSEQUAL = 409,
     TMINUS = 410,
     TMOD = 411,
     TNOTEQUAL = 412,
     TOR = 413,
     TPLUS = 414,
     TQUESTION = 415,
     TSEMI = 416,
     TSHIFTLEFT = 417,
     TSHIFTRIGHT = 418,
     TSTAR = 419,
     TSWAP = 420,
     TLCBR = 421,
     TRCBR = 422,
     TLP = 423,
     TRP = 424,
     TLSBR = 425,
     TRSBR = 426,
     TNOELSE = 427,
     TDOTDOTOPENHIGH = 428,
     TUMINUS = 429,
     TUPLUS = 430,
     TLNOT = 431
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
#line 205 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);


/* Line 387 of yacc.c  */
#line 495 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 502 "bison-chapel.cpp"
/* Unqualified %code blocks.  */
/* Line 391 of yacc.c  */
#line 39 "chapel.ypp"

  #include <string>
  int         captureTokens;
  std::string captureString;
  bool        parsingPrivate=false;

/* Line 391 of yacc.c  */
#line 213 "chapel.ypp"

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
#line 580 "bison-chapel.cpp"

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
#define YYLAST   19875

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  177
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  160
/* YYNRULES -- Number of rules.  */
#define YYNRULES  676
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1207

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   431

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
     175,   176
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
     718,   723,   727,   734,   740,   747,   753,   762,   770,   777,
     781,   783,   787,   789,   791,   793,   795,   797,   799,   801,
     803,   805,   807,   809,   811,   813,   815,   817,   819,   821,
     823,   825,   827,   834,   839,   847,   853,   857,   864,   867,
     871,   875,   878,   881,   885,   889,   890,   893,   896,   900,
     906,   908,   912,   916,   922,   928,   929,   932,   936,   939,
     943,   950,   957,   959,   961,   963,   964,   967,   968,   971,
     975,   981,   987,   989,   991,   994,   998,  1000,  1004,  1005,
    1006,  1015,  1016,  1018,  1020,  1021,  1022,  1033,  1037,  1041,
    1047,  1053,  1057,  1059,  1063,  1065,  1067,  1069,  1071,  1073,
    1075,  1077,  1079,  1081,  1083,  1085,  1087,  1089,  1091,  1093,
    1095,  1097,  1099,  1101,  1103,  1105,  1107,  1109,  1111,  1113,
    1115,  1118,  1120,  1122,  1124,  1126,  1128,  1130,  1132,  1134,
    1136,  1138,  1140,  1142,  1143,  1147,  1151,  1152,  1154,  1158,
    1163,  1169,  1174,  1181,  1188,  1189,  1191,  1193,  1195,  1197,
    1199,  1202,  1205,  1207,  1209,  1211,  1212,  1214,  1216,  1219,
    1221,  1223,  1225,  1227,  1228,  1230,  1233,  1235,  1237,  1239,
    1240,  1242,  1244,  1246,  1248,  1250,  1252,  1254,  1257,  1260,
    1261,  1264,  1267,  1272,  1277,  1279,  1283,  1287,  1291,  1295,
    1299,  1303,  1307,  1310,  1312,  1314,  1318,  1323,  1328,  1331,
    1336,  1337,  1340,  1343,  1345,  1347,  1349,  1352,  1354,  1359,
    1363,  1365,  1369,  1373,  1379,  1381,  1383,  1387,  1389,  1392,
    1396,  1397,  1400,  1403,  1407,  1410,  1415,  1419,  1423,  1428,
    1432,  1433,  1436,  1439,  1442,  1444,  1445,  1448,  1451,  1454,
    1456,  1461,  1466,  1473,  1477,  1478,  1480,  1482,  1486,  1491,
    1495,  1500,  1507,  1508,  1511,  1514,  1517,  1520,  1522,  1524,
    1528,  1532,  1534,  1538,  1540,  1542,  1544,  1548,  1552,  1553,
    1555,  1557,  1561,  1565,  1569,  1571,  1573,  1575,  1577,  1579,
    1582,  1584,  1586,  1588,  1590,  1592,  1594,  1597,  1602,  1607,
    1612,  1618,  1621,  1624,  1626,  1629,  1631,  1634,  1636,  1639,
    1641,  1644,  1646,  1648,  1655,  1662,  1667,  1677,  1687,  1695,
    1702,  1709,  1714,  1724,  1734,  1742,  1747,  1754,  1761,  1771,
    1781,  1788,  1790,  1792,  1794,  1796,  1798,  1800,  1802,  1804,
    1808,  1809,  1811,  1816,  1818,  1822,  1827,  1829,  1833,  1838,
    1842,  1846,  1848,  1850,  1853,  1855,  1858,  1860,  1862,  1866,
    1868,  1871,  1874,  1877,  1880,  1883,  1892,  1901,  1911,  1921,
    1926,  1928,  1930,  1932,  1934,  1936,  1938,  1940,  1942,  1944,
    1949,  1953,  1957,  1961,  1964,  1967,  1970,  1972,  1973,  1975,
    1978,  1981,  1983,  1985,  1987,  1989,  1991,  1993,  1996,  1999,
    2001,  2003,  2008,  2013,  2018,  2022,  2026,  2030,  2034,  2040,
    2044,  2049,  2053,  2058,  2060,  2062,  2064,  2066,  2068,  2070,
    2072,  2074,  2076,  2078,  2080,  2084,  2089,  2093,  2098,  2102,
    2107,  2111,  2117,  2121,  2125,  2129,  2133,  2137,  2141,  2145,
    2149,  2153,  2157,  2161,  2165,  2169,  2173,  2177,  2181,  2185,
    2189,  2193,  2197,  2201,  2205,  2209,  2212,  2215,  2218,  2221,
    2224,  2227,  2230,  2234,  2238,  2242,  2246,  2250,  2254,  2258,
    2262,  2264,  2266,  2268,  2270,  2272,  2274
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     178,     0,    -1,   179,    -1,    -1,   179,   180,    -1,   182,
      -1,   181,   182,    -1,    77,     8,    -1,   181,    77,     8,
      -1,   184,    -1,   188,    -1,   190,    -1,   197,    -1,   198,
      -1,   201,    -1,   212,    -1,   202,    -1,   221,    -1,   224,
      -1,   230,    -1,   225,    -1,   222,    -1,   238,    -1,   232,
      -1,   233,    -1,   237,    -1,   211,    -1,   308,   161,    -1,
      14,   182,    -1,    15,   309,   182,    -1,    16,   203,   161,
      -1,    23,   309,   190,    -1,    28,   203,   161,    -1,    30,
     296,   161,    -1,    56,   205,   182,    -1,    60,   320,   210,
      -1,    60,   210,    -1,    69,   320,   210,    -1,    91,   320,
     210,    -1,    91,   210,    -1,    97,   182,    -1,   117,   320,
     161,    -1,     1,   161,    -1,   185,   186,    63,   205,    -1,
     183,   166,   167,    -1,   183,   166,   191,   167,    -1,   183,
     166,     1,   167,    -1,    -1,    82,    -1,    79,    -1,    -1,
      81,    -1,    -1,    82,    -1,    79,    -1,    -1,    47,   189,
     187,   186,    63,   205,   161,    -1,   166,   167,    -1,   166,
     191,   167,    -1,   166,     1,   167,    -1,   180,    -1,   191,
     180,    -1,   320,    -1,   320,    13,   320,    -1,   192,   142,
     320,    -1,   192,   142,   320,    13,   320,    -1,   320,    -1,
     320,    13,   320,    -1,   320,    13,   107,    -1,   193,   142,
     320,    -1,   193,   142,   320,    13,   320,    -1,   193,   142,
     320,    13,   107,    -1,    -1,   192,    -1,   164,    -1,   192,
      -1,    -1,    82,    -1,    79,    -1,   196,   110,   193,   161,
      -1,   196,   110,   320,    36,   195,   161,    -1,   196,   110,
     320,    13,   320,    36,   195,   161,    -1,   196,   110,   320,
      13,   107,    36,   195,   161,    -1,   196,   110,   320,    70,
     194,   161,    -1,   196,   110,   320,    13,   320,    70,   194,
     161,    -1,   196,   110,   320,    13,   107,    70,   194,   161,
      -1,   196,    45,   200,   161,    -1,   320,    -1,   320,    13,
     206,    -1,   320,   144,   166,   192,   167,    -1,   199,    -1,
     200,   142,   199,    -1,    87,   295,   161,    -1,   323,   259,
     322,   161,    -1,   323,   165,   322,   161,    -1,   323,   133,
     322,   161,    -1,   323,   127,   322,   161,    -1,   323,   128,
     322,   161,    -1,   323,   121,    66,   161,    -1,    -1,     3,
      -1,     3,    -1,    67,    -1,    99,    -1,    39,    -1,   102,
      -1,   207,    -1,     3,    -1,    67,    -1,    99,    -1,    39,
      -1,   102,    -1,   207,    -1,     3,    -1,    99,    -1,    17,
      -1,    51,    -1,   106,    -1,    83,    -1,    44,    -1,    25,
      -1,    20,    -1,    95,    -1,    97,    -1,    93,    -1,    74,
      -1,    92,    -1,    18,    -1,   109,    -1,    33,    -1,    48,
      -1,    61,    -1,    68,    -1,   112,    -1,    17,    -1,    35,
      -1,    51,    -1,   106,    -1,    83,    -1,    44,    -1,    25,
      -1,    20,    -1,    95,    -1,    61,    -1,    68,    -1,   112,
      -1,    97,    -1,    93,    -1,    33,    -1,    48,    -1,    32,
     182,    -1,   190,    -1,    88,   161,    -1,    88,   322,   161,
      -1,   161,    -1,   214,    -1,    82,   214,    -1,    -1,    79,
     213,   214,    -1,   253,    -1,   282,    -1,   245,    -1,   278,
      -1,   241,    -1,   215,    -1,   216,    -1,    42,   320,   161,
      -1,    42,   320,    36,   195,   161,    -1,    42,   320,    70,
     194,   161,    -1,    42,   282,    -1,    -1,    38,    84,   217,
     205,   243,   166,   244,   167,    -1,    -1,    38,     8,    84,
     218,   205,   243,   166,   244,   167,    -1,    -1,    37,    84,
     219,   205,   243,   166,   244,   167,    -1,    -1,    37,     8,
      84,   220,   205,   243,   166,   244,   167,    -1,    38,   321,
     253,    -1,    37,   321,   253,    -1,    38,   321,   281,   283,
     161,    -1,    37,   321,   281,   283,   161,    -1,    38,    11,
      -1,    32,   182,   115,   320,   161,    -1,   115,   320,   210,
      -1,    24,   320,    46,   320,   309,   210,    -1,    24,   320,
      46,   223,   309,   210,    -1,    24,   320,   309,   210,    -1,
      40,   320,    46,   320,   210,    -1,    40,   320,    46,   223,
     210,    -1,    40,   320,   210,    -1,    40,   223,   210,    -1,
      40,    75,   205,    46,   320,   210,    -1,    41,   320,    46,
     320,   210,    -1,    41,   320,    46,   320,   312,   210,    -1,
      41,   320,    46,   223,   210,    -1,    41,   320,    46,   223,
     312,   210,    -1,    41,   320,   210,    -1,    41,   320,   312,
     210,    -1,    41,   223,   210,    -1,    41,   223,   312,   210,
      -1,   170,   295,    46,   320,   171,   182,    -1,   170,   295,
      46,   320,   312,   171,   182,    -1,   170,   295,    46,   223,
     171,   182,    -1,   170,   295,    46,   223,   312,   171,   182,
      -1,   170,   295,   171,   182,    -1,   170,   295,   312,   171,
     182,    -1,   170,   223,   171,   182,    -1,   170,   223,   312,
     171,   182,    -1,   118,   168,   295,   169,    -1,    43,   320,
      98,   182,    -1,    43,   320,   190,    -1,    43,   320,    98,
     182,    34,   182,    -1,    43,   320,   190,    34,   182,    -1,
      43,   320,   259,   320,    98,   182,    -1,    43,   320,   259,
     320,   190,    -1,    43,   320,   259,   320,    98,   182,    34,
     182,    -1,    43,   320,   259,   320,   190,    34,   182,    -1,
      55,   205,   168,   226,   169,   190,    -1,    55,   205,   190,
      -1,   227,    -1,   226,   142,   227,    -1,   205,    -1,     3,
      -1,    17,    -1,    51,    -1,   106,    -1,    83,    -1,    44,
      -1,    25,    -1,    20,    -1,    95,    -1,    61,    -1,    68,
      -1,   112,    -1,   229,    -1,    67,    -1,    99,    -1,    39,
      -1,   102,    -1,    33,    -1,    48,    -1,    54,   205,   168,
     300,   169,   161,    -1,   228,    54,   205,   161,    -1,   228,
      54,   205,   168,   300,   169,   161,    -1,    54,   205,   168,
     300,   169,    -1,   228,    54,   205,    -1,   228,    54,   205,
     168,   300,   169,    -1,    29,   182,    -1,   103,   320,   161,
      -1,   104,   320,   161,    -1,   103,   202,    -1,   104,   202,
      -1,   103,   190,   234,    -1,   104,   190,   234,    -1,    -1,
     234,   235,    -1,    21,   190,    -1,    21,   236,   190,    -1,
      21,   168,   236,   169,   190,    -1,   205,    -1,   205,   141,
     320,    -1,   100,   320,   161,    -1,    90,   320,   166,   239,
     167,    -1,    90,   320,   166,     1,   167,    -1,    -1,   239,
     240,    -1,   113,   295,   210,    -1,    71,   182,    -1,    71,
      32,   182,    -1,   242,   205,   243,   166,   244,   167,    -1,
     242,   205,   243,   166,     1,   167,    -1,    22,    -1,    84,
      -1,   108,    -1,    -1,   141,   295,    -1,    -1,   244,   212,
      -1,   244,   181,   212,    -1,   246,   205,   166,   247,   167,
      -1,   246,   205,   166,     1,   167,    -1,    35,    -1,   248,
      -1,   247,   142,    -1,   247,   142,   248,    -1,   205,    -1,
     205,   121,   320,    -1,    -1,    -1,    57,   250,   261,   251,
     268,   290,   274,   271,    -1,    -1,    49,    -1,    73,    -1,
      -1,    -1,   252,   267,   254,   256,   255,   268,   289,   269,
     274,   270,    -1,   266,   258,   260,    -1,   266,   259,   260,
      -1,   266,   257,   144,   258,   260,    -1,   266,   257,   144,
     259,   260,    -1,   266,     1,   260,    -1,   302,    -1,   168,
     320,   169,    -1,   204,    -1,   137,    -1,   139,    -1,   140,
      -1,   138,    -1,   147,    -1,   157,    -1,   154,    -1,   150,
      -1,   153,    -1,   149,    -1,   159,    -1,   155,    -1,   164,
      -1,   143,    -1,   162,    -1,   163,    -1,   156,    -1,   148,
      -1,   136,    -1,    19,    -1,   151,    -1,    12,    -1,   165,
      -1,   152,    -1,    53,    -1,   205,   136,    -1,   121,    -1,
     132,    -1,   129,    -1,   131,    -1,   125,    -1,   130,    -1,
     126,    -1,   122,    -1,   123,    -1,   124,    -1,   135,    -1,
     134,    -1,    -1,   168,   262,   169,    -1,   168,   262,   169,
      -1,    -1,   263,    -1,   262,   142,   263,    -1,   264,   205,
     294,   287,    -1,   181,   264,   205,   294,   287,    -1,   264,
     205,   294,   273,    -1,   264,   168,   286,   169,   294,   287,
      -1,   264,   168,   286,   169,   294,   273,    -1,    -1,   265,
      -1,    46,    -1,    50,    -1,    72,    -1,    27,    -1,    27,
      46,    -1,    27,    86,    -1,    75,    -1,    86,    -1,   105,
      -1,    -1,    75,    -1,    86,    -1,    27,    86,    -1,    27,
      -1,   105,    -1,    80,    -1,    52,    -1,    -1,    27,    -1,
      27,    86,    -1,    86,    -1,    75,    -1,   105,    -1,    -1,
     101,    -1,   161,    -1,   271,    -1,   190,    -1,   211,    -1,
       4,    -1,   146,    -1,   146,   320,    -1,   146,   272,    -1,
      -1,   114,   320,    -1,    59,   275,    -1,   114,   320,    59,
     275,    -1,    59,   275,   114,   320,    -1,   276,    -1,   275,
     142,   276,    -1,   277,   121,   277,    -1,   277,   153,   277,
      -1,   277,   154,   277,    -1,   277,   147,   277,    -1,   277,
     149,   277,    -1,   277,   150,   277,    -1,    88,   277,    -1,
       3,    -1,    99,    -1,   105,   279,   161,    -1,    26,   105,
     279,   161,    -1,    38,   105,   279,   161,    -1,   205,   280,
      -1,   205,   280,   142,   279,    -1,    -1,   121,   303,    -1,
     121,   291,    -1,    75,    -1,    27,    -1,    86,    -1,    27,
      86,    -1,   111,    -1,    26,   281,   283,   161,    -1,   281,
     283,   161,    -1,   284,    -1,   283,   142,   284,    -1,   205,
     290,   287,    -1,   168,   286,   169,   290,   287,    -1,   107,
      -1,   205,    -1,   168,   286,   169,    -1,   285,    -1,   285,
     142,    -1,   285,   142,   286,    -1,    -1,   121,    66,    -1,
     121,   322,    -1,   170,   171,   303,    -1,   170,   171,    -1,
     170,   295,   171,   303,    -1,   170,   295,   171,    -1,   170,
     171,   288,    -1,   170,   295,   171,   288,    -1,   170,     1,
     171,    -1,    -1,   141,   303,    -1,   141,   288,    -1,   141,
     209,    -1,     1,    -1,    -1,   141,   303,    -1,   141,   291,
      -1,   141,   209,    -1,     1,    -1,   170,   295,   171,   303,
      -1,   170,   295,   171,   291,    -1,   170,   295,    46,   320,
     171,   303,    -1,   170,     1,   171,    -1,    -1,   303,    -1,
     272,    -1,   170,   171,   292,    -1,   170,   295,   171,   292,
      -1,   170,   171,   293,    -1,   170,   295,   171,   293,    -1,
     170,   295,    46,   320,   171,   292,    -1,    -1,   141,   303,
      -1,   141,   272,    -1,   141,   209,    -1,   141,   293,    -1,
     320,    -1,   272,    -1,   295,   142,   320,    -1,   295,   142,
     272,    -1,   320,    -1,   296,   142,   320,    -1,   107,    -1,
     322,    -1,   272,    -1,   297,   142,   297,    -1,   298,   142,
     297,    -1,    -1,   300,    -1,   301,    -1,   300,   142,   301,
      -1,   206,   121,   272,    -1,   206,   121,   322,    -1,   272,
      -1,   322,    -1,   206,    -1,   208,    -1,   304,    -1,   304,
     160,    -1,   160,    -1,   307,    -1,   323,    -1,   306,    -1,
     333,    -1,   332,    -1,    93,   320,    -1,    48,   168,   299,
     169,    -1,    33,   168,   299,   169,    -1,    96,   168,   299,
     169,    -1,    94,    96,   168,   299,   169,    -1,    14,   320,
      -1,    97,   320,    -1,    74,    -1,    74,   320,    -1,   109,
      -1,   109,   320,    -1,    92,    -1,    92,   320,    -1,    18,
      -1,    18,   320,    -1,    22,    -1,    84,    -1,    40,   320,
      46,   320,    32,   320,    -1,    40,   320,    46,   223,    32,
     320,    -1,    40,   320,    32,   320,    -1,    40,   320,    46,
     320,    32,    43,   320,    98,   320,    -1,    40,   320,    46,
     223,    32,    43,   320,    98,   320,    -1,    40,   320,    32,
      43,   320,    98,   320,    -1,    41,   320,    46,   320,    32,
     320,    -1,    41,   320,    46,   223,    32,   320,    -1,    41,
     320,    32,   320,    -1,    41,   320,    46,   320,    32,    43,
     320,    98,   320,    -1,    41,   320,    46,   223,    32,    43,
     320,    98,   320,    -1,    41,   320,    32,    43,   320,    98,
     320,    -1,   170,   295,   171,   320,    -1,   170,   295,    46,
     320,   171,   320,    -1,   170,   295,    46,   223,   171,   320,
      -1,   170,   295,    46,   320,   171,    43,   320,    98,   320,
      -1,   170,   295,    46,   223,   171,    43,   320,    98,   320,
      -1,    43,   320,    98,   320,    34,   320,    -1,    65,    -1,
     307,    -1,   302,    -1,   326,    -1,   325,    -1,   249,    -1,
     318,    -1,   319,    -1,   316,   152,   320,    -1,    -1,   310,
      -1,   116,   168,   311,   169,    -1,   314,    -1,   311,   142,
     314,    -1,   116,   168,   313,   169,    -1,   314,    -1,   313,
     142,   314,    -1,   315,   302,   290,   287,    -1,   336,    85,
     302,    -1,   320,    85,   302,    -1,    27,    -1,    46,    -1,
      27,    46,    -1,    86,    -1,    27,    86,    -1,   111,    -1,
     323,    -1,   316,   152,   320,    -1,    64,    -1,    64,    74,
      -1,    64,    92,    -1,    64,   109,    -1,    64,    18,    -1,
     317,   320,    -1,    64,    74,   168,   320,   169,   168,   299,
     169,    -1,    64,    92,   168,   320,   169,   168,   299,   169,
      -1,    64,    74,   168,   320,   169,   168,   299,   169,   160,
      -1,    64,    92,   168,   320,   169,   168,   299,   169,   160,
      -1,    58,   283,    46,   320,    -1,   330,    -1,   303,    -1,
     305,    -1,   334,    -1,   335,    -1,   249,    -1,   318,    -1,
     319,    -1,   231,    -1,   168,   146,   320,   169,    -1,   320,
     141,   320,    -1,   320,   145,   320,    -1,   320,   173,   320,
      -1,   320,   145,    -1,   145,   320,    -1,   173,   320,    -1,
     145,    -1,    -1,   320,    -1,   103,   320,    -1,   104,   320,
      -1,   320,    -1,   302,    -1,   325,    -1,   326,    -1,   327,
      -1,   323,    -1,   320,   136,    -1,   304,   160,    -1,   249,
      -1,   329,    -1,   324,   168,   299,   169,    -1,   324,   170,
     299,   171,    -1,    78,   168,   299,   169,    -1,   320,   144,
     206,    -1,   320,   144,   105,    -1,   320,   144,    33,    -1,
     320,   144,    61,    -1,   320,   144,    20,   168,   169,    -1,
     168,   297,   169,    -1,   168,   297,   142,   169,    -1,   168,
     298,   169,    -1,   168,   298,   142,   169,    -1,    39,    -1,
     102,    -1,     8,    -1,     9,    -1,   328,    -1,   329,    -1,
       5,    -1,     6,    -1,     7,    -1,    10,    -1,    67,    -1,
     166,   295,   167,    -1,   166,   295,   142,   167,    -1,   170,
     295,   171,    -1,   170,   295,   142,   171,    -1,   170,   331,
     171,    -1,   170,   331,   142,   171,    -1,   320,   119,   320,
      -1,   331,   142,   320,   119,   320,    -1,   320,   159,   320,
      -1,   320,   155,   320,    -1,   320,   164,   320,    -1,   320,
     143,   320,    -1,   320,   162,   320,    -1,   320,   163,   320,
      -1,   320,   156,   320,    -1,   320,   147,   320,    -1,   320,
     157,   320,    -1,   320,   154,   320,    -1,   320,   150,   320,
      -1,   320,   153,   320,    -1,   320,   149,   320,    -1,   320,
     137,   320,    -1,   320,   139,   320,    -1,   320,   140,   320,
      -1,   320,   120,   320,    -1,   320,   158,   320,    -1,   320,
     148,   320,    -1,   320,    19,   320,    -1,   320,    12,   320,
      -1,   320,   151,   320,    -1,   320,    31,   320,    -1,   159,
     320,    -1,   155,   320,    -1,    62,   320,    -1,    76,   320,
      -1,   136,   320,    -1,   320,   136,    -1,   138,   320,    -1,
     320,    85,   320,    -1,   320,    85,   223,    -1,   336,    85,
     320,    -1,   336,    85,   223,    -1,   320,    89,   320,    -1,
     320,    89,   223,    -1,   336,    89,   320,    -1,   336,    89,
     223,    -1,   159,    -1,   164,    -1,   120,    -1,   158,    -1,
     137,    -1,   139,    -1,   140,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   509,   509,   514,   515,   521,   522,   527,   528,   533,
     534,   535,   536,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,   561,   562,   563,
     564,   565,   566,   570,   583,   588,   593,   601,   602,   603,
     607,   608,   612,   613,   614,   619,   618,   639,   640,   641,
     646,   647,   652,   657,   662,   666,   675,   680,   685,   690,
     694,   698,   706,   711,   715,   720,   724,   725,   726,   730,
     731,   732,   733,   734,   735,   736,   740,   745,   746,   747,
     751,   752,   756,   760,   762,   764,   766,   768,   770,   777,
     778,   782,   783,   784,   785,   786,   787,   790,   791,   792,
     793,   794,   795,   807,   808,   819,   820,   821,   822,   823,
     824,   825,   826,   827,   828,   829,   830,   831,   832,   833,
     834,   835,   836,   837,   841,   842,   843,   844,   845,   846,
     847,   848,   849,   850,   851,   852,   859,   860,   861,   862,
     866,   867,   871,   872,   876,   877,   878,   888,   888,   893,
     894,   895,   896,   897,   898,   899,   903,   904,   905,   906,
     911,   910,   926,   925,   942,   941,   957,   956,   972,   976,
     981,   989,  1000,  1007,  1008,  1009,  1010,  1011,  1012,  1013,
    1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,
    1024,  1025,  1031,  1037,  1043,  1049,  1056,  1063,  1067,  1074,
    1078,  1079,  1080,  1081,  1082,  1084,  1086,  1088,  1093,  1095,
    1103,  1104,  1109,  1114,  1115,  1116,  1117,  1118,  1119,  1120,
    1121,  1122,  1123,  1124,  1125,  1126,  1133,  1134,  1135,  1136,
    1145,  1146,  1150,  1152,  1155,  1161,  1163,  1166,  1172,  1175,
    1176,  1177,  1178,  1179,  1180,  1184,  1185,  1189,  1190,  1191,
    1195,  1196,  1200,  1203,  1205,  1210,  1211,  1215,  1217,  1219,
    1226,  1236,  1250,  1255,  1260,  1268,  1269,  1274,  1275,  1277,
    1282,  1298,  1305,  1314,  1322,  1326,  1333,  1334,  1339,  1344,
    1338,  1371,  1374,  1378,  1386,  1396,  1385,  1428,  1432,  1437,
    1441,  1446,  1453,  1454,  1458,  1459,  1460,  1461,  1462,  1463,
    1464,  1465,  1466,  1467,  1468,  1469,  1470,  1471,  1472,  1473,
    1474,  1475,  1476,  1477,  1478,  1479,  1480,  1481,  1482,  1483,
    1484,  1488,  1489,  1490,  1491,  1492,  1493,  1494,  1495,  1496,
    1497,  1498,  1499,  1503,  1504,  1508,  1512,  1513,  1514,  1518,
    1520,  1522,  1524,  1526,  1531,  1532,  1536,  1537,  1538,  1539,
    1540,  1541,  1542,  1543,  1544,  1548,  1549,  1550,  1551,  1552,
    1553,  1557,  1558,  1562,  1563,  1564,  1565,  1566,  1567,  1571,
    1572,  1575,  1576,  1580,  1581,  1585,  1589,  1590,  1591,  1599,
    1600,  1602,  1604,  1606,  1611,  1613,  1618,  1619,  1620,  1621,
    1622,  1623,  1624,  1628,  1630,  1635,  1637,  1639,  1644,  1657,
    1674,  1675,  1677,  1682,  1683,  1684,  1685,  1686,  1690,  1696,
    1704,  1705,  1713,  1715,  1720,  1722,  1724,  1729,  1731,  1733,
    1740,  1741,  1742,  1747,  1749,  1751,  1755,  1759,  1761,  1765,
    1773,  1774,  1775,  1776,  1777,  1782,  1783,  1784,  1785,  1786,
    1806,  1810,  1814,  1822,  1829,  1830,  1831,  1835,  1837,  1843,
    1845,  1847,  1852,  1853,  1854,  1855,  1856,  1862,  1863,  1864,
    1865,  1869,  1870,  1874,  1875,  1876,  1880,  1881,  1885,  1886,
    1890,  1891,  1895,  1896,  1897,  1898,  1902,  1903,  1914,  1916,
    1918,  1924,  1925,  1926,  1927,  1928,  1929,  1931,  1933,  1935,
    1937,  1939,  1941,  1944,  1946,  1948,  1950,  1952,  1954,  1956,
    1958,  1961,  1963,  1968,  1970,  1972,  1974,  1976,  1978,  1980,
    1982,  1984,  1986,  1988,  1990,  1992,  1999,  2005,  2011,  2017,
    2026,  2036,  2044,  2045,  2046,  2047,  2048,  2049,  2050,  2051,
    2056,  2057,  2061,  2065,  2066,  2070,  2074,  2075,  2079,  2083,
    2087,  2094,  2095,  2096,  2097,  2098,  2099,  2103,  2104,  2109,
    2111,  2115,  2119,  2123,  2131,  2136,  2142,  2148,  2155,  2165,
    2173,  2174,  2175,  2176,  2177,  2178,  2179,  2180,  2181,  2182,
    2184,  2186,  2188,  2203,  2205,  2207,  2209,  2214,  2215,  2219,
    2220,  2221,  2225,  2226,  2227,  2228,  2237,  2238,  2239,  2240,
    2241,  2245,  2246,  2247,  2251,  2252,  2253,  2254,  2255,  2263,
    2264,  2265,  2266,  2270,  2271,  2275,  2276,  2280,  2281,  2282,
    2283,  2284,  2285,  2286,  2287,  2289,  2291,  2292,  2293,  2297,
    2305,  2306,  2310,  2311,  2312,  2313,  2314,  2315,  2316,  2317,
    2318,  2319,  2320,  2321,  2322,  2323,  2324,  2325,  2326,  2327,
    2328,  2329,  2330,  2331,  2332,  2337,  2338,  2339,  2340,  2341,
    2342,  2343,  2347,  2348,  2349,  2350,  2354,  2355,  2356,  2357,
    2362,  2363,  2364,  2365,  2366,  2367,  2368
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
  "TRP", "TLSBR", "TRSBR", "TNOELSE", "TDOTDOTOPENHIGH", "TUMINUS",
  "TUPLUS", "TLNOT", "$accept", "program", "toplevel_stmt_ls",
  "toplevel_stmt", "pragma_ls", "stmt", "module_decl_start",
  "module_decl_stmt", "access_control", "opt_prototype",
  "include_access_control", "include_module_stmt", "$@1", "block_stmt",
  "stmt_ls", "renames_ls", "use_renames_ls", "opt_only_ls", "except_ls",
  "use_access_control", "use_stmt", "import_stmt", "import_expr",
  "import_ls", "require_stmt", "assignment_stmt", "opt_label_ident",
  "ident_fn_def", "ident_def", "ident_use", "internal_type_ident_def",
  "scalar_type", "reserved_type_ident_use", "do_stmt", "return_stmt",
  "class_level_stmt", "@2", "private_decl", "forwarding_stmt",
  "extern_export_decl_stmt", "$@3", "$@4", "$@5", "$@6",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "interface_stmt", "ifc_formal_ls", "ifc_formal", "implements_type_ident",
  "implements_type_error_ident", "implements_stmt", "ifc_constraint",
  "defer_stmt", "try_stmt", "catch_stmt_ls", "catch_stmt", "catch_expr",
  "throw_stmt", "select_stmt", "when_stmt_ls", "when_stmt",
  "class_decl_stmt", "class_tag", "opt_inherit", "class_level_stmt_ls",
  "enum_decl_stmt", "enum_header", "enum_ls", "enum_item",
  "lambda_decl_expr", "$@7", "$@8", "linkage_spec", "fn_decl_stmt", "$@9",
  "$@10", "fn_decl_stmt_inner", "fn_decl_receiver_expr", "fn_ident",
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
     425,   426,   427,   428,   429,   430,   431
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   177,   178,   179,   179,   180,   180,   181,   181,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   183,   184,   184,   184,   185,   185,   185,
     186,   186,   187,   187,   187,   189,   188,   190,   190,   190,
     191,   191,   192,   192,   192,   192,   193,   193,   193,   193,
     193,   193,   194,   194,   195,   195,   196,   196,   196,   197,
     197,   197,   197,   197,   197,   197,   198,   199,   199,   199,
     200,   200,   201,   202,   202,   202,   202,   202,   202,   203,
     203,   204,   204,   204,   204,   204,   204,   205,   205,   205,
     205,   205,   205,   206,   206,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   209,   209,   209,   209,
     210,   210,   211,   211,   212,   212,   212,   213,   212,   214,
     214,   214,   214,   214,   214,   214,   215,   215,   215,   215,
     217,   216,   218,   216,   219,   216,   220,   216,   216,   216,
     216,   216,   221,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   223,
     224,   224,   224,   224,   224,   224,   224,   224,   225,   225,
     226,   226,   227,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   229,   229,   229,   229,
     229,   229,   230,   230,   230,   231,   231,   231,   232,   233,
     233,   233,   233,   233,   233,   234,   234,   235,   235,   235,
     236,   236,   237,   238,   238,   239,   239,   240,   240,   240,
     241,   241,   242,   242,   242,   243,   243,   244,   244,   244,
     245,   245,   246,   247,   247,   247,   248,   248,   250,   251,
     249,   252,   252,   252,   254,   255,   253,   256,   256,   256,
     256,   256,   257,   257,   258,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   260,   260,   261,   262,   262,   262,   263,
     263,   263,   263,   263,   264,   264,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   266,   266,   266,   266,   266,
     266,   267,   267,   268,   268,   268,   268,   268,   268,   269,
     269,   270,   270,   271,   271,   272,   273,   273,   273,   274,
     274,   274,   274,   274,   275,   275,   276,   276,   276,   276,
     276,   276,   276,   277,   277,   278,   278,   278,   279,   279,
     280,   280,   280,   281,   281,   281,   281,   281,   282,   282,
     283,   283,   284,   284,   285,   285,   285,   286,   286,   286,
     287,   287,   287,   288,   288,   288,   288,   288,   288,   288,
     289,   289,   289,   289,   289,   290,   290,   290,   290,   290,
     291,   291,   291,   291,   292,   292,   292,   293,   293,   293,
     293,   293,   294,   294,   294,   294,   294,   295,   295,   295,
     295,   296,   296,   297,   297,   297,   298,   298,   299,   299,
     300,   300,   301,   301,   301,   301,   302,   302,   303,   303,
     303,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     306,   307,   308,   308,   308,   308,   308,   308,   308,   308,
     309,   309,   310,   311,   311,   312,   313,   313,   314,   314,
     314,   315,   315,   315,   315,   315,   315,   316,   316,   317,
     317,   317,   317,   317,   318,   318,   318,   318,   318,   319,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   321,   321,   322,
     322,   322,   323,   323,   323,   323,   324,   324,   324,   324,
     324,   325,   325,   325,   326,   326,   326,   326,   326,   327,
     327,   327,   327,   328,   328,   329,   329,   330,   330,   330,
     330,   330,   330,   330,   330,   330,   330,   330,   330,   330,
     331,   331,   332,   332,   332,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   333,   333,   333,   333,   333,
     333,   333,   334,   334,   334,   334,   335,   335,   335,   335,
     336,   336,   336,   336,   336,   336,   336
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
       4,     3,     6,     5,     6,     5,     8,     7,     6,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     6,     4,     7,     5,     3,     6,     2,     3,
       3,     2,     2,     3,     3,     0,     2,     2,     3,     5,
       1,     3,     3,     5,     5,     0,     2,     3,     2,     3,
       6,     6,     1,     1,     1,     0,     2,     0,     2,     3,
       5,     5,     1,     1,     2,     3,     1,     3,     0,     0,
       8,     0,     1,     1,     0,     0,    10,     3,     3,     5,
       5,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     3,     3,     0,     1,     3,     4,
       5,     4,     6,     6,     0,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     0,     1,     1,     2,     1,
       1,     1,     1,     0,     1,     2,     1,     1,     1,     0,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     0,
       2,     2,     4,     4,     1,     3,     3,     3,     3,     3,
       3,     3,     2,     1,     1,     3,     4,     4,     2,     4,
       0,     2,     2,     1,     1,     1,     2,     1,     4,     3,
       1,     3,     3,     5,     1,     1,     3,     1,     2,     3,
       0,     2,     2,     3,     2,     4,     3,     3,     4,     3,
       0,     2,     2,     2,     1,     0,     2,     2,     2,     1,
       4,     4,     6,     3,     0,     1,     1,     3,     4,     3,
       4,     6,     0,     2,     2,     2,     2,     1,     1,     3,
       3,     1,     3,     1,     1,     1,     3,     3,     0,     1,
       1,     3,     3,     3,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     2,     4,     4,     4,
       5,     2,     2,     1,     2,     1,     2,     1,     2,     1,
       2,     1,     1,     6,     6,     4,     9,     9,     7,     6,
       6,     4,     9,     9,     7,     4,     6,     6,     9,     9,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     1,     4,     1,     3,     4,     1,     3,     4,     3,
       3,     1,     1,     2,     1,     2,     1,     1,     3,     1,
       2,     2,     2,     2,     2,     8,     8,     9,     9,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       3,     3,     3,     2,     2,     2,     1,     0,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     4,     4,     4,     3,     3,     3,     3,     5,     3,
       4,     3,     4,     1,     1,     1,     1,     1,     1,     1,
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
       3,     0,     0,     1,     0,   113,   619,   620,   621,   615,
     616,   622,     0,   540,    99,   134,   509,   141,   511,   540,
       0,   140,     0,   414,    99,     0,     0,     0,   240,   135,
     587,   587,   613,     0,     0,     0,     0,   139,    55,   241,
     292,   136,     0,     0,     0,   288,     0,     0,   143,     0,
     559,   531,   623,   144,     0,   293,   503,   413,     0,     0,
       0,   157,    48,   138,   512,   415,     0,     0,     0,     0,
     507,     0,     0,   142,     0,     0,   114,     0,   614,     0,
       0,     0,   137,   274,   505,   417,   145,     0,     0,   672,
       0,   674,     0,   675,   676,   586,     0,   673,   670,   490,
     154,   671,     0,     0,     0,     0,     4,     0,     5,     0,
       9,    50,    10,    11,     0,    12,    13,    14,    16,   486,
     487,    26,    15,   155,   164,   165,    17,    21,    18,    20,
       0,   235,    19,   578,    23,    24,    25,    22,   163,     0,
     161,     0,   575,     0,   159,   162,     0,   160,   592,   571,
     488,   572,   493,   491,     0,     0,     0,   576,   577,     0,
     492,     0,   593,   594,   595,   617,   618,   570,   495,   494,
     573,   574,     0,    42,    28,   501,     0,     0,   541,   100,
       0,     0,   511,   135,     0,     0,     0,     0,   512,     0,
       0,     0,     0,   575,   592,   491,   576,   577,   510,   492,
     593,   594,     0,   540,     0,     0,   416,     0,   248,     0,
     471,     0,   478,   615,   512,   588,   291,   615,   182,   512,
       0,   291,     0,     0,     0,     0,     0,     0,     0,   169,
       0,     0,    52,   478,   107,   115,   127,   121,   120,   129,
     110,   119,   130,   116,   131,   108,   132,   125,   118,   126,
     124,   122,   123,   109,   111,   117,   128,   133,     0,   112,
       0,     0,     0,     0,     0,     0,   420,     0,   151,    36,
       0,   657,   563,   560,   561,   562,     0,   504,   658,     7,
     478,   291,   272,   282,   587,   273,   156,   385,   468,     0,
     467,     0,     0,   152,   591,     0,     0,    39,     0,   508,
     496,     0,   478,    40,   502,     0,   255,   251,     0,   492,
     255,   252,     0,   410,     0,   506,     0,     0,   659,   661,
     584,   656,   655,     0,    57,    60,     0,     0,   473,     0,
     475,     0,     0,   474,     0,     0,   467,     0,   585,     0,
       6,     0,    51,     0,     0,     0,     0,   275,     0,   372,
     371,   294,     0,   489,    27,     0,   564,     0,     0,     0,
       0,     0,     0,   660,     0,     0,     0,     0,     0,     0,
     583,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   331,   338,   339,
     340,   335,   337,     0,     0,   333,   336,   334,   332,     0,
     342,   341,     0,     0,   478,   478,     0,     0,     0,    29,
      30,     0,     0,     0,     0,     0,     0,     0,    31,     0,
       0,     0,     0,    32,     0,    33,     0,   486,   484,     0,
     479,   480,   485,   176,     0,   179,     0,   172,     0,     0,
     178,     0,     0,     0,   191,     0,     0,   190,     0,   199,
       0,     0,     0,   197,     0,     0,    72,   166,     0,   331,
     211,     0,    54,    53,    50,     0,     0,     0,   219,    34,
     354,   289,   424,     0,   425,   427,     0,   449,     0,   430,
       0,     0,   150,    35,     0,     0,    37,     0,   158,     0,
      92,   589,   590,   153,     0,    38,   478,     0,   262,   253,
     249,   254,   250,     0,   408,   405,   184,    41,    59,    58,
      61,     0,   624,     0,     0,   609,     0,   611,     0,     0,
       0,     0,     0,     0,     0,     0,   628,     8,     0,    44,
       0,     0,    90,     0,    87,     0,    66,   246,     0,     0,
       0,   365,   419,   539,   652,   651,   654,   663,   662,   667,
     666,   648,   645,   646,   647,   580,   635,   113,     0,   606,
     607,   114,   605,   604,   581,   639,   650,   644,   642,   653,
     643,   641,   633,   638,   640,   649,   632,   636,   637,   634,
     582,     0,     0,     0,     0,     0,     0,     0,     0,   665,
     664,   669,   668,   551,   552,   554,   556,     0,   543,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   626,
     246,   540,   540,   187,   406,   418,   472,     0,     0,   498,
       0,     0,   275,     0,     0,   275,   407,     0,     0,     0,
       0,   515,     0,     0,     0,   200,     0,   521,     0,     0,
     198,   671,    75,     0,    62,    73,     0,   210,     0,     0,
       0,     0,   497,     0,   222,     0,   220,   359,   356,   357,
     358,   362,   363,   364,   354,     0,   347,     0,   355,   373,
       0,   428,     0,   148,   149,   147,   146,     0,   448,   447,
     571,     0,   422,   569,   421,     0,     0,   603,   470,   469,
       0,     0,     0,   499,     0,   256,   412,   571,     0,   625,
     579,   610,   476,   612,   477,   207,     0,     0,     0,   627,
     205,   525,     0,   630,   629,     0,    46,    45,    43,     0,
      86,     0,     0,     0,    79,     0,     0,    72,   243,     0,
     276,     0,     0,   286,     0,   283,   369,   366,   367,   370,
     295,     0,     0,    98,    96,    97,    95,    94,    93,   601,
     602,   553,   555,     0,   542,   134,   141,   140,   139,   136,
     143,   144,   138,   142,   137,   145,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   183,   482,   483,   481,   275,     0,   181,   275,     0,
     180,     0,   209,     0,     0,   189,     0,   188,     0,   546,
       0,     0,   195,     0,     0,   193,     0,     0,   167,     0,
     168,     0,     0,   213,     0,   215,     0,   245,     0,     0,
     360,   361,     0,   354,   345,     0,   462,   374,   377,   376,
     378,     0,   426,   429,   430,     0,     0,   431,   432,     0,
       0,   264,     0,     0,   263,   266,   500,     0,   257,   260,
       0,   409,   208,     0,     0,     0,     0,   206,     0,    91,
      88,     0,    69,    68,    67,     0,     0,     0,     0,   291,
     281,     0,   284,   280,   368,   373,   343,   101,   326,   115,
     324,   121,   120,   104,   119,   116,   329,   131,   102,   132,
     118,   122,   103,   105,   117,   133,   323,   305,   308,   306,
     307,   318,   309,   322,   314,   312,   325,   328,   313,   311,
     316,   321,   310,   315,   319,   320,   317,   327,     0,   304,
       0,   106,     0,   343,   343,   302,   608,   544,   430,   592,
     592,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,   186,   185,     0,   277,     0,   277,   192,     0,     0,
     514,     0,   513,     0,   545,     0,     0,   520,   196,     0,
     519,   194,    64,    63,   212,   530,   214,     0,     0,   242,
     221,   218,   462,   348,     0,     0,   430,   375,   389,   423,
     453,     0,   626,   478,   478,     0,   268,     0,     0,     0,
     258,     0,   203,   527,     0,     0,   201,   526,     0,   631,
       0,     0,     0,    72,     0,    72,    80,    83,   247,   271,
     157,   291,   270,   291,   278,   287,   285,     0,   354,   301,
       0,   330,     0,   297,   298,   548,     0,     0,     0,     0,
       0,     0,     0,     0,   247,   277,   291,   277,   291,   518,
       0,     0,   547,   524,     0,     0,     0,     0,   217,    56,
     430,   462,     0,   465,   464,   466,   571,   386,   351,   349,
       0,     0,     0,     0,   451,   571,     0,     0,   269,   267,
       0,   261,     0,   204,     0,   202,    89,    71,    70,     0,
       0,     0,     0,   244,   279,   444,     0,   379,     0,   303,
     101,   103,   343,   343,     0,     0,     0,     0,     0,     0,
     291,   175,   291,   171,     0,     0,     0,     0,    65,   216,
     350,   430,   454,     0,   388,   387,   403,     0,   404,   391,
     394,     0,   390,   383,   384,   290,     0,   565,   566,   259,
       0,     0,    82,    85,    81,    84,     0,   443,   442,   571,
     380,   389,   344,   299,   300,     0,     0,     0,     0,     0,
       0,   177,   173,   517,   516,   523,   522,   353,   352,   456,
     457,   459,   571,     0,   626,   402,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   571,   567,   568,   529,   528,
       0,   434,     0,     0,     0,   458,   460,   393,   395,   396,
     399,   400,   401,   397,   398,   392,   439,   437,   571,   626,
     381,   296,   382,   454,   438,   571,   461
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   325,   107,   647,   109,   110,   111,   343,
     464,   112,   232,   113,   326,   642,   535,   646,   643,   114,
     115,   116,   532,   533,   117,   118,   180,   919,   264,   119,
     259,   120,   678,   269,   121,   122,   281,   123,   124,   125,
     438,   624,   434,   621,   126,   127,   776,   128,   129,   655,
     656,   192,   131,   132,   133,   134,   135,   499,   695,   850,
     136,   137,   691,   845,   138,   139,   539,   869,   140,   141,
     734,   735,   193,   262,   669,   143,   144,   541,   875,   740,
     922,   923,   461,  1019,   471,   665,   666,   667,   668,   741,
     351,   831,  1141,  1201,  1125,   428,  1058,  1062,  1119,  1120,
    1121,   145,   314,   504,   146,   147,   265,   266,   475,   476,
     682,  1138,  1087,   479,   679,  1160,  1055,   976,   327,   209,
     331,   332,   429,   430,   431,   194,   149,   150,   151,   152,
     195,   154,   177,   178,   597,   450,   798,   598,   599,   155,
     156,   196,   197,   159,   221,   432,   199,   161,   200,   201,
     164,   165,   166,   167,   337,   168,   169,   170,   171,   172
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1099
static const yytype_int16 yypact[] =
{
   -1099,   175,  3244, -1099,   -49,   126, -1099, -1099, -1099, -1099,
   -1099, -1099,  4616,    74,   188,   234, 14216,   245, 19455,    74,
   10915,   266,   313,   228,   188,  4616, 10915,  4616,   166, 19507,
   11084,  8026,   290,  8195,  9217,  6666, 10915,   343, -1099,   292,
   -1099,   376, 19559, 19559, 19559, -1099,  3008,  9386,   429, 10915,
     148, -1099,   457,   460, 10915, -1099, 14216, -1099, 10915,   427,
     347,    53, 19764,   466, 19611, -1099,  9557,  7517, 10915,  9386,
   14216, 10915,   225,   475,   368,  4616,   493, 10915,   498, 11253,
   11253, 19559,   507, -1099, 14216, -1099,   510, 10915, 10915, -1099,
   10915, -1099, 10915, -1099, -1099, 13746, 10915, -1099, 10915, -1099,
   -1099, -1099,  3590,  6837,  8366, 10915, -1099,  4445, -1099,   392,
   -1099,   484, -1099, -1099,    55, -1099, -1099, -1099, -1099, -1099,
   -1099, -1099, -1099, -1099, -1099, -1099, -1099, -1099, -1099, -1099,
     514, -1099, -1099, -1099, -1099, -1099, -1099, -1099, -1099, 19559,
   -1099, 19559,   411,    62, -1099, -1099,  3008, -1099,   410, -1099,
     423, -1099, -1099,   428,   449,   436, 10915,   450,   462, 18946,
    2870,   249,   487,   488, -1099, -1099,   330, -1099, -1099, -1099,
   -1099, -1099,    14, -1099, -1099, 18946,   464,  4616, -1099, -1099,
     496, 10915, -1099, -1099, 10915, 10915, 10915, 19559, -1099, 10915,
    9557,  9557,   600,   467, -1099, -1099, -1099, -1099,   471,   470,
   -1099, -1099,   495, 15647, 19559,  3008, -1099,   505, -1099,   -65,
   18946,   552,  7688,   587, 19663, 18946,   448,   594, -1099, 19715,
   19559,   448, 19559,   513,    32, 15409,    30, 15333,   381, -1099,
    3068, 14627,   384,  7688, -1099, -1099, -1099, -1099, -1099, -1099,
   -1099, -1099, -1099, -1099, -1099, -1099, -1099, -1099, -1099, -1099,
   -1099, -1099, -1099, -1099, -1099, -1099, -1099, -1099,   518, -1099,
     477,  4616,   521,  1658,    94,     5, -1099,  4616, -1099, -1099,
   15723,   544, -1099,   522,   523, -1099, 15723,   471,   544, -1099,
    7688,  2195, -1099, -1099,  9726, -1099, -1099, -1099, -1099,   131,
   18946, 10915, 10915, -1099, 18946,   532, 16271, -1099, 15723,   471,
   18946,   533,  7688, -1099, 18946, 16347, -1099, -1099, 16427, 13739,
   -1099, -1099, 16507,   581,   543,   471, 15723, 16587,   182,   182,
    1651,   544,   544,    97, -1099, -1099,  3761,   -34, -1099, 10915,
   -1099,   -45,   -31, -1099,   112,    -3, 16667,   124,  1651,   710,
   -1099,  3932, -1099,   656, 10915, 10915, 19559,   580,   557, -1099,
   -1099, -1099,   140,   474, -1099, 10915,   584, 10915, 10915, 10915,
    9217,  9217, 10915,   504, 10915, 10915, 10915, 10915, 10915,   223,
   13746, 10915, 10915, 10915, 10915, 10915, 10915, 10915, 10915, 10915,
   10915, 10915, 10915, 10915, 10915, 10915, 10915,   663, -1099, -1099,
   -1099, -1099, -1099,  8535,  8535, -1099, -1099, -1099, -1099,  8535,
   -1099, -1099,  8535,  8535,  7688,  7688,  9217,  9217,  7348, -1099,
   -1099, 15803, 15879, 16743,   562,    41, 19559,  4103, -1099,  9217,
      32,   564,   243, -1099, 10915, -1099, 10915,   610, -1099,   567,
     591, -1099, -1099, -1099, 19559, -1099,  3008, -1099, 19559,   576,
   -1099,  3008,   692,  9557, -1099,  4787,  9217, -1099,   571, -1099,
      32,  4958,  9217, -1099,    32, 11422, 10915, -1099,  4616, -1099,
     706, 10915, -1099, -1099,   484,   574,  7688, 19559, -1099, -1099,
     553, -1099, -1099,  1658, -1099,   602,   578, -1099, 11591,   624,
   10915,  3008, -1099, -1099, 10915, 10915, -1099,   579, -1099,  9557,
   -1099, 18946, 18946, -1099,    44, -1099,  7688,   583, -1099,   732,
   -1099,   732, -1099, 11760,   617, -1099, -1099, -1099, -1099, -1099,
   -1099,  8706, -1099, 16823,  7008, -1099,  7179, -1099,  4616,   596,
    9217,  8877,  3419,   597, 10915,  9895, -1099, -1099,   284, -1099,
    4274, 19559, -1099,   264, 16905,   308, 15491,   113,  9557,   603,
   19403,   300, -1099, 16981, 19193, 19193,   473, -1099,   473, -1099,
     473,  2992,  1012,  1307,  1915,   471,   182, -1099,   604, -1099,
   -1099, -1099, -1099, -1099,  1651,  2425,   473,  1261,  1261, 19193,
    1261,  1261,  1118,   182,  2425, 19234,  1118,   544,   544,   182,
    1651,   601,   614,   615,   616,   619,   620,   622,   627, -1099,
     473, -1099,   473,   207, -1099, -1099, -1099,   109, -1099,   659,
   19022,   295, 11929,  9217, 12098,  9217, 10915,  7688,  9217,  2660,
     618,    74, 17061, -1099, -1099, -1099, 18946, 17137,  7688, -1099,
    7688, 19559,   580,   334, 19559,   580, -1099,   374, 10915,   155,
   10915, 18946,    38, 15959,  7348, -1099, 10915, 18946,    37, 15567,
   -1099,   632,   643,   638, 17217,   643,   641,   754, 17293,  4616,
   16035,   742, -1099,   170, -1099,   241, -1099,   214, -1099, -1099,
   -1099, -1099, -1099, -1099,   767,   247, -1099, 14447, -1099,   432,
     642,  1658,    94,   -18,    -4, 10915, 10915,  6497, -1099, -1099,
     382,  7857, -1099, 18946, -1099, 17373, 17449, -1099, -1099, 18946,
     647,   146,   646, -1099,  2877, -1099, -1099,   408, 19559, -1099,
   -1099, -1099, -1099, -1099, -1099, -1099,  4616,   132, 16115, -1099,
   -1099, 18946,  4616, 18946, -1099, 17531, -1099, -1099, -1099, 10915,
   -1099,   107,   125, 10915, -1099, 10064, 11422, 10915, -1099,  7688,
     668,  2393,   649,   697,   169, -1099,   733, -1099, -1099, -1099,
   -1099, 13582,   651, -1099, -1099, -1099, -1099, -1099, -1099, -1099,
   -1099, -1099, -1099,  7348, -1099, -1099, -1099, -1099, -1099, -1099,
   -1099, -1099, -1099, -1099, -1099, -1099,    40,  9217,  9217, 10915,
     789, 17607, 10915,   791, 17687,   258,   657, 17763,  7688,    32,
      32, -1099, -1099, -1099, -1099,   580,   661, -1099,   580,   664,
   -1099, 15723, -1099, 14703,  5129, -1099,  5300, -1099,   260, -1099,
   14783,  5471, -1099,    32,  5642, -1099,    32, 10915, -1099, 10915,
   -1099,  4616, 10915, -1099,  4616,   798, 19559,   672, 19559,   495,
   -1099, -1099, 19559,  1027, -1099,  1658,   693,   749, -1099, -1099,
   -1099,    48, -1099, -1099,   624,   666,    60, -1099, -1099,   679,
     682, -1099,  5813,  9557, -1099, -1099, -1099, 19559, -1099,   699,
     495, -1099, -1099,  5984,   680,  6155,   683, -1099, 10915, -1099,
   -1099, 10915, 17843,    49, 16191,   695,   700,   267,   696,  1512,
   -1099, 10915, 19559, -1099, -1099,   432,   701,   476, -1099,   716,
   -1099,   723,   729,   738,   731,   734, -1099,   735,   741,   739,
     743,   747,   490,   746,   748,   752, -1099, -1099, -1099, -1099,
   -1099, -1099, -1099, -1099, -1099, -1099, -1099, -1099, -1099, -1099,
   -1099, -1099, -1099, -1099, -1099, -1099, -1099, -1099, 10915, -1099,
     750,   761,   755,   701,   701, -1099, -1099, -1099,   624,   273,
     278, 17919, 12267, 12436, 17999, 12605, 12774, -1099, 12943, 13112,
     337, -1099, -1099,   702, -1099,   736, -1099, -1099,  4616, 10915,
   18946, 10915, 18946,  7348, -1099,  4616, 10915, 18946, -1099, 10915,
   18946, -1099, 18081, 18946, -1099, 18946,   869,  4616,   744, -1099,
   -1099, -1099,   693, -1099,   740, 10235,   280, -1099,   202, -1099,
   -1099,  9217, 14350,  7688,  7688,  4616, -1099,    29,   753, 10915,
   -1099, 10915, -1099, 18946,  4616, 10915, -1099, 18946,  4616, 18946,
     271, 10404, 11422, 10915, 11422, 10915, -1099, -1099,   758, -1099,
   -1099,  2195, -1099, 19373, -1099, 18946, -1099,   128,   553, -1099,
   18157, -1099, 14545, -1099, -1099, -1099, 10915, 10915, 10915, 10915,
   10915, 10915, 10915, 10915, -1099, -1099,  1813, -1099,  2765, 17293,
   14859, 14939, -1099, 17293, 15015, 15095, 10915,  4616, -1099, -1099,
     624,   693,  9048, -1099, -1099, -1099,   311,  9557, -1099, -1099,
     153, 10915,   -15, 18238, -1099,   715,   756,   759,   552, -1099,
     495, 18946, 15171, -1099, 15251, -1099, -1099, -1099, 18946,   768,
     769,   770,   772, -1099, -1099, -1099, 13281,   803,   377, -1099,
     771,   774,   701,   701, 18319, 18395, 18475, 18551, 18631, 18707,
   19161, -1099, 19178, -1099,  4616,  4616,  4616,  4616, 18946, -1099,
   -1099,   280, 10575,    63, -1099, 18946, -1099,   138, -1099,   133,
   -1099,   506, 18790, -1099, -1099, -1099, 13112,   751,   775, -1099,
    4616,  4616, -1099, -1099, -1099, -1099,  6326, -1099, -1099,   237,
   -1099,   202, -1099, -1099, -1099, 10915, 10915, 10915, 10915, 10915,
   10915, -1099, -1099, 17293, 17293, 17293, 17293, -1099, -1099, -1099,
   -1099, -1099,   316,  9217, 13914, -1099, 10915,   153,   138,   138,
     138,   138,   138,   138,   153,   724, -1099, -1099, 17293, 17293,
     737, 13450,    91,   -41, 18866, -1099, -1099, 18946, -1099, -1099,
   -1099, -1099, -1099, -1099, -1099,   792, -1099, -1099,   364, 14082,
   -1099, -1099, -1099, 10746, -1099,   486, -1099
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1099, -1099, -1099,     3,  -368,  1841, -1099, -1099, -1099,   478,
   -1099, -1099, -1099,   262,   599,  -450, -1099,  -701,  -696, -1099,
   -1099, -1099,   226, -1099, -1099,     9,   924, -1099,  2418,  -159,
    -724, -1099,  -927,   582,  -991,  -837, -1099,   -61, -1099, -1099,
   -1099, -1099, -1099, -1099, -1099, -1099,   121, -1099, -1099, -1099,
     134,  1023, -1099, -1099, -1099, -1099, -1099,   639, -1099,   104,
   -1099, -1099, -1099, -1099, -1099, -1099,  -591,  -659, -1099, -1099,
   -1099,    81,  1171, -1099, -1099, -1099,    33, -1099, -1099, -1099,
   -1099,   -68,  -157,  -885, -1099,   -63,   135,   293, -1099, -1099,
   -1099,    84, -1099, -1099,  -223,    28,  -150,  -179,  -211,  -203,
    -255, -1099,  -193, -1099,    11,   930,  -126,   485, -1099,  -448,
    -806, -1032, -1099,  -643,  -488, -1098, -1060,  -891,   -64, -1099,
     161, -1099,  -220,  -459,   348,   529,  -377, -1099, -1099, -1099,
    1192, -1099,   -10, -1099, -1099,  -217, -1099,  -618, -1099, -1099,
   -1099,  1387,  1401,   -12,   937,   183,   671, -1099,  1967,  2064,
   -1099, -1099, -1099, -1099, -1099, -1099, -1099, -1099, -1099,  -396
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -627
static const yytype_int16 yytable[] =
{
     175,   286,   289,   403,   198,   106,   645,   653,   203,   202,
     454,   421,   601,   465,   210,   696,   799,   921,   215,   215,
     352,   225,   227,   230,   231,   670,   866,   439,   979,   834,
     865,   786,  1014,   205,   789,   270,  -240,   271,  1023,  1024,
     335,   477,   276,   520,   277,   690,   278,    67,  1053,   477,
    -241,   480,  1161,   427,   290,   294,   296,   298,   299,   300,
     487,   267,   267,   304,   267,   305,  1185,   308,   312,   801,
     794,  1124,   315,    67,   427,   316,   317,   424,   318,   422,
     319,  1050,   497,   320,   321,  1002,   322,   608,   307,   311,
     290,   294,   336,   338,   288,   477,   425,   514,   -78,   406,
     344,   680,   664,   407,  1186,  1206,   981,  -445,   511,  1163,
     557,   516,   173,   448,   349,  -265,   -49,   519,   523,  1003,
    1200,   427,  1025,   928,   515,   417,   697,   415,   557,  1085,
     288,   330,   288,   512,   -49,   927,  -445,   608,   517,   521,
    -445,  1116,   350,   427,   356,   558,   448,   481,   775,  1197,
     212,   417,   403,   448,   224,   226,  1116,  -265,   559,  1137,
    1111,  -445,  -445,   -78,   233,   345,   272,  1204,   522,   175,
    1059,   489,   411,   412,   413,     3,  1084,   304,   290,   336,
    -223,   478,  -445,   521,   587,   588,   560,  -440,   978,   478,
     176,   179,  1124,   420,   943,   417,   417,   945,   417,  1014,
     294,  1014,   521,   417,   417,   521,   561,  1143,  1144,  -445,
     563,  -265,   609,   359,  -445,  -445,  -440,   842,   288,   288,
     488,   294,   273,   833,   561,   334,   557,   436,   448,  -440,
     562,   982,   441,   521,  1164,   478,  -445,  1118,   601,   205,
     274,  1117,  -440,   558,  1110,   427,   427,  1166,   448,   435,
     295,   753,  1118,   751,   440,  -445,   559,   275,   173,   843,
     820,  1060,  1199,  1014,   508,  1014,   525,   360,   294,  1086,
     867,   361,   215,   489,   728,  1167,   692,   645,   754,   491,
     492,   729,   481,   518,   560,  1036,   333,  1038,  -224,  -440,
     294,   861,   490,   752,  -440,   526,  -441,   489,   921,  -230,
     821,   542,  1080,   853,  1082,  1158,  1079,   427,  1081,   268,
     623,   872,   620,   844,   206,   627,  1061,   513,   363,   940,
    -229,   301,   561,   367,   792,  -441,   369,   736,   562,   510,
     372,   268,   534,   536,   212,  1042,   873,   427,  -441,   817,
      23,   306,   310,   543,  -238,   544,   545,   546,   548,   550,
     551,  -441,   552,   553,   554,   555,   556,   601,   564,   565,
     566,   567,   568,   569,   570,   571,   572,   573,   574,   575,
     576,   577,   578,   579,   580,   737,  1100,   974,  1102,   629,
     768,   294,   294,   818,   407,   481,   738,   294,    57,   823,
     294,   294,   294,   294,   590,   592,   600,  -228,  -441,    65,
     620,   681,   953,  -441,   615,   739,   719,   612,    23,   620,
     819,  1000,   616,   807,   617,  -550,   824,   404,   204,   405,
    -549,   803,   806,  -433,    85,   720,  1057,   937,  -446,   954,
    -225,   290,  -463,   631,   633,   279,  1008,  -455,  1076,   637,
     639,  -446,  -550,   644,   644,   173,   648,  -549,   427,   650,
     723,   716,  -433,  -463,   294,   664,    57,  -463,  -455,   827,
     233,   427,  -455,   462,   418,  -433,   463,    65,   683,   724,
    -446,   288,   685,   686,   730,    23,   481,   689,  -433,   620,
    -463,   547,   549,  -232,   294,  -455,   268,   268,   268,   268,
     854,   856,    85,   460,  1064,   787,  -446,    40,  -600,   689,
    -600,  1013,   294,  -446,   294,   851,  1034,   828,   708,   689,
     711,  -236,   713,   715,  -233,   280,   481,   688,   829,   823,
    -227,    55,   468,    57,  -446,  -433,   290,   589,   591,  -231,
    -433,   148,   268,   510,    65,   790,   302,   830,   268,   688,
     611,   148,   330,  -446,   330,  -435,  1142,  -237,  -446,   688,
    -411,  -446,  -239,   645,   148,   645,   148,   601,   341,    85,
     268,  -226,   860,   563,  -234,   342,   288,   632,   346,  -411,
     427,  -533,  -536,   638,  -435,   359,   582,   583,   268,  -599,
     657,  -599,   584,   353,   924,   585,   586,  -435,   355,  -532,
     631,   771,   637,   774,   648,   294,   777,   711,  1056,   658,
    -435,   779,   780,   659,   148,  1065,   294,   363,   294,   363,
     354,  -537,  -107,   836,   367,   369,   791,   369,   793,   427,
    -113,   372,   600,  -538,   800,   660,  -109,  1168,   661,   360,
      59,   148,   408,   361,  -114,  -599,   148,  -599,  -596,   662,
    -596,   707,  -598,   417,  -598,   467,   782,  -435,  -535,  -534,
     664,   297,  -435,  1169,   416,  1170,  1171,   410,   663,  1172,
    1173,   417,   557,   300,   304,   336,   423,   426,  1013,   294,
    1013,   433,  -597,   160,  -597,   702,   755,   704,   437,   756,
     363,   443,   268,   160,   757,   367,   466,   368,   369,   470,
     484,   485,   372,   493,   183,  -346,   160,   333,   160,   333,
     379,   496,   503,   758,   505,   288,   148,   534,   385,  1139,
     759,   862,   268,   864,   644,   644,   268,   294,   527,   531,
     760,   538,  -346,   540,   770,   614,   773,   761,   369,   581,
     607,   618,  1013,   620,  1013,  1162,   619,   626,   628,   634,
     649,   600,   762,   652,   671,   681,   160,   672,   687,  1175,
     309,   309,   693,   694,   763,   548,   590,   931,   561,   698,
     934,  -450,   743,  1066,  1067,   764,   294,   706,   712,   731,
    -452,   765,   742,   160,  -450,   744,   745,   746,   160,   987,
     747,   748,   950,  -452,   952,   807,   778,  1162,   811,   957,
     148,   749,   960,   -74,   657,   962,   148,   963,   750,   808,
     965,   783,   810,  -450,  1198,   816,   444,   447,   449,   453,
     489,   832,  -452,   658,   841,   846,   870,   659,   871,   874,
     926,   932,  1205,   935,   427,   427,  1162,   944,   938,  -450,
     946,   290,   967,   969,   975,   977,  -450,   980,  -452,   660,
     989,   993,   661,   997,   339,  -452,   999,   983,   160,   644,
     984,   994,   483,   662,   998,   148,  1006,  -450,   486,  1015,
    -134,  1007,  1165,  1009,   838,  1093,  -452,  -141,  1035,  1018,
     148,   288,   663,  -140,  -110,  -139,  -450,  -108,  -136,  -143,
     495,  -450,  -111,  -144,  -450,  -452,  1021,  -138,   547,   589,
    -452,  -142,  -137,  -452,   268,   268,  -145,  -112,   506,  1022,
     268,   268,  1037,  1047,  1140,  1049,  1020,  -107,  1196,  1051,
    -109,  1176,   815,  1189,  1190,  1191,  1192,  1193,  1194,  1083,
     950,   952,  1070,   957,   960,  1127,   993,   997,  1128,  1132,
    1133,  1134,   160,  1135,  1167,  1177,  1039,  1040,   160,  1041,
     530,   600,   651,  1043,  1044,   859,   148,  1045,   207,   501,
     286,   988,   970,  1016,  1092,  1088,   848,   822,   973,  1017,
    1202,  1157,  1183,  1195,  1188,   229,   684,   216,   784,  1063,
     711,   294,   294,     0,   148,     0,     0,  1071,     0,  1072,
     148,     0,     0,  1074,     0,     0,     0,   148,  1113,  1078,
     644,   644,   644,   644,     0,     0,     0,   160,     0,     0,
       0,     0,   613,  1054,     0,     0,     0,     0,     0,     0,
       0,     0,   160,     0,  1039,  1094,  1095,  1043,  1096,  1097,
    1098,  1099,     0,     0,     0,   130,     0,     0,     0,     0,
       0,     0,   635,     0,  1108,   130,   640,     0,     0,     0,
     336,   268,   268,   359,     0,  1115,     0,   148,   130,  1122,
     130,   148,     0,   268,   657,   460,     0,     0,     0,   148,
       0,     0,   460,     0,     0,   268,     0,     0,   268,     0,
       0,     0,  1182,   658,     0,     0,     0,   659,     0,     0,
     288,   971,     0,     0,     0,  1114,     0,     0,   160,     0,
       0,     0,  1153,  1154,  1155,  1156,     0,   360,   130,   660,
       0,   361,   661,     0,    59,     0,     0,     0,     0,     0,
       0,     0,   990,   662,   997,     0,   160,     0,  1178,  1179,
       0,     0,   160,     0,   336,   130,     0,     0,   766,   160,
     130,     0,   663,  1153,  1154,  1155,  1156,  1178,  1179,     0,
    1159,     0,     0,     0,     0,     0,     0,     0,   363,   359,
       0,  1184,   711,   367,  1187,   368,   369,     0,     0,     0,
     372,     0,     0,     0,   288,     0,     0,     0,   379,     0,
       0,     0,     0,   142,   383,   384,   385,     0,   148,     0,
       0,     0,     0,   142,     0,     0,     0,   711,     0,   160,
       0,   997,  1159,   160,   153,     0,   142,     0,   142,     0,
     130,   160,     0,   360,   153,     0,     0,   361,     0,     0,
       0,     0,     0,     0,   795,   797,     0,   153,     0,   153,
     802,   805,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1159,     0,     0,     0,   148,     0,     0,     0,     0,
       0,   148,     0,     0,     0,     0,   142,     0,     0,   268,
       0,     0,     0,     0,   363,   364,     0,   365,   366,   367,
       0,   368,   369,     0,     0,     0,   372,   153,     0,     0,
     925,     0,     0,   142,   379,     0,     0,     0,   142,     0,
     383,   384,   385,     0,   130,     0,     0,     0,     0,     0,
     130,     0,   359,     0,   153,     0,   929,   930,     0,   153,
       0,     0,   460,   460,     0,     0,   460,   460,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     160,     0,     0,   148,  1123,   148,     0,     0,     0,     0,
     148,     0,  1129,   148,   460,     0,   460,     0,   359,     0,
     148,     0,     0,   148,     0,     0,   360,     0,   142,   130,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   941,   942,     0,   130,     0,     0,     0,     0,   153,
       0,   148,     0,   947,     0,     0,     0,   160,     0,     0,
       0,     0,   148,   160,   148,   958,     0,     0,   961,   157,
       0,     0,   360,     0,     0,     0,   361,   363,   364,   157,
     365,   366,   367,   158,   368,   369,   370,     0,     0,   372,
       0,     0,   157,   158,   157,     0,   378,   379,     0,     0,
     382,     0,     0,   383,   384,   385,   158,     0,   158,     0,
       0,     0,   142,     0,   386,     0,     0,     0,   142,     0,
     130,     0,     0,   363,   364,  1123,     0,   366,   367,     0,
     368,   369,     0,   153,     0,   372,     0,     0,     0,   153,
       0,     0,   157,   379,     0,   160,     0,   160,   130,   383,
     384,   385,   160,     0,   130,   160,   158,   148,     0,     0,
       0,   130,   160,     0,   148,   160,     0,     0,     0,   157,
       0,     0,     0,     0,   157,     0,   148,   142,     0,     0,
       0,     0,     0,   158,     0,     0,     0,     0,   158,     0,
       0,     0,   142,   160,   148,     0,     0,     0,   153,     0,
       0,     0,     0,   148,   160,     0,   160,   148,     0,     0,
       0,     0,     0,   153,   282,     0,     0,     0,    22,    23,
       0,   130,     0,     0,     0,   130,     0,   283,     0,    30,
     284,     0,     0,   130,    35,     0,     0,     0,     0,     0,
       0,    40,     0,     0,   157,     0,     0,     0,     0,  1069,
       0,     0,     0,     0,     0,     0,   148,     0,   158,     0,
       0,     0,     0,     0,     0,    55,     0,    57,   142,    59,
       0,  1010,     0,     0,  1011,     0,   285,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   153,
       0,     0,     0,     0,     0,     0,   142,    81,     0,   160,
      83,     0,   142,    85,     0,     0,   160,     0,     0,   142,
       0,     0,     0,   148,   148,   148,   148,   153,   160,     0,
       0,     0,     0,   153,     0,     0,     0,     0,   157,     0,
     153,     0,     0,     0,   157,     0,   160,     0,     0,   148,
     148,   234,   158,     0,     0,   160,     0,     0,   158,   160,
       0,     0,   130,   100,     0,   235,   236,     0,   237,  1012,
       0,     0,   359,   238,     0,     0,     0,     0,     0,   142,
       0,   239,     0,   142,     0,     0,     0,   240,     0,     0,
       0,   142,   241,     0,     0,     0,   242,     0,     0,   243,
     153,     0,     0,   157,   153,     0,     0,     0,   160,   244,
       0,     0,   153,     0,     0,   245,   246,   158,   157,   130,
       0,     0,   247,     0,     0,   130,   360,     0,     0,     0,
     361,   248,   158,     0,     0,     0,     0,     0,     0,     0,
     249,   250,     0,   251,     0,   252,     0,   253,     0,     0,
     254,     0,     0,     0,   255,   472,     0,   256,     0,     0,
     257,     0,     0,     0,     0,   160,   160,   160,   160,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,     0,     0,     0,   372,
       0,   160,   160,     0,   157,     0,   378,   379,     0,     0,
     382,     0,     0,   383,   384,   385,     0,   130,   158,   130,
     142,     0,     0,     0,   130,     0,   473,   130,     0,     0,
       0,     0,   157,     0,   130,   282,     0,   130,   157,    22,
      23,   153,     0,   108,     0,   157,   158,     0,   283,     0,
      30,   284,   158,   174,     0,    35,     0,     0,     0,   158,
       0,     0,    40,     0,     0,   130,   208,     0,   211,     0,
       0,     0,     0,     0,     0,     0,   130,   142,   130,     0,
       0,     0,     0,   142,     0,     0,    55,     0,    57,     0,
      59,     0,  1010,     0,     0,  1011,     0,   285,   153,    65,
       0,     0,     0,     0,   153,   157,     0,     0,     0,   157,
       0,     0,     0,     0,     0,     0,   303,   157,    81,   158,
       0,    83,     0,   158,    85,     0,     0,     0,     0,     0,
       0,   158,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   108,     0,     0,   359,     0,   340,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   142,     0,   142,     0,   162,
       0,   130,   142,     0,   100,   142,     0,     0,   130,   162,
    1101,     0,   142,     0,     0,   142,   153,     0,   153,     0,
     130,     0,   162,   153,   162,     0,   153,     0,     0,     0,
     360,     0,     0,   153,   361,     0,   153,     0,   130,     0,
       0,     0,     0,   142,     0,     0,     0,   130,   409,     0,
       0,   130,     0,     0,   142,     0,   142,     0,     0,     0,
       0,     0,     0,     0,   153,     0,   157,     0,     0,     0,
       0,     0,   162,     0,     0,   153,     0,   153,     0,     0,
     158,   363,   364,     0,     0,     0,   367,     0,   368,   369,
       0,     0,     0,   372,     0,     0,   163,     0,     0,   162,
     130,   379,     0,     0,   162,     0,   163,   383,   384,   385,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   163,
       0,   163,     0,   157,     0,     0,     0,     0,     0,   157,
       0,     0,   469,     0,     0,     0,     0,   158,   482,     0,
       0,     0,     0,   158,     0,     0,     0,     0,     0,   142,
       0,     0,     0,     0,     0,     0,   142,   130,   130,   130,
     130,     0,     0,     0,     0,     0,     0,     0,   142,   163,
     153,     0,     0,     0,   162,     0,     0,   153,     0,     0,
       0,     0,     0,   130,   130,     0,   142,     0,     0,   153,
       0,     0,     0,     0,     0,   142,   163,   108,     0,   142,
       0,   163,     0,     0,     0,     0,     0,   153,     0,     0,
       0,   157,   108,   157,     0,     0,   153,     0,   157,     0,
     153,   157,     0,     0,     0,   158,     0,   158,   157,     0,
       0,   157,   158,     0,     0,   158,     0,     0,     0,     0,
       0,     0,   158,     0,     0,   158,     0,   282,   142,     0,
       0,    22,    23,     0,     0,     0,     0,     0,   162,   157,
     283,     0,    30,   284,   162,     0,     0,    35,     0,   153,
     157,   163,   157,   158,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   158,     0,   158,     0,   108,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,     0,     0,     0,   142,   142,   142,   142,   285,
       0,    65,     0,     0,     0,     0,   482,     0,     0,     0,
       0,     0,   482,   162,     0,     0,   153,   153,   153,   153,
      81,   142,   142,    83,     0,     0,    85,     0,   162,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   153,   153,     0,   163,     0,     0,     0,     0,
       0,   163,     0,     0,     0,   157,     0,     0,     0,     0,
       0,     0,   157,     0,     0,     0,     0,     0,     0,   158,
       0,     0,     0,     0,   157,     0,   158,     0,     0,   705,
       0,     0,     0,   710,     0,     0,     0,     0,   158,     0,
       0,   108,   157,     0,     0,     0,     0,     0,     0,     0,
       0,   157,     0,     0,   162,   157,   158,     0,     0,     0,
     163,     0,     0,     0,   868,   158,     0,     0,     0,   158,
       0,     0,     0,     0,     0,   163,     0,     0,     0,     0,
       0,     0,   162,     0,     0,  -277,     0,     0,   162,  -277,
    -277,     0,     0,     0,     0,   162,     0,     0,  -277,     0,
    -277,  -277,     0,     0,   157,  -277,     0,     0,     0,     0,
       0,     0,  -277,     0,     0,  -277,     0,     0,   158,     0,
       0,     0,     0,     0,     0,     0,   359,     0,     0,     0,
     258,   260,   261,     0,     0,     0,  -277,     0,  -277,     0,
    -277,     0,  -277,  -277,     0,  -277,     0,  -277,     0,  -277,
       0,   163,     0,     0,     0,   162,     0,     0,     0,   162,
     813,   157,   157,   157,   157,     0,     0,   162,  -277,   313,
       0,  -277,     0,     0,  -277,   158,   158,   158,   158,   163,
     360,     0,     0,     0,   361,   163,     0,   157,   157,     0,
       0,     0,   163,     0,     0,     0,     0,     0,     0,     0,
       0,   158,   158,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   852,     0,     0,
       0,     0,     0,   857,  -277,     0,     0,   347,     0,   348,
    -277,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,     0,     0,   372,   373,   374,     0,     0,   376,   377,
     378,   379,   163,     0,   382,     0,   163,   383,   384,   385,
       0,     0,     0,     0,   163,     0,     0,     0,   386,     0,
       0,     0,     0,     0,     0,   414,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   162,     0,     0,     0,
       0,     0,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   482,     0,   482,   313,     0,
     442,     0,   482,     0,     0,   482,     0,     0,     0,     0,
       0,     0,   964,     0,     0,   966,     0,     0,     0,     0,
       0,     0,     0,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,   162,   181,     0,     0,    15,    16,   162,
      17,   474,   182,   986,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,   992,   183,   996,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,   163,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,   162,    78,   162,   537,     0,    82,     0,   162,    84,
     163,   162,    86,     0,     0,     0,   163,     0,   162,     0,
       0,   162,     0,     0,     0,     0,     0,   282,     0,     0,
       0,    22,    23,     0,     0,     0,     0,     0,    92,     0,
     283,     0,    30,   284,     0,     0,     0,    35,  1048,   162,
       0,     0,     0,     0,    40,     0,     0,     0,     0,     0,
     162,     0,   162,     0,     0,     0,  1068,     0,   103,     0,
     191,     0,     0,     0,   610,  1073,     0,     0,    55,  1075,
      57,     0,    59,     0,  1010,     0,     0,  1011,     0,   285,
       0,    65,   622,     0,     0,     0,   625,     0,   163,     0,
     163,     0,     0,     0,     0,   163,     0,     0,   163,     0,
      81,     0,     0,    83,     0,   163,    85,     0,   163,     0,
     234,     0,     0,     0,     0,   654,     0,     0,  1109,     0,
       0,   474,     0,     0,   235,   236,     0,   237,     0,     0,
       0,     0,   238,     0,     0,     0,   163,     0,     0,     0,
     239,     0,     0,     0,     0,   162,   240,   163,     0,   163,
       0,   241,   162,     0,     0,   242,   100,     0,   243,     0,
       0,     0,  1103,     0,   162,     0,     0,     0,   244,     0,
       0,     0,     0,     0,   245,   246,     0,     0,     0,   718,
       0,   247,   162,     0,     0,     0,     0,     0,   733,     0,
     248,   162,     0,     0,     0,   162,     0,     0,     0,   249,
     250,     0,   251,     0,   252,     0,   253,     0,     0,   254,
       0,     0,     0,   255,     0,     0,   256,     0,     0,   257,
       0,   387,   388,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,   400,   401,     0,     0,     0,     0,
       0,   234,   163,     0,   162,     0,     0,     0,     0,   163,
       0,     0,  -557,   359,     0,   235,   236,     0,   237,     0,
       0,   163,     0,   238,     0,   402,     0,     0,  -596,   785,
    -596,   239,   788,   417,     0,   847,     0,   240,     0,   163,
       0,     0,   241,     0,     0,     0,   242,     0,   163,   243,
       0,     0,   163,     0,     0,     0,     0,     0,     0,   244,
       0,   162,   162,   162,   162,   245,   246,   360,     0,     0,
     357,   361,   247,     0,     0,   826,     0,   358,     0,   474,
       0,   248,     0,     0,     0,     0,     0,   162,   162,   359,
     249,   250,     0,   251,   455,   252,     0,   253,     0,     0,
     254,   163,   849,     0,   255,     0,   313,   256,     0,     0,
     257,     0,     0,     0,     0,     0,     0,     0,   363,   364,
       0,   365,   366,   367,     0,   368,   369,   370,   456,   371,
     372,   373,   374,     0,     0,   376,   377,   378,   379,   380,
       0,   382,     0,   360,   383,   384,   385,   361,     0,   920,
       0,     0,     0,     0,     0,   386,     0,     0,   163,   163,
     163,   163,     0,     0,     0,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,   163,   163,     0,     0,     0,     0,
       0,     0,     0,     0,   363,   364,     0,   365,   366,   367,
       0,   368,   369,   370,     0,   371,   372,   373,   374,   375,
       0,   376,   377,   378,   379,   380,   381,   382,     0,   457,
     383,   384,   385,     0,   968,     0,   654,     0,     0,     0,
     972,   386,     0,   474,    -2,     4,     0,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,   849,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,   -76,
     733,    38,    39,    40,     0,    41,  -291,     0,    42,    43,
      44,    45,    46,     0,    47,    48,    49,   -47,    50,    51,
       0,    52,    53,    54,     0,     0,     0,    55,    56,    57,
      58,    59,    60,    61,  -291,   -47,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,    75,     0,    76,    77,     0,    78,    79,    80,    81,
      82,     0,    83,    84,   -76,    85,    86,     0,     0,    87,
       0,    88,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   100,     0,     0,   101,     0,
     102,     0,   103,     0,   104,     0,     0,   105,     0,     0,
       4,     0,     5,     0,     6,     7,     8,     9,    10,    11,
       0,  -626,     0,    12,    13,    14,    15,    16,  -626,    17,
     920,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    -626,    27,    28,  -626,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,   -76,     0,    38,    39,    40,     0,
      41,  -291,     0,    42,    43,    44,    45,    46,     0,    47,
      48,    49,   -47,    50,    51,     0,    52,    53,    54,     0,
       0,     0,    55,    56,    57,    58,     0,    60,    61,  -291,
     -47,    62,    63,    64,  -626,    65,    66,    67,  -626,    68,
      69,    70,    71,    72,    73,    74,    75,     0,    76,    77,
       0,    78,    79,    80,    81,    82,     0,    83,    84,   -76,
      85,    86,     0,     0,    87,     0,    88,     0,     0,  -626,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -626,  -626,    92,  -626,  -626,
    -626,  -626,  -626,  -626,  -626,     0,  -626,  -626,  -626,  -626,
    -626,     0,  -626,  -626,  -626,  -626,  -626,  -626,  -626,  -626,
     100,  -626,  -626,  -626,     0,   102,  -626,   103,     0,   104,
       0,   323,  -626,     5,   287,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,   -76,     0,    38,    39,    40,
       0,    41,  -291,     0,    42,    43,    44,    45,    46,     0,
      47,    48,    49,   -47,    50,    51,     0,    52,    53,    54,
       0,     0,     0,    55,    56,    57,    58,    59,    60,    61,
    -291,   -47,    62,    63,    64,     0,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,     0,    83,    84,
     -76,    85,    86,     0,     0,    87,     0,    88,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,   100,     0,     0,   101,     0,   102,   324,   103,     0,
     104,     0,     4,   105,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,   -76,     0,    38,    39,
      40,     0,    41,  -291,     0,    42,    43,    44,    45,    46,
       0,    47,    48,    49,   -47,    50,    51,     0,    52,    53,
      54,     0,     0,     0,    55,    56,    57,    58,    59,    60,
      61,  -291,   -47,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,     0,    83,
      84,   -76,    85,    86,     0,     0,    87,     0,    88,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,   509,   103,
       0,   104,     0,   528,   105,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,   -76,     0,    38,
      39,    40,     0,    41,  -291,     0,    42,    43,    44,    45,
      46,     0,    47,    48,    49,   -47,    50,    51,     0,    52,
      53,    54,     0,     0,     0,    55,    56,    57,    58,    59,
      60,    61,  -291,   -47,    62,    63,    64,     0,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,     0,
      83,    84,   -76,    85,    86,     0,     0,    87,     0,    88,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,   100,     0,     0,   101,     0,   102,   529,
     103,     0,   104,     0,   323,   105,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,   -76,     0,
      38,    39,    40,     0,    41,  -291,     0,    42,    43,    44,
      45,    46,     0,    47,    48,    49,   -47,    50,    51,     0,
      52,    53,    54,     0,     0,     0,    55,    56,    57,    58,
      59,    60,    61,  -291,   -47,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -76,    85,    86,     0,     0,    87,     0,
      88,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
     324,   103,     0,   104,     0,     4,   105,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,   -76,
       0,    38,    39,    40,     0,    41,  -291,     0,    42,    43,
      44,    45,    46,     0,    47,    48,    49,   -47,    50,    51,
       0,    52,    53,    54,     0,     0,     0,    55,    56,    57,
      58,    59,    60,    61,  -291,   -47,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,    75,     0,    76,    77,     0,    78,    79,    80,    81,
      82,     0,    83,    84,   -76,    85,    86,     0,     0,    87,
       0,    88,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   100,     0,     0,   101,     0,
     102,   717,   103,     0,   104,     0,     4,   105,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
     -76,     0,    38,    39,    40,     0,    41,  -291,     0,    42,
      43,    44,    45,    46,     0,    47,    48,    49,   -47,    50,
      51,     0,    52,    53,    54,     0,     0,     0,    55,    56,
      57,    58,   339,    60,    61,  -291,   -47,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -76,    85,    86,     0,     0,
      87,     0,    88,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,     0,   103,     0,   104,     0,     4,   105,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,   -76,     0,    38,    39,    40,     0,    41,  -291,     0,
      42,    43,    44,    45,    46,     0,    47,    48,    49,   -47,
      50,    51,     0,    52,    53,    54,     0,     0,     0,    55,
      56,    57,    58,     0,    60,    61,  -291,   -47,    62,    63,
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
     630,    37,   -76,     0,    38,    39,    40,     0,    41,  -291,
       0,    42,    43,    44,    45,    46,     0,    47,    48,    49,
     -47,    50,    51,     0,    52,    53,    54,     0,     0,     0,
      55,    56,    57,    58,     0,    60,    61,  -291,   -47,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -76,    85,    86,
       0,     0,    87,     0,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,     0,   103,     0,   104,     0,     4,
     105,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,   636,    37,   -76,     0,    38,    39,    40,     0,    41,
    -291,     0,    42,    43,    44,    45,    46,     0,    47,    48,
      49,   -47,    50,    51,     0,    52,    53,    54,     0,     0,
       0,    55,    56,    57,    58,     0,    60,    61,  -291,   -47,
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
      34,    35,   949,    37,   -76,     0,    38,    39,    40,     0,
      41,  -291,     0,    42,    43,    44,    45,    46,     0,    47,
      48,    49,   -47,    50,    51,     0,    52,    53,    54,     0,
       0,     0,    55,    56,    57,    58,     0,    60,    61,  -291,
     -47,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,    75,     0,    76,    77,
       0,    78,    79,    80,    81,    82,     0,    83,    84,   -76,
      85,    86,     0,     0,    87,     0,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,     0,   103,     0,   104,
       0,     4,   105,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,   951,    37,   -76,     0,    38,    39,    40,
       0,    41,  -291,     0,    42,    43,    44,    45,    46,     0,
      47,    48,    49,   -47,    50,    51,     0,    52,    53,    54,
       0,     0,     0,    55,    56,    57,    58,     0,    60,    61,
    -291,   -47,    62,    63,    64,     0,    65,    66,    67,     0,
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
      32,    33,    34,    35,   956,    37,   -76,     0,    38,    39,
      40,     0,    41,  -291,     0,    42,    43,    44,    45,    46,
       0,    47,    48,    49,   -47,    50,    51,     0,    52,    53,
      54,     0,     0,     0,    55,    56,    57,    58,     0,    60,
      61,  -291,   -47,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,     0,    83,
      84,   -76,    85,    86,     0,     0,    87,     0,    88,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,     0,   103,
       0,   104,     0,     4,   105,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,   959,    37,   -76,     0,    38,
      39,    40,     0,    41,  -291,     0,    42,    43,    44,    45,
      46,     0,    47,    48,    49,   -47,    50,    51,     0,    52,
      53,    54,     0,     0,     0,    55,    56,    57,    58,     0,
      60,    61,  -291,   -47,    62,    63,    64,     0,    65,    66,
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
      23,    24,    25,    26,     0,   985,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,   -76,     0,
      38,    39,    40,     0,    41,  -291,     0,    42,    43,    44,
      45,    46,     0,    47,    48,    49,   -47,    50,    51,     0,
      52,    53,    54,     0,     0,     0,    55,    56,    57,    58,
       0,    60,    61,  -291,   -47,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -76,    85,    86,     0,     0,    87,     0,
      88,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
       0,   103,     0,   104,     0,     4,   105,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,   991,    37,   -76,
       0,    38,    39,    40,     0,    41,  -291,     0,    42,    43,
      44,    45,    46,     0,    47,    48,    49,   -47,    50,    51,
       0,    52,    53,    54,     0,     0,     0,    55,    56,    57,
      58,     0,    60,    61,  -291,   -47,    62,    63,    64,     0,
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
      29,     0,    30,    31,    32,    33,    34,    35,   995,    37,
     -76,     0,    38,    39,    40,     0,    41,  -291,     0,    42,
      43,    44,    45,    46,     0,    47,    48,    49,   -47,    50,
      51,     0,    52,    53,    54,     0,     0,     0,    55,    56,
      57,    58,     0,    60,    61,  -291,   -47,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -76,    85,    86,     0,     0,
      87,     0,    88,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,     0,   103,     0,   104,     0,  1180,   105,     5,
     287,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,   191,  1181,   835,   105,
       5,   287,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,   228,    23,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
      56,    57,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,    65,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,    85,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,   191,     0,     0,   105,
       5,   287,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
     291,   292,     0,    82,   328,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,   329,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     0,
     105,     5,   287,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,   291,   292,     0,    82,   328,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,   701,   191,     0,
       0,   105,     5,   287,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   186,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,   291,   292,     0,    82,   328,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,   703,   191,
       0,     5,   105,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,   593,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,   594,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,   595,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,   596,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
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
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
     291,   292,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   293,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     0,
     105,     5,   287,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,   291,   292,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
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
       0,    50,    51,   837,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
     291,   292,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,   217,    10,    11,   218,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
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
      51,     0,    52,    53,     0,     0,     0,     0,     0,    56,
     222,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,   223,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   190,     0,   103,     0,   191,     0,     0,   105,     5,
     287,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
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
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,   291,   292,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   190,     0,   103,     0,   191,     0,     0,   105,     5,
     287,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,   699,   103,     0,   191,     0,     0,   105,
       5,   287,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,   709,     0,
     105,     5,   287,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,     0,   191,  1112,
       5,   105,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,   223,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,   267,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   102,     0,   103,     0,   191,     0,     0,   105,
       5,   287,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,   217,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
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
      51,     0,    52,    53,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   190,     0,   103,     0,   191,   714,     5,   105,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   181,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   183,
       0,     0,     0,    32,   184,   185,     0,   186,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,   187,     0,
       0,    45,    46,     0,     0,    48,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,   188,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   189,     0,    76,     0,     0,    78,     0,     0,     0,
      82,   863,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     190,     0,   103,     0,   191,     0,     0,   105,     5,   287,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,   673,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,   674,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,   675,    72,
      73,    74,   676,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   190,     0,   103,     0,  1052,     0,     5,   105,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   181,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   183,
       0,     0,     0,    32,   184,   185,     0,   186,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,   187,     0,
       0,    45,    46,     0,     0,    48,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,   188,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   189,     0,    76,     0,     0,    78,     0,     0,     0,
      82,  1077,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     190,     0,   103,     0,   191,     0,     0,   105,     5,   287,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   190,     0,   103,     0,  1052,     0,     0,   105,     5,
     287,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,  1033,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
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
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   190,     0,   103,     0,   191,     0,     5,   105,     6,
       7,     8,   213,    10,    11,     0,     0,     0,   181,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   183,
       0,     0,     0,    32,   184,   185,     0,   186,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,   187,     0,
       0,    45,    46,     0,     0,    48,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,   214,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   189,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
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
      52,    53,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   102,
       0,   103,     0,   191,     0,     5,   105,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   183,     0,     0,
       0,    32,   184,   185,     0,   186,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,     0,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   641,     0,   190,     0,
     103,     0,   191,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   673,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,   674,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,   675,    72,    73,    74,   676,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,   677,     0,     5,   105,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
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
     677,     0,     5,   105,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   769,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,     0,   191,
       0,     5,   105,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   772,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
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
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
    1027,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,  1028,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
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
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,  1030,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   190,     0,   103,     0,   191,     0,     5,   105,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   181,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   183,
       0,     0,     0,    32,   184,   185,     0,  1031,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,   187,     0,
       0,    45,    46,     0,     0,    48,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,   188,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   189,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     190,     0,   103,     0,   191,     0,     5,   105,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   181,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   183,     0,
       0,     0,    32,   184,   185,     0,  1032,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,   187,     0,     0,
      45,    46,     0,     0,    48,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   190,
       0,   103,     0,   191,     0,     5,   105,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   183,     0,     0,
       0,    32,   184,   185,     0,  1033,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,     0,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,    56,     0,    58,     0,
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
       0,     0,     0,     0,   673,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,   674,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,   675,    72,    73,    74,   676,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,  1136,     0,     5,   105,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   876,     0,   877,    90,    91,    92,    93,
      94,     0,     0,     0,   878,    95,     0,     0,     0,   879,
     236,   880,   881,     0,     0,    96,     0,   882,    97,    98,
      99,     0,     0,     0,   101,   239,     0,   183,   103,     0,
    1136,   883,     0,   105,     0,     0,   884,     0,     0,     0,
     242,     0,     0,   885,     0,   886,     0,     0,     0,     0,
       0,     0,     0,   887,     0,     0,     0,     0,     0,   888,
     889,     0,     0,     0,     0,     0,   247,     0,     0,     0,
       0,     0,     0,     0,     0,   890,     0,     0,     0,     0,
       0,     0,     0,     0,   249,   250,     0,   891,     0,   252,
       0,   892,     0,     0,   893,     0,     0,     0,   894,     0,
       0,   256,     0,     0,   895,     0,     0,     0,     0,     0,
       0,     0,     0,   459,   388,   389,   390,   391,   392,     0,
       0,   395,   396,   397,   398,     0,   400,   401,   896,   897,
     898,   899,   900,     0,     0,   901,     0,     0,     0,   902,
     903,   904,   905,   906,   907,   908,   909,   910,   911,   912,
       0,   913,     0,     0,   914,   915,   916,   917,     0,     5,
     918,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
     387,   388,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,   400,   401,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,   402,    98,    99,  -596,     0,  -596,
     101,     0,   190,     0,   103,     0,   191,     5,   287,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   181,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   183,
       0,     0,     0,    32,   184,   185,     0,   186,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,   187,     0,
       0,    45,    46,     0,     0,    48,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,   188,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   189,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,  -454,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,  -454,     0,     0,     0,
    -454,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,     0,     0,     0,
     190,     0,   103,  -454,  1052,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   183,     0,     0,
       0,    32,   184,   185,     0,   186,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,  -436,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
    -436,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,  -436,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,  -436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
      92,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,    99,  -436,     0,     0,     0,     0,  -436,    28,
     103,   183,  1136,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,     5,    92,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,    99,     0,     0,     0,
       0,     0,   190,    28,   103,   183,   191,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
     234,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,   235,   236,     0,   237,     0,     0,
       0,     0,   238,     0,     0,     0,     0,     0,     0,     0,
     239,     0,     0,     0,     0,     0,   240,     0,    92,     0,
       0,   241,     0,     0,     0,   242,     0,     0,   243,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   244,     0,
      99,     0,     0,     0,   245,   246,   190,     0,   103,     0,
     677,   247,     0,     0,     0,     0,     0,     0,     0,     0,
     248,     0,     0,     0,     0,     0,     0,     0,     0,   249,
     250,     0,   251,     0,   252,     0,   253,     0,  1090,   254,
       0,     0,     0,   255,     0,     0,   256,   878,     0,   257,
       0,     0,   235,   236,   880,   237,     0,     0,     0,     0,
     238,     0,     0,     0,     0,     0,     0,     0,   239,     0,
       0,     0,     0,     0,   883,     0,     0,     0,     0,   241,
       0,     0,     0,   242,     0,     0,   243,     0,   886,     0,
       0,     0,     0,     0,     0,     0,   244,     0,     0,     0,
       0,     0,   888,   246,     0,   825,     0,     0,     0,   247,
       0,     0,     0,     0,     0,     0,     0,     0,   248,     0,
       0,     0,     0,     0,     0,     0,     0,   249,   250,   357,
     251,     0,   252,     0,  1091,     0,   358,   893,     0,     0,
       0,   255,     0,     0,   256,     0,     0,   257,   359,     0,
       0,     0,     0,     0,     0,     0,   459,   388,   389,   390,
     391,   392,     0,     0,   395,   396,   397,   398,     0,   400,
     401,   896,   897,   898,   899,   900,     0,     0,   901,     0,
       0,     0,   902,   903,   904,   905,   906,   907,   908,   909,
     910,   911,   912,     0,   913,     0,     0,   914,   915,   916,
     917,     0,   360,     0,     0,   357,   361,     0,     0,     0,
       0,     0,   358,     0,     0,   458,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   362,   459,   388,
     389,   390,   391,   392,     0,     0,   395,   396,   397,   398,
       0,   400,   401,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,   360,   383,
     384,   385,   361,   417,     0,   357,     0,     0,     0,     0,
     386,   948,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,     0,
       0,     0,     0,   362,   459,   388,   389,   390,   391,   392,
       0,     0,   395,   396,   397,   398,     0,   400,   401,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,     0,   383,   384,   385,   360,   417,
       0,   357,   361,     0,     0,     0,   386,     0,   358,     0,
       0,   955,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   362,   459,   388,   389,   390,   391,   392,
       0,     0,   395,   396,   397,   398,     0,   400,   401,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,   360,   383,   384,   385,   361,   417,
       0,   357,     0,     0,     0,     0,   386,  1104,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,     0,   362,
     459,   388,   389,   390,   391,   392,     0,     0,   395,   396,
     397,   398,     0,   400,   401,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
       0,   383,   384,   385,   360,   417,     0,   357,   361,     0,
       0,     0,   386,     0,   358,     0,     0,  1105,     0,     0,
       0,     0,     0,     0,     0,     0,   359,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   362,
     459,   388,   389,   390,   391,   392,     0,     0,   395,   396,
     397,   398,     0,   400,   401,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
     360,   383,   384,   385,   361,   417,     0,   357,     0,     0,
       0,     0,   386,  1106,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,     0,     0,     0,
       0,     0,     0,     0,     0,   362,   459,   388,   389,   390,
     391,   392,     0,     0,   395,   396,   397,   398,     0,   400,
     401,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,     0,   383,   384,   385,
     360,   417,     0,   357,   361,     0,     0,     0,   386,     0,
     358,     0,     0,  1107,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   362,   459,   388,   389,   390,
     391,   392,     0,     0,   395,   396,   397,   398,     0,   400,
     401,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,   360,   383,   384,   385,
     361,   417,     0,   357,     0,     0,     0,     0,   386,  1130,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,     0,     0,     0,
       0,   362,   459,   388,   389,   390,   391,   392,     0,     0,
     395,   396,   397,   398,     0,   400,   401,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,     0,   383,   384,   385,   360,   417,     0,     0,
     361,     0,     0,     0,   386,   357,     0,     0,     0,  1131,
       0,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,   451,     0,     0,     0,     0,
       0,   362,   459,   388,   389,   390,   391,   392,     0,   452,
     395,   396,   397,   398,     0,   400,   401,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,     0,   383,   384,   385,     0,   417,   360,     0,
       0,   357,   361,     0,   386,     0,     0,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,   445,     0,     0,     0,     0,     0,     0,     0,   448,
       0,     0,     0,   362,     0,   446,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,   360,   383,   384,   385,   361,   417,
       0,     0,     0,   357,   725,     0,   386,     0,     0,     0,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,   726,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,   727,   376,   377,   378,   379,   380,   381,   382,     0,
       0,   383,   384,   385,     0,   417,   360,     0,     0,   357,
     361,     0,   386,     0,     0,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,   804,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,   360,   383,   384,   385,   361,     0,     0,   357,
       0,     0,     0,     0,   386,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,     0,
       0,     0,     0,   448,     0,     0,     0,   362,     0,     0,
       0,     0,     0,   419,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,     0,   383,
     384,   385,   360,   417,     0,   357,   361,     0,     0,     0,
     386,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,   267,     0,     0,     0,     0,
       0,     0,     0,   176,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,   360,   383,
     384,   385,   361,     0,     0,   357,     0,     0,     0,     0,
     386,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,   602,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,   603,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,     0,   383,   384,   385,   360,   417,
       0,   357,   361,     0,     0,     0,   386,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,   604,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,   605,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,   360,   383,   384,   385,   361,     0,
       0,   357,     0,     0,     0,     0,   386,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,   796,     0,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
       0,   383,   384,   385,   360,     0,     0,   357,   361,     0,
       0,     0,   386,     0,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
     360,   383,   384,   385,   361,   417,     0,   357,     0,     0,
       0,     0,   386,   814,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,     0,     0,     0,
       0,     0,     0,     0,     0,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,     0,   383,   384,   385,
     360,   417,     0,   357,   361,     0,     0,     0,   386,     0,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,  1004,     0,     0,
       0,   448,     0,     0,     0,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,  1005,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,   360,   383,   384,   385,
     361,     0,     0,   357,     0,     0,   855,     0,   386,     0,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,     0,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,     0,   383,   384,   385,   360,     0,     0,   357,
     361,     0,     0,     0,   386,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,   360,   383,   384,   385,   361,   494,     0,   357,
       0,     0,     0,     0,   386,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,   498,   383,
     384,   385,   360,     0,     0,     0,   361,     0,     0,   357,
     386,     0,     0,     0,     0,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,   500,   383,
     384,   385,   360,     0,     0,     0,   361,     0,     0,   357,
     386,     0,     0,     0,     0,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,   502,   383,
     384,   385,   360,     0,     0,     0,   361,     0,     0,   357,
     386,     0,     0,     0,     0,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,   507,   383,
     384,   385,   360,     0,     0,   357,   361,     0,     0,     0,
     386,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   524,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,   360,   383,
     384,   385,   361,     0,     0,   357,     0,     0,     0,     0,
     386,   606,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,     0,   383,   384,   385,   360,     0,
       0,     0,   361,     0,     0,     0,   386,   357,   721,     0,
       0,     0,     0,     0,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,     0,   383,   384,   385,     0,     0,
     360,     0,   700,   357,   361,     0,   386,     0,     0,     0,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,   364,     0,   365,   366,   367,     0,   368,   722,
     370,     0,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,   360,   383,   384,   385,
     361,     0,     0,   357,     0,     0,     0,     0,   386,     0,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,     0,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,  -558,   376,   377,   378,   379,   380,   381,
     382,     0,     0,   383,   384,   385,   360,     0,     0,   357,
     361,     0,     0,     0,   386,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,     0,
       0,     0,     0,     0,     0,     0,     0,   176,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,   360,   383,   384,   385,   361,     0,     0,   357,
     809,     0,     0,     0,   386,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,   781,   383,
     384,   385,   360,     0,     0,   357,   361,     0,     0,     0,
     386,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,   812,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,   360,   383,
     384,   385,   361,     0,     0,   357,     0,     0,     0,     0,
     386,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,     0,   383,   384,   385,   360,     0,
       0,   357,   361,     0,     0,     0,   386,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,   360,   383,   384,   385,   361,     0,
       0,     0,   839,   357,     0,     0,   386,     0,     0,     0,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
       0,   383,   384,   385,     0,     0,   360,     0,   840,   357,
     361,     0,   386,     0,     0,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,   933,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     858,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,   360,   383,   384,   385,   361,     0,     0,   357,
       0,     0,     0,     0,   386,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,   936,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,     0,   383,
     384,   385,   360,     0,     0,   357,   361,     0,     0,     0,
     386,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,   360,   383,
     384,   385,   361,     0,     0,   357,  1001,     0,     0,     0,
     386,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,     0,   383,   384,   385,   360,     0,
       0,   357,   361,     0,   939,     0,   386,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,   360,   383,   384,   385,   361,     0,
       0,   357,     0,     0,     0,     0,   386,  1026,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
       0,   383,   384,   385,   360,     0,     0,     0,   361,     0,
       0,     0,   386,   357,  1046,     0,     0,  1029,     0,     0,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
       0,   383,   384,   385,     0,     0,   360,     0,     0,   357,
     361,     0,   386,     0,     0,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,   360,   383,   384,   385,   361,     0,     0,     0,
     357,     0,     0,     0,   386,     0,     0,   358,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   359,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,     0,   383,
     384,   385,     0,   360,     0,     0,  1089,   361,     0,     0,
     386,   357,     0,     0,     0,     0,     0,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   363,   364,     0,   365,   366,   367,
       0,   368,   369,   370,     0,   371,   372,   373,   374,   375,
       0,   376,   377,   378,   379,   380,   381,   382,     0,     0,
     383,   384,   385,     0,   360,     0,     0,   357,   361,  1126,
       0,   386,     0,     0,   358,     0,     0,  1145,     0,     0,
       0,     0,     0,     0,     0,     0,   359,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
     360,   383,   384,   385,   361,     0,     0,   357,     0,     0,
       0,     0,   386,  1146,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,     0,     0,     0,
       0,     0,     0,     0,     0,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,     0,   383,   384,   385,
     360,     0,     0,   357,   361,     0,     0,     0,   386,     0,
     358,     0,     0,  1147,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,   360,   383,   384,   385,
     361,     0,     0,   357,     0,     0,     0,     0,   386,  1148,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,     0,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,     0,   383,   384,   385,   360,     0,     0,   357,
     361,     0,     0,     0,   386,     0,   358,     0,     0,  1149,
       0,     0,     0,     0,     0,     0,     0,     0,   359,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,   360,   383,   384,   385,   361,     0,     0,     0,
       0,     0,   357,     0,   386,  1150,     0,     0,     0,   358,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   359,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,  1174,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,     0,   383,
     384,   385,     0,     0,     0,   360,     0,     0,   357,   361,
     386,     0,     0,     0,     0,   358,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   359,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     362,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   363,   364,     0,   365,
     366,   367,     0,   368,   369,   370,     0,   371,   372,   373,
     374,   375,     0,   376,   377,   378,   379,   380,   381,   382,
       0,   360,   383,   384,   385,   361,     0,     0,   357,     0,
       0,     0,     0,   386,     0,   358,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   359,     0,     0,
       0,     0,     0,     0,     0,     0,   362,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   363,   364,     0,   365,   366,   367,     0,   368,
     369,   370,     0,   371,   372,   373,   374,   375,     0,   376,
     377,   378,   379,   380,   381,   382,     0,     0,   383,   384,
     385,   360,     0,     0,   357,   361,     0,  1203,     0,   386,
       0,   358,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   359,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   362,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   363,   364,     0,   365,   366,   367,     0,   368,
     369,   370,     0,   371,   372,   373,   374,   375,     0,   376,
     377,   378,   379,   380,   381,   382,     0,   767,   383,   384,
     385,   361,     0,     0,     0,     0,     0,     0,     0,   386,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   363,   364,
       0,   365,   366,   367,     0,   368,   369,   370,     0,   371,
     372,   373,   374,   375,     0,   376,   377,   378,   379,   380,
     381,   382,     0,   282,   383,   384,   385,    22,    23,     0,
       0,     0,     0,     0,     0,   386,   283,     0,    30,   284,
     282,     0,     0,    35,    22,    23,     0,     0,     0,     0,
      40,     0,     0,   283,     0,    30,   284,     0,     0,     0,
      35,     0,     0,     0,   359,     0,     0,    40,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
    1010,     0,     0,  1011,     0,   285,     0,    65,     0,     0,
       0,    55,     0,    57,     0,    59,     0,  1010,     0,     0,
    1011,     0,   285,     0,    65,   359,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,   360,     0,
       0,     0,   361,    81,     0,     0,    83,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,   360,
       0,     0,   100,   361,     0,     0,     0,     0,  1151,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,   100,
     371,   372,   373,   374,     0,  1152,   376,   377,   378,   379,
     380,   381,   382,     0,   362,   383,   384,   385,     0,     0,
       0,     0,     0,     0,     0,     0,   386,     0,     0,     0,
     363,   364,     0,   365,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,   374,     0,     0,   376,   377,   378,
     379,   380,     0,   382,     0,   282,   383,   384,   385,    22,
      23,     0,     0,     0,   732,     0,   234,   386,   283,     0,
      30,   284,     0,     0,     0,    35,     0,     0,     0,     0,
     235,   236,    40,   237,     0,     0,     0,     0,   238,     0,
       0,     0,     0,     0,     0,     0,   239,     0,     0,     0,
       0,     0,   240,     0,     0,     0,    55,   241,    57,     0,
     339,   242,  1010,     0,   243,  1011,     0,   285,  -272,    65,
       0,     0,     0,     0,   244,     0,     0,     0,     0,     0,
     245,   246,  -272,  -272,     0,  -272,     0,   247,    81,     0,
    -272,    83,     0,     0,    85,     0,   248,     0,  -272,     0,
       0,     0,     0,     0,  -272,   249,   250,     0,   251,  -272,
     252,     0,   253,  -272,     0,   254,  -272,     0,     0,   255,
    -282,     0,   256,     0,     0,   257,  -272,     0,     0,     0,
       0,     0,  -272,  -272,  -282,  -282,     0,  -282,     0,  -272,
       0,     0,  -282,     0,   100,     0,     0,     0,  -272,     0,
    -282,     0,     0,     0,     0,     0,  -282,  -272,  -272,     0,
    -272,  -282,  -272,     0,  -272,  -282,     0,  -272,  -282,     0,
       0,  -272,   234,     0,  -272,     0,     0,  -272,  -282,     0,
       0,     0,     0,     0,  -282,  -282,   235,   236,     0,   237,
       0,  -282,     0,     0,   238,     0,     0,     0,     0,     0,
    -282,     0,   239,     0,     0,     0,     0,     0,   240,  -282,
    -282,     0,  -282,   241,  -282,     0,  -282,   242,     0,  -282,
     243,     0,     0,  -282,  -273,     0,  -282,     0,     0,  -282,
     244,     0,     0,     0,     0,     0,   245,   246,  -273,  -273,
       0,  -273,     0,   247,     0,     0,  -273,     0,     0,     0,
       0,     0,   248,     0,  -273,     0,     0,     0,     0,     0,
    -273,   249,   250,     0,   251,  -273,   252,     0,   253,  -273,
       0,   254,  -273,     0,     0,   255,  -174,     0,   256,     0,
       0,   257,  -273,     0,     0,     0,     0,     0,  -273,  -273,
    -174,  -174,     0,  -174,     0,  -273,     0,     0,  -174,     0,
       0,     0,     0,     0,  -273,     0,  -174,     0,     0,     0,
       0,     0,  -174,  -273,  -273,     0,  -273,  -174,  -273,     0,
    -273,  -174,     0,  -273,  -174,     0,     0,  -273,  -170,     0,
    -273,     0,     0,  -273,  -174,     0,     0,     0,     0,     0,
    -174,  -174,  -170,  -170,     0,  -170,     0,  -174,     0,     0,
    -170,     0,     0,     0,     0,     0,  -174,     0,  -170,     0,
       0,     0,     0,     0,  -170,  -174,  -174,     0,  -174,  -170,
    -174,     0,  -174,  -170,     0,  -174,  -170,     0,     0,  -174,
       0,     0,  -174,     0,     0,  -174,  -170,     0,     0,     0,
       0,     0,  -170,  -170,     0,     0,   282,     0,     0,  -170,
      22,    23,     0,     0,     0,     0,     0,     0,  -170,   283,
       0,    30,   284,     0,     0,     0,    35,  -170,  -170,   -77,
    -170,     0,  -170,    40,  -170,     0,  -291,  -170,     0,     0,
       0,  -170,     0,     0,  -170,     0,     0,  -170,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,     0,     0,     0,  -291,     0,     0,     0,   285,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    81,
       0,     0,    83,     0,   -77,    85
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1099)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      12,    62,    66,   160,    16,     2,   456,   466,    20,    19,
     227,   204,   408,   233,    26,   503,   634,   741,    30,    31,
     146,    33,    34,    35,    36,   473,   727,   220,   834,   672,
     726,   622,   869,    22,   625,    47,    54,    49,   923,   924,
     104,     1,    54,    46,    56,     1,    58,    88,   975,     1,
      54,    46,  1112,   212,    66,    67,    68,    69,    70,    71,
     280,    32,    32,    75,    32,    77,  1164,    79,    80,    32,
      32,  1062,    84,    88,   233,    87,    88,   142,    90,   205,
      92,   972,   302,    95,    96,    36,    98,    46,    79,    80,
     102,   103,   104,   105,    66,     1,   161,   142,    45,    85,
      45,   478,   470,    89,  1164,  1203,    46,    59,   142,    46,
       3,   142,   161,   116,    52,    71,    63,   334,   335,    70,
     161,   280,   928,   766,   169,   166,   503,   191,     3,     1,
     102,   103,   104,   167,    81,   753,    88,    46,   169,   142,
      46,     3,    80,   302,   156,    20,   116,   142,   607,  1181,
     168,   166,   309,   116,    33,    34,     3,   113,    33,  1086,
    1051,   121,   114,   110,   168,   110,    18,  1199,   171,   181,
     976,   142,   184,   185,   186,     0,  1013,   189,   190,   191,
      54,   141,   142,   142,   404,   405,    61,    59,   831,   141,
     116,     3,  1183,   203,   785,   166,   166,   788,   166,  1036,
     212,  1038,   142,   166,   166,   142,    99,  1092,  1093,   169,
     369,   167,   171,    31,   166,   121,    88,    71,   190,   191,
     281,   233,    74,   671,    99,   104,     3,   216,   116,   101,
     105,   171,   221,   142,   171,   141,   142,    99,   634,   228,
      92,    88,   114,    20,  1050,   404,   405,   114,   116,   216,
      67,   142,    99,    46,   221,   161,    33,   109,   161,   113,
      46,    59,   171,  1100,   167,  1102,   142,    85,   280,   141,
     729,    89,   284,   142,   161,   142,   496,   727,   169,   291,
     292,   168,   142,   171,    61,   944,   103,   946,    54,   161,
     302,   166,   161,    86,   166,   171,    59,   142,  1022,    54,
      86,   161,  1003,   171,  1005,  1111,  1002,   466,  1004,    47,
     436,   142,   142,   167,    86,   441,   114,   329,   136,   778,
      54,    96,    99,   141,   169,    88,   144,    27,   105,   326,
     148,    69,   344,   345,   168,   953,   167,   496,   101,   169,
      27,    79,    80,   355,    54,   357,   358,   359,   360,   361,
     362,   114,   364,   365,   366,   367,   368,   753,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,    75,  1035,   825,  1037,   443,
      85,   393,   394,   142,    89,   142,    86,   399,    75,   142,
     402,   403,   404,   405,   406,   407,   408,    54,   161,    86,
     142,   121,   142,   166,   161,   105,   142,   419,    27,   142,
     169,   861,   424,   142,   426,   142,   169,   168,   105,   170,
     142,   638,   639,    59,   111,   161,   146,   169,    46,   169,
      54,   443,   121,   445,   446,     8,   169,   121,   167,   451,
     452,    59,   169,   455,   456,   161,   458,   169,   607,   461,
     142,   167,    88,   142,   466,   823,    75,   146,   142,    27,
     168,   620,   146,    79,   202,   101,    82,    86,   480,   161,
      88,   443,   484,   485,   538,    27,   142,   489,   114,   142,
     169,   360,   361,    54,   496,   169,   224,   225,   226,   227,
     707,   708,   111,   231,   982,   161,   114,    49,   168,   511,
     170,   869,   514,   121,   516,   698,   169,    75,   520,   521,
     522,    54,   524,   525,    54,   168,   142,   489,    86,   142,
      54,    73,   260,    75,   142,   161,   538,   406,   407,    54,
     166,     2,   270,   530,    86,   161,   168,   105,   276,   511,
     419,    12,   514,   161,   516,    59,   169,    54,   166,   521,
     142,   169,    54,  1003,    25,  1005,    27,   953,   166,   111,
     298,    54,   721,   722,    54,    81,   538,   446,    54,   161,
     729,   161,   161,   452,    88,    31,   393,   394,   316,   168,
      27,   170,   399,   160,   741,   402,   403,   101,   152,   161,
     602,   603,   604,   605,   606,   607,   608,   609,   975,    46,
     114,   611,   612,    50,    75,   982,   618,   136,   620,   136,
     161,   161,   136,   677,   141,   144,   628,   144,   630,   778,
     144,   148,   634,   161,   636,    72,   136,   121,    75,    85,
      77,   102,   168,    89,   144,   168,   107,   170,   168,    86,
     170,   520,   168,   166,   170,   168,   618,   161,   161,   161,
    1018,    69,   166,   147,    54,   149,   150,   161,   105,   153,
     154,   166,     3,   675,   676,   677,   161,   115,  1036,   681,
    1038,    84,   168,     2,   170,   514,    17,   516,    84,    20,
     136,   168,   420,    12,    25,   141,   168,   143,   144,   168,
     168,   168,   148,   161,    35,   142,    25,   514,    27,   516,
     156,   168,   121,    44,   161,   677,   177,   719,   164,  1086,
      51,   723,   450,   725,   726,   727,   454,   729,     8,    63,
      61,   141,   169,   166,   603,   161,   605,    68,   144,    66,
     168,   121,  1100,   142,  1102,  1112,   169,   161,    46,   168,
      34,   753,    83,   169,   142,   121,    75,   169,   169,  1126,
      79,    80,   169,    21,    95,   767,   768,   769,    99,   142,
     772,    46,   161,   983,   984,   106,   778,   171,   171,   166,
      46,   112,   168,   102,    59,   161,   161,   161,   107,   843,
     161,   161,   794,    59,   796,   142,   168,  1164,    34,   801,
     261,   169,   804,   161,    27,   807,   267,   809,   171,   161,
     812,   618,   161,    88,  1181,    63,   224,   225,   226,   227,
     142,   169,    88,    46,   167,   169,   167,    50,   121,    86,
     169,    32,  1199,    32,   983,   984,  1203,   166,   171,   114,
     166,   843,    34,   161,   141,    86,   121,   171,   114,    72,
     141,   853,    75,   855,    77,   121,   858,   168,   177,   861,
     168,   171,   270,    86,   171,   326,   161,   142,   276,   871,
     144,   161,  1117,   167,   681,  1022,   142,   144,   166,   168,
     341,   843,   105,   144,   136,   144,   161,   136,   144,   144,
     298,   166,   136,   144,   169,   161,   136,   144,   767,   768,
     166,   144,   144,   169,   632,   633,   144,   136,   316,   144,
     638,   639,   166,    34,   101,   161,   918,   136,   171,   169,
     136,   160,   650,  1168,  1169,  1170,  1171,  1172,  1173,   161,
     932,   933,   169,   935,   936,   169,   938,   939,   169,   161,
     161,   161,   261,   161,   142,   160,   948,   949,   267,   951,
     341,   953,   464,   955,   956,   719,   417,   959,    24,   310,
    1011,   847,   818,   872,  1022,  1018,   694,   664,   823,   875,
    1183,  1111,  1141,  1174,  1167,    35,   481,    30,   620,   981,
     982,   983,   984,    -1,   445,    -1,    -1,   989,    -1,   991,
     451,    -1,    -1,   995,    -1,    -1,    -1,   458,  1052,  1001,
    1002,  1003,  1004,  1005,    -1,    -1,    -1,   326,    -1,    -1,
      -1,    -1,   420,   975,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   341,    -1,  1026,  1027,  1028,  1029,  1030,  1031,
    1032,  1033,    -1,    -1,    -1,     2,    -1,    -1,    -1,    -1,
      -1,    -1,   450,    -1,  1046,    12,   454,    -1,    -1,    -1,
    1052,   779,   780,    31,    -1,  1057,    -1,   518,    25,  1061,
      27,   522,    -1,   791,    27,   793,    -1,    -1,    -1,   530,
      -1,    -1,   800,    -1,    -1,   803,    -1,    -1,   806,    -1,
      -1,    -1,  1136,    46,    -1,    -1,    -1,    50,    -1,    -1,
    1052,   819,    -1,    -1,    -1,  1057,    -1,    -1,   417,    -1,
      -1,    -1,  1104,  1105,  1106,  1107,    -1,    85,    75,    72,
      -1,    89,    75,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   850,    86,  1126,    -1,   445,    -1,  1130,  1131,
      -1,    -1,   451,    -1,  1136,   102,    -1,    -1,   599,   458,
     107,    -1,   105,  1145,  1146,  1147,  1148,  1149,  1150,    -1,
    1112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    31,
      -1,  1163,  1164,   141,  1166,   143,   144,    -1,    -1,    -1,
     148,    -1,    -1,    -1,  1136,    -1,    -1,    -1,   156,    -1,
      -1,    -1,    -1,     2,   162,   163,   164,    -1,   649,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,  1199,    -1,   518,
      -1,  1203,  1164,   522,     2,    -1,    25,    -1,    27,    -1,
     177,   530,    -1,    85,    12,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,   632,   633,    -1,    25,    -1,    27,
     638,   639,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1203,    -1,    -1,    -1,   706,    -1,    -1,    -1,    -1,
      -1,   712,    -1,    -1,    -1,    -1,    75,    -1,    -1,   987,
      -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,    -1,    -1,    -1,   148,    75,    -1,    -1,
     741,    -1,    -1,   102,   156,    -1,    -1,    -1,   107,    -1,
     162,   163,   164,    -1,   261,    -1,    -1,    -1,    -1,    -1,
     267,    -1,    31,    -1,   102,    -1,   767,   768,    -1,   107,
      -1,    -1,  1040,  1041,    -1,    -1,  1044,  1045,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     649,    -1,    -1,   794,  1062,   796,    -1,    -1,    -1,    -1,
     801,    -1,  1070,   804,  1072,    -1,  1074,    -1,    31,    -1,
     811,    -1,    -1,   814,    -1,    -1,    85,    -1,   177,   326,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   779,   780,    -1,   341,    -1,    -1,    -1,    -1,   177,
      -1,   842,    -1,   791,    -1,    -1,    -1,   706,    -1,    -1,
      -1,    -1,   853,   712,   855,   803,    -1,    -1,   806,     2,
      -1,    -1,    85,    -1,    -1,    -1,    89,   136,   137,    12,
     139,   140,   141,     2,   143,   144,   145,    -1,    -1,   148,
      -1,    -1,    25,    12,    27,    -1,   155,   156,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    25,    -1,    27,    -1,
      -1,    -1,   261,    -1,   173,    -1,    -1,    -1,   267,    -1,
     417,    -1,    -1,   136,   137,  1183,    -1,   140,   141,    -1,
     143,   144,    -1,   261,    -1,   148,    -1,    -1,    -1,   267,
      -1,    -1,    75,   156,    -1,   794,    -1,   796,   445,   162,
     163,   164,   801,    -1,   451,   804,    75,   948,    -1,    -1,
      -1,   458,   811,    -1,   955,   814,    -1,    -1,    -1,   102,
      -1,    -1,    -1,    -1,   107,    -1,   967,   326,    -1,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    -1,   341,   842,   985,    -1,    -1,    -1,   326,    -1,
      -1,    -1,    -1,   994,   853,    -1,   855,   998,    -1,    -1,
      -1,    -1,    -1,   341,    22,    -1,    -1,    -1,    26,    27,
      -1,   518,    -1,    -1,    -1,   522,    -1,    35,    -1,    37,
      38,    -1,    -1,   530,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,   177,    -1,    -1,    -1,    -1,   987,
      -1,    -1,    -1,    -1,    -1,    -1,  1047,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    -1,    75,   417,    77,
      -1,    79,    -1,    -1,    82,    -1,    84,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   417,
      -1,    -1,    -1,    -1,    -1,    -1,   445,   105,    -1,   948,
     108,    -1,   451,   111,    -1,    -1,   955,    -1,    -1,   458,
      -1,    -1,    -1,  1104,  1105,  1106,  1107,   445,   967,    -1,
      -1,    -1,    -1,   451,    -1,    -1,    -1,    -1,   261,    -1,
     458,    -1,    -1,    -1,   267,    -1,   985,    -1,    -1,  1130,
    1131,     3,   261,    -1,    -1,   994,    -1,    -1,   267,   998,
      -1,    -1,   649,   161,    -1,    17,    18,    -1,    20,   167,
      -1,    -1,    31,    25,    -1,    -1,    -1,    -1,    -1,   518,
      -1,    33,    -1,   522,    -1,    -1,    -1,    39,    -1,    -1,
      -1,   530,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
     518,    -1,    -1,   326,   522,    -1,    -1,    -1,  1047,    61,
      -1,    -1,   530,    -1,    -1,    67,    68,   326,   341,   706,
      -1,    -1,    74,    -1,    -1,   712,    85,    -1,    -1,    -1,
      89,    83,   341,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    -1,    95,    -1,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,    -1,   106,   107,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,  1104,  1105,  1106,  1107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,    -1,    -1,    -1,   148,
      -1,  1130,  1131,    -1,   417,    -1,   155,   156,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,   794,   417,   796,
     649,    -1,    -1,    -1,   801,    -1,   168,   804,    -1,    -1,
      -1,    -1,   445,    -1,   811,    22,    -1,   814,   451,    26,
      27,   649,    -1,     2,    -1,   458,   445,    -1,    35,    -1,
      37,    38,   451,    12,    -1,    42,    -1,    -1,    -1,   458,
      -1,    -1,    49,    -1,    -1,   842,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   853,   706,   855,    -1,
      -1,    -1,    -1,   712,    -1,    -1,    73,    -1,    75,    -1,
      77,    -1,    79,    -1,    -1,    82,    -1,    84,   706,    86,
      -1,    -1,    -1,    -1,   712,   518,    -1,    -1,    -1,   522,
      -1,    -1,    -1,    -1,    -1,    -1,    75,   530,   105,   518,
      -1,   108,    -1,   522,   111,    -1,    -1,    -1,    -1,    -1,
      -1,   530,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    31,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   794,    -1,   796,    -1,     2,
      -1,   948,   801,    -1,   161,   804,    -1,    -1,   955,    12,
     167,    -1,   811,    -1,    -1,   814,   794,    -1,   796,    -1,
     967,    -1,    25,   801,    27,    -1,   804,    -1,    -1,    -1,
      85,    -1,    -1,   811,    89,    -1,   814,    -1,   985,    -1,
      -1,    -1,    -1,   842,    -1,    -1,    -1,   994,   177,    -1,
      -1,   998,    -1,    -1,   853,    -1,   855,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   842,    -1,   649,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    -1,   853,    -1,   855,    -1,    -1,
     649,   136,   137,    -1,    -1,    -1,   141,    -1,   143,   144,
      -1,    -1,    -1,   148,    -1,    -1,     2,    -1,    -1,   102,
    1047,   156,    -1,    -1,   107,    -1,    12,   162,   163,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,   706,    -1,    -1,    -1,    -1,    -1,   712,
      -1,    -1,   261,    -1,    -1,    -1,    -1,   706,   267,    -1,
      -1,    -1,    -1,   712,    -1,    -1,    -1,    -1,    -1,   948,
      -1,    -1,    -1,    -1,    -1,    -1,   955,  1104,  1105,  1106,
    1107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   967,    75,
     948,    -1,    -1,    -1,   177,    -1,    -1,   955,    -1,    -1,
      -1,    -1,    -1,  1130,  1131,    -1,   985,    -1,    -1,   967,
      -1,    -1,    -1,    -1,    -1,   994,   102,   326,    -1,   998,
      -1,   107,    -1,    -1,    -1,    -1,    -1,   985,    -1,    -1,
      -1,   794,   341,   796,    -1,    -1,   994,    -1,   801,    -1,
     998,   804,    -1,    -1,    -1,   794,    -1,   796,   811,    -1,
      -1,   814,   801,    -1,    -1,   804,    -1,    -1,    -1,    -1,
      -1,    -1,   811,    -1,    -1,   814,    -1,    22,  1047,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,   261,   842,
      35,    -1,    37,    38,   267,    -1,    -1,    42,    -1,  1047,
     853,   177,   855,   842,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   853,    -1,   855,    -1,   417,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,
      75,    -1,    -1,    -1,    -1,  1104,  1105,  1106,  1107,    84,
      -1,    86,    -1,    -1,    -1,    -1,   445,    -1,    -1,    -1,
      -1,    -1,   451,   326,    -1,    -1,  1104,  1105,  1106,  1107,
     105,  1130,  1131,   108,    -1,    -1,   111,    -1,   341,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1130,  1131,    -1,   261,    -1,    -1,    -1,    -1,
      -1,   267,    -1,    -1,    -1,   948,    -1,    -1,    -1,    -1,
      -1,    -1,   955,    -1,    -1,    -1,    -1,    -1,    -1,   948,
      -1,    -1,    -1,    -1,   967,    -1,   955,    -1,    -1,   518,
      -1,    -1,    -1,   522,    -1,    -1,    -1,    -1,   967,    -1,
      -1,   530,   985,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   994,    -1,    -1,   417,   998,   985,    -1,    -1,    -1,
     326,    -1,    -1,    -1,     1,   994,    -1,    -1,    -1,   998,
      -1,    -1,    -1,    -1,    -1,   341,    -1,    -1,    -1,    -1,
      -1,    -1,   445,    -1,    -1,    22,    -1,    -1,   451,    26,
      27,    -1,    -1,    -1,    -1,   458,    -1,    -1,    35,    -1,
      37,    38,    -1,    -1,  1047,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,  1047,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    -1,    73,    -1,    75,    -1,
      77,    -1,    79,    80,    -1,    82,    -1,    84,    -1,    86,
      -1,   417,    -1,    -1,    -1,   518,    -1,    -1,    -1,   522,
     649,  1104,  1105,  1106,  1107,    -1,    -1,   530,   105,    81,
      -1,   108,    -1,    -1,   111,  1104,  1105,  1106,  1107,   445,
      85,    -1,    -1,    -1,    89,   451,    -1,  1130,  1131,    -1,
      -1,    -1,   458,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1130,  1131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   706,    -1,    -1,
      -1,    -1,    -1,   712,   161,    -1,    -1,   139,    -1,   141,
     167,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,    -1,   148,   149,   150,    -1,    -1,   153,   154,
     155,   156,   518,    -1,   159,    -1,   522,   162,   163,   164,
      -1,    -1,    -1,    -1,   530,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   649,    -1,    -1,    -1,
      -1,    -1,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   794,    -1,   796,   220,    -1,
     222,    -1,   801,    -1,    -1,   804,    -1,    -1,    -1,    -1,
      -1,    -1,   811,    -1,    -1,   814,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,   706,    14,    -1,    -1,    17,    18,   712,
      20,   263,    22,   842,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,   853,    35,   855,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,   649,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,   794,   102,   796,   346,    -1,   106,    -1,   801,   109,
     706,   804,   112,    -1,    -1,    -1,   712,    -1,   811,    -1,
      -1,   814,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,   138,    -1,
      35,    -1,    37,    38,    -1,    -1,    -1,    42,   967,   842,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
     853,    -1,   855,    -1,    -1,    -1,   985,    -1,   168,    -1,
     170,    -1,    -1,    -1,   416,   994,    -1,    -1,    73,   998,
      75,    -1,    77,    -1,    79,    -1,    -1,    82,    -1,    84,
      -1,    86,   434,    -1,    -1,    -1,   438,    -1,   794,    -1,
     796,    -1,    -1,    -1,    -1,   801,    -1,    -1,   804,    -1,
     105,    -1,    -1,   108,    -1,   811,   111,    -1,   814,    -1,
       3,    -1,    -1,    -1,    -1,   467,    -1,    -1,  1047,    -1,
      -1,   473,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,   842,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,   948,    39,   853,    -1,   855,
      -1,    44,   955,    -1,    -1,    48,   161,    -1,    51,    -1,
      -1,    -1,   167,    -1,   967,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    67,    68,    -1,    -1,    -1,   531,
      -1,    74,   985,    -1,    -1,    -1,    -1,    -1,   540,    -1,
      83,   994,    -1,    -1,    -1,   998,    -1,    -1,    -1,    92,
      93,    -1,    95,    -1,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,     3,   948,    -1,  1047,    -1,    -1,    -1,    -1,   955,
      -1,    -1,   152,    31,    -1,    17,    18,    -1,    20,    -1,
      -1,   967,    -1,    25,    -1,   165,    -1,    -1,   168,   621,
     170,    33,   624,   166,    -1,   168,    -1,    39,    -1,   985,
      -1,    -1,    44,    -1,    -1,    -1,    48,    -1,   994,    51,
      -1,    -1,   998,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,  1104,  1105,  1106,  1107,    67,    68,    85,    -1,    -1,
      12,    89,    74,    -1,    -1,   667,    -1,    19,    -1,   671,
      -1,    83,    -1,    -1,    -1,    -1,    -1,  1130,  1131,    31,
      92,    93,    -1,    95,    36,    97,    -1,    99,    -1,    -1,
     102,  1047,   694,    -1,   106,    -1,   698,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
      -1,   139,   140,   141,    -1,   143,   144,   145,    70,   147,
     148,   149,   150,    -1,    -1,   153,   154,   155,   156,   157,
      -1,   159,    -1,    85,   162,   163,   164,    89,    -1,   741,
      -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,  1104,  1105,
    1106,  1107,    -1,    -1,    -1,    -1,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,  1130,  1131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,    -1,   161,
     162,   163,   164,    -1,   816,    -1,   818,    -1,    -1,    -1,
     822,   173,    -1,   825,     0,     1,    -1,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,   847,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
     872,    47,    48,    49,    -1,    51,    52,    -1,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    63,    64,    65,
      -1,    67,    68,    69,    -1,    -1,    -1,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    -1,
      86,    87,    88,    -1,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    99,   100,    -1,   102,   103,   104,   105,
     106,    -1,   108,   109,   110,   111,   112,    -1,    -1,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,    -1,
     166,    -1,   168,    -1,   170,    -1,    -1,   173,    -1,    -1,
       1,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
    1022,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    49,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    -1,    67,    68,    69,    -1,
      -1,    -1,    73,    74,    75,    76,    -1,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    -1,    99,   100,
      -1,   102,   103,   104,   105,   106,    -1,   108,   109,   110,
     111,   112,    -1,    -1,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,   166,   167,   168,    -1,   170,
      -1,     1,   173,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    49,
      -1,    51,    52,    -1,    54,    55,    56,    57,    58,    -1,
      60,    61,    62,    63,    64,    65,    -1,    67,    68,    69,
      -1,    -1,    -1,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    -1,    86,    87,    88,    -1,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,    99,
     100,    -1,   102,   103,   104,   105,   106,    -1,   108,   109,
     110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,   161,    -1,    -1,   164,    -1,   166,   167,   168,    -1,
     170,    -1,     1,   173,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    -1,    51,    52,    -1,    54,    55,    56,    57,    58,
      -1,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    -1,    -1,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    -1,    86,    87,    88,
      -1,    90,    91,    92,    93,    94,    95,    96,    97,    -1,
      99,   100,    -1,   102,   103,   104,   105,   106,    -1,   108,
     109,   110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,
     159,   160,   161,    -1,    -1,   164,    -1,   166,   167,   168,
      -1,   170,    -1,     1,   173,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    -1,    51,    52,    -1,    54,    55,    56,    57,
      58,    -1,    60,    61,    62,    63,    64,    65,    -1,    67,
      68,    69,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    -1,    86,    87,
      88,    -1,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    99,   100,    -1,   102,   103,   104,   105,   106,    -1,
     108,   109,   110,   111,   112,    -1,    -1,   115,    -1,   117,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
     158,   159,   160,   161,    -1,    -1,   164,    -1,   166,   167,
     168,    -1,   170,    -1,     1,   173,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    54,    55,    56,
      57,    58,    -1,    60,    61,    62,    63,    64,    65,    -1,
      67,    68,    69,    -1,    -1,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    -1,    86,
      87,    88,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    -1,    99,   100,    -1,   102,   103,   104,   105,   106,
      -1,   108,   109,   110,   111,   112,    -1,    -1,   115,    -1,
     117,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,    -1,   166,
     167,   168,    -1,   170,    -1,     1,   173,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    51,    52,    -1,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    63,    64,    65,
      -1,    67,    68,    69,    -1,    -1,    -1,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    -1,
      86,    87,    88,    -1,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    99,   100,    -1,   102,   103,   104,   105,
     106,    -1,   108,   109,   110,   111,   112,    -1,    -1,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,    -1,
     166,   167,   168,    -1,   170,    -1,     1,   173,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    -1,    51,    52,    -1,    54,
      55,    56,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    -1,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      -1,    86,    87,    88,    -1,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    99,   100,    -1,   102,   103,   104,
     105,   106,    -1,   108,   109,   110,   111,   112,    -1,    -1,
     115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,     1,   173,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    49,    -1,    51,    52,    -1,
      54,    55,    56,    57,    58,    -1,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    -1,    -1,    73,
      74,    75,    76,    -1,    78,    79,    80,    81,    82,    83,
      84,    -1,    86,    87,    88,    -1,    90,    91,    92,    93,
      94,    95,    96,    97,    -1,    99,   100,    -1,   102,   103,
     104,   105,   106,    -1,   108,   109,   110,   111,   112,    -1,
      -1,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,   161,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,     1,   173,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    49,    -1,    51,    52,
      -1,    54,    55,    56,    57,    58,    -1,    60,    61,    62,
      63,    64,    65,    -1,    67,    68,    69,    -1,    -1,    -1,
      73,    74,    75,    76,    -1,    78,    79,    80,    81,    82,
      83,    84,    -1,    86,    87,    88,    -1,    90,    91,    92,
      93,    94,    95,    96,    97,    -1,    99,   100,    -1,   102,
     103,   104,   105,   106,    -1,   108,   109,   110,   111,   112,
      -1,    -1,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,   161,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,     1,
     173,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    49,    -1,    51,
      52,    -1,    54,    55,    56,    57,    58,    -1,    60,    61,
      62,    63,    64,    65,    -1,    67,    68,    69,    -1,    -1,
      -1,    73,    74,    75,    76,    -1,    78,    79,    80,    81,
      82,    83,    84,    -1,    86,    87,    88,    -1,    90,    91,
      92,    93,    94,    95,    96,    97,    -1,    99,   100,    -1,
     102,   103,   104,   105,   106,    -1,   108,   109,   110,   111,
     112,    -1,    -1,   115,    -1,   117,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
       1,   173,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    49,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    -1,    67,    68,    69,    -1,
      -1,    -1,    73,    74,    75,    76,    -1,    78,    79,    80,
      81,    82,    83,    84,    -1,    86,    87,    88,    -1,    90,
      91,    92,    93,    94,    95,    96,    97,    -1,    99,   100,
      -1,   102,   103,   104,   105,   106,    -1,   108,   109,   110,
     111,   112,    -1,    -1,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,     1,   173,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    49,
      -1,    51,    52,    -1,    54,    55,    56,    57,    58,    -1,
      60,    61,    62,    63,    64,    65,    -1,    67,    68,    69,
      -1,    -1,    -1,    73,    74,    75,    76,    -1,    78,    79,
      80,    81,    82,    83,    84,    -1,    86,    87,    88,    -1,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,    99,
     100,    -1,   102,   103,   104,   105,   106,    -1,   108,   109,
     110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,   161,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,
     170,    -1,     1,   173,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    -1,    51,    52,    -1,    54,    55,    56,    57,    58,
      -1,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    -1,    -1,    73,    74,    75,    76,    -1,    78,
      79,    80,    81,    82,    83,    84,    -1,    86,    87,    88,
      -1,    90,    91,    92,    93,    94,    95,    96,    97,    -1,
      99,   100,    -1,   102,   103,   104,   105,   106,    -1,   108,
     109,   110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,
     159,   160,   161,    -1,    -1,   164,    -1,   166,    -1,   168,
      -1,   170,    -1,     1,   173,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    -1,    51,    52,    -1,    54,    55,    56,    57,
      58,    -1,    60,    61,    62,    63,    64,    65,    -1,    67,
      68,    69,    -1,    -1,    -1,    73,    74,    75,    76,    -1,
      78,    79,    80,    81,    82,    83,    84,    -1,    86,    87,
      88,    -1,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    99,   100,    -1,   102,   103,   104,   105,   106,    -1,
     108,   109,   110,   111,   112,    -1,    -1,   115,    -1,   117,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
     158,   159,   160,   161,    -1,    -1,   164,    -1,   166,    -1,
     168,    -1,   170,    -1,     1,   173,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    54,    55,    56,
      57,    58,    -1,    60,    61,    62,    63,    64,    65,    -1,
      67,    68,    69,    -1,    -1,    -1,    73,    74,    75,    76,
      -1,    78,    79,    80,    81,    82,    83,    84,    -1,    86,
      87,    88,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    -1,    99,   100,    -1,   102,   103,   104,   105,   106,
      -1,   108,   109,   110,   111,   112,    -1,    -1,   115,    -1,
     117,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,    -1,   166,
      -1,   168,    -1,   170,    -1,     1,   173,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    51,    52,    -1,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    63,    64,    65,
      -1,    67,    68,    69,    -1,    -1,    -1,    73,    74,    75,
      76,    -1,    78,    79,    80,    81,    82,    83,    84,    -1,
      86,    87,    88,    -1,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    99,   100,    -1,   102,   103,   104,   105,
     106,    -1,   108,   109,   110,   111,   112,    -1,    -1,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,    -1,
     166,    -1,   168,    -1,   170,    -1,     1,   173,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    -1,    51,    52,    -1,    54,
      55,    56,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    -1,    -1,    73,    74,
      75,    76,    -1,    78,    79,    80,    81,    82,    83,    84,
      -1,    86,    87,    88,    -1,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    99,   100,    -1,   102,   103,   104,
     105,   106,    -1,   108,   109,   110,   111,   112,    -1,    -1,
     115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,     1,   173,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,   171,     1,   173,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,
     173,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    86,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,   173,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
     103,   104,    -1,   106,   107,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,
     173,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,   103,   104,    -1,   106,   107,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,   169,   170,    -1,
      -1,   173,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,   103,   104,    -1,   106,   107,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,    -1,   168,   169,   170,
      -1,     3,   173,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    46,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,   111,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
       3,   173,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
     103,   104,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,   161,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,
     173,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,   103,   104,    -1,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
       3,   173,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    66,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
     103,   104,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,
     173,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,   105,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      75,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,   118,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,    -1,   173,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,   118,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,    -1,   102,   103,   104,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,    -1,   173,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,   167,   168,    -1,   170,    -1,    -1,   173,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,   171,    -1,
     173,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,   171,
       3,   173,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,   118,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,
     173,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,   173,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,
     173,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,   105,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,   171,     3,   173,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,    65,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,
     106,   107,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,    -1,
     166,    -1,   168,    -1,   170,    -1,    -1,   173,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,    65,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,
     106,   107,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,    -1,
     166,    -1,   168,    -1,   170,    -1,    -1,   173,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,    -1,   173,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,    65,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,    -1,
     166,    -1,   168,    -1,   170,    -1,     3,   173,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    76,
      -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
      97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,   158,   159,   160,    -1,    -1,    -1,   164,    -1,   166,
      -1,   168,    -1,   170,    -1,     3,   173,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,
      78,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,    97,
      -1,    99,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
     158,   159,   160,    -1,    -1,    -1,   164,    -1,   166,    -1,
     168,    -1,   170,    -1,     3,   173,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,    97,    -1,
      99,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,
     159,   160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,
      -1,   170,    -1,     3,   173,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,
     170,    -1,     3,   173,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,     3,   173,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
       3,   173,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,
     173,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,    65,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,    -1,
     166,    -1,   168,    -1,   170,    -1,     3,   173,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    76,
      -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
      97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,   158,   159,   160,    -1,    -1,    -1,   164,    -1,   166,
      -1,   168,    -1,   170,    -1,     3,   173,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,
      78,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,    97,
      -1,    99,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
     158,   159,   160,    -1,    -1,    -1,   164,    -1,   166,    -1,
     168,    -1,   170,    -1,     3,   173,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,    97,    -1,
      99,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,
     159,   160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,
      -1,   170,    -1,     3,   173,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    12,   145,    -1,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,   155,    -1,    25,   158,   159,
     160,    -1,    -1,    -1,   164,    33,    -1,    35,   168,    -1,
     170,    39,    -1,   173,    -1,    -1,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    -1,    95,    -1,    97,
      -1,    99,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,   122,   123,   124,   125,   126,    -1,
      -1,   129,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,   139,   140,    -1,    -1,   143,    -1,    -1,    -1,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,     3,
     168,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   165,   159,   160,   168,    -1,   170,
     164,    -1,   166,    -1,   168,    -1,   170,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,    65,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,   142,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,    -1,    -1,    -1,
     166,    -1,   168,   169,   170,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    59,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,
      78,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    92,    93,    94,    95,    96,    97,
      -1,    99,    -1,   101,   102,    -1,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
     138,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,   160,   161,    -1,    -1,    -1,    -1,   166,    33,
     168,    35,   170,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,     3,   138,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,   160,    -1,    -1,    -1,
      -1,    -1,   166,    33,   168,    35,   170,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    99,
       3,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,   112,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,   138,    -1,
      -1,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
     160,    -1,    -1,    -1,    67,    68,   166,    -1,   168,    -1,
     170,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    -1,    95,    -1,    97,    -1,    99,    -1,     3,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    12,    -1,   112,
      -1,    -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    67,    68,    -1,   168,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    12,
      95,    -1,    97,    -1,    99,    -1,    19,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,   122,   123,   124,
     125,   126,    -1,    -1,   129,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,   139,   140,    -1,    -1,   143,    -1,
      -1,    -1,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    85,    -1,    -1,    12,    89,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,
     123,   124,   125,   126,    -1,    -1,   129,   130,   131,   132,
      -1,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    85,   162,
     163,   164,    89,   166,    -1,    12,    -1,    -1,    -1,    -1,
     173,    98,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,   126,
      -1,    -1,   129,   130,   131,   132,    -1,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,   162,   163,   164,    85,   166,
      -1,    12,    89,    -1,    -1,    -1,   173,    -1,    19,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,   126,
      -1,    -1,   129,   130,   131,   132,    -1,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,   166,
      -1,    12,    -1,    -1,    -1,    -1,   173,    98,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,   124,   125,   126,    -1,    -1,   129,   130,
     131,   132,    -1,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    85,   166,    -1,    12,    89,    -1,
      -1,    -1,   173,    -1,    19,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,   124,   125,   126,    -1,    -1,   129,   130,
     131,   132,    -1,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      85,   162,   163,   164,    89,   166,    -1,    12,    -1,    -1,
      -1,    -1,   173,    98,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,   121,   122,   123,   124,
     125,   126,    -1,    -1,   129,   130,   131,   132,    -1,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    -1,   162,   163,   164,
      85,   166,    -1,    12,    89,    -1,    -1,    -1,   173,    -1,
      19,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,   121,   122,   123,   124,
     125,   126,    -1,    -1,   129,   130,   131,   132,    -1,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    85,   162,   163,   164,
      89,   166,    -1,    12,    -1,    -1,    -1,    -1,   173,    98,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,   121,   122,   123,   124,   125,   126,    -1,    -1,
     129,   130,   131,   132,    -1,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    -1,   162,   163,   164,    85,   166,    -1,    -1,
      89,    -1,    -1,    -1,   173,    12,    -1,    -1,    -1,    98,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,   120,   121,   122,   123,   124,   125,   126,    -1,    46,
     129,   130,   131,   132,    -1,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    -1,   162,   163,   164,    -1,   166,    85,    -1,
      -1,    12,    89,    -1,   173,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,   120,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,   166,
      -1,    -1,    -1,    12,    13,    -1,   173,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    70,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    -1,   166,    85,    -1,    -1,    12,
      89,    -1,   173,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    85,   162,   163,   164,    89,    -1,    -1,    12,
      -1,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    -1,   162,
     163,   164,    85,   166,    -1,    12,    89,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    85,   162,
     163,   164,    89,    -1,    -1,    12,    -1,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,   162,   163,   164,    85,   166,
      -1,    12,    89,    -1,    -1,    -1,   173,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,    -1,
      -1,    12,    -1,    -1,    -1,    -1,   173,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    85,    -1,    -1,    12,    89,    -1,
      -1,    -1,   173,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      85,   162,   163,   164,    89,   166,    -1,    12,    -1,    -1,
      -1,    -1,   173,    98,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    -1,   162,   163,   164,
      85,   166,    -1,    12,    89,    -1,    -1,    -1,   173,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,   116,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    70,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    85,   162,   163,   164,
      89,    -1,    -1,    12,    -1,    -1,   171,    -1,   173,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    -1,   162,   163,   164,    85,    -1,    -1,    12,
      89,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    85,   162,   163,   164,    89,   166,    -1,    12,
      -1,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,   161,   162,
     163,   164,    85,    -1,    -1,    -1,    89,    -1,    -1,    12,
     173,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,   161,   162,
     163,   164,    85,    -1,    -1,    -1,    89,    -1,    -1,    12,
     173,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,   161,   162,
     163,   164,    85,    -1,    -1,    -1,    89,    -1,    -1,    12,
     173,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,   161,   162,
     163,   164,    85,    -1,    -1,    12,    89,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    85,   162,
     163,   164,    89,    -1,    -1,    12,    -1,    -1,    -1,    -1,
     173,    98,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,   162,   163,   164,    85,    -1,
      -1,    -1,    89,    -1,    -1,    -1,   173,    12,    13,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      85,    -1,   169,    12,    89,    -1,   173,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    85,   162,   163,   164,
      89,    -1,    -1,    12,    -1,    -1,    -1,    -1,   173,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,    -1,    -1,   162,   163,   164,    85,    -1,    -1,    12,
      89,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    85,   162,   163,   164,    89,    -1,    -1,    12,
      13,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,   161,   162,
     163,   164,    85,    -1,    -1,    12,    89,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    85,   162,
     163,   164,    89,    -1,    -1,    12,    -1,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,   162,   163,   164,    85,    -1,
      -1,    12,    89,    -1,    -1,    -1,   173,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,    -1,
      -1,    -1,   169,    12,    -1,    -1,   173,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    85,    -1,   169,    12,
      89,    -1,   173,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    85,   162,   163,   164,    89,    -1,    -1,    12,
      -1,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    -1,   162,
     163,   164,    85,    -1,    -1,    12,    89,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    85,   162,
     163,   164,    89,    -1,    -1,    12,    13,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,   162,   163,   164,    85,    -1,
      -1,    12,    89,    -1,   171,    -1,   173,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,    -1,
      -1,    12,    -1,    -1,    -1,    -1,   173,    98,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    85,    -1,    -1,    -1,    89,    -1,
      -1,    -1,   173,    12,    13,    -1,    -1,    98,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    85,    -1,    -1,    12,
      89,    -1,   173,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    85,   162,   163,   164,    89,    -1,    -1,    -1,
      12,    -1,    -1,    -1,   173,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    -1,   162,
     163,   164,    -1,    85,    -1,    -1,   169,    89,    -1,    -1,
     173,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,    -1,    -1,
     162,   163,   164,    -1,    85,    -1,    -1,    12,    89,   171,
      -1,   173,    -1,    -1,    19,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      85,   162,   163,   164,    89,    -1,    -1,    12,    -1,    -1,
      -1,    -1,   173,    98,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    -1,   162,   163,   164,
      85,    -1,    -1,    12,    89,    -1,    -1,    -1,   173,    -1,
      19,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    85,   162,   163,   164,
      89,    -1,    -1,    12,    -1,    -1,    -1,    -1,   173,    98,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    -1,   162,   163,   164,    85,    -1,    -1,    12,
      89,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    85,   162,   163,   164,    89,    -1,    -1,    -1,
      -1,    -1,    12,    -1,   173,    98,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    59,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,    85,    -1,    -1,    12,    89,
     173,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
      -1,    85,   162,   163,   164,    89,    -1,    -1,    12,    -1,
      -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,    -1,    -1,   162,   163,
     164,    85,    -1,    -1,    12,    89,    -1,   171,    -1,   173,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,    -1,    85,   162,   163,
     164,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
      -1,   139,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,    -1,    22,   162,   163,   164,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,   173,    35,    -1,    37,    38,
      22,    -1,    -1,    42,    26,    27,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    31,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    82,    -1,    84,    -1,    86,    -1,    -1,
      -1,    73,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,
      82,    -1,    84,    -1,    86,    31,   105,    -1,    -1,   108,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    89,   105,    -1,    -1,   108,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,   161,    89,    -1,    -1,    -1,    -1,   167,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,   161,
     147,   148,   149,   150,    -1,   167,   153,   154,   155,   156,
     157,   158,   159,    -1,   120,   162,   163,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,   150,    -1,    -1,   153,   154,   155,
     156,   157,    -1,   159,    -1,    22,   162,   163,   164,    26,
      27,    -1,    -1,    -1,     1,    -1,     3,   173,    35,    -1,
      37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      17,    18,    49,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    73,    44,    75,    -1,
      77,    48,    79,    -1,    51,    82,    -1,    84,     3,    86,
      -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      67,    68,    17,    18,    -1,    20,    -1,    74,   105,    -1,
      25,   108,    -1,    -1,   111,    -1,    83,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    92,    93,    -1,    95,    44,
      97,    -1,    99,    48,    -1,   102,    51,    -1,    -1,   106,
       3,    -1,   109,    -1,    -1,   112,    61,    -1,    -1,    -1,
      -1,    -1,    67,    68,    17,    18,    -1,    20,    -1,    74,
      -1,    -1,    25,    -1,   161,    -1,    -1,    -1,    83,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,    92,    93,    -1,
      95,    44,    97,    -1,    99,    48,    -1,   102,    51,    -1,
      -1,   106,     3,    -1,   109,    -1,    -1,   112,    61,    -1,
      -1,    -1,    -1,    -1,    67,    68,    17,    18,    -1,    20,
      -1,    74,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    92,
      93,    -1,    95,    44,    97,    -1,    99,    48,    -1,   102,
      51,    -1,    -1,   106,     3,    -1,   109,    -1,    -1,   112,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    17,    18,
      -1,    20,    -1,    74,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    92,    93,    -1,    95,    44,    97,    -1,    99,    48,
      -1,   102,    51,    -1,    -1,   106,     3,    -1,   109,    -1,
      -1,   112,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,
      17,    18,    -1,    20,    -1,    74,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    83,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    92,    93,    -1,    95,    44,    97,    -1,
      99,    48,    -1,   102,    51,    -1,    -1,   106,     3,    -1,
     109,    -1,    -1,   112,    61,    -1,    -1,    -1,    -1,    -1,
      67,    68,    17,    18,    -1,    20,    -1,    74,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    83,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    92,    93,    -1,    95,    44,
      97,    -1,    99,    48,    -1,   102,    51,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,   112,    61,    -1,    -1,    -1,
      -1,    -1,    67,    68,    -1,    -1,    22,    -1,    -1,    74,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    83,    35,
      -1,    37,    38,    -1,    -1,    -1,    42,    92,    93,    45,
      95,    -1,    97,    49,    99,    -1,    52,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    75,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,    -1,   108,    -1,   110,   111
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   178,   179,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,    35,
      37,    38,    39,    40,    41,    42,    43,    44,    47,    48,
      49,    51,    54,    55,    56,    57,    58,    60,    61,    62,
      64,    65,    67,    68,    69,    73,    74,    75,    76,    77,
      78,    79,    82,    83,    84,    86,    87,    88,    90,    91,
      92,    93,    94,    95,    96,    97,    99,   100,   102,   103,
     104,   105,   106,   108,   109,   111,   112,   115,   117,   120,
     136,   137,   138,   139,   140,   145,   155,   158,   159,   160,
     161,   164,   166,   168,   170,   173,   180,   181,   182,   183,
     184,   185,   188,   190,   196,   197,   198,   201,   202,   206,
     208,   211,   212,   214,   215,   216,   221,   222,   224,   225,
     228,   229,   230,   231,   232,   233,   237,   238,   241,   242,
     245,   246,   249,   252,   253,   278,   281,   282,   302,   303,
     304,   305,   306,   307,   308,   316,   317,   318,   319,   320,
     323,   324,   325,   326,   327,   328,   329,   330,   332,   333,
     334,   335,   336,   161,   182,   320,   116,   309,   310,     3,
     203,    14,    22,    35,    40,    41,    43,    54,    84,    97,
     166,   170,   228,   249,   302,   307,   318,   319,   320,   323,
     325,   326,   309,   320,   105,   281,    86,   203,   182,   296,
     320,   182,   168,     8,    84,   320,   321,     8,    11,    84,
     105,   321,    75,   118,   223,   320,   223,   320,    26,   282,
     320,   320,   189,   168,     3,    17,    18,    20,    25,    33,
      39,    44,    48,    51,    61,    67,    68,    74,    83,    92,
      93,    95,    97,    99,   102,   106,   109,   112,   205,   207,
     205,   205,   250,   168,   205,   283,   284,    32,   190,   210,
     320,   320,    18,    74,    92,   109,   320,   320,   320,     8,
     168,   213,    22,    35,    38,    84,   214,     4,   272,   295,
     320,   103,   104,   161,   320,   322,   320,   210,   320,   320,
     320,    96,   168,   182,   320,   320,   190,   202,   320,   323,
     190,   202,   320,   205,   279,   320,   320,   320,   320,   320,
     320,   320,   320,     1,   167,   180,   191,   295,   107,   146,
     272,   297,   298,   322,   223,   295,   320,   331,   320,    77,
     182,   166,    81,   186,    45,   110,    54,   205,   205,    52,
      80,   267,   283,   160,   161,   152,   320,    12,    19,    31,
      85,    89,   120,   136,   137,   139,   140,   141,   143,   144,
     145,   147,   148,   149,   150,   151,   153,   154,   155,   156,
     157,   158,   159,   162,   163,   164,   173,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   165,   259,   168,   170,    85,    89,   168,   182,
     161,   320,   320,   320,   205,   295,    54,   166,   190,    46,
     309,   279,   283,   161,   142,   161,   115,   206,   272,   299,
     300,   301,   322,    84,   219,   253,   281,    84,   217,   279,
     253,   281,   205,   168,   210,    32,    46,   210,   116,   210,
     312,    32,    46,   210,   312,    36,    70,   161,    98,   121,
     190,   259,    79,    82,   187,   299,   168,   168,   190,   182,
     168,   261,   107,   168,   205,   285,   286,     1,   141,   290,
      46,   142,   182,   210,   168,   168,   210,   299,   214,   142,
     161,   320,   320,   161,   166,   210,   168,   299,   161,   234,
     161,   234,   161,   121,   280,   161,   210,   161,   167,   167,
     180,   142,   167,   320,   142,   169,   142,   169,   171,   312,
      46,   142,   171,   312,   119,   142,   171,     8,     1,   167,
     191,    63,   199,   200,   320,   193,   320,   205,   141,   243,
     166,   254,   161,   320,   320,   320,   320,   223,   320,   223,
     320,   320,   320,   320,   320,   320,   320,     3,    20,    33,
      61,    99,   105,   206,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,    66,   322,   322,   322,   322,   322,   299,   299,   223,
     320,   223,   320,    27,    46,    86,   111,   311,   314,   315,
     320,   336,    32,    46,    32,    46,    98,   168,    46,   171,
     205,   223,   320,   210,   161,   161,   320,   320,   121,   169,
     142,   220,   205,   283,   218,   205,   161,   283,    46,   295,
      43,   320,   223,   320,   168,   210,    43,   320,   223,   320,
     210,   164,   192,   195,   320,   192,   194,   182,   320,    34,
     320,   186,   169,   300,   205,   226,   227,    27,    46,    50,
      72,    75,    86,   105,   181,   262,   263,   264,   265,   251,
     286,   142,   169,    33,    48,    93,    97,   170,   209,   291,
     303,   121,   287,   320,   284,   320,   320,   169,   272,   320,
       1,   239,   299,   169,    21,   235,   291,   303,   142,   167,
     169,   169,   297,   169,   297,   182,   171,   223,   320,   171,
     182,   320,   171,   320,   171,   320,   167,   167,   205,   142,
     161,    13,   144,   142,   161,    13,    36,    70,   161,   168,
     295,   166,     1,   205,   247,   248,    27,    75,    86,   105,
     256,   266,   168,   161,   161,   161,   161,   161,   161,   169,
     171,    46,    86,   142,   169,    17,    20,    25,    44,    51,
      61,    68,    83,    95,   106,   112,   302,    85,    85,    43,
     223,   320,    43,   223,   320,   300,   223,   320,   168,   309,
     309,   161,   272,   322,   301,   205,   243,   161,   205,   243,
     161,   320,   169,   320,    32,   210,    32,   210,   313,   314,
     320,    32,   210,   312,    32,   210,   312,   142,   161,    13,
     161,    34,    34,   182,    98,   190,    63,   169,   142,   169,
      46,    86,   264,   142,   169,   168,   205,    27,    75,    86,
     105,   268,   169,   286,   290,     1,   295,    66,   322,   169,
     169,   167,    71,   113,   167,   240,   169,   168,   190,   205,
     236,   279,   182,   171,   312,   171,   312,   182,   119,   199,
     206,   166,   320,   107,   320,   195,   194,   300,     1,   244,
     167,   121,   142,   167,    86,   255,     1,     3,    12,    17,
      19,    20,    25,    39,    44,    51,    53,    61,    67,    68,
      83,    95,    99,   102,   106,   112,   136,   137,   138,   139,
     140,   143,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   159,   162,   163,   164,   165,   168,   204,
     205,   207,   257,   258,   259,   302,   169,   314,   290,   302,
     302,   320,    32,    32,   320,    32,    32,   169,   171,   171,
     300,   210,   210,   243,   166,   243,   166,   210,    98,    43,
     320,    43,   320,   142,   169,    98,    43,   320,   210,    43,
     320,   210,   320,   320,   182,   320,   182,    34,   205,   161,
     227,   190,   205,   263,   286,   141,   294,    86,   290,   287,
     171,    46,   171,   168,   168,    32,   182,   295,   236,   141,
     190,    43,   182,   320,   171,    43,   182,   320,   171,   320,
     192,    13,    36,    70,    36,    70,   161,   161,   169,   167,
      79,    82,   167,   181,   212,   320,   248,   268,   168,   260,
     320,   136,   144,   260,   260,   287,    98,    43,    43,    98,
      43,    43,    43,    43,   169,   166,   244,   166,   244,   320,
     320,   320,   314,   320,   320,   320,    13,    34,   182,   161,
     294,   169,   170,   209,   272,   293,   303,   146,   273,   287,
      59,   114,   274,   320,   291,   303,   299,   299,   182,   210,
     169,   320,   320,   182,   320,   182,   167,   107,   320,   195,
     194,   195,   194,   161,   212,     1,   141,   289,   262,   169,
       3,    99,   258,   259,   320,   320,   320,   320,   320,   320,
     244,   167,   244,   167,    98,    98,    98,    98,   320,   182,
     287,   294,   171,   295,   272,   320,     3,    88,    99,   275,
     276,   277,   320,   190,   211,   271,   171,   169,   169,   190,
      98,    98,   161,   161,   161,   161,   170,   209,   288,   303,
     101,   269,   169,   260,   260,    98,    98,    98,    98,    98,
      98,   167,   167,   320,   320,   320,   320,   273,   287,   272,
     292,   293,   303,    46,   171,   277,   114,   142,   121,   147,
     149,   150,   153,   154,    59,   303,   160,   160,   320,   320,
       1,   171,   295,   274,   320,   292,   293,   320,   276,   277,
     277,   277,   277,   277,   277,   275,   171,   288,   303,   171,
     161,   270,   271,   171,   288,   303,   292
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
#line 509 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 514 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 515 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 522 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 527 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 528 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 551 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 552 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 553 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 554 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 555 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 556 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 557 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 558 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 559 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 560 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 561 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 562 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 563 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 564 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 565 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 566 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 571 "chapel.ypp"
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
#line 584 "chapel.ypp"
    { (yyvsp[(1) - (3)].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (3)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (3)]).prevModule;  // restore previous module name
    }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 589 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = (yyvsp[(3) - (4)].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 594 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 601 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 602 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 603 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 607 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 613 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 619 "chapel.ypp"
    {
    (yylsp[(1) - (1)]).comment = context->latestComment;
    context->latestComment = NULL;
  }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 624 "chapel.ypp"
    {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[(6) - (7)].pch), (yyvsp[(3) - (7)].b), (yyvsp[(4) - (7)].b), (yylsp[(1) - (7)]).comment);
 }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 639 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 646 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 647 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 652 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 657 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 662 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 666 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 675 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 680 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 685 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 690 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 694 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 698 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 706 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 715 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 724 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 725 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 726 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 730 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (4)].ponlylist), (yyvsp[(1) - (4)].b)); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 731 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), "", (yyvsp[(5) - (6)].ponlylist), true, (yyvsp[(1) - (6)].b)); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 732 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), (yyvsp[(5) - (8)].pexpr), (yyvsp[(7) - (8)].ponlylist), true, (yyvsp[(1) - (8)].b)); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 733 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), new UnresolvedSymExpr("_"), (yyvsp[(7) - (8)].ponlylist), true, (yyvsp[(1) - (8)].b)); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 734 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), "", (yyvsp[(5) - (6)].ponlylist), false, (yyvsp[(1) - (6)].b)); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), (yyvsp[(5) - (8)].pexpr), (yyvsp[(7) - (8)].ponlylist), false, (yyvsp[(1) - (8)].b)); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), new UnresolvedSymExpr("_"), (yyvsp[(7) - (8)].ponlylist), false, (yyvsp[(1) - (8)].b)); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 740 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(3) - (4)].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[(1) - (4)].b));}
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 745 "chapel.ypp"
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[(1) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist)); }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pimportstmt)); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (3)].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[(3) - (3)].pimportstmt)); }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 761 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 763 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 765 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 767 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 771 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), new SymExpr(gNoInit), "="); }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 777 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 778 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 782 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 783 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 784 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 785 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 786 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 787 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 790 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 791 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 792 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 793 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 794 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 795 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 807 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 808 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 819 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 820 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 821 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 822 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 823 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 824 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 825 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 826 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 827 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 828 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 829 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 830 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 831 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 832 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 833 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 834 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 835 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 836 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 837 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 841 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 842 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 843 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 844 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 845 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 846 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 847 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 848 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 849 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 850 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 851 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 852 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 859 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 860 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 861 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 862 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 866 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 867 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 871 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 872 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 876 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 878 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 888 "chapel.ypp"
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 889 "chapel.ypp"
    { parsingPrivate=(yyvsp[(2) - (3)].b); applyPrivateToBlock((yyvsp[(3) - (3)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(3) - (3)].pblockstmt); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 903 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 904 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 905 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 906 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 911 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 916 "chapel.ypp"
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
#line 926 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 931 "chapel.ypp"
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
#line 942 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 947 "chapel.ypp"
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
#line 957 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 962 "chapel.ypp"
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
#line 973 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 977 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 982 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 990 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 1001 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 1007 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 1008 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 1009 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 1010 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 1011 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 1012 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 1013 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), true, false); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 1014 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), true, false); }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1016 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 1017 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1018 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1022 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1023 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1024 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1026 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1032 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1038 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1044 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1050 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1057 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1064 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1068 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1074 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1078 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1079 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1080 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1081 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1082 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1084 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1086 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1088 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1094 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[(2) - (6)].pch), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt))); }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1096 "chapel.ypp"
    { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[(2) - (3)].pch), ls, (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1103 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1104 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[(1) - (1)].pch), INTENT_TYPE); }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1114 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1115 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1116 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1117 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1118 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1119 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1120 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1121 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1122 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1123 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1124 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1125 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1127 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[(1) - (1)].pch)); }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1133 "chapel.ypp"
    { (yyval.pch) = "none"; }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1134 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1135 "chapel.ypp"
    { (yyval.pch) = "false"; }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1136 "chapel.ypp"
    { (yyval.pch) = "true"; }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1145 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1146 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1151 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(2) - (6)].pch), (yyvsp[(4) - (6)].pcallexpr), NULL)); }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1153 "chapel.ypp"
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[(1) - (4)].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(3) - (4)].pch), act, NULL)); }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1156 "chapel.ypp"
    { (yyvsp[(5) - (7)].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[(1) - (7)].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(3) - (7)].pch), (yyvsp[(5) - (7)].pcallexpr), NULL)); }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1162 "chapel.ypp"
    { (yyval.pexpr) = IfcConstraint::build((yyvsp[(2) - (5)].pch), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1164 "chapel.ypp"
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[(1) - (3)].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[(3) - (3)].pch), act); }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1167 "chapel.ypp"
    { (yyvsp[(5) - (6)].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[(1) - (6)].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pcallexpr)); }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1172 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1175 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1176 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1177 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1178 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1179 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1180 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1184 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1185 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1190 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1191 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1195 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1196 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1200 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1204 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1206 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1210 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1216 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1218 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1220 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1227 "chapel.ypp"
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

  case 271:
/* Line 1792 of yacc.c  */
#line 1237 "chapel.ypp"
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

  case 272:
/* Line 1792 of yacc.c  */
#line 1250 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1255 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1260 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1268 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1269 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1276 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1283 "chapel.ypp"
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

  case 281:
/* Line 1792 of yacc.c  */
#line 1299 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1306 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1323 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1327 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1333 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1339 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1344 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1349 "chapel.ypp"
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

  case 291:
/* Line 1792 of yacc.c  */
#line 1371 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1374 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1378 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1386 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1396 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1403 "chapel.ypp"
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

  case 297:
/* Line 1792 of yacc.c  */
#line 1429 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1433 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1438 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1442 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1447 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1454 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1458 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1459 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1460 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1461 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1462 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1463 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1464 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1465 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1466 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1467 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1469 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1470 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1471 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1472 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1473 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1474 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1476 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1479 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1480 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1481 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1488 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1490 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1492 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1493 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1494 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1496 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1497 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1498 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1499 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1527 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1532 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1536 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1541 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1542 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1543 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1544 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1548 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1550 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1552 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1553 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1557 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1563 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1564 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1565 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1566 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1571 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1572 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1575 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1591 "chapel.ypp"
    { if (DefExpr* def = toDefExpr((yyvsp[(2) - (2)].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
                         }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1601 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1605 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1607 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1612 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1614 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1618 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1619 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1620 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1621 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1622 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1623 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1624 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1629 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1631 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1636 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1638 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1640 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1645 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1658 "chapel.ypp"
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

  case 410:
/* Line 1792 of yacc.c  */
#line 1674 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1676 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1678 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1682 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1683 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1684 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1685 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1686 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1691 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1697 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1706 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1721 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1725 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1730 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1732 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1734 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1740 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1741 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1742 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1748 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1750 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1752 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1756 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1760 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1762 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1766 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1773 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1775 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1776 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1777 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1782 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1783 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1784 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1785 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1786 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1807 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1811 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1815 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1823 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1829 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1830 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1831 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1836 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1838 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1844 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1846 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1848 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1852 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1853 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1854 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1855 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1856 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1862 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1863 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1864 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1865 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1869 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1870 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1874 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1875 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1876 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1880 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1881 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1885 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1890 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1891 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1895 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1896 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1897 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1898 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1902 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1903 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1915 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1917 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1919 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1930 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1932 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1934 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1936 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1938 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1940 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1942 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1945 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1947 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1949 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1951 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1953 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 1955 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1957 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1959 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1962 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1964 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1969 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1971 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1973 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1975 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1977 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1979 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1981 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1983 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 1985 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 1987 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 523:
/* Line 1792 of yacc.c  */
#line 1989 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 1991 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 1993 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 2000 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 2006 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 2012 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 2018 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 2027 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 2036 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 2052 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 2056 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 2061 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 2065 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 2066 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 2070 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 2074 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 2075 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 2080 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 2084 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 2088 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 2094 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 2095 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 2096 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 2097 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 2098 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 2099 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 2105 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 2110 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 2112 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2116 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2120 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2124 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 2132 "chapel.ypp"
    { (yyvsp[(1) - (2)].pcallexpr)->insertAtTail((yyvsp[(2) - (2)].pexpr));
      (yyval.pexpr) = (yyvsp[(1) - (2)].pcallexpr); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2137 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 2143 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 2149 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2156 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2166 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2183 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2185 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2187 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2189 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), false, true); }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2204 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 584:
/* Line 1792 of yacc.c  */
#line 2206 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 585:
/* Line 1792 of yacc.c  */
#line 2208 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr), true); }
    break;

  case 586:
/* Line 1792 of yacc.c  */
#line 2210 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
    break;

  case 587:
/* Line 1792 of yacc.c  */
#line 2214 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 588:
/* Line 1792 of yacc.c  */
#line 2215 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 589:
/* Line 1792 of yacc.c  */
#line 2219 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 590:
/* Line 1792 of yacc.c  */
#line 2220 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 591:
/* Line 1792 of yacc.c  */
#line 2221 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 596:
/* Line 1792 of yacc.c  */
#line 2237 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 597:
/* Line 1792 of yacc.c  */
#line 2238 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 598:
/* Line 1792 of yacc.c  */
#line 2239 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr));}
    break;

  case 599:
/* Line 1792 of yacc.c  */
#line 2240 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 601:
/* Line 1792 of yacc.c  */
#line 2245 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 602:
/* Line 1792 of yacc.c  */
#line 2246 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 603:
/* Line 1792 of yacc.c  */
#line 2247 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 604:
/* Line 1792 of yacc.c  */
#line 2251 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 605:
/* Line 1792 of yacc.c  */
#line 2252 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 606:
/* Line 1792 of yacc.c  */
#line 2253 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 607:
/* Line 1792 of yacc.c  */
#line 2254 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 608:
/* Line 1792 of yacc.c  */
#line 2255 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 609:
/* Line 1792 of yacc.c  */
#line 2263 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 610:
/* Line 1792 of yacc.c  */
#line 2264 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 611:
/* Line 1792 of yacc.c  */
#line 2265 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 612:
/* Line 1792 of yacc.c  */
#line 2266 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 613:
/* Line 1792 of yacc.c  */
#line 2270 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 614:
/* Line 1792 of yacc.c  */
#line 2271 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 615:
/* Line 1792 of yacc.c  */
#line 2275 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 616:
/* Line 1792 of yacc.c  */
#line 2276 "chapel.ypp"
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 619:
/* Line 1792 of yacc.c  */
#line 2282 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 620:
/* Line 1792 of yacc.c  */
#line 2283 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 621:
/* Line 1792 of yacc.c  */
#line 2284 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 622:
/* Line 1792 of yacc.c  */
#line 2285 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 623:
/* Line 1792 of yacc.c  */
#line 2286 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 624:
/* Line 1792 of yacc.c  */
#line 2287 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr),
                                            new SymExpr(gTrue)); }
    break;

  case 625:
/* Line 1792 of yacc.c  */
#line 2289 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr),
                                                   new SymExpr(gTrue)); }
    break;

  case 626:
/* Line 1792 of yacc.c  */
#line 2291 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 627:
/* Line 1792 of yacc.c  */
#line 2292 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 628:
/* Line 1792 of yacc.c  */
#line 2294 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 629:
/* Line 1792 of yacc.c  */
#line 2298 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 630:
/* Line 1792 of yacc.c  */
#line 2305 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 631:
/* Line 1792 of yacc.c  */
#line 2306 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 632:
/* Line 1792 of yacc.c  */
#line 2310 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 633:
/* Line 1792 of yacc.c  */
#line 2311 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 634:
/* Line 1792 of yacc.c  */
#line 2312 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 635:
/* Line 1792 of yacc.c  */
#line 2313 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 636:
/* Line 1792 of yacc.c  */
#line 2314 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 637:
/* Line 1792 of yacc.c  */
#line 2315 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 638:
/* Line 1792 of yacc.c  */
#line 2316 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 639:
/* Line 1792 of yacc.c  */
#line 2317 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 640:
/* Line 1792 of yacc.c  */
#line 2318 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 641:
/* Line 1792 of yacc.c  */
#line 2319 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 642:
/* Line 1792 of yacc.c  */
#line 2320 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 643:
/* Line 1792 of yacc.c  */
#line 2321 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 644:
/* Line 1792 of yacc.c  */
#line 2322 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 645:
/* Line 1792 of yacc.c  */
#line 2323 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 646:
/* Line 1792 of yacc.c  */
#line 2324 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 647:
/* Line 1792 of yacc.c  */
#line 2325 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 648:
/* Line 1792 of yacc.c  */
#line 2326 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 649:
/* Line 1792 of yacc.c  */
#line 2327 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 650:
/* Line 1792 of yacc.c  */
#line 2328 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 651:
/* Line 1792 of yacc.c  */
#line 2329 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 652:
/* Line 1792 of yacc.c  */
#line 2330 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 653:
/* Line 1792 of yacc.c  */
#line 2331 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 654:
/* Line 1792 of yacc.c  */
#line 2332 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr),
                                               new SymExpr(gTrue)); }
    break;

  case 655:
/* Line 1792 of yacc.c  */
#line 2337 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 656:
/* Line 1792 of yacc.c  */
#line 2338 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 657:
/* Line 1792 of yacc.c  */
#line 2339 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 658:
/* Line 1792 of yacc.c  */
#line 2340 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 659:
/* Line 1792 of yacc.c  */
#line 2341 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 660:
/* Line 1792 of yacc.c  */
#line 2342 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 661:
/* Line 1792 of yacc.c  */
#line 2343 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 662:
/* Line 1792 of yacc.c  */
#line 2347 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 663:
/* Line 1792 of yacc.c  */
#line 2348 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 664:
/* Line 1792 of yacc.c  */
#line 2349 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 665:
/* Line 1792 of yacc.c  */
#line 2350 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 666:
/* Line 1792 of yacc.c  */
#line 2354 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 667:
/* Line 1792 of yacc.c  */
#line 2355 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 668:
/* Line 1792 of yacc.c  */
#line 2356 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 669:
/* Line 1792 of yacc.c  */
#line 2357 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 670:
/* Line 1792 of yacc.c  */
#line 2362 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 671:
/* Line 1792 of yacc.c  */
#line 2363 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 672:
/* Line 1792 of yacc.c  */
#line 2364 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 673:
/* Line 1792 of yacc.c  */
#line 2365 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 674:
/* Line 1792 of yacc.c  */
#line 2366 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 675:
/* Line 1792 of yacc.c  */
#line 2367 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 676:
/* Line 1792 of yacc.c  */
#line 2368 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 10979 "bison-chapel.cpp"
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


