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
#line 46 "chapel.ypp"

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
#line 66 "chapel.ypp"

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
#line 147 "chapel.ypp"

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
#line 170 "chapel.ypp"

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
#line 251 "bison-chapel.cpp"

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
     TINDEX = 302,
     TINLINE = 303,
     TINOUT = 304,
     TINT = 305,
     TITER = 306,
     TINITEQUALS = 307,
     TLABEL = 308,
     TLAMBDA = 309,
     TLET = 310,
     TLIFETIME = 311,
     TLOCAL = 312,
     TLOCALE = 313,
     TMINUSMINUS = 314,
     TMODULE = 315,
     TNEW = 316,
     TNIL = 317,
     TNOINIT = 318,
     TNONE = 319,
     TNOTHING = 320,
     TON = 321,
     TONLY = 322,
     TOTHERWISE = 323,
     TOUT = 324,
     TOVERRIDE = 325,
     TOWNED = 326,
     TPARAM = 327,
     TPLUSPLUS = 328,
     TPRAGMA = 329,
     TPRIMITIVE = 330,
     TPRIVATE = 331,
     TPROC = 332,
     TPROTOTYPE = 333,
     TPUBLIC = 334,
     TREAL = 335,
     TRECORD = 336,
     TREDUCE = 337,
     TREF = 338,
     TREQUIRE = 339,
     TRETURN = 340,
     TSCAN = 341,
     TSELECT = 342,
     TSERIAL = 343,
     TSHARED = 344,
     TSINGLE = 345,
     TSPARSE = 346,
     TSTRING = 347,
     TSUBDOMAIN = 348,
     TSYNC = 349,
     TTHEN = 350,
     TTHIS = 351,
     TTHROW = 352,
     TTHROWS = 353,
     TTRUE = 354,
     TTRY = 355,
     TTRYBANG = 356,
     TTYPE = 357,
     TUINT = 358,
     TUNDERSCORE = 359,
     TUNION = 360,
     TUNMANAGED = 361,
     TUSE = 362,
     TVAR = 363,
     TVOID = 364,
     TWHEN = 365,
     TWHERE = 366,
     TWHILE = 367,
     TWITH = 368,
     TYIELD = 369,
     TZIP = 370,
     TALIAS = 371,
     TAND = 372,
     TASSIGN = 373,
     TASSIGNBAND = 374,
     TASSIGNBOR = 375,
     TASSIGNBXOR = 376,
     TASSIGNDIVIDE = 377,
     TASSIGNEXP = 378,
     TASSIGNLAND = 379,
     TASSIGNLOR = 380,
     TASSIGNMINUS = 381,
     TASSIGNMOD = 382,
     TASSIGNMULTIPLY = 383,
     TASSIGNPLUS = 384,
     TASSIGNREDUCE = 385,
     TASSIGNSL = 386,
     TASSIGNSR = 387,
     TBANG = 388,
     TBAND = 389,
     TBNOT = 390,
     TBOR = 391,
     TBXOR = 392,
     TCOLON = 393,
     TCOMMA = 394,
     TDIVIDE = 395,
     TDOT = 396,
     TDOTDOT = 397,
     TDOTDOTDOT = 398,
     TEQUAL = 399,
     TEXP = 400,
     TGREATER = 401,
     TGREATEREQUAL = 402,
     THASH = 403,
     TIO = 404,
     TLESS = 405,
     TLESSEQUAL = 406,
     TMINUS = 407,
     TMOD = 408,
     TNOTEQUAL = 409,
     TOR = 410,
     TPLUS = 411,
     TQUESTION = 412,
     TSEMI = 413,
     TSHIFTLEFT = 414,
     TSHIFTRIGHT = 415,
     TSTAR = 416,
     TSWAP = 417,
     TLCBR = 418,
     TRCBR = 419,
     TLP = 420,
     TRP = 421,
     TLSBR = 422,
     TRSBR = 423,
     TNOELSE = 424,
     TUMINUS = 425,
     TUPLUS = 426,
     TLNOT = 427
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
#line 201 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);


/* Line 387 of yacc.c  */
#line 488 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 495 "bison-chapel.cpp"
/* Unqualified %code blocks.  */
/* Line 391 of yacc.c  */
#line 38 "chapel.ypp"

  #include <string>
  int         captureTokens;
  std::string captureString;
  bool        parsingPrivate=false;

