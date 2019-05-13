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
     TBANG = 381,
     TBAND = 382,
     TBNOT = 383,
     TBOR = 384,
     TBXOR = 385,
     TCOLON = 386,
     TCOMMA = 387,
     TDIVIDE = 388,
     TDOT = 389,
     TDOTDOT = 390,
     TDOTDOTDOT = 391,
     TEQUAL = 392,
     TEXP = 393,
     TGREATER = 394,
     TGREATEREQUAL = 395,
     THASH = 396,
     TIO = 397,
     TLESS = 398,
     TLESSEQUAL = 399,
     TMINUS = 400,
     TMOD = 401,
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
#define YYLAST   16815

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  165
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  138
/* YYNRULES -- Number of rules.  */
#define YYNRULES  583
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1043

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
    1327,  1329,  1331,  1333,  1335,  1337,  1340,  1345,  1350,  1355,
    1361,  1364,  1367,  1370,  1373,  1376,  1379,  1382,  1389,  1396,
    1401,  1411,  1421,  1429,  1436,  1443,  1448,  1458,  1468,  1476,
    1481,  1488,  1495,  1505,  1515,  1522,  1524,  1526,  1528,  1530,
    1532,  1534,  1536,  1538,  1542,  1543,  1545,  1550,  1552,  1556,
    1561,  1563,  1567,  1572,  1576,  1580,  1582,  1584,  1587,  1589,
    1592,  1594,  1596,  1600,  1603,  1608,  1610,  1612,  1614,  1616,
    1618,  1620,  1622,  1624,  1629,  1633,  1637,  1641,  1645,  1648,
    1651,  1653,  1654,  1656,  1659,  1662,  1664,  1666,  1668,  1670,
    1672,  1674,  1676,  1681,  1686,  1691,  1695,  1699,  1703,  1707,
    1711,  1716,  1720,  1725,  1727,  1729,  1731,  1733,  1735,  1737,
    1739,  1741,  1745,  1750,  1754,  1759,  1763,  1768,  1772,  1778,
    1782,  1786,  1790,  1794,  1798,  1802,  1806,  1810,  1814,  1818,
    1822,  1826,  1830,  1834,  1838,  1842,  1846,  1850,  1854,  1858,
    1862,  1866,  1870,  1873,  1876,  1879,  1882,  1885,  1888,  1891,
    1895,  1899,  1903,  1907,  1911,  1915,  1919,  1923,  1925,  1927,
    1929,  1931,  1933,  1935
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
     287,    -1,   287,    11,   287,    -1,   177,   132,   287,    -1,
     177,   132,   287,    11,   287,    -1,    -1,   177,    -1,   154,
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
      -1,   185,   131,   287,    -1,    91,   287,   151,    -1,    81,
     287,   156,   207,   157,    -1,    81,   287,   156,     1,   157,
      -1,    -1,   207,   208,    -1,   103,   264,   191,    -1,    62,
     170,    -1,    62,    29,   170,    -1,   210,   185,   211,   156,
     212,   157,    -1,    35,   190,   210,   185,   211,   156,   212,
     157,    -1,   210,   185,   211,   156,     1,   157,    -1,    35,
     190,   210,   185,   211,   156,     1,   157,    -1,    19,    -1,
      75,    -1,    99,    -1,    -1,   131,   264,    -1,    -1,   212,
     193,    -1,   212,   169,   193,    -1,   214,   185,   156,   215,
     157,    -1,   214,   185,   156,     1,   157,    -1,    32,    -1,
     216,    -1,   215,   132,    -1,   215,   132,   216,    -1,   185,
      -1,   185,   111,   287,    -1,    -1,    -1,    50,   218,   229,
     219,   236,   259,   243,   239,    -1,    -1,    44,    -1,    34,
     288,    -1,    35,   288,    -1,    64,    -1,    -1,    -1,   220,
     235,   222,   224,   223,   236,   258,   237,   243,   238,    -1,
     234,   226,   228,    -1,   234,   227,   228,    -1,   234,   225,
     134,   226,   228,    -1,   234,   225,   134,   227,   228,    -1,
     234,     1,   228,    -1,   271,    -1,   158,   287,   159,    -1,
     184,    -1,   128,   185,    -1,   127,    -1,   129,    -1,   130,
      -1,   128,    -1,   137,    -1,   147,    -1,   144,    -1,   140,
      -1,   143,    -1,   139,    -1,   149,    -1,   145,    -1,   154,
      -1,   133,    -1,   152,    -1,   153,    -1,   146,    -1,   138,
      -1,   126,    -1,    17,    -1,   141,    -1,    10,    -1,   155,
      -1,   142,    -1,    48,    -1,   185,   126,    -1,   111,    -1,
     122,    -1,   119,    -1,   121,    -1,   115,    -1,   120,    -1,
     116,    -1,   112,    -1,   113,    -1,   114,    -1,   125,    -1,
     124,    -1,    -1,   158,   230,   159,    -1,   158,   230,   159,
      -1,    -1,   231,    -1,   230,   132,   231,    -1,   232,   185,
     263,   256,    -1,   169,   232,   185,   263,   256,    -1,   232,
     185,   263,   242,    -1,   232,   158,   255,   159,   263,   256,
      -1,   232,   158,   255,   159,   263,   242,    -1,    -1,   233,
      -1,    42,    -1,    45,    -1,    63,    -1,    24,    -1,    24,
      42,    -1,    24,    77,    -1,    66,    -1,    77,    -1,    96,
      -1,    -1,    66,    -1,    77,    -1,    24,    77,    -1,    24,
      -1,    96,    -1,    71,    -1,    47,    -1,    -1,    24,    -1,
      24,    77,    -1,    77,    -1,    66,    -1,    96,    -1,    -1,
      92,    -1,   151,    -1,   239,    -1,   175,    -1,   192,    -1,
     150,   185,    -1,   150,    -1,    -1,   240,    -1,   136,   287,
      -1,   136,   241,    -1,    -1,   104,   287,    -1,    52,   244,
      -1,   104,   287,    52,   244,    -1,    52,   244,   104,   287,
      -1,   245,    -1,   244,   132,   245,    -1,   246,   111,   246,
      -1,   246,   143,   246,    -1,   246,   144,   246,    -1,   246,
     137,   246,    -1,   246,   139,   246,    -1,   246,   140,   246,
      -1,    79,   246,    -1,     3,    -1,    90,    -1,    96,   248,
     151,    -1,    23,    96,   248,   151,    -1,    35,    96,   248,
     151,    -1,   185,   249,    -1,   185,   249,   132,   248,    -1,
      -1,   111,   272,    -1,   111,   260,    -1,    66,    -1,    24,
      -1,    77,    -1,    24,    77,    -1,   102,    -1,    23,   250,
     252,   151,    -1,    35,   288,   250,   252,   151,    -1,   250,
     252,   151,    -1,   253,    -1,   252,   132,   253,    -1,   185,
     259,   256,    -1,   158,   255,   159,   259,   256,    -1,    98,
      -1,   185,    -1,   158,   255,   159,    -1,   254,    -1,   254,
     132,    -1,   254,   132,   255,    -1,    -1,   111,    59,    -1,
     111,   289,    -1,   160,   161,   272,    -1,   160,   161,    -1,
     160,   264,   161,   272,    -1,   160,   264,   161,    -1,   160,
     161,   257,    -1,   160,   264,   161,   257,    -1,   160,     1,
     161,    -1,    -1,   131,   272,    -1,   131,   257,    -1,   131,
     189,    -1,     1,    -1,    -1,   131,   272,    -1,   131,   260,
      -1,   131,   189,    -1,     1,    -1,   160,   264,   161,   272,
      -1,   160,   264,   161,   260,    -1,   160,   264,    42,   287,
     161,   272,    -1,   160,     1,   161,    -1,    -1,   272,    -1,
     240,    -1,   160,   161,   261,    -1,   160,   264,   161,   261,
      -1,   160,   161,   262,    -1,   160,   264,   161,   262,    -1,
     160,   264,    42,   287,   161,   261,    -1,    -1,   131,   272,
      -1,   131,   240,    -1,   131,   189,    -1,   131,   262,    -1,
     287,    -1,   240,    -1,   264,   132,   287,    -1,   264,   132,
     240,    -1,   287,    -1,   265,   132,   287,    -1,    98,    -1,
     289,    -1,   240,    -1,   266,   132,   266,    -1,   267,   132,
     266,    -1,    -1,   269,    -1,   270,    -1,   269,   132,   270,
      -1,   186,   111,   240,    -1,   186,   111,   289,    -1,   240,
      -1,   289,    -1,   186,    -1,   188,    -1,   275,    -1,   290,
      -1,   274,    -1,   299,    -1,   298,    -1,    84,   287,    -1,
      43,   158,   268,   159,    -1,    30,   158,   268,   159,    -1,
      87,   158,   268,   159,    -1,    85,    87,   158,   268,   159,
      -1,    12,   287,    -1,    88,   287,    -1,    16,   287,    -1,
      65,   287,    -1,   100,   287,    -1,    83,   287,    -1,   271,
     150,    -1,    37,   287,    42,   287,    29,   287,    -1,    37,
     287,    42,   198,    29,   287,    -1,    37,   287,    29,   287,
      -1,    37,   287,    42,   287,    29,    40,   287,    89,   287,
      -1,    37,   287,    42,   198,    29,    40,   287,    89,   287,
      -1,    37,   287,    29,    40,   287,    89,   287,    -1,    38,
     287,    42,   287,    29,   287,    -1,    38,   287,    42,   198,
      29,   287,    -1,    38,   287,    29,   287,    -1,    38,   287,
      42,   287,    29,    40,   287,    89,   287,    -1,    38,   287,
      42,   198,    29,    40,   287,    89,   287,    -1,    38,   287,
      29,    40,   287,    89,   287,    -1,   160,   264,   161,   287,
      -1,   160,   264,    42,   287,   161,   287,    -1,   160,   264,
      42,   198,   161,   287,    -1,   160,   264,    42,   287,   161,
      40,   287,    89,   287,    -1,   160,   264,    42,   198,   161,
      40,   287,    89,   287,    -1,    40,   287,    89,   287,    31,
     287,    -1,    58,    -1,   275,    -1,   271,    -1,   293,    -1,
     292,    -1,   217,    -1,   285,    -1,   286,    -1,   284,   142,
     287,    -1,    -1,   278,    -1,   106,   158,   279,   159,    -1,
     282,    -1,   279,   132,   282,    -1,   106,   158,   281,   159,
      -1,   282,    -1,   281,   132,   282,    -1,   283,   271,   259,
     256,    -1,   302,    76,   271,    -1,   287,    76,   271,    -1,
      24,    -1,    42,    -1,    24,    42,    -1,    77,    -1,    24,
      77,    -1,   102,    -1,   290,    -1,   284,   142,   287,    -1,
      57,   287,    -1,    51,   252,    42,   287,    -1,   296,    -1,
     272,    -1,   273,    -1,   300,    -1,   301,    -1,   217,    -1,
     285,    -1,   286,    -1,   158,   136,   287,   159,    -1,   287,
     131,   287,    -1,   287,   131,    16,    -1,   287,   131,   100,
      -1,   287,   135,   287,    -1,   287,   135,    -1,   135,   287,
      -1,   135,    -1,    -1,   287,    -1,    94,   287,    -1,    95,
     287,    -1,   287,    -1,   271,    -1,   292,    -1,   293,    -1,
     294,    -1,   290,    -1,   217,    -1,   291,   158,   268,   159,
      -1,   291,   160,   268,   161,    -1,    69,   158,   268,   159,
      -1,   287,   134,   186,    -1,   287,   134,    96,    -1,   287,
     134,    30,    -1,   287,   134,    54,    -1,   158,   266,   159,
      -1,   158,   266,   132,   159,    -1,   158,   267,   159,    -1,
     158,   267,   132,   159,    -1,    36,    -1,    93,    -1,   295,
      -1,     4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,
      -1,   156,   264,   157,    -1,   156,   264,   132,   157,    -1,
     160,   264,   161,    -1,   160,   264,   132,   161,    -1,   160,
     297,   161,    -1,   160,   297,   132,   161,    -1,   287,   109,
     287,    -1,   297,   132,   287,   109,   287,    -1,   287,   149,
     287,    -1,   287,   145,   287,    -1,   287,   154,   287,    -1,
     287,   133,   287,    -1,   287,   152,   287,    -1,   287,   153,
     287,    -1,   287,   146,   287,    -1,   287,   137,   287,    -1,
     287,   147,   287,    -1,   287,   144,   287,    -1,   287,   140,
     287,    -1,   287,   143,   287,    -1,   287,   139,   287,    -1,
     287,   127,   287,    -1,   287,   129,   287,    -1,   287,   130,
     287,    -1,   287,   110,   287,    -1,   287,   148,   287,    -1,
     287,   138,   287,    -1,   287,    17,   287,    -1,   287,    10,
     287,    -1,   287,   141,   287,    -1,   287,    28,   287,    -1,
     149,   287,    -1,   145,   287,    -1,    55,   287,    -1,    67,
     287,    -1,   126,   287,    -1,   287,   126,    -1,   128,   287,
      -1,   287,    76,   287,    -1,   287,    76,   198,    -1,   302,
      76,   287,    -1,   302,    76,   198,    -1,   287,    80,   287,
      -1,   287,    80,   198,    -1,   302,    80,   287,    -1,   302,
      80,   198,    -1,   149,    -1,   154,    -1,   110,    -1,   148,
      -1,   127,    -1,   129,    -1,   130,    -1
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
    1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,  1237,
    1241,  1242,  1243,  1244,  1245,  1246,  1247,  1248,  1249,  1250,
    1251,  1252,  1256,  1257,  1261,  1265,  1266,  1267,  1271,  1273,
    1275,  1277,  1279,  1284,  1285,  1289,  1290,  1291,  1292,  1293,
    1294,  1295,  1296,  1297,  1301,  1302,  1303,  1304,  1305,  1306,
    1310,  1311,  1315,  1316,  1317,  1318,  1319,  1320,  1324,  1325,
    1328,  1329,  1333,  1334,  1338,  1340,  1345,  1346,  1350,  1351,
    1355,  1356,  1358,  1360,  1362,  1367,  1368,  1371,  1372,  1373,
    1374,  1375,  1376,  1377,  1380,  1381,  1384,  1386,  1388,  1393,
    1406,  1423,  1424,  1426,  1431,  1432,  1433,  1434,  1435,  1439,
    1445,  1451,  1459,  1460,  1468,  1470,  1475,  1477,  1479,  1484,
    1486,  1488,  1495,  1496,  1497,  1502,  1504,  1506,  1510,  1514,
    1516,  1520,  1528,  1529,  1530,  1531,  1532,  1537,  1538,  1539,
    1540,  1541,  1561,  1565,  1569,  1577,  1584,  1585,  1586,  1590,
    1592,  1598,  1600,  1602,  1607,  1608,  1609,  1610,  1611,  1617,
    1618,  1619,  1620,  1624,  1625,  1629,  1630,  1631,  1635,  1636,
    1640,  1641,  1645,  1646,  1650,  1651,  1652,  1653,  1657,  1658,
    1669,  1670,  1671,  1672,  1673,  1674,  1676,  1678,  1680,  1682,
    1684,  1686,  1688,  1690,  1692,  1694,  1696,  1701,  1703,  1705,
    1707,  1709,  1711,  1713,  1715,  1717,  1719,  1721,  1723,  1725,
    1732,  1738,  1744,  1750,  1759,  1769,  1777,  1778,  1779,  1780,
    1781,  1782,  1783,  1784,  1789,  1790,  1794,  1798,  1799,  1803,
    1807,  1808,  1812,  1816,  1820,  1827,  1828,  1829,  1830,  1831,
    1832,  1836,  1837,  1842,  1847,  1855,  1856,  1857,  1858,  1859,
    1860,  1861,  1862,  1863,  1865,  1867,  1869,  1871,  1873,  1875,
    1877,  1882,  1883,  1886,  1887,  1888,  1891,  1892,  1893,  1894,
    1905,  1906,  1910,  1911,  1912,  1916,  1917,  1918,  1919,  1927,
    1928,  1929,  1930,  1934,  1935,  1939,  1940,  1941,  1942,  1943,
    1944,  1945,  1946,  1947,  1948,  1949,  1953,  1961,  1962,  1966,
    1967,  1968,  1969,  1970,  1971,  1972,  1973,  1974,  1975,  1976,
    1977,  1978,  1979,  1980,  1981,  1982,  1983,  1984,  1985,  1986,
    1987,  1988,  1992,  1993,  1994,  1995,  1996,  1997,  1998,  2002,
    2003,  2004,  2005,  2009,  2010,  2011,  2012,  2017,  2018,  2019,
    2020,  2021,  2022,  2023
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
  "TASSIGNSR", "TBANG", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON",
  "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP",
  "TGREATER", "TGREATEREQUAL", "THASH", "TIO", "TLESS", "TLESSEQUAL",
  "TMINUS", "TMOD", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI",
  "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP",
  "TRP", "TLSBR", "TRSBR", "TNOELSE", "TUMINUS", "TUPLUS", "$accept",
  "program", "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
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
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   228,   228,   229,   230,   230,   230,   231,   231,
     231,   231,   231,   232,   232,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   234,   234,   234,   234,   234,   234,
     235,   235,   236,   236,   236,   236,   236,   236,   237,   237,
     238,   238,   239,   239,   240,   240,   241,   241,   242,   242,
     243,   243,   243,   243,   243,   244,   244,   245,   245,   245,
     245,   245,   245,   245,   246,   246,   247,   247,   247,   248,
     248,   249,   249,   249,   250,   250,   250,   250,   250,   251,
     251,   251,   252,   252,   253,   253,   254,   254,   254,   255,
     255,   255,   256,   256,   256,   257,   257,   257,   257,   257,
     257,   257,   258,   258,   258,   258,   258,   259,   259,   259,
     259,   259,   260,   260,   260,   260,   261,   261,   261,   262,
     262,   262,   262,   262,   263,   263,   263,   263,   263,   264,
     264,   264,   264,   265,   265,   266,   266,   266,   267,   267,
     268,   268,   269,   269,   270,   270,   270,   270,   271,   271,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   274,   275,   276,   276,   276,   276,
     276,   276,   276,   276,   277,   277,   278,   279,   279,   280,
     281,   281,   282,   282,   282,   283,   283,   283,   283,   283,
     283,   284,   284,   285,   286,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   288,   288,   289,   289,   289,   290,   290,   290,   290,
     291,   291,   292,   292,   292,   293,   293,   293,   293,   294,
     294,   294,   294,   295,   295,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   297,   297,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   299,   299,   299,   299,   299,   299,   299,   300,
     300,   300,   300,   301,   301,   301,   301,   302,   302,   302,
     302,   302,   302,   302
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
       1,     1,     1,     1,     1,     2,     4,     4,     4,     5,
       2,     2,     2,     2,     2,     2,     2,     6,     6,     4,
       9,     9,     7,     6,     6,     4,     9,     9,     7,     4,
       6,     6,     9,     9,     6,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     1,     4,     1,     3,     4,
       1,     3,     4,     3,     3,     1,     1,     2,     1,     2,
       1,     1,     3,     2,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     3,     3,     3,     3,     2,     2,
       1,     0,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     4,     4,     4,     3,     3,     3,     3,     3,
       4,     3,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     4,     3,     4,     3,     4,     3,     5,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   526,   527,   528,   529,
     530,     0,   464,    70,   100,     0,   198,   464,     0,   105,
       0,   345,    70,     0,     0,     0,     0,   208,   501,   501,
     523,     0,     0,     0,     0,   104,     0,   218,   101,     0,
     214,     0,     0,   107,     0,     0,   455,     0,   221,     0,
     344,     0,     0,     0,    45,    44,   103,   199,   346,     0,
       0,     0,     0,     0,     0,     0,   106,     0,     0,    83,
       0,   524,     0,     0,     0,   102,   200,     0,     0,   348,
       0,     0,   579,     0,   581,     0,   582,   583,   500,     0,
     580,   577,   122,   578,     0,     0,     0,     4,     0,     5,
       0,     9,    46,    10,    11,    12,    14,   418,   419,    22,
      13,   123,   129,    15,    17,    16,    19,    20,    21,    18,
     128,     0,   126,     0,   490,     0,   130,   127,     0,   131,
     506,   486,   487,   422,   420,     0,     0,   491,   492,     0,
     421,     0,   507,   508,   509,   525,   485,   424,   423,   488,
     489,     0,    38,    24,   430,     0,     0,   465,    71,     0,
       0,     0,     0,     0,     0,     0,     0,   490,   506,   420,
     491,   492,   432,   421,   507,   508,     0,   464,     0,     0,
     347,     0,   172,     0,   403,     0,   410,   502,   219,   529,
     136,     0,     0,   220,     0,     0,     0,     0,     0,     0,
       0,   501,   135,     0,     0,   410,    77,    84,    95,    89,
      97,    79,    88,    98,    85,    99,    93,    87,    94,    92,
      90,    91,    78,    80,    86,    96,     0,    81,     0,     0,
       0,     0,   352,     0,   119,    32,     0,   564,   483,     0,
     433,   565,     7,   410,   501,   125,   124,   315,   400,     0,
     399,     0,     0,   120,   505,     0,     0,    35,     0,   435,
     425,     0,   410,    36,   431,     0,   179,   175,     0,   421,
     179,   176,     0,   341,     0,   434,     0,   399,     0,     0,
     566,   568,   499,   563,   562,     0,    48,    51,     0,     0,
     405,     0,   407,     0,     0,   406,     0,     0,   399,     0,
       0,     6,     0,    47,     0,   201,     0,   301,   300,   222,
       0,   436,    23,     0,     0,     0,     0,     0,     0,     0,
     567,     0,     0,     0,     0,     0,     0,   498,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   260,   267,   268,   269,   264,   266,     0,
       0,   262,   265,   263,   261,     0,   271,   270,     0,     0,
     410,   410,     0,     0,     0,    25,    26,     0,     0,     0,
       0,     0,    27,     0,     0,     0,     0,    28,     0,    29,
       0,   418,   416,     0,   411,   412,   417,     0,     0,     0,
       0,     0,   145,     0,     0,   144,     0,   153,     0,     0,
       0,   151,     0,     0,     0,    57,   132,     0,   165,     0,
       0,    30,   283,   215,   356,     0,   357,   359,     0,   381,
       0,   362,     0,     0,   118,    31,    33,     0,   314,     0,
      64,   503,   504,   121,     0,    34,   410,     0,   186,   177,
     173,   178,   174,     0,   339,   336,    61,     0,    57,   138,
      37,    50,    49,    52,     0,   531,     0,     0,   519,     0,
     521,     0,     0,     0,     0,     0,     0,     0,     0,   535,
       8,     0,    40,     0,     0,     0,     0,     0,   294,   351,
     463,   559,   558,   561,   570,   569,   574,   573,   555,   552,
     553,   554,   495,   496,   494,   542,   517,   518,   516,   515,
     497,   546,   557,   551,   549,   560,   550,   548,   540,   545,
     547,   556,   539,   543,   544,   541,     0,     0,     0,     0,
       0,     0,     0,   572,   571,   576,   575,   475,   476,   478,
     480,     0,   467,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   533,   464,   464,   141,   337,   349,   404,     0,
       0,   427,     0,   338,   201,     0,     0,     0,     0,   439,
       0,     0,     0,   154,     0,   445,     0,     0,   152,   578,
      60,     0,    53,    58,     0,   164,     0,     0,     0,   426,
     288,   285,   286,   287,   291,   292,   293,   283,     0,   276,
       0,   284,   302,     0,   360,     0,   112,   114,   115,   110,
     111,   109,   108,   113,     0,   380,   379,   486,     0,   354,
     484,   353,   514,   402,   401,     0,     0,     0,   428,     0,
     180,   343,   486,     0,     0,     0,   532,   493,   520,   408,
     522,   409,   161,     0,     0,     0,   534,   159,   449,     0,
     537,   536,     0,    42,    41,    39,   202,     0,     0,   212,
       0,   209,   298,   295,   296,   299,   223,     0,    68,    69,
      67,    66,    65,   512,   513,   477,   479,     0,   466,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   137,   414,   415,   413,     0,   350,     0,   163,
       0,     0,   143,     0,   142,     0,   470,     0,     0,   149,
       0,     0,   147,     0,     0,   133,     0,   134,     0,     0,
     167,     0,   169,   289,   290,     0,   283,   274,     0,   394,
     303,   306,   305,   307,     0,   358,   361,   362,     0,     0,
     363,   364,   188,     0,     0,   187,   190,   429,     0,   181,
     184,     0,   340,    62,    63,   162,     0,     0,     0,     0,
     160,     0,     0,   217,   207,     0,   210,   206,   297,   302,
     272,    72,   255,    84,   253,    89,    74,    88,    85,   258,
      99,    87,    90,    73,    75,    86,   252,   234,   237,   235,
     236,   247,   238,   251,   243,   241,   254,   257,   242,   240,
     245,   250,   239,   244,   248,   249,   246,   256,     0,   232,
       0,    76,     0,   272,   272,   230,   468,   362,   506,   506,
       0,     0,     0,     0,     0,     0,     0,     0,   140,   139,
       0,   146,     0,     0,   438,     0,   437,     0,   469,     0,
       0,   444,   150,     0,   443,   148,    55,    54,   166,   454,
     168,     0,   394,   277,     0,     0,   362,   304,   320,   355,
     385,     0,   533,     0,   192,     0,     0,     0,   182,     0,
     157,   451,     0,     0,   155,   450,     0,   538,   196,   501,
     217,   217,   194,   217,   204,   213,   211,     0,   283,   229,
     233,     0,   259,     0,   225,   226,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   217,   442,     0,     0,
     471,   448,     0,     0,     0,     0,   171,   362,   394,     0,
     397,   396,   398,   486,   316,   280,   278,     0,     0,     0,
       0,   383,   486,   193,   191,     0,   185,     0,   158,     0,
     156,   205,   376,     0,   308,     0,   231,    72,    73,   272,
     272,     0,     0,     0,     0,     0,     0,   197,   195,     0,
       0,     0,     0,    56,   170,   279,   362,   386,     0,   317,
     319,   318,   334,     0,   335,   322,   325,     0,   321,   312,
     313,   216,     0,   183,     0,     0,     0,   375,   374,   486,
     309,   320,   273,   227,   228,     0,     0,     0,     0,     0,
       0,   441,   440,   447,   446,   282,   281,   388,   389,   391,
     486,     0,   533,   333,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   486,   453,   452,     0,   366,     0,     0,
       0,   390,   392,   324,   326,   327,   330,   331,   332,   328,
     329,   323,   371,   369,   486,   533,   310,   224,   311,   386,
     370,   486,   393
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   287,    98,   575,   100,   101,   102,   304,
     103,   288,   570,   574,   571,   104,   105,   106,   159,   799,
     230,   107,   227,   108,   605,   192,   235,   109,   110,   111,
     112,   113,   114,   678,   115,   116,   117,   439,   620,   741,
     118,   119,   616,   736,   120,   121,   476,   753,   122,   123,
     650,   651,   167,   228,   592,   125,   126,   478,   759,   656,
     802,   803,   409,   879,   413,   588,   589,   590,   591,   657,
     309,   724,   981,  1037,   971,   248,   960,   915,   919,   965,
     966,   967,   127,   274,   444,   128,   129,   231,   232,   417,
     418,   609,   978,   934,   421,   606,   998,   912,   846,   289,
     183,   293,   294,   383,   384,   385,   168,   131,   132,   133,
     169,   135,   156,   157,   531,   398,   695,   532,   533,   136,
     170,   171,   139,   193,   386,   173,   141,   174,   175,   144,
     145,   146,   299,   147,   148,   149,   150,   151
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -935
static const yytype_int16 yypact[] =
{
    -935,    45,  2877,  -935,   -47,  -935,  -935,  -935,  -935,  -935,
    -935,  4302,     1,   234,  -935,  9634,  -935,     1,  9634,  -935,
     277,   144,   234,  4302,  9634,  4302,   113,  -935,  9634,  6793,
    -935,  8146,  8732,  6635,  9634,  -935,   125,  -935,  -935, 16715,
    -935,  2574,  8867,  -935,  9634,  9634,  -935,  9634,  -935,  9634,
    -935,  9634,   246,   141,  1254,  1254,  -935,  -935,  -935,  9002,
    7695,  9634,  8867,  9634,  9634,   253,  -935,   200,  4302,  -935,
    9634,  -935,  9769,  9769, 16715,  -935,  -935,  9634,  9002,  -935,
    9634,  9634,  -935,  9634,  -935,  9634,  -935,  -935, 11794,  9634,
    -935,  9634,  -935,  -935,  3354,  6928,  8281,  -935,  4144,  -935,
     210,  -935,   298,  -935,  -935,  -935,  -935,  -935,  -935,  -935,
    -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,
    -935, 16715,  -935, 16715,   327,   331,  -935,  -935,  2574,  -935,
     224,  -935,  -935,  -935,   252,   263,   275,   273,   314, 16249,
   12031,   250,   317,   322,  -935,  -935,  -935,  -935,  -935,  -935,
    -935,   218,  -935,  -935, 16249,   311,  4302,  -935,  -935,   325,
    9634,  9634,  9634,  9634,  9634,  9002,  9002,   334,   336,  -935,
    -935,  -935,   357,   341,  -935,  -935,   344, 14124, 16715,  2574,
    -935,   356,  -935,   -26, 16249,   403,  7853, 16249,  -935,   185,
    -935, 16715,   262,   318, 16715,   354,    18, 13670,    15, 13595,
     318,  9634,  -935, 14045, 12920,  7853,  -935,  -935,  -935,  -935,
    -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,
    -935,  -935,  -935,  -935,  -935,  -935,  4302,  -935,   362,  1657,
      31,    12,  -935,  4302,  -935,  -935, 13745,   481,   357, 13745,
     357,   481,  -935,  7853,  9634,  -935,  -935, 16715,  -935,    98,
   16249,  9634,  9634,  -935, 16249,   363, 13970,  -935, 13745,   357,
   16249,   365,  7853,  -935, 16249, 14424,  -935,  -935, 14503, 11791,
    -935,  -935, 14582,   402,   364,   357,    99, 14199, 13745, 14661,
     442,   442,  1049,   481,   481,   -43,  -935,  -935,  3512,    79,
    -935,  9634,  -935,    54,    95,  -935,   -31,    97, 14740,   -34,
     514,  -935,  3670,  -935,   476,   404,   377,  -935,  -935,  -935,
     204,  -935,  -935,  9634,  9634,  9634,  9634,  8732,  8732,  9634,
    -935,  9634,  9634,  9634,  9904,  9634,   420, 11794,  9634,  9634,
    9634,  9634,  9634,  9634,  9634,  9634,  9634,  9634,  9634,  9634,
    9634,  9634,  9634,  -935,  -935,  -935,  -935,  -935,  -935,  8439,
    8439,  -935,  -935,  -935,  -935,  8439,  -935,  -935,  8439,  8439,
    7853,  7853,  8732,  8732,  7402,  -935,  -935, 14274, 14349, 14815,
      55,  3828,  -935,  8732,    18,   383,   268,  -935,  9634,  -935,
    9634,   427,  -935,   385,   416,  -935,  -935,   399, 16715,  2574,
     510,  9002,  -935,  4460,  8732,  -935,   396,  -935,    18,  4618,
    8732,  -935,    18,   318, 10039,  9634,  -935,  4302,   524,  9634,
     397,  -935,   201,  -935,  -935,  1657,  -935,   426,   401,  -935,
   10174,   451,  9634,  2574,  -935,  -935,  -935,   411,  -935,  9002,
    -935, 16249, 16249,  -935,    26,  -935,  7853,   413,  -935,   556,
    -935,   556,  -935, 10309,   443,  -935,  -935, 10039,  9634,  -935,
    -935,  -935,  -935,  -935,  8574,  -935, 12770,  7086,  -935,  7244,
    -935,  4302,   417,  8732,  6040,  3196,   418,  9634,  6358,  -935,
    -935,    28,  -935,  3986, 16715,  9002,   421, 16660,   310,  -935,
   14890, 16438, 16438,   359,  -935,   359,  -935,   359,   868,  1082,
    1257,   139, 12163, 12163,   357,   442,  -935,  -935,  -935,  -935,
    1049,  1468,   359, 12587, 12587, 16438, 12587, 12587,   626,   442,
    1468, 16513,   626,   481,   481,   442,   430,   431,   432,   436,
     444,   435,   437,  -935,   359,  -935,   359,    49,  -935,  -935,
    -935,   130,  -935,  1053, 16324,   220, 10444,  8732, 10579,  8732,
    9634,  8732, 12385,     1, 14965,  -935,  -935,  -935, 16249, 15040,
    7853,  -935,  7853,  -935,   404,   328,  9634,   131,  9634, 16249,
      50, 13820,  7402,  -935,  9634, 16249,    17,  2393,  -935,   448,
     470,   453, 15119,   470,   457,   579, 15194,  4302, 13895,  -935,
     135,  -935,  -935,  -935,  -935,  -935,  -935,   604,   209,  -935,
    2685,  -935,   339,   458,  1657,    31,  9634,   113,   125,  9634,
    9634,  9634,  9634,  9634,  5882,  -935,  -935,   507,  8011,  -935,
   16249,  -935,  -935,  -935, 16249,   463,    34,   465,  -935,  1900,
    -935,  -935,   329, 16715,   475,   479,  -935,  -935,  -935,  -935,
    -935,  -935,  -935,  4302,     4,  2076,  -935,  -935, 16249,  4302,
   16249,  -935, 15269,  -935,  -935,  -935,   499,  2250,   480,   521,
      82,  -935,   561,  -935,  -935,  -935,  -935,  6479,  -935,  -935,
    -935,  -935,  -935,  -935,  -935,  -935,  -935,  7402,  -935,    29,
    8732,  8732,  9634,   613, 15344,  9634,   618, 15419,   487, 12545,
      18,    18,  -935,  -935,  -935,  -935,   484,  -935, 13745,  -935,
   12995,  4776,  -935,  4934,  -935,   227,  -935, 13070,  5092,  -935,
      18,  5250,  -935,    18,  9634,  -935,  9634,  -935,  4302,  9634,
    -935,  4302,   619,  -935,  -935, 16715,   829,  -935,  1657,   520,
     575,  -935,  -935,  -935,    89,  -935,  -935,   451,   492,    77,
    -935,  -935,  -935,  5408,  9002,  -935,  -935,  -935, 16715,  -935,
     526,   344,  -935,  -935,  -935,  -935,  5566,   494,  5724,   501,
    -935,  9634,   508,   885,  -935,  9634, 16715,  -935,  -935,   339,
     513,   -25,  -935,   541,  -935,   543,   559,   552,   554,  -935,
     562,   567,   569,   231,   582,   581,  -935,  -935, 16715,  -935,
    -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,
    -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  9634,  -935,
     592,   593,   586,   513,   513,  -935,  -935,   451,   129,   136,
   15494, 10714, 10849, 15569, 10984, 11119, 11254, 11389,  -935,  -935,
    2416,  -935,  4302,  9634, 16249,  9634, 16249,  7402,  -935,  4302,
    9634, 16249,  -935,  9634, 16249,  -935, 15644, 16249,  -935, 16249,
     690,  4302,   520,  -935,   563,  9160,   106,  -935,    72,  -935,
    -935,  8732, 12274,  4302,  -935,    70,   565,  9634,  -935,  9634,
    -935, 16249,  4302,  9634,  -935, 16249,  4302, 16249,  -935,  7560,
    1097,  1097,  -935,  2723,  -935, 16249,  -935,    41,   201,  -935,
    -935, 12845,  -935, 12464,  -935,  -935,  -935,  9634,  9634,  9634,
    9634,  9634,  9634,  9634,  9634,   568, 16456, 15194, 13145, 13220,
    -935, 15194, 13295, 13370,  9634,  4302,  -935,   451,   520,  6199,
    -935,  -935,  -935,   123,  9002,  -935,  -935,   145,  9634,   -24,
   12620,  -935,   691,   403,  -935,   344, 16249, 13445,  -935, 13520,
    -935,  -935,  -935, 11524,   640,   232,  -935,   608,   610,   513,
     513, 15719, 15794, 15869, 15944, 16019, 16094,  -935,  -935,  4302,
    4302,  4302,  4302, 16249,  -935,  -935,   106,  9318,    86,  -935,
    -935, 16249,  -935,    32,  -935,   -15,  -935,   573, 16174,  -935,
    -935,  -935, 11389,  -935,  4302,  4302,  3035,  -935,  -935,   293,
    -935,    72,  -935,  -935,  -935,  9634,  9634,  9634,  9634,  9634,
    9634, 15194, 15194, 15194, 15194,  -935,  -935,  -935,  -935,  -935,
     245,  8732, 11925,  -935,  9634,   145,    32,    32,    32,    32,
      32,    32,   145,   706, 15194, 15194,   577, 11659,    96,   -38,
   12695,  -935,  -935, 16249,  -935,  -935,  -935,  -935,  -935,  -935,
    -935,   607,  -935,  -935,   355, 12036,  -935,  -935,  -935,  9476,
    -935,   517,  -935
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -935,  -935,  -935,    -1,  -407,  2005,  -935,  -935,  -935,  -935,
     367,   440,  -200,   299,   302,  -935,  -935,   358,   724,  -935,
    1898,  -162,  -631,  -935,  -774,  -935,   971,  -863,  -689,   -46,
    -935,  -935,  -935,   104,  -935,  -935,  -935,   493,  -935,    13,
    -935,  -935,  -935,  -935,  -935,   558,   208,   -66,  -935,  -935,
    -935,    10,  1028,  -935,  -935,  -935,  -935,  -935,  -935,  -935,
    -935,  -115,  -138,  -717,  -935,  -107,    57,   189,  -935,  -935,
    -935,    22,  -935,  -935,  -242,   240,  -935,  -174,  -197,  -226,
    -217,  -280,  -935,  -172,  -935,    -4,   758,  -114,   370,  -935,
    -386,  -712,  -923,  -935,  -558,  -440,  -934,  -855,  -739,   -20,
    -935,    46,  -935,  -180,  -935,   242,   611,  -328,  -935,  -935,
    1091,  -935,    -7,  -935,  -935,  -139,  -935,  -551,  -935,  -935,
    1216,  1414,   -11,     0,    83,   390,  -935,  1735,  1861,  -935,
    -935,  -935,  -935,  -935,  -935,  -935,  -935,  -352
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -534
static const yytype_int16 yytable[] =
{
     154,    97,   359,   621,   172,   587,   375,   177,   245,   246,
     176,   696,   535,   184,   310,   849,   179,   187,   187,   387,
     197,   199,   203,   204,   381,   410,   801,   615,   188,   593,
     419,   236,   419,   237,   238,   962,   239,   727,   240,   249,
     241,    60,   932,   381,   233,     3,   698,   233,   250,   254,
     256,   258,   259,   260,   422,    60,   970,   264,   276,   265,
     402,   268,   272,   427,   874,   376,   275,   277,  1021,   278,
     279,   910,   280,  -377,   281,   396,   297,   282,   283,   691,
     284,   381,   437,   250,   254,   298,   884,   885,  -189,  1004,
     419,   665,   607,  -372,  1033,   886,   733,   541,   468,   233,
     381,   -77,   999,   907,   152,  1042,   378,   155,   152,   -82,
     396,   807,  1040,  1036,   451,   622,   806,  1005,   371,   851,
    -372,   396,   964,   396,   917,   379,   666,   469,  1001,  -189,
     461,   359,   371,  -372,   916,   196,   198,   734,   541,   463,
    -377,  -377,  -377,   255,   423,  -372,   370,  1022,   962,   154,
     367,   368,   369,   264,   250,   298,   970,   462,   466,   977,
     420,  -377,   420,  -377,   499,   746,   848,   316,  -377,   956,
     374,   371,   933,   371,   371,   254,   918,   713,   295,   152,
     521,   522,  -377,  -189,   931,   643,   457,   464,  -377,   389,
     187,   735,  -372,  -377,   254,   955,   179,  -372,   381,   381,
     296,   403,   429,   396,  -117,   573,   371,   874,   726,   464,
     535,   454,   714,   458,   756,   317,   542,   608,   464,   318,
     420,   180,   983,   984,   963,   580,   371,   459,   464,   464,
     429,   429,   254,   187,  -395,   964,   455,   158,   852,   757,
     431,   432,   914,   581,   996,  -377,   582,  1002,   573,   430,
     446,   254,   801,   242,   460,  -395,   617,  1035,   465,  -395,
    -117,  -474,   667,   429,   583,   320,   321,   584,  -473,    52,
     324,   186,   325,   326,   381,   555,   900,   329,   585,   311,
     456,    16,  -395,   205,  -117,   336,   311,   453,  -474,   668,
     689,   340,   341,   342,   362,  -473,   671,   586,   363,   243,
     363,    21,   480,   481,   482,   483,   485,   487,   488,   587,
     489,   490,   491,   494,   495,   535,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   844,  -275,   652,   292,   423,    57,   254,   254,
     261,   716,    21,    50,   254,  -373,   873,   254,   254,   254,
     254,   524,   526,   534,    58,   479,  -387,   -78,   262,   827,
    -275,    76,   544,   720,   716,   -83,   302,   548,   717,   549,
     303,   557,  -373,   178,   311,  -457,   653,  -387,   307,    79,
     250,  -387,   559,   561,    50,  -373,   828,   654,   565,   567,
     381,   982,   140,   572,   572,    58,   576,  -373,   578,   389,
     423,   140,   308,  -456,  -387,   721,   655,  -365,   360,   234,
     361,   610,   921,   140,   312,   140,   722,   313,   614,   547,
      79,   484,   486,     5,  -461,   254,   382,   700,   703,   234,
     267,   271,   516,   517,  -365,   723,   572,   572,   518,   266,
     270,   519,   520,   614,  -373,   382,   254,  -365,   254,  -373,
     496,   742,   635,   614,   638,   646,   640,   642,   140,  -365,
     423,  -342,   269,   269,   250,  -462,   523,   525,  -459,   364,
     316,   587,   453,  -458,   497,   535,   366,   543,  -460,   687,
    -342,   172,   275,   382,   140,  -511,   311,  -511,   140,   873,
     324,   326,  -511,   326,  -511,   747,   749,   329,   560,  -510,
     371,  -510,   382,   629,   566,   631,  -365,   377,   380,   316,
      69,  -365,   391,   443,   433,   445,   498,   913,   317,   804,
     412,   470,   318,   436,   922,   559,   674,   565,   677,   576,
     679,   638,   474,   477,   546,   475,   680,   681,   550,   254,
     295,   254,   295,   372,   551,   688,   140,   690,   552,  -378,
     553,   534,   556,   697,   562,   577,   579,   317,   594,  -378,
     595,   318,   608,   234,   234,   234,   234,   634,   320,  -367,
     612,   408,   618,   324,   619,   623,   326,   647,   633,   639,
     329,   658,   659,   660,   729,   172,  -378,   661,   240,   259,
     260,   264,   275,   298,   663,   662,  -367,   254,   664,   -59,
     382,   382,   704,   234,   705,   979,   234,   320,   707,  -367,
     708,  -378,   324,   130,   325,   326,   140,   725,  -378,   329,
     732,  -367,   130,   140,   737,   234,   743,   336,   580,  1000,
     744,   429,   755,   684,   130,   342,   130,   754,   758,  -378,
     820,   673,   811,   676,  1013,   234,   581,   814,   816,   582,
     841,   845,   847,   850,   316,   862,   534,   857,  -378,   485,
     524,   810,   866,  -378,   813,   868,  -378,   583,  -367,   613,
     584,   878,   300,  -367,  1000,  -100,   382,  -105,   140,   130,
     824,   585,   826,  1003,  1006,   -79,  -104,   831,  -101,  1034,
     834,   731,   140,   836,   613,   837,  -107,   292,   839,   292,
     586,  -103,   317,  -106,   613,   130,   318,  1041,   -80,   130,
    1007,  1000,  1008,  1009,   855,  -102,  1010,  1011,   882,   -81,
     883,   905,   908,   250,   925,   947,  1025,  1026,  1027,  1028,
    1029,  1030,   980,  -382,   -77,   861,   -78,   865,  1032,  1005,
     867,   234,   473,  -382,   875,   940,   181,   625,  -384,   624,
     388,   856,   320,   321,   896,   322,   323,   324,  -384,   325,
     326,   140,   686,   441,   329,   234,   876,   130,   939,   234,
    -382,   935,   336,   843,   484,   523,   715,  1038,   340,   341,
     342,   877,   995,   140,  1019,  -384,  1031,   881,  1024,   140,
     683,   202,   382,   611,   685,  -382,     0,   140,     0,     0,
     824,   826,  -382,   831,   834,   861,   865,     0,     0,     0,
    -384,   897,   898,     0,   899,     0,   534,  -384,   901,   902,
       0,     0,   903,  -382,   245,   246,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   130,  -384,     0,
     920,   638,  -382,     0,   130,     0,   926,  -382,   927,     0,
    -382,   140,   929,   580,     0,   140,     0,  -384,   187,     0,
       0,     0,  -384,   140,     0,  -384,     0,     0,     0,     0,
       0,   581,     0,     0,   582,     0,   897,   941,   942,   901,
     943,   944,   945,   946,     0,     0,     0,     0,     0,   958,
       0,     0,   583,   953,     0,   584,   316,    52,   298,   130,
       0,     0,     0,   961,    16,     0,   585,   968,    20,    21,
       0,     0,     0,   130,     0,     0,     0,    27,     0,    28,
     869,     0,     0,     0,    33,   586,     0,   234,   234,    37,
       0,     0,     0,   234,   234,     0,     0,     0,   991,   992,
     993,   994,     0,     0,   317,   712,     0,     0,   318,    48,
       0,    50,     0,    52,     0,   870,  1018,     0,   871,     0,
      57,   865,    58,  1014,  1015,   298,     0,   140,     0,     0,
       0,     0,     0,     0,   991,   992,   993,   994,  1014,  1015,
       0,    74,   130,     0,    76,     0,   739,    79,     0,     0,
    1020,   638,     0,  1023,   320,   321,     0,   322,   323,   324,
       0,   325,   326,   327,   130,   328,   329,   330,   331,     0,
     130,   333,   334,   335,   336,   337,     0,   339,   130,     0,
     340,   341,   342,   140,   638,     0,     0,     0,   865,   140,
     124,     0,     0,   257,     0,     0,    92,     0,     0,   124,
       0,     0,   872,     0,     0,     0,     0,   234,   234,     0,
       0,   124,     0,   124,     0,   234,     5,   408,     0,     0,
       0,     0,     0,     0,   408,     0,     0,   234,    14,     0,
     234,     0,   130,     0,     0,    19,   130,   316,     0,     0,
       0,   140,     0,   140,   130,   911,     0,     0,   140,     0,
       0,   140,     0,   134,    35,     0,   124,     0,   140,    38,
       0,   140,   134,     0,     0,     0,     0,    43,   858,     0,
     316,     0,     0,     0,   134,     0,   134,     0,     0,     0,
     200,    21,   124,   140,     0,   317,   124,    56,     0,   318,
       0,    28,   244,     0,     0,     0,   140,     0,   140,    66,
       0,    37,     0,    69,   669,     0,     0,     0,     0,     0,
      75,     0,     0,     0,   959,     0,     0,     0,   317,   134,
       0,    48,   318,    50,     0,     0,     0,   392,   395,   397,
     401,     0,     0,     0,    58,   320,   321,     0,   322,   323,
     324,     0,   325,   326,   124,   134,     0,   329,   130,   134,
       0,     0,     0,     0,   335,   336,     0,   997,   339,    79,
       0,   340,   341,   342,     0,     0,     0,   425,   320,     0,
     426,     0,   140,   324,     0,   325,   326,     0,   137,   140,
     329,     0,   234,     0,     0,     0,     0,   137,   336,   435,
       0,   140,     0,     0,   340,   341,   342,     0,     0,   137,
       0,   137,   997,   140,   130,     0,     0,   134,     0,   449,
     130,     0,   140,     0,   124,     0,   140,     0,     0,     0,
       0,   124,     0,     0,     0,   408,   408,     0,   805,   408,
     408,     0,     0,     0,     0,     0,     0,   200,    21,   997,
       0,   808,   809,     0,   137,   316,   969,     0,    28,   244,
       0,     0,   973,     0,   408,   140,   408,     0,    37,     0,
       0,  -217,   130,     0,   130,     0,     0,     0,     0,   130,
     137,     0,   130,     0,   137,     0,   124,   134,    48,   130,
      50,     0,   130,     0,   134,  -217,     0,     0,     0,     0,
     124,    58,     0,   317,     0,     0,     0,   318,     0,   140,
     140,   140,   140,     0,   130,   545,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    79,   130,     0,   130,
       0,     0,     0,     0,   140,   140,     0,     0,     0,   563,
       0,     0,   137,   568,     0,     0,     0,     0,     0,   134,
       0,     0,     0,   320,   321,     0,   969,   323,   324,     0,
     325,   326,     0,   134,     0,   329,     0,     0,     0,   124,
       0,     0,     0,   336,     0,     0,     0,     0,     0,   340,
     341,   342,     0,     0,     0,     0,   138,     0,     0,     0,
       0,   124,     0,     0,     0,   138,     0,   124,     0,     0,
       0,     0,     0,   130,     0,   124,     0,   138,     0,   138,
     130,     0,   137,     0,     0,     0,     0,     0,     0,   137,
       0,     0,   130,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   134,     0,   130,     0,     0,     0,     0,     0,
       0,     0,     0,   130,     0,     0,     0,   130,     0,     0,
       0,     0,   138,     0,   134,     0,     0,     0,     0,   124,
     134,     0,     0,   124,     0,     0,   316,     0,   134,     0,
       0,   124,     0,     0,   137,     0,     0,     0,   138,     0,
       0,     0,   138,     0,     0,     0,   130,     0,   137,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   692,   694,     0,     0,     0,     0,   699,   702,     0,
       0,     0,     0,     0,   317,     0,     0,     0,   318,     0,
       0,     0,   134,     0,     0,     0,   134,     0,     0,     0,
     130,   130,   130,   130,   134,     0,     0,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   130,   130,   137,     0,     0,
       0,     0,     0,     0,   320,   321,     0,   322,   323,   324,
       0,   325,   326,   327,     0,   124,   329,   330,   331,   137,
       0,   333,   334,   335,   336,   137,     0,   339,     0,     0,
     340,   341,   342,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,   138,     0,     0,
       0,   818,   819,     0,     0,     0,     0,     0,     0,   821,
     206,   124,     0,     0,     0,     0,     0,   124,   134,     0,
       0,   832,   207,   208,   835,     0,     0,   137,     0,   209,
       0,   137,     0,     0,     0,     0,     0,   210,     0,   137,
       0,     0,     0,   211,     0,     0,     0,     0,   212,     0,
     213,     0,   138,   214,     0,     0,     0,     0,     0,     0,
       0,   215,     0,     0,     0,     0,   138,     0,     0,   124,
       0,   124,   216,     0,   134,     0,   124,     0,     0,   124,
     134,   217,     0,     0,     0,     0,   124,   142,     0,   124,
     218,   219,     0,   220,     0,   221,   142,   222,     0,     0,
     223,     0,     0,     0,   224,   414,     0,   225,   142,     0,
     142,   124,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   124,     0,   124,     0,     0,     0,
       0,     0,   134,     0,   134,   138,     0,     0,     0,   134,
       0,     0,   134,   137,     0,     0,     0,     0,     0,   134,
       0,     0,   134,   142,     0,     0,     0,   138,     0,     0,
       0,     0,     0,   138,     0,   415,     0,     0,     0,     0,
       0,   138,     0,     0,   134,     0,   924,     0,     0,   142,
       0,     0,     0,   142,     0,     0,     0,   134,     0,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   137,
     124,     0,     0,     0,     0,   137,     0,   124,     0,     0,
       0,     0,     0,   143,     0,     0,     0,     0,     0,   124,
       0,     0,   143,     0,     0,   138,     0,     0,     0,   138,
       0,   124,     0,     0,   143,     0,   143,   138,     0,     0,
     124,   142,     0,     0,   124,     0,     0,     0,     0,     0,
       0,     0,     0,   206,     0,     0,     0,   137,     0,   137,
       0,     0,     0,   134,   137,   207,   208,   137,     0,     0,
     134,     0,   209,     0,   137,     0,     0,   137,     0,   143,
     210,     0,   134,   124,     0,     0,   211,   226,     0,     0,
       0,   212,     0,   213,   134,     0,   214,     0,     0,   137,
       0,     0,     0,   134,   215,   143,     0,   134,     0,   143,
       0,   142,   137,     0,   137,   216,     0,     0,   142,     0,
       0,     0,   273,     0,   217,     0,     0,   124,   124,   124,
     124,     0,     0,   218,   219,     0,   220,     0,   221,     0,
     222,   138,     0,   223,     0,     0,   134,   224,     0,     0,
     225,     0,   124,   124,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,   153,   143,     0,   305,
       0,   306,     0,   142,     0,     0,     0,     0,   182,     0,
     185,     0,     0,     0,     0,     0,     0,   142,   137,     0,
     134,   134,   134,   134,     0,   137,     0,   138,     0,     0,
       0,     0,     0,   138,     0,     0,   371,   137,   738,     0,
       0,     0,     0,     0,     0,   134,   134,     0,     0,   137,
       0,     0,     0,   263,     0,     0,   273,     0,   137,     0,
       0,     0,   137,     0,     0,     0,   314,   143,     0,   273,
       0,     0,   390,   315,   143,     0,     0,     0,     0,    99,
       0,     0,     0,   301,   316,   138,   142,   138,     0,     0,
       0,     0,   138,     0,     0,   138,     0,     0,     0,     0,
       0,   137,   138,     0,     0,   138,     0,   416,   142,     0,
       0,     0,     0,     0,   142,     0,     0,     0,     0,     0,
       0,     0,   142,     0,     0,   428,     0,   138,     0,   143,
       0,     0,   317,     0,     0,     0,   318,     0,     0,     0,
     138,   365,   138,   143,     0,   137,   137,   137,   137,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,   319,     0,     0,     0,
     137,   137,     0,     0,     0,     0,   142,     0,     0,     0,
     142,     0,   320,   321,     0,   322,   323,   324,   142,   325,
     326,   327,     0,   328,   329,   330,   331,   332,     0,   333,
     334,   335,   336,   337,   338,   339,     0,     0,   340,   341,
     342,   411,   143,     0,     0,     0,   138,   748,   424,     0,
       0,     0,     0,   138,     0,     0,     0,     0,     0,     0,
       0,   752,     0,     0,   143,   138,     0,     0,     0,     0,
     143,     0,     0,     0,     0,     0,     0,   138,   143,  -203,
       0,     0,     0,  -203,  -203,     0,   138,     0,     0,     0,
     138,     0,  -203,     0,  -203,  -203,   554,     0,     0,  -203,
       0,     0,     0,    99,  -203,     0,     0,  -203,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
       0,     0,   142,   416,  -203,     0,  -203,     0,  -203,   138,
    -203,  -203,   143,  -203,     0,  -203,   143,  -203,     0,     0,
       0,     0,     0,     0,   143,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -203,     0,     0,  -203,
       0,     0,  -203,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   138,   138,   138,   138,     0,   142,     0,
       0,     0,   645,     0,   142,   649,    99,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   138,   138,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,  -203,     0,   314,   424,     0,     0,  -203,     0,     0,
     315,     0,     0,     0,     0,     0,     0,   895,     0,     0,
       0,   316,   701,     0,     0,     0,   142,     0,   142,     0,
       0,     0,     0,   142,     0,  -203,   142,     0,   143,  -203,
    -203,     0,     0,   142,     0,     0,   142,     0,  -203,     0,
    -203,  -203,     0,     0,     0,  -203,     0,     0,     0,     0,
    -203,     0,     0,  -203,     0,     0,   632,     0,   142,   317,
     637,     0,     0,   318,     0,     0,     0,     0,    99,     0,
    -203,   142,  -203,   142,  -203,     0,  -203,  -203,   719,  -203,
       0,  -203,   416,  -203,   143,     0,     0,     0,     0,   396,
     143,     0,     0,   319,     0,     0,     0,     0,     0,     0,
       0,     0,  -203,     0,     0,  -203,     0,   740,  -203,   320,
     321,   273,   322,   323,   324,     0,   325,   326,   327,     0,
     328,   329,   330,   331,   332,     0,   333,   334,   335,   336,
     337,   338,   339,     0,     0,   340,   341,   342,     0,   371,
       0,     0,   143,     0,   143,   800,     0,   142,     0,   143,
       0,     0,   143,     0,   142,     0,     0,  -203,     0,   143,
       0,     0,   143,  -203,     0,     0,   142,   206,     0,     0,
       0,     0,   710,     0,     0,     0,     0,     0,   142,   207,
     208,     0,     0,     0,   143,     0,   209,   142,     0,     0,
       0,   142,     0,     0,   210,     0,     0,   143,     0,   143,
     211,     0,     0,   842,     0,   212,   416,   213,     0,     0,
     214,     0,     0,     0,     0,     0,     0,     0,   215,     0,
       0,     0,     0,     0,     0,     0,   740,     0,   745,   216,
     142,     0,     0,     0,   750,     0,     0,     0,   217,     0,
       0,     0,     0,     0,   649,     0,     0,   218,   219,     0,
     220,     0,   221,     0,   222,     0,     0,   223,     0,     0,
       0,   224,     0,     0,   225,     0,   880,     0,     0,     0,
       0,     0,     0,   143,   142,   142,   142,   142,   206,     0,
     143,     0,     0,     0,     0,     0,   424,     0,   424,     0,
     207,   208,   143,   424,     0,     0,   424,   209,     0,   142,
     142,     0,     0,   838,   143,   210,   840,     0,     0,     0,
       0,   211,     0,   143,     0,     0,   212,   143,   213,     0,
       0,   214,   229,     0,     0,     0,     0,     0,   854,   215,
       0,     0,    16,     0,     0,     0,    20,    21,     0,     0,
     216,   860,     0,   864,     0,    27,     0,    28,   869,   217,
       0,     0,    33,     0,     0,     0,   143,    37,   218,   219,
       0,   220,     0,   221,     0,   222,     0,     0,   223,     0,
       0,   800,   224,     0,     0,   225,     0,    48,     0,    50,
       0,   300,     0,   870,     0,     0,   871,     0,    57,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     143,   143,   143,   143,     0,     0,     0,     0,     0,    74,
       0,     0,    76,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   143,   143,     0,     0,     0,
       0,     0,     0,   718,     0,     0,   906,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   923,     0,
       0,     0,     0,     0,     0,     0,     0,   928,     0,     0,
       0,   930,     0,     0,    92,     0,     0,    -2,     4,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,    26,     0,    27,
     954,    28,    29,    30,    31,    32,    33,    34,    35,     0,
      36,    37,     0,    38,  -217,     0,    39,    40,    41,     0,
      42,    43,    44,   -43,    45,    46,     0,    47,     0,     0,
       0,    48,    49,    50,    51,    52,    53,    54,  -217,   -43,
      55,    56,    57,     0,    58,    59,    60,     0,    61,    62,
      63,    64,    65,    66,    67,    68,     0,    69,    70,     0,
      71,    72,    73,    74,    75,     0,    76,    77,    78,    79,
       0,     0,    80,     0,    81,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,    87,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,    90,    91,     0,    92,     0,
       0,    93,     0,    94,     0,    95,  1016,    96,     5,     6,
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
       0,    83,    84,    85,    86,    87,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,    90,    91,   247,     0,     0,     0,    93,
       0,   165,     0,    95,     0,   166,  1017,     4,     0,     5,
       6,     7,     8,     9,    10,     0,  -533,     0,    11,    12,
      13,    14,    15,  -533,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,  -533,    25,    26,  -533,    27,     0,
      28,    29,    30,    31,    32,    33,    34,    35,     0,    36,
      37,     0,    38,  -217,     0,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,    47,     0,     0,     0,
      48,    49,    50,    51,     0,    53,    54,  -217,   -43,    55,
      56,    57,  -533,    58,    59,    60,  -533,    61,    62,    63,
      64,    65,    66,    67,    68,     0,    69,    70,     0,    71,
      72,    73,    74,    75,     0,    76,    77,    78,    79,     0,
       0,    80,     0,    81,     0,     0,  -533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -533,  -533,    85,  -533,  -533,  -533,  -533,  -533,
    -533,  -533,     0,  -533,  -533,  -533,  -533,  -533,     0,  -533,
    -533,  -533,  -533,  -533,  -533,  -533,     0,    92,  -533,  -533,
    -533,     0,    94,  -533,    95,   285,    96,     5,     6,     7,
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
      83,    84,    85,    86,    87,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,    90,    91,   247,    92,     0,     0,    93,     0,
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
      85,    86,    87,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
      90,    91,     0,    92,     0,     0,    93,     0,    94,   452,
      95,   471,    96,     5,     6,     7,     8,     9,    10,     0,
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
      87,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,    90,    91,
       0,    92,     0,     0,    93,     0,    94,   472,    95,   285,
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
       0,     0,     0,     0,    83,    84,    85,    86,    87,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,    90,    91,     0,    92,
       0,     0,    93,     0,    94,   286,    95,     4,    96,     5,
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
       0,     0,    83,    84,    85,    86,    87,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,    90,    91,     0,    92,     0,     0,
      93,     0,    94,   644,    95,     4,    96,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,    37,     0,
      38,  -217,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,     0,     0,     0,    48,    49,
      50,    51,   300,    53,    54,  -217,   -43,    55,    56,    57,
       0,    58,    59,    60,     0,    61,    62,    63,    64,    65,
      66,    67,    68,     0,    69,    70,     0,    71,    72,    73,
      74,    75,     0,    76,    77,    78,    79,     0,     0,    80,
       0,    81,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,    87,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,    90,    91,     0,    92,     0,     0,    93,     0,
      94,     0,    95,     4,    96,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,    25,    26,     0,    27,     0,    28,    29,    30,    31,
      32,    33,    34,    35,     0,    36,    37,     0,    38,  -217,
       0,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,     0,     0,     0,    48,    49,    50,    51,
       0,    53,    54,  -217,   -43,    55,    56,    57,     0,    58,
      59,    60,     0,    61,    62,    63,    64,    65,    66,    67,
      68,     0,    69,    70,     0,    71,    72,    73,    74,    75,
       0,    76,    77,    78,    79,     0,     0,    80,     0,    81,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,    87,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
      90,    91,     0,    92,     0,     0,    93,     0,    94,     0,
      95,     4,    96,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
     558,    35,     0,    36,    37,     0,    38,  -217,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,     0,     0,     0,    48,    49,    50,    51,     0,    53,
      54,  -217,   -43,    55,    56,    57,     0,    58,    59,    60,
       0,    61,    62,    63,    64,    65,    66,    67,    68,     0,
      69,    70,     0,    71,    72,    73,    74,    75,     0,    76,
      77,    78,    79,     0,     0,    80,     0,    81,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
      87,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,    90,    91,
       0,    92,     0,     0,    93,     0,    94,     0,    95,     4,
      96,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,   564,    35,
       0,    36,    37,     0,    38,  -217,     0,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,    47,     0,
       0,     0,    48,    49,    50,    51,     0,    53,    54,  -217,
     -43,    55,    56,    57,     0,    58,    59,    60,     0,    61,
      62,    63,    64,    65,    66,    67,    68,     0,    69,    70,
       0,    71,    72,    73,    74,    75,     0,    76,    77,    78,
      79,     0,     0,    80,     0,    81,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,    87,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,    90,    91,     0,    92,
       0,     0,    93,     0,    94,     0,    95,     4,    96,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,    25,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,   823,    35,     0,    36,
      37,     0,    38,  -217,     0,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,    47,     0,     0,     0,
      48,    49,    50,    51,     0,    53,    54,  -217,   -43,    55,
      56,    57,     0,    58,    59,    60,     0,    61,    62,    63,
      64,    65,    66,    67,    68,     0,    69,    70,     0,    71,
      72,    73,    74,    75,     0,    76,    77,    78,    79,     0,
       0,    80,     0,    81,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,    87,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,    90,    91,     0,    92,     0,     0,
      93,     0,    94,     0,    95,     4,    96,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,   825,    35,     0,    36,    37,     0,
      38,  -217,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,     0,     0,     0,    48,    49,
      50,    51,     0,    53,    54,  -217,   -43,    55,    56,    57,
       0,    58,    59,    60,     0,    61,    62,    63,    64,    65,
      66,    67,    68,     0,    69,    70,     0,    71,    72,    73,
      74,    75,     0,    76,    77,    78,    79,     0,     0,    80,
       0,    81,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,    87,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,    90,    91,     0,    92,     0,     0,    93,     0,
      94,     0,    95,     4,    96,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,    25,    26,     0,    27,     0,    28,    29,    30,    31,
      32,    33,   830,    35,     0,    36,    37,     0,    38,  -217,
       0,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,     0,     0,     0,    48,    49,    50,    51,
       0,    53,    54,  -217,   -43,    55,    56,    57,     0,    58,
      59,    60,     0,    61,    62,    63,    64,    65,    66,    67,
      68,     0,    69,    70,     0,    71,    72,    73,    74,    75,
       0,    76,    77,    78,    79,     0,     0,    80,     0,    81,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,    87,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
      90,    91,     0,    92,     0,     0,    93,     0,    94,     0,
      95,     4,    96,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
     833,    35,     0,    36,    37,     0,    38,  -217,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,     0,     0,     0,    48,    49,    50,    51,     0,    53,
      54,  -217,   -43,    55,    56,    57,     0,    58,    59,    60,
       0,    61,    62,    63,    64,    65,    66,    67,    68,     0,
      69,    70,     0,    71,    72,    73,    74,    75,     0,    76,
      77,    78,    79,     0,     0,    80,     0,    81,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
      87,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,    90,    91,
       0,    92,     0,     0,    93,     0,    94,     0,    95,     4,
      96,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,   853,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,    34,    35,
       0,    36,    37,     0,    38,  -217,     0,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,    47,     0,
       0,     0,    48,    49,    50,    51,     0,    53,    54,  -217,
     -43,    55,    56,    57,     0,    58,    59,    60,     0,    61,
      62,    63,    64,    65,    66,    67,    68,     0,    69,    70,
       0,    71,    72,    73,    74,    75,     0,    76,    77,    78,
      79,     0,     0,    80,     0,    81,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,    87,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,    90,    91,     0,    92,
       0,     0,    93,     0,    94,     0,    95,     4,    96,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,    25,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,   859,    35,     0,    36,
      37,     0,    38,  -217,     0,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,    47,     0,     0,     0,
      48,    49,    50,    51,     0,    53,    54,  -217,   -43,    55,
      56,    57,     0,    58,    59,    60,     0,    61,    62,    63,
      64,    65,    66,    67,    68,     0,    69,    70,     0,    71,
      72,    73,    74,    75,     0,    76,    77,    78,    79,     0,
       0,    80,     0,    81,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,    87,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,    90,    91,     0,    92,     0,     0,
      93,     0,    94,     0,    95,     4,    96,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,   863,    35,     0,    36,    37,     0,
      38,  -217,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,     0,     0,     0,    48,    49,
      50,    51,     0,    53,    54,  -217,   -43,    55,    56,    57,
       0,    58,    59,    60,     0,    61,    62,    63,    64,    65,
      66,    67,    68,     0,    69,    70,     0,    71,    72,    73,
      74,    75,     0,    76,    77,    78,    79,     0,     0,    80,
       0,    81,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,    87,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,    90,    91,     0,    92,     0,     0,    93,     0,
      94,     0,    95,   728,    96,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   160,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   163,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,    87,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
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
      87,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,    90,    91,
     247,     0,     0,     0,    93,     0,   165,     0,    95,     0,
     166,   636,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   160,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,    87,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,    90,    91,   247,
       0,     0,     0,    93,     0,   165,     0,    95,     0,   166,
     957,     5,     6,     7,     8,     9,    10,     0,     0,     0,
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
     760,     0,   761,     0,    83,    84,    85,    86,    87,   762,
       0,     0,     0,    88,   763,   208,   764,     0,     0,     0,
       0,   765,     0,    89,     0,     0,    90,    91,     0,   210,
       0,     0,    93,     0,   165,   766,    95,     0,   166,   641,
     767,     0,   213,     0,     0,   768,     0,   769,     0,     0,
       0,     0,     0,   770,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   216,     0,     0,     0,     0,     0,
       0,     0,     0,   771,     0,     0,     0,     0,     0,     0,
       0,     0,   218,   219,     0,   772,     0,   221,     0,   773,
       0,     0,   774,     0,     0,     0,   775,     0,     0,   225,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     343,   344,   345,   346,   347,   348,     0,     0,   351,   352,
     353,   354,     0,   356,   357,   776,   777,   778,   779,   780,
       0,     0,   781,     0,     0,     0,   782,   783,   784,   785,
     786,   787,   788,   789,   790,   791,   792,     0,   793,     0,
       0,   794,   795,   796,   797,     0,     0,   798,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   160,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,   200,    21,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
     201,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,    50,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,    58,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,    79,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,    87,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,    90,    91,     0,     0,     0,     0,    93,
       0,   165,     0,    95,     0,   166,     5,     6,     7,     8,
     189,    10,   190,     0,     0,   160,     0,     0,    14,    15,
       0,     0,  -116,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,  -116,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   164,     0,    69,     0,     0,    71,     0,     0,   191,
      75,     0,  -116,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,    87,     0,     0,     0,     0,    88,     0,
       0,     5,     6,     7,     8,     9,    10,     0,    89,     0,
     160,    90,    91,    14,    15,     0,     0,    93,     0,   165,
      19,    95,     0,   166,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   161,   162,     0,   163,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,   251,   252,     0,    75,   290,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,    87,     0,
       0,     0,     0,    88,   291,     0,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,    90,    91,   247,     0,
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
     251,   252,     0,    75,   290,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,    87,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,    90,    91,   247,     0,     0,     0,
      93,     0,   165,     0,    95,   628,   166,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   160,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
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
      83,    84,    85,    86,    87,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,    90,    91,   247,     0,     0,     0,    93,     0,
     165,     0,    95,   630,   166,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   160,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,   527,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   163,    35,   528,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,   529,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,   530,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,    87,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
      90,    91,     0,     0,     0,     0,    93,     0,   165,     0,
      95,     0,   166,     5,     6,     7,     8,   189,    10,     0,
       0,     0,   160,     0,     0,    14,    15,     0,     0,  -116,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,  -116,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,     0,     0,   191,    75,     0,  -116,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
      87,     0,     0,     0,     0,    88,     0,     0,     5,     6,
       7,     8,     9,    10,     0,    89,     0,   160,    90,    91,
      14,    15,     0,     0,    93,     0,   165,    19,    95,     0,
     166,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,   251,
     252,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,    87,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,    90,    91,     0,   253,     0,     0,    93,
       0,   165,     0,    95,     0,   166,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   160,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   164,     0,    69,     0,     0,    71,   251,   252,     0,
      75,     0,     0,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,    87,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,    90,    91,   247,     0,     0,     0,    93,     0,   165,
       0,    95,     0,   166,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   160,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
     730,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   164,
       0,    69,     0,     0,    71,   251,   252,     0,    75,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,    87,     0,     0,     0,     0,    88,     0,     0,     5,
       6,     7,     8,     9,    10,     0,    89,     0,   160,    90,
      91,    14,    15,     0,     0,    93,     0,   165,    19,    95,
       0,   166,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,   194,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,   195,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,    87,     0,     0,     0,
       0,    88,     0,     0,     5,     6,     7,     8,     9,    10,
       0,    89,     0,   160,    90,    91,    14,    15,     0,     0,
      93,     0,   165,    19,    95,     0,   166,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   164,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,   195,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,    87,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,    90,
      91,   247,     0,     0,     0,    93,     0,   165,     0,    95,
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
       0,     0,     0,     0,     0,    83,    84,    85,    86,    87,
       0,     0,     0,     0,    88,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    89,     0,   160,    90,    91,    14,
      15,     0,     0,    93,     0,   165,    19,    95,     0,   166,
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
      83,    84,    85,    86,    87,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,    90,    91,   247,     0,     0,     0,    93,     0,
     165,   626,    95,     0,   166,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   160,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   163,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
     195,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,    87,     0,     0,     0,     0,    88,     0,     0,
       5,     6,     7,     8,     9,    10,     0,    89,     0,   160,
      90,    91,    14,    15,     0,     0,    93,     0,   165,    19,
      95,     0,   166,     0,     0,     0,   233,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   164,     0,    69,     0,     0,
      71,     0,     0,     0,    75,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,    87,     0,     0,
       0,     0,    88,     0,     0,     5,     6,     7,     8,     9,
      10,     0,    89,     0,   160,    90,    91,    14,    15,     0,
       0,    93,     0,    94,    19,    95,     0,   166,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   163,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,    87,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
      90,    91,   247,     0,     0,     0,    93,     0,   165,     0,
      95,     0,   166,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   160,     0,     0,    14,   596,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
     597,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,   598,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,   599,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,   600,   601,    65,    66,    67,   602,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
     603,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
      87,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,    90,    91,
     247,     0,     0,     0,    93,     0,   165,     0,    95,     0,
     909,     5,     6,     7,     8,     9,    10,     0,     0,     0,
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
       0,     0,     0,     0,    83,    84,    85,    86,    87,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,    90,    91,   247,     0,
       0,     0,    93,     0,   165,     0,    95,     0,   909,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   160,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   894,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,    87,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,    90,    91,   247,     0,     0,     0,
      93,     0,   165,     0,    95,     0,   166,     5,     6,     7,
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
      83,    84,    85,    86,    87,     0,     0,     0,     0,    88,
       0,     0,     5,     6,     7,     8,     9,    10,     0,    89,
       0,   160,    90,    91,    14,    15,     0,     0,    93,     0,
     165,    19,    95,     0,   166,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,    87,
       0,     0,     0,     0,    88,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    89,     0,   160,    90,    91,    14,
     492,     0,     0,    93,     0,    94,    19,    95,     0,   166,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,   493,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,    87,     0,     0,     0,     0,    88,
       0,     0,     5,     6,     7,     8,     9,    10,     0,    89,
       0,   160,    90,    91,    14,    15,     0,     0,    93,     0,
     165,    19,    95,     0,   166,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,    87,
       0,     0,     0,     0,    88,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    89,     0,   160,    90,    91,    14,
     596,     0,     0,   569,     0,   165,    19,    95,     0,   166,
       0,     0,     0,     0,   597,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,     0,   598,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,   599,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,   600,   601,    65,
      66,    67,   602,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,   603,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,    87,     0,     0,     0,     0,    88,
       0,     0,     5,     6,     7,     8,     9,    10,     0,    89,
       0,   160,    90,    91,    14,    15,     0,     0,    93,     0,
     165,    19,    95,     0,   604,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,    87,
       0,     0,     0,     0,    88,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    89,     0,   160,    90,    91,    14,
      15,     0,     0,    93,     0,   165,    19,    95,     0,   604,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   672,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,    87,     0,     0,     0,     0,    88,
       0,     0,     5,     6,     7,     8,     9,    10,     0,    89,
       0,   160,    90,    91,    14,    15,     0,     0,    93,     0,
     165,    19,    95,     0,   166,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   675,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,    87,
       0,     0,     0,     0,    88,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    89,     0,   160,    90,    91,    14,
      15,     0,     0,    93,     0,   165,    19,    95,     0,   166,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   888,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,    87,     0,     0,     0,     0,    88,
       0,     0,     5,     6,     7,     8,     9,    10,     0,    89,
       0,   160,    90,    91,    14,    15,     0,     0,    93,     0,
     165,    19,    95,     0,   166,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   889,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,    87,
       0,     0,     0,     0,    88,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    89,     0,   160,    90,    91,    14,
      15,     0,     0,    93,     0,   165,    19,    95,     0,   166,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   891,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,    87,     0,     0,     0,     0,    88,
       0,     0,     5,     6,     7,     8,     9,    10,     0,    89,
       0,   160,    90,    91,    14,    15,     0,     0,    93,     0,
     165,    19,    95,     0,   166,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   892,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,    87,
       0,     0,     0,     0,    88,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    89,     0,   160,    90,    91,    14,
      15,     0,     0,    93,     0,   165,    19,    95,     0,   166,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   893,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,    87,     0,     0,     0,     0,    88,
       0,     0,     5,     6,     7,     8,     9,    10,     0,    89,
       0,   160,    90,    91,    14,    15,     0,     0,    93,     0,
     165,    19,    95,     0,   166,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   894,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,    87,
       0,     0,     0,     0,    88,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    89,     0,   160,    90,    91,    14,
     596,     0,     0,    93,     0,   165,    19,    95,     0,   166,
       0,     0,     0,     0,   597,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,     0,   598,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,   599,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,   600,   601,    65,
      66,    67,   602,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,   603,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,    87,     0,     0,     0,     0,    88,
       0,     0,     5,     6,     7,     8,     9,    10,     0,    89,
       0,   160,    90,    91,    14,    15,     0,     0,    93,     0,
     165,    19,    95,     0,   976,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,    87,
       0,     0,     0,     0,    88,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    89,     0,   160,    90,    91,    14,
      15,     0,     0,    93,     0,     0,    19,    95,     0,   976,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,     0,     0,     0,
      83,    84,    85,    86,    87,     0,     0,     0,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   160,     0,    89,
      14,    15,     0,    91,     0,     0,   358,    19,    93,  -510,
     165,  -510,    95,     0,   166,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -386,     0,     0,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   160,     0,
       0,    14,    15,    85,     0,     0,     0,  -386,    19,     0,
       0,  -386,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   161,   162,   247,   163,    35,     0,    36,
       0,   165,    38,    95,  -386,   909,    40,    41,  -368,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,  -368,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,  -368,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
    -368,     0,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     5,     6,     7,     8,
       9,    10,     0,  -481,     0,   160,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,   358,  -368,     0,  -510,
       0,  -510,  -368,    26,    95,     0,   976,     0,     0,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   164,     0,    69,     0,     0,    71,     0,     0,     0,
      75,     0,     0,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   160,     0,     0,    14,
      15,    85,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,     0,    36,     0,   165,
      38,    95,     0,   166,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   160,     0,     0,
      14,    15,    85,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
     165,    38,    95,     0,   604,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,   937,    63,    64,
      65,    66,    67,   164,   762,    69,     0,     0,    71,   207,
     208,   764,    75,     0,     0,    77,   209,     0,     0,     0,
       0,     0,     0,     0,   210,     0,     0,     0,     0,     0,
     766,     0,     0,     0,     0,   212,     0,   213,     0,     0,
     214,     0,   769,    85,     0,     0,     0,     0,   215,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   216,
       0,     0,     0,     0,     0,     0,     0,     0,   217,     0,
       0,     0,     0,    95,     0,   166,     0,   218,   219,     0,
     220,     0,   221,     0,   938,   314,     0,   774,     0,     0,
       0,   224,   315,     0,   225,     0,     0,     0,     0,     0,
       0,     0,     0,   316,     0,   343,   344,   345,   346,   347,
     348,     0,     0,   351,   352,   353,   354,     0,   356,   357,
     776,   777,   778,   779,   780,     0,     0,   781,     0,     0,
       0,   782,   783,   784,   785,   786,   787,   788,   789,   790,
     791,   792,     0,   793,     0,   316,   794,   795,   796,   797,
       0,   317,     0,     0,     0,   318,     0,     0,     0,     0,
     314,     0,     0,     0,     0,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,     0,     0,   319,     0,     0,     0,     0,
       0,     0,     0,   317,     0,     0,     0,   318,     0,     0,
       0,   320,   321,     0,   322,   323,   324,     0,   325,   326,
     327,     0,   328,   329,   330,   331,   332,     0,   333,   334,
     335,   336,   337,   338,   339,     0,   317,   340,   341,   342,
     318,     0,     0,     0,     0,   314,   817,     0,     0,     0,
       0,     0,   315,   320,   321,     0,   322,   323,   324,     0,
     325,   326,   327,   316,     0,   329,     0,     0,     0,     0,
     319,     0,   335,   336,     0,     0,   339,     0,     0,   340,
     341,   342,     0,     0,     0,     0,   320,   321,     0,   322,
     323,   324,     0,   325,   326,   327,     0,   328,   329,   330,
     331,   332,     0,   333,   334,   335,   336,   337,   338,   339,
       0,   317,   340,   341,   342,   318,     0,     0,     0,     0,
     314,   972,     0,     0,     0,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,     0,     0,   319,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   320,   321,     0,   322,   323,   324,     0,   325,   326,
     327,     0,   328,   329,   330,   331,   332,     0,   333,   334,
     335,   336,   337,   338,   339,     0,   317,   340,   341,   342,
     318,     0,     0,     0,     0,   314,  1039,     0,     0,     0,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   316,     0,     0,     0,     0,     0,     0,
     319,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   320,   321,     0,   322,
     323,   324,     0,   325,   326,   327,     0,   328,   329,   330,
     331,   332,     0,   333,   334,   335,   336,   337,   338,   339,
       0,   317,   340,   341,   342,   318,     0,     0,     0,   627,
     314,     0,     0,     0,     0,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,     0,     0,   319,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   320,   321,     0,   322,   323,   324,     0,   325,   326,
     327,     0,   328,   329,   330,   331,   332,     0,   333,   334,
     335,   336,   337,   338,   339,     0,   317,   340,   341,   342,
     318,     0,     0,     0,   936,   314,     0,     0,     0,   407,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   316,     0,     0,     0,     0,     0,     0,
     319,   343,   344,   345,   346,   347,   348,     0,     0,   351,
     352,   353,   354,     0,   356,   357,   320,   321,     0,   322,
     323,   324,     0,   325,   326,   327,     0,   328,   329,   330,
     331,   332,     0,   333,   334,   335,   336,   337,   338,   339,
       0,   317,   340,   341,   342,   318,   371,     0,     0,     0,
     314,     0,     0,     0,   822,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,     0,     0,   319,   343,   344,   345,   346,
     347,   348,     0,     0,   351,   352,   353,   354,     0,   356,
     357,   320,   321,     0,   322,   323,   324,     0,   325,   326,
     327,     0,   328,   329,   330,   331,   332,     0,   333,   334,
     335,   336,   337,   338,   339,     0,   317,   340,   341,   342,
     318,   371,     0,     0,     0,   314,     0,     0,     0,   829,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   316,     0,     0,     0,     0,     0,     0,
     319,   343,   344,   345,   346,   347,   348,     0,     0,   351,
     352,   353,   354,     0,   356,   357,   320,   321,     0,   322,
     323,   324,     0,   325,   326,   327,     0,   328,   329,   330,
     331,   332,     0,   333,   334,   335,   336,   337,   338,   339,
       0,   317,   340,   341,   342,   318,   371,     0,     0,     0,
     314,     0,     0,     0,   949,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,     0,     0,   319,   343,   344,   345,   346,
     347,   348,     0,     0,   351,   352,   353,   354,     0,   356,
     357,   320,   321,     0,   322,   323,   324,     0,   325,   326,
     327,     0,   328,   329,   330,   331,   332,     0,   333,   334,
     335,   336,   337,   338,   339,     0,   317,   340,   341,   342,
     318,   371,     0,     0,     0,   314,     0,     0,     0,   950,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   316,     0,     0,     0,     0,     0,     0,
     319,   343,   344,   345,   346,   347,   348,     0,     0,   351,
     352,   353,   354,     0,   356,   357,   320,   321,     0,   322,
     323,   324,     0,   325,   326,   327,     0,   328,   329,   330,
     331,   332,     0,   333,   334,   335,   336,   337,   338,   339,
       0,   317,   340,   341,   342,   318,   371,     0,     0,     0,
     314,     0,     0,     0,   951,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,     0,     0,   319,   343,   344,   345,   346,
     347,   348,     0,     0,   351,   352,   353,   354,     0,   356,
     357,   320,   321,     0,   322,   323,   324,     0,   325,   326,
     327,     0,   328,   329,   330,   331,   332,     0,   333,   334,
     335,   336,   337,   338,   339,     0,   317,   340,   341,   342,
     318,   371,     0,     0,     0,   314,     0,     0,     0,   952,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   316,     0,     0,     0,     0,     0,     0,
     319,   343,   344,   345,   346,   347,   348,     0,     0,   351,
     352,   353,   354,     0,   356,   357,   320,   321,     0,   322,
     323,   324,     0,   325,   326,   327,     0,   328,   329,   330,
     331,   332,     0,   333,   334,   335,   336,   337,   338,   339,
       0,   317,   340,   341,   342,   318,   371,     0,     0,     0,
     314,     0,     0,     0,   974,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,     0,     0,   319,   343,   344,   345,   346,
     347,   348,     0,     0,   351,   352,   353,   354,     0,   356,
     357,   320,   321,     0,   322,   323,   324,     0,   325,   326,
     327,     0,   328,   329,   330,   331,   332,     0,   333,   334,
     335,   336,   337,   338,   339,     0,   317,   340,   341,   342,
     318,   371,     0,     0,     0,   314,     0,     0,     0,   975,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   316,   399,     0,     0,     0,     0,     0,
     319,   343,   344,   345,   346,   347,   348,   400,     0,   351,
     352,   353,   354,     0,   356,   357,   320,   321,     0,   322,
     323,   324,     0,   325,   326,   327,     0,   328,   329,   330,
     331,   332,     0,   333,   334,   335,   336,   337,   338,   339,
       0,   317,   340,   341,   342,   318,   371,     0,     0,     0,
     314,     0,     0,     0,     0,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,   393,
       0,   396,     0,     0,     0,   319,     0,     0,     0,     0,
       0,     0,   394,     0,     0,     0,     0,     0,     0,     0,
       0,   320,   321,     0,   322,   323,   324,     0,   325,   326,
     327,     0,   328,   329,   330,   331,   332,     0,   333,   334,
     335,   336,   337,   338,   339,     0,   317,   340,   341,   342,
     318,   371,     0,     0,     0,   314,     0,     0,     0,     0,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   316,   233,     0,     0,     0,     0,     0,
     319,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   320,   321,     0,   322,
     323,   324,     0,   325,   326,   327,     0,   328,   329,   330,
     331,   332,     0,   333,   334,   335,   336,   337,   338,   339,
       0,   317,   340,   341,   342,   318,   371,     0,     0,     0,
     314,     0,     0,     0,     0,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,   693,
       0,     0,     0,     0,     0,   319,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   320,   321,     0,   322,   323,   324,     0,   325,   326,
     327,     0,   328,   329,   330,   331,   332,     0,   333,   334,
     335,   336,   337,   338,   339,     0,   317,   340,   341,   342,
     318,   371,     0,     0,     0,   314,     0,     0,     0,     0,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   316,     0,     0,     0,     0,     0,     0,
     319,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   320,   321,     0,   322,
     323,   324,     0,   325,   326,   327,     0,   328,   329,   330,
     331,   332,     0,   333,   334,   335,   336,   337,   338,   339,
       0,   317,   340,   341,   342,   318,   371,     0,     0,     0,
     314,     0,     0,     0,   711,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,     0,     0,   319,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   320,   321,     0,   322,   323,   324,     0,   325,   326,
     327,     0,   328,   329,   330,   331,   332,     0,   333,   334,
     335,   336,   337,   338,   339,     0,   317,   340,   341,   342,
     318,   371,     0,     0,     0,   314,     0,     0,     0,     0,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   316,     0,     0,     0,     0,   404,     0,
     319,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   320,   321,     0,   322,
     323,   324,     0,   325,   326,   327,   405,   328,   329,   330,
     331,   332,     0,   333,   334,   335,   336,   337,   338,   339,
       0,   317,   340,   341,   342,   318,   434,     0,     0,     0,
       0,     0,     0,     0,   314,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,   319,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   373,     0,     0,     0,
       0,   320,   321,     0,   322,   323,   324,     0,   325,   326,
     327,     0,   328,   329,   330,   331,   332,     0,   333,   334,
     335,   336,   337,   338,   339,     0,   406,   340,   341,   342,
     317,     0,     0,     0,   318,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   316,     0,     0,
     155,     0,   447,     0,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     320,   321,     0,   322,   323,   324,     0,   325,   326,   327,
     448,   328,   329,   330,   331,   332,     0,   333,   334,   335,
     336,   337,   338,   339,     0,   317,   340,   341,   342,   318,
       0,     0,     0,     0,   314,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,   536,     0,     0,     0,     0,     0,   319,
       0,     0,     0,     0,     0,     0,   537,     0,     0,     0,
       0,     0,     0,     0,     0,   320,   321,     0,   322,   323,
     324,     0,   325,   326,   327,     0,   328,   329,   330,   331,
     332,     0,   333,   334,   335,   336,   337,   338,   339,     0,
     317,   340,   341,   342,   318,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   316,   538,     0,
       0,     0,     0,     0,   319,     0,     0,     0,     0,     0,
       0,   539,     0,     0,     0,     0,     0,     0,     0,     0,
     320,   321,     0,   322,   323,   324,     0,   325,   326,   327,
       0,   328,   329,   330,   331,   332,     0,   333,   334,   335,
     336,   337,   338,   339,     0,   317,   340,   341,   342,   318,
       0,     0,     0,     0,   314,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,     0,     0,     0,     0,   319,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,   321,     0,   322,   323,
     324,     0,   325,   326,   327,     0,   328,   329,   330,   331,
     332,     0,   333,   334,   335,   336,   337,   338,   339,     0,
     317,   340,   341,   342,   318,     0,     0,     0,     0,     0,
       0,     0,     0,   314,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   316,     0,     0,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     320,   321,     0,   322,   323,   324,     0,   325,   326,   327,
       0,   328,   329,   330,   331,   332,     0,   333,   334,   335,
     336,   337,   338,   339,     0,   438,   340,   341,   342,   317,
       0,     0,     0,   318,     0,     0,     0,     0,     0,     0,
       0,     0,   314,     0,     0,     0,     0,     0,     0,   315,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     316,     0,     0,   319,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   320,
     321,     0,   322,   323,   324,     0,   325,   326,   327,     0,
     328,   329,   330,   331,   332,     0,   333,   334,   335,   336,
     337,   338,   339,     0,   440,   340,   341,   342,   317,     0,
       0,     0,   318,     0,     0,     0,     0,     0,     0,     0,
       0,   314,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   316,
       0,     0,   319,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   320,   321,
       0,   322,   323,   324,     0,   325,   326,   327,     0,   328,
     329,   330,   331,   332,     0,   333,   334,   335,   336,   337,
     338,   339,     0,   442,   340,   341,   342,   317,     0,     0,
       0,   318,     0,     0,     0,     0,     0,     0,     0,     0,
     314,     0,     0,     0,     0,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,   319,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   320,   321,     0,
     322,   323,   324,     0,   325,   326,   327,     0,   328,   329,
     330,   331,   332,     0,   333,   334,   335,   336,   337,   338,
     339,     0,   450,   340,   341,   342,   317,     0,     0,     0,
     318,     0,     0,     0,     0,   314,     0,     0,     0,     0,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   316,     0,     0,     0,     0,     0,   467,
     319,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   320,   321,     0,   322,
     323,   324,     0,   325,   326,   327,     0,   328,   329,   330,
     331,   332,     0,   333,   334,   335,   336,   337,   338,   339,
       0,   317,   340,   341,   342,   318,     0,     0,     0,     0,
     314,     0,     0,     0,   540,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,     0,     0,   319,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   320,   321,     0,   322,   323,   324,     0,   325,   326,
     327,     0,   328,   329,   330,   331,   332,     0,   333,   334,
     335,   336,   337,   338,   339,     0,   317,   340,   341,   342,
     318,     0,     0,     0,     0,   314,     0,     0,     0,     0,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   316,     0,     0,     0,     0,     0,     0,
     319,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   320,   321,     0,   322,
     323,   324,     0,   325,   326,   327,     0,   328,   329,   330,
     331,   332,  -482,   333,   334,   335,   336,   337,   338,   339,
       0,   317,   340,   341,   342,   318,     0,     0,     0,     0,
     314,     0,     0,     0,     0,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,   155,     0,     0,     0,   319,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   320,   321,     0,   322,   323,   324,     0,   325,   326,
     327,     0,   328,   329,   330,   331,   332,     0,   333,   334,
     335,   336,   337,   338,   339,     0,   317,   340,   341,   342,
     318,     0,     0,     0,     0,     0,     0,     0,     0,   314,
     706,     0,     0,     0,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   316,     0,     0,
     319,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   320,   321,     0,   322,
     323,   324,     0,   325,   326,   327,     0,   328,   329,   330,
     331,   332,     0,   333,   334,   335,   336,   337,   338,   339,
       0,   682,   340,   341,   342,   317,     0,     0,     0,   318,
       0,     0,     0,     0,   314,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,   709,     0,     0,     0,   319,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,   321,     0,   322,   323,
     324,     0,   325,   326,   327,     0,   328,   329,   330,   331,
     332,     0,   333,   334,   335,   336,   337,   338,   339,     0,
     317,   340,   341,   342,   318,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   316,     0,     0,
       0,     0,     0,     0,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     320,   321,     0,   322,   323,   324,     0,   325,   326,   327,
       0,   328,   329,   330,   331,   332,     0,   333,   334,   335,
     336,   337,   338,   339,     0,   317,   340,   341,   342,   318,
       0,     0,     0,     0,   314,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,   812,     0,     0,     0,     0,   751,   319,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,   321,     0,   322,   323,
     324,     0,   325,   326,   327,     0,   328,   329,   330,   331,
     332,     0,   333,   334,   335,   336,   337,   338,   339,     0,
     317,   340,   341,   342,   318,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   316,   815,     0,
       0,     0,     0,     0,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     320,   321,     0,   322,   323,   324,     0,   325,   326,   327,
       0,   328,   329,   330,   331,   332,     0,   333,   334,   335,
     336,   337,   338,   339,     0,   317,   340,   341,   342,   318,
       0,     0,     0,     0,   314,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,     0,     0,     0,     0,   319,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,   321,     0,   322,   323,
     324,     0,   325,   326,   327,     0,   328,   329,   330,   331,
     332,     0,   333,   334,   335,   336,   337,   338,   339,     0,
     317,   340,   341,   342,   318,     0,     0,     0,     0,   314,
       0,     0,     0,   887,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   316,     0,     0,
       0,     0,     0,     0,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     320,   321,     0,   322,   323,   324,     0,   325,   326,   327,
       0,   328,   329,   330,   331,   332,     0,   333,   334,   335,
     336,   337,   338,   339,     0,   317,   340,   341,   342,   318,
       0,     0,     0,     0,   314,   904,     0,     0,   890,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,     0,     0,     0,     0,   319,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,   321,     0,   322,   323,
     324,     0,   325,   326,   327,     0,   328,   329,   330,   331,
     332,     0,   333,   334,   335,   336,   337,   338,   339,     0,
     317,   340,   341,   342,   318,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   316,     0,     0,
       0,     0,     0,     0,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     320,   321,     0,   322,   323,   324,     0,   325,   326,   327,
       0,   328,   329,   330,   331,   332,     0,   333,   334,   335,
     336,   337,   338,   339,     0,   317,   340,   341,   342,   318,
       0,     0,     0,     0,   314,     0,     0,     0,   985,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,     0,     0,     0,     0,   319,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,   321,     0,   322,   323,
     324,     0,   325,   326,   327,     0,   328,   329,   330,   331,
     332,     0,   333,   334,   335,   336,   337,   338,   339,     0,
     317,   340,   341,   342,   318,     0,     0,     0,     0,   314,
       0,     0,     0,   986,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   316,     0,     0,
       0,     0,     0,     0,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     320,   321,     0,   322,   323,   324,     0,   325,   326,   327,
       0,   328,   329,   330,   331,   332,     0,   333,   334,   335,
     336,   337,   338,   339,     0,   317,   340,   341,   342,   318,
       0,     0,     0,     0,   314,     0,     0,     0,   987,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,     0,     0,     0,     0,   319,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,   321,     0,   322,   323,
     324,     0,   325,   326,   327,     0,   328,   329,   330,   331,
     332,     0,   333,   334,   335,   336,   337,   338,   339,     0,
     317,   340,   341,   342,   318,     0,     0,     0,     0,   314,
       0,     0,     0,   988,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   316,     0,     0,
       0,     0,     0,     0,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     320,   321,     0,   322,   323,   324,     0,   325,   326,   327,
       0,   328,   329,   330,   331,   332,     0,   333,   334,   335,
     336,   337,   338,   339,     0,   317,   340,   341,   342,   318,
       0,     0,     0,     0,   314,     0,     0,     0,   989,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,     0,     0,     0,     0,   319,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,   321,     0,   322,   323,
     324,     0,   325,   326,   327,     0,   328,   329,   330,   331,
     332,     0,   333,   334,   335,   336,   337,   338,   339,     0,
     317,   340,   341,   342,   318,     0,     0,     0,     0,     0,
       0,     0,     0,   990,   314,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,     0,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     320,   321,     0,   322,   323,   324,  1012,   325,   326,   327,
       0,   328,   329,   330,   331,   332,     0,   333,   334,   335,
     336,   337,   338,   339,     0,     0,   340,   341,   342,     0,
     317,     0,     0,     0,   318,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   316,     0,     0,
       0,     0,     0,     0,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     320,   321,     0,   322,   323,   324,     0,   325,   326,   327,
       0,   328,   329,   330,   331,   332,     0,   333,   334,   335,
     336,   337,   338,   339,     0,   317,   340,   341,   342,   318,
       0,     0,     0,     0,   314,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,     0,     0,     0,     0,   319,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,   321,     0,   322,   323,
     324,     0,   325,   326,   327,     0,   328,   329,   330,   331,
     332,     0,   333,   334,   335,   336,   337,   338,   339,     0,
     670,   340,   341,   342,   318,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     320,   321,     0,   322,   323,   324,     0,   325,   326,   327,
       0,   328,   329,   330,   331,   332,   316,   333,   334,   335,
     336,   337,   338,   339,     0,    16,   340,   341,   342,    20,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
      28,   869,     0,     0,     0,    33,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   317,     0,     0,     0,   318,     0,
      48,     0,    50,     0,    52,     0,   870,     0,     0,   871,
       0,    57,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   316,     0,     0,     0,     0,     0,     0,   319,     0,
       0,     0,    74,     0,     0,    76,     0,     0,    79,     0,
       0,     0,     0,     0,   320,   321,     0,   322,   323,   324,
       0,   325,   326,   327,     0,   328,   329,   330,   331,     0,
       0,   333,   334,   335,   336,   337,   338,   339,     0,   317,
     340,   341,   342,   318,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,   948,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   320,
     321,     0,   322,   323,   324,     0,   325,   326,   327,     0,
     328,   329,   330,   331,     0,     0,   333,   334,   335,   336,
     337,   648,   339,   206,     0,   340,   341,   342,     0,     0,
       0,     0,     0,     0,     0,   207,   208,     0,     0,     0,
       0,     0,   209,     0,     0,     0,     0,     0,     0,     0,
     210,     0,     0,     0,     0,     0,   211,     0,     0,     0,
       0,   212,     0,   213,     0,     0,   214,     0,     0,     0,
       0,     0,     0,     0,   215,     0,     0,     0,   206,     0,
       0,     0,     0,     0,     0,   216,     0,     0,     0,     0,
     207,   208,     0,     0,   217,     0,     0,   209,     0,     0,
       0,     0,     0,   218,   219,   210,   220,     0,   221,     0,
     222,   211,     0,   223,     0,     0,   212,   224,   213,     0,
     225,   214,     0,     0,     0,     0,     0,     0,     0,   215,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     216,     0,     0,     0,     0,     0,     0,     0,     0,   217,
       0,     0,     0,     0,     0,     0,     0,     0,   218,   219,
       0,   220,     0,   221,     0,   222,     0,     0,   223,     0,
       0,     0,   224,     0,     0,   225
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-935)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,     2,   140,   443,    15,   412,   178,    18,    54,    55,
      17,   562,   364,    24,   128,   727,    20,    28,    29,   191,
      31,    32,    33,    34,   186,   205,   657,     1,    28,   415,
       1,    42,     1,    44,    45,     3,    47,   595,    49,    59,
      51,    79,     1,   205,    29,     0,    29,    29,    59,    60,
      61,    62,    63,    64,    42,    79,   919,    68,    78,    70,
     199,    72,    73,   243,   753,   179,    77,    78,  1002,    80,
      81,   845,    83,    42,    85,   106,    96,    88,    89,    29,
      91,   243,   262,    94,    95,    96,   803,   804,    62,   104,
       1,    42,   420,    52,  1017,   807,    62,    42,   132,    29,
     262,   126,   957,   842,   151,  1039,   132,   106,   151,   134,
     106,   669,  1035,   151,   157,   443,   667,   132,   156,    42,
      79,   106,    90,   106,    52,   151,    77,   161,    42,   103,
     161,   269,   156,    92,   846,    31,    32,   103,    42,    42,
     111,    52,   111,    60,   132,   104,   166,  1002,     3,   160,
     161,   162,   163,   164,   165,   166,  1019,   296,   297,   933,
     131,   132,   131,   132,   326,   161,   724,    28,    79,   908,
     177,   156,   131,   156,   156,   186,   104,    42,    95,   151,
     360,   361,   151,   157,   873,   157,   132,   132,   159,   193,
     201,   157,   151,   104,   205,   907,   200,   156,   360,   361,
      96,   201,   132,   106,    19,   405,   156,   896,   594,   132,
     562,   132,    77,   159,   132,    76,   161,   111,   132,    80,
     131,    77,   939,   940,    79,    24,   156,   132,   132,   132,
     132,   132,   243,   244,   111,    90,   157,     3,   161,   157,
     251,   252,   136,    42,   956,   156,    45,   161,   448,   151,
     151,   262,   883,     7,   159,   132,   436,   161,   161,   136,
      75,   132,   132,   132,    63,   126,   127,    66,   132,    68,
     131,   158,   133,   134,   436,   389,   827,   138,    77,   150,
     291,    19,   159,   158,    99,   146,   150,   288,   159,   159,
     159,   152,   153,   154,    76,   159,    76,    96,    80,   158,
      80,    24,   313,   314,   315,   316,   317,   318,   319,   716,
     321,   322,   323,   324,   325,   667,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   718,   132,    24,    95,   132,    75,   349,   350,
      87,   132,    24,    66,   355,    52,   753,   358,   359,   360,
     361,   362,   363,   364,    77,   151,   111,   126,   158,   132,
     159,    99,   373,    24,   132,   134,   156,   378,   159,   380,
      72,   391,    79,    96,   150,   151,    66,   132,    47,   102,
     391,   136,   393,   394,    66,    92,   159,    77,   399,   400,
     552,   159,     2,   404,   405,    77,   407,   104,   409,   403,
     132,    11,    71,   151,   159,    66,    96,    52,   158,    42,
     160,   422,   852,    23,   151,    25,    77,   142,   429,   151,
     102,   317,   318,     3,   151,   436,   186,   566,   567,    62,
      72,    73,   349,   350,    79,    96,   447,   448,   355,    72,
      73,   358,   359,   454,   151,   205,   457,    92,   459,   156,
      30,   623,   463,   464,   465,   475,   467,   468,    68,   104,
     132,   132,    72,    73,   475,   151,   362,   363,   151,   158,
      28,   878,   473,   151,    54,   827,   151,   373,   151,   151,
     151,   492,   493,   243,    94,   158,   150,   160,    98,   896,
     131,   134,   158,   134,   160,   634,   635,   138,   394,   158,
     156,   160,   262,   457,   400,   459,   151,   151,   105,    28,
      90,   156,   158,   111,   151,   151,    96,   845,    76,   657,
     158,     7,    80,   158,   852,   536,   537,   538,   539,   540,
     541,   542,    56,   156,   151,   131,   543,   544,   111,   550,
     457,   552,   459,   176,   159,   556,   156,   558,   132,    42,
     151,   562,    42,   564,   158,    31,   159,    76,   132,    52,
     159,    80,   111,   196,   197,   198,   199,   463,   126,    52,
     159,   204,   159,   131,    18,   132,   134,   156,   161,   161,
     138,   151,   151,   151,   604,   596,    79,   151,   599,   600,
     601,   602,   603,   604,   159,   151,    79,   608,   161,   151,
     360,   361,   132,   236,   151,   933,   239,   126,   151,    92,
      31,   104,   131,     2,   133,   134,   226,   159,   111,   138,
     157,   104,    11,   233,   159,   258,   151,   146,    24,   957,
     151,   132,   111,   550,    23,   154,    25,   157,    77,   132,
     156,   537,    29,   539,   972,   278,    42,    29,   161,    45,
      31,   131,    77,   161,    28,   161,   667,   131,   151,   670,
     671,   672,   161,   156,   675,   157,   159,    63,   151,   429,
      66,   158,    68,   156,  1002,   134,   436,   134,   288,    68,
     691,    77,   693,   963,   111,   126,   134,   698,   134,  1017,
     701,   608,   302,   704,   454,   706,   134,   457,   709,   459,
      96,   134,    76,   134,   464,    94,    80,  1035,   126,    98,
     137,  1039,   139,   140,   734,   134,   143,   144,   126,   126,
     134,    31,   159,   734,   159,   157,  1006,  1007,  1008,  1009,
    1010,  1011,    92,    42,   126,   746,   126,   748,   161,   132,
     751,   374,   302,    52,   755,   883,    22,   448,    42,   447,
     192,   738,   126,   127,   820,   129,   130,   131,    52,   133,
     134,   371,   554,   270,   138,   398,   756,   156,   883,   402,
      79,   878,   146,   716,   670,   671,   587,  1019,   152,   153,
     154,   759,   956,   393,   981,    79,  1012,   798,  1005,   399,
     550,    33,   552,   423,   552,   104,    -1,   407,    -1,    -1,
     811,   812,   111,   814,   815,   816,   817,    -1,    -1,    -1,
     104,   822,   823,    -1,   825,    -1,   827,   111,   829,   830,
      -1,    -1,   833,   132,   870,   871,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   132,    -1,
     851,   852,   151,    -1,   233,    -1,   857,   156,   859,    -1,
     159,   461,   863,    24,    -1,   465,    -1,   151,   869,    -1,
      -1,    -1,   156,   473,    -1,   159,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    -1,    45,    -1,   887,   888,   889,   890,
     891,   892,   893,   894,    -1,    -1,    -1,    -1,    -1,   909,
      -1,    -1,    63,   904,    -1,    66,    28,    68,   909,   288,
      -1,    -1,    -1,   914,    19,    -1,    77,   918,    23,    24,
      -1,    -1,    -1,   302,    -1,    -1,    -1,    32,    -1,    34,
      35,    -1,    -1,    -1,    39,    96,    -1,   560,   561,    44,
      -1,    -1,    -1,   566,   567,    -1,    -1,    -1,   949,   950,
     951,   952,    -1,    -1,    76,   578,    -1,    -1,    80,    64,
      -1,    66,    -1,    68,    -1,    70,   976,    -1,    73,    -1,
      75,   972,    77,   974,   975,   976,    -1,   577,    -1,    -1,
      -1,    -1,    -1,    -1,   985,   986,   987,   988,   989,   990,
      -1,    96,   371,    -1,    99,    -1,   619,   102,    -1,    -1,
    1001,  1002,    -1,  1004,   126,   127,    -1,   129,   130,   131,
      -1,   133,   134,   135,   393,   137,   138,   139,   140,    -1,
     399,   143,   144,   145,   146,   147,    -1,   149,   407,    -1,
     152,   153,   154,   633,  1035,    -1,    -1,    -1,  1039,   639,
       2,    -1,    -1,    62,    -1,    -1,   151,    -1,    -1,    11,
      -1,    -1,   157,    -1,    -1,    -1,    -1,   680,   681,    -1,
      -1,    23,    -1,    25,    -1,   688,     3,   690,    -1,    -1,
      -1,    -1,    -1,    -1,   697,    -1,    -1,   700,    15,    -1,
     703,    -1,   461,    -1,    -1,    22,   465,    28,    -1,    -1,
      -1,   691,    -1,   693,   473,   845,    -1,    -1,   698,    -1,
      -1,   701,    -1,     2,    41,    -1,    68,    -1,   708,    46,
      -1,   711,    11,    -1,    -1,    -1,    -1,    54,   741,    -1,
      28,    -1,    -1,    -1,    23,    -1,    25,    -1,    -1,    -1,
      23,    24,    94,   733,    -1,    76,    98,    74,    -1,    80,
      -1,    34,    35,    -1,    -1,    -1,   746,    -1,   748,    86,
      -1,    44,    -1,    90,   533,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    -1,    -1,   914,    -1,    -1,    -1,    76,    68,
      -1,    64,    80,    66,    -1,    -1,    -1,   196,   197,   198,
     199,    -1,    -1,    -1,    77,   126,   127,    -1,   129,   130,
     131,    -1,   133,   134,   156,    94,    -1,   138,   577,    98,
      -1,    -1,    -1,    -1,   145,   146,    -1,   957,   149,   102,
      -1,   152,   153,   154,    -1,    -1,    -1,   236,   126,    -1,
     239,    -1,   822,   131,    -1,   133,   134,    -1,     2,   829,
     138,    -1,   855,    -1,    -1,    -1,    -1,    11,   146,   258,
      -1,   841,    -1,    -1,   152,   153,   154,    -1,    -1,    23,
      -1,    25,  1002,   853,   633,    -1,    -1,   156,    -1,   278,
     639,    -1,   862,    -1,   226,    -1,   866,    -1,    -1,    -1,
      -1,   233,    -1,    -1,    -1,   898,   899,    -1,   657,   902,
     903,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,  1039,
      -1,   670,   671,    -1,    68,    28,   919,    -1,    34,    35,
      -1,    -1,   925,    -1,   927,   905,   929,    -1,    44,    -1,
      -1,    47,   691,    -1,   693,    -1,    -1,    -1,    -1,   698,
      94,    -1,   701,    -1,    98,    -1,   288,   226,    64,   708,
      66,    -1,   711,    -1,   233,    71,    -1,    -1,    -1,    -1,
     302,    77,    -1,    76,    -1,    -1,    -1,    80,    -1,   949,
     950,   951,   952,    -1,   733,   374,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   746,    -1,   748,
      -1,    -1,    -1,    -1,   974,   975,    -1,    -1,    -1,   398,
      -1,    -1,   156,   402,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    -1,   126,   127,    -1,  1019,   130,   131,    -1,
     133,   134,    -1,   302,    -1,   138,    -1,    -1,    -1,   371,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,   152,
     153,   154,    -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,
      -1,   393,    -1,    -1,    -1,    11,    -1,   399,    -1,    -1,
      -1,    -1,    -1,   822,    -1,   407,    -1,    23,    -1,    25,
     829,    -1,   226,    -1,    -1,    -1,    -1,    -1,    -1,   233,
      -1,    -1,   841,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   371,    -1,   853,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   862,    -1,    -1,    -1,   866,    -1,    -1,
      -1,    -1,    68,    -1,   393,    -1,    -1,    -1,    -1,   461,
     399,    -1,    -1,   465,    -1,    -1,    28,    -1,   407,    -1,
      -1,   473,    -1,    -1,   288,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    98,    -1,    -1,    -1,   905,    -1,   302,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   560,   561,    -1,    -1,    -1,    -1,   566,   567,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,
      -1,    -1,   461,    -1,    -1,    -1,   465,    -1,    -1,    -1,
     949,   950,   951,   952,   473,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   974,   975,   371,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,    -1,   129,   130,   131,
      -1,   133,   134,   135,    -1,   577,   138,   139,   140,   393,
      -1,   143,   144,   145,   146,   399,    -1,   149,    -1,    -1,
     152,   153,   154,   407,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,    -1,    -1,    -1,    -1,    -1,    -1,   233,    -1,    -1,
      -1,   680,   681,    -1,    -1,    -1,    -1,    -1,    -1,   688,
       3,   633,    -1,    -1,    -1,    -1,    -1,   639,   577,    -1,
      -1,   700,    15,    16,   703,    -1,    -1,   461,    -1,    22,
      -1,   465,    -1,    -1,    -1,    -1,    -1,    30,    -1,   473,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    -1,
      43,    -1,   288,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,   302,    -1,    -1,   691,
      -1,   693,    65,    -1,   633,    -1,   698,    -1,    -1,   701,
     639,    74,    -1,    -1,    -1,    -1,   708,     2,    -1,   711,
      83,    84,    -1,    86,    -1,    88,    11,    90,    -1,    -1,
      93,    -1,    -1,    -1,    97,    98,    -1,   100,    23,    -1,
      25,   733,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   746,    -1,   748,    -1,    -1,    -1,
      -1,    -1,   691,    -1,   693,   371,    -1,    -1,    -1,   698,
      -1,    -1,   701,   577,    -1,    -1,    -1,    -1,    -1,   708,
      -1,    -1,   711,    68,    -1,    -1,    -1,   393,    -1,    -1,
      -1,    -1,    -1,   399,    -1,   158,    -1,    -1,    -1,    -1,
      -1,   407,    -1,    -1,   733,    -1,   855,    -1,    -1,    94,
      -1,    -1,    -1,    98,    -1,    -1,    -1,   746,    -1,   748,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   633,
     822,    -1,    -1,    -1,    -1,   639,    -1,   829,    -1,    -1,
      -1,    -1,    -1,     2,    -1,    -1,    -1,    -1,    -1,   841,
      -1,    -1,    11,    -1,    -1,   461,    -1,    -1,    -1,   465,
      -1,   853,    -1,    -1,    23,    -1,    25,   473,    -1,    -1,
     862,   156,    -1,    -1,   866,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    -1,    -1,    -1,   691,    -1,   693,
      -1,    -1,    -1,   822,   698,    15,    16,   701,    -1,    -1,
     829,    -1,    22,    -1,   708,    -1,    -1,   711,    -1,    68,
      30,    -1,   841,   905,    -1,    -1,    36,    39,    -1,    -1,
      -1,    41,    -1,    43,   853,    -1,    46,    -1,    -1,   733,
      -1,    -1,    -1,   862,    54,    94,    -1,   866,    -1,    98,
      -1,   226,   746,    -1,   748,    65,    -1,    -1,   233,    -1,
      -1,    -1,    74,    -1,    74,    -1,    -1,   949,   950,   951,
     952,    -1,    -1,    83,    84,    -1,    86,    -1,    88,    -1,
      90,   577,    -1,    93,    -1,    -1,   905,    97,    -1,    -1,
     100,    -1,   974,   975,    -1,    -1,    -1,     2,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    11,   156,    -1,   121,
      -1,   123,    -1,   288,    -1,    -1,    -1,    -1,    23,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,   302,   822,    -1,
     949,   950,   951,   952,    -1,   829,    -1,   633,    -1,    -1,
      -1,    -1,    -1,   639,    -1,    -1,   156,   841,   158,    -1,
      -1,    -1,    -1,    -1,    -1,   974,   975,    -1,    -1,   853,
      -1,    -1,    -1,    68,    -1,    -1,   178,    -1,   862,    -1,
      -1,    -1,   866,    -1,    -1,    -1,    10,   226,    -1,   191,
      -1,    -1,   194,    17,   233,    -1,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    98,    28,   691,   371,   693,    -1,    -1,
      -1,    -1,   698,    -1,    -1,   701,    -1,    -1,    -1,    -1,
      -1,   905,   708,    -1,    -1,   711,    -1,   229,   393,    -1,
      -1,    -1,    -1,    -1,   399,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   407,    -1,    -1,   247,    -1,   733,    -1,   288,
      -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,
     746,   156,   748,   302,    -1,   949,   950,   951,   952,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,
     974,   975,    -1,    -1,    -1,    -1,   461,    -1,    -1,    -1,
     465,    -1,   126,   127,    -1,   129,   130,   131,   473,   133,
     134,   135,    -1,   137,   138,   139,   140,   141,    -1,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,   152,   153,
     154,   226,   371,    -1,    -1,    -1,   822,   161,   233,    -1,
      -1,    -1,    -1,   829,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,    -1,   393,   841,    -1,    -1,    -1,    -1,
     399,    -1,    -1,    -1,    -1,    -1,    -1,   853,   407,    19,
      -1,    -1,    -1,    23,    24,    -1,   862,    -1,    -1,    -1,
     866,    -1,    32,    -1,    34,    35,   388,    -1,    -1,    39,
      -1,    -1,    -1,   288,    44,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   302,    -1,    -1,
      -1,    -1,   577,   415,    64,    -1,    66,    -1,    68,   905,
      70,    71,   461,    73,    -1,    75,   465,    77,    -1,    -1,
      -1,    -1,    -1,    -1,   473,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,    99,
      -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   949,   950,   951,   952,    -1,   633,    -1,
      -1,    -1,   474,    -1,   639,   477,   371,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   974,   975,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   393,    -1,
      -1,   151,    -1,    10,   399,    -1,    -1,   157,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,
      -1,    28,    29,    -1,    -1,    -1,   691,    -1,   693,    -1,
      -1,    -1,    -1,   698,    -1,    19,   701,    -1,   577,    23,
      24,    -1,    -1,   708,    -1,    -1,   711,    -1,    32,    -1,
      34,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,   461,    -1,   733,    76,
     465,    -1,    -1,    80,    -1,    -1,    -1,    -1,   473,    -1,
      64,   746,    66,   748,    68,    -1,    70,    71,   590,    73,
      -1,    75,   594,    77,   633,    -1,    -1,    -1,    -1,   106,
     639,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    -1,    99,    -1,   619,   102,   126,
     127,   623,   129,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,   140,   141,    -1,   143,   144,   145,   146,
     147,   148,   149,    -1,    -1,   152,   153,   154,    -1,   156,
      -1,    -1,   691,    -1,   693,   657,    -1,   822,    -1,   698,
      -1,    -1,   701,    -1,   829,    -1,    -1,   151,    -1,   708,
      -1,    -1,   711,   157,    -1,    -1,   841,     3,    -1,    -1,
      -1,    -1,   577,    -1,    -1,    -1,    -1,    -1,   853,    15,
      16,    -1,    -1,    -1,   733,    -1,    22,   862,    -1,    -1,
      -1,   866,    -1,    -1,    30,    -1,    -1,   746,    -1,   748,
      36,    -1,    -1,   715,    -1,    41,   718,    43,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   738,    -1,   633,    65,
     905,    -1,    -1,    -1,   639,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    -1,    -1,   756,    -1,    -1,    83,    84,    -1,
      86,    -1,    88,    -1,    90,    -1,    -1,    93,    -1,    -1,
      -1,    97,    -1,    -1,   100,    -1,   778,    -1,    -1,    -1,
      -1,    -1,    -1,   822,   949,   950,   951,   952,     3,    -1,
     829,    -1,    -1,    -1,    -1,    -1,   691,    -1,   693,    -1,
      15,    16,   841,   698,    -1,    -1,   701,    22,    -1,   974,
     975,    -1,    -1,   708,   853,    30,   711,    -1,    -1,    -1,
      -1,    36,    -1,   862,    -1,    -1,    41,   866,    43,    -1,
      -1,    46,   158,    -1,    -1,    -1,    -1,    -1,   733,    54,
      -1,    -1,    19,    -1,    -1,    -1,    23,    24,    -1,    -1,
      65,   746,    -1,   748,    -1,    32,    -1,    34,    35,    74,
      -1,    -1,    39,    -1,    -1,    -1,   905,    44,    83,    84,
      -1,    86,    -1,    88,    -1,    90,    -1,    -1,    93,    -1,
      -1,   883,    97,    -1,    -1,   100,    -1,    64,    -1,    66,
      -1,    68,    -1,    70,    -1,    -1,    73,    -1,    75,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     949,   950,   951,   952,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   974,   975,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   841,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   853,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   862,    -1,    -1,
      -1,   866,    -1,    -1,   151,    -1,    -1,     0,     1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
     905,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    58,    -1,    60,    -1,    -1,
      -1,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    -1,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    -1,
      93,    94,    95,    96,    97,    -1,    99,   100,   101,   102,
      -1,    -1,   105,    -1,   107,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,    -1,    -1,   148,   149,    -1,   151,    -1,
      -1,   154,    -1,   156,    -1,   158,     1,   160,     3,     4,
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
      -1,   126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,    -1,    -1,   148,   149,   150,    -1,    -1,    -1,   154,
      -1,   156,    -1,   158,    -1,   160,   161,     1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,    12,    13,
      14,    15,    16,    17,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    57,    58,    -1,    60,    -1,    -1,    -1,
      64,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    -1,    93,
      94,    95,    96,    97,    -1,    99,   100,   101,   102,    -1,
      -1,   105,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,    -1,   137,   138,   139,   140,   141,    -1,   143,
     144,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
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
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,    -1,   148,   149,   150,   151,    -1,    -1,   154,    -1,
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
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,
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
     130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,   148,   149,
      -1,   151,    -1,    -1,   154,    -1,   156,   157,   158,     1,
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
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,    -1,
      -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,    -1,    -1,   148,   149,    -1,   151,
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
      -1,    -1,   126,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    -1,    -1,   148,   149,    -1,   151,    -1,    -1,
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
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,    -1,   148,   149,    -1,   151,    -1,    -1,   154,    -1,
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
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,
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
     130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,   148,   149,
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
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,    -1,
      -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,    -1,    -1,   148,   149,    -1,   151,
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
      -1,    -1,   126,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    -1,    -1,   148,   149,    -1,   151,    -1,    -1,
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
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,    -1,   148,   149,    -1,   151,    -1,    -1,   154,    -1,
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
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,
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
     130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,   148,   149,
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
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,    -1,
      -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,    -1,    -1,   148,   149,    -1,   151,
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
      -1,    -1,   126,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    -1,    -1,   148,   149,    -1,   151,    -1,    -1,
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
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,    -1,   148,   149,    -1,   151,    -1,    -1,   154,    -1,
     156,    -1,   158,     1,   160,     3,     4,     5,     6,     7,
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
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,
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
     130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,   148,   149,
     150,    -1,    -1,    -1,   154,    -1,   156,    -1,   158,    -1,
     160,   161,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    84,    85,    86,    87,    88,    -1,    90,
      -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,    -1,    -1,   148,   149,   150,
      -1,    -1,    -1,   154,    -1,   156,    -1,   158,    -1,   160,
     161,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
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
       1,    -1,     3,    -1,   126,   127,   128,   129,   130,    10,
      -1,    -1,    -1,   135,    15,    16,    17,    -1,    -1,    -1,
      -1,    22,    -1,   145,    -1,    -1,   148,   149,    -1,    30,
      -1,    -1,   154,    -1,   156,    36,   158,    -1,   160,   161,
      41,    -1,    43,    -1,    -1,    46,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    84,    -1,    86,    -1,    88,    -1,    90,
      -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,   113,   114,   115,   116,    -1,    -1,   119,   120,
     121,   122,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,    -1,    -1,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,    -1,   149,    -1,
      -1,   152,   153,   154,   155,    -1,    -1,   158,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      35,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    66,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,    88,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,   100,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,    -1,    -1,   148,   149,    -1,    -1,    -1,    -1,   154,
      -1,   156,    -1,   158,    -1,   160,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    19,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      67,    -1,    69,    -1,    -1,    -1,    -1,    74,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    84,    85,    86,
      87,    88,    -1,    90,    -1,    -1,    93,    -1,    -1,    96,
      97,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,   145,    -1,
      12,   148,   149,    15,    16,    -1,    -1,   154,    -1,   156,
      22,   158,    -1,   160,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    84,    85,    86,    87,    88,    -1,    90,    -1,
      -1,    93,    94,    95,    -1,    97,    98,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,    -1,
      -1,    -1,    -1,   135,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,    -1,    -1,   148,   149,   150,    -1,
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
      94,    95,    -1,    97,    98,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    -1,    -1,   148,   149,   150,    -1,    -1,    -1,
     154,    -1,   156,    -1,   158,   159,   160,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
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
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,    -1,   148,   149,   150,    -1,    -1,    -1,   154,    -1,
     156,    -1,   158,   159,   160,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    42,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    67,
      -1,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    83,    84,    85,    86,    87,
      88,    -1,    90,    -1,    -1,    93,    -1,    -1,    -1,    97,
      -1,    -1,   100,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,
     148,   149,    -1,    -1,    -1,    -1,   154,    -1,   156,    -1,
     158,    -1,   160,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    19,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,
      -1,    -1,    -1,    -1,    74,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    84,    85,    86,    87,    88,    -1,
      90,    -1,    -1,    93,    -1,    -1,    96,    97,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
     130,    -1,    -1,    -1,    -1,   135,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,   145,    -1,    12,   148,   149,
      15,    16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,
     160,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,    88,    -1,    90,    -1,    -1,    93,    94,
      95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,    -1,    -1,   148,   149,    -1,   151,    -1,    -1,   154,
      -1,   156,    -1,   158,    -1,   160,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      67,    -1,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    84,    85,    86,
      87,    88,    -1,    90,    -1,    -1,    93,    94,    95,    -1,
      97,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,
      -1,   148,   149,   150,    -1,    -1,    -1,   154,    -1,   156,
      -1,   158,    -1,   160,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,
      69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    84,    85,    86,    87,    88,
      -1,    90,    -1,    -1,    93,    94,    95,    -1,    97,    -1,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,   145,    -1,    12,   148,
     149,    15,    16,    -1,    -1,   154,    -1,   156,    22,   158,
      -1,   160,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,    -1,    69,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      84,    85,    86,    87,    88,    -1,    90,    -1,    -1,    93,
      -1,    -1,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,   135,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,   145,    -1,    12,   148,   149,    15,    16,    -1,    -1,
     154,    -1,   156,    22,   158,    -1,   160,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,
      69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    84,    85,    86,    87,    88,
      -1,    90,    -1,    -1,    93,    -1,    -1,    -1,    97,    -1,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,   148,
     149,   150,    -1,    -1,    -1,   154,    -1,   156,    -1,   158,
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
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   145,    -1,    12,   148,   149,    15,
      16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,   160,
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
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,    -1,   148,   149,   150,    -1,    -1,    -1,   154,    -1,
     156,   157,   158,    -1,   160,     3,     4,     5,     6,     7,
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
     108,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,   145,    -1,    12,
     148,   149,    15,    16,    -1,    -1,   154,    -1,   156,    22,
     158,    -1,   160,    -1,    -1,    -1,    29,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    67,    -1,    69,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    84,    85,    86,    87,    88,    -1,    90,    -1,    -1,
      93,    -1,    -1,    -1,    97,    -1,    -1,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,   145,    -1,    12,   148,   149,    15,    16,    -1,
      -1,   154,    -1,   156,    22,   158,    -1,   160,    -1,    -1,
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
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,
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
     130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,   148,   149,
     150,    -1,    -1,    -1,   154,    -1,   156,    -1,   158,    -1,
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
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,    -1,
      -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,    -1,    -1,   148,   149,   150,    -1,
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
      -1,    -1,   126,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    -1,    -1,   148,   149,   150,    -1,    -1,    -1,
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
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   145,
      -1,    12,   148,   149,    15,    16,    -1,    -1,   154,    -1,
     156,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    84,    85,    86,    87,    88,    -1,    90,
      -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   145,    -1,    12,   148,   149,    15,
      16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,   160,
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
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   145,
      -1,    12,   148,   149,    15,    16,    -1,    -1,   154,    -1,
     156,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    84,    85,    86,    87,    88,    -1,    90,
      -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   145,    -1,    12,   148,   149,    15,
      16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,   160,
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
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   145,
      -1,    12,   148,   149,    15,    16,    -1,    -1,   154,    -1,
     156,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    84,    85,    86,    87,    88,    -1,    90,
      -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   145,    -1,    12,   148,   149,    15,
      16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,   160,
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
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   145,
      -1,    12,   148,   149,    15,    16,    -1,    -1,   154,    -1,
     156,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    84,    85,    86,    87,    88,    -1,    90,
      -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   145,    -1,    12,   148,   149,    15,
      16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,   160,
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
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   145,
      -1,    12,   148,   149,    15,    16,    -1,    -1,   154,    -1,
     156,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    84,    85,    86,    87,    88,    -1,    90,
      -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   145,    -1,    12,   148,   149,    15,
      16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,   160,
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
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   145,
      -1,    12,   148,   149,    15,    16,    -1,    -1,   154,    -1,
     156,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    84,    85,    86,    87,    88,    -1,    90,
      -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   145,    -1,    12,   148,   149,    15,
      16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,   160,
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
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   145,
      -1,    12,   148,   149,    15,    16,    -1,    -1,   154,    -1,
     156,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    84,    85,    86,    87,    88,    -1,    90,
      -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   145,    -1,    12,   148,   149,    15,
      16,    -1,    -1,   154,    -1,   156,    22,   158,    -1,   160,
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
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   145,
      -1,    12,   148,   149,    15,    16,    -1,    -1,   154,    -1,
     156,    22,   158,    -1,   160,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    84,    85,    86,    87,    88,    -1,    90,
      -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   145,    -1,    12,   148,   149,    15,
      16,    -1,    -1,   154,    -1,    -1,    22,   158,    -1,   160,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,    85,
      86,    87,    88,    -1,    90,    -1,    -1,    93,    -1,    -1,
      -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,    -1,    -1,
     126,   127,   128,   129,   130,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,   145,
      15,    16,    -1,   149,    -1,    -1,   155,    22,   154,   158,
     156,   160,   158,    -1,   160,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,    88,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,   128,    -1,    -1,    -1,   132,    22,    -1,
      -1,   136,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,   150,    40,    41,    -1,    43,
      -1,   156,    46,   158,   159,   160,    50,    51,    52,    -1,
      54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    83,
      84,    85,    86,    87,    88,    -1,    90,    -1,    92,    93,
      -1,    -1,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,
     104,    -1,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   128,    -1,     3,     4,     5,     6,
       7,     8,    -1,   142,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,   155,   151,    -1,   158,
      -1,   160,   156,    30,   158,    -1,   160,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      67,    -1,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    84,    85,    86,
      87,    88,    -1,    90,    -1,    -1,    93,    -1,    -1,    -1,
      97,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,   128,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,   156,
      46,   158,    -1,   160,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,    85,
      86,    87,    88,    -1,    90,    -1,    -1,    93,    -1,    -1,
      -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,   128,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
     156,    46,   158,    -1,   160,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    83,    84,
      85,    86,    87,    88,    10,    90,    -1,    -1,    93,    15,
      16,    17,    97,    -1,    -1,   100,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    48,   128,    -1,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    -1,   158,    -1,   160,    -1,    83,    84,    -1,
      86,    -1,    88,    -1,    90,    10,    -1,    93,    -1,    -1,
      -1,    97,    17,    -1,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,   111,   112,   113,   114,   115,
     116,    -1,    -1,   119,   120,   121,   122,    -1,   124,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,    -1,    -1,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,    -1,   149,    -1,    28,   152,   153,   154,   155,
      -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,
      -1,   126,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,    -1,    76,   152,   153,   154,
      80,    -1,    -1,    -1,    -1,    10,   161,    -1,    -1,    -1,
      -1,    -1,    17,   126,   127,    -1,   129,   130,   131,    -1,
     133,   134,   135,    28,    -1,   138,    -1,    -1,    -1,    -1,
     110,    -1,   145,   146,    -1,    -1,   149,    -1,    -1,   152,
     153,   154,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
      -1,    76,   152,   153,   154,    80,    -1,    -1,    -1,    -1,
      10,   161,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,    -1,    76,   152,   153,   154,
      80,    -1,    -1,    -1,    -1,    10,   161,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
      -1,    76,   152,   153,   154,    80,    -1,    -1,    -1,   159,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,    -1,    76,   152,   153,   154,
      80,    -1,    -1,    -1,   159,    10,    -1,    -1,    -1,    89,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,   114,   115,   116,    -1,    -1,   119,
     120,   121,   122,    -1,   124,   125,   126,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
      -1,    76,   152,   153,   154,    80,   156,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    89,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,   119,   120,   121,   122,    -1,   124,
     125,   126,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,    -1,    76,   152,   153,   154,
      80,   156,    -1,    -1,    -1,    10,    -1,    -1,    -1,    89,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,   114,   115,   116,    -1,    -1,   119,
     120,   121,   122,    -1,   124,   125,   126,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
      -1,    76,   152,   153,   154,    80,   156,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    89,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,   119,   120,   121,   122,    -1,   124,
     125,   126,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,    -1,    76,   152,   153,   154,
      80,   156,    -1,    -1,    -1,    10,    -1,    -1,    -1,    89,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,   114,   115,   116,    -1,    -1,   119,
     120,   121,   122,    -1,   124,   125,   126,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
      -1,    76,   152,   153,   154,    80,   156,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    89,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,   119,   120,   121,   122,    -1,   124,
     125,   126,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,    -1,    76,   152,   153,   154,
      80,   156,    -1,    -1,    -1,    10,    -1,    -1,    -1,    89,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,   114,   115,   116,    -1,    -1,   119,
     120,   121,   122,    -1,   124,   125,   126,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
      -1,    76,   152,   153,   154,    80,   156,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    89,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,   119,   120,   121,   122,    -1,   124,
     125,   126,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,    -1,    76,   152,   153,   154,
      80,   156,    -1,    -1,    -1,    10,    -1,    -1,    -1,    89,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,   114,   115,   116,    42,    -1,   119,
     120,   121,   122,    -1,   124,   125,   126,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
      -1,    76,   152,   153,   154,    80,   156,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,    -1,    76,   152,   153,   154,
      80,   156,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
      -1,    76,   152,   153,   154,    80,   156,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,    -1,    76,   152,   153,   154,
      80,   156,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
      -1,    76,   152,   153,   154,    80,   156,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    89,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,    -1,    76,   152,   153,   154,
      80,   156,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    33,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    61,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
      -1,    76,   152,   153,   154,    80,   156,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,   126,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
     106,    -1,    33,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,   129,   130,   131,    -1,   133,   134,   135,
      61,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,    -1,    76,   152,   153,   154,    80,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,    -1,
      76,   152,   153,   154,    80,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,    -1,    76,   152,   153,   154,    80,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,    -1,
      76,   152,   153,   154,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,    76,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,    -1,   129,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,   140,   141,    -1,   143,   144,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    76,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
      -1,   129,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,   140,   141,    -1,   143,   144,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,    76,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,
     129,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,   140,   141,    -1,   143,   144,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    76,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
      -1,    76,   152,   153,   154,    80,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    89,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,    -1,    76,   152,   153,   154,
      80,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    76,   152,   153,   154,    80,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,    -1,    76,   152,   153,   154,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      11,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,    76,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    31,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,    -1,
      76,   152,   153,   154,    80,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,    -1,    76,   152,   153,   154,    80,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    29,    -1,    -1,    -1,    -1,   109,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,    -1,
      76,   152,   153,   154,    80,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,    -1,    76,   152,   153,   154,    80,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,    -1,
      76,   152,   153,   154,    80,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    89,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,    -1,    76,   152,   153,   154,    80,
      -1,    -1,    -1,    -1,    10,    11,    -1,    -1,    89,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,    -1,
      76,   152,   153,   154,    80,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,    -1,    76,   152,   153,   154,    80,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    89,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,    -1,
      76,   152,   153,   154,    80,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    89,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,    -1,    76,   152,   153,   154,    80,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    89,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,    -1,
      76,   152,   153,   154,    80,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    89,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,    -1,    76,   152,   153,   154,    80,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    89,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,    -1,
      76,   152,   153,   154,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,   129,   130,   131,    52,   133,   134,   135,
      -1,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,    -1,    -1,   152,   153,   154,    -1,
      76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,    -1,    76,   152,   153,   154,    80,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,    -1,   129,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,    -1,
      76,   152,   153,   154,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,   140,   141,    28,   143,   144,   145,
     146,   147,   148,   149,    -1,    19,   152,   153,   154,    23,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      34,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,
      64,    -1,    66,    -1,    68,    -1,    70,    -1,    -1,    73,
      -1,    75,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    96,    -1,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,    -1,    -1,   126,   127,    -1,   129,   130,   131,
      -1,   133,   134,   135,    -1,   137,   138,   139,   140,    -1,
      -1,   143,   144,   145,   146,   147,   148,   149,    -1,    76,
     152,   153,   154,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,    -1,   129,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,   140,    -1,    -1,   143,   144,   145,   146,
     147,     1,   149,     3,    -1,   152,   153,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,
      15,    16,    -1,    -1,    74,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    83,    84,    30,    86,    -1,    88,    -1,
      90,    36,    -1,    93,    -1,    -1,    41,    97,    43,    -1,
     100,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      -1,    86,    -1,    88,    -1,    90,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,   100
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
     105,   107,   110,   126,   127,   128,   129,   130,   135,   145,
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
      98,   136,   240,   266,   267,   289,   198,   264,   287,   297,
      68,   170,   156,    72,   174,   185,   185,    47,    71,   235,
     252,   150,   151,   142,    10,    17,    28,    76,    80,   110,
     126,   127,   129,   130,   131,   133,   134,   135,   137,   138,
     139,   140,   141,   143,   144,   145,   146,   147,   148,   149,
     152,   153,   154,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   155,   227,
     158,   160,    76,    80,   158,   170,   151,   287,   287,   287,
     264,   156,   175,    42,   277,   248,   252,   151,   132,   151,
     105,   186,   240,   268,   269,   270,   289,   248,   210,   250,
     185,   158,   191,    29,    42,   191,   106,   191,   280,    29,
      42,   191,   280,   288,    33,    61,   151,    89,   175,   227,
     268,   170,   158,   229,    98,   158,   185,   254,   255,     1,
     131,   259,    42,   132,   170,   191,   191,   268,   185,   132,
     151,   287,   287,   151,   156,   191,   158,   268,   151,   202,
     151,   202,   151,   111,   249,   151,   151,    33,    61,   191,
     151,   157,   157,   168,   132,   157,   287,   132,   159,   132,
     159,   161,   280,    42,   132,   161,   280,   109,   132,   161,
       7,     1,   157,   176,    56,   131,   211,   156,   222,   151,
     287,   287,   287,   287,   198,   287,   198,   287,   287,   287,
     287,   287,    16,   100,   287,   287,    30,    54,    96,   186,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   289,   289,   289,   289,
     289,   268,   268,   198,   287,   198,   287,    24,    42,    77,
     102,   279,   282,   283,   287,   302,    29,    42,    29,    42,
      89,    42,   161,   198,   287,   191,   151,   151,   287,   287,
     111,   159,   132,   151,   185,   252,    42,   264,    40,   287,
     198,   287,   158,   191,    40,   287,   198,   287,   191,   154,
     177,   179,   287,   177,   178,   170,   287,    31,   287,   159,
      24,    42,    45,    63,    66,    77,    96,   169,   230,   231,
     232,   233,   219,   255,   132,   159,    16,    30,    43,    65,
      83,    84,    88,   100,   160,   189,   260,   272,   111,   256,
     287,   253,   159,   240,   287,     1,   207,   268,   159,    18,
     203,   260,   272,   132,   179,   178,   157,   159,   159,   266,
     159,   266,   170,   161,   198,   287,   161,   170,   287,   161,
     287,   161,   287,   157,   157,   185,   264,   156,     1,   185,
     215,   216,    24,    66,    77,    96,   224,   234,   151,   151,
     151,   151,   151,   159,   161,    42,    77,   132,   159,   271,
      76,    76,    40,   198,   287,    40,   198,   287,   198,   287,
     277,   277,   151,   240,   289,   270,   211,   151,   287,   159,
     287,    29,   191,    29,   191,   281,   282,   287,    29,   191,
     280,    29,   191,   280,   132,   151,    11,   151,    31,    31,
     170,    89,   175,    42,    77,   232,   132,   159,   158,   185,
      24,    66,    77,    96,   236,   159,   255,   259,     1,   264,
      59,   289,   157,    62,   103,   157,   208,   159,   158,   175,
     185,   204,   248,   151,   151,   170,   161,   280,   161,   280,
     170,   109,     1,   212,   157,   111,   132,   157,    77,   223,
       1,     3,    10,    15,    17,    22,    36,    41,    46,    48,
      54,    74,    86,    90,    93,    97,   126,   127,   128,   129,
     130,   133,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   149,   152,   153,   154,   155,   158,   184,
     185,   187,   225,   226,   227,   271,   282,   259,   271,   271,
     287,    29,    29,   287,    29,    29,   161,   161,   191,   191,
     156,   191,    89,    40,   287,    40,   287,   132,   159,    89,
      40,   287,   191,    40,   287,   191,   287,   287,   170,   287,
     170,    31,   185,   231,   255,   131,   263,    77,   259,   256,
     161,    42,   161,    29,   170,   264,   204,   131,   175,    40,
     170,   287,   161,    40,   170,   287,   161,   287,   157,    35,
      70,    73,   157,   169,   193,   287,   216,   236,   158,   228,
     185,   287,   126,   134,   228,   228,   256,    89,    40,    40,
      89,    40,    40,    40,    40,     1,   212,   287,   287,   287,
     282,   287,   287,   287,    11,    31,   170,   263,   159,   160,
     189,   240,   262,   272,   136,   242,   256,    52,   104,   243,
     287,   260,   272,   170,   191,   159,   287,   287,   170,   287,
     170,   193,     1,   131,   258,   230,   159,     3,    90,   226,
     227,   287,   287,   287,   287,   287,   287,   157,   157,    89,
      89,    89,    89,   287,   170,   256,   263,   161,   264,   240,
     241,   287,     3,    79,    90,   244,   245,   246,   287,   175,
     192,   239,   161,   175,    89,    89,   160,   189,   257,   272,
      92,   237,   159,   228,   228,    89,    89,    89,    89,    89,
      89,   287,   287,   287,   287,   242,   256,   240,   261,   262,
     272,    42,   161,   246,   104,   132,   111,   137,   139,   140,
     143,   144,    52,   272,   287,   287,     1,   161,   264,   243,
     287,   261,   262,   287,   245,   246,   246,   246,   246,   246,
     246,   244,   161,   257,   272,   161,   151,   238,   239,   161,
     257,   272,   261
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
#line 1237 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1241 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1242 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1243 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1244 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1245 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1246 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1247 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1248 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1249 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1250 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1251 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1252 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1256 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1257 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1261 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1265 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1266 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1267 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1272 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1276 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1280 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1284 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1317 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1318 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1319 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1320 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1324 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1325 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1328 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1341 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1345 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1350 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1351 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1357 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1363 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1367 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1374 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1375 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1376 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1377 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1380 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1381 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1387 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1389 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1394 "chapel.ypp"
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
#line 1407 "chapel.ypp"
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
#line 1423 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1425 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1427 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1431 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1432 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1434 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1435 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1440 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1446 "chapel.ypp"
    {
      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), context->latestComment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
      context->latestComment = NULL;
    }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1452 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1461 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1469 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1471 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1476 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1480 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1487 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1496 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1497 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1517 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1521 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1532 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1541 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1566 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1570 "chapel.ypp"
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
#line 1578 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1584 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1586 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1591 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1593 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1601 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1607 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1609 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1610 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1611 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1617 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1618 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1619 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1620 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1624 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1625 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1629 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1630 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1631 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1635 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1636 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1640 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1645 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1646 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1650 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1651 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1652 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1653 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1657 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1658 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1677 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1679 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1681 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1683 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1685 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1687 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1691 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1693 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1695 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1697 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1704 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1708 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1710 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1712 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1720 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1722 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1724 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1726 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1733 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1739 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1745 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1751 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1760 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1769 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1785 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1789 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1794 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1798 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1799 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1803 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1807 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1808 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1813 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1817 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1821 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1827 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1828 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1829 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1830 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1831 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1832 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1838 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1843 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1848 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1864 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1866 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1868 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1870 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1872 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1874 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1876 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1878 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1882 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1883 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1886 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1887 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1888 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1910 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1911 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1912 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1916 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1917 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1918 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1919 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1927 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1928 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 1929 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 1930 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 523:
/* Line 1792 of yacc.c  */
#line 1934 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 1935 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 1940 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 1941 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 1942 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 1943 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 1944 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 1945 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 1946 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 1947 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 1948 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 1950 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 1954 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 1961 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 1962 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 1966 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 1967 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 1968 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 1969 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 1970 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 1971 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 1972 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 1973 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 1974 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 1975 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 1976 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 1977 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 1978 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 1979 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 1980 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 1981 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 1982 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 1983 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 1984 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 1985 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 1986 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 1987 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 1988 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 1992 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 1993 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 1994 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 1995 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 1996 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 1997 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 1998 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2002 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2003 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2004 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2005 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2009 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2010 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2011 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2012 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2017 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2018 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2019 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2020 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2021 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2022 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2023 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 9596 "bison-chapel.cpp"
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


