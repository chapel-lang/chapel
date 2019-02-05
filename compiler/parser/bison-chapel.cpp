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
#define YYLAST   16100

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  164
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  138
/* YYNRULES -- Number of rules.  */
#define YYNRULES  579
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1038

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
     877,   878,   882,   886,   887,   889,   893,   898,   904,   909,
     916,   923,   924,   926,   928,   930,   932,   934,   937,   940,
     942,   944,   946,   947,   949,   951,   954,   956,   958,   960,
     962,   963,   965,   968,   970,   972,   974,   975,   977,   979,
     981,   983,   985,   988,   990,   991,   993,   996,   999,  1000,
    1003,  1006,  1011,  1016,  1018,  1022,  1026,  1030,  1034,  1038,
    1042,  1046,  1049,  1051,  1053,  1057,  1062,  1067,  1070,  1075,
    1076,  1079,  1082,  1084,  1086,  1088,  1091,  1093,  1098,  1104,
    1108,  1110,  1114,  1118,  1124,  1126,  1128,  1132,  1134,  1137,
    1141,  1142,  1145,  1148,  1152,  1155,  1160,  1164,  1168,  1173,
    1177,  1178,  1181,  1184,  1187,  1189,  1190,  1193,  1196,  1199,
    1201,  1206,  1211,  1218,  1222,  1223,  1225,  1227,  1231,  1236,
    1240,  1245,  1252,  1253,  1256,  1259,  1262,  1265,  1267,  1269,
    1273,  1277,  1279,  1283,  1285,  1287,  1289,  1293,  1297,  1298,
    1300,  1302,  1306,  1310,  1314,  1316,  1318,  1320,  1322,  1324,
    1326,  1328,  1330,  1332,  1335,  1340,  1345,  1350,  1356,  1359,
    1362,  1365,  1368,  1371,  1374,  1381,  1388,  1393,  1403,  1413,
    1421,  1428,  1435,  1440,  1450,  1460,  1468,  1473,  1480,  1487,
    1497,  1507,  1514,  1516,  1518,  1520,  1522,  1524,  1526,  1528,
    1530,  1534,  1535,  1537,  1542,  1544,  1548,  1553,  1555,  1559,
    1564,  1568,  1572,  1574,  1576,  1579,  1581,  1584,  1586,  1588,
    1592,  1595,  1600,  1602,  1604,  1606,  1608,  1610,  1612,  1614,
    1616,  1621,  1625,  1629,  1633,  1637,  1640,  1643,  1645,  1646,
    1648,  1651,  1654,  1656,  1658,  1660,  1662,  1664,  1666,  1668,
    1673,  1678,  1683,  1687,  1691,  1695,  1699,  1703,  1708,  1712,
    1717,  1719,  1721,  1723,  1725,  1727,  1729,  1731,  1733,  1737,
    1742,  1746,  1751,  1755,  1760,  1764,  1770,  1774,  1778,  1782,
    1786,  1790,  1794,  1798,  1802,  1806,  1810,  1814,  1818,  1822,
    1826,  1830,  1834,  1838,  1842,  1846,  1850,  1854,  1858,  1862,
    1865,  1868,  1871,  1874,  1877,  1880,  1884,  1888,  1892,  1896,
    1900,  1904,  1908,  1912,  1914,  1916,  1918,  1920,  1922,  1924
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
      38,   286,   190,    -1,    38,   286,   279,   190,    -1,    38,
     197,   190,    -1,    38,   197,   279,   190,    -1,   159,   263,
      42,   286,   160,   169,    -1,   159,   263,    42,   286,   279,
     160,   169,    -1,   159,   263,    42,   197,   160,   169,    -1,
     159,   263,    42,   197,   279,   160,   169,    -1,   159,   263,
     160,   169,    -1,   159,   263,   279,   160,   169,    -1,   159,
     197,   160,   169,    -1,   159,   197,   279,   160,   169,    -1,
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
    1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,  1239,  1240,
    1241,  1242,  1243,  1244,  1245,  1246,  1247,  1248,  1249,  1250,
    1254,  1255,  1259,  1263,  1264,  1265,  1269,  1271,  1273,  1275,
    1277,  1282,  1283,  1287,  1288,  1289,  1290,  1291,  1292,  1293,
    1294,  1295,  1299,  1300,  1301,  1302,  1303,  1304,  1308,  1309,
    1313,  1314,  1315,  1316,  1317,  1318,  1322,  1323,  1326,  1327,
    1331,  1332,  1336,  1338,  1343,  1344,  1348,  1349,  1353,  1354,
    1356,  1358,  1360,  1365,  1366,  1369,  1370,  1371,  1372,  1373,
    1374,  1375,  1378,  1379,  1382,  1384,  1386,  1391,  1404,  1421,
    1422,  1424,  1429,  1430,  1431,  1432,  1433,  1437,  1443,  1449,
    1457,  1458,  1466,  1468,  1473,  1475,  1477,  1482,  1484,  1486,
    1493,  1494,  1495,  1500,  1502,  1504,  1508,  1512,  1514,  1518,
    1526,  1527,  1528,  1529,  1530,  1535,  1536,  1537,  1538,  1539,
    1559,  1563,  1567,  1575,  1582,  1583,  1584,  1588,  1590,  1596,
    1598,  1600,  1605,  1606,  1607,  1608,  1609,  1615,  1616,  1617,
    1618,  1622,  1623,  1627,  1628,  1629,  1633,  1634,  1638,  1639,
    1643,  1644,  1648,  1649,  1650,  1651,  1655,  1656,  1667,  1668,
    1669,  1670,  1671,  1672,  1674,  1676,  1678,  1680,  1682,  1684,
    1686,  1688,  1690,  1692,  1697,  1699,  1701,  1703,  1705,  1707,
    1709,  1711,  1713,  1715,  1717,  1719,  1721,  1728,  1734,  1740,
    1746,  1755,  1765,  1773,  1774,  1775,  1776,  1777,  1778,  1779,
    1780,  1785,  1786,  1790,  1794,  1795,  1799,  1803,  1804,  1808,
    1812,  1816,  1823,  1824,  1825,  1826,  1827,  1828,  1832,  1833,
    1838,  1843,  1851,  1852,  1853,  1854,  1855,  1856,  1857,  1858,
    1859,  1861,  1863,  1865,  1867,  1869,  1871,  1873,  1878,  1879,
    1882,  1883,  1884,  1887,  1888,  1889,  1890,  1901,  1902,  1906,
    1907,  1908,  1912,  1913,  1914,  1915,  1923,  1924,  1925,  1926,
    1930,  1931,  1935,  1936,  1937,  1938,  1939,  1940,  1941,  1942,
    1943,  1944,  1945,  1949,  1957,  1958,  1962,  1963,  1964,  1965,
    1966,  1967,  1968,  1969,  1970,  1971,  1972,  1973,  1974,  1975,
    1976,  1977,  1978,  1979,  1980,  1981,  1982,  1983,  1984,  1988,
    1989,  1990,  1991,  1992,  1993,  1997,  1998,  1999,  2000,  2004,
    2005,  2006,  2007,  2012,  2013,  2014,  2015,  2016,  2017,  2018
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
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   197,   198,   198,   198,   198,   198,   198,
     198,   198,   199,   200,   200,   200,   200,   200,   200,   201,
     201,   202,   202,   202,   203,   203,   204,   205,   205,   206,
     206,   207,   207,   207,   208,   208,   208,   208,   209,   209,
     209,   210,   210,   211,   211,   211,   212,   212,   213,   214,
     214,   214,   215,   215,   217,   218,   216,   219,   219,   219,
     219,   219,   221,   222,   220,   223,   223,   223,   223,   223,
     224,   224,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     227,   227,   228,   229,   229,   229,   230,   230,   230,   230,
     230,   231,   231,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   233,   233,   233,   233,   233,   233,   234,   234,
     235,   235,   235,   235,   235,   235,   236,   236,   237,   237,
     238,   238,   239,   239,   240,   240,   241,   241,   242,   242,
     242,   242,   242,   243,   243,   244,   244,   244,   244,   244,
     244,   244,   245,   245,   246,   246,   246,   247,   247,   248,
     248,   248,   249,   249,   249,   249,   249,   250,   250,   250,
     251,   251,   252,   252,   253,   253,   253,   254,   254,   254,
     255,   255,   255,   256,   256,   256,   256,   256,   256,   256,
     257,   257,   257,   257,   257,   258,   258,   258,   258,   258,
     259,   259,   259,   259,   260,   260,   260,   261,   261,   261,
     261,   261,   262,   262,   262,   262,   262,   263,   263,   263,
     263,   264,   264,   265,   265,   265,   266,   266,   267,   267,
     268,   268,   269,   269,   269,   269,   270,   270,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   273,   274,   275,   275,   275,   275,   275,   275,   275,
     275,   276,   276,   277,   278,   278,   279,   280,   280,   281,
     281,   281,   282,   282,   282,   282,   282,   282,   283,   283,
     284,   285,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   287,   287,
     288,   288,   288,   289,   289,   289,   289,   290,   290,   291,
     291,   291,   292,   292,   292,   292,   293,   293,   293,   293,
     294,   294,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   296,   296,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   298,
     298,   298,   298,   298,   298,   299,   299,   299,   299,   300,
     300,   300,   300,   301,   301,   301,   301,   301,   301,   301
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
       0,     3,     3,     0,     1,     3,     4,     5,     4,     6,
       6,     0,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     0,     1,     1,     2,     1,     1,     1,     1,
       0,     1,     2,     1,     1,     1,     0,     1,     1,     1,
       1,     1,     2,     1,     0,     1,     2,     2,     0,     2,
       2,     4,     4,     1,     3,     3,     3,     3,     3,     3,
       3,     2,     1,     1,     3,     4,     4,     2,     4,     0,
       2,     2,     1,     1,     1,     2,     1,     4,     5,     3,
       1,     3,     3,     5,     1,     1,     3,     1,     2,     3,
       0,     2,     2,     3,     2,     4,     3,     3,     4,     3,
       0,     2,     2,     2,     1,     0,     2,     2,     2,     1,
       4,     4,     6,     3,     0,     1,     1,     3,     4,     3,
       4,     6,     0,     2,     2,     2,     2,     1,     1,     3,
       3,     1,     3,     1,     1,     1,     3,     3,     0,     1,
       1,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     4,     4,     4,     5,     2,     2,
       2,     2,     2,     2,     6,     6,     4,     9,     9,     7,
       6,     6,     4,     9,     9,     7,     4,     6,     6,     9,
       9,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     1,     4,     1,     3,     4,     1,     3,     4,
       3,     3,     1,     1,     2,     1,     2,     1,     1,     3,
       2,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     3,     3,     3,     3,     2,     2,     1,     0,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     4,     3,     3,     3,     3,     3,     4,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     4,
       3,     4,     3,     4,     3,     5,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   523,   524,   525,   526,
     527,     0,   461,    70,   100,     0,   198,   461,     0,   105,
       0,   343,    70,     0,     0,     0,     0,   208,   498,   498,
     520,     0,     0,     0,     0,   104,     0,   218,   101,     0,
     214,     0,     0,   107,     0,     0,   452,     0,   221,     0,
     342,     0,     0,     0,    45,    44,   103,   199,   344,     0,
       0,     0,     0,     0,     0,     0,   106,     0,     0,    83,
       0,   521,     0,     0,     0,   102,   200,     0,     0,   346,
       0,     0,   575,   577,     0,   578,   579,   497,     0,     0,
     576,   573,   122,   574,     0,     0,     0,     4,     0,     5,
       0,     9,    46,    10,    11,    12,    14,   416,   417,    22,
      13,   123,   129,    15,    17,    16,    19,    20,    21,    18,
     128,     0,   126,     0,   487,     0,   130,   127,     0,   131,
     503,   483,   484,   420,   418,     0,     0,   488,   489,     0,
     419,     0,   504,   505,   506,   522,   482,   422,   421,   485,
     486,     0,    38,    24,   428,     0,     0,   462,    71,     0,
       0,     0,     0,     0,     0,     0,     0,   487,   503,   418,
     488,   489,   430,   419,   504,   505,     0,   461,     0,     0,
     345,     0,   172,     0,   401,     0,   408,   499,   219,   526,
     136,     0,     0,   220,     0,     0,     0,     0,     0,     0,
       0,   498,   135,     0,     0,   408,    77,    84,    95,    89,
      97,    79,    88,    98,    85,    99,    93,    87,    94,    92,
      90,    91,    78,    80,    86,    96,     0,    81,     0,     0,
       0,     0,   350,     0,   119,    32,     0,   561,   480,     0,
     431,   562,     7,   408,   498,   125,   124,   313,   398,     0,
     397,     0,     0,   120,   502,     0,     0,    35,     0,   433,
     423,     0,   408,    36,   429,     0,   179,   175,     0,   419,
     179,   176,     0,   339,     0,   432,     0,   397,     0,     0,
     564,   496,   560,   563,   559,     0,    48,    51,     0,     0,
     403,     0,   405,     0,     0,   404,     0,     0,   397,     0,
       0,     6,     0,    47,     0,   201,     0,   299,   298,   222,
       0,    23,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   495,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   258,   265,   266,   267,   262,   264,     0,     0,   260,
     263,   261,   259,     0,   269,   268,     0,     0,   408,   408,
       0,     0,     0,    25,    26,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,    28,     0,    29,     0,   416,
     414,     0,   409,   410,   415,     0,     0,     0,     0,     0,
     145,     0,     0,   144,     0,   153,     0,     0,     0,   151,
       0,     0,     0,    57,   132,     0,   165,     0,     0,    30,
     281,   215,   354,     0,   355,   357,     0,   379,     0,   360,
       0,     0,   118,    31,    33,     0,   312,     0,    64,   500,
     501,   121,     0,    34,   408,     0,   186,   177,   173,   178,
     174,     0,   337,   334,    61,     0,    57,   138,    37,    50,
      49,    52,     0,   528,     0,     0,   516,     0,   518,     0,
       0,     0,     0,     0,     0,     0,     0,   532,     8,     0,
      40,     0,     0,     0,     0,     0,   292,   349,   460,   556,
     555,   558,   566,   565,   570,   569,   552,   549,   550,   551,
     492,   493,   491,   539,   514,   515,   513,   512,   494,   543,
     554,   548,   546,   557,   547,   545,   537,   542,   544,   553,
     536,   540,   541,   538,     0,     0,     0,     0,     0,     0,
       0,   568,   567,   572,   571,   472,   473,   475,   477,     0,
     464,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     530,   461,   461,   141,   335,   347,   402,     0,     0,   425,
       0,   336,   201,     0,     0,     0,     0,   436,     0,     0,
       0,   154,     0,   442,     0,     0,   152,   574,    60,     0,
      53,    58,     0,   164,     0,     0,     0,   424,   286,   283,
     284,   285,   289,   290,   291,   281,     0,   274,     0,   282,
     300,     0,   358,     0,   112,   114,   115,   110,   111,   109,
     108,   113,     0,   378,   377,   483,     0,   352,   481,   351,
     511,   400,   399,     0,     0,     0,   426,     0,   180,   341,
     483,     0,     0,     0,   529,   490,   517,   406,   519,   407,
     161,     0,     0,     0,   531,   159,   446,     0,   534,   533,
       0,    42,    41,    39,   202,     0,     0,   212,     0,   209,
     296,   293,   294,   297,   223,     0,    68,    69,    67,    66,
      65,   509,   510,   474,   476,     0,   463,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     137,   412,   413,   411,     0,   348,     0,   163,     0,     0,
     143,     0,   142,     0,   467,     0,     0,   149,     0,     0,
     147,     0,     0,   133,     0,   134,     0,     0,   167,     0,
     169,   287,   288,     0,   281,   272,     0,   392,   301,   304,
     303,   305,     0,   356,   359,   360,     0,     0,   361,   362,
     188,     0,     0,   187,   190,   427,     0,   181,   184,     0,
     338,    62,    63,   162,     0,     0,     0,     0,   160,     0,
       0,   217,   207,     0,   210,   206,   295,   300,   270,    72,
     255,    84,   253,    89,    74,    88,    85,    99,    87,    90,
      73,    75,    86,   234,   237,   235,   236,   247,   238,   251,
     243,   241,   254,   257,   242,   240,   245,   250,   252,   239,
     244,   248,   249,   246,   256,     0,   232,    76,     0,   270,
     270,   230,   465,   360,   503,   503,     0,     0,     0,     0,
       0,     0,     0,     0,   140,   139,     0,   146,     0,     0,
     435,     0,   434,     0,   466,     0,     0,   441,   150,     0,
     440,   148,    55,    54,   166,   451,   168,     0,   392,   275,
       0,     0,   360,   302,   318,   353,   383,     0,   530,     0,
     192,     0,     0,     0,   182,     0,   157,   448,     0,     0,
     155,   447,     0,   535,   196,   498,   217,   217,   194,   217,
     204,   213,   211,     0,   281,   229,   233,     0,     0,   225,
     226,   469,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   217,   439,     0,     0,   468,   445,     0,     0,     0,
       0,   171,   360,   392,     0,   395,   394,   396,   483,   314,
     278,   276,     0,     0,     0,     0,   381,   483,   193,   191,
       0,   185,     0,   158,     0,   156,   205,   374,     0,   306,
       0,   231,    72,    73,   270,   270,     0,     0,     0,     0,
       0,     0,   197,   195,     0,     0,     0,     0,    56,   170,
     277,   360,   384,     0,   315,   317,   316,   332,     0,   333,
     320,   323,     0,   319,   310,   311,   216,     0,   183,     0,
       0,     0,   373,   372,   483,   307,   318,   271,   227,   228,
       0,     0,     0,     0,     0,     0,   438,   437,   444,   443,
     280,   279,   386,   387,   389,   483,     0,   530,   331,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   483,   450,
     449,     0,   364,     0,     0,     0,   388,   390,   322,   324,
     325,   328,   329,   330,   326,   327,   321,   369,   367,   483,
     530,   308,   224,   309,   384,   368,   483,   391
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   287,    98,   573,   100,   101,   102,   304,
     103,   288,   568,   572,   569,   104,   105,   106,   159,   796,
     230,   107,   227,   108,   603,   192,   235,   109,   110,   111,
     112,   113,   114,   676,   115,   116,   117,   437,   618,   739,
     118,   119,   614,   734,   120,   121,   474,   751,   122,   123,
     648,   649,   167,   228,   590,   125,   126,   476,   757,   654,
     798,   799,   407,   875,   411,   586,   587,   588,   589,   655,
     309,   722,   976,  1032,   966,   248,   955,   910,   914,   960,
     961,   962,   127,   274,   442,   128,   129,   231,   232,   415,
     416,   607,   973,   929,   419,   604,   993,   907,   842,   289,
     183,   293,   294,   381,   382,   383,   168,   131,   132,   133,
     169,   135,   156,   157,   529,   396,   693,   530,   531,   136,
     170,   171,   139,   193,   384,   173,   141,   174,   175,   144,
     145,   146,   299,   147,   148,   149,   150,   151
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -870
static const yytype_int16 yypact[] =
{
    -870,    93,  2979,  -870,   -13,  -870,  -870,  -870,  -870,  -870,
    -870,  4395,    72,   184,  -870,  9651,  -870,    72,  9651,  -870,
     391,    25,   184,  4395,  9651,  4395,    45,  -870,  9651,  6829,
    -870,  8173,  8755,  6596,  9651,  -870,   136,  -870,  -870, 15054,
    -870,  2479,  8889,  -870,  9651,  9651,  -870,  9651,  -870,  9651,
    -870,  9651,   290,   173,   769,   769,  -870,  -870,  -870,  9023,
    7725,  9651,  8889,  9651,  9651,   124,  -870,   181,  4395,  -870,
    9651,  -870,  9785,  9785, 15054,  -870,  -870,  9651,  9023,  -870,
    9651,  9651,  -870,  -870,  9651,  -870,  -870, 11795,  9651,  9651,
    -870,  9651,  -870,  -870,  3453,  6963,  8307,  -870,  4238,  -870,
     198,  -870,   293,  -870,  -870,  -870,  -870,  -870,  -870,  -870,
    -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,
    -870, 15054,  -870, 15054,   316,    71,  -870,  -870,  2479,  -870,
     226,  -870,  -870,  -870,   240,   249,   267,   264,   282, 15755,
    1986,   139,   309,   319,  -870,  -870,  -870,  -870,  -870,  -870,
    -870,   310,  -870,  -870, 15755,   325,  4395,  -870,  -870,   338,
    9651,  9651,  9651,  9651,  9651,  9023,  9023,   272,  -870,  -870,
    -870,  -870,   372,   317,  -870,  -870,   352, 13969, 15054,  2479,
    -870,   359,  -870,   123, 15755,   411,  7882, 15755,  -870,   186,
    -870, 15054,   202,   429, 15054,   360,    46, 13528,    68, 13458,
     429,  9651,  -870, 13914, 12810,  7882,  -870,  -870,  -870,  -870,
    -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,
    -870,  -870,  -870,  -870,  -870,  -870,  4395,  -870,   363,  2392,
      70,     4,  -870,  4395,  -870,  -870, 13668,   485,   372, 13668,
     372,   485,  -870,  7882,  9651,  -870,  -870, 15054,  -870,   209,
   15755,  9651,  9651,  -870, 15755,   382, 13829,  -870, 13668,   372,
   15755,   365,  7882,  -870, 15755, 14252,  -870,  -870, 14283, 11923,
    -870,  -870, 14422,   424,   388,   372,   213, 14058, 13668, 14453,
     304,   902,   485,   304,   485,   100,  -870,  -870,  3610,    99,
    -870,  9651,  -870,    85,    89,  -870,   -40,    65, 14508,   -35,
     533,  -870,  3767,  -870,   489,   417,   392,  -870,  -870,  -870,
     222,  -870,  9651,  9651,  9651,  9651,  8755,  8755,  9651,  9651,
    9651,  9651,  9919,  9651,   539, 11795,  9651,  9651,  9651,  9651,
    9651,  9651,  9651,  9651,  9651,  9651,  9651,  9651,  9651,  9651,
    9651,  -870,  -870,  -870,  -870,  -870,  -870,  8464,  8464,  -870,
    -870,  -870,  -870,  8464,  -870,  -870,  8464,  8464,  7882,  7882,
    8755,  8755,  7434,  -870,  -870, 14113, 14197, 14592,    54,  3924,
    -870,  8755,    46,   398,   224,  -870,  9651,  -870,  9651,   440,
    -870,   396,   425,  -870,  -870,   406, 15054,  2479,   515,  9023,
    -870,  4552,  8755,  -870,   402,  -870,    46,  4709,  8755,  -870,
      46,   429, 10053,  9651,  -870,  4395,   531,  9651,   409,  -870,
     436,  -870,  -870,  2392,  -870,   441,   412,  -870, 10187,   464,
    9651,  2479,  -870,  -870,  -870,   418,  -870,  9023,  -870, 15755,
   15755,  -870,    29,  -870,  7882,   419,  -870,   561,  -870,   561,
    -870, 10321,   450,  -870,  -870, 10053,  9651,  -870,  -870,  -870,
    -870,  -870,  8598,  -870, 12651,  7120,  -870,  7277,  -870,  4395,
     422,  8755,  6122,  3296,   437,  9651,  6438,  -870,  -870,   333,
    -870,  4081, 15054,  9023,   445, 16001,   218,  -870, 14647,  1332,
    1332,   163,  -870,   163,  -870,   163,  2230,   962,  1412,  1494,
   12053, 12053,   372,   304,  -870,  -870,  -870,  -870,   902,  1011,
     163,   138,   138,  1332,   138,   138,  1138,   304,  1011, 15897,
    1138,   485,   485,   304,   434,   446,   451,   454,   458,   453,
     449,  -870,   163,  -870,   163,    30,  -870,  -870,  -870,   101,
    -870,  1564, 15791,   435, 10455,  8755, 10589,  8755,  9651,  8755,
   12247,    72, 14687,  -870,  -870,  -870, 15755, 14732,  7882,  -870,
    7882,  -870,   417,   225,  9651,   106,  9651, 15755,    57, 13738,
    7434,  -870,  9651, 15755,    69, 13598,  -870,   465,   493,   469,
   14826,   493,   470,   594, 14871,  4395, 13774,  -870,   146,  -870,
    -870,  -870,  -870,  -870,  -870,   764,   203,  -870, 12325,  -870,
     332,   468,  2392,    70,  9651,    45,   136,  9651,  9651,  9651,
    9651,  9651,  5965,  -870,  -870,   455,  8039,  -870, 15755,  -870,
    -870,  -870, 15755,   471,    13,   472,  -870,  1270,  -870,  -870,
     313, 15054,   483,   496,  -870,  -870,  -870,  -870,  -870,  -870,
    -870,  4395,   -37,  2502,  -870,  -870, 15755,  4395, 15755,  -870,
   14912,  -870,  -870,  -870,   505,  1159,   491,   529,   119,  -870,
     573,  -870,  -870,  -870,  -870,  6674,  -870,  -870,  -870,  -870,
    -870,  -870,  -870,  -870,  -870,  7434,  -870,    34,  8755,  8755,
    9651,   626, 15051,  9651,   627, 15082,   500, 12490,    46,    46,
    -870,  -870,  -870,  -870,   506,  -870, 13668,  -870, 12880,  4866,
    -870,  5023,  -870,   204,  -870, 12950,  5180,  -870,    46,  5337,
    -870,    46,  9651,  -870,  9651,  -870,  4395,  9651,  -870,  4395,
     634,  -870,  -870, 15054,   881,  -870,  2392,   537,   592,  -870,
    -870,  -870,    43,  -870,  -870,   464,   509,    74,  -870,  -870,
    -870,  5494,  9023,  -870,  -870,  -870, 15054,  -870,   543,   352,
    -870,  -870,  -870,  -870,  5651,   514,  5808,   524,  -870,  9651,
     519,  1768,  -870,  9651, 15054,  -870,  -870,   332,   538,   549,
    -870,   558,  -870,   567,  -870,   569,   571,   575,   576,   579,
     581,  -870,   582,  -870, 15054,  -870,  -870,  -870,  -870,  -870,
    -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,
    -870,  -870,  -870,  -870,  -870,  9651,  -870,  -870,   583,   538,
     538,  -870,  -870,   464,   211,   268, 15157, 10723, 10857, 15227,
   10991, 11125, 11259, 11393,  -870,  -870,  1935,  -870,  4395,  9651,
   15755,  9651, 15755,  7434,  -870,  4395,  9651, 15755,  -870,  9651,
   15755,  -870, 15263, 15755,  -870, 15755,   685,  4395,   537,  -870,
     559,  9180,    23,  -870,    58,  -870,  -870,  8755, 12150,  4395,
    -870,    26,   564,  9651,  -870,  9651,  -870, 15755,  4395,  9651,
    -870, 15755,  4395, 15755,  -870,  7591,   629,   629,  -870, 14443,
    -870, 15755,  -870,    36,   436,  -870,  -870, 12735, 12410,  -870,
    -870,  -870,  9651,  9651,  9651,  9651,  9651,  9651,  9651,  9651,
     563, 15917, 14871, 13020, 13090,  -870, 14871, 13160, 13230,  9651,
    4395,  -870,   464,   537,  6280,  -870,  -870,  -870,   127,  9023,
    -870,  -870,   129,  9651,   -20, 12560,  -870,   521,   411,  -870,
     352, 15755, 13300,  -870, 13370,  -870,  -870,  -870, 11527,   632,
     300,  -870,  -870,  -870,   538,   538, 15333, 15403, 15439, 15509,
   15579, 15615,  -870,  -870,  4395,  4395,  4395,  4395, 15755,  -870,
    -870,    23,  9337,    86,  -870,  -870, 15755,  -870,    53,  -870,
      -4,  -870,   358, 15685,  -870,  -870,  -870, 11393,  -870,  4395,
    4395,  3136,  -870,  -870,   315,  -870,    58,  -870,  -870,  -870,
    9651,  9651,  9651,  9651,  9651,  9651, 14871, 14871, 14871, 14871,
    -870,  -870,  -870,  -870,  -870,   327,  8755,  2776,  -870,  9651,
     129,    53,    53,    53,    53,    53,    53,   129,   599, 14871,
   14871,   565, 11661,    98,    80, 12596,  -870,  -870, 15755,  -870,
    -870,  -870,  -870,  -870,  -870,  -870,   596,  -870,  -870,   322,
   11929,  -870,  -870,  -870,  9494,  -870,   490,  -870
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -870,  -870,  -870,     0,  -383,  2043,  -870,  -870,  -870,  -870,
     206,   430,  -298,   281,   289,  -870,  -870,   375,   714,  -870,
      15,  -162,  -640,  -870,  -781,  -870,  1612,  -869,  -709,   -46,
    -870,  -870,  -870,   326,  -870,  -870,  -870,   467,  -870,     5,
    -870,  -870,  -870,  -870,  -870,   548,   191,   -72,  -870,  -870,
    -870,    -9,   976,  -870,  -870,  -870,  -870,  -870,  -870,  -870,
    -870,  -132,  -139,  -708,  -870,  -127,    38,   165,  -870,  -870,
    -870,    -1,  -870,  -870,  -266,   625,  -870,  -198,  -218,  -246,
    -238,   -52,  -870,  -172,  -870,   -17,   730,  -100,   345,  -870,
    -381,  -699,  -635,  -870,  -554,  -429,  -789,  -830,  -774,   -49,
    -870,  -347,  -870,  -180,  -870,   217,   550,  -330,  -870,  -870,
    1297,  -870,    -6,  -870,  -870,  -194,  -870,  -546,  -870,  -870,
    1326,  1502,   -11,   -12,    64,   687,  -870,  1761,  2027,  -870,
    -870,  -870,  -870,  -870,  -870,  -870,  -870,  -321
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -531
static const yytype_int16 yytable[] =
{
     154,   357,    97,   179,   172,   400,   373,   177,   245,   246,
     249,   176,   619,   184,   694,   797,   188,   187,   187,   385,
     197,   199,   203,   204,   379,   408,   845,   585,   310,   276,
     613,   236,   591,   237,   238,   417,   239,   927,   240,   725,
     241,   533,   870,   379,   417,   965,   420,   297,   250,   254,
     256,   258,   259,   260,   226,   233,   957,   264,    60,   265,
     905,   268,   272,   425,   902,   394,   275,   277,   394,   278,
     279,   417,   663,   280,   731,   233,   281,   282,   283,   374,
     284,   379,   435,   250,   254,   298,   689,  -370,   605,   273,
    -189,   879,   880,     3,  -375,   466,   539,   233,   696,   999,
     379,   180,   460,   464,   881,   571,   664,   461,   627,   912,
     629,   620,  -375,   803,  -370,   732,   847,   368,   307,   802,
     459,  -375,   994,   744,   255,   467,  1000,  -370,   996,   951,
     357,  -189,   957,   606,   421,   369,   305,   152,   306,  -370,
     539,   308,   959,   911,  -375,   965,  -375,   972,   571,   154,
     365,   366,   367,   264,   250,   298,   427,   909,    60,   295,
     926,   913,   497,   418,  -375,   928,   315,  1017,   844,   733,
     394,   372,   418,   394,   394,   254,   387,   155,   519,   520,
    -375,   369,   870,   179,   462,  -189,  -370,   158,   711,   401,
     187,  -370,  -375,   273,   254,   462,   379,   379,  -375,   418,
    -375,   369,   186,   950,   462,  -117,   273,   958,  1016,   388,
     261,   724,   369,   316,   540,   455,   462,   317,   959,   457,
    -375,    16,   712,   369,   369,   463,   978,   979,   462,   452,
    1031,   665,   254,   187,   848,   369,   427,  -393,   797,   533,
     429,   430,   650,   456,   414,  1037,   997,   458,   234,   754,
     152,   254,   991,   376,   615,   453,   449,  -393,  1030,   666,
    -117,  -393,   426,   319,   687,   320,   321,   322,   234,   323,
     324,   325,   379,   377,   327,   755,    57,   895,   266,   270,
     454,   333,   334,   651,  -117,  -393,   337,   553,   451,   338,
     339,   340,   322,   205,   652,   324,   358,   242,   359,   327,
      76,   478,   479,   480,   481,   483,   485,   486,   487,   488,
     489,   492,   493,   653,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     243,   585,   315,   714,   823,   840,   254,   254,   262,   427,
     555,  -471,   254,   427,   533,   254,   254,   254,   254,   522,
     524,   532,   421,   302,   421,   421,   718,   196,   198,   428,
     542,   715,   824,   444,   303,   546,  -371,   547,   869,  -471,
     698,   701,   477,  -363,   545,   685,  -454,  1028,   250,   316,
     557,   559,   370,   317,   387,   360,   563,   565,   379,   361,
    -453,   570,   570,  -371,   574,  1035,   576,   719,  -470,   311,
    -363,   552,   234,   234,   234,   234,  -371,   312,   720,   608,
     406,   514,   515,  -363,  -458,    21,   612,   516,  -371,   916,
     517,   518,   296,   254,   644,  -363,  -470,   721,   414,  -508,
     714,  -508,  -459,   322,   570,   570,   324,  -385,   745,   747,
     327,   612,   234,  -340,   254,   234,   254,   267,   271,   740,
     633,   612,   636,    21,   638,   640,    50,  -385,   977,  -456,
     578,  -385,   250,  -340,   234,  -371,  -457,    58,  1001,  -455,
    -371,   451,  -363,  -508,  -507,  -508,  -507,  -363,   579,   172,
     275,   580,   362,   152,   234,  -385,   178,   643,   364,   641,
     647,   585,    79,  1002,    50,  1003,  1004,  -376,   581,  1005,
    1006,   582,   533,    52,   324,    58,  -376,   369,   869,   375,
     669,   908,   583,   315,   361,   378,   800,   389,   917,   295,
     410,   295,   434,   557,   672,   563,   675,   574,   677,   636,
      79,   584,   431,  -376,   441,   678,   679,   254,   443,   254,
     468,  -365,     5,   686,   472,   688,   473,   475,   544,   532,
     548,   695,   130,   727,   549,   550,   551,   554,  -376,   560,
     316,   130,   575,  -380,   317,  -376,  -273,   577,  -365,   494,
     593,   592,  -380,   130,   606,   130,   610,   616,   234,   617,
     621,  -365,   631,   172,   656,  -376,   240,   259,   260,   264,
     275,   298,   495,  -365,  -273,   254,   657,   637,   974,  -380,
     645,   658,   234,   717,   659,  -376,   234,   414,   660,   662,
    -376,   661,   682,  -376,   322,   -59,   323,   324,   130,   703,
     705,   327,   995,   702,  -380,   706,   723,   730,    69,   334,
     735,  -380,   738,   741,   496,   427,   273,  1008,   340,   753,
    -365,  -382,   482,   484,   130,  -365,   742,   752,   130,   756,
    -382,  -380,   200,    21,   532,   807,   810,   483,   522,   806,
     812,   816,   809,    28,   244,   837,   841,   995,   843,   846,
     729,  -380,   853,    37,   858,   864,  -380,  -382,   820,  -380,
     822,   -82,  1029,   851,   862,   827,   521,   523,   830,   140,
    -100,   832,    48,   833,    50,   874,   835,   541,   140,  -105,
    1036,  -104,  -382,  -101,   995,    58,   130,  -107,  -103,  -382,
     140,  -106,   140,   -83,  -102,   878,   900,   903,   558,   942,
     292,   250,   920,   975,   564,  1027,  1000,   623,   838,  -382,
      79,   414,   471,   857,   622,   861,   181,   439,   863,   935,
     386,   852,   871,   684,   891,   872,   934,   930,  1033,  -382,
     713,   738,   839,   990,  -382,   140,   873,  -382,  1014,   269,
     269,  1026,  1019,   202,   234,   234,   609,   683,     0,   647,
     234,   234,     0,     0,     0,     0,   130,     0,     0,     0,
       0,   140,   710,   130,   877,   140,     0,   632,   578,   876,
       0,     0,   200,    21,     0,     0,   820,   822,     0,   827,
     830,   857,   861,    28,   244,     0,   579,   892,   893,   580,
     894,   380,   532,    37,   896,   897,  -217,     0,   898,     0,
     245,   246,     0,   737,     0,     0,   581,     0,     0,   582,
     380,   300,    48,     0,    50,     0,   915,   636,   130,  -217,
     583,     0,   921,   140,   922,    58,     0,     0,   924,     0,
       0,     0,   130,     0,   187,   953,     0,     0,     0,   584,
       0,   671,     0,   674,     0,     0,     0,     0,   380,     0,
      79,   892,   936,   937,   896,   938,   939,   940,   941,     0,
       0,     0,     0,     0,   234,   234,     0,   380,   948,     0,
       0,     0,   234,   298,   406,     0,     0,     0,   956,     0,
       0,   406,   963,     0,   234,   578,   998,   234,     0,     0,
       0,     0,     0,   140,     0,     0,     0,     0,     0,   130,
     140,     0,  1013,   579,     0,     0,   580,     0,     0,     0,
     315,     0,     0,   986,   987,   988,   989,     0,     0,     0,
       0,   130,     0,   581,     0,   854,   582,   130,    52,  1020,
    1021,  1022,  1023,  1024,  1025,   130,   861,   583,  1009,  1010,
     298,     0,     0,     0,     0,     0,     0,     0,     0,   986,
     987,   988,   989,  1009,  1010,   140,   584,   316,   124,     0,
       0,   317,     0,   380,   380,  1015,   636,   124,  1018,   140,
     315,     0,     0,     0,   482,   521,     0,     0,     0,   124,
       0,   124,     0,     0,     0,     0,     0,     0,     0,   130,
       0,     0,     0,   130,     0,     0,     0,     0,     0,   636,
       0,   130,     0,   861,     0,     0,     0,   319,     0,   320,
     321,   322,     0,   323,   324,     0,     0,   316,   327,   315,
       0,   317,     0,     0,   124,   333,   334,     0,     0,     0,
     337,     0,   611,   338,   339,   340,   140,   234,     0,   380,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     124,     0,     0,     0,   124,     0,     0,   611,   140,     0,
     292,   667,   292,     0,   140,     0,   316,   611,     0,     0,
     317,   322,   140,   323,   324,     0,     0,     0,   327,   406,
     406,     0,     0,   406,   406,     0,   334,     0,     0,     0,
       0,     0,     0,   338,   339,   340,     0,     0,     0,     0,
     964,     0,     0,     0,     0,   130,   968,     0,   406,     0,
     406,     0,   124,     0,     0,     0,   319,     0,   320,   321,
     322,     0,   323,   324,   325,     0,   140,   327,   328,   329,
     140,     0,   331,   332,   333,   334,     0,     0,   140,   337,
     750,     0,   338,   339,   340,     0,   315,     0,     0,     0,
       0,     0,     0,   681,     0,   380,     0,     0,  -203,     0,
       0,   130,  -203,  -203,     0,     0,     0,   130,     0,     0,
       0,  -203,     0,  -203,  -203,     0,     0,     0,  -203,     0,
       0,     0,   124,  -203,     0,   801,  -203,     0,     0,   124,
       0,     0,     0,   316,     0,     0,     0,   317,   804,   805,
     964,     0,  -203,     0,  -203,     0,  -203,     0,  -203,  -203,
       0,  -203,     0,  -203,     0,  -203,     0,     0,     0,   130,
       0,   130,     0,     0,     0,     0,   130,     0,     0,   130,
       0,     0,     0,     0,  -203,     0,   130,  -203,     0,   130,
    -203,     0,   140,   319,   124,   320,   321,   322,     0,   323,
     324,     0,     0,   206,   327,     0,     0,     0,   124,     0,
       0,   130,   334,     0,     0,   207,   208,     0,     0,   338,
     339,   340,   209,     0,   130,     0,   130,     0,     0,   134,
     210,     0,     0,     0,     0,     0,   211,     0,   134,  -203,
       0,   212,     0,   213,     0,  -203,   214,     0,   140,     0,
     134,     0,   134,   215,   140,     0,     0,     0,   137,     0,
       0,     0,     0,     0,   216,     0,     0,   137,     0,     0,
       0,     0,     0,   217,     0,   124,     0,     0,     0,   137,
       0,   137,   218,   219,     0,   220,     0,   221,     0,   222,
     315,     0,   223,     0,     0,   134,   224,   124,   130,   225,
       0,     0,     0,   124,     0,   130,   140,     0,   140,     0,
       0,   124,     0,   140,     0,     0,   140,   130,     0,     0,
       0,   134,     0,   140,   137,   134,   140,     0,     0,   130,
       0,     0,     0,     0,     0,     0,     0,   316,   130,     0,
       0,   317,   130,     0,     0,     0,     0,     0,   140,     0,
     137,     0,     0,     0,   137,   369,     0,   736,     0,     0,
       0,   140,     0,   140,     0,   124,     0,     0,     0,   124,
     315,   318,     0,     0,     0,     0,     0,   124,     0,     0,
     130,     0,     0,   134,     0,     0,     0,   319,     0,   320,
     321,   322,     0,   323,   324,   325,   906,   326,   327,   328,
     329,     0,     0,   331,   332,   333,   334,     0,   335,   336,
     337,     0,   137,   338,   339,   340,     0,   316,     0,     0,
       0,   317,     0,     0,   130,   130,   130,   130,     0,     0,
       0,     0,     0,     0,   138,   140,     0,     0,     0,     0,
       0,     0,   140,   138,     0,     0,     0,     0,     0,   130,
     130,     0,   315,   134,   140,   138,     0,   138,     0,     0,
     134,     0,     0,     0,   954,     0,   140,   319,     0,     0,
     321,   322,     0,   323,   324,   140,     0,     0,   327,   140,
       0,   124,   137,     0,     0,     0,   334,     0,     0,   137,
       0,     0,     0,   338,   339,   340,     0,     5,     0,   316,
     138,     0,     0,   317,     0,     0,     0,   992,     0,    14,
       0,     0,     0,     0,     0,   134,    19,   140,     0,     0,
       0,     0,     0,     0,     0,     0,   138,     0,     0,   134,
     138,     0,     0,     0,     0,    35,     0,   124,     0,     0,
      38,     0,     0,   124,   137,     0,     0,    43,     0,   319,
       0,     0,   992,   322,     0,   323,   324,     0,   137,     0,
     327,   140,   140,   140,   140,     0,     0,    56,   334,     0,
       0,     0,     0,     0,     0,   338,   339,   340,     0,    66,
       0,     0,     0,    69,     0,     0,   140,   140,   138,   992,
      75,     0,     0,     0,     0,   124,   134,   124,     0,     0,
       0,     0,   124,     0,   257,   124,     0,     0,     0,     0,
       0,     0,   124,     0,     0,   124,     0,     0,   134,     0,
       0,     0,     0,     0,   134,   137,     0,     0,     0,     0,
       0,     0,   134,     0,     0,     0,     0,   124,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   137,     0,     0,
     124,     0,   124,   137,     0,     0,     0,     0,   138,     0,
       0,   137,     0,     0,     0,   138,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   134,     0,     0,     0,
     134,     0,     0,   142,     0,     0,     0,     0,   134,     0,
       0,     0,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   142,   137,   142,    16,     0,   137,
     138,    20,    21,     0,   124,     0,     0,   137,     0,     0,
      27,   124,    28,   865,   138,     0,     0,    33,   390,   393,
     395,   399,    37,   124,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   124,     0,     0,     0,   142,
       0,    48,     0,    50,   124,    52,     0,   866,   124,     0,
     867,     0,    57,     0,    58,     0,     0,     0,   423,     0,
       0,   424,     0,     0,     0,   142,     0,     0,     0,   142,
       0,     0,     0,    74,     0,     0,    76,     0,     0,    79,
     433,   138,   134,     0,     0,     0,   124,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     447,     0,     0,   138,     0,     0,     0,     0,     0,   138,
       0,   137,     0,     0,     0,     0,     0,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   142,    92,     0,
     124,   124,   124,   124,   868,     0,     0,     0,   134,     0,
       0,     0,     0,     0,   134,     0,   890,     0,     0,     0,
       0,     0,     0,     0,     0,   124,   124,     0,     0,     0,
       0,     0,     0,     0,  -203,     0,     0,   137,  -203,  -203,
       0,   138,     0,   137,     0,   138,     0,  -203,     0,  -203,
    -203,     0,     0,   138,  -203,     0,     0,     0,     0,  -203,
       0,     0,  -203,     0,   543,     0,   134,   142,   134,     0,
       0,     0,     0,   134,   142,     0,   134,     0,  -203,     0,
    -203,     0,  -203,   134,  -203,  -203,   134,  -203,   561,  -203,
       0,  -203,   566,     0,     0,   137,     0,   137,     0,     0,
       0,     0,   137,     0,     0,   137,     0,     0,   134,   143,
    -203,     0,   137,  -203,     0,   137,  -203,     0,   143,     0,
       0,   134,     0,   134,     0,    99,     0,     0,     0,   142,
     143,     0,   143,     0,   153,     0,     0,   137,     0,     0,
       0,     0,     0,   142,     0,     0,   182,     0,   185,     0,
     137,     0,   137,     0,     0,     0,     0,   138,     0,     0,
       0,     0,     0,     0,     0,  -203,     0,     0,     0,     0,
       0,  -203,     0,     0,     0,   143,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   263,     0,     0,     0,   134,     0,     0,     0,     0,
       0,   143,   134,     0,     0,   143,  -478,     0,     0,     0,
     142,     0,     0,   138,   134,     0,     0,    99,     0,   138,
     356,   301,     0,  -507,   137,  -507,   134,     0,     0,     0,
       0,   137,   142,     0,     0,   134,     0,     0,   142,   134,
       0,     0,     0,   137,     0,     0,   142,     0,     0,     0,
     690,   692,     0,     0,     0,   137,   697,   700,     0,     0,
       0,     0,     0,   143,   137,     0,     0,     0,   137,     0,
       0,   138,     0,   138,     0,     0,     0,   134,   138,   363,
       0,   138,     0,     0,     0,     0,     0,     0,   138,     0,
       0,   138,     0,     0,     0,     0,     0,     0,     0,     0,
     142,     0,     0,     0,   142,     0,   137,     0,     0,     0,
       0,     0,   142,   138,     0,     0,     0,     0,     0,     0,
       0,   134,   134,   134,   134,     0,   138,     0,   138,     0,
       0,     0,     0,   143,     0,     0,     0,     0,   315,     0,
     143,     0,     0,     0,     0,     0,   134,   134,     0,   409,
     137,   137,   137,   137,     0,     0,   422,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     814,   815,     0,     0,     0,   137,   137,     0,   817,     0,
       0,     0,     0,     0,     0,   316,     0,     0,     0,   317,
     828,     0,     0,   831,     0,   143,     0,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,   138,     0,   143,
       0,    99,     0,     0,     0,     0,   142,     0,     0,   138,
       0,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,   138,     0,     0,     0,   319,     0,   320,   321,   322,
     138,   323,   324,   325,   138,   326,   327,   328,   329,     0,
       0,   331,   332,   333,   334,     0,   335,     0,   337,     0,
       0,   338,   339,   340,     0,     0,     0,     0,     0,     0,
       0,     0,   142,     0,     0,   206,   143,     0,   142,     0,
       0,     0,   138,     0,     0,     0,     0,   207,   208,     0,
       0,     0,    99,     0,   209,     0,     0,     0,   143,     0,
       0,     0,   210,     0,   143,     0,     0,     0,   211,     0,
       0,     0,   143,   212,   422,   213,     0,     0,   214,     0,
     422,     0,     0,     0,     0,   215,   138,   138,   138,   138,
     142,     0,   142,     0,     0,     0,   216,   142,     0,     0,
     142,     0,     0,   919,     0,   217,     0,   142,     0,     0,
     142,   138,   138,     0,   218,   219,     0,   220,     0,   221,
       0,   222,   206,     0,   223,     0,   143,     0,   224,   412,
     143,   225,   142,     0,   207,   208,     0,     0,   143,     0,
       0,   209,   630,     0,     0,   142,   635,   142,     0,   210,
       0,     0,   313,     0,    99,   211,     0,     0,     0,   314,
     212,     0,   213,     0,     0,   214,     0,     0,     0,     0,
     315,     0,   215,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   216,     0,     0,     0,     0,     0,   413,
       0,     0,   217,     0,     0,     0,     0,     0,     0,     0,
       0,   218,   219,     0,   220,     0,   221,     0,   222,     0,
       0,   223,     0,     0,     0,   224,     0,   316,   225,   142,
       0,   317,     0,     0,     0,     0,   142,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   142,     0,
       0,     0,   143,     0,     0,     0,     0,   394,     0,     0,
     142,   318,     0,     0,     0,     0,     0,     0,   708,   142,
       0,     0,     0,   142,     0,     0,     0,   319,     0,   320,
     321,   322,     0,   323,   324,   325,   229,   326,   327,   328,
     329,   330,     0,   331,   332,   333,   334,     0,   335,   336,
     337,     0,     0,   338,   339,   340,     0,     0,   143,     0,
       0,   142,   746,     0,   143,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   743,     0,     0,     0,     0,     0,
     748,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   142,   142,   142,   142,     0,
       0,     0,     0,     0,     0,     0,   143,     0,   143,     0,
       0,     0,     0,   143,     0,     0,   143,     0,     0,     0,
     142,   142,   422,   143,   422,     0,   143,     0,     0,   422,
       0,     0,   422,     0,     0,     0,     0,     0,     0,   834,
       0,     0,   836,     0,     0,     0,     0,     0,   143,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   143,     0,   143,   850,     0,     0,     0,     0,     5,
       6,     7,     8,     9,    10,     0,     0,   856,   160,   860,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,     0,    36,
       0,     0,    38,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,   143,     0,     0,     0,    56,
       0,     0,   143,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,   143,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,   143,     0,     0,     0,
     901,     0,     0,     0,     0,   143,  -384,     0,     0,   143,
       0,     0,   918,     0,     0,     0,     0,     0,     0,     0,
       0,   923,    84,     0,     0,   925,  -384,     0,     0,     0,
    -384,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,   247,     0,   143,     0,     0,
       0,   165,     0,    95,  -384,   904,     0,     0,     0,     0,
       0,     0,     0,   949,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   143,   143,   143,   143,     0,     0,     0,     0,    -2,
       4,     0,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,   143,   143,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,    25,    26,
       0,    27,     0,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,    37,     0,    38,  -217,    39,    40,    41,
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
       0,     0,    93,     0,    94,     0,    95,  1011,    96,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   160,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,     0,    36,
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
       0,    89,     0,    90,    91,   247,     0,     0,     0,    93,
       0,   165,     0,    95,     0,   166,  1012,     4,     0,     5,
       6,     7,     8,     9,    10,     0,  -530,     0,    11,    12,
      13,    14,    15,  -530,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,  -530,    25,    26,  -530,    27,     0,
      28,    29,    30,    31,    32,    33,    34,    35,     0,    36,
      37,     0,    38,  -217,    39,    40,    41,     0,    42,    43,
      44,   -43,    45,    46,     0,    47,     0,     0,     0,    48,
      49,    50,    51,     0,    53,    54,  -217,   -43,    55,    56,
      57,  -530,    58,    59,    60,  -530,    61,    62,    63,    64,
      65,    66,    67,    68,     0,    69,    70,     0,    71,    72,
      73,    74,    75,     0,    76,    77,    78,    79,     0,     0,
      80,     0,    81,     0,     0,  -530,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -530,    84,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
       0,  -530,  -530,  -530,  -530,  -530,     0,  -530,  -530,  -530,
    -530,    89,  -530,  -530,  -530,     0,    92,  -530,  -530,  -530,
       0,    94,  -530,    95,   285,    96,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,    25,    26,     0,    27,     0,    28,    29,    30,
      31,    32,    33,    34,    35,     0,    36,    37,     0,    38,
    -217,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,     0,     0,     0,    48,    49,    50,    51,
      52,    53,    54,  -217,   -43,    55,    56,    57,     0,    58,
      59,    60,     0,    61,    62,    63,    64,    65,    66,    67,
      68,     0,    69,    70,     0,    71,    72,    73,    74,    75,
       0,    76,    77,    78,    79,     0,     0,    80,     0,    81,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,   247,    92,     0,     0,    93,     0,    94,   286,
      95,     4,    96,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
      34,    35,     0,    36,    37,     0,    38,  -217,    39,    40,
      41,     0,    42,    43,    44,   -43,    45,    46,     0,    47,
       0,     0,     0,    48,    49,    50,    51,    52,    53,    54,
    -217,   -43,    55,    56,    57,     0,    58,    59,    60,     0,
      61,    62,    63,    64,    65,    66,    67,    68,     0,    69,
      70,     0,    71,    72,    73,    74,    75,     0,    76,    77,
      78,    79,     0,     0,    80,     0,    81,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,     0,    89,     0,    90,    91,     0,
      92,     0,     0,    93,     0,    94,   450,    95,   469,    96,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,    26,     0,    27,
       0,    28,    29,    30,    31,    32,    33,    34,    35,     0,
      36,    37,     0,    38,  -217,    39,    40,    41,     0,    42,
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
      38,  -217,    39,    40,    41,     0,    42,    43,    44,   -43,
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
     286,    95,     4,    96,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
      25,    26,     0,    27,     0,    28,    29,    30,    31,    32,
      33,    34,    35,     0,    36,    37,     0,    38,  -217,    39,
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
       0,    92,     0,     0,    93,     0,    94,   642,    95,     4,
      96,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,    34,    35,
       0,    36,    37,     0,    38,  -217,    39,    40,    41,     0,
      42,    43,    44,   -43,    45,    46,     0,    47,     0,     0,
       0,    48,    49,    50,    51,   300,    53,    54,  -217,   -43,
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
      29,    30,    31,    32,    33,    34,    35,     0,    36,    37,
       0,    38,  -217,    39,    40,    41,     0,    42,    43,    44,
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
      32,    33,   556,    35,     0,    36,    37,     0,    38,  -217,
      39,    40,    41,     0,    42,    43,    44,   -43,    45,    46,
       0,    47,     0,     0,     0,    48,    49,    50,    51,     0,
      53,    54,  -217,   -43,    55,    56,    57,     0,    58,    59,
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
      18,    19,    20,    21,    22,    23,    24,     0,    25,    26,
       0,    27,     0,    28,    29,    30,    31,    32,    33,   562,
      35,     0,    36,    37,     0,    38,  -217,    39,    40,    41,
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
      28,    29,    30,    31,    32,    33,   819,    35,     0,    36,
      37,     0,    38,  -217,    39,    40,    41,     0,    42,    43,
      44,   -43,    45,    46,     0,    47,     0,     0,     0,    48,
      49,    50,    51,     0,    53,    54,  -217,   -43,    55,    56,
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
      31,    32,    33,   821,    35,     0,    36,    37,     0,    38,
    -217,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
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
     826,    35,     0,    36,    37,     0,    38,  -217,    39,    40,
      41,     0,    42,    43,    44,   -43,    45,    46,     0,    47,
       0,     0,     0,    48,    49,    50,    51,     0,    53,    54,
    -217,   -43,    55,    56,    57,     0,    58,    59,    60,     0,
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
       0,    28,    29,    30,    31,    32,    33,   829,    35,     0,
      36,    37,     0,    38,  -217,    39,    40,    41,     0,    42,
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
      23,    24,     0,   849,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,    37,     0,
      38,  -217,    39,    40,    41,     0,    42,    43,    44,   -43,
      45,    46,     0,    47,     0,     0,     0,    48,    49,    50,
      51,     0,    53,    54,  -217,   -43,    55,    56,    57,     0,
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
      33,   855,    35,     0,    36,    37,     0,    38,  -217,    39,
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
      27,     0,    28,    29,    30,    31,    32,    33,   859,    35,
       0,    36,    37,     0,    38,  -217,    39,    40,    41,     0,
      42,    43,    44,   -43,    45,    46,     0,    47,     0,     0,
       0,    48,    49,    50,    51,     0,    53,    54,  -217,   -43,
      55,    56,    57,     0,    58,    59,    60,     0,    61,    62,
      63,    64,    65,    66,    67,    68,     0,    69,    70,     0,
      71,    72,    73,    74,    75,     0,    76,    77,    78,    79,
       0,     0,    80,     0,    81,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    89,     0,    90,    91,     0,    92,     0,
       0,    93,     0,    94,     0,    95,   726,    96,     5,     6,
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
      89,     0,    90,    91,   247,     0,     0,     0,    93,     0,
     165,     0,    95,     0,   166,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   160,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   163,    35,     0,    36,     0,     0,    38,     0,
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
       0,   166,   634,     5,     6,     7,     8,     9,    10,     0,
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
       0,     0,     0,    88,     0,    89,     0,    90,    91,   247,
       0,     0,     0,    93,     0,   165,     0,    95,     0,   166,
     952,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     160,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   161,   162,     0,   163,    35,
       0,    36,     0,     0,    38,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   164,     0,    69,     0,     0,
      71,     0,     0,     0,    75,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    89,     0,    90,    91,     0,     0,     0,
       0,    93,     0,   165,     0,    95,     0,   166,   639,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   160,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,   200,
      21,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,   201,    30,   161,   162,     0,   163,    35,     0,    36,
       0,     0,    38,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,    50,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,    58,     0,     0,   758,     0,   759,    63,    64,
      65,    66,    67,   164,   760,    69,     0,     0,    71,   761,
     208,   762,    75,     0,     0,    77,   763,    79,     0,     0,
       0,     0,     0,     0,   210,    82,     0,     0,     0,     0,
     764,     0,     0,     0,     0,   765,     0,   213,     0,     0,
     766,    83,    84,    85,    86,     0,     0,   767,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,   216,    88,
       0,    89,     0,    90,    91,     0,     0,   768,     0,    93,
       0,   165,     0,    95,     0,   166,   218,   219,     0,   769,
       0,   221,     0,   770,     0,     0,   771,     0,     0,     0,
     772,     0,     0,   225,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   341,   342,   343,   344,   345,   346,
       0,     0,   349,   350,   351,   352,     0,   354,   355,   773,
     774,   775,   776,     0,     0,   777,     0,     0,     0,   778,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
     789,     0,   790,     0,     0,   791,   792,   793,   794,     0,
       0,   795,     5,     6,     7,     8,   189,    10,   190,     0,
       0,   160,     0,     0,    14,    15,     0,     0,  -116,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,  -116,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,     0,     0,   191,    75,     0,  -116,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    88,     0,    89,   160,    90,    91,    14,    15,
       0,     0,    93,     0,   165,    19,    95,     0,   166,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,   251,   252,     0,    75,
     290,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,   291,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,   247,     0,     0,     0,    93,     0,   165,     0,
      95,     0,   166,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   160,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,   251,   252,     0,    75,   290,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,     0,    89,     0,    90,    91,   247,
       0,     0,     0,    93,     0,   165,     0,    95,   626,   166,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   160,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
     251,   252,     0,    75,   290,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    89,     0,    90,    91,   247,     0,     0,     0,
      93,     0,   165,     0,    95,   628,   166,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   160,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,   525,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,   526,    36,     0,     0,
      38,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
     527,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   164,     0,    69,     0,     0,    71,     0,     0,     0,
      75,     0,     0,    77,     0,   528,     0,     0,     0,     0,
       0,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,    89,
       0,    90,    91,     0,     0,     0,     0,    93,     0,   165,
       0,    95,     0,   166,     5,     6,     7,     8,   189,    10,
       0,     0,     0,   160,     0,     0,    14,    15,     0,     0,
    -116,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,  -116,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,     0,     0,   191,    75,     0,  -116,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
      66,    67,   164,     0,    69,     0,     0,    71,   251,   252,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
      89,     0,    90,    91,     0,   253,     0,     0,    93,     0,
     165,     0,    95,     0,   166,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   160,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   163,    35,     0,    36,     0,     0,    38,     0,
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
      91,   247,     0,     0,     0,    93,     0,   165,     0,    95,
       0,   166,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   160,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,   728,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,   251,   252,     0,    75,     0,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    88,     0,    89,   160,    90,    91,    14,    15,
       0,     0,    93,     0,   165,    19,    95,     0,   166,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
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
      36,     0,     0,    38,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,   195,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    89,     0,    90,    91,   247,     0,     0,     0,
      93,     0,   165,     0,    95,     0,   166,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   160,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,     0,    36,     0,     0,
      38,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   164,     0,    69,     0,     0,    71,   251,   252,     0,
      75,     0,     0,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    88,     0,    89,
     160,    90,    91,    14,    15,     0,     0,    93,     0,   165,
      19,    95,     0,   166,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   161,   162,     0,   163,    35,
       0,    36,     0,     0,    38,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   164,     0,    69,     0,     0,
      71,     0,     0,     0,    75,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    89,     0,    90,    91,   247,     0,     0,
       0,    93,     0,   165,   624,    95,     0,   166,     5,     6,
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
       0,     0,   195,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    88,     0,
      89,   160,    90,    91,    14,    15,     0,     0,    93,     0,
     165,    19,    95,     0,   166,     0,     0,     0,   233,    26,
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
       0,     0,    87,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    88,     0,    89,   160,    90,    91,    14,    15,
       0,     0,    93,     0,    94,    19,    95,     0,   166,     0,
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
      90,    91,   247,     0,     0,     0,    93,     0,   165,     0,
      95,     0,   166,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   160,     0,     0,    14,   594,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
     595,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,   596,     0,     0,    38,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,   597,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,   598,   599,    65,    66,    67,   600,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,   601,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,     0,    89,     0,    90,    91,   247,
       0,     0,     0,    93,     0,   165,     0,    95,     0,   904,
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
      88,     0,    89,     0,    90,    91,   247,     0,     0,     0,
      93,     0,   165,     0,    95,     0,   904,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   160,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   889,    35,     0,    36,     0,     0,
      38,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   164,     0,    69,     0,     0,    71,     0,     0,     0,
      75,     0,     0,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,    89,
       0,    90,    91,   247,     0,     0,     0,    93,     0,   165,
       0,    95,     0,   166,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   160,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
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
      89,   160,    90,    91,    14,   490,     0,     0,    93,     0,
      94,    19,    95,     0,   166,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    88,     0,    89,   160,    90,    91,    14,    15,
       0,     0,    93,     0,   165,    19,    95,     0,   166,     0,
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
      90,    91,    14,   594,     0,     0,   567,     0,   165,    19,
      95,     0,   166,     0,     0,     0,     0,   595,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
     596,     0,     0,    38,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,   597,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,   598,
     599,    65,    66,    67,   600,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,   601,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      88,     0,    89,   160,    90,    91,    14,    15,     0,     0,
      93,     0,   165,    19,    95,     0,   602,     0,     0,     0,
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
       0,     0,     0,     0,    87,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    88,     0,    89,   160,    90,    91,
      14,    15,     0,     0,    93,     0,   165,    19,    95,     0,
     602,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   670,    35,     0,    36,     0,
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
       0,     0,     0,     0,     0,    30,   161,   162,     0,   673,
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
     161,   162,     0,   883,    35,     0,    36,     0,     0,    38,
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
       0,     0,     0,    30,   161,   162,     0,   884,    35,     0,
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
       0,   886,    35,     0,    36,     0,     0,    38,     0,     0,
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
       0,    30,   161,   162,     0,   887,    35,     0,    36,     0,
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
       0,     0,     0,     0,     0,    30,   161,   162,     0,   888,
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
     161,   162,     0,   889,    35,     0,    36,     0,     0,    38,
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
      90,    91,    14,   594,     0,     0,    93,     0,   165,    19,
      95,     0,   166,     0,     0,     0,     0,   595,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
     596,     0,     0,    38,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,   597,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,   598,
     599,    65,    66,    67,   600,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,   601,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      88,     0,    89,   160,    90,    91,    14,    15,     0,     0,
      93,     0,   165,    19,    95,     0,   971,     0,     0,     0,
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
       0,     0,     0,     0,    87,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    88,     0,    89,   160,    90,    91,
      14,    15,     0,     0,    93,     0,     0,    19,    95,     0,
     971,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    88,     0,
      89,   160,     0,    91,    14,    15,     0,     0,    93,     0,
     165,    19,    95,     0,   166,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,    40,    41,
    -366,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,  -366,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
    -366,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,  -366,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,     0,
       0,     0,     0,     0,     0,    84,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   160,     0,     0,    14,    15,
       0,     0,     0,     0,    89,    19,     0,   356,     0,  -366,
    -507,     0,  -507,    26,  -366,     0,    95,     0,   971,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   160,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,    84,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,    89,    40,
      41,     0,     0,    43,    44,     0,    45,    46,   165,     0,
      95,     0,   166,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   160,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,    84,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,    89,    40,    41,     0,     0,
      43,    44,     0,    45,    46,   165,     0,    95,     0,   602,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,   206,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
     207,   208,     0,    75,     0,     0,    77,   209,     0,     0,
       0,     0,     0,     0,     0,   210,     0,     0,     0,     0,
       0,   211,     0,     0,     0,     0,   212,     0,   213,     0,
       0,   214,     0,    84,     0,     0,     0,     0,   215,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   216,
       0,     0,    89,     0,     0,     0,     0,     0,   217,     0,
       0,     0,     0,     0,    95,     0,   166,   218,   219,     0,
     220,     0,   221,   932,   222,     0,     0,   223,     0,     0,
     760,   224,     0,     0,   225,   207,   208,   762,     0,     0,
       0,     0,   209,     0,     0,     0,     0,     0,     0,     0,
     210,     0,     0,     0,     0,     0,   764,     0,     0,     0,
       0,   212,     0,   213,     0,     0,   214,     0,     0,     0,
       0,     0,     0,   215,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   216,     0,     0,     0,     0,     0,
       0,     0,   716,   217,     0,     0,     0,     0,     0,     0,
       0,     0,   218,   219,     0,   220,     0,   221,     0,   933,
     313,     0,   771,     0,     0,     0,   224,   314,     0,   225,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
     341,   342,   343,   344,   345,   346,     0,     0,   349,   350,
     351,   352,     0,   354,   355,   773,   774,   775,   776,     0,
       0,   777,     0,     0,     0,   778,   779,   780,   781,   782,
     783,   784,   785,   786,   787,   788,   789,     0,   790,     0,
       0,   791,   792,   793,   794,   316,     0,     0,     0,   317,
     313,     0,     0,     0,     0,     0,     0,   314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   318,
       0,     0,     0,     0,     0,     0,   313,     0,     0,     0,
       0,     0,     0,   314,     0,   319,     0,   320,   321,   322,
       0,   323,   324,   325,   315,   326,   327,   328,   329,   330,
       0,   331,   332,   333,   334,   316,   335,   336,   337,   317,
       0,   338,   339,   340,     0,     0,     0,     0,     0,     0,
     813,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,     0,     0,     0,     0,     0,     0,   314,   318,
       0,   316,     0,     0,     0,   317,     0,     0,     0,   315,
       0,     0,     0,     0,     0,   319,     0,   320,   321,   322,
       0,   323,   324,   325,     0,   326,   327,   328,   329,   330,
       0,   331,   332,   333,   334,   318,   335,   336,   337,     0,
       0,   338,   339,   340,     0,     0,     0,     0,     0,     0,
     967,   319,     0,   320,   321,   322,   316,   323,   324,   325,
     317,   326,   327,   328,   329,   330,     0,   331,   332,   333,
     334,     0,   335,   336,   337,   313,     0,   338,   339,   340,
       0,     0,   314,     0,     0,     0,  1034,     0,     0,     0,
     318,     0,     0,   315,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   319,     0,   320,   321,
     322,     0,   323,   324,   325,     0,   326,   327,   328,   329,
     330,     0,   331,   332,   333,   334,     0,   335,   336,   337,
       0,     0,   338,   339,   340,     0,     0,     0,     0,   625,
     316,     0,     0,     0,   317,     0,     0,     0,     0,     0,
     313,     0,     0,     0,     0,     0,     0,   314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,     0,   318,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     319,     0,   320,   321,   322,     0,   323,   324,   325,     0,
     326,   327,   328,   329,   330,     0,   331,   332,   333,   334,
       0,   335,   336,   337,     0,   316,   338,   339,   340,   317,
     313,     0,     0,   931,     0,     0,     0,   314,   405,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   318,
     341,   342,   343,   344,   345,   346,     0,     0,   349,   350,
     351,   352,     0,   354,   355,   319,     0,   320,   321,   322,
       0,   323,   324,   325,     0,   326,   327,   328,   329,   330,
       0,   331,   332,   333,   334,   316,   335,   336,   337,   317,
     313,   338,   339,   340,     0,   369,     0,   314,   818,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   318,
     341,   342,   343,   344,   345,   346,     0,     0,   349,   350,
     351,   352,     0,   354,   355,   319,     0,   320,   321,   322,
       0,   323,   324,   325,     0,   326,   327,   328,   329,   330,
       0,   331,   332,   333,   334,   316,   335,   336,   337,   317,
     313,   338,   339,   340,     0,   369,     0,   314,   825,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   318,
     341,   342,   343,   344,   345,   346,     0,     0,   349,   350,
     351,   352,     0,   354,   355,   319,     0,   320,   321,   322,
       0,   323,   324,   325,     0,   326,   327,   328,   329,   330,
       0,   331,   332,   333,   334,   316,   335,   336,   337,   317,
     313,   338,   339,   340,     0,   369,     0,   314,   944,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   318,
     341,   342,   343,   344,   345,   346,     0,     0,   349,   350,
     351,   352,     0,   354,   355,   319,     0,   320,   321,   322,
       0,   323,   324,   325,     0,   326,   327,   328,   329,   330,
       0,   331,   332,   333,   334,   316,   335,   336,   337,   317,
     313,   338,   339,   340,     0,   369,     0,   314,   945,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   318,
     341,   342,   343,   344,   345,   346,     0,     0,   349,   350,
     351,   352,     0,   354,   355,   319,     0,   320,   321,   322,
       0,   323,   324,   325,     0,   326,   327,   328,   329,   330,
       0,   331,   332,   333,   334,   316,   335,   336,   337,   317,
     313,   338,   339,   340,     0,   369,     0,   314,   946,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   318,
     341,   342,   343,   344,   345,   346,     0,     0,   349,   350,
     351,   352,     0,   354,   355,   319,     0,   320,   321,   322,
       0,   323,   324,   325,     0,   326,   327,   328,   329,   330,
       0,   331,   332,   333,   334,   316,   335,   336,   337,   317,
     313,   338,   339,   340,     0,   369,     0,   314,   947,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   318,
     341,   342,   343,   344,   345,   346,     0,     0,   349,   350,
     351,   352,     0,   354,   355,   319,     0,   320,   321,   322,
       0,   323,   324,   325,     0,   326,   327,   328,   329,   330,
       0,   331,   332,   333,   334,   316,   335,   336,   337,   317,
     313,   338,   339,   340,     0,   369,     0,   314,   969,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   318,
     341,   342,   343,   344,   345,   346,     0,     0,   349,   350,
     351,   352,     0,   354,   355,   319,     0,   320,   321,   322,
       0,   323,   324,   325,     0,   326,   327,   328,   329,   330,
       0,   331,   332,   333,   334,   316,   335,   336,   337,   317,
       0,   338,   339,   340,     0,   369,     0,     0,   970,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   313,     0,
       0,     0,     0,     0,     0,   314,     0,     0,     0,   318,
     341,   342,   343,   344,   345,   346,   315,   397,   349,   350,
     351,   352,     0,   354,   355,   319,     0,   320,   321,   322,
     398,   323,   324,   325,     0,   326,   327,   328,   329,   330,
       0,   331,   332,   333,   334,     0,   335,   336,   337,     0,
       0,   338,   339,   340,     0,   369,     0,     0,     0,     0,
       0,     0,     0,   316,     0,     0,     0,   317,   313,     0,
       0,     0,     0,     0,     0,   314,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   315,   391,     0,     0,
       0,     0,     0,   394,     0,     0,     0,   318,     0,     0,
     392,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,   316,   335,   336,   337,   317,   313,   338,
     339,   340,     0,   369,     0,   314,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   315,   699,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,   316,   335,   336,   337,   317,   313,   338,
     339,   340,     0,   369,     0,   314,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   315,   233,     0,     0,
       0,     0,     0,   394,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,   316,   335,   336,   337,   317,   313,   338,
     339,   340,     0,   369,     0,   314,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   315,   691,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,   313,     0,     0,     0,     0,     0,
       0,   314,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,   315,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,   316,   335,   336,   337,   317,     0,   338,
     339,   340,     0,   369,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   313,
       0,     0,     0,     0,     0,     0,   314,   318,     0,   316,
       0,     0,     0,   317,     0,     0,     0,   315,     0,     0,
       0,     0,   709,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,   318,   335,   336,   337,     0,     0,   338,
     339,   340,     0,   369,     0,     0,     0,     0,     0,   319,
       0,   320,   321,   322,   316,   323,   324,   325,   317,   326,
     327,   328,   329,   330,     0,   331,   332,   333,   334,     0,
     335,   336,   337,     0,   313,   338,   339,   340,     0,   369,
       0,   314,     0,     0,     0,     0,     0,     0,   318,     0,
       0,     0,   315,     0,     0,     0,     0,   402,     0,     0,
       0,     0,     0,     0,   319,     0,   320,   321,   322,     0,
     323,   324,   325,     0,   326,   327,   328,   329,   330,     0,
     331,   332,   333,   334,   403,   335,   336,   337,     0,   313,
     338,   339,   340,     0,   432,     0,   314,     0,     0,   316,
       0,     0,     0,   317,     0,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   371,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   318,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   319,
       0,   320,   321,   322,   316,   323,   324,   325,   317,   326,
     327,   328,   329,   330,     0,   331,   332,   333,   334,     0,
     335,   336,   337,     0,   404,   338,   339,   340,   313,     0,
       0,     0,     0,     0,   155,   314,     0,     0,   318,     0,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
       0,   445,     0,     0,   319,     0,   320,   321,   322,     0,
     323,   324,   325,     0,   326,   327,   328,   329,   330,     0,
     331,   332,   333,   334,     0,   335,   336,   337,   446,     0,
     338,   339,   340,   313,     0,     0,     0,     0,     0,     0,
     314,     0,     0,   316,     0,     0,     0,   317,     0,     0,
       0,   315,   534,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   535,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,   316,   323,
     324,   325,   317,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,   313,     0,   338,
     339,   340,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,   318,     0,     0,   315,   536,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,   537,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,   313,     0,   338,   339,   340,     0,     0,   314,
       0,     0,   316,     0,     0,     0,   317,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,     0,     0,     0,     0,     0,     0,
     314,     0,     0,     0,     0,     0,   318,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   319,     0,   320,   321,   322,   316,   323,   324,
     325,   317,   326,   327,   328,   329,   330,     0,   331,   332,
     333,   334,     0,   335,   336,   337,     0,     0,   338,   339,
     340,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,   318,   317,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   319,     0,   320,
     321,   322,     0,   323,   324,   325,     0,   326,   327,   328,
     329,   330,   318,   331,   332,   333,   334,     0,   335,   336,
     337,     0,   436,   338,   339,   340,     0,     0,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,   313,   438,   338,   339,   340,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    16,   313,     0,     0,    20,    21,     0,     0,
     314,     0,     0,     0,     0,    27,     0,    28,   865,     0,
       0,   315,    33,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   316,     0,     0,
       0,   317,     0,     0,     0,     0,    48,     0,    50,     0,
     300,     0,   866,     0,     0,   867,     0,    57,   313,    58,
       0,     0,     0,     0,     0,   314,     0,     0,   316,     0,
       0,   318,   317,     0,     0,     0,   315,     0,    74,     0,
       0,    76,     0,     0,    79,     0,     0,   319,     0,   320,
     321,   322,     0,   323,   324,   325,     0,   326,   327,   328,
     329,   330,   318,   331,   332,   333,   334,     0,   335,   336,
     337,     0,   440,   338,   339,   340,     0,     0,   319,     0,
     320,   321,   322,   316,   323,   324,   325,   317,   326,   327,
     328,   329,   330,    92,   331,   332,   333,   334,     0,   335,
     336,   337,   313,   448,   338,   339,   340,     0,     0,   314,
       0,     0,     0,     0,     0,     0,   465,   318,     0,     0,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,   313,     0,   338,
     339,   340,     0,     0,   314,     0,     0,   316,     0,     0,
       0,   317,     0,     0,     0,   315,     0,     0,     0,     0,
     538,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,     0,     0,
       0,   318,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,   319,     0,   320,
     321,   322,   316,   323,   324,   325,   317,   326,   327,   328,
     329,   330,     0,   331,   332,   333,   334,     0,   335,   336,
     337,     0,   313,   338,   339,   340,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,   318,     0,     0,     0,
     315,     0,   316,     0,     0,     0,   317,     0,     0,     0,
       0,     0,   319,     0,   320,   321,   322,     0,   323,   324,
     325,     0,   326,   327,   328,   329,   330,  -479,   331,   332,
     333,   334,   155,   335,   336,   337,   318,     0,   338,   339,
     340,     0,     0,     0,     0,     0,     0,   316,     0,     0,
       0,   317,   319,     0,   320,   321,   322,     0,   323,   324,
     325,     0,   326,   327,   328,   329,   330,     0,   331,   332,
     333,   334,     0,   335,   336,   337,   313,   704,   338,   339,
     340,   318,     0,   314,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   315,     0,     0,   319,     0,   320,
     321,   322,     0,   323,   324,   325,     0,   326,   327,   328,
     329,   330,     0,   331,   332,   333,   334,     0,   335,   336,
     337,   313,   680,   338,   339,   340,     0,     0,   314,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
       0,   316,   707,     0,     0,   317,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,     0,     0,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,   318,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,   316,     0,     0,     0,
     317,   319,     0,   320,   321,   322,     0,   323,   324,   325,
       0,   326,   327,   328,   329,   330,     0,   331,   332,   333,
     334,     0,   335,   336,   337,     0,     0,   338,   339,   340,
     318,     0,     0,     0,     0,     0,     0,   316,     0,     0,
       0,   317,     0,     0,     0,     0,   319,     0,   320,   321,
     322,     0,   323,   324,   325,     0,   326,   327,   328,   329,
     330,     0,   331,   332,   333,   334,     0,   335,   336,   337,
     749,   318,   338,   339,   340,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   319,     0,   320,
     321,   322,     0,   323,   324,   325,     0,   326,   327,   328,
     329,   330,     0,   331,   332,   333,   334,   206,   335,   336,
     337,   313,     0,   338,   339,   340,     0,     0,   314,   207,
     208,     0,     0,     0,     0,     0,   209,     0,     0,   315,
     808,     0,     0,     0,   210,     0,     0,     0,     0,     0,
     211,     0,   313,     0,     0,   212,     0,   213,     0,   314,
     214,     0,     0,     0,     0,     0,     0,   215,     0,     0,
     315,   811,     0,     0,     0,     0,     0,     0,   216,     0,
       0,     0,     0,     0,     0,     0,   316,   217,     0,     0,
     317,     0,     0,     0,     0,     0,   218,   219,     0,   220,
       0,   221,     0,   222,     0,     0,   223,     0,     0,     0,
     224,     0,     0,   225,     0,     0,     0,   316,     0,     0,
     318,   317,     0,     0,     0,     0,     0,   313,     0,     0,
       0,     0,     0,     0,   314,     0,   319,     0,   320,   321,
     322,     0,   323,   324,   325,   315,   326,   327,   328,   329,
     330,   318,   331,   332,   333,   334,     0,   335,   336,   337,
       0,     0,   338,   339,   340,     0,     0,   319,     0,   320,
     321,   322,     0,   323,   324,   325,     0,   326,   327,   328,
     329,   330,     0,   331,   332,   333,   334,     0,   335,   336,
     337,     0,   316,   338,   339,   340,   317,   313,     0,     0,
       0,     0,     0,     0,   314,   882,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   318,     0,     0,     0,
       0,     0,     0,   313,   899,     0,     0,     0,     0,     0,
     314,     0,   319,     0,   320,   321,   322,     0,   323,   324,
     325,   315,   326,   327,   328,   329,   330,     0,   331,   332,
     333,   334,   316,   335,   336,   337,   317,     0,   338,   339,
     340,     0,     0,     0,     0,   885,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   318,     0,   316,     0,
       0,     0,   317,   313,     0,     0,     0,     0,     0,     0,
     314,     0,   319,     0,   320,   321,   322,     0,   323,   324,
     325,   315,   326,   327,   328,   329,   330,     0,   331,   332,
     333,   334,   318,   335,   336,   337,     0,     0,   338,   339,
     340,     0,     0,     0,     0,     0,     0,     0,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,   316,   335,
     336,   337,   317,   313,   338,   339,   340,     0,     0,     0,
     314,   980,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   318,     0,     0,     0,     0,     0,     0,   313,
       0,     0,     0,     0,     0,     0,   314,     0,   319,     0,
     320,   321,   322,     0,   323,   324,   325,   315,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,   316,   335,
     336,   337,   317,     0,   338,   339,   340,     0,     0,     0,
       0,   981,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   318,     0,   316,     0,     0,     0,   317,   313,
       0,     0,     0,     0,     0,     0,   314,   982,   319,     0,
     320,   321,   322,     0,   323,   324,   325,   315,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,   318,   335,
     336,   337,     0,     0,   338,   339,   340,     0,     0,     0,
       0,     0,     0,     0,   319,     0,   320,   321,   322,     0,
     323,   324,   325,     0,   326,   327,   328,   329,   330,     0,
     331,   332,   333,   334,   316,   335,   336,   337,   317,   313,
     338,   339,   340,     0,     0,     0,   314,   983,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   318,     0,
       0,     0,     0,     0,     0,   313,     0,     0,     0,     0,
       0,     0,   314,     0,   319,     0,   320,   321,   322,     0,
     323,   324,   325,   315,   326,   327,   328,   329,   330,     0,
     331,   332,   333,   334,   316,   335,   336,   337,   317,     0,
     338,   339,   340,     0,     0,     0,     0,   984,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   318,     0,
     316,     0,     0,     0,   317,   313,     0,     0,     0,     0,
       0,     0,   314,   985,   319,     0,   320,   321,   322,     0,
     323,   324,   325,   315,   326,   327,   328,   329,   330,     0,
     331,   332,   333,   334,   318,   335,   336,   337,     0,     0,
     338,   339,   340,     0,     0,     0,  1007,     0,     0,     0,
     319,     0,   320,   321,   322,     0,   323,   324,   325,     0,
     326,   327,   328,   329,   330,     0,   331,   332,   333,   334,
     316,   335,   336,   337,   317,   313,   338,   339,   340,     0,
       0,     0,   314,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   315,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   318,     0,     0,     0,     0,     0,
       0,   313,     0,     0,     0,     0,     0,     0,   314,     0,
     319,     0,   320,   321,   322,     0,   323,   324,   325,   315,
     326,   327,   328,   329,   330,     0,   331,   332,   333,   334,
     316,   335,   336,   337,   317,     0,   338,   339,   340,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   318,     0,   668,     0,     0,     0,
     317,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     319,     0,   320,   321,   322,     0,   323,   324,   325,     0,
     326,   327,   328,   329,   330,     0,   331,   332,   333,   334,
     318,   335,   336,   337,     0,     0,   338,   339,   340,     0,
       0,     0,     0,     0,     0,     0,   319,     0,   320,   321,
     322,     0,   323,   324,   325,   315,   326,   327,   328,   329,
     330,     0,   331,   332,   333,   334,    16,   335,   336,   337,
      20,    21,   338,   339,   340,     0,     0,     0,     0,    27,
       0,    28,   865,     0,     0,     0,    33,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,     0,   317,     0,     0,     0,
      48,     0,    50,     0,    52,     0,   866,     0,     0,   867,
       0,    57,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,   646,     0,   206,     0,   318,     0,     0,     0,
       0,     0,    74,     0,     0,    76,   207,   208,    79,     0,
       0,     0,   319,   209,   320,   321,   322,     0,   323,   324,
     325,   210,   326,   327,   328,   329,     0,   211,   331,   332,
     333,   334,   212,   335,   213,   337,     0,   214,   338,   339,
     340,     0,     0,     0,   215,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   216,     0,    92,     0,     0,
       0,     0,     0,   943,   217,     0,     0,     0,     0,     0,
       0,     0,     0,   218,   219,     0,   220,     0,   221,     0,
     222,     0,     0,   223,     0,     0,     0,   224,     0,     0,
     225
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-870)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,   140,     2,    20,    15,   199,   178,    18,    54,    55,
      59,    17,   441,    24,   560,   655,    28,    28,    29,   191,
      31,    32,    33,    34,   186,   205,   725,   410,   128,    78,
       1,    42,   413,    44,    45,     1,    47,     1,    49,   593,
      51,   362,   751,   205,     1,   914,    42,    96,    59,    60,
      61,    62,    63,    64,    39,    29,     3,    68,    78,    70,
     841,    72,    73,   243,   838,   105,    77,    78,   105,    80,
      81,     1,    42,    84,    61,    29,    87,    88,    89,   179,
      91,   243,   262,    94,    95,    96,    29,    51,   418,    74,
      61,   799,   800,     0,    51,   130,    42,    29,    29,   103,
     262,    76,   296,   297,   803,   403,    76,    42,   455,    51,
     457,   441,    42,   667,    78,   102,    42,   166,    47,   665,
     160,    78,   952,   160,    60,   160,   130,    91,    42,   903,
     269,   102,     3,   110,   130,   155,   121,   150,   123,   103,
      42,    70,    89,   842,   110,  1014,   103,   928,   446,   160,
     161,   162,   163,   164,   165,   166,   130,   134,    78,    95,
     869,   103,   324,   129,   130,   129,    28,   997,   722,   156,
     105,   177,   129,   105,   105,   186,   193,   105,   358,   359,
     110,   155,   891,   200,   130,   156,   150,     3,    42,   201,
     201,   155,   158,   178,   205,   130,   358,   359,   155,   129,
     130,   155,   157,   902,   130,    19,   191,    78,   997,   194,
      86,   592,   155,    75,   160,   130,   130,    79,    89,   130,
     150,    19,    76,   155,   155,   160,   934,   935,   130,   130,
     150,   130,   243,   244,   160,   155,   130,   110,   878,   560,
     251,   252,    24,   158,   229,  1034,   160,   158,    42,   130,
     150,   262,   951,   130,   434,   156,   156,   130,   160,   158,
      74,   134,   247,   125,   158,   127,   128,   129,    62,   131,
     132,   133,   434,   150,   136,   156,    74,   823,    72,    73,
     291,   143,   144,    65,    98,   158,   148,   387,   288,   151,
     152,   153,   129,   157,    76,   132,   157,     7,   159,   136,
      98,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,    95,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     157,   714,    28,   130,   130,   716,   347,   348,   157,   130,
     389,   130,   353,   130,   665,   356,   357,   358,   359,   360,
     361,   362,   130,   155,   130,   130,    24,    31,    32,   150,
     371,   158,   158,   150,    71,   376,    51,   378,   751,   158,
     564,   565,   150,    51,   150,   150,   150,  1012,   389,    75,
     391,   392,   176,    79,   401,    75,   397,   398,   550,    79,
     150,   402,   403,    78,   405,  1030,   407,    65,   130,   150,
      78,   386,   196,   197,   198,   199,    91,   140,    76,   420,
     204,   347,   348,    91,   150,    24,   427,   353,   103,   848,
     356,   357,    96,   434,   473,   103,   158,    95,   413,   157,
     130,   159,   150,   129,   445,   446,   132,   110,   632,   633,
     136,   452,   236,   130,   455,   239,   457,    72,    73,   621,
     461,   462,   463,    24,   465,   466,    65,   130,   158,   150,
      24,   134,   473,   150,   258,   150,   150,    76,   110,   150,
     155,   471,   150,   157,   157,   159,   159,   155,    42,   490,
     491,    45,   157,   150,   278,   158,    95,   472,   150,   156,
     475,   874,   101,   135,    65,   137,   138,    42,    62,   141,
     142,    65,   823,    67,   132,    76,    51,   155,   891,   150,
      75,   841,    76,    28,    79,   104,   655,   157,   848,   455,
     157,   457,   157,   534,   535,   536,   537,   538,   539,   540,
     101,    95,   150,    78,   110,   541,   542,   548,   150,   550,
       7,    51,     3,   554,    55,   556,   129,   155,   150,   560,
     110,   562,     2,   602,   158,   130,   150,    42,   103,   157,
      75,    11,    31,    42,    79,   110,   130,   158,    78,    30,
     158,   130,    51,    23,   110,    25,   158,   158,   372,    18,
     130,    91,   160,   594,   150,   130,   597,   598,   599,   600,
     601,   602,    53,   103,   158,   606,   150,   160,   928,    78,
     155,   150,   396,   588,   150,   150,   400,   592,   150,   160,
     155,   158,   548,   158,   129,   150,   131,   132,    68,   150,
     150,   136,   952,   130,   103,    31,   158,   156,    89,   144,
     158,   110,   617,   150,    95,   130,   621,   967,   153,   110,
     150,    42,   316,   317,    94,   155,   150,   156,    98,    76,
      51,   130,    23,    24,   665,    29,    29,   668,   669,   670,
     160,   155,   673,    34,    35,    31,   129,   997,    76,   160,
     606,   150,   129,    44,   160,   156,   155,    78,   689,   158,
     691,   132,  1012,   732,   160,   696,   360,   361,   699,     2,
     132,   702,    63,   704,    65,   157,   707,   371,    11,   132,
    1030,   132,   103,   132,  1034,    76,   156,   132,   132,   110,
      23,   132,    25,   132,   132,   132,    31,   158,   392,   156,
      95,   732,   158,    91,   398,   160,   130,   446,   713,   130,
     101,   716,   302,   744,   445,   746,    22,   270,   749,   878,
     192,   736,   753,   552,   816,   754,   878,   874,  1014,   150,
     585,   736,   714,   951,   155,    68,   757,   158,   976,    72,
      73,  1007,  1000,    33,   558,   559,   421,   550,    -1,   754,
     564,   565,    -1,    -1,    -1,    -1,   226,    -1,    -1,    -1,
      -1,    94,   576,   233,   795,    98,    -1,   461,    24,   774,
      -1,    -1,    23,    24,    -1,    -1,   807,   808,    -1,   810,
     811,   812,   813,    34,    35,    -1,    42,   818,   819,    45,
     821,   186,   823,    44,   825,   826,    47,    -1,   829,    -1,
     866,   867,    -1,   617,    -1,    -1,    62,    -1,    -1,    65,
     205,    67,    63,    -1,    65,    -1,   847,   848,   288,    70,
      76,    -1,   853,   156,   855,    76,    -1,    -1,   859,    -1,
      -1,    -1,   302,    -1,   865,   904,    -1,    -1,    -1,    95,
      -1,   535,    -1,   537,    -1,    -1,    -1,    -1,   243,    -1,
     101,   882,   883,   884,   885,   886,   887,   888,   889,    -1,
      -1,    -1,    -1,    -1,   678,   679,    -1,   262,   899,    -1,
      -1,    -1,   686,   904,   688,    -1,    -1,    -1,   909,    -1,
      -1,   695,   913,    -1,   698,    24,   958,   701,    -1,    -1,
      -1,    -1,    -1,   226,    -1,    -1,    -1,    -1,    -1,   369,
     233,    -1,   971,    42,    -1,    -1,    45,    -1,    -1,    -1,
      28,    -1,    -1,   944,   945,   946,   947,    -1,    -1,    -1,
      -1,   391,    -1,    62,    -1,   739,    65,   397,    67,  1001,
    1002,  1003,  1004,  1005,  1006,   405,   967,    76,   969,   970,
     971,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   980,
     981,   982,   983,   984,   985,   288,    95,    75,     2,    -1,
      -1,    79,    -1,   358,   359,   996,   997,    11,   999,   302,
      28,    -1,    -1,    -1,   668,   669,    -1,    -1,    -1,    23,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   459,
      -1,    -1,    -1,   463,    -1,    -1,    -1,    -1,    -1,  1030,
      -1,   471,    -1,  1034,    -1,    -1,    -1,   125,    -1,   127,
     128,   129,    -1,   131,   132,    -1,    -1,    75,   136,    28,
      -1,    79,    -1,    -1,    68,   143,   144,    -1,    -1,    -1,
     148,    -1,   427,   151,   152,   153,   369,   851,    -1,   434,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    98,    -1,    -1,   452,   391,    -1,
     455,   531,   457,    -1,   397,    -1,    75,   462,    -1,    -1,
      79,   129,   405,   131,   132,    -1,    -1,    -1,   136,   893,
     894,    -1,    -1,   897,   898,    -1,   144,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,    -1,    -1,    -1,    -1,
     914,    -1,    -1,    -1,    -1,   575,   920,    -1,   922,    -1,
     924,    -1,   156,    -1,    -1,    -1,   125,    -1,   127,   128,
     129,    -1,   131,   132,   133,    -1,   459,   136,   137,   138,
     463,    -1,   141,   142,   143,   144,    -1,    -1,   471,   148,
       1,    -1,   151,   152,   153,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,   548,    -1,   550,    -1,    -1,    19,    -1,
      -1,   631,    23,    24,    -1,    -1,    -1,   637,    -1,    -1,
      -1,    32,    -1,    34,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,   226,    44,    -1,   655,    47,    -1,    -1,   233,
      -1,    -1,    -1,    75,    -1,    -1,    -1,    79,   668,   669,
    1014,    -1,    63,    -1,    65,    -1,    67,    -1,    69,    70,
      -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,   689,
      -1,   691,    -1,    -1,    -1,    -1,   696,    -1,    -1,   699,
      -1,    -1,    -1,    -1,    95,    -1,   706,    98,    -1,   709,
     101,    -1,   575,   125,   288,   127,   128,   129,    -1,   131,
     132,    -1,    -1,     3,   136,    -1,    -1,    -1,   302,    -1,
      -1,   731,   144,    -1,    -1,    15,    16,    -1,    -1,   151,
     152,   153,    22,    -1,   744,    -1,   746,    -1,    -1,     2,
      30,    -1,    -1,    -1,    -1,    -1,    36,    -1,    11,   150,
      -1,    41,    -1,    43,    -1,   156,    46,    -1,   631,    -1,
      23,    -1,    25,    53,   637,    -1,    -1,    -1,     2,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    73,    -1,   369,    -1,    -1,    -1,    23,
      -1,    25,    82,    83,    -1,    85,    -1,    87,    -1,    89,
      28,    -1,    92,    -1,    -1,    68,    96,   391,   818,    99,
      -1,    -1,    -1,   397,    -1,   825,   689,    -1,   691,    -1,
      -1,   405,    -1,   696,    -1,    -1,   699,   837,    -1,    -1,
      -1,    94,    -1,   706,    68,    98,   709,    -1,    -1,   849,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,   858,    -1,
      -1,    79,   862,    -1,    -1,    -1,    -1,    -1,   731,    -1,
      94,    -1,    -1,    -1,    98,   155,    -1,   157,    -1,    -1,
      -1,   744,    -1,   746,    -1,   459,    -1,    -1,    -1,   463,
      28,   109,    -1,    -1,    -1,    -1,    -1,   471,    -1,    -1,
     900,    -1,    -1,   156,    -1,    -1,    -1,   125,    -1,   127,
     128,   129,    -1,   131,   132,   133,   841,   135,   136,   137,
     138,    -1,    -1,   141,   142,   143,   144,    -1,   146,   147,
     148,    -1,   156,   151,   152,   153,    -1,    75,    -1,    -1,
      -1,    79,    -1,    -1,   944,   945,   946,   947,    -1,    -1,
      -1,    -1,    -1,    -1,     2,   818,    -1,    -1,    -1,    -1,
      -1,    -1,   825,    11,    -1,    -1,    -1,    -1,    -1,   969,
     970,    -1,    28,   226,   837,    23,    -1,    25,    -1,    -1,
     233,    -1,    -1,    -1,   909,    -1,   849,   125,    -1,    -1,
     128,   129,    -1,   131,   132,   858,    -1,    -1,   136,   862,
      -1,   575,   226,    -1,    -1,    -1,   144,    -1,    -1,   233,
      -1,    -1,    -1,   151,   152,   153,    -1,     3,    -1,    75,
      68,    -1,    -1,    79,    -1,    -1,    -1,   952,    -1,    15,
      -1,    -1,    -1,    -1,    -1,   288,    22,   900,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,   302,
      98,    -1,    -1,    -1,    -1,    41,    -1,   631,    -1,    -1,
      46,    -1,    -1,   637,   288,    -1,    -1,    53,    -1,   125,
      -1,    -1,   997,   129,    -1,   131,   132,    -1,   302,    -1,
     136,   944,   945,   946,   947,    -1,    -1,    73,   144,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,    -1,    85,
      -1,    -1,    -1,    89,    -1,    -1,   969,   970,   156,  1034,
      96,    -1,    -1,    -1,    -1,   689,   369,   691,    -1,    -1,
      -1,    -1,   696,    -1,    62,   699,    -1,    -1,    -1,    -1,
      -1,    -1,   706,    -1,    -1,   709,    -1,    -1,   391,    -1,
      -1,    -1,    -1,    -1,   397,   369,    -1,    -1,    -1,    -1,
      -1,    -1,   405,    -1,    -1,    -1,    -1,   731,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   391,    -1,    -1,
     744,    -1,   746,   397,    -1,    -1,    -1,    -1,   226,    -1,
      -1,   405,    -1,    -1,    -1,   233,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   459,    -1,    -1,    -1,
     463,    -1,    -1,     2,    -1,    -1,    -1,    -1,   471,    -1,
      -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,   459,    25,    19,    -1,   463,
     288,    23,    24,    -1,   818,    -1,    -1,   471,    -1,    -1,
      32,   825,    34,    35,   302,    -1,    -1,    39,   196,   197,
     198,   199,    44,   837,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   849,    -1,    -1,    -1,    68,
      -1,    63,    -1,    65,   858,    67,    -1,    69,   862,    -1,
      72,    -1,    74,    -1,    76,    -1,    -1,    -1,   236,    -1,
      -1,   239,    -1,    -1,    -1,    94,    -1,    -1,    -1,    98,
      -1,    -1,    -1,    95,    -1,    -1,    98,    -1,    -1,   101,
     258,   369,   575,    -1,    -1,    -1,   900,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     278,    -1,    -1,   391,    -1,    -1,    -1,    -1,    -1,   397,
      -1,   575,    -1,    -1,    -1,    -1,    -1,   405,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   150,    -1,
     944,   945,   946,   947,   156,    -1,    -1,    -1,   631,    -1,
      -1,    -1,    -1,    -1,   637,    -1,     1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   969,   970,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,   631,    23,    24,
      -1,   459,    -1,   637,    -1,   463,    -1,    32,    -1,    34,
      35,    -1,    -1,   471,    39,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,   372,    -1,   689,   226,   691,    -1,
      -1,    -1,    -1,   696,   233,    -1,   699,    -1,    63,    -1,
      65,    -1,    67,   706,    69,    70,   709,    72,   396,    74,
      -1,    76,   400,    -1,    -1,   689,    -1,   691,    -1,    -1,
      -1,    -1,   696,    -1,    -1,   699,    -1,    -1,   731,     2,
      95,    -1,   706,    98,    -1,   709,   101,    -1,    11,    -1,
      -1,   744,    -1,   746,    -1,     2,    -1,    -1,    -1,   288,
      23,    -1,    25,    -1,    11,    -1,    -1,   731,    -1,    -1,
      -1,    -1,    -1,   302,    -1,    -1,    23,    -1,    25,    -1,
     744,    -1,   746,    -1,    -1,    -1,    -1,   575,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,    -1,    68,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    68,    -1,    -1,    -1,   818,    -1,    -1,    -1,    -1,
      -1,    94,   825,    -1,    -1,    98,   140,    -1,    -1,    -1,
     369,    -1,    -1,   631,   837,    -1,    -1,    94,    -1,   637,
     154,    98,    -1,   157,   818,   159,   849,    -1,    -1,    -1,
      -1,   825,   391,    -1,    -1,   858,    -1,    -1,   397,   862,
      -1,    -1,    -1,   837,    -1,    -1,   405,    -1,    -1,    -1,
     558,   559,    -1,    -1,    -1,   849,   564,   565,    -1,    -1,
      -1,    -1,    -1,   156,   858,    -1,    -1,    -1,   862,    -1,
      -1,   689,    -1,   691,    -1,    -1,    -1,   900,   696,   156,
      -1,   699,    -1,    -1,    -1,    -1,    -1,    -1,   706,    -1,
      -1,   709,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     459,    -1,    -1,    -1,   463,    -1,   900,    -1,    -1,    -1,
      -1,    -1,   471,   731,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   944,   945,   946,   947,    -1,   744,    -1,   746,    -1,
      -1,    -1,    -1,   226,    -1,    -1,    -1,    -1,    28,    -1,
     233,    -1,    -1,    -1,    -1,    -1,   969,   970,    -1,   226,
     944,   945,   946,   947,    -1,    -1,   233,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     678,   679,    -1,    -1,    -1,   969,   970,    -1,   686,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    79,
     698,    -1,    -1,   701,    -1,   288,    -1,    -1,    -1,    -1,
     818,    -1,    -1,    -1,    -1,    -1,    -1,   825,    -1,   302,
      -1,   288,    -1,    -1,    -1,    -1,   575,    -1,    -1,   837,
      -1,    -1,    -1,    -1,    -1,   302,    -1,    -1,    -1,    -1,
      -1,   849,    -1,    -1,    -1,   125,    -1,   127,   128,   129,
     858,   131,   132,   133,   862,   135,   136,   137,   138,    -1,
      -1,   141,   142,   143,   144,    -1,   146,    -1,   148,    -1,
      -1,   151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   631,    -1,    -1,     3,   369,    -1,   637,    -1,
      -1,    -1,   900,    -1,    -1,    -1,    -1,    15,    16,    -1,
      -1,    -1,   369,    -1,    22,    -1,    -1,    -1,   391,    -1,
      -1,    -1,    30,    -1,   397,    -1,    -1,    -1,    36,    -1,
      -1,    -1,   405,    41,   391,    43,    -1,    -1,    46,    -1,
     397,    -1,    -1,    -1,    -1,    53,   944,   945,   946,   947,
     689,    -1,   691,    -1,    -1,    -1,    64,   696,    -1,    -1,
     699,    -1,    -1,   851,    -1,    73,    -1,   706,    -1,    -1,
     709,   969,   970,    -1,    82,    83,    -1,    85,    -1,    87,
      -1,    89,     3,    -1,    92,    -1,   459,    -1,    96,    97,
     463,    99,   731,    -1,    15,    16,    -1,    -1,   471,    -1,
      -1,    22,   459,    -1,    -1,   744,   463,   746,    -1,    30,
      -1,    -1,    10,    -1,   471,    36,    -1,    -1,    -1,    17,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    -1,
      28,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    -1,    85,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    96,    -1,    75,    99,   818,
      -1,    79,    -1,    -1,    -1,    -1,   825,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   837,    -1,
      -1,    -1,   575,    -1,    -1,    -1,    -1,   105,    -1,    -1,
     849,   109,    -1,    -1,    -1,    -1,    -1,    -1,   575,   858,
      -1,    -1,    -1,   862,    -1,    -1,    -1,   125,    -1,   127,
     128,   129,    -1,   131,   132,   133,   157,   135,   136,   137,
     138,   139,    -1,   141,   142,   143,   144,    -1,   146,   147,
     148,    -1,    -1,   151,   152,   153,    -1,    -1,   631,    -1,
      -1,   900,   160,    -1,   637,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   631,    -1,    -1,    -1,    -1,    -1,
     637,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   944,   945,   946,   947,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   689,    -1,   691,    -1,
      -1,    -1,    -1,   696,    -1,    -1,   699,    -1,    -1,    -1,
     969,   970,   689,   706,   691,    -1,   709,    -1,    -1,   696,
      -1,    -1,   699,    -1,    -1,    -1,    -1,    -1,    -1,   706,
      -1,    -1,   709,    -1,    -1,    -1,    -1,    -1,   731,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   744,    -1,   746,   731,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,   744,    12,   746,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    -1,    53,
      54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    66,    -1,    68,   818,    -1,    -1,    -1,    73,
      -1,    -1,   825,    -1,    -1,    -1,    -1,    -1,    82,    83,
      84,    85,    86,    87,   837,    89,    -1,    -1,    92,    -1,
      -1,    -1,    96,    -1,    -1,    99,   849,    -1,    -1,    -1,
     837,    -1,    -1,    -1,    -1,   858,   110,    -1,    -1,   862,
      -1,    -1,   849,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   858,   126,    -1,    -1,   862,   130,    -1,    -1,    -1,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,   149,    -1,   900,    -1,    -1,
      -1,   155,    -1,   157,   158,   159,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   900,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   944,   945,   946,   947,    -1,    -1,    -1,    -1,     0,
       1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,   969,   970,    19,    20,
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
      -1,    -1,   153,    -1,   155,    -1,   157,     1,   159,     3,
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
      -1,   155,    -1,   157,    -1,   159,   160,     1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,    12,    13,
      14,    15,    16,    17,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    48,    49,    50,    -1,    52,    53,
      54,    55,    56,    57,    -1,    59,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    -1,    92,    93,
      94,    95,    96,    -1,    98,    99,   100,   101,    -1,    -1,
     104,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
      -1,   135,   136,   137,   138,   139,    -1,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
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
     147,   148,   149,   150,    -1,    -1,   153,    -1,   155,   156,
     157,     1,   159,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    48,    49,
      50,    -1,    52,    53,    54,    55,    56,    57,    -1,    59,
      -1,    -1,    -1,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    -1,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    89,
      90,    -1,    92,    93,    94,    95,    96,    -1,    98,    99,
     100,   101,    -1,    -1,   104,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,   145,    -1,   147,   148,    -1,
     150,    -1,    -1,   153,    -1,   155,   156,   157,     1,   159,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    48,    49,    50,    -1,    52,
      53,    54,    55,    56,    57,    -1,    59,    -1,    -1,    -1,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    76,    77,    78,    -1,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    -1,    92,
      93,    94,    95,    96,    -1,    98,    99,   100,   101,    -1,
      -1,   104,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,   148,    -1,   150,    -1,    -1,
     153,    -1,   155,   156,   157,     1,   159,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
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
     156,   157,     1,   159,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    48,
      49,    50,    -1,    52,    53,    54,    55,    56,    57,    -1,
      59,    -1,    -1,    -1,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      -1,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    -1,    92,    93,    94,    95,    96,    -1,    98,
      99,   100,   101,    -1,    -1,   104,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,   148,
      -1,   150,    -1,    -1,   153,    -1,   155,   156,   157,     1,
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
     155,    -1,   157,    -1,   159,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
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
      -1,   159,   160,     3,     4,     5,     6,     7,     8,    -1,
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
     160,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      -1,    53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    66,    -1,    68,    -1,    -1,    -1,
      -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    83,    84,    85,    86,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    96,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,   145,    -1,   147,   148,    -1,    -1,    -1,
      -1,   153,    -1,   155,    -1,   157,    -1,   159,   160,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    35,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    -1,    53,
      54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    65,    66,    -1,    68,    -1,    -1,    -1,    -1,    73,
      -1,    -1,    76,    -1,    -1,     1,    -1,     3,    82,    83,
      84,    85,    86,    87,    10,    89,    -1,    -1,    92,    15,
      16,    17,    96,    -1,    -1,    99,    22,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    30,   109,    -1,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,   125,   126,   127,   128,    -1,    -1,    53,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,   143,
      -1,   145,    -1,   147,   148,    -1,    -1,    73,    -1,   153,
      -1,   155,    -1,   157,    -1,   159,    82,    83,    -1,    85,
      -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,    -1,
      96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,   115,
      -1,    -1,   118,   119,   120,   121,    -1,   123,   124,   125,
     126,   127,   128,    -1,    -1,   131,    -1,    -1,    -1,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,   148,    -1,    -1,   151,   152,   153,   154,    -1,
      -1,   157,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    19,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    -1,    53,    54,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    66,    -1,    68,    -1,    -1,
      -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    84,    85,    86,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    95,    96,    -1,    98,    99,    -1,
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
      87,    -1,    89,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,   145,    -1,
     147,   148,   149,    -1,    -1,    -1,   153,    -1,   155,    -1,
     157,    -1,   159,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      50,    -1,    -1,    53,    54,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    66,    -1,    68,    -1,
      -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    84,    85,    86,    87,    -1,    89,
      -1,    -1,    92,    93,    94,    -1,    96,    97,    -1,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,   145,    -1,   147,   148,   149,
      -1,    -1,    -1,   153,    -1,   155,    -1,   157,   158,   159,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    -1,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    66,    -1,    68,    -1,    -1,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    -1,    89,    -1,    -1,    92,
      93,    94,    -1,    96,    97,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,   148,   149,    -1,    -1,    -1,
     153,    -1,   155,    -1,   157,   158,   159,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    42,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    -1,    53,    54,    -1,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      66,    -1,    68,    -1,    -1,    -1,    -1,    73,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,
      86,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,    -1,
      96,    -1,    -1,    99,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,   145,
      -1,   147,   148,    -1,    -1,    -1,    -1,   153,    -1,   155,
      -1,   157,    -1,   159,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      19,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    -1,    53,    54,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    66,    -1,    68,
      -1,    -1,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,    98,
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
      85,    86,    87,    -1,    89,    -1,    -1,    92,    93,    94,
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
     145,    -1,   147,   148,    -1,   150,    -1,    -1,   153,    -1,
     155,    -1,   157,    -1,   159,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    50,    -1,    -1,    53,    54,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    66,    -1,
      68,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,    87,
      -1,    89,    -1,    -1,    92,    93,    94,    -1,    96,    -1,
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
      -1,    -1,    53,    54,    -1,    56,    57,    58,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    66,    -1,    68,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    84,    85,    86,    87,    -1,    89,    -1,
      -1,    92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   143,    -1,   145,    12,   147,   148,    15,    16,
      -1,    -1,   153,    -1,   155,    22,   157,    -1,   159,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    -1,    53,    54,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,
      -1,    68,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,
      87,    -1,    89,    -1,    -1,    92,    -1,    -1,    -1,    96,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   107,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,   148,   149,    -1,    -1,    -1,
     153,    -1,   155,    -1,   157,    -1,   159,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
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
      -1,     3,     4,     5,     6,     7,     8,   143,    -1,   145,
      12,   147,   148,    15,    16,    -1,    -1,   153,    -1,   155,
      22,   157,    -1,   159,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      -1,    53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    66,    -1,    68,    -1,    -1,    -1,
      -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    83,    84,    85,    86,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    96,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,   145,    -1,   147,   148,   149,    -1,    -1,
      -1,   153,    -1,   155,   156,   157,    -1,   159,     3,     4,
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
      -1,    -1,   107,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   143,    -1,
     145,    12,   147,   148,    15,    16,    -1,    -1,   153,    -1,
     155,    22,   157,    -1,   159,    -1,    -1,    -1,    29,    30,
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
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,   145,    -1,
     147,   148,   149,    -1,    -1,    -1,   153,    -1,   155,    -1,
     157,    -1,   159,     3,     4,     5,     6,     7,     8,    -1,
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
     153,    -1,   155,    -1,   157,    -1,   159,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    -1,    53,    54,    -1,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      66,    -1,    68,    -1,    -1,    -1,    -1,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,
      86,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,    -1,
      96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,   145,
      -1,   147,   148,   149,    -1,    -1,    -1,   153,    -1,   155,
      -1,   157,    -1,   159,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
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
      15,    16,    -1,    -1,   153,    -1,    -1,    22,   157,    -1,
     159,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    50,    -1,    -1,    53,    54,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    66,    -1,    68,    -1,    -1,    -1,    -1,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,
      85,    86,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   143,    -1,
     145,    12,    -1,   148,    15,    16,    -1,    -1,   153,    -1,
     155,    22,   157,    -1,   159,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    66,    -1,    68,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    82,    83,    84,    85,    86,    87,    -1,    89,    -1,
      91,    92,    -1,    -1,    -1,    96,    -1,    -1,    99,    -1,
      -1,    -1,   103,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,   145,    22,    -1,   154,    -1,   150,
     157,    -1,   159,    30,   155,    -1,   157,    -1,   159,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    -1,    53,    54,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    66,
      -1,    68,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,
      87,    -1,    89,    -1,    -1,    92,    -1,    -1,    -1,    96,
      -1,    -1,    99,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,   145,    49,
      50,    -1,    -1,    53,    54,    -1,    56,    57,   155,    -1,
     157,    -1,   159,    -1,    64,    -1,    66,    -1,    68,    -1,
      -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    84,    85,    86,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    96,    -1,    -1,    99,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,   145,    49,    50,    -1,    -1,
      53,    54,    -1,    56,    57,   155,    -1,   157,    -1,   159,
      -1,    64,    -1,    66,    -1,    68,    -1,    -1,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    82,
      83,    84,    85,    86,    87,    -1,    89,    -1,    -1,    92,
      15,    16,    -1,    96,    -1,    -1,    99,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,   126,    -1,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    73,    -1,
      -1,    -1,    -1,    -1,   157,    -1,   159,    82,    83,    -1,
      85,    -1,    87,     3,    89,    -1,    -1,    92,    -1,    -1,
      10,    96,    -1,    -1,    99,    15,    16,    17,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    -1,    85,    -1,    87,    -1,    89,
      10,    -1,    92,    -1,    -1,    -1,    96,    17,    -1,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
     110,   111,   112,   113,   114,   115,    -1,    -1,   118,   119,
     120,   121,    -1,   123,   124,   125,   126,   127,   128,    -1,
      -1,   131,    -1,    -1,    -1,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,   148,    -1,
      -1,   151,   152,   153,   154,    75,    -1,    -1,    -1,    79,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    28,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    75,   146,   147,   148,    79,
      -1,   151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,   109,
      -1,    75,    -1,    -1,    -1,    79,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    -1,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,   109,   146,   147,   148,    -1,
      -1,   151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,
     160,   125,    -1,   127,   128,   129,    75,   131,   132,   133,
      79,   135,   136,   137,   138,   139,    -1,   141,   142,   143,
     144,    -1,   146,   147,   148,    10,    -1,   151,   152,   153,
      -1,    -1,    17,    -1,    -1,    -1,   160,    -1,    -1,    -1,
     109,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,   127,   128,
     129,    -1,   131,   132,   133,    -1,   135,   136,   137,   138,
     139,    -1,   141,   142,   143,   144,    -1,   146,   147,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,    -1,    -1,   158,
      75,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,   127,   128,   129,    -1,   131,   132,   133,    -1,
     135,   136,   137,   138,   139,    -1,   141,   142,   143,   144,
      -1,   146,   147,   148,    -1,    75,   151,   152,   153,    79,
      10,    -1,    -1,   158,    -1,    -1,    -1,    17,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,   118,   119,
     120,   121,    -1,   123,   124,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    -1,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    75,   146,   147,   148,    79,
      10,   151,   152,   153,    -1,   155,    -1,    17,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,   118,   119,
     120,   121,    -1,   123,   124,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    -1,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    75,   146,   147,   148,    79,
      10,   151,   152,   153,    -1,   155,    -1,    17,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,   118,   119,
     120,   121,    -1,   123,   124,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    -1,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    75,   146,   147,   148,    79,
      10,   151,   152,   153,    -1,   155,    -1,    17,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,   118,   119,
     120,   121,    -1,   123,   124,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    -1,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    75,   146,   147,   148,    79,
      10,   151,   152,   153,    -1,   155,    -1,    17,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,   118,   119,
     120,   121,    -1,   123,   124,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    -1,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    75,   146,   147,   148,    79,
      10,   151,   152,   153,    -1,   155,    -1,    17,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,   118,   119,
     120,   121,    -1,   123,   124,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    -1,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    75,   146,   147,   148,    79,
      10,   151,   152,   153,    -1,   155,    -1,    17,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,   118,   119,
     120,   121,    -1,   123,   124,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    -1,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    75,   146,   147,   148,    79,
      10,   151,   152,   153,    -1,   155,    -1,    17,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,   118,   119,
     120,   121,    -1,   123,   124,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,    -1,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    75,   146,   147,   148,    79,
      -1,   151,   152,   153,    -1,   155,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,   115,    28,    29,   118,   119,
     120,   121,    -1,   123,   124,   125,    -1,   127,   128,   129,
      42,   131,   132,   133,    -1,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,   144,    -1,   146,   147,   148,    -1,
      -1,   151,   152,   153,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,    79,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,
      -1,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,   127,   128,   129,    -1,   131,
     132,   133,    -1,   135,   136,   137,   138,   139,    -1,   141,
     142,   143,   144,    75,   146,   147,   148,    79,    10,   151,
     152,   153,    -1,   155,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,   127,   128,   129,    -1,   131,
     132,   133,    -1,   135,   136,   137,   138,   139,    -1,   141,
     142,   143,   144,    75,   146,   147,   148,    79,    10,   151,
     152,   153,    -1,   155,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,
      -1,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,   127,   128,   129,    -1,   131,
     132,   133,    -1,   135,   136,   137,   138,   139,    -1,   141,
     142,   143,   144,    75,   146,   147,   148,    79,    10,   151,
     152,   153,    -1,   155,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,   125,    -1,   127,   128,   129,    -1,   131,
     132,   133,    28,   135,   136,   137,   138,   139,    -1,   141,
     142,   143,   144,    75,   146,   147,   148,    79,    -1,   151,
     152,   153,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,   109,    -1,    75,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    88,   125,    -1,   127,   128,   129,    -1,   131,
     132,   133,    -1,   135,   136,   137,   138,   139,    -1,   141,
     142,   143,   144,   109,   146,   147,   148,    -1,    -1,   151,
     152,   153,    -1,   155,    -1,    -1,    -1,    -1,    -1,   125,
      -1,   127,   128,   129,    75,   131,   132,   133,    79,   135,
     136,   137,   138,   139,    -1,   141,   142,   143,   144,    -1,
     146,   147,   148,    -1,    10,   151,   152,   153,    -1,   155,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,    -1,
     131,   132,   133,    -1,   135,   136,   137,   138,   139,    -1,
     141,   142,   143,   144,    60,   146,   147,   148,    -1,    10,
     151,   152,   153,    -1,   155,    -1,    17,    -1,    -1,    75,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,   127,   128,   129,    75,   131,   132,   133,    79,   135,
     136,   137,   138,   139,    -1,   141,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,    10,    -1,
      -1,    -1,    -1,    -1,   105,    17,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    33,    -1,    -1,   125,    -1,   127,   128,   129,    -1,
     131,   132,   133,    -1,   135,   136,   137,   138,   139,    -1,
     141,   142,   143,   144,    -1,   146,   147,   148,    60,    -1,
     151,   152,   153,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    75,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,   127,   128,   129,    75,   131,
     132,   133,    79,   135,   136,   137,   138,   139,    -1,   141,
     142,   143,   144,    -1,   146,   147,   148,    10,    -1,   151,
     152,   153,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    42,
     127,   128,   129,    -1,   131,   132,   133,    -1,   135,   136,
     137,   138,   139,    -1,   141,   142,   143,   144,    -1,   146,
     147,   148,    10,    -1,   151,   152,   153,    -1,    -1,    17,
      -1,    -1,    75,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,   127,   128,   129,    75,   131,   132,
     133,    79,   135,   136,   137,   138,   139,    -1,   141,   142,
     143,   144,    -1,   146,   147,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      -1,   109,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,   127,
     128,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
     138,   139,   109,   141,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,    -1,    -1,   125,    -1,
     127,   128,   129,    -1,   131,   132,   133,    -1,   135,   136,
     137,   138,   139,    -1,   141,   142,   143,   144,    -1,   146,
     147,   148,    10,   150,   151,   152,   153,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    10,    -1,    -1,    23,    24,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    32,    -1,    34,    35,    -1,
      -1,    28,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,
      67,    -1,    69,    -1,    -1,    72,    -1,    74,    10,    76,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    75,    -1,
      -1,   109,    79,    -1,    -1,    -1,    28,    -1,    95,    -1,
      -1,    98,    -1,    -1,   101,    -1,    -1,   125,    -1,   127,
     128,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
     138,   139,   109,   141,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,    -1,    -1,   125,    -1,
     127,   128,   129,    75,   131,   132,   133,    79,   135,   136,
     137,   138,   139,   150,   141,   142,   143,   144,    -1,   146,
     147,   148,    10,   150,   151,   152,   153,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,   108,   109,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,   127,   128,   129,    -1,   131,
     132,   133,    -1,   135,   136,   137,   138,   139,    -1,   141,
     142,   143,   144,    -1,   146,   147,   148,    10,    -1,   151,
     152,   153,    -1,    -1,    17,    -1,    -1,    75,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,   109,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,   125,    -1,   127,
     128,   129,    75,   131,   132,   133,    79,   135,   136,   137,
     138,   139,    -1,   141,   142,   143,   144,    -1,   146,   147,
     148,    -1,    10,   151,   152,   153,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      28,    -1,    75,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,   125,    -1,   127,   128,   129,    -1,   131,   132,
     133,    -1,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   105,   146,   147,   148,   109,    -1,   151,   152,
     153,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    79,   125,    -1,   127,   128,   129,    -1,   131,   132,
     133,    -1,   135,   136,   137,   138,   139,    -1,   141,   142,
     143,   144,    -1,   146,   147,   148,    10,    11,   151,   152,
     153,   109,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,   125,    -1,   127,
     128,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
     138,   139,    -1,   141,   142,   143,   144,    -1,   146,   147,
     148,    10,   150,   151,   152,   153,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    75,    31,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      79,   125,    -1,   127,   128,   129,    -1,   131,   132,   133,
      -1,   135,   136,   137,   138,   139,    -1,   141,   142,   143,
     144,    -1,   146,   147,   148,    -1,    -1,   151,   152,   153,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    -1,   125,    -1,   127,   128,
     129,    -1,   131,   132,   133,    -1,   135,   136,   137,   138,
     139,    -1,   141,   142,   143,   144,    -1,   146,   147,   148,
     108,   109,   151,   152,   153,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,   127,
     128,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
     138,   139,    -1,   141,   142,   143,   144,     3,   146,   147,
     148,    10,    -1,   151,   152,   153,    -1,    -1,    17,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    28,
      29,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    -1,    10,    -1,    -1,    41,    -1,    43,    -1,    17,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    73,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    82,    83,    -1,    85,
      -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,    -1,
      96,    -1,    -1,    99,    -1,    -1,    -1,    75,    -1,    -1,
     109,    79,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,   125,    -1,   127,   128,
     129,    -1,   131,   132,   133,    28,   135,   136,   137,   138,
     139,   109,   141,   142,   143,   144,    -1,   146,   147,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,   125,    -1,   127,
     128,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
     138,   139,    -1,   141,   142,   143,   144,    -1,   146,   147,
     148,    -1,    75,   151,   152,   153,    79,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    11,    -1,    -1,    -1,    -1,    -1,
      17,    -1,   125,    -1,   127,   128,   129,    -1,   131,   132,
     133,    28,   135,   136,   137,   138,   139,    -1,   141,   142,
     143,   144,    75,   146,   147,   148,    79,    -1,   151,   152,
     153,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    75,    -1,
      -1,    -1,    79,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,   125,    -1,   127,   128,   129,    -1,   131,   132,
     133,    28,   135,   136,   137,   138,   139,    -1,   141,   142,
     143,   144,   109,   146,   147,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
     127,   128,   129,    -1,   131,   132,   133,    -1,   135,   136,
     137,   138,   139,    -1,   141,   142,   143,   144,    75,   146,
     147,   148,    79,    10,   151,   152,   153,    -1,    -1,    -1,
      17,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,   125,    -1,
     127,   128,   129,    -1,   131,   132,   133,    28,   135,   136,
     137,   138,   139,    -1,   141,   142,   143,   144,    75,   146,
     147,   148,    79,    -1,   151,   152,   153,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    75,    -1,    -1,    -1,    79,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    88,   125,    -1,
     127,   128,   129,    -1,   131,   132,   133,    28,   135,   136,
     137,   138,   139,    -1,   141,   142,   143,   144,   109,   146,
     147,   148,    -1,    -1,   151,   152,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,    -1,
     131,   132,   133,    -1,   135,   136,   137,   138,   139,    -1,
     141,   142,   143,   144,    75,   146,   147,   148,    79,    10,
     151,   152,   153,    -1,    -1,    -1,    17,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,   125,    -1,   127,   128,   129,    -1,
     131,   132,   133,    28,   135,   136,   137,   138,   139,    -1,
     141,   142,   143,   144,    75,   146,   147,   148,    79,    -1,
     151,   152,   153,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      75,    -1,    -1,    -1,    79,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    88,   125,    -1,   127,   128,   129,    -1,
     131,   132,   133,    28,   135,   136,   137,   138,   139,    -1,
     141,   142,   143,   144,   109,   146,   147,   148,    -1,    -1,
     151,   152,   153,    -1,    -1,    -1,    51,    -1,    -1,    -1,
     125,    -1,   127,   128,   129,    -1,   131,   132,   133,    -1,
     135,   136,   137,   138,   139,    -1,   141,   142,   143,   144,
      75,   146,   147,   148,    79,    10,   151,   152,   153,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
     125,    -1,   127,   128,   129,    -1,   131,   132,   133,    28,
     135,   136,   137,   138,   139,    -1,   141,   142,   143,   144,
      75,   146,   147,   148,    79,    -1,   151,   152,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    75,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,   127,   128,   129,    -1,   131,   132,   133,    -1,
     135,   136,   137,   138,   139,    -1,   141,   142,   143,   144,
     109,   146,   147,   148,    -1,    -1,   151,   152,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,   127,   128,
     129,    -1,   131,   132,   133,    28,   135,   136,   137,   138,
     139,    -1,   141,   142,   143,   144,    19,   146,   147,   148,
      23,    24,   151,   152,   153,    -1,    -1,    -1,    -1,    32,
      -1,    34,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      63,    -1,    65,    -1,    67,    -1,    69,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    -1,    98,    15,    16,   101,    -1,
      -1,    -1,   125,    22,   127,   128,   129,    -1,   131,   132,
     133,    30,   135,   136,   137,   138,    -1,    36,   141,   142,
     143,   144,    41,   146,    43,   148,    -1,    46,   151,   152,
     153,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,   150,    -1,    -1,
      -1,    -1,    -1,   156,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    -1,    85,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    96,    -1,    -1,
      99
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
       9,    95,   189,   287,    65,   107,   197,   286,   197,   286,
      23,    35,   250,   286,   286,   157,     3,    15,    16,    22,
      30,    36,    41,    43,    46,    53,    64,    73,    82,    83,
      85,    87,    89,    92,    96,    99,   184,   186,   217,   157,
     184,   251,   252,    29,   174,   190,   286,   286,   286,   286,
     286,   286,     7,   157,    35,   193,   193,   149,   239,   263,
     286,    93,    94,   150,   286,   288,   286,   190,   286,   286,
     286,    86,   157,   169,   286,   286,   174,   181,   286,   289,
     174,   181,   286,   184,   247,   286,   263,   286,   286,   286,
     286,   286,   286,   286,   286,     1,   156,   167,   175,   263,
      97,   134,   239,   265,   266,   288,   197,   263,   286,   296,
      67,   169,   155,    71,   173,   184,   184,    47,    70,   234,
     251,   150,   140,    10,    17,    28,    75,    79,   109,   125,
     127,   128,   129,   131,   132,   133,   135,   136,   137,   138,
     139,   141,   142,   143,   144,   146,   147,   148,   151,   152,
     153,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   154,   226,   157,   159,
      75,    79,   157,   169,   150,   286,   286,   286,   263,   155,
     174,    42,   276,   247,   251,   150,   130,   150,   104,   185,
     239,   267,   268,   269,   288,   247,   209,   249,   184,   157,
     190,    29,    42,   190,   105,   190,   279,    29,    42,   190,
     279,   287,    33,    60,   150,    88,   174,   226,   267,   169,
     157,   228,    97,   157,   184,   253,   254,     1,   129,   258,
      42,   130,   169,   190,   190,   267,   184,   130,   150,   286,
     286,   150,   155,   190,   157,   267,   150,   201,   150,   201,
     150,   110,   248,   150,   150,    33,    60,   190,   150,   156,
     156,   167,   130,   156,   286,   130,   158,   130,   158,   160,
     279,    42,   130,   160,   279,   108,   130,   160,     7,     1,
     156,   175,    55,   129,   210,   155,   221,   150,   286,   286,
     286,   286,   197,   286,   197,   286,   286,   286,   286,   286,
      16,    99,   286,   286,    30,    53,    95,   185,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   288,   288,   288,   288,   288,   267,
     267,   197,   286,   197,   286,    24,    42,    76,   101,   278,
     281,   282,   286,   301,    29,    42,    29,    42,    88,    42,
     160,   197,   286,   190,   150,   150,   286,   286,   110,   158,
     130,   150,   184,   251,    42,   263,    40,   286,   197,   286,
     157,   190,    40,   286,   197,   286,   190,   153,   176,   178,
     286,   176,   177,   169,   286,    31,   286,   158,    24,    42,
      45,    62,    65,    76,    95,   168,   229,   230,   231,   232,
     218,   254,   130,   158,    16,    30,    43,    64,    82,    83,
      87,    99,   159,   188,   259,   271,   110,   255,   286,   252,
     158,   239,   286,     1,   206,   267,   158,    18,   202,   259,
     271,   130,   178,   177,   156,   158,   158,   265,   158,   265,
     169,   160,   197,   286,   160,   169,   286,   160,   286,   160,
     286,   156,   156,   184,   263,   155,     1,   184,   214,   215,
      24,    65,    76,    95,   223,   233,   150,   150,   150,   150,
     150,   158,   160,    42,    76,   130,   158,   270,    75,    75,
      40,   197,   286,    40,   197,   286,   197,   286,   276,   276,
     150,   239,   288,   269,   210,   150,   286,   158,   286,    29,
     190,    29,   190,   280,   281,   286,    29,   190,   279,    29,
     190,   279,   130,   150,    11,   150,    31,    31,   169,    88,
     174,    42,    76,   231,   130,   158,   157,   184,    24,    65,
      76,    95,   235,   158,   254,   258,     1,   263,    58,   288,
     156,    61,   102,   156,   207,   158,   157,   174,   184,   203,
     247,   150,   150,   169,   160,   279,   160,   279,   169,   108,
       1,   211,   156,   110,   130,   156,    76,   222,     1,     3,
      10,    15,    17,    22,    36,    41,    46,    53,    73,    85,
      89,    92,    96,   125,   126,   127,   128,   131,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     148,   151,   152,   153,   154,   157,   183,   186,   224,   225,
     226,   270,   281,   258,   270,   270,   286,    29,    29,   286,
      29,    29,   160,   160,   190,   190,   155,   190,    88,    40,
     286,    40,   286,   130,   158,    88,    40,   286,   190,    40,
     286,   190,   286,   286,   169,   286,   169,    31,   184,   230,
     254,   129,   262,    76,   258,   255,   160,    42,   160,    29,
     169,   263,   203,   129,   174,    40,   169,   286,   160,    40,
     169,   286,   160,   286,   156,    35,    69,    72,   156,   168,
     192,   286,   215,   235,   157,   227,   184,   286,   132,   227,
     227,   255,    88,    40,    40,    88,    40,    40,    40,    40,
       1,   211,   286,   286,   286,   281,   286,   286,   286,    11,
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
#line 1239 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1240 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1241 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1242 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1243 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1244 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1245 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1246 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1247 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1248 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1249 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1250 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1254 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1255 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1259 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1263 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1264 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1265 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1270 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1272 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1276 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1278 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1283 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1288 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1308 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1309 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1313 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1314 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1317 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1318 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1322 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1323 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1326 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1332 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1337 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1343 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1348 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1349 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1353 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1357 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1365 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1366 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1370 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1374 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1375 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1378 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1379 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1383 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1387 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1392 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1405 "chapel.ypp"
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

  case 339:
/* Line 1792 of yacc.c  */
#line 1421 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1423 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1425 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1429 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1430 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1431 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1432 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1438 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1444 "chapel.ypp"
    {
      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), context->latestComment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
      context->latestComment = NULL;
    }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1450 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1459 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1467 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1469 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1474 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1476 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1487 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1493 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1494 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1501 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1519 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1526 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1527 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1536 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1560 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1564 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1568 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1576 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1582 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1583 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1584 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1591 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1597 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1601 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1605 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1606 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1607 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1609 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1615 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1616 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1617 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1618 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1622 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1623 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1627 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1629 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1633 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1634 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1638 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1643 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1644 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1648 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1649 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1650 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1651 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1656 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1673 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1677 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1679 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1681 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1683 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1685 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1687 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1691 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1693 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1698 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1704 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1708 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1710 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1712 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1720 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1722 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1729 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1735 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1741 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1747 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1756 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1765 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1781 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1785 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1790 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1794 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1795 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1799 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1803 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1804 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1809 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1813 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1817 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1823 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1824 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1825 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1826 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1827 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1828 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1834 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1839 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1844 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1860 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1862 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1864 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1866 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1868 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1870 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1872 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1874 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1878 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1879 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1882 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1883 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1884 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1906 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1907 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1908 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1912 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1913 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1914 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1915 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1923 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1924 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1925 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1926 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1930 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 1931 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 523:
/* Line 1792 of yacc.c  */
#line 1936 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 1937 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 1938 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 1939 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 1940 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 1941 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 1942 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 1943 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 1944 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 1946 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 1950 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 1957 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 1958 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 1962 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 1963 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 1964 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 1965 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 1966 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 1967 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 1968 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 1969 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 1970 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 1971 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 1972 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 1973 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 1974 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 1975 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 1976 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 1977 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 1978 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 1979 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 1980 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 1981 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 1982 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 1983 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 1984 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 1988 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 1989 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 1990 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 1991 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 1992 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 1993 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 1997 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 1998 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 1999 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2000 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2004 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2005 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2006 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2007 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2012 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2013 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2014 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2015 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2016 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2017 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2018 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 9421 "bison-chapel.cpp"
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