/* Line 391 of yacc.c  */
#line 209 "chapel.ypp"

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
#line 573 "bison-chapel.cpp"

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
#define YYLAST   17997

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  173
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  146
/* YYNRULES -- Number of rules.  */
#define YYNRULES  624
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1120

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   427

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
     165,   166,   167,   168,   169,   170,   171,   172
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    14,    17,    21,
      23,    25,    27,    29,    31,    33,    35,    37,    39,    41,
      43,    45,    47,    49,    51,    54,    57,    61,    65,    69,
      73,    77,    81,    85,    88,    92,    96,    99,   102,   106,
     109,   114,   118,   123,   128,   129,   131,   133,   134,   136,
     139,   143,   147,   149,   152,   154,   158,   162,   168,   169,
     171,   173,   175,   176,   178,   180,   185,   192,   201,   208,
     217,   221,   227,   231,   236,   241,   246,   251,   256,   257,
     259,   261,   263,   265,   267,   269,   271,   273,   275,   277,
     279,   281,   283,   285,   287,   289,   291,   293,   295,   297,
     299,   301,   303,   305,   307,   309,   311,   313,   315,   317,
     319,   321,   323,   325,   327,   329,   331,   333,   335,   337,
     339,   341,   343,   345,   347,   349,   351,   353,   355,   358,
     360,   363,   367,   369,   371,   374,   375,   379,   381,   383,
     385,   387,   389,   391,   393,   397,   403,   409,   412,   413,
     422,   423,   433,   434,   443,   444,   454,   458,   462,   468,
     474,   477,   483,   487,   494,   501,   506,   512,   518,   522,
     526,   533,   539,   546,   552,   559,   563,   568,   572,   577,
     584,   592,   599,   607,   612,   618,   623,   629,   634,   639,
     643,   650,   656,   663,   669,   678,   686,   689,   693,   697,
     700,   703,   707,   711,   712,   715,   718,   722,   728,   730,
     734,   738,   744,   750,   751,   754,   758,   761,   765,   772,
     779,   781,   783,   785,   786,   789,   790,   793,   797,   803,
     809,   811,   813,   816,   820,   822,   826,   827,   828,   837,
     838,   840,   842,   843,   844,   855,   859,   863,   869,   875,
     879,   881,   885,   887,   890,   892,   894,   896,   898,   900,
     902,   904,   906,   908,   910,   912,   914,   916,   918,   920,
     922,   924,   926,   928,   930,   932,   934,   936,   938,   940,
     943,   945,   947,   949,   951,   953,   955,   957,   959,   961,
     963,   965,   967,   968,   972,   976,   977,   979,   983,   988,
     994,   999,  1006,  1013,  1014,  1016,  1018,  1020,  1022,  1024,
    1027,  1030,  1032,  1034,  1036,  1037,  1039,  1041,  1044,  1046,
    1048,  1050,  1052,  1053,  1055,  1058,  1060,  1062,  1064,  1065,
    1067,  1069,  1071,  1073,  1075,  1078,  1080,  1082,  1085,  1088,
    1089,  1092,  1095,  1100,  1105,  1107,  1111,  1115,  1119,  1123,
    1127,  1131,  1135,  1138,  1140,  1142,  1146,  1151,  1156,  1159,
    1164,  1165,  1168,  1171,  1173,  1175,  1177,  1180,  1182,  1187,
    1191,  1193,  1197,  1201,  1207,  1209,  1211,  1215,  1217,  1220,
    1224,  1225,  1228,  1231,  1235,  1238,  1243,  1247,  1251,  1256,
    1260,  1261,  1264,  1267,  1270,  1272,  1273,  1276,  1279,  1282,
    1284,  1289,  1294,  1301,  1305,  1306,  1308,  1310,  1314,  1319,
    1323,  1328,  1335,  1336,  1339,  1342,  1345,  1348,  1350,  1352,
    1356,  1360,  1362,  1366,  1368,  1370,  1372,  1376,  1380,  1381,
    1383,  1385,  1389,  1393,  1397,  1399,  1401,  1403,  1405,  1407,
    1410,  1412,  1414,  1416,  1418,  1420,  1422,  1425,  1430,  1435,
    1440,  1446,  1449,  1452,  1454,  1457,  1459,  1462,  1464,  1467,
    1469,  1472,  1474,  1476,  1478,  1485,  1492,  1497,  1507,  1517,
    1525,  1532,  1539,  1544,  1554,  1564,  1572,  1577,  1584,  1591,
    1601,  1611,  1618,  1620,  1622,  1624,  1626,  1628,  1630,  1632,
    1634,  1638,  1639,  1641,  1646,  1648,  1652,  1657,  1659,  1663,
    1668,  1672,  1676,  1678,  1680,  1683,  1685,  1688,  1690,  1692,
    1696,  1698,  1701,  1704,  1707,  1710,  1713,  1722,  1731,  1741,
    1751,  1757,  1763,  1768,  1770,  1772,  1774,  1776,  1778,  1780,
    1782,  1784,  1789,  1793,  1797,  1800,  1803,  1805,  1806,  1808,
    1811,  1814,  1816,  1818,  1820,  1822,  1824,  1826,  1829,  1832,
    1834,  1839,  1844,  1849,  1853,  1857,  1861,  1865,  1871,  1875,
    1880,  1884,  1889,  1891,  1893,  1895,  1897,  1899,  1901,  1903,
    1905,  1907,  1909,  1913,  1918,  1922,  1927,  1931,  1936,  1940,
    1946,  1950,  1954,  1958,  1962,  1966,  1970,  1974,  1978,  1982,
    1986,  1990,  1994,  1998,  2002,  2006,  2010,  2014,  2018,  2022,
    2026,  2030,  2034,  2038,  2041,  2044,  2047,  2050,  2053,  2056,
    2059,  2063,  2067,  2071,  2075,  2079,  2083,  2087,  2091,  2093,
    2095,  2097,  2099,  2101,  2103
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     174,     0,    -1,   175,    -1,    -1,   175,   176,    -1,   178,
      -1,   177,   178,    -1,    74,     8,    -1,   177,    74,     8,
      -1,   180,    -1,   183,    -1,   189,    -1,   190,    -1,   191,
      -1,   202,    -1,   192,    -1,   211,    -1,   214,    -1,   212,
      -1,   221,    -1,   215,    -1,   216,    -1,   220,    -1,   201,
      -1,   291,   158,    -1,    14,   178,    -1,    15,   292,   178,
      -1,    16,   193,   158,    -1,    23,   292,   183,    -1,    28,
     193,   158,    -1,    30,   279,   158,    -1,    53,   195,   178,
      -1,    57,   303,   200,    -1,    57,   200,    -1,    66,   303,
     200,    -1,    88,   303,   200,    -1,    88,   200,    -1,    94,
     178,    -1,   114,   303,   158,    -1,     1,   158,    -1,   181,
     182,    60,   195,    -1,   179,   163,   164,    -1,   179,   163,
     184,   164,    -1,   179,   163,     1,   164,    -1,    -1,    79,
      -1,    76,    -1,    -1,    78,    -1,   163,   164,    -1,   163,
     184,   164,    -1,   163,     1,   164,    -1,   176,    -1,   184,
     176,    -1,   303,    -1,   303,    13,   303,    -1,   185,   139,
     303,    -1,   185,   139,   303,    13,   303,    -1,    -1,   185,
      -1,   161,    -1,   185,    -1,    -1,    79,    -1,    76,    -1,
     188,   107,   185,   158,    -1,   188,   107,   303,    36,   187,
     158,    -1,   188,   107,   303,    13,   303,    36,   187,   158,
      -1,   188,   107,   303,    67,   186,   158,    -1,   188,   107,
     303,    13,   303,    67,   186,   158,    -1,    45,   196,   158,
      -1,    45,   196,    13,   196,   158,    -1,    84,   278,   158,
      -1,   306,   242,   305,   158,    -1,   306,   162,   305,   158,
      -1,   306,   130,   305,   158,    -1,   306,   124,   305,   158,
      -1,   306,   125,   305,   158,    -1,    -1,     3,    -1,     3,
      -1,    64,    -1,    96,    -1,    39,    -1,    99,    -1,   197,
      -1,     3,    -1,    64,    -1,    96,    -1,    39,    -1,    99,
      -1,   197,    -1,     3,    -1,    96,    -1,    17,    -1,    50,
      -1,   103,    -1,    80,    -1,    44,    -1,    25,    -1,    20,
      -1,    92,    -1,    94,    -1,    90,    -1,    71,    -1,    89,
      -1,    18,    -1,   106,    -1,    33,    -1,    47,    -1,    58,
      -1,    65,    -1,   109,    -1,    17,    -1,    50,    -1,   103,
      -1,    80,    -1,    44,    -1,    25,    -1,    20,    -1,    92,
      -1,    58,    -1,    65,    -1,   109,    -1,    94,    -1,    90,
      -1,    33,    -1,    47,    -1,    32,   178,    -1,   183,    -1,
      85,   158,    -1,    85,   305,   158,    -1,   158,    -1,   204,
      -1,    79,   204,    -1,    -1,    76,   203,   204,    -1,   236,
      -1,   265,    -1,   228,    -1,   261,    -1,   224,    -1,   205,
      -1,   206,    -1,    42,   303,   158,    -1,    42,   303,    36,
     187,   158,    -1,    42,   303,    67,   186,   158,    -1,    42,
     265,    -1,    -1,    38,    81,   207,   195,   226,   163,   227,
     164,    -1,    -1,    38,     8,    81,   208,   195,   226,   163,
     227,   164,    -1,    -1,    37,    81,   209,   195,   226,   163,
     227,   164,    -1,    -1,    37,     8,    81,   210,   195,   226,
     163,   227,   164,    -1,    38,   304,   236,    -1,    37,   304,
     236,    -1,    38,   304,   264,   266,   158,    -1,    37,   304,
     264,   266,   158,    -1,    38,    11,    -1,    32,   178,   112,
     303,   158,    -1,   112,   303,   200,    -1,    24,   303,    46,
     303,   292,   200,    -1,    24,   303,    46,   213,   292,   200,
      -1,    24,   303,   292,   200,    -1,    40,   303,    46,   303,
     200,    -1,    40,   303,    46,   213,   200,    -1,    40,   303,
     200,    -1,    40,   213,   200,    -1,    40,    72,   195,    46,
     303,   200,    -1,    41,   303,    46,   303,   200,    -1,    41,
     303,    46,   303,   295,   200,    -1,    41,   303,    46,   213,
     200,    -1,    41,   303,    46,   213,   295,   200,    -1,    41,
     303,   200,    -1,    41,   303,   295,   200,    -1,    41,   213,
     200,    -1,    41,   213,   295,   200,    -1,   167,   278,    46,
     303,   168,   178,    -1,   167,   278,    46,   303,   295,   168,
     178,    -1,   167,   278,    46,   213,   168,   178,    -1,   167,
     278,    46,   213,   295,   168,   178,    -1,   167,   278,   168,
     178,    -1,   167,   278,   295,   168,   178,    -1,   167,   213,
     168,   178,    -1,   167,   213,   295,   168,   178,    -1,   115,
     165,   278,   166,    -1,    43,   303,    95,   178,    -1,    43,
     303,   183,    -1,    43,   303,    95,   178,    34,   178,    -1,
      43,   303,   183,    34,   178,    -1,    43,   303,   242,   303,
      95,   178,    -1,    43,   303,   242,   303,   183,    -1,    43,
     303,   242,   303,    95,   178,    34,   178,    -1,    43,   303,
     242,   303,   183,    34,   178,    -1,    29,   178,    -1,   100,
     303,   158,    -1,   101,   303,   158,    -1,   100,   192,    -1,
     101,   192,    -1,   100,   183,   217,    -1,   101,   183,   217,
      -1,    -1,   217,   218,    -1,    21,   183,    -1,    21,   219,
     183,    -1,    21,   165,   219,   166,   183,    -1,   195,    -1,
     195,   138,   303,    -1,    97,   303,   158,    -1,    87,   303,
     163,   222,   164,    -1,    87,   303,   163,     1,   164,    -1,
      -1,   222,   223,    -1,   110,   278,   200,    -1,    68,   178,
      -1,    68,    32,   178,    -1,   225,   195,   226,   163,   227,
     164,    -1,   225,   195,   226,   163,     1,   164,    -1,    22,
      -1,    81,    -1,   105,    -1,    -1,   138,   278,    -1,    -1,
     227,   202,    -1,   227,   177,   202,    -1,   229,   195,   163,
     230,   164,    -1,   229,   195,   163,     1,   164,    -1,    35,
      -1,   231,    -1,   230,   139,    -1,   230,   139,   231,    -1,
     195,    -1,   195,   118,   303,    -1,    -1,    -1,    54,   233,
     244,   234,   251,   273,   257,   254,    -1,    -1,    48,    -1,
      70,    -1,    -1,    -1,   235,   250,   237,   239,   238,   251,
     272,   252,   257,   253,    -1,   249,   241,   243,    -1,   249,
     242,   243,    -1,   249,   240,   141,   241,   243,    -1,   249,
     240,   141,   242,   243,    -1,   249,     1,   243,    -1,   285,
      -1,   165,   303,   166,    -1,   194,    -1,   135,   195,    -1,
     134,    -1,   136,    -1,   137,    -1,   135,    -1,   144,    -1,
     154,    -1,   151,    -1,   147,    -1,   150,    -1,   146,    -1,
     156,    -1,   152,    -1,   161,    -1,   140,    -1,   159,    -1,
     160,    -1,   153,    -1,   145,    -1,   133,    -1,    19,    -1,
     148,    -1,    12,    -1,   162,    -1,   149,    -1,    52,    -1,
     195,   133,    -1,   118,    -1,   129,    -1,   126,    -1,   128,
      -1,   122,    -1,   127,    -1,   123,    -1,   119,    -1,   120,
      -1,   121,    -1,   132,    -1,   131,    -1,    -1,   165,   245,
     166,    -1,   165,   245,   166,    -1,    -1,   246,    -1,   245,
     139,   246,    -1,   247,   195,   277,   270,    -1,   177,   247,
     195,   277,   270,    -1,   247,   195,   277,   256,    -1,   247,
     165,   269,   166,   277,   270,    -1,   247,   165,   269,   166,
     277,   256,    -1,    -1,   248,    -1,    46,    -1,    49,    -1,
      69,    -1,    27,    -1,    27,    46,    -1,    27,    83,    -1,
      72,    -1,    83,    -1,   102,    -1,    -1,    72,    -1,    83,
      -1,    27,    83,    -1,    27,    -1,   102,    -1,    77,    -1,
      51,    -1,    -1,    27,    -1,    27,    83,    -1,    83,    -1,
      72,    -1,   102,    -1,    -1,    98,    -1,   158,    -1,   254,
      -1,   183,    -1,   201,    -1,   157,   195,    -1,     4,    -1,
     143,    -1,   143,   303,    -1,   143,   255,    -1,    -1,   111,
     303,    -1,    56,   258,    -1,   111,   303,    56,   258,    -1,
      56,   258,   111,   303,    -1,   259,    -1,   258,   139,   259,
      -1,   260,   118,   260,    -1,   260,   150,   260,    -1,   260,
     151,   260,    -1,   260,   144,   260,    -1,   260,   146,   260,
      -1,   260,   147,   260,    -1,    85,   260,    -1,     3,    -1,
      96,    -1,   102,   262,   158,    -1,    26,   102,   262,   158,
      -1,    38,   102,   262,   158,    -1,   195,   263,    -1,   195,
     263,   139,   262,    -1,    -1,   118,   286,    -1,   118,   274,
      -1,    72,    -1,    27,    -1,    83,    -1,    27,    83,    -1,
     108,    -1,    26,   264,   266,   158,    -1,   264,   266,   158,
      -1,   267,    -1,   266,   139,   267,    -1,   195,   273,   270,
      -1,   165,   269,   166,   273,   270,    -1,   104,    -1,   195,
      -1,   165,   269,   166,    -1,   268,    -1,   268,   139,    -1,
     268,   139,   269,    -1,    -1,   118,    63,    -1,   118,   305,
      -1,   167,   168,   286,    -1,   167,   168,    -1,   167,   278,
     168,   286,    -1,   167,   278,   168,    -1,   167,   168,   271,
      -1,   167,   278,   168,   271,    -1,   167,     1,   168,    -1,
      -1,   138,   286,    -1,   138,   271,    -1,   138,   199,    -1,
       1,    -1,    -1,   138,   286,    -1,   138,   274,    -1,   138,
     199,    -1,     1,    -1,   167,   278,   168,   286,    -1,   167,
     278,   168,   274,    -1,   167,   278,    46,   303,   168,   286,
      -1,   167,     1,   168,    -1,    -1,   286,    -1,   255,    -1,
     167,   168,   275,    -1,   167,   278,   168,   275,    -1,   167,
     168,   276,    -1,   167,   278,   168,   276,    -1,   167,   278,
      46,   303,   168,   275,    -1,    -1,   138,   286,    -1,   138,
     255,    -1,   138,   199,    -1,   138,   276,    -1,   303,    -1,
     255,    -1,   278,   139,   303,    -1,   278,   139,   255,    -1,
     303,    -1,   279,   139,   303,    -1,   104,    -1,   305,    -1,
     255,    -1,   280,   139,   280,    -1,   281,   139,   280,    -1,
      -1,   283,    -1,   284,    -1,   283,   139,   284,    -1,   196,
     118,   255,    -1,   196,   118,   305,    -1,   255,    -1,   305,
      -1,   196,    -1,   198,    -1,   287,    -1,   287,   157,    -1,
     157,    -1,   290,    -1,   306,    -1,   289,    -1,   315,    -1,
     314,    -1,    90,   303,    -1,    47,   165,   282,   166,    -1,
      33,   165,   282,   166,    -1,    93,   165,   282,   166,    -1,
      91,    93,   165,   282,   166,    -1,    14,   303,    -1,    94,
     303,    -1,    71,    -1,    71,   303,    -1,   106,    -1,   106,
     303,    -1,    89,    -1,    89,   303,    -1,    18,    -1,    18,
     303,    -1,    22,    -1,    81,    -1,    35,    -1,    40,   303,
      46,   303,    32,   303,    -1,    40,   303,    46,   213,    32,
     303,    -1,    40,   303,    32,   303,    -1,    40,   303,    46,
     303,    32,    43,   303,    95,   303,    -1,    40,   303,    46,
     213,    32,    43,   303,    95,   303,    -1,    40,   303,    32,
      43,   303,    95,   303,    -1,    41,   303,    46,   303,    32,
     303,    -1,    41,   303,    46,   213,    32,   303,    -1,    41,
     303,    32,   303,    -1,    41,   303,    46,   303,    32,    43,
     303,    95,   303,    -1,    41,   303,    46,   213,    32,    43,
     303,    95,   303,    -1,    41,   303,    32,    43,   303,    95,
     303,    -1,   167,   278,   168,   303,    -1,   167,   278,    46,
     303,   168,   303,    -1,   167,   278,    46,   213,   168,   303,
      -1,   167,   278,    46,   303,   168,    43,   303,    95,   303,
      -1,   167,   278,    46,   213,   168,    43,   303,    95,   303,
      -1,    43,   303,    95,   303,    34,   303,    -1,    62,    -1,
     290,    -1,   285,    -1,   309,    -1,   308,    -1,   232,    -1,
     301,    -1,   302,    -1,   299,   149,   303,    -1,    -1,   293,
      -1,   113,   165,   294,   166,    -1,   297,    -1,   294,   139,
     297,    -1,   113,   165,   296,   166,    -1,   297,    -1,   296,
     139,   297,    -1,   298,   285,   273,   270,    -1,   318,    82,
     285,    -1,   303,    82,   285,    -1,    27,    -1,    46,    -1,
      27,    46,    -1,    83,    -1,    27,    83,    -1,   108,    -1,
     306,    -1,   299,   149,   303,    -1,    61,    -1,    61,    71,
      -1,    61,    89,    -1,    61,   106,    -1,    61,    18,    -1,
     300,   303,    -1,    61,    71,   165,   303,   166,   165,   282,
     166,    -1,    61,    89,   165,   303,   166,   165,   282,   166,
      -1,    61,    71,   165,   303,   166,   165,   282,   166,   157,
      -1,    61,    89,   165,   303,   166,   165,   282,   166,   157,
      -1,    61,    71,   165,   303,   166,    -1,    61,    89,   165,
     303,   166,    -1,    55,   266,    46,   303,    -1,   312,    -1,
     286,    -1,   288,    -1,   316,    -1,   317,    -1,   232,    -1,
     301,    -1,   302,    -1,   165,   143,   303,   166,    -1,   303,
     138,   303,    -1,   303,   142,   303,    -1,   303,   142,    -1,
     142,   303,    -1,   142,    -1,    -1,   303,    -1,   100,   303,
      -1,   101,   303,    -1,   303,    -1,   285,    -1,   308,    -1,
     309,    -1,   310,    -1,   306,    -1,   303,   133,    -1,   287,
     157,    -1,   232,    -1,   307,   165,   282,   166,    -1,   307,
     167,   282,   168,    -1,    75,   165,   282,   166,    -1,   303,
     141,   196,    -1,   303,   141,   102,    -1,   303,   141,    33,
      -1,   303,   141,    58,    -1,   303,   141,    20,   165,   166,
      -1,   165,   280,   166,    -1,   165,   280,   139,   166,    -1,
     165,   281,   166,    -1,   165,   281,   139,   166,    -1,    39,
      -1,    99,    -1,   311,    -1,     5,    -1,     6,    -1,     7,
      -1,     8,    -1,     9,    -1,    10,    -1,    64,    -1,   163,
     278,   164,    -1,   163,   278,   139,   164,    -1,   167,   278,
     168,    -1,   167,   278,   139,   168,    -1,   167,   313,   168,
      -1,   167,   313,   139,   168,    -1,   303,   116,   303,    -1,
     313,   139,   303,   116,   303,    -1,   303,   156,   303,    -1,
     303,   152,   303,    -1,   303,   161,   303,    -1,   303,   140,
     303,    -1,   303,   159,   303,    -1,   303,   160,   303,    -1,
     303,   153,   303,    -1,   303,   144,   303,    -1,   303,   154,
     303,    -1,   303,   151,   303,    -1,   303,   147,   303,    -1,
     303,   150,   303,    -1,   303,   146,   303,    -1,   303,   134,
     303,    -1,   303,   136,   303,    -1,   303,   137,   303,    -1,
     303,   117,   303,    -1,   303,   155,   303,    -1,   303,   145,
     303,    -1,   303,    19,   303,    -1,   303,    12,   303,    -1,
     303,   148,   303,    -1,   303,    31,   303,    -1,   156,   303,
      -1,   152,   303,    -1,    59,   303,    -1,    73,   303,    -1,
     133,   303,    -1,   303,   133,    -1,   135,   303,    -1,   303,
      82,   303,    -1,   303,    82,   213,    -1,   318,    82,   303,
      -1,   318,    82,   213,    -1,   303,    86,   303,    -1,   303,
      86,   213,    -1,   318,    86,   303,    -1,   318,    86,   213,
      -1,   156,    -1,   161,    -1,   117,    -1,   155,    -1,   134,
      -1,   136,    -1,   137,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   497,   497,   502,   503,   509,   510,   515,   516,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,   539,   540,   541,
     542,   543,   544,   545,   546,   547,   548,   549,   550,   551,
     555,   568,   573,   578,   586,   587,   588,   592,   593,   606,
     607,   608,   613,   614,   619,   624,   629,   633,   640,   645,
     649,   654,   658,   659,   660,   664,   665,   666,   667,   668,
     672,   673,   678,   682,   684,   686,   688,   690,   697,   698,
     702,   703,   704,   705,   706,   707,   710,   711,   712,   713,
     714,   715,   727,   728,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,   750,   751,   752,   753,   754,
     755,   756,   757,   761,   762,   763,   764,   765,   766,   767,
     768,   769,   770,   771,   778,   779,   780,   781,   785,   786,
     790,   791,   795,   796,   797,   807,   807,   812,   813,   814,
     815,   816,   817,   818,   822,   823,   824,   825,   830,   829,
     845,   844,   861,   860,   876,   875,   891,   895,   900,   908,
     919,   926,   927,   928,   929,   930,   931,   932,   933,   934,
     935,   936,   937,   938,   939,   940,   941,   942,   943,   944,
     950,   956,   962,   968,   975,   982,   986,   993,   997,   998,
     999,  1000,  1001,  1003,  1005,  1007,  1012,  1015,  1016,  1017,
    1018,  1019,  1020,  1024,  1025,  1029,  1030,  1031,  1035,  1036,
    1040,  1043,  1045,  1050,  1051,  1055,  1057,  1059,  1066,  1076,
    1090,  1095,  1100,  1108,  1109,  1114,  1115,  1117,  1122,  1138,
    1145,  1154,  1162,  1166,  1173,  1174,  1179,  1184,  1178,  1211,
    1214,  1218,  1226,  1236,  1225,  1268,  1272,  1277,  1281,  1286,
    1293,  1294,  1298,  1299,  1300,  1301,  1302,  1303,  1304,  1305,
    1306,  1307,  1308,  1309,  1310,  1311,  1312,  1313,  1314,  1315,
    1316,  1317,  1318,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1329,  1330,  1331,  1332,  1333,  1334,  1335,  1336,  1337,  1338,
    1339,  1340,  1344,  1345,  1349,  1353,  1354,  1355,  1359,  1361,
    1363,  1365,  1367,  1372,  1373,  1377,  1378,  1379,  1380,  1381,
    1382,  1383,  1384,  1385,  1389,  1390,  1391,  1392,  1393,  1394,
    1398,  1399,  1403,  1404,  1405,  1406,  1407,  1408,  1412,  1413,
    1416,  1417,  1421,  1422,  1426,  1431,  1435,  1436,  1437,  1445,
    1446,  1448,  1450,  1452,  1457,  1459,  1464,  1465,  1466,  1467,
    1468,  1469,  1470,  1474,  1476,  1481,  1483,  1485,  1490,  1503,
    1520,  1521,  1523,  1528,  1529,  1530,  1531,  1532,  1536,  1542,
    1550,  1551,  1559,  1561,  1566,  1568,  1570,  1575,  1577,  1579,
    1586,  1587,  1588,  1593,  1595,  1597,  1601,  1605,  1607,  1611,
    1619,  1620,  1621,  1622,  1623,  1628,  1629,  1630,  1631,  1632,
    1652,  1656,  1660,  1668,  1675,  1676,  1677,  1681,  1683,  1689,
    1691,  1693,  1698,  1699,  1700,  1701,  1702,  1708,  1709,  1710,
    1711,  1715,  1716,  1720,  1721,  1722,  1726,  1727,  1731,  1732,
    1736,  1737,  1741,  1742,  1743,  1744,  1748,  1749,  1760,  1762,
    1764,  1770,  1771,  1772,  1773,  1774,  1775,  1777,  1779,  1781,
    1783,  1785,  1787,  1790,  1792,  1794,  1796,  1798,  1800,  1802,
    1804,  1807,  1809,  1811,  1816,  1818,  1820,  1822,  1824,  1826,
    1828,  1830,  1832,  1834,  1836,  1838,  1840,  1847,  1853,  1859,
    1865,  1874,  1884,  1892,  1893,  1894,  1895,  1896,  1897,  1898,
    1899,  1904,  1905,  1909,  1913,  1914,  1918,  1922,  1923,  1927,
    1931,  1935,  1942,  1943,  1944,  1945,  1946,  1947,  1951,  1952,
    1957,  1959,  1963,  1967,  1971,  1979,  1984,  1990,  1996,  2003,
    2012,  2016,  2023,  2031,  2032,  2033,  2034,  2035,  2036,  2037,
    2038,  2039,  2041,  2043,  2045,  2047,  2049,  2054,  2055,  2059,
    2060,  2061,  2065,  2066,  2067,  2068,  2079,  2080,  2081,  2082,
    2086,  2087,  2088,  2092,  2093,  2094,  2095,  2096,  2104,  2105,
    2106,  2107,  2111,  2112,  2116,  2117,  2118,  2119,  2120,  2121,
    2122,  2123,  2124,  2125,  2126,  2127,  2128,  2132,  2140,  2141,
    2145,  2146,  2147,  2148,  2149,  2150,  2151,  2152,  2153,  2154,
    2155,  2156,  2157,  2158,  2159,  2160,  2161,  2162,  2163,  2164,
    2165,  2166,  2167,  2171,  2172,  2173,  2174,  2175,  2176,  2177,
    2181,  2182,  2183,  2184,  2188,  2189,  2190,  2191,  2196,  2197,
    2198,  2199,  2200,  2201,  2202
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
  "TFORWARDING", "TIF", "TIMAG", "TIMPORT", "TIN", "TINDEX", "TINLINE",
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
  "TNOELSE", "TUMINUS", "TUPLUS", "TLNOT", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_start", "module_decl_stmt", "access_control",
  "opt_prototype", "block_stmt", "stmt_ls", "renames_ls", "opt_only_ls",
  "except_ls", "use_access_control", "use_stmt", "import_stmt",
  "require_stmt", "assignment_stmt", "opt_label_ident", "ident_fn_def",
  "ident_def", "ident_use", "internal_type_ident_def", "scalar_type",
  "reserved_type_ident_use", "do_stmt", "return_stmt", "class_level_stmt",
  "@1", "private_decl", "forwarding_stmt", "extern_export_decl_stmt",
  "$@2", "$@3", "$@4", "$@5", "extern_block_stmt", "loop_stmt",
  "zippered_iterator", "if_stmt", "defer_stmt", "try_stmt",
  "catch_stmt_ls", "catch_stmt", "catch_expr", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt", "enum_header",
  "enum_ls", "enum_item", "lambda_decl_expr", "$@6", "$@7", "linkage_spec",
  "fn_decl_stmt", "$@8", "$@9", "fn_decl_stmt_inner",
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
  "shadow_var_prefix", "io_expr", "new_maybe_decorated", "new_expr",
  "let_expr", "expr", "opt_expr", "opt_try_expr", "lhs_expr", "fun_expr",
  "call_expr", "dot_expr", "parenthesized_expr", "bool_literal",
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
     425,   426,   427
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   173,   174,   175,   175,   176,   176,   177,   177,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     179,   180,   180,   180,   181,   181,   181,   182,   182,   183,
     183,   183,   184,   184,   185,   185,   185,   185,   186,   186,
     187,   187,   188,   188,   188,   189,   189,   189,   189,   189,
     190,   190,   191,   192,   192,   192,   192,   192,   193,   193,
     194,   194,   194,   194,   194,   194,   195,   195,   195,   195,
     195,   195,   196,   196,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   199,   199,   199,   199,   200,   200,
     201,   201,   202,   202,   202,   203,   202,   204,   204,   204,
     204,   204,   204,   204,   205,   205,   205,   205,   207,   206,
     208,   206,   209,   206,   210,   206,   206,   206,   206,   206,
     211,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   213,   214,   214,
     214,   214,   214,   214,   214,   214,   215,   216,   216,   216,
     216,   216,   216,   217,   217,   218,   218,   218,   219,   219,
     220,   221,   221,   222,   222,   223,   223,   223,   224,   224,
     225,   225,   225,   226,   226,   227,   227,   227,   228,   228,
     229,   230,   230,   230,   231,   231,   233,   234,   232,   235,
     235,   235,   237,   238,   236,   239,   239,   239,   239,   239,
     240,   240,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   243,   243,   244,   245,   245,   245,   246,   246,
     246,   246,   246,   247,   247,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   249,   249,   249,   249,   249,   249,
     250,   250,   251,   251,   251,   251,   251,   251,   252,   252,
     253,   253,   254,   254,   255,   255,   256,   256,   256,   257,
     257,   257,   257,   257,   258,   258,   259,   259,   259,   259,
     259,   259,   259,   260,   260,   261,   261,   261,   262,   262,
     263,   263,   263,   264,   264,   264,   264,   264,   265,   265,
     266,   266,   267,   267,   268,   268,   268,   269,   269,   269,
     270,   270,   270,   271,   271,   271,   271,   271,   271,   271,
     272,   272,   272,   272,   272,   273,   273,   273,   273,   273,
     274,   274,   274,   274,   275,   275,   275,   276,   276,   276,
     276,   276,   277,   277,   277,   277,   277,   278,   278,   278,
     278,   279,   279,   280,   280,   280,   281,   281,   282,   282,
     283,   283,   284,   284,   284,   284,   285,   285,   286,   286,
     286,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   289,   290,   291,   291,   291,   291,   291,   291,   291,
     291,   292,   292,   293,   294,   294,   295,   296,   296,   297,
     297,   297,   298,   298,   298,   298,   298,   298,   299,   299,
     300,   300,   300,   300,   300,   301,   301,   301,   301,   301,
     301,   301,   302,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   304,   304,   305,
     305,   305,   306,   306,   306,   306,   307,   307,   307,   307,
     308,   308,   308,   309,   309,   309,   309,   309,   310,   310,
     310,   310,   311,   311,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   313,   313,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   315,   315,   315,   315,   315,   315,   315,
     316,   316,   316,   316,   317,   317,   317,   317,   318,   318,
     318,   318,   318,   318,   318
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     2,     3,     3,     2,     2,     3,     2,
       4,     3,     4,     4,     0,     1,     1,     0,     1,     2,
       3,     3,     1,     2,     1,     3,     3,     5,     0,     1,
       1,     1,     0,     1,     1,     4,     6,     8,     6,     8,
       3,     5,     3,     4,     4,     4,     4,     4,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       2,     3,     1,     1,     2,     0,     3,     1,     1,     1,
       1,     1,     1,     1,     3,     5,     5,     2,     0,     8,
       0,     9,     0,     8,     0,     9,     3,     3,     5,     5,
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
       1,     1,     1,     1,     2,     1,     1,     2,     2,     0,
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
       2,     1,     1,     1,     6,     6,     4,     9,     9,     7,
       6,     6,     4,     9,     9,     7,     4,     6,     6,     9,
       9,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     1,     4,     1,     3,     4,     1,     3,     4,
       3,     3,     1,     1,     2,     1,     2,     1,     1,     3,
       1,     2,     2,     2,     2,     2,     8,     8,     9,     9,
       5,     5,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     3,     3,     2,     2,     1,     0,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       4,     4,     4,     3,     3,     3,     3,     5,     3,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
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
       3,     0,     0,     1,     0,    92,   565,   566,   567,   568,
     569,   570,     0,   491,    78,   113,   459,   119,   461,   491,
       0,   118,     0,   364,    78,     0,     0,     0,     0,   463,
     537,   537,   562,     0,     0,     0,     0,   117,     0,     0,
     240,   114,     0,   236,     0,     0,   121,     0,   510,   482,
     571,   122,     0,   241,   453,   363,     0,     0,     0,   135,
      45,   116,   462,   365,     0,     0,     0,     0,   457,     0,
       0,   120,     0,     0,    93,     0,   563,     0,     0,     0,
     115,   222,   455,   367,   123,     0,     0,   620,     0,   622,
       0,   623,   624,   536,     0,   621,   618,   440,   132,   619,
       0,     0,     0,     4,     0,     5,     0,     9,    47,    10,
       0,    11,    12,    13,    15,   436,   437,    23,    14,   133,
     142,   143,    16,    18,    17,    20,    21,    22,    19,   141,
       0,   139,     0,   528,     0,   137,   140,     0,   138,   542,
     524,   438,   525,   443,   441,     0,     0,     0,   529,   530,
       0,   442,     0,   543,   544,   545,   564,   523,   445,   444,
     526,   527,     0,    39,    25,   451,     0,     0,   492,    79,
       0,     0,   461,   463,     0,     0,     0,   462,     0,     0,
       0,   528,   542,   441,   529,   530,   460,   442,   543,   544,
       0,   491,     0,     0,   366,     0,   196,     0,   421,     0,
     428,   568,   462,   538,   239,   568,   160,   462,     0,   239,
       0,     0,     0,     0,     0,     0,     0,   147,     0,     0,
       0,   428,    86,    94,   106,   100,    99,   108,    89,    98,
     109,    95,   110,    87,   111,   104,    97,   105,   103,   101,
     102,    88,    90,    96,   107,   112,     0,    91,     0,     0,
       0,     0,   370,     0,   129,    33,     0,   605,   514,   511,
     512,   513,     0,   454,   606,     7,   428,   239,   220,   230,
     537,   221,   134,   335,   440,   418,     0,   417,     0,     0,
     130,   541,     0,     0,    36,     0,   458,   446,     0,   428,
      37,   452,     0,   203,   199,     0,   442,   203,   200,     0,
     360,     0,   456,     0,     0,   607,   609,   535,   604,   603,
       0,    49,    52,     0,     0,   423,     0,   425,     0,     0,
     424,     0,     0,   417,     0,     0,     6,     0,    48,     0,
       0,   223,     0,   321,   320,   242,     0,   439,    24,     0,
     515,     0,     0,     0,     0,     0,     0,   608,     0,     0,
       0,     0,     0,     0,   534,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     280,   287,   288,   289,   284,   286,     0,     0,   282,   285,
     283,   281,     0,   291,   290,     0,     0,   428,   428,     0,
       0,     0,    26,    27,     0,     0,     0,     0,     0,    28,
       0,     0,     0,     0,    29,     0,    30,     0,   436,   434,
       0,   429,   430,   435,   154,     0,   157,     0,   150,     0,
       0,   156,     0,     0,     0,   169,     0,     0,   168,     0,
     177,     0,     0,     0,   175,     0,     0,    58,   144,     0,
     189,     0,     0,    70,     0,    31,   303,   237,   374,     0,
     375,   377,     0,   399,     0,   380,     0,     0,   128,    32,
       0,     0,    34,     0,   136,   334,     0,    72,   539,   540,
     131,     0,    35,   428,     0,   210,   201,   197,   202,   198,
       0,   358,   355,   162,    38,    51,    50,    53,     0,   572,
       0,     0,   558,     0,   560,     0,     0,     0,     0,     0,
       0,     0,     0,   576,     8,     0,    41,     0,     0,     0,
      54,     0,     0,     0,   314,   369,   490,   600,   599,   602,
     611,   610,   615,   614,   596,   593,   594,   595,   532,   583,
       0,   555,   556,   554,   553,   533,   587,   598,   592,   590,
     601,   591,   589,   581,   586,   588,   597,   580,   584,   585,
     582,     0,     0,     0,     0,     0,     0,     0,   613,   612,
     617,   616,   502,   503,   505,   507,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   574,   491,   491,
     165,   356,   368,   422,     0,     0,   448,     0,     0,   223,
       0,     0,   223,   357,     0,     0,     0,     0,   466,     0,
       0,     0,   178,     0,   472,     0,     0,   176,   619,    61,
       0,    54,    59,     0,   188,     0,     0,     0,     0,   447,
     308,   305,   306,   307,   311,   312,   313,   303,     0,   296,
       0,   304,   322,     0,   378,     0,   126,   127,   125,   124,
       0,   398,   397,   524,     0,   372,   522,   371,     0,     0,
     552,   420,   419,     0,     0,     0,   449,     0,   204,   362,
     524,     0,   573,   531,   559,   426,   561,   427,   185,     0,
       0,     0,   575,   183,   476,     0,   578,   577,     0,    43,
      42,    40,     0,    65,     0,     0,    58,   224,     0,     0,
     234,     0,   231,   318,   315,   316,   319,   243,     0,     0,
      76,    77,    75,    74,    73,   550,   551,   504,   506,     0,
     493,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   161,   432,   433,   431,   223,     0,
     159,   223,     0,   158,     0,   187,     0,     0,   167,     0,
     166,     0,   497,     0,     0,   173,     0,     0,   171,     0,
     145,     0,   146,     0,     0,   191,     0,   193,    71,   309,
     310,     0,   303,   294,     0,   412,   323,   326,   325,   327,
       0,   376,   379,   380,     0,     0,   381,   382,   520,   521,
     212,     0,     0,   211,   214,   450,     0,   205,   208,     0,
     359,   186,     0,     0,     0,     0,   184,     0,    56,    55,
       0,     0,     0,   239,   229,     0,   232,   228,   317,   322,
     292,    80,   275,    94,   273,   100,    99,    83,    98,    95,
     278,   110,    81,   111,    97,   101,    82,    84,    96,   112,
     272,   254,   257,   255,   256,   267,   258,   271,   263,   261,
     274,   277,   262,   260,   265,   270,   259,   264,   268,   269,
     266,   276,     0,   252,     0,    85,     0,   292,   292,   250,
     557,   495,   380,   542,   542,     0,     0,     0,     0,     0,
       0,     0,     0,   164,   163,     0,   225,     0,   225,   170,
       0,     0,   465,     0,   464,     0,   496,     0,     0,   471,
     174,     0,   470,   172,    55,   190,   481,   192,     0,   412,
     297,     0,     0,   380,   324,   339,   373,   403,     0,   574,
     428,   428,     0,   216,     0,     0,     0,   206,     0,   181,
     478,     0,     0,   179,   477,     0,   579,     0,     0,    58,
      66,    68,   219,   135,   239,   218,   239,   226,   235,   233,
       0,   303,   249,   253,     0,   279,     0,   245,   246,   499,
       0,     0,     0,     0,     0,     0,     0,     0,   225,   239,
     225,   239,   469,     0,     0,   498,   475,     0,     0,     0,
     195,   380,   412,     0,   415,   414,   416,   524,   336,   300,
     298,     0,     0,     0,     0,   401,   524,     0,     0,   217,
     215,     0,   209,     0,   182,     0,   180,    57,     0,     0,
     227,   394,     0,   328,     0,   251,    80,    82,   292,   292,
       0,     0,     0,     0,     0,     0,   239,   153,   239,   149,
       0,     0,     0,     0,   194,   299,   380,   404,     0,   338,
     337,   353,     0,   354,   341,   344,     0,   340,   332,   333,
     238,     0,   516,   517,   207,     0,     0,    67,    69,     0,
     393,   392,   524,   329,   339,   293,   247,   248,     0,     0,
       0,     0,     0,     0,   155,   151,   468,   467,   474,   473,
     302,   301,   406,   407,   409,   524,     0,   574,   352,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   524,   518,
     519,   480,   479,     0,   384,     0,     0,     0,   408,   410,
     343,   345,   346,   349,   350,   351,   347,   348,   342,   389,
     387,   524,   574,   330,   244,   331,   404,   388,   524,   411
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   312,   104,   614,   106,   107,   108,   329,
     109,   313,   609,   613,   610,   110,   111,   112,   113,   114,
     170,   853,   250,   115,   247,   116,   641,   255,   117,   118,
     267,   119,   120,   121,   419,   591,   415,   588,   122,   123,
     720,   124,   125,   126,   476,   658,   789,   127,   128,   654,
     784,   129,   130,   512,   803,   131,   132,   691,   692,   181,
     248,   632,   134,   135,   514,   809,   697,   856,   857,   441,
     942,   447,   628,   629,   630,   631,   698,   335,   770,  1054,
    1114,  1040,   275,   979,   983,  1034,  1035,  1036,   136,   301,
     481,   137,   138,   251,   252,   451,   452,   645,  1051,  1003,
     455,   642,  1073,   976,   903,   314,   197,   318,   319,   410,
     411,   412,   182,   140,   141,   142,   143,   183,   145,   167,
     168,   566,   431,   741,   567,   568,   146,   147,   184,   185,
     150,   209,   413,   187,   152,   188,   189,   155,   156,   157,
     324,   158,   159,   160,   161,   162
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -981
static const yytype_int16 yypact[] =
{
    -981,    79,  2935,  -981,   -42,  -981,  -981,  -981,  -981,  -981,
    -981,  -981,  4421,    29,   174,  -981, 13067,  -981, 17606,    29,
    9998,  -981,   208,   132,   174,  4421,  9998,  4421,    64, 17664,
   10161,  8030,  -981,  8193,  8849,  6716,  9998,  -981,    61,    90,
    -981,  -981, 17701,  -981,  2626,  9012,  -981,  9998,   199,  -981,
    -981,  -981,  9998,  -981, 13067,  -981,  9998,   289,   137,    36,
   17889,  -981, 17759,  -981,  9177,  7539,  9998,  9012, 13067,  9998,
     287,  -981,   218,  4421,  -981,  9998,  -981, 10324, 10324, 17701,
    -981,  -981, 13067,  -981,  -981,  9998,  9998,  -981,  9998,  -981,
    9998,  -981,  -981, 12606,  9998,  -981,  9998,  -981,  -981,  -981,
    3433,  6881,  8358,  -981,  4258,  -981,   228,  -981,   316,  -981,
     290,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,
    -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,
   17701,  -981, 17701,   302,    44,  -981,  -981,  2626,  -981,   250,
    -981,   261,  -981,  -981,   268,   276,   270,  9998,   277,   282,
   17055, 12599,   363,   301,   308,  -981,  -981,  -981,  -981,  -981,
    -981,  -981,   391,  -981,  -981, 17055,   280,  4421,  -981,  -981,
     325,  9998,  -981,  -981,  9998,  9998,  9998,  -981,  9998,  9177,
    9177,   364,  -981,  -981,  -981,  -981,   -22,   372,  -981,  -981,
     311, 15292, 17701,  2626,  -981,   326,  -981,   154, 17055,   380,
    7704,   416, 17796, 17055,   213,   428,  -981, 17854, 17701,   213,
   17701,   348,    45, 14730,    15, 14654,   333,  -981, 15142, 13930,
      32,  7704,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,
    -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,
    -981,  -981,  -981,  -981,  -981,  -981,  4421,  -981,   351,  1094,
      47,    22,  -981,  4421,  -981,  -981, 14882,   365,  -981,   367,
     368,  -981, 14882,   -22,   365,  -981,  7704,  1290,  -981,  -981,
    9340,  -981,  -981,  -981, 17701,  -981,   168, 17055,  9998,  9998,
    -981, 17055,   361, 15043,  -981, 14882,   -22, 17055,   373,  7704,
    -981, 17055, 15519,  -981,  -981, 15599, 13060,  -981,  -981, 15679,
     433,   388,   -22, 14882, 15711,   170,   170, 17424,   365,   365,
      86,  -981,  -981,  3598,   -38,  -981,  9998,  -981,   102,   106,
    -981,   -14,     5, 15760,   -36,   547,  -981,  3763,  -981,   506,
    9998,   429,   417,  -981,  -981,  -981,   227,   378,  -981,  9998,
     438,  9998,  9998,  9998,  8849,  8849,  9998,   385,  9998,  9998,
    9998,  9998,  9998,   828, 12606,  9998,  9998,  9998,  9998,  9998,
    9998,  9998,  9998,  9998,  9998,  9998,  9998,  9998,  9998,  9998,
    -981,  -981,  -981,  -981,  -981,  -981,  8521,  8521,  -981,  -981,
    -981,  -981,  8521,  -981,  -981,  8521,  8521,  7704,  7704,  8849,
    8849,  7376,  -981,  -981, 15341, 15438, 15856,    13,  3928,  -981,
    8849,    45,   430,   229,  -981,  9998,  -981,  9998,   472,  -981,
     420,   453,  -981,  -981,  -981, 17701,  -981,  2626,  -981, 17701,
     435,  -981,  2626,   549,  9177,  -981,  4584,  8849,  -981,   431,
    -981,    45,  4747,  8849,  -981,    45, 10487,  9998,  -981,  4421,
     563,  9998,    61,  -981,   437,  -981,   475,  -981,  -981,  1094,
    -981,   467,   441,  -981, 10650,   490,  9998,  2626,  -981,  -981,
    9998,  9998,  -981,   445,  -981,  -981,  9177,  -981, 17055, 17055,
    -981,    38,  -981,  7704,   446,  -981,   594,  -981,   594,  -981,
   10813,   477,  -981,  -981,  -981,  -981,  -981,  -981,  8686,  -981,
   13669,  7046,  -981,  7211,  -981,  4421,   450,  8849,  6218,  3268,
     452,  9998,  6550,  -981,  -981,   259,  -981,  4093, 17701,   242,
   15195,  9177,   458, 17569,   451,  -981, 15905, 17260, 17260,   305,
    -981,   305,  -981,   305, 17336,   605,  1822,  2334,   -22,   170,
     457,  -981,  -981,  -981,  -981, 17424, 17393,   305, 12759, 12759,
   17260, 12759, 12759,  2063,   170, 17393, 17300,  2063,   365,   365,
     170,   466,   471,   479,   481,   485,   464,   478,  -981,   305,
    -981,   305,   138,  -981,  -981,  -981,   120,  -981, 13622, 17095,
     422, 10976,  8849, 11139,  8849,  9998,  8849, 13329,    29, 15950,
    -981,  -981,  -981, 17055, 16050,  7704,  -981,  7704, 17701,   429,
     243, 17701,   429,  -981,   245,  9998,   126,  9998, 17055,    60,
   14958,  7376,  -981,  9998, 17055,    26, 14806,  -981,   487,   508,
     491, 16095,   508,   492,   617, 16135,  4421, 14990,   494,  -981,
     179,  -981,  -981,  -981,  -981,  -981,  -981,   611,   140,  -981,
   13409,  -981,   551,   493,  1094,    47,    64,    90,  9998,  9998,
    6053,  -981,  -981,   672,  7867,  -981, 17055,  -981, 13709, 13762,
    -981,  -981, 17055,   497,    19,   496,  -981,  2498,  -981,  -981,
     253, 17701,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  4421,
     -13,  1411,  -981,  -981, 17055,  4421, 17055,  -981, 16247,  -981,
    -981,  -981,  9998,  -981,  9998, 10487,  9998,   519,  1638,   499,
     546,   114,  -981,   583,  -981,  -981,  -981,  -981,  2769,   501,
    -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  7376,
    -981,    33,  8849,  8849,  9998,   636, 16287,  9998,   643, 16327,
     509,  2139,    45,    45,  -981,  -981,  -981,  -981,   429,   513,
    -981,   429,   515,  -981, 14882,  -981, 14010,  4910,  -981,  5073,
    -981,   220,  -981, 14090,  5236,  -981,    45,  5399,  -981,    45,
    -981,  9998,  -981,  4421,  9998,  -981,  4421,   645,  -981,  -981,
    -981, 17701,   970,  -981,  1094,   544,   601,  -981,  -981,  -981,
      68,  -981,  -981,   490,   520,    56,  -981,  -981,   525,   528,
    -981,  5562,  9177,  -981,  -981,  -981, 17701,  -981,   557,   311,
    -981,  -981,  5725,   531,  5888,   535,  -981,  9998, 16439, 15487,
     538,   548,   543,  1733,  -981,  9998, 17701,  -981,  -981,   551,
     545,   148,  -981,   568,  -981,   570,   571,   582,   575,   576,
    -981,   581,   593,   590,   592,   595,   184,   604,   600,   607,
    -981,  -981, 17701,  -981,  -981,  -981,  -981,  -981,  -981,  -981,
    -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,
    -981,  -981,  9998,  -981,   616,   618,   613,   545,   545,  -981,
    -981,  -981,   490,   223,   224, 16479, 11302, 11465, 16519, 11628,
   11791, 11954, 12117,  -981,  -981,   589,  -981,   596,  -981,  -981,
    4421,  9998, 17055,  9998, 17055,  7376,  -981,  4421,  9998, 17055,
    -981,  9998, 17055,  -981, 17055,  -981, 17055,   700,  4421,   544,
    -981,   597,  9505,   166,  -981,    93,  -981,  -981,  8849, 13198,
    7704,  7704,  4421,  -981,    35,   602,  9998,  -981,  9998,  -981,
   17055,  4421,  9998,  -981, 17055,  4421, 17055,  9998, 10487,  9998,
    -981,  -981,  -981,  -981,  1290,  -981,  2083,  -981, 17055,  -981,
      49,   475,  -981,  -981, 13854,  -981, 13467,  -981,  -981,  -981,
    9998,  9998,  9998,  9998,  9998,  9998,  9998,  9998,  -981,  1978,
    -981,  2319, 16135, 14170, 14250,  -981, 16135, 14330, 14410,  4421,
    -981,   490,   544,  6384,  -981,  -981,  -981,   -35,  9177,  -981,
    -981,   142,  9998,    -5, 13523,  -981,   913,   606,   608,   380,
    -981,   311, 17055, 14490,  -981, 14570,  -981, 17055,   598,   603,
    -981,  -981, 12280,   662,   233,  -981,   634,   640,   545,   545,
   16631, 16671, 16711, 16823, 16863, 16903, 17231,  -981, 17245,  -981,
    4421,  4421,  4421,  4421,  -981,  -981,   166,  9670,    74,  -981,
   17055,  -981,   107,  -981,    89,  -981,   653, 17015,  -981,  -981,
    -981, 12117,   627,   629,  -981,  4421,  4421,  -981,  -981,  3100,
    -981,  -981,   354,  -981,    93,  -981,  -981,  -981,  9998,  9998,
    9998,  9998,  9998,  9998,  -981,  -981, 16135, 16135, 16135, 16135,
    -981,  -981,  -981,  -981,  -981,   121,  8849, 12771,  -981,  9998,
     142,   107,   107,   107,   107,   107,   107,   142,   984,  -981,
    -981, 16135, 16135,   620, 12443,    95,    85, 13612,  -981,  -981,
   17055,  -981,  -981,  -981,  -981,  -981,  -981,  -981,   652,  -981,
    -981,   357, 12936,  -981,  -981,  -981,  9835,  -981,   377,  -981
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -981,  -981,  -981,     7,  -434,  2012,  -981,  -981,  -981,  -981,
     386,   448,  -325,  -658,  -654,  -981,  -981,  -981,  -981,   169,
     770,  -981,   -17,    69,  -671,  -981,  -865,   564,  -940,  -767,
    -981,   -57,  -981,  -981,  -981,  -981,  -981,  -981,  -981,  -981,
     265,  -981,  -981,  -981,   510,  -981,    12,  -981,  -981,  -981,
    -981,  -981,  -981,  -498,  -738,  -981,  -981,  -981,     2,   988,
    -981,  -981,  -981,   200,  -981,  -981,  -981,  -981,  -136,  -150,
    -772,  -981,  -129,    51,   187,  -981,  -981,  -981,     9,  -981,
    -981,  -280,   336,  -207,  -232,  -264,  -255,  -240,  -981,  -179,
    -981,    10,   798,  -122,   379,  -981,  -423,  -753,  -818,  -981,
    -594,  -473,  -980,  -897,  -839,   -53,  -981,    63,  -981,  -191,
    -981,   260,   468,  -408,  -981,  -981,  -981,  1078,  -981,    -9,
    -981,  -981,  -199,  -981,  -584,  -981,  -981,  -981,  1192,  1439,
     -12,   820,   -63,   728,  -981,  1634,  1821,  -981,  -981,  -981,
    -981,  -981,  -981,  -981,  -981,  -385
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -575
static const yytype_int16 yytable[] =
{
     165,   386,   282,   272,   186,   509,   570,   659,   191,   103,
     190,   276,   627,   402,   198,   336,   435,   742,   203,   203,
     906,   213,   215,   218,   219,   246,   633,   855,   801,   420,
     444,   800,   193,   256,   453,   257,   937,   974,   320,   653,
     262,   773,   263,  1039,   264,   442,   643,   253,   453,   322,
    1001,   497,   277,   281,   283,   285,   286,   287,   744,   576,
     971,   291,   300,   292,     5,   295,   299,   253,   456,   453,
     302,   403,   660,   303,   304,   463,   305,   253,   306,     3,
      65,   307,   308,  -413,   309,   947,   948,   781,   277,   281,
     323,   729,   737,  -395,   732,   333,   -46,  1098,   474,   429,
     429,   488,   908,   502,  -413,  -390,  -213,   220,  -413,   949,
    1031,   347,   612,   331,   -46,   332,   163,   862,   429,   353,
    1076,   334,   496,   500,  -395,   861,   489,   397,   429,   782,
    1074,  -413,   503,  1026,  -390,   340,  1119,  1050,   959,   429,
     961,   576,   166,   -64,   498,  1031,   386,  -390,  -213,   981,
     980,  -395,   498,  -395,   495,   792,  1039,    74,   398,   165,
    -390,   457,   394,   395,   396,  -395,   291,   277,   323,  1000,
      65,   454,  -395,   499,   466,   300,   905,   169,   398,  -395,
    1099,   577,   401,   783,   707,   454,  -395,  1002,   281,   398,
     443,   300,   937,   423,   937,   498,   556,   557,   398,  -395,
    1079,   343,  -213,  1033,   982,  -395,   454,  -390,   398,   281,
     464,   772,  -390,   498,   417,   194,   570,   258,  1025,   422,
    1016,   708,  1018,   398,   909,   759,   193,  1032,  1080,   200,
     875,  -395,   450,   877,   498,    23,  1056,  1057,  1033,  -405,
      23,   491,  1077,  1113,   163,   493,   294,   298,   398,   937,
     485,   937,   344,   806,   281,   221,   345,   465,   203,   709,
    -405,    40,   760,  1112,  -405,   466,   468,   469,   492,   408,
     259,   999,   494,  1071,   998,   855,  1110,   281,   807,   762,
      55,   -86,   655,    53,   644,    55,   710,  -405,   260,   -92,
     408,    63,   735,   405,  1117,   590,    63,   265,   212,   214,
     594,   965,   266,   347,   490,   261,   763,   466,   351,   978,
     192,   353,   406,   551,   552,   356,    83,   -88,   510,   553,
     487,    83,   554,   555,   570,   -93,   467,   516,   627,   517,
     518,   519,   521,   523,   524,   408,   525,   526,   527,   528,
     529,   901,   535,   536,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   408,   885,
      23,   612,  -501,  -500,   281,   281,   457,   321,   457,   936,
     281,   596,   762,   281,   281,   281,   281,   559,   561,   569,
     288,   682,   457,   289,   457,   515,   886,   582,   579,  -501,
    -500,   327,  -361,   583,   328,   584,   343,   330,   589,  1055,
     683,   730,   592,   733,   416,    55,   746,   749,  -484,   421,
    -391,  -361,   277,  -383,   598,   600,    63,   163,   337,   339,
     604,   606,   534,   679,   611,   611,  -483,   615,   320,   617,
     320,   254,   450,  -385,   338,  -488,   985,   317,   347,  -391,
    -489,    83,  -383,   351,   646,   391,   353,   344,   648,   649,
     356,   345,  -391,   254,   652,  -383,   408,   408,   687,  -486,
    -487,   281,  -385,   293,   297,  -391,  -485,  -549,  -383,  -549,
     139,   793,   795,   389,   398,  -385,   652,   390,   693,   281,
     139,   281,   790,   393,   404,   671,   652,   674,  -385,   676,
     678,   681,   407,   139,   977,   139,   690,   414,   347,   277,
     570,   986,   620,   351,   713,   352,   353,   627,   390,   418,
     356,   618,  -391,   424,   487,  -383,   446,  -391,   363,   470,
    -383,   621,   726,   694,   622,   936,   369,   936,   387,  -549,
     388,  -549,   460,   461,   695,  -385,   409,  -546,   473,  -546,
    -385,   139,   408,  -548,   623,  -548,   482,   624,   858,    57,
    -547,   480,  -547,   696,   665,   504,   667,   409,   625,   598,
     716,   604,   719,   615,   721,   674,   508,   511,   139,   722,
     723,   728,   139,   281,   731,   281,   399,   626,   766,   353,
     513,   777,   936,   734,   936,   736,   586,   775,   581,   569,
     585,   743,   587,   593,  1052,   595,   601,   616,   254,   254,
     254,   254,   409,   619,   612,   440,   634,   635,   644,   520,
     522,   650,   656,   765,  -295,   657,   661,   450,   669,  1075,
     675,   688,   699,   767,   700,   409,   287,   291,   323,   701,
     705,   284,   281,  1088,   768,   139,   343,   702,   620,   703,
     788,  -295,   254,   704,   300,   -60,   706,   682,   254,   750,
     752,   753,   758,   769,   558,   560,   408,   621,   466,   771,
     622,   780,   785,   804,   805,   578,   808,   860,   866,  1075,
     798,   254,   799,   611,   611,   869,   876,   871,   878,   898,
     623,   854,   902,   624,   904,   325,  1111,   344,   907,   254,
     910,   345,   599,   911,   625,   916,   930,   569,   605,   921,
     521,   559,   865,   925,  1118,   868,   931,   932,  1075,  -113,
     941,  -119,  -118,   626,   139,   -89,  -117,  -114,  -396,   987,
     988,   139,  -121,   409,   409,   882,   -87,   884,  -396,   914,
     151,  -122,   889,  -116,   969,   892,  -120,   -90,   347,   894,
     151,  -115,   896,   351,   899,   352,   353,   450,  -123,   945,
     356,   -91,   958,   151,   946,   151,  1047,  -396,   363,   960,
    1053,  1048,   670,   972,   367,   368,   369,   -86,   991,   788,
     277,  1081,  1042,   -88,  1043,   507,   425,   428,   430,   434,
     920,   139,   924,  -396,  1089,   926,  1090,   254,  1109,   690,
    -396,  1080,  1078,   938,   195,   139,  1009,  1082,   915,  1083,
    1084,   151,   651,  1085,  1086,   296,   296,   478,   939,   409,
    1008,  -396,  1004,   900,   761,   943,  1115,   254,   940,  1070,
     459,   254,  1096,  1108,   651,  1101,   462,   317,   151,   317,
    -396,     5,   151,   217,   651,  -396,   647,   715,  -396,   718,
     944,  1102,  1103,  1104,  1105,  1106,  1107,   727,   530,   472,
     204,     0,     0,     0,   882,   884,     0,   889,   892,   920,
     924,   531,     0,     0,     0,     0,   139,   483,   962,   963,
       0,   964,     0,   569,     0,   966,   967,   272,     0,   968,
       0,     0,     0,     0,     0,     0,   532,     0,     0,     0,
       0,     0,     0,     0,   139,   151,   984,   674,   281,   281,
     139,     0,     0,     0,   992,     0,   993,   139,     0,     0,
     995,     0,     0,     0,     0,   997,   611,   611,     0,     0,
    1028,   725,     0,   409,    74,     0,     0,     0,     0,   854,
     533,     0,     0,     0,     0,     0,     0,     0,   962,  1010,
    1011,   966,  1012,  1013,  1014,  1015,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -400,
       0,   323,     0,   139,     0,   580,  1030,   139,     0,  -400,
    1037,     0,     0,     0,   151,   139,     0,   520,   558,   408,
     408,   151,     0,     0,     0,   254,   254,     0,     0,     0,
     133,   254,   254,     0,     0,   602,  1095,   620,  -400,   607,
     133,     0,     0,   757,     0,     0,     0,     0,  1066,  1067,
    1068,  1069,     0,   133,     0,   133,   621,     0,     0,   622,
       0,     0,     0,     0,  -400,     0,     0,     0,     0,   924,
    -402,  -400,     0,  1091,  1092,     0,   711,   323,     0,   623,
    -402,   151,   624,   787,    57,     0,  1066,  1067,  1068,  1069,
    1091,  1092,  -400,   625,     0,   151,     0,     0,     0,     0,
       0,   133,     0,     0,  1097,   674,     0,  1100,     0,  -402,
       0,  -400,   626,     0,     0,     0,  -400,     0,     0,  -400,
     144,     0,     0,     0,   139,     0,     0,     0,   133,     0,
     144,     0,   133,     0,     0,  -402,     0,   222,     0,     0,
     674,     0,  -402,   144,   924,   144,     0,     0,   254,   254,
       0,   223,   224,     0,   225,     0,     0,     0,     0,   226,
     254,     0,   440,  -402,     0,     0,   151,   227,     0,   440,
       0,     0,   254,   228,     0,   254,     0,   139,   229,     0,
       0,   230,  -402,   139,   231,     0,     0,  -402,     0,     0,
    -402,   144,   232,     0,   151,   133,     0,     0,   233,   234,
     151,     0,     0,   738,   740,   235,   859,   151,     0,   745,
     748,     0,     0,     0,   236,   917,     0,     0,   144,     0,
     863,   864,   144,   237,   238,     0,   239,     0,   240,     0,
     241,     0,     0,   242,   148,     0,     0,   243,   448,     0,
     244,     0,     0,   245,   148,   139,     0,   139,     0,     0,
       0,     0,   139,     0,     0,   139,     0,   148,     0,   148,
       0,   139,     0,   151,   139,     0,     0,   151,     0,     0,
       0,     0,     0,     0,   133,   151,     0,     0,   975,     0,
       0,   133,     0,     0,     0,   144,   409,   409,     0,   139,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   449,
     139,     0,   139,     0,     0,   148,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   873,   874,     0,     0,
       0,     0,   148,     0,     0,     0,   148,     0,   879,     0,
     254,   133,     0,     0,     0,     0,     0,     0,     0,     0,
     890,     0,   268,   893,  1029,   133,    22,    23,     0,     0,
       0,     0,     0,     0,   144,   269,     0,    30,   270,     0,
       0,   144,    35,     0,     0,     0,     0,     0,    40,     0,
       0,     0,     0,     0,   151,     0,     0,     0,   139,   440,
     440,     0,     0,   440,   440,   139,     0,     0,     0,   148,
      53,     0,    55,  1072,     0,     0,   139,     0,     0,  1038,
       0,   271,     0,    63,     0,     0,     0,  1044,     0,   440,
     139,   440,     0,     0,     0,     0,   133,     0,     0,   139,
       0,   144,    79,   139,     0,    81,     0,   151,    83,     0,
       0,     0,     0,   151,     0,   144,     0,     0,     0,     0,
       0,     0,     0,  1072,   133,     0,     0,     0,     0,     0,
     133,     0,     0,   341,     0,     0,     0,   133,     0,     0,
     342,     0,     0,     0,     0,     0,     0,   139,   148,     0,
       0,   149,   343,     0,     0,   148,     0,     0,     0,     0,
       0,   149,  1072,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   149,   151,   149,   151,     0,     0,
       0,     0,   151,     0,     0,   151,   144,     0,   990,     0,
       0,   151,  1038,   133,   151,     0,     0,   133,   139,   139,
     139,   139,     0,   344,     0,   133,     0,   345,     0,     0,
       0,     0,     0,     0,   144,   148,     0,     0,     0,   151,
     144,     0,   149,   139,   139,     0,     0,   144,     0,   148,
     151,     0,   151,     0,   429,     0,     0,     0,   346,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   149,
       0,     0,     0,   149,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,     0,   355,   356,   357,   358,   359,
       0,   360,   361,   362,   363,   364,   365,   366,     0,     0,
     367,   368,   369,   144,     0,     0,     0,   144,     0,   794,
       0,     0,     0,     0,     0,   144,     0,     0,     0,     0,
     148,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   133,     0,   149,     0,   151,     0,
       0,     0,     0,     0,     0,   151,     0,     0,   148,     0,
       0,     0,     0,     0,   148,     0,   151,     0,     0,     0,
       0,   148,     0,     0,     0,     0,   153,     0,     0,   802,
     151,     0,     0,     0,     0,     0,   153,     0,     0,   151,
       0,     0,     0,   151,     0,     0,     0,   133,     0,   153,
    -225,   153,     0,   133,  -225,  -225,     0,     0,     0,     0,
       0,     0,     0,  -225,     0,  -225,  -225,     0,     0,     0,
    -225,     0,     0,     0,     0,   149,  -225,   148,     0,  -225,
       0,   148,   149,     0,   144,     0,     0,   151,     0,   148,
       0,     0,     0,     0,     0,     0,     0,   153,  -225,     0,
    -225,     0,  -225,     0,  -225,  -225,     0,  -225,     0,  -225,
       0,  -225,     0,     0,     0,   133,     0,   133,     0,     0,
       0,     0,   133,     0,   153,   133,     0,     0,   153,     0,
    -225,   133,     0,  -225,   133,     0,  -225,   144,   151,   151,
     151,   151,   149,   144,     0,   268,     0,     0,     0,    22,
      23,     0,     0,     0,     0,     0,   149,     0,   269,   133,
      30,   270,     0,   151,   151,    35,     0,     0,     0,     0,
     133,    40,   133,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -225,     0,     0,     0,
       0,   153,  -225,    53,     0,    55,     0,    57,   148,   933,
       0,     0,   934,     0,   271,   144,    63,   144,     0,     0,
       0,     0,   144,   154,     0,   144,     0,     0,     0,     0,
       0,   144,     0,   154,   144,    79,     0,   149,    81,     0,
       0,    83,     0,     0,     0,     0,   154,     0,   154,     0,
       0,     0,     0,   343,     0,     0,     0,     0,     0,   144,
       0,   148,     0,     0,     0,   149,     0,   148,   133,     0,
     144,   149,   144,     0,     0,   133,     0,     0,   149,     0,
     153,     0,     0,     0,     0,     0,   133,   153,     0,     0,
       0,    98,     0,     0,   154,     0,     0,   935,     0,     0,
     133,     0,     0,     0,   344,     0,     0,     0,   345,   133,
       0,     0,     0,   133,     0,     0,     0,     0,     0,     0,
       0,   154,     0,     0,     0,   154,     0,     0,     0,   148,
       0,   148,     0,     0,   149,     0,   148,     0,   149,   148,
       0,     0,     0,     0,     0,   148,   149,   153,   148,     0,
       0,     0,     0,     0,     0,   347,   348,   133,   144,   350,
     351,   153,   352,   353,     0,   144,     0,   356,     0,     0,
       0,     0,     0,   148,     0,   363,   144,     0,     0,     0,
       0,   367,   368,   369,   148,     0,   148,     0,   154,     0,
     144,     0,     0,     0,     0,     0,     0,     0,     0,   144,
     268,     0,     0,   144,    22,    23,     0,     0,   133,   133,
     133,   133,     0,   269,   105,    30,   270,     0,     0,     0,
      35,     0,     0,     0,   164,     0,    40,     0,     0,     0,
       0,     0,   153,   133,   133,     0,     0,   196,     0,   199,
       0,     0,     0,     0,     0,     0,     0,   144,    53,     0,
      55,     0,    57,     0,   933,   149,     0,   934,     0,   271,
     153,    63,     0,     0,     0,     0,   153,   154,     0,     0,
       0,     0,   148,   153,   154,     0,     0,     0,     0,   148,
      79,     0,     0,    81,     0,   290,    83,     0,     0,     0,
     148,     0,     0,     0,   343,     0,     0,     0,   144,   144,
     144,   144,     0,     0,   148,   268,     0,     0,   149,    22,
      23,     0,   105,   148,   149,     0,   326,   148,   269,     0,
      30,   270,     0,   144,   144,    35,     0,     0,     0,   153,
       0,    40,     0,   153,   154,     0,    98,     0,     0,     0,
       0,   153,  1017,     0,     0,   344,     0,     0,   154,   345,
       0,   341,     0,    53,     0,    55,     0,   325,   342,   933,
       0,   148,   934,     0,   271,     0,    63,     0,     0,     0,
     343,     0,     0,     0,     0,     0,   149,     0,   149,   392,
       0,     0,     0,   149,     0,    79,   149,     0,    81,     0,
       0,    83,   149,     0,     0,   149,   347,   348,     0,   349,
     350,   351,     0,   352,   353,     0,     0,     0,   356,     0,
       0,     0,   148,   148,   148,   148,   363,     0,     0,   154,
     149,   344,   367,   368,   369,   345,     0,     0,     0,     0,
       0,   149,     0,   149,     0,     0,     0,   148,   148,     0,
       0,    98,     0,     0,     0,     0,     0,   154,     0,     0,
     153,     0,     0,   154,     0,     0,   346,     0,   445,     0,
     154,     0,     0,     0,     0,   458,     0,     0,     0,     0,
       0,     0,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,     0,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,     0,     0,   367,   368,
     369,     0,     0,   153,     0,     0,     0,   872,     0,   153,
       0,     0,     0,     0,     0,     0,   154,     0,     0,   149,
     154,     0,     0,     0,     0,   105,   149,     0,   154,     0,
       0,     0,     0,     0,     0,     0,     0,   149,     0,   105,
       0,   268,     0,     0,     0,    22,    23,     0,     0,     0,
       0,   149,     0,     0,   269,     0,    30,   270,     0,     0,
     149,    35,     0,     0,   149,   343,     0,    40,     0,     0,
       0,   153,     0,   153,     0,     0,     0,     0,   153,     0,
       0,   153,     0,     0,     0,     0,     0,   153,     0,    53,
     153,    55,     0,    57,     0,   933,     0,     0,   934,     0,
     271,     0,    63,     0,     0,     0,     0,     0,   149,     0,
     105,     0,     0,     0,     0,   153,   344,     0,     0,     0,
     345,    79,     0,     0,    81,     0,   153,    83,   153,     0,
       0,     0,     0,     0,     0,     0,     0,   154,   458,     0,
       0,     0,     0,     0,   458,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   149,
     149,   149,   149,     0,     0,     0,     0,   347,   348,     0,
       0,     0,   351,     0,   352,   353,     0,    98,     0,   356,
       0,     0,     0,  1019,   149,   149,     0,   363,     0,     0,
     154,     0,     0,   367,   368,   369,   154,     0,     0,     0,
       0,   222,     0,     0,     0,     0,     0,   668,     0,     0,
       0,   673,     0,     0,   153,   223,   224,     0,   225,   105,
       0,   153,     0,   226,     0,     0,     0,     0,     0,     0,
       0,   227,   153,     0,     0,     0,     0,   228,     0,     0,
       0,     0,   229,     0,     0,   230,   153,     0,   231,     0,
       0,     0,     0,     0,     0,   153,   232,     0,   154,   153,
     154,     0,   233,   234,     0,   154,     0,     0,   154,   235,
       0,     0,     0,     0,   154,     0,     0,   154,   236,     0,
       0,     0,     0,     0,     0,     0,     0,   237,   238,     0,
     239,     0,   240,     0,   241,     0,     0,   242,     0,     0,
       0,   243,   154,   153,   244,     0,     0,   245,     0,     0,
       0,     0,     0,   154,     0,   154,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   755,   222,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   223,   224,     0,   225,     0,     0,     0,
       0,   226,     0,     0,   153,   153,   153,   153,     0,   227,
       0,   398,     0,   786,     0,   228,     0,     0,     0,     0,
     229,     0,     0,   230,     0,     0,   231,     0,     0,   153,
     153,   791,     0,     0,   232,     0,     0,   796,     0,     0,
     233,   234,     0,     0,     0,     0,     0,   235,     0,     0,
       0,   154,     0,     0,     0,     0,   236,     0,   154,     0,
       0,     0,     0,     0,     0,   237,   238,     0,   239,   154,
     240,     0,   241,     0,     0,   242,     0,     0,     0,   243,
       0,     0,   244,   154,     0,   245,     0,     0,     0,     0,
       0,     0,   154,     0,     0,     0,   154,     0,     0,   458,
       0,   458,     0,     0,     0,     0,   458,     0,     0,   458,
       0,     0,     0,     0,     0,   895,     0,     0,   897,     0,
     810,     0,   811,     0,     0,     0,     0,     0,     0,     0,
       0,   812,     0,     0,     0,     0,   813,   224,   814,   815,
     154,   249,     0,   913,   816,     0,     0,     0,     0,     0,
       0,     0,   227,     0,   919,     0,   923,     0,   817,     0,
       0,     0,     0,   818,     0,     0,   230,     0,     0,   819,
       0,   820,     0,     0,     0,     0,     0,   821,     0,     0,
       0,     0,     0,   822,   823,     0,     0,     0,     0,     0,
     235,   154,   154,   154,   154,     0,     0,     0,     0,   824,
       0,     0,     0,     0,     0,     0,     0,     0,   237,   238,
       0,   825,     0,   240,     0,   826,   154,   154,   827,     0,
       0,     0,   828,     0,     0,   244,     0,     0,   829,     0,
       0,     0,     0,     0,     0,     0,     0,   370,   371,   372,
     373,   374,   375,     0,     0,   378,   379,   380,   381,     0,
     383,   384,   830,   831,   832,   833,   834,     0,     0,   835,
     970,     0,     0,   836,   837,   838,   839,   840,   841,   842,
     843,   844,   845,   846,   989,   847,     0,     0,   848,   849,
     850,   851,     0,   994,   852,    -2,     4,   996,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
      38,  1024,    39,    40,     0,    41,  -239,     0,    42,    43,
      44,     0,    45,    46,    47,   -44,    48,    49,     0,    50,
      51,    52,     0,     0,     0,    53,    54,    55,    56,    57,
      58,    59,  -239,   -44,    60,    61,    62,     0,    63,    64,
      65,     0,    66,    67,    68,    69,    70,    71,    72,    73,
       0,    74,    75,     0,    76,    77,    78,    79,    80,     0,
      81,    82,   -62,    83,    84,     0,     0,    85,     0,    86,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,    98,     0,     0,    99,     0,   100,     0,
     101,  1093,   102,     5,   273,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   177,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   178,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,   274,     0,     0,
       0,    99,     0,   179,     0,   101,     0,   180,  1094,     4,
       0,     5,     0,     6,     7,     8,     9,    10,    11,     0,
    -574,     0,    12,    13,    14,    15,    16,  -574,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,  -574,
      27,    28,  -574,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,     0,    39,    40,     0,    41,  -239,
       0,    42,    43,    44,     0,    45,    46,    47,   -44,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,     0,    58,    59,  -239,   -44,    60,    61,    62,
    -574,    63,    64,    65,  -574,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,     0,    81,    82,   -62,    83,    84,     0,     0,
      85,     0,    86,     0,     0,  -574,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -574,  -574,    90,  -574,  -574,  -574,  -574,  -574,  -574,
    -574,     0,  -574,  -574,  -574,  -574,  -574,     0,  -574,  -574,
    -574,  -574,  -574,  -574,  -574,  -574,    98,  -574,  -574,  -574,
       0,   100,  -574,   101,   310,   102,     5,   273,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,    38,     0,
      39,    40,     0,    41,  -239,     0,    42,    43,    44,     0,
      45,    46,    47,   -44,    48,    49,     0,    50,    51,    52,
       0,     0,     0,    53,    54,    55,    56,    57,    58,    59,
    -239,   -44,    60,    61,    62,     0,    63,    64,    65,     0,
      66,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,     0,    81,    82,
     -62,    83,    84,     0,     0,    85,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
     274,    98,     0,     0,    99,     0,   100,   311,   101,     4,
     102,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,     0,    39,    40,     0,    41,  -239,
       0,    42,    43,    44,     0,    45,    46,    47,   -44,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,    57,    58,    59,  -239,   -44,    60,    61,    62,
       0,    63,    64,    65,     0,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,     0,    81,    82,   -62,    83,    84,     0,     0,
      85,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,    98,     0,     0,    99,
       0,   100,   486,   101,   505,   102,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,    38,     0,
      39,    40,     0,    41,  -239,     0,    42,    43,    44,     0,
      45,    46,    47,   -44,    48,    49,     0,    50,    51,    52,
       0,     0,     0,    53,    54,    55,    56,    57,    58,    59,
    -239,   -44,    60,    61,    62,     0,    63,    64,    65,     0,
      66,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,     0,    81,    82,
     -62,    83,    84,     0,     0,    85,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,     0,   100,   506,   101,   310,
     102,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,     0,    39,    40,     0,    41,  -239,
       0,    42,    43,    44,     0,    45,    46,    47,   -44,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,    57,    58,    59,  -239,   -44,    60,    61,    62,
       0,    63,    64,    65,     0,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,     0,    81,    82,   -62,    83,    84,     0,     0,
      85,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,    98,     0,     0,    99,
       0,   100,   311,   101,     4,   102,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,    38,     0,
      39,    40,     0,    41,  -239,     0,    42,    43,    44,     0,
      45,    46,    47,   -44,    48,    49,     0,    50,    51,    52,
       0,     0,     0,    53,    54,    55,    56,    57,    58,    59,
    -239,   -44,    60,    61,    62,     0,    63,    64,    65,     0,
      66,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,     0,    81,    82,
     -62,    83,    84,     0,     0,    85,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,     0,   100,   680,   101,     4,
     102,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,     0,    39,    40,     0,    41,  -239,
       0,    42,    43,    44,     0,    45,    46,    47,   -44,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,   325,    58,    59,  -239,   -44,    60,    61,    62,
       0,    63,    64,    65,     0,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,     0,    81,    82,   -62,    83,    84,     0,     0,
      85,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,    98,     0,     0,    99,
       0,   100,     4,   101,     5,   102,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,    38,     0,    39,    40,
       0,    41,  -239,     0,    42,    43,    44,     0,    45,    46,
      47,   -44,    48,    49,     0,    50,    51,    52,     0,     0,
       0,    53,    54,    55,    56,     0,    58,    59,  -239,   -44,
      60,    61,    62,     0,    63,    64,    65,     0,    66,    67,
      68,    69,    70,    71,    72,    73,     0,    74,    75,     0,
      76,    77,    78,    79,    80,     0,    81,    82,   -62,    83,
      84,     0,     0,    85,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,    98,
       0,     0,    99,     0,   100,     4,   101,     5,   102,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,   597,    37,    38,
       0,    39,    40,     0,    41,  -239,     0,    42,    43,    44,
       0,    45,    46,    47,   -44,    48,    49,     0,    50,    51,
      52,     0,     0,     0,    53,    54,    55,    56,     0,    58,
      59,  -239,   -44,    60,    61,    62,     0,    63,    64,    65,
       0,    66,    67,    68,    69,    70,    71,    72,    73,     0,
      74,    75,     0,    76,    77,    78,    79,    80,     0,    81,
      82,   -62,    83,    84,     0,     0,    85,     0,    86,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,    98,     0,     0,    99,     0,   100,     4,   101,
       5,   102,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
     603,    37,    38,     0,    39,    40,     0,    41,  -239,     0,
      42,    43,    44,     0,    45,    46,    47,   -44,    48,    49,
       0,    50,    51,    52,     0,     0,     0,    53,    54,    55,
      56,     0,    58,    59,  -239,   -44,    60,    61,    62,     0,
      63,    64,    65,     0,    66,    67,    68,    69,    70,    71,
      72,    73,     0,    74,    75,     0,    76,    77,    78,    79,
      80,     0,    81,    82,   -62,    83,    84,     0,     0,    85,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,    98,     0,     0,    99,     0,
     100,     4,   101,     5,   102,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,   881,    37,    38,     0,    39,    40,     0,
      41,  -239,     0,    42,    43,    44,     0,    45,    46,    47,
     -44,    48,    49,     0,    50,    51,    52,     0,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -239,   -44,    60,
      61,    62,     0,    63,    64,    65,     0,    66,    67,    68,
      69,    70,    71,    72,    73,     0,    74,    75,     0,    76,
      77,    78,    79,    80,     0,    81,    82,   -62,    83,    84,
       0,     0,    85,     0,    86,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,    98,     0,
       0,    99,     0,   100,     4,   101,     5,   102,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,   883,    37,    38,     0,
      39,    40,     0,    41,  -239,     0,    42,    43,    44,     0,
      45,    46,    47,   -44,    48,    49,     0,    50,    51,    52,
       0,     0,     0,    53,    54,    55,    56,     0,    58,    59,
    -239,   -44,    60,    61,    62,     0,    63,    64,    65,     0,
      66,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,     0,    81,    82,
     -62,    83,    84,     0,     0,    85,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,     0,   100,     4,   101,     5,
     102,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,   888,
      37,    38,     0,    39,    40,     0,    41,  -239,     0,    42,
      43,    44,     0,    45,    46,    47,   -44,    48,    49,     0,
      50,    51,    52,     0,     0,     0,    53,    54,    55,    56,
       0,    58,    59,  -239,   -44,    60,    61,    62,     0,    63,
      64,    65,     0,    66,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
       0,    81,    82,   -62,    83,    84,     0,     0,    85,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,    98,     0,     0,    99,     0,   100,
       4,   101,     5,   102,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,   891,    37,    38,     0,    39,    40,     0,    41,
    -239,     0,    42,    43,    44,     0,    45,    46,    47,   -44,
      48,    49,     0,    50,    51,    52,     0,     0,     0,    53,
      54,    55,    56,     0,    58,    59,  -239,   -44,    60,    61,
      62,     0,    63,    64,    65,     0,    66,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,     0,    81,    82,   -62,    83,    84,     0,
       0,    85,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,    98,     0,     0,
      99,     0,   100,     4,   101,     5,   102,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,   912,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,    38,     0,    39,
      40,     0,    41,  -239,     0,    42,    43,    44,     0,    45,
      46,    47,   -44,    48,    49,     0,    50,    51,    52,     0,
       0,     0,    53,    54,    55,    56,     0,    58,    59,  -239,
     -44,    60,    61,    62,     0,    63,    64,    65,     0,    66,
      67,    68,    69,    70,    71,    72,    73,     0,    74,    75,
       0,    76,    77,    78,    79,    80,     0,    81,    82,   -62,
      83,    84,     0,     0,    85,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
      98,     0,     0,    99,     0,   100,     4,   101,     5,   102,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,   918,    37,
      38,     0,    39,    40,     0,    41,  -239,     0,    42,    43,
      44,     0,    45,    46,    47,   -44,    48,    49,     0,    50,
      51,    52,     0,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -239,   -44,    60,    61,    62,     0,    63,    64,
      65,     0,    66,    67,    68,    69,    70,    71,    72,    73,
       0,    74,    75,     0,    76,    77,    78,    79,    80,     0,
      81,    82,   -62,    83,    84,     0,     0,    85,     0,    86,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,    98,     0,     0,    99,     0,   100,     4,
     101,     5,   102,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,   922,    37,    38,     0,    39,    40,     0,    41,  -239,
       0,    42,    43,    44,     0,    45,    46,    47,   -44,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,     0,    58,    59,  -239,   -44,    60,    61,    62,
       0,    63,    64,    65,     0,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,     0,    81,    82,   -62,    83,    84,     0,     0,
      85,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,    98,     0,     0,    99,
       0,   100,     0,   101,   774,   102,     5,   273,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   171,     0,     0,
      15,    16,     0,    17,     0,   172,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   173,     0,
       0,     0,    32,   174,   175,     0,   176,    37,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   177,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   178,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
     274,     0,     0,     0,    99,     0,   179,     0,   101,     0,
     180,     5,   273,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   176,    37,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   177,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   178,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,   274,     0,     0,     0,    99,
       0,   179,     0,   101,     0,   180,   672,     5,   273,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   176,    37,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   177,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   178,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,   274,     0,     0,     0,    99,     0,   179,     0,   101,
       0,   180,  1027,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   177,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   178,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   179,     0,   101,     0,   180,   677,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,   216,    23,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,    55,    56,
       0,    58,     0,     0,     0,     0,    61,   177,     0,    63,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     178,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,    83,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   179,
       0,   101,     0,   180,     5,   273,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
       0,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   176,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   177,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   178,     0,    74,     0,     0,
      76,   278,   279,     0,    80,   315,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,   316,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,   274,     0,
       0,     0,    99,     0,   179,     0,   101,     0,   180,     5,
     273,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   177,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     178,     0,    74,     0,     0,    76,   278,   279,     0,    80,
     315,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,   274,     0,     0,     0,    99,     0,   179,
       0,   101,   664,   180,     5,   273,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
       0,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   176,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   177,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   178,     0,    74,     0,     0,
      76,   278,   279,     0,    80,   315,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,   274,     0,
       0,     0,    99,     0,   179,     0,   101,   666,   180,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,   562,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,   563,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   177,     0,   564,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     178,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,   565,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   179,
       0,   101,     5,   180,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   171,     0,     0,    15,    16,     0,    17,
       0,   172,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   173,     0,     0,     0,    32,   174,
     175,     0,   176,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     177,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   178,     0,    74,     0,     0,    76,   278,
     279,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,   280,     0,     0,
      99,     0,   179,     0,   101,     0,   180,     5,   273,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   176,    37,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   177,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   178,     0,
      74,     0,     0,    76,   278,   279,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,   274,     0,     0,     0,    99,     0,   179,     0,   101,
       5,   180,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   171,     0,     0,    15,    16,     0,    17,     0,   172,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   173,     0,     0,     0,    32,   174,   175,     0,
     176,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
     776,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   177,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   178,     0,    74,     0,     0,    76,   278,   279,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     179,     0,   101,     5,   180,     6,     7,     8,   205,    10,
      11,   206,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   207,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   178,     0,    74,     0,     0,    76,
       0,     0,   208,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   179,     0,   101,     5,   180,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   171,     0,     0,
      15,    16,     0,    17,     0,   172,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   173,     0,
       0,     0,    32,   174,   175,     0,   176,    37,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,   210,    56,     0,    58,     0,
       0,     0,     0,    61,   177,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   178,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,   211,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   179,     0,   101,     0,
     180,     5,   273,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   176,    37,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   177,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   178,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,   211,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,   274,     0,     0,     0,    99,
       0,   179,     0,   101,     5,   180,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
       0,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   176,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   177,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   178,     0,    74,     0,     0,
      76,   278,   279,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   179,     0,   101,     0,   180,     5,
     273,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   177,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     178,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,   274,     0,     0,     0,    99,     0,   179,
     662,   101,     5,   180,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   171,     0,     0,    15,    16,     0,    17,
       0,   172,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   173,     0,     0,     0,    32,   174,
     175,     0,   176,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     177,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   178,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,   211,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   179,     0,   101,     5,   180,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   171,     0,     0,    15,
      16,     0,    17,     0,   172,     0,     0,    21,     0,     0,
       0,     0,     0,     0,   253,    28,     0,   173,     0,     0,
       0,    32,   174,   175,     0,   176,    37,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   177,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   178,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   100,     0,   101,     0,   180,
       5,   273,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   171,     0,     0,    15,    16,     0,    17,     0,   172,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   173,     0,     0,     0,    32,   174,   175,     0,
     176,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   177,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   178,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,   274,     0,     0,     0,    99,     0,
     179,     0,   101,     5,   180,     6,     7,     8,   205,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   207,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   178,     0,    74,     0,     0,    76,
       0,     0,   208,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   179,     0,   101,     0,   180,     5,   273,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   171,
       0,     0,    15,    16,     0,    17,     0,   172,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,   636,     0,
     173,     0,     0,     0,    32,   174,   175,     0,   176,    37,
       0,     0,   637,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   177,     0,     0,     0,
       0,     0,     0,     0,    68,   638,    70,    71,    72,   639,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,   274,     0,     0,     0,    99,     0,   179,     0,
     101,     0,   973,     5,   273,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   177,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   178,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,   274,     0,     0,
       0,    99,     0,   179,     0,   101,     0,   973,     5,   273,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   171,
       0,     0,    15,    16,     0,    17,     0,   172,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     173,     0,     0,     0,    32,   174,   175,     0,   957,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   177,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   178,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,   274,     0,     0,     0,    99,     0,   179,     0,
     101,     5,   180,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   176,    37,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   177,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   178,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   179,     0,   101,     5,   180,     6,     7,     8,   201,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
       0,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   176,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   202,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   178,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   179,     0,   101,     5,   180,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   176,    37,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   177,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   178,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   100,     0,   101,
       5,   180,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   171,     0,     0,    15,    16,     0,    17,     0,   172,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   173,     0,     0,     0,    32,   174,   175,     0,
     176,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   177,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   178,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,   608,     0,
     179,     0,   101,     5,   180,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   636,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,   637,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   177,     0,     0,     0,     0,     0,     0,     0,    68,
     638,    70,    71,    72,   639,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   179,     0,   101,     5,   640,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   171,     0,     0,
      15,    16,     0,    17,     0,   172,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   173,     0,
       0,     0,    32,   174,   175,     0,   176,    37,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   177,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   178,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   179,     0,   101,     5,
     640,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   714,
      37,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   177,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     178,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   179,
       0,   101,     5,   180,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   171,     0,     0,    15,    16,     0,    17,
       0,   172,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   173,     0,     0,     0,    32,   174,
     175,     0,   717,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     177,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   178,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   179,     0,   101,     5,   180,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   171,     0,     0,    15,
      16,     0,    17,     0,   172,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   173,     0,     0,
       0,    32,   174,   175,     0,   951,    37,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   177,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   178,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   179,     0,   101,     5,   180,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   171,
       0,     0,    15,    16,     0,    17,     0,   172,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     173,     0,     0,     0,    32,   174,   175,     0,   952,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   177,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   178,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   179,     0,
     101,     5,   180,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   954,    37,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   177,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   178,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   179,     0,   101,     5,   180,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
       0,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   955,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   177,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   178,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   179,     0,   101,     5,   180,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   956,    37,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   177,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   178,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   179,     0,   101,
       5,   180,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   171,     0,     0,    15,    16,     0,    17,     0,   172,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   173,     0,     0,     0,    32,   174,   175,     0,
     957,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   177,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   178,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     179,     0,   101,     5,   180,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   636,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,   637,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   177,     0,     0,     0,     0,     0,     0,     0,    68,
     638,    70,    71,    72,   639,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   179,     0,   101,     5,  1049,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   171,     0,     0,
      15,    16,     0,    17,     0,   172,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   173,     0,
       0,     0,    32,   174,   175,     0,   176,    37,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   177,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   178,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,     0,     0,   101,     5,
    1049,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   177,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     178,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,  -508,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,   385,    96,    97,  -546,     0,  -546,    99,     0,   179,
       0,   101,     0,   180,     5,   273,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
     343,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   176,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,   344,    54,     0,    56,   345,    58,     0,     0,     0,
       0,    61,   177,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   178,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,     0,  -404,
       0,     0,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,     0,     0,   356,     0,    90,     0,     0,     0,
    -404,   362,   363,     0,  -404,   366,     0,     0,   367,   368,
     369,     0,     0,     0,     0,     0,     0,     0,   274,     0,
       0,     0,     0,     0,   179,     0,   101,  -404,   973,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,  -386,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   177,     0,     0,
       0,  -386,     0,     0,     0,    68,    69,    70,    71,    72,
     178,     0,    74,     0,  -386,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,  -386,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,    90,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   171,     0,     0,    15,    16,     0,    17,     0,   172,
       0,     0,    21,    97,  -386,     0,     0,     0,     0,  -386,
      28,   101,   173,  1049,     0,     0,    32,   174,   175,     0,
     176,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   177,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   178,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,     0,     0,     0,     0,     0,     0,     0,
      88,     5,    90,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,   385,    21,    97,  -546,     0,  -546,     0,     0,
     179,    28,   101,   173,   180,     0,     0,    32,   174,   175,
       0,   176,    37,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   177,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   178,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,    90,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   171,     0,     0,    15,    16,     0,    17,
       0,   172,     0,     0,    21,    97,     0,     0,     0,     0,
       0,   179,    28,   101,   173,   640,     0,     0,    32,   174,
     175,     0,   176,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     177,     0,   222,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   178,     0,    74,   223,   224,    76,   225,
       0,     0,    80,     0,   226,    82,     0,     0,    84,     0,
       0,     0,   227,     0,     0,     0,     0,     0,   228,     0,
       0,     0,     0,   229,     0,     0,   230,     0,     0,   231,
       0,     0,     0,     0,    90,     0,     0,   232,     0,     0,
    1006,     0,     0,   233,   234,     0,     0,     0,     0,   812,
     235,     0,     0,     0,   223,   224,   814,   225,     0,   236,
       0,     0,   226,     0,   101,     0,   180,     0,   237,   238,
     227,   239,     0,   240,     0,   241,   817,     0,   242,     0,
       0,   229,   243,     0,   230,   244,     0,   231,   245,   820,
       0,     0,     0,     0,     0,   232,     0,     0,     0,     0,
       0,   822,   234,     0,     0,   341,     0,     0,   235,     0,
       0,     0,   342,     0,     0,     0,     0,   236,     0,     0,
       0,     0,     0,     0,   343,     0,   237,   238,     0,   239,
       0,   240,     0,  1007,     0,     0,   827,     0,     0,     0,
     243,     0,     0,   244,   764,     0,   245,     0,     0,     0,
       0,     0,     0,     0,     0,   370,   371,   372,   373,   374,
     375,     0,     0,   378,   379,   380,   381,     0,   383,   384,
     830,   831,   832,   833,   834,   344,     0,   835,     0,   345,
       0,   836,   837,   838,   839,   840,   841,   842,   843,   844,
     845,   846,     0,   847,   341,     5,   848,   849,   850,   851,
       0,   342,     0,     0,     0,     0,     0,     0,     0,    15,
     346,     0,    17,   343,     0,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,    37,   355,   356,   357,
     358,   359,    41,   360,   361,   362,   363,   364,   365,   366,
      46,   341,   367,   368,   369,     0,     0,    51,   342,     0,
       0,  1041,     0,     0,   344,     0,     0,     0,   345,     0,
     343,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    71,     0,     0,     0,    74,     0,
       0,   341,     0,     0,     0,    80,     0,     0,   342,   346,
       0,    84,     0,     0,     0,     0,     0,     0,     0,     0,
     343,     0,     0,     0,     0,   347,   348,     0,   349,   350,
     351,   344,   352,   353,   354,   345,   355,   356,   357,   358,
     359,     0,   360,   361,   362,   363,   364,   365,   366,     0,
       0,   367,   368,   369,   341,     0,     0,     0,     0,     0,
    1116,   342,     0,     0,     0,     0,   346,     0,     0,     0,
       0,   344,     0,   343,     0,   345,     0,     0,     0,     0,
       0,     0,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,     0,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,   346,     0,   367,   368,
     369,     0,     0,     0,     0,   663,     0,     0,     0,     0,
       0,     0,   347,   348,   344,   349,   350,   351,   345,   352,
     353,   354,     0,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,   341,     0,   367,   368,
     369,     0,     0,   342,     0,   778,     0,     0,     0,   346,
       0,     0,     0,     0,     0,   343,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,   348,     0,   349,   350,
     351,     0,   352,   353,   354,     0,   355,   356,   357,   358,
     359,     0,   360,   361,   362,   363,   364,   365,   366,     0,
       0,   367,   368,   369,     0,     0,     0,     0,   779,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
     345,     0,   341,     0,     0,     0,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,   348,     0,
     349,   350,   351,     0,   352,   353,   354,     0,   355,   356,
     357,   358,   359,     0,   360,   361,   362,   363,   364,   365,
     366,     0,   344,   367,   368,   369,   345,     0,     0,     0,
    1005,     0,   341,     0,     0,   439,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
       0,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,   344,   398,     0,     0,   345,     0,     0,     0,
       0,     0,   341,     0,     0,   880,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
       0,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,   344,   398,     0,     0,   345,     0,     0,     0,
       0,     0,   341,     0,     0,   887,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
       0,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,   344,   398,     0,     0,   345,     0,     0,     0,
       0,     0,   341,     0,     0,  1020,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
       0,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,   344,   398,     0,     0,   345,     0,     0,     0,
       0,     0,   341,     0,     0,  1021,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
       0,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,   344,   398,     0,     0,   345,     0,     0,     0,
       0,     0,   341,     0,     0,  1022,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
       0,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,   344,   398,     0,     0,   345,     0,     0,     0,
       0,     0,   341,     0,     0,  1023,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
       0,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,   344,   398,     0,     0,   345,     0,     0,     0,
       0,     0,   341,     0,     0,  1045,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
       0,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,   344,   398,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,  1046,   341,     0,     0,     0,
       0,     0,     0,   342,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   343,   432,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
     433,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,     0,   398,     0,     0,   344,     0,     0,     0,
     345,     0,   341,     0,     0,     0,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,   426,     0,     0,     0,     0,   429,     0,     0,
       0,   346,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,   348,     0,
     349,   350,   351,     0,   352,   353,   354,     0,   355,   356,
     357,   358,   359,     0,   360,   361,   362,   363,   364,   365,
     366,     0,   344,   367,   368,   369,   345,   398,   341,     0,
       0,     0,     0,     0,     0,   342,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   343,   747,     0,
       0,     0,     0,     0,     0,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,   344,   367,
     368,   369,   345,   398,   341,     0,     0,     0,     0,     0,
       0,   342,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   343,   253,     0,     0,     0,     0,   429,
       0,     0,     0,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
     348,     0,   349,   350,   351,     0,   352,   353,   354,     0,
     355,   356,   357,   358,   359,     0,   360,   361,   362,   363,
     364,   365,   366,     0,   344,   367,   368,   369,   345,   398,
     341,     0,     0,     0,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
     739,     0,     0,     0,     0,     0,     0,     0,     0,   346,
       0,     0,   341,     0,     0,     0,     0,     0,     0,   342,
       0,     0,     0,     0,     0,   347,   348,     0,   349,   350,
     351,   343,   352,   353,   354,     0,   355,   356,   357,   358,
     359,     0,   360,   361,   362,   363,   364,   365,   366,     0,
     344,   367,   368,   369,   345,   398,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   341,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   344,     0,   343,   346,   345,     0,     0,     0,
       0,     0,     0,     0,     0,   756,     0,     0,     0,     0,
       0,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,   346,   360,   361,
     362,   363,   364,   365,   366,     0,     0,   367,   368,   369,
       0,   398,     0,   347,   348,   344,   349,   350,   351,   345,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,     0,   398,   341,     0,     0,     0,     0,     0,
     346,   342,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   343,     0,     0,   347,   348,   436,   349,
     350,   351,     0,   352,   353,   354,     0,   355,   356,   357,
     358,   359,     0,   360,   361,   362,   363,   364,   365,   366,
       0,     0,   367,   368,   369,     0,   471,   341,   684,   437,
       0,     0,     0,     0,   342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   344,     0,   343,     0,   345,     0,
       0,   685,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   346,
       0,     0,   686,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,   348,   344,   349,   350,
     351,   345,   352,   353,   354,     0,   355,   356,   357,   358,
     359,     0,   360,   361,   362,   363,   364,   365,   366,     0,
     438,   367,   368,   369,   341,     0,     0,     0,     0,     0,
       0,   342,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   343,     0,     0,     0,     0,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,   400,   355,
     356,   357,   358,   359,     0,   360,   361,   362,   363,   364,
     365,   366,     0,   341,   367,   368,   369,     0,     0,     0,
     342,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,   571,   344,     0,     0,     0,   345,     0,
       0,     0,     0,     0,     0,     0,     0,   572,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   166,     0,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   344,     0,   347,   348,   345,   349,   350,
     351,     0,   352,   353,   354,     0,   355,   356,   357,   358,
     359,     0,   360,   361,   362,   363,   364,   365,   366,     0,
     341,   367,   368,   369,     0,     0,     0,   342,   346,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
     573,     0,     0,     0,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,   574,   355,   356,   357,   358,   359,
       0,   360,   361,   362,   363,   364,   365,   366,     0,   341,
     367,   368,   369,     0,     0,     0,   342,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   343,     0,
     344,     0,     0,   928,   345,     0,     0,     0,     0,     0,
       0,   341,     0,     0,     0,     0,     0,     0,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     343,     0,     0,     0,   929,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   344,
       0,   347,   348,   345,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,     0,     0,   367,   368,   369,
       0,   344,     0,     0,   346,   345,     0,     0,     0,     0,
       0,   341,     0,     0,     0,     0,     0,     0,   342,     0,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
     343,   355,   356,   357,   358,   359,   346,   360,   361,   362,
     363,   364,   365,   366,     0,     0,   367,   368,   369,     0,
       0,     0,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,     0,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,     0,   475,   367,   368,
     369,   344,     0,     0,     0,   345,     0,     0,     0,     0,
       0,   341,     0,     0,     0,     0,     0,     0,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     343,     0,     0,     0,     0,     0,   346,     0,     0,     0,
       0,     0,     0,   341,     0,     0,     0,     0,     0,     0,
     342,     0,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,   343,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,     0,   477,   367,   368,
     369,   344,     0,     0,     0,   345,     0,     0,     0,     0,
       0,     0,   341,     0,     0,     0,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,   344,     0,     0,   346,   345,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,     0,   355,   356,   357,   358,   359,   346,   360,
     361,   362,   363,   364,   365,   366,     0,   479,   367,   368,
     369,     0,   344,     0,   347,   348,   345,   349,   350,   351,
       0,   352,   353,   354,     0,   355,   356,   357,   358,   359,
       0,   360,   361,   362,   363,   364,   365,   366,   341,   484,
     367,   368,   369,     0,     0,   342,   501,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   343,     0,     0,
       0,     0,     0,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,   341,     0,   367,
     368,   369,     0,     0,   342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,     0,   344,     0,
       0,     0,   345,     0,     0,     0,     0,     0,     0,     0,
       0,   575,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   341,     0,     0,     0,     0,     0,     0,   342,
       0,     0,     0,   346,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   344,     0,   347,
     348,   345,   349,   350,   351,     0,   352,   353,   354,     0,
     355,   356,   357,   358,   359,     0,   360,   361,   362,   363,
     364,   365,   366,     0,     0,   367,   368,   369,     0,     0,
       0,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   344,     0,     0,     0,   345,     0,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,     0,   355,
     356,   357,   358,   359,  -509,   360,   361,   362,   363,   364,
     365,   366,   341,   166,   367,   368,   369,   346,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,   341,   751,   367,
     368,   369,     0,     0,   342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,     0,     0,     0,
       0,     0,   344,     0,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   341,     0,     0,
       0,     0,     0,     0,   342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,   346,     0,   754,
       0,     0,     0,     0,     0,     0,     0,   344,     0,     0,
       0,   345,     0,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,   724,   367,
     368,   369,   346,     0,     0,     0,     0,   344,     0,     0,
       0,   345,     0,     0,     0,     0,     0,     0,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,     0,   355,
     356,   357,   358,   359,     0,   360,   361,   362,   363,   364,
     365,   366,   346,     0,   367,   368,   369,     0,     0,   341,
       0,     0,     0,     0,     0,     0,   342,     0,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,   343,   355,
     356,   357,   358,   359,     0,   360,   361,   362,   363,   364,
     365,   366,     0,     0,   367,   368,   369,     0,     0,   341,
       0,     0,     0,     0,     0,     0,   342,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   343,   867,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   344,
       0,     0,     0,   345,     0,     0,     0,     0,     0,   341,
       0,     0,     0,     0,     0,     0,   342,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   343,   870,
       0,     0,     0,   797,   346,     0,     0,     0,     0,   344,
       0,     0,     0,   345,     0,     0,     0,     0,     0,     0,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
       0,   355,   356,   357,   358,   359,     0,   360,   361,   362,
     363,   364,   365,   366,   346,     0,   367,   368,   369,   344,
       0,     0,     0,   345,     0,     0,     0,     0,     0,     0,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
       0,   355,   356,   357,   358,   359,     0,   360,   361,   362,
     363,   364,   365,   366,   346,     0,   367,   368,   369,     0,
       0,   341,   927,     0,     0,     0,     0,     0,   342,     0,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
     343,   355,   356,   357,   358,   359,     0,   360,   361,   362,
     363,   364,   365,   366,     0,     0,   367,   368,   369,     0,
       0,   341,     0,     0,     0,     0,     0,     0,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     343,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,     0,     0,     0,   345,     0,     0,     0,     0,
       0,   341,     0,     0,     0,     0,     0,     0,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     343,     0,     0,     0,     0,     0,   346,     0,     0,     0,
       0,   344,     0,     0,     0,   345,     0,     0,     0,     0,
       0,     0,   347,   348,   950,   349,   350,   351,     0,   352,
     353,   354,     0,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,   346,     0,   367,   368,
     369,   344,     0,     0,     0,   345,     0,     0,     0,     0,
       0,     0,   347,   348,   953,   349,   350,   351,     0,   352,
     353,   354,     0,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,   346,     0,   367,   368,
     369,     0,     0,   341,     0,     0,     0,     0,     0,     0,
     342,     0,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,   343,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,     0,     0,   367,   368,
     369,     0,     0,   341,     0,     0,     0,     0,     0,     0,
     342,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   344,     0,     0,     0,   345,     0,     0,
       0,     0,     0,   341,     0,     0,  1058,     0,     0,     0,
     342,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,     0,     0,     0,     0,     0,   346,     0,
       0,     0,     0,   344,     0,     0,     0,   345,     0,     0,
       0,     0,     0,     0,   347,   348,  1059,   349,   350,   351,
       0,   352,   353,   354,     0,   355,   356,   357,   358,   359,
       0,   360,   361,   362,   363,   364,   365,   366,   346,     0,
     367,   368,   369,   344,     0,     0,     0,   345,     0,     0,
       0,     0,     0,     0,   347,   348,  1060,   349,   350,   351,
       0,   352,   353,   354,     0,   355,   356,   357,   358,   359,
       0,   360,   361,   362,   363,   364,   365,   366,   346,     0,
     367,   368,   369,     0,     0,   341,     0,     0,     0,     0,
       0,     0,   342,     0,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,   343,   355,   356,   357,   358,   359,
       0,   360,   361,   362,   363,   364,   365,   366,     0,     0,
     367,   368,   369,     0,     0,   341,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,     0,     0,     0,   345,
       0,     0,     0,     0,     0,   341,     0,     0,  1061,     0,
       0,     0,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   343,     0,     0,     0,     0,     0,
     346,     0,     0,     0,     0,   344,     0,     0,     0,   345,
       0,     0,     0,     0,     0,     0,   347,   348,  1062,   349,
     350,   351,     0,   352,   353,   354,     0,   355,   356,   357,
     358,   359,     0,   360,   361,   362,   363,   364,   365,   366,
     346,     0,   367,   368,   369,   344,     0,     0,     0,   345,
       0,     0,     0,     0,     0,     0,   347,   348,  1063,   349,
     350,   351,     0,   352,   353,   354,     0,   355,   356,   357,
     358,   359,     0,   360,   361,   362,   363,   364,   365,   366,
     346,     0,   367,   368,   369,     0,     0,   341,     0,     0,
       0,     0,     0,     0,   342,     0,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,   343,   355,   356,   357,
     358,   359,     0,   360,   361,   362,   363,   364,   365,   366,
       0,     0,   367,   368,   369,     0,     0,   341,     0,     0,
       0,  1087,     0,     0,   342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,     0,     0,
       0,   345,     0,     0,     0,     0,     0,   341,     0,     0,
       0,     0,     0,     0,   342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,     0,     0,     0,
       0,     0,   346,     0,     0,     0,     0,   344,     0,     0,
       0,   345,     0,     0,     0,     0,     0,     0,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,     0,   355,
     356,   357,   358,   359,     0,   360,   361,   362,   363,   364,
     365,   366,   346,     0,   367,   368,   369,   712,     0,     0,
       0,   345,     0,     0,     0,     0,     0,     0,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,     0,   355,
     356,   357,   358,   359,     0,   360,   361,   362,   363,   364,
     365,   366,   346,     0,   367,   368,   369,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,     0,   355,
     356,   357,   358,   359,     0,   360,   361,   362,   363,   364,
     365,   366,     0,   268,   367,   368,   369,    22,    23,     0,
       0,     0,     0,     0,     0,     0,   269,   268,    30,   270,
       0,    22,    23,    35,     0,     0,     0,     0,     0,    40,
     269,     0,    30,   270,     0,     0,     0,    35,     0,     0,
       0,   343,     0,    40,     0,     0,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,   933,     0,     0,
     934,     0,   271,     0,    63,    53,     0,    55,     0,    57,
       0,   933,     0,     0,   934,     0,   271,     0,    63,     0,
       0,   343,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,   344,     0,     0,     0,   345,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   343,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   346,     0,     0,
       0,     0,   344,     0,     0,     0,   345,     0,     0,    98,
       0,     0,     0,   347,   348,  1064,   349,   350,   351,     0,
     352,   353,   354,    98,   355,   356,   357,   358,     0,  1065,
     360,   361,   362,   363,   364,   365,   366,   346,   344,   367,
     368,   369,   345,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,     0,     0,
     360,   361,   362,   363,   364,   343,   366,     0,     0,   367,
     368,   369,     0,     0,     0,     0,     0,     0,     0,   347,
     348,     0,   349,   350,   351,   344,   352,   353,   354,   345,
     355,   356,   357,   358,     0,     0,   360,   361,   362,   363,
     364,     0,   366,     0,     0,   367,   368,   369,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
     345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,     0,     0,   356,   357,
     358,     0,     0,   360,   361,   362,   363,     0,     0,   366,
       0,     0,   367,   368,   369,     0,     0,   347,   348,     0,
     349,   350,   351,     0,   352,   353,     0,     0,     0,   356,
     689,     0,   222,     0,     0,     0,   362,   363,     0,     0,
     366,     0,     0,   367,   368,   369,   223,   224,     0,   225,
       0,     0,     0,     0,   226,     0,     0,     0,     0,     0,
       0,     0,   227,     0,     0,     0,     0,     0,   228,  -220,
       0,     0,     0,   229,     0,     0,   230,     0,     0,   231,
       0,     0,     0,  -220,  -220,     0,  -220,   232,     0,     0,
       0,  -220,     0,   233,   234,     0,     0,     0,     0,  -220,
     235,     0,     0,     0,     0,  -220,     0,     0,     0,   236,
    -220,     0,     0,  -220,     0,     0,  -220,     0,   237,   238,
       0,   239,     0,   240,  -220,   241,     0,  -230,   242,     0,
    -220,  -220,   243,     0,     0,   244,     0,  -220,   245,     0,
       0,  -230,  -230,     0,  -230,     0,  -220,     0,     0,  -230,
       0,     0,     0,     0,     0,  -220,  -220,  -230,  -220,     0,
    -220,     0,  -220,  -230,   222,  -220,     0,     0,  -230,  -220,
       0,  -230,  -220,     0,  -230,  -220,     0,     0,   223,   224,
       0,   225,  -230,     0,     0,     0,   226,     0,  -230,  -230,
       0,     0,     0,     0,   227,  -230,     0,     0,     0,     0,
     228,     0,     0,     0,  -230,   229,     0,     0,   230,     0,
       0,   231,     0,  -230,  -230,     0,  -230,     0,  -230,   232,
    -230,     0,  -221,  -230,     0,   233,   234,  -230,     0,     0,
    -230,     0,   235,  -230,     0,     0,  -221,  -221,     0,  -221,
       0,   236,     0,     0,  -221,     0,     0,     0,     0,     0,
     237,   238,  -221,   239,     0,   240,     0,   241,  -221,  -152,
     242,     0,     0,  -221,   243,     0,  -221,   244,     0,  -221,
     245,     0,     0,  -152,  -152,     0,  -152,  -221,     0,     0,
       0,  -152,     0,  -221,  -221,     0,     0,     0,     0,  -152,
    -221,     0,     0,     0,     0,  -152,     0,     0,     0,  -221,
    -152,     0,     0,  -152,     0,     0,  -152,     0,  -221,  -221,
       0,  -221,     0,  -221,  -152,  -221,     0,  -148,  -221,     0,
    -152,  -152,  -221,     0,     0,  -221,     0,  -152,  -221,     0,
       0,  -148,  -148,     0,  -148,     0,  -152,     0,     0,  -148,
       0,     0,     0,     0,     0,  -152,  -152,  -148,  -152,     0,
    -152,     0,  -152,  -148,     0,  -152,     0,     0,  -148,  -152,
       0,  -148,  -152,     0,  -148,  -152,     0,     0,     0,     0,
       0,   268,  -148,     0,     0,    22,    23,     0,  -148,  -148,
       0,     0,     0,     0,   269,  -148,    30,   270,     0,     0,
       0,    35,     0,     0,  -148,     0,     0,    40,     0,     0,
    -239,     0,     0,  -148,  -148,     0,  -148,     0,  -148,     0,
    -148,     0,     0,  -148,     0,     0,     0,  -148,     0,    53,
    -148,    55,     0,  -148,     0,     0,  -239,     0,     0,     0,
     271,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    79,     0,     0,    81,     0,   -63,    83
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-981)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      12,   151,    65,    60,    16,   330,   391,   480,    20,     2,
      19,    64,   446,   192,    26,   137,   215,   601,    30,    31,
     773,    33,    34,    35,    36,    42,   449,   698,   686,   208,
     221,   685,    22,    45,     1,    47,   803,   902,   101,     1,
      52,   635,    54,   983,    56,    13,   454,    32,     1,   102,
       1,    46,    64,    65,    66,    67,    68,    69,    32,    46,
     899,    73,    79,    75,     3,    77,    78,    32,    46,     1,
      82,   193,   480,    85,    86,   266,    88,    32,    90,     0,
      85,    93,    94,   118,    96,   857,   858,    68,   100,   101,
     102,   589,    32,    46,   592,    51,    60,  1077,   289,   113,
     113,   139,    46,   139,   139,    56,    68,    38,   143,   862,
       3,   133,   437,   130,    78,   132,   158,   711,   113,   141,
      46,    77,   321,   322,    56,   709,   164,   180,   113,   110,
    1027,   166,   168,   972,    85,   147,  1116,  1002,   876,   113,
     878,    46,   113,   107,   139,     3,   296,    98,   110,    56,
     903,   118,   139,    85,   168,   168,  1096,    96,   163,   171,
     111,   139,   174,   175,   176,   118,   178,   179,   180,   936,
      85,   138,   139,   168,   139,   192,   770,     3,   163,   111,
    1077,   168,   191,   164,    46,   138,   139,   138,   200,   163,
     158,   208,   959,   210,   961,   139,   387,   388,   163,   166,
     111,    31,   164,    96,   111,   158,   138,   158,   163,   221,
     267,   634,   163,   139,   204,    83,   601,    18,   971,   209,
     958,    83,   960,   163,   168,    46,   216,    85,   139,   165,
     728,   163,   249,   731,   139,    27,  1008,  1009,    96,   118,
      27,   139,   168,   158,   158,   139,    77,    78,   163,  1016,
     164,  1018,    82,   139,   266,   165,    86,   274,   270,   139,
     139,    48,    83,   168,   143,   139,   278,   279,   166,   200,
      71,   929,   166,  1026,   928,   946,  1094,   289,   164,   139,
      72,   133,   473,    70,   118,    72,   166,   166,    89,   141,
     221,    83,   166,   139,  1112,   417,    83,     8,    33,    34,
     422,   885,   165,   133,   316,   106,   166,   139,   138,   143,
     102,   141,   158,   376,   377,   145,   108,   133,   330,   382,
     313,   108,   385,   386,   709,   141,   158,   339,   762,   341,
     342,   343,   344,   345,   346,   266,   348,   349,   350,   351,
     352,   764,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   289,   139,
      27,   686,   139,   139,   376,   377,   139,   102,   139,   803,
     382,   424,   139,   385,   386,   387,   388,   389,   390,   391,
      93,   139,   139,   165,   139,   158,   166,   158,   400,   166,
     166,   163,   139,   405,    78,   407,    31,   107,   415,   166,
     158,   158,   419,   158,   204,    72,   605,   606,   158,   209,
      56,   158,   424,    56,   426,   427,    83,   158,   157,   149,
     432,   433,   353,   164,   436,   437,   158,   439,   491,   441,
     493,    45,   449,    56,   158,   158,   909,   101,   133,    85,
     158,   108,    85,   138,   456,   165,   141,    82,   460,   461,
     145,    86,    98,    67,   466,    98,   387,   388,   511,   158,
     158,   473,    85,    77,    78,   111,   158,   165,   111,   167,
       2,   670,   671,    82,   163,    98,   488,    86,    27,   491,
      12,   493,   661,   158,   158,   497,   498,   499,   111,   501,
     502,   508,   112,    25,   902,    27,   513,    81,   133,   511,
     885,   909,    27,   138,    82,   140,   141,   941,    86,    81,
     145,   442,   158,   165,   507,   158,   165,   163,   153,   158,
     163,    46,   585,    72,    49,   959,   161,   961,   165,   165,
     167,   167,   165,   165,    83,   158,   200,   165,   165,   167,
     163,    73,   473,   165,    69,   167,   158,    72,   698,    74,
     165,   118,   167,   102,   491,     8,   493,   221,    83,   571,
     572,   573,   574,   575,   576,   577,    60,   138,   100,   578,
     579,   588,   104,   585,   591,   587,   190,   102,    27,   141,
     163,   644,  1016,   595,  1018,   597,   166,   640,   158,   601,
     118,   603,   139,   158,  1002,    46,   165,    34,   212,   213,
     214,   215,   266,   166,   929,   219,   139,   166,   118,   344,
     345,   166,   166,   630,   139,    21,   139,   634,   168,  1027,
     168,   163,   165,    72,   158,   289,   638,   639,   640,   158,
     166,    67,   644,  1041,    83,   167,    31,   158,    27,   158,
     657,   166,   256,   158,   661,   158,   168,   139,   262,   158,
     158,    34,   158,   102,   389,   390,   587,    46,   139,   166,
      49,   164,   166,   164,   118,   400,    83,   166,    32,  1077,
     682,   285,   684,   685,   686,    32,   163,   168,   163,    34,
      69,   698,   138,    72,    83,    74,  1094,    82,   168,   303,
     165,    86,   427,   165,    83,   138,   158,   709,   433,   168,
     712,   713,   714,   168,  1112,   717,   158,   164,  1116,   141,
     165,   141,   141,   102,   246,   133,   141,   141,    46,   910,
     911,   253,   141,   387,   388,   737,   133,   739,    56,   782,
       2,   141,   744,   141,    34,   747,   141,   133,   133,   751,
      12,   141,   754,   138,   761,   140,   141,   764,   141,   133,
     145,   133,   163,    25,   141,    27,   158,    85,   153,   163,
      98,   158,   497,   166,   159,   160,   161,   133,   166,   786,
     782,   118,   166,   133,   166,   327,   212,   213,   214,   215,
     792,   313,   794,   111,   157,   797,   157,   401,   168,   806,
     118,   139,  1032,   805,    24,   327,   946,   144,   786,   146,
     147,    73,   466,   150,   151,    77,    78,   297,   806,   473,
     946,   139,   941,   762,   627,   832,  1096,   431,   809,  1026,
     256,   435,  1054,  1087,   488,  1080,   262,   491,   100,   493,
     158,     3,   104,    35,   498,   163,   457,   572,   166,   574,
     852,  1081,  1082,  1083,  1084,  1085,  1086,   587,    20,   285,
      30,    -1,    -1,    -1,   866,   867,    -1,   869,   870,   871,
     872,    33,    -1,    -1,    -1,    -1,   398,   303,   880,   881,
      -1,   883,    -1,   885,    -1,   887,   888,   934,    -1,   891,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   426,   167,   908,   909,   910,   911,
     432,    -1,    -1,    -1,   916,    -1,   918,   439,    -1,    -1,
     922,    -1,    -1,    -1,    -1,   927,   928,   929,    -1,    -1,
     973,   585,    -1,   587,    96,    -1,    -1,    -1,    -1,   946,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   950,   951,
     952,   953,   954,   955,   956,   957,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      -1,   973,    -1,   495,    -1,   401,   978,   499,    -1,    56,
     982,    -1,    -1,    -1,   246,   507,    -1,   712,   713,   910,
     911,   253,    -1,    -1,    -1,   599,   600,    -1,    -1,    -1,
       2,   605,   606,    -1,    -1,   431,  1049,    27,    85,   435,
      12,    -1,    -1,   617,    -1,    -1,    -1,    -1,  1020,  1021,
    1022,  1023,    -1,    25,    -1,    27,    46,    -1,    -1,    49,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,  1041,
      46,   118,    -1,  1045,  1046,    -1,   568,  1049,    -1,    69,
      56,   313,    72,   657,    74,    -1,  1058,  1059,  1060,  1061,
    1062,  1063,   139,    83,    -1,   327,    -1,    -1,    -1,    -1,
      -1,    73,    -1,    -1,  1076,  1077,    -1,  1079,    -1,    85,
      -1,   158,   102,    -1,    -1,    -1,   163,    -1,    -1,   166,
       2,    -1,    -1,    -1,   616,    -1,    -1,    -1,   100,    -1,
      12,    -1,   104,    -1,    -1,   111,    -1,     3,    -1,    -1,
    1112,    -1,   118,    25,  1116,    27,    -1,    -1,   722,   723,
      -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,
     734,    -1,   736,   139,    -1,    -1,   398,    33,    -1,   743,
      -1,    -1,   746,    39,    -1,   749,    -1,   669,    44,    -1,
      -1,    47,   158,   675,    50,    -1,    -1,   163,    -1,    -1,
     166,    73,    58,    -1,   426,   167,    -1,    -1,    64,    65,
     432,    -1,    -1,   599,   600,    71,   698,   439,    -1,   605,
     606,    -1,    -1,    -1,    80,   789,    -1,    -1,   100,    -1,
     712,   713,   104,    89,    90,    -1,    92,    -1,    94,    -1,
      96,    -1,    -1,    99,     2,    -1,    -1,   103,   104,    -1,
     106,    -1,    -1,   109,    12,   737,    -1,   739,    -1,    -1,
      -1,    -1,   744,    -1,    -1,   747,    -1,    25,    -1,    27,
      -1,   753,    -1,   495,   756,    -1,    -1,   499,    -1,    -1,
      -1,    -1,    -1,    -1,   246,   507,    -1,    -1,   902,    -1,
      -1,   253,    -1,    -1,    -1,   167,   910,   911,    -1,   781,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
     792,    -1,   794,    -1,    -1,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   722,   723,    -1,    -1,
      -1,    -1,   100,    -1,    -1,    -1,   104,    -1,   734,    -1,
     914,   313,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     746,    -1,    22,   749,   978,   327,    26,    27,    -1,    -1,
      -1,    -1,    -1,    -1,   246,    35,    -1,    37,    38,    -1,
      -1,   253,    42,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,   616,    -1,    -1,    -1,   880,   963,
     964,    -1,    -1,   967,   968,   887,    -1,    -1,    -1,   167,
      70,    -1,    72,  1027,    -1,    -1,   898,    -1,    -1,   983,
      -1,    81,    -1,    83,    -1,    -1,    -1,   991,    -1,   993,
     912,   995,    -1,    -1,    -1,    -1,   398,    -1,    -1,   921,
      -1,   313,   102,   925,    -1,   105,    -1,   669,   108,    -1,
      -1,    -1,    -1,   675,    -1,   327,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1077,   426,    -1,    -1,    -1,    -1,    -1,
     432,    -1,    -1,    12,    -1,    -1,    -1,   439,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,   969,   246,    -1,
      -1,     2,    31,    -1,    -1,   253,    -1,    -1,    -1,    -1,
      -1,    12,  1116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,   737,    27,   739,    -1,    -1,
      -1,    -1,   744,    -1,    -1,   747,   398,    -1,   914,    -1,
      -1,   753,  1096,   495,   756,    -1,    -1,   499,  1020,  1021,
    1022,  1023,    -1,    82,    -1,   507,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,   426,   313,    -1,    -1,    -1,   781,
     432,    -1,    73,  1045,  1046,    -1,    -1,   439,    -1,   327,
     792,    -1,   794,    -1,   113,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
      -1,    -1,    -1,   104,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,   495,    -1,    -1,    -1,   499,    -1,   168,
      -1,    -1,    -1,    -1,    -1,   507,    -1,    -1,    -1,    -1,
     398,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   616,    -1,   167,    -1,   880,    -1,
      -1,    -1,    -1,    -1,    -1,   887,    -1,    -1,   426,    -1,
      -1,    -1,    -1,    -1,   432,    -1,   898,    -1,    -1,    -1,
      -1,   439,    -1,    -1,    -1,    -1,     2,    -1,    -1,     1,
     912,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,   921,
      -1,    -1,    -1,   925,    -1,    -1,    -1,   669,    -1,    25,
      22,    27,    -1,   675,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    -1,   246,    48,   495,    -1,    51,
      -1,   499,   253,    -1,   616,    -1,    -1,   969,    -1,   507,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    70,    -1,
      72,    -1,    74,    -1,    76,    77,    -1,    79,    -1,    81,
      -1,    83,    -1,    -1,    -1,   737,    -1,   739,    -1,    -1,
      -1,    -1,   744,    -1,   100,   747,    -1,    -1,   104,    -1,
     102,   753,    -1,   105,   756,    -1,   108,   669,  1020,  1021,
    1022,  1023,   313,   675,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,    -1,   327,    -1,    35,   781,
      37,    38,    -1,  1045,  1046,    42,    -1,    -1,    -1,    -1,
     792,    48,   794,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,
      -1,   167,   164,    70,    -1,    72,    -1,    74,   616,    76,
      -1,    -1,    79,    -1,    81,   737,    83,   739,    -1,    -1,
      -1,    -1,   744,     2,    -1,   747,    -1,    -1,    -1,    -1,
      -1,   753,    -1,    12,   756,   102,    -1,   398,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   781,
      -1,   669,    -1,    -1,    -1,   426,    -1,   675,   880,    -1,
     792,   432,   794,    -1,    -1,   887,    -1,    -1,   439,    -1,
     246,    -1,    -1,    -1,    -1,    -1,   898,   253,    -1,    -1,
      -1,   158,    -1,    -1,    73,    -1,    -1,   164,    -1,    -1,
     912,    -1,    -1,    -1,    82,    -1,    -1,    -1,    86,   921,
      -1,    -1,    -1,   925,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,    -1,    -1,    -1,   104,    -1,    -1,    -1,   737,
      -1,   739,    -1,    -1,   495,    -1,   744,    -1,   499,   747,
      -1,    -1,    -1,    -1,    -1,   753,   507,   313,   756,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   969,   880,   137,
     138,   327,   140,   141,    -1,   887,    -1,   145,    -1,    -1,
      -1,    -1,    -1,   781,    -1,   153,   898,    -1,    -1,    -1,
      -1,   159,   160,   161,   792,    -1,   794,    -1,   167,    -1,
     912,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   921,
      22,    -1,    -1,   925,    26,    27,    -1,    -1,  1020,  1021,
    1022,  1023,    -1,    35,     2,    37,    38,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    12,    -1,    48,    -1,    -1,    -1,
      -1,    -1,   398,  1045,  1046,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   969,    70,    -1,
      72,    -1,    74,    -1,    76,   616,    -1,    79,    -1,    81,
     426,    83,    -1,    -1,    -1,    -1,   432,   246,    -1,    -1,
      -1,    -1,   880,   439,   253,    -1,    -1,    -1,    -1,   887,
     102,    -1,    -1,   105,    -1,    73,   108,    -1,    -1,    -1,
     898,    -1,    -1,    -1,    31,    -1,    -1,    -1,  1020,  1021,
    1022,  1023,    -1,    -1,   912,    22,    -1,    -1,   669,    26,
      27,    -1,   100,   921,   675,    -1,   104,   925,    35,    -1,
      37,    38,    -1,  1045,  1046,    42,    -1,    -1,    -1,   495,
      -1,    48,    -1,   499,   313,    -1,   158,    -1,    -1,    -1,
      -1,   507,   164,    -1,    -1,    82,    -1,    -1,   327,    86,
      -1,    12,    -1,    70,    -1,    72,    -1,    74,    19,    76,
      -1,   969,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,   737,    -1,   739,   167,
      -1,    -1,    -1,   744,    -1,   102,   747,    -1,   105,    -1,
      -1,   108,   753,    -1,    -1,   756,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,    -1,    -1,    -1,   145,    -1,
      -1,    -1,  1020,  1021,  1022,  1023,   153,    -1,    -1,   398,
     781,    82,   159,   160,   161,    86,    -1,    -1,    -1,    -1,
      -1,   792,    -1,   794,    -1,    -1,    -1,  1045,  1046,    -1,
      -1,   158,    -1,    -1,    -1,    -1,    -1,   426,    -1,    -1,
     616,    -1,    -1,   432,    -1,    -1,   117,    -1,   246,    -1,
     439,    -1,    -1,    -1,    -1,   253,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    -1,   159,   160,
     161,    -1,    -1,   669,    -1,    -1,    -1,   168,    -1,   675,
      -1,    -1,    -1,    -1,    -1,    -1,   495,    -1,    -1,   880,
     499,    -1,    -1,    -1,    -1,   313,   887,    -1,   507,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   898,    -1,   327,
      -1,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,   912,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,
     921,    42,    -1,    -1,   925,    31,    -1,    48,    -1,    -1,
      -1,   737,    -1,   739,    -1,    -1,    -1,    -1,   744,    -1,
      -1,   747,    -1,    -1,    -1,    -1,    -1,   753,    -1,    70,
     756,    72,    -1,    74,    -1,    76,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    -1,   969,    -1,
     398,    -1,    -1,    -1,    -1,   781,    82,    -1,    -1,    -1,
      86,   102,    -1,    -1,   105,    -1,   792,   108,   794,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   616,   426,    -1,
      -1,    -1,    -1,    -1,   432,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1020,
    1021,  1022,  1023,    -1,    -1,    -1,    -1,   133,   134,    -1,
      -1,    -1,   138,    -1,   140,   141,    -1,   158,    -1,   145,
      -1,    -1,    -1,   164,  1045,  1046,    -1,   153,    -1,    -1,
     669,    -1,    -1,   159,   160,   161,   675,    -1,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,   495,    -1,    -1,
      -1,   499,    -1,    -1,   880,    17,    18,    -1,    20,   507,
      -1,   887,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,   898,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,   912,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,   921,    58,    -1,   737,   925,
     739,    -1,    64,    65,    -1,   744,    -1,    -1,   747,    71,
      -1,    -1,    -1,    -1,   753,    -1,    -1,   756,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,
      92,    -1,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,
      -1,   103,   781,   969,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,   792,    -1,   794,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   616,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,  1020,  1021,  1022,  1023,    -1,    33,
      -1,   163,    -1,   165,    -1,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,  1045,
    1046,   669,    -1,    -1,    58,    -1,    -1,   675,    -1,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,   880,    -1,    -1,    -1,    -1,    80,    -1,   887,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    92,   898,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,   912,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,   921,    -1,    -1,    -1,   925,    -1,    -1,   737,
      -1,   739,    -1,    -1,    -1,    -1,   744,    -1,    -1,   747,
      -1,    -1,    -1,    -1,    -1,   753,    -1,    -1,   756,    -1,
       1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    17,    18,    19,    20,
     969,   165,    -1,   781,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,   792,    -1,   794,    -1,    39,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,  1020,  1021,  1022,  1023,    -1,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      -1,    92,    -1,    94,    -1,    96,  1045,  1046,    99,    -1,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,   120,
     121,   122,   123,    -1,    -1,   126,   127,   128,   129,    -1,
     131,   132,   133,   134,   135,   136,   137,    -1,    -1,   140,
     898,    -1,    -1,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   912,   156,    -1,    -1,   159,   160,
     161,   162,    -1,   921,   165,     0,     1,   925,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,   969,    47,    48,    -1,    50,    51,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    62,    -1,    64,
      65,    66,    -1,    -1,    -1,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    -1,    83,    84,
      85,    -1,    87,    88,    89,    90,    91,    92,    93,    94,
      -1,    96,    97,    -1,    99,   100,   101,   102,   103,    -1,
     105,   106,   107,   108,   109,    -1,    -1,   112,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
     155,   156,   157,   158,    -1,    -1,   161,    -1,   163,    -1,
     165,     1,   167,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
      -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,   161,    -1,   163,    -1,   165,    -1,   167,   168,     1,
      -1,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      12,    -1,    14,    15,    16,    17,    18,    19,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    -1,    50,    51,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      62,    -1,    64,    65,    66,    -1,    -1,    -1,    70,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    96,    97,    -1,    99,   100,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,    -1,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,   163,   164,   165,     1,   167,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    -1,    50,    51,    -1,    53,    54,    55,    -1,
      57,    58,    59,    60,    61,    62,    -1,    64,    65,    66,
      -1,    -1,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      87,    88,    89,    90,    91,    92,    93,    94,    -1,    96,
      97,    -1,    99,   100,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,
     157,   158,    -1,    -1,   161,    -1,   163,   164,   165,     1,
     167,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    -1,    50,    51,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      62,    -1,    64,    65,    66,    -1,    -1,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      -1,    83,    84,    85,    -1,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    96,    97,    -1,    99,   100,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,    -1,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,   158,    -1,    -1,   161,
      -1,   163,   164,   165,     1,   167,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    -1,    50,    51,    -1,    53,    54,    55,    -1,
      57,    58,    59,    60,    61,    62,    -1,    64,    65,    66,
      -1,    -1,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      87,    88,    89,    90,    91,    92,    93,    94,    -1,    96,
      97,    -1,    99,   100,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,
     157,   158,    -1,    -1,   161,    -1,   163,   164,   165,     1,
     167,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    -1,    50,    51,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      62,    -1,    64,    65,    66,    -1,    -1,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      -1,    83,    84,    85,    -1,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    96,    97,    -1,    99,   100,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,    -1,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,   158,    -1,    -1,   161,
      -1,   163,   164,   165,     1,   167,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    -1,    50,    51,    -1,    53,    54,    55,    -1,
      57,    58,    59,    60,    61,    62,    -1,    64,    65,    66,
      -1,    -1,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      87,    88,    89,    90,    91,    92,    93,    94,    -1,    96,
      97,    -1,    99,   100,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,
     157,   158,    -1,    -1,   161,    -1,   163,   164,   165,     1,
     167,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    -1,    50,    51,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      62,    -1,    64,    65,    66,    -1,    -1,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      -1,    83,    84,    85,    -1,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    96,    97,    -1,    99,   100,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,    -1,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,   158,    -1,    -1,   161,
      -1,   163,     1,   165,     3,   167,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      -1,    50,    51,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    62,    -1,    64,    65,    66,    -1,    -1,
      -1,    70,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    -1,    83,    84,    85,    -1,    87,    88,
      89,    90,    91,    92,    93,    94,    -1,    96,    97,    -1,
      99,   100,   101,   102,   103,    -1,   105,   106,   107,   108,
     109,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,   158,
      -1,    -1,   161,    -1,   163,     1,   165,     3,   167,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    -1,    50,    51,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    62,    -1,    64,    65,
      66,    -1,    -1,    -1,    70,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    -1,    83,    84,    85,
      -1,    87,    88,    89,    90,    91,    92,    93,    94,    -1,
      96,    97,    -1,    99,   100,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,    -1,    -1,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,   158,    -1,    -1,   161,    -1,   163,     1,   165,
       3,   167,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    -1,    50,    51,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    62,
      -1,    64,    65,    66,    -1,    -1,    -1,    70,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    -1,
      83,    84,    85,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    -1,    96,    97,    -1,    99,   100,   101,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,    -1,   112,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,   158,    -1,    -1,   161,    -1,
     163,     1,   165,     3,   167,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    -1,
      50,    51,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    62,    -1,    64,    65,    66,    -1,    -1,    -1,
      70,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    87,    88,    89,
      90,    91,    92,    93,    94,    -1,    96,    97,    -1,    99,
     100,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
      -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,   158,    -1,
      -1,   161,    -1,   163,     1,   165,     3,   167,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    -1,    50,    51,    -1,    53,    54,    55,    -1,
      57,    58,    59,    60,    61,    62,    -1,    64,    65,    66,
      -1,    -1,    -1,    70,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      87,    88,    89,    90,    91,    92,    93,    94,    -1,    96,
      97,    -1,    99,   100,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,
     157,   158,    -1,    -1,   161,    -1,   163,     1,   165,     3,
     167,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    -1,    50,    51,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    62,    -1,
      64,    65,    66,    -1,    -1,    -1,    70,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      84,    85,    -1,    87,    88,    89,    90,    91,    92,    93,
      94,    -1,    96,    97,    -1,    99,   100,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,    -1,    -1,   112,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,   156,   157,   158,    -1,    -1,   161,    -1,   163,
       1,   165,     3,   167,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    -1,    50,
      51,    -1,    53,    54,    55,    -1,    57,    58,    59,    60,
      61,    62,    -1,    64,    65,    66,    -1,    -1,    -1,    70,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    -1,    83,    84,    85,    -1,    87,    88,    89,    90,
      91,    92,    93,    94,    -1,    96,    97,    -1,    99,   100,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
      -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,   158,    -1,    -1,
     161,    -1,   163,     1,   165,     3,   167,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    -1,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    -1,    64,    65,    66,    -1,
      -1,    -1,    70,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    -1,    83,    84,    85,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    -1,    96,    97,
      -1,    99,   100,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,
     158,    -1,    -1,   161,    -1,   163,     1,   165,     3,   167,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    -1,    50,    51,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    62,    -1,    64,
      65,    66,    -1,    -1,    -1,    70,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    -1,    83,    84,
      85,    -1,    87,    88,    89,    90,    91,    92,    93,    94,
      -1,    96,    97,    -1,    99,   100,   101,   102,   103,    -1,
     105,   106,   107,   108,   109,    -1,    -1,   112,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
     155,   156,   157,   158,    -1,    -1,   161,    -1,   163,     1,
     165,     3,   167,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    -1,    50,    51,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      62,    -1,    64,    65,    66,    -1,    -1,    -1,    70,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      -1,    83,    84,    85,    -1,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    96,    97,    -1,    99,   100,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,    -1,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,   158,    -1,    -1,   161,
      -1,   163,    -1,   165,     1,   167,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,
      -1,    58,    59,    -1,    61,    62,    -1,    64,    65,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,
      -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    92,    93,    94,    -1,    96,
      -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,
     157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,
     167,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,
      62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,
      -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,
      -1,   163,    -1,   165,    -1,   167,   168,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,    65,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,
      -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,
      -1,   167,   168,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
      -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,   161,    -1,   163,    -1,   165,    -1,   167,   168,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,
      -1,   165,    -1,   167,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,   100,   101,    -1,   103,   104,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    96,    -1,    -1,    99,   100,   101,    -1,   103,
     104,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,
      -1,   165,   166,   167,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,   100,   101,    -1,   103,   104,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,   166,   167,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,
      -1,   165,     3,   167,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
      61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    96,    -1,    -1,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,   158,    -1,    -1,
     161,    -1,   163,    -1,   165,    -1,   167,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,    65,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,
      -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      96,    -1,    -1,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,
       3,   167,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,
     163,    -1,   165,     3,   167,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
      -1,    -1,   102,   103,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,   161,    -1,   163,    -1,   165,     3,   167,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,
      -1,    58,    59,    -1,    61,    62,    -1,    64,    65,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    -1,    75,    -1,
      -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    92,    93,    94,    -1,    96,
      -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,   115,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,
     157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,
     167,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,
      62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,
      -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,   115,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,
      -1,   163,    -1,   165,     3,   167,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,
     164,   165,     3,   167,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
      61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,
     161,    -1,   163,    -1,   165,     3,   167,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,
      58,    59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,
      -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    91,    92,    93,    94,    -1,    96,    -1,
      -1,    99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,
      -1,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,
     163,    -1,   165,     3,   167,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
      -1,    -1,   102,   103,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,   161,    -1,   163,    -1,   165,    -1,   167,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,
      65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,
      75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
     155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,
     165,    -1,   167,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
      -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,   161,    -1,   163,    -1,   165,    -1,   167,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,
      65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,
      75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
     155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,
     165,     3,   167,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,
      62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,
      -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,
      -1,   163,    -1,   165,     3,   167,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,     3,   167,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,    65,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,
      -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,
       3,   167,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,
     163,    -1,   165,     3,   167,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
      -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,   161,    -1,   163,    -1,   165,     3,   167,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,
      -1,    58,    59,    -1,    61,    62,    -1,    64,    65,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,
      -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    92,    93,    94,    -1,    96,
      -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,
     157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,     3,
     167,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,
      -1,   165,     3,   167,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
      61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,
     161,    -1,   163,    -1,   165,     3,   167,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,
      58,    59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,
      -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    91,    92,    93,    94,    -1,    96,    -1,
      -1,    99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,
      -1,    -1,    -1,   161,    -1,   163,    -1,   165,     3,   167,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,
      65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,
      75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
     155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,
     165,     3,   167,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,
      62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,
      -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,
      -1,   163,    -1,   165,     3,   167,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,     3,   167,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,    65,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,
      -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,
       3,   167,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,
     163,    -1,   165,     3,   167,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
      -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,   161,    -1,   163,    -1,   165,     3,   167,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,
      -1,    58,    59,    -1,    61,    62,    -1,    64,    65,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,
      -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    92,    93,    94,    -1,    96,
      -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,
     157,    -1,    -1,    -1,   161,    -1,    -1,    -1,   165,     3,
     167,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,   109,    -1,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   162,   156,   157,   165,    -1,   167,   161,    -1,   163,
      -1,   165,    -1,   167,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      31,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,
      -1,    82,    71,    -1,    73,    86,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,    -1,   145,    -1,   135,    -1,    -1,    -1,
     139,   152,   153,    -1,   143,   156,    -1,    -1,   159,   160,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,    -1,    -1,    -1,   163,    -1,   165,   166,   167,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    55,    56,    -1,    58,    59,    -1,    61,    62,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    96,    -1,    98,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,   109,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,   135,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,   157,   158,    -1,    -1,    -1,    -1,   163,
      33,   165,    35,   167,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,     3,   135,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,   162,    25,   157,   165,    -1,   167,    -1,    -1,
     163,    33,   165,    35,   167,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,
      62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,
      -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,   135,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,   157,    -1,    -1,    -1,    -1,
      -1,   163,    33,   165,    35,   167,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
      61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,
      81,    -1,     3,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    96,    17,    18,    99,    20,
      -1,    -1,   103,    -1,    25,   106,    -1,    -1,   109,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    -1,   135,    -1,    -1,    58,    -1,    -1,
       3,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    12,
      71,    -1,    -1,    -1,    17,    18,    19,    20,    -1,    80,
      -1,    -1,    25,    -1,   165,    -1,   167,    -1,    89,    90,
      33,    92,    -1,    94,    -1,    96,    39,    -1,    99,    -1,
      -1,    44,   103,    -1,    47,   106,    -1,    50,   109,    52,
      -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,
      -1,    64,    65,    -1,    -1,    12,    -1,    -1,    71,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    89,    90,    -1,    92,
      -1,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,   165,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,   120,   121,   122,
     123,    -1,    -1,   126,   127,   128,   129,    -1,   131,   132,
     133,   134,   135,   136,   137,    82,    -1,   140,    -1,    86,
      -1,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,    -1,   156,    12,     3,   159,   160,   161,   162,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
     117,    -1,    20,    31,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    44,   144,   145,   146,
     147,   148,    50,   150,   151,   152,   153,   154,   155,   156,
      58,    12,   159,   160,   161,    -1,    -1,    65,    19,    -1,
      -1,   168,    -1,    -1,    82,    -1,    -1,    -1,    86,    -1,
      31,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    96,    -1,
      -1,    12,    -1,    -1,    -1,   103,    -1,    -1,    19,   117,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    82,   140,   141,   142,    86,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      -1,   159,   160,   161,    12,    -1,    -1,    -1,    -1,    -1,
     168,    19,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    82,    -1,    31,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   117,    -1,   159,   160,
     161,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    82,   136,   137,   138,    86,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    12,    -1,   159,   160,
     161,    -1,    -1,    19,    -1,   166,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,
      86,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    82,   159,   160,   161,    86,    -1,    -1,    -1,
     166,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      46,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    -1,   163,    -1,    -1,    82,    -1,    -1,    -1,
      86,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    82,   159,   160,   161,    86,   163,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    82,   159,
     160,   161,    86,   163,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,    -1,    82,   159,   160,   161,    86,   163,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    31,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      82,   159,   160,   161,    86,   163,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    31,   117,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,   117,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      -1,   163,    -1,   133,   134,    82,   136,   137,   138,    86,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    -1,   163,    12,    -1,    -1,    -1,    -1,    -1,
     117,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,   133,   134,    36,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,    -1,   159,   160,   161,    -1,   163,    12,    13,    67,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    31,    -1,    86,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,    82,   136,   137,
     138,    86,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
     158,   159,   160,   161,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    46,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    12,   159,   160,   161,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    82,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,   133,   134,    86,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      12,   159,   160,   161,    -1,    -1,    -1,    19,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    46,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    12,
     159,   160,   161,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      82,    -1,    -1,    36,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    67,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,   133,   134,    86,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      -1,    82,    -1,    -1,   117,    86,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      31,   144,   145,   146,   147,   148,   117,   150,   151,   152,
     153,   154,   155,   156,    -1,    -1,   159,   160,   161,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,   158,   159,   160,
     161,    82,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    31,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,   158,   159,   160,
     161,    82,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    82,    -1,    -1,   117,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,   117,   150,
     151,   152,   153,   154,   155,   156,    -1,   158,   159,   160,
     161,    -1,    82,    -1,   133,   134,    86,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    12,   158,
     159,   160,   161,    -1,    -1,    19,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    12,    -1,   159,
     160,   161,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    82,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    82,    -1,   133,
     134,    86,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    -1,    86,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,    12,   113,   159,   160,   161,   117,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    12,    13,   159,
     160,   161,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,   117,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,
      -1,    86,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,   158,   159,
     160,   161,   117,    -1,    -1,    -1,    -1,    82,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   117,    -1,   159,   160,   161,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    31,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,   117,    -1,   159,   160,   161,    82,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,   117,    -1,   159,   160,   161,    -1,
      -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      31,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,    -1,   159,   160,   161,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    95,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   117,    -1,   159,   160,
     161,    82,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    95,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   117,    -1,   159,   160,
     161,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    31,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    -1,   159,   160,
     161,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    95,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   117,    -1,
     159,   160,   161,    82,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    95,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   117,    -1,
     159,   160,   161,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    31,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    95,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    95,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     117,    -1,   159,   160,   161,    82,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    95,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     117,    -1,   159,   160,   161,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    31,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,    -1,   159,   160,   161,    -1,    -1,    12,    -1,    -1,
      -1,    56,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    82,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   117,    -1,   159,   160,   161,    82,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   117,    -1,   159,   160,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    22,   159,   160,   161,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    22,    37,    38,
      -1,    26,    27,    42,    -1,    -1,    -1,    -1,    -1,    48,
      35,    -1,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,
      -1,    31,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,
      79,    -1,    81,    -1,    83,    70,    -1,    72,    -1,    74,
      -1,    76,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,
      -1,    31,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,
      -1,    -1,    82,    -1,    -1,    -1,    86,   102,    -1,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    -1,    86,    -1,    -1,   158,
      -1,    -1,    -1,   133,   134,   164,   136,   137,   138,    -1,
     140,   141,   142,   158,   144,   145,   146,   147,    -1,   164,
     150,   151,   152,   153,   154,   155,   156,   117,    82,   159,
     160,   161,    86,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,    -1,    -1,
     150,   151,   152,   153,   154,    31,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,    82,   140,   141,   142,    86,
     144,   145,   146,   147,    -1,    -1,   150,   151,   152,   153,
     154,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,    -1,   145,   146,
     147,    -1,    -1,   150,   151,   152,   153,    -1,    -1,   156,
      -1,    -1,   159,   160,   161,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,    -1,    -1,    -1,   145,
       1,    -1,     3,    -1,    -1,    -1,   152,   153,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,     3,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    17,    18,    -1,    20,    58,    -1,    -1,
      -1,    25,    -1,    64,    65,    -1,    -1,    -1,    -1,    33,
      71,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    80,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    89,    90,
      -1,    92,    -1,    94,    58,    96,    -1,     3,    99,    -1,
      64,    65,   103,    -1,    -1,   106,    -1,    71,   109,    -1,
      -1,    17,    18,    -1,    20,    -1,    80,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    89,    90,    33,    92,    -1,
      94,    -1,    96,    39,     3,    99,    -1,    -1,    44,   103,
      -1,    47,   106,    -1,    50,   109,    -1,    -1,    17,    18,
      -1,    20,    58,    -1,    -1,    -1,    25,    -1,    64,    65,
      -1,    -1,    -1,    -1,    33,    71,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    80,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    89,    90,    -1,    92,    -1,    94,    58,
      96,    -1,     3,    99,    -1,    64,    65,   103,    -1,    -1,
     106,    -1,    71,   109,    -1,    -1,    17,    18,    -1,    20,
      -1,    80,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      89,    90,    33,    92,    -1,    94,    -1,    96,    39,     3,
      99,    -1,    -1,    44,   103,    -1,    47,   106,    -1,    50,
     109,    -1,    -1,    17,    18,    -1,    20,    58,    -1,    -1,
      -1,    25,    -1,    64,    65,    -1,    -1,    -1,    -1,    33,
      71,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    80,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    89,    90,
      -1,    92,    -1,    94,    58,    96,    -1,     3,    99,    -1,
      64,    65,   103,    -1,    -1,   106,    -1,    71,   109,    -1,
      -1,    17,    18,    -1,    20,    -1,    80,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    89,    90,    33,    92,    -1,
      94,    -1,    96,    39,    -1,    99,    -1,    -1,    44,   103,
      -1,    47,   106,    -1,    50,   109,    -1,    -1,    -1,    -1,
      -1,    22,    58,    -1,    -1,    26,    27,    -1,    64,    65,
      -1,    -1,    -1,    -1,    35,    71,    37,    38,    -1,    -1,
      -1,    42,    -1,    -1,    80,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    89,    90,    -1,    92,    -1,    94,    -1,
      96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    70,
     106,    72,    -1,   109,    -1,    -1,    77,    -1,    -1,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,   107,   108
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   174,   175,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,    35,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    47,
      48,    50,    53,    54,    55,    57,    58,    59,    61,    62,
      64,    65,    66,    70,    71,    72,    73,    74,    75,    76,
      79,    80,    81,    83,    84,    85,    87,    88,    89,    90,
      91,    92,    93,    94,    96,    97,    99,   100,   101,   102,
     103,   105,   106,   108,   109,   112,   114,   117,   133,   134,
     135,   136,   137,   142,   152,   155,   156,   157,   158,   161,
     163,   165,   167,   176,   177,   178,   179,   180,   181,   183,
     188,   189,   190,   191,   192,   196,   198,   201,   202,   204,
     205,   206,   211,   212,   214,   215,   216,   220,   221,   224,
     225,   228,   229,   232,   235,   236,   261,   264,   265,   285,
     286,   287,   288,   289,   290,   291,   299,   300,   301,   302,
     303,   306,   307,   308,   309,   310,   311,   312,   314,   315,
     316,   317,   318,   158,   178,   303,   113,   292,   293,     3,
     193,    14,    22,    35,    40,    41,    43,    81,    94,   163,
     167,   232,   285,   290,   301,   302,   303,   306,   308,   309,
     292,   303,   102,   264,    83,   193,   178,   279,   303,   178,
     165,     8,    81,   303,   304,     8,    11,    81,   102,   304,
      72,   115,   213,   303,   213,   303,    26,   265,   303,   303,
     196,   165,     3,    17,    18,    20,    25,    33,    39,    44,
      47,    50,    58,    64,    65,    71,    80,    89,    90,    92,
      94,    96,    99,   103,   106,   109,   195,   197,   233,   165,
     195,   266,   267,    32,   183,   200,   303,   303,    18,    71,
      89,   106,   303,   303,   303,     8,   165,   203,    22,    35,
      38,    81,   204,     4,   157,   255,   278,   303,   100,   101,
     158,   303,   305,   303,   200,   303,   303,   303,    93,   165,
     178,   303,   303,   183,   192,   303,   306,   183,   192,   303,
     195,   262,   303,   303,   303,   303,   303,   303,   303,   303,
       1,   164,   176,   184,   278,   104,   143,   255,   280,   281,
     305,   213,   278,   303,   313,    74,   178,   163,    78,   182,
     107,   195,   195,    51,    77,   250,   266,   157,   158,   149,
     303,    12,    19,    31,    82,    86,   117,   133,   134,   136,
     137,   138,   140,   141,   142,   144,   145,   146,   147,   148,
     150,   151,   152,   153,   154,   155,   156,   159,   160,   161,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   162,   242,   165,   167,    82,
      86,   165,   178,   158,   303,   303,   303,   278,   163,   183,
      46,   292,   262,   266,   158,   139,   158,   112,   196,   255,
     282,   283,   284,   305,    81,   209,   236,   264,    81,   207,
     262,   236,   264,   195,   165,   200,    32,    46,   200,   113,
     200,   295,    32,    46,   200,   295,    36,    67,   158,    95,
     183,   242,    13,   158,   282,   178,   165,   244,   104,   165,
     195,   268,   269,     1,   138,   273,    46,   139,   178,   200,
     165,   165,   200,   282,   204,   195,   139,   158,   303,   303,
     158,   163,   200,   165,   282,   158,   217,   158,   217,   158,
     118,   263,   158,   200,   158,   164,   164,   176,   139,   164,
     303,   139,   166,   139,   166,   168,   295,    46,   139,   168,
     295,   116,   139,   168,     8,     1,   164,   184,    60,   185,
     303,   138,   226,   163,   237,   158,   303,   303,   303,   303,
     213,   303,   213,   303,   303,   303,   303,   303,   303,   303,
      20,    33,    58,   102,   196,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   305,   305,   305,   305,   305,   282,   282,   213,   303,
     213,   303,    27,    46,    83,   108,   294,   297,   298,   303,
     318,    32,    46,    32,    46,    95,    46,   168,   213,   303,
     200,   158,   158,   303,   303,   118,   166,   139,   210,   195,
     266,   208,   195,   158,   266,    46,   278,    43,   303,   213,
     303,   165,   200,    43,   303,   213,   303,   200,   161,   185,
     187,   303,   185,   186,   178,   303,    34,   303,   196,   166,
      27,    46,    49,    69,    72,    83,   102,   177,   245,   246,
     247,   248,   234,   269,   139,   166,    33,    47,    90,    94,
     167,   199,   274,   286,   118,   270,   303,   267,   303,   303,
     166,   255,   303,     1,   222,   282,   166,    21,   218,   274,
     286,   139,   164,   166,   166,   280,   166,   280,   178,   168,
     213,   303,   168,   178,   303,   168,   303,   168,   303,   164,
     164,   195,   139,   158,    13,    36,    67,   278,   163,     1,
     195,   230,   231,    27,    72,    83,   102,   239,   249,   165,
     158,   158,   158,   158,   158,   166,   168,    46,    83,   139,
     166,   285,    82,    82,    43,   213,   303,    43,   213,   303,
     213,   303,   292,   292,   158,   255,   305,   284,   195,   226,
     158,   195,   226,   158,   303,   166,   303,    32,   200,    32,
     200,   296,   297,   303,    32,   200,   295,    32,   200,   295,
     158,    13,   158,    34,    34,   178,    95,   183,   158,    46,
      83,   247,   139,   166,   165,   195,    27,    72,    83,   102,
     251,   166,   269,   273,     1,   278,    63,   305,   166,   166,
     164,    68,   110,   164,   223,   166,   165,   183,   195,   219,
     262,   178,   168,   295,   168,   295,   178,   116,   303,   303,
     187,   186,     1,   227,   164,   118,   139,   164,    83,   238,
       1,     3,    12,    17,    19,    20,    25,    39,    44,    50,
      52,    58,    64,    65,    80,    92,    96,    99,   103,   109,
     133,   134,   135,   136,   137,   140,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   156,   159,   160,
     161,   162,   165,   194,   195,   197,   240,   241,   242,   285,
     166,   297,   273,   285,   285,   303,    32,    32,   303,    32,
      32,   168,   168,   200,   200,   226,   163,   226,   163,   200,
      95,    43,   303,    43,   303,   139,   166,    95,    43,   303,
     200,    43,   303,   200,   303,   178,   303,   178,    34,   195,
     246,   269,   138,   277,    83,   273,   270,   168,    46,   168,
     165,   165,    32,   178,   278,   219,   138,   183,    43,   178,
     303,   168,    43,   178,   303,   168,   303,    13,    36,    67,
     158,   158,   164,    76,    79,   164,   177,   202,   303,   231,
     251,   165,   243,   195,   303,   133,   141,   243,   243,   270,
      95,    43,    43,    95,    43,    43,    43,    43,   163,   227,
     163,   227,   303,   303,   303,   297,   303,   303,   303,    34,
     178,   277,   166,   167,   199,   255,   276,   286,   143,   256,
     270,    56,   111,   257,   303,   274,   286,   282,   282,   178,
     200,   166,   303,   303,   178,   303,   178,   303,   187,   186,
     202,     1,   138,   272,   245,   166,     3,    96,   241,   242,
     303,   303,   303,   303,   303,   303,   227,   164,   227,   164,
      95,    95,    95,    95,   178,   270,   277,   168,   278,   255,
     303,     3,    85,    96,   258,   259,   260,   303,   183,   201,
     254,   168,   166,   166,   183,    95,    95,   158,   158,   167,
     199,   271,   286,    98,   252,   166,   243,   243,    95,    95,
      95,    95,    95,    95,   164,   164,   303,   303,   303,   303,
     256,   270,   255,   275,   276,   286,    46,   168,   260,   111,
     139,   118,   144,   146,   147,   150,   151,    56,   286,   157,
     157,   303,   303,     1,   168,   278,   257,   303,   275,   276,
     303,   259,   260,   260,   260,   260,   260,   260,   258,   168,
     271,   286,   168,   158,   253,   254,   168,   271,   286,   275
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
#line 497 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 502 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 503 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 515 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 516 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 540 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 542 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 543 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 544 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 545 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 546 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 547 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 548 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 549 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 550 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 551 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 556 "chapel.ypp"
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

  case 41:
/* Line 1792 of yacc.c  */
#line 569 "chapel.ypp"
    { (yyvsp[(1) - (3)].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (3)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (3)]).prevModule;  // restore previous module name
    }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 574 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = (yyvsp[(3) - (4)].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 579 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 588 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 592 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 593 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 607 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 613 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 619 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 624 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 629 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 633 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 649 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 658 "chapel.ypp"
    { (yyval.b) = (currentModuleType != MOD_INTERNAL ? true : false); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 659 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 660 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 664 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (4)].ponlylist), (yyvsp[(1) - (4)].b)); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 665 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), "", (yyvsp[(5) - (6)].ponlylist), true, (yyvsp[(1) - (6)].b)); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 666 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), (yyvsp[(5) - (8)].pexpr), (yyvsp[(7) - (8)].ponlylist), true, (yyvsp[(1) - (8)].b)); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 667 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), "", (yyvsp[(5) - (6)].ponlylist), false, (yyvsp[(1) - (6)].b)); }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 668 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), (yyvsp[(5) - (8)].pexpr), (yyvsp[(7) - (8)].ponlylist), false, (yyvsp[(1) - (8)].b)); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 672 "chapel.ypp"
    { (yyval.pblockstmt) = buildImportStmt(new UnresolvedSymExpr((yyvsp[(2) - (3)].pch))); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 674 "chapel.ypp"
    { (yyval.pblockstmt) = buildImportStmt(new UnresolvedSymExpr((yyvsp[(2) - (5)].pch)), (yyvsp[(4) - (5)].pch)); }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 678 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 683 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 685 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 687 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 698 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 702 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 703 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 704 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 705 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 706 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 707 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 710 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 711 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 712 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 713 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 714 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 715 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 727 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 728 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 739 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 740 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 742 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 743 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 744 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 745 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 748 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 749 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 761 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 762 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 763 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 764 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 765 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 766 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 767 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 768 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 770 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 771 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 778 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 779 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 780 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 781 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 785 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 786 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 790 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 791 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 795 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 797 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 807 "chapel.ypp"
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 808 "chapel.ypp"
    { parsingPrivate=(yyvsp[(2) - (3)].b); applyPrivateToBlock((yyvsp[(3) - (3)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(3) - (3)].pblockstmt); }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 822 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 823 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 824 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 825 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 830 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 835 "chapel.ypp"
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

  case 150:
/* Line 1792 of yacc.c  */
#line 845 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 850 "chapel.ypp"
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

  case 152:
/* Line 1792 of yacc.c  */
#line 861 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 866 "chapel.ypp"
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

  case 154:
/* Line 1792 of yacc.c  */
#line 876 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 881 "chapel.ypp"
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

  case 156:
/* Line 1792 of yacc.c  */
#line 892 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 896 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 901 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 909 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 920 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 926 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 927 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 928 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 929 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 930 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 931 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 932 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), true, false); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 933 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 934 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), true, false); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 935 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 936 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 937 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 938 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 939 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 940 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 941 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 942 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 943 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 945 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 951 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 957 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 963 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 969 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 976 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 983 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 987 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 993 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 997 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 998 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 999 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 1000 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1001 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 1003 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1005 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1007 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1012 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1016 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1017 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1018 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1024 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1025 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1029 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1030 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1031 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1035 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1036 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1040 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1044 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1046 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1050 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1051 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1056 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1058 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1060 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1067 "chapel.ypp"
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

  case 219:
/* Line 1792 of yacc.c  */
#line 1077 "chapel.ypp"
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

  case 220:
/* Line 1792 of yacc.c  */
#line 1090 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1095 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1100 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1108 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1114 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1116 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1118 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1123 "chapel.ypp"
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

  case 229:
/* Line 1792 of yacc.c  */
#line 1139 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1146 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1155 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1163 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1167 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1173 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1174 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1179 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1184 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1189 "chapel.ypp"
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

  case 239:
/* Line 1792 of yacc.c  */
#line 1211 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1214 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1218 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1226 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1236 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1243 "chapel.ypp"
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

  case 245:
/* Line 1792 of yacc.c  */
#line 1269 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1273 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1278 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1282 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1287 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1298 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1307 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1308 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1309 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1312 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1313 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1314 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1317 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1318 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1319 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1320 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1321 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1322 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1323 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1324 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1325 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1329 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1330 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1331 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1332 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1333 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1335 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1336 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1337 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1338 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1340 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1344 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1345 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1349 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1353 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1354 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1364 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1366 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1368 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1377 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1378 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1379 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1380 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1381 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1382 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1383 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1384 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1389 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1390 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1391 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1392 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1393 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1394 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1398 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1399 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1403 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1404 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1405 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1406 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1407 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1408 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1412 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1413 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1416 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1422 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1426 "chapel.ypp"
    { 
      DefExpr * de = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch)));
      USR_WARN(de, "support for space in query expressions is deprecated");
      (yyval.pexpr) = de;
  }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1431 "chapel.ypp"
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1435 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1436 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1437 "chapel.ypp"
    { if (DefExpr* def = toDefExpr((yyvsp[(2) - (2)].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
                         }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1445 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1447 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1449 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1451 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1453 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1458 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1460 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1464 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1465 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1466 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1467 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1468 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1469 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1470 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1486 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1491 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1504 "chapel.ypp"
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

  case 360:
/* Line 1792 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1532 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1537 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1543 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1552 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1560 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1569 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1571 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1576 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1578 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1580 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1586 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1587 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1588 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1594 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1598 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1602 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1606 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1612 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1619 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1620 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1621 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1622 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1623 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1629 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1630 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1631 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1653 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1657 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1661 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1669 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1676 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1677 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1682 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1684 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1690 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1692 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1694 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1698 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1699 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1701 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1708 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1709 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1710 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1711 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1715 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1720 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1721 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1722 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1726 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1727 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1731 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1736 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1737 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1741 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1742 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1743 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1744 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1748 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1749 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1761 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1763 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1765 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1776 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1778 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1780 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1782 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1784 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1786 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1788 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1791 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1793 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1795 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1797 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1799 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1801 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1803 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1805 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1808 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1810 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1812 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1817 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1819 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1821 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1823 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1825 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1827 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1829 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1831 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1833 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1835 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1837 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1839 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1841 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1848 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1854 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1860 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1866 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1875 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1884 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1900 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1904 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1909 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1913 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1914 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1918 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1922 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1923 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1928 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1932 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1936 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1942 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1943 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1944 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1945 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1946 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1947 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1953 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1958 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1960 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1964 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1968 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1972 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1980 "chapel.ypp"
    { (yyvsp[(1) - (2)].pcallexpr)->insertAtTail((yyvsp[(2) - (2)].pexpr));
      (yyval.pexpr) = (yyvsp[(1) - (2)].pcallexpr); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1985 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1991 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1997 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 2004 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 2013 "chapel.ypp"
    { USR_WARN((yyvsp[(4) - (5)].pexpr), "the syntax 'new owned(an expression)' is deprecated");
      USR_PRINT((yyvsp[(4) - (5)].pexpr), "instead use 'owned.create(an expression)'");
      (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtOwned->symbol, (yyvsp[(4) - (5)].pexpr))); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 2017 "chapel.ypp"
    { USR_WARN((yyvsp[(4) - (5)].pexpr), "the syntax 'new shared(an expression)' is deprecated");
      USR_PRINT((yyvsp[(4) - (5)].pexpr), "instead use 'shared.create(an expression)'");
      (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtShared->symbol, (yyvsp[(4) - (5)].pexpr))); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 2024 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 2040 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 2042 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 2044 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 2046 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 2048 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 2050 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 2054 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 2055 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 2059 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 2060 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 2061 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 2079 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 2080 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 2081 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr));}
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 2082 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 2086 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 2087 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 2088 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 2092 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 2093 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 2094 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 2095 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 2096 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 2104 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 2105 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 2106 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2107 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2111 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2112 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2117 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 2118 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 2119 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2120 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2121 "chapel.ypp"
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2122 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2123 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2124 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2125 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2126 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2127 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2129 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2133 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2140 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2141 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2145 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2146 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2147 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2148 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 584:
/* Line 1792 of yacc.c  */
#line 2149 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 585:
/* Line 1792 of yacc.c  */
#line 2150 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 586:
/* Line 1792 of yacc.c  */
#line 2151 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 587:
/* Line 1792 of yacc.c  */
#line 2152 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 588:
/* Line 1792 of yacc.c  */
#line 2153 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 589:
/* Line 1792 of yacc.c  */
#line 2154 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 590:
/* Line 1792 of yacc.c  */
#line 2155 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 591:
/* Line 1792 of yacc.c  */
#line 2156 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 592:
/* Line 1792 of yacc.c  */
#line 2157 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 593:
/* Line 1792 of yacc.c  */
#line 2158 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 594:
/* Line 1792 of yacc.c  */
#line 2159 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 595:
/* Line 1792 of yacc.c  */
#line 2160 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 596:
/* Line 1792 of yacc.c  */
#line 2161 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 597:
/* Line 1792 of yacc.c  */
#line 2162 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 598:
/* Line 1792 of yacc.c  */
#line 2163 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 599:
/* Line 1792 of yacc.c  */
#line 2164 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 600:
/* Line 1792 of yacc.c  */
#line 2165 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 601:
/* Line 1792 of yacc.c  */
#line 2166 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 602:
/* Line 1792 of yacc.c  */
#line 2167 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 603:
/* Line 1792 of yacc.c  */
#line 2171 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 604:
/* Line 1792 of yacc.c  */
#line 2172 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 605:
/* Line 1792 of yacc.c  */
#line 2173 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 606:
/* Line 1792 of yacc.c  */
#line 2174 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 607:
/* Line 1792 of yacc.c  */
#line 2175 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 608:
/* Line 1792 of yacc.c  */
#line 2176 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 609:
/* Line 1792 of yacc.c  */
#line 2177 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 610:
/* Line 1792 of yacc.c  */
#line 2181 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 611:
/* Line 1792 of yacc.c  */
#line 2182 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 612:
/* Line 1792 of yacc.c  */
#line 2183 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 613:
/* Line 1792 of yacc.c  */
#line 2184 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 614:
/* Line 1792 of yacc.c  */
#line 2188 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 615:
/* Line 1792 of yacc.c  */
#line 2189 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 616:
/* Line 1792 of yacc.c  */
#line 2190 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 617:
/* Line 1792 of yacc.c  */
#line 2191 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 618:
/* Line 1792 of yacc.c  */
#line 2196 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 619:
/* Line 1792 of yacc.c  */
#line 2197 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 620:
/* Line 1792 of yacc.c  */
#line 2198 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 621:
/* Line 1792 of yacc.c  */
#line 2199 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 622:
/* Line 1792 of yacc.c  */
#line 2200 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 623:
/* Line 1792 of yacc.c  */
#line 2201 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 624:
/* Line 1792 of yacc.c  */
#line 2202 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 10218 "bison-chapel.cpp"
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


