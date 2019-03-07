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
     TON = 315,
     TONLY = 316,
     TOTHERWISE = 317,
     TOUT = 318,
     TOVERRIDE = 319,
     TOWNED = 320,
     TPARAM = 321,
     TPLUSPLUS = 322,
     TPRAGMA = 323,
     TPRIMITIVE = 324,
     TPRIVATE = 325,
     TPROC = 326,
     TPROTOTYPE = 327,
     TPUBLIC = 328,
     TREAL = 329,
     TRECORD = 330,
     TREDUCE = 331,
     TREF = 332,
     TREQUIRE = 333,
     TRETURN = 334,
     TSCAN = 335,
     TSELECT = 336,
     TSERIAL = 337,
     TSHARED = 338,
     TSINGLE = 339,
     TSPARSE = 340,
     TSTRING = 341,
     TSUBDOMAIN = 342,
     TSYNC = 343,
     TTHEN = 344,
     TTHIS = 345,
     TTHROW = 346,
     TTHROWS = 347,
     TTRUE = 348,
     TTRY = 349,
     TTRYBANG = 350,
     TTYPE = 351,
     TUINT = 352,
     TUNDERSCORE = 353,
     TUNION = 354,
     TUNMANAGED = 355,
     TUSE = 356,
     TVAR = 357,
     TWHEN = 358,
     TWHERE = 359,
     TWHILE = 360,
     TWITH = 361,
     TYIELD = 362,
     TZIP = 363,
     TALIAS = 364,
     TAND = 365,
     TASSIGN = 366,
     TASSIGNBAND = 367,
     TASSIGNBOR = 368,
     TASSIGNBXOR = 369,
     TASSIGNDIVIDE = 370,
     TASSIGNEXP = 371,
     TASSIGNLAND = 372,
     TASSIGNLOR = 373,
     TASSIGNMINUS = 374,
     TASSIGNMOD = 375,
     TASSIGNMULTIPLY = 376,
     TASSIGNPLUS = 377,
     TASSIGNREDUCE = 378,
     TASSIGNSL = 379,
     TASSIGNSR = 380,
     TBAND = 381,
     TBNOT = 382,
     TBOR = 383,
     TBXOR = 384,
     TCOLON = 385,
     TCOMMA = 386,
     TDIVIDE = 387,
     TDOT = 388,
     TDOTDOT = 389,
     TDOTDOTDOT = 390,
     TEQUAL = 391,
     TEXP = 392,
     TGREATER = 393,
     TGREATEREQUAL = 394,
     THASH = 395,
     TIO = 396,
     TLESS = 397,
     TLESSEQUAL = 398,
     TMINUS = 399,
     TMOD = 400,
     TNOT = 401,
     TNOTEQUAL = 402,
     TOR = 403,
     TPLUS = 404,
     TQUESTION = 405,
     TSEMI = 406,
     TSHIFTLEFT = 407,
     TSHIFTRIGHT = 408,
     TSTAR = 409,
     TSWAP = 410,
     TLCBR = 411,
     TRCBR = 412,
     TLP = 413,
     TRP = 414,
     TLSBR = 415,
     TRSBR = 416,
     TNOELSE = 417,
     TUMINUS = 418,
     TUPLUS = 419
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
#line 479 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 486 "bison-chapel.cpp"
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
#line 565 "bison-chapel.cpp"

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
#define YYLAST   16554

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  165
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  138
/* YYNRULES -- Number of rules.  */
#define YYNRULES  580
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1039

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   419

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
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164
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
     857,   859,   861,   863,   865,   867,   869,   871,   873,   875,
     877,   879,   880,   884,   888,   889,   891,   895,   900,   906,
     911,   918,   925,   926,   928,   930,   932,   934,   936,   939,
     942,   944,   946,   948,   949,   951,   953,   956,   958,   960,
     962,   964,   965,   967,   970,   972,   974,   976,   977,   979,
     981,   983,   985,   987,   990,   992,   993,   995,   998,  1001,
    1002,  1005,  1008,  1013,  1018,  1020,  1024,  1028,  1032,  1036,
    1040,  1044,  1048,  1051,  1053,  1055,  1059,  1064,  1069,  1072,
    1077,  1078,  1081,  1084,  1086,  1088,  1090,  1093,  1095,  1100,
    1106,  1110,  1112,  1116,  1120,  1126,  1128,  1130,  1134,  1136,
    1139,  1143,  1144,  1147,  1150,  1154,  1157,  1162,  1166,  1170,
    1175,  1179,  1180,  1183,  1186,  1189,  1191,  1192,  1195,  1198,
    1201,  1203,  1208,  1213,  1220,  1224,  1225,  1227,  1229,  1233,
    1238,  1242,  1247,  1254,  1255,  1258,  1261,  1264,  1267,  1269,
    1271,  1275,  1279,  1281,  1285,  1287,  1289,  1291,  1295,  1299,
    1300,  1302,  1304,  1308,  1312,  1316,  1318,  1320,  1322,  1324,
    1326,  1328,  1330,  1332,  1334,  1337,  1342,  1347,  1352,  1358,
    1361,  1364,  1367,  1370,  1373,  1376,  1383,  1390,  1395,  1405,
    1415,  1423,  1430,  1437,  1442,  1452,  1462,  1470,  1475,  1482,
    1489,  1499,  1509,  1516,  1518,  1520,  1522,  1524,  1526,  1528,
    1530,  1532,  1536,  1537,  1539,  1544,  1546,  1550,  1555,  1557,
    1561,  1566,  1570,  1574,  1576,  1578,  1581,  1583,  1586,  1588,
    1590,  1594,  1597,  1602,  1604,  1606,  1608,  1610,  1612,  1614,
    1616,  1618,  1623,  1627,  1631,  1635,  1639,  1642,  1645,  1647,
    1648,  1650,  1653,  1656,  1658,  1660,  1662,  1664,  1666,  1668,
    1670,  1675,  1680,  1685,  1689,  1693,  1697,  1701,  1705,  1710,
    1714,  1719,  1721,  1723,  1725,  1727,  1729,  1731,  1733,  1735,
    1739,  1744,  1748,  1753,  1757,  1762,  1766,  1772,  1776,  1780,
    1784,  1788,  1792,  1796,  1800,  1804,  1808,  1812,  1816,  1820,
    1824,  1828,  1832,  1836,  1840,  1844,  1848,  1852,  1856,  1860,
    1864,  1867,  1870,  1873,  1876,  1879,  1882,  1886,  1890,  1894,
    1898,  1902,  1906,  1910,  1914,  1916,  1918,  1920,  1922,  1924,
    1926
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     166,     0,    -1,   167,    -1,    -1,   167,   168,    -1,   170,
      -1,   169,   170,    -1,    68,     7,    -1,   169,    68,     7,
      -1,   172,    -1,   175,    -1,   180,    -1,   181,    -1,   193,
      -1,   182,    -1,   196,    -1,   199,    -1,   197,    -1,   206,
      -1,   200,    -1,   201,    -1,   205,    -1,   192,    -1,   276,
     151,    -1,    12,   170,    -1,    13,   277,   170,    -1,    14,
     183,   151,    -1,    20,   277,   175,    -1,    25,   183,   151,
      -1,    27,   265,   151,    -1,    49,   185,   170,    -1,    53,
     287,   191,    -1,    53,   191,    -1,    60,   287,   191,    -1,
      82,   287,   191,    -1,    82,   191,    -1,    88,   170,    -1,
     107,   287,   151,    -1,     1,   151,    -1,   173,   174,    56,
     185,    -1,   171,   156,   157,    -1,   171,   156,   176,   157,
      -1,   171,   156,     1,   157,    -1,    -1,    73,    -1,    70,
      -1,    -1,    72,    -1,   156,   157,    -1,   156,   176,   157,
      -1,   156,     1,   157,    -1,   168,    -1,   176,   168,    -1,
     287,    -1,   287,    11,   287,    -1,   177,   131,   287,    -1,
     177,   131,   287,    11,   287,    -1,    -1,   177,    -1,   154,
      -1,   177,    -1,   101,   264,   151,    -1,   101,   287,    33,
     179,   151,    -1,   101,   287,    61,   178,   151,    -1,    78,
     264,   151,    -1,   290,   227,   289,   151,    -1,   290,   155,
     289,   151,    -1,   290,   123,   289,   151,    -1,   290,   117,
     289,   151,    -1,   290,   118,   289,   151,    -1,    -1,     3,
      -1,     3,    -1,    90,    -1,    36,    -1,    93,    -1,   187,
      -1,     3,    -1,    90,    -1,    36,    -1,    93,    -1,   187,
      -1,     3,    -1,    90,    -1,    15,    -1,    46,    -1,    97,
      -1,    74,    -1,    41,    -1,    22,    -1,    86,    -1,    88,
      -1,    84,    -1,    65,    -1,    83,    -1,    16,    -1,   100,
      -1,    30,    -1,    43,    -1,    54,    -1,    15,    -1,    46,
      -1,    97,    -1,    74,    -1,    41,    -1,    22,    -1,    86,
      -1,    54,    -1,    88,    -1,    84,    -1,    65,    -1,    83,
      -1,    16,    -1,   100,    -1,    30,    -1,    43,    -1,    -1,
       7,    -1,    29,   170,    -1,   175,    -1,    79,   151,    -1,
      79,   289,   151,    -1,   151,    -1,   194,    -1,    73,   194,
      -1,    70,   194,    -1,   213,    -1,   247,    -1,   209,    -1,
     195,    -1,   221,    -1,   251,    -1,    39,   287,   151,    -1,
      39,   287,    33,   179,   151,    -1,    39,   287,    61,   178,
     151,    -1,    39,   251,    -1,    35,     9,    -1,    29,   170,
     105,   287,   151,    -1,   105,   287,   191,    -1,    21,   287,
      42,   287,   277,   191,    -1,    21,   287,    42,   198,   277,
     191,    -1,    21,   287,   277,   191,    -1,    37,   287,    42,
     287,   191,    -1,    37,   287,    42,   198,   191,    -1,    37,
     287,   191,    -1,    37,   198,   191,    -1,    37,    66,   185,
      42,   287,   191,    -1,    38,   287,    42,   287,   191,    -1,
      38,   287,    42,   287,   280,   191,    -1,    38,   287,    42,
     198,   191,    -1,    38,   287,    42,   198,   280,   191,    -1,
      38,   287,   191,    -1,    38,   287,   280,   191,    -1,    38,
     198,   191,    -1,    38,   198,   280,   191,    -1,   160,   264,
      42,   287,   161,   170,    -1,   160,   264,    42,   287,   280,
     161,   170,    -1,   160,   264,    42,   198,   161,   170,    -1,
     160,   264,    42,   198,   280,   161,   170,    -1,   160,   264,
     161,   170,    -1,   160,   264,   280,   161,   170,    -1,   160,
     198,   161,   170,    -1,   160,   198,   280,   161,   170,    -1,
     108,   158,   264,   159,    -1,    40,   287,    89,   170,    -1,
      40,   287,   175,    -1,    40,   287,    89,   170,    31,   170,
      -1,    40,   287,   175,    31,   170,    -1,    40,   287,   227,
     287,    89,   170,    -1,    40,   287,   227,   287,   175,    -1,
      40,   287,   227,   287,    89,   170,    31,   170,    -1,    40,
     287,   227,   287,   175,    31,   170,    -1,    26,   170,    -1,
      94,   287,   151,    -1,    95,   287,   151,    -1,    94,   182,
      -1,    95,   182,    -1,    94,   175,   202,    -1,    95,   175,
     202,    -1,    -1,   202,   203,    -1,    18,   175,    -1,    18,
     204,   175,    -1,    18,   158,   204,   159,   175,    -1,   185,
      -1,   185,   130,   287,    -1,    91,   287,   151,    -1,    81,
     287,   156,   207,   157,    -1,    81,   287,   156,     1,   157,
      -1,    -1,   207,   208,    -1,   103,   264,   191,    -1,    62,
     170,    -1,    62,    29,   170,    -1,   210,   185,   211,   156,
     212,   157,    -1,    35,   190,   210,   185,   211,   156,   212,
     157,    -1,   210,   185,   211,   156,     1,   157,    -1,    35,
     190,   210,   185,   211,   156,     1,   157,    -1,    19,    -1,
      75,    -1,    99,    -1,    -1,   130,   264,    -1,    -1,   212,
     193,    -1,   212,   169,   193,    -1,   214,   185,   156,   215,
     157,    -1,   214,   185,   156,     1,   157,    -1,    32,    -1,
     216,    -1,   215,   131,    -1,   215,   131,   216,    -1,   185,
      -1,   185,   111,   287,    -1,    -1,    -1,    50,   218,   229,
     219,   236,   259,   243,   239,    -1,    -1,    44,    -1,    34,
     288,    -1,    35,   288,    -1,    64,    -1,    -1,    -1,   220,
     235,   222,   224,   223,   236,   258,   237,   243,   238,    -1,
     234,   226,   228,    -1,   234,   227,   228,    -1,   234,   225,
     133,   226,   228,    -1,   234,   225,   133,   227,   228,    -1,
     234,     1,   228,    -1,   271,    -1,   158,   287,   159,    -1,
     184,    -1,   127,   185,    -1,   126,    -1,   128,    -1,   129,
      -1,   127,    -1,   136,    -1,   147,    -1,   143,    -1,   139,
      -1,   142,    -1,   138,    -1,   149,    -1,   144,    -1,   154,
      -1,   132,    -1,   152,    -1,   153,    -1,   145,    -1,   137,
      -1,   146,    -1,    17,    -1,   140,    -1,    10,    -1,   155,
      -1,   141,    -1,    48,    -1,   111,    -1,   122,    -1,   119,
      -1,   121,    -1,   115,    -1,   120,    -1,   116,    -1,   112,
      -1,   113,    -1,   114,    -1,   125,    -1,   124,    -1,    -1,
     158,   230,   159,    -1,   158,   230,   159,    -1,    -1,   231,
      -1,   230,   131,   231,    -1,   232,   185,   263,   256,    -1,
     169,   232,   185,   263,   256,    -1,   232,   185,   263,   242,
      -1,   232,   158,   255,   159,   263,   256,    -1,   232,   158,
     255,   159,   263,   242,    -1,    -1,   233,    -1,    42,    -1,
      45,    -1,    63,    -1,    24,    -1,    24,    42,    -1,    24,
      77,    -1,    66,    -1,    77,    -1,    96,    -1,    -1,    66,
      -1,    77,    -1,    24,    77,    -1,    24,    -1,    96,    -1,
      71,    -1,    47,    -1,    -1,    24,    -1,    24,    77,    -1,
      77,    -1,    66,    -1,    96,    -1,    -1,    92,    -1,   151,
      -1,   239,    -1,   175,    -1,   192,    -1,   150,   185,    -1,
     150,    -1,    -1,   240,    -1,   135,   287,    -1,   135,   241,
      -1,    -1,   104,   287,    -1,    52,   244,    -1,   104,   287,
      52,   244,    -1,    52,   244,   104,   287,    -1,   245,    -1,
     244,   131,   245,    -1,   246,   111,   246,    -1,   246,   142,
     246,    -1,   246,   143,   246,    -1,   246,   136,   246,    -1,
     246,   138,   246,    -1,   246,   139,   246,    -1,    79,   246,
      -1,     3,    -1,    90,    -1,    96,   248,   151,    -1,    23,
      96,   248,   151,    -1,    35,    96,   248,   151,    -1,   185,
     249,    -1,   185,   249,   131,   248,    -1,    -1,   111,   272,
      -1,   111,   260,    -1,    66,    -1,    24,    -1,    77,    -1,
      24,    77,    -1,   102,    -1,    23,   250,   252,   151,    -1,
      35,   288,   250,   252,   151,    -1,   250,   252,   151,    -1,
     253,    -1,   252,   131,   253,    -1,   185,   259,   256,    -1,
     158,   255,   159,   259,   256,    -1,    98,    -1,   185,    -1,
     158,   255,   159,    -1,   254,    -1,   254,   131,    -1,   254,
     131,   255,    -1,    -1,   111,    59,    -1,   111,   289,    -1,
     160,   161,   272,    -1,   160,   161,    -1,   160,   264,   161,
     272,    -1,   160,   264,   161,    -1,   160,   161,   257,    -1,
     160,   264,   161,   257,    -1,   160,     1,   161,    -1,    -1,
     130,   272,    -1,   130,   257,    -1,   130,   189,    -1,     1,
      -1,    -1,   130,   272,    -1,   130,   260,    -1,   130,   189,
      -1,     1,    -1,   160,   264,   161,   272,    -1,   160,   264,
     161,   260,    -1,   160,   264,    42,   287,   161,   272,    -1,
     160,     1,   161,    -1,    -1,   272,    -1,   240,    -1,   160,
     161,   261,    -1,   160,   264,   161,   261,    -1,   160,   161,
     262,    -1,   160,   264,   161,   262,    -1,   160,   264,    42,
     287,   161,   261,    -1,    -1,   130,   272,    -1,   130,   240,
      -1,   130,   189,    -1,   130,   262,    -1,   287,    -1,   240,
      -1,   264,   131,   287,    -1,   264,   131,   240,    -1,   287,
      -1,   265,   131,   287,    -1,    98,    -1,   289,    -1,   240,
      -1,   266,   131,   266,    -1,   267,   131,   266,    -1,    -1,
     269,    -1,   270,    -1,   269,   131,   270,    -1,   186,   111,
     240,    -1,   186,   111,   289,    -1,   240,    -1,   289,    -1,
     186,    -1,   188,    -1,   275,    -1,   290,    -1,   274,    -1,
     299,    -1,   298,    -1,    84,   287,    -1,    43,   158,   268,
     159,    -1,    30,   158,   268,   159,    -1,    87,   158,   268,
     159,    -1,    85,    87,   158,   268,   159,    -1,    12,   287,
      -1,    88,   287,    -1,    16,   287,    -1,    65,   287,    -1,
     100,   287,    -1,    83,   287,    -1,    37,   287,    42,   287,
      29,   287,    -1,    37,   287,    42,   198,    29,   287,    -1,
      37,   287,    29,   287,    -1,    37,   287,    42,   287,    29,
      40,   287,    89,   287,    -1,    37,   287,    42,   198,    29,
      40,   287,    89,   287,    -1,    37,   287,    29,    40,   287,
      89,   287,    -1,    38,   287,    42,   287,    29,   287,    -1,
      38,   287,    42,   198,    29,   287,    -1,    38,   287,    29,
     287,    -1,    38,   287,    42,   287,    29,    40,   287,    89,
     287,    -1,    38,   287,    42,   198,    29,    40,   287,    89,
     287,    -1,    38,   287,    29,    40,   287,    89,   287,    -1,
     160,   264,   161,   287,    -1,   160,   264,    42,   287,   161,
     287,    -1,   160,   264,    42,   198,   161,   287,    -1,   160,
     264,    42,   287,   161,    40,   287,    89,   287,    -1,   160,
     264,    42,   198,   161,    40,   287,    89,   287,    -1,    40,
     287,    89,   287,    31,   287,    -1,    58,    -1,   275,    -1,
     271,    -1,   293,    -1,   292,    -1,   217,    -1,   285,    -1,
     286,    -1,   284,   141,   287,    -1,    -1,   278,    -1,   106,
     158,   279,   159,    -1,   282,    -1,   279,   131,   282,    -1,
     106,   158,   281,   159,    -1,   282,    -1,   281,   131,   282,
      -1,   283,   271,   259,   256,    -1,   302,    76,   271,    -1,
     287,    76,   271,    -1,    24,    -1,    42,    -1,    24,    42,
      -1,    77,    -1,    24,    77,    -1,   102,    -1,   290,    -1,
     284,   141,   287,    -1,    57,   287,    -1,    51,   252,    42,
     287,    -1,   296,    -1,   272,    -1,   273,    -1,   300,    -1,
     301,    -1,   217,    -1,   285,    -1,   286,    -1,   158,   135,
     287,   159,    -1,   287,   130,   287,    -1,   287,   130,    16,
      -1,   287,   130,   100,    -1,   287,   134,   287,    -1,   287,
     134,    -1,   134,   287,    -1,   134,    -1,    -1,   287,    -1,
      94,   287,    -1,    95,   287,    -1,   287,    -1,   271,    -1,
     292,    -1,   293,    -1,   294,    -1,   290,    -1,   217,    -1,
     291,   158,   268,   159,    -1,   291,   160,   268,   161,    -1,
      69,   158,   268,   159,    -1,   287,   133,   186,    -1,   287,
     133,    96,    -1,   287,   133,    30,    -1,   287,   133,    54,
      -1,   158,   266,   159,    -1,   158,   266,   131,   159,    -1,
     158,   267,   159,    -1,   158,   267,   131,   159,    -1,    36,
      -1,    93,    -1,   295,    -1,     4,    -1,     5,    -1,     6,
      -1,     7,    -1,     8,    -1,   156,   264,   157,    -1,   156,
     264,   131,   157,    -1,   160,   264,   161,    -1,   160,   264,
     131,   161,    -1,   160,   297,   161,    -1,   160,   297,   131,
     161,    -1,   287,   109,   287,    -1,   297,   131,   287,   109,
     287,    -1,   287,   149,   287,    -1,   287,   144,   287,    -1,
     287,   154,   287,    -1,   287,   132,   287,    -1,   287,   152,
     287,    -1,   287,   153,   287,    -1,   287,   145,   287,    -1,
     287,   136,   287,    -1,   287,   147,   287,    -1,   287,   143,
     287,    -1,   287,   139,   287,    -1,   287,   142,   287,    -1,
     287,   138,   287,    -1,   287,   126,   287,    -1,   287,   128,
     287,    -1,   287,   129,   287,    -1,   287,   110,   287,    -1,
     287,   148,   287,    -1,   287,   137,   287,    -1,   287,    17,
     287,    -1,   287,    10,   287,    -1,   287,   140,   287,    -1,
     287,    28,   287,    -1,   149,   287,    -1,   144,   287,    -1,
      55,   287,    -1,    67,   287,    -1,   146,   287,    -1,   127,
     287,    -1,   287,    76,   287,    -1,   287,    76,   198,    -1,
     302,    76,   287,    -1,   302,    76,   198,    -1,   287,    80,
     287,    -1,   287,    80,   198,    -1,   302,    80,   287,    -1,
     302,    80,   198,    -1,   149,    -1,   154,    -1,   110,    -1,
     148,    -1,   126,    -1,   128,    -1,   129,    -1
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
     815,   816,   817,   818,   819,   820,   826,   832,   838,   844,
     851,   858,   862,   869,   873,   874,   875,   876,   877,   879,
     881,   883,   888,   891,   892,   893,   894,   895,   896,   900,
     901,   905,   906,   907,   911,   912,   916,   919,   921,   926,
     927,   931,   933,   935,   942,   952,   962,   972,   985,   990,
     995,  1003,  1004,  1009,  1010,  1012,  1017,  1033,  1040,  1049,
    1057,  1061,  1068,  1069,  1074,  1079,  1073,  1106,  1112,  1119,
    1125,  1131,  1142,  1148,  1141,  1180,  1184,  1189,  1193,  1198,
    1205,  1206,  1210,  1211,  1212,  1213,  1214,  1215,  1216,  1217,
    1218,  1219,  1220,  1221,  1222,  1223,  1224,  1225,  1226,  1227,
    1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,  1240,
    1241,  1242,  1243,  1244,  1245,  1246,  1247,  1248,  1249,  1250,
    1251,  1255,  1256,  1260,  1264,  1265,  1266,  1270,  1272,  1274,
    1276,  1278,  1283,  1284,  1288,  1289,  1290,  1291,  1292,  1293,
    1294,  1295,  1296,  1300,  1301,  1302,  1303,  1304,  1305,  1309,
    1310,  1314,  1315,  1316,  1317,  1318,  1319,  1323,  1324,  1327,
    1328,  1332,  1333,  1337,  1339,  1344,  1345,  1349,  1350,  1354,
    1355,  1357,  1359,  1361,  1366,  1367,  1370,  1371,  1372,  1373,
    1374,  1375,  1376,  1379,  1380,  1383,  1385,  1387,  1392,  1405,
    1422,  1423,  1425,  1430,  1431,  1432,  1433,  1434,  1438,  1444,
    1450,  1458,  1459,  1467,  1469,  1474,  1476,  1478,  1483,  1485,
    1487,  1494,  1495,  1496,  1501,  1503,  1505,  1509,  1513,  1515,
    1519,  1527,  1528,  1529,  1530,  1531,  1536,  1537,  1538,  1539,
    1540,  1560,  1564,  1568,  1576,  1583,  1584,  1585,  1589,  1591,
    1597,  1599,  1601,  1606,  1607,  1608,  1609,  1610,  1616,  1617,
    1618,  1619,  1623,  1624,  1628,  1629,  1630,  1634,  1635,  1639,
    1640,  1644,  1645,  1649,  1650,  1651,  1652,  1656,  1657,  1668,
    1669,  1670,  1671,  1672,  1673,  1675,  1677,  1679,  1681,  1683,
    1685,  1687,  1689,  1691,  1693,  1698,  1700,  1702,  1704,  1706,
    1708,  1710,  1712,  1714,  1716,  1718,  1720,  1722,  1729,  1735,
    1741,  1747,  1756,  1766,  1774,  1775,  1776,  1777,  1778,  1779,
    1780,  1781,  1786,  1787,  1791,  1795,  1796,  1800,  1804,  1805,
    1809,  1813,  1817,  1824,  1825,  1826,  1827,  1828,  1829,  1833,
    1834,  1839,  1844,  1852,  1853,  1854,  1855,  1856,  1857,  1858,
    1859,  1860,  1862,  1864,  1866,  1868,  1870,  1872,  1874,  1879,
    1880,  1883,  1884,  1885,  1888,  1889,  1890,  1891,  1902,  1903,
    1907,  1908,  1909,  1913,  1914,  1915,  1916,  1924,  1925,  1926,
    1927,  1931,  1932,  1936,  1937,  1938,  1939,  1940,  1941,  1942,
    1943,  1944,  1945,  1946,  1950,  1958,  1959,  1963,  1964,  1965,
    1966,  1967,  1968,  1969,  1970,  1971,  1972,  1973,  1974,  1975,
    1976,  1977,  1978,  1979,  1980,  1981,  1982,  1983,  1984,  1985,
    1989,  1990,  1991,  1992,  1993,  1994,  1998,  1999,  2000,  2001,
    2005,  2006,  2007,  2008,  2013,  2014,  2015,  2016,  2017,  2018,
    2019
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
  "TNIL", "TNOINIT", "TON", "TONLY", "TOTHERWISE", "TOUT", "TOVERRIDE",
  "TOWNED", "TPARAM", "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE",
  "TPROC", "TPROTOTYPE", "TPUBLIC", "TREAL", "TRECORD", "TREDUCE", "TREF",
  "TREQUIRE", "TRETURN", "TSCAN", "TSELECT", "TSERIAL", "TSHARED",
  "TSINGLE", "TSPARSE", "TSTRING", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS",
  "TTHROW", "TTHROWS", "TTRUE", "TTRY", "TTRYBANG", "TTYPE", "TUINT",
  "TUNDERSCORE", "TUNION", "TUNMANAGED", "TUSE", "TVAR", "TWHEN", "TWHERE",
  "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN",
  "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
  "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
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
     415,   416,   417,   418,   419
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   165,   166,   167,   167,   168,   168,   169,   169,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   171,
     172,   172,   172,   173,   173,   173,   174,   174,   175,   175,
     175,   176,   176,   177,   177,   177,   177,   178,   178,   179,
     179,   180,   180,   180,   181,   182,   182,   182,   182,   182,
     183,   183,   184,   184,   184,   184,   184,   185,   185,   185,
     185,   185,   186,   186,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     188,   188,   188,   188,   188,   188,   188,   188,   189,   189,
     189,   189,   189,   189,   189,   189,   190,   190,   191,   191,
     192,   192,   193,   193,   193,   193,   193,   193,   193,   193,
     194,   194,   195,   195,   195,   195,   196,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   198,   199,   199,   199,   199,   199,   199,
     199,   199,   200,   201,   201,   201,   201,   201,   201,   202,
     202,   203,   203,   203,   204,   204,   205,   206,   206,   207,
     207,   208,   208,   208,   209,   209,   209,   209,   210,   210,
     210,   211,   211,   212,   212,   212,   213,   213,   214,   215,
     215,   215,   216,   216,   218,   219,   217,   220,   220,   220,
     220,   220,   222,   223,   221,   224,   224,   224,   224,   224,
     225,   225,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   228,   228,   229,   230,   230,   230,   231,   231,   231,
     231,   231,   232,   232,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   234,   234,   234,   234,   234,   234,   235,
     235,   236,   236,   236,   236,   236,   236,   237,   237,   238,
     238,   239,   239,   240,   240,   241,   241,   242,   242,   243,
     243,   243,   243,   243,   244,   244,   245,   245,   245,   245,
     245,   245,   245,   246,   246,   247,   247,   247,   248,   248,
     249,   249,   249,   250,   250,   250,   250,   250,   251,   251,
     251,   252,   252,   253,   253,   254,   254,   254,   255,   255,
     255,   256,   256,   256,   257,   257,   257,   257,   257,   257,
     257,   258,   258,   258,   258,   258,   259,   259,   259,   259,
     259,   260,   260,   260,   260,   261,   261,   261,   262,   262,
     262,   262,   262,   263,   263,   263,   263,   263,   264,   264,
     264,   264,   265,   265,   266,   266,   266,   267,   267,   268,
     268,   269,   269,   270,   270,   270,   270,   271,   271,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   274,   275,   276,   276,   276,   276,   276,   276,
     276,   276,   277,   277,   278,   279,   279,   280,   281,   281,
     282,   282,   282,   283,   283,   283,   283,   283,   283,   284,
     284,   285,   286,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   288,
     288,   289,   289,   289,   290,   290,   290,   290,   291,   291,
     292,   292,   292,   293,   293,   293,   293,   294,   294,   294,
     294,   295,   295,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   297,   297,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     299,   299,   299,   299,   299,   299,   300,   300,   300,   300,
     301,   301,   301,   301,   302,   302,   302,   302,   302,   302,
     302
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     3,     0,     1,     3,     4,     5,     4,
       6,     6,     0,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     0,     1,     2,     1,     1,     1,     0,     1,     1,
       1,     1,     1,     2,     1,     0,     1,     2,     2,     0,
       2,     2,     4,     4,     1,     3,     3,     3,     3,     3,
       3,     3,     2,     1,     1,     3,     4,     4,     2,     4,
       0,     2,     2,     1,     1,     1,     2,     1,     4,     5,
       3,     1,     3,     3,     5,     1,     1,     3,     1,     2,
       3,     0,     2,     2,     3,     2,     4,     3,     3,     4,
       3,     0,     2,     2,     2,     1,     0,     2,     2,     2,
       1,     4,     4,     6,     3,     0,     1,     1,     3,     4,
       3,     4,     6,     0,     2,     2,     2,     2,     1,     1,
       3,     3,     1,     3,     1,     1,     1,     3,     3,     0,
       1,     1,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     4,     4,     4,     5,     2,
       2,     2,     2,     2,     2,     6,     6,     4,     9,     9,
       7,     6,     6,     4,     9,     9,     7,     4,     6,     6,
       9,     9,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     0,     1,     4,     1,     3,     4,     1,     3,
       4,     3,     3,     1,     1,     2,     1,     2,     1,     1,
       3,     2,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     3,     3,     3,     3,     2,     2,     1,     0,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     4,     3,     3,     3,     3,     3,     4,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       4,     3,     4,     3,     4,     3,     5,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   524,   525,   526,   527,
     528,     0,   462,    70,   100,     0,   198,   462,     0,   105,
       0,   344,    70,     0,     0,     0,     0,   208,   499,   499,
     521,     0,     0,     0,     0,   104,     0,   218,   101,     0,
     214,     0,     0,   107,     0,     0,   453,     0,   221,     0,
     343,     0,     0,     0,    45,    44,   103,   199,   345,     0,
       0,     0,     0,     0,     0,     0,   106,     0,     0,    83,
       0,   522,     0,     0,     0,   102,   200,     0,     0,   347,
       0,     0,   576,   578,     0,   579,   580,   498,     0,     0,
     577,   574,   122,   575,     0,     0,     0,     4,     0,     5,
       0,     9,    46,    10,    11,    12,    14,   417,   418,    22,
      13,   123,   129,    15,    17,    16,    19,    20,    21,    18,
     128,     0,   126,     0,   488,     0,   130,   127,     0,   131,
     504,   484,   485,   421,   419,     0,     0,   489,   490,     0,
     420,     0,   505,   506,   507,   523,   483,   423,   422,   486,
     487,     0,    38,    24,   429,     0,     0,   463,    71,     0,
       0,     0,     0,     0,     0,     0,     0,   488,   504,   419,
     489,   490,   431,   420,   505,   506,     0,   462,     0,     0,
     346,     0,   172,     0,   402,     0,   409,   500,   219,   527,
     136,     0,     0,   220,     0,     0,     0,     0,     0,     0,
       0,   499,   135,     0,     0,   409,    77,    84,    95,    89,
      97,    79,    88,    98,    85,    99,    93,    87,    94,    92,
      90,    91,    78,    80,    86,    96,     0,    81,     0,     0,
       0,     0,   351,     0,   119,    32,     0,   562,   481,     0,
     432,   563,     7,   409,   499,   125,   124,   314,   399,     0,
     398,     0,     0,   120,   503,     0,     0,    35,     0,   434,
     424,     0,   409,    36,   430,     0,   179,   175,     0,   420,
     179,   176,     0,   340,     0,   433,     0,   398,     0,     0,
     565,   497,   561,   564,   560,     0,    48,    51,     0,     0,
     404,     0,   406,     0,     0,   405,     0,     0,   398,     0,
       0,     6,     0,    47,     0,   201,     0,   300,   299,   222,
       0,    23,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   496,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   266,   267,   268,   263,   265,     0,     0,   261,
     264,   262,   260,     0,   270,   269,     0,     0,   409,   409,
       0,     0,     0,    25,    26,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,    28,     0,    29,     0,   417,
     415,     0,   410,   411,   416,     0,     0,     0,     0,     0,
     145,     0,     0,   144,     0,   153,     0,     0,     0,   151,
       0,     0,     0,    57,   132,     0,   165,     0,     0,    30,
     282,   215,   355,     0,   356,   358,     0,   380,     0,   361,
       0,     0,   118,    31,    33,     0,   313,     0,    64,   501,
     502,   121,     0,    34,   409,     0,   186,   177,   173,   178,
     174,     0,   338,   335,    61,     0,    57,   138,    37,    50,
      49,    52,     0,   529,     0,     0,   517,     0,   519,     0,
       0,     0,     0,     0,     0,     0,     0,   533,     8,     0,
      40,     0,     0,     0,     0,     0,   293,   350,   461,   557,
     556,   559,   567,   566,   571,   570,   553,   550,   551,   552,
     493,   494,   492,   540,   515,   516,   514,   513,   495,   544,
     555,   549,   547,   558,   548,   546,   538,   543,   545,   554,
     537,   541,   542,   539,     0,     0,     0,     0,     0,     0,
       0,   569,   568,   573,   572,   473,   474,   476,   478,     0,
     465,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     531,   462,   462,   141,   336,   348,   403,     0,     0,   426,
       0,   337,   201,     0,     0,     0,     0,   437,     0,     0,
       0,   154,     0,   443,     0,     0,   152,   575,    60,     0,
      53,    58,     0,   164,     0,     0,     0,   425,   287,   284,
     285,   286,   290,   291,   292,   282,     0,   275,     0,   283,
     301,     0,   359,     0,   112,   114,   115,   110,   111,   109,
     108,   113,     0,   379,   378,   484,     0,   353,   482,   352,
     512,   401,   400,     0,     0,     0,   427,     0,   180,   342,
     484,     0,     0,     0,   530,   491,   518,   407,   520,   408,
     161,     0,     0,     0,   532,   159,   447,     0,   535,   534,
       0,    42,    41,    39,   202,     0,     0,   212,     0,   209,
     297,   294,   295,   298,   223,     0,    68,    69,    67,    66,
      65,   510,   511,   475,   477,     0,   464,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     137,   413,   414,   412,     0,   349,     0,   163,     0,     0,
     143,     0,   142,     0,   468,     0,     0,   149,     0,     0,
     147,     0,     0,   133,     0,   134,     0,     0,   167,     0,
     169,   288,   289,     0,   282,   273,     0,   393,   302,   305,
     304,   306,     0,   357,   360,   361,     0,     0,   362,   363,
     188,     0,     0,   187,   190,   428,     0,   181,   184,     0,
     339,    62,    63,   162,     0,     0,     0,     0,   160,     0,
       0,   217,   207,     0,   210,   206,   296,   301,   271,    72,
     255,    84,   253,    89,    74,    88,    85,   258,    99,    87,
      90,    73,    75,    86,   234,   237,   235,   236,   247,   238,
     251,   243,   241,   254,   257,   242,   240,   245,   250,   252,
     239,   244,   248,   249,   246,   256,     0,   232,    76,     0,
     271,   271,   230,   466,   361,   504,   504,     0,     0,     0,
       0,     0,     0,     0,     0,   140,   139,     0,   146,     0,
       0,   436,     0,   435,     0,   467,     0,     0,   442,   150,
       0,   441,   148,    55,    54,   166,   452,   168,     0,   393,
     276,     0,     0,   361,   303,   319,   354,   384,     0,   531,
       0,   192,     0,     0,     0,   182,     0,   157,   449,     0,
       0,   155,   448,     0,   536,   196,   499,   217,   217,   194,
     217,   204,   213,   211,     0,   282,   229,   233,     0,     0,
     225,   226,   470,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   217,   440,     0,     0,   469,   446,     0,     0,
       0,     0,   171,   361,   393,     0,   396,   395,   397,   484,
     315,   279,   277,     0,     0,     0,     0,   382,   484,   193,
     191,     0,   185,     0,   158,     0,   156,   205,   375,     0,
     307,     0,   231,    72,    73,   271,   271,     0,     0,     0,
       0,     0,     0,   197,   195,     0,     0,     0,     0,    56,
     170,   278,   361,   385,     0,   316,   318,   317,   333,     0,
     334,   321,   324,     0,   320,   311,   312,   216,     0,   183,
       0,     0,     0,   374,   373,   484,   308,   319,   272,   227,
     228,     0,     0,     0,     0,     0,     0,   439,   438,   445,
     444,   281,   280,   387,   388,   390,   484,     0,   531,   332,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   484,
     451,   450,     0,   365,     0,     0,     0,   389,   391,   323,
     325,   326,   329,   330,   331,   327,   328,   322,   370,   368,
     484,   531,   309,   224,   310,   385,   369,   484,   392
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   287,    98,   573,   100,   101,   102,   304,
     103,   288,   568,   572,   569,   104,   105,   106,   159,   797,
     230,   107,   227,   108,   603,   192,   235,   109,   110,   111,
     112,   113,   114,   676,   115,   116,   117,   437,   618,   739,
     118,   119,   614,   734,   120,   121,   474,   751,   122,   123,
     648,   649,   167,   228,   590,   125,   126,   476,   757,   654,
     799,   800,   407,   876,   411,   586,   587,   588,   589,   655,
     309,   722,   977,  1033,   967,   248,   956,   911,   915,   961,
     962,   963,   127,   274,   442,   128,   129,   231,   232,   415,
     416,   607,   974,   930,   419,   604,   994,   908,   843,   289,
     183,   293,   294,   381,   382,   383,   168,   131,   132,   133,
     169,   135,   156,   157,   529,   396,   693,   530,   531,   136,
     170,   171,   139,   193,   384,   173,   141,   174,   175,   144,
     145,   146,   299,   147,   148,   149,   150,   151
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -901
static const yytype_int16 yypact[] =
{
    -901,    82,  3043,  -901,   -64,  -901,  -901,  -901,  -901,  -901,
    -901,  4468,    -9,   130,  -901,  9800,  -901,    -9,  9800,  -901,
     181,    37,   130,  4468,  9800,  4468,    21,  -901,  9800,  6959,
    -901,  8312,  8898,  6801,  9800,  -901,    30,  -901,  -901, 16454,
    -901,  2474,  9033,  -901,  9800,  9800,  -901,  9800,  -901,  9800,
    -901,  9800,   223,    91,  1561,  1561,  -901,  -901,  -901,  9168,
    7861,  9800,  9033,  9800,  9800,    51,  -901,   106,  4468,  -901,
    9800,  -901,  9935,  9935, 16454,  -901,  -901,  9800,  9168,  -901,
    9800,  9800,  -901,  -901,  9800,  -901,  -901, 11960,  9800,  9800,
    -901,  9800,  -901,  -901,  3520,  7094,  8447,  -901,  4310,  -901,
     115,  -901,   292,  -901,  -901,  -901,  -901,  -901,  -901,  -901,
    -901,  -901,  -901,  -901,  -901,  -901,  -901,  -901,  -901,  -901,
    -901, 16454,  -901, 16454,   305,    55,  -901,  -901,  2474,  -901,
     220,  -901,  -901,  -901,   226,   247,   266,   261,   264, 16112,
    2172,    53,   271,   282,  -901,  -901,  -901,  -901,  -901,  -901,
    -901,   341,  -901,  -901, 16112,   287,  4468,  -901,  -901,   316,
    9800,  9800,  9800,  9800,  9800,  9168,  9168,   194,  -901,  -901,
    -901,  -901,   353,   318,  -901,  -901,   333, 14252, 16454,  2474,
    -901,   344,  -901,   128, 16112,   395,  8019, 16112,  -901,   127,
    -901, 16454,   161,   180, 16454,   347,    17, 13812,    10, 13737,
     180,  9800,  -901, 14197, 13062,  8019,  -901,  -901,  -901,  -901,
    -901,  -901,  -901,  -901,  -901,  -901,  -901,  -901,  -901,  -901,
    -901,  -901,  -901,  -901,  -901,  -901,  4468,  -901,   348,  2372,
      67,    33,  -901,  4468,  -901,  -901, 13887,   338,   353, 13887,
     353,   338,  -901,  8019,  9800,  -901,  -901, 16454,  -901,   222,
   16112,  9800,  9800,  -901, 16112,   356, 14112,  -901, 13887,   353,
   16112,   350,  8019,  -901, 16112, 14432,  -901,  -901, 14572, 12222,
    -901,  -901, 14617,   398,   360,   353,   262, 14337, 13887, 14653,
     351,  1725,   338,   351,   338,   143,  -901,  -901,  3678,   124,
    -901,  9800,  -901,   -31,    97,  -901,   -20,    89, 14793,    84,
     505,  -901,  3836,  -901,   457,   387,   362,  -901,  -901,  -901,
     273,  -901,  9800,  9800,  9800,  9800,  8898,  8898,  9800,  9800,
    9800,  9800, 10070,  9800,   131, 11960,  9800,  9800,  9800,  9800,
    9800,  9800,  9800,  9800,  9800,  9800,  9800,  9800,  9800,  9800,
    9800,  -901,  -901,  -901,  -901,  -901,  -901,  8605,  8605,  -901,
    -901,  -901,  -901,  8605,  -901,  -901,  8605,  8605,  8019,  8019,
    8898,  8898,  7568,  -901,  -901, 14392, 14477, 14824,    68,  3994,
    -901,  8898,    17,   368,   279,  -901,  9800,  -901,  9800,   409,
    -901,   363,   390,  -901,  -901,   379, 16454,  2474,   489,  9168,
    -901,  4626,  8898,  -901,   374,  -901,    17,  4784,  8898,  -901,
      17,   180, 10205,  9800,  -901,  4468,   502,  9800,   375,  -901,
     419,  -901,  -901,  2372,  -901,   407,   383,  -901, 10340,   436,
    9800,  2474,  -901,  -901,  -901,   393,  -901,  9168,  -901, 16112,
   16112,  -901,    29,  -901,  8019,   396,  -901,   536,  -901,   536,
    -901, 10475,   427,  -901,  -901, 10205,  9800,  -901,  -901,  -901,
    -901,  -901,  8740,  -901, 12886,  7252,  -901,  7410,  -901,  4468,
     403,  8898,  6206,  3362,   408,  9800,  6524,  -901,  -901,   204,
    -901,  4152, 16454,  9168,   415,  2779,   188,  -901, 14879, 16294,
   16294,   361,  -901,   361,  -901,   361, 14159,   811,  1301,   760,
   12353, 12353,   353,   351,  -901,  -901,  -901,  -901,  1725, 16150,
     361,   956,   956, 16294,   956,   956,  1039,   351, 16150, 16349,
    1039,   338,   338,   351,   421,   423,   425,   429,   441,   400,
     420,  -901,   361,  -901,   361,    65,  -901,  -901,  -901,   107,
    -901,   579, 16187,   397, 10610,  8898, 10745,  8898,  9800,  8898,
   12603,    -9, 14964,  -901,  -901,  -901, 16112, 15019,  8019,  -901,
    8019,  -901,   387,   297,  9800,   111,  9800, 16112,    35, 13962,
    7568,  -901,  9800, 16112,    13, 12977,  -901,   445,   466,   448,
   15050,   466,   455,   576, 15105,  4468, 14037,  -901,   211,  -901,
    -901,  -901,  -901,  -901,  -901,   560,   137,  -901,  2553,  -901,
     201,   449,  2372,    67,  9800,    21,    30,  9800,  9800,  9800,
    9800,  9800,  6048,  -901,  -901,   462,  8177,  -901, 16112,  -901,
    -901,  -901, 16112,   452,    62,   453,  -901,  1310,  -901,  -901,
     309, 16454,   465,   473,  -901,  -901,  -901,  -901,  -901,  -901,
    -901,  4468,    -1,  2828,  -901,  -901, 16112,  4468, 16112,  -901,
   15190,  -901,  -901,  -901,   486,  1505,   474,   524,   231,  -901,
     561,  -901,  -901,  -901,  -901,  6645,  -901,  -901,  -901,  -901,
    -901,  -901,  -901,  -901,  -901,  7568,  -901,    28,  8898,  8898,
    9800,   612, 15245,  9800,   614, 15276,   484,  2868,    17,    17,
    -901,  -901,  -901,  -901,   493,  -901, 13887,  -901, 13137,  4942,
    -901,  5100,  -901,   200,  -901, 13212,  5258,  -901,    17,  5416,
    -901,    17,  9800,  -901,  9800,  -901,  4468,  9800,  -901,  4468,
     624,  -901,  -901, 16454,   728,  -901,  2372,   530,   587,  -901,
    -901,  -901,    44,  -901,  -901,   436,   506,    70,  -901,  -901,
    -901,  5574,  9168,  -901,  -901,  -901, 16454,  -901,   538,   333,
    -901,  -901,  -901,  -901,  5732,   516,  5890,   518,  -901,  9800,
     509,  2656,  -901,  9800, 16454,  -901,  -901,   201,   523,   549,
    -901,   550,  -901,   551,  -901,   553,   554,  -901,   556,   557,
     562,   564,  -901,   567,  -901, 16454,  -901,  -901,  -901,  -901,
    -901,  -901,  -901,  -901,  -901,  -901,  -901,  -901,  -901,  -901,
    -901,  -901,  -901,  -901,  -901,  -901,  9800,  -901,  -901,   568,
     523,   523,  -901,  -901,   436,   213,   243, 15416, 10880, 11015,
   15447, 11150, 11285, 11420, 11555,  -901,  -901,  2012,  -901,  4468,
    9800, 16112,  9800, 16112,  7568,  -901,  4468,  9800, 16112,  -901,
    9800, 16112,  -901, 15502, 16112,  -901, 16112,   663,  4468,   530,
    -901,   543,  9326,   158,  -901,    59,  -901,  -901,  8898, 12478,
    4468,  -901,    25,   546,  9800,  -901,  9800,  -901, 16112,  4468,
    9800,  -901, 16112,  4468, 16112,  -901,  7726,   818,   818,  -901,
     680,  -901, 16112,  -901,    92,   419,  -901,  -901, 12922, 12691,
    -901,  -901,  -901,  9800,  9800,  9800,  9800,  9800,  9800,  9800,
    9800,   559, 16314, 15105, 13287, 13362,  -901, 15105, 13437, 13512,
    9800,  4468,  -901,   436,   530,  6365,  -901,  -901,  -901,   -32,
    9168,  -901,  -901,    78,  9800,   -19, 12745,  -901,   965,   395,
    -901,   333, 16112, 13587,  -901, 13662,  -901,  -901,  -901, 11690,
     615,   244,  -901,  -901,  -901,   523,   523, 15587, 15662, 15737,
   15812, 15887, 15962,  -901,  -901,  4468,  4468,  4468,  4468, 16112,
    -901,  -901,   158,  9484,    76,  -901,  -901, 16112,  -901,    87,
    -901,   237,  -901,   508, 16037,  -901,  -901,  -901, 11555,  -901,
    4468,  4468,  3201,  -901,  -901,   182,  -901,    59,  -901,  -901,
    -901,  9800,  9800,  9800,  9800,  9800,  9800, 15105, 15105, 15105,
   15105,  -901,  -901,  -901,  -901,  -901,   294,  8898, 12095,  -901,
    9800,    78,    87,    87,    87,    87,    87,    87,    78,  1161,
   15105, 15105,   545, 11825,    83,   184, 12831,  -901,  -901, 16112,
    -901,  -901,  -901,  -901,  -901,  -901,  -901,   580,  -901,  -901,
     183, 12228,  -901,  -901,  -901,  9642,  -901,   291,  -901
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -901,  -901,  -901,     3,  -382,  1523,  -901,  -901,  -901,  -901,
     364,   411,  -274,   274,   265,  -901,  -901,   430,   695,  -901,
     540,  -142,  -640,  -901,  -736,  -901,   630,  -868,  -725,   -46,
    -901,  -901,  -901,    40,  -901,  -901,  -901,   456,  -901,   -14,
    -901,  -901,  -901,  -901,  -901,   531,   178,   -88,  -901,  -901,
    -901,   -22,  1045,  -901,  -901,  -901,  -901,  -901,  -901,  -901,
    -901,  -140,  -139,  -712,  -901,  -138,    27,   160,  -901,  -901,
    -901,   -10,  -901,  -901,  -272,   177,  -901,  -203,  -226,  -254,
    -245,  -332,  -901,  -172,  -901,   -17,   725,  -114,   340,  -901,
    -386,  -700,  -655,  -901,  -552,  -430,  -900,  -858,  -796,    -4,
    -901,    42,  -901,  -149,  -901,   209,   542,  -383,  -901,  -901,
    1072,  -901,    15,  -901,  -901,  -175,  -901,  -548,  -901,  -901,
    1397,  1496,   -11,   -12,   -58,   981,  -901,  1913,  2090,  -901,
    -901,  -901,  -901,  -901,  -901,  -901,  -901,  -352
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -532
static const yytype_int16 yytable[] =
{
     154,   357,   255,   179,   172,    97,   373,   177,   245,   246,
     533,   619,   694,   184,   310,   798,   188,   187,   187,   385,
     197,   199,   203,   204,   400,   846,   871,   591,   585,   417,
     613,   236,   176,   237,   238,   605,   239,   295,   240,   233,
     241,   725,   696,   903,   379,   417,   233,   966,   250,   254,
     256,   258,   259,   260,   233,   249,   408,   264,   620,   265,
      60,   268,   272,   379,   689,   374,   275,   277,   417,   278,
     279,   196,   198,   280,   276,   420,   281,   282,   283,  -394,
     284,   958,     3,   250,   254,   298,   394,   152,   880,   881,
     958,  -189,   297,   928,   425,   995,  -376,   155,  1017,  -394,
     455,   379,   307,  -394,   882,   394,   906,   663,   952,  -376,
     539,   913,   848,   435,   180,   804,   394,   803,   997,   394,
     379,   460,   464,  -376,   731,   539,   308,  -394,   456,   571,
     357,   461,  -189,   158,     5,  1038,   296,   369,   261,  -376,
    1018,   459,   664,   912,  -371,   927,  -117,   966,  -376,   154,
     365,   366,   367,   264,   250,   298,   427,   959,   418,  -376,
     744,   494,   368,   914,   421,   732,   369,   871,   960,   369,
     845,  -371,   571,   369,   418,   254,   387,   960,  -376,   186,
      16,   369,   497,   179,  -371,   495,  -189,  -376,   205,   401,
     187,   369,   372,   973,   254,   394,  -371,   418,  -376,   462,
    -376,   462,  -117,   951,    21,    21,   724,   462,   533,   519,
     520,   358,   650,   359,   462,   466,   379,   379,  -376,   733,
     462,    69,   929,   979,   980,   718,  -117,   496,   457,   540,
     242,   849,   254,   187,  -372,  -364,    57,   998,   665,   798,
     429,   430,   427,  -371,  1031,   467,    50,    50,  -371,   243,
     463,   254,   992,   711,   651,   452,   458,    58,    58,   376,
      76,  -372,  -364,    60,   262,   652,   666,   719,   714,   606,
     687,   302,   292,   553,  -372,  -364,   896,   178,   720,   377,
     454,   453,    79,    79,   653,   615,  -372,  -364,   712,   514,
     515,   451,   379,   910,   152,   516,   715,   721,   517,   518,
     449,   478,   479,   480,   481,   483,   485,   486,   487,   488,
     489,   492,   493,   533,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     841,   824,   585,  -372,  -364,  1032,   254,   254,  -372,  -364,
     369,  1000,   254,  -366,  -472,   254,   254,   254,   254,   522,
     524,   532,  -509,   427,  -509,   152,   482,   484,  1029,   825,
     542,   641,   754,   380,   303,   546,   315,   547,  1001,   870,
    -366,  -455,  -472,   428,  -471,   714,  1036,  -454,   250,   315,
     557,   559,   380,  -366,   387,   555,   563,   565,   755,   698,
     701,   570,   570,   427,   574,  -366,   576,   295,   311,   295,
     521,   523,  -471,   978,   421,  -386,   234,   312,   379,   608,
     421,   541,  -459,   444,   316,  -460,   612,   360,   317,   917,
     380,   361,  -457,   254,   477,  -386,   234,   316,   421,  -386,
     545,   317,   558,  -456,   570,   570,   266,   270,   564,   380,
    -341,   612,  -366,   578,   254,   362,   254,  -366,   685,   740,
     633,   612,   636,  -386,   638,   640,  -458,   745,   747,   909,
    -341,   579,   250,  -509,   580,  -509,   918,   364,   322,   644,
     323,   324,   533,   669,   451,   327,  -508,   361,  -508,   172,
     275,   322,   581,   334,   324,   582,   324,    52,   327,   369,
     682,   322,   340,   585,   324,   375,   583,   627,   327,   629,
     378,   632,   267,   271,  -377,   389,   410,   431,   434,   441,
     870,   443,   468,   472,  -377,   584,   801,   473,   475,   544,
     548,   550,   549,   557,   672,   563,   675,   574,   677,   636,
     551,   554,   560,   575,   577,   380,   380,   254,   592,   254,
     370,  -377,   593,   686,   130,   688,   975,   606,   729,   532,
    -274,   695,   610,   130,   617,   616,   678,   679,   621,   661,
     234,   234,   234,   234,   631,   130,  -377,   130,   406,   637,
     996,   645,   656,  -377,   657,   671,   658,   674,  -274,   226,
     659,   662,     5,   172,   578,  1009,   240,   259,   260,   264,
     275,   298,   660,  -377,    14,   254,   -59,   702,   727,   703,
     234,    19,   579,   234,   611,   580,   705,   706,   723,   730,
     130,   380,   735,  -377,   273,   996,   741,   427,  -377,  1002,
      35,  -377,   234,   581,   742,    38,   582,   999,   300,   611,
    1030,   752,   292,    43,   292,   753,   130,   583,   756,   611,
     130,   808,   234,   811,  1003,   813,  1004,  1005,  1037,   817,
    1006,  1007,   996,    56,   532,   838,   584,   483,   522,   807,
     842,   305,   810,   306,   844,    66,   865,   847,   854,    69,
    1021,  1022,  1023,  1024,  1025,  1026,    75,   859,   821,   863,
     823,   875,   -82,  -100,  -105,   828,  -104,  -101,   831,  -107,
    -103,   833,   257,   834,   901,  -106,   836,   -83,   130,    16,
    -102,   879,   904,    20,    21,   921,  1028,   976,   482,   521,
     622,  1001,    27,   471,    28,   866,   943,   181,   273,    33,
     623,   250,   853,   386,    37,   681,   439,   380,   852,   892,
     684,   273,   873,   858,   388,   862,   234,   931,   864,   935,
     936,   840,   872,  1034,    48,   713,    50,   874,   300,   991,
     867,  1015,   578,   868,  1027,    57,  1020,    58,   202,   683,
     234,   609,     0,     0,   234,     0,     0,     0,   130,   414,
     579,     0,     0,   580,     0,   130,    74,     0,     0,    76,
       0,     0,    79,     0,     0,   878,     0,   426,   315,     0,
       0,   581,     0,     0,   582,     0,    52,   821,   823,     0,
     828,   831,   858,   862,     0,   583,     0,     0,   893,   894,
       0,   895,     0,   532,     0,   897,   898,     0,     0,   899,
       0,   245,   246,     0,   584,     0,   390,   393,   395,   399,
     130,    92,     0,     0,     0,     0,   316,   916,   636,   315,
     317,   200,    21,   922,   130,   923,     0,     0,     0,   925,
       0,     0,    28,   244,     0,   187,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,   423,     0,     0,   424,
       0,     0,   893,   937,   938,   897,   939,   940,   941,   942,
       0,     0,    48,     0,    50,     0,   319,   316,   433,   949,
     322,   317,   323,   324,   298,    58,     0,   327,     0,   957,
       0,   954,     0,   964,     0,   334,     0,     0,   447,     0,
       0,   130,   338,   339,   340,     0,     0,     0,     0,     0,
      79,     0,   234,   234,     0,     0,   552,     0,   234,   234,
       0,     0,     0,   130,   987,   988,   989,   990,     0,   130,
     710,   322,     0,   323,   324,     0,     0,   130,   327,     0,
       0,     0,     0,   414,     0,     0,   334,   862,     0,  1010,
    1011,   298,     0,   338,   339,   340,     0,     0,  1014,     0,
     987,   988,   989,   990,  1010,  1011,     0,     0,     0,     0,
       0,   737,     0,   140,   315,     0,  1016,   636,     0,  1019,
       0,     0,   140,     0,     0,     0,     0,     0,     0,     0,
       0,   130,   543,     0,   140,   130,   140,  -381,     0,     0,
       0,     0,   643,   130,     0,   647,     0,  -381,     0,   907,
     636,     0,     0,     0,   862,     0,   561,     0,     0,     0,
     566,     0,   316,     0,     0,     0,   317,     0,     0,     0,
       0,     0,   234,   234,  -381,     0,     0,   124,     0,   140,
     234,     0,   406,   269,   269,     0,   124,     0,     0,   406,
       0,     0,   234,     0,     0,   234,     0,   315,   124,  -381,
     124,     0,     0,   667,   134,   140,  -381,     0,     0,   140,
       0,     0,   319,   134,   320,   321,   322,   955,   323,   324,
     325,     0,     0,   327,     0,   134,  -381,   134,     0,     0,
     333,   334,     0,   855,     0,   337,     0,     0,   338,   339,
     340,     0,     0,   124,     0,   316,  -381,   130,     0,   317,
       0,  -381,     0,     0,  -381,     0,     0,     0,   717,     0,
     993,     0,   414,     0,     0,     0,     0,   140,     0,   124,
     134,     0,     0,   124,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   738,     0,     0,
       0,   273,     0,     0,     0,   319,   134,   320,   321,   322,
     134,   323,   324,   130,     0,   993,   327,     0,     0,   130,
       0,     0,     0,     0,   334,     0,     0,     0,   690,   692,
       0,   338,   339,   340,   697,   700,     0,   802,     0,     0,
       0,   124,     0,  -383,     0,     0,     0,   140,     0,     0,
     805,   806,   993,  -383,   140,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   134,     0,
       0,   130,     0,   130,     0,     0,     0,     0,   130,     0,
    -383,   130,     0,     0,     0,     0,     0,     0,   130,     0,
       0,   130,     0,   839,     0,     0,   414,     0,   406,   406,
       0,     0,   406,   406,     0,  -383,     0,     0,     0,   140,
       0,   124,  -383,   130,     0,     0,   738,     0,   124,   965,
       0,     0,     0,   140,     0,   969,   130,   406,   130,   406,
       0,     0,  -383,     0,   647,     0,     0,     0,   134,     0,
       0,     0,     0,     0,     0,   134,     0,     0,   815,   816,
       0,     0,  -383,   206,     0,   877,   818,  -383,     0,     0,
    -383,     0,     0,     0,     0,   207,   208,     0,   829,   315,
       0,   832,   209,   124,     0,     0,     0,     0,     0,     0,
     210,     0,     0,     0,     0,     0,   211,   124,     0,     0,
     140,   212,     0,   213,     0,     0,   214,     0,     0,     0,
     134,   130,     0,     0,   215,     0,     0,     0,   130,     0,
       0,     0,   140,     0,   134,   216,     0,   316,   140,   965,
     130,   317,     0,     0,   217,     0,   140,     0,     0,     0,
       0,     0,   130,   218,   219,     0,   220,     0,   221,   137,
     222,   130,     0,   223,     0,   130,     0,   224,   137,     0,
     225,     0,     0,     0,   124,     0,     0,     0,     0,     0,
     137,     0,   137,     0,     0,     0,     0,   319,     0,     0,
     321,   322,     0,   323,   324,     0,   124,     0,   327,     0,
     140,   134,   124,   130,   140,     0,   334,     0,     0,     0,
     124,     0,   140,   338,   339,   340,     0,     0,     0,     0,
       0,     0,     0,   134,     0,   137,   369,     0,   736,   134,
       0,     0,     0,     0,     0,     0,     0,   134,     0,     0,
       0,     0,   920,     0,     0,     0,     0,   130,   130,   130,
     130,   137,     0,     0,     0,   137,     0,     0,   138,     0,
       0,     0,     0,     0,   124,     0,   750,   138,   124,     0,
       0,     0,   130,   130,     0,     0,   124,     0,     0,   138,
       0,   138,     0,     0,  -203,    99,     0,     0,  -203,  -203,
       0,   134,     0,     0,   153,   134,     0,  -203,     0,  -203,
    -203,     0,     0,   134,  -203,     0,   182,     0,   185,  -203,
       0,     0,  -203,   137,     0,     0,   140,     0,     0,     0,
       0,     0,     0,     0,   138,     0,     0,     0,     0,  -203,
       0,  -203,     0,  -203,     0,  -203,  -203,     0,  -203,     0,
    -203,     0,  -203,     0,   200,    21,     0,     0,     0,     0,
     138,   263,     0,     0,   138,    28,   244,     0,     0,     0,
       0,  -203,     0,     0,  -203,    37,     0,  -203,  -217,     0,
       0,     0,   140,     0,     0,     0,     0,    99,   140,     0,
     124,   301,     0,   137,     0,    48,     0,    50,     0,     0,
     137,     0,  -217,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,   134,     0,     0,
       0,     0,   138,     0,     0,     0,  -203,     0,     0,     0,
       0,     0,  -203,    79,     0,     0,     0,     0,     0,     0,
     140,     0,   140,     0,     0,     0,   124,   140,     0,   363,
     140,     0,   124,     0,     0,   137,     0,   140,     0,     0,
     140,     0,     0,     0,     0,     0,     0,     0,     0,   137,
       0,     0,     0,   134,     0,     0,     0,     0,     0,   134,
       0,     0,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   138,     0,     0,   140,     0,   140,     0,   138,
       0,     0,     0,     0,   124,     0,   124,     0,     0,     0,
       0,   124,     0,     0,   124,     0,     0,     0,     0,   409,
       0,   124,     0,   315,   124,     0,   422,     0,     0,     0,
       0,   134,     0,   134,     0,     0,   137,     0,   134,     0,
       0,   134,     0,     0,     0,     0,   124,     0,   134,     0,
       0,   134,     0,     0,   138,     0,     0,     0,   137,   124,
       0,   124,     0,     0,   137,     0,     0,     0,   138,     0,
     140,   316,   137,   134,     0,   317,     0,   140,     0,     0,
       0,    99,     0,     0,     0,     0,   134,     0,   134,   140,
       0,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,   140,     0,     0,     0,     0,     0,     0,     0,     0,
     140,     0,     0,     0,   140,     0,     0,     0,     0,     0,
       0,   319,     0,   320,   321,   322,   137,   323,   324,     0,
     137,     0,   327,     0,   124,   138,     0,     0,   137,   333,
     334,   124,     0,     0,   337,     0,     0,   338,   339,   340,
       0,     0,   140,   124,     0,     0,     0,   138,     0,     0,
       0,   134,    99,   138,     0,   124,     0,     0,   134,     0,
       0,   138,     0,     0,   124,     0,     0,     0,   124,     0,
     134,     0,     0,     0,   422,   142,     0,     0,     0,     0,
     422,     0,   134,     0,   142,     0,   140,   140,   140,   140,
       0,   134,     0,     0,     0,   134,   142,     0,   142,     0,
       0,     0,     0,     0,     0,     0,   124,     0,     0,     0,
       0,   140,   140,     0,     0,   138,     0,     0,     0,   138,
       0,     0,     0,     0,     0,     0,     0,   138,     0,     0,
       0,     0,   137,   134,     0,     0,     0,     0,     0,     0,
       0,   142,   630,     0,     0,     0,   635,     0,     0,     0,
     124,   124,   124,   124,    99,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   142,     0,     0,
       0,   142,     0,   891,     0,   124,   124,   134,   134,   134,
     134,     0,     0,     0,     0,     0,     0,     0,   137,     0,
       0,  -203,     0,     0,   137,  -203,  -203,     0,     0,     0,
       0,     0,   134,   134,  -203,     0,  -203,  -203,     0,     0,
       0,  -203,     0,     0,     0,     0,  -203,     0,     0,  -203,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   142,
       0,   138,     0,     0,     0,     0,  -203,     0,  -203,     0,
    -203,     0,  -203,  -203,     0,  -203,   137,  -203,   137,  -203,
       0,     0,   143,   137,     0,     0,   137,     0,   708,     0,
       0,   143,     0,   137,     0,     0,   137,     0,  -203,     0,
       0,  -203,     0,   143,  -203,   143,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   138,   137,     0,
       0,     0,     0,   138,     0,     0,     0,     0,     0,   142,
       0,   137,     0,   137,     0,     0,   142,     0,     0,     0,
       0,     0,     0,     0,   743,     0,     0,     0,   143,     0,
     748,     0,     0,  -203,     0,     0,     0,     0,     0,  -203,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   143,   138,     0,   138,   143,     0,
       0,     0,   138,     0,     0,   138,     0,     0,     0,     0,
       0,   142,   138,     0,     0,   138,     0,     0,     0,     0,
       0,     0,   422,     0,   422,   142,   137,     0,     0,   422,
       0,     0,   422,   137,     0,     0,     0,   138,     0,   835,
       0,     0,   837,     0,     0,   137,     0,     0,     0,     0,
     138,     0,   138,     0,     0,     0,   143,   137,     0,     0,
       0,     0,     0,     0,   851,     0,   137,     0,     0,     0,
     137,     0,     0,     0,     0,     0,     0,   857,     0,   861,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   142,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   137,     0,
       0,     0,     0,     0,   142,     0,     0,     0,     0,     0,
     142,     0,     0,  -479,     0,   138,   143,     0,   142,     0,
       0,     0,   138,   143,     0,     0,     0,   356,     0,     0,
    -508,     0,  -508,     0,   138,     0,     0,     0,     0,     0,
       0,     0,   137,   137,   137,   137,   138,     0,     0,     0,
       0,     0,     0,     0,     0,   138,     0,     0,     0,   138,
       0,   902,     0,     0,     0,     0,     0,   137,   137,     0,
       0,     0,   142,   919,     0,   206,   142,     0,   143,     0,
       0,     0,   924,     0,   142,     0,   926,   207,   208,     0,
       0,     0,   143,     0,   209,     0,     0,   138,     0,     0,
       0,     0,   210,     0,     0,     0,     0,     0,   211,     0,
       0,     0,     0,   212,     0,   213,     0,     0,   214,     0,
       0,     0,     0,     0,   950,     0,   215,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   216,     0,     0,
       0,   138,   138,   138,   138,     0,   217,     0,     0,     0,
       0,     0,     0,     0,     0,   218,   219,     0,   220,   143,
     221,     0,   222,     0,     0,   223,   138,   138,     0,   224,
     412,     0,   225,     0,     0,     0,     0,   206,     0,     0,
       0,   143,     0,     0,     0,     0,     0,   143,   142,   207,
     208,     0,     0,     0,     0,   143,   209,     0,     0,     0,
       0,     0,     0,     0,   210,     0,     0,     0,     0,     0,
     211,     0,     0,     0,     0,   212,     0,   213,     0,     0,
     214,     0,     0,     0,     0,     0,     0,     0,   215,     0,
     413,     0,     0,     0,     0,     0,     0,     0,     0,   216,
       0,     0,     0,     0,   142,     0,     0,     0,   217,   143,
     142,     0,     0,   143,     0,     0,   206,   218,   219,     0,
     220,   143,   221,     0,   222,     0,     0,   223,   207,   208,
       0,   224,     0,     0,   225,   209,     0,     0,     0,     0,
       0,     0,     0,   210,     0,     0,     0,     0,     0,   211,
       0,     0,     0,     0,   212,     0,   213,     0,     0,   214,
       0,     0,   142,     0,   142,     0,     0,   215,     0,   142,
       0,     0,   142,     0,     0,     0,     0,     0,   216,   142,
       0,     0,   142,     0,     0,     0,     0,   217,     0,     0,
       0,     0,   229,     0,     0,     0,   218,   219,     0,   220,
       0,   221,     0,   222,   142,     0,   223,     0,     0,     0,
     224,     0,     0,   225,     0,     0,     0,   142,     0,   142,
       0,     0,     0,     0,     0,   143,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    16,     0,     0,     0,    20,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
      28,   866,     0,     0,     0,    33,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   716,     0,     0,     0,     0,     0,     0,     0,     0,
      48,   143,    50,     0,    52,     0,   867,   143,     0,   868,
       0,    57,   142,    58,     0,     0,     0,     0,     0,   142,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   142,    74,     0,     0,    76,     0,     0,    79,     0,
       0,     0,     0,   142,     0,     0,     0,     0,     0,     0,
       0,     0,   142,     0,     0,     0,   142,     0,     0,   143,
     646,   143,   206,     0,     0,     0,   143,     0,     0,   143,
       0,     0,     0,     0,   207,   208,   143,     0,     0,   143,
       0,   209,     0,     0,     0,     0,     0,    92,     0,   210,
       0,     0,     0,   869,   142,   211,     0,     0,     0,     0,
     212,   143,   213,     0,     0,   214,     0,     0,     0,     0,
       0,     0,     0,   215,   143,     0,   143,     0,   313,     0,
       0,     0,     0,     0,   216,   314,     0,     0,     0,     0,
       0,     0,     0,   217,     0,     0,   315,     0,   142,   142,
     142,   142,   218,   219,     0,   220,     0,   221,     0,   222,
       0,     0,   223,     0,     0,     0,   224,     0,   313,   225,
       0,     0,     0,   142,   142,   314,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,   316,     0,     0,     0,   317,   143,
       0,     0,     0,     0,     0,     0,   143,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   143,     0,
       0,     0,     0,     0,   394,     0,     0,     0,   318,     0,
     143,     0,     0,     0,   316,     0,     0,     0,   317,   143,
       0,     0,     0,   143,   319,     0,   320,   321,   322,     0,
     323,   324,   325,     0,   326,   327,   328,   329,   330,     0,
     331,   332,   333,   334,     0,   335,   336,   337,   318,     0,
     338,   339,   340,     0,     0,     0,     0,     0,     0,   746,
       0,   143,     0,     0,   319,     0,   320,   321,   322,     0,
     323,   324,   325,     0,   326,   327,   328,   329,   330,     0,
     331,   332,   333,   334,     0,   335,   336,   337,     0,     0,
     338,   339,   340,     0,     0,     0,     0,     0,     0,   814,
       0,     0,     0,     0,     0,   143,   143,   143,   143,     0,
       0,     0,     0,    -2,     4,     0,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
     143,   143,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,    25,    26,     0,    27,     0,    28,    29,    30,
      31,    32,    33,    34,    35,     0,    36,    37,     0,    38,
    -217,     0,    39,    40,    41,     0,    42,    43,    44,   -43,
      45,    46,     0,    47,     0,     0,     0,    48,    49,    50,
      51,    52,    53,    54,  -217,   -43,    55,    56,    57,     0,
      58,    59,    60,     0,    61,    62,    63,    64,    65,    66,
      67,    68,     0,    69,    70,     0,    71,    72,    73,    74,
      75,     0,    76,    77,    78,    79,     0,     0,    80,     0,
      81,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,    89,
       0,    90,    91,     0,    92,     0,     0,    93,     0,    94,
       0,    95,  1012,    96,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   160,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   164,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,   247,     0,     0,     0,    93,     0,   165,     0,    95,
       0,   166,  1013,     4,     0,     5,     6,     7,     8,     9,
      10,     0,  -531,     0,    11,    12,    13,    14,    15,  -531,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
    -531,    25,    26,  -531,    27,     0,    28,    29,    30,    31,
      32,    33,    34,    35,     0,    36,    37,     0,    38,  -217,
       0,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,     0,     0,     0,    48,    49,    50,    51,
       0,    53,    54,  -217,   -43,    55,    56,    57,  -531,    58,
      59,    60,  -531,    61,    62,    63,    64,    65,    66,    67,
      68,     0,    69,    70,     0,    71,    72,    73,    74,    75,
       0,    76,    77,    78,    79,     0,     0,    80,     0,    81,
       0,     0,  -531,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -531,    84,
    -531,  -531,  -531,  -531,  -531,  -531,  -531,     0,  -531,  -531,
    -531,  -531,  -531,     0,  -531,  -531,  -531,  -531,    89,  -531,
    -531,  -531,     0,    92,  -531,  -531,  -531,     0,    94,  -531,
      95,   285,    96,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
      34,    35,     0,    36,    37,     0,    38,  -217,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,     0,     0,     0,    48,    49,    50,    51,    52,    53,
      54,  -217,   -43,    55,    56,    57,     0,    58,    59,    60,
       0,    61,    62,    63,    64,    65,    66,    67,    68,     0,
      69,    70,     0,    71,    72,    73,    74,    75,     0,    76,
      77,    78,    79,     0,     0,    80,     0,    81,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,     0,    89,     0,    90,    91,
     247,    92,     0,     0,    93,     0,    94,   286,    95,     4,
      96,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,    34,    35,
       0,    36,    37,     0,    38,  -217,     0,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,    47,     0,
       0,     0,    48,    49,    50,    51,    52,    53,    54,  -217,
     -43,    55,    56,    57,     0,    58,    59,    60,     0,    61,
      62,    63,    64,    65,    66,    67,    68,     0,    69,    70,
       0,    71,    72,    73,    74,    75,     0,    76,    77,    78,
      79,     0,     0,    80,     0,    81,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,     0,    92,
       0,     0,    93,     0,    94,   450,    95,   469,    96,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,    25,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,    34,    35,     0,    36,
      37,     0,    38,  -217,     0,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,    47,     0,     0,     0,
      48,    49,    50,    51,    52,    53,    54,  -217,   -43,    55,
      56,    57,     0,    58,    59,    60,     0,    61,    62,    63,
      64,    65,    66,    67,    68,     0,    69,    70,     0,    71,
      72,    73,    74,    75,     0,    76,    77,    78,    79,     0,
       0,    80,     0,    81,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    89,     0,    90,    91,     0,    92,     0,     0,
      93,     0,    94,   470,    95,   285,    96,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,    37,     0,
      38,  -217,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,     0,     0,     0,    48,    49,
      50,    51,    52,    53,    54,  -217,   -43,    55,    56,    57,
       0,    58,    59,    60,     0,    61,    62,    63,    64,    65,
      66,    67,    68,     0,    69,    70,     0,    71,    72,    73,
      74,    75,     0,    76,    77,    78,    79,     0,     0,    80,
       0,    81,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
      89,     0,    90,    91,     0,    92,     0,     0,    93,     0,
      94,   286,    95,     4,    96,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,    25,    26,     0,    27,     0,    28,    29,    30,    31,
      32,    33,    34,    35,     0,    36,    37,     0,    38,  -217,
       0,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,     0,     0,     0,    48,    49,    50,    51,
      52,    53,    54,  -217,   -43,    55,    56,    57,     0,    58,
      59,    60,     0,    61,    62,    63,    64,    65,    66,    67,
      68,     0,    69,    70,     0,    71,    72,    73,    74,    75,
       0,    76,    77,    78,    79,     0,     0,    80,     0,    81,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,     0,    92,     0,     0,    93,     0,    94,   642,
      95,     4,    96,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
      34,    35,     0,    36,    37,     0,    38,  -217,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,     0,     0,     0,    48,    49,    50,    51,   300,    53,
      54,  -217,   -43,    55,    56,    57,     0,    58,    59,    60,
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
      27,     0,    28,    29,    30,    31,    32,    33,    34,    35,
       0,    36,    37,     0,    38,  -217,     0,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,    47,     0,
       0,     0,    48,    49,    50,    51,     0,    53,    54,  -217,
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
      28,    29,    30,    31,    32,    33,   556,    35,     0,    36,
      37,     0,    38,  -217,     0,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,    47,     0,     0,     0,
      48,    49,    50,    51,     0,    53,    54,  -217,   -43,    55,
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
      30,    31,    32,    33,   562,    35,     0,    36,    37,     0,
      38,  -217,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,     0,     0,     0,    48,    49,
      50,    51,     0,    53,    54,  -217,   -43,    55,    56,    57,
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
      32,    33,   820,    35,     0,    36,    37,     0,    38,  -217,
       0,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,     0,     0,     0,    48,    49,    50,    51,
       0,    53,    54,  -217,   -43,    55,    56,    57,     0,    58,
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
     822,    35,     0,    36,    37,     0,    38,  -217,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,     0,     0,     0,    48,    49,    50,    51,     0,    53,
      54,  -217,   -43,    55,    56,    57,     0,    58,    59,    60,
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
      27,     0,    28,    29,    30,    31,    32,    33,   827,    35,
       0,    36,    37,     0,    38,  -217,     0,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,    47,     0,
       0,     0,    48,    49,    50,    51,     0,    53,    54,  -217,
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
      28,    29,    30,    31,    32,    33,   830,    35,     0,    36,
      37,     0,    38,  -217,     0,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,    47,     0,     0,     0,
      48,    49,    50,    51,     0,    53,    54,  -217,   -43,    55,
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
      23,    24,     0,   850,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,    37,     0,
      38,  -217,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,     0,     0,     0,    48,    49,
      50,    51,     0,    53,    54,  -217,   -43,    55,    56,    57,
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
      32,    33,   856,    35,     0,    36,    37,     0,    38,  -217,
       0,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,     0,     0,     0,    48,    49,    50,    51,
       0,    53,    54,  -217,   -43,    55,    56,    57,     0,    58,
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
     860,    35,     0,    36,    37,     0,    38,  -217,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,     0,     0,     0,    48,    49,    50,    51,     0,    53,
      54,  -217,   -43,    55,    56,    57,     0,    58,    59,    60,
       0,    61,    62,    63,    64,    65,    66,    67,    68,     0,
      69,    70,     0,    71,    72,    73,    74,    75,     0,    76,
      77,    78,    79,     0,     0,    80,     0,    81,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,     0,    89,     0,    90,    91,
       0,    92,     0,     0,    93,     0,    94,     0,    95,   726,
      96,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     160,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   161,   162,     0,   163,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,   247,     0,
       0,     0,    93,     0,   165,     0,    95,     0,   166,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   160,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    89,     0,    90,    91,   247,     0,     0,     0,
      93,     0,   165,     0,    95,     0,   166,   634,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   160,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,   247,     0,     0,     0,    93,
       0,   165,     0,    95,     0,   166,   953,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   160,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   758,     0,   759,     0,
      83,    84,    85,    86,     0,   760,     0,     0,    87,     0,
     761,   208,   762,     0,     0,     0,     0,   763,    88,     0,
      89,     0,    90,    91,     0,   210,     0,     0,    93,     0,
     165,   764,    95,     0,   166,   639,   765,     0,   213,     0,
       0,   766,     0,   767,     0,     0,     0,     0,     0,   768,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     216,     0,     0,     0,     0,     0,     0,     0,     0,   769,
       0,     0,     0,     0,     0,     0,     0,     0,   218,   219,
       0,   770,     0,   221,     0,   771,     0,     0,   772,     0,
       0,     0,   773,     0,     0,   225,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   341,   342,   343,   344,
     345,   346,     0,     0,   349,   350,   351,   352,     0,   354,
     355,   774,   775,   776,   777,     0,     0,   778,     0,     0,
       0,   779,   780,   781,   782,   783,   784,   785,   786,   787,
     788,   789,   790,     0,   791,     0,     0,   792,   793,   794,
     795,     0,     0,   796,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   160,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,   200,    21,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,   201,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,    49,    50,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,    58,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   164,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,    77,     0,    79,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,     0,     0,     0,     0,    93,     0,   165,     0,    95,
       0,   166,     5,     6,     7,     8,   189,    10,   190,     0,
       0,   160,     0,     0,    14,    15,     0,     0,  -116,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
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
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,   251,   252,
       0,    75,   290,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,   291,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
      89,     0,    90,    91,   247,     0,     0,     0,    93,     0,
     165,     0,    95,     0,   166,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   160,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   163,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,   251,   252,     0,    75,
     290,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,   247,     0,     0,     0,    93,     0,   165,     0,
      95,   626,   166,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   160,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,   251,   252,     0,    75,   290,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,     0,    89,     0,    90,    91,
     247,     0,     0,     0,    93,     0,   165,     0,    95,   628,
     166,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     160,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,   525,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   161,   162,     0,   163,    35,
     526,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,   527,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,    77,     0,
     528,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,     0,     0,
       0,     0,    93,     0,   165,     0,    95,     0,   166,     5,
       6,     7,     8,   189,    10,     0,     0,     0,   160,     0,
       0,    14,    15,     0,     0,  -116,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,  -116,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,   191,    75,     0,  -116,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      88,     0,    89,   160,    90,    91,    14,    15,     0,     0,
      93,     0,   165,    19,    95,     0,   166,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   164,
       0,    69,     0,     0,    71,   251,   252,     0,    75,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,     0,   253,     0,     0,    93,     0,   165,     0,    95,
       0,   166,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   160,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,   251,   252,     0,    75,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,     0,    89,     0,    90,    91,   247,
       0,     0,     0,    93,     0,   165,     0,    95,     0,   166,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   160,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,   728,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   164,     0,    69,     0,     0,
      71,   251,   252,     0,    75,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    88,     0,    89,   160,    90,    91,    14,    15,     0,
       0,    93,     0,   165,    19,    95,     0,   166,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   163,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,   194,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
     195,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    88,     0,    89,   160,
      90,    91,    14,    15,     0,     0,    93,     0,   165,    19,
      95,     0,   166,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   164,     0,    69,     0,     0,
      71,     0,     0,     0,    75,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,   195,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    89,     0,    90,    91,   247,     0,     0,
       0,    93,     0,   165,     0,    95,     0,   166,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   160,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,   251,
     252,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    88,
       0,    89,   160,    90,    91,    14,    15,     0,     0,    93,
       0,   165,    19,    95,     0,   166,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,     0,     0,
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
     247,     0,     0,     0,    93,     0,   165,   624,    95,     0,
     166,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     160,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   161,   162,     0,   163,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,     0,     0,     0,     0,   195,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    88,     0,    89,   160,    90,    91,    14,    15,
       0,     0,    93,     0,   165,    19,    95,     0,   166,     0,
       0,     0,   233,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   164,     0,    69,     0,     0,    71,     0,     0,     0,
      75,     0,     0,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    88,     0,    89,
     160,    90,    91,    14,    15,     0,     0,    93,     0,    94,
      19,    95,     0,   166,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   161,   162,     0,   163,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,   247,     0,
       0,     0,    93,     0,   165,     0,    95,     0,   166,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   160,     0,
       0,    14,   594,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,   595,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,     0,   596,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,   597,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,   598,
     599,    65,    66,    67,   600,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,   601,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    89,     0,    90,    91,   247,     0,     0,     0,
      93,     0,   165,     0,    95,     0,   905,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   160,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
      89,     0,    90,    91,   247,     0,     0,     0,    93,     0,
     165,     0,    95,     0,   905,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   160,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   890,    35,     0,    36,     0,     0,    38,     0,
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
      90,    91,   247,     0,     0,     0,    93,     0,   165,     0,
      95,     0,   166,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   160,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,     0,     0,
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
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    88,
       0,    89,   160,    90,    91,    14,   490,     0,     0,    93,
       0,    94,    19,    95,     0,   166,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
     491,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    88,     0,    89,   160,    90,    91,
      14,    15,     0,     0,    93,     0,   165,    19,    95,     0,
     166,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    88,
       0,    89,   160,    90,    91,    14,   594,     0,     0,   567,
       0,   165,    19,    95,     0,   166,     0,     0,     0,     0,
     595,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,   596,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,   597,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,   598,   599,    65,    66,    67,   600,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
     601,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    88,     0,    89,   160,    90,    91,
      14,    15,     0,     0,    93,     0,   165,    19,    95,     0,
     602,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    88,
       0,    89,   160,    90,    91,    14,    15,     0,     0,    93,
       0,   165,    19,    95,     0,   602,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     670,    35,     0,    36,     0,     0,    38,     0,     0,     0,
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
       0,    30,   161,   162,     0,   673,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    88,
       0,    89,   160,    90,    91,    14,    15,     0,     0,    93,
       0,   165,    19,    95,     0,   166,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     884,    35,     0,    36,     0,     0,    38,     0,     0,     0,
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
       0,    30,   161,   162,     0,   885,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    88,
       0,    89,   160,    90,    91,    14,    15,     0,     0,    93,
       0,   165,    19,    95,     0,   166,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     887,    35,     0,    36,     0,     0,    38,     0,     0,     0,
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
       0,    30,   161,   162,     0,   888,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    88,
       0,    89,   160,    90,    91,    14,    15,     0,     0,    93,
       0,   165,    19,    95,     0,   166,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     889,    35,     0,    36,     0,     0,    38,     0,     0,     0,
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
       0,    30,   161,   162,     0,   890,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    88,
       0,    89,   160,    90,    91,    14,   594,     0,     0,    93,
       0,   165,    19,    95,     0,   166,     0,     0,     0,     0,
     595,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,   596,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,   597,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,   598,   599,    65,    66,    67,   600,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
     601,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    88,     0,    89,   160,    90,    91,
      14,    15,     0,     0,    93,     0,   165,    19,    95,     0,
     972,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    88,
       0,    89,   160,    90,    91,    14,    15,     0,     0,    93,
       0,     0,    19,    95,     0,   972,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    88,     0,    89,   160,     0,    91,
      14,    15,     0,     0,    93,     0,   165,    19,    95,     0,
     166,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -385,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,     0,     0,     0,  -385,     0,     0,     0,
    -385,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     160,    89,     0,    14,    15,   247,     0,     0,     0,     0,
      19,   165,     0,    95,  -385,   905,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   161,   162,     0,   163,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
    -367,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,  -367,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
    -367,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,  -367,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,     0,
       0,     0,     0,     0,     0,    84,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   160,     0,     0,    14,    15,
       0,     0,     0,     0,    89,    19,     0,   356,     0,  -367,
    -508,     0,  -508,    26,  -367,     0,    95,     0,   972,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   164,     0,    69,     0,     0,    71,     0,     0,     0,
      75,     0,     0,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      84,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     160,     0,     0,    14,    15,     0,     0,     0,     0,    89,
      19,     0,     0,     0,     0,     0,     0,     0,    26,   165,
       0,    95,     0,   166,    30,   161,   162,     0,   163,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    84,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   160,     0,     0,    14,    15,
       0,     0,     0,     0,    89,    19,     0,     0,     0,     0,
       0,     0,     0,    26,   165,     0,    95,     0,   602,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   164,     0,    69,   933,     0,    71,     0,     0,     0,
      75,   760,     0,    77,     0,     0,   207,   208,   762,     0,
       0,     0,     0,   209,     0,     0,     0,     0,     0,     0,
       0,   210,     0,     0,     0,     0,     0,   764,     0,     0,
      84,     0,   212,     0,   213,     0,     0,   214,     0,   767,
       0,     0,     0,     0,     0,   215,     0,     0,     0,    89,
       0,     0,     0,     0,     0,   313,   216,     0,     0,     0,
       0,    95,   314,   166,     0,   217,     0,     0,     0,     0,
       0,     0,     0,   315,   218,   219,     0,   220,     0,   221,
       0,   934,     0,     0,   772,     0,     0,     0,   224,     0,
       0,   225,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   341,   342,   343,   344,   345,   346,     0,     0,
     349,   350,   351,   352,     0,   354,   355,   774,   775,   776,
     777,   316,     0,   778,     0,   317,     0,   779,   780,   781,
     782,   783,   784,   785,   786,   787,   788,   789,   790,     0,
     791,   313,     0,   792,   793,   794,   795,     0,   314,     0,
       0,     0,     0,     0,     0,   318,     0,     0,     0,   315,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   319,     0,   320,   321,   322,     0,   323,   324,   325,
       0,   326,   327,   328,   329,   330,     0,   331,   332,   333,
     334,     0,   335,   336,   337,     0,   313,   338,   339,   340,
       0,     0,     0,   314,     0,     0,   968,   316,     0,     0,
       0,   317,     0,     0,   315,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,     0,     0,     0,     0,     0,     0,   314,
       0,   318,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,   319,     0,   320,
     321,   322,   316,   323,   324,   325,   317,   326,   327,   328,
     329,   330,     0,   331,   332,   333,   334,     0,   335,   336,
     337,     0,     0,   338,   339,   340,     0,   313,     0,     0,
       0,     0,  1035,     0,   314,     0,   318,     0,   316,     0,
       0,     0,   317,     0,     0,   315,   699,     0,     0,     0,
       0,     0,   319,     0,   320,   321,   322,     0,   323,   324,
     325,     0,   326,   327,   328,   329,   330,     0,   331,   332,
     333,   334,   318,   335,   336,   337,     0,     0,   338,   339,
     340,     0,     0,     0,     0,   625,     0,     0,   319,     0,
     320,   321,   322,   316,   323,   324,   325,   317,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,   313,     0,   338,   339,   340,     0,     0,   314,
       0,   932,     0,   394,     0,     0,     0,   318,     0,     0,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,     0,     0,   338,
     339,   340,     0,   369,     0,     0,     0,     0,   316,     0,
       0,     0,   317,     0,     0,     0,     0,   313,     0,     0,
       0,   405,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
       0,     0,   318,   341,   342,   343,   344,   345,   346,     0,
       0,   349,   350,   351,   352,     0,   354,   355,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   316,   338,   339,   340,   317,   369,     0,
       0,     0,   313,     0,     0,     0,   819,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,   318,   341,   342,
     343,   344,   345,   346,     0,     0,   349,   350,   351,   352,
       0,   354,   355,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,     0,   316,   338,
     339,   340,   317,   369,     0,     0,     0,   313,     0,     0,
       0,   826,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
       0,     0,   318,   341,   342,   343,   344,   345,   346,     0,
       0,   349,   350,   351,   352,     0,   354,   355,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   316,   338,   339,   340,   317,   369,     0,
       0,     0,   313,     0,     0,     0,   945,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,   318,   341,   342,
     343,   344,   345,   346,     0,     0,   349,   350,   351,   352,
       0,   354,   355,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,     0,   316,   338,
     339,   340,   317,   369,     0,     0,     0,   313,     0,     0,
       0,   946,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
       0,     0,   318,   341,   342,   343,   344,   345,   346,     0,
       0,   349,   350,   351,   352,     0,   354,   355,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   316,   338,   339,   340,   317,   369,     0,
       0,     0,   313,     0,     0,     0,   947,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,   318,   341,   342,
     343,   344,   345,   346,     0,     0,   349,   350,   351,   352,
       0,   354,   355,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,     0,   316,   338,
     339,   340,   317,   369,     0,     0,     0,   313,     0,     0,
       0,   948,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
       0,     0,   318,   341,   342,   343,   344,   345,   346,     0,
       0,   349,   350,   351,   352,     0,   354,   355,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   316,   338,   339,   340,   317,   369,     0,
       0,     0,   313,     0,     0,     0,   970,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,   318,   341,   342,
     343,   344,   345,   346,     0,     0,   349,   350,   351,   352,
       0,   354,   355,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,     0,   316,   338,
     339,   340,   317,   369,     0,     0,     0,   313,     0,     0,
       0,   971,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,   397,     0,     0,     0,
       0,     0,   318,   341,   342,   343,   344,   345,   346,   398,
       0,   349,   350,   351,   352,     0,   354,   355,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   316,   338,   339,   340,   317,   369,     0,
       0,     0,   313,     0,     0,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,   391,     0,   394,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,   392,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,     0,   316,   338,
     339,   340,   317,   369,     0,     0,     0,   313,     0,     0,
       0,     0,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,   233,     0,     0,     0,
       0,     0,   318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   316,   338,   339,   340,   317,   369,     0,
       0,     0,   313,     0,     0,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,   691,     0,     0,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,     0,   316,   338,
     339,   340,   317,   369,     0,     0,     0,   313,     0,     0,
       0,     0,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
       0,     0,   318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   316,   338,   339,   340,   317,   369,     0,
       0,     0,   313,     0,     0,     0,   709,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,   315,   316,   338,
     339,   340,   317,   369,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,     0,     0,
       0,     0,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,   318,     0,     0,   315,     0,     0,     0,     0,
     402,     0,     0,     0,     0,   316,     0,     0,   319,   317,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,   403,   335,
     336,   337,   313,     0,   338,   339,   340,     0,   432,   314,
       0,     0,     0,   316,     0,     0,     0,   317,     0,     0,
     315,     0,     0,     0,     0,   319,     0,   320,   321,   322,
       0,   323,   324,   325,   371,   326,   327,   328,   329,     0,
       0,   331,   332,   333,   334,     0,   335,   318,   337,     0,
       0,   338,   339,   340,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,   316,   323,
     324,   325,   317,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,   313,   404,   338,
     339,   340,     0,     0,   314,     0,     0,     0,   155,     0,
       0,     0,   318,     0,     0,   315,     0,     0,     0,     0,
     445,     0,     0,     0,     0,     0,     0,     0,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,   446,   335,
     336,   337,   313,     0,   338,   339,   340,     0,     0,   314,
       0,     0,     0,   316,     0,     0,     0,   317,     0,     0,
     315,   534,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   535,     0,     0,     0,     0,     0,
       0,     0,   313,     0,     0,     0,     0,   318,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,   319,     0,   320,   321,   322,   316,   323,
     324,   325,   317,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,   313,     0,   338,
     339,   340,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,   318,     0,     0,   315,   536,     0,   316,     0,
       0,     0,   317,     0,     0,     0,     0,     0,   319,   537,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,   318,     0,   338,   339,   340,     0,     0,     0,
       0,     0,     0,   316,     0,     0,     0,   317,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,   313,   436,   338,   339,   340,   318,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,   313,     0,   338,
     339,   340,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,   316,     0,
       0,     0,   317,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,     0,     0,     0,     0,     0,     0,
     314,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   315,   318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   316,     0,     0,     0,   317,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   438,   338,   339,   340,   318,     0,   316,
       0,     0,     0,   317,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,   318,   335,   336,   337,     0,   440,   338,
     339,   340,     0,     0,     0,     0,     0,     0,     0,   319,
       0,   320,   321,   322,     0,   323,   324,   325,     0,   326,
     327,   328,   329,   330,     0,   331,   332,   333,   334,     0,
     335,   336,   337,   313,   448,   338,   339,   340,     0,     0,
     314,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   313,     0,     0,     0,     0,     0,
       0,   314,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   316,
       0,     0,     0,   317,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   313,
       0,     0,     0,     0,     0,     0,   314,     0,     0,     0,
     316,     0,   465,   318,   317,     0,     0,   315,     0,     0,
       0,     0,     0,   538,     0,     0,     0,     0,     0,   319,
       0,   320,   321,   322,     0,   323,   324,   325,     0,   326,
     327,   328,   329,   330,   318,   331,   332,   333,   334,     0,
     335,   336,   337,     0,     0,   338,   339,   340,     0,     0,
     319,     0,   320,   321,   322,   316,   323,   324,   325,   317,
     326,   327,   328,   329,   330,     0,   331,   332,   333,   334,
       0,   335,   336,   337,   313,     0,   338,   339,   340,     0,
       0,   314,     0,     0,     0,     0,     0,     0,     0,   318,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   319,     0,   320,   321,   322,
       0,   323,   324,   325,     0,   326,   327,   328,   329,   330,
    -480,   331,   332,   333,   334,     0,   335,   336,   337,   313,
       0,   338,   339,   340,     0,     0,   314,     0,     0,     0,
     316,     0,     0,     0,   317,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,   704,     0,     0,     0,     0,     0,   314,     0,     0,
     155,     0,     0,     0,   318,     0,     0,     0,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     319,     0,   320,   321,   322,   316,   323,   324,   325,   317,
     326,   327,   328,   329,   330,     0,   331,   332,   333,   334,
       0,   335,   336,   337,     0,   313,   338,   339,   340,     0,
       0,     0,   314,     0,     0,     0,   316,     0,     0,   318,
     317,     0,     0,   315,     0,     0,   707,     0,     0,     0,
       0,     0,     0,     0,     0,   319,     0,   320,   321,   322,
       0,   323,   324,   325,     0,   326,   327,   328,   329,   330,
     318,   331,   332,   333,   334,     0,   335,   336,   337,     0,
     680,   338,   339,   340,     0,     0,   319,     0,   320,   321,
     322,   316,   323,   324,   325,   317,   326,   327,   328,   329,
     330,     0,   331,   332,   333,   334,     0,   335,   336,   337,
     313,     0,   338,   339,   340,     0,     0,   314,     0,     0,
       0,     0,     0,     0,     0,   318,     0,     0,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   319,     0,   320,   321,   322,     0,   323,   324,   325,
       0,   326,   327,   328,   329,   330,     0,   331,   332,   333,
     334,     0,   335,   336,   337,   313,     0,   338,   339,   340,
       0,     0,   314,     0,     0,     0,   316,     0,     0,     0,
     317,     0,     0,   315,   809,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,     0,     0,
       0,     0,     0,   314,     0,     0,     0,     0,     0,   749,
     318,     0,     0,     0,   315,   812,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   319,     0,   320,   321,
     322,   316,   323,   324,   325,   317,   326,   327,   328,   329,
     330,     0,   331,   332,   333,   334,     0,   335,   336,   337,
       0,     0,   338,   339,   340,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,   318,   317,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   319,     0,   320,   321,   322,     0,   323,   324,   325,
       0,   326,   327,   328,   329,   330,   318,   331,   332,   333,
     334,     0,   335,   336,   337,     0,     0,   338,   339,   340,
       0,     0,   319,     0,   320,   321,   322,     0,   323,   324,
     325,     0,   326,   327,   328,   329,   330,     0,   331,   332,
     333,   334,     0,   335,   336,   337,   313,     0,   338,   339,
     340,     0,     0,   314,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   315,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,     0,     0,
       0,     0,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,     0,   317,     0,     0,     0,
       0,     0,     0,     0,     0,   883,     0,     0,     0,     0,
       0,     0,   313,   900,     0,     0,     0,     0,     0,   314,
       0,     0,     0,   316,     0,     0,   318,   317,     0,     0,
     315,     0,     0,     0,     0,     0,   886,     0,     0,     0,
       0,     0,   319,     0,   320,   321,   322,     0,   323,   324,
     325,     0,   326,   327,   328,   329,   330,   318,   331,   332,
     333,   334,     0,   335,   336,   337,     0,     0,   338,   339,
     340,     0,     0,   319,     0,   320,   321,   322,   316,   323,
     324,   325,   317,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,   313,     0,   338,
     339,   340,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,   318,     0,     0,   315,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,     0,   338,   339,   340,     0,     0,     0,
       0,     0,     0,   316,     0,     0,     0,   317,     0,     0,
       0,     0,   313,     0,     0,     0,   981,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,     0,   316,   338,
     339,   340,   317,     0,     0,     0,     0,   313,     0,     0,
       0,   982,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
       0,     0,   318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   316,   338,   339,   340,   317,     0,     0,
       0,     0,   313,     0,     0,     0,   983,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,     0,   316,   338,
     339,   340,   317,     0,     0,     0,     0,   313,     0,     0,
       0,   984,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
       0,     0,   318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   316,   338,   339,   340,   317,     0,     0,
       0,     0,   313,     0,     0,     0,   985,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,     0,   316,   338,
     339,   340,   317,     0,     0,     0,     0,   313,     0,     0,
       0,   986,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
       0,     0,   318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,  1008,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   316,   338,   339,   340,   317,     0,     0,
       0,     0,   313,     0,     0,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,   315,   331,
     332,   333,   334,     0,   335,   336,   337,     0,   316,   338,
     339,   340,   317,     0,     0,     0,     0,   313,     0,     0,
       0,     0,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
       0,     0,   318,     0,     0,     0,   316,     0,     0,     0,
     317,     0,     0,     0,     0,     0,     0,     0,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   668,   338,   339,   340,   317,     0,     0,
       0,     0,     0,     0,     0,     0,   319,     0,   320,   321,
     322,     0,   323,   324,   325,     0,     0,   327,   328,   329,
       0,     0,   331,   332,   333,   334,     0,   318,     0,   337,
       0,     0,   338,   339,   340,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,   315,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,    16,   335,   336,   337,    20,    21,   338,
     339,   340,     0,     0,     0,     0,    27,     0,    28,   866,
       0,     0,     0,    33,     0,     0,     0,     0,    37,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     316,     0,     0,     0,   317,     0,     0,   315,    48,     0,
      50,     0,    52,     0,   867,     0,     0,   868,     0,    57,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   318,     0,     0,     0,     0,     0,
      74,     0,     0,    76,     0,     0,    79,     0,     0,     0,
     319,     0,   320,   321,   322,   316,   323,   324,   325,   317,
     326,   327,   328,   329,     0,     0,   331,   332,   333,   334,
       0,   335,   336,   337,     0,     0,   338,   339,   340,     0,
       0,     0,     0,     0,     0,     0,     0,   206,     0,   318,
       0,     0,     0,     0,     0,    92,     0,     0,     0,   207,
     208,   944,     0,     0,     0,   319,   209,   320,   321,   322,
       0,   323,   324,   325,   210,   326,   327,   328,   329,     0,
     211,   331,   332,   333,   334,   212,   335,   213,   337,     0,
     214,   338,   339,   340,     0,     0,     0,     0,   215,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   216,
       0,     0,     0,     0,     0,     0,     0,     0,   217,     0,
       0,     0,     0,     0,     0,     0,     0,   218,   219,     0,
     220,     0,   221,     0,   222,     0,     0,   223,     0,     0,
       0,   224,     0,     0,   225
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-901)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,   140,    60,    20,    15,     2,   178,    18,    54,    55,
     362,   441,   560,    24,   128,   655,    28,    28,    29,   191,
      31,    32,    33,    34,   199,   725,   751,   413,   410,     1,
       1,    42,    17,    44,    45,   418,    47,    95,    49,    29,
      51,   593,    29,   839,   186,     1,    29,   915,    59,    60,
      61,    62,    63,    64,    29,    59,   205,    68,   441,    70,
      79,    72,    73,   205,    29,   179,    77,    78,     1,    80,
      81,    31,    32,    84,    78,    42,    87,    88,    89,   111,
      91,     3,     0,    94,    95,    96,   106,   151,   800,   801,
       3,    62,    96,     1,   243,   953,    52,   106,   998,   131,
     131,   243,    47,   135,   804,   106,   842,    42,   904,    42,
      42,    52,    42,   262,    77,   667,   106,   665,    42,   106,
     262,   296,   297,    79,    62,    42,    71,   159,   159,   403,
     269,    42,   103,     3,     3,  1035,    96,   156,    87,   111,
     998,   161,    77,   843,    52,   870,    19,  1015,   104,   160,
     161,   162,   163,   164,   165,   166,   131,    79,   130,   131,
     161,    30,   166,   104,   131,   103,   156,   892,    90,   156,
     722,    79,   446,   156,   130,   186,   193,    90,   111,   158,
      19,   156,   324,   200,    92,    54,   157,   159,   158,   201,
     201,   156,   177,   929,   205,   106,   104,   130,   131,   131,
     156,   131,    75,   903,    24,    24,   592,   131,   560,   358,
     359,   158,    24,   160,   131,   131,   358,   359,   151,   157,
     131,    90,   130,   935,   936,    24,    99,    96,   131,   161,
       7,   161,   243,   244,    52,    52,    75,   161,   131,   879,
     251,   252,   131,   151,   161,   161,    66,    66,   156,   158,
     161,   262,   952,    42,    66,   131,   159,    77,    77,   131,
      99,    79,    79,    79,   158,    77,   159,    66,   131,   111,
     159,   156,    95,   387,    92,    92,   824,    96,    77,   151,
     291,   157,   102,   102,    96,   434,   104,   104,    77,   347,
     348,   288,   434,   135,   151,   353,   159,    96,   356,   357,
     157,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   665,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     716,   131,   714,   151,   151,   151,   347,   348,   156,   156,
     156,   104,   353,    52,   131,   356,   357,   358,   359,   360,
     361,   362,   158,   131,   160,   151,   316,   317,  1013,   159,
     371,   157,   131,   186,    72,   376,    28,   378,   131,   751,
      79,   151,   159,   151,   131,   131,  1031,   151,   389,    28,
     391,   392,   205,    92,   401,   389,   397,   398,   157,   564,
     565,   402,   403,   131,   405,   104,   407,   455,   151,   457,
     360,   361,   159,   159,   131,   111,    42,   141,   550,   420,
     131,   371,   151,   151,    76,   151,   427,    76,    80,   849,
     243,    80,   151,   434,   151,   131,    62,    76,   131,   135,
     151,    80,   392,   151,   445,   446,    72,    73,   398,   262,
     131,   452,   151,    24,   455,   158,   457,   156,   151,   621,
     461,   462,   463,   159,   465,   466,   151,   632,   633,   842,
     151,    42,   473,   158,    45,   160,   849,   151,   130,   473,
     132,   133,   824,    76,   471,   137,   158,    80,   160,   490,
     491,   130,    63,   145,   133,    66,   133,    68,   137,   156,
     548,   130,   154,   875,   133,   151,    77,   455,   137,   457,
     105,   461,    72,    73,    42,   158,   158,   151,   158,   111,
     892,   151,     7,    56,    52,    96,   655,   130,   156,   151,
     111,   131,   159,   534,   535,   536,   537,   538,   539,   540,
     151,    42,   158,    31,   159,   358,   359,   548,   131,   550,
     176,    79,   159,   554,     2,   556,   929,   111,   606,   560,
     131,   562,   159,    11,    18,   159,   541,   542,   131,   159,
     196,   197,   198,   199,   161,    23,   104,    25,   204,   161,
     953,   156,   151,   111,   151,   535,   151,   537,   159,    39,
     151,   161,     3,   594,    24,   968,   597,   598,   599,   600,
     601,   602,   151,   131,    15,   606,   151,   131,   602,   151,
     236,    22,    42,   239,   427,    45,   151,    31,   159,   157,
      68,   434,   159,   151,    74,   998,   151,   131,   156,   111,
      41,   159,   258,    63,   151,    46,    66,   959,    68,   452,
    1013,   157,   455,    54,   457,   111,    94,    77,    77,   462,
      98,    29,   278,    29,   136,   161,   138,   139,  1031,   156,
     142,   143,  1035,    74,   665,    31,    96,   668,   669,   670,
     130,   121,   673,   123,    77,    86,   157,   161,   130,    90,
    1002,  1003,  1004,  1005,  1006,  1007,    97,   161,   689,   161,
     691,   158,   133,   133,   133,   696,   133,   133,   699,   133,
     133,   702,    62,   704,    31,   133,   707,   133,   156,    19,
     133,   133,   159,    23,    24,   159,   161,    92,   668,   669,
     445,   131,    32,   302,    34,    35,   157,    22,   178,    39,
     446,   732,   736,   192,    44,   548,   270,   550,   732,   817,
     552,   191,   754,   744,   194,   746,   372,   875,   749,   879,
     879,   714,   753,  1015,    64,   585,    66,   757,    68,   952,
      70,   977,    24,    73,  1008,    75,  1001,    77,    33,   550,
     396,   421,    -1,    -1,   400,    -1,    -1,    -1,   226,   229,
      42,    -1,    -1,    45,    -1,   233,    96,    -1,    -1,    99,
      -1,    -1,   102,    -1,    -1,   796,    -1,   247,    28,    -1,
      -1,    63,    -1,    -1,    66,    -1,    68,   808,   809,    -1,
     811,   812,   813,   814,    -1,    77,    -1,    -1,   819,   820,
      -1,   822,    -1,   824,    -1,   826,   827,    -1,    -1,   830,
      -1,   867,   868,    -1,    96,    -1,   196,   197,   198,   199,
     288,   151,    -1,    -1,    -1,    -1,    76,   848,   849,    28,
      80,    23,    24,   854,   302,   856,    -1,    -1,    -1,   860,
      -1,    -1,    34,    35,    -1,   866,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    -1,   236,    -1,    -1,   239,
      -1,    -1,   883,   884,   885,   886,   887,   888,   889,   890,
      -1,    -1,    64,    -1,    66,    -1,   126,    76,   258,   900,
     130,    80,   132,   133,   905,    77,    -1,   137,    -1,   910,
      -1,   905,    -1,   914,    -1,   145,    -1,    -1,   278,    -1,
      -1,   369,   152,   153,   154,    -1,    -1,    -1,    -1,    -1,
     102,    -1,   558,   559,    -1,    -1,   386,    -1,   564,   565,
      -1,    -1,    -1,   391,   945,   946,   947,   948,    -1,   397,
     576,   130,    -1,   132,   133,    -1,    -1,   405,   137,    -1,
      -1,    -1,    -1,   413,    -1,    -1,   145,   968,    -1,   970,
     971,   972,    -1,   152,   153,   154,    -1,    -1,   972,    -1,
     981,   982,   983,   984,   985,   986,    -1,    -1,    -1,    -1,
      -1,   617,    -1,     2,    28,    -1,   997,   998,    -1,  1000,
      -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   459,   372,    -1,    23,   463,    25,    42,    -1,    -1,
      -1,    -1,   472,   471,    -1,   475,    -1,    52,    -1,   842,
    1031,    -1,    -1,    -1,  1035,    -1,   396,    -1,    -1,    -1,
     400,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,   678,   679,    79,    -1,    -1,     2,    -1,    68,
     686,    -1,   688,    72,    73,    -1,    11,    -1,    -1,   695,
      -1,    -1,   698,    -1,    -1,   701,    -1,    28,    23,   104,
      25,    -1,    -1,   531,     2,    94,   111,    -1,    -1,    98,
      -1,    -1,   126,    11,   128,   129,   130,   910,   132,   133,
     134,    -1,    -1,   137,    -1,    23,   131,    25,    -1,    -1,
     144,   145,    -1,   739,    -1,   149,    -1,    -1,   152,   153,
     154,    -1,    -1,    68,    -1,    76,   151,   575,    -1,    80,
      -1,   156,    -1,    -1,   159,    -1,    -1,    -1,   588,    -1,
     953,    -1,   592,    -1,    -1,    -1,    -1,   156,    -1,    94,
      68,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   617,    -1,    -1,
      -1,   621,    -1,    -1,    -1,   126,    94,   128,   129,   130,
      98,   132,   133,   631,    -1,   998,   137,    -1,    -1,   637,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,   558,   559,
      -1,   152,   153,   154,   564,   565,    -1,   655,    -1,    -1,
      -1,   156,    -1,    42,    -1,    -1,    -1,   226,    -1,    -1,
     668,   669,  1035,    52,   233,    -1,   852,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   689,    -1,   691,    -1,    -1,    -1,    -1,   696,    -1,
      79,   699,    -1,    -1,    -1,    -1,    -1,    -1,   706,    -1,
      -1,   709,    -1,   713,    -1,    -1,   716,    -1,   894,   895,
      -1,    -1,   898,   899,    -1,   104,    -1,    -1,    -1,   288,
      -1,   226,   111,   731,    -1,    -1,   736,    -1,   233,   915,
      -1,    -1,    -1,   302,    -1,   921,   744,   923,   746,   925,
      -1,    -1,   131,    -1,   754,    -1,    -1,    -1,   226,    -1,
      -1,    -1,    -1,    -1,    -1,   233,    -1,    -1,   678,   679,
      -1,    -1,   151,     3,    -1,   775,   686,   156,    -1,    -1,
     159,    -1,    -1,    -1,    -1,    15,    16,    -1,   698,    28,
      -1,   701,    22,   288,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,   302,    -1,    -1,
     369,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
     288,   819,    -1,    -1,    54,    -1,    -1,    -1,   826,    -1,
      -1,    -1,   391,    -1,   302,    65,    -1,    76,   397,  1015,
     838,    80,    -1,    -1,    74,    -1,   405,    -1,    -1,    -1,
      -1,    -1,   850,    83,    84,    -1,    86,    -1,    88,     2,
      90,   859,    -1,    93,    -1,   863,    -1,    97,    11,    -1,
     100,    -1,    -1,    -1,   369,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    25,    -1,    -1,    -1,    -1,   126,    -1,    -1,
     129,   130,    -1,   132,   133,    -1,   391,    -1,   137,    -1,
     459,   369,   397,   901,   463,    -1,   145,    -1,    -1,    -1,
     405,    -1,   471,   152,   153,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   391,    -1,    68,   156,    -1,   158,   397,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   405,    -1,    -1,
      -1,    -1,   852,    -1,    -1,    -1,    -1,   945,   946,   947,
     948,    94,    -1,    -1,    -1,    98,    -1,    -1,     2,    -1,
      -1,    -1,    -1,    -1,   459,    -1,     1,    11,   463,    -1,
      -1,    -1,   970,   971,    -1,    -1,   471,    -1,    -1,    23,
      -1,    25,    -1,    -1,    19,     2,    -1,    -1,    23,    24,
      -1,   459,    -1,    -1,    11,   463,    -1,    32,    -1,    34,
      35,    -1,    -1,   471,    39,    -1,    23,    -1,    25,    44,
      -1,    -1,    47,   156,    -1,    -1,   575,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    64,
      -1,    66,    -1,    68,    -1,    70,    71,    -1,    73,    -1,
      75,    -1,    77,    -1,    23,    24,    -1,    -1,    -1,    -1,
      94,    68,    -1,    -1,    98,    34,    35,    -1,    -1,    -1,
      -1,    96,    -1,    -1,    99,    44,    -1,   102,    47,    -1,
      -1,    -1,   631,    -1,    -1,    -1,    -1,    94,   637,    -1,
     575,    98,    -1,   226,    -1,    64,    -1,    66,    -1,    -1,
     233,    -1,    71,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   575,    -1,    -1,
      -1,    -1,   156,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,   157,   102,    -1,    -1,    -1,    -1,    -1,    -1,
     689,    -1,   691,    -1,    -1,    -1,   631,   696,    -1,   156,
     699,    -1,   637,    -1,    -1,   288,    -1,   706,    -1,    -1,
     709,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   302,
      -1,    -1,    -1,   631,    -1,    -1,    -1,    -1,    -1,   637,
      -1,    -1,   731,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,    -1,    -1,   744,    -1,   746,    -1,   233,
      -1,    -1,    -1,    -1,   689,    -1,   691,    -1,    -1,    -1,
      -1,   696,    -1,    -1,   699,    -1,    -1,    -1,    -1,   226,
      -1,   706,    -1,    28,   709,    -1,   233,    -1,    -1,    -1,
      -1,   689,    -1,   691,    -1,    -1,   369,    -1,   696,    -1,
      -1,   699,    -1,    -1,    -1,    -1,   731,    -1,   706,    -1,
      -1,   709,    -1,    -1,   288,    -1,    -1,    -1,   391,   744,
      -1,   746,    -1,    -1,   397,    -1,    -1,    -1,   302,    -1,
     819,    76,   405,   731,    -1,    80,    -1,   826,    -1,    -1,
      -1,   288,    -1,    -1,    -1,    -1,   744,    -1,   746,   838,
      -1,    -1,    -1,    -1,    -1,   302,    -1,    -1,    -1,    -1,
      -1,   850,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     859,    -1,    -1,    -1,   863,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,   128,   129,   130,   459,   132,   133,    -1,
     463,    -1,   137,    -1,   819,   369,    -1,    -1,   471,   144,
     145,   826,    -1,    -1,   149,    -1,    -1,   152,   153,   154,
      -1,    -1,   901,   838,    -1,    -1,    -1,   391,    -1,    -1,
      -1,   819,   369,   397,    -1,   850,    -1,    -1,   826,    -1,
      -1,   405,    -1,    -1,   859,    -1,    -1,    -1,   863,    -1,
     838,    -1,    -1,    -1,   391,     2,    -1,    -1,    -1,    -1,
     397,    -1,   850,    -1,    11,    -1,   945,   946,   947,   948,
      -1,   859,    -1,    -1,    -1,   863,    23,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   901,    -1,    -1,    -1,
      -1,   970,   971,    -1,    -1,   459,    -1,    -1,    -1,   463,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   471,    -1,    -1,
      -1,    -1,   575,   901,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    68,   459,    -1,    -1,    -1,   463,    -1,    -1,    -1,
     945,   946,   947,   948,   471,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    98,    -1,     1,    -1,   970,   971,   945,   946,   947,
     948,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   631,    -1,
      -1,    19,    -1,    -1,   637,    23,    24,    -1,    -1,    -1,
      -1,    -1,   970,   971,    32,    -1,    34,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,   575,    -1,    -1,    -1,    -1,    64,    -1,    66,    -1,
      68,    -1,    70,    71,    -1,    73,   689,    75,   691,    77,
      -1,    -1,     2,   696,    -1,    -1,   699,    -1,   575,    -1,
      -1,    11,    -1,   706,    -1,    -1,   709,    -1,    96,    -1,
      -1,    99,    -1,    23,   102,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   631,   731,    -1,
      -1,    -1,    -1,   637,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   744,    -1,   746,    -1,    -1,   233,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   631,    -1,    -1,    -1,    68,    -1,
     637,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,   689,    -1,   691,    98,    -1,
      -1,    -1,   696,    -1,    -1,   699,    -1,    -1,    -1,    -1,
      -1,   288,   706,    -1,    -1,   709,    -1,    -1,    -1,    -1,
      -1,    -1,   689,    -1,   691,   302,   819,    -1,    -1,   696,
      -1,    -1,   699,   826,    -1,    -1,    -1,   731,    -1,   706,
      -1,    -1,   709,    -1,    -1,   838,    -1,    -1,    -1,    -1,
     744,    -1,   746,    -1,    -1,    -1,   156,   850,    -1,    -1,
      -1,    -1,    -1,    -1,   731,    -1,   859,    -1,    -1,    -1,
     863,    -1,    -1,    -1,    -1,    -1,    -1,   744,    -1,   746,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   369,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   901,    -1,
      -1,    -1,    -1,    -1,   391,    -1,    -1,    -1,    -1,    -1,
     397,    -1,    -1,   141,    -1,   819,   226,    -1,   405,    -1,
      -1,    -1,   826,   233,    -1,    -1,    -1,   155,    -1,    -1,
     158,    -1,   160,    -1,   838,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   945,   946,   947,   948,   850,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   859,    -1,    -1,    -1,   863,
      -1,   838,    -1,    -1,    -1,    -1,    -1,   970,   971,    -1,
      -1,    -1,   459,   850,    -1,     3,   463,    -1,   288,    -1,
      -1,    -1,   859,    -1,   471,    -1,   863,    15,    16,    -1,
      -1,    -1,   302,    -1,    22,    -1,    -1,   901,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,   901,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      -1,   945,   946,   947,   948,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    84,    -1,    86,   369,
      88,    -1,    90,    -1,    -1,    93,   970,   971,    -1,    97,
      98,    -1,   100,    -1,    -1,    -1,    -1,     3,    -1,    -1,
      -1,   391,    -1,    -1,    -1,    -1,    -1,   397,   575,    15,
      16,    -1,    -1,    -1,    -1,   405,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    -1,   631,    -1,    -1,    -1,    74,   459,
     637,    -1,    -1,   463,    -1,    -1,     3,    83,    84,    -1,
      86,   471,    88,    -1,    90,    -1,    -1,    93,    15,    16,
      -1,    97,    -1,    -1,   100,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,   689,    -1,   691,    -1,    -1,    54,    -1,   696,
      -1,    -1,   699,    -1,    -1,    -1,    -1,    -1,    65,   706,
      -1,    -1,   709,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    -1,   158,    -1,    -1,    -1,    83,    84,    -1,    86,
      -1,    88,    -1,    90,   731,    -1,    93,    -1,    -1,    -1,
      97,    -1,    -1,   100,    -1,    -1,    -1,   744,    -1,   746,
      -1,    -1,    -1,    -1,    -1,   575,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    23,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      34,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,   631,    66,    -1,    68,    -1,    70,   637,    -1,    73,
      -1,    75,   819,    77,    -1,    -1,    -1,    -1,    -1,   826,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   838,    96,    -1,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,    -1,   850,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   859,    -1,    -1,    -1,   863,    -1,    -1,   689,
       1,   691,     3,    -1,    -1,    -1,   696,    -1,    -1,   699,
      -1,    -1,    -1,    -1,    15,    16,   706,    -1,    -1,   709,
      -1,    22,    -1,    -1,    -1,    -1,    -1,   151,    -1,    30,
      -1,    -1,    -1,   157,   901,    36,    -1,    -1,    -1,    -1,
      41,   731,    43,    -1,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,   744,    -1,   746,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    65,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    28,    -1,   945,   946,
     947,   948,    83,    84,    -1,    86,    -1,    88,    -1,    90,
      -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    10,   100,
      -1,    -1,    -1,   970,   971,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,   819,
      -1,    -1,    -1,    -1,    -1,    -1,   826,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   838,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
     850,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,   859,
      -1,    -1,    -1,   863,   126,    -1,   128,   129,   130,    -1,
     132,   133,   134,    -1,   136,   137,   138,   139,   140,    -1,
     142,   143,   144,   145,    -1,   147,   148,   149,   110,    -1,
     152,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,   901,    -1,    -1,   126,    -1,   128,   129,   130,    -1,
     132,   133,   134,    -1,   136,   137,   138,   139,   140,    -1,
     142,   143,   144,   145,    -1,   147,   148,   149,    -1,    -1,
     152,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,    -1,    -1,    -1,    -1,   945,   946,   947,   948,    -1,
      -1,    -1,    -1,     0,     1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
     970,   971,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    -1,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    55,    56,
      57,    58,    -1,    60,    -1,    -1,    -1,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    -1,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    -1,    93,    94,    95,    96,
      97,    -1,    99,   100,   101,   102,    -1,    -1,   105,    -1,
     107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,   146,
      -1,   148,   149,    -1,   151,    -1,    -1,   154,    -1,   156,
      -1,   158,     1,   160,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,
      69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    84,    85,    86,    87,    88,
      -1,    90,    -1,    -1,    93,    -1,    -1,    -1,    97,    -1,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,    -1,   146,    -1,   148,
     149,   150,    -1,    -1,    -1,   154,    -1,   156,    -1,   158,
      -1,   160,   161,     1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    12,    13,    14,    15,    16,    17,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    57,
      58,    -1,    60,    -1,    -1,    -1,    64,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    -1,    93,    94,    95,    96,    97,
      -1,    99,   100,   101,   102,    -1,    -1,   105,    -1,   107,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,    -1,   156,   157,
     158,     1,   160,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    -1,
      60,    -1,    -1,    -1,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    -1,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    -1,    93,    94,    95,    96,    97,    -1,    99,
     100,   101,   102,    -1,    -1,   105,    -1,   107,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,    -1,   146,    -1,   148,   149,
     150,   151,    -1,    -1,   154,    -1,   156,   157,   158,     1,
     160,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    -1,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    -1,    60,    -1,
      -1,    -1,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    -1,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      -1,    93,    94,    95,    96,    97,    -1,    99,   100,   101,
     102,    -1,    -1,   105,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,    -1,   146,    -1,   148,   149,    -1,   151,
      -1,    -1,   154,    -1,   156,   157,   158,     1,   160,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    57,    58,    -1,    60,    -1,    -1,    -1,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    -1,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    -1,    93,
      94,    95,    96,    97,    -1,    99,   100,   101,   102,    -1,
      -1,   105,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,    -1,   146,    -1,   148,   149,    -1,   151,    -1,    -1,
     154,    -1,   156,   157,   158,     1,   160,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    -1,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    57,    58,    -1,    60,    -1,    -1,    -1,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      -1,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    -1,    93,    94,    95,
      96,    97,    -1,    99,   100,   101,   102,    -1,    -1,   105,
      -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,
     146,    -1,   148,   149,    -1,   151,    -1,    -1,   154,    -1,
     156,   157,   158,     1,   160,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    57,
      58,    -1,    60,    -1,    -1,    -1,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    -1,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    -1,    93,    94,    95,    96,    97,
      -1,    99,   100,   101,   102,    -1,    -1,   105,    -1,   107,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,   146,    -1,
     148,   149,    -1,   151,    -1,    -1,   154,    -1,   156,   157,
     158,     1,   160,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    -1,
      60,    -1,    -1,    -1,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    -1,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    -1,    93,    94,    95,    96,    97,    -1,    99,
     100,   101,   102,    -1,    -1,   105,    -1,   107,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,    -1,   146,    -1,   148,   149,
      -1,   151,    -1,    -1,   154,    -1,   156,    -1,   158,     1,
     160,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    -1,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    -1,    60,    -1,
      -1,    -1,    64,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    -1,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      -1,    93,    94,    95,    96,    97,    -1,    99,   100,   101,
     102,    -1,    -1,   105,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,    -1,   146,    -1,   148,   149,    -1,   151,
      -1,    -1,   154,    -1,   156,    -1,   158,     1,   160,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    57,    58,    -1,    60,    -1,    -1,    -1,
      64,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    -1,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    -1,    93,
      94,    95,    96,    97,    -1,    99,   100,   101,   102,    -1,
      -1,   105,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,    -1,   146,    -1,   148,   149,    -1,   151,    -1,    -1,
     154,    -1,   156,    -1,   158,     1,   160,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    -1,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    57,    58,    -1,    60,    -1,    -1,    -1,    64,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      -1,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    -1,    93,    94,    95,
      96,    97,    -1,    99,   100,   101,   102,    -1,    -1,   105,
      -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,
     146,    -1,   148,   149,    -1,   151,    -1,    -1,   154,    -1,
     156,    -1,   158,     1,   160,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    57,
      58,    -1,    60,    -1,    -1,    -1,    64,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    -1,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    -1,    93,    94,    95,    96,    97,
      -1,    99,   100,   101,   102,    -1,    -1,   105,    -1,   107,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,   146,    -1,
     148,   149,    -1,   151,    -1,    -1,   154,    -1,   156,    -1,
     158,     1,   160,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    -1,
      60,    -1,    -1,    -1,    64,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    -1,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    -1,    93,    94,    95,    96,    97,    -1,    99,
     100,   101,   102,    -1,    -1,   105,    -1,   107,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,    -1,   146,    -1,   148,   149,
      -1,   151,    -1,    -1,   154,    -1,   156,    -1,   158,     1,
     160,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    -1,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    -1,    60,    -1,
      -1,    -1,    64,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    -1,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      -1,    93,    94,    95,    96,    97,    -1,    99,   100,   101,
     102,    -1,    -1,   105,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,    -1,   146,    -1,   148,   149,    -1,   151,
      -1,    -1,   154,    -1,   156,    -1,   158,     1,   160,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    57,    58,    -1,    60,    -1,    -1,    -1,
      64,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    -1,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    -1,    93,
      94,    95,    96,    97,    -1,    99,   100,   101,   102,    -1,
      -1,   105,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,    -1,   146,    -1,   148,   149,    -1,   151,    -1,    -1,
     154,    -1,   156,    -1,   158,     1,   160,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    -1,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    57,    58,    -1,    60,    -1,    -1,    -1,    64,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      -1,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    -1,    93,    94,    95,
      96,    97,    -1,    99,   100,   101,   102,    -1,    -1,   105,
      -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,
     146,    -1,   148,   149,    -1,   151,    -1,    -1,   154,    -1,
     156,    -1,   158,     1,   160,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    57,
      58,    -1,    60,    -1,    -1,    -1,    64,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    -1,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    -1,    93,    94,    95,    96,    97,
      -1,    99,   100,   101,   102,    -1,    -1,   105,    -1,   107,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,   146,    -1,
     148,   149,    -1,   151,    -1,    -1,   154,    -1,   156,    -1,
     158,     1,   160,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    -1,
      60,    -1,    -1,    -1,    64,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    -1,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    -1,    93,    94,    95,    96,    97,    -1,    99,
     100,   101,   102,    -1,    -1,   105,    -1,   107,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,    -1,   146,    -1,   148,   149,
      -1,   151,    -1,    -1,   154,    -1,   156,    -1,   158,     1,
     160,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    84,    85,    86,    87,    88,    -1,    90,    -1,
      -1,    93,    -1,    -1,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,    -1,   146,    -1,   148,   149,   150,    -1,
      -1,    -1,   154,    -1,   156,    -1,   158,    -1,   160,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      84,    85,    86,    87,    88,    -1,    90,    -1,    -1,    93,
      -1,    -1,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,    -1,   146,    -1,   148,   149,   150,    -1,    -1,    -1,
     154,    -1,   156,    -1,   158,    -1,   160,   161,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,    88,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
      -1,   146,    -1,   148,   149,   150,    -1,    -1,    -1,   154,
      -1,   156,    -1,   158,    -1,   160,   161,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,    85,
      86,    87,    88,    -1,    90,    -1,    -1,    93,    -1,    -1,
      -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,
     126,   127,   128,   129,    -1,    10,    -1,    -1,   134,    -1,
      15,    16,    17,    -1,    -1,    -1,    -1,    22,   144,    -1,
     146,    -1,   148,   149,    -1,    30,    -1,    -1,   154,    -1,
     156,    36,   158,    -1,   160,   161,    41,    -1,    43,    -1,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      -1,    86,    -1,    88,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,
     115,   116,    -1,    -1,   119,   120,   121,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,    -1,   132,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,    -1,   149,    -1,    -1,   152,   153,   154,
     155,    -1,    -1,   158,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    35,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,    -1,
      69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    83,    84,    85,    86,    87,    88,
      -1,    90,    -1,    -1,    93,    -1,    -1,    -1,    97,    -1,
      -1,   100,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,    -1,   146,    -1,   148,
     149,    -1,    -1,    -1,    -1,   154,    -1,   156,    -1,   158,
      -1,   160,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    19,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,
      -1,    -1,    -1,    74,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    84,    85,    86,    87,    88,    -1,    90,
      -1,    -1,    93,    -1,    -1,    96,    97,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,   144,    -1,   146,    12,   148,   149,    15,
      16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,   160,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,    85,
      86,    87,    88,    -1,    90,    -1,    -1,    93,    94,    95,
      -1,    97,    98,    -1,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,
     146,    -1,   148,   149,   150,    -1,    -1,    -1,   154,    -1,
     156,    -1,   158,    -1,   160,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    67,
      -1,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    84,    85,    86,    87,
      88,    -1,    90,    -1,    -1,    93,    94,    95,    -1,    97,
      98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,   146,    -1,
     148,   149,   150,    -1,    -1,    -1,   154,    -1,   156,    -1,
     158,   159,   160,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    84,    85,    86,    87,    88,    -1,
      90,    -1,    -1,    93,    94,    95,    -1,    97,    98,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,    -1,   146,    -1,   148,   149,
     150,    -1,    -1,    -1,   154,    -1,   156,    -1,   158,   159,
     160,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      42,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    83,    84,    85,    86,    87,    88,    -1,    90,    -1,
      -1,    93,    -1,    -1,    -1,    97,    -1,    -1,   100,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,    -1,   146,    -1,   148,   149,    -1,    -1,
      -1,    -1,   154,    -1,   156,    -1,   158,    -1,   160,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    19,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,
      74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      84,    85,    86,    87,    88,    -1,    90,    -1,    -1,    93,
      -1,    -1,    96,    97,    -1,    99,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     144,    -1,   146,    12,   148,   149,    15,    16,    -1,    -1,
     154,    -1,   156,    22,   158,    -1,   160,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,
      69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    84,    85,    86,    87,    88,
      -1,    90,    -1,    -1,    93,    94,    95,    -1,    97,    -1,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,    -1,   146,    -1,   148,
     149,    -1,   151,    -1,    -1,   154,    -1,   156,    -1,   158,
      -1,   160,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    84,    85,    86,    87,    88,    -1,    90,
      -1,    -1,    93,    94,    95,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,    -1,   146,    -1,   148,   149,   150,
      -1,    -1,    -1,   154,    -1,   156,    -1,   158,    -1,   160,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    59,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    67,    -1,    69,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    84,    85,    86,    87,    88,    -1,    90,    -1,    -1,
      93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,   144,    -1,   146,    12,   148,   149,    15,    16,    -1,
      -1,   154,    -1,   156,    22,   158,    -1,   160,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,
      -1,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    84,    85,    86,    87,
      88,    -1,    90,    -1,    -1,    93,    -1,    -1,    -1,    97,
      -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   144,    -1,   146,    12,
     148,   149,    15,    16,    -1,    -1,   154,    -1,   156,    22,
     158,    -1,   160,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    67,    -1,    69,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    84,    85,    86,    87,    88,    -1,    90,    -1,    -1,
      93,    -1,    -1,    -1,    97,    -1,    -1,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,    -1,   146,    -1,   148,   149,   150,    -1,    -1,
      -1,   154,    -1,   156,    -1,   158,    -1,   160,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,    88,    -1,    90,    -1,    -1,    93,    94,
      95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   144,
      -1,   146,    12,   148,   149,    15,    16,    -1,    -1,   154,
      -1,   156,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    84,    85,    86,    87,    88,    -1,
      90,    -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,    -1,   146,    -1,   148,   149,
     150,    -1,    -1,    -1,   154,    -1,   156,   157,   158,    -1,
     160,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    84,    85,    86,    87,    88,    -1,    90,    -1,
      -1,    93,    -1,    -1,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   144,    -1,   146,    12,   148,   149,    15,    16,
      -1,    -1,   154,    -1,   156,    22,   158,    -1,   160,    -1,
      -1,    -1,    29,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      67,    -1,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    84,    85,    86,
      87,    88,    -1,    90,    -1,    -1,    93,    -1,    -1,    -1,
      97,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,   144,    -1,   146,
      12,   148,   149,    15,    16,    -1,    -1,   154,    -1,   156,
      22,   158,    -1,   160,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    84,    85,    86,    87,    88,    -1,    90,    -1,
      -1,    93,    -1,    -1,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,    -1,   146,    -1,   148,   149,   150,    -1,
      -1,    -1,   154,    -1,   156,    -1,   158,    -1,   160,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      84,    85,    86,    87,    88,    -1,    90,    -1,    -1,    93,
      -1,    -1,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,    -1,   146,    -1,   148,   149,   150,    -1,    -1,    -1,
     154,    -1,   156,    -1,   158,    -1,   160,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,    85,
      86,    87,    88,    -1,    90,    -1,    -1,    93,    -1,    -1,
      -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,
     146,    -1,   148,   149,   150,    -1,    -1,    -1,   154,    -1,
     156,    -1,   158,    -1,   160,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    67,
      -1,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    84,    85,    86,    87,
      88,    -1,    90,    -1,    -1,    93,    -1,    -1,    -1,    97,
      -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,   146,    -1,
     148,   149,   150,    -1,    -1,    -1,   154,    -1,   156,    -1,
     158,    -1,   160,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    84,    85,    86,    87,    88,    -1,
      90,    -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   144,    -1,   146,    12,   148,   149,
      15,    16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,
     160,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,    88,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   144,
      -1,   146,    12,   148,   149,    15,    16,    -1,    -1,   154,
      -1,   156,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    84,    85,    86,    87,    88,    -1,
      90,    -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   144,    -1,   146,    12,   148,   149,
      15,    16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,
     160,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,    88,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   144,
      -1,   146,    12,   148,   149,    15,    16,    -1,    -1,   154,
      -1,   156,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    84,    85,    86,    87,    88,    -1,
      90,    -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   144,    -1,   146,    12,   148,   149,
      15,    16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,
     160,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,    88,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   144,
      -1,   146,    12,   148,   149,    15,    16,    -1,    -1,   154,
      -1,   156,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    84,    85,    86,    87,    88,    -1,
      90,    -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   144,    -1,   146,    12,   148,   149,
      15,    16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,
     160,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,    88,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   144,
      -1,   146,    12,   148,   149,    15,    16,    -1,    -1,   154,
      -1,   156,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    84,    85,    86,    87,    88,    -1,
      90,    -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   144,    -1,   146,    12,   148,   149,
      15,    16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,
     160,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,    88,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   144,
      -1,   146,    12,   148,   149,    15,    16,    -1,    -1,   154,
      -1,   156,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    84,    85,    86,    87,    88,    -1,
      90,    -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   144,    -1,   146,    12,   148,   149,
      15,    16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,
     160,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,    88,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   144,
      -1,   146,    12,   148,   149,    15,    16,    -1,    -1,   154,
      -1,   156,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    84,    85,    86,    87,    88,    -1,
      90,    -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   144,    -1,   146,    12,   148,   149,
      15,    16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,
     160,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,    88,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   144,
      -1,   146,    12,   148,   149,    15,    16,    -1,    -1,   154,
      -1,   156,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    84,    85,    86,    87,    88,    -1,
      90,    -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   144,    -1,   146,    12,   148,   149,
      15,    16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,
     160,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,    88,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   144,
      -1,   146,    12,   148,   149,    15,    16,    -1,    -1,   154,
      -1,    -1,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    84,    85,    86,    87,    88,    -1,
      90,    -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   144,    -1,   146,    12,    -1,   149,
      15,    16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,
     160,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,    88,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,   131,    -1,    -1,    -1,
     135,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,   146,    -1,    15,    16,   150,    -1,    -1,    -1,    -1,
      22,   156,    -1,   158,   159,   160,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      52,    -1,    54,    55,    -1,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    83,    84,    85,    86,    87,    88,    -1,    90,    -1,
      92,    93,    -1,    -1,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,   104,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,   146,    22,    -1,   155,    -1,   151,
     158,    -1,   160,    30,   156,    -1,   158,    -1,   160,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      67,    -1,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    84,    85,    86,
      87,    88,    -1,    90,    -1,    -1,    93,    -1,    -1,    -1,
      97,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,   146,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,   156,
      -1,   158,    -1,   160,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    84,    85,    86,    87,    88,    -1,    90,    -1,
      -1,    93,    -1,    -1,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,   146,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,   156,    -1,   158,    -1,   160,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      67,    -1,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    84,    85,    86,
      87,    88,    -1,    90,     3,    -1,    93,    -1,    -1,    -1,
      97,    10,    -1,   100,    -1,    -1,    15,    16,    17,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,
     127,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    10,    65,    -1,    -1,    -1,
      -1,   158,    17,   160,    -1,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    83,    84,    -1,    86,    -1,    88,
      -1,    90,    -1,    -1,    93,    -1,    -1,    -1,    97,    -1,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,   115,   116,    -1,    -1,
     119,   120,   121,   122,    -1,   124,   125,   126,   127,   128,
     129,    76,    -1,   132,    -1,    80,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,    -1,
     149,    10,    -1,   152,   153,   154,   155,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,   128,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,   139,   140,    -1,   142,   143,   144,
     145,    -1,   147,   148,   149,    -1,    10,   152,   153,   154,
      -1,    -1,    -1,    17,    -1,    -1,   161,    76,    -1,    -1,
      -1,    80,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,   128,
     129,   130,    76,   132,   133,   134,    80,   136,   137,   138,
     139,   140,    -1,   142,   143,   144,   145,    -1,   147,   148,
     149,    -1,    -1,   152,   153,   154,    -1,    10,    -1,    -1,
      -1,    -1,   161,    -1,    17,    -1,   110,    -1,    76,    -1,
      -1,    -1,    80,    -1,    -1,    28,    29,    -1,    -1,    -1,
      -1,    -1,   126,    -1,   128,   129,   130,    -1,   132,   133,
     134,    -1,   136,   137,   138,   139,   140,    -1,   142,   143,
     144,   145,   110,   147,   148,   149,    -1,    -1,   152,   153,
     154,    -1,    -1,    -1,    -1,   159,    -1,    -1,   126,    -1,
     128,   129,   130,    76,   132,   133,   134,    80,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    -1,   147,
     148,   149,    10,    -1,   152,   153,   154,    -1,    -1,    17,
      -1,   159,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,    -1,   147,   148,   149,    -1,    -1,   152,
     153,   154,    -1,   156,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    89,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,   115,   116,    -1,
      -1,   119,   120,   121,   122,    -1,   124,   125,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    -1,   147,
     148,   149,    -1,    76,   152,   153,   154,    80,   156,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    89,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,   114,   115,   116,    -1,    -1,   119,   120,   121,   122,
      -1,   124,   125,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,    -1,   147,   148,   149,    -1,    76,   152,
     153,   154,    80,   156,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    89,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,   115,   116,    -1,
      -1,   119,   120,   121,   122,    -1,   124,   125,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    -1,   147,
     148,   149,    -1,    76,   152,   153,   154,    80,   156,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    89,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,   114,   115,   116,    -1,    -1,   119,   120,   121,   122,
      -1,   124,   125,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,    -1,   147,   148,   149,    -1,    76,   152,
     153,   154,    80,   156,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    89,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,   115,   116,    -1,
      -1,   119,   120,   121,   122,    -1,   124,   125,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    -1,   147,
     148,   149,    -1,    76,   152,   153,   154,    80,   156,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    89,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,   114,   115,   116,    -1,    -1,   119,   120,   121,   122,
      -1,   124,   125,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,    -1,   147,   148,   149,    -1,    76,   152,
     153,   154,    80,   156,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    89,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,   115,   116,    -1,
      -1,   119,   120,   121,   122,    -1,   124,   125,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    -1,   147,
     148,   149,    -1,    76,   152,   153,   154,    80,   156,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    89,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,   114,   115,   116,    -1,    -1,   119,   120,   121,   122,
      -1,   124,   125,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,    -1,   147,   148,   149,    -1,    76,   152,
     153,   154,    80,   156,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    89,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,   115,   116,    42,
      -1,   119,   120,   121,   122,    -1,   124,   125,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    -1,   147,
     148,   149,    -1,    76,   152,   153,   154,    80,   156,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    29,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,    -1,   147,   148,   149,    -1,    76,   152,
     153,   154,    80,   156,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    -1,   147,
     148,   149,    -1,    76,   152,   153,   154,    80,   156,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    29,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,    -1,   147,   148,   149,    -1,    76,   152,
     153,   154,    80,   156,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    -1,   147,
     148,   149,    -1,    76,   152,   153,   154,    80,   156,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    89,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,    -1,   147,   148,   149,    28,    76,   152,
     153,   154,    80,   156,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    76,    -1,    -1,   126,    80,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    61,   147,
     148,   149,    10,    -1,   152,   153,   154,    -1,   156,    17,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,
      28,    -1,    -1,    -1,    -1,   126,    -1,   128,   129,   130,
      -1,   132,   133,   134,    42,   136,   137,   138,   139,    -1,
      -1,   142,   143,   144,   145,    -1,   147,   110,   149,    -1,
      -1,   152,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,   129,   130,    76,   132,
     133,   134,    80,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,    -1,   147,   148,   149,    10,   151,   152,
     153,   154,    -1,    -1,    17,    -1,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    61,   147,
     148,   149,    10,    -1,   152,   153,   154,    -1,    -1,    17,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,   110,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,   126,    -1,   128,   129,   130,    76,   132,
     133,   134,    80,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,    -1,   147,   148,   149,    10,    -1,   152,
     153,   154,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    28,    29,    -1,    76,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,   126,    42,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    -1,   147,
     148,   149,   110,    -1,   152,   153,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    -1,   147,
     148,   149,    10,   151,   152,   153,   154,   110,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,    -1,   147,   148,   149,    10,    -1,   152,
     153,   154,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    76,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   110,    -1,    76,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,   110,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,   128,   129,   130,    -1,   132,   133,   134,    -1,   136,
     137,   138,   139,   140,    -1,   142,   143,   144,   145,    -1,
     147,   148,   149,    10,   151,   152,   153,   154,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      76,    -1,   109,   110,    80,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,   126,
      -1,   128,   129,   130,    -1,   132,   133,   134,    -1,   136,
     137,   138,   139,   140,   110,   142,   143,   144,   145,    -1,
     147,   148,   149,    -1,    -1,   152,   153,   154,    -1,    -1,
     126,    -1,   128,   129,   130,    76,   132,   133,   134,    80,
     136,   137,   138,   139,   140,    -1,   142,   143,   144,   145,
      -1,   147,   148,   149,    10,    -1,   152,   153,   154,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,   128,   129,   130,
      -1,   132,   133,   134,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,   147,   148,   149,    10,
      -1,   152,   153,   154,    -1,    -1,    17,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    80,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    11,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,   128,   129,   130,    76,   132,   133,   134,    80,
     136,   137,   138,   139,   140,    -1,   142,   143,   144,   145,
      -1,   147,   148,   149,    -1,    10,   152,   153,   154,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    76,    -1,    -1,   110,
      80,    -1,    -1,    28,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,   128,   129,   130,
      -1,   132,   133,   134,    -1,   136,   137,   138,   139,   140,
     110,   142,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,   126,    -1,   128,   129,
     130,    76,   132,   133,   134,    80,   136,   137,   138,   139,
     140,    -1,   142,   143,   144,   145,    -1,   147,   148,   149,
      10,    -1,   152,   153,   154,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,   128,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,   139,   140,    -1,   142,   143,   144,
     145,    -1,   147,   148,   149,    10,    -1,   152,   153,   154,
      -1,    -1,    17,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      80,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,   128,   129,
     130,    76,   132,   133,   134,    80,   136,   137,   138,   139,
     140,    -1,   142,   143,   144,   145,    -1,   147,   148,   149,
      -1,    -1,   152,   153,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    -1,   110,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,   128,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,   139,   140,   110,   142,   143,   144,
     145,    -1,   147,   148,   149,    -1,    -1,   152,   153,   154,
      -1,    -1,   126,    -1,   128,   129,   130,    -1,   132,   133,
     134,    -1,   136,   137,   138,   139,   140,    -1,   142,   143,
     144,   145,    -1,   147,   148,   149,    10,    -1,   152,   153,
     154,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    11,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    76,    -1,    -1,   110,    80,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,   126,    -1,   128,   129,   130,    -1,   132,   133,
     134,    -1,   136,   137,   138,   139,   140,   110,   142,   143,
     144,   145,    -1,   147,   148,   149,    -1,    -1,   152,   153,
     154,    -1,    -1,   126,    -1,   128,   129,   130,    76,   132,
     133,   134,    80,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,    -1,   147,   148,   149,    10,    -1,   152,
     153,   154,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    -1,   147,
     148,   149,    -1,    -1,   152,   153,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    89,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,    -1,   147,   148,   149,    -1,    76,   152,
     153,   154,    80,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    89,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    -1,   147,
     148,   149,    -1,    76,   152,   153,   154,    80,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    89,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,    -1,   147,   148,   149,    -1,    76,   152,
     153,   154,    80,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    89,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    -1,   147,
     148,   149,    -1,    76,   152,   153,   154,    80,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    89,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,    -1,   147,   148,   149,    -1,    76,   152,
     153,   154,    80,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    89,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    52,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    -1,   147,
     148,   149,    -1,    76,   152,   153,   154,    80,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,   139,   140,    28,   142,
     143,   144,   145,    -1,   147,   148,   149,    -1,    76,   152,
     153,   154,    80,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,   145,    -1,   147,
     148,   149,    -1,    76,   152,   153,   154,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,   128,   129,
     130,    -1,   132,   133,   134,    -1,    -1,   137,   138,   139,
      -1,    -1,   142,   143,   144,   145,    -1,   110,    -1,   149,
      -1,    -1,   152,   153,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,    28,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,   145,    19,   147,   148,   149,    23,    24,   152,
     153,   154,    -1,    -1,    -1,    -1,    32,    -1,    34,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    80,    -1,    -1,    28,    64,    -1,
      66,    -1,    68,    -1,    70,    -1,    -1,    73,    -1,    75,
      -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,
     126,    -1,   128,   129,   130,    76,   132,   133,   134,    80,
     136,   137,   138,   139,    -1,    -1,   142,   143,   144,   145,
      -1,   147,   148,   149,    -1,    -1,   152,   153,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,   110,
      -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    15,
      16,   157,    -1,    -1,    -1,   126,    22,   128,   129,   130,
      -1,   132,   133,   134,    30,   136,   137,   138,   139,    -1,
      36,   142,   143,   144,   145,    41,   147,    43,   149,    -1,
      46,   152,   153,   154,    -1,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,    -1,
      86,    -1,    88,    -1,    90,    -1,    -1,    93,    -1,    -1,
      -1,    97,    -1,    -1,   100
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   166,   167,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    15,    16,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    29,    30,    32,    34,    35,
      36,    37,    38,    39,    40,    41,    43,    44,    46,    49,
      50,    51,    53,    54,    55,    57,    58,    60,    64,    65,
      66,    67,    68,    69,    70,    73,    74,    75,    77,    78,
      79,    81,    82,    83,    84,    85,    86,    87,    88,    90,
      91,    93,    94,    95,    96,    97,    99,   100,   101,   102,
     105,   107,   110,   126,   127,   128,   129,   134,   144,   146,
     148,   149,   151,   154,   156,   158,   160,   168,   169,   170,
     171,   172,   173,   175,   180,   181,   182,   186,   188,   192,
     193,   194,   195,   196,   197,   199,   200,   201,   205,   206,
     209,   210,   213,   214,   217,   220,   221,   247,   250,   251,
     271,   272,   273,   274,   275,   276,   284,   285,   286,   287,
     290,   291,   292,   293,   294,   295,   296,   298,   299,   300,
     301,   302,   151,   170,   287,   106,   277,   278,     3,   183,
      12,    37,    38,    40,    88,   156,   160,   217,   271,   275,
     285,   286,   287,   290,   292,   293,   277,   287,    96,   250,
      77,   183,   170,   265,   287,   170,   158,   287,   288,     7,
       9,    96,   190,   288,    66,   108,   198,   287,   198,   287,
      23,    35,   251,   287,   287,   158,     3,    15,    16,    22,
      30,    36,    41,    43,    46,    54,    65,    74,    83,    84,
      86,    88,    90,    93,    97,   100,   185,   187,   218,   158,
     185,   252,   253,    29,   175,   191,   287,   287,   287,   287,
     287,   287,     7,   158,    35,   194,   194,   150,   240,   264,
     287,    94,    95,   151,   287,   289,   287,   191,   287,   287,
     287,    87,   158,   170,   287,   287,   175,   182,   287,   290,
     175,   182,   287,   185,   248,   287,   264,   287,   287,   287,
     287,   287,   287,   287,   287,     1,   157,   168,   176,   264,
      98,   135,   240,   266,   267,   289,   198,   264,   287,   297,
      68,   170,   156,    72,   174,   185,   185,    47,    71,   235,
     252,   151,   141,    10,    17,    28,    76,    80,   110,   126,
     128,   129,   130,   132,   133,   134,   136,   137,   138,   139,
     140,   142,   143,   144,   145,   147,   148,   149,   152,   153,
     154,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   155,   227,   158,   160,
      76,    80,   158,   170,   151,   287,   287,   287,   264,   156,
     175,    42,   277,   248,   252,   151,   131,   151,   105,   186,
     240,   268,   269,   270,   289,   248,   210,   250,   185,   158,
     191,    29,    42,   191,   106,   191,   280,    29,    42,   191,
     280,   288,    33,    61,   151,    89,   175,   227,   268,   170,
     158,   229,    98,   158,   185,   254,   255,     1,   130,   259,
      42,   131,   170,   191,   191,   268,   185,   131,   151,   287,
     287,   151,   156,   191,   158,   268,   151,   202,   151,   202,
     151,   111,   249,   151,   151,    33,    61,   191,   151,   157,
     157,   168,   131,   157,   287,   131,   159,   131,   159,   161,
     280,    42,   131,   161,   280,   109,   131,   161,     7,     1,
     157,   176,    56,   130,   211,   156,   222,   151,   287,   287,
     287,   287,   198,   287,   198,   287,   287,   287,   287,   287,
      16,   100,   287,   287,    30,    54,    96,   186,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   289,   289,   289,   289,   289,   268,
     268,   198,   287,   198,   287,    24,    42,    77,   102,   279,
     282,   283,   287,   302,    29,    42,    29,    42,    89,    42,
     161,   198,   287,   191,   151,   151,   287,   287,   111,   159,
     131,   151,   185,   252,    42,   264,    40,   287,   198,   287,
     158,   191,    40,   287,   198,   287,   191,   154,   177,   179,
     287,   177,   178,   170,   287,    31,   287,   159,    24,    42,
      45,    63,    66,    77,    96,   169,   230,   231,   232,   233,
     219,   255,   131,   159,    16,    30,    43,    65,    83,    84,
      88,   100,   160,   189,   260,   272,   111,   256,   287,   253,
     159,   240,   287,     1,   207,   268,   159,    18,   203,   260,
     272,   131,   179,   178,   157,   159,   159,   266,   159,   266,
     170,   161,   198,   287,   161,   170,   287,   161,   287,   161,
     287,   157,   157,   185,   264,   156,     1,   185,   215,   216,
      24,    66,    77,    96,   224,   234,   151,   151,   151,   151,
     151,   159,   161,    42,    77,   131,   159,   271,    76,    76,
      40,   198,   287,    40,   198,   287,   198,   287,   277,   277,
     151,   240,   289,   270,   211,   151,   287,   159,   287,    29,
     191,    29,   191,   281,   282,   287,    29,   191,   280,    29,
     191,   280,   131,   151,    11,   151,    31,    31,   170,    89,
     175,    42,    77,   232,   131,   159,   158,   185,    24,    66,
      77,    96,   236,   159,   255,   259,     1,   264,    59,   289,
     157,    62,   103,   157,   208,   159,   158,   175,   185,   204,
     248,   151,   151,   170,   161,   280,   161,   280,   170,   109,
       1,   212,   157,   111,   131,   157,    77,   223,     1,     3,
      10,    15,    17,    22,    36,    41,    46,    48,    54,    74,
      86,    90,    93,    97,   126,   127,   128,   129,   132,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   149,   152,   153,   154,   155,   158,   184,   187,   225,
     226,   227,   271,   282,   259,   271,   271,   287,    29,    29,
     287,    29,    29,   161,   161,   191,   191,   156,   191,    89,
      40,   287,    40,   287,   131,   159,    89,    40,   287,   191,
      40,   287,   191,   287,   287,   170,   287,   170,    31,   185,
     231,   255,   130,   263,    77,   259,   256,   161,    42,   161,
      29,   170,   264,   204,   130,   175,    40,   170,   287,   161,
      40,   170,   287,   161,   287,   157,    35,    70,    73,   157,
     169,   193,   287,   216,   236,   158,   228,   185,   287,   133,
     228,   228,   256,    89,    40,    40,    89,    40,    40,    40,
      40,     1,   212,   287,   287,   287,   282,   287,   287,   287,
      11,    31,   170,   263,   159,   160,   189,   240,   262,   272,
     135,   242,   256,    52,   104,   243,   287,   260,   272,   170,
     191,   159,   287,   287,   170,   287,   170,   193,     1,   130,
     258,   230,   159,     3,    90,   226,   227,   287,   287,   287,
     287,   287,   287,   157,   157,    89,    89,    89,    89,   287,
     170,   256,   263,   161,   264,   240,   241,   287,     3,    79,
      90,   244,   245,   246,   287,   175,   192,   239,   161,   175,
      89,    89,   160,   189,   257,   272,    92,   237,   159,   228,
     228,    89,    89,    89,    89,    89,    89,   287,   287,   287,
     287,   242,   256,   240,   261,   262,   272,    42,   161,   246,
     104,   131,   111,   136,   138,   139,   142,   143,    52,   272,
     287,   287,     1,   161,   264,   243,   287,   261,   262,   287,
     245,   246,   246,   246,   246,   246,   246,   244,   161,   257,
     272,   161,   151,   238,   239,   161,   257,   272,   261
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
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 805 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 806 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
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
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 813 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 814 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 815 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 816 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 817 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 818 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 819 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 821 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 827 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 833 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 839 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 845 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 852 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 859 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 863 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 869 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 873 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 874 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 875 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 876 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 877 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 879 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 881 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 883 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 888 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 891 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 892 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 893 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 894 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 895 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 896 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 900 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 901 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 905 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 906 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 907 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 911 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 912 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 916 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 920 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 922 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 926 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 927 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 932 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 934 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 936 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 943 "chapel.ypp"
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
#line 953 "chapel.ypp"
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
#line 963 "chapel.ypp"
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
#line 973 "chapel.ypp"
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
#line 985 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 990 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 995 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1003 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1004 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1009 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1011 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1013 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1018 "chapel.ypp"
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
#line 1034 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1041 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1050 "chapel.ypp"
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
#line 1058 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1062 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1068 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1069 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1074 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1079 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1084 "chapel.ypp"
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
#line 1106 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1112 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1119 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[(2) - (2)].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1125 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[(2) - (2)].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1131 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1142 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1148 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1155 "chapel.ypp"
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
#line 1181 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1185 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1190 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1194 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1199 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1206 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1210 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1213 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1214 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1215 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1216 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1217 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1218 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1219 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1220 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1221 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1225 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1226 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1227 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1228 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1230 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1231 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1232 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1234 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1235 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1236 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1240 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1241 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1242 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1243 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1244 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1245 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1246 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1247 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1248 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1249 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1250 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1251 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1255 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1256 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1260 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1264 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1265 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1266 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1271 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1273 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1277 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1279 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1283 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1284 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1288 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1309 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1314 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1317 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1318 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1319 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1323 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1324 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1327 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1333 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1338 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1340 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1344 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1349 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1350 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1354 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1356 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1358 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1366 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1367 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1370 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1374 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1375 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1376 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1379 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1380 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1384 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1386 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1388 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1393 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1406 "chapel.ypp"
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

  case 340:
/* Line 1792 of yacc.c  */
#line 1422 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1424 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1426 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1430 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1431 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1432 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1434 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1439 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1445 "chapel.ypp"
    {
      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), context->latestComment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
      context->latestComment = NULL;
    }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1451 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1460 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1470 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1479 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1486 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1488 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1494 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1496 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1520 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1527 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1536 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1561 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1565 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1569 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1577 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1583 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1584 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1592 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1598 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1600 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1602 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1606 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1607 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1609 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1610 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1616 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1617 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1618 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1619 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1623 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1624 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1629 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1630 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1634 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1635 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1639 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1644 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1645 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1649 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1650 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1651 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1652 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1656 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1657 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1674 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1676 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1678 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1680 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1682 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1684 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1686 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1688 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1690 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1692 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1694 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1699 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1701 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1703 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1705 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1707 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1709 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1711 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1713 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1715 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1719 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1721 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1723 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1730 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1736 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1742 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1748 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1757 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1766 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1782 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1786 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1791 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1795 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1796 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1800 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1804 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1805 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1810 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1814 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1818 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1824 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1825 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1826 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1827 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1828 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1829 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1835 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1840 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1845 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1861 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1863 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1865 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1867 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1869 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1871 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1873 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1875 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1879 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1880 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1883 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1884 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1885 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1907 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1908 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1909 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1913 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1914 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1915 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1916 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1924 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1925 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1926 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1927 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 1931 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 1932 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 1937 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 1938 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 1939 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 1940 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 1941 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 1942 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 1943 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 1944 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 1945 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 1947 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 1951 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 1958 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 1959 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 1963 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 1964 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 1965 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 1966 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 1967 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 1968 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 1969 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 1970 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 1971 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 1972 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 1973 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 1974 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 1975 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 1976 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 1977 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 1978 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 1979 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 1980 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 1981 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 1982 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 1983 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 1984 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 1985 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 1989 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 1990 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 1991 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 1992 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 1993 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 1994 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 1998 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 1999 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2000 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2001 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2005 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2006 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2007 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2008 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2013 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2014 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2015 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2016 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2017 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2018 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2019 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 9522 "bison-chapel.cpp"
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


