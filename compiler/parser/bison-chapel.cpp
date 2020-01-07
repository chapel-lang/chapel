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
     TIN = 300,
     TINDEX = 301,
     TINLINE = 302,
     TINOUT = 303,
     TINT = 304,
     TITER = 305,
     TINITEQUALS = 306,
     TLABEL = 307,
     TLAMBDA = 308,
     TLET = 309,
     TLIFETIME = 310,
     TLOCAL = 311,
     TLOCALE = 312,
     TMINUSMINUS = 313,
     TMODULE = 314,
     TNEW = 315,
     TNIL = 316,
     TNOINIT = 317,
     TNONE = 318,
     TNOTHING = 319,
     TON = 320,
     TONLY = 321,
     TOTHERWISE = 322,
     TOUT = 323,
     TOVERRIDE = 324,
     TOWNED = 325,
     TPARAM = 326,
     TPLUSPLUS = 327,
     TPRAGMA = 328,
     TPRIMITIVE = 329,
     TPRIVATE = 330,
     TPROC = 331,
     TPROTOTYPE = 332,
     TPUBLIC = 333,
     TREAL = 334,
     TRECORD = 335,
     TREDUCE = 336,
     TREF = 337,
     TREQUIRE = 338,
     TRETURN = 339,
     TSCAN = 340,
     TSELECT = 341,
     TSERIAL = 342,
     TSHARED = 343,
     TSINGLE = 344,
     TSPARSE = 345,
     TSTRING = 346,
     TSUBDOMAIN = 347,
     TSYNC = 348,
     TTHEN = 349,
     TTHIS = 350,
     TTHROW = 351,
     TTHROWS = 352,
     TTRUE = 353,
     TTRY = 354,
     TTRYBANG = 355,
     TTYPE = 356,
     TUINT = 357,
     TUNDERSCORE = 358,
     TUNION = 359,
     TUNMANAGED = 360,
     TUSE = 361,
     TVAR = 362,
     TVOID = 363,
     TWHEN = 364,
     TWHERE = 365,
     TWHILE = 366,
     TWITH = 367,
     TYIELD = 368,
     TZIP = 369,
     TALIAS = 370,
     TAND = 371,
     TASSIGN = 372,
     TASSIGNBAND = 373,
     TASSIGNBOR = 374,
     TASSIGNBXOR = 375,
     TASSIGNDIVIDE = 376,
     TASSIGNEXP = 377,
     TASSIGNLAND = 378,
     TASSIGNLOR = 379,
     TASSIGNMINUS = 380,
     TASSIGNMOD = 381,
     TASSIGNMULTIPLY = 382,
     TASSIGNPLUS = 383,
     TASSIGNREDUCE = 384,
     TASSIGNSL = 385,
     TASSIGNSR = 386,
     TBANG = 387,
     TBAND = 388,
     TBNOT = 389,
     TBOR = 390,
     TBXOR = 391,
     TCOLON = 392,
     TCOMMA = 393,
     TDIVIDE = 394,
     TDOT = 395,
     TDOTDOT = 396,
     TDOTDOTDOT = 397,
     TEQUAL = 398,
     TEXP = 399,
     TGREATER = 400,
     TGREATEREQUAL = 401,
     THASH = 402,
     TIO = 403,
     TLESS = 404,
     TLESSEQUAL = 405,
     TMINUS = 406,
     TMOD = 407,
     TNOTEQUAL = 408,
     TOR = 409,
     TPLUS = 410,
     TQUESTION = 411,
     TSEMI = 412,
     TSHIFTLEFT = 413,
     TSHIFTRIGHT = 414,
     TSTAR = 415,
     TSWAP = 416,
     TLCBR = 417,
     TRCBR = 418,
     TLP = 419,
     TRP = 420,
     TLSBR = 421,
     TRSBR = 422,
     TNOELSE = 423,
     TUMINUS = 424,
     TUPLUS = 425,
     TLNOT = 426
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
#line 487 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 494 "bison-chapel.cpp"
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
#line 572 "bison-chapel.cpp"

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
#define YYLAST   18062

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  172
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  145
/* YYNRULES -- Number of rules.  */
#define YYNRULES  619
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1105

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   426

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
     165,   166,   167,   168,   169,   170,   171
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
     344,   345,   349,   351,   353,   355,   357,   359,   361,   363,
     367,   373,   379,   382,   383,   392,   393,   403,   404,   413,
     414,   424,   428,   432,   438,   444,   447,   453,   457,   464,
     471,   476,   482,   488,   492,   496,   503,   509,   516,   522,
     529,   533,   538,   542,   547,   554,   562,   569,   577,   582,
     588,   593,   599,   604,   609,   613,   620,   626,   633,   639,
     648,   656,   659,   663,   667,   670,   673,   677,   681,   682,
     685,   688,   692,   698,   700,   704,   708,   714,   720,   721,
     724,   728,   731,   735,   742,   749,   751,   753,   755,   756,
     759,   760,   763,   767,   773,   779,   781,   783,   786,   790,
     792,   796,   797,   798,   807,   808,   810,   812,   813,   814,
     825,   829,   833,   839,   845,   849,   851,   855,   857,   860,
     862,   864,   866,   868,   870,   872,   874,   876,   878,   880,
     882,   884,   886,   888,   890,   892,   894,   896,   898,   900,
     902,   904,   906,   908,   910,   913,   915,   917,   919,   921,
     923,   925,   927,   929,   931,   933,   935,   937,   938,   942,
     946,   947,   949,   953,   958,   964,   969,   976,   983,   984,
     986,   988,   990,   992,   994,   997,  1000,  1002,  1004,  1006,
    1007,  1009,  1011,  1014,  1016,  1018,  1020,  1022,  1023,  1025,
    1028,  1030,  1032,  1034,  1035,  1037,  1039,  1041,  1043,  1045,
    1048,  1050,  1052,  1055,  1058,  1059,  1062,  1065,  1070,  1075,
    1077,  1081,  1085,  1089,  1093,  1097,  1101,  1105,  1108,  1110,
    1112,  1116,  1121,  1126,  1129,  1134,  1135,  1138,  1141,  1143,
    1145,  1147,  1150,  1152,  1157,  1161,  1163,  1167,  1171,  1177,
    1179,  1181,  1185,  1187,  1190,  1194,  1195,  1198,  1201,  1205,
    1208,  1213,  1217,  1221,  1226,  1230,  1231,  1234,  1237,  1240,
    1242,  1243,  1246,  1249,  1252,  1254,  1259,  1264,  1271,  1275,
    1276,  1278,  1280,  1284,  1289,  1293,  1298,  1305,  1306,  1309,
    1312,  1315,  1318,  1320,  1322,  1326,  1330,  1332,  1336,  1338,
    1340,  1342,  1346,  1350,  1351,  1353,  1355,  1359,  1363,  1367,
    1369,  1371,  1373,  1375,  1377,  1380,  1382,  1384,  1386,  1388,
    1390,  1392,  1395,  1400,  1405,  1410,  1416,  1419,  1422,  1424,
    1427,  1429,  1432,  1434,  1437,  1439,  1442,  1444,  1446,  1448,
    1455,  1462,  1467,  1477,  1487,  1495,  1502,  1509,  1514,  1524,
    1534,  1542,  1547,  1554,  1561,  1571,  1581,  1588,  1590,  1592,
    1594,  1596,  1598,  1600,  1602,  1604,  1608,  1609,  1611,  1616,
    1618,  1622,  1627,  1629,  1633,  1638,  1642,  1646,  1648,  1650,
    1653,  1655,  1658,  1660,  1662,  1666,  1668,  1671,  1674,  1677,
    1680,  1683,  1692,  1701,  1711,  1721,  1727,  1733,  1738,  1740,
    1742,  1744,  1746,  1748,  1750,  1752,  1754,  1759,  1763,  1767,
    1770,  1773,  1775,  1776,  1778,  1781,  1784,  1786,  1788,  1790,
    1792,  1794,  1796,  1799,  1802,  1804,  1809,  1814,  1819,  1823,
    1827,  1831,  1835,  1841,  1845,  1850,  1854,  1859,  1861,  1863,
    1865,  1867,  1869,  1871,  1873,  1875,  1877,  1879,  1883,  1888,
    1892,  1897,  1901,  1906,  1910,  1916,  1920,  1924,  1928,  1932,
    1936,  1940,  1944,  1948,  1952,  1956,  1960,  1964,  1968,  1972,
    1976,  1980,  1984,  1988,  1992,  1996,  2000,  2004,  2008,  2011,
    2014,  2017,  2020,  2023,  2026,  2029,  2033,  2037,  2041,  2045,
    2049,  2053,  2057,  2061,  2063,  2065,  2067,  2069,  2071,  2073
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     173,     0,    -1,   174,    -1,    -1,   174,   175,    -1,   177,
      -1,   176,   177,    -1,    73,     8,    -1,   176,    73,     8,
      -1,   179,    -1,   182,    -1,   188,    -1,   189,    -1,   200,
      -1,   190,    -1,   209,    -1,   212,    -1,   210,    -1,   219,
      -1,   213,    -1,   214,    -1,   218,    -1,   199,    -1,   289,
     157,    -1,    14,   177,    -1,    15,   290,   177,    -1,    16,
     191,   157,    -1,    23,   290,   182,    -1,    28,   191,   157,
      -1,    30,   277,   157,    -1,    52,   193,   177,    -1,    56,
     301,   198,    -1,    56,   198,    -1,    65,   301,   198,    -1,
      87,   301,   198,    -1,    87,   198,    -1,    93,   177,    -1,
     113,   301,   157,    -1,     1,   157,    -1,   180,   181,    59,
     193,    -1,   178,   162,   163,    -1,   178,   162,   183,   163,
      -1,   178,   162,     1,   163,    -1,    -1,    78,    -1,    75,
      -1,    -1,    77,    -1,   162,   163,    -1,   162,   183,   163,
      -1,   162,     1,   163,    -1,   175,    -1,   183,   175,    -1,
     301,    -1,   301,    13,   301,    -1,   184,   138,   301,    -1,
     184,   138,   301,    13,   301,    -1,    -1,   184,    -1,   160,
      -1,   184,    -1,    -1,    78,    -1,    75,    -1,   187,   106,
     276,   157,    -1,   187,   106,   301,    36,   186,   157,    -1,
     187,   106,   301,    66,   185,   157,    -1,    83,   276,   157,
      -1,   304,   240,   303,   157,    -1,   304,   161,   303,   157,
      -1,   304,   129,   303,   157,    -1,   304,   123,   303,   157,
      -1,   304,   124,   303,   157,    -1,    -1,     3,    -1,     3,
      -1,    63,    -1,    95,    -1,    39,    -1,    98,    -1,   195,
      -1,     3,    -1,    63,    -1,    95,    -1,    39,    -1,    98,
      -1,   195,    -1,     3,    -1,    95,    -1,    17,    -1,    49,
      -1,   102,    -1,    79,    -1,    44,    -1,    25,    -1,    20,
      -1,    91,    -1,    93,    -1,    89,    -1,    70,    -1,    88,
      -1,    18,    -1,   105,    -1,    33,    -1,    46,    -1,    57,
      -1,    64,    -1,   108,    -1,    17,    -1,    49,    -1,   102,
      -1,    79,    -1,    44,    -1,    25,    -1,    20,    -1,    91,
      -1,    57,    -1,    64,    -1,   108,    -1,    93,    -1,    89,
      -1,    33,    -1,    46,    -1,    32,   177,    -1,   182,    -1,
      84,   157,    -1,    84,   303,   157,    -1,   157,    -1,   202,
      -1,    78,   202,    -1,    -1,    75,   201,   202,    -1,   234,
      -1,   263,    -1,   226,    -1,   259,    -1,   222,    -1,   203,
      -1,   204,    -1,    42,   301,   157,    -1,    42,   301,    36,
     186,   157,    -1,    42,   301,    66,   185,   157,    -1,    42,
     263,    -1,    -1,    38,    80,   205,   193,   224,   162,   225,
     163,    -1,    -1,    38,     8,    80,   206,   193,   224,   162,
     225,   163,    -1,    -1,    37,    80,   207,   193,   224,   162,
     225,   163,    -1,    -1,    37,     8,    80,   208,   193,   224,
     162,   225,   163,    -1,    38,   302,   234,    -1,    37,   302,
     234,    -1,    38,   302,   262,   264,   157,    -1,    37,   302,
     262,   264,   157,    -1,    38,    11,    -1,    32,   177,   111,
     301,   157,    -1,   111,   301,   198,    -1,    24,   301,    45,
     301,   290,   198,    -1,    24,   301,    45,   211,   290,   198,
      -1,    24,   301,   290,   198,    -1,    40,   301,    45,   301,
     198,    -1,    40,   301,    45,   211,   198,    -1,    40,   301,
     198,    -1,    40,   211,   198,    -1,    40,    71,   193,    45,
     301,   198,    -1,    41,   301,    45,   301,   198,    -1,    41,
     301,    45,   301,   293,   198,    -1,    41,   301,    45,   211,
     198,    -1,    41,   301,    45,   211,   293,   198,    -1,    41,
     301,   198,    -1,    41,   301,   293,   198,    -1,    41,   211,
     198,    -1,    41,   211,   293,   198,    -1,   166,   276,    45,
     301,   167,   177,    -1,   166,   276,    45,   301,   293,   167,
     177,    -1,   166,   276,    45,   211,   167,   177,    -1,   166,
     276,    45,   211,   293,   167,   177,    -1,   166,   276,   167,
     177,    -1,   166,   276,   293,   167,   177,    -1,   166,   211,
     167,   177,    -1,   166,   211,   293,   167,   177,    -1,   114,
     164,   276,   165,    -1,    43,   301,    94,   177,    -1,    43,
     301,   182,    -1,    43,   301,    94,   177,    34,   177,    -1,
      43,   301,   182,    34,   177,    -1,    43,   301,   240,   301,
      94,   177,    -1,    43,   301,   240,   301,   182,    -1,    43,
     301,   240,   301,    94,   177,    34,   177,    -1,    43,   301,
     240,   301,   182,    34,   177,    -1,    29,   177,    -1,    99,
     301,   157,    -1,   100,   301,   157,    -1,    99,   190,    -1,
     100,   190,    -1,    99,   182,   215,    -1,   100,   182,   215,
      -1,    -1,   215,   216,    -1,    21,   182,    -1,    21,   217,
     182,    -1,    21,   164,   217,   165,   182,    -1,   193,    -1,
     193,   137,   301,    -1,    96,   301,   157,    -1,    86,   301,
     162,   220,   163,    -1,    86,   301,   162,     1,   163,    -1,
      -1,   220,   221,    -1,   109,   276,   198,    -1,    67,   177,
      -1,    67,    32,   177,    -1,   223,   193,   224,   162,   225,
     163,    -1,   223,   193,   224,   162,     1,   163,    -1,    22,
      -1,    80,    -1,   104,    -1,    -1,   137,   276,    -1,    -1,
     225,   200,    -1,   225,   176,   200,    -1,   227,   193,   162,
     228,   163,    -1,   227,   193,   162,     1,   163,    -1,    35,
      -1,   229,    -1,   228,   138,    -1,   228,   138,   229,    -1,
     193,    -1,   193,   117,   301,    -1,    -1,    -1,    53,   231,
     242,   232,   249,   271,   255,   252,    -1,    -1,    47,    -1,
      69,    -1,    -1,    -1,   233,   248,   235,   237,   236,   249,
     270,   250,   255,   251,    -1,   247,   239,   241,    -1,   247,
     240,   241,    -1,   247,   238,   140,   239,   241,    -1,   247,
     238,   140,   240,   241,    -1,   247,     1,   241,    -1,   283,
      -1,   164,   301,   165,    -1,   192,    -1,   134,   193,    -1,
     133,    -1,   135,    -1,   136,    -1,   134,    -1,   143,    -1,
     153,    -1,   150,    -1,   146,    -1,   149,    -1,   145,    -1,
     155,    -1,   151,    -1,   160,    -1,   139,    -1,   158,    -1,
     159,    -1,   152,    -1,   144,    -1,   132,    -1,    19,    -1,
     147,    -1,    12,    -1,   161,    -1,   148,    -1,    51,    -1,
     193,   132,    -1,   117,    -1,   128,    -1,   125,    -1,   127,
      -1,   121,    -1,   126,    -1,   122,    -1,   118,    -1,   119,
      -1,   120,    -1,   131,    -1,   130,    -1,    -1,   164,   243,
     165,    -1,   164,   243,   165,    -1,    -1,   244,    -1,   243,
     138,   244,    -1,   245,   193,   275,   268,    -1,   176,   245,
     193,   275,   268,    -1,   245,   193,   275,   254,    -1,   245,
     164,   267,   165,   275,   268,    -1,   245,   164,   267,   165,
     275,   254,    -1,    -1,   246,    -1,    45,    -1,    48,    -1,
      68,    -1,    27,    -1,    27,    45,    -1,    27,    82,    -1,
      71,    -1,    82,    -1,   101,    -1,    -1,    71,    -1,    82,
      -1,    27,    82,    -1,    27,    -1,   101,    -1,    76,    -1,
      50,    -1,    -1,    27,    -1,    27,    82,    -1,    82,    -1,
      71,    -1,   101,    -1,    -1,    97,    -1,   157,    -1,   252,
      -1,   182,    -1,   199,    -1,   156,   193,    -1,     4,    -1,
     142,    -1,   142,   301,    -1,   142,   253,    -1,    -1,   110,
     301,    -1,    55,   256,    -1,   110,   301,    55,   256,    -1,
      55,   256,   110,   301,    -1,   257,    -1,   256,   138,   257,
      -1,   258,   117,   258,    -1,   258,   149,   258,    -1,   258,
     150,   258,    -1,   258,   143,   258,    -1,   258,   145,   258,
      -1,   258,   146,   258,    -1,    84,   258,    -1,     3,    -1,
      95,    -1,   101,   260,   157,    -1,    26,   101,   260,   157,
      -1,    38,   101,   260,   157,    -1,   193,   261,    -1,   193,
     261,   138,   260,    -1,    -1,   117,   284,    -1,   117,   272,
      -1,    71,    -1,    27,    -1,    82,    -1,    27,    82,    -1,
     107,    -1,    26,   262,   264,   157,    -1,   262,   264,   157,
      -1,   265,    -1,   264,   138,   265,    -1,   193,   271,   268,
      -1,   164,   267,   165,   271,   268,    -1,   103,    -1,   193,
      -1,   164,   267,   165,    -1,   266,    -1,   266,   138,    -1,
     266,   138,   267,    -1,    -1,   117,    62,    -1,   117,   303,
      -1,   166,   167,   284,    -1,   166,   167,    -1,   166,   276,
     167,   284,    -1,   166,   276,   167,    -1,   166,   167,   269,
      -1,   166,   276,   167,   269,    -1,   166,     1,   167,    -1,
      -1,   137,   284,    -1,   137,   269,    -1,   137,   197,    -1,
       1,    -1,    -1,   137,   284,    -1,   137,   272,    -1,   137,
     197,    -1,     1,    -1,   166,   276,   167,   284,    -1,   166,
     276,   167,   272,    -1,   166,   276,    45,   301,   167,   284,
      -1,   166,     1,   167,    -1,    -1,   284,    -1,   253,    -1,
     166,   167,   273,    -1,   166,   276,   167,   273,    -1,   166,
     167,   274,    -1,   166,   276,   167,   274,    -1,   166,   276,
      45,   301,   167,   273,    -1,    -1,   137,   284,    -1,   137,
     253,    -1,   137,   197,    -1,   137,   274,    -1,   301,    -1,
     253,    -1,   276,   138,   301,    -1,   276,   138,   253,    -1,
     301,    -1,   277,   138,   301,    -1,   103,    -1,   303,    -1,
     253,    -1,   278,   138,   278,    -1,   279,   138,   278,    -1,
      -1,   281,    -1,   282,    -1,   281,   138,   282,    -1,   194,
     117,   253,    -1,   194,   117,   303,    -1,   253,    -1,   303,
      -1,   194,    -1,   196,    -1,   285,    -1,   285,   156,    -1,
     156,    -1,   288,    -1,   304,    -1,   287,    -1,   313,    -1,
     312,    -1,    89,   301,    -1,    46,   164,   280,   165,    -1,
      33,   164,   280,   165,    -1,    92,   164,   280,   165,    -1,
      90,    92,   164,   280,   165,    -1,    14,   301,    -1,    93,
     301,    -1,    70,    -1,    70,   301,    -1,   105,    -1,   105,
     301,    -1,    88,    -1,    88,   301,    -1,    18,    -1,    18,
     301,    -1,    22,    -1,    80,    -1,    35,    -1,    40,   301,
      45,   301,    32,   301,    -1,    40,   301,    45,   211,    32,
     301,    -1,    40,   301,    32,   301,    -1,    40,   301,    45,
     301,    32,    43,   301,    94,   301,    -1,    40,   301,    45,
     211,    32,    43,   301,    94,   301,    -1,    40,   301,    32,
      43,   301,    94,   301,    -1,    41,   301,    45,   301,    32,
     301,    -1,    41,   301,    45,   211,    32,   301,    -1,    41,
     301,    32,   301,    -1,    41,   301,    45,   301,    32,    43,
     301,    94,   301,    -1,    41,   301,    45,   211,    32,    43,
     301,    94,   301,    -1,    41,   301,    32,    43,   301,    94,
     301,    -1,   166,   276,   167,   301,    -1,   166,   276,    45,
     301,   167,   301,    -1,   166,   276,    45,   211,   167,   301,
      -1,   166,   276,    45,   301,   167,    43,   301,    94,   301,
      -1,   166,   276,    45,   211,   167,    43,   301,    94,   301,
      -1,    43,   301,    94,   301,    34,   301,    -1,    61,    -1,
     288,    -1,   283,    -1,   307,    -1,   306,    -1,   230,    -1,
     299,    -1,   300,    -1,   297,   148,   301,    -1,    -1,   291,
      -1,   112,   164,   292,   165,    -1,   295,    -1,   292,   138,
     295,    -1,   112,   164,   294,   165,    -1,   295,    -1,   294,
     138,   295,    -1,   296,   283,   271,   268,    -1,   316,    81,
     283,    -1,   301,    81,   283,    -1,    27,    -1,    45,    -1,
      27,    45,    -1,    82,    -1,    27,    82,    -1,   107,    -1,
     304,    -1,   297,   148,   301,    -1,    60,    -1,    60,    70,
      -1,    60,    88,    -1,    60,   105,    -1,    60,    18,    -1,
     298,   301,    -1,    60,    70,   164,   301,   165,   164,   280,
     165,    -1,    60,    88,   164,   301,   165,   164,   280,   165,
      -1,    60,    70,   164,   301,   165,   164,   280,   165,   156,
      -1,    60,    88,   164,   301,   165,   164,   280,   165,   156,
      -1,    60,    70,   164,   301,   165,    -1,    60,    88,   164,
     301,   165,    -1,    54,   264,    45,   301,    -1,   310,    -1,
     284,    -1,   286,    -1,   314,    -1,   315,    -1,   230,    -1,
     299,    -1,   300,    -1,   164,   142,   301,   165,    -1,   301,
     137,   301,    -1,   301,   141,   301,    -1,   301,   141,    -1,
     141,   301,    -1,   141,    -1,    -1,   301,    -1,    99,   301,
      -1,   100,   301,    -1,   301,    -1,   283,    -1,   306,    -1,
     307,    -1,   308,    -1,   304,    -1,   301,   132,    -1,   285,
     156,    -1,   230,    -1,   305,   164,   280,   165,    -1,   305,
     166,   280,   167,    -1,    74,   164,   280,   165,    -1,   301,
     140,   194,    -1,   301,   140,   101,    -1,   301,   140,    33,
      -1,   301,   140,    57,    -1,   301,   140,    20,   164,   165,
      -1,   164,   278,   165,    -1,   164,   278,   138,   165,    -1,
     164,   279,   165,    -1,   164,   279,   138,   165,    -1,    39,
      -1,    98,    -1,   309,    -1,     5,    -1,     6,    -1,     7,
      -1,     8,    -1,     9,    -1,    10,    -1,    63,    -1,   162,
     276,   163,    -1,   162,   276,   138,   163,    -1,   166,   276,
     167,    -1,   166,   276,   138,   167,    -1,   166,   311,   167,
      -1,   166,   311,   138,   167,    -1,   301,   115,   301,    -1,
     311,   138,   301,   115,   301,    -1,   301,   155,   301,    -1,
     301,   151,   301,    -1,   301,   160,   301,    -1,   301,   139,
     301,    -1,   301,   158,   301,    -1,   301,   159,   301,    -1,
     301,   152,   301,    -1,   301,   143,   301,    -1,   301,   153,
     301,    -1,   301,   150,   301,    -1,   301,   146,   301,    -1,
     301,   149,   301,    -1,   301,   145,   301,    -1,   301,   133,
     301,    -1,   301,   135,   301,    -1,   301,   136,   301,    -1,
     301,   116,   301,    -1,   301,   154,   301,    -1,   301,   144,
     301,    -1,   301,    19,   301,    -1,   301,    12,   301,    -1,
     301,   147,   301,    -1,   301,    31,   301,    -1,   155,   301,
      -1,   151,   301,    -1,    58,   301,    -1,    72,   301,    -1,
     132,   301,    -1,   301,   132,    -1,   134,   301,    -1,   301,
      81,   301,    -1,   301,    81,   211,    -1,   316,    81,   301,
      -1,   316,    81,   211,    -1,   301,    85,   301,    -1,   301,
      85,   211,    -1,   316,    85,   301,    -1,   316,    85,   211,
      -1,   155,    -1,   160,    -1,   116,    -1,   154,    -1,   133,
      -1,   135,    -1,   136,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   497,   497,   502,   503,   509,   510,   515,   516,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,   539,   540,   541,
     542,   543,   544,   545,   546,   547,   548,   549,   550,   554,
     567,   572,   577,   585,   586,   587,   591,   592,   605,   606,
     607,   612,   613,   618,   623,   628,   632,   639,   644,   648,
     653,   657,   658,   659,   663,   664,   665,   669,   673,   675,
     677,   679,   681,   688,   689,   693,   694,   695,   696,   697,
     698,   701,   702,   703,   704,   705,   706,   718,   719,   730,
     731,   732,   733,   734,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   752,   753,
     754,   755,   756,   757,   758,   759,   760,   761,   762,   769,
     770,   771,   772,   776,   777,   781,   782,   786,   787,   788,
     798,   798,   803,   804,   805,   806,   807,   808,   809,   813,
     814,   815,   816,   821,   820,   836,   835,   852,   851,   867,
     866,   882,   886,   891,   899,   910,   917,   918,   919,   920,
     921,   922,   923,   924,   925,   926,   927,   928,   929,   930,
     931,   932,   933,   934,   935,   941,   947,   953,   959,   966,
     973,   977,   984,   988,   989,   990,   991,   992,   994,   996,
     998,  1003,  1006,  1007,  1008,  1009,  1010,  1011,  1015,  1016,
    1020,  1021,  1022,  1026,  1027,  1031,  1034,  1036,  1041,  1042,
    1046,  1048,  1050,  1057,  1067,  1081,  1086,  1091,  1099,  1100,
    1105,  1106,  1108,  1113,  1129,  1136,  1145,  1153,  1157,  1164,
    1165,  1170,  1175,  1169,  1202,  1205,  1209,  1217,  1227,  1216,
    1259,  1263,  1268,  1272,  1277,  1284,  1285,  1289,  1290,  1291,
    1292,  1293,  1294,  1295,  1296,  1297,  1298,  1299,  1300,  1301,
    1302,  1303,  1304,  1305,  1306,  1307,  1308,  1309,  1310,  1311,
    1312,  1313,  1314,  1315,  1316,  1320,  1321,  1322,  1323,  1324,
    1325,  1326,  1327,  1328,  1329,  1330,  1331,  1335,  1336,  1340,
    1344,  1345,  1346,  1350,  1352,  1354,  1356,  1358,  1363,  1364,
    1368,  1369,  1370,  1371,  1372,  1373,  1374,  1375,  1376,  1380,
    1381,  1382,  1383,  1384,  1385,  1389,  1390,  1394,  1395,  1396,
    1397,  1398,  1399,  1403,  1404,  1407,  1408,  1412,  1413,  1417,
    1422,  1426,  1427,  1428,  1436,  1437,  1439,  1441,  1443,  1448,
    1450,  1455,  1456,  1457,  1458,  1459,  1460,  1461,  1465,  1467,
    1472,  1474,  1476,  1481,  1494,  1511,  1512,  1514,  1519,  1520,
    1521,  1522,  1523,  1527,  1533,  1541,  1542,  1550,  1552,  1557,
    1559,  1561,  1566,  1568,  1570,  1577,  1578,  1579,  1584,  1586,
    1588,  1592,  1596,  1598,  1602,  1610,  1611,  1612,  1613,  1614,
    1619,  1620,  1621,  1622,  1623,  1643,  1647,  1651,  1659,  1666,
    1667,  1668,  1672,  1674,  1680,  1682,  1684,  1689,  1690,  1691,
    1692,  1693,  1699,  1700,  1701,  1702,  1706,  1707,  1711,  1712,
    1713,  1717,  1718,  1722,  1723,  1727,  1728,  1732,  1733,  1734,
    1735,  1739,  1740,  1751,  1753,  1755,  1761,  1762,  1763,  1764,
    1765,  1766,  1768,  1770,  1772,  1774,  1776,  1778,  1781,  1783,
    1785,  1787,  1789,  1791,  1793,  1795,  1798,  1800,  1802,  1807,
    1809,  1811,  1813,  1815,  1817,  1819,  1821,  1823,  1825,  1827,
    1829,  1831,  1838,  1844,  1850,  1856,  1865,  1875,  1883,  1884,
    1885,  1886,  1887,  1888,  1889,  1890,  1895,  1896,  1900,  1904,
    1905,  1909,  1913,  1914,  1918,  1922,  1926,  1933,  1934,  1935,
    1936,  1937,  1938,  1942,  1943,  1948,  1950,  1954,  1958,  1962,
    1970,  1975,  1981,  1987,  1994,  2003,  2005,  2010,  2018,  2019,
    2020,  2021,  2022,  2023,  2024,  2025,  2026,  2028,  2030,  2032,
    2034,  2036,  2041,  2042,  2046,  2047,  2048,  2052,  2053,  2054,
    2055,  2066,  2067,  2068,  2069,  2073,  2074,  2075,  2079,  2080,
    2081,  2082,  2083,  2091,  2092,  2093,  2094,  2098,  2099,  2103,
    2104,  2105,  2106,  2107,  2108,  2109,  2110,  2111,  2112,  2113,
    2114,  2115,  2119,  2127,  2128,  2132,  2133,  2134,  2135,  2136,
    2137,  2138,  2139,  2140,  2141,  2142,  2143,  2144,  2145,  2146,
    2147,  2148,  2149,  2150,  2151,  2152,  2153,  2154,  2158,  2159,
    2160,  2161,  2162,  2163,  2164,  2168,  2169,  2170,  2171,  2175,
    2176,  2177,  2178,  2183,  2184,  2185,  2186,  2187,  2188,  2189
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
  "TFORWARDING", "TIF", "TIMAG", "TIN", "TINDEX", "TINLINE", "TINOUT",
  "TINT", "TITER", "TINITEQUALS", "TLABEL", "TLAMBDA", "TLET", "TLIFETIME",
  "TLOCAL", "TLOCALE", "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT",
  "TNONE", "TNOTHING", "TON", "TONLY", "TOTHERWISE", "TOUT", "TOVERRIDE",
  "TOWNED", "TPARAM", "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE",
  "TPROC", "TPROTOTYPE", "TPUBLIC", "TREAL", "TRECORD", "TREDUCE", "TREF",
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
     425,   426
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   172,   173,   174,   174,   175,   175,   176,   176,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   178,
     179,   179,   179,   180,   180,   180,   181,   181,   182,   182,
     182,   183,   183,   184,   184,   184,   184,   185,   185,   186,
     186,   187,   187,   187,   188,   188,   188,   189,   190,   190,
     190,   190,   190,   191,   191,   192,   192,   192,   192,   192,
     192,   193,   193,   193,   193,   193,   193,   194,   194,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   197,
     197,   197,   197,   198,   198,   199,   199,   200,   200,   200,
     201,   200,   202,   202,   202,   202,   202,   202,   202,   203,
     203,   203,   203,   205,   204,   206,   204,   207,   204,   208,
     204,   204,   204,   204,   204,   209,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   211,   212,   212,   212,   212,   212,   212,   212,
     212,   213,   214,   214,   214,   214,   214,   214,   215,   215,
     216,   216,   216,   217,   217,   218,   219,   219,   220,   220,
     221,   221,   221,   222,   222,   223,   223,   223,   224,   224,
     225,   225,   225,   226,   226,   227,   228,   228,   228,   229,
     229,   231,   232,   230,   233,   233,   233,   235,   236,   234,
     237,   237,   237,   237,   237,   238,   238,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   241,   241,   242,
     243,   243,   243,   244,   244,   244,   244,   244,   245,   245,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   247,
     247,   247,   247,   247,   247,   248,   248,   249,   249,   249,
     249,   249,   249,   250,   250,   251,   251,   252,   252,   253,
     253,   254,   254,   254,   255,   255,   255,   255,   255,   256,
     256,   257,   257,   257,   257,   257,   257,   257,   258,   258,
     259,   259,   259,   260,   260,   261,   261,   261,   262,   262,
     262,   262,   262,   263,   263,   264,   264,   265,   265,   266,
     266,   266,   267,   267,   267,   268,   268,   268,   269,   269,
     269,   269,   269,   269,   269,   270,   270,   270,   270,   270,
     271,   271,   271,   271,   271,   272,   272,   272,   272,   273,
     273,   273,   274,   274,   274,   274,   274,   275,   275,   275,
     275,   275,   276,   276,   276,   276,   277,   277,   278,   278,
     278,   279,   279,   280,   280,   281,   281,   282,   282,   282,
     282,   283,   283,   284,   284,   284,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   287,   288,   289,   289,
     289,   289,   289,   289,   289,   289,   290,   290,   291,   292,
     292,   293,   294,   294,   295,   295,   295,   296,   296,   296,
     296,   296,   296,   297,   297,   298,   298,   298,   298,   298,
     299,   299,   299,   299,   299,   299,   299,   300,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   302,   302,   303,   303,   303,   304,   304,   304,
     304,   305,   305,   305,   305,   306,   306,   306,   307,   307,
     307,   307,   307,   308,   308,   308,   308,   309,   309,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   311,   311,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   313,   313,
     313,   313,   313,   313,   313,   314,   314,   314,   314,   315,
     315,   315,   315,   316,   316,   316,   316,   316,   316,   316
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
       0,     3,     1,     1,     1,     1,     1,     1,     1,     3,
       5,     5,     2,     0,     8,     0,     9,     0,     8,     0,
       9,     3,     3,     5,     5,     2,     5,     3,     6,     6,
       4,     5,     5,     3,     3,     6,     5,     6,     5,     6,
       3,     4,     3,     4,     6,     7,     6,     7,     4,     5,
       4,     5,     4,     4,     3,     6,     5,     6,     5,     8,
       7,     2,     3,     3,     2,     2,     3,     3,     0,     2,
       2,     3,     5,     1,     3,     3,     5,     5,     0,     2,
       3,     2,     3,     6,     6,     1,     1,     1,     0,     2,
       0,     2,     3,     5,     5,     1,     1,     2,     3,     1,
       3,     0,     0,     8,     0,     1,     1,     0,     0,    10,
       3,     3,     5,     5,     3,     1,     3,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     3,     3,
       0,     1,     3,     4,     5,     4,     6,     6,     0,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     0,
       1,     1,     2,     1,     1,     1,     1,     0,     1,     2,
       1,     1,     1,     0,     1,     1,     1,     1,     1,     2,
       1,     1,     2,     2,     0,     2,     2,     4,     4,     1,
       3,     3,     3,     3,     3,     3,     3,     2,     1,     1,
       3,     4,     4,     2,     4,     0,     2,     2,     1,     1,
       1,     2,     1,     4,     3,     1,     3,     3,     5,     1,
       1,     3,     1,     2,     3,     0,     2,     2,     3,     2,
       4,     3,     3,     4,     3,     0,     2,     2,     2,     1,
       0,     2,     2,     2,     1,     4,     4,     6,     3,     0,
       1,     1,     3,     4,     3,     4,     6,     0,     2,     2,
       2,     2,     1,     1,     3,     3,     1,     3,     1,     1,
       1,     3,     3,     0,     1,     1,     3,     3,     3,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     2,     4,     4,     4,     5,     2,     2,     1,     2,
       1,     2,     1,     2,     1,     2,     1,     1,     1,     6,
       6,     4,     9,     9,     7,     6,     6,     4,     9,     9,
       7,     4,     6,     6,     9,     9,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     0,     1,     4,     1,
       3,     4,     1,     3,     4,     3,     3,     1,     1,     2,
       1,     2,     1,     1,     3,     1,     2,     2,     2,     2,
       2,     8,     8,     9,     9,     5,     5,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     3,     3,     2,
       2,     1,     0,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     2,     2,     1,     4,     4,     4,     3,     3,
       3,     3,     5,     3,     4,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     4,     3,
       4,     3,     4,     3,     5,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    87,   560,   561,   562,   563,
     564,   565,     0,   486,    73,   108,   454,   114,   456,   486,
       0,   113,     0,   359,    73,     0,     0,     0,     0,   458,
     532,   532,   557,     0,     0,     0,     0,   112,     0,   235,
     109,     0,   231,     0,     0,   116,     0,   505,   477,   566,
     117,     0,   236,   448,   358,     0,     0,     0,   130,    44,
     111,   457,   360,     0,     0,     0,     0,   452,     0,     0,
     115,     0,     0,    88,     0,   558,     0,     0,     0,   110,
     217,   450,   362,   118,     0,     0,   615,     0,   617,     0,
     618,   619,   531,     0,   616,   613,   435,   127,   614,     0,
       0,     0,     4,     0,     5,     0,     9,    46,    10,     0,
      11,    12,    14,   431,   432,    22,    13,   128,   137,   138,
      15,    17,    16,    19,    20,    21,    18,   136,     0,   134,
       0,   523,     0,   132,   135,     0,   133,   537,   519,   433,
     520,   438,   436,     0,     0,     0,   524,   525,     0,   437,
       0,   538,   539,   540,   559,   518,   440,   439,   521,   522,
       0,    38,    24,   446,     0,     0,   487,    74,     0,     0,
     456,   458,     0,     0,     0,   457,     0,     0,     0,   523,
     537,   436,   524,   525,   455,   437,   538,   539,     0,   486,
       0,     0,   361,     0,   191,     0,   416,     0,   423,   563,
     457,   533,   234,   563,   155,   457,     0,   234,     0,     0,
       0,     0,     0,     0,     0,   142,     0,     0,   423,    81,
      89,   101,    95,    94,   103,    84,    93,   104,    90,   105,
      82,   106,    99,    92,   100,    98,    96,    97,    83,    85,
      91,   102,   107,     0,    86,     0,     0,     0,     0,   365,
       0,   124,    32,     0,   600,   509,   506,   507,   508,     0,
     449,   601,     7,   423,   234,   215,   225,   532,   216,   129,
     330,   435,   413,     0,   412,     0,     0,   125,   536,     0,
       0,    35,     0,   453,   441,     0,   423,    36,   447,     0,
     198,   194,     0,   437,   198,   195,     0,   355,     0,   451,
       0,     0,   602,   604,   530,   599,   598,     0,    48,    51,
       0,     0,   418,     0,   420,     0,     0,   419,     0,     0,
     412,     0,     0,     6,     0,    47,     0,     0,   218,     0,
     316,   315,   237,     0,   434,    23,     0,   510,     0,     0,
       0,     0,     0,     0,   603,     0,     0,     0,     0,     0,
       0,   529,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   275,   282,   283,
     284,   279,   281,     0,     0,   277,   280,   278,   276,     0,
     286,   285,     0,     0,   423,   423,     0,     0,     0,    25,
      26,     0,     0,     0,     0,     0,    27,     0,     0,     0,
       0,    28,     0,    29,     0,   431,   429,     0,   424,   425,
     430,   149,     0,   152,     0,   145,     0,     0,   151,     0,
       0,     0,   164,     0,     0,   163,     0,   172,     0,     0,
       0,   170,     0,     0,    57,   139,     0,   184,     0,     0,
      30,   298,   232,   369,     0,   370,   372,     0,   394,     0,
     375,     0,     0,   123,    31,     0,     0,    33,     0,   131,
     329,     0,    67,   534,   535,   126,     0,    34,   423,     0,
     205,   196,   192,   197,   193,     0,   353,   350,   157,    37,
      50,    49,    52,     0,   567,     0,     0,   553,     0,   555,
       0,     0,     0,     0,     0,     0,     0,     0,   571,     8,
       0,    40,     0,     0,     0,   412,     0,     0,     0,   309,
     364,   485,   595,   594,   597,   606,   605,   610,   609,   591,
     588,   589,   590,   527,   578,     0,   550,   551,   549,   548,
     528,   582,   593,   587,   585,   596,   586,   584,   576,   581,
     583,   592,   575,   579,   580,   577,     0,     0,     0,     0,
       0,     0,     0,   608,   607,   612,   611,   497,   498,   500,
     502,     0,   489,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   569,   486,   486,   160,   351,   363,   417,     0,
       0,   443,     0,     0,   218,     0,     0,   218,   352,     0,
       0,     0,     0,   461,     0,     0,     0,   173,     0,   467,
       0,     0,   171,   614,    60,     0,    53,    58,     0,   183,
       0,     0,     0,   442,   303,   300,   301,   302,   306,   307,
     308,   298,     0,   291,     0,   299,   317,     0,   373,     0,
     121,   122,   120,   119,     0,   393,   392,   519,     0,   367,
     517,   366,     0,     0,   547,   415,   414,     0,     0,     0,
     444,     0,   199,   357,   519,     0,   568,   526,   554,   421,
     556,   422,   180,     0,     0,     0,   570,   178,   471,     0,
     573,   572,     0,    42,    41,    39,    64,     0,    57,   219,
       0,     0,   229,     0,   226,   313,   310,   311,   314,   238,
       0,     0,    71,    72,    70,    69,    68,   545,   546,   499,
     501,     0,   488,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   156,   427,   428,   426,
     218,     0,   154,   218,     0,   153,     0,   182,     0,     0,
     162,     0,   161,     0,   492,     0,     0,   168,     0,     0,
     166,     0,     0,   140,     0,   141,     0,     0,   186,     0,
     188,   304,   305,     0,   298,   289,     0,   407,   318,   321,
     320,   322,     0,   371,   374,   375,     0,     0,   376,   377,
     515,   516,   207,     0,     0,   206,   209,   445,     0,   200,
     203,     0,   354,   181,     0,     0,     0,     0,   179,     0,
       0,     0,     0,   234,   224,     0,   227,   223,   312,   317,
     287,    75,   270,    89,   268,    95,    94,    78,    93,    90,
     273,   105,    76,   106,    92,    96,    77,    79,    91,   107,
     267,   249,   252,   250,   251,   262,   253,   266,   258,   256,
     269,   272,   257,   255,   260,   265,   254,   259,   263,   264,
     261,   271,     0,   247,     0,    80,     0,   287,   287,   245,
     552,   490,   375,   537,   537,     0,     0,     0,     0,     0,
       0,     0,     0,   159,   158,     0,   220,     0,   220,   165,
       0,     0,   460,     0,   459,     0,   491,     0,     0,   466,
     169,     0,   465,   167,    55,    54,   185,   476,   187,     0,
     407,   292,     0,     0,   375,   319,   334,   368,   398,     0,
     569,   423,   423,     0,   211,     0,     0,     0,   201,     0,
     176,   473,     0,     0,   174,   472,     0,   574,    65,    66,
     214,   130,   234,   213,   234,   221,   230,   228,     0,   298,
     244,   248,     0,   274,     0,   240,   241,   494,     0,     0,
       0,     0,     0,     0,     0,     0,   220,   234,   220,   234,
     464,     0,     0,   493,   470,     0,     0,     0,     0,   190,
     375,   407,     0,   410,   409,   411,   519,   331,   295,   293,
       0,     0,     0,     0,   396,   519,     0,     0,   212,   210,
       0,   204,     0,   177,     0,   175,   222,   389,     0,   323,
       0,   246,    75,    77,   287,   287,     0,     0,     0,     0,
       0,     0,   234,   148,   234,   144,     0,     0,     0,     0,
      56,   189,   294,   375,   399,     0,   333,   332,   348,     0,
     349,   336,   339,     0,   335,   327,   328,   233,     0,   511,
     512,   202,     0,     0,     0,   388,   387,   519,   324,   334,
     288,   242,   243,     0,     0,     0,     0,     0,     0,   150,
     146,   463,   462,   469,   468,   297,   296,   401,   402,   404,
     519,     0,   569,   347,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   519,   513,   514,   475,   474,     0,   379,
       0,     0,     0,   403,   405,   338,   340,   341,   344,   345,
     346,   342,   343,   337,   384,   382,   519,   569,   325,   239,
     326,   399,   383,   519,   406
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   309,   103,   609,   105,   106,   107,   326,
     108,   310,   604,   608,   605,   109,   110,   111,   112,   168,
     843,   247,   113,   244,   114,   635,   252,   115,   116,   264,
     117,   118,   119,   416,   586,   412,   583,   120,   121,   712,
     122,   123,   124,   471,   652,   781,   125,   126,   648,   776,
     127,   128,   507,   793,   129,   130,   683,   684,   179,   245,
     626,   132,   133,   509,   799,   689,   846,   847,   438,   930,
     442,   622,   623,   624,   625,   690,   332,   762,  1039,  1099,
    1027,   272,   968,   972,  1021,  1022,  1023,   134,   298,   476,
     135,   136,   248,   249,   446,   447,   639,  1036,   989,   450,
     636,  1058,   965,   894,   311,   195,   315,   316,   407,   408,
     409,   180,   138,   139,   140,   141,   181,   143,   165,   166,
     561,   428,   733,   562,   563,   144,   145,   182,   183,   148,
     207,   410,   185,   150,   186,   187,   153,   154,   155,   321,
     156,   157,   158,   159,   160
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -987
static const yytype_int16 yypact[] =
{
    -987,   100,  3113,  -987,    20,  -987,  -987,  -987,  -987,  -987,
    -987,  -987,  4590,   119,   294,  -987, 13309,  -987, 16455,   119,
   10259,  -987,   420,   216,   294,  4590, 10259,  4590,   156, 17726,
   10421,  8177,  -987,  8339,  9117,  6871, 10259,  -987,   159,  -987,
    -987, 17783,  -987,  2284,  9279,  -987, 10259,   201,  -987,  -987,
    -987, 10259,  -987, 13309,  -987, 10259,   299,   204,   190, 15211,
    -987, 17840,  -987,  9443,  7689, 10259,  9279, 13309, 10259,   218,
    -987,   205,  4590,  -987, 10259,  -987, 10583, 10583, 17783,  -987,
    -987, 13309,  -987,  -987, 10259, 10259,  -987, 10259,  -987, 10259,
    -987,  -987, 12851, 10259,  -987, 10259,  -987,  -987,  -987,  3608,
    7035,  8503,  -987,  4428,  -987,   235,  -987,   331,  -987,   301,
    -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,
    -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987, 17783,  -987,
   17783,   348,    41,  -987,  -987,  2284,  -987,   277,  -987,   263,
    -987,  -987,   283,   308,   321, 10259,   322,   329, 17329, 12844,
     -55,   347,   353,  -987,  -987,  -987,  -987,  -987,  -987,  -987,
     313,  -987,  -987, 17329,   314,  4590,  -987,  -987,   354, 10259,
    -987,  -987, 10259, 10259, 10259,  -987, 10259,  9443,  9443,   192,
    -987,  -987,  -987,  -987,   -34,   298,  -987,  -987,   357, 15302,
   17783,  2284,  -987,   359,  -987,   -44, 17329,   411,  7853,   445,
   17897, 17329,   401,   449,  -987, 17954, 17783,   401, 17783,   366,
       8, 14836,    25, 14804,    22,  -987, 15245, 14052,  7853,  -987,
    -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,
    -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,
    -987,  -987,  -987,  4590,  -987,   367,  1539,    85,    29,  -987,
    4590,  -987,  -987, 14980,  1265,  -987,   375,   377,  -987, 14980,
     -34,  1265,  -987,  7853,  1872,  -987,  -987,  9605,  -987,  -987,
    -987, 17783,  -987,   102, 17329, 10259, 10259,  -987, 17329,   388,
   15213,  -987, 14980,   -34, 17329,   389,  7853,  -987, 17329, 15389,
    -987,  -987, 15654, 13302,  -987,  -987, 15703,   444,   405,   -34,
   14980, 15798,   504,   504,  1051,  1265,  1265,   236,  -987,  -987,
    3772,   -35,  -987, 10259,  -987,    -6,    96,  -987,   -33,    86,
   15847,   -41,   557,  -987,  3936,  -987,   507,  9443,   432,   412,
    -987,  -987,  -987,   143,   333,  -987, 10259,   439, 10259, 10259,
   10259,  9117,  9117, 10259,   351, 10259, 10259, 10259, 10259, 10259,
     135, 12851, 10259, 10259, 10259, 10259, 10259, 10259, 10259, 10259,
   10259, 10259, 10259, 10259, 10259, 10259, 10259,  -987,  -987,  -987,
    -987,  -987,  -987,  8665,  8665,  -987,  -987,  -987,  -987,  8665,
    -987,  -987,  8665,  8665,  7853,  7853,  9117,  9117,  7527,  -987,
    -987, 15446, 15478, 15879,    50,  4100,  -987,  9117,     8,   425,
     293,  -987, 10259,  -987, 10259,   471,  -987,   427,   457,  -987,
    -987,  -987, 17783,  -987,  2284,  -987, 17783,   442,  -987,  2284,
     551,  9443,  -987,  4752,  9117,  -987,   447,  -987,     8,  4914,
    9117,  -987,     8, 10745, 10259,  -987,  4590,   569, 10259,   443,
    -987,   545,  -987,  -987,  1539,  -987,   477,   454,  -987, 10907,
     512, 10259,  2284,  -987,  -987, 10259, 10259,  -987,   463,  -987,
    -987,  9443,  -987, 17329, 17329,  -987,    32,  -987,  7853,   467,
    -987,   610,  -987,   610,  -987, 11069,   496,  -987,  -987,  -987,
    -987,  -987,  -987,  8829,  -987, 13810,  7199,  -987,  7363,  -987,
    4590,   470,  9117,  6376,  3444,   475, 10259,  6706,  -987,  -987,
     272,  -987,  4264, 17783,   303, 15622,  9443,   476, 17669,   466,
    -987, 16023,   664,   664,   -30,  -987,   -30,  -987,   -30, 13764,
    1540,  1577,   626,   -34,   504,   481,  -987,  -987,  -987,  -987,
    1051, 16842,   -30,  1372,  1372,   664,  1372,  1372,   465,   504,
   16842, 17516,   465,  1265,  1265,   504,   490,   492,   493,   494,
     498,   495,   501,  -987,   -30,  -987,   -30,   146,  -987,  -987,
    -987,   112,  -987,  1070, 17405,   376, 11231,  9117, 11393,  9117,
   10259,  9117, 13569,   119, 16068,  -987,  -987,  -987, 17329, 16109,
    7853,  -987,  7853, 17783,   432,   320, 17783,   432,  -987,   330,
   10259,   138, 10259, 17329,    34, 15037,  7527,  -987, 10259, 17329,
      27, 14893,  -987,   506,   524,   514, 16220,   524,   515,   630,
   16260,  4590, 15069,  -987,   180,  -987,  -987,  -987,  -987,  -987,
    -987,   220,   144,  -987,  2539,  -987,   505,   508,  1539,    85,
     156,   159, 10259, 10259,  6212,  -987,  -987,   743,  8015,  -987,
   17329,  -987, 13867, 13908,  -987,  -987, 17329,   516,   -19,   517,
    -987,  2033,  -987,  -987,   335, 17783,  -987,  -987,  -987,  -987,
    -987,  -987,  -987,  4590,   -27,  2692,  -987,  -987, 17329,  4590,
   17329,  -987, 16301,  -987,  -987,  -987,  -987, 10745, 10259,   531,
    1066,   525,   560,   131,  -987,   605,  -987,  -987,  -987,  -987,
    8957,   526,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,
    -987,  7527,  -987,    35,  9117,  9117, 10259,   658, 16445, 10259,
     666, 16477,   529,  2897,     8,     8,  -987,  -987,  -987,  -987,
     432,   540,  -987,   432,   541,  -987, 14980,  -987, 14128,  5076,
    -987,  5238,  -987,   239,  -987, 14204,  5400,  -987,     8,  5562,
    -987,     8, 10259,  -987, 10259,  -987,  4590, 10259,  -987,  4590,
     670,  -987,  -987, 17783,   585,  -987,  1539,   568,   624,  -987,
    -987,  -987,   123,  -987,  -987,   512,   546,    74,  -987,  -987,
     544,   548,  -987,  5724,  9443,  -987,  -987,  -987, 17783,  -987,
     578,   357,  -987,  -987,  5886,   555,  6048,   556,  -987, 10259,
     561,   571,   562,  1232,  -987, 10259, 17783,  -987,  -987,   505,
     565,    95,  -987,   593,  -987,   594,   596,   606,   597,   599,
    -987,   600,   609,   602,   603,   607,   225,   614,   611,   612,
    -987,  -987, 17783,  -987,  -987,  -987,  -987,  -987,  -987,  -987,
    -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,
    -987,  -987, 10259,  -987,   618,   621,   615,   565,   565,  -987,
    -987,  -987,   512,   251,   262, 16621, 11555, 11717, 16653, 11879,
   12041, 12203, 12365,  -987,  -987,   595,  -987,   598,  -987,  -987,
    4590, 10259, 17329, 10259, 17329,  7527,  -987,  4590, 10259, 17329,
    -987, 10259, 17329,  -987, 16710, 17329,  -987, 17329,   722,  4590,
     568,  -987,   604,  9769,   163,  -987,    37,  -987,  -987,  9117,
   13439,  7853,  7853,  4590,  -987,    46,   608, 10259,  -987, 10259,
    -987, 17329,  4590, 10259,  -987, 17329,  4590, 17329,  -987,  -987,
    -987,  -987,  1872,  -987, 15645,  -987, 17329,  -987,   142,   545,
    -987,  -987, 13954,  -987, 13666,  -987,  -987,  -987, 10259, 10259,
   10259, 10259, 10259, 10259, 10259, 10259,  -987,  1801,  -987,  2711,
   16260, 14280, 14356,  -987, 16260, 14432, 14508, 10259,  4590,  -987,
     512,   568,  6541,  -987,  -987,  -987,   221,  9443,  -987,  -987,
     148, 10259,   -13,  2938,  -987,   786,   616,   617,   411,  -987,
     357, 17329, 14584,  -987, 14660,  -987,  -987,  -987, 12527,   667,
     268,  -987,   635,   636,   565,   565, 16797, 16873, 16949, 17025,
   17101, 17177, 16034,  -987, 17540,  -987,  4590,  4590,  4590,  4590,
   17329,  -987,  -987,   163,  9933,    78,  -987, 17329,  -987,    47,
    -987,   108,  -987,   535, 17253,  -987,  -987,  -987, 12365,   619,
     620,  -987,  4590,  4590,  3277,  -987,  -987,   160,  -987,    37,
    -987,  -987,  -987, 10259, 10259, 10259, 10259, 10259, 10259,  -987,
    -987, 16260, 16260, 16260, 16260,  -987,  -987,  -987,  -987,  -987,
     249,  9117, 13015,  -987, 10259,   148,    47,    47,    47,    47,
      47,    47,   148,  1124,  -987,  -987, 16260, 16260,   622, 12689,
      91,   121, 13722,  -987,  -987, 17329,  -987,  -987,  -987,  -987,
    -987,  -987,  -987,   633,  -987,  -987,   341, 13179,  -987,  -987,
    -987, 10097,  -987,   497,  -987
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -987,  -987,  -987,     4,  -429,  2191,  -987,  -987,  -987,  -987,
     242,   455,  -406,   113,   116,  -987,  -987,  -987,   161,   766,
    -987,   784,  -181,  -659,  -987,  -830,   726,  -928,  -748,  -987,
     -54,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,   147,
    -987,  -987,  -987,   500,  -987,    17,  -987,  -987,  -987,  -987,
    -987,  -987,  -549,  -798,  -987,  -987,  -987,     6,  1056,  -987,
    -987,  -987,   153,  -987,  -987,  -987,  -987,  -128,  -147,  -801,
    -987,  -118,    58,   211,  -987,  -987,  -987,    39,  -987,  -987,
    -255,   -73,  -179,  -206,  -233,  -225,  -687,  -987,  -180,  -987,
       7,   811,  -124,   403,  -987,  -443,  -740,  -674,  -987,  -587,
    -468,  -986,  -906,  -860,   -43,  -987,    40,  -987,  -202,  -987,
     274,   511,  -353,  -987,  -987,  -987,  1253,  -987,   -10,  -987,
    -987,  -198,  -987,  -583,  -987,  -987,  -987,  1567,  1681,   -12,
     827,    63,   942,  -987,  1763,  1995,  -987,  -987,  -987,  -987,
    -987,  -987,  -987,  -987,  -385
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -570
static const yytype_int16 yytable[] =
{
     163,   627,   383,   565,   184,   269,   102,   653,   189,   188,
     399,   333,   621,   734,   196,   432,   439,   405,   201,   201,
     273,   211,   213,   216,   217,   897,   417,   314,   607,   191,
     960,   845,   253,   647,   254,   721,   448,   405,   724,   259,
     250,   260,   765,   261,  1026,   925,   935,   936,   773,    23,
    1018,   274,   278,   280,   282,   283,   284,   250,   319,   736,
     288,   458,   289,   963,   292,   296,   729,   400,   947,   299,
     949,    64,   300,   301,   451,   302,  1083,   303,   250,   426,
     304,   305,   405,   306,   469,   426,   448,   274,   278,   320,
     774,   330,   970,    54,   402,   571,   637,   497,   344,  -208,
       3,  1013,   344,   483,    62,   405,   350,   348,  1059,   384,
     350,   385,   937,   403,   353,  1104,   852,   331,   851,   899,
     491,   495,   654,  1061,   448,   406,   498,   279,   484,    82,
    -390,   492,   486,   337,   490,   394,   571,   426,     5,   426,
     784,  -208,  1020,   987,   775,   406,   383,   971,  1002,   395,
    1004,  1018,  -390,  1026,   969,   525,  1084,   163,  1035,   487,
     391,   392,   393,   317,   288,   274,   320,   452,   526,   529,
     395,   865,   449,  -390,   867,   896,   986,   161,  -390,   398,
     210,   212,   551,   552,   461,   764,   278,   395,   493,   395,
     406,   699,   527,  1041,  1042,  -208,   395,  -385,   426,   925,
    -390,   925,  -390,   405,   405,    64,   278,  -390,   395,   414,
     459,   565,   493,   406,   419,  -386,   493,   572,  1064,   255,
    1012,   191,   449,  -390,   493,   751,  -385,   -81,   700,   493,
      73,   164,  1019,  -390,   488,   -87,   528,   291,   295,  -385,
     461,   900,  -390,  1020,  -386,  1062,  1065,   614,   318,   -45,
     701,   278,  -385,   494,   925,   201,   925,  -386,  1097,   462,
     449,   489,   752,   463,   464,   615,   649,   -45,   616,   796,
    -386,   256,   607,  1056,   278,   845,   461,   702,  1098,   988,
     638,   452,   754,   395,   504,  -390,   251,   405,   617,   257,
     585,   618,   953,   322,   797,   589,   -63,   167,   192,  -385,
     510,   485,   619,   727,  -385,   967,   258,   262,   251,   755,
     285,   406,   406,   892,   482,   505,   565,  -386,   290,   294,
     198,   620,  -386,   218,   511,   621,   512,   513,   514,   516,
     518,   519,  1063,   520,   521,   522,   523,   524,  -408,   530,
     531,   532,   533,   534,   535,   536,   537,   538,   539,   540,
     541,   542,   543,   544,   545,   413,  -544,   -83,  -544,  -408,
     418,   278,   278,  -408,   924,   -88,  -400,   278,   263,   286,
     278,   278,   278,   278,   554,   556,   564,   875,   591,  1087,
    1088,  1089,  1090,  1091,  1092,   574,  -408,  -400,   645,  -496,
     578,  -400,   579,   161,   386,   406,  -378,   324,   387,   480,
    -495,   405,   738,   741,   876,  1095,   754,   327,   325,   274,
     645,   593,   595,   314,  -400,   314,  -496,   599,   601,   334,
     645,   606,   606,  1102,   610,  -378,   612,  -495,    23,   161,
     396,   452,   974,  1040,  -479,   673,   546,   547,  -378,   640,
    -478,   461,   548,   642,   643,   549,   550,    23,    39,   646,
     577,  -378,   251,   251,   251,   251,   278,   705,   452,   437,
     676,   387,  -541,   679,  -541,   335,   785,   787,   452,   336,
      52,   646,    54,  -356,   278,   782,   278,   722,   388,  -483,
     665,   646,   668,    62,   670,   672,  -484,   725,   515,   517,
     565,    54,  -356,   685,   274,   251,   340,  -543,  -378,  -543,
     621,   251,    62,  -378,  -481,  -482,   482,   717,    82,   406,
    -480,   390,  -544,   137,  -544,  -542,   401,  -542,   924,   395,
     924,   190,   404,   137,   251,   411,   659,    82,   661,   415,
     421,   441,   758,   553,   555,   340,   137,   686,   137,   455,
     966,   456,   251,   848,   573,   465,   341,   975,   687,   317,
     342,   317,  -380,   468,   593,   708,   599,   711,   610,   713,
     668,   475,   477,   714,   715,   499,   503,   688,   278,   506,
     278,   594,   614,   924,   508,   924,   759,   600,   726,   350,
     728,  -380,   576,   137,   564,   341,   735,   760,   580,   342,
     615,   767,   581,   616,  -380,   582,   590,   344,   345,   588,
     346,   347,   348,   611,   349,   350,   761,  -380,   613,   353,
     137,   596,   614,   617,   137,   628,   618,   360,    56,   629,
     284,   288,   320,   364,   365,   366,   278,   619,   644,   638,
     615,   651,   650,   616,   655,  1037,   344,   663,   680,   664,
     251,   348,   669,   718,   350,   691,   620,   692,   353,   693,
     694,   695,  1066,   617,  -380,   696,   618,   340,    56,  -380,
     697,  1060,   742,   -59,   746,   606,   606,   619,   698,   461,
     251,   743,   745,   763,   251,  1073,   137,   795,  1067,   772,
    1068,  1069,   777,  -290,  1070,  1071,   620,   798,   794,   564,
     856,   850,   516,   554,   855,   340,   861,   858,   859,   976,
     977,   769,   866,   868,   889,   893,   895,   341,   901,  1060,
    -290,   342,   902,   898,   707,   907,   710,   872,   918,   874,
     405,   405,   912,   916,   879,   920,  1096,   882,   919,   929,
     884,   905,   885,  -108,  -114,   887,  -113,  -112,   -84,  -109,
    -116,   -82,  -117,  -111,  1103,   341,   -85,  -115,  1060,   342,
     933,  -110,  -118,   -86,   137,   934,   958,   946,   344,   345,
     948,   137,   274,   348,  1038,   349,   350,   -81,   -83,   961,
     353,  1065,   911,   980,   915,  1074,  1075,   917,   360,   502,
     343,  1029,  1030,   926,   364,   365,   366,   995,  -391,  1094,
     193,   791,   281,   790,   473,   906,   344,   345,  -391,   346,
     347,   348,   927,   349,   350,   351,   994,   352,   353,   354,
     355,   990,   891,   357,   358,   359,   360,   361,   362,   363,
     964,   137,   364,   365,   366,   243,  1100,  -391,   406,   406,
     932,  -395,   753,  1081,  1055,   137,   251,   251,   928,  1093,
    1086,  -395,   251,   251,   872,   874,   215,   879,   882,   911,
     915,   515,   553,  -391,   750,   641,   719,   202,   950,   951,
    -391,   952,   297,   564,     0,   954,   955,     0,   269,   956,
    -395,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -391,     0,     0,     0,     0,     0,   973,   668,   278,
     278,     0,     0,   779,  1016,   981,  -395,   982,     0,     0,
    -391,   984,     0,  -395,     0,  -391,   137,     0,  -391,     0,
       0,     0,   328,     0,   329,     0,     0,     0,     0,  1015,
       0,     0,     0,     0,  -395,     0,   950,   996,   997,   954,
     998,   999,  1000,  1001,   137,     0,   422,   425,   427,   431,
     137,  1057,     0,  -395,   149,  1010,     0,   137,  -395,     0,
     320,  -395,     0,     0,   149,  1017,   251,   251,     0,  1024,
       0,     0,     0,     0,     0,     0,     0,   149,   251,   149,
     437,     0,     0,     0,   297,     0,     0,   437,     0,   454,
     251,     0,     0,   251,     0,   457,     0,     0,     0,  1057,
     297,  1080,   420,     0,  1051,  1052,  1053,  1054,     0,     0,
       0,   137,     0,     0,     0,   137,     0,     0,   467,     0,
       0,     0,     0,   137,   149,     0,   915,     0,   293,   293,
    1076,  1077,   320,   908,     0,     0,   478,     0,  1057,     0,
     445,  1051,  1052,  1053,  1054,  1076,  1077,     0,     0,     0,
       0,   149,     0,     0,     0,   149,     0,     0,     0,  1082,
     668,     0,  1085,     0,     0,   460,     0,     0,   131,     0,
       0,     0,     0,     0,     0,     0,     0,   792,   131,     0,
       0,     0,     0,     5,   703,     0,     0,     0,     0,     0,
       0,   131,   340,   131,     0,   668,     0,    15,  -220,   915,
      17,     0,  -220,  -220,     0,    21,     0,     0,     0,     0,
       0,  -220,     0,  -220,  -220,     0,     0,   149,  -220,     0,
       0,     0,     0,  -220,    37,     0,  -220,     0,     0,    40,
       0,     0,   137,     0,   575,     0,     0,    45,   131,     0,
       0,     0,   341,     0,    50,  -220,   342,  -220,     0,  -220,
       0,  -220,  -220,     0,  -220,     0,  -220,   251,  -220,    60,
       0,     0,     0,     0,   597,   131,     0,     0,   602,   131,
       0,    70,     0,     0,     0,    73,     0,  -220,     0,  -397,
    -220,     0,    79,  -220,   137,     0,     0,     0,    83,  -397,
     137,     0,     0,   344,   345,   149,   346,   347,   348,     0,
     349,   350,   149,   437,   437,   353,   584,   437,   437,     0,
     587,   849,   359,   360,     0,     0,   363,     0,  -397,   364,
     365,   366,     0,     0,  1025,   853,   854,     0,     0,     0,
       0,   131,  1031,  -220,   437,     0,   437,     0,   445,  -220,
       0,     0,     0,     0,  -397,     0,     0,     0,     0,     0,
     137,  -397,   137,     0,     0,     0,     0,   137,     0,     0,
     137,     0,   149,     0,   265,   142,     0,   137,    22,    23,
     137,     0,  -397,     0,     0,   142,   149,   266,     0,    30,
     267,     0,     0,     0,    35,     0,     0,     0,   142,    39,
     142,  -397,     0,     0,   137,     0,  -397,   675,     0,  -397,
       0,     0,   682,     0,     0,   137,   340,   137,     0,   131,
       0,    52,     0,    54,     0,    56,   131,   921,     0,     0,
     922,     0,   268,     0,    62,     0,     0,     0,     0,     0,
     730,   732,     0,  1025,     0,   142,   737,   740,     0,     0,
       0,     0,     0,    78,     0,     0,    80,   149,     0,    82,
       0,     0,     0,     0,     0,     0,   341,     0,     0,     0,
     342,     0,   142,     0,     0,     0,   142,     0,     0,     0,
       0,     0,     0,     0,     0,   149,   131,   720,     0,     0,
     723,   149,     0,     0,     0,     0,     0,     0,   149,     0,
     131,   137,     0,     0,     0,     0,     0,     0,   137,    97,
       0,     0,     0,     0,     0,   923,     0,   344,     0,     0,
     137,     0,   348,   340,   349,   350,     0,     0,   757,   353,
       0,     0,   445,     0,   137,     0,     0,   360,   142,     0,
       0,     0,     0,   137,     0,   366,     0,   137,     0,     0,
       0,     0,   149,     0,     0,   780,   149,     0,     0,   297,
     863,   864,     0,     0,   149,     0,     0,     0,     0,     0,
       0,   131,   869,   341,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,   880,     0,     0,   883,     0,   137,
       0,     0,     0,     0,   844,     0,     0,     0,     0,   131,
       0,     0,     0,     0,     0,   131,     0,     0,     0,     0,
       0,     0,   131,     0,     0,     0,   142,     0,     0,     0,
       0,     0,     0,   142,   344,   345,     0,   346,   347,   348,
       0,   349,   350,   351,     0,     0,   353,   137,   137,   137,
     137,     0,     0,   359,   360,     0,     0,   363,     0,     0,
     364,   365,   366,     0,     0,     0,     0,   890,     0,     0,
     445,     0,   219,   137,   137,     0,   131,     0,     0,     0,
     131,     0,     0,   149,     0,     0,   220,   221,   131,   222,
       0,     0,   780,   142,   223,     0,     0,     0,     0,   146,
       0,   340,   224,     0,     0,     0,     0,   142,   225,   146,
     682,     0,     0,   226,     0,   227,     0,     0,   228,     0,
       0,     0,   146,     0,   146,     0,   229,     0,     0,     0,
       0,     0,   230,   231,     0,   149,   931,     0,   340,   232,
       0,   149,     0,     0,     0,     0,     0,     0,   233,     0,
       0,   341,     0,     0,     0,   342,     0,   234,   235,     0,
     236,   979,   237,     0,   238,     0,     0,   239,     0,   146,
       0,   240,   443,     0,   241,     0,     0,   242,   142,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   341,     0,
       0,     0,   342,     0,     0,     0,   146,   131,     0,     0,
     146,   149,   344,   149,     0,     0,   142,   348,   149,   349,
     350,   149,   142,   147,   353,     0,     0,     0,   149,   142,
       0,   149,   360,   147,     0,     0,     0,     0,   364,   365,
     366,     0,     0,   444,     0,     0,   147,     0,   147,   344,
     345,     0,     0,   347,   348,   149,   349,   350,   844,   131,
       0,   353,     0,     0,     0,   131,   149,     0,   149,   360,
       0,     0,   146,     0,     0,   364,   365,   366,     0,     0,
       0,     0,     0,   142,     0,     0,     0,   142,     0,     0,
       0,     0,     0,   147,     0,   142,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   151,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   151,     0,     0,     0,     0,
     147,     0,     0,     0,   147,   131,     0,   131,   151,     0,
     151,     0,   131,     0,     0,   131,     0,     0,     0,     0,
       0,     0,   131,     0,     0,   131,     0,     0,     0,     0,
     146,     0,   149,     0,     0,     0,     0,   146,     0,   149,
       0,     0,     0,   265,     0,     0,     0,    22,    23,   131,
       0,   149,     0,     0,     0,   151,   266,     0,    30,   267,
     131,     0,   131,    35,     0,   149,   147,     0,    39,     0,
       0,     0,     0,     0,   149,     0,     0,     0,   149,     0,
       0,     0,   151,     0,   142,     0,   151,     0,     0,     0,
      52,     0,    54,     0,    56,     0,   921,   146,     0,   922,
       0,   268,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   146,     0,     0,   265,     0,     0,     0,    22,    23,
     149,     0,    78,     0,     0,    80,     0,   266,    82,    30,
     267,     0,     0,     0,    35,     0,   142,     0,     0,    39,
       0,     0,   142,     0,   147,     0,   131,     0,   151,     0,
       0,   147,     0,   131,     0,     0,     0,     0,     0,     0,
       0,    52,     0,    54,     0,   131,     0,     0,   149,   149,
     149,   149,   268,     0,    62,     0,     0,     0,    97,   131,
       0,     0,   146,     0,  1003,     0,     0,     0,   131,     0,
       0,     0,   131,    78,   149,   149,    80,     0,     0,    82,
       0,     0,   142,     0,   142,     0,     0,     0,     0,   142,
     146,   147,   142,     0,     0,     0,   146,   152,     0,   142,
       0,     0,   142,   146,     0,   147,   151,   152,     0,     0,
       0,     0,     0,   151,   131,     0,     0,     0,     0,     0,
     152,     0,   152,     0,     0,     0,   142,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   219,   142,     0,   142,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     220,   221,     0,   222,     0,     0,     0,   146,   223,     0,
       0,   146,   131,   131,   131,   131,   224,   152,     0,   146,
       0,     0,   225,   151,     0,     0,   147,   226,     0,   227,
       0,     0,   228,     0,     0,     0,     0,   151,   131,   131,
     229,     0,     0,     0,   152,     0,   230,   231,   152,     0,
       0,     0,     0,   232,   147,     0,     0,     0,     0,     0,
     147,     0,   233,     0,     0,     0,     0,   147,     0,     0,
       0,   234,   235,   142,   236,     0,   237,     0,   238,     0,
     142,   239,     0,     0,     0,   240,     0,     0,   241,     0,
       0,   242,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   142,     0,   151,     0,
     152,     0,     0,     0,     0,   142,     0,     0,     0,   142,
       0,   147,     0,     0,     0,   147,     0,     0,   146,     0,
       0,     0,     0,   147,     0,     0,   151,     0,     0,     0,
       0,     0,   151,   104,     0,   395,     0,   778,     0,   151,
       0,     0,     0,   162,     0,     0,     0,     0,     0,     0,
       0,   142,     0,     0,     0,     0,   194,     0,   197,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     146,     0,     0,     0,     0,     0,   146,     0,   152,     0,
       0,     0,     0,     0,     0,   152,     0,     0,     0,     0,
       0,     0,     0,   151,     0,     0,     0,   151,     0,   142,
     142,   142,   142,   287,     0,   151,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   142,   142,   219,     0,     0,
     104,     0,   147,     0,   323,     0,   146,     0,   146,     0,
       0,   220,   221,   146,   222,   152,   146,     0,     0,   223,
       0,     0,     0,   146,     0,     0,   146,   224,     0,   152,
       0,     0,     0,   225,     0,     0,     0,     0,   226,     0,
     227,     0,     0,   228,     0,     0,     0,     0,     0,     0,
     146,   229,     0,     0,   147,     0,     0,   230,   231,     0,
     147,   146,     0,   146,   232,     0,   389,     0,     0,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,     0,   234,   235,   151,   236,     0,   237,     0,   238,
       0,     0,   239,     0,     0,     0,   240,     0,     0,   241,
     152,     0,   242,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     147,     0,   147,     0,     0,     0,     0,   147,   152,     0,
     147,     0,     0,     0,   152,     0,   151,   147,     0,     0,
     147,   152,   151,     0,   440,     0,     0,   146,     0,     0,
       0,   453,     0,     0,   146,     0,     0,     0,   246,     0,
       0,     0,     0,     0,   147,     0,   146,     0,     0,     0,
       0,     0,     0,     0,     0,   147,     0,   147,     0,     0,
     146,     0,     0,     0,     0,     0,     0,     0,     0,   146,
       0,     0,     0,   146,     0,   152,     0,     0,     0,   152,
       0,     0,   151,     0,   151,     0,     0,   152,     0,   151,
       0,   104,   151,     0,     0,     0,     0,     0,     0,   151,
       0,     0,   151,     0,     0,   104,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   146,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   151,     0,     0,     0,
       0,     0,   219,     0,     0,     0,     0,   151,     0,   151,
       0,   147,     0,     0,     0,     0,   220,   221,   147,   222,
       0,     0,     0,     0,   223,     0,     0,     0,     0,     0,
     147,     0,   224,   146,   146,   146,   146,     0,   225,     0,
       0,     0,     0,   226,   147,   227,   104,     0,   228,     0,
       0,     0,     0,   147,     0,     0,   229,   147,     0,   146,
     146,     0,   230,   231,     0,     0,   152,     0,     0,   232,
       0,     0,     0,     0,   453,     0,     0,     0,   233,     0,
     453,     0,     0,     0,     0,     0,     0,   234,   235,     0,
     236,     0,   237,   151,   238,     0,     0,   239,     0,   147,
     151,   240,     0,     0,   241,     0,     0,   242,     0,     0,
       0,     0,   151,     0,     0,     0,     0,     0,   152,     0,
       0,     0,     0,     0,   152,     0,   151,     0,     0,     0,
       0,     0,     0,     0,     0,   151,     0,     0,     0,   151,
       0,   662,     0,     0,     0,   667,     0,   147,   147,   147,
     147,     0,     0,   104,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   756,   338,     0,     0,     0,     0,     0,
       0,   339,     0,   147,   147,     0,     0,     0,     0,     0,
       0,   151,     0,   340,   152,     0,   152,     0,     0,     0,
       0,   152,     0,   265,   152,     0,     0,    22,    23,     0,
       0,   152,     0,     0,   152,     0,   266,     0,    30,   267,
       0,     0,     0,    35,     0,     0,     0,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   152,   151,
     151,   151,   151,   341,     0,     0,     0,   342,     0,   152,
      52,   152,    54,     0,    56,     0,   921,     0,     0,   922,
       0,   268,     0,    62,     0,   151,   151,     0,     0,     0,
       0,     0,   748,     0,   426,     0,     0,     0,   343,     0,
       0,     0,    78,     0,     0,    80,     0,     0,    82,     0,
       0,     0,     0,     0,   344,   345,     0,   346,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,   355,   356,
       0,   357,   358,   359,   360,   361,   362,   363,     0,     0,
     364,   365,   366,     0,   783,     0,     0,     0,     0,   786,
     788,     0,     0,     0,     0,   152,     0,     0,    97,     0,
       0,     0,   152,     0,  1005,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   152,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   152,     0,
       0,     0,     0,     0,     0,     0,     0,   152,     0,   338,
       0,   152,     0,     0,     0,     0,   339,     0,     0,     0,
     453,     0,   453,     0,     0,     0,     0,   453,   340,     0,
     453,     0,     0,     0,     0,     0,     0,   886,     0,     0,
     888,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     338,     0,     0,   152,     0,     0,     0,   339,     0,     0,
       0,     0,     0,     0,   904,     0,     0,     0,     0,   340,
       0,     0,     0,     0,     0,   910,     0,   914,   341,     0,
       0,     0,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   152,   152,   152,   152,     0,     0,     0,     0,     0,
       0,     0,     0,   343,     0,     0,     0,     0,     0,   341,
       0,     0,     0,   342,     0,     0,     0,   152,   152,   344,
     345,     0,   346,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,   355,   356,     0,   357,   358,   359,   360,
     361,   362,   363,     0,   343,   364,   365,   366,     0,     0,
       0,     0,     0,     0,   862,     0,     0,     0,     0,     0,
     344,   345,     0,   346,   347,   348,     0,   349,   350,   351,
     959,   352,   353,   354,   355,   356,     0,   357,   358,   359,
     360,   361,   362,   363,   978,     0,   364,   365,   366,     0,
       0,     0,     0,   983,     0,  1028,     0,   985,     0,     0,
       0,     0,     0,    -2,     4,     0,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,  1011,
      30,    31,    32,    33,    34,    35,    36,    37,     0,    38,
      39,     0,    40,  -234,     0,    41,    42,    43,     0,    44,
      45,    46,   -43,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,    56,    57,    58,  -234,
     -43,    59,    60,    61,     0,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
       0,    75,    76,    77,    78,    79,     0,    80,    81,   -61,
      82,    83,     0,     0,    84,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,     0,   100,  1078,   101,
       5,   270,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   169,     0,     0,    15,    16,     0,    17,     0,   170,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   171,     0,     0,     0,    32,   172,   173,     0,
     174,    37,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   175,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     176,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,   271,     0,     0,     0,    98,     0,   177,
       0,   100,     0,   178,  1079,     4,     0,     5,     0,     6,
       7,     8,     9,    10,    11,     0,  -569,     0,    12,    13,
      14,    15,    16,  -569,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,  -569,    27,    28,  -569,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,     0,
      38,    39,     0,    40,  -234,     0,    41,    42,    43,     0,
      44,    45,    46,   -43,    47,    48,     0,    49,    50,    51,
       0,     0,     0,    52,    53,    54,    55,     0,    57,    58,
    -234,   -43,    59,    60,    61,  -569,    62,    63,    64,  -569,
      65,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      74,     0,    75,    76,    77,    78,    79,     0,    80,    81,
     -61,    82,    83,     0,     0,    84,     0,    85,     0,     0,
    -569,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -569,  -569,    89,  -569,
    -569,  -569,  -569,  -569,  -569,  -569,     0,  -569,  -569,  -569,
    -569,  -569,     0,  -569,  -569,  -569,  -569,  -569,  -569,  -569,
    -569,    97,  -569,  -569,  -569,     0,    99,  -569,   100,   307,
     101,     5,   270,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,     0,    38,    39,     0,    40,  -234,     0,
      41,    42,    43,     0,    44,    45,    46,   -43,    47,    48,
       0,    49,    50,    51,     0,     0,     0,    52,    53,    54,
      55,    56,    57,    58,  -234,   -43,    59,    60,    61,     0,
      62,    63,    64,     0,    65,    66,    67,    68,    69,    70,
      71,    72,     0,    73,    74,     0,    75,    76,    77,    78,
      79,     0,    80,    81,   -61,    82,    83,     0,     0,    84,
       0,    85,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,   271,    97,     0,     0,    98,     0,
      99,   308,   100,     4,   101,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,     0,    38,    39,
       0,    40,  -234,     0,    41,    42,    43,     0,    44,    45,
      46,   -43,    47,    48,     0,    49,    50,    51,     0,     0,
       0,    52,    53,    54,    55,    56,    57,    58,  -234,   -43,
      59,    60,    61,     0,    62,    63,    64,     0,    65,    66,
      67,    68,    69,    70,    71,    72,     0,    73,    74,     0,
      75,    76,    77,    78,    79,     0,    80,    81,   -61,    82,
      83,     0,     0,    84,     0,    85,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,    97,
       0,     0,    98,     0,    99,   481,   100,   500,   101,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,    39,     0,    40,  -234,     0,    41,    42,
      43,     0,    44,    45,    46,   -43,    47,    48,     0,    49,
      50,    51,     0,     0,     0,    52,    53,    54,    55,    56,
      57,    58,  -234,   -43,    59,    60,    61,     0,    62,    63,
      64,     0,    65,    66,    67,    68,    69,    70,    71,    72,
       0,    73,    74,     0,    75,    76,    77,    78,    79,     0,
      80,    81,   -61,    82,    83,     0,     0,    84,     0,    85,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,    97,     0,     0,    98,     0,    99,   501,
     100,   307,   101,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,     0,    38,    39,     0,    40,
    -234,     0,    41,    42,    43,     0,    44,    45,    46,   -43,
      47,    48,     0,    49,    50,    51,     0,     0,     0,    52,
      53,    54,    55,    56,    57,    58,  -234,   -43,    59,    60,
      61,     0,    62,    63,    64,     0,    65,    66,    67,    68,
      69,    70,    71,    72,     0,    73,    74,     0,    75,    76,
      77,    78,    79,     0,    80,    81,   -61,    82,    83,     0,
       0,    84,     0,    85,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,    97,     0,     0,
      98,     0,    99,   308,   100,     4,   101,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,     0,
      38,    39,     0,    40,  -234,     0,    41,    42,    43,     0,
      44,    45,    46,   -43,    47,    48,     0,    49,    50,    51,
       0,     0,     0,    52,    53,    54,    55,    56,    57,    58,
    -234,   -43,    59,    60,    61,     0,    62,    63,    64,     0,
      65,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      74,     0,    75,    76,    77,    78,    79,     0,    80,    81,
     -61,    82,    83,     0,     0,    84,     0,    85,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,    97,     0,     0,    98,     0,    99,   674,   100,     4,
     101,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,     0,    38,    39,     0,    40,  -234,     0,
      41,    42,    43,     0,    44,    45,    46,   -43,    47,    48,
       0,    49,    50,    51,     0,     0,     0,    52,    53,    54,
      55,   322,    57,    58,  -234,   -43,    59,    60,    61,     0,
      62,    63,    64,     0,    65,    66,    67,    68,    69,    70,
      71,    72,     0,    73,    74,     0,    75,    76,    77,    78,
      79,     0,    80,    81,   -61,    82,    83,     0,     0,    84,
       0,    85,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,    97,     0,     0,    98,     0,
      99,     4,   100,     5,   101,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,     0,    38,    39,     0,    40,
    -234,     0,    41,    42,    43,     0,    44,    45,    46,   -43,
      47,    48,     0,    49,    50,    51,     0,     0,     0,    52,
      53,    54,    55,     0,    57,    58,  -234,   -43,    59,    60,
      61,     0,    62,    63,    64,     0,    65,    66,    67,    68,
      69,    70,    71,    72,     0,    73,    74,     0,    75,    76,
      77,    78,    79,     0,    80,    81,   -61,    82,    83,     0,
       0,    84,     0,    85,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,    97,     0,     0,
      98,     0,    99,     4,   100,     5,   101,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,   592,    37,     0,    38,    39,
       0,    40,  -234,     0,    41,    42,    43,     0,    44,    45,
      46,   -43,    47,    48,     0,    49,    50,    51,     0,     0,
       0,    52,    53,    54,    55,     0,    57,    58,  -234,   -43,
      59,    60,    61,     0,    62,    63,    64,     0,    65,    66,
      67,    68,    69,    70,    71,    72,     0,    73,    74,     0,
      75,    76,    77,    78,    79,     0,    80,    81,   -61,    82,
      83,     0,     0,    84,     0,    85,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,    97,
       0,     0,    98,     0,    99,     4,   100,     5,   101,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,   598,    37,     0,
      38,    39,     0,    40,  -234,     0,    41,    42,    43,     0,
      44,    45,    46,   -43,    47,    48,     0,    49,    50,    51,
       0,     0,     0,    52,    53,    54,    55,     0,    57,    58,
    -234,   -43,    59,    60,    61,     0,    62,    63,    64,     0,
      65,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      74,     0,    75,    76,    77,    78,    79,     0,    80,    81,
     -61,    82,    83,     0,     0,    84,     0,    85,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,    97,     0,     0,    98,     0,    99,     4,   100,     5,
     101,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,   871,
      37,     0,    38,    39,     0,    40,  -234,     0,    41,    42,
      43,     0,    44,    45,    46,   -43,    47,    48,     0,    49,
      50,    51,     0,     0,     0,    52,    53,    54,    55,     0,
      57,    58,  -234,   -43,    59,    60,    61,     0,    62,    63,
      64,     0,    65,    66,    67,    68,    69,    70,    71,    72,
       0,    73,    74,     0,    75,    76,    77,    78,    79,     0,
      80,    81,   -61,    82,    83,     0,     0,    84,     0,    85,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,    97,     0,     0,    98,     0,    99,     4,
     100,     5,   101,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,   873,    37,     0,    38,    39,     0,    40,  -234,     0,
      41,    42,    43,     0,    44,    45,    46,   -43,    47,    48,
       0,    49,    50,    51,     0,     0,     0,    52,    53,    54,
      55,     0,    57,    58,  -234,   -43,    59,    60,    61,     0,
      62,    63,    64,     0,    65,    66,    67,    68,    69,    70,
      71,    72,     0,    73,    74,     0,    75,    76,    77,    78,
      79,     0,    80,    81,   -61,    82,    83,     0,     0,    84,
       0,    85,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,    97,     0,     0,    98,     0,
      99,     4,   100,     5,   101,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,   878,    37,     0,    38,    39,     0,    40,
    -234,     0,    41,    42,    43,     0,    44,    45,    46,   -43,
      47,    48,     0,    49,    50,    51,     0,     0,     0,    52,
      53,    54,    55,     0,    57,    58,  -234,   -43,    59,    60,
      61,     0,    62,    63,    64,     0,    65,    66,    67,    68,
      69,    70,    71,    72,     0,    73,    74,     0,    75,    76,
      77,    78,    79,     0,    80,    81,   -61,    82,    83,     0,
       0,    84,     0,    85,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,    97,     0,     0,
      98,     0,    99,     4,   100,     5,   101,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,   881,    37,     0,    38,    39,
       0,    40,  -234,     0,    41,    42,    43,     0,    44,    45,
      46,   -43,    47,    48,     0,    49,    50,    51,     0,     0,
       0,    52,    53,    54,    55,     0,    57,    58,  -234,   -43,
      59,    60,    61,     0,    62,    63,    64,     0,    65,    66,
      67,    68,    69,    70,    71,    72,     0,    73,    74,     0,
      75,    76,    77,    78,    79,     0,    80,    81,   -61,    82,
      83,     0,     0,    84,     0,    85,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,    97,
       0,     0,    98,     0,    99,     4,   100,     5,   101,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,   903,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,     0,
      38,    39,     0,    40,  -234,     0,    41,    42,    43,     0,
      44,    45,    46,   -43,    47,    48,     0,    49,    50,    51,
       0,     0,     0,    52,    53,    54,    55,     0,    57,    58,
    -234,   -43,    59,    60,    61,     0,    62,    63,    64,     0,
      65,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      74,     0,    75,    76,    77,    78,    79,     0,    80,    81,
     -61,    82,    83,     0,     0,    84,     0,    85,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,    97,     0,     0,    98,     0,    99,     4,   100,     5,
     101,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,   909,
      37,     0,    38,    39,     0,    40,  -234,     0,    41,    42,
      43,     0,    44,    45,    46,   -43,    47,    48,     0,    49,
      50,    51,     0,     0,     0,    52,    53,    54,    55,     0,
      57,    58,  -234,   -43,    59,    60,    61,     0,    62,    63,
      64,     0,    65,    66,    67,    68,    69,    70,    71,    72,
       0,    73,    74,     0,    75,    76,    77,    78,    79,     0,
      80,    81,   -61,    82,    83,     0,     0,    84,     0,    85,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,    97,     0,     0,    98,     0,    99,     4,
     100,     5,   101,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,   913,    37,     0,    38,    39,     0,    40,  -234,     0,
      41,    42,    43,     0,    44,    45,    46,   -43,    47,    48,
       0,    49,    50,    51,     0,     0,     0,    52,    53,    54,
      55,     0,    57,    58,  -234,   -43,    59,    60,    61,     0,
      62,    63,    64,     0,    65,    66,    67,    68,    69,    70,
      71,    72,     0,    73,    74,     0,    75,    76,    77,    78,
      79,     0,    80,    81,   -61,    82,    83,     0,     0,    84,
       0,    85,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,    97,     0,     0,    98,     0,
      99,     0,   100,   766,   101,     5,   270,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   169,     0,     0,    15,
      16,     0,    17,     0,   170,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   171,     0,     0,
       0,    32,   172,   173,     0,   174,    37,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   175,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   176,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,   271,     0,
       0,     0,    98,     0,   177,     0,   100,     0,   178,     5,
     270,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     169,     0,     0,    15,    16,     0,    17,     0,   170,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   171,     0,     0,     0,    32,   172,   173,     0,   174,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   175,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   176,
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,   271,     0,     0,     0,    98,     0,   177,     0,
     100,     0,   178,   666,     5,   270,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   169,     0,     0,    15,    16,
       0,    17,     0,   170,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   171,     0,     0,     0,
      32,   172,   173,     0,   174,    37,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   175,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   176,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,   271,     0,     0,
       0,    98,     0,   177,     0,   100,     0,   178,  1014,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     169,     0,     0,    15,    16,     0,    17,     0,   170,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   171,     0,     0,     0,    32,   172,   173,     0,   174,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   175,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   176,
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   177,     0,
     100,     0,   178,   671,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   169,     0,     0,    15,    16,
       0,    17,     0,   170,     0,     0,    21,   214,    23,     0,
       0,     0,     0,     0,    28,     0,   171,     0,     0,     0,
      32,   172,   173,     0,   174,    37,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,    54,    55,     0,    57,     0,     0,     0,     0,
      60,   175,     0,    62,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   176,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,    82,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   177,     0,   100,     0,   178,     5,   270,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   169,
       0,     0,    15,    16,     0,    17,     0,   170,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     171,     0,     0,     0,    32,   172,   173,     0,   174,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   175,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   176,     0,
      73,     0,     0,    75,   275,   276,     0,    79,   312,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,   313,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,   271,     0,     0,     0,    98,     0,   177,     0,   100,
       0,   178,     5,   270,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   171,     0,     0,     0,    32,   172,
     173,     0,   174,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   175,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   176,     0,    73,     0,     0,    75,   275,   276,
       0,    79,   312,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,   271,     0,     0,     0,    98,
       0,   177,     0,   100,   658,   178,     5,   270,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   169,     0,     0,
      15,    16,     0,    17,     0,   170,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   171,     0,
       0,     0,    32,   172,   173,     0,   174,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   175,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   176,     0,    73,     0,
       0,    75,   275,   276,     0,    79,   312,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,   271,
       0,     0,     0,    98,     0,   177,     0,   100,   660,   178,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   169,     0,     0,    15,    16,     0,    17,     0,   170,
       0,     0,    21,     0,   557,     0,     0,     0,     0,     0,
      28,     0,   171,     0,     0,     0,    32,   172,   173,     0,
     174,    37,   558,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   175,     0,   559,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     176,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,   560,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   177,
       0,   100,     5,   178,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   171,     0,     0,     0,    32,   172,
     173,     0,   174,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   175,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   176,     0,    73,     0,     0,    75,   275,   276,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,   277,     0,     0,    98,
       0,   177,     0,   100,     0,   178,     5,   270,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   169,     0,     0,
      15,    16,     0,    17,     0,   170,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   171,     0,
       0,     0,    32,   172,   173,     0,   174,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   175,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   176,     0,    73,     0,
       0,    75,   275,   276,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,   271,
       0,     0,     0,    98,     0,   177,     0,   100,     5,   178,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   169,
       0,     0,    15,    16,     0,    17,     0,   170,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     171,     0,     0,     0,    32,   172,   173,     0,   174,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,   768,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   175,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   176,     0,
      73,     0,     0,    75,   275,   276,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,   177,     0,   100,
       5,   178,     6,     7,     8,   203,    10,    11,   204,     0,
       0,   169,     0,     0,    15,    16,     0,    17,     0,   170,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   171,     0,     0,     0,    32,   172,   173,     0,
     174,    37,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   205,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     176,     0,    73,     0,     0,    75,     0,     0,   206,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   177,
       0,   100,     5,   178,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   171,     0,     0,     0,    32,   172,
     173,     0,   174,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
     208,    55,     0,    57,     0,     0,     0,     0,    60,   175,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   176,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,   209,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   177,     0,   100,     0,   178,     5,   270,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   169,     0,     0,
      15,    16,     0,    17,     0,   170,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   171,     0,
       0,     0,    32,   172,   173,     0,   174,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   175,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   176,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,   209,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,   271,
       0,     0,     0,    98,     0,   177,     0,   100,     5,   178,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   169,
       0,     0,    15,    16,     0,    17,     0,   170,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     171,     0,     0,     0,    32,   172,   173,     0,   174,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   175,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   176,     0,
      73,     0,     0,    75,   275,   276,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,   177,     0,   100,
       0,   178,     5,   270,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   171,     0,     0,     0,    32,   172,
     173,     0,   174,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   175,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   176,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   800,     0,
     801,    87,    88,    89,    90,    91,     0,     0,     0,   802,
      92,     0,     0,     0,   803,   221,   804,   805,     0,     0,
      93,     0,   806,    94,    95,   271,     0,     0,     0,    98,
     224,   177,   656,   100,     0,   178,   807,     0,     0,     0,
       0,   808,     0,   227,     0,     0,   809,     0,   810,     0,
       0,     0,     0,     0,   811,     0,     0,     0,     0,     0,
     812,   813,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,     0,   814,     0,     0,     0,
       0,     0,     0,     0,     0,   234,   235,     0,   815,     0,
     237,     0,   816,     0,     0,   817,     0,     0,     0,   818,
       0,     0,   241,     0,     0,   819,     0,     0,     0,     0,
       0,     0,     0,     0,   367,   368,   369,   370,   371,   372,
       0,     0,   375,   376,   377,   378,     0,   380,   381,   820,
     821,   822,   823,   824,     0,     0,   825,     0,     0,     0,
     826,   827,   828,   829,   830,   831,   832,   833,   834,   835,
     836,     0,   837,     0,     0,   838,   839,   840,   841,     0,
       5,   842,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   169,     0,     0,    15,    16,     0,    17,     0,   170,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   171,     0,     0,     0,    32,   172,   173,     0,
     174,    37,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   175,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     176,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,   209,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   177,
       0,   100,     5,   178,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,     0,     0,     0,     0,     0,
       0,   250,    28,     0,   171,     0,     0,     0,    32,   172,
     173,     0,   174,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   175,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   176,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,    99,     0,   100,     0,   178,     5,   270,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   169,     0,     0,
      15,    16,     0,    17,     0,   170,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   171,     0,
       0,     0,    32,   172,   173,     0,   174,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   175,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   176,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,   271,
       0,     0,     0,    98,     0,   177,     0,   100,     5,   178,
       6,     7,     8,   203,    10,    11,     0,     0,     0,   169,
       0,     0,    15,    16,     0,    17,     0,   170,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     171,     0,     0,     0,    32,   172,   173,     0,   174,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   205,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   176,     0,
      73,     0,     0,    75,     0,     0,   206,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,   177,     0,   100,
       0,   178,     5,   270,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,   630,     0,   171,     0,     0,     0,    32,   172,
     173,     0,   174,    37,     0,   631,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   175,
       0,     0,     0,     0,     0,     0,     0,    67,   632,    69,
      70,    71,   633,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,   271,     0,     0,     0,    98,
       0,   177,     0,   100,     0,   962,     5,   270,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   169,     0,     0,
      15,    16,     0,    17,     0,   170,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   171,     0,
       0,     0,    32,   172,   173,     0,   174,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   175,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   176,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,   271,
       0,     0,     0,    98,     0,   177,     0,   100,     0,   962,
       5,   270,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   169,     0,     0,    15,    16,     0,    17,     0,   170,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   171,     0,     0,     0,    32,   172,   173,     0,
     945,    37,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   175,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     176,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,   271,     0,     0,     0,    98,     0,   177,
       0,   100,     5,   178,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   171,     0,     0,     0,    32,   172,
     173,     0,   174,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   175,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   176,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   177,     0,   100,     5,   178,     6,     7,     8,   199,
      10,    11,     0,     0,     0,   169,     0,     0,    15,    16,
       0,    17,     0,   170,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   171,     0,     0,     0,
      32,   172,   173,     0,   174,    37,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   200,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   176,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   177,     0,   100,     5,   178,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   169,     0,     0,
      15,    16,     0,    17,     0,   170,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   171,     0,
       0,     0,    32,   172,   173,     0,   174,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   175,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   176,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,    99,     0,   100,     5,   178,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   169,
       0,     0,    15,    16,     0,    17,     0,   170,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     171,     0,     0,     0,    32,   172,   173,     0,   174,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   175,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   176,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,   603,     0,   177,     0,   100,
       5,   178,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   169,     0,     0,    15,    16,     0,    17,     0,   170,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
     630,     0,   171,     0,     0,     0,    32,   172,   173,     0,
     174,    37,     0,   631,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   175,     0,     0,
       0,     0,     0,     0,     0,    67,   632,    69,    70,    71,
     633,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   177,
       0,   100,     5,   634,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   171,     0,     0,     0,    32,   172,
     173,     0,   174,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   175,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   176,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   177,     0,   100,     5,   634,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   169,     0,     0,    15,    16,
       0,    17,     0,   170,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   171,     0,     0,     0,
      32,   172,   173,     0,   706,    37,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   175,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   176,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   177,     0,   100,     5,   178,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   169,     0,     0,
      15,    16,     0,    17,     0,   170,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   171,     0,
       0,     0,    32,   172,   173,     0,   709,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   175,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   176,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,   177,     0,   100,     5,   178,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   169,
       0,     0,    15,    16,     0,    17,     0,   170,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     171,     0,     0,     0,    32,   172,   173,     0,   939,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   175,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   176,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,   177,     0,   100,
       5,   178,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   169,     0,     0,    15,    16,     0,    17,     0,   170,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   171,     0,     0,     0,    32,   172,   173,     0,
     940,    37,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   175,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     176,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   177,
       0,   100,     5,   178,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   171,     0,     0,     0,    32,   172,
     173,     0,   942,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   175,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   176,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   177,     0,   100,     5,   178,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   169,     0,     0,    15,    16,
       0,    17,     0,   170,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   171,     0,     0,     0,
      32,   172,   173,     0,   943,    37,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   175,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   176,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   177,     0,   100,     5,   178,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   169,     0,     0,
      15,    16,     0,    17,     0,   170,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   171,     0,
       0,     0,    32,   172,   173,     0,   944,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   175,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   176,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,   177,     0,   100,     5,   178,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   169,
       0,     0,    15,    16,     0,    17,     0,   170,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     171,     0,     0,     0,    32,   172,   173,     0,   945,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   175,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   176,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,   177,     0,   100,
       5,   178,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   169,     0,     0,    15,    16,     0,    17,     0,   170,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
     630,     0,   171,     0,     0,     0,    32,   172,   173,     0,
     174,    37,     0,   631,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   175,     0,     0,
       0,     0,     0,     0,     0,    67,   632,    69,    70,    71,
     633,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   177,
       0,   100,     5,  1034,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   171,     0,     0,     0,    32,   172,
     173,     0,   174,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   175,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   176,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,     0,     0,   100,     5,  1034,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   169,     0,     0,    15,    16,
       0,    17,     0,   170,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   171,     0,     0,     0,
      32,   172,   173,     0,   174,    37,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   175,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   176,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,  -503,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,   382,    95,    96,  -541,     0,
    -541,    98,     0,   177,     0,   100,     0,   178,     5,   270,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   169,
       0,     0,    15,    16,     0,    17,     0,   170,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     171,     0,     0,     0,    32,   172,   173,     0,   174,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   175,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   176,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,     0,  -399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,  -399,     0,     0,     0,  -399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   271,     0,     0,     0,     0,     0,   177,     0,   100,
    -399,   962,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   171,     0,     0,     0,    32,   172,
     173,     0,   174,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,  -381,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   175,
       0,     0,     0,  -381,     0,     0,     0,    67,    68,    69,
      70,    71,   176,     0,    73,     0,  -381,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,  -381,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,    89,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,    96,  -381,     0,     0,     0,
       0,  -381,    28,   100,   171,  1034,     0,     0,    32,   172,
     173,     0,   174,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   175,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   176,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,     0,     0,     0,     0,     0,     0,
       0,    87,     5,    89,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,   382,    21,    96,  -541,     0,  -541,     0,
       0,   177,    28,   100,   171,   178,     0,     0,    32,   172,
     173,     0,   174,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   175,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   176,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,    89,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,    96,     0,     0,     0,     0,
       0,   177,    28,   100,   171,   634,     0,     0,    32,   172,
     173,     0,   174,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   175,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   176,     0,    73,     0,     0,    75,     0,   992,
       0,    79,     0,     0,    81,     0,     0,    83,   802,     0,
       0,     0,     0,   220,   221,   804,   222,     0,     0,     0,
       0,   223,     0,     0,     0,     0,     0,     0,     0,   224,
       0,     0,     0,    89,     0,   807,     0,     0,     0,     0,
     226,     0,   227,     0,     0,   228,     0,   810,     0,     0,
       0,     0,     0,   229,     0,     0,     0,     0,     0,   812,
     231,     0,     0,   100,   338,   178,   232,     0,     0,     0,
       0,   339,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   340,   234,   235,     0,   236,     0,   237,
       0,   993,     0,     0,   817,     0,     0,     0,   240,     0,
       0,   241,     0,     0,   242,     0,     0,     0,     0,     0,
       0,     0,     0,   367,   368,   369,   370,   371,   372,     0,
       0,   375,   376,   377,   378,   340,   380,   381,   820,   821,
     822,   823,   824,   341,     0,   825,     0,   342,     0,   826,
     827,   828,   829,   830,   831,   832,   833,   834,   835,   836,
       0,   837,   338,     0,   838,   839,   840,   841,     0,   339,
       0,     0,     0,     0,     0,     0,     0,     0,   343,     0,
       0,   340,     0,     0,     0,   341,     0,     0,     0,   342,
       0,     0,     0,     0,   344,   345,     0,   346,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,   355,   356,
       0,   357,   358,   359,   360,   361,   362,   363,     0,   338,
     364,   365,   366,     0,     0,     0,   339,     0,     0,  1101,
       0,   341,     0,     0,     0,   342,   344,   345,   340,   346,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
     355,     0,     0,   357,   358,   359,   360,   361,     0,   363,
     338,     0,   364,   365,   366,     0,   343,   339,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   340,
       0,     0,   344,   345,     0,   346,   347,   348,   341,   349,
     350,   351,   342,   352,   353,   354,   355,   356,     0,   357,
     358,   359,   360,   361,   362,   363,   338,     0,   364,   365,
     366,     0,     0,   339,     0,   657,     0,     0,     0,     0,
       0,     0,     0,   343,     0,   340,     0,     0,     0,   341,
       0,     0,     0,   342,     0,     0,     0,     0,     0,   344,
     345,     0,   346,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,   355,   356,     0,   357,   358,   359,   360,
     361,   362,   363,     0,   343,   364,   365,   366,     0,     0,
       0,     0,   770,     0,     0,   341,     0,     0,     0,   342,
     344,   345,     0,   346,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,   355,   356,     0,   357,   358,   359,
     360,   361,   362,   363,   338,     0,   364,   365,   366,     0,
     343,   339,     0,   771,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   340,     0,     0,   344,   345,     0,   346,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
     355,   356,     0,   357,   358,   359,   360,   361,   362,   363,
       0,     0,   364,   365,   366,     0,     0,     0,     0,   991,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   341,     0,     0,     0,   342,     0,     0,
     338,     0,     0,     0,     0,     0,   436,   339,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   340,
       0,     0,     0,     0,     0,     0,     0,     0,   343,   367,
     368,   369,   370,   371,   372,     0,     0,   375,   376,   377,
     378,     0,   380,   381,   344,   345,     0,   346,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,   355,   356,
       0,   357,   358,   359,   360,   361,   362,   363,     0,   341,
     364,   365,   366,   342,   395,     0,   338,     0,     0,     0,
       0,     0,   870,   339,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   340,     0,     0,     0,     0,
       0,     0,     0,     0,   343,   367,   368,   369,   370,   371,
     372,     0,     0,   375,   376,   377,   378,     0,   380,   381,
     344,   345,     0,   346,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,   355,   356,     0,   357,   358,   359,
     360,   361,   362,   363,     0,   341,   364,   365,   366,   342,
     395,     0,   338,     0,     0,     0,     0,     0,   877,   339,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   340,     0,     0,     0,     0,     0,     0,     0,     0,
     343,   367,   368,   369,   370,   371,   372,     0,     0,   375,
     376,   377,   378,     0,   380,   381,   344,   345,     0,   346,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
     355,   356,     0,   357,   358,   359,   360,   361,   362,   363,
       0,   341,   364,   365,   366,   342,   395,     0,   338,     0,
       0,     0,     0,     0,  1006,   339,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   340,     0,     0,
       0,     0,     0,     0,     0,     0,   343,   367,   368,   369,
     370,   371,   372,     0,     0,   375,   376,   377,   378,     0,
     380,   381,   344,   345,     0,   346,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,   355,   356,     0,   357,
     358,   359,   360,   361,   362,   363,     0,   341,   364,   365,
     366,   342,   395,     0,   338,     0,     0,     0,     0,     0,
    1007,   339,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   340,     0,     0,     0,     0,     0,     0,
       0,     0,   343,   367,   368,   369,   370,   371,   372,     0,
       0,   375,   376,   377,   378,     0,   380,   381,   344,   345,
       0,   346,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,   355,   356,     0,   357,   358,   359,   360,   361,
     362,   363,     0,   341,   364,   365,   366,   342,   395,     0,
     338,     0,     0,     0,     0,     0,  1008,   339,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   340,
       0,     0,     0,     0,     0,     0,     0,     0,   343,   367,
     368,   369,   370,   371,   372,     0,     0,   375,   376,   377,
     378,     0,   380,   381,   344,   345,     0,   346,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,   355,   356,
       0,   357,   358,   359,   360,   361,   362,   363,     0,   341,
     364,   365,   366,   342,   395,     0,   338,     0,     0,     0,
       0,     0,  1009,   339,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   340,     0,     0,     0,     0,
       0,     0,     0,     0,   343,   367,   368,   369,   370,   371,
     372,     0,     0,   375,   376,   377,   378,     0,   380,   381,
     344,   345,     0,   346,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,   355,   356,     0,   357,   358,   359,
     360,   361,   362,   363,     0,   341,   364,   365,   366,   342,
     395,     0,   338,     0,     0,     0,     0,     0,  1032,   339,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   340,     0,     0,     0,     0,     0,     0,     0,     0,
     343,   367,   368,   369,   370,   371,   372,     0,     0,   375,
     376,   377,   378,     0,   380,   381,   344,   345,     0,   346,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
     355,   356,     0,   357,   358,   359,   360,   361,   362,   363,
       0,   341,   364,   365,   366,   342,   395,     0,     0,     0,
       0,     0,     0,     0,  1033,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,   367,   368,   369,
     370,   371,   372,     0,     0,   375,   376,   377,   378,     0,
     380,   381,   344,   345,     0,   346,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,   355,   356,     0,   357,
     358,   359,   360,   361,   362,   363,   338,     0,   364,   365,
     366,     0,   395,   339,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   340,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   338,   430,
       0,     0,     0,     0,     0,   339,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   340,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,   341,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   338,     0,     0,     0,     0,
       0,     0,   339,     0,     0,     0,   426,   341,     0,     0,
     343,   342,     0,     0,   340,   739,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,   345,     0,   346,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
     355,   356,   343,   357,   358,   359,   360,   361,   362,   363,
       0,     0,   364,   365,   366,     0,   395,     0,   344,   345,
       0,   346,   347,   348,   341,   349,   350,   351,   342,   352,
     353,   354,   355,   356,     0,   357,   358,   359,   360,   361,
     362,   363,   338,     0,   364,   365,   366,     0,   395,   339,
       0,     0,     0,     0,     0,   426,     0,     0,     0,   343,
       0,   340,   250,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,   345,     0,   346,   347,
     348,     0,   349,   350,   351,     0,   352,   353,   354,   355,
     356,     0,   357,   358,   359,   360,   361,   362,   363,   338,
       0,   364,   365,   366,     0,   395,   339,     0,     0,     0,
       0,   341,     0,     0,     0,   342,     0,     0,   340,   731,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   338,     0,     0,     0,     0,     0,     0,   339,     0,
       0,     0,     0,     0,     0,     0,   343,     0,     0,     0,
     340,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   344,   345,     0,   346,   347,   348,   341,   349,
     350,   351,   342,   352,   353,   354,   355,   356,     0,   357,
     358,   359,   360,   361,   362,   363,     0,     0,   364,   365,
     366,     0,   395,     0,     0,     0,     0,     0,     0,     0,
     341,     0,     0,   343,   342,     0,     0,     0,     0,     0,
       0,     0,     0,   749,     0,     0,     0,     0,     0,   344,
     345,     0,   346,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,   355,   356,   343,   357,   358,   359,   360,
     361,   362,   363,     0,     0,   364,   365,   366,     0,   395,
       0,   344,   345,     0,   346,   347,   348,     0,   349,   350,
     351,     0,   352,   353,   354,   355,   356,     0,   357,   358,
     359,   360,   361,   362,   363,   338,     0,   364,   365,   366,
       0,   395,   339,   265,     0,     0,     0,    22,    23,     0,
       0,     0,     0,     0,   340,     0,   266,     0,    30,   267,
       0,     0,     0,    35,     0,     0,     0,   338,    39,     0,
       0,  -234,     0,     0,   339,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   340,     0,     0,     0,
      52,   433,    54,     0,     0,     0,     0,  -234,     0,     0,
       0,   268,     0,    62,   341,     0,     0,     0,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   434,    78,     0,   338,    80,     0,   -62,    82,     0,
       0,   339,     0,     0,     0,     0,   341,     0,     0,   343,
     342,     0,     0,   340,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,   345,   397,   346,   347,
     348,     0,   349,   350,   351,     0,   352,   353,   354,   355,
     356,   343,   357,   358,   359,   360,   361,   362,   363,     0,
       0,   364,   365,   366,     0,   466,     0,   344,   345,     0,
     346,   347,   348,   341,   349,   350,   351,   342,   352,   353,
     354,   355,   356,     0,   357,   358,   359,   360,   361,   362,
     363,   338,   435,   364,   365,   366,     0,     0,   339,     0,
       0,     0,     0,     0,   164,     0,     0,     0,   343,     0,
     340,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   344,   345,     0,   346,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,   355,   356,
       0,   357,   358,   359,   360,   361,   362,   363,   338,     0,
     364,   365,   366,     0,     0,   339,     0,     0,     0,     0,
     341,     0,     0,     0,   342,     0,     0,   340,   566,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     338,   567,     0,     0,     0,     0,     0,   339,     0,     0,
       0,     0,     0,     0,     0,   343,     0,     0,     0,   340,
     568,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,   345,   569,   346,   347,   348,   341,   349,   350,
     351,   342,   352,   353,   354,   355,   356,     0,   357,   358,
     359,   360,   361,   362,   363,     0,   470,   364,   365,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   341,
       0,     0,   343,   342,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   344,   345,
       0,   346,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,   355,   356,   343,   357,   358,   359,   360,   361,
     362,   363,     0,     0,   364,   365,   366,     0,     0,     0,
     344,   345,     0,   346,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,   355,   356,     0,   357,   358,   359,
     360,   361,   362,   363,   338,     0,   364,   365,   366,     0,
       0,   339,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   340,     0,     0,     0,     0,   677,     0,
       0,     0,     0,     0,     0,     0,   338,   265,     0,     0,
       0,    22,    23,   339,     0,     0,     0,     0,     0,     0,
     266,     0,    30,   267,     0,   340,     0,    35,   678,     0,
       0,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   341,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,    52,   338,    54,     0,   322,     0,
     921,     0,   339,   922,     0,   268,     0,    62,     0,     0,
       0,     0,     0,     0,   340,   341,     0,     0,   343,   342,
       0,     0,     0,     0,     0,     0,    78,     0,     0,    80,
       0,     0,    82,     0,   344,   345,     0,   346,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,   355,   356,
     343,   357,   358,   359,   360,   361,   362,   363,     0,     0,
     364,   365,   366,     0,   341,     0,   344,   345,   342,   346,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
     355,   356,    97,   357,   358,   359,   360,   361,   362,   363,
     338,   472,   364,   365,   366,     0,     0,   339,     0,   343,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   340,
       0,     0,     0,     0,     0,   344,   345,     0,   346,   347,
     348,     0,   349,   350,   351,     0,   352,   353,   354,   355,
     356,     0,   357,   358,   359,   360,   361,   362,   363,   338,
     474,   364,   365,   366,     0,     0,   339,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   340,   341,
       0,     0,     0,   342,     0,     0,     0,     0,     0,     0,
       0,   338,     0,     0,     0,     0,     0,     0,   339,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     340,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   341,     0,
     344,   345,   342,   346,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,   355,   356,     0,   357,   358,   359,
     360,   361,   362,   363,     0,   479,   364,   365,   366,     0,
     341,     0,   496,   343,   342,     0,     0,     0,     0,     0,
       0,     0,     0,   570,     0,     0,     0,     0,     0,   344,
     345,     0,   346,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,   355,   356,   343,   357,   358,   359,   360,
     361,   362,   363,     0,     0,   364,   365,   366,     0,     0,
       0,   344,   345,     0,   346,   347,   348,     0,   349,   350,
     351,     0,   352,   353,   354,   355,   356,     0,   357,   358,
     359,   360,   361,   362,   363,   338,     0,   364,   365,   366,
       0,     0,   339,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   340,     0,   265,     0,     0,     0,
      22,    23,     0,     0,     0,     0,     0,     0,     0,   266,
       0,    30,   267,     0,     0,     0,    35,     0,     0,     0,
     338,    39,     0,     0,     0,     0,     0,   339,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   340,
       0,     0,     0,    52,   341,    54,     0,    56,   342,   921,
       0,     0,   922,     0,   268,     0,    62,     0,     0,     0,
       0,   338,     0,     0,     0,     0,     0,     0,   339,     0,
       0,     0,     0,     0,     0,    78,     0,     0,    80,   343,
     340,    82,     0,     0,     0,     0,     0,     0,     0,   341,
       0,     0,     0,   342,     0,   344,   345,     0,   346,   347,
     348,     0,   349,   350,   351,     0,   352,   353,   354,   355,
     356,  -504,   357,   358,   359,   360,   361,   362,   363,     0,
     164,   364,   365,   366,   343,     0,     0,     0,     0,     0,
     341,    97,     0,     0,   342,     0,     0,  1049,     0,     0,
     344,   345,     0,   346,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,   355,   356,     0,   357,   358,   359,
     360,   361,   362,   363,     0,   343,   364,   365,   366,     0,
       0,     0,   338,   744,     0,     0,     0,     0,     0,   339,
       0,   344,   345,     0,   346,   347,   348,     0,   349,   350,
     351,   340,   352,   353,   354,   355,   356,     0,   357,   358,
     359,   360,   361,   362,   363,     0,   716,   364,   365,   366,
       0,     0,   338,     0,     0,     0,     0,     0,     0,   339,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   340,     0,     0,   747,     0,     0,     0,     0,     0,
       0,   341,     0,     0,     0,   342,     0,     0,     0,     0,
       0,     0,     0,   338,     0,     0,     0,     0,     0,     0,
     339,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   340,     0,     0,     0,   343,     0,     0,     0,
       0,   341,     0,     0,     0,   342,     0,     0,     0,     0,
       0,     0,   344,   345,     0,   346,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,   355,   356,     0,   357,
     358,   359,   360,   361,   362,   363,   343,     0,   364,   365,
     366,     0,   341,     0,     0,     0,   342,     0,     0,     0,
       0,     0,   344,   345,     0,   346,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,   355,   356,     0,   357,
     358,   359,   360,   361,   362,   363,   789,   343,   364,   365,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   344,   345,     0,   346,   347,   348,     0,
     349,   350,   351,     0,   352,   353,   354,   355,   356,     0,
     357,   358,   359,   360,   361,   362,   363,   338,  -215,   364,
     365,   366,     0,     0,   339,     0,     0,     0,     0,     0,
       0,     0,  -215,  -215,     0,  -215,   340,   857,     0,     0,
    -215,     0,     0,     0,     0,     0,     0,     0,  -215,   338,
       0,     0,     0,     0,  -215,     0,   339,     0,     0,  -215,
       0,  -215,     0,     0,  -215,     0,     0,     0,   340,   860,
       0,     0,  -215,     0,     0,     0,     0,     0,  -215,  -215,
       0,     0,     0,     0,     0,  -215,   341,     0,     0,     0,
     342,     0,     0,     0,  -215,     0,     0,     0,     0,     0,
       0,     0,     0,  -215,  -215,     0,  -215,     0,  -215,     0,
    -215,     0,     0,  -215,     0,     0,     0,  -215,   341,     0,
    -215,   343,   342,  -215,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,   345,     0,
     346,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,   355,   356,   343,   357,   358,   359,   360,   361,   362,
     363,     0,     0,   364,   365,   366,     0,     0,     0,   344,
     345,     0,   346,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,   355,   356,     0,   357,   358,   359,   360,
     361,   362,   363,   338,     0,   364,   365,   366,     0,     0,
     339,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   340,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   338,     0,     0,     0,     0,
       0,     0,   339,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   340,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   341,     0,     0,     0,   342,     0,     0,     0,
       0,     0,     0,     0,     0,   938,     0,     0,     0,     0,
       0,     0,   338,   957,     0,     0,     0,     0,     0,   339,
       0,     0,     0,     0,   341,     0,     0,   343,   342,     0,
       0,   340,     0,     0,     0,     0,     0,   941,     0,     0,
       0,     0,     0,   344,   345,     0,   346,   347,   348,     0,
     349,   350,   351,     0,   352,   353,   354,   355,   356,   343,
     357,   358,   359,   360,   361,   362,   363,     0,     0,   364,
     365,   366,     0,     0,     0,   344,   345,     0,   346,   347,
     348,   341,   349,   350,   351,   342,   352,   353,   354,   355,
     356,     0,   357,   358,   359,   360,   361,   362,   363,   338,
       0,   364,   365,   366,     0,     0,   339,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,     0,   340,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   344,   345,     0,   346,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,   355,   356,     0,   357,
     358,   359,   360,   361,   362,   363,     0,     0,   364,   365,
     366,     0,     0,   340,     0,     0,     0,     0,   341,     0,
       0,     0,   342,     0,     0,   338,     0,     0,     0,     0,
       0,  1043,   339,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   340,     0,     0,     0,     0,     0,
       0,     0,     0,   343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   341,     0,     0,     0,   342,     0,   344,
     345,     0,   346,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,   355,   356,     0,   357,   358,   359,   360,
     361,   362,   363,     0,   341,   364,   365,   366,   342,     0,
       0,   338,     0,     0,     0,     0,     0,  1044,   339,     0,
       0,     0,     0,     0,   344,   345,     0,   346,   347,   348,
     340,   349,   350,   351,     0,     0,   353,   354,   355,   343,
       0,   357,   358,   359,   360,     0,     0,   363,     0,     0,
     364,   365,   366,     0,     0,   344,   345,     0,   346,   347,
     348,     0,   349,   350,   351,     0,   352,   353,   354,   355,
     356,     0,   357,   358,   359,   360,   361,   362,   363,     0,
     341,   364,   365,   366,   342,     0,     0,   338,     0,     0,
       0,     0,     0,  1045,   339,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   340,     0,     0,     0,
       0,     0,     0,     0,     0,   343,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,   345,     0,   346,   347,   348,     0,   349,   350,
     351,     0,   352,   353,   354,   355,   356,     0,   357,   358,
     359,   360,   361,   362,   363,     0,   341,   364,   365,   366,
     342,     0,     0,   338,     0,     0,     0,     0,     0,  1046,
     339,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   340,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,   345,     0,
     346,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,   355,   356,     0,   357,   358,   359,   360,   361,   362,
     363,     0,   341,   364,   365,   366,   342,     0,     0,   338,
       0,     0,     0,     0,     0,  1047,   339,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   340,     0,
       0,     0,     0,     0,     0,     0,     0,   343,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   344,   345,     0,   346,   347,   348,     0,
     349,   350,   351,     0,   352,   353,   354,   355,   356,     0,
     357,   358,   359,   360,   361,   362,   363,     0,   341,   364,
     365,   366,   342,     0,     0,   338,     0,     0,     0,     0,
       0,  1048,   339,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   340,     0,     0,     0,     0,     0,
       0,     0,     0,   343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1072,   344,
     345,     0,   346,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,   355,   356,     0,   357,   358,   359,   360,
     361,   362,   363,     0,   341,   364,   365,   366,   342,     0,
       0,   338,     0,     0,     0,     0,     0,     0,   339,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     340,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,   345,     0,   346,   347,
     348,     0,   349,   350,   351,     0,   352,   353,   354,   355,
     356,     0,   357,   358,   359,   360,   361,   362,   363,     0,
     341,   364,   365,   366,   342,     0,     0,   338,     0,     0,
       0,     0,     0,     0,   339,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   340,     0,     0,     0,
       0,     0,     0,     0,     0,   343,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,   345,     0,   346,   347,   348,     0,   349,   350,
     351,     0,   352,   353,   354,   355,   356,     0,   357,   358,
     359,   360,   361,   362,   363,     0,   704,   364,   365,   366,
     342,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,   345,     0,
     346,   347,   348,     0,   349,   350,   351,   340,   352,   353,
     354,   355,   356,     0,   357,   358,   359,   360,   361,   362,
     363,     0,   265,   364,   365,   366,    22,    23,     0,     0,
       0,     0,     0,     0,     0,   266,     0,    30,   267,     0,
       0,     0,    35,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   341,     0,     0,
       0,   342,     0,     0,     0,     0,     0,     0,     0,    52,
       0,    54,     0,    56,     0,   921,     0,     0,   922,     0,
     268,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,     0,     0,     0,     0,     0,     0,     0,
       0,    78,     0,     0,    80,     0,     0,    82,   344,   345,
       0,   346,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,   355,     0,     0,   357,   358,   359,   360,   361,
     681,   363,   219,     0,   364,   365,   366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   220,   221,     0,   222,
       0,     0,     0,     0,   223,     0,     0,    97,     0,     0,
       0,     0,   224,  1050,     0,     0,     0,     0,   225,     0,
       0,     0,     0,   226,     0,   227,     0,     0,   228,     0,
       0,     0,     0,     0,     0,     0,   229,     0,     0,  -225,
       0,     0,   230,   231,     0,     0,     0,     0,     0,   232,
       0,     0,     0,  -225,  -225,     0,  -225,     0,   233,     0,
       0,  -225,     0,     0,     0,     0,     0,   234,   235,  -225,
     236,     0,   237,     0,   238,  -225,     0,   239,     0,     0,
    -225,   240,  -225,     0,   241,  -225,     0,   242,     0,     0,
       0,     0,     0,  -225,     0,     0,   219,     0,     0,  -225,
    -225,     0,     0,     0,     0,     0,  -225,     0,     0,     0,
     220,   221,     0,   222,     0,  -225,     0,     0,   223,     0,
       0,     0,     0,     0,  -225,  -225,   224,  -225,     0,  -225,
       0,  -225,   225,     0,  -225,     0,     0,   226,  -225,   227,
       0,  -225,   228,     0,  -225,     0,     0,     0,     0,     0,
     229,     0,     0,  -216,     0,     0,   230,   231,     0,     0,
       0,     0,     0,   232,     0,     0,     0,  -216,  -216,     0,
    -216,     0,   233,     0,     0,  -216,     0,     0,     0,     0,
       0,   234,   235,  -216,   236,     0,   237,     0,   238,  -216,
       0,   239,     0,     0,  -216,   240,  -216,     0,   241,  -216,
       0,   242,     0,     0,     0,     0,     0,  -216,     0,     0,
    -147,     0,     0,  -216,  -216,     0,     0,     0,     0,     0,
    -216,     0,     0,     0,  -147,  -147,     0,  -147,     0,  -216,
       0,     0,  -147,     0,     0,     0,     0,     0,  -216,  -216,
    -147,  -216,     0,  -216,     0,  -216,  -147,     0,  -216,     0,
       0,  -147,  -216,  -147,     0,  -216,  -147,     0,  -216,     0,
       0,     0,     0,     0,  -147,     0,     0,  -143,     0,     0,
    -147,  -147,     0,     0,     0,     0,     0,  -147,     0,     0,
       0,  -143,  -143,     0,  -143,     0,  -147,     0,     0,  -143,
       0,     0,     0,     0,     0,  -147,  -147,  -143,  -147,     0,
    -147,     0,  -147,  -143,     0,  -147,     0,     0,  -143,  -147,
    -143,     0,  -147,  -143,     0,  -147,     0,     0,     0,     0,
       0,  -143,     0,     0,     0,     0,     0,  -143,  -143,     0,
       0,     0,     0,     0,  -143,     0,     0,     0,     0,     0,
       0,     0,     0,  -143,     0,     0,     0,     0,     0,     0,
       0,     0,  -143,  -143,     0,  -143,     0,  -143,     0,  -143,
       0,     0,  -143,     0,     0,     0,  -143,     0,     0,  -143,
       0,     0,  -143
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-987)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      12,   444,   149,   388,    16,    59,     2,   475,    20,    19,
     190,   135,   441,   596,    26,   213,   218,   198,    30,    31,
      63,    33,    34,    35,    36,   765,   206,   100,   434,    22,
     890,   690,    44,     1,    46,   584,     1,   218,   587,    51,
      32,    53,   629,    55,   972,   793,   847,   848,    67,    27,
       3,    63,    64,    65,    66,    67,    68,    32,   101,    32,
      72,   263,    74,   893,    76,    77,    32,   191,   866,    81,
     868,    84,    84,    85,    45,    87,  1062,    89,    32,   112,
      92,    93,   263,    95,   286,   112,     1,    99,   100,   101,
     109,    50,    55,    71,   138,    45,   449,   138,   132,    67,
       0,   961,   132,   138,    82,   286,   140,   137,  1014,   164,
     140,   166,   852,   157,   144,  1101,   703,    76,   701,    45,
     318,   319,   475,    45,     1,   198,   167,    64,   163,   107,
      45,    45,   138,   145,   167,   178,    45,   112,     3,   112,
     167,   109,    95,     1,   163,   218,   293,   110,   946,   162,
     948,     3,   117,  1081,   894,    20,  1062,   169,   988,   165,
     172,   173,   174,   100,   176,   177,   178,   138,    33,   350,
     162,   720,   137,   138,   723,   762,   924,   157,    55,   189,
      33,    34,   384,   385,   138,   628,   198,   162,   138,   162,
     263,    45,    57,   994,   995,   163,   162,    55,   112,   947,
     165,   949,   117,   384,   385,    84,   218,    84,   162,   202,
     264,   596,   138,   286,   207,    55,   138,   167,   110,    18,
     960,   214,   137,   138,   138,    45,    84,   132,    82,   138,
      95,   112,    84,   110,   138,   140,   101,    76,    77,    97,
     138,   167,   157,    95,    84,   167,   138,    27,   101,    59,
     138,   263,   110,   167,  1002,   267,  1004,    97,   167,   157,
     137,   165,    82,   275,   276,    45,   468,    77,    48,   138,
     110,    70,   678,  1013,   286,   934,   138,   165,   157,   137,
     117,   138,   138,   162,   327,   162,    44,   468,    68,    88,
     414,    71,   875,    73,   163,   419,   106,     3,    82,   157,
     157,   313,    82,   165,   162,   142,   105,     8,    66,   165,
      92,   384,   385,   756,   310,   327,   701,   157,    76,    77,
     164,   101,   162,   164,   336,   754,   338,   339,   340,   341,
     342,   343,  1019,   345,   346,   347,   348,   349,   117,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   202,   164,   132,   166,   138,
     207,   373,   374,   142,   793,   140,   117,   379,   164,   164,
     382,   383,   384,   385,   386,   387,   388,   138,   421,  1066,
    1067,  1068,  1069,  1070,  1071,   397,   165,   138,   461,   138,
     402,   142,   404,   157,    81,   468,    55,   162,    85,   163,
     138,   582,   600,   601,   165,  1079,   138,   106,    77,   421,
     483,   423,   424,   486,   165,   488,   165,   429,   430,   156,
     493,   433,   434,  1097,   436,    84,   438,   165,    27,   157,
     188,   138,   900,   165,   157,   163,   373,   374,    97,   451,
     157,   138,   379,   455,   456,   382,   383,    27,    47,   461,
     157,   110,   210,   211,   212,   213,   468,    81,   138,   217,
     157,    85,   164,   506,   166,   157,   664,   665,   138,   148,
      69,   483,    71,   138,   486,   655,   488,   157,   164,   157,
     492,   493,   494,    82,   496,   497,   157,   157,   341,   342,
     875,    71,   157,    27,   506,   253,    31,   164,   157,   166,
     929,   259,    82,   162,   157,   157,   502,   580,   107,   582,
     157,   157,   164,     2,   166,   164,   157,   166,   947,   162,
     949,   101,   111,    12,   282,    80,   486,   107,   488,    80,
     164,   164,    27,   386,   387,    31,    25,    71,    27,   164,
     893,   164,   300,   690,   397,   157,    81,   900,    82,   486,
      85,   488,    55,   164,   566,   567,   568,   569,   570,   571,
     572,   117,   157,   573,   574,     8,    59,   101,   580,   137,
     582,   424,    27,  1002,   162,  1004,    71,   430,   590,   140,
     592,    84,   157,    72,   596,    81,   598,    82,   117,    85,
      45,   634,   165,    48,    97,   138,    45,   132,   133,   157,
     135,   136,   137,    34,   139,   140,   101,   110,   165,   144,
      99,   164,    27,    68,   103,   138,    71,   152,    73,   165,
     632,   633,   634,   158,   159,   160,   638,    82,   165,   117,
      45,    21,   165,    48,   138,   988,   132,   167,   162,   492,
     398,   137,   167,   580,   140,   164,   101,   157,   144,   157,
     157,   157,   117,    68,   157,   157,    71,    31,    73,   162,
     165,  1014,   138,   157,    34,   677,   678,    82,   167,   138,
     428,   157,   157,   165,   432,  1028,   165,   117,   143,   163,
     145,   146,   165,   138,   149,   150,   101,    82,   163,   701,
      32,   165,   704,   705,   706,    31,   167,   709,    32,   901,
     902,   638,   162,   162,    34,   137,    82,    81,   164,  1062,
     165,    85,   164,   167,   567,   137,   569,   729,   157,   731,
     901,   902,   167,   167,   736,   163,  1079,   739,   157,   164,
     742,   774,   744,   140,   140,   747,   140,   140,   132,   140,
     140,   132,   140,   140,  1097,    81,   132,   140,  1101,    85,
     132,   140,   140,   132,   243,   140,    34,   162,   132,   133,
     162,   250,   774,   137,    97,   139,   140,   132,   132,   165,
     144,   138,   784,   165,   786,   156,   156,   789,   152,   324,
     116,   165,   165,   795,   158,   159,   160,   934,    45,   167,
      24,   678,    66,   677,   294,   778,   132,   133,    55,   135,
     136,   137,   796,   139,   140,   141,   934,   143,   144,   145,
     146,   929,   754,   149,   150,   151,   152,   153,   154,   155,
     893,   310,   158,   159,   160,    41,  1081,    84,   901,   902,
     842,    45,   621,  1039,  1013,   324,   594,   595,   799,  1072,
    1065,    55,   600,   601,   856,   857,    35,   859,   860,   861,
     862,   704,   705,   110,   612,   452,   582,    30,   870,   871,
     117,   873,    78,   875,    -1,   877,   878,    -1,   922,   881,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,   899,   900,   901,
     902,    -1,    -1,   651,   967,   907,   110,   909,    -1,    -1,
     157,   913,    -1,   117,    -1,   162,   395,    -1,   165,    -1,
      -1,    -1,   128,    -1,   130,    -1,    -1,    -1,    -1,   962,
      -1,    -1,    -1,    -1,   138,    -1,   938,   939,   940,   941,
     942,   943,   944,   945,   423,    -1,   210,   211,   212,   213,
     429,  1014,    -1,   157,     2,   957,    -1,   436,   162,    -1,
     962,   165,    -1,    -1,    12,   967,   714,   715,    -1,   971,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,   726,    27,
     728,    -1,    -1,    -1,   190,    -1,    -1,   735,    -1,   253,
     738,    -1,    -1,   741,    -1,   259,    -1,    -1,    -1,  1062,
     206,  1034,   208,    -1,  1006,  1007,  1008,  1009,    -1,    -1,
      -1,   490,    -1,    -1,    -1,   494,    -1,    -1,   282,    -1,
      -1,    -1,    -1,   502,    72,    -1,  1028,    -1,    76,    77,
    1032,  1033,  1034,   781,    -1,    -1,   300,    -1,  1101,    -1,
     246,  1043,  1044,  1045,  1046,  1047,  1048,    -1,    -1,    -1,
      -1,    99,    -1,    -1,    -1,   103,    -1,    -1,    -1,  1061,
    1062,    -1,  1064,    -1,    -1,   271,    -1,    -1,     2,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    12,    -1,
      -1,    -1,    -1,     3,   563,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    31,    27,    -1,  1097,    -1,    17,    22,  1101,
      20,    -1,    26,    27,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    37,    38,    -1,    -1,   165,    42,    -1,
      -1,    -1,    -1,    47,    44,    -1,    50,    -1,    -1,    49,
      -1,    -1,   611,    -1,   398,    -1,    -1,    57,    72,    -1,
      -1,    -1,    81,    -1,    64,    69,    85,    71,    -1,    73,
      -1,    75,    76,    -1,    78,    -1,    80,   905,    82,    79,
      -1,    -1,    -1,    -1,   428,    99,    -1,    -1,   432,   103,
      -1,    91,    -1,    -1,    -1,    95,    -1,   101,    -1,    45,
     104,    -1,   102,   107,   663,    -1,    -1,    -1,   108,    55,
     669,    -1,    -1,   132,   133,   243,   135,   136,   137,    -1,
     139,   140,   250,   951,   952,   144,   412,   955,   956,    -1,
     416,   690,   151,   152,    -1,    -1,   155,    -1,    84,   158,
     159,   160,    -1,    -1,   972,   704,   705,    -1,    -1,    -1,
      -1,   165,   980,   157,   982,    -1,   984,    -1,   444,   163,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
     729,   117,   731,    -1,    -1,    -1,    -1,   736,    -1,    -1,
     739,    -1,   310,    -1,    22,     2,    -1,   746,    26,    27,
     749,    -1,   138,    -1,    -1,    12,   324,    35,    -1,    37,
      38,    -1,    -1,    -1,    42,    -1,    -1,    -1,    25,    47,
      27,   157,    -1,    -1,   773,    -1,   162,   503,    -1,   165,
      -1,    -1,   508,    -1,    -1,   784,    31,   786,    -1,   243,
      -1,    69,    -1,    71,    -1,    73,   250,    75,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    -1,
     594,   595,    -1,  1081,    -1,    72,   600,   601,    -1,    -1,
      -1,    -1,    -1,   101,    -1,    -1,   104,   395,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      85,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   423,   310,   583,    -1,    -1,
     586,   429,    -1,    -1,    -1,    -1,    -1,    -1,   436,    -1,
     324,   870,    -1,    -1,    -1,    -1,    -1,    -1,   877,   157,
      -1,    -1,    -1,    -1,    -1,   163,    -1,   132,    -1,    -1,
     889,    -1,   137,    31,   139,   140,    -1,    -1,   624,   144,
      -1,    -1,   628,    -1,   903,    -1,    -1,   152,   165,    -1,
      -1,    -1,    -1,   912,    -1,   160,    -1,   916,    -1,    -1,
      -1,    -1,   490,    -1,    -1,   651,   494,    -1,    -1,   655,
     714,   715,    -1,    -1,   502,    -1,    -1,    -1,    -1,    -1,
      -1,   395,   726,    81,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,   738,    -1,    -1,   741,    -1,   958,
      -1,    -1,    -1,    -1,   690,    -1,    -1,    -1,    -1,   423,
      -1,    -1,    -1,    -1,    -1,   429,    -1,    -1,    -1,    -1,
      -1,    -1,   436,    -1,    -1,    -1,   243,    -1,    -1,    -1,
      -1,    -1,    -1,   250,   132,   133,    -1,   135,   136,   137,
      -1,   139,   140,   141,    -1,    -1,   144,  1006,  1007,  1008,
    1009,    -1,    -1,   151,   152,    -1,    -1,   155,    -1,    -1,
     158,   159,   160,    -1,    -1,    -1,    -1,   753,    -1,    -1,
     756,    -1,     3,  1032,  1033,    -1,   490,    -1,    -1,    -1,
     494,    -1,    -1,   611,    -1,    -1,    17,    18,   502,    20,
      -1,    -1,   778,   310,    25,    -1,    -1,    -1,    -1,     2,
      -1,    31,    33,    -1,    -1,    -1,    -1,   324,    39,    12,
     796,    -1,    -1,    44,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    25,    -1,    27,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,   663,   822,    -1,    31,    70,
      -1,   669,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    81,    -1,    -1,    -1,    85,    -1,    88,    89,    -1,
      91,   905,    93,    -1,    95,    -1,    -1,    98,    -1,    72,
      -1,   102,   103,    -1,   105,    -1,    -1,   108,   395,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    99,   611,    -1,    -1,
     103,   729,   132,   731,    -1,    -1,   423,   137,   736,   139,
     140,   739,   429,     2,   144,    -1,    -1,    -1,   746,   436,
      -1,   749,   152,    12,    -1,    -1,    -1,    -1,   158,   159,
     160,    -1,    -1,   164,    -1,    -1,    25,    -1,    27,   132,
     133,    -1,    -1,   136,   137,   773,   139,   140,   934,   663,
      -1,   144,    -1,    -1,    -1,   669,   784,    -1,   786,   152,
      -1,    -1,   165,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,    -1,    -1,   490,    -1,    -1,    -1,   494,    -1,    -1,
      -1,    -1,    -1,    72,    -1,   502,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      99,    -1,    -1,    -1,   103,   729,    -1,   731,    25,    -1,
      27,    -1,   736,    -1,    -1,   739,    -1,    -1,    -1,    -1,
      -1,    -1,   746,    -1,    -1,   749,    -1,    -1,    -1,    -1,
     243,    -1,   870,    -1,    -1,    -1,    -1,   250,    -1,   877,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,   773,
      -1,   889,    -1,    -1,    -1,    72,    35,    -1,    37,    38,
     784,    -1,   786,    42,    -1,   903,   165,    -1,    47,    -1,
      -1,    -1,    -1,    -1,   912,    -1,    -1,    -1,   916,    -1,
      -1,    -1,    99,    -1,   611,    -1,   103,    -1,    -1,    -1,
      69,    -1,    71,    -1,    73,    -1,    75,   310,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   324,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
     958,    -1,   101,    -1,    -1,   104,    -1,    35,   107,    37,
      38,    -1,    -1,    -1,    42,    -1,   663,    -1,    -1,    47,
      -1,    -1,   669,    -1,   243,    -1,   870,    -1,   165,    -1,
      -1,   250,    -1,   877,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    71,    -1,   889,    -1,    -1,  1006,  1007,
    1008,  1009,    80,    -1,    82,    -1,    -1,    -1,   157,   903,
      -1,    -1,   395,    -1,   163,    -1,    -1,    -1,   912,    -1,
      -1,    -1,   916,   101,  1032,  1033,   104,    -1,    -1,   107,
      -1,    -1,   729,    -1,   731,    -1,    -1,    -1,    -1,   736,
     423,   310,   739,    -1,    -1,    -1,   429,     2,    -1,   746,
      -1,    -1,   749,   436,    -1,   324,   243,    12,    -1,    -1,
      -1,    -1,    -1,   250,   958,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,   773,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,   784,    -1,   786,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,    -1,    -1,    -1,   490,    25,    -1,
      -1,   494,  1006,  1007,  1008,  1009,    33,    72,    -1,   502,
      -1,    -1,    39,   310,    -1,    -1,   395,    44,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    -1,   324,  1032,  1033,
      57,    -1,    -1,    -1,    99,    -1,    63,    64,   103,    -1,
      -1,    -1,    -1,    70,   423,    -1,    -1,    -1,    -1,    -1,
     429,    -1,    79,    -1,    -1,    -1,    -1,   436,    -1,    -1,
      -1,    88,    89,   870,    91,    -1,    93,    -1,    95,    -1,
     877,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,    -1,
      -1,   108,   889,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   903,    -1,   395,    -1,
     165,    -1,    -1,    -1,    -1,   912,    -1,    -1,    -1,   916,
      -1,   490,    -1,    -1,    -1,   494,    -1,    -1,   611,    -1,
      -1,    -1,    -1,   502,    -1,    -1,   423,    -1,    -1,    -1,
      -1,    -1,   429,     2,    -1,   162,    -1,   164,    -1,   436,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   958,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     663,    -1,    -1,    -1,    -1,    -1,   669,    -1,   243,    -1,
      -1,    -1,    -1,    -1,    -1,   250,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   490,    -1,    -1,    -1,   494,    -1,  1006,
    1007,  1008,  1009,    72,    -1,   502,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1032,  1033,     3,    -1,    -1,
      99,    -1,   611,    -1,   103,    -1,   729,    -1,   731,    -1,
      -1,    17,    18,   736,    20,   310,   739,    -1,    -1,    25,
      -1,    -1,    -1,   746,    -1,    -1,   749,    33,    -1,   324,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
     773,    57,    -1,    -1,   663,    -1,    -1,    63,    64,    -1,
     669,   784,    -1,   786,    70,    -1,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,   611,    91,    -1,    93,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,
     395,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     729,    -1,   731,    -1,    -1,    -1,    -1,   736,   423,    -1,
     739,    -1,    -1,    -1,   429,    -1,   663,   746,    -1,    -1,
     749,   436,   669,    -1,   243,    -1,    -1,   870,    -1,    -1,
      -1,   250,    -1,    -1,   877,    -1,    -1,    -1,   164,    -1,
      -1,    -1,    -1,    -1,   773,    -1,   889,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   784,    -1,   786,    -1,    -1,
     903,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   912,
      -1,    -1,    -1,   916,    -1,   490,    -1,    -1,    -1,   494,
      -1,    -1,   729,    -1,   731,    -1,    -1,   502,    -1,   736,
      -1,   310,   739,    -1,    -1,    -1,    -1,    -1,    -1,   746,
      -1,    -1,   749,    -1,    -1,   324,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   958,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   773,    -1,    -1,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    -1,   784,    -1,   786,
      -1,   870,    -1,    -1,    -1,    -1,    17,    18,   877,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
     889,    -1,    33,  1006,  1007,  1008,  1009,    -1,    39,    -1,
      -1,    -1,    -1,    44,   903,    46,   395,    -1,    49,    -1,
      -1,    -1,    -1,   912,    -1,    -1,    57,   916,    -1,  1032,
    1033,    -1,    63,    64,    -1,    -1,   611,    -1,    -1,    70,
      -1,    -1,    -1,    -1,   423,    -1,    -1,    -1,    79,    -1,
     429,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    -1,
      91,    -1,    93,   870,    95,    -1,    -1,    98,    -1,   958,
     877,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,   889,    -1,    -1,    -1,    -1,    -1,   663,    -1,
      -1,    -1,    -1,    -1,   669,    -1,   903,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   912,    -1,    -1,    -1,   916,
      -1,   490,    -1,    -1,    -1,   494,    -1,  1006,  1007,  1008,
    1009,    -1,    -1,   502,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   164,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,  1032,  1033,    -1,    -1,    -1,    -1,    -1,
      -1,   958,    -1,    31,   729,    -1,   731,    -1,    -1,    -1,
      -1,   736,    -1,    22,   739,    -1,    -1,    26,    27,    -1,
      -1,   746,    -1,    -1,   749,    -1,    35,    -1,    37,    38,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   773,  1006,
    1007,  1008,  1009,    81,    -1,    -1,    -1,    85,    -1,   784,
      69,   786,    71,    -1,    73,    -1,    75,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,  1032,  1033,    -1,    -1,    -1,
      -1,    -1,   611,    -1,   112,    -1,    -1,    -1,   116,    -1,
      -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,
      -1,   139,   140,   141,    -1,   143,   144,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,   154,   155,    -1,    -1,
     158,   159,   160,    -1,   663,    -1,    -1,    -1,    -1,   167,
     669,    -1,    -1,    -1,    -1,   870,    -1,    -1,   157,    -1,
      -1,    -1,   877,    -1,   163,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   889,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   903,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   912,    -1,    12,
      -1,   916,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
     729,    -1,   731,    -1,    -1,    -1,    -1,   736,    31,    -1,
     739,    -1,    -1,    -1,    -1,    -1,    -1,   746,    -1,    -1,
     749,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,   958,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,   773,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   784,    -1,   786,    81,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1006,  1007,  1008,  1009,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    85,    -1,    -1,    -1,  1032,  1033,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,    -1,   116,   158,   159,   160,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,   141,
     889,   143,   144,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,   903,    -1,   158,   159,   160,    -1,
      -1,    -1,    -1,   912,    -1,   167,    -1,   916,    -1,    -1,
      -1,    -1,    -1,     0,     1,    -1,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,   958,
      37,    38,    39,    40,    41,    42,    43,    44,    -1,    46,
      47,    -1,    49,    50,    -1,    52,    53,    54,    -1,    56,
      57,    58,    59,    60,    61,    -1,    63,    64,    65,    -1,
      -1,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    86,
      87,    88,    89,    90,    91,    92,    93,    -1,    95,    96,
      -1,    98,    99,   100,   101,   102,    -1,   104,   105,   106,
     107,   108,    -1,    -1,   111,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,
     157,    -1,    -1,   160,    -1,   162,    -1,   164,     1,   166,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    54,    -1,    -1,    57,    58,    -1,    60,    61,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    72,
      -1,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,
      93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,   154,   155,   156,    -1,    -1,    -1,   160,    -1,   162,
      -1,   164,    -1,   166,   167,     1,    -1,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    18,    19,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    47,    -1,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    58,    59,    60,    61,    -1,    63,    64,    65,
      -1,    -1,    -1,    69,    70,    71,    72,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    -1,    95,
      96,    -1,    98,    99,   100,   101,   102,    -1,   104,   105,
     106,   107,   108,    -1,    -1,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    -1,   162,   163,   164,     1,
     166,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    46,    47,    -1,    49,    50,    -1,
      52,    53,    54,    -1,    56,    57,    58,    59,    60,    61,
      -1,    63,    64,    65,    -1,    -1,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    86,    87,    88,    89,    90,    91,
      92,    93,    -1,    95,    96,    -1,    98,    99,   100,   101,
     102,    -1,   104,   105,   106,   107,   108,    -1,    -1,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,   154,   155,   156,   157,    -1,    -1,   160,    -1,
     162,   163,   164,     1,   166,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    46,    47,
      -1,    49,    50,    -1,    52,    53,    54,    -1,    56,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    -1,    -1,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    86,    87,
      88,    89,    90,    91,    92,    93,    -1,    95,    96,    -1,
      98,    99,   100,   101,   102,    -1,   104,   105,   106,   107,
     108,    -1,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,   157,
      -1,    -1,   160,    -1,   162,   163,   164,     1,   166,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    47,    -1,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    -1,    -1,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    86,    87,    88,    89,    90,    91,    92,    93,
      -1,    95,    96,    -1,    98,    99,   100,   101,   102,    -1,
     104,   105,   106,   107,   108,    -1,    -1,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
     134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,
     154,   155,   156,   157,    -1,    -1,   160,    -1,   162,   163,
     164,     1,   166,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    46,    47,    -1,    49,
      50,    -1,    52,    53,    54,    -1,    56,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    -1,    -1,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    86,    87,    88,    89,
      90,    91,    92,    93,    -1,    95,    96,    -1,    98,    99,
     100,   101,   102,    -1,   104,   105,   106,   107,   108,    -1,
      -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,    -1,   154,   155,   156,   157,    -1,    -1,
     160,    -1,   162,   163,   164,     1,   166,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    47,    -1,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    58,    59,    60,    61,    -1,    63,    64,    65,
      -1,    -1,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    90,    91,    92,    93,    -1,    95,
      96,    -1,    98,    99,   100,   101,   102,    -1,   104,   105,
     106,   107,   108,    -1,    -1,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,
     156,   157,    -1,    -1,   160,    -1,   162,   163,   164,     1,
     166,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    46,    47,    -1,    49,    50,    -1,
      52,    53,    54,    -1,    56,    57,    58,    59,    60,    61,
      -1,    63,    64,    65,    -1,    -1,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    86,    87,    88,    89,    90,    91,
      92,    93,    -1,    95,    96,    -1,    98,    99,   100,   101,
     102,    -1,   104,   105,   106,   107,   108,    -1,    -1,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,   154,   155,   156,   157,    -1,    -1,   160,    -1,
     162,     1,   164,     3,   166,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    46,    47,    -1,    49,
      50,    -1,    52,    53,    54,    -1,    56,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    -1,    -1,    -1,    69,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    86,    87,    88,    89,
      90,    91,    92,    93,    -1,    95,    96,    -1,    98,    99,
     100,   101,   102,    -1,   104,   105,   106,   107,   108,    -1,
      -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,    -1,   154,   155,   156,   157,    -1,    -1,
     160,    -1,   162,     1,   164,     3,   166,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    46,    47,
      -1,    49,    50,    -1,    52,    53,    54,    -1,    56,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    -1,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    86,    87,
      88,    89,    90,    91,    92,    93,    -1,    95,    96,    -1,
      98,    99,   100,   101,   102,    -1,   104,   105,   106,   107,
     108,    -1,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,   157,
      -1,    -1,   160,    -1,   162,     1,   164,     3,   166,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    47,    -1,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    58,    59,    60,    61,    -1,    63,    64,    65,
      -1,    -1,    -1,    69,    70,    71,    72,    -1,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    90,    91,    92,    93,    -1,    95,
      96,    -1,    98,    99,   100,   101,   102,    -1,   104,   105,
     106,   107,   108,    -1,    -1,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,
     156,   157,    -1,    -1,   160,    -1,   162,     1,   164,     3,
     166,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    47,    -1,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    -1,    -1,    -1,    69,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    86,    87,    88,    89,    90,    91,    92,    93,
      -1,    95,    96,    -1,    98,    99,   100,   101,   102,    -1,
     104,   105,   106,   107,   108,    -1,    -1,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
     134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,
     154,   155,   156,   157,    -1,    -1,   160,    -1,   162,     1,
     164,     3,   166,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    46,    47,    -1,    49,    50,    -1,
      52,    53,    54,    -1,    56,    57,    58,    59,    60,    61,
      -1,    63,    64,    65,    -1,    -1,    -1,    69,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    86,    87,    88,    89,    90,    91,
      92,    93,    -1,    95,    96,    -1,    98,    99,   100,   101,
     102,    -1,   104,   105,   106,   107,   108,    -1,    -1,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,   154,   155,   156,   157,    -1,    -1,   160,    -1,
     162,     1,   164,     3,   166,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    46,    47,    -1,    49,
      50,    -1,    52,    53,    54,    -1,    56,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    -1,    -1,    -1,    69,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    86,    87,    88,    89,
      90,    91,    92,    93,    -1,    95,    96,    -1,    98,    99,
     100,   101,   102,    -1,   104,   105,   106,   107,   108,    -1,
      -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,    -1,   154,   155,   156,   157,    -1,    -1,
     160,    -1,   162,     1,   164,     3,   166,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    46,    47,
      -1,    49,    50,    -1,    52,    53,    54,    -1,    56,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    -1,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    86,    87,
      88,    89,    90,    91,    92,    93,    -1,    95,    96,    -1,
      98,    99,   100,   101,   102,    -1,   104,   105,   106,   107,
     108,    -1,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,   157,
      -1,    -1,   160,    -1,   162,     1,   164,     3,   166,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    47,    -1,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    58,    59,    60,    61,    -1,    63,    64,    65,
      -1,    -1,    -1,    69,    70,    71,    72,    -1,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    90,    91,    92,    93,    -1,    95,
      96,    -1,    98,    99,   100,   101,   102,    -1,   104,   105,
     106,   107,   108,    -1,    -1,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,
     156,   157,    -1,    -1,   160,    -1,   162,     1,   164,     3,
     166,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    47,    -1,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    -1,    -1,    -1,    69,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    86,    87,    88,    89,    90,    91,    92,    93,
      -1,    95,    96,    -1,    98,    99,   100,   101,   102,    -1,
     104,   105,   106,   107,   108,    -1,    -1,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
     134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,
     154,   155,   156,   157,    -1,    -1,   160,    -1,   162,     1,
     164,     3,   166,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    46,    47,    -1,    49,    50,    -1,
      52,    53,    54,    -1,    56,    57,    58,    59,    60,    61,
      -1,    63,    64,    65,    -1,    -1,    -1,    69,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    86,    87,    88,    89,    90,    91,
      92,    93,    -1,    95,    96,    -1,    98,    99,   100,   101,
     102,    -1,   104,   105,   106,   107,   108,    -1,    -1,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,   154,   155,   156,   157,    -1,    -1,   160,    -1,
     162,    -1,   164,     1,   166,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    54,    -1,    -1,    57,
      58,    -1,    60,    61,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    70,    -1,    72,    -1,    74,    -1,    -1,    -1,
      -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    89,    90,    91,    92,    93,    -1,    95,    -1,    -1,
      98,    -1,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,    -1,
      -1,    -1,   160,    -1,   162,    -1,   164,    -1,   166,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      54,    -1,    -1,    57,    58,    -1,    60,    61,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,
      74,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,    93,
      -1,    95,    -1,    -1,    98,    -1,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
     134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,
     154,   155,   156,    -1,    -1,    -1,   160,    -1,   162,    -1,
     164,    -1,   166,   167,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    54,    -1,    -1,    57,    58,
      -1,    60,    61,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    70,    -1,    72,    -1,    74,    -1,    -1,    -1,    -1,
      79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      89,    90,    91,    92,    93,    -1,    95,    -1,    -1,    98,
      -1,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,    -1,    -1,   154,   155,   156,    -1,    -1,
      -1,   160,    -1,   162,    -1,   164,    -1,   166,   167,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      54,    -1,    -1,    57,    58,    -1,    60,    61,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,
      74,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,    93,
      -1,    95,    -1,    -1,    98,    -1,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
     134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,
     154,   155,   156,    -1,    -1,    -1,   160,    -1,   162,    -1,
     164,    -1,   166,   167,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    26,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    54,    -1,    -1,    57,    58,
      -1,    60,    61,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    -1,    -1,
      79,    80,    -1,    82,    -1,    -1,    -1,    -1,    -1,    88,
      89,    90,    91,    92,    93,    -1,    95,    -1,    -1,    98,
      -1,    -1,    -1,   102,    -1,    -1,   105,    -1,   107,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,    -1,    -1,   154,   155,   156,    -1,    -1,
      -1,   160,    -1,   162,    -1,   164,    -1,   166,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,
      -1,    -1,    57,    58,    -1,    60,    61,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,
      -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    89,    90,    91,    92,    93,    -1,
      95,    -1,    -1,    98,    99,   100,    -1,   102,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,
     155,   156,    -1,    -1,    -1,   160,    -1,   162,    -1,   164,
      -1,   166,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,
      61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    -1,    95,    -1,    -1,    98,    99,   100,
      -1,   102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,    -1,    -1,   154,   155,   156,    -1,    -1,    -1,   160,
      -1,   162,    -1,   164,   165,   166,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,    -1,
      57,    58,    -1,    60,    61,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,    -1,
      -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    89,    90,    91,    92,    93,    -1,    95,    -1,
      -1,    98,    99,   100,    -1,   102,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,
      -1,    -1,    -1,   160,    -1,   162,    -1,   164,   165,   166,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    45,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    54,    -1,    -1,    57,    58,    -1,    60,    61,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    72,
      -1,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,    82,
      -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,
      93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,   102,
      -1,    -1,   105,    -1,   107,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,   154,   155,   156,    -1,    -1,    -1,   160,    -1,   162,
      -1,   164,     3,   166,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,
      61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    -1,    95,    -1,    -1,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,    -1,    -1,   154,   155,   156,   157,    -1,    -1,   160,
      -1,   162,    -1,   164,    -1,   166,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,    -1,
      57,    58,    -1,    60,    61,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,    -1,
      -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    89,    90,    91,    92,    93,    -1,    95,    -1,
      -1,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,
      -1,    -1,    -1,   160,    -1,   162,    -1,   164,     3,   166,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,
      -1,    -1,    57,    58,    -1,    60,    61,    62,    63,    64,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,
      -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    89,    90,    91,    92,    93,    -1,
      95,    -1,    -1,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,
     155,   156,    -1,    -1,    -1,   160,    -1,   162,    -1,   164,
       3,   166,     5,     6,     7,     8,     9,    10,    11,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    54,    -1,    -1,    57,    58,    -1,    60,    61,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    72,
      -1,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,
      93,    -1,    95,    -1,    -1,    98,    -1,    -1,   101,   102,
      -1,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,   154,   155,   156,    -1,    -1,    -1,   160,    -1,   162,
      -1,   164,     3,   166,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,
      61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,   114,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,    -1,    -1,   154,   155,   156,    -1,    -1,    -1,   160,
      -1,   162,    -1,   164,    -1,   166,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,    -1,
      57,    58,    -1,    60,    61,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,    -1,
      -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    89,    90,    91,    92,    93,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,   114,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,
      -1,    -1,    -1,   160,    -1,   162,    -1,   164,     3,   166,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,
      -1,    -1,    57,    58,    -1,    60,    61,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,
      -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    89,    90,    91,    92,    93,    -1,
      95,    -1,    -1,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,
     155,   156,    -1,    -1,    -1,   160,    -1,   162,    -1,   164,
      -1,   166,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,
      61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,   132,   133,   134,   135,   136,    -1,    -1,    -1,    12,
     141,    -1,    -1,    -1,    17,    18,    19,    20,    -1,    -1,
     151,    -1,    25,   154,   155,   156,    -1,    -1,    -1,   160,
      33,   162,   163,   164,    -1,   166,    39,    -1,    -1,    -1,
      -1,    44,    -1,    46,    -1,    -1,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    -1,    91,    -1,
      93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,   119,   120,   121,   122,
      -1,    -1,   125,   126,   127,   128,    -1,   130,   131,   132,
     133,   134,   135,   136,    -1,    -1,   139,    -1,    -1,    -1,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,    -1,   155,    -1,    -1,   158,   159,   160,   161,    -1,
       3,   164,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    54,    -1,    -1,    57,    58,    -1,    60,    61,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    72,
      -1,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,
      93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,   114,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,   154,   155,   156,    -1,    -1,    -1,   160,    -1,   162,
      -1,   164,     3,   166,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,
      61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,    -1,    -1,   154,   155,   156,    -1,    -1,    -1,   160,
      -1,   162,    -1,   164,    -1,   166,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,    -1,
      57,    58,    -1,    60,    61,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,    -1,
      -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    89,    90,    91,    92,    93,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,
      -1,    -1,    -1,   160,    -1,   162,    -1,   164,     3,   166,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,
      -1,    -1,    57,    58,    -1,    60,    61,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,
      -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    89,    90,    91,    92,    93,    -1,
      95,    -1,    -1,    98,    -1,    -1,   101,   102,    -1,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,
     155,   156,    -1,    -1,    -1,   160,    -1,   162,    -1,   164,
      -1,   166,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,
      61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,    -1,    -1,   154,   155,   156,    -1,    -1,    -1,   160,
      -1,   162,    -1,   164,    -1,   166,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,    -1,
      57,    58,    -1,    60,    61,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,    -1,
      -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    89,    90,    91,    92,    93,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,
      -1,    -1,    -1,   160,    -1,   162,    -1,   164,    -1,   166,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    54,    -1,    -1,    57,    58,    -1,    60,    61,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    72,
      -1,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,
      93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,   154,   155,   156,    -1,    -1,    -1,   160,    -1,   162,
      -1,   164,     3,   166,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,
      61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,    -1,    -1,   154,   155,   156,    -1,    -1,    -1,   160,
      -1,   162,    -1,   164,     3,   166,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    54,    -1,    -1,    57,    58,
      -1,    60,    61,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    70,    -1,    72,    -1,    74,    -1,    -1,    -1,    -1,
      79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      89,    90,    91,    92,    93,    -1,    95,    -1,    -1,    98,
      -1,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,    -1,    -1,   154,   155,   156,    -1,    -1,
      -1,   160,    -1,   162,    -1,   164,     3,   166,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,    -1,
      57,    58,    -1,    60,    61,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,    -1,
      -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    89,    90,    91,    92,    93,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,
      -1,    -1,    -1,   160,    -1,   162,    -1,   164,     3,   166,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,
      -1,    -1,    57,    58,    -1,    60,    61,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,
      -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    89,    90,    91,    92,    93,    -1,
      95,    -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,
     155,   156,    -1,    -1,    -1,   160,    -1,   162,    -1,   164,
       3,   166,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    54,    -1,    -1,    57,    58,    -1,    60,    61,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    72,
      -1,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,
      93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,   154,   155,   156,    -1,    -1,    -1,   160,    -1,   162,
      -1,   164,     3,   166,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,
      61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,    -1,    -1,   154,   155,   156,    -1,    -1,    -1,   160,
      -1,   162,    -1,   164,     3,   166,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    54,    -1,    -1,    57,    58,
      -1,    60,    61,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    70,    -1,    72,    -1,    74,    -1,    -1,    -1,    -1,
      79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      89,    90,    91,    92,    93,    -1,    95,    -1,    -1,    98,
      -1,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,    -1,    -1,   154,   155,   156,    -1,    -1,
      -1,   160,    -1,   162,    -1,   164,     3,   166,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,    -1,
      57,    58,    -1,    60,    61,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,    -1,
      -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    89,    90,    91,    92,    93,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,
      -1,    -1,    -1,   160,    -1,   162,    -1,   164,     3,   166,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,
      -1,    -1,    57,    58,    -1,    60,    61,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,
      -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    89,    90,    91,    92,    93,    -1,
      95,    -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,
     155,   156,    -1,    -1,    -1,   160,    -1,   162,    -1,   164,
       3,   166,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    54,    -1,    -1,    57,    58,    -1,    60,    61,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    72,
      -1,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,
      93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,   154,   155,   156,    -1,    -1,    -1,   160,    -1,   162,
      -1,   164,     3,   166,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,
      61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,    -1,    -1,   154,   155,   156,    -1,    -1,    -1,   160,
      -1,   162,    -1,   164,     3,   166,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    54,    -1,    -1,    57,    58,
      -1,    60,    61,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    70,    -1,    72,    -1,    74,    -1,    -1,    -1,    -1,
      79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      89,    90,    91,    92,    93,    -1,    95,    -1,    -1,    98,
      -1,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,    -1,    -1,   154,   155,   156,    -1,    -1,
      -1,   160,    -1,   162,    -1,   164,     3,   166,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,    -1,
      57,    58,    -1,    60,    61,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,    -1,
      -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    89,    90,    91,    92,    93,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,
      -1,    -1,    -1,   160,    -1,   162,    -1,   164,     3,   166,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,
      -1,    -1,    57,    58,    -1,    60,    61,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,
      -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    89,    90,    91,    92,    93,    -1,
      95,    -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,
     155,   156,    -1,    -1,    -1,   160,    -1,   162,    -1,   164,
       3,   166,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    54,    -1,    -1,    57,    58,    -1,    60,    61,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    72,
      -1,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,
      93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,   154,   155,   156,    -1,    -1,    -1,   160,    -1,   162,
      -1,   164,     3,   166,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,
      61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,    -1,    -1,   154,   155,   156,    -1,    -1,    -1,   160,
      -1,    -1,    -1,   164,     3,   166,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    54,    -1,    -1,    57,    58,
      -1,    60,    61,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    70,    -1,    72,    -1,    74,    -1,    -1,    -1,    -1,
      79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      89,    90,    91,    92,    93,    -1,    95,    -1,    -1,    98,
      -1,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,    -1,    -1,   161,   155,   156,   164,    -1,
     166,   160,    -1,   162,    -1,   164,    -1,   166,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,
      -1,    -1,    57,    58,    -1,    60,    61,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,
      -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    89,    90,    91,    92,    93,    -1,
      95,    -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,    -1,    -1,    -1,   162,    -1,   164,
     165,   166,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    54,    55,    -1,    57,    58,    -1,    60,
      61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    -1,    95,    -1,    97,    98,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,   134,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,   156,   157,    -1,    -1,    -1,
      -1,   162,    33,   164,    35,   166,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,
      61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,     3,   134,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,   161,    25,   156,   164,    -1,   166,    -1,
      -1,   162,    33,   164,    35,   166,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,
      61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,   134,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,   156,    -1,    -1,    -1,    -1,
      -1,   162,    33,   164,    35,   166,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,
      61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,     3,
      -1,   102,    -1,    -1,   105,    -1,    -1,   108,    12,    -1,
      -1,    -1,    -1,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,   134,    -1,    39,    -1,    -1,    -1,    -1,
      44,    -1,    46,    -1,    -1,    49,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,   164,    12,   166,    70,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    88,    89,    -1,    91,    -1,    93,
      -1,    95,    -1,    -1,    98,    -1,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,   119,   120,   121,   122,    -1,
      -1,   125,   126,   127,   128,    31,   130,   131,   132,   133,
     134,   135,   136,    81,    -1,   139,    -1,    85,    -1,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
      -1,   155,    12,    -1,   158,   159,   160,   161,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      -1,    31,    -1,    -1,    -1,    81,    -1,    -1,    -1,    85,
      -1,    -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,
      -1,   139,   140,   141,    -1,   143,   144,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,   154,   155,    -1,    12,
     158,   159,   160,    -1,    -1,    -1,    19,    -1,    -1,   167,
      -1,    81,    -1,    -1,    -1,    85,   132,   133,    31,   135,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
     146,    -1,    -1,   149,   150,   151,   152,   153,    -1,   155,
      12,    -1,   158,   159,   160,    -1,   116,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,   132,   133,    -1,   135,   136,   137,    81,   139,
     140,   141,    85,   143,   144,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,    12,    -1,   158,   159,
     160,    -1,    -1,    19,    -1,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    31,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,    -1,   116,   158,   159,   160,    -1,    -1,
      -1,    -1,   165,    -1,    -1,    81,    -1,    -1,    -1,    85,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,    12,    -1,   158,   159,   160,    -1,
     116,    19,    -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,   132,   133,    -1,   135,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
      -1,    -1,   158,   159,   160,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    -1,    -1,    -1,    85,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    94,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,   121,   122,    -1,    -1,   125,   126,   127,
     128,    -1,   130,   131,   132,   133,    -1,   135,   136,   137,
      -1,   139,   140,   141,    -1,   143,   144,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,   154,   155,    -1,    81,
     158,   159,   160,    85,   162,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,   121,
     122,    -1,    -1,   125,   126,   127,   128,    -1,   130,   131,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,    -1,    81,   158,   159,   160,    85,
     162,    -1,    12,    -1,    -1,    -1,    -1,    -1,    94,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,   121,   122,    -1,    -1,   125,
     126,   127,   128,    -1,   130,   131,   132,   133,    -1,   135,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
      -1,    81,   158,   159,   160,    85,   162,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    94,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,   121,   122,    -1,    -1,   125,   126,   127,   128,    -1,
     130,   131,   132,   133,    -1,   135,   136,   137,    -1,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,    -1,    81,   158,   159,
     160,    85,   162,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      94,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,   121,   122,    -1,
      -1,   125,   126,   127,   128,    -1,   130,   131,   132,   133,
      -1,   135,   136,   137,    -1,   139,   140,   141,    -1,   143,
     144,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,    -1,    81,   158,   159,   160,    85,   162,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    94,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,   121,   122,    -1,    -1,   125,   126,   127,
     128,    -1,   130,   131,   132,   133,    -1,   135,   136,   137,
      -1,   139,   140,   141,    -1,   143,   144,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,   154,   155,    -1,    81,
     158,   159,   160,    85,   162,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,   121,
     122,    -1,    -1,   125,   126,   127,   128,    -1,   130,   131,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,    -1,    81,   158,   159,   160,    85,
     162,    -1,    12,    -1,    -1,    -1,    -1,    -1,    94,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,   121,   122,    -1,    -1,   125,
     126,   127,   128,    -1,   130,   131,   132,   133,    -1,   135,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
      -1,    81,   158,   159,   160,    85,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,   121,   122,    -1,    -1,   125,   126,   127,   128,    -1,
     130,   131,   132,   133,    -1,   135,   136,   137,    -1,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,    12,    -1,   158,   159,
     160,    -1,   162,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    45,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    81,    -1,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,   112,    81,    -1,    -1,
     116,    85,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,    -1,   135,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,   116,   149,   150,   151,   152,   153,   154,   155,
      -1,    -1,   158,   159,   160,    -1,   162,    -1,   132,   133,
      -1,   135,   136,   137,    81,   139,   140,   141,    85,   143,
     144,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,    12,    -1,   158,   159,   160,    -1,   162,    19,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,    -1,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,    12,
      -1,   158,   159,   160,    -1,   162,    19,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    85,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,    -1,   135,   136,   137,    81,   139,
     140,   141,    85,   143,   144,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,    -1,    -1,   158,   159,
     160,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,   116,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,   146,   147,   116,   149,   150,   151,   152,
     153,   154,   155,    -1,    -1,   158,   159,   160,    -1,   162,
      -1,   132,   133,    -1,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,    12,    -1,   158,   159,   160,
      -1,   162,    19,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    35,    -1,    37,    38,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    12,    47,    -1,
      -1,    50,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      69,    36,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    80,    -1,    82,    81,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,   101,    -1,    12,   104,    -1,   106,   107,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    81,    -1,    -1,   116,
      85,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,    45,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,   146,
     147,   116,   149,   150,   151,   152,   153,   154,   155,    -1,
      -1,   158,   159,   160,    -1,   162,    -1,   132,   133,    -1,
     135,   136,   137,    81,   139,   140,   141,    85,   143,   144,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,    12,   157,   158,   159,   160,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,
      -1,   139,   140,   141,    -1,   143,   144,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,   154,   155,    12,    -1,
     158,   159,   160,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    85,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    45,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,    45,   135,   136,   137,    81,   139,   140,
     141,    85,   143,   144,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      -1,    -1,   116,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
      -1,   135,   136,   137,    -1,   139,   140,   141,    -1,   143,
     144,   145,   146,   147,   116,   149,   150,   151,   152,   153,
     154,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,    12,    -1,   158,   159,   160,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    22,    -1,    -1,
      -1,    26,    27,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    37,    38,    -1,    31,    -1,    42,    66,    -1,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    12,    71,    -1,    73,    -1,
      75,    -1,    19,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    81,    -1,    -1,   116,    85,
      -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,   104,
      -1,    -1,   107,    -1,   132,   133,    -1,   135,   136,   137,
      -1,   139,   140,   141,    -1,   143,   144,   145,   146,   147,
     116,   149,   150,   151,   152,   153,   154,   155,    -1,    -1,
     158,   159,   160,    -1,    81,    -1,   132,   133,    85,   135,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,   157,   149,   150,   151,   152,   153,   154,   155,
      12,   157,   158,   159,   160,    -1,    -1,    19,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   132,   133,    -1,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,    12,
     157,   158,   159,   160,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    81,
      -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,
     132,   133,    85,   135,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,    -1,
      81,    -1,   115,   116,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,   146,   147,   116,   149,   150,   151,   152,
     153,   154,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   132,   133,    -1,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,    12,    -1,   158,   159,   160,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    22,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      12,    47,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    69,    81,    71,    -1,    73,    85,    75,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,   104,   116,
      31,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    85,    -1,   132,   133,    -1,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,    -1,
     112,   158,   159,   160,   116,    -1,    -1,    -1,    -1,    -1,
      81,   157,    -1,    -1,    85,    -1,    -1,   163,    -1,    -1,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,    -1,   116,   158,   159,   160,    -1,
      -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,   132,   133,    -1,   135,   136,   137,    -1,   139,   140,
     141,    31,   143,   144,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,    -1,   135,   136,   137,    -1,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   116,    -1,   158,   159,
     160,    -1,    81,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,   132,   133,    -1,   135,   136,   137,    -1,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   115,   116,   158,   159,
     160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,   154,   155,    12,     3,   158,
     159,   160,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    18,    -1,    20,    31,    32,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    12,
      -1,    -1,    -1,    -1,    39,    -1,    19,    -1,    -1,    44,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    31,    32,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    70,    81,    -1,    -1,    -1,
      85,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    89,    -1,    91,    -1,    93,    -1,
      95,    -1,    -1,    98,    -1,    -1,    -1,   102,    81,    -1,
     105,   116,    85,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,    -1,
     135,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,   116,   149,   150,   151,   152,   153,   154,
     155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,    12,    -1,   158,   159,   160,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    81,    -1,    -1,   116,    85,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,   146,   147,   116,
     149,   150,   151,   152,   153,   154,   155,    -1,    -1,   158,
     159,   160,    -1,    -1,    -1,   132,   133,    -1,   135,   136,
     137,    81,   139,   140,   141,    85,   143,   144,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,    12,
      -1,   158,   159,   160,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,    -1,   135,   136,   137,    -1,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,    -1,    -1,   158,   159,
     160,    -1,    -1,    31,    -1,    -1,    -1,    -1,    81,    -1,
      -1,    -1,    85,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    -1,    -1,    -1,    85,    -1,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,    -1,    81,   158,   159,   160,    85,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    94,    19,    -1,
      -1,    -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,
      31,   139,   140,   141,    -1,    -1,   144,   145,   146,   116,
      -1,   149,   150,   151,   152,    -1,    -1,   155,    -1,    -1,
     158,   159,   160,    -1,    -1,   132,   133,    -1,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,    -1,
      81,   158,   159,   160,    85,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    94,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,    -1,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,    -1,    81,   158,   159,   160,
      85,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    94,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,    -1,
     135,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,    -1,    81,   158,   159,   160,    85,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    94,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,   154,   155,    -1,    81,   158,
     159,   160,    85,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,    -1,    81,   158,   159,   160,    85,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,    -1,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,    -1,
      81,   158,   159,   160,    85,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,    -1,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,    -1,    81,   158,   159,   160,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,    -1,
     135,   136,   137,    -1,   139,   140,   141,    31,   143,   144,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,    -1,    22,   158,   159,   160,    26,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,   132,   133,
      -1,   135,   136,   137,    -1,   139,   140,   141,    -1,   143,
     144,   145,   146,    -1,    -1,   149,   150,   151,   152,   153,
       1,   155,     3,    -1,   158,   159,   160,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,   157,    -1,    -1,
      -1,    -1,    33,   163,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,     3,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    17,    18,    -1,    20,    -1,    79,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    88,    89,    33,
      91,    -1,    93,    -1,    95,    39,    -1,    98,    -1,    -1,
      44,   102,    46,    -1,   105,    49,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,     3,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      17,    18,    -1,    20,    -1,    79,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    88,    89,    33,    91,    -1,    93,
      -1,    95,    39,    -1,    98,    -1,    -1,    44,   102,    46,
      -1,   105,    49,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      57,    -1,    -1,     3,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    17,    18,    -1,
      20,    -1,    79,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    88,    89,    33,    91,    -1,    93,    -1,    95,    39,
      -1,    98,    -1,    -1,    44,   102,    46,    -1,   105,    49,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,
       3,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,    79,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    88,    89,
      33,    91,    -1,    93,    -1,    95,    39,    -1,    98,    -1,
      -1,    44,   102,    46,    -1,   105,    49,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    -1,     3,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    17,    18,    -1,    20,    -1,    79,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    88,    89,    33,    91,    -1,
      93,    -1,    95,    39,    -1,    98,    -1,    -1,    44,   102,
      46,    -1,   105,    49,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    -1,    91,    -1,    93,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,   108
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   173,   174,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,    35,
      37,    38,    39,    40,    41,    42,    43,    44,    46,    47,
      49,    52,    53,    54,    56,    57,    58,    60,    61,    63,
      64,    65,    69,    70,    71,    72,    73,    74,    75,    78,
      79,    80,    82,    83,    84,    86,    87,    88,    89,    90,
      91,    92,    93,    95,    96,    98,    99,   100,   101,   102,
     104,   105,   107,   108,   111,   113,   116,   132,   133,   134,
     135,   136,   141,   151,   154,   155,   156,   157,   160,   162,
     164,   166,   175,   176,   177,   178,   179,   180,   182,   187,
     188,   189,   190,   194,   196,   199,   200,   202,   203,   204,
     209,   210,   212,   213,   214,   218,   219,   222,   223,   226,
     227,   230,   233,   234,   259,   262,   263,   283,   284,   285,
     286,   287,   288,   289,   297,   298,   299,   300,   301,   304,
     305,   306,   307,   308,   309,   310,   312,   313,   314,   315,
     316,   157,   177,   301,   112,   290,   291,     3,   191,    14,
      22,    35,    40,    41,    43,    80,    93,   162,   166,   230,
     283,   288,   299,   300,   301,   304,   306,   307,   290,   301,
     101,   262,    82,   191,   177,   277,   301,   177,   164,     8,
      80,   301,   302,     8,    11,    80,   101,   302,    71,   114,
     211,   301,   211,   301,    26,   263,   301,   301,   164,     3,
      17,    18,    20,    25,    33,    39,    44,    46,    49,    57,
      63,    64,    70,    79,    88,    89,    91,    93,    95,    98,
     102,   105,   108,   193,   195,   231,   164,   193,   264,   265,
      32,   182,   198,   301,   301,    18,    70,    88,   105,   301,
     301,   301,     8,   164,   201,    22,    35,    38,    80,   202,
       4,   156,   253,   276,   301,    99,   100,   157,   301,   303,
     301,   198,   301,   301,   301,    92,   164,   177,   301,   301,
     182,   190,   301,   304,   182,   190,   301,   193,   260,   301,
     301,   301,   301,   301,   301,   301,   301,     1,   163,   175,
     183,   276,   103,   142,   253,   278,   279,   303,   211,   276,
     301,   311,    73,   177,   162,    77,   181,   106,   193,   193,
      50,    76,   248,   264,   156,   157,   148,   301,    12,    19,
      31,    81,    85,   116,   132,   133,   135,   136,   137,   139,
     140,   141,   143,   144,   145,   146,   147,   149,   150,   151,
     152,   153,   154,   155,   158,   159,   160,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   161,   240,   164,   166,    81,    85,   164,   177,
     157,   301,   301,   301,   276,   162,   182,    45,   290,   260,
     264,   157,   138,   157,   111,   194,   253,   280,   281,   282,
     303,    80,   207,   234,   262,    80,   205,   260,   234,   262,
     193,   164,   198,    32,    45,   198,   112,   198,   293,    32,
      45,   198,   293,    36,    66,   157,    94,   182,   240,   280,
     177,   164,   242,   103,   164,   193,   266,   267,     1,   137,
     271,    45,   138,   177,   198,   164,   164,   198,   280,   202,
     193,   138,   157,   301,   301,   157,   162,   198,   164,   280,
     157,   215,   157,   215,   157,   117,   261,   157,   198,   157,
     163,   163,   175,   138,   163,   301,   138,   165,   138,   165,
     167,   293,    45,   138,   167,   293,   115,   138,   167,     8,
       1,   163,   183,    59,   276,   301,   137,   224,   162,   235,
     157,   301,   301,   301,   301,   211,   301,   211,   301,   301,
     301,   301,   301,   301,   301,    20,    33,    57,   101,   194,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   303,   303,   303,   303,
     303,   280,   280,   211,   301,   211,   301,    27,    45,    82,
     107,   292,   295,   296,   301,   316,    32,    45,    32,    45,
      94,    45,   167,   211,   301,   198,   157,   157,   301,   301,
     117,   165,   138,   208,   193,   264,   206,   193,   157,   264,
      45,   276,    43,   301,   211,   301,   164,   198,    43,   301,
     211,   301,   198,   160,   184,   186,   301,   184,   185,   177,
     301,    34,   301,   165,    27,    45,    48,    68,    71,    82,
     101,   176,   243,   244,   245,   246,   232,   267,   138,   165,
      33,    46,    89,    93,   166,   197,   272,   284,   117,   268,
     301,   265,   301,   301,   165,   253,   301,     1,   220,   280,
     165,    21,   216,   272,   284,   138,   163,   165,   165,   278,
     165,   278,   177,   167,   211,   301,   167,   177,   301,   167,
     301,   167,   301,   163,   163,   193,   157,    36,    66,   276,
     162,     1,   193,   228,   229,    27,    71,    82,   101,   237,
     247,   164,   157,   157,   157,   157,   157,   165,   167,    45,
      82,   138,   165,   283,    81,    81,    43,   211,   301,    43,
     211,   301,   211,   301,   290,   290,   157,   253,   303,   282,
     193,   224,   157,   193,   224,   157,   301,   165,   301,    32,
     198,    32,   198,   294,   295,   301,    32,   198,   293,    32,
     198,   293,   138,   157,    13,   157,    34,    34,   177,    94,
     182,    45,    82,   245,   138,   165,   164,   193,    27,    71,
      82,   101,   249,   165,   267,   271,     1,   276,    62,   303,
     165,   165,   163,    67,   109,   163,   221,   165,   164,   182,
     193,   217,   260,   177,   167,   293,   167,   293,   177,   115,
     186,   185,     1,   225,   163,   117,   138,   163,    82,   236,
       1,     3,    12,    17,    19,    20,    25,    39,    44,    49,
      51,    57,    63,    64,    79,    91,    95,    98,   102,   108,
     132,   133,   134,   135,   136,   139,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   155,   158,   159,
     160,   161,   164,   192,   193,   195,   238,   239,   240,   283,
     165,   295,   271,   283,   283,   301,    32,    32,   301,    32,
      32,   167,   167,   198,   198,   224,   162,   224,   162,   198,
      94,    43,   301,    43,   301,   138,   165,    94,    43,   301,
     198,    43,   301,   198,   301,   301,   177,   301,   177,    34,
     193,   244,   267,   137,   275,    82,   271,   268,   167,    45,
     167,   164,   164,    32,   177,   276,   217,   137,   182,    43,
     177,   301,   167,    43,   177,   301,   167,   301,   157,   157,
     163,    75,    78,   163,   176,   200,   301,   229,   249,   164,
     241,   193,   301,   132,   140,   241,   241,   268,    94,    43,
      43,    94,    43,    43,    43,    43,   162,   225,   162,   225,
     301,   301,   301,   295,   301,   301,   301,    13,    34,   177,
     275,   165,   166,   197,   253,   274,   284,   142,   254,   268,
      55,   110,   255,   301,   272,   284,   280,   280,   177,   198,
     165,   301,   301,   177,   301,   177,   200,     1,   137,   270,
     243,   165,     3,    95,   239,   240,   301,   301,   301,   301,
     301,   301,   225,   163,   225,   163,    94,    94,    94,    94,
     301,   177,   268,   275,   167,   276,   253,   301,     3,    84,
      95,   256,   257,   258,   301,   182,   199,   252,   167,   165,
     165,   182,    94,    94,   166,   197,   269,   284,    97,   250,
     165,   241,   241,    94,    94,    94,    94,    94,    94,   163,
     163,   301,   301,   301,   301,   254,   268,   253,   273,   274,
     284,    45,   167,   258,   110,   138,   117,   143,   145,   146,
     149,   150,    55,   284,   156,   156,   301,   301,     1,   167,
     276,   255,   301,   273,   274,   301,   257,   258,   258,   258,
     258,   258,   258,   256,   167,   269,   284,   167,   157,   251,
     252,   167,   269,   284,   273
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

  case 23:
/* Line 1792 of yacc.c  */
#line 535 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 540 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 542 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 543 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 544 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 545 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 546 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 547 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 548 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 549 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 550 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 555 "chapel.ypp"
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
#line 568 "chapel.ypp"
    { (yyvsp[(1) - (3)].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (3)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (3)]).prevModule;  // restore previous module name
    }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 573 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = (yyvsp[(3) - (4)].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 578 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 585 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 591 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 592 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 605 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 607 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 613 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 618 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 623 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 628 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 632 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 639 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 648 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 657 "chapel.ypp"
    { (yyval.b) = (currentModuleType != MOD_INTERNAL ? true : false); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 658 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 659 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 663 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (4)].pcallexpr), (yyvsp[(1) - (4)].b)); }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 664 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), (yyvsp[(5) - (6)].ponlylist), true, (yyvsp[(1) - (6)].b)); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 665 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), (yyvsp[(5) - (6)].ponlylist), false, (yyvsp[(1) - (6)].b)); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 669 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 674 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 676 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 678 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 680 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 682 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 694 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 698 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 702 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 703 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 704 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 705 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 706 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 718 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 719 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 730 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 731 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 732 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 733 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 734 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 737 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 738 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 739 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 740 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 742 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 743 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 744 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 745 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 748 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 758 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 759 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 760 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 761 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 762 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 770 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 771 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 772 "chapel.ypp"
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
#line 798 "chapel.ypp"
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 799 "chapel.ypp"
    { parsingPrivate=(yyvsp[(2) - (3)].b); applyPrivateToBlock((yyvsp[(3) - (3)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(3) - (3)].pblockstmt); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 813 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 814 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 815 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 816 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 821 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 826 "chapel.ypp"
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

  case 145:
/* Line 1792 of yacc.c  */
#line 836 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 841 "chapel.ypp"
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

  case 147:
/* Line 1792 of yacc.c  */
#line 852 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 857 "chapel.ypp"
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

  case 149:
/* Line 1792 of yacc.c  */
#line 867 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 872 "chapel.ypp"
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

  case 151:
/* Line 1792 of yacc.c  */
#line 883 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 887 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 892 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 900 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 911 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 917 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 918 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 919 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 920 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 921 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 922 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 923 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 924 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 925 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 926 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 927 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 928 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 929 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 930 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 931 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 932 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 933 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 934 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 936 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 942 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 948 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 954 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 960 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 967 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 974 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 978 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 984 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 988 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 989 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 990 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 991 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 992 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 994 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 996 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 998 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 1003 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1006 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 1007 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1008 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1009 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1010 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1011 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1016 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1022 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1026 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1031 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1035 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1037 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1041 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1042 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1047 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1049 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1051 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1058 "chapel.ypp"
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

  case 214:
/* Line 1792 of yacc.c  */
#line 1068 "chapel.ypp"
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

  case 215:
/* Line 1792 of yacc.c  */
#line 1081 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1086 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1091 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1099 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1105 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1107 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1114 "chapel.ypp"
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

  case 224:
/* Line 1792 of yacc.c  */
#line 1130 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1137 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1146 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1154 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1158 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1164 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1165 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1170 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1175 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1180 "chapel.ypp"
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

  case 234:
/* Line 1792 of yacc.c  */
#line 1202 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1205 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1209 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1217 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1227 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1234 "chapel.ypp"
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

  case 240:
/* Line 1792 of yacc.c  */
#line 1260 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1264 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1269 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1273 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1278 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1298 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1307 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1308 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1309 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1312 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1313 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1314 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1320 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1321 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1322 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1323 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1324 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1325 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1326 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1327 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1328 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1329 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1330 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1331 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1335 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1336 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1340 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1344 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1345 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1346 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1351 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1353 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1357 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1359 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1363 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1364 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1370 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1374 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1375 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1376 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1380 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1381 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1382 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1383 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1384 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1389 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1390 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1394 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1395 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1396 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1397 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1398 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1399 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1403 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1404 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1407 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1413 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1417 "chapel.ypp"
    { 
      DefExpr * de = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch)));
      USR_WARN(de, "support for space in query expressions is deprecated");
      (yyval.pexpr) = de;
  }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1422 "chapel.ypp"
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1426 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1427 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1428 "chapel.ypp"
    { if (DefExpr* def = toDefExpr((yyvsp[(2) - (2)].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
                         }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1436 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1438 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1440 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1442 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1444 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1449 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1451 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1455 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1456 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1457 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1458 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1459 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1460 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1461 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1466 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1473 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1482 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1495 "chapel.ypp"
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

  case 355:
/* Line 1792 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1528 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1534 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1543 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1553 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1560 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1569 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1571 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1577 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1578 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1579 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1587 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1593 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1597 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1603 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1610 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1611 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1612 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1613 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1614 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
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
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
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
#line 1644 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1648 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1652 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1660 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1666 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1667 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1668 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1673 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1681 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1683 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1685 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1690 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1691 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1692 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1693 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1699 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1701 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1707 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1711 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1712 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1713 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1722 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1727 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1728 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1732 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1733 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1734 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1735 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1739 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1740 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1752 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1754 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1756 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1767 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1769 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1771 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1773 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1775 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1777 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1779 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1782 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1784 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1786 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1788 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1790 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1792 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1794 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1796 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1799 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1801 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1803 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1808 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1810 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1812 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1814 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1816 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1818 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1820 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1822 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1824 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1826 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1828 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1830 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1832 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1839 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1845 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1851 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1857 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1866 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1875 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1891 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1895 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1900 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1904 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1905 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1909 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1913 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1914 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1919 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1923 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1927 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1933 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1934 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1935 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1936 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1937 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1938 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1944 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1949 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1951 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1955 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 1959 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1963 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1971 "chapel.ypp"
    { (yyvsp[(1) - (2)].pcallexpr)->insertAtTail((yyvsp[(2) - (2)].pexpr));
      (yyval.pexpr) = (yyvsp[(1) - (2)].pcallexpr); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1976 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1982 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1988 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1995 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 2004 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtOwned->symbol, (yyvsp[(4) - (5)].pexpr))); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 2006 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtShared->symbol, (yyvsp[(4) - (5)].pexpr))); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 2011 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 2027 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 2029 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 2031 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 2033 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 2035 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 2037 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 2041 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 2042 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 2046 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 2047 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 2048 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 2066 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 2067 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 2068 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr));}
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 2069 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 2073 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 2074 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 2075 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 2079 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 2080 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 2081 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 2082 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 2083 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 2091 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 2092 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 2093 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 2094 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 2098 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 2099 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 2104 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2105 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2106 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2107 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 2108 "chapel.ypp"
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2109 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 2110 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 2111 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2112 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2113 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2114 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2116 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2120 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2127 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2128 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2132 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2133 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2134 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2135 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2136 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2137 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2138 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2139 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2140 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 584:
/* Line 1792 of yacc.c  */
#line 2141 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 585:
/* Line 1792 of yacc.c  */
#line 2142 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 586:
/* Line 1792 of yacc.c  */
#line 2143 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 587:
/* Line 1792 of yacc.c  */
#line 2144 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 588:
/* Line 1792 of yacc.c  */
#line 2145 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 589:
/* Line 1792 of yacc.c  */
#line 2146 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 590:
/* Line 1792 of yacc.c  */
#line 2147 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 591:
/* Line 1792 of yacc.c  */
#line 2148 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 592:
/* Line 1792 of yacc.c  */
#line 2149 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 593:
/* Line 1792 of yacc.c  */
#line 2150 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 594:
/* Line 1792 of yacc.c  */
#line 2151 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 595:
/* Line 1792 of yacc.c  */
#line 2152 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 596:
/* Line 1792 of yacc.c  */
#line 2153 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 597:
/* Line 1792 of yacc.c  */
#line 2154 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 598:
/* Line 1792 of yacc.c  */
#line 2158 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 599:
/* Line 1792 of yacc.c  */
#line 2159 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 600:
/* Line 1792 of yacc.c  */
#line 2160 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 601:
/* Line 1792 of yacc.c  */
#line 2161 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 602:
/* Line 1792 of yacc.c  */
#line 2162 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 603:
/* Line 1792 of yacc.c  */
#line 2163 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 604:
/* Line 1792 of yacc.c  */
#line 2164 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 605:
/* Line 1792 of yacc.c  */
#line 2168 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 606:
/* Line 1792 of yacc.c  */
#line 2169 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 607:
/* Line 1792 of yacc.c  */
#line 2170 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 608:
/* Line 1792 of yacc.c  */
#line 2171 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 609:
/* Line 1792 of yacc.c  */
#line 2175 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 610:
/* Line 1792 of yacc.c  */
#line 2176 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 611:
/* Line 1792 of yacc.c  */
#line 2177 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 612:
/* Line 1792 of yacc.c  */
#line 2178 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 613:
/* Line 1792 of yacc.c  */
#line 2183 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 614:
/* Line 1792 of yacc.c  */
#line 2184 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 615:
/* Line 1792 of yacc.c  */
#line 2185 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 616:
/* Line 1792 of yacc.c  */
#line 2186 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 617:
/* Line 1792 of yacc.c  */
#line 2187 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 618:
/* Line 1792 of yacc.c  */
#line 2188 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 619:
/* Line 1792 of yacc.c  */
#line 2189 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 10192 "bison-chapel.cpp"
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


