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
     TLABEL = 303,
     TLAMBDA = 304,
     TLET = 305,
     TLIFETIME = 306,
     TLOCAL = 307,
     TLOCALE = 308,
     TMINUSMINUS = 309,
     TMODULE = 310,
     TNEW = 311,
     TNIL = 312,
     TNOINIT = 313,
     TON = 314,
     TONLY = 315,
     TOTHERWISE = 316,
     TOUT = 317,
     TOVERRIDE = 318,
     TOWNED = 319,
     TPARAM = 320,
     TPLUSPLUS = 321,
     TPRAGMA = 322,
     TPRIMITIVE = 323,
     TPRIVATE = 324,
     TPROC = 325,
     TPROTOTYPE = 326,
     TPUBLIC = 327,
     TREAL = 328,
     TRECORD = 329,
     TREDUCE = 330,
     TREF = 331,
     TREQUIRE = 332,
     TRETURN = 333,
     TSCAN = 334,
     TSELECT = 335,
     TSERIAL = 336,
     TSHARED = 337,
     TSINGLE = 338,
     TSPARSE = 339,
     TSTRING = 340,
     TSUBDOMAIN = 341,
     TSYNC = 342,
     TTHEN = 343,
     TTHIS = 344,
     TTHROW = 345,
     TTHROWS = 346,
     TTRUE = 347,
     TTRY = 348,
     TTRYBANG = 349,
     TTYPE = 350,
     TUINT = 351,
     TUNDERSCORE = 352,
     TUNION = 353,
     TUNMANAGED = 354,
     TUSE = 355,
     TVAR = 356,
     TWHEN = 357,
     TWHERE = 358,
     TWHILE = 359,
     TWITH = 360,
     TYIELD = 361,
     TZIP = 362,
     TALIAS = 363,
     TAND = 364,
     TASSIGN = 365,
     TASSIGNBAND = 366,
     TASSIGNBOR = 367,
     TASSIGNBXOR = 368,
     TASSIGNDIVIDE = 369,
     TASSIGNEXP = 370,
     TASSIGNLAND = 371,
     TASSIGNLOR = 372,
     TASSIGNMINUS = 373,
     TASSIGNMOD = 374,
     TASSIGNMULTIPLY = 375,
     TASSIGNPLUS = 376,
     TASSIGNREDUCE = 377,
     TASSIGNSL = 378,
     TASSIGNSR = 379,
     TBAND = 380,
     TBNOT = 381,
     TBOR = 382,
     TBXOR = 383,
     TCOLON = 384,
     TCOMMA = 385,
     TDIVIDE = 386,
     TDOT = 387,
     TDOTDOT = 388,
     TDOTDOTDOT = 389,
     TEQUAL = 390,
     TEXP = 391,
     TGREATER = 392,
     TGREATEREQUAL = 393,
     THASH = 394,
     TIO = 395,
     TLESS = 396,
     TLESSEQUAL = 397,
     TMINUS = 398,
     TMOD = 399,
     TNOT = 400,
     TNOTEQUAL = 401,
     TOR = 402,
     TPLUS = 403,
     TQUESTION = 404,
     TSEMI = 405,
     TSHIFTLEFT = 406,
     TSHIFTRIGHT = 407,
     TSTAR = 408,
     TSWAP = 409,
     TLCBR = 410,
     TRCBR = 411,
     TLP = 412,
     TRP = 413,
     TLSBR = 414,
     TRSBR = 415,
     TNOELSE = 416,
     TUMINUS = 417,
     TUPLUS = 418
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
#line 478 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 485 "bison-chapel.cpp"
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
#line 564 "bison-chapel.cpp"

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
#define YYLAST   15772

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  164
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  138
/* YYNRULES -- Number of rules.  */
#define YYNRULES  575
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1028

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   418

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
     155,   156,   157,   158,   159,   160,   161,   162,   163
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
     443,   450,   454,   459,   466,   474,   481,   489,   494,   500,
     505,   510,   514,   521,   527,   534,   540,   549,   557,   560,
     564,   568,   571,   574,   578,   582,   583,   586,   589,   593,
     599,   601,   605,   609,   615,   621,   622,   625,   629,   632,
     636,   643,   652,   659,   668,   670,   672,   674,   675,   678,
     679,   682,   686,   692,   698,   700,   702,   705,   709,   711,
     715,   716,   717,   726,   727,   729,   732,   735,   737,   738,
     739,   750,   754,   758,   764,   770,   774,   776,   780,   782,
     785,   787,   789,   791,   793,   795,   797,   799,   801,   803,
     805,   807,   809,   811,   813,   815,   817,   819,   821,   823,
     825,   827,   829,   831,   833,   835,   837,   839,   841,   843,
     845,   847,   849,   851,   853,   855,   857,   858,   862,   866,
     867,   869,   873,   878,   884,   889,   896,   903,   904,   906,
     908,   910,   912,   914,   917,   920,   922,   924,   926,   927,
     929,   931,   934,   936,   938,   940,   942,   943,   945,   948,
     950,   952,   954,   955,   957,   959,   961,   963,   965,   968,
     970,   971,   973,   976,   979,   980,   983,   986,   991,   996,
     998,  1002,  1006,  1010,  1014,  1018,  1022,  1026,  1029,  1031,
    1033,  1037,  1042,  1047,  1050,  1055,  1056,  1059,  1062,  1064,
    1066,  1068,  1071,  1073,  1078,  1084,  1088,  1090,  1094,  1098,
    1104,  1106,  1108,  1112,  1114,  1117,  1121,  1122,  1125,  1128,
    1132,  1135,  1140,  1144,  1148,  1153,  1157,  1158,  1161,  1164,
    1167,  1169,  1170,  1173,  1176,  1179,  1181,  1186,  1191,  1198,
    1202,  1203,  1205,  1207,  1211,  1216,  1220,  1225,  1232,  1233,
    1236,  1239,  1242,  1245,  1247,  1249,  1253,  1257,  1259,  1263,
    1265,  1267,  1269,  1273,  1277,  1278,  1280,  1282,  1286,  1290,
    1294,  1296,  1298,  1300,  1302,  1304,  1306,  1308,  1310,  1312,
    1315,  1320,  1325,  1330,  1336,  1339,  1342,  1345,  1348,  1351,
    1354,  1361,  1368,  1373,  1383,  1393,  1401,  1408,  1415,  1420,
    1430,  1440,  1448,  1453,  1460,  1467,  1477,  1487,  1494,  1496,
    1498,  1500,  1502,  1504,  1506,  1508,  1510,  1514,  1515,  1517,
    1522,  1524,  1528,  1533,  1535,  1539,  1544,  1548,  1552,  1554,
    1556,  1559,  1561,  1564,  1566,  1568,  1572,  1575,  1580,  1582,
    1584,  1586,  1588,  1590,  1592,  1594,  1596,  1601,  1605,  1609,
    1613,  1617,  1620,  1623,  1625,  1626,  1628,  1631,  1634,  1636,
    1638,  1640,  1642,  1644,  1646,  1648,  1653,  1658,  1663,  1667,
    1671,  1675,  1679,  1683,  1688,  1692,  1697,  1699,  1701,  1703,
    1705,  1707,  1709,  1711,  1713,  1717,  1722,  1726,  1731,  1735,
    1740,  1744,  1750,  1754,  1758,  1762,  1766,  1770,  1774,  1778,
    1782,  1786,  1790,  1794,  1798,  1802,  1806,  1810,  1814,  1818,
    1822,  1826,  1830,  1834,  1838,  1842,  1845,  1848,  1851,  1854,
    1857,  1860,  1864,  1868,  1872,  1876,  1880,  1884,  1888,  1892,
    1894,  1896,  1898,  1900,  1902,  1904
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     165,     0,    -1,   166,    -1,    -1,   166,   167,    -1,   169,
      -1,   168,   169,    -1,    67,     7,    -1,   168,    67,     7,
      -1,   171,    -1,   174,    -1,   179,    -1,   180,    -1,   192,
      -1,   181,    -1,   195,    -1,   198,    -1,   196,    -1,   205,
      -1,   199,    -1,   200,    -1,   204,    -1,   191,    -1,   275,
     150,    -1,    12,   169,    -1,    13,   276,   169,    -1,    14,
     182,   150,    -1,    20,   276,   174,    -1,    25,   182,   150,
      -1,    27,   264,   150,    -1,    48,   184,   169,    -1,    52,
     286,   190,    -1,    52,   190,    -1,    59,   286,   190,    -1,
      81,   286,   190,    -1,    81,   190,    -1,    87,   169,    -1,
     106,   286,   150,    -1,     1,   150,    -1,   172,   173,    55,
     184,    -1,   170,   155,   156,    -1,   170,   155,   175,   156,
      -1,   170,   155,     1,   156,    -1,    -1,    72,    -1,    69,
      -1,    -1,    71,    -1,   155,   156,    -1,   155,   175,   156,
      -1,   155,     1,   156,    -1,   167,    -1,   175,   167,    -1,
     286,    -1,   286,    11,   286,    -1,   176,   130,   286,    -1,
     176,   130,   286,    11,   286,    -1,    -1,   176,    -1,   153,
      -1,   176,    -1,   100,   263,   150,    -1,   100,   286,    33,
     178,   150,    -1,   100,   286,    60,   177,   150,    -1,    77,
     263,   150,    -1,   289,   226,   288,   150,    -1,   289,   154,
     288,   150,    -1,   289,   122,   288,   150,    -1,   289,   116,
     288,   150,    -1,   289,   117,   288,   150,    -1,    -1,     3,
      -1,     3,    -1,    89,    -1,    36,    -1,    92,    -1,   186,
      -1,     3,    -1,    89,    -1,    36,    -1,    92,    -1,   186,
      -1,     3,    -1,    89,    -1,    15,    -1,    46,    -1,    96,
      -1,    73,    -1,    41,    -1,    22,    -1,    85,    -1,    87,
      -1,    83,    -1,    64,    -1,    82,    -1,    16,    -1,    99,
      -1,    30,    -1,    43,    -1,    53,    -1,    15,    -1,    46,
      -1,    96,    -1,    73,    -1,    41,    -1,    22,    -1,    85,
      -1,    53,    -1,    87,    -1,    83,    -1,    64,    -1,    82,
      -1,    16,    -1,    99,    -1,    30,    -1,    43,    -1,    -1,
       7,    -1,    29,   169,    -1,   174,    -1,    78,   150,    -1,
      78,   288,   150,    -1,   150,    -1,   193,    -1,    72,   193,
      -1,    69,   193,    -1,   212,    -1,   246,    -1,   208,    -1,
     194,    -1,   220,    -1,   250,    -1,    39,   286,   150,    -1,
      39,   286,    33,   178,   150,    -1,    39,   286,    60,   177,
     150,    -1,    39,   250,    -1,    35,     9,    -1,    29,   169,
     104,   286,   150,    -1,   104,   286,   190,    -1,    21,   286,
      42,   286,   276,   190,    -1,    21,   286,    42,   197,   276,
     190,    -1,    21,   286,   276,   190,    -1,    37,   286,    42,
     286,   190,    -1,    37,   286,    42,   197,   190,    -1,    37,
     286,   190,    -1,    37,   197,   190,    -1,    37,    65,   184,
      42,   286,   190,    -1,    38,   286,    42,   286,   190,    -1,
      38,   286,    42,   286,   279,   190,    -1,    38,   286,    42,
     197,   190,    -1,    38,   286,    42,   197,   279,   190,    -1,
      38,   286,   190,    -1,    38,   286,   279,   190,    -1,   159,
     263,    42,   286,   160,   169,    -1,   159,   263,    42,   286,
     279,   160,   169,    -1,   159,   263,    42,   197,   160,   169,
      -1,   159,   263,    42,   197,   279,   160,   169,    -1,   159,
     263,   160,   169,    -1,   159,   263,   279,   160,   169,    -1,
     107,   157,   263,   158,    -1,    40,   286,    88,   169,    -1,
      40,   286,   174,    -1,    40,   286,    88,   169,    31,   169,
      -1,    40,   286,   174,    31,   169,    -1,    40,   286,   226,
     286,    88,   169,    -1,    40,   286,   226,   286,   174,    -1,
      40,   286,   226,   286,    88,   169,    31,   169,    -1,    40,
     286,   226,   286,   174,    31,   169,    -1,    26,   169,    -1,
      93,   286,   150,    -1,    94,   286,   150,    -1,    93,   181,
      -1,    94,   181,    -1,    93,   174,   201,    -1,    94,   174,
     201,    -1,    -1,   201,   202,    -1,    18,   174,    -1,    18,
     203,   174,    -1,    18,   157,   203,   158,   174,    -1,   184,
      -1,   184,   129,   286,    -1,    90,   286,   150,    -1,    80,
     286,   155,   206,   156,    -1,    80,   286,   155,     1,   156,
      -1,    -1,   206,   207,    -1,   102,   263,   190,    -1,    61,
     169,    -1,    61,    29,   169,    -1,   209,   184,   210,   155,
     211,   156,    -1,    35,   189,   209,   184,   210,   155,   211,
     156,    -1,   209,   184,   210,   155,     1,   156,    -1,    35,
     189,   209,   184,   210,   155,     1,   156,    -1,    19,    -1,
      74,    -1,    98,    -1,    -1,   129,   263,    -1,    -1,   211,
     192,    -1,   211,   168,   192,    -1,   213,   184,   155,   214,
     156,    -1,   213,   184,   155,     1,   156,    -1,    32,    -1,
     215,    -1,   214,   130,    -1,   214,   130,   215,    -1,   184,
      -1,   184,   110,   286,    -1,    -1,    -1,    49,   217,   228,
     218,   235,   258,   242,   238,    -1,    -1,    44,    -1,    34,
     287,    -1,    35,   287,    -1,    63,    -1,    -1,    -1,   219,
     234,   221,   223,   222,   235,   257,   236,   242,   237,    -1,
     233,   225,   227,    -1,   233,   226,   227,    -1,   233,   224,
     132,   225,   227,    -1,   233,   224,   132,   226,   227,    -1,
     233,     1,   227,    -1,   270,    -1,   157,   286,   158,    -1,
     183,    -1,   126,   184,    -1,   125,    -1,   127,    -1,   128,
      -1,   126,    -1,   135,    -1,   146,    -1,   142,    -1,   138,
      -1,   141,    -1,   137,    -1,   148,    -1,   143,    -1,   153,
      -1,   131,    -1,   151,    -1,   152,    -1,   144,    -1,   136,
      -1,   145,    -1,    17,    -1,   139,    -1,    10,    -1,   154,
      -1,   140,    -1,   110,    -1,   121,    -1,   118,    -1,   120,
      -1,   114,    -1,   119,    -1,   115,    -1,   111,    -1,   112,
      -1,   113,    -1,   124,    -1,   123,    -1,    -1,   157,   229,
     158,    -1,   157,   229,   158,    -1,    -1,   230,    -1,   229,
     130,   230,    -1,   231,   184,   262,   255,    -1,   168,   231,
     184,   262,   255,    -1,   231,   184,   262,   241,    -1,   231,
     157,   254,   158,   262,   255,    -1,   231,   157,   254,   158,
     262,   241,    -1,    -1,   232,    -1,    42,    -1,    45,    -1,
      62,    -1,    24,    -1,    24,    42,    -1,    24,    76,    -1,
      65,    -1,    76,    -1,    95,    -1,    -1,    65,    -1,    76,
      -1,    24,    76,    -1,    24,    -1,    95,    -1,    70,    -1,
      47,    -1,    -1,    24,    -1,    24,    76,    -1,    76,    -1,
      65,    -1,    95,    -1,    -1,    91,    -1,   150,    -1,   238,
      -1,   174,    -1,   191,    -1,   149,   184,    -1,   149,    -1,
      -1,   239,    -1,   134,   286,    -1,   134,   240,    -1,    -1,
     103,   286,    -1,    51,   243,    -1,   103,   286,    51,   243,
      -1,    51,   243,   103,   286,    -1,   244,    -1,   243,   130,
     244,    -1,   245,   110,   245,    -1,   245,   141,   245,    -1,
     245,   142,   245,    -1,   245,   135,   245,    -1,   245,   137,
     245,    -1,   245,   138,   245,    -1,    78,   245,    -1,     3,
      -1,    89,    -1,    95,   247,   150,    -1,    23,    95,   247,
     150,    -1,    35,    95,   247,   150,    -1,   184,   248,    -1,
     184,   248,   130,   247,    -1,    -1,   110,   271,    -1,   110,
     259,    -1,    65,    -1,    24,    -1,    76,    -1,    24,    76,
      -1,   101,    -1,    23,   249,   251,   150,    -1,    35,   287,
     249,   251,   150,    -1,   249,   251,   150,    -1,   252,    -1,
     251,   130,   252,    -1,   184,   258,   255,    -1,   157,   254,
     158,   258,   255,    -1,    97,    -1,   184,    -1,   157,   254,
     158,    -1,   253,    -1,   253,   130,    -1,   253,   130,   254,
      -1,    -1,   110,    58,    -1,   110,   288,    -1,   159,   160,
     271,    -1,   159,   160,    -1,   159,   263,   160,   271,    -1,
     159,   263,   160,    -1,   159,   160,   256,    -1,   159,   263,
     160,   256,    -1,   159,     1,   160,    -1,    -1,   129,   271,
      -1,   129,   256,    -1,   129,   188,    -1,     1,    -1,    -1,
     129,   271,    -1,   129,   259,    -1,   129,   188,    -1,     1,
      -1,   159,   263,   160,   271,    -1,   159,   263,   160,   259,
      -1,   159,   263,    42,   286,   160,   271,    -1,   159,     1,
     160,    -1,    -1,   271,    -1,   239,    -1,   159,   160,   260,
      -1,   159,   263,   160,   260,    -1,   159,   160,   261,    -1,
     159,   263,   160,   261,    -1,   159,   263,    42,   286,   160,
     260,    -1,    -1,   129,   271,    -1,   129,   239,    -1,   129,
     188,    -1,   129,   261,    -1,   286,    -1,   239,    -1,   263,
     130,   286,    -1,   263,   130,   239,    -1,   286,    -1,   264,
     130,   286,    -1,    97,    -1,   288,    -1,   239,    -1,   265,
     130,   265,    -1,   266,   130,   265,    -1,    -1,   268,    -1,
     269,    -1,   268,   130,   269,    -1,   185,   110,   239,    -1,
     185,   110,   288,    -1,   239,    -1,   288,    -1,   185,    -1,
     187,    -1,   274,    -1,   289,    -1,   273,    -1,   298,    -1,
     297,    -1,    83,   286,    -1,    43,   157,   267,   158,    -1,
      30,   157,   267,   158,    -1,    86,   157,   267,   158,    -1,
      84,    86,   157,   267,   158,    -1,    12,   286,    -1,    87,
     286,    -1,    16,   286,    -1,    64,   286,    -1,    99,   286,
      -1,    82,   286,    -1,    37,   286,    42,   286,    29,   286,
      -1,    37,   286,    42,   197,    29,   286,    -1,    37,   286,
      29,   286,    -1,    37,   286,    42,   286,    29,    40,   286,
      88,   286,    -1,    37,   286,    42,   197,    29,    40,   286,
      88,   286,    -1,    37,   286,    29,    40,   286,    88,   286,
      -1,    38,   286,    42,   286,    29,   286,    -1,    38,   286,
      42,   197,    29,   286,    -1,    38,   286,    29,   286,    -1,
      38,   286,    42,   286,    29,    40,   286,    88,   286,    -1,
      38,   286,    42,   197,    29,    40,   286,    88,   286,    -1,
      38,   286,    29,    40,   286,    88,   286,    -1,   159,   263,
     160,   286,    -1,   159,   263,    42,   286,   160,   286,    -1,
     159,   263,    42,   197,   160,   286,    -1,   159,   263,    42,
     286,   160,    40,   286,    88,   286,    -1,   159,   263,    42,
     197,   160,    40,   286,    88,   286,    -1,    40,   286,    88,
     286,    31,   286,    -1,    57,    -1,   274,    -1,   270,    -1,
     292,    -1,   291,    -1,   216,    -1,   284,    -1,   285,    -1,
     283,   140,   286,    -1,    -1,   277,    -1,   105,   157,   278,
     158,    -1,   281,    -1,   278,   130,   281,    -1,   105,   157,
     280,   158,    -1,   281,    -1,   280,   130,   281,    -1,   282,
     270,   258,   255,    -1,   301,    75,   270,    -1,   286,    75,
     270,    -1,    24,    -1,    42,    -1,    24,    42,    -1,    76,
      -1,    24,    76,    -1,   101,    -1,   289,    -1,   283,   140,
     286,    -1,    56,   286,    -1,    50,   251,    42,   286,    -1,
     295,    -1,   271,    -1,   272,    -1,   299,    -1,   300,    -1,
     216,    -1,   284,    -1,   285,    -1,   157,   134,   286,   158,
      -1,   286,   129,   286,    -1,   286,   129,    16,    -1,   286,
     129,    99,    -1,   286,   133,   286,    -1,   286,   133,    -1,
     133,   286,    -1,   133,    -1,    -1,   286,    -1,    93,   286,
      -1,    94,   286,    -1,   286,    -1,   270,    -1,   291,    -1,
     292,    -1,   293,    -1,   289,    -1,   216,    -1,   290,   157,
     267,   158,    -1,   290,   159,   267,   160,    -1,    68,   157,
     267,   158,    -1,   286,   132,   185,    -1,   286,   132,    95,
      -1,   286,   132,    30,    -1,   286,   132,    53,    -1,   157,
     265,   158,    -1,   157,   265,   130,   158,    -1,   157,   266,
     158,    -1,   157,   266,   130,   158,    -1,    36,    -1,    92,
      -1,   294,    -1,     4,    -1,     5,    -1,     6,    -1,     7,
      -1,     8,    -1,   155,   263,   156,    -1,   155,   263,   130,
     156,    -1,   159,   263,   160,    -1,   159,   263,   130,   160,
      -1,   159,   296,   160,    -1,   159,   296,   130,   160,    -1,
     286,   108,   286,    -1,   296,   130,   286,   108,   286,    -1,
     286,   148,   286,    -1,   286,   143,   286,    -1,   286,   153,
     286,    -1,   286,   131,   286,    -1,   286,   151,   286,    -1,
     286,   152,   286,    -1,   286,   144,   286,    -1,   286,   135,
     286,    -1,   286,   146,   286,    -1,   286,   142,   286,    -1,
     286,   138,   286,    -1,   286,   141,   286,    -1,   286,   137,
     286,    -1,   286,   125,   286,    -1,   286,   127,   286,    -1,
     286,   128,   286,    -1,   286,   109,   286,    -1,   286,   147,
     286,    -1,   286,   136,   286,    -1,   286,    17,   286,    -1,
     286,    10,   286,    -1,   286,   139,   286,    -1,   286,    28,
     286,    -1,   148,   286,    -1,   143,   286,    -1,    54,   286,
      -1,    66,   286,    -1,   145,   286,    -1,   126,   286,    -1,
     286,    75,   286,    -1,   286,    75,   197,    -1,   301,    75,
     286,    -1,   301,    75,   197,    -1,   286,    79,   286,    -1,
     286,    79,   197,    -1,   301,    79,   286,    -1,   301,    79,
     197,    -1,   148,    -1,   153,    -1,   109,    -1,   147,    -1,
     125,    -1,   127,    -1,   128,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   487,   487,   492,   493,   499,   500,   505,   506,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,   539,   540,   544,
     557,   562,   567,   575,   576,   577,   581,   582,   595,   596,
     597,   602,   603,   608,   613,   618,   622,   629,   634,   638,
     643,   647,   648,   649,   653,   657,   659,   661,   663,   665,
     672,   673,   677,   678,   679,   680,   681,   684,   685,   686,
     687,   688,   700,   701,   712,   713,   714,   715,   716,   717,
     718,   719,   720,   721,   722,   723,   724,   725,   726,   727,
     731,   732,   733,   734,   735,   736,   737,   738,   745,   746,
     747,   748,   749,   750,   751,   752,   757,   758,   762,   763,
     767,   768,   772,   773,   774,   775,   776,   777,   778,   779,
     783,   784,   788,   789,   790,   791,   795,   802,   803,   804,
     805,   806,   807,   808,   809,   810,   811,   812,   813,   814,
     815,   816,   817,   818,   824,   830,   836,   842,   849,   859,
     863,   864,   865,   866,   867,   869,   871,   873,   878,   881,
     882,   883,   884,   885,   886,   890,   891,   895,   896,   897,
     901,   902,   906,   909,   911,   916,   917,   921,   923,   925,
     932,   942,   952,   962,   975,   980,   985,   993,   994,   999,
    1000,  1002,  1007,  1023,  1030,  1039,  1047,  1051,  1058,  1059,
    1064,  1069,  1063,  1096,  1102,  1109,  1115,  1121,  1132,  1138,
    1131,  1170,  1174,  1179,  1183,  1188,  1195,  1196,  1200,  1201,
    1202,  1203,  1204,  1205,  1206,  1207,  1208,  1209,  1210,  1211,
    1212,  1213,  1214,  1215,  1216,  1217,  1218,  1219,  1220,  1221,
    1222,  1223,  1224,  1225,  1229,  1230,  1231,  1232,  1233,  1234,
    1235,  1236,  1237,  1238,  1239,  1240,  1244,  1245,  1249,  1253,
    1254,  1255,  1259,  1261,  1263,  1265,  1267,  1272,  1273,  1277,
    1278,  1279,  1280,  1281,  1282,  1283,  1284,  1285,  1289,  1290,
    1291,  1292,  1293,  1294,  1298,  1299,  1303,  1304,  1305,  1306,
    1307,  1308,  1312,  1313,  1316,  1317,  1321,  1322,  1326,  1328,
    1333,  1334,  1338,  1339,  1343,  1344,  1346,  1348,  1350,  1355,
    1356,  1359,  1360,  1361,  1362,  1363,  1364,  1365,  1368,  1369,
    1372,  1374,  1376,  1381,  1394,  1411,  1412,  1414,  1419,  1420,
    1421,  1422,  1423,  1427,  1433,  1439,  1447,  1448,  1456,  1458,
    1463,  1465,  1467,  1472,  1474,  1476,  1483,  1484,  1485,  1490,
    1492,  1494,  1498,  1502,  1504,  1508,  1516,  1517,  1518,  1519,
    1520,  1525,  1526,  1527,  1528,  1529,  1549,  1553,  1557,  1565,
    1572,  1573,  1574,  1578,  1580,  1586,  1588,  1590,  1595,  1596,
    1597,  1598,  1599,  1605,  1606,  1607,  1608,  1612,  1613,  1617,
    1618,  1619,  1623,  1624,  1628,  1629,  1633,  1634,  1638,  1639,
    1640,  1641,  1645,  1646,  1657,  1658,  1659,  1660,  1661,  1662,
    1664,  1666,  1668,  1670,  1672,  1674,  1676,  1678,  1680,  1682,
    1687,  1689,  1691,  1693,  1695,  1697,  1699,  1701,  1703,  1705,
    1707,  1709,  1711,  1718,  1724,  1730,  1736,  1745,  1755,  1763,
    1764,  1765,  1766,  1767,  1768,  1769,  1770,  1775,  1776,  1780,
    1784,  1785,  1789,  1793,  1794,  1798,  1802,  1806,  1813,  1814,
    1815,  1816,  1817,  1818,  1822,  1823,  1828,  1833,  1841,  1842,
    1843,  1844,  1845,  1846,  1847,  1848,  1849,  1851,  1853,  1855,
    1857,  1859,  1861,  1863,  1868,  1869,  1872,  1873,  1874,  1877,
    1878,  1879,  1880,  1891,  1892,  1896,  1897,  1898,  1902,  1903,
    1904,  1905,  1913,  1914,  1915,  1916,  1920,  1921,  1925,  1926,
    1927,  1928,  1929,  1930,  1931,  1932,  1933,  1934,  1935,  1939,
    1947,  1948,  1952,  1953,  1954,  1955,  1956,  1957,  1958,  1959,
    1960,  1961,  1962,  1963,  1964,  1965,  1966,  1967,  1968,  1969,
    1970,  1971,  1972,  1973,  1974,  1978,  1979,  1980,  1981,  1982,
    1983,  1987,  1988,  1989,  1990,  1994,  1995,  1996,  1997,  2002,
    2003,  2004,  2005,  2006,  2007,  2008
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
  "TINOUT", "TINT", "TITER", "TLABEL", "TLAMBDA", "TLET", "TLIFETIME",
  "TLOCAL", "TLOCALE", "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT",
  "TON", "TONLY", "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED", "TPARAM",
  "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE",
  "TPUBLIC", "TREAL", "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN",
  "TSCAN", "TSELECT", "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE",
  "TSTRING", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS", "TTHROW", "TTHROWS",
  "TTRUE", "TTRY", "TTRYBANG", "TTYPE", "TUINT", "TUNDERSCORE", "TUNION",
  "TUNMANAGED", "TUSE", "TVAR", "TWHEN", "TWHERE", "TWHILE", "TWITH",
  "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND",
  "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP",
  "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL",
  "TASSIGNSR", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA",
  "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER",
  "TGREATEREQUAL", "THASH", "TIO", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD",
  "TNOT", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT",
  "TSHIFTRIGHT", "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR",
  "TRSBR", "TNOELSE", "TUMINUS", "TUPLUS", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_start", "module_decl_stmt", "access_control",
  "opt_prototype", "block_stmt", "stmt_ls", "only_ls", "opt_only_ls",
  "except_ls", "use_stmt", "require_stmt", "assignment_stmt",
  "opt_label_ident", "ident_fn_def", "ident_def", "ident_use",
  "internal_type_ident_def", "scalar_type", "reserved_type_ident_use",
  "opt_string", "do_stmt", "return_stmt", "class_level_stmt",
  "private_decl", "forwarding_stmt", "extern_block_stmt", "loop_stmt",
  "zippered_iterator", "if_stmt", "defer_stmt", "try_stmt",
  "catch_stmt_ls", "catch_stmt", "catch_expr", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt", "enum_header",
  "enum_ls", "enum_item", "lambda_decl_expr", "$@1", "$@2", "linkage_spec",
  "fn_decl_stmt", "$@3", "$@4", "fn_decl_stmt_inner",
  "fn_decl_receiver_expr", "fn_ident", "assignop_ident", "opt_formal_ls",
  "req_formal_ls", "formal_ls", "formal", "opt_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_or_iter",
  "opt_ret_tag", "opt_throws_error", "opt_function_body_stmt",
  "function_body_stmt", "query_expr", "opt_query_expr", "var_arg_expr",
  "opt_lifetime_where", "lifetime_components_expr", "lifetime_expr",
  "lifetime_ident", "type_alias_decl_stmt", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_type", "var_decl_stmt",
  "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
  "tuple_var_decl_component", "tuple_var_decl_stmt_inner_ls",
  "opt_init_expr", "ret_array_type", "opt_ret_type", "opt_type",
  "array_type", "opt_formal_array_elt_type", "formal_array_type",
  "opt_formal_type", "expr_ls", "simple_expr_ls", "tuple_component",
  "tuple_expr_ls", "opt_actual_ls", "actual_ls", "actual_expr",
  "ident_expr", "type_level_expr", "for_expr", "cond_expr", "nil_expr",
  "stmt_level_expr", "opt_task_intent_ls", "task_intent_clause",
  "task_intent_ls", "forall_intent_clause", "forall_intent_ls",
  "intent_expr", "shadow_var_prefix", "io_expr", "new_expr", "let_expr",
  "expr", "opt_expr", "opt_try_expr", "lhs_expr", "fun_expr", "call_expr",
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
     415,   416,   417,   418
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   164,   165,   166,   166,   167,   167,   168,   168,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   170,
     171,   171,   171,   172,   172,   172,   173,   173,   174,   174,
     174,   175,   175,   176,   176,   176,   176,   177,   177,   178,
     178,   179,   179,   179,   180,   181,   181,   181,   181,   181,
     182,   182,   183,   183,   183,   183,   183,   184,   184,   184,
     184,   184,   185,   185,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     187,   187,   187,   187,   187,   187,   187,   187,   188,   188,
     188,   188,   188,   188,   188,   188,   189,   189,   190,   190,
     191,   191,   192,   192,   192,   192,   192,   192,   192,   192,
     193,   193,   194,   194,   194,   194,   195,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   197,
     198,   198,   198,   198,   198,   198,   198,   198,   199,   200,
     200,   200,   200,   200,   200,   201,   201,   202,   202,   202,
     203,   203,   204,   205,   205,   206,   206,   207,   207,   207,
     208,   208,   208,   208,   209,   209,   209,   210,   210,   211,
     211,   211,   212,   212,   213,   214,   214,   214,   215,   215,
     217,   218,   216,   219,   219,   219,   219,   219,   221,   222,
     220,   223,   223,   223,   223,   223,   224,   224,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   227,   227,   228,   229,
     229,   229,   230,   230,   230,   230,   230,   231,   231,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   233,   233,
     233,   233,   233,   233,   234,   234,   235,   235,   235,   235,
     235,   235,   236,   236,   237,   237,   238,   238,   239,   239,
     240,   240,   241,   241,   242,   242,   242,   242,   242,   243,
     243,   244,   244,   244,   244,   244,   244,   244,   245,   245,
     246,   246,   246,   247,   247,   248,   248,   248,   249,   249,
     249,   249,   249,   250,   250,   250,   251,   251,   252,   252,
     253,   253,   253,   254,   254,   254,   255,   255,   255,   256,
     256,   256,   256,   256,   256,   256,   257,   257,   257,   257,
     257,   258,   258,   258,   258,   258,   259,   259,   259,   259,
     260,   260,   260,   261,   261,   261,   261,   261,   262,   262,
     262,   262,   262,   263,   263,   263,   263,   264,   264,   265,
     265,   265,   266,   266,   267,   267,   268,   268,   269,   269,
     269,   269,   270,   270,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   273,   274,   275,
     275,   275,   275,   275,   275,   275,   275,   276,   276,   277,
     278,   278,   279,   280,   280,   281,   281,   281,   282,   282,
     282,   282,   282,   282,   283,   283,   284,   285,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   287,   287,   288,   288,   288,   289,
     289,   289,   289,   290,   290,   291,   291,   291,   292,   292,
     292,   292,   293,   293,   293,   293,   294,   294,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     296,   296,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   298,   298,   298,   298,   298,
     298,   299,   299,   299,   299,   300,   300,   300,   300,   301,
     301,   301,   301,   301,   301,   301
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
       6,     3,     4,     6,     7,     6,     7,     4,     5,     4,
       4,     3,     6,     5,     6,     5,     8,     7,     2,     3,
       3,     2,     2,     3,     3,     0,     2,     2,     3,     5,
       1,     3,     3,     5,     5,     0,     2,     3,     2,     3,
       6,     8,     6,     8,     1,     1,     1,     0,     2,     0,
       2,     3,     5,     5,     1,     1,     2,     3,     1,     3,
       0,     0,     8,     0,     1,     2,     2,     1,     0,     0,
      10,     3,     3,     5,     5,     3,     1,     3,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     3,     3,     0,
       1,     3,     4,     5,     4,     6,     6,     0,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     0,     1,
       1,     2,     1,     1,     1,     1,     0,     1,     2,     1,
       1,     1,     0,     1,     1,     1,     1,     1,     2,     1,
       0,     1,     2,     2,     0,     2,     2,     4,     4,     1,
       3,     3,     3,     3,     3,     3,     3,     2,     1,     1,
       3,     4,     4,     2,     4,     0,     2,     2,     1,     1,
       1,     2,     1,     4,     5,     3,     1,     3,     3,     5,
       1,     1,     3,     1,     2,     3,     0,     2,     2,     3,
       2,     4,     3,     3,     4,     3,     0,     2,     2,     2,
       1,     0,     2,     2,     2,     1,     4,     4,     6,     3,
       0,     1,     1,     3,     4,     3,     4,     6,     0,     2,
       2,     2,     2,     1,     1,     3,     3,     1,     3,     1,
       1,     1,     3,     3,     0,     1,     1,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       4,     4,     4,     5,     2,     2,     2,     2,     2,     2,
       6,     6,     4,     9,     9,     7,     6,     6,     4,     9,
       9,     7,     4,     6,     6,     9,     9,     6,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     1,     4,
       1,     3,     4,     1,     3,     4,     3,     3,     1,     1,
       2,     1,     2,     1,     1,     3,     2,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     3,     3,     3,
       3,     2,     2,     1,     0,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     4,     3,     3,
       3,     3,     3,     4,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     4,     3,     4,     3,     4,
       3,     5,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   519,   520,   521,   522,
     523,     0,   457,    70,   100,     0,   194,   457,     0,   105,
       0,   339,    70,     0,     0,     0,     0,   204,   494,   494,
     516,     0,     0,     0,     0,   104,     0,   214,   101,     0,
     210,     0,     0,   107,     0,     0,   448,     0,   217,     0,
     338,     0,     0,     0,    45,    44,   103,   195,   340,     0,
       0,     0,     0,     0,     0,     0,   106,     0,     0,    83,
       0,   517,     0,     0,     0,   102,   196,     0,     0,   342,
       0,     0,   571,   573,     0,   574,   575,   493,     0,     0,
     572,   569,   122,   570,     0,     0,     0,     4,     0,     5,
       0,     9,    46,    10,    11,    12,    14,   412,   413,    22,
      13,   123,   129,    15,    17,    16,    19,    20,    21,    18,
     128,     0,   126,     0,   483,     0,   130,   127,     0,   131,
     499,   479,   480,   416,   414,     0,     0,   484,   485,     0,
     415,     0,   500,   501,   502,   518,   478,   418,   417,   481,
     482,     0,    38,    24,   424,     0,     0,   458,    71,     0,
       0,     0,     0,     0,     0,     0,     0,   483,   499,   414,
     484,   485,   426,   415,   500,   501,     0,   457,     0,     0,
     341,     0,   168,     0,   397,     0,   404,   495,   215,   522,
     136,     0,     0,   216,     0,     0,     0,     0,     0,     0,
     494,   135,     0,     0,   404,    77,    84,    95,    89,    97,
      79,    88,    98,    85,    99,    93,    87,    94,    92,    90,
      91,    78,    80,    86,    96,     0,    81,     0,     0,     0,
       0,   346,     0,   119,    32,     0,   557,   476,     0,   427,
     558,     7,   404,   494,   125,   124,   309,   394,     0,   393,
       0,     0,   120,   498,     0,     0,    35,     0,   429,   419,
       0,   404,    36,   425,     0,   175,   171,     0,   415,   175,
     172,     0,   335,     0,   428,     0,   393,     0,     0,   560,
     492,   556,   559,   555,     0,    48,    51,     0,     0,   399,
       0,   401,     0,     0,   400,     0,   393,     0,     0,     6,
       0,    47,     0,   197,     0,   295,   294,   218,     0,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   254,
     261,   262,   263,   258,   260,     0,     0,   256,   259,   257,
     255,     0,   265,   264,     0,     0,   404,   404,     0,     0,
       0,    25,    26,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     0,    28,     0,    29,     0,   412,   410,     0,
     405,   406,   411,     0,     0,     0,     0,     0,   145,     0,
       0,   144,     0,     0,     0,   151,     0,     0,     0,    57,
     132,     0,   161,     0,     0,    30,   277,   211,   350,     0,
     351,   353,     0,   375,     0,   356,     0,     0,   118,    31,
      33,     0,   308,     0,    64,   496,   497,   121,     0,    34,
     404,     0,   182,   173,   169,   174,   170,     0,   333,   330,
      61,     0,    57,   138,    37,    50,    49,    52,     0,   524,
       0,     0,   512,     0,   514,     0,     0,     0,     0,     0,
       0,   528,     8,     0,    40,     0,     0,     0,     0,     0,
     288,   345,   456,   552,   551,   554,   562,   561,   566,   565,
     548,   545,   546,   547,   488,   489,   487,   535,   510,   511,
     509,   508,   490,   539,   550,   544,   542,   553,   543,   541,
     533,   538,   540,   549,   532,   536,   537,   534,     0,     0,
       0,     0,     0,     0,     0,   564,   563,   568,   567,   468,
     469,   471,   473,     0,   460,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   526,   457,   457,   141,   331,   343,
     398,     0,     0,   421,     0,   332,   197,     0,     0,     0,
       0,   432,     0,     0,     0,   438,     0,     0,     0,   152,
     570,    60,     0,    53,    58,     0,   160,     0,     0,     0,
     420,   282,   279,   280,   281,   285,   286,   287,   277,     0,
     270,     0,   278,   296,     0,   354,     0,   112,   114,   115,
     110,   111,   109,   108,   113,     0,   374,   373,   479,     0,
     348,   477,   347,   507,   396,   395,     0,     0,     0,   422,
       0,   176,   337,   479,     0,     0,     0,   525,   486,   513,
     402,   515,   403,     0,     0,   527,   157,   442,     0,   530,
     529,     0,    42,    41,    39,   198,     0,     0,   208,     0,
     205,   292,   289,   290,   293,   219,     0,    68,    69,    67,
      66,    65,   505,   506,   470,   472,     0,   459,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   137,   408,   409,   407,     0,   344,     0,   159,     0,
       0,   143,     0,   142,     0,     0,   149,     0,     0,   147,
       0,     0,   463,     0,   133,     0,   134,     0,     0,   163,
       0,   165,   283,   284,     0,   277,   268,     0,   388,   297,
     300,   299,   301,     0,   352,   355,   356,     0,     0,   357,
     358,   184,     0,     0,   183,   186,   423,     0,   177,   180,
       0,   334,    62,    63,     0,     0,     0,     0,   158,     0,
       0,   213,   203,     0,   206,   202,   291,   296,   266,    72,
     251,    84,   249,    89,    74,    88,    85,    99,    87,    90,
      73,    75,    86,   230,   233,   231,   232,   243,   234,   247,
     239,   237,   250,   253,   238,   236,   241,   246,   248,   235,
     240,   244,   245,   242,   252,     0,   228,    76,     0,   266,
     266,   226,   461,   356,   499,   499,     0,     0,     0,     0,
       0,     0,     0,     0,   140,   139,     0,   146,     0,     0,
     431,     0,   430,     0,     0,   437,   150,     0,   436,   148,
       0,   462,    55,    54,   162,   447,   164,     0,   388,   271,
       0,     0,   356,   298,   314,   349,   379,     0,   526,     0,
     188,     0,     0,     0,   178,     0,   155,   444,     0,     0,
     153,   443,     0,   531,   192,   494,   213,   213,   190,   213,
     200,   209,   207,     0,   277,   225,   229,     0,     0,   221,
     222,   465,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   213,   435,     0,     0,   441,     0,     0,   464,     0,
       0,   167,   356,   388,     0,   391,   390,   392,   479,   310,
     274,   272,     0,     0,     0,     0,   377,   479,   189,   187,
       0,   181,     0,   156,     0,   154,   201,   370,     0,   302,
       0,   227,    72,    73,   266,   266,     0,     0,     0,     0,
       0,     0,   193,   191,     0,     0,     0,     0,    56,   166,
     273,   356,   380,     0,   311,   313,   312,   328,     0,   329,
     316,   319,     0,   315,   306,   307,   212,     0,   179,     0,
       0,     0,   369,   368,   479,   303,   314,   267,   223,   224,
       0,     0,     0,     0,     0,     0,   434,   433,   440,   439,
     276,   275,   382,   383,   385,   479,     0,   526,   327,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   479,   446,
     445,     0,   360,     0,     0,     0,   384,   386,   318,   320,
     321,   324,   325,   326,   322,   323,   317,   365,   363,   479,
     526,   304,   220,   305,   380,   364,   479,   387
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   286,    98,   566,   100,   101,   102,   302,
     103,   287,   561,   565,   562,   104,   105,   106,   159,   786,
     229,   107,   226,   108,   596,   192,   234,   109,   110,   111,
     112,   113,   114,   667,   115,   116,   117,   433,   611,   730,
     118,   119,   607,   725,   120,   121,   468,   741,   122,   123,
     639,   640,   167,   227,   583,   125,   126,   470,   747,   645,
     788,   789,   403,   865,   407,   579,   580,   581,   582,   646,
     307,   713,   966,  1022,   956,   247,   945,   900,   904,   950,
     951,   952,   127,   273,   438,   128,   129,   230,   231,   411,
     412,   600,   963,   919,   415,   597,   983,   897,   832,   288,
     183,   292,   293,   379,   380,   381,   168,   131,   132,   133,
     169,   135,   156,   157,   523,   396,   691,   524,   525,   136,
     170,   171,   139,   193,   382,   173,   141,   174,   175,   144,
     145,   146,   297,   147,   148,   149,   150,   151
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -897
static const yytype_int16 yypact[] =
{
    -897,    97,  2604,  -897,   -46,  -897,  -897,  -897,  -897,  -897,
    -897,  4020,     3,   223,  -897,  9043,  -897,     3,  9043,  -897,
     190,   140,   223,  4020,  9043,  4020,    80,  -897,  9043,  6378,
    -897,  7722,  9043,  6221,  9043,  -897,    95,  -897,  -897, 14586,
    -897, 11917,  8147,  -897,  9043,  9043,  -897,  9043,  -897,  9043,
    -897,  9043,   255,   126,  1267,  1267,  -897,  -897,  -897,  8281,
    7274,  9043,  8147,  9043,  9043,   192,  -897,   136,  4020,  -897,
    9043,  -897,  9177,  9177, 14586,  -897,  -897,  9043,  8281,  -897,
    9043,  9043,  -897,  -897,  9043,  -897,  -897, 11187,  9043,  9043,
    -897,  9043,  -897,  -897,  3078,  6512,  8281,  -897,  3863,  -897,
     174,  -897,   282,  -897,  -897,  -897,  -897,  -897,  -897,  -897,
    -897,  -897,  -897,  -897,  -897,  -897,  -897,  -897,  -897,  -897,
    -897, 14586,  -897, 14586,   245,    48,  -897,  -897, 11917,  -897,
     204,  -897,  -897,  -897,   243,   246,   268,   267,   279, 15287,
   11447,   254,   285,   293,  -897,  -897,  -897,  -897,  -897,  -897,
    -897,    30,  -897,  -897, 15287,   304,  4020,  -897,  -897,   312,
    9043,  9043,  9043,  9043,  9043,  8281,  8281,   261,  -897,  -897,
    -897,  -897,   333,   275,  -897,  -897,   316, 13501, 14586, 11917,
    -897,   322,  -897,    37, 15287,   380,  7431, 15287,  -897,    71,
    -897, 14586,    84,   309, 14586,   328,    29, 13060, 12990,   309,
    9043,  -897, 13446, 12342,  7431,  -897,  -897,  -897,  -897,  -897,
    -897,  -897,  -897,  -897,  -897,  -897,  -897,  -897,  -897,  -897,
    -897,  -897,  -897,  -897,  -897,  4020,  -897,   330,  1774,    92,
      13,  -897,  4020,  -897,  -897, 13200,   128,   333, 13200,   333,
     128,  -897,  7431,  9043,  -897,  -897, 14586,  -897,    99, 15287,
    9043,  9043,  -897, 15287,   341, 13361,  -897, 13200,   333, 15287,
     331,  7431,  -897, 15287, 13784,  -897,  -897, 13815, 15613,  -897,
    -897, 13954,   382,   343,   333,   121, 13590, 13200, 13985,   148,
    2747,   128,   148,   128,   182,  -897,  -897,  3235,    27,  -897,
    9043,  -897,   -38,    40,  -897,   105, 14040,   -23,   487,  -897,
    3392,  -897,   440,   367,   342,  -897,  -897,  -897,   144,  -897,
    9043,  9043,  9043,  9043,  8438,  8438,  9043,  9043,  9043,  9043,
    9311,  9043,   394, 11187,  9043,  9043,  9043,  9043,  9043,  9043,
    9043,  9043,  9043,  9043,  9043,  9043,  9043,  9043,  9043,  -897,
    -897,  -897,  -897,  -897,  -897,  7856,  7856,  -897,  -897,  -897,
    -897,  7856,  -897,  -897,  7856,  7856,  7431,  7431,  8438,  8438,
    6983,  -897,  -897, 13645, 13729, 14124,    58,  3549,  -897,  8438,
      29,   348,   145,  -897,  9043,  -897,  9043,   389,  -897,   345,
     370,  -897,  -897,   356, 14586, 11917,   467,  8281,  -897,  4177,
    8438,  -897,  4334,  8438,   353,  -897,    29,   309,  9445,  9043,
    -897,  4020,   480,  9043,   358,  -897,   532,  -897,  -897,  1774,
    -897,   384,   369,  -897,  9579,   415,  9043, 11917,  -897,  -897,
    -897,   374,  -897,  8281,  -897, 15287, 15287,  -897,    38,  -897,
    7431,   377,  -897,   510,  -897,   510,  -897,  9713,   410,  -897,
    -897,  9445,  9043,  -897,  -897,  -897,  -897,  -897,  7990,  -897,
    2172,  6669,  -897,  6826,  -897,  8438,  5747,  2921,   381,  9043,
    6063,  -897,  -897,   259,  -897,  3706, 14586,  8281,   387,  1430,
     338,  -897, 14179, 15429, 15429,   262,  -897,   262,  -897,   262,
   15531,  1090,  1245,   689, 11577, 11577,   333,   148,  -897,  -897,
    -897,  -897,  2747, 15569,   262,  1970,  1970, 15429,  1970,  1970,
     926,   148, 15569, 15499,   926,   128,   128,   148,   395,   396,
     398,   399,   401,   397,   393,  -897,   262,  -897,   262,    56,
    -897,  -897,  -897,    98,  -897,   855, 15323,   213,  9847,  8438,
    9981,  8438,  9043,  8438, 11771,     3, 14219,  -897,  -897,  -897,
   15287, 14264,  7431,  -897,  7431,  -897,   367,   200,  9043,   100,
    9043, 15287,    35, 13270,  9043, 15287,    17, 13130,  6983,  -897,
     408,   429,   412, 14358,   429,   419,   529, 14403,  4020, 13306,
    -897,   177,  -897,  -897,  -897,  -897,  -897,  -897,   734,   103,
    -897, 11939,  -897,   362,   407,  1774,    92,  9043,    80,    95,
    9043,  9043,  9043,  9043,  9043,  5590,  -897,  -897,   617,  7588,
    -897, 15287,  -897,  -897,  -897, 15287,   414,     4,   417,  -897,
    2333,  -897,  -897,   209, 14586,   422,   423,  -897,  -897,  -897,
    -897,  -897,  -897,   -24,   890,  -897,  -897, 15287,  4020, 15287,
    -897, 14444,  -897,  -897,  -897,   448,  2107,   430,   479,    82,
    -897,   515,  -897,  -897,  -897,  -897,  2430,  -897,  -897,  -897,
    -897,  -897,  -897,  -897,  -897,  -897,  6983,  -897,    34,  8438,
    8438,  9043,   564, 14583,  9043,   567, 14614,   442, 12097,    29,
      29,  -897,  -897,  -897,  -897,   445,  -897, 13200,  -897, 12412,
    4491,  -897,  4648,  -897, 12482,  4805,  -897,    29,  4962,  -897,
      29,   115,  -897,  9043,  -897,  9043,  -897,  4020,  9043,  -897,
    4020,   572,  -897,  -897, 14586,  1532,  -897,  1774,   475,   530,
    -897,  -897,  -897,    70,  -897,  -897,   415,   447,    74,  -897,
    -897,  -897,  5119,  8281,  -897,  -897,  -897, 14586,  -897,   476,
     316,  -897,  -897,  -897,  5276,   449,  5433,   453,  -897,  9043,
     459,  1894,  -897,  9043, 14586,  -897,  -897,   362,   460,   486,
    -897,   488,  -897,   493,  -897,   494,   496,   500,   501,   503,
     505,  -897,   506,  -897, 14586,  -897,  -897,  -897,  -897,  -897,
    -897,  -897,  -897,  -897,  -897,  -897,  -897,  -897,  -897,  -897,
    -897,  -897,  -897,  -897,  -897,  9043,  -897,  -897,   507,   460,
     460,  -897,  -897,   415,   138,   222, 14689, 10115, 10249, 14759,
   10383, 10517, 10651, 10785,  -897,  -897, 11845,  -897,  4020,  9043,
   15287,  9043, 15287,  4020,  9043, 15287,  -897,  9043, 15287,  -897,
    6983,  -897, 14795, 15287,  -897, 15287,   588,  4020,   475,  -897,
     483,  8572,    -8,  -897,    21,  -897,  -897,  8438, 11674,  4020,
    -897,    50,   485,  9043,  -897,  9043,  -897, 15287,  4020,  9043,
    -897, 15287,  4020, 15287,  -897,  7140,  1528,  1528,  -897, 13975,
    -897, 15287,  -897,    36,   532,  -897,  -897, 12258, 12017,  -897,
    -897,  -897,  9043,  9043,  9043,  9043,  9043,  9043,  9043,  9043,
     490, 15449, 14403, 12552, 12622, 14403, 12692, 12762,  -897,  9043,
    4020,  -897,   415,   475,  5905,  -897,  -897,  -897,    90,  8281,
    -897,  -897,   165,  9043,    60, 12167,  -897,   942,   380,  -897,
     316, 15287, 12832,  -897, 12902,  -897,  -897,  -897, 10919,   553,
     242,  -897,  -897,  -897,   460,   460, 14865, 14935, 14971, 15041,
   15111, 15147,  -897,  -897,  4020,  4020,  4020,  4020, 15287,  -897,
    -897,    -8,  8729,    81,  -897,  -897, 15287,  -897,    57,  -897,
     133,  -897,   426, 15217,  -897,  -897,  -897, 10785,  -897,  4020,
    4020,  2761,  -897,  -897,   286,  -897,    21,  -897,  -897,  -897,
    9043,  9043,  9043,  9043,  9043,  9043, 14403, 14403, 14403, 14403,
    -897,  -897,  -897,  -897,  -897,   241,  8438, 11321,  -897,  9043,
     165,    57,    57,    57,    57,    57,    57,   165,  1054, 14403,
   14403,   491, 11053,    87,    11, 12203,  -897,  -897, 15287,  -897,
    -897,  -897,  -897,  -897,  -897,  -897,   517,  -897,  -897,   435,
   11453,  -897,  -897,  -897,  8886,  -897,   520,  -897
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -897,  -897,  -897,    -1,  -374,  1346,  -897,  -897,  -897,  -897,
     225,   352,  -325,   212,   214,  -897,  -897,   269,   634,  -897,
     513,  -160,  -622,  -897,  -777,  -897,  1144,  -879,  -700,   -45,
    -897,  -897,  -897,   111,  -897,  -897,  -897,   388,  -897,   -69,
    -897,  -897,  -897,  -897,  -897,   468,   117,  -142,  -897,  -897,
    -897,   -79,   599,  -897,  -897,  -897,  -897,  -897,  -897,  -897,
    -897,  -202,  -138,  -747,  -897,  -191,   -27,   101,  -897,  -897,
    -897,   -67,  -897,  -897,  -321,   273,  -897,  -256,  -280,  -308,
    -298,  -541,  -897,  -175,  -897,   -14,   666,  -116,   283,  -897,
    -379,  -697,  -647,  -897,  -539,  -432,  -896,  -856,  -760,   -51,
    -897,    25,  -897,  -148,  -897,   157,   456,  -326,  -897,  -897,
     918,  -897,    -6,  -897,  -897,  -267,  -897,  -544,  -897,  -897,
    1200,  1336,   -11,     1,    15,   782,  -897,  1519,  1662,  -897,
    -897,  -897,  -897,  -897,  -897,  -897,  -897,  -345
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -527
static const yytype_int16 yytable[] =
{
     154,    97,   355,   371,   172,   612,   179,   177,   248,   244,
     245,   176,   308,   184,   692,   527,   383,   187,   187,   835,
     197,   198,   202,   203,   787,   955,   377,   275,   458,   188,
     584,   235,   578,   236,   237,   413,   238,   917,   239,   606,
     240,   860,   869,   870,   377,   295,   685,   716,   249,   253,
     255,   257,   258,   259,   895,   416,   404,   263,   232,   264,
     947,   267,   271,   372,   680,   722,   274,   276,   892,   277,
     278,   413,   902,   279,   564,   254,   280,   281,   282,   232,
     283,   394,   377,   249,   253,   296,   984,  -366,   598,    60,
    -117,  1006,   451,   413,   421,   305,   871,     3,   654,  -185,
     533,   377,   599,    16,   152,   358,   723,   460,   155,   359,
     294,   613,   792,   431,  -366,   366,   837,   564,   306,   793,
     452,  -371,   394,   986,   903,   955,   899,  -366,  1027,   533,
     355,  1007,   655,   941,  -371,   901,   734,   461,    60,  -366,
    -185,   962,   196,   417,  -371,  -117,   949,   455,  -371,   154,
     363,   364,   365,   263,   249,   296,   313,   448,    57,   916,
     724,  1021,   491,   414,  -371,   918,   367,   374,   947,  -117,
     453,   370,   367,  -371,   834,   253,   313,   968,   969,   385,
     423,   860,    76,   449,   367,   179,  -366,   375,   456,   187,
     367,  -366,  -371,   253,  -185,   940,   377,   377,   454,   414,
    -389,   397,  -371,   314,   456,   367,   715,   315,   513,   514,
     394,   456,   744,   527,    21,   367,   180,   456,   534,   702,
    -389,   414,  -371,   314,  -389,  -371,   158,   315,   656,   423,
     423,   253,   187,   705,   838,   456,   989,   186,   745,   425,
     426,   987,  -371,   948,   981,   820,   787,  1020,  -389,   424,
     253,   423,   204,   703,   949,    50,   657,   320,   678,   321,
     322,   706,   241,   990,   325,   457,    58,   233,  -467,   547,
     377,   440,   332,   821,   417,   417,   888,   320,   260,   450,
     322,   338,   608,   242,   325,   178,   447,   233,   660,   687,
     690,    79,   359,   261,   471,   539,  -467,   265,   269,   472,
     473,   474,   475,   477,   479,   480,   481,   482,   483,   486,
     487,   527,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,   830,   300,
     417,   578,   152,    21,   253,   253,   549,  -367,   445,  -336,
     253,   266,   270,   253,   253,   253,   253,   516,   518,   526,
     676,  -381,  -466,   301,  -450,  1018,   735,   737,   536,  -336,
     508,   509,   641,   540,  -367,   541,   510,   859,   291,   511,
     512,  -381,   705,  1025,    50,  -381,   249,  -367,   551,   553,
    -466,   555,   557,   385,   377,    58,   709,   563,   563,  -367,
     567,   320,   569,  -449,   322,  -453,   309,     5,   325,  -381,
     967,   368,  -504,   642,  -504,   601,   906,   988,   310,   152,
      79,   356,   605,   357,   643,   632,   635,  -454,  -504,   253,
    -504,   233,   233,   233,   488,   476,   478,   710,   402,  -455,
     563,   563,  -503,   644,  -503,  -452,  -367,   605,   711,   731,
     253,  -367,   253,  -451,   624,   605,   627,   489,   629,   631,
    1010,  1011,  1012,  1013,  1014,  1015,   249,   712,   130,   378,
     233,   360,   362,   233,   447,   322,   294,   130,   294,   515,
     517,   367,   373,   172,   274,   527,   620,   378,   622,   130,
     535,   130,   233,    69,   376,   387,  -359,   406,   430,   490,
     578,   427,   437,   439,   462,   466,   467,   469,   538,   542,
     544,   552,   233,   543,   556,   898,   545,   859,   790,   548,
     558,   568,   907,  -359,   585,   378,   570,   551,   663,   555,
     666,   567,   668,   627,   130,   599,  -359,   586,   610,   669,
     670,   253,   603,   253,   378,   609,   991,   677,  -359,   679,
     614,   628,   636,   684,   718,   647,   648,   526,   649,   650,
     130,   651,   225,   653,   130,   652,   571,   673,   -59,   693,
     697,   992,   694,   993,   994,   714,   623,   995,   996,   696,
     721,  -361,   732,   733,   572,   726,   172,   573,   423,   239,
     258,   259,   263,   274,   296,  -359,   742,   272,   253,   743,
    -359,   746,   964,   797,   574,   233,   800,   575,  -361,    52,
     806,   124,   802,   827,   831,   843,   833,   836,   576,   848,
     124,  -361,   130,   852,   720,   854,   985,   864,   -82,   890,
    -100,   233,   124,  -361,   124,  -105,  -104,   577,  -101,   378,
     378,   998,  -107,  -103,   303,  -106,   304,   -83,  -102,   868,
     662,   893,   665,   910,   965,   526,   932,   990,   477,   516,
     796,  1017,   465,   799,   616,   615,   181,   435,   842,  -372,
     384,   985,  -269,   675,   881,   862,   924,   124,  -372,   810,
    -361,   812,   841,   920,   815,  -361,  1019,   818,   829,   704,
     863,   130,   822,  1023,   823,   980,  1004,   825,   130,  1016,
    -269,   272,  1009,   124,  1026,  -372,   604,   124,   985,   201,
     602,   674,     0,   378,   272,     0,     0,   386,     0,     0,
       0,     0,   249,     0,     0,     0,     0,   313,     0,     0,
    -372,   604,     0,   847,   291,   851,   291,  -372,   853,   604,
     925,     0,   861,     0,     0,     0,     0,     0,     0,     0,
       0,   410,     0,   130,     0,     0,     0,  -372,     0,     0,
       0,     0,     0,     0,     0,   124,   130,     0,   571,   422,
       0,     0,     0,     0,   314,     0,     0,  -372,   315,     0,
     476,   515,  -372,     0,   867,  -372,   572,   233,   233,   573,
       0,   233,   233,     0,   140,     0,   810,   812,     0,   815,
     818,   847,   851,   140,   701,     0,   574,   882,   883,   575,
     884,   298,   885,   886,     0,   140,   887,   140,     0,   526,
     576,   244,   245,     0,   317,   672,     0,   378,   320,     0,
     321,   322,     0,   130,   124,   325,   905,   627,     0,   577,
       0,   124,   911,   332,   912,   728,     0,     0,   914,     0,
     336,   337,   338,   943,   187,   130,     0,     0,   130,     0,
     140,     0,     0,     0,   268,   268,     0,   130,     5,     0,
       0,   882,   926,   927,   885,   928,   929,   930,   931,     0,
      14,     0,     0,     0,     0,     0,   140,    19,   938,     0,
     140,     0,     0,   296,     0,     0,   124,     0,   946,     0,
       0,     0,   953,     0,   233,   233,    35,   546,     0,   124,
     311,    38,   233,     0,   402,     0,     0,   312,    43,   402,
    1003,     0,   233,   130,     0,   233,     0,     0,   313,     0,
     134,   130,   410,   976,   977,   978,   979,     0,    56,   134,
       0,     0,     0,     0,     0,     0,     0,     0,   140,     0,
      66,   134,     0,   134,    69,     0,   851,     0,   999,  1000,
     296,    75,     0,     0,   313,   844,     0,     0,     0,   976,
     977,   978,   979,   999,  1000,   314,   124,     0,     0,   315,
       0,     0,     0,     0,     0,  1005,   627,     0,  1008,   634,
       0,   658,   638,     0,  -376,     0,   134,     0,   124,     0,
       0,   124,     0,  -376,     0,   394,     0,     0,     0,   316,
     124,   314,     0,     0,     0,   315,     0,   140,     0,   627,
       0,     0,   134,   851,   140,   317,   134,   318,   319,   320,
    -376,   321,   322,   323,   130,   324,   325,   326,   327,   328,
       0,   329,   330,   331,   332,     0,   333,   334,   335,     0,
       0,   336,   337,   338,     0,  -376,     0,     0,     0,     0,
     736,   317,  -376,   318,   319,   320,   124,   321,   322,     0,
       0,     0,   325,     0,   124,     0,   233,     0,     0,   140,
     332,     0,  -376,     0,   134,     0,     0,   336,   337,   338,
       0,     0,   140,     0,   130,     0,     0,     0,     0,     0,
       0,     0,  -376,     0,   708,     0,  -378,  -376,   410,     0,
    -376,     0,   791,     0,   896,  -378,     0,     0,   402,   402,
       0,   402,   402,     0,     0,   794,   795,     0,   313,     0,
       0,     0,     0,   729,     0,     0,     0,   272,     0,   954,
       0,     0,  -378,     0,     0,   958,   130,   402,   130,   402,
       0,   130,     0,   134,   130,     0,     0,     0,     0,   140,
     134,     0,     0,   130,     0,     0,   130,  -378,     0,     0,
       0,     0,     0,     0,  -378,   314,     0,   124,     0,   315,
       0,   140,   944,     0,   140,     0,     0,     0,   130,     0,
       0,     0,     0,   140,  -378,     0,     0,     0,     0,     0,
     130,     0,   130,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   137,     0,  -378,   134,   256,     0,     0,  -378,
       0,   137,  -378,     0,     0,   982,     0,   828,   134,   320,
     410,   321,   322,   137,     0,   137,   325,   124,     0,   954,
       0,     0,     0,     0,   332,     0,     0,     0,     0,   140,
     729,   336,   337,   338,     0,     0,     0,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   638,     0,     0,
     982,     0,     0,     0,   130,     0,     0,     0,   137,   130,
       0,     0,     0,   313,     0,     0,     0,   866,     0,   124,
       0,   124,     0,   130,   124,   134,     0,   124,     0,     0,
     199,    21,     0,     0,   137,   130,   124,   982,   137,   124,
       0,    28,   243,     0,   130,     0,     0,   134,   130,     0,
     134,    37,     0,     0,  -213,     0,     0,     0,     0,   134,
     314,   124,     0,     0,   315,     0,     0,     0,     0,     0,
      48,     0,    50,   124,     0,   124,     0,  -213,   138,     0,
     388,   391,   395,    58,     0,     0,   130,   138,    99,     0,
     140,     0,     0,     0,     0,     0,   137,   153,     0,   138,
       0,   138,     0,     0,     0,     0,     0,     0,    79,   182,
     317,   185,     0,   319,   320,   134,   321,   322,     0,   419,
       0,   325,   420,   134,     0,     0,     0,     0,     0,   332,
     130,   130,   130,   130,     0,     0,   336,   337,   338,     0,
       0,   429,     0,     0,   138,     0,     0,   124,     0,     0,
     140,     0,   124,     0,   262,   130,   130,     0,     0,     0,
       0,   443,     0,     0,     0,   137,   124,     0,     0,     0,
     138,   637,   137,   205,   138,     0,     0,     0,   124,     0,
      99,     0,     0,     0,   299,   206,   207,   124,     0,     0,
       0,   124,   208,     0,     0,     0,     0,     0,     0,     0,
     209,     0,   140,     0,   140,     0,   210,   140,     0,     0,
     140,   211,     0,   212,     0,     0,   213,     0,     0,   140,
       0,     0,   140,   214,     0,     0,   134,   137,     0,   124,
       0,     0,   138,     0,   215,     0,     0,     0,     0,     0,
     137,     0,   361,   216,   140,     0,     0,     0,     0,     0,
       0,     0,   217,   218,   537,   219,   140,   220,   140,   221,
       0,   142,   222,     0,     0,     0,   223,     0,     0,   224,
     142,     0,     0,   124,   124,   124,   124,     0,     0,     0,
     559,     0,   142,     0,   142,     0,   134,     0,     0,     0,
       0,   199,    21,     0,     0,     0,   571,     0,   124,   124,
       0,   138,    28,   243,     0,     0,     0,   137,   138,     0,
       0,   405,    37,     0,   572,     0,     0,   573,   418,     0,
       0,     0,     0,     0,     0,     0,     0,   142,     0,   137,
     140,    48,   137,    50,   574,   140,     0,   575,   134,    52,
     134,   137,     0,   134,    58,     0,   134,     0,   576,   140,
       0,     0,     0,   142,     0,   134,     0,   142,   134,     0,
       0,   140,     0,   138,     0,     0,     0,   577,     0,    79,
     140,     0,     0,    99,   140,     0,   138,     0,     0,     0,
     134,     0,     0,     0,     0,     0,    99,     0,     0,     0,
       0,     0,   134,     0,   134,     0,     0,   137,     0,     0,
       0,     0,     0,     0,   143,   137,     0,     0,     0,     0,
       0,     0,   140,   143,     0,   142,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   143,     0,   143,     0,     0,
       0,     0,     0,     0,     0,     0,   681,   683,     0,     0,
     686,   689,     0,   138,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   140,   140,   140,   140,
       0,     0,     0,     0,     0,   138,   134,     0,   138,     0,
     143,   134,     0,     0,     0,   418,     0,   138,   418,     0,
       0,   140,   140,     0,   142,   134,     0,     0,     0,     0,
       0,   142,     0,     0,     0,     0,   143,   134,     0,     0,
     143,     0,     0,     0,     0,     0,   134,     0,   137,     0,
     134,     0,     0,     0,     0,     0,     0,   205,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   206,
     207,     0,     0,   138,     0,     0,   208,     0,     0,     0,
       0,   138,     0,   626,   209,     0,   142,     0,   134,     0,
     210,    99,     0,   804,   805,   211,     0,   212,   143,   142,
     213,   807,     0,     0,     0,     0,     0,   214,   137,     0,
       0,   816,     0,     0,   819,     0,     0,     0,   215,     0,
       0,     0,     0,     0,     0,     0,     0,   216,     0,     0,
       0,     0,   134,   134,   134,   134,   217,   218,     0,   219,
       0,   220,     0,   221,     0,     0,   222,     0,     0,     0,
     223,   408,     0,   224,     0,     0,     0,   134,   134,     0,
     137,     0,   137,     0,     0,   137,   142,   143,   137,     0,
       0,     0,     0,     0,   143,     0,     0,   137,     0,     0,
     137,     0,     0,     0,   138,     0,     0,     0,   142,     0,
       0,   142,     0,    16,   699,     0,     0,    20,    21,     0,
     142,     0,   137,     0,     0,     0,    27,     0,    28,   855,
       0,   409,     0,    33,   137,     0,   137,     0,    37,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   143,
       0,     0,     0,     0,     0,     0,     0,    48,     0,    50,
       0,    52,   143,   856,   138,     0,   857,     0,    57,     0,
      58,     0,     0,     0,   738,     0,   142,     0,     0,     0,
       0,     0,     0,     0,   142,   909,     0,     0,     0,    74,
       0,     0,    76,     0,     0,    79,     0,     0,   313,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   137,     0,
       0,     0,     0,   137,     0,     0,   138,     0,   138,     0,
       0,   138,     0,     0,   138,     0,   418,   137,   418,   143,
       0,   418,     0,   138,   418,     0,   138,     0,     0,   137,
       0,     0,     0,   824,    92,   314,   826,     0,   137,   315,
     858,   143,   137,     0,   143,     0,     0,     0,   138,     0,
       0,     0,     0,   143,     0,     0,     0,     0,   840,     0,
     138,     0,   138,     0,     0,     0,     0,     0,     0,     0,
     846,     0,   850,     0,     0,     0,     0,   142,     0,     0,
     137,     0,     0,     0,     0,   317,     0,   318,   319,   320,
       0,   321,   322,   323,     0,     0,   325,     0,   740,     0,
       0,     0,     0,   331,   332,     0,     0,     0,   335,   143,
       0,   336,   337,   338,     0,     0,  -199,   143,     0,     0,
    -199,  -199,     0,     0,   137,   137,   137,   137,     0,  -199,
       0,  -199,  -199,     0,   138,     0,  -199,   142,     0,   138,
       0,  -199,     0,     0,  -199,     0,     0,     0,     0,   137,
     137,     0,     0,   138,     0,     0,     0,     0,     0,     0,
    -199,     0,  -199,   891,  -199,   138,  -199,  -199,     0,  -199,
       0,  -199,   311,  -199,   138,   908,     0,     0,   138,   312,
       0,     0,     0,     0,   913,     0,     0,     0,   915,   142,
     313,   142,  -199,     0,   142,  -199,     0,   142,  -199,     0,
       0,     0,     0,     0,     0,     0,   142,     0,     0,   142,
       0,     0,     0,     0,     0,     0,   138,     0,     0,     0,
     143,     0,     0,     0,     0,     0,   939,     0,     0,     0,
       0,   142,     0,     0,     0,     0,     0,   314,     0,     0,
       0,   315,     0,   142,     0,   142,     0,  -199,     0,     0,
       0,     0,     0,  -199,     0,     0,     0,     0,     0,     0,
     138,   138,   138,   138,     0,     0,     0,     0,     0,     0,
       0,   316,     0,     0,     0,     0,     0,     0,     0,     0,
     143,     0,     0,     0,     0,   138,   138,   317,     0,   318,
     319,   320,     0,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,   337,   338,     0,   142,     0,     0,
     618,     0,   142,     0,     0,     0,   205,     0,     0,     0,
       0,     0,   143,     0,   143,     0,   142,   143,   206,   207,
     143,     0,     0,     0,     0,   208,     0,     0,   142,   143,
       0,     0,   143,   209,     0,     0,     0,   142,     0,   210,
       0,   142,     0,     0,   211,     0,   212,     0,     0,   213,
       0,     0,     0,     0,   143,     0,   214,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   143,   215,   143,     0,
       0,     0,     0,     0,     0,     0,   216,     0,     0,   142,
       0,     0,     0,     0,     0,   217,   218,     0,   219,     0,
     220,     0,   221,     0,     0,   222,     0,     0,     0,   223,
       0,   748,   224,   749,     0,     0,     0,     0,     0,     0,
     750,     0,     0,     0,     0,   751,   207,   752,     0,     0,
       0,     0,   753,   142,   142,   142,   142,     0,     0,     0,
     209,     0,     0,     0,     0,     0,   754,     0,     0,     0,
     143,   755,     0,   212,     0,   143,   756,     0,   142,   142,
       0,     0,     0,   757,     0,     0,     0,     0,   367,   143,
     727,     0,     0,     0,   215,     0,     0,     0,     0,     0,
       0,   143,     0,   758,     0,     0,     0,     0,     0,     0,
     143,     0,   217,   218,   143,   759,     0,   220,     0,   760,
       0,     0,   761,     0,     0,     0,   762,     0,     0,   224,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     339,   340,   341,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   143,   352,   353,   763,   764,   765,   766,     0,
       0,   767,     0,     0,     0,   768,   769,   770,   771,   772,
     773,   774,   775,   776,   777,   778,   779,     0,   780,     0,
       0,   781,   782,   783,   784,     0,     0,   785,     0,     0,
       0,     0,     0,     0,     0,     0,   143,   143,   143,   143,
       0,     0,     0,     0,    -2,     4,     0,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,   143,   143,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,    37,     0,
      38,  -213,    39,    40,    41,     0,    42,    43,    44,   -43,
      45,    46,     0,    47,     0,     0,     0,    48,    49,    50,
      51,    52,    53,    54,  -213,   -43,    55,    56,    57,     0,
      58,    59,    60,     0,    61,    62,    63,    64,    65,    66,
      67,    68,     0,    69,    70,     0,    71,    72,    73,    74,
      75,     0,    76,    77,    78,    79,     0,     0,    80,     0,
      81,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,    89,
       0,    90,    91,     0,    92,     0,     0,    93,     0,    94,
       0,    95,  1001,    96,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   160,     0,   313,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,   314,     0,     0,    49,   315,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,   317,     0,   318,   319,   320,     0,   321,   322,
       0,     0,     0,   325,     0,     0,    83,    84,    85,    86,
     331,   332,     0,     0,    87,   335,     0,     0,   336,   337,
     338,     0,     0,     0,    88,     0,    89,     0,    90,    91,
     246,     0,     0,     0,    93,     0,   165,     0,    95,     0,
     166,  1002,     4,     0,     5,     6,     7,     8,     9,    10,
       0,  -526,     0,    11,    12,    13,    14,    15,  -526,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,  -526,
      25,    26,  -526,    27,     0,    28,    29,    30,    31,    32,
      33,    34,    35,     0,    36,    37,     0,    38,  -213,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,     0,     0,     0,    48,    49,    50,    51,     0,    53,
      54,  -213,   -43,    55,    56,    57,  -526,    58,    59,    60,
    -526,    61,    62,    63,    64,    65,    66,    67,    68,     0,
      69,    70,     0,    71,    72,    73,    74,    75,     0,    76,
      77,    78,    79,     0,     0,    80,     0,    81,     0,     0,
    -526,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -526,    84,  -526,  -526,
    -526,  -526,  -526,  -526,  -526,     0,  -526,  -526,  -526,  -526,
    -526,     0,  -526,  -526,  -526,  -526,    89,  -526,  -526,  -526,
       0,    92,  -526,  -526,  -526,     0,    94,  -526,    95,   284,
      96,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,    34,    35,
       0,    36,    37,     0,    38,  -213,    39,    40,    41,     0,
      42,    43,    44,   -43,    45,    46,     0,    47,     0,     0,
       0,    48,    49,    50,    51,    52,    53,    54,  -213,   -43,
      55,    56,    57,     0,    58,    59,    60,     0,    61,    62,
      63,    64,    65,    66,    67,    68,     0,    69,    70,     0,
      71,    72,    73,    74,    75,     0,    76,    77,    78,    79,
       0,     0,    80,     0,    81,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    89,     0,    90,    91,   246,    92,     0,
       0,    93,     0,    94,   285,    95,     4,    96,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,    25,    26,     0,    27,     0,    28,
      29,    30,    31,    32,    33,    34,    35,     0,    36,    37,
       0,    38,  -213,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,     0,     0,     0,    48,    49,
      50,    51,    52,    53,    54,  -213,   -43,    55,    56,    57,
       0,    58,    59,    60,     0,    61,    62,    63,    64,    65,
      66,    67,    68,     0,    69,    70,     0,    71,    72,    73,
      74,    75,     0,    76,    77,    78,    79,     0,     0,    80,
       0,    81,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
      89,     0,    90,    91,     0,    92,     0,     0,    93,     0,
      94,   446,    95,   463,    96,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,    25,    26,     0,    27,     0,    28,    29,    30,    31,
      32,    33,    34,    35,     0,    36,    37,     0,    38,  -213,
      39,    40,    41,     0,    42,    43,    44,   -43,    45,    46,
       0,    47,     0,     0,     0,    48,    49,    50,    51,    52,
      53,    54,  -213,   -43,    55,    56,    57,     0,    58,    59,
      60,     0,    61,    62,    63,    64,    65,    66,    67,    68,
       0,    69,    70,     0,    71,    72,    73,    74,    75,     0,
      76,    77,    78,    79,     0,     0,    80,     0,    81,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,     0,    92,     0,     0,    93,     0,    94,   464,    95,
     284,    96,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,    25,    26,
       0,    27,     0,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,    37,     0,    38,  -213,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,    47,     0,
       0,     0,    48,    49,    50,    51,    52,    53,    54,  -213,
     -43,    55,    56,    57,     0,    58,    59,    60,     0,    61,
      62,    63,    64,    65,    66,    67,    68,     0,    69,    70,
       0,    71,    72,    73,    74,    75,     0,    76,    77,    78,
      79,     0,     0,    80,     0,    81,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,     0,    92,
       0,     0,    93,     0,    94,   285,    95,     4,    96,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,    25,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,    34,    35,     0,    36,
      37,     0,    38,  -213,    39,    40,    41,     0,    42,    43,
      44,   -43,    45,    46,     0,    47,     0,     0,     0,    48,
      49,    50,    51,    52,    53,    54,  -213,   -43,    55,    56,
      57,     0,    58,    59,    60,     0,    61,    62,    63,    64,
      65,    66,    67,    68,     0,    69,    70,     0,    71,    72,
      73,    74,    75,     0,    76,    77,    78,    79,     0,     0,
      80,     0,    81,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,     0,    92,     0,     0,    93,
       0,    94,   633,    95,     4,    96,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,    25,    26,     0,    27,     0,    28,    29,    30,
      31,    32,    33,    34,    35,     0,    36,    37,     0,    38,
    -213,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,     0,     0,     0,    48,    49,    50,    51,
     298,    53,    54,  -213,   -43,    55,    56,    57,     0,    58,
      59,    60,     0,    61,    62,    63,    64,    65,    66,    67,
      68,     0,    69,    70,     0,    71,    72,    73,    74,    75,
       0,    76,    77,    78,    79,     0,     0,    80,     0,    81,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,     0,    92,     0,     0,    93,     0,    94,     0,
      95,     4,    96,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
      34,    35,     0,    36,    37,     0,    38,  -213,    39,    40,
      41,     0,    42,    43,    44,   -43,    45,    46,     0,    47,
       0,     0,     0,    48,    49,    50,    51,     0,    53,    54,
    -213,   -43,    55,    56,    57,     0,    58,    59,    60,     0,
      61,    62,    63,    64,    65,    66,    67,    68,     0,    69,
      70,     0,    71,    72,    73,    74,    75,     0,    76,    77,
      78,    79,     0,     0,    80,     0,    81,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,     0,    89,     0,    90,    91,     0,
      92,     0,     0,    93,     0,    94,     0,    95,     4,    96,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,    26,     0,    27,
       0,    28,    29,    30,    31,    32,    33,   550,    35,     0,
      36,    37,     0,    38,  -213,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,    47,     0,     0,     0,
      48,    49,    50,    51,     0,    53,    54,  -213,   -43,    55,
      56,    57,     0,    58,    59,    60,     0,    61,    62,    63,
      64,    65,    66,    67,    68,     0,    69,    70,     0,    71,
      72,    73,    74,    75,     0,    76,    77,    78,    79,     0,
       0,    80,     0,    81,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    89,     0,    90,    91,     0,    92,     0,     0,
      93,     0,    94,     0,    95,     4,    96,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,   554,    35,     0,    36,    37,     0,
      38,  -213,    39,    40,    41,     0,    42,    43,    44,   -43,
      45,    46,     0,    47,     0,     0,     0,    48,    49,    50,
      51,     0,    53,    54,  -213,   -43,    55,    56,    57,     0,
      58,    59,    60,     0,    61,    62,    63,    64,    65,    66,
      67,    68,     0,    69,    70,     0,    71,    72,    73,    74,
      75,     0,    76,    77,    78,    79,     0,     0,    80,     0,
      81,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,    89,
       0,    90,    91,     0,    92,     0,     0,    93,     0,    94,
       0,    95,     4,    96,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
      25,    26,     0,    27,     0,    28,    29,    30,    31,    32,
      33,   809,    35,     0,    36,    37,     0,    38,  -213,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,     0,     0,     0,    48,    49,    50,    51,     0,    53,
      54,  -213,   -43,    55,    56,    57,     0,    58,    59,    60,
       0,    61,    62,    63,    64,    65,    66,    67,    68,     0,
      69,    70,     0,    71,    72,    73,    74,    75,     0,    76,
      77,    78,    79,     0,     0,    80,     0,    81,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,     0,    89,     0,    90,    91,
       0,    92,     0,     0,    93,     0,    94,     0,    95,     4,
      96,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,   811,    35,
       0,    36,    37,     0,    38,  -213,    39,    40,    41,     0,
      42,    43,    44,   -43,    45,    46,     0,    47,     0,     0,
       0,    48,    49,    50,    51,     0,    53,    54,  -213,   -43,
      55,    56,    57,     0,    58,    59,    60,     0,    61,    62,
      63,    64,    65,    66,    67,    68,     0,    69,    70,     0,
      71,    72,    73,    74,    75,     0,    76,    77,    78,    79,
       0,     0,    80,     0,    81,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    89,     0,    90,    91,     0,    92,     0,
       0,    93,     0,    94,     0,    95,     4,    96,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,    25,    26,     0,    27,     0,    28,
      29,    30,    31,    32,    33,   814,    35,     0,    36,    37,
       0,    38,  -213,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,     0,     0,     0,    48,    49,
      50,    51,     0,    53,    54,  -213,   -43,    55,    56,    57,
       0,    58,    59,    60,     0,    61,    62,    63,    64,    65,
      66,    67,    68,     0,    69,    70,     0,    71,    72,    73,
      74,    75,     0,    76,    77,    78,    79,     0,     0,    80,
       0,    81,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
      89,     0,    90,    91,     0,    92,     0,     0,    93,     0,
      94,     0,    95,     4,    96,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,    25,    26,     0,    27,     0,    28,    29,    30,    31,
      32,    33,   817,    35,     0,    36,    37,     0,    38,  -213,
      39,    40,    41,     0,    42,    43,    44,   -43,    45,    46,
       0,    47,     0,     0,     0,    48,    49,    50,    51,     0,
      53,    54,  -213,   -43,    55,    56,    57,     0,    58,    59,
      60,     0,    61,    62,    63,    64,    65,    66,    67,    68,
       0,    69,    70,     0,    71,    72,    73,    74,    75,     0,
      76,    77,    78,    79,     0,     0,    80,     0,    81,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,     0,    92,     0,     0,    93,     0,    94,     0,    95,
       4,    96,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,   839,    26,
       0,    27,     0,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,    37,     0,    38,  -213,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,    47,     0,
       0,     0,    48,    49,    50,    51,     0,    53,    54,  -213,
     -43,    55,    56,    57,     0,    58,    59,    60,     0,    61,
      62,    63,    64,    65,    66,    67,    68,     0,    69,    70,
       0,    71,    72,    73,    74,    75,     0,    76,    77,    78,
      79,     0,     0,    80,     0,    81,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,     0,    92,
       0,     0,    93,     0,    94,     0,    95,     4,    96,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,    25,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,   845,    35,     0,    36,
      37,     0,    38,  -213,    39,    40,    41,     0,    42,    43,
      44,   -43,    45,    46,     0,    47,     0,     0,     0,    48,
      49,    50,    51,     0,    53,    54,  -213,   -43,    55,    56,
      57,     0,    58,    59,    60,     0,    61,    62,    63,    64,
      65,    66,    67,    68,     0,    69,    70,     0,    71,    72,
      73,    74,    75,     0,    76,    77,    78,    79,     0,     0,
      80,     0,    81,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,     0,    92,     0,     0,    93,
       0,    94,     0,    95,     4,    96,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,    25,    26,     0,    27,     0,    28,    29,    30,
      31,    32,    33,   849,    35,     0,    36,    37,     0,    38,
    -213,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,     0,     0,     0,    48,    49,    50,    51,
       0,    53,    54,  -213,   -43,    55,    56,    57,     0,    58,
      59,    60,     0,    61,    62,    63,    64,    65,    66,    67,
      68,     0,    69,    70,     0,    71,    72,    73,    74,    75,
       0,    76,    77,    78,    79,     0,     0,    80,     0,    81,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,     0,    92,     0,     0,    93,     0,    94,     0,
      95,   717,    96,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   160,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,     0,    89,     0,    90,    91,   246,
       0,     0,     0,    93,     0,   165,     0,    95,     0,   166,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   160,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    89,     0,    90,    91,   246,     0,     0,     0,
      93,     0,   165,     0,    95,     0,   166,   625,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   160,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
      89,     0,    90,    91,   246,     0,     0,     0,    93,     0,
     165,     0,    95,     0,   166,   942,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   160,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,     0,     0,     0,     0,    93,     0,   165,     0,
      95,     0,   166,   630,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   160,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,   199,    21,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,   200,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,    50,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,    58,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
      77,     0,    79,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,     0,    89,     0,    90,    91,
       0,     0,     0,     0,    93,     0,   165,     0,    95,     0,
     166,     5,     6,     7,     8,   189,    10,   190,     0,     0,
     160,     0,     0,    14,    15,     0,     0,  -116,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   161,   162,     0,   163,    35,
       0,    36,     0,     0,    38,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,  -116,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   164,     0,    69,     0,     0,
      71,     0,     0,   191,    75,     0,  -116,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    88,     0,    89,   160,    90,    91,    14,    15,     0,
       0,    93,     0,   165,    19,    95,     0,   166,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   163,    35,     0,    36,     0,     0,    38,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   164,
       0,    69,     0,     0,    71,   250,   251,     0,    75,   289,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,   290,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,   246,     0,     0,     0,    93,     0,   165,     0,    95,
       0,   166,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   160,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,   250,   251,     0,    75,   289,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,   246,     0,
       0,     0,    93,     0,   165,     0,    95,   619,   166,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   160,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,     0,    36,
       0,     0,    38,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,   250,
     251,     0,    75,   289,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,   246,     0,     0,     0,    93,
       0,   165,     0,    95,   621,   166,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   160,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,   519,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,   520,    36,     0,     0,    38,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,   521,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,   522,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,     0,     0,     0,     0,    93,     0,   165,     0,
      95,     0,   166,     5,     6,     7,     8,   189,    10,     0,
       0,     0,   160,     0,     0,    14,    15,     0,     0,  -116,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,  -116,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,     0,     0,   191,    75,     0,  -116,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    88,     0,    89,   160,    90,    91,    14,
      15,     0,     0,    93,     0,   165,    19,    95,     0,   166,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,     0,    36,     0,     0,
      38,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   164,     0,    69,     0,     0,    71,   250,   251,     0,
      75,     0,     0,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,    89,
       0,    90,    91,     0,   252,     0,     0,    93,     0,   165,
       0,    95,     0,   166,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   160,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,   250,   251,     0,    75,     0,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,     0,    89,     0,    90,    91,
     246,     0,     0,     0,    93,     0,   165,     0,    95,     0,
     166,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     160,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   161,   162,     0,   163,    35,
       0,    36,     0,     0,    38,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,   719,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   164,     0,    69,     0,     0,
      71,   250,   251,     0,    75,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    88,     0,    89,   160,    90,    91,    14,    15,     0,
       0,    93,     0,   165,    19,    95,     0,   166,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   163,    35,     0,    36,     0,     0,    38,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,    49,   194,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   164,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,   195,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    88,     0,    89,   160,    90,
      91,    14,    15,     0,     0,    93,     0,   165,    19,    95,
       0,   166,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,     0,    36,
       0,     0,    38,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,   250,
     251,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    88,
       0,    89,   160,    90,    91,    14,    15,     0,     0,    93,
       0,   165,    19,    95,     0,   166,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,     0,    89,     0,    90,    91,   246,
       0,     0,     0,    93,     0,   165,   617,    95,     0,   166,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   160,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,   232,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      88,     0,    89,   160,    90,    91,    14,    15,     0,     0,
      93,     0,    94,    19,    95,     0,   166,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,     0,    89,     0,    90,    91,
     246,     0,     0,     0,    93,     0,   165,     0,    95,     0,
     166,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     160,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   161,   162,     0,   163,    35,
       0,    36,     0,     0,    38,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   164,     0,    69,     0,     0,
      71,     0,     0,     0,    75,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,   195,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    88,     0,    89,   160,    90,    91,    14,   587,     0,
       0,    93,     0,   165,    19,    95,     0,   166,     0,     0,
       0,     0,   588,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   163,    35,     0,   589,     0,     0,    38,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,   590,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,   591,   592,    65,    66,    67,   593,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,   594,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,   246,     0,     0,     0,    93,     0,   165,     0,    95,
       0,   894,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   160,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,   246,     0,
       0,     0,    93,     0,   165,     0,    95,     0,   894,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   160,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   879,    35,     0,    36,
       0,     0,    38,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,   246,     0,     0,     0,    93,
       0,   165,     0,    95,     0,   166,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   160,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    88,     0,    89,   160,
      90,    91,    14,    15,     0,     0,    93,     0,   165,    19,
      95,     0,   166,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      88,     0,    89,   160,    90,    91,    14,   484,     0,     0,
      93,     0,    94,    19,    95,     0,   166,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
     485,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    88,     0,    89,   160,    90,    91,
      14,    15,     0,     0,    93,     0,   165,    19,    95,     0,
     166,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    88,     0,
      89,   160,    90,    91,    14,   587,     0,     0,   560,     0,
     165,    19,    95,     0,   166,     0,     0,     0,     0,   588,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,   589,     0,     0,    38,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,   590,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,   591,   592,    65,    66,    67,   593,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,   594,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    88,     0,    89,   160,    90,    91,    14,    15,
       0,     0,    93,     0,   165,    19,    95,     0,   595,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    88,     0,    89,   160,
      90,    91,    14,    15,     0,     0,    93,     0,   165,    19,
      95,     0,   595,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   661,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      88,     0,    89,   160,    90,    91,    14,    15,     0,     0,
      93,     0,   165,    19,    95,     0,   166,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   664,    35,     0,    36,     0,     0,    38,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    88,     0,    89,   160,    90,    91,
      14,    15,     0,     0,    93,     0,   165,    19,    95,     0,
     166,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   873,    35,     0,    36,     0,
       0,    38,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    88,     0,
      89,   160,    90,    91,    14,    15,     0,     0,    93,     0,
     165,    19,    95,     0,   166,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   874,
      35,     0,    36,     0,     0,    38,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    88,     0,    89,   160,    90,    91,    14,    15,
       0,     0,    93,     0,   165,    19,    95,     0,   166,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   876,    35,     0,    36,     0,     0,    38,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    88,     0,    89,   160,
      90,    91,    14,    15,     0,     0,    93,     0,   165,    19,
      95,     0,   166,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   877,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      88,     0,    89,   160,    90,    91,    14,    15,     0,     0,
      93,     0,   165,    19,    95,     0,   166,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   878,    35,     0,    36,     0,     0,    38,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    88,     0,    89,   160,    90,    91,
      14,    15,     0,     0,    93,     0,   165,    19,    95,     0,
     166,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   879,    35,     0,    36,     0,
       0,    38,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    88,     0,
      89,   160,    90,    91,    14,   587,     0,     0,    93,     0,
     165,    19,    95,     0,   166,     0,     0,     0,     0,   588,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,   589,     0,     0,    38,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,   590,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,   591,   592,    65,    66,    67,   593,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,   594,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    88,     0,    89,   160,    90,    91,    14,    15,
       0,     0,    93,     0,   165,    19,    95,     0,   961,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    88,     0,    89,   160,
      90,    91,    14,    15,     0,     0,    93,     0,     0,    19,
      95,     0,   961,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      88,     0,    89,   160,     0,    91,    14,    15,     0,     0,
      93,     0,   165,    19,    95,     0,   166,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,     0,
       0,  -380,     0,     0,     0,  -380,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   160,    89,     0,    14,    15,
     246,     0,     0,     0,     0,    19,   165,     0,    95,  -380,
     894,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,    40,    41,  -362,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,  -362,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,  -362,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,  -362,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,     0,     0,     0,     0,     0,     0,     0,    84,
       5,     6,     7,     8,     9,    10,     0,  -474,     0,   160,
       0,     0,    14,    15,     0,     0,     0,     0,    89,    19,
       0,   354,     0,  -362,  -503,     0,  -503,    26,  -362,     0,
      95,     0,   961,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   160,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,    84,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,     0,    36,     0,     0,
      38,     0,    89,    40,    41,     0,     0,    43,    44,     0,
      45,    46,   165,     0,    95,     0,   166,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   164,     0,    69,     0,     0,    71,     0,     0,     0,
      75,     0,     0,    77,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   160,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
      84,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,    89,
      40,    41,     0,     0,    43,    44,     0,    45,    46,   165,
       0,    95,     0,   595,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,   880,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,  -199,     0,     0,    75,  -199,  -199,
      77,     0,     0,     0,     0,     0,     0,  -199,     0,  -199,
    -199,     0,     0,     0,  -199,     0,     0,     0,     0,  -199,
       0,     0,  -199,     0,     0,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -199,     0,
    -199,     0,  -199,     0,  -199,  -199,    89,  -199,     0,  -199,
     205,  -199,     0,     0,     0,     0,     0,     0,    95,     0,
     166,     0,   206,   207,     0,     0,     0,     0,     0,   208,
    -199,     0,   205,  -199,     0,     0,  -199,   209,     0,     0,
       0,     0,     0,   210,   206,   207,     0,     0,   211,     0,
     212,   208,     0,   213,     0,     0,     0,     0,     0,   209,
     214,     0,     0,     0,     0,   210,     0,     0,     0,     0,
     211,   215,   212,     0,     0,   213,     0,     0,     0,     0,
     216,     0,   214,     0,     0,  -199,     0,     0,     0,   217,
     218,  -199,   219,   215,   220,     0,   221,     0,     0,   222,
       0,     0,   216,   223,     0,     0,   224,     0,     0,     0,
     922,   217,   218,     0,   219,     0,   220,   750,   221,     0,
       0,   222,   206,   207,   752,   223,     0,     0,   224,   208,
       0,     0,     0,     0,     0,     0,     0,   209,     0,     0,
       0,     0,     0,   754,     0,     0,     0,     0,   211,     0,
     212,     0,     0,   213,     0,     0,     0,     0,     0,     0,
     214,     0,     0,     0,   228,     0,     0,     0,     0,     0,
       0,   215,     0,     0,     0,     0,     0,     0,     0,     0,
     216,     0,     0,     0,     0,     0,   707,     0,     0,   217,
     218,     0,   219,     0,   220,     0,   923,   311,     0,   761,
       0,     0,     0,   223,   312,     0,   224,     0,     0,     0,
       0,     0,     0,     0,     0,   313,     0,   339,   340,   341,
     342,   343,   344,     0,     0,   347,   348,   349,   350,     0,
     352,   353,   763,   764,   765,   766,     0,     0,   767,     0,
       0,     0,   768,   769,   770,   771,   772,   773,   774,   775,
     776,   777,   778,   779,     0,   780,     0,     0,   781,   782,
     783,   784,   314,     0,     0,     0,   315,   311,     0,     0,
       0,     0,     0,     0,   312,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   313,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   316,     0,     0,     0,
       0,     0,     0,   311,     0,     0,     0,     0,     0,     0,
     312,     0,   317,     0,   318,   319,   320,     0,   321,   322,
     323,   313,   324,   325,   326,   327,   328,     0,   329,   330,
     331,   332,   314,   333,   334,   335,   315,     0,   336,   337,
     338,     0,     0,     0,     0,     0,     0,   803,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   311,     0,
       0,     0,     0,     0,     0,   312,   316,     0,   314,     0,
       0,     0,   315,     0,     0,     0,   313,     0,     0,     0,
       0,     0,   317,     0,   318,   319,   320,     0,   321,   322,
     323,     0,   324,   325,   326,   327,   328,     0,   329,   330,
     331,   332,   316,   333,   334,   335,     0,     0,   336,   337,
     338,     0,     0,     0,     0,     0,     0,   957,   317,     0,
     318,   319,   320,   314,   321,   322,   323,   315,   324,   325,
     326,   327,   328,     0,   329,   330,   331,   332,     0,   333,
     334,   335,   311,     0,   336,   337,   338,     0,     0,   312,
       0,     0,     0,  1024,     0,     0,     0,   316,     0,     0,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   317,     0,   318,   319,   320,     0,   321,
     322,   323,     0,   324,   325,   326,   327,   328,     0,   329,
     330,   331,   332,     0,   333,   334,   335,     0,     0,   336,
     337,   338,     0,     0,     0,     0,   921,   314,     0,     0,
       0,   315,   311,     0,     0,     0,     0,     0,     0,   312,
     401,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   316,   339,   340,   341,   342,   343,   344,     0,     0,
     347,   348,   349,   350,     0,   352,   353,   317,     0,   318,
     319,   320,     0,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,   314,   333,   334,
     335,   315,   311,   336,   337,   338,     0,   367,     0,   312,
     808,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   316,   339,   340,   341,   342,   343,   344,     0,     0,
     347,   348,   349,   350,     0,   352,   353,   317,     0,   318,
     319,   320,     0,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,   314,   333,   334,
     335,   315,   311,   336,   337,   338,     0,   367,     0,   312,
     813,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   316,   339,   340,   341,   342,   343,   344,     0,     0,
     347,   348,   349,   350,     0,   352,   353,   317,     0,   318,
     319,   320,     0,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,   314,   333,   334,
     335,   315,   311,   336,   337,   338,     0,   367,     0,   312,
     934,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   316,   339,   340,   341,   342,   343,   344,     0,     0,
     347,   348,   349,   350,     0,   352,   353,   317,     0,   318,
     319,   320,     0,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,   314,   333,   334,
     335,   315,   311,   336,   337,   338,     0,   367,     0,   312,
     935,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   316,   339,   340,   341,   342,   343,   344,     0,     0,
     347,   348,   349,   350,     0,   352,   353,   317,     0,   318,
     319,   320,     0,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,   314,   333,   334,
     335,   315,   311,   336,   337,   338,     0,   367,     0,   312,
     936,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   316,   339,   340,   341,   342,   343,   344,     0,     0,
     347,   348,   349,   350,     0,   352,   353,   317,     0,   318,
     319,   320,     0,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,   314,   333,   334,
     335,   315,   311,   336,   337,   338,     0,   367,     0,   312,
     937,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   316,   339,   340,   341,   342,   343,   344,     0,     0,
     347,   348,   349,   350,     0,   352,   353,   317,     0,   318,
     319,   320,     0,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,   314,   333,   334,
     335,   315,   311,   336,   337,   338,     0,   367,     0,   312,
     959,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   316,   339,   340,   341,   342,   343,   344,     0,     0,
     347,   348,   349,   350,     0,   352,   353,   317,     0,   318,
     319,   320,     0,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,   314,   333,   334,
     335,   315,     0,   336,   337,   338,     0,   367,     0,     0,
     960,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     311,     0,     0,     0,     0,     0,     0,   312,     0,     0,
       0,   316,   339,   340,   341,   342,   343,   344,   313,   392,
     347,   348,   349,   350,     0,   352,   353,   317,     0,   318,
     319,   320,   393,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,   337,   338,     0,   367,     0,     0,
       0,     0,     0,     0,     0,   314,     0,     0,     0,   315,
     311,     0,     0,     0,     0,     0,     0,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   313,   389,
       0,     0,     0,     0,     0,   394,     0,     0,     0,   316,
       0,     0,   390,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   317,     0,   318,   319,   320,
       0,   321,   322,   323,     0,   324,   325,   326,   327,   328,
       0,   329,   330,   331,   332,   314,   333,   334,   335,   315,
     311,   336,   337,   338,     0,   367,     0,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   313,   688,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   316,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   317,     0,   318,   319,   320,
       0,   321,   322,   323,     0,   324,   325,   326,   327,   328,
       0,   329,   330,   331,   332,   314,   333,   334,   335,   315,
     311,   336,   337,   338,     0,   367,     0,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   313,   232,
       0,     0,     0,     0,     0,   394,     0,     0,     0,   316,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   317,     0,   318,   319,   320,
       0,   321,   322,   323,     0,   324,   325,   326,   327,   328,
       0,   329,   330,   331,   332,   314,   333,   334,   335,   315,
     311,   336,   337,   338,     0,   367,     0,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   313,   682,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   316,
       0,     0,     0,     0,     0,     0,   311,     0,     0,     0,
       0,     0,     0,   312,     0,   317,     0,   318,   319,   320,
       0,   321,   322,   323,   313,   324,   325,   326,   327,   328,
       0,   329,   330,   331,   332,   314,   333,   334,   335,   315,
       0,   336,   337,   338,     0,   367,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,     0,     0,     0,     0,     0,     0,   312,   316,
       0,   314,     0,     0,     0,   315,     0,     0,     0,   313,
       0,     0,     0,     0,   700,   317,     0,   318,   319,   320,
       0,   321,   322,   323,     0,   324,   325,   326,   327,   328,
       0,   329,   330,   331,   332,   316,   333,   334,   335,     0,
       0,   336,   337,   338,     0,   367,     0,     0,     0,     0,
       0,   317,     0,   318,   319,   320,   314,   321,   322,   323,
     315,   324,   325,   326,   327,   328,     0,   329,   330,   331,
     332,     0,   333,   334,   335,     0,   311,   336,   337,   338,
       0,   367,     0,   312,     0,     0,     0,     0,     0,     0,
     316,     0,     0,     0,   313,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,   317,     0,   318,   319,
     320,     0,   321,   322,   323,     0,   324,   325,   326,   327,
     328,     0,   329,   330,   331,   332,   399,   333,   334,   335,
       0,   311,   336,   337,   338,     0,   428,     0,   312,     0,
       0,   314,     0,     0,     0,   315,     0,     0,     0,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   369,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   316,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   317,     0,   318,   319,   320,   314,   321,   322,   323,
     315,   324,   325,   326,   327,   328,     0,   329,   330,   331,
     332,     0,   333,   334,   335,     0,   400,   336,   337,   338,
     311,     0,     0,     0,     0,     0,   155,   312,     0,     0,
     316,     0,     0,     0,     0,     0,     0,     0,   313,     0,
       0,     0,     0,   441,     0,     0,   317,     0,   318,   319,
     320,     0,   321,   322,   323,     0,   324,   325,   326,   327,
     328,     0,   329,   330,   331,   332,     0,   333,   334,   335,
     442,     0,   336,   337,   338,   311,     0,     0,     0,     0,
       0,     0,   312,     0,     0,   314,     0,     0,     0,   315,
       0,     0,     0,   313,   528,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   529,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   316,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   317,     0,   318,   319,   320,
     314,   321,   322,   323,   315,   324,   325,   326,   327,   328,
       0,   329,   330,   331,   332,     0,   333,   334,   335,   311,
       0,   336,   337,   338,     0,     0,   312,     0,     0,     0,
       0,     0,     0,     0,   316,     0,     0,   313,   530,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     317,   531,   318,   319,   320,     0,   321,   322,   323,     0,
     324,   325,   326,   327,   328,     0,   329,   330,   331,   332,
       0,   333,   334,   335,   311,     0,   336,   337,   338,     0,
       0,   312,     0,     0,   314,     0,     0,     0,   315,     0,
       0,     0,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   311,     0,     0,     0,     0,
       0,     0,   312,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,   313,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   317,     0,   318,   319,   320,   314,
     321,   322,   323,   315,   324,   325,   326,   327,   328,     0,
     329,   330,   331,   332,     0,   333,   334,   335,     0,     0,
     336,   337,   338,     0,     0,     0,     0,     0,     0,     0,
     314,     0,     0,   316,   315,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   317,
       0,   318,   319,   320,     0,   321,   322,   323,     0,   324,
     325,   326,   327,   328,   316,   329,   330,   331,   332,     0,
     333,   334,   335,     0,   432,   336,   337,   338,     0,     0,
     317,     0,   318,   319,   320,     0,   321,   322,   323,     0,
     324,   325,   326,   327,   328,     0,   329,   330,   331,   332,
       0,   333,   334,   335,   311,   434,   336,   337,   338,     0,
       0,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    16,   311,     0,     0,    20,    21,
       0,     0,   312,     0,     0,     0,     0,    27,     0,    28,
     855,     0,     0,   313,    33,     0,     0,     0,     0,    37,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   314,
       0,     0,     0,   315,     0,     0,     0,     0,    48,     0,
      50,     0,   298,     0,   856,     0,     0,   857,     0,    57,
     311,    58,     0,     0,     0,     0,     0,   312,     0,     0,
     314,     0,     0,   316,   315,     0,     0,     0,   313,     0,
      74,     0,     0,    76,     0,     0,    79,     0,     0,   317,
       0,   318,   319,   320,     0,   321,   322,   323,     0,   324,
     325,   326,   327,   328,   316,   329,   330,   331,   332,     0,
     333,   334,   335,     0,   436,   336,   337,   338,     0,     0,
     317,     0,   318,   319,   320,   314,   321,   322,   323,   315,
     324,   325,   326,   327,   328,    92,   329,   330,   331,   332,
       0,   333,   334,   335,   311,   444,   336,   337,   338,     0,
       0,   312,     0,     0,     0,     0,     0,     0,   459,   316,
       0,     0,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   317,     0,   318,   319,   320,
       0,   321,   322,   323,     0,   324,   325,   326,   327,   328,
       0,   329,   330,   331,   332,     0,   333,   334,   335,   311,
       0,   336,   337,   338,     0,     0,   312,     0,     0,   314,
       0,     0,     0,   315,     0,     0,     0,   313,     0,     0,
       0,     0,   532,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   311,
       0,     0,     0,   316,     0,     0,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,     0,   317,
       0,   318,   319,   320,   314,   321,   322,   323,   315,   324,
     325,   326,   327,   328,     0,   329,   330,   331,   332,     0,
     333,   334,   335,     0,   311,   336,   337,   338,     0,     0,
       0,   312,     0,     0,     0,     0,     0,     0,   316,     0,
       0,     0,   313,     0,   314,     0,     0,     0,   315,     0,
       0,     0,     0,     0,   317,     0,   318,   319,   320,     0,
     321,   322,   323,     0,   324,   325,   326,   327,   328,  -475,
     329,   330,   331,   332,   155,   333,   334,   335,   316,     0,
     336,   337,   338,     0,     0,     0,     0,     0,     0,   314,
       0,     0,     0,   315,   317,     0,   318,   319,   320,     0,
     321,   322,   323,     0,   324,   325,   326,   327,   328,     0,
     329,   330,   331,   332,     0,   333,   334,   335,   311,   695,
     336,   337,   338,   316,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,     0,   317,
       0,   318,   319,   320,     0,   321,   322,   323,     0,   324,
     325,   326,   327,   328,     0,   329,   330,   331,   332,     0,
     333,   334,   335,   311,   671,   336,   337,   338,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,     0,   314,   698,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,     0,     0,     0,     0,     0,
       0,   312,     0,     0,     0,     0,     0,   316,     0,     0,
       0,     0,   313,     0,     0,     0,     0,     0,   314,     0,
       0,     0,   315,   317,     0,   318,   319,   320,     0,   321,
     322,   323,     0,   324,   325,   326,   327,   328,     0,   329,
     330,   331,   332,     0,   333,   334,   335,     0,     0,   336,
     337,   338,   316,     0,     0,     0,     0,     0,     0,   314,
       0,     0,     0,   315,     0,     0,     0,     0,   317,     0,
     318,   319,   320,     0,   321,   322,   323,     0,   324,   325,
     326,   327,   328,     0,   329,   330,   331,   332,     0,   333,
     334,   335,   739,   316,   336,   337,   338,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   317,
       0,   318,   319,   320,     0,   321,   322,   323,     0,   324,
     325,   326,   327,   328,     0,   329,   330,   331,   332,   205,
     333,   334,   335,   311,     0,   336,   337,   338,     0,     0,
     312,   206,   207,     0,     0,     0,     0,     0,   208,     0,
       0,   313,   798,     0,     0,     0,   209,     0,     0,     0,
       0,     0,   210,     0,   311,     0,     0,   211,     0,   212,
       0,   312,   213,     0,     0,     0,     0,     0,     0,   214,
       0,     0,   313,   801,     0,     0,     0,     0,     0,     0,
     215,     0,     0,     0,     0,     0,     0,     0,   314,   216,
       0,     0,   315,     0,     0,     0,     0,     0,   217,   218,
       0,   219,     0,   220,     0,   221,     0,     0,   222,     0,
       0,     0,   223,     0,     0,   224,     0,     0,     0,   314,
       0,     0,   316,   315,     0,     0,     0,     0,     0,   311,
       0,     0,     0,     0,     0,     0,   312,     0,   317,     0,
     318,   319,   320,     0,   321,   322,   323,   313,   324,   325,
     326,   327,   328,   316,   329,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,   337,   338,     0,     0,   317,
       0,   318,   319,   320,     0,   321,   322,   323,     0,   324,
     325,   326,   327,   328,     0,   329,   330,   331,   332,     0,
     333,   334,   335,     0,   314,   336,   337,   338,   315,   311,
       0,     0,     0,     0,     0,     0,   312,   872,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,     0,     0,   311,   889,     0,     0,     0,
       0,     0,   312,     0,   317,     0,   318,   319,   320,     0,
     321,   322,   323,   313,   324,   325,   326,   327,   328,     0,
     329,   330,   331,   332,   314,   333,   334,   335,   315,     0,
     336,   337,   338,     0,     0,     0,     0,   875,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
     314,     0,     0,     0,   315,   311,     0,     0,     0,     0,
       0,     0,   312,     0,   317,     0,   318,   319,   320,     0,
     321,   322,   323,   313,   324,   325,   326,   327,   328,     0,
     329,   330,   331,   332,   316,   333,   334,   335,     0,     0,
     336,   337,   338,     0,     0,     0,     0,     0,     0,     0,
     317,     0,   318,   319,   320,     0,   321,   322,   323,     0,
     324,   325,   326,   327,   328,     0,   329,   330,   331,   332,
     314,   333,   334,   335,   315,   311,   336,   337,   338,     0,
       0,     0,   312,   970,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   316,     0,     0,     0,     0,     0,
       0,   311,     0,     0,     0,     0,     0,     0,   312,     0,
     317,     0,   318,   319,   320,     0,   321,   322,   323,   313,
     324,   325,   326,   327,   328,     0,   329,   330,   331,   332,
     314,   333,   334,   335,   315,     0,   336,   337,   338,     0,
       0,     0,     0,   971,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   316,     0,   314,     0,     0,     0,
     315,   311,     0,     0,     0,     0,     0,     0,   312,   972,
     317,     0,   318,   319,   320,     0,   321,   322,   323,   313,
     324,   325,   326,   327,   328,     0,   329,   330,   331,   332,
     316,   333,   334,   335,     0,     0,   336,   337,   338,     0,
       0,     0,     0,     0,     0,     0,   317,     0,   318,   319,
     320,     0,   321,   322,   323,     0,   324,   325,   326,   327,
     328,     0,   329,   330,   331,   332,   314,   333,   334,   335,
     315,   311,   336,   337,   338,     0,     0,     0,   312,   973,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     316,     0,     0,     0,     0,     0,     0,   311,     0,     0,
       0,     0,     0,     0,   312,     0,   317,     0,   318,   319,
     320,     0,   321,   322,   323,   313,   324,   325,   326,   327,
     328,     0,   329,   330,   331,   332,   314,   333,   334,   335,
     315,     0,   336,   337,   338,     0,     0,     0,     0,   974,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     316,     0,   314,     0,     0,     0,   315,   311,     0,     0,
       0,     0,     0,     0,   312,   975,   317,     0,   318,   319,
     320,     0,   321,   322,   323,   313,   324,   325,   326,   327,
     328,     0,   329,   330,   331,   332,   316,   333,   334,   335,
       0,     0,   336,   337,   338,     0,     0,     0,   997,     0,
       0,     0,   317,     0,   318,   319,   320,     0,   321,   322,
     323,     0,   324,   325,   326,   327,   328,     0,   329,   330,
     331,   332,   314,   333,   334,   335,   315,   311,   336,   337,
     338,     0,     0,     0,   312,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   313,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   316,     0,     0,     0,
       0,     0,     0,   311,     0,     0,     0,     0,     0,     0,
     312,     0,   317,     0,   318,   319,   320,     0,   321,   322,
     323,   313,   324,   325,   326,   327,   328,     0,   329,   330,
     331,   332,   314,   333,   334,   335,   315,     0,   336,   337,
     338,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   316,     0,   659,     0,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   317,     0,   318,   319,   320,     0,   321,   322,
     323,     0,   324,   325,   326,   327,   328,     0,   329,   330,
     331,   332,   316,   333,   334,   335,     0,     0,   336,   337,
     338,     0,     0,     0,     0,     0,     0,     0,   317,     0,
     318,   319,   320,     0,   321,   322,   323,   313,   324,   325,
     326,   327,   328,     0,   329,   330,   331,   332,    16,   333,
     334,   335,    20,    21,   336,   337,   338,     0,     0,     0,
       0,    27,     0,    28,   855,     0,     0,     0,    33,     0,
       0,     0,     0,    37,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   314,     0,     0,     0,   315,     0,
       0,     0,    48,     0,    50,     0,    52,     0,   856,     0,
       0,   857,     0,    57,     0,    58,     0,   313,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,     0,    74,     0,     0,    76,     0,     0,
      79,     0,     0,     0,   317,     0,   318,   319,   320,   313,
     321,   322,   323,     0,   324,   325,   326,   327,     0,     0,
     329,   330,   331,   332,   314,   333,   334,   335,   315,     0,
     336,   337,   338,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,     0,    92,
       0,     0,     0,     0,     0,   933,   314,     0,   316,     0,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   317,     0,   318,   319,   320,     0,
     321,   322,   323,     0,   324,   325,   326,   327,     0,     0,
     329,   330,   331,   332,   314,   333,     0,   335,   315,     0,
     336,   337,   338,     0,     0,     0,   317,     0,   318,   319,
     320,     0,   321,   322,   323,     0,   324,   325,   326,   327,
       0,     0,   329,   330,   331,   332,     0,   333,     0,   335,
       0,     0,   336,   337,   338,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   317,     0,   318,   319,   320,     0,
     321,   322,   323,     0,     0,   325,   326,   327,     0,     0,
     329,   330,   331,   332,     0,     0,     0,   335,     0,     0,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   354,     0,     0,
    -503,     0,  -503
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-897)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,     2,   140,   178,    15,   437,    20,    18,    59,    54,
      55,    17,   128,    24,   558,   360,   191,    28,    29,   716,
      31,    32,    33,    34,   646,   904,   186,    78,   295,    28,
     409,    42,   406,    44,    45,     1,    47,     1,    49,     1,
      51,   741,   789,   790,   204,    96,    29,   586,    59,    60,
      61,    62,    63,    64,   831,    42,   204,    68,    29,    70,
       3,    72,    73,   179,    29,    61,    77,    78,   828,    80,
      81,     1,    51,    84,   399,    60,    87,    88,    89,    29,
      91,   105,   242,    94,    95,    96,   942,    51,   414,    78,
      19,   987,   130,     1,   242,    47,   793,     0,    42,    61,
      42,   261,   110,    19,   150,    75,   102,   130,   105,    79,
      95,   437,   656,   261,    78,   166,    42,   442,    70,   658,
     158,    51,   105,    42,   103,  1004,   134,    91,  1024,    42,
     268,   987,    76,   893,    42,   832,   160,   160,    78,   103,
     102,   918,    31,   130,   110,    74,    89,    42,    78,   160,
     161,   162,   163,   164,   165,   166,    28,   130,    74,   859,
     156,   150,   322,   129,   130,   129,   155,   130,     3,    98,
     130,   177,   155,   103,   713,   186,    28,   924,   925,   193,
     130,   881,    98,   156,   155,   199,   150,   150,   130,   200,
     155,   155,   158,   204,   156,   892,   356,   357,   158,   129,
     110,   200,   110,    75,   130,   155,   585,    79,   356,   357,
     105,   130,   130,   558,    24,   155,    76,   130,   160,    42,
     130,   129,   130,    75,   134,   155,     3,    79,   130,   130,
     130,   242,   243,   130,   160,   130,   103,   157,   156,   250,
     251,   160,   150,    78,   941,   130,   868,   160,   158,   150,
     261,   130,   157,    76,    89,    65,   158,   129,   158,   131,
     132,   158,     7,   130,   136,   160,    76,    42,   130,   385,
     430,   150,   144,   158,   130,   130,   820,   129,    86,   290,
     132,   153,   430,   157,   136,    95,   287,    62,    75,   556,
     557,   101,    79,   157,   150,   150,   158,    72,    73,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   656,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   707,   155,
     130,   705,   150,    24,   345,   346,   387,    51,   156,   130,
     351,    72,    73,   354,   355,   356,   357,   358,   359,   360,
     150,   110,   130,    71,   150,  1002,   623,   624,   369,   150,
     345,   346,    24,   374,    78,   376,   351,   741,    95,   354,
     355,   130,   130,  1020,    65,   134,   387,    91,   389,   390,
     158,   392,   393,   397,   544,    76,    24,   398,   399,   103,
     401,   129,   403,   150,   132,   150,   150,     3,   136,   158,
     158,   176,   157,    65,   159,   416,   838,   948,   140,   150,
     101,   157,   423,   159,    76,   156,   467,   150,   157,   430,
     159,   196,   197,   198,    30,   314,   315,    65,   203,   150,
     441,   442,   157,    95,   159,   150,   150,   448,    76,   614,
     451,   155,   453,   150,   455,   456,   457,    53,   459,   460,
     991,   992,   993,   994,   995,   996,   467,    95,     2,   186,
     235,   157,   150,   238,   465,   132,   451,    11,   453,   358,
     359,   155,   150,   484,   485,   820,   451,   204,   453,    23,
     369,    25,   257,    89,   104,   157,    51,   157,   157,    95,
     864,   150,   110,   150,     7,    55,   129,   155,   150,   110,
     130,   390,   277,   158,   393,   831,   150,   881,   646,    42,
     157,    31,   838,    78,   130,   242,   158,   528,   529,   530,
     531,   532,   533,   534,    68,   110,    91,   158,    18,   535,
     536,   542,   158,   544,   261,   158,   110,   548,   103,   550,
     130,   160,   155,   554,   595,   150,   150,   558,   150,   150,
      94,   150,    39,   160,    98,   158,    24,   542,   150,   130,
      31,   135,   150,   137,   138,   158,   455,   141,   142,   150,
     156,    51,   150,   150,    42,   158,   587,    45,   130,   590,
     591,   592,   593,   594,   595,   150,   156,    74,   599,   110,
     155,    76,   918,    29,    62,   370,    29,    65,    78,    67,
     155,     2,   160,    31,   129,   129,    76,   160,    76,   160,
      11,    91,   156,   160,   599,   156,   942,   157,   132,    31,
     132,   396,    23,   103,    25,   132,   132,    95,   132,   356,
     357,   957,   132,   132,   121,   132,   123,   132,   132,   132,
     529,   158,   531,   158,    91,   656,   156,   130,   659,   660,
     661,   160,   300,   664,   442,   441,    22,   269,   727,    42,
     192,   987,   130,   546,   806,   744,   868,    68,    51,   680,
     150,   682,   723,   864,   685,   155,  1002,   688,   705,   578,
     747,   225,   693,  1004,   695,   941,   966,   698,   232,   997,
     158,   178,   990,    94,  1020,    78,   423,    98,  1024,    33,
     417,   544,    -1,   430,   191,    -1,    -1,   194,    -1,    -1,
      -1,    -1,   723,    -1,    -1,    -1,    -1,    28,    -1,    -1,
     103,   448,    -1,   734,   451,   736,   453,   110,   739,   456,
     868,    -1,   743,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   228,    -1,   287,    -1,    -1,    -1,   130,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,   300,    -1,    24,   246,
      -1,    -1,    -1,    -1,    75,    -1,    -1,   150,    79,    -1,
     659,   660,   155,    -1,   785,   158,    42,   552,   553,    45,
      -1,   556,   557,    -1,     2,    -1,   797,   798,    -1,   800,
     801,   802,   803,    11,   569,    -1,    62,   808,   809,    65,
     811,    67,   813,   814,    -1,    23,   817,    25,    -1,   820,
      76,   856,   857,    -1,   125,   542,    -1,   544,   129,    -1,
     131,   132,    -1,   367,   225,   136,   837,   838,    -1,    95,
      -1,   232,   843,   144,   845,   610,    -1,    -1,   849,    -1,
     151,   152,   153,   894,   855,   389,    -1,    -1,   392,    -1,
      68,    -1,    -1,    -1,    72,    73,    -1,   401,     3,    -1,
      -1,   872,   873,   874,   875,   876,   877,   878,   879,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    94,    22,   889,    -1,
      98,    -1,    -1,   894,    -1,    -1,   287,    -1,   899,    -1,
      -1,    -1,   903,    -1,   669,   670,    41,   384,    -1,   300,
      10,    46,   677,    -1,   679,    -1,    -1,    17,    53,   684,
     961,    -1,   687,   457,    -1,   690,    -1,    -1,    28,    -1,
       2,   465,   409,   934,   935,   936,   937,    -1,    73,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      85,    23,    -1,    25,    89,    -1,   957,    -1,   959,   960,
     961,    96,    -1,    -1,    28,   730,    -1,    -1,    -1,   970,
     971,   972,   973,   974,   975,    75,   367,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,   986,   987,    -1,   989,   466,
      -1,   525,   469,    -1,    42,    -1,    68,    -1,   389,    -1,
      -1,   392,    -1,    51,    -1,   105,    -1,    -1,    -1,   109,
     401,    75,    -1,    -1,    -1,    79,    -1,   225,    -1,  1020,
      -1,    -1,    94,  1024,   232,   125,    98,   127,   128,   129,
      78,   131,   132,   133,   568,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    -1,   146,   147,   148,    -1,
      -1,   151,   152,   153,    -1,   103,    -1,    -1,    -1,    -1,
     160,   125,   110,   127,   128,   129,   457,   131,   132,    -1,
      -1,    -1,   136,    -1,   465,    -1,   841,    -1,    -1,   287,
     144,    -1,   130,    -1,   156,    -1,    -1,   151,   152,   153,
      -1,    -1,   300,    -1,   628,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,   581,    -1,    42,   155,   585,    -1,
     158,    -1,   646,    -1,   831,    51,    -1,    -1,   883,   884,
      -1,   886,   887,    -1,    -1,   659,   660,    -1,    28,    -1,
      -1,    -1,    -1,   610,    -1,    -1,    -1,   614,    -1,   904,
      -1,    -1,    78,    -1,    -1,   910,   680,   912,   682,   914,
      -1,   685,    -1,   225,   688,    -1,    -1,    -1,    -1,   367,
     232,    -1,    -1,   697,    -1,    -1,   700,   103,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    75,    -1,   568,    -1,    79,
      -1,   389,   899,    -1,   392,    -1,    -1,    -1,   722,    -1,
      -1,    -1,    -1,   401,   130,    -1,    -1,    -1,    -1,    -1,
     734,    -1,   736,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     2,    -1,   150,   287,    62,    -1,    -1,   155,
      -1,    11,   158,    -1,    -1,   942,    -1,   704,   300,   129,
     707,   131,   132,    23,    -1,    25,   136,   628,    -1,  1004,
      -1,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,   457,
     727,   151,   152,   153,    -1,    -1,    -1,   465,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   744,    -1,    -1,
     987,    -1,    -1,    -1,   808,    -1,    -1,    -1,    68,   813,
      -1,    -1,    -1,    28,    -1,    -1,    -1,   764,    -1,   680,
      -1,   682,    -1,   827,   685,   367,    -1,   688,    -1,    -1,
      23,    24,    -1,    -1,    94,   839,   697,  1024,    98,   700,
      -1,    34,    35,    -1,   848,    -1,    -1,   389,   852,    -1,
     392,    44,    -1,    -1,    47,    -1,    -1,    -1,    -1,   401,
      75,   722,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,   734,    -1,   736,    -1,    70,     2,    -1,
     196,   197,   198,    76,    -1,    -1,   890,    11,     2,    -1,
     568,    -1,    -1,    -1,    -1,    -1,   156,    11,    -1,    23,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,   101,    23,
     125,    25,    -1,   128,   129,   457,   131,   132,    -1,   235,
      -1,   136,   238,   465,    -1,    -1,    -1,    -1,    -1,   144,
     934,   935,   936,   937,    -1,    -1,   151,   152,   153,    -1,
      -1,   257,    -1,    -1,    68,    -1,    -1,   808,    -1,    -1,
     628,    -1,   813,    -1,    68,   959,   960,    -1,    -1,    -1,
      -1,   277,    -1,    -1,    -1,   225,   827,    -1,    -1,    -1,
      94,     1,   232,     3,    98,    -1,    -1,    -1,   839,    -1,
      94,    -1,    -1,    -1,    98,    15,    16,   848,    -1,    -1,
      -1,   852,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,   680,    -1,   682,    -1,    36,   685,    -1,    -1,
     688,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,   697,
      -1,    -1,   700,    53,    -1,    -1,   568,   287,    -1,   890,
      -1,    -1,   156,    -1,    64,    -1,    -1,    -1,    -1,    -1,
     300,    -1,   156,    73,   722,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,   370,    85,   734,    87,   736,    89,
      -1,     2,    92,    -1,    -1,    -1,    96,    -1,    -1,    99,
      11,    -1,    -1,   934,   935,   936,   937,    -1,    -1,    -1,
     396,    -1,    23,    -1,    25,    -1,   628,    -1,    -1,    -1,
      -1,    23,    24,    -1,    -1,    -1,    24,    -1,   959,   960,
      -1,   225,    34,    35,    -1,    -1,    -1,   367,   232,    -1,
      -1,   225,    44,    -1,    42,    -1,    -1,    45,   232,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,   389,
     808,    63,   392,    65,    62,   813,    -1,    65,   680,    67,
     682,   401,    -1,   685,    76,    -1,   688,    -1,    76,   827,
      -1,    -1,    -1,    94,    -1,   697,    -1,    98,   700,    -1,
      -1,   839,    -1,   287,    -1,    -1,    -1,    95,    -1,   101,
     848,    -1,    -1,   287,   852,    -1,   300,    -1,    -1,    -1,
     722,    -1,    -1,    -1,    -1,    -1,   300,    -1,    -1,    -1,
      -1,    -1,   734,    -1,   736,    -1,    -1,   457,    -1,    -1,
      -1,    -1,    -1,    -1,     2,   465,    -1,    -1,    -1,    -1,
      -1,    -1,   890,    11,    -1,   156,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   552,   553,    -1,    -1,
     556,   557,    -1,   367,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   367,    -1,    -1,   934,   935,   936,   937,
      -1,    -1,    -1,    -1,    -1,   389,   808,    -1,   392,    -1,
      68,   813,    -1,    -1,    -1,   389,    -1,   401,   392,    -1,
      -1,   959,   960,    -1,   225,   827,    -1,    -1,    -1,    -1,
      -1,   232,    -1,    -1,    -1,    -1,    94,   839,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,   848,    -1,   568,    -1,
     852,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      16,    -1,    -1,   457,    -1,    -1,    22,    -1,    -1,    -1,
      -1,   465,    -1,   457,    30,    -1,   287,    -1,   890,    -1,
      36,   465,    -1,   669,   670,    41,    -1,    43,   156,   300,
      46,   677,    -1,    -1,    -1,    -1,    -1,    53,   628,    -1,
      -1,   687,    -1,    -1,   690,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,
      -1,    -1,   934,   935,   936,   937,    82,    83,    -1,    85,
      -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,    -1,
      96,    97,    -1,    99,    -1,    -1,    -1,   959,   960,    -1,
     680,    -1,   682,    -1,    -1,   685,   367,   225,   688,    -1,
      -1,    -1,    -1,    -1,   232,    -1,    -1,   697,    -1,    -1,
     700,    -1,    -1,    -1,   568,    -1,    -1,    -1,   389,    -1,
      -1,   392,    -1,    19,   568,    -1,    -1,    23,    24,    -1,
     401,    -1,   722,    -1,    -1,    -1,    32,    -1,    34,    35,
      -1,   157,    -1,    39,   734,    -1,   736,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   287,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    67,   300,    69,   628,    -1,    72,    -1,    74,    -1,
      76,    -1,    -1,    -1,   628,    -1,   457,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   465,   841,    -1,    -1,    -1,    95,
      -1,    -1,    98,    -1,    -1,   101,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   808,    -1,
      -1,    -1,    -1,   813,    -1,    -1,   680,    -1,   682,    -1,
      -1,   685,    -1,    -1,   688,    -1,   680,   827,   682,   367,
      -1,   685,    -1,   697,   688,    -1,   700,    -1,    -1,   839,
      -1,    -1,    -1,   697,   150,    75,   700,    -1,   848,    79,
     156,   389,   852,    -1,   392,    -1,    -1,    -1,   722,    -1,
      -1,    -1,    -1,   401,    -1,    -1,    -1,    -1,   722,    -1,
     734,    -1,   736,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     734,    -1,   736,    -1,    -1,    -1,    -1,   568,    -1,    -1,
     890,    -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    -1,    -1,   136,    -1,     1,    -1,
      -1,    -1,    -1,   143,   144,    -1,    -1,    -1,   148,   457,
      -1,   151,   152,   153,    -1,    -1,    19,   465,    -1,    -1,
      23,    24,    -1,    -1,   934,   935,   936,   937,    -1,    32,
      -1,    34,    35,    -1,   808,    -1,    39,   628,    -1,   813,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    -1,   959,
     960,    -1,    -1,   827,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,   827,    67,   839,    69,    70,    -1,    72,
      -1,    74,    10,    76,   848,   839,    -1,    -1,   852,    17,
      -1,    -1,    -1,    -1,   848,    -1,    -1,    -1,   852,   680,
      28,   682,    95,    -1,   685,    98,    -1,   688,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   697,    -1,    -1,   700,
      -1,    -1,    -1,    -1,    -1,    -1,   890,    -1,    -1,    -1,
     568,    -1,    -1,    -1,    -1,    -1,   890,    -1,    -1,    -1,
      -1,   722,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    79,    -1,   734,    -1,   736,    -1,   150,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,
     934,   935,   936,   937,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     628,    -1,    -1,    -1,    -1,   959,   960,   125,    -1,   127,
     128,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
     138,   139,    -1,   141,   142,   143,   144,    -1,   146,   147,
     148,    -1,    -1,   151,   152,   153,    -1,   808,    -1,    -1,
     158,    -1,   813,    -1,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,   680,    -1,   682,    -1,   827,   685,    15,    16,
     688,    -1,    -1,    -1,    -1,    22,    -1,    -1,   839,   697,
      -1,    -1,   700,    30,    -1,    -1,    -1,   848,    -1,    36,
      -1,   852,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    -1,   722,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   734,    64,   736,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,   890,
      -1,    -1,    -1,    -1,    -1,    82,    83,    -1,    85,    -1,
      87,    -1,    89,    -1,    -1,    92,    -1,    -1,    -1,    96,
      -1,     1,    99,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    16,    17,    -1,    -1,
      -1,    -1,    22,   934,   935,   936,   937,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,
     808,    41,    -1,    43,    -1,   813,    46,    -1,   959,   960,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,   155,   827,
     157,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,   839,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,
     848,    -1,    82,    83,   852,    85,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    96,    -1,    -1,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,   114,   115,    -1,    -1,   118,   119,
     120,   121,   890,   123,   124,   125,   126,   127,   128,    -1,
      -1,   131,    -1,    -1,    -1,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,   148,    -1,
      -1,   151,   152,   153,   154,    -1,    -1,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   934,   935,   936,   937,
      -1,    -1,    -1,    -1,     0,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,   959,   960,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    -1,
      46,    47,    48,    49,    50,    -1,    52,    53,    54,    55,
      56,    57,    -1,    59,    -1,    -1,    -1,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    -1,    80,    81,    82,    83,    84,    85,
      86,    87,    -1,    89,    90,    -1,    92,    93,    94,    95,
      96,    -1,    98,    99,   100,   101,    -1,    -1,   104,    -1,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,   145,
      -1,   147,   148,    -1,   150,    -1,    -1,   153,    -1,   155,
      -1,   157,     1,   159,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    28,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    -1,    53,    54,    -1,    56,    57,    -1,
      -1,    -1,    75,    -1,    -1,    64,    79,    66,    -1,    68,
      -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,   125,    -1,   127,   128,   129,    -1,   131,   132,
      -1,    -1,    -1,   136,    -1,    -1,   125,   126,   127,   128,
     143,   144,    -1,    -1,   133,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,   148,
     149,    -1,    -1,    -1,   153,    -1,   155,    -1,   157,    -1,
     159,   160,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    10,    -1,    12,    13,    14,    15,    16,    17,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    48,
      49,    50,    -1,    52,    53,    54,    55,    56,    57,    -1,
      59,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    -1,    92,    93,    94,    95,    96,    -1,    98,
      99,   100,   101,    -1,    -1,   104,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,    -1,   135,   136,   137,   138,
     139,    -1,   141,   142,   143,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,    -1,   155,   156,   157,     1,
     159,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    -1,    46,    47,    48,    49,    50,    -1,
      52,    53,    54,    55,    56,    57,    -1,    59,    -1,    -1,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    -1,
      92,    93,    94,    95,    96,    -1,    98,    99,   100,   101,
      -1,    -1,   104,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,   145,    -1,   147,   148,   149,   150,    -1,
      -1,   153,    -1,   155,   156,   157,     1,   159,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      -1,    46,    47,    48,    49,    50,    -1,    52,    53,    54,
      55,    56,    57,    -1,    59,    -1,    -1,    -1,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    -1,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    -1,    92,    93,    94,
      95,    96,    -1,    98,    99,   100,   101,    -1,    -1,   104,
      -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
     145,    -1,   147,   148,    -1,   150,    -1,    -1,   153,    -1,
     155,   156,   157,     1,   159,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    46,    47,
      48,    49,    50,    -1,    52,    53,    54,    55,    56,    57,
      -1,    59,    -1,    -1,    -1,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    -1,    80,    81,    82,    83,    84,    85,    86,    87,
      -1,    89,    90,    -1,    92,    93,    94,    95,    96,    -1,
      98,    99,   100,   101,    -1,    -1,   104,    -1,   106,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
     148,    -1,   150,    -1,    -1,   153,    -1,   155,   156,   157,
       1,   159,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    48,    49,    50,
      -1,    52,    53,    54,    55,    56,    57,    -1,    59,    -1,
      -1,    -1,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    77,    78,    -1,    80,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      -1,    92,    93,    94,    95,    96,    -1,    98,    99,   100,
     101,    -1,    -1,   104,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,   145,    -1,   147,   148,    -1,   150,
      -1,    -1,   153,    -1,   155,   156,   157,     1,   159,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    48,    49,    50,    -1,    52,    53,
      54,    55,    56,    57,    -1,    59,    -1,    -1,    -1,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    76,    77,    78,    -1,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    -1,    92,    93,
      94,    95,    96,    -1,    98,    99,   100,   101,    -1,    -1,
     104,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,   145,    -1,   147,   148,    -1,   150,    -1,    -1,   153,
      -1,   155,   156,   157,     1,   159,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    -1,    46,
      47,    48,    49,    50,    -1,    52,    53,    54,    55,    56,
      57,    -1,    59,    -1,    -1,    -1,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    -1,    92,    93,    94,    95,    96,
      -1,    98,    99,   100,   101,    -1,    -1,   104,    -1,   106,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,   145,    -1,
     147,   148,    -1,   150,    -1,    -1,   153,    -1,   155,    -1,
     157,     1,   159,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    48,    49,
      50,    -1,    52,    53,    54,    55,    56,    57,    -1,    59,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    -1,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    89,
      90,    -1,    92,    93,    94,    95,    96,    -1,    98,    99,
     100,   101,    -1,    -1,   104,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,   145,    -1,   147,   148,    -1,
     150,    -1,    -1,   153,    -1,   155,    -1,   157,     1,   159,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    48,    49,    50,    -1,    52,
      53,    54,    55,    56,    57,    -1,    59,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,    74,    -1,    76,    77,    78,    -1,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    -1,    92,
      93,    94,    95,    96,    -1,    98,    99,   100,   101,    -1,
      -1,   104,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,   148,    -1,   150,    -1,    -1,
     153,    -1,   155,    -1,   157,     1,   159,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    -1,
      46,    47,    48,    49,    50,    -1,    52,    53,    54,    55,
      56,    57,    -1,    59,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    -1,    80,    81,    82,    83,    84,    85,
      86,    87,    -1,    89,    90,    -1,    92,    93,    94,    95,
      96,    -1,    98,    99,   100,   101,    -1,    -1,   104,    -1,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,   145,
      -1,   147,   148,    -1,   150,    -1,    -1,   153,    -1,   155,
      -1,   157,     1,   159,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    48,
      49,    50,    -1,    52,    53,    54,    55,    56,    57,    -1,
      59,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      -1,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    -1,    92,    93,    94,    95,    96,    -1,    98,
      99,   100,   101,    -1,    -1,   104,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,   148,
      -1,   150,    -1,    -1,   153,    -1,   155,    -1,   157,     1,
     159,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    -1,    46,    47,    48,    49,    50,    -1,
      52,    53,    54,    55,    56,    57,    -1,    59,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    -1,
      92,    93,    94,    95,    96,    -1,    98,    99,   100,   101,
      -1,    -1,   104,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,   145,    -1,   147,   148,    -1,   150,    -1,
      -1,   153,    -1,   155,    -1,   157,     1,   159,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      -1,    46,    47,    48,    49,    50,    -1,    52,    53,    54,
      55,    56,    57,    -1,    59,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    -1,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    -1,    92,    93,    94,
      95,    96,    -1,    98,    99,   100,   101,    -1,    -1,   104,
      -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
     145,    -1,   147,   148,    -1,   150,    -1,    -1,   153,    -1,
     155,    -1,   157,     1,   159,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    46,    47,
      48,    49,    50,    -1,    52,    53,    54,    55,    56,    57,
      -1,    59,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    -1,    80,    81,    82,    83,    84,    85,    86,    87,
      -1,    89,    90,    -1,    92,    93,    94,    95,    96,    -1,
      98,    99,   100,   101,    -1,    -1,   104,    -1,   106,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
     148,    -1,   150,    -1,    -1,   153,    -1,   155,    -1,   157,
       1,   159,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    48,    49,    50,
      -1,    52,    53,    54,    55,    56,    57,    -1,    59,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    77,    78,    -1,    80,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      -1,    92,    93,    94,    95,    96,    -1,    98,    99,   100,
     101,    -1,    -1,   104,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,   145,    -1,   147,   148,    -1,   150,
      -1,    -1,   153,    -1,   155,    -1,   157,     1,   159,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    48,    49,    50,    -1,    52,    53,
      54,    55,    56,    57,    -1,    59,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    -1,    76,    77,    78,    -1,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    -1,    92,    93,
      94,    95,    96,    -1,    98,    99,   100,   101,    -1,    -1,
     104,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,   145,    -1,   147,   148,    -1,   150,    -1,    -1,   153,
      -1,   155,    -1,   157,     1,   159,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    -1,    46,
      47,    48,    49,    50,    -1,    52,    53,    54,    55,    56,
      57,    -1,    59,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    -1,    92,    93,    94,    95,    96,
      -1,    98,    99,   100,   101,    -1,    -1,   104,    -1,   106,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,   145,    -1,
     147,   148,    -1,   150,    -1,    -1,   153,    -1,   155,    -1,
     157,     1,   159,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      50,    -1,    -1,    53,    54,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    66,    -1,    68,    -1,
      -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    84,    85,    86,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    96,    -1,    -1,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,   145,    -1,   147,   148,   149,
      -1,    -1,    -1,   153,    -1,   155,    -1,   157,    -1,   159,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    -1,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    66,    -1,    68,    -1,    -1,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    -1,    89,    -1,    -1,    92,
      -1,    -1,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,   148,   149,    -1,    -1,    -1,
     153,    -1,   155,    -1,   157,    -1,   159,   160,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    50,    -1,    -1,    53,    54,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    66,    -1,    68,    -1,    -1,    -1,    -1,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,
      85,    86,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
     145,    -1,   147,   148,   149,    -1,    -1,    -1,   153,    -1,
     155,    -1,   157,    -1,   159,   160,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    -1,    53,    54,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    66,
      -1,    68,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,
      87,    -1,    89,    -1,    -1,    92,    -1,    -1,    -1,    96,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,   145,    -1,
     147,   148,    -1,    -1,    -1,    -1,   153,    -1,   155,    -1,
     157,    -1,   159,   160,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    35,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    -1,    53,    54,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    65,    66,    -1,    68,
      -1,    -1,    -1,    -1,    73,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    96,    -1,    -1,
      99,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,   148,
      -1,    -1,    -1,    -1,   153,    -1,   155,    -1,   157,    -1,
     159,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    19,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      -1,    53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    66,    -1,    68,    -1,    -1,    -1,
      -1,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    83,    84,    85,    86,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    95,    96,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,   143,    -1,   145,    12,   147,   148,    15,    16,    -1,
      -1,   153,    -1,   155,    22,   157,    -1,   159,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    50,    -1,    -1,    53,    54,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    66,    -1,
      68,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,    87,
      -1,    89,    -1,    -1,    92,    93,    94,    -1,    96,    97,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
     148,   149,    -1,    -1,    -1,   153,    -1,   155,    -1,   157,
      -1,   159,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    -1,    53,    54,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    66,    -1,    68,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    84,    85,    86,    87,    -1,    89,    -1,
      -1,    92,    93,    94,    -1,    96,    97,    -1,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,   145,    -1,   147,   148,   149,    -1,
      -1,    -1,   153,    -1,   155,    -1,   157,   158,   159,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    -1,    53,
      54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    66,    -1,    68,    -1,    -1,    -1,    -1,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,
      84,    85,    86,    87,    -1,    89,    -1,    -1,    92,    93,
      94,    -1,    96,    97,    -1,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,   145,    -1,   147,   148,   149,    -1,    -1,    -1,   153,
      -1,   155,    -1,   157,   158,   159,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    42,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    -1,    53,    54,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    66,
      -1,    68,    -1,    -1,    -1,    -1,    73,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,
      87,    -1,    89,    -1,    -1,    92,    -1,    -1,    -1,    96,
      -1,    -1,    99,    -1,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,   145,    -1,
     147,   148,    -1,    -1,    -1,    -1,   153,    -1,   155,    -1,
     157,    -1,   159,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    19,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      50,    -1,    -1,    53,    54,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    66,    -1,    68,    -1,
      -1,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    84,    85,    86,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    95,    96,    -1,    98,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,   143,    -1,   145,    12,   147,   148,    15,
      16,    -1,    -1,   153,    -1,   155,    22,   157,    -1,   159,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    -1,    53,    54,    -1,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      66,    -1,    68,    -1,    -1,    -1,    -1,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,
      86,    87,    -1,    89,    -1,    -1,    92,    93,    94,    -1,
      96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,   145,
      -1,   147,   148,    -1,   150,    -1,    -1,   153,    -1,   155,
      -1,   157,    -1,   159,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    -1,    53,    54,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    66,    -1,    68,
      -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    -1,
      89,    -1,    -1,    92,    93,    94,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,   148,
     149,    -1,    -1,    -1,   153,    -1,   155,    -1,   157,    -1,
     159,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      -1,    53,    54,    -1,    56,    57,    58,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    66,    -1,    68,    -1,    -1,    -1,
      -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    83,    84,    85,    86,    87,    -1,    89,    -1,    -1,
      92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,   143,    -1,   145,    12,   147,   148,    15,    16,    -1,
      -1,   153,    -1,   155,    22,   157,    -1,   159,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    50,    -1,    -1,    53,    54,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,    -1,
      68,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,    87,
      -1,    89,    -1,    -1,    92,    -1,    -1,    -1,    96,    -1,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,   143,    -1,   145,    12,   147,
     148,    15,    16,    -1,    -1,   153,    -1,   155,    22,   157,
      -1,   159,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    -1,    53,
      54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    66,    -1,    68,    -1,    -1,    -1,    -1,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,
      84,    85,    86,    87,    -1,    89,    -1,    -1,    92,    93,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   143,
      -1,   145,    12,   147,   148,    15,    16,    -1,    -1,   153,
      -1,   155,    22,   157,    -1,   159,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      50,    -1,    -1,    53,    54,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    66,    -1,    68,    -1,
      -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    84,    85,    86,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    96,    -1,    -1,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,   145,    -1,   147,   148,   149,
      -1,    -1,    -1,   153,    -1,   155,   156,   157,    -1,   159,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    -1,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    66,    -1,    68,    -1,    -1,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    -1,    89,    -1,    -1,    92,
      -1,    -1,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     143,    -1,   145,    12,   147,   148,    15,    16,    -1,    -1,
     153,    -1,   155,    22,   157,    -1,   159,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    -1,    53,    54,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    66,    -1,    68,
      -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,   148,
     149,    -1,    -1,    -1,   153,    -1,   155,    -1,   157,    -1,
     159,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      -1,    53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    66,    -1,    68,    -1,    -1,    -1,
      -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    83,    84,    85,    86,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    96,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,   143,    -1,   145,    12,   147,   148,    15,    16,    -1,
      -1,   153,    -1,   155,    22,   157,    -1,   159,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    50,    -1,    -1,    53,    54,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    66,    -1,
      68,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,    87,
      -1,    89,    -1,    -1,    92,    -1,    -1,    -1,    96,    -1,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
     148,   149,    -1,    -1,    -1,   153,    -1,   155,    -1,   157,
      -1,   159,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    -1,    53,    54,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    66,    -1,    68,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    84,    85,    86,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    96,    -1,    -1,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,   145,    -1,   147,   148,   149,    -1,
      -1,    -1,   153,    -1,   155,    -1,   157,    -1,   159,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    -1,    53,
      54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    66,    -1,    68,    -1,    -1,    -1,    -1,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,
      84,    85,    86,    87,    -1,    89,    -1,    -1,    92,    -1,
      -1,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,   145,    -1,   147,   148,   149,    -1,    -1,    -1,   153,
      -1,   155,    -1,   157,    -1,   159,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    -1,    53,    54,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    66,
      -1,    68,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,
      87,    -1,    89,    -1,    -1,    92,    -1,    -1,    -1,    96,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   143,    -1,   145,    12,
     147,   148,    15,    16,    -1,    -1,   153,    -1,   155,    22,
     157,    -1,   159,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    -1,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    66,    -1,    68,    -1,    -1,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    -1,    89,    -1,    -1,    92,
      -1,    -1,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     143,    -1,   145,    12,   147,   148,    15,    16,    -1,    -1,
     153,    -1,   155,    22,   157,    -1,   159,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    -1,    53,    54,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    66,    -1,    68,
      -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   143,    -1,   145,    12,   147,   148,
      15,    16,    -1,    -1,   153,    -1,   155,    22,   157,    -1,
     159,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    50,    -1,    -1,    53,    54,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    66,    -1,    68,    -1,    -1,    -1,    -1,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,
      85,    86,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   143,    -1,
     145,    12,   147,   148,    15,    16,    -1,    -1,   153,    -1,
     155,    22,   157,    -1,   159,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    -1,    53,    54,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    66,    -1,    68,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    84,    85,    86,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    96,    -1,    -1,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   143,    -1,   145,    12,   147,   148,    15,    16,
      -1,    -1,   153,    -1,   155,    22,   157,    -1,   159,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    -1,    53,    54,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    66,
      -1,    68,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,
      87,    -1,    89,    -1,    -1,    92,    -1,    -1,    -1,    96,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   143,    -1,   145,    12,
     147,   148,    15,    16,    -1,    -1,   153,    -1,   155,    22,
     157,    -1,   159,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    -1,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    66,    -1,    68,    -1,    -1,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    -1,    89,    -1,    -1,    92,
      -1,    -1,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     143,    -1,   145,    12,   147,   148,    15,    16,    -1,    -1,
     153,    -1,   155,    22,   157,    -1,   159,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    -1,    53,    54,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    66,    -1,    68,
      -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   143,    -1,   145,    12,   147,   148,
      15,    16,    -1,    -1,   153,    -1,   155,    22,   157,    -1,
     159,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    50,    -1,    -1,    53,    54,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    66,    -1,    68,    -1,    -1,    -1,    -1,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,
      85,    86,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   143,    -1,
     145,    12,   147,   148,    15,    16,    -1,    -1,   153,    -1,
     155,    22,   157,    -1,   159,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    -1,    53,    54,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    66,    -1,    68,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    84,    85,    86,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    96,    -1,    -1,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   143,    -1,   145,    12,   147,   148,    15,    16,
      -1,    -1,   153,    -1,   155,    22,   157,    -1,   159,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    -1,    53,    54,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    66,
      -1,    68,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,
      87,    -1,    89,    -1,    -1,    92,    -1,    -1,    -1,    96,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   143,    -1,   145,    12,
     147,   148,    15,    16,    -1,    -1,   153,    -1,   155,    22,
     157,    -1,   159,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    -1,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    66,    -1,    68,    -1,    -1,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    -1,    89,    -1,    -1,    92,
      -1,    -1,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     143,    -1,   145,    12,   147,   148,    15,    16,    -1,    -1,
     153,    -1,   155,    22,   157,    -1,   159,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    -1,    53,    54,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    66,    -1,    68,
      -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   143,    -1,   145,    12,   147,   148,
      15,    16,    -1,    -1,   153,    -1,   155,    22,   157,    -1,
     159,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    50,    -1,    -1,    53,    54,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    66,    -1,    68,    -1,    -1,    -1,    -1,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,
      85,    86,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   143,    -1,
     145,    12,   147,   148,    15,    16,    -1,    -1,   153,    -1,
     155,    22,   157,    -1,   159,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    -1,    53,    54,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    66,    -1,    68,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    84,    85,    86,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    96,    -1,    -1,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   143,    -1,   145,    12,   147,   148,    15,    16,
      -1,    -1,   153,    -1,   155,    22,   157,    -1,   159,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    -1,    53,    54,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    66,
      -1,    68,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,
      87,    -1,    89,    -1,    -1,    92,    -1,    -1,    -1,    96,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   143,    -1,   145,    12,
     147,   148,    15,    16,    -1,    -1,   153,    -1,    -1,    22,
     157,    -1,   159,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    -1,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    66,    -1,    68,    -1,    -1,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    -1,    89,    -1,    -1,    92,
      -1,    -1,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     143,    -1,   145,    12,    -1,   148,    15,    16,    -1,    -1,
     153,    -1,   155,    22,   157,    -1,   159,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    -1,    53,    54,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    66,    -1,    68,
      -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,   130,    -1,    -1,    -1,   134,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,   145,    -1,    15,    16,
     149,    -1,    -1,    -1,    -1,    22,   155,    -1,   157,   158,
     159,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    66,
      -1,    68,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    82,    83,    84,    85,    86,
      87,    -1,    89,    -1,    91,    92,    -1,    -1,    -1,    96,
      -1,    -1,    99,    -1,    -1,    -1,   103,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
       3,     4,     5,     6,     7,     8,    -1,   140,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,   145,    22,
      -1,   154,    -1,   150,   157,    -1,   159,    30,   155,    -1,
     157,    -1,   159,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    -1,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    66,    -1,    68,    -1,    -1,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    -1,    89,    -1,    -1,    92,
      -1,    -1,    -1,    96,    -1,    -1,    99,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,   145,    49,    50,    -1,    -1,    53,    54,    -1,
      56,    57,   155,    -1,   157,    -1,   159,    -1,    64,    -1,
      66,    -1,    68,    -1,    -1,    -1,    -1,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,
      86,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,    -1,
      96,    -1,    -1,    99,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,   145,
      49,    50,    -1,    -1,    53,    54,    -1,    56,    57,   155,
      -1,   157,    -1,   159,    -1,    64,    -1,    66,    -1,    68,
      -1,    -1,    -1,    -1,    73,    -1,     1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    -1,
      89,    -1,    -1,    92,    19,    -1,    -1,    96,    23,    24,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    -1,    67,    -1,    69,    70,   145,    72,    -1,    74,
       3,    76,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
     159,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      95,    -1,     3,    98,    -1,    -1,   101,    30,    -1,    -1,
      -1,    -1,    -1,    36,    15,    16,    -1,    -1,    41,    -1,
      43,    22,    -1,    46,    -1,    -1,    -1,    -1,    -1,    30,
      53,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,
      41,    64,    43,    -1,    -1,    46,    -1,    -1,    -1,    -1,
      73,    -1,    53,    -1,    -1,   150,    -1,    -1,    -1,    82,
      83,   156,    85,    64,    87,    -1,    89,    -1,    -1,    92,
      -1,    -1,    73,    96,    -1,    -1,    99,    -1,    -1,    -1,
       3,    82,    83,    -1,    85,    -1,    87,    10,    89,    -1,
      -1,    92,    15,    16,    17,    96,    -1,    -1,    99,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    -1,    -1,   157,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,    82,
      83,    -1,    85,    -1,    87,    -1,    89,    10,    -1,    92,
      -1,    -1,    -1,    96,    17,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,   110,   111,   112,
     113,   114,   115,    -1,    -1,   118,   119,   120,   121,    -1,
     123,   124,   125,   126,   127,   128,    -1,    -1,   131,    -1,
      -1,    -1,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,   148,    -1,    -1,   151,   152,
     153,   154,    75,    -1,    -1,    -1,    79,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,   125,    -1,   127,   128,   129,    -1,   131,   132,
     133,    28,   135,   136,   137,   138,   139,    -1,   141,   142,
     143,   144,    75,   146,   147,   148,    79,    -1,   151,   152,
     153,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,   109,    -1,    75,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,   125,    -1,   127,   128,   129,    -1,   131,   132,
     133,    -1,   135,   136,   137,   138,   139,    -1,   141,   142,
     143,   144,   109,   146,   147,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,    -1,    -1,    -1,    -1,   160,   125,    -1,
     127,   128,   129,    75,   131,   132,   133,    79,   135,   136,
     137,   138,   139,    -1,   141,   142,   143,   144,    -1,   146,
     147,   148,    10,    -1,   151,   152,   153,    -1,    -1,    17,
      -1,    -1,    -1,   160,    -1,    -1,    -1,   109,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,   127,   128,   129,    -1,   131,
     132,   133,    -1,   135,   136,   137,   138,   139,    -1,   141,
     142,   143,   144,    -1,   146,   147,   148,    -1,    -1,   151,
     152,   153,    -1,    -1,    -1,    -1,   158,    75,    -1,    -1,
      -1,    79,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,   121,    -1,   123,   124,   125,    -1,   127,
     128,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
     138,   139,    -1,   141,   142,   143,   144,    75,   146,   147,
     148,    79,    10,   151,   152,   153,    -1,   155,    -1,    17,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,   121,    -1,   123,   124,   125,    -1,   127,
     128,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
     138,   139,    -1,   141,   142,   143,   144,    75,   146,   147,
     148,    79,    10,   151,   152,   153,    -1,   155,    -1,    17,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,   121,    -1,   123,   124,   125,    -1,   127,
     128,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
     138,   139,    -1,   141,   142,   143,   144,    75,   146,   147,
     148,    79,    10,   151,   152,   153,    -1,   155,    -1,    17,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,   121,    -1,   123,   124,   125,    -1,   127,
     128,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
     138,   139,    -1,   141,   142,   143,   144,    75,   146,   147,
     148,    79,    10,   151,   152,   153,    -1,   155,    -1,    17,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,   121,    -1,   123,   124,   125,    -1,   127,
     128,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
     138,   139,    -1,   141,   142,   143,   144,    75,   146,   147,
     148,    79,    10,   151,   152,   153,    -1,   155,    -1,    17,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,   121,    -1,   123,   124,   125,    -1,   127,
     128,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
     138,   139,    -1,   141,   142,   143,   144,    75,   146,   147,
     148,    79,    10,   151,   152,   153,    -1,   155,    -1,    17,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,   121,    -1,   123,   124,   125,    -1,   127,
     128,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
     138,   139,    -1,   141,   142,   143,   144,    75,   146,   147,
     148,    79,    10,   151,   152,   153,    -1,   155,    -1,    17,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,   121,    -1,   123,   124,   125,    -1,   127,
     128,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
     138,   139,    -1,   141,   142,   143,   144,    75,   146,   147,
     148,    79,    -1,   151,   152,   153,    -1,   155,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   114,   115,    28,    29,
     118,   119,   120,   121,    -1,   123,   124,   125,    -1,   127,
     128,   129,    42,   131,   132,   133,    -1,   135,   136,   137,
     138,   139,    -1,   141,   142,   143,   144,    -1,   146,   147,
     148,    -1,    -1,   151,   152,   153,    -1,   155,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    79,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,    -1,    -1,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    -1,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    75,   146,   147,   148,    79,
      10,   151,   152,   153,    -1,   155,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    -1,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    75,   146,   147,   148,    79,
      10,   151,   152,   153,    -1,   155,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,    -1,    -1,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    -1,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    75,   146,   147,   148,    79,
      10,   151,   152,   153,    -1,   155,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    28,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    75,   146,   147,   148,    79,
      -1,   151,   152,   153,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,   109,
      -1,    75,    -1,    -1,    -1,    79,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    88,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    -1,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,   109,   146,   147,   148,    -1,
      -1,   151,   152,   153,    -1,   155,    -1,    -1,    -1,    -1,
      -1,   125,    -1,   127,   128,   129,    75,   131,   132,   133,
      79,   135,   136,   137,   138,   139,    -1,   141,   142,   143,
     144,    -1,   146,   147,   148,    -1,    10,   151,   152,   153,
      -1,   155,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,   127,   128,
     129,    -1,   131,   132,   133,    -1,   135,   136,   137,   138,
     139,    -1,   141,   142,   143,   144,    60,   146,   147,   148,
      -1,    10,   151,   152,   153,    -1,   155,    -1,    17,    -1,
      -1,    75,    -1,    -1,    -1,    79,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,   127,   128,   129,    75,   131,   132,   133,
      79,   135,   136,   137,   138,   139,    -1,   141,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
      10,    -1,    -1,    -1,    -1,    -1,   105,    17,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    33,    -1,    -1,   125,    -1,   127,   128,
     129,    -1,   131,   132,   133,    -1,   135,   136,   137,   138,
     139,    -1,   141,   142,   143,   144,    -1,   146,   147,   148,
      60,    -1,   151,   152,   153,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    75,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,
      75,   131,   132,   133,    79,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    -1,   146,   147,   148,    10,
      -1,   151,   152,   153,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    42,   127,   128,   129,    -1,   131,   132,   133,    -1,
     135,   136,   137,   138,   139,    -1,   141,   142,   143,   144,
      -1,   146,   147,   148,    10,    -1,   151,   152,   153,    -1,
      -1,    17,    -1,    -1,    75,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,    75,
     131,   132,   133,    79,   135,   136,   137,   138,   139,    -1,
     141,   142,   143,   144,    -1,   146,   147,   148,    -1,    -1,
     151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    -1,   109,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,   127,   128,   129,    -1,   131,   132,   133,    -1,   135,
     136,   137,   138,   139,   109,   141,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,    -1,    -1,
     125,    -1,   127,   128,   129,    -1,   131,   132,   133,    -1,
     135,   136,   137,   138,   139,    -1,   141,   142,   143,   144,
      -1,   146,   147,   148,    10,   150,   151,   152,   153,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    10,    -1,    -1,    23,    24,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    32,    -1,    34,
      35,    -1,    -1,    28,    39,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    63,    -1,
      65,    -1,    67,    -1,    69,    -1,    -1,    72,    -1,    74,
      10,    76,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      75,    -1,    -1,   109,    79,    -1,    -1,    -1,    28,    -1,
      95,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,   125,
      -1,   127,   128,   129,    -1,   131,   132,   133,    -1,   135,
     136,   137,   138,   139,   109,   141,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,    -1,    -1,
     125,    -1,   127,   128,   129,    75,   131,   132,   133,    79,
     135,   136,   137,   138,   139,   150,   141,   142,   143,   144,
      -1,   146,   147,   148,    10,   150,   151,   152,   153,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    -1,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    -1,   146,   147,   148,    10,
      -1,   151,   152,   153,    -1,    -1,    17,    -1,    -1,    75,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,   109,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,   125,
      -1,   127,   128,   129,    75,   131,   132,   133,    79,   135,
     136,   137,   138,   139,    -1,   141,   142,   143,   144,    -1,
     146,   147,   148,    -1,    10,   151,   152,   153,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    28,    -1,    75,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,    -1,
     131,   132,   133,    -1,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   105,   146,   147,   148,   109,    -1,
     151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    79,   125,    -1,   127,   128,   129,    -1,
     131,   132,   133,    -1,   135,   136,   137,   138,   139,    -1,
     141,   142,   143,   144,    -1,   146,   147,   148,    10,    11,
     151,   152,   153,   109,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,   125,
      -1,   127,   128,   129,    -1,   131,   132,   133,    -1,   135,
     136,   137,   138,   139,    -1,   141,   142,   143,   144,    -1,
     146,   147,   148,    10,   150,   151,   152,   153,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    75,    31,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    79,   125,    -1,   127,   128,   129,    -1,   131,
     132,   133,    -1,   135,   136,   137,   138,   139,    -1,   141,
     142,   143,   144,    -1,   146,   147,   148,    -1,    -1,   151,
     152,   153,   109,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,   125,    -1,
     127,   128,   129,    -1,   131,   132,   133,    -1,   135,   136,
     137,   138,   139,    -1,   141,   142,   143,   144,    -1,   146,
     147,   148,   108,   109,   151,   152,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,   127,   128,   129,    -1,   131,   132,   133,    -1,   135,
     136,   137,   138,   139,    -1,   141,   142,   143,   144,     3,
     146,   147,   148,    10,    -1,   151,   152,   153,    -1,    -1,
      17,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    28,    29,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    10,    -1,    -1,    41,    -1,    43,
      -1,    17,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    73,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    82,    83,
      -1,    85,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,
      -1,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,    75,
      -1,    -1,   109,    79,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,   125,    -1,
     127,   128,   129,    -1,   131,   132,   133,    28,   135,   136,
     137,   138,   139,   109,   141,   142,   143,   144,    -1,   146,
     147,   148,    -1,    -1,   151,   152,   153,    -1,    -1,   125,
      -1,   127,   128,   129,    -1,   131,   132,   133,    -1,   135,
     136,   137,   138,   139,    -1,   141,   142,   143,   144,    -1,
     146,   147,   148,    -1,    75,   151,   152,   153,    79,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    11,    -1,    -1,    -1,
      -1,    -1,    17,    -1,   125,    -1,   127,   128,   129,    -1,
     131,   132,   133,    28,   135,   136,   137,   138,   139,    -1,
     141,   142,   143,   144,    75,   146,   147,   148,    79,    -1,
     151,   152,   153,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      75,    -1,    -1,    -1,    79,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,   125,    -1,   127,   128,   129,    -1,
     131,   132,   133,    28,   135,   136,   137,   138,   139,    -1,
     141,   142,   143,   144,   109,   146,   147,   148,    -1,    -1,
     151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,   127,   128,   129,    -1,   131,   132,   133,    -1,
     135,   136,   137,   138,   139,    -1,   141,   142,   143,   144,
      75,   146,   147,   148,    79,    10,   151,   152,   153,    -1,
      -1,    -1,    17,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
     125,    -1,   127,   128,   129,    -1,   131,   132,   133,    28,
     135,   136,   137,   138,   139,    -1,   141,   142,   143,   144,
      75,   146,   147,   148,    79,    -1,   151,   152,   153,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    75,    -1,    -1,    -1,
      79,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    88,
     125,    -1,   127,   128,   129,    -1,   131,   132,   133,    28,
     135,   136,   137,   138,   139,    -1,   141,   142,   143,   144,
     109,   146,   147,   148,    -1,    -1,   151,   152,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,   127,   128,
     129,    -1,   131,   132,   133,    -1,   135,   136,   137,   138,
     139,    -1,   141,   142,   143,   144,    75,   146,   147,   148,
      79,    10,   151,   152,   153,    -1,    -1,    -1,    17,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,   125,    -1,   127,   128,
     129,    -1,   131,   132,   133,    28,   135,   136,   137,   138,
     139,    -1,   141,   142,   143,   144,    75,   146,   147,   148,
      79,    -1,   151,   152,   153,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    75,    -1,    -1,    -1,    79,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    88,   125,    -1,   127,   128,
     129,    -1,   131,   132,   133,    28,   135,   136,   137,   138,
     139,    -1,   141,   142,   143,   144,   109,   146,   147,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,    -1,    51,    -1,
      -1,    -1,   125,    -1,   127,   128,   129,    -1,   131,   132,
     133,    -1,   135,   136,   137,   138,   139,    -1,   141,   142,
     143,   144,    75,   146,   147,   148,    79,    10,   151,   152,
     153,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,   125,    -1,   127,   128,   129,    -1,   131,   132,
     133,    28,   135,   136,   137,   138,   139,    -1,   141,   142,
     143,   144,    75,   146,   147,   148,    79,    -1,   151,   152,
     153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    75,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,   127,   128,   129,    -1,   131,   132,
     133,    -1,   135,   136,   137,   138,   139,    -1,   141,   142,
     143,   144,   109,   146,   147,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
     127,   128,   129,    -1,   131,   132,   133,    28,   135,   136,
     137,   138,   139,    -1,   141,   142,   143,   144,    19,   146,
     147,   148,    23,    24,   151,   152,   153,    -1,    -1,    -1,
      -1,    32,    -1,    34,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    63,    -1,    65,    -1,    67,    -1,    69,    -1,
      -1,    72,    -1,    74,    -1,    76,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,    -1,
     101,    -1,    -1,    -1,   125,    -1,   127,   128,   129,    28,
     131,   132,   133,    -1,   135,   136,   137,   138,    -1,    -1,
     141,   142,   143,   144,    75,   146,   147,   148,    79,    -1,
     151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,   150,
      -1,    -1,    -1,    -1,    -1,   156,    75,    -1,   109,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,    -1,
     131,   132,   133,    -1,   135,   136,   137,   138,    -1,    -1,
     141,   142,   143,   144,    75,   146,    -1,   148,    79,    -1,
     151,   152,   153,    -1,    -1,    -1,   125,    -1,   127,   128,
     129,    -1,   131,   132,   133,    -1,   135,   136,   137,   138,
      -1,    -1,   141,   142,   143,   144,    -1,   146,    -1,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,    -1,
     131,   132,   133,    -1,    -1,   136,   137,   138,    -1,    -1,
     141,   142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,
     151,   152,   153,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,
     157,    -1,   159
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   165,   166,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    15,    16,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    29,    30,    32,    34,    35,
      36,    37,    38,    39,    40,    41,    43,    44,    46,    48,
      49,    50,    52,    53,    54,    56,    57,    59,    63,    64,
      65,    66,    67,    68,    69,    72,    73,    74,    76,    77,
      78,    80,    81,    82,    83,    84,    85,    86,    87,    89,
      90,    92,    93,    94,    95,    96,    98,    99,   100,   101,
     104,   106,   109,   125,   126,   127,   128,   133,   143,   145,
     147,   148,   150,   153,   155,   157,   159,   167,   168,   169,
     170,   171,   172,   174,   179,   180,   181,   185,   187,   191,
     192,   193,   194,   195,   196,   198,   199,   200,   204,   205,
     208,   209,   212,   213,   216,   219,   220,   246,   249,   250,
     270,   271,   272,   273,   274,   275,   283,   284,   285,   286,
     289,   290,   291,   292,   293,   294,   295,   297,   298,   299,
     300,   301,   150,   169,   286,   105,   276,   277,     3,   182,
      12,    37,    38,    40,    87,   155,   159,   216,   270,   274,
     284,   285,   286,   289,   291,   292,   276,   286,    95,   249,
      76,   182,   169,   264,   286,   169,   157,   286,   287,     7,
       9,    95,   189,   287,    65,   107,   197,   286,   286,    23,
      35,   250,   286,   286,   157,     3,    15,    16,    22,    30,
      36,    41,    43,    46,    53,    64,    73,    82,    83,    85,
      87,    89,    92,    96,    99,   184,   186,   217,   157,   184,
     251,   252,    29,   174,   190,   286,   286,   286,   286,   286,
     286,     7,   157,    35,   193,   193,   149,   239,   263,   286,
      93,    94,   150,   286,   288,   286,   190,   286,   286,   286,
      86,   157,   169,   286,   286,   174,   181,   286,   289,   174,
     181,   286,   184,   247,   286,   263,   286,   286,   286,   286,
     286,   286,   286,   286,     1,   156,   167,   175,   263,    97,
     134,   239,   265,   266,   288,   263,   286,   296,    67,   169,
     155,    71,   173,   184,   184,    47,    70,   234,   251,   150,
     140,    10,    17,    28,    75,    79,   109,   125,   127,   128,
     129,   131,   132,   133,   135,   136,   137,   138,   139,   141,
     142,   143,   144,   146,   147,   148,   151,   152,   153,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   154,   226,   157,   159,    75,    79,
     157,   169,   150,   286,   286,   286,   263,   155,   174,    42,
     276,   247,   251,   150,   130,   150,   104,   185,   239,   267,
     268,   269,   288,   247,   209,   249,   184,   157,   190,    29,
      42,   190,    29,    42,   105,   190,   279,   287,    33,    60,
     150,    88,   174,   226,   267,   169,   157,   228,    97,   157,
     184,   253,   254,     1,   129,   258,    42,   130,   169,   190,
     190,   267,   184,   130,   150,   286,   286,   150,   155,   190,
     157,   267,   150,   201,   150,   201,   150,   110,   248,   150,
     150,    33,    60,   190,   150,   156,   156,   167,   130,   156,
     286,   130,   158,   130,   158,    42,   130,   160,   279,   108,
     130,   160,     7,     1,   156,   175,    55,   129,   210,   155,
     221,   150,   286,   286,   286,   286,   197,   286,   197,   286,
     286,   286,   286,   286,    16,    99,   286,   286,    30,    53,
      95,   185,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   288,   288,
     288,   288,   288,   267,   267,   197,   286,   197,   286,    24,
      42,    76,   101,   278,   281,   282,   286,   301,    29,    42,
      29,    42,    88,    42,   160,   197,   286,   190,   150,   150,
     286,   286,   110,   158,   130,   150,   184,   251,    42,   263,
      40,   286,   197,   286,    40,   286,   197,   286,   157,   190,
     153,   176,   178,   286,   176,   177,   169,   286,    31,   286,
     158,    24,    42,    45,    62,    65,    76,    95,   168,   229,
     230,   231,   232,   218,   254,   130,   158,    16,    30,    43,
      64,    82,    83,    87,    99,   159,   188,   259,   271,   110,
     255,   286,   252,   158,   239,   286,     1,   206,   267,   158,
      18,   202,   259,   271,   130,   178,   177,   156,   158,   158,
     265,   158,   265,   197,   286,   160,   169,   286,   160,   286,
     160,   286,   156,   156,   184,   263,   155,     1,   184,   214,
     215,    24,    65,    76,    95,   223,   233,   150,   150,   150,
     150,   150,   158,   160,    42,    76,   130,   158,   270,    75,
      75,    40,   197,   286,    40,   197,   286,   197,   286,   276,
     276,   150,   239,   288,   269,   210,   150,   286,   158,   286,
      29,   190,    29,   190,   286,    29,   190,   279,    29,   190,
     279,   280,   281,   130,   150,    11,   150,    31,    31,   169,
      88,   174,    42,    76,   231,   130,   158,   157,   184,    24,
      65,    76,    95,   235,   158,   254,   258,     1,   263,    58,
     288,   156,    61,   102,   156,   207,   158,   157,   174,   184,
     203,   247,   150,   150,   160,   279,   160,   279,   169,   108,
       1,   211,   156,   110,   130,   156,    76,   222,     1,     3,
      10,    15,    17,    22,    36,    41,    46,    53,    73,    85,
      89,    92,    96,   125,   126,   127,   128,   131,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     148,   151,   152,   153,   154,   157,   183,   186,   224,   225,
     226,   270,   281,   258,   270,   270,   286,    29,    29,   286,
      29,    29,   160,   160,   190,   190,   155,   190,    88,    40,
     286,    40,   286,    88,    40,   286,   190,    40,   286,   190,
     130,   158,   286,   286,   169,   286,   169,    31,   184,   230,
     254,   129,   262,    76,   258,   255,   160,    42,   160,    29,
     169,   263,   203,   129,   174,    40,   169,   286,   160,    40,
     169,   286,   160,   286,   156,    35,    69,    72,   156,   168,
     192,   286,   215,   235,   157,   227,   184,   286,   132,   227,
     227,   255,    88,    40,    40,    88,    40,    40,    40,    40,
       1,   211,   286,   286,   286,   286,   286,   286,   281,    11,
      31,   169,   262,   158,   159,   188,   239,   261,   271,   134,
     241,   255,    51,   103,   242,   286,   259,   271,   169,   190,
     158,   286,   286,   169,   286,   169,   192,     1,   129,   257,
     229,   158,     3,    89,   225,   226,   286,   286,   286,   286,
     286,   286,   156,   156,    88,    88,    88,    88,   286,   169,
     255,   262,   160,   263,   239,   240,   286,     3,    78,    89,
     243,   244,   245,   286,   174,   191,   238,   160,   174,    88,
      88,   159,   188,   256,   271,    91,   236,   158,   227,   227,
      88,    88,    88,    88,    88,    88,   286,   286,   286,   286,
     241,   255,   239,   260,   261,   271,    42,   160,   245,   103,
     130,   110,   135,   137,   138,   141,   142,    51,   271,   286,
     286,     1,   160,   263,   242,   286,   260,   261,   286,   244,
     245,   245,   245,   245,   245,   245,   243,   160,   256,   271,
     160,   150,   237,   238,   160,   256,   271,   260
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
#line 487 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 492 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 493 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 500 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 525 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 526 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 527 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 528 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 529 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 530 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 532 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 533 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 534 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 535 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 540 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 545 "chapel.ypp"
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
#line 558 "chapel.ypp"
    { (yyvsp[(1) - (3)].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (3)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (3)]).prevModule;  // restore previous module name
    }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 563 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = (yyvsp[(3) - (4)].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 568 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 575 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 595 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 596 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 597 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 602 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 603 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 613 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 618 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 622 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 629 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 638 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 647 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 648 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 649 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 653 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 658 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 660 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 662 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 664 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 666 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 672 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 673 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 677 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 678 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 679 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 680 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 681 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 684 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 685 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 686 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 687 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 712 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 713 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 714 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 715 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 716 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 717 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 718 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 719 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 720 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 721 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 722 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 723 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 724 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 725 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 726 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 727 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 731 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 732 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 733 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 734 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 737 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 738 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 745 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pch) = "_owned"; }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 748 "chapel.ypp"
    { (yyval.pch) = "_shared"; }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 749 "chapel.ypp"
    { (yyval.pch) = "_borrowed"; }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pch) = "_unmanaged"; }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 758 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 762 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 763 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 767 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 768 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 772 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 774 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 775 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 788 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 789 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 790 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 791 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 796 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 802 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 803 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 804 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 805 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 806 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 807 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 808 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 809 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 810 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 811 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 812 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 813 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 814 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 815 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 816 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 817 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 819 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 825 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 831 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 837 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 843 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 850 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 859 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 863 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 864 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 865 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 866 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 867 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 869 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 871 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 873 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 878 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 881 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 882 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 883 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 884 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 885 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 886 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 890 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 891 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 895 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 896 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 897 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 901 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 902 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 906 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 910 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 912 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 916 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 917 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 922 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 924 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 926 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 933 "chapel.ypp"
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

  case 191:
/* Line 1792 of yacc.c  */
#line 943 "chapel.ypp"
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

  case 192:
/* Line 1792 of yacc.c  */
#line 953 "chapel.ypp"
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

  case 193:
/* Line 1792 of yacc.c  */
#line 963 "chapel.ypp"
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

  case 194:
/* Line 1792 of yacc.c  */
#line 975 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 980 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 985 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 993 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 994 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 999 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1001 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1003 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1008 "chapel.ypp"
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

  case 203:
/* Line 1792 of yacc.c  */
#line 1024 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1031 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1040 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1048 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1052 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1058 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1059 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1064 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1069 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1074 "chapel.ypp"
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

  case 213:
/* Line 1792 of yacc.c  */
#line 1096 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1102 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1109 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[(2) - (2)].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1115 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[(2) - (2)].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1121 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1132 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1138 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1145 "chapel.ypp"
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

  case 221:
/* Line 1792 of yacc.c  */
#line 1171 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1175 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1180 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1184 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1189 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1196 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1200 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1201 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1203 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1204 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1205 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1206 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1207 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1208 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1209 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1210 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1213 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1214 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1215 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1216 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1217 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1218 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1219 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1220 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1221 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1225 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1230 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1231 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1232 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1234 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1235 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1236 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1237 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1238 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1239 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1240 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1244 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1245 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1249 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1253 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1254 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1255 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1260 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1262 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1264 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1266 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1268 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1272 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1273 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1277 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1283 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1284 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1298 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1307 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1308 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1312 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1313 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1322 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1327 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1329 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1333 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1338 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1343 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1345 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1347 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1349 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1351 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1356 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1363 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1364 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1365 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1375 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1377 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1382 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1395 "chapel.ypp"
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

  case 335:
/* Line 1792 of yacc.c  */
#line 1411 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1413 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1415 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1419 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1420 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1421 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1422 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1423 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1428 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1434 "chapel.ypp"
    {
      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), context->latestComment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
      context->latestComment = NULL;
    }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1440 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1449 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1457 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1459 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1464 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1466 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1473 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1493 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1499 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1509 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1517 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1518 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1526 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1527 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1550 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1554 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1558 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1566 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1572 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1573 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1574 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1579 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1587 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1591 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1597 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1598 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1605 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1606 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1607 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1612 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1613 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1617 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1618 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1619 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1623 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1624 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1633 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1634 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1638 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1639 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1640 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1641 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1645 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1646 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1663 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1665 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1667 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1669 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1671 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1673 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1677 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1679 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1681 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1683 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1688 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1690 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1692 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1694 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1696 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1698 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1704 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1708 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1710 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1712 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1719 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1725 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1731 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1737 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1746 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1755 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1771 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1775 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1780 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1784 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1785 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1789 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1793 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1794 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1799 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1803 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1807 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1813 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1814 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1815 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1816 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1817 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1818 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1824 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1829 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1834 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1850 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1852 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1854 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1856 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1858 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1860 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1862 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1864 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1868 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1869 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1872 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1873 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1874 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1896 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1897 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1898 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 1902 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1903 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1904 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1905 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1913 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1914 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1915 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1916 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1920 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1921 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1926 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1927 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 1928 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 1929 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 523:
/* Line 1792 of yacc.c  */
#line 1930 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 1931 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 1932 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 1933 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 1934 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 1936 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 1940 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 1947 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 1948 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 1952 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 1953 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 1954 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 1955 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 1956 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 1957 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 1958 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 1959 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 1960 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 1961 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 1962 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 1963 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 1964 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 1965 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 1966 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 1967 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 1968 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 1969 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 1970 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 1971 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 1972 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 1973 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 1974 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 1978 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 1979 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 1980 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 1981 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 1982 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 1983 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 1987 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 1988 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 1989 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 1990 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 1994 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 1995 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 1996 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 1997 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2002 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2003 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2004 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2005 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2006 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2007 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2008 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 9322 "bison-chapel.cpp"
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


