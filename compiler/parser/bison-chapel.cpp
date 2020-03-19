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
     TDOTDOTOPENHIGH = 425,
     TUMINUS = 426,
     TUPLUS = 427,
     TLNOT = 428
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
#line 489 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 496 "bison-chapel.cpp"
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
#line 574 "bison-chapel.cpp"

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
#define YYLAST   19031

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  174
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  146
/* YYNRULES -- Number of rules.  */
#define YYNRULES  626
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1124

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   428

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
     165,   166,   167,   168,   169,   170,   171,   172,   173
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
    1782,  1784,  1789,  1793,  1797,  1801,  1804,  1807,  1810,  1812,
    1813,  1815,  1818,  1821,  1823,  1825,  1827,  1829,  1831,  1833,
    1836,  1839,  1841,  1846,  1851,  1856,  1860,  1864,  1868,  1872,
    1878,  1882,  1887,  1891,  1896,  1898,  1900,  1902,  1904,  1906,
    1908,  1910,  1912,  1914,  1916,  1920,  1925,  1929,  1934,  1938,
    1943,  1947,  1953,  1957,  1961,  1965,  1969,  1973,  1977,  1981,
    1985,  1989,  1993,  1997,  2001,  2005,  2009,  2013,  2017,  2021,
    2025,  2029,  2033,  2037,  2041,  2045,  2048,  2051,  2054,  2057,
    2060,  2063,  2066,  2070,  2074,  2078,  2082,  2086,  2090,  2094,
    2098,  2100,  2102,  2104,  2106,  2108,  2110
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     175,     0,    -1,   176,    -1,    -1,   176,   177,    -1,   179,
      -1,   178,   179,    -1,    74,     8,    -1,   178,    74,     8,
      -1,   181,    -1,   184,    -1,   190,    -1,   191,    -1,   192,
      -1,   203,    -1,   193,    -1,   212,    -1,   215,    -1,   213,
      -1,   222,    -1,   216,    -1,   217,    -1,   221,    -1,   202,
      -1,   292,   158,    -1,    14,   179,    -1,    15,   293,   179,
      -1,    16,   194,   158,    -1,    23,   293,   184,    -1,    28,
     194,   158,    -1,    30,   280,   158,    -1,    53,   196,   179,
      -1,    57,   304,   201,    -1,    57,   201,    -1,    66,   304,
     201,    -1,    88,   304,   201,    -1,    88,   201,    -1,    94,
     179,    -1,   114,   304,   158,    -1,     1,   158,    -1,   182,
     183,    60,   196,    -1,   180,   163,   164,    -1,   180,   163,
     185,   164,    -1,   180,   163,     1,   164,    -1,    -1,    79,
      -1,    76,    -1,    -1,    78,    -1,   163,   164,    -1,   163,
     185,   164,    -1,   163,     1,   164,    -1,   177,    -1,   185,
     177,    -1,   304,    -1,   304,    13,   304,    -1,   186,   139,
     304,    -1,   186,   139,   304,    13,   304,    -1,    -1,   186,
      -1,   161,    -1,   186,    -1,    -1,    79,    -1,    76,    -1,
     189,   107,   186,   158,    -1,   189,   107,   304,    36,   188,
     158,    -1,   189,   107,   304,    13,   304,    36,   188,   158,
      -1,   189,   107,   304,    67,   187,   158,    -1,   189,   107,
     304,    13,   304,    67,   187,   158,    -1,    45,   197,   158,
      -1,    45,   197,    13,   197,   158,    -1,    84,   279,   158,
      -1,   307,   243,   306,   158,    -1,   307,   162,   306,   158,
      -1,   307,   130,   306,   158,    -1,   307,   124,   306,   158,
      -1,   307,   125,   306,   158,    -1,    -1,     3,    -1,     3,
      -1,    64,    -1,    96,    -1,    39,    -1,    99,    -1,   198,
      -1,     3,    -1,    64,    -1,    96,    -1,    39,    -1,    99,
      -1,   198,    -1,     3,    -1,    96,    -1,    17,    -1,    50,
      -1,   103,    -1,    80,    -1,    44,    -1,    25,    -1,    20,
      -1,    92,    -1,    94,    -1,    90,    -1,    71,    -1,    89,
      -1,    18,    -1,   106,    -1,    33,    -1,    47,    -1,    58,
      -1,    65,    -1,   109,    -1,    17,    -1,    50,    -1,   103,
      -1,    80,    -1,    44,    -1,    25,    -1,    20,    -1,    92,
      -1,    58,    -1,    65,    -1,   109,    -1,    94,    -1,    90,
      -1,    33,    -1,    47,    -1,    32,   179,    -1,   184,    -1,
      85,   158,    -1,    85,   306,   158,    -1,   158,    -1,   205,
      -1,    79,   205,    -1,    -1,    76,   204,   205,    -1,   237,
      -1,   266,    -1,   229,    -1,   262,    -1,   225,    -1,   206,
      -1,   207,    -1,    42,   304,   158,    -1,    42,   304,    36,
     188,   158,    -1,    42,   304,    67,   187,   158,    -1,    42,
     266,    -1,    -1,    38,    81,   208,   196,   227,   163,   228,
     164,    -1,    -1,    38,     8,    81,   209,   196,   227,   163,
     228,   164,    -1,    -1,    37,    81,   210,   196,   227,   163,
     228,   164,    -1,    -1,    37,     8,    81,   211,   196,   227,
     163,   228,   164,    -1,    38,   305,   237,    -1,    37,   305,
     237,    -1,    38,   305,   265,   267,   158,    -1,    37,   305,
     265,   267,   158,    -1,    38,    11,    -1,    32,   179,   112,
     304,   158,    -1,   112,   304,   201,    -1,    24,   304,    46,
     304,   293,   201,    -1,    24,   304,    46,   214,   293,   201,
      -1,    24,   304,   293,   201,    -1,    40,   304,    46,   304,
     201,    -1,    40,   304,    46,   214,   201,    -1,    40,   304,
     201,    -1,    40,   214,   201,    -1,    40,    72,   196,    46,
     304,   201,    -1,    41,   304,    46,   304,   201,    -1,    41,
     304,    46,   304,   296,   201,    -1,    41,   304,    46,   214,
     201,    -1,    41,   304,    46,   214,   296,   201,    -1,    41,
     304,   201,    -1,    41,   304,   296,   201,    -1,    41,   214,
     201,    -1,    41,   214,   296,   201,    -1,   167,   279,    46,
     304,   168,   179,    -1,   167,   279,    46,   304,   296,   168,
     179,    -1,   167,   279,    46,   214,   168,   179,    -1,   167,
     279,    46,   214,   296,   168,   179,    -1,   167,   279,   168,
     179,    -1,   167,   279,   296,   168,   179,    -1,   167,   214,
     168,   179,    -1,   167,   214,   296,   168,   179,    -1,   115,
     165,   279,   166,    -1,    43,   304,    95,   179,    -1,    43,
     304,   184,    -1,    43,   304,    95,   179,    34,   179,    -1,
      43,   304,   184,    34,   179,    -1,    43,   304,   243,   304,
      95,   179,    -1,    43,   304,   243,   304,   184,    -1,    43,
     304,   243,   304,    95,   179,    34,   179,    -1,    43,   304,
     243,   304,   184,    34,   179,    -1,    29,   179,    -1,   100,
     304,   158,    -1,   101,   304,   158,    -1,   100,   193,    -1,
     101,   193,    -1,   100,   184,   218,    -1,   101,   184,   218,
      -1,    -1,   218,   219,    -1,    21,   184,    -1,    21,   220,
     184,    -1,    21,   165,   220,   166,   184,    -1,   196,    -1,
     196,   138,   304,    -1,    97,   304,   158,    -1,    87,   304,
     163,   223,   164,    -1,    87,   304,   163,     1,   164,    -1,
      -1,   223,   224,    -1,   110,   279,   201,    -1,    68,   179,
      -1,    68,    32,   179,    -1,   226,   196,   227,   163,   228,
     164,    -1,   226,   196,   227,   163,     1,   164,    -1,    22,
      -1,    81,    -1,   105,    -1,    -1,   138,   279,    -1,    -1,
     228,   203,    -1,   228,   178,   203,    -1,   230,   196,   163,
     231,   164,    -1,   230,   196,   163,     1,   164,    -1,    35,
      -1,   232,    -1,   231,   139,    -1,   231,   139,   232,    -1,
     196,    -1,   196,   118,   304,    -1,    -1,    -1,    54,   234,
     245,   235,   252,   274,   258,   255,    -1,    -1,    48,    -1,
      70,    -1,    -1,    -1,   236,   251,   238,   240,   239,   252,
     273,   253,   258,   254,    -1,   250,   242,   244,    -1,   250,
     243,   244,    -1,   250,   241,   141,   242,   244,    -1,   250,
     241,   141,   243,   244,    -1,   250,     1,   244,    -1,   286,
      -1,   165,   304,   166,    -1,   195,    -1,   135,   196,    -1,
     134,    -1,   136,    -1,   137,    -1,   135,    -1,   144,    -1,
     154,    -1,   151,    -1,   147,    -1,   150,    -1,   146,    -1,
     156,    -1,   152,    -1,   161,    -1,   140,    -1,   159,    -1,
     160,    -1,   153,    -1,   145,    -1,   133,    -1,    19,    -1,
     148,    -1,    12,    -1,   162,    -1,   149,    -1,    52,    -1,
     196,   133,    -1,   118,    -1,   129,    -1,   126,    -1,   128,
      -1,   122,    -1,   127,    -1,   123,    -1,   119,    -1,   120,
      -1,   121,    -1,   132,    -1,   131,    -1,    -1,   165,   246,
     166,    -1,   165,   246,   166,    -1,    -1,   247,    -1,   246,
     139,   247,    -1,   248,   196,   278,   271,    -1,   178,   248,
     196,   278,   271,    -1,   248,   196,   278,   257,    -1,   248,
     165,   270,   166,   278,   271,    -1,   248,   165,   270,   166,
     278,   257,    -1,    -1,   249,    -1,    46,    -1,    49,    -1,
      69,    -1,    27,    -1,    27,    46,    -1,    27,    83,    -1,
      72,    -1,    83,    -1,   102,    -1,    -1,    72,    -1,    83,
      -1,    27,    83,    -1,    27,    -1,   102,    -1,    77,    -1,
      51,    -1,    -1,    27,    -1,    27,    83,    -1,    83,    -1,
      72,    -1,   102,    -1,    -1,    98,    -1,   158,    -1,   255,
      -1,   184,    -1,   202,    -1,   157,   196,    -1,     4,    -1,
     143,    -1,   143,   304,    -1,   143,   256,    -1,    -1,   111,
     304,    -1,    56,   259,    -1,   111,   304,    56,   259,    -1,
      56,   259,   111,   304,    -1,   260,    -1,   259,   139,   260,
      -1,   261,   118,   261,    -1,   261,   150,   261,    -1,   261,
     151,   261,    -1,   261,   144,   261,    -1,   261,   146,   261,
      -1,   261,   147,   261,    -1,    85,   261,    -1,     3,    -1,
      96,    -1,   102,   263,   158,    -1,    26,   102,   263,   158,
      -1,    38,   102,   263,   158,    -1,   196,   264,    -1,   196,
     264,   139,   263,    -1,    -1,   118,   287,    -1,   118,   275,
      -1,    72,    -1,    27,    -1,    83,    -1,    27,    83,    -1,
     108,    -1,    26,   265,   267,   158,    -1,   265,   267,   158,
      -1,   268,    -1,   267,   139,   268,    -1,   196,   274,   271,
      -1,   165,   270,   166,   274,   271,    -1,   104,    -1,   196,
      -1,   165,   270,   166,    -1,   269,    -1,   269,   139,    -1,
     269,   139,   270,    -1,    -1,   118,    63,    -1,   118,   306,
      -1,   167,   168,   287,    -1,   167,   168,    -1,   167,   279,
     168,   287,    -1,   167,   279,   168,    -1,   167,   168,   272,
      -1,   167,   279,   168,   272,    -1,   167,     1,   168,    -1,
      -1,   138,   287,    -1,   138,   272,    -1,   138,   200,    -1,
       1,    -1,    -1,   138,   287,    -1,   138,   275,    -1,   138,
     200,    -1,     1,    -1,   167,   279,   168,   287,    -1,   167,
     279,   168,   275,    -1,   167,   279,    46,   304,   168,   287,
      -1,   167,     1,   168,    -1,    -1,   287,    -1,   256,    -1,
     167,   168,   276,    -1,   167,   279,   168,   276,    -1,   167,
     168,   277,    -1,   167,   279,   168,   277,    -1,   167,   279,
      46,   304,   168,   276,    -1,    -1,   138,   287,    -1,   138,
     256,    -1,   138,   200,    -1,   138,   277,    -1,   304,    -1,
     256,    -1,   279,   139,   304,    -1,   279,   139,   256,    -1,
     304,    -1,   280,   139,   304,    -1,   104,    -1,   306,    -1,
     256,    -1,   281,   139,   281,    -1,   282,   139,   281,    -1,
      -1,   284,    -1,   285,    -1,   284,   139,   285,    -1,   197,
     118,   256,    -1,   197,   118,   306,    -1,   256,    -1,   306,
      -1,   197,    -1,   199,    -1,   288,    -1,   288,   157,    -1,
     157,    -1,   291,    -1,   307,    -1,   290,    -1,   316,    -1,
     315,    -1,    90,   304,    -1,    47,   165,   283,   166,    -1,
      33,   165,   283,   166,    -1,    93,   165,   283,   166,    -1,
      91,    93,   165,   283,   166,    -1,    14,   304,    -1,    94,
     304,    -1,    71,    -1,    71,   304,    -1,   106,    -1,   106,
     304,    -1,    89,    -1,    89,   304,    -1,    18,    -1,    18,
     304,    -1,    22,    -1,    81,    -1,    35,    -1,    40,   304,
      46,   304,    32,   304,    -1,    40,   304,    46,   214,    32,
     304,    -1,    40,   304,    32,   304,    -1,    40,   304,    46,
     304,    32,    43,   304,    95,   304,    -1,    40,   304,    46,
     214,    32,    43,   304,    95,   304,    -1,    40,   304,    32,
      43,   304,    95,   304,    -1,    41,   304,    46,   304,    32,
     304,    -1,    41,   304,    46,   214,    32,   304,    -1,    41,
     304,    32,   304,    -1,    41,   304,    46,   304,    32,    43,
     304,    95,   304,    -1,    41,   304,    46,   214,    32,    43,
     304,    95,   304,    -1,    41,   304,    32,    43,   304,    95,
     304,    -1,   167,   279,   168,   304,    -1,   167,   279,    46,
     304,   168,   304,    -1,   167,   279,    46,   214,   168,   304,
      -1,   167,   279,    46,   304,   168,    43,   304,    95,   304,
      -1,   167,   279,    46,   214,   168,    43,   304,    95,   304,
      -1,    43,   304,    95,   304,    34,   304,    -1,    62,    -1,
     291,    -1,   286,    -1,   310,    -1,   309,    -1,   233,    -1,
     302,    -1,   303,    -1,   300,   149,   304,    -1,    -1,   294,
      -1,   113,   165,   295,   166,    -1,   298,    -1,   295,   139,
     298,    -1,   113,   165,   297,   166,    -1,   298,    -1,   297,
     139,   298,    -1,   299,   286,   274,   271,    -1,   319,    82,
     286,    -1,   304,    82,   286,    -1,    27,    -1,    46,    -1,
      27,    46,    -1,    83,    -1,    27,    83,    -1,   108,    -1,
     307,    -1,   300,   149,   304,    -1,    61,    -1,    61,    71,
      -1,    61,    89,    -1,    61,   106,    -1,    61,    18,    -1,
     301,   304,    -1,    61,    71,   165,   304,   166,   165,   283,
     166,    -1,    61,    89,   165,   304,   166,   165,   283,   166,
      -1,    61,    71,   165,   304,   166,   165,   283,   166,   157,
      -1,    61,    89,   165,   304,   166,   165,   283,   166,   157,
      -1,    61,    71,   165,   304,   166,    -1,    61,    89,   165,
     304,   166,    -1,    55,   267,    46,   304,    -1,   313,    -1,
     287,    -1,   289,    -1,   317,    -1,   318,    -1,   233,    -1,
     302,    -1,   303,    -1,   165,   143,   304,   166,    -1,   304,
     138,   304,    -1,   304,   142,   304,    -1,   304,   170,   304,
      -1,   304,   142,    -1,   142,   304,    -1,   170,   304,    -1,
     142,    -1,    -1,   304,    -1,   100,   304,    -1,   101,   304,
      -1,   304,    -1,   286,    -1,   309,    -1,   310,    -1,   311,
      -1,   307,    -1,   304,   133,    -1,   288,   157,    -1,   233,
      -1,   308,   165,   283,   166,    -1,   308,   167,   283,   168,
      -1,    75,   165,   283,   166,    -1,   304,   141,   197,    -1,
     304,   141,   102,    -1,   304,   141,    33,    -1,   304,   141,
      58,    -1,   304,   141,    20,   165,   166,    -1,   165,   281,
     166,    -1,   165,   281,   139,   166,    -1,   165,   282,   166,
      -1,   165,   282,   139,   166,    -1,    39,    -1,    99,    -1,
     312,    -1,     5,    -1,     6,    -1,     7,    -1,     8,    -1,
       9,    -1,    10,    -1,    64,    -1,   163,   279,   164,    -1,
     163,   279,   139,   164,    -1,   167,   279,   168,    -1,   167,
     279,   139,   168,    -1,   167,   314,   168,    -1,   167,   314,
     139,   168,    -1,   304,   116,   304,    -1,   314,   139,   304,
     116,   304,    -1,   304,   156,   304,    -1,   304,   152,   304,
      -1,   304,   161,   304,    -1,   304,   140,   304,    -1,   304,
     159,   304,    -1,   304,   160,   304,    -1,   304,   153,   304,
      -1,   304,   144,   304,    -1,   304,   154,   304,    -1,   304,
     151,   304,    -1,   304,   147,   304,    -1,   304,   150,   304,
      -1,   304,   146,   304,    -1,   304,   134,   304,    -1,   304,
     136,   304,    -1,   304,   137,   304,    -1,   304,   117,   304,
      -1,   304,   155,   304,    -1,   304,   145,   304,    -1,   304,
      19,   304,    -1,   304,    12,   304,    -1,   304,   148,   304,
      -1,   304,    31,   304,    -1,   156,   304,    -1,   152,   304,
      -1,    59,   304,    -1,    73,   304,    -1,   133,   304,    -1,
     304,   133,    -1,   135,   304,    -1,   304,    82,   304,    -1,
     304,    82,   214,    -1,   319,    82,   304,    -1,   319,    82,
     214,    -1,   304,    86,   304,    -1,   304,    86,   214,    -1,
     319,    86,   304,    -1,   319,    86,   214,    -1,   156,    -1,
     161,    -1,   117,    -1,   155,    -1,   134,    -1,   136,    -1,
     137,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   499,   499,   504,   505,   511,   512,   517,   518,   523,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   547,   548,   549,   550,   551,   552,   553,
     557,   570,   575,   580,   588,   589,   590,   594,   595,   608,
     609,   610,   615,   616,   621,   626,   631,   635,   642,   647,
     651,   656,   660,   661,   662,   666,   667,   668,   669,   670,
     674,   675,   680,   684,   686,   688,   690,   692,   699,   700,
     704,   705,   706,   707,   708,   709,   712,   713,   714,   715,
     716,   717,   729,   730,   741,   742,   743,   744,   745,   746,
     747,   748,   749,   750,   751,   752,   753,   754,   755,   756,
     757,   758,   759,   763,   764,   765,   766,   767,   768,   769,
     770,   771,   772,   773,   780,   781,   782,   783,   787,   788,
     792,   793,   797,   798,   799,   809,   809,   814,   815,   816,
     817,   818,   819,   820,   824,   825,   826,   827,   832,   831,
     847,   846,   863,   862,   878,   877,   893,   897,   902,   910,
     921,   928,   929,   930,   931,   932,   933,   934,   935,   936,
     937,   938,   939,   940,   941,   942,   943,   944,   945,   946,
     952,   958,   964,   970,   977,   984,   988,   995,   999,  1000,
    1001,  1002,  1003,  1005,  1007,  1009,  1014,  1017,  1018,  1019,
    1020,  1021,  1022,  1026,  1027,  1031,  1032,  1033,  1037,  1038,
    1042,  1045,  1047,  1052,  1053,  1057,  1059,  1061,  1068,  1078,
    1092,  1097,  1102,  1110,  1111,  1116,  1117,  1119,  1124,  1140,
    1147,  1156,  1164,  1168,  1175,  1176,  1181,  1186,  1180,  1213,
    1216,  1220,  1228,  1238,  1227,  1270,  1274,  1279,  1283,  1288,
    1295,  1296,  1300,  1301,  1302,  1303,  1304,  1305,  1306,  1307,
    1308,  1309,  1310,  1311,  1312,  1313,  1314,  1315,  1316,  1317,
    1318,  1319,  1320,  1321,  1322,  1323,  1324,  1325,  1326,  1327,
    1331,  1332,  1333,  1334,  1335,  1336,  1337,  1338,  1339,  1340,
    1341,  1342,  1346,  1347,  1351,  1355,  1356,  1357,  1361,  1363,
    1365,  1367,  1369,  1374,  1375,  1379,  1380,  1381,  1382,  1383,
    1384,  1385,  1386,  1387,  1391,  1392,  1393,  1394,  1395,  1396,
    1400,  1401,  1405,  1406,  1407,  1408,  1409,  1410,  1414,  1415,
    1418,  1419,  1423,  1424,  1428,  1433,  1437,  1438,  1439,  1447,
    1448,  1450,  1452,  1454,  1459,  1461,  1466,  1467,  1468,  1469,
    1470,  1471,  1472,  1476,  1478,  1483,  1485,  1487,  1492,  1505,
    1522,  1523,  1525,  1530,  1531,  1532,  1533,  1534,  1538,  1544,
    1552,  1553,  1561,  1563,  1568,  1570,  1572,  1577,  1579,  1581,
    1588,  1589,  1590,  1595,  1597,  1599,  1603,  1607,  1609,  1613,
    1621,  1622,  1623,  1624,  1625,  1630,  1631,  1632,  1633,  1634,
    1654,  1658,  1662,  1670,  1677,  1678,  1679,  1683,  1685,  1691,
    1693,  1695,  1700,  1701,  1702,  1703,  1704,  1710,  1711,  1712,
    1713,  1717,  1718,  1722,  1723,  1724,  1728,  1729,  1733,  1734,
    1738,  1739,  1743,  1744,  1745,  1746,  1750,  1751,  1762,  1764,
    1766,  1772,  1773,  1774,  1775,  1776,  1777,  1779,  1781,  1783,
    1785,  1787,  1789,  1792,  1794,  1796,  1798,  1800,  1802,  1804,
    1806,  1809,  1811,  1813,  1818,  1820,  1822,  1824,  1826,  1828,
    1830,  1832,  1834,  1836,  1838,  1840,  1842,  1849,  1855,  1861,
    1867,  1876,  1886,  1894,  1895,  1896,  1897,  1898,  1899,  1900,
    1901,  1906,  1907,  1911,  1915,  1916,  1920,  1924,  1925,  1929,
    1933,  1937,  1944,  1945,  1946,  1947,  1948,  1949,  1953,  1954,
    1959,  1961,  1965,  1969,  1973,  1981,  1986,  1992,  1998,  2005,
    2014,  2016,  2021,  2029,  2030,  2031,  2032,  2033,  2034,  2035,
    2036,  2037,  2039,  2041,  2043,  2058,  2060,  2062,  2064,  2069,
    2070,  2074,  2075,  2076,  2080,  2081,  2082,  2083,  2094,  2095,
    2096,  2097,  2101,  2102,  2103,  2107,  2108,  2109,  2110,  2111,
    2119,  2120,  2121,  2122,  2126,  2127,  2131,  2132,  2133,  2134,
    2135,  2136,  2137,  2138,  2139,  2140,  2141,  2142,  2143,  2147,
    2155,  2156,  2160,  2161,  2162,  2163,  2164,  2165,  2166,  2167,
    2168,  2169,  2170,  2171,  2172,  2173,  2174,  2175,  2176,  2177,
    2178,  2179,  2180,  2181,  2182,  2186,  2187,  2188,  2189,  2190,
    2191,  2192,  2196,  2197,  2198,  2199,  2203,  2204,  2205,  2206,
    2211,  2212,  2213,  2214,  2215,  2216,  2217
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
  "TNOELSE", "TDOTDOTOPENHIGH", "TUMINUS", "TUPLUS", "TLNOT", "$accept",
  "program", "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
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
     425,   426,   427,   428
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   174,   175,   176,   176,   177,   177,   178,   178,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     180,   181,   181,   181,   182,   182,   182,   183,   183,   184,
     184,   184,   185,   185,   186,   186,   186,   186,   187,   187,
     188,   188,   189,   189,   189,   190,   190,   190,   190,   190,
     191,   191,   192,   193,   193,   193,   193,   193,   194,   194,
     195,   195,   195,   195,   195,   195,   196,   196,   196,   196,
     196,   196,   197,   197,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   200,   200,   200,   200,   201,   201,
     202,   202,   203,   203,   203,   204,   203,   205,   205,   205,
     205,   205,   205,   205,   206,   206,   206,   206,   208,   207,
     209,   207,   210,   207,   211,   207,   207,   207,   207,   207,
     212,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   214,   215,   215,
     215,   215,   215,   215,   215,   215,   216,   217,   217,   217,
     217,   217,   217,   218,   218,   219,   219,   219,   220,   220,
     221,   222,   222,   223,   223,   224,   224,   224,   225,   225,
     226,   226,   226,   227,   227,   228,   228,   228,   229,   229,
     230,   231,   231,   231,   232,   232,   234,   235,   233,   236,
     236,   236,   238,   239,   237,   240,   240,   240,   240,   240,
     241,   241,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   244,   244,   245,   246,   246,   246,   247,   247,
     247,   247,   247,   248,   248,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   250,   250,   250,   250,   250,   250,
     251,   251,   252,   252,   252,   252,   252,   252,   253,   253,
     254,   254,   255,   255,   256,   256,   257,   257,   257,   258,
     258,   258,   258,   258,   259,   259,   260,   260,   260,   260,
     260,   260,   260,   261,   261,   262,   262,   262,   263,   263,
     264,   264,   264,   265,   265,   265,   265,   265,   266,   266,
     267,   267,   268,   268,   269,   269,   269,   270,   270,   270,
     271,   271,   271,   272,   272,   272,   272,   272,   272,   272,
     273,   273,   273,   273,   273,   274,   274,   274,   274,   274,
     275,   275,   275,   275,   276,   276,   276,   277,   277,   277,
     277,   277,   278,   278,   278,   278,   278,   279,   279,   279,
     279,   280,   280,   281,   281,   281,   282,   282,   283,   283,
     284,   284,   285,   285,   285,   285,   286,   286,   287,   287,
     287,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   290,   291,   292,   292,   292,   292,   292,   292,   292,
     292,   293,   293,   294,   295,   295,   296,   297,   297,   298,
     298,   298,   299,   299,   299,   299,   299,   299,   300,   300,
     301,   301,   301,   301,   301,   302,   302,   302,   302,   302,
     302,   302,   303,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   305,
     305,   306,   306,   306,   307,   307,   307,   307,   308,   308,
     308,   308,   309,   309,   309,   310,   310,   310,   310,   310,
     311,   311,   311,   311,   312,   312,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     314,   314,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   316,   316,   316,   316,   316,
     316,   316,   317,   317,   317,   317,   318,   318,   318,   318,
     319,   319,   319,   319,   319,   319,   319
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
       1,     4,     3,     3,     3,     2,     2,     2,     1,     0,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     4,     4,     4,     3,     3,     3,     3,     5,
       3,     4,     3,     4,     1,     1,     1,     1,     1,     1,
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
       3,     0,     0,     1,     0,    92,   567,   568,   569,   570,
     571,   572,     0,   491,    78,   113,   459,   119,   461,   491,
       0,   118,     0,   364,    78,     0,     0,     0,     0,   463,
     539,   539,   564,     0,     0,     0,     0,   117,     0,     0,
     240,   114,     0,   236,     0,     0,   121,     0,   510,   482,
     573,   122,     0,   241,   453,   363,     0,     0,     0,   135,
      45,   116,   462,   365,     0,     0,     0,     0,   457,     0,
       0,   120,     0,     0,    93,     0,   565,     0,     0,     0,
     115,   222,   455,   367,   123,     0,     0,   622,     0,   624,
       0,   625,   626,   538,     0,   623,   620,   440,   132,   621,
       0,     0,     0,     0,     4,     0,     5,     0,     9,    47,
      10,     0,    11,    12,    13,    15,   436,   437,    23,    14,
     133,   142,   143,    16,    18,    17,    20,    21,    22,    19,
     141,     0,   139,     0,   528,     0,   137,   140,     0,   138,
     544,   524,   438,   525,   443,   441,     0,     0,     0,   529,
     530,     0,   442,     0,   545,   546,   547,   566,   523,   445,
     444,   526,   527,     0,    39,    25,   451,     0,     0,   492,
      79,     0,     0,   461,   463,     0,     0,     0,   462,     0,
       0,     0,   528,   544,   441,   529,   530,   460,   442,   545,
     546,     0,   491,     0,     0,   366,     0,   196,     0,   421,
       0,   428,   570,   462,   540,   239,   570,   160,   462,     0,
     239,     0,     0,     0,     0,     0,     0,     0,   147,     0,
       0,     0,   428,    86,    94,   106,   100,    99,   108,    89,
      98,   109,    95,   110,    87,   111,   104,    97,   105,   103,
     101,   102,    88,    90,    96,   107,   112,     0,    91,     0,
       0,     0,     0,   370,     0,   129,    33,     0,   607,   514,
     511,   512,   513,     0,   454,   608,     7,   428,   239,   220,
     230,   539,   221,   134,   335,   440,   418,     0,   417,     0,
       0,   130,   543,     0,     0,    36,     0,   458,   446,     0,
     428,    37,   452,     0,   203,   199,     0,   442,   203,   200,
       0,   360,     0,   456,     0,     0,   609,   611,   536,   606,
     605,     0,    49,    52,     0,     0,   423,     0,   425,     0,
       0,   424,     0,     0,   417,     0,   537,     0,     6,     0,
      48,     0,     0,   223,     0,   321,   320,   242,     0,   439,
      24,     0,   515,     0,     0,     0,     0,     0,     0,   610,
       0,     0,     0,     0,     0,     0,   535,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   280,   287,   288,   289,   284,   286,     0,
       0,   282,   285,   283,   281,     0,   291,   290,     0,     0,
     428,   428,     0,     0,     0,    26,    27,     0,     0,     0,
       0,     0,    28,     0,     0,     0,     0,    29,     0,    30,
       0,   436,   434,     0,   429,   430,   435,   154,     0,   157,
       0,   150,     0,     0,   156,     0,     0,     0,   169,     0,
       0,   168,     0,   177,     0,     0,     0,   175,     0,     0,
      58,   144,     0,   189,     0,     0,    70,     0,    31,   303,
     237,   374,     0,   375,   377,     0,   399,     0,   380,     0,
       0,   128,    32,     0,     0,    34,     0,   136,   334,     0,
      72,   541,   542,   131,     0,    35,   428,     0,   210,   201,
     197,   202,   198,     0,   358,   355,   162,    38,    51,    50,
      53,     0,   574,     0,     0,   560,     0,   562,     0,     0,
       0,     0,     0,     0,     0,     0,   578,     8,     0,    41,
       0,     0,     0,    54,     0,     0,     0,   314,   369,   490,
     602,   601,   604,   613,   612,   617,   616,   598,   595,   596,
     597,   532,   585,     0,   557,   558,   556,   555,   533,   589,
     600,   594,   592,   603,   593,   591,   583,   588,   590,   599,
     582,   586,   587,   584,   534,     0,     0,     0,     0,     0,
       0,     0,   615,   614,   619,   618,   502,   503,   505,   507,
       0,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   576,   491,   491,   165,   356,   368,   422,     0,     0,
     448,     0,     0,   223,     0,     0,   223,   357,     0,     0,
       0,     0,   466,     0,     0,     0,   178,     0,   472,     0,
       0,   176,   621,    61,     0,    54,    59,     0,   188,     0,
       0,     0,     0,   447,   308,   305,   306,   307,   311,   312,
     313,   303,     0,   296,     0,   304,   322,     0,   378,     0,
     126,   127,   125,   124,     0,   398,   397,   524,     0,   372,
     522,   371,     0,     0,   554,   420,   419,     0,     0,     0,
     449,     0,   204,   362,   524,     0,   575,   531,   561,   426,
     563,   427,   185,     0,     0,     0,   577,   183,   476,     0,
     580,   579,     0,    43,    42,    40,     0,    65,     0,     0,
      58,   224,     0,     0,   234,     0,   231,   318,   315,   316,
     319,   243,     0,     0,    76,    77,    75,    74,    73,   552,
     553,   504,   506,     0,   493,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   161,   432,
     433,   431,   223,     0,   159,   223,     0,   158,     0,   187,
       0,     0,   167,     0,   166,     0,   497,     0,     0,   173,
       0,     0,   171,     0,   145,     0,   146,     0,     0,   191,
       0,   193,    71,   309,   310,     0,   303,   294,     0,   412,
     323,   326,   325,   327,     0,   376,   379,   380,     0,     0,
     381,   382,   520,   521,   212,     0,     0,   211,   214,   450,
       0,   205,   208,     0,   359,   186,     0,     0,     0,     0,
     184,     0,    56,    55,     0,     0,     0,   239,   229,     0,
     232,   228,   317,   322,   292,    80,   275,    94,   273,   100,
      99,    83,    98,    95,   278,   110,    81,   111,    97,   101,
      82,    84,    96,   112,   272,   254,   257,   255,   256,   267,
     258,   271,   263,   261,   274,   277,   262,   260,   265,   270,
     259,   264,   268,   269,   266,   276,     0,   252,     0,    85,
       0,   292,   292,   250,   559,   495,   380,   544,   544,     0,
       0,     0,     0,     0,     0,     0,     0,   164,   163,     0,
     225,     0,   225,   170,     0,     0,   465,     0,   464,     0,
     496,     0,     0,   471,   174,     0,   470,   172,    55,   190,
     481,   192,     0,   412,   297,     0,     0,   380,   324,   339,
     373,   403,     0,   576,   428,   428,     0,   216,     0,     0,
       0,   206,     0,   181,   478,     0,     0,   179,   477,     0,
     581,     0,     0,    58,    66,    68,   219,   135,   239,   218,
     239,   226,   235,   233,     0,   303,   249,   253,     0,   279,
       0,   245,   246,   499,     0,     0,     0,     0,     0,     0,
       0,     0,   225,   239,   225,   239,   469,     0,     0,   498,
     475,     0,     0,     0,   195,   380,   412,     0,   415,   414,
     416,   524,   336,   300,   298,     0,     0,     0,     0,   401,
     524,     0,     0,   217,   215,     0,   209,     0,   182,     0,
     180,    57,     0,     0,   227,   394,     0,   328,     0,   251,
      80,    82,   292,   292,     0,     0,     0,     0,     0,     0,
     239,   153,   239,   149,     0,     0,     0,     0,   194,   299,
     380,   404,     0,   338,   337,   353,     0,   354,   341,   344,
       0,   340,   332,   333,   238,     0,   516,   517,   207,     0,
       0,    67,    69,     0,   393,   392,   524,   329,   339,   293,
     247,   248,     0,     0,     0,     0,     0,     0,   155,   151,
     468,   467,   474,   473,   302,   301,   406,   407,   409,   524,
       0,   576,   352,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   524,   518,   519,   480,   479,     0,   384,     0,
       0,     0,   408,   410,   343,   345,   346,   349,   350,   351,
     347,   348,   342,   389,   387,   524,   576,   330,   244,   331,
     404,   388,   524,   411
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   313,   105,   618,   107,   108,   109,   331,
     110,   314,   613,   617,   614,   111,   112,   113,   114,   115,
     171,   857,   251,   116,   248,   117,   645,   256,   118,   119,
     268,   120,   121,   122,   422,   595,   418,   592,   123,   124,
     724,   125,   126,   127,   479,   662,   793,   128,   129,   658,
     788,   130,   131,   515,   807,   132,   133,   695,   696,   182,
     249,   636,   135,   136,   517,   813,   701,   860,   861,   444,
     946,   450,   632,   633,   634,   635,   702,   337,   774,  1058,
    1118,  1044,   276,   983,   987,  1038,  1039,  1040,   137,   302,
     484,   138,   139,   252,   253,   454,   455,   649,  1055,  1007,
     458,   646,  1077,   980,   907,   315,   198,   319,   320,   413,
     414,   415,   183,   141,   142,   143,   144,   184,   146,   168,
     169,   570,   434,   745,   571,   572,   147,   148,   185,   186,
     151,   210,   416,   188,   153,   189,   190,   156,   157,   158,
     325,   159,   160,   161,   162,   163
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1015
static const yytype_int16 yypact[] =
{
   -1015,   102,  2863, -1015,   -46, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015,  4207,    13,   153, -1015, 13138, -1015, 18652,    13,
    9897, -1015,   411,    97,   153,  4207,  9897,  4207,    86, 18732,
   10063,  2654, -1015,  7557,  8561,  6221,  9897, -1015,    31,   109,
   -1015, -1015, 18769, -1015, 13538,  8727, -1015,  9897,    46, -1015,
   -1015, -1015,  9897, -1015, 13138, -1015,  9897,   234,   122,   188,
    1709, -1015, 18827, -1015,  8895,  7057,  9897,  8727, 13138,  9897,
     163, -1015,   125,  4207, -1015,  9897, -1015, 10229, 10229, 18769,
   -1015, -1015, 13138, -1015, -1015,  9897,  9897, -1015,  9897, -1015,
    9897, -1015, -1015, 12677,  9897, -1015,  9897, -1015, -1015, -1015,
    3199,  6389,  7725,  9897, -1015,  4039, -1015,   201, -1015,   308,
   -1015,   297, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, 18769, -1015, 18769,   378,   242, -1015, -1015, 13538, -1015,
     251, -1015,   259, -1015, -1015,   278,   288,   300,  9897,   322,
     323, 17973,  2229,   220,   338,   339, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015,   362, -1015, -1015, 17973,   349,  4207, -1015,
   -1015,   346,  9897, -1015, -1015,  9897,  9897,  9897, -1015,  9897,
    8895,  8895,   345, -1015, -1015, -1015, -1015,   155,   353, -1015,
   -1015,   360, 14865, 18769, 13538, -1015,   369, -1015,   232, 17973,
     418,  7225,   454, 18864, 17973,   393,   457, -1015, 18922, 18769,
     393, 18769,   374,    39, 14544,    15, 14487,   367, -1015, 14639,
   13754,    33,  7225, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015,  4207, -1015,   379,
    2359,    48,    76, -1015,  4207, -1015, -1015, 14941,   949, -1015,
     383,   384, -1015, 14941,   155,   949, -1015,  7225,  1870, -1015,
   -1015,  9061, -1015, -1015, -1015, 18769, -1015,   249, 17973,  9897,
    9897, -1015, 17973,   395, 15477, -1015, 14941,   155, 17973,   392,
    7225, -1015, 17973, 15553, -1015, -1015, 15633, 12670, -1015, -1015,
   15713,   436,   401,   155, 14941, 15793,   161,   161,   990,   949,
     949,   139, -1015, -1015,  3367,   140, -1015,  9897, -1015,    51,
      91, -1015,   -27,   -18, 15873,    -7,   990,   552, -1015,  3535,
   -1015,   501,  9897,   424,   407, -1015, -1015, -1015,   253,   366,
   -1015,  9897,   430,  9897,  9897,  9897,  8561,  8561,  9897,   391,
    9897,  9897,  9897,  9897,  9897,   225, 12677,  9897,  9897,  9897,
    9897,  9897,  9897,  9897,  9897,  9897,  9897,  9897,  9897,  9897,
    9897,  9897,  9897, -1015, -1015, -1015, -1015, -1015, -1015,  7891,
    7891, -1015, -1015, -1015, -1015,  7891, -1015, -1015,  7891,  7891,
    7225,  7225,  8561,  8561,  6891, -1015, -1015, 15017, 15093, 15949,
      59,  3703, -1015,  8561,    39,   416,   254, -1015,  9897, -1015,
    9897,   458, -1015,   412,   441, -1015, -1015, -1015, 18769, -1015,
   13538, -1015, 18769,   428, -1015, 13538,   536,  8895, -1015,  4375,
    8561, -1015,   423, -1015,    39,  4543,  8561, -1015,    39, 10395,
    9897, -1015,  4207,   556,  9897,    31, -1015,   426, -1015,   554,
   -1015, -1015,  2359, -1015,   459,   438, -1015, 10561,   484,  9897,
   13538, -1015, -1015,  9897,  9897, -1015,   439, -1015, -1015,  8895,
   -1015, 17973, 17973, -1015,    30, -1015,  7225,   445, -1015,   591,
   -1015,   591, -1015, 10727,   474, -1015, -1015, -1015, -1015, -1015,
   -1015,  8059, -1015, 16029,  6557, -1015,  6725, -1015,  4207,   448,
    8561,  8227,  3031,   449,  9897,  9227, -1015, -1015,   255, -1015,
    3871, 18769,   271, 14696,  8895,   456, 18615,   235, -1015, 16105,
   18286, 18286,   396, -1015,   396, -1015,   396, 18381,  1392,   999,
    1327,   155,   161,   455, -1015, -1015, -1015, -1015,   990, 18419,
     396,   671,   671, 18286,   671,   671,   642,   161, 18419, 18343,
     642,   949,   949,   161,   990,   463,   464,   466,   467,   469,
     468,   461, -1015,   396, -1015,   396,    -3, -1015, -1015, -1015,
     110, -1015,  1546, 18049,   413, 10893,  8561, 11059,  8561,  9897,
    8561, 13400,    13, 16181, -1015, -1015, -1015, 17973, 16257,  7225,
   -1015,  7225, 18769,   424,   315, 18769,   424, -1015,   319,  9897,
     114,  9897, 17973,    40, 15169,  6891, -1015,  9897, 17973,    18,
   14789, -1015,   477,   500,   482, 16337,   500,   486,   608, 16413,
    4207, 15245,   487, -1015,    55, -1015, -1015, -1015, -1015, -1015,
   -1015,   569,   126, -1015, 13575, -1015,   415,   480,  2359,    48,
      86,   109,  9897,  9897,  6055, -1015, -1015,   389,  7391, -1015,
   17973, -1015, 16489, 16565, -1015, -1015, 17973,   485,    -6,   488,
   -1015, 13480, -1015, -1015,   333, 18769, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015,  4207,   -20, 15325, -1015, -1015, 17973,  4207,
   17973, -1015, 16641, -1015, -1015, -1015,  9897, -1015,  9897, 10395,
    9897,   509,  1350,   494,   547,   145, -1015,   583, -1015, -1015,
   -1015, -1015, 12516,   502, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015,  6891, -1015,    35,  8561,  8561,  9897,   635,
   16717,  9897,   637, 16793,   504, 16869,    39,    39, -1015, -1015,
   -1015, -1015,   424,   507, -1015,   424,   512, -1015, 14941, -1015,
   13834,  4711, -1015,  4879, -1015,   223, -1015, 13914,  5047, -1015,
      39,  5215, -1015,    39, -1015,  9897, -1015,  4207,  9897, -1015,
    4207,   645, -1015, -1015, -1015, 18769,   686, -1015,  2359,   543,
     602, -1015, -1015, -1015,    74, -1015, -1015,   484,   520,    61,
   -1015, -1015,   524,   525, -1015,  5383,  8895, -1015, -1015, -1015,
   18769, -1015,   553,   360, -1015, -1015,  5551,   526,  5719,   527,
   -1015,  9897, 16945, 15401,   534,   538,   533,  2485, -1015,  9897,
   18769, -1015, -1015,   415,   545,   262, -1015,   557, -1015,   562,
     570,   584,   575,   578, -1015,   580,   590,   585,   586,   589,
     293,   600,   593,   596, -1015, -1015, 18769, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015,  9897, -1015,   605,   607,
     603,   545,   545, -1015, -1015, -1015,   484,   231,   233, 17021,
   11225, 11391, 17101, 11557, 11723, 11889, 12055, -1015, -1015,   579,
   -1015,   588, -1015, -1015,  4207,  9897, 17973,  9897, 17973,  6891,
   -1015,  4207,  9897, 17973, -1015,  9897, 17973, -1015, 17973, -1015,
   17973,   707,  4207,   543, -1015,   581,  9395,   265, -1015,    43,
   -1015, -1015,  8561, 13269,  7225,  7225,  4207, -1015,    60,   595,
    9897, -1015,  9897, -1015, 17973,  4207,  9897, -1015, 17973,  4207,
   17973,  9897, 10395,  9897, -1015, -1015, -1015, -1015,  1870, -1015,
   18555, -1015, 17973, -1015,   123,   554, -1015, -1015, 17181, -1015,
   13673, -1015, -1015, -1015,  9897,  9897,  9897,  9897,  9897,  9897,
    9897,  9897, -1015, 18185, -1015, 18199, 16413, 13994, 14074, -1015,
   16413, 14154, 14234,  4207, -1015,   484,   543,  8395, -1015, -1015,
   -1015,   132,  8895, -1015, -1015,   111,  9897,    -8, 17257, -1015,
     843,   597,   599,   418, -1015,   360, 17973, 14314, -1015, 14394,
   -1015, 17973,   587,   594, -1015, -1015, 12221,   650,   236, -1015,
     616,   621,   545,   545, 17333, 17414, 17494, 17574, 17654, 17734,
   18257, -1015, 18297, -1015,  4207,  4207,  4207,  4207, -1015, -1015,
     265,  9563,    65, -1015, 17973, -1015,   120, -1015,   133, -1015,
     164, 17821, -1015, -1015, -1015, 12055,   609,   610, -1015,  4207,
    4207, -1015, -1015,  5887, -1015, -1015,    77, -1015,    43, -1015,
   -1015, -1015,  9897,  9897,  9897,  9897,  9897,  9897, -1015, -1015,
   16413, 16413, 16413, 16413, -1015, -1015, -1015, -1015, -1015,   146,
    8561, 12842, -1015,  9897,   111,   120,   120,   120,   120,   120,
     120,   111,  1111, -1015, -1015, 16413, 16413,   604, 12387,    70,
     -48, 17897, -1015, -1015, 17973, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015,   617, -1015, -1015,   358, 13007, -1015, -1015, -1015,
    9731, -1015,   601, -1015
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1015, -1015, -1015,    14,  -423,  1623, -1015, -1015, -1015, -1015,
     246,   433,  -327,  -670,  -662, -1015, -1015, -1015, -1015,    93,
     744, -1015,  1708,    10,  -677, -1015,  -855,  1887,  -955,  -768,
   -1015,   -54, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
      75, -1015, -1015, -1015,   472, -1015,   -17, -1015, -1015, -1015,
   -1015, -1015, -1015,  -496,  -738, -1015, -1015, -1015,   -33,   973,
   -1015, -1015, -1015,    36, -1015, -1015, -1015, -1015,  -165,  -151,
    -793, -1015,  -155,    32,   175, -1015, -1015, -1015,     1, -1015,
   -1015,  -290,   324,  -213,  -239,  -270,  -262,  -426, -1015,  -180,
   -1015,     8,   791,   -51,   368, -1015,  -407,  -770,  -959, -1015,
    -581,  -473, -1014,  -952,  -800,   -61, -1015,  -347, -1015,  -207,
   -1015,   243,   582,  -398, -1015, -1015, -1015,  1089, -1015,   -10,
   -1015, -1015,  -204, -1015,  -588, -1015, -1015, -1015,  1163,  1268,
     -12,   803,   -63,   769, -1015,  1656,  1826, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015,  -383
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -577
static const yytype_int16 yytable[] =
{
     166,   389,   283,   277,   187,   512,   273,   910,   192,   191,
     663,   574,   438,   405,   199,   447,   104,   746,   204,   204,
     805,   214,   216,   219,   220,   859,   631,   804,   500,   423,
     194,   657,  1043,   257,     5,   258,   456,    65,   321,   941,
     263,   323,   264,   711,   265,   637,   445,   254,   221,   456,
     748,   978,   278,   282,   284,   286,   287,   288,   777,   647,
     466,   292,   785,   293,   259,   296,   300,  1102,   951,   952,
     303,   254,   741,   304,   305,   456,   306,    65,   307,  1078,
     712,   308,   309,   477,   310,   664,   432,   338,   278,   282,
     324,   326,   254,   432,  -395,   432,   953,   733,  -213,   985,
     736,   763,     3,   975,   786,   580,  1123,   912,   213,   215,
    1117,  1080,   164,   616,  1035,   401,   580,   260,   499,   503,
     400,   501,   459,  1035,  1005,   865,   167,    74,   432,  1103,
    -395,   432,   505,  -391,   866,   261,   342,   984,   764,  1114,
    -213,   498,   963,   406,   965,  1043,   389,   669,   796,   671,
     502,  1054,   262,  -395,   986,   401,   170,  1121,   787,  -395,
     166,   506,  -391,   397,   398,   399,  -395,   292,   278,   324,
     295,   299,  1004,   457,  -395,  -391,  1030,   322,   401,  -390,
     195,   401,   404,   560,   561,  -395,   457,  -395,  -391,   282,
     494,   446,   345,   909,  -213,   941,  1036,   941,   501,   469,
     501,  -395,   401,   401,   501,  1029,  -395,  1037,  -390,   501,
     282,   411,   457,   420,   467,   460,  1037,   495,   425,  1060,
    1061,  -390,   574,   401,  1020,   194,  1022,   581,     5,   913,
     496,   776,   411,  1081,  -390,  -391,   879,  -395,  1116,   881,
    -391,   419,   266,   346,  1083,   533,   424,   347,   -46,   713,
    -413,   201,   941,   469,   941,   282,   289,   497,   534,   204,
    1075,  1006,   697,  1003,  -405,   766,   -46,   471,   472,   659,
    1002,  -413,  1084,   859,   222,  -413,   714,   411,   282,   491,
     739,  -390,  1085,   535,   810,  -405,  -390,   267,   349,  -405,
     290,   255,   767,   335,   349,   -64,   355,   164,  -413,   353,
     411,   969,   355,   488,   492,   493,   358,   698,  1086,   811,
    1087,  1088,  -405,   255,  1089,  1090,   555,   556,   699,   336,
     513,    74,   557,   294,   298,   558,   559,   536,   490,   519,
     574,   520,   521,   522,   524,   526,   527,   700,   528,   529,
     530,   531,   532,   631,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   547,   548,   549,   550,   551,   552,   553,
     554,   905,   889,   616,   329,   537,   600,   282,   282,   594,
    -501,   408,  -500,   282,   598,   766,   282,   282,   282,   282,
     563,   565,   573,   648,   940,   390,   330,   391,   469,   890,
     409,   583,   460,   460,    23,   -86,   587,  -501,   588,  -500,
     411,   411,  1059,   -92,   332,   750,   753,   470,   982,  -484,
     686,   518,   586,   164,  -383,   278,   339,   602,   604,   683,
      23,   523,   525,   608,   610,   318,   -88,   615,   615,   687,
     619,   321,   621,   321,   -93,  -396,  -483,   402,    23,    55,
     989,    40,   770,  -383,   392,  -396,   340,   650,   393,   341,
      63,   652,   653,   691,   460,   622,  -383,   656,   460,   255,
     255,   255,   255,    53,   282,    55,   443,   562,   564,  -383,
     797,   799,  -361,   734,  -396,    83,    63,   737,   582,   656,
    -488,  -489,   282,    55,   282,   794,   411,   771,   675,   656,
     678,  -361,   680,   682,    63,   717,  -486,  -485,   772,   393,
    -396,    83,   278,   255,   396,   603,   574,  -396,   981,   255,
    -551,   609,  -551,   193,   394,   990,  -383,   773,  -548,    83,
    -548,  -383,   631,   401,   490,   412,   730,   407,  -396,   349,
     410,  -550,   255,  -550,   353,   417,  -487,   355,   421,   427,
     940,   358,   940,  -551,   449,  -551,   412,  -396,   463,   464,
     255,   862,  -396,   473,   483,  -396,  -549,   476,  -549,   485,
     507,   511,   514,   602,   720,   608,   723,   619,   725,   678,
     516,   355,   726,   727,   585,   674,   589,   282,   590,   282,
     591,   624,   599,   779,   140,   781,   597,   738,   605,   740,
     620,   412,   623,   573,   140,   747,   624,   940,   638,   940,
     625,   411,   648,   626,   639,   654,   616,   140,  1056,   140,
    1082,   660,   661,   665,   412,   625,   673,   679,   626,   692,
     703,   704,   705,   627,   706,   707,   628,   708,    57,   710,
     288,   292,   324,  1079,   709,   -60,   282,   629,   627,   686,
     754,   628,   757,   327,   756,   762,   775,  1092,   469,   784,
     255,   719,   629,   722,   789,   140,   630,  -385,   808,  1106,
    1107,  1108,  1109,  1110,  1111,   809,   812,   870,   864,   873,
     880,   630,   875,   345,   802,   882,   803,   615,   615,   902,
     255,   906,   140,  1079,   255,   908,  -385,   140,   911,   914,
     915,   920,   934,  -295,   925,   929,   935,   936,  -113,  -385,
    1115,   573,   345,  -119,   524,   563,   869,   991,   992,   872,
     945,  -118,  -385,   624,   412,   412,  -117,   -89,  1122,  -114,
    -295,  -121,  1079,   -87,   346,   918,  -122,  -116,   347,   886,
    -120,   888,   625,   -90,  -115,   626,   893,  -123,   949,   896,
     -91,   973,   962,   898,   950,  1051,   900,   976,  1057,   -86,
     140,   964,  1052,   346,   -88,   627,  1084,   347,   628,  -385,
      57,   995,   510,  1046,  -385,  1047,  1093,  1094,   196,   629,
     481,   152,  1113,   919,   278,   349,   350,   943,   351,   352,
     353,   152,   354,   355,   924,  1012,   928,   358,   630,   930,
    1008,   523,   562,   655,   152,   365,   152,   942,   904,  1013,
     412,   369,   370,   371,   349,   350,   765,   351,   352,   353,
    1119,   354,   355,   356,   944,   655,   358,  1074,   318,  1100,
     318,  1112,  1105,   364,   365,   655,   218,   368,   651,   140,
     369,   370,   371,   205,   731,     0,   140,     0,     0,     0,
       0,   372,   152,     0,   948,     0,   297,   297,     0,   255,
     255,     0,     0,     0,     0,   255,   255,     0,   886,   888,
       0,   893,   896,   924,   928,     0,     0,   761,     0,   152,
       0,     0,   966,   967,   152,   968,     0,   573,     0,   970,
     971,     0,     0,   972,   273,     0,     0,     0,     0,  -400,
       0,     0,     0,     0,     0,     0,   140,     0,     0,  -400,
     988,   678,   282,   282,     0,     0,     0,   791,   996,     0,
     997,   140,     0,   729,   999,   412,  1032,     0,     0,  1001,
     615,   615,     0,     0,   411,   411,     0,     0,  -400,     0,
       0,     0,     0,     0,     0,     0,     0,   152,     0,     0,
       0,     0,   966,  1014,  1015,   970,  1016,  1017,  1018,  1019,
       0,     0,     0,     0,  -400,     0,     0,     0,     0,     0,
       0,  -400,     0,     0,     0,   324,     0,     0,     0,     0,
    1034,     0,   255,   255,  1041,   134,     0,     0,     0,     0,
     345,     0,  -400,   140,   255,   134,   443,     0,     0,     0,
       0,     0,  1099,   443,     0,     0,   255,     0,   134,   255,
     134,  -400,     0,     0,     0,     0,  -400,     0,     0,  -400,
       0,   140,  1070,  1071,  1072,  1073,   152,   140,     0,     0,
       0,   345,     0,   152,   140,     0,     0,     0,     0,     0,
     345,   346,     0,   928,     0,   347,     0,  1095,  1096,   921,
       0,   324,     0,     0,     0,     0,   134,     0,     0,     0,
    1070,  1071,  1072,  1073,  1095,  1096,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1101,   678,
       0,  1104,   346,   134,     0,     0,   347,     0,   134,     0,
     140,   346,   349,   152,   140,   347,     0,   353,     0,   354,
     355,   145,   140,     0,   358,     0,     0,     0,   152,     0,
       0,   145,   365,     0,   678,     0,     0,     0,   928,     0,
     371,     0,     0,     0,   145,     0,   145,     0,     0,     0,
       0,     0,     0,   349,   350,     0,   351,   352,   353,     0,
     354,   355,   349,   350,     0,   358,   352,   353,     0,   354,
     355,   134,   364,   365,   358,     0,   368,     0,     0,   369,
     370,   371,   365,     0,   715,     0,     0,  -402,   369,   370,
     371,     0,   145,     0,   255,   149,     0,  -402,     0,     0,
     152,     0,     0,     0,     0,   149,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   149,   145,
     149,     0,     0,     0,   145,     0,  -402,     0,   152,     0,
       0,     0,   140,     0,   152,     0,     0,     0,     0,     0,
       0,   152,     0,   443,   443,     0,     0,   443,   443,     0,
     134,     0,  -402,     0,     0,     0,     0,   134,     0,  -402,
     979,     0,     0,  1042,     0,     0,   149,     0,   412,   412,
       0,  1048,     0,   443,     0,   443,     0,     0,     0,     0,
    -402,     0,     0,     0,     0,   140,     0,   145,     0,     0,
       0,   140,     0,   149,     0,     0,     0,   152,   149,  -402,
     150,   152,     0,     0,  -402,     0,     0,  -402,     0,   152,
     150,     0,     0,     0,   863,     0,     0,   134,     0,     0,
       0,     0,     0,   150,     0,   150,     0,     0,   867,   868,
       0,     0,   134,     0,     0,     0,  1033,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   140,     0,   140,     0,     0,     0,     0,
     140,   149,     0,   140,     0,     0,   145,     0,     0,   140,
       0,   150,   140,   145,     0,     0,  1042,     0,     0,     0,
       0,   806,     0,     0,     0,  1076,     0,     0,   345,     0,
       0,     0,     0,     0,     0,     0,     0,   140,   150,     0,
       0,     0,  -225,   150,   134,     0,  -225,  -225,   140,     0,
     140,     0,     0,     0,     0,  -225,     0,  -225,  -225,   152,
       0,     0,  -225,     0,     0,     0,     0,     0,  -225,     0,
       0,  -225,   134,   145,     0,  1076,     0,     0,   134,   346,
     149,     0,     0,   347,     0,   134,     0,   149,   145,     0,
    -225,     0,  -225,   345,  -225,     0,  -225,  -225,     0,  -225,
       0,  -225,     0,  -225,     0,     0,   150,     0,     0,     0,
       0,     0,   152,     0,  1076,     0,     0,     0,   152,     0,
       0,     0,  -225,     0,     0,  -225,     0,     0,  -225,     0,
     349,   350,     0,     0,     0,   353,   140,   354,   355,     0,
       0,   134,   358,   140,   346,   134,     0,   149,   347,     0,
     365,     0,     0,   134,   140,     0,   369,   370,   371,     0,
     145,     0,   149,     0,     0,     0,     0,     0,   140,     0,
       0,     0,     0,     0,     0,     0,     0,   140,  -225,     0,
     152,   140,   152,     0,  -225,   150,     0,   152,   145,     0,
     152,     0,   150,     0,   145,   349,   152,     0,     0,   152,
     353,   145,   354,   355,     0,     0,     0,   358,     0,     0,
       0,     0,     0,     0,     0,   365,     0,     0,     0,     5,
       0,   369,   370,   371,   152,   140,     0,     0,     0,     0,
       0,     0,     0,    15,   149,   152,    17,   152,     0,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   150,     0,     0,     0,     0,   145,     0,     0,
      37,   145,   149,   134,     0,     0,    41,   150,   149,   145,
       0,     0,     0,     0,    46,   149,   140,   140,   140,   140,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   106,    61,     0,     0,     0,
       0,   140,   140,     0,     0,   165,     0,     0,    71,     0,
       0,     0,    74,     0,     0,     0,   134,     0,   197,    80,
     200,     0,   134,   152,     0,    84,     0,     0,   154,     0,
     152,   149,     0,     0,     0,   149,     0,     0,   154,   150,
       0,   152,     0,   149,     0,     0,     0,     0,     0,     0,
       0,   154,     0,   154,     0,   152,     0,     0,     0,     0,
       0,     0,     0,     0,   152,     0,   291,   150,   152,     0,
       0,     0,     0,   150,     0,     0,     0,     0,     0,   145,
     150,     0,     0,     0,   134,     0,   134,     0,     0,     0,
       0,   134,     0,   106,   134,     0,     0,     0,   328,   154,
     134,   269,     0,   134,     0,    22,    23,     0,     0,     0,
       0,     0,   152,     0,   270,     0,    30,   271,     0,     0,
     247,    35,     0,     0,     0,     0,   154,    40,   134,     0,
    -239,   154,   145,     0,     0,     0,   150,     0,   145,   134,
     150,   134,     0,     0,     0,     0,     0,     0,   150,    53,
       0,    55,     0,   149,     0,     0,  -239,   301,     0,     0,
     272,   395,    63,   152,   152,   152,   152,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    79,     0,     0,    81,     0,   -63,    83,   152,   152,
       0,     0,     0,     0,   154,     0,     0,     0,   155,     0,
     145,     0,   145,     0,     0,     0,   149,   145,   155,   333,
     145,   334,   149,     0,     0,     0,   145,     0,     0,   145,
       0,   155,     0,   155,     0,     0,     0,   134,     0,     0,
       0,     0,     0,     0,   134,     0,     0,     0,     0,     0,
     448,     0,     0,     0,   145,   134,     0,   461,     0,     0,
       0,     0,     0,     0,     0,   145,     0,   145,   150,   134,
       0,     0,   269,     0,     0,     0,    22,    23,   134,   155,
       0,   301,   134,   154,   149,   270,   149,    30,   271,     0,
     154,   149,    35,     0,   149,     0,     0,   301,    40,   426,
     149,     0,     0,   149,     0,     0,   155,     0,     0,     0,
       0,   155,     0,     0,     0,     0,     0,   106,     0,     0,
      53,   150,    55,     0,     0,     0,   134,   150,   149,     0,
       0,   272,   106,    63,   285,     0,     0,     0,   453,   149,
       0,   149,     0,     0,     0,     0,     0,     0,     0,     0,
     154,     0,    79,   145,     0,    81,     0,     0,    83,     0,
     145,     0,     0,   468,     0,   154,     0,     0,     0,     0,
       0,   145,     0,     0,   155,     0,     0,   134,   134,   134,
     134,     0,     0,     0,     0,   145,     0,     0,     0,   150,
       0,   150,     0,     0,   145,     0,   150,     0,   145,   150,
       0,     0,   134,   134,   106,   150,     0,     0,   150,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   149,     0,     0,
       0,     0,   461,   150,   149,     0,     0,   154,   461,     0,
       0,     0,   145,     0,   150,   149,   150,     0,     0,     0,
       0,     0,     0,   155,     0,     0,     0,     0,     0,   149,
     155,     0,     0,     0,     0,   154,     0,     0,   149,     0,
       0,   154,   149,     0,     0,     0,     0,     0,   154,     0,
     428,   431,   433,   437,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   145,   145,   145,   145,     0,     0,     0,
       0,   672,     0,     0,     0,   677,   593,     0,     0,     0,
     596,     0,     0,   106,     0,     0,   149,     0,   145,   145,
     155,     0,     0,     0,   462,     0,     0,     0,     0,     0,
     465,     0,   150,     0,   154,   155,     0,     0,   154,   150,
     453,     0,     0,     0,     0,     0,   154,     0,     0,     0,
     150,     0,     0,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   150,     0,     0,   149,   149,   149,
     149,   486,     0,   150,     0,     0,     0,   150,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   149,   149,     0,     0,     0,     0,     0,   685,
       0,     0,     0,     0,   694,     0,     0,   155,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   150,     0,   759,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   155,     0,     0,     0,     0,
       0,   155,     0,     0,     0,     0,     0,     0,   155,     0,
       0,     0,     0,     0,     0,     0,   154,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   584,   150,   150,   150,   150,   795,     0,     0,     0,
     732,     0,   800,   735,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   150,   150,     0,
       0,   606,     0,     0,   155,   611,     0,     0,   155,   154,
       0,     0,     0,     0,     0,   154,   155,     0,     0,     0,
       0,     0,   769,     0,     0,     0,   453,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   223,     0,   461,     0,   461,     0,     0,   792,
       0,   461,     0,   301,   461,     0,   224,   225,  -508,   226,
     899,     0,     0,   901,   227,     0,     0,     0,     0,     0,
       0,   388,   228,     0,  -548,     0,  -548,   154,   229,   154,
       0,     0,     0,   230,   154,     0,   231,   154,   917,   232,
     858,     0,     0,   154,     0,     0,   154,   233,     0,   923,
       0,   927,     0,   234,   235,     0,     0,     0,     0,     0,
     236,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,   154,     0,     0,     0,     0,   155,     0,   238,   239,
       0,   240,   154,   241,   154,   242,     0,     0,   243,     0,
       0,     0,   244,   451,     0,   245,     0,     0,   246,     0,
       0,     0,     0,   903,     0,     0,   453,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     742,   744,     0,     0,     0,     0,   749,   752,   792,   155,
       0,     0,     0,     0,     0,   155,     0,   269,     0,     0,
       0,    22,    23,     0,     0,     0,     0,     0,   694,     0,
     270,     0,    30,   271,   452,   974,     0,    35,     0,     0,
       0,     0,     0,    40,     0,     0,     0,     0,     0,   993,
     154,     0,     0,     0,   947,     0,     0,   154,   998,     0,
       0,     0,  1000,     0,     0,    53,     0,    55,   154,    57,
       0,   937,     0,     0,   938,     0,   272,   155,    63,   155,
       0,     0,   154,     0,   155,     0,     0,   155,     0,     0,
       0,   154,     0,   155,     0,   154,   155,    79,     0,     0,
      81,     0,     0,    83,     0,     0,  1028,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   155,     0,   877,   878,     0,     0,     0,     0,     0,
       0,     0,   155,     0,   155,   883,     0,     0,     0,   154,
       0,     0,     0,     0,     0,     0,     0,   894,     0,     0,
     897,     0,     0,    98,     0,     0,     0,     0,     0,   939,
       0,     0,     0,     0,     0,     0,     0,     5,   858,     6,
       7,     8,   206,    10,    11,   207,     0,     0,   172,     0,
       0,    15,    16,     0,    17,     0,   173,     0,     0,    21,
     154,   154,   154,   154,     0,     0,     0,    28,     0,   174,
       0,     0,     0,    32,   175,   176,     0,   177,    37,     0,
       0,    39,     0,     0,    41,   154,   154,     0,    43,    44,
     155,     0,    46,    47,     0,    48,    49,   155,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,   155,    58,
       0,     0,     0,     0,    61,   208,     0,     0,     0,     0,
       0,     0,   155,    68,    69,    70,    71,    72,   179,     0,
      74,   155,     0,    76,     0,   155,   209,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,   155,
       0,     0,     0,     0,     0,   994,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   180,     0,   101,
       0,   181,     0,     0,   103,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     155,   155,   155,   155,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    -2,     4,     0,     5,     0,     6,     7,
       8,     9,    10,    11,     0,   155,   155,    12,    13,    14,
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
      97,    98,     0,     0,    99,     0,   100,     0,   101,     0,
     102,     0,     4,   103,     5,     0,     6,     7,     8,     9,
      10,    11,     0,  -576,     0,    12,    13,    14,    15,    16,
    -576,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,  -576,    27,    28,  -576,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,    38,     0,    39,    40,
       0,    41,  -239,     0,    42,    43,    44,     0,    45,    46,
      47,   -44,    48,    49,     0,    50,    51,    52,     0,     0,
       0,    53,    54,    55,    56,     0,    58,    59,  -239,   -44,
      60,    61,    62,  -576,    63,    64,    65,  -576,    66,    67,
      68,    69,    70,    71,    72,    73,     0,    74,    75,     0,
      76,    77,    78,    79,    80,     0,    81,    82,   -62,    83,
      84,     0,     0,    85,     0,    86,     0,     0,  -576,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -576,  -576,    90,  -576,  -576,  -576,
    -576,  -576,  -576,  -576,     0,  -576,  -576,  -576,  -576,  -576,
       0,  -576,  -576,  -576,  -576,  -576,  -576,  -576,  -576,    98,
    -576,  -576,  -576,     0,   100,  -576,   101,     0,   102,     0,
     311,  -576,     5,   274,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    38,     0,    39,    40,     0,    41,
    -239,     0,    42,    43,    44,     0,    45,    46,    47,   -44,
      48,    49,     0,    50,    51,    52,     0,     0,     0,    53,
      54,    55,    56,    57,    58,    59,  -239,   -44,    60,    61,
      62,     0,    63,    64,    65,     0,    66,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,     0,    81,    82,   -62,    83,    84,     0,
       0,    85,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,   275,    98,     0,     0,
      99,     0,   100,   312,   101,     0,   102,     0,     4,   103,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,    38,     0,    39,    40,     0,    41,  -239,     0,
      42,    43,    44,     0,    45,    46,    47,   -44,    48,    49,
       0,    50,    51,    52,     0,     0,     0,    53,    54,    55,
      56,    57,    58,    59,  -239,   -44,    60,    61,    62,     0,
      63,    64,    65,     0,    66,    67,    68,    69,    70,    71,
      72,    73,     0,    74,    75,     0,    76,    77,    78,    79,
      80,     0,    81,    82,   -62,    83,    84,     0,     0,    85,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,    98,     0,     0,    99,     0,
     100,   489,   101,     0,   102,     0,   508,   103,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
      38,     0,    39,    40,     0,    41,  -239,     0,    42,    43,
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
      95,    96,    97,    98,     0,     0,    99,     0,   100,   509,
     101,     0,   102,     0,   311,   103,     5,     0,     6,     7,
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
      97,    98,     0,     0,    99,     0,   100,   312,   101,     0,
     102,     0,     4,   103,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,    38,     0,    39,    40,
       0,    41,  -239,     0,    42,    43,    44,     0,    45,    46,
      47,   -44,    48,    49,     0,    50,    51,    52,     0,     0,
       0,    53,    54,    55,    56,    57,    58,    59,  -239,   -44,
      60,    61,    62,     0,    63,    64,    65,     0,    66,    67,
      68,    69,    70,    71,    72,    73,     0,    74,    75,     0,
      76,    77,    78,    79,    80,     0,    81,    82,   -62,    83,
      84,     0,     0,    85,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,    98,
       0,     0,    99,     0,   100,   684,   101,     0,   102,     0,
       4,   103,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    38,     0,    39,    40,     0,    41,
    -239,     0,    42,    43,    44,     0,    45,    46,    47,   -44,
      48,    49,     0,    50,    51,    52,     0,     0,     0,    53,
      54,    55,    56,   327,    58,    59,  -239,   -44,    60,    61,
      62,     0,    63,    64,    65,     0,    66,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,     0,    81,    82,   -62,    83,    84,     0,
       0,    85,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,    98,     0,     0,
      99,     0,   100,     0,   101,     0,   102,     0,     4,   103,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,    38,     0,    39,    40,     0,    41,  -239,     0,
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
     100,     0,   101,     0,   102,     0,     4,   103,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,   601,    37,
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
      95,    96,    97,    98,     0,     0,    99,     0,   100,     0,
     101,     0,   102,     0,     4,   103,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,   607,    37,    38,     0,
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
      97,    98,     0,     0,    99,     0,   100,     0,   101,     0,
     102,     0,     4,   103,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,   885,    37,    38,     0,    39,    40,
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
       0,     0,    99,     0,   100,     0,   101,     0,   102,     0,
       4,   103,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,   887,    37,    38,     0,    39,    40,     0,    41,
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
      99,     0,   100,     0,   101,     0,   102,     0,     4,   103,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
     892,    37,    38,     0,    39,    40,     0,    41,  -239,     0,
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
     100,     0,   101,     0,   102,     0,     4,   103,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,   895,    37,
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
      95,    96,    97,    98,     0,     0,    99,     0,   100,     0,
     101,     0,   102,     0,     4,   103,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,   916,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,    38,     0,
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
      97,    98,     0,     0,    99,     0,   100,     0,   101,     0,
     102,     0,     4,   103,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,   922,    37,    38,     0,    39,    40,
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
       0,     0,    99,     0,   100,     0,   101,     0,   102,     0,
       4,   103,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,   926,    37,    38,     0,    39,    40,     0,    41,
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
      99,     0,   100,     0,   101,     0,   102,     0,  1097,   103,
       5,   274,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   172,     0,     0,    15,    16,     0,    17,     0,   173,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   174,     0,     0,     0,    32,   175,   176,     0,
     177,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   178,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   179,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,   275,     0,     0,     0,    99,     0,
     180,     0,   101,     0,   181,  1098,   778,   103,     5,   274,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   172,
       0,     0,    15,    16,     0,    17,     0,   173,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     174,     0,     0,     0,    32,   175,   176,     0,   177,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   178,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   179,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,   275,     0,     0,     0,    99,     0,   180,     0,
     101,     0,   181,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   172,     0,     0,    15,    16,
       0,    17,     0,   173,     0,     0,    21,   217,    23,     0,
       0,     0,     0,     0,    28,     0,   174,     0,     0,     0,
      32,   175,   176,     0,   177,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,    55,    56,     0,    58,     0,     0,     0,
       0,    61,   178,     0,    63,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   179,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,    83,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   180,     0,   101,     0,   181,     0,
       0,   103,     5,   274,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   172,     0,     0,    15,    16,     0,    17,
       0,   173,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   174,     0,     0,     0,    32,   175,
     176,     0,   177,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     178,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   179,     0,    74,     0,     0,    76,   279,
     280,     0,    80,   316,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,   317,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,   275,     0,     0,     0,
      99,     0,   180,     0,   101,     0,   181,     0,     0,   103,
       5,   274,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   172,     0,     0,    15,    16,     0,    17,     0,   173,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   174,     0,     0,     0,    32,   175,   176,     0,
     177,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   178,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   179,     0,    74,     0,     0,    76,   279,   280,     0,
      80,   316,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,   275,     0,     0,     0,    99,     0,
     180,     0,   101,   668,   181,     0,     0,   103,     5,   274,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   172,
       0,     0,    15,    16,     0,    17,     0,   173,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     174,     0,     0,     0,    32,   175,   176,     0,   177,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   178,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   179,
       0,    74,     0,     0,    76,   279,   280,     0,    80,   316,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,   275,     0,     0,     0,    99,     0,   180,     0,
     101,   670,   181,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   172,     0,     0,    15,    16,
       0,    17,     0,   173,     0,     0,    21,     0,   566,     0,
       0,     0,     0,     0,    28,     0,   174,     0,     0,     0,
      32,   175,   176,     0,   177,    37,     0,   567,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   178,     0,   568,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   179,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,   569,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   180,     0,   101,     0,   181,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   172,     0,     0,    15,    16,     0,    17,     0,   173,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   174,     0,     0,     0,    32,   175,   176,     0,
     177,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   178,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   179,     0,    74,     0,     0,    76,   279,   280,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,   281,     0,     0,    99,     0,
     180,     0,   101,     0,   181,     0,     0,   103,     5,   274,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   172,
       0,     0,    15,    16,     0,    17,     0,   173,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     174,     0,     0,     0,    32,   175,   176,     0,   177,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   178,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   179,
       0,    74,     0,     0,    76,   279,   280,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,   275,     0,     0,     0,    99,     0,   180,     0,
     101,     0,   181,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   172,     0,     0,    15,    16,
       0,    17,     0,   173,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   174,     0,     0,     0,
      32,   175,   176,     0,   177,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,   780,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   178,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   179,     0,    74,     0,     0,
      76,   279,   280,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   180,     0,   101,     0,   181,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   172,     0,     0,    15,    16,     0,    17,     0,   173,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   174,     0,     0,     0,    32,   175,   176,     0,
     177,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,   211,
      56,     0,    58,     0,     0,     0,     0,    61,   178,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   179,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,   212,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     180,     0,   101,     0,   181,     0,     0,   103,     5,   274,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   172,
       0,     0,    15,    16,     0,    17,     0,   173,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     174,     0,     0,     0,    32,   175,   176,     0,   177,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   178,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   179,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
     212,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,   275,     0,     0,     0,    99,     0,   180,     0,
     101,     0,   181,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   172,     0,     0,    15,    16,
       0,    17,     0,   173,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   174,     0,     0,     0,
      32,   175,   176,     0,   177,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   178,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   179,     0,    74,     0,     0,
      76,   279,   280,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   180,     0,   101,     0,   181,     0,
       0,   103,     5,   274,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   172,     0,     0,    15,    16,     0,    17,
       0,   173,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   174,     0,     0,     0,    32,   175,
     176,     0,   177,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     178,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   179,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,   275,     0,     0,     0,
      99,     0,   180,   666,   101,     0,   181,     0,     0,   103,
       5,   274,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   172,     0,     0,    15,    16,     0,    17,     0,   173,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   174,     0,     0,     0,    32,   175,   176,     0,
     177,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   178,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   179,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,   275,     0,     0,     0,    99,     0,
     180,     0,   101,     0,   181,   676,     0,   103,     5,   274,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   172,
       0,     0,    15,    16,     0,    17,     0,   173,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     174,     0,     0,     0,    32,   175,   176,     0,   177,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   178,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   179,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,   275,     0,     0,     0,    99,     0,   180,     0,
     101,     0,   181,  1031,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   172,     0,     0,    15,    16,
       0,    17,     0,   173,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   174,     0,     0,     0,
      32,   175,   176,     0,   177,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   178,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   179,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,   212,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   180,     0,   101,     0,   181,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   172,     0,     0,    15,    16,     0,    17,     0,   173,
       0,     0,    21,     0,     0,     0,     0,     0,     0,   254,
      28,     0,   174,     0,     0,     0,    32,   175,   176,     0,
     177,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   178,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   179,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     100,     0,   101,     0,   181,     0,     0,   103,     5,   274,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   172,
       0,     0,    15,    16,     0,    17,     0,   173,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     174,     0,     0,     0,    32,   175,   176,     0,   177,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   178,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   179,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,   275,     0,     0,     0,    99,     0,   180,     0,
     101,     0,   181,     0,     5,   103,     6,     7,     8,   206,
      10,    11,     0,     0,     0,   172,     0,     0,    15,    16,
       0,    17,     0,   173,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   174,     0,     0,     0,
      32,   175,   176,     0,   177,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   208,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   179,     0,    74,     0,     0,
      76,     0,     0,   209,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   180,     0,   101,     0,   181,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   172,     0,     0,    15,    16,     0,    17,     0,   173,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   174,     0,     0,     0,    32,   175,   176,     0,
     177,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   178,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   179,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     180,     0,   101,     0,   181,   681,     0,   103,     5,   274,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   172,
       0,     0,    15,    16,     0,    17,     0,   173,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,   640,     0,
     174,     0,     0,     0,    32,   175,   176,     0,   177,    37,
       0,     0,   641,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   178,     0,     0,     0,
       0,     0,     0,     0,    68,   642,    70,    71,    72,   643,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,   275,     0,     0,     0,    99,     0,   180,     0,
     101,     0,   977,     0,     0,   103,     5,   274,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   172,     0,     0,
      15,    16,     0,    17,     0,   173,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   174,     0,
       0,     0,    32,   175,   176,     0,   177,    37,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   178,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   179,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
     275,     0,     0,     0,    99,     0,   180,     0,   101,     0,
     977,     0,     0,   103,     5,   274,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   172,     0,     0,    15,    16,
       0,    17,     0,   173,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   174,     0,     0,     0,
      32,   175,   176,     0,   961,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   178,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   179,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,   275,     0,
       0,     0,    99,     0,   180,     0,   101,     0,   181,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   172,     0,     0,    15,    16,     0,    17,     0,   173,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   174,     0,     0,     0,    32,   175,   176,     0,
     177,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   178,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   179,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     180,     0,   101,     0,   181,     0,     5,   103,     6,     7,
       8,   202,    10,    11,     0,     0,     0,   172,     0,     0,
      15,    16,     0,    17,     0,   173,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   174,     0,
       0,     0,    32,   175,   176,     0,   177,    37,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   203,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   179,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   180,     0,   101,     0,
     181,     0,     5,   103,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   172,     0,     0,    15,    16,     0,    17,
       0,   173,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   174,     0,     0,     0,    32,   175,
     176,     0,   177,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     178,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   179,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   100,     0,   101,     0,   181,     0,     5,   103,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   172,
       0,     0,    15,    16,     0,    17,     0,   173,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     174,     0,     0,     0,    32,   175,   176,     0,   177,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   178,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   179,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,   612,     0,   180,     0,
     101,     0,   181,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   172,     0,     0,    15,    16,
       0,    17,     0,   173,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   640,     0,   174,     0,     0,     0,
      32,   175,   176,     0,   177,    37,     0,     0,   641,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   178,     0,     0,     0,     0,     0,     0,     0,
      68,   642,    70,    71,    72,   643,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   180,     0,   101,     0,   644,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   172,     0,     0,    15,    16,     0,    17,     0,   173,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   174,     0,     0,     0,    32,   175,   176,     0,
     177,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   178,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   179,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     180,     0,   101,     0,   644,     0,     5,   103,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   172,     0,     0,
      15,    16,     0,    17,     0,   173,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   174,     0,
       0,     0,    32,   175,   176,     0,   718,    37,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   178,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   179,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   180,     0,   101,     0,
     181,     0,     5,   103,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   172,     0,     0,    15,    16,     0,    17,
       0,   173,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   174,     0,     0,     0,    32,   175,
     176,     0,   721,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     178,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   179,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   180,     0,   101,     0,   181,     0,     5,   103,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   172,
       0,     0,    15,    16,     0,    17,     0,   173,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     174,     0,     0,     0,    32,   175,   176,     0,   955,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   178,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   179,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   180,     0,
     101,     0,   181,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   172,     0,     0,    15,    16,
       0,    17,     0,   173,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   174,     0,     0,     0,
      32,   175,   176,     0,   956,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   178,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   179,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   180,     0,   101,     0,   181,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   172,     0,     0,    15,    16,     0,    17,     0,   173,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   174,     0,     0,     0,    32,   175,   176,     0,
     958,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   178,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   179,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     180,     0,   101,     0,   181,     0,     5,   103,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   172,     0,     0,
      15,    16,     0,    17,     0,   173,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   174,     0,
       0,     0,    32,   175,   176,     0,   959,    37,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   178,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   179,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   180,     0,   101,     0,
     181,     0,     5,   103,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   172,     0,     0,    15,    16,     0,    17,
       0,   173,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   174,     0,     0,     0,    32,   175,
     176,     0,   960,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     178,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   179,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   180,     0,   101,     0,   181,     0,     5,   103,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   172,
       0,     0,    15,    16,     0,    17,     0,   173,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     174,     0,     0,     0,    32,   175,   176,     0,   961,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   178,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   179,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   180,     0,
     101,     0,   181,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   172,     0,     0,    15,    16,
       0,    17,     0,   173,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   640,     0,   174,     0,     0,     0,
      32,   175,   176,     0,   177,    37,     0,     0,   641,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   178,     0,     0,     0,     0,     0,     0,     0,
      68,   642,    70,    71,    72,   643,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   180,     0,   101,     0,  1053,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   172,     0,     0,    15,    16,     0,    17,     0,   173,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   174,     0,     0,     0,    32,   175,   176,     0,
     177,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   178,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   179,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   814,     0,   815,
      88,    89,    90,    91,    92,     0,     0,     0,   816,    93,
       0,     0,     0,   817,   225,   818,   819,     0,     0,    94,
       0,   820,    95,    96,    97,     0,     0,     0,    99,   228,
       0,     0,   101,     0,  1053,   821,     0,   103,     0,     0,
     822,     0,     0,   231,     0,     0,   823,     0,   824,     0,
       0,     0,     0,     0,   825,     0,     0,     0,     0,     0,
     826,   827,     0,     0,     0,     0,     0,   236,     0,     0,
       0,     0,     0,     0,     0,     0,   828,     0,     0,     0,
       0,     0,     0,     0,     0,   238,   239,     0,   829,     0,
     241,     0,   830,     0,     0,   831,     0,     0,     0,   832,
       0,     0,   245,     0,     0,   833,     0,     0,     0,     0,
       0,     0,     0,     0,   373,   374,   375,   376,   377,   378,
       0,     0,   381,   382,   383,   384,     0,   386,   387,   834,
     835,   836,   837,   838,     0,     0,   839,     0,     0,     0,
     840,   841,   842,   843,   844,   845,   846,   847,   848,   849,
     850,     0,   851,     0,     0,   852,   853,   854,   855,     0,
       5,   856,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   172,     0,     0,    15,    16,     0,    17,     0,   173,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   174,     0,     0,     0,    32,   175,   176,     0,
     177,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   178,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   179,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,   388,    96,    97,  -548,     0,  -548,    99,     0,
     180,     0,   101,     0,   181,     5,   274,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   172,     0,     0,    15,
      16,     0,    17,     0,   173,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   174,     0,     0,
       0,    32,   175,   176,     0,   177,    37,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   178,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   179,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,     0,
    -404,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
       0,  -404,     0,     0,     0,  -404,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   275,
       0,     0,     0,     0,     0,   180,     0,   101,  -404,   977,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   172,     0,     0,    15,    16,     0,    17,     0,   173,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   174,     0,     0,     0,    32,   175,   176,     0,
     177,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,  -386,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   178,     0,
       0,     0,  -386,     0,     0,     0,    68,    69,    70,    71,
      72,   179,     0,    74,     0,  -386,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,  -386,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     5,    90,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   172,     0,     0,    15,    16,     0,    17,     0,
     173,     0,     0,    21,    97,  -386,     0,     0,     0,     0,
    -386,    28,   101,   174,  1053,     0,     0,    32,   175,   176,
       0,   177,    37,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   178,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   179,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     5,    90,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   172,     0,     0,    15,    16,     0,    17,
       0,   173,     0,     0,    21,    97,     0,     0,     0,     0,
       0,   180,    28,   101,   174,   181,     0,     0,    32,   175,
     176,     0,   177,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     178,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   179,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     5,    90,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   172,     0,     0,    15,    16,     0,
      17,     0,   173,     0,     0,    21,    97,     0,     0,     0,
       0,     0,   180,    28,   101,   174,   644,     0,     0,    32,
     175,   176,     0,   177,    37,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   178,     0,   223,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   179,     0,    74,   224,   225,    76,
     226,     0,     0,    80,     0,   227,    82,     0,     0,    84,
       0,     0,     0,   228,     0,     0,     0,     0,     0,   229,
       0,     0,     0,     0,   230,     0,     0,   231,     0,     0,
     232,     0,     0,     0,     0,    90,     0,     0,   233,     0,
       0,   223,     0,     0,   234,   235,     0,     0,     0,     0,
       0,   236,     0,     0,     0,   224,   225,     0,   226,     0,
     237,     0,     0,   227,     0,   101,     0,   181,     0,   238,
     239,   228,   240,     0,   241,     0,   242,   229,   223,   243,
       0,     0,   230,   244,     0,   231,   245,     0,   232,   246,
       0,     0,   224,   225,     0,   226,   233,     0,     0,     0,
     227,     0,   234,   235,     0,     0,     0,     0,   228,   236,
       0,     0,     0,     0,   229,     0,     0,     0,   237,   230,
       0,     0,   231,     0,     0,   232,     0,   238,   239,     0,
     240,     0,   241,   233,   242,     0,     0,   243,     0,   234,
     235,   244,     0,   401,   245,   790,   236,   246,     0,     0,
       0,     0,     0,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,   238,   239,     0,   240,     0,   241,
       0,   242,     0,     0,   243,     0,  1010,     0,   244,     0,
       0,   245,     0,     0,   246,   816,     0,     0,     0,     0,
     224,   225,   818,   226,     0,     0,     0,     0,   227,     0,
       0,     0,     0,   250,     0,     0,   228,     0,     0,     0,
       0,     0,   821,     0,     0,     0,     0,   230,     0,     0,
     231,     0,     0,   232,     0,   824,     0,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,   826,   235,     0,
     768,     0,     0,     0,   236,     0,     0,     0,     0,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,   238,   239,     0,   240,   343,   241,     0,  1011,
       0,     0,   831,   344,     0,     0,   244,     0,     0,   245,
       0,     0,   246,     0,     0,   345,     0,     0,     0,     0,
       0,   373,   374,   375,   376,   377,   378,     0,     0,   381,
     382,   383,   384,     0,   386,   387,   834,   835,   836,   837,
     838,     0,     0,   839,     0,     0,     0,   840,   841,   842,
     843,   844,   845,   846,   847,   848,   849,   850,     0,   851,
       0,     0,   852,   853,   854,   855,   346,     0,     0,     0,
     347,     0,     0,     0,     0,     0,   343,     0,     0,   442,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,   348,   373,   374,   375,   376,   377,   378,     0,     0,
     381,   382,   383,   384,     0,   386,   387,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,     0,   357,   358,
     359,   360,   361,     0,   362,   363,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   346,   401,     0,     0,
     347,     0,     0,     0,   372,     0,   343,     0,     0,   884,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,   348,   373,   374,   375,   376,   377,   378,     0,     0,
     381,   382,   383,   384,     0,   386,   387,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,     0,   357,   358,
     359,   360,   361,     0,   362,   363,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   346,   401,     0,     0,
     347,     0,     0,     0,   372,     0,   343,     0,     0,   891,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,   348,   373,   374,   375,   376,   377,   378,     0,     0,
     381,   382,   383,   384,     0,   386,   387,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,     0,   357,   358,
     359,   360,   361,     0,   362,   363,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   346,   401,     0,     0,
     347,     0,     0,     0,   372,     0,   343,     0,     0,  1024,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,   348,   373,   374,   375,   376,   377,   378,     0,     0,
     381,   382,   383,   384,     0,   386,   387,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,     0,   357,   358,
     359,   360,   361,     0,   362,   363,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   346,   401,     0,     0,
     347,     0,     0,     0,   372,     0,   343,     0,     0,  1025,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,   348,   373,   374,   375,   376,   377,   378,     0,     0,
     381,   382,   383,   384,     0,   386,   387,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,     0,   357,   358,
     359,   360,   361,     0,   362,   363,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   346,   401,     0,     0,
     347,     0,     0,     0,   372,     0,   343,     0,     0,  1026,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,   348,   373,   374,   375,   376,   377,   378,     0,     0,
     381,   382,   383,   384,     0,   386,   387,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,     0,   357,   358,
     359,   360,   361,     0,   362,   363,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   346,   401,     0,     0,
     347,     0,     0,     0,   372,     0,   343,     0,     0,  1027,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,   348,   373,   374,   375,   376,   377,   378,     0,     0,
     381,   382,   383,   384,     0,   386,   387,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,     0,   357,   358,
     359,   360,   361,     0,   362,   363,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   346,   401,     0,     0,
     347,     0,     0,     0,   372,     0,   343,     0,     0,  1049,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,   348,   373,   374,   375,   376,   377,   378,     0,     0,
     381,   382,   383,   384,     0,   386,   387,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,     0,   357,   358,
     359,   360,   361,     0,   362,   363,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   346,   401,     0,     0,
     347,     0,     0,     0,   372,     0,     0,     0,     0,  1050,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,   348,   373,   374,   375,   376,   377,   378,   345,   435,
     381,   382,   383,   384,     0,   386,   387,   349,   350,     0,
     351,   352,   353,   436,   354,   355,   356,     0,   357,   358,
     359,   360,   361,     0,   362,   363,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   343,   401,     0,     0,
       0,     0,     0,   344,   372,     0,     0,     0,     0,   346,
       0,     0,     0,   347,     0,   345,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,   348,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     349,   350,     0,   351,   352,   353,   346,   354,   355,   356,
     347,   357,   358,   359,   360,   361,     0,   362,   363,   364,
     365,   366,   367,   368,     0,     0,   369,   370,   371,     0,
     401,   343,     0,     0,     0,     0,     0,   372,   344,     0,
       0,   348,     0,     0,     0,     0,     0,     0,     0,     0,
     345,     0,     0,     0,     0,   439,     0,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,     0,   357,   358,
     359,   360,   361,     0,   362,   363,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   440,   401,   343,   688,
       0,     0,     0,     0,   372,   344,     0,     0,     0,     0,
       0,   346,     0,     0,     0,   347,     0,   345,     0,     0,
       0,     0,   689,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   348,     0,     0,     0,
       0,     0,     0,   690,     0,     0,     0,     0,     0,     0,
       0,     0,   349,   350,     0,   351,   352,   353,   346,   354,
     355,   356,   347,   357,   358,   359,   360,   361,     0,   362,
     363,   364,   365,   366,   367,   368,     0,   441,   369,   370,
     371,   343,     0,     0,     0,     0,     0,     0,   344,   372,
       0,     0,     0,   348,     0,     0,     0,     0,     0,     0,
     345,   751,     0,     0,     0,     0,     0,     0,     0,   349,
     350,     0,   351,   352,   353,     0,   354,   355,   356,     0,
     357,   358,   359,   360,   361,     0,   362,   363,   364,   365,
     366,   367,   368,     0,     0,   369,   370,   371,     0,     0,
       0,     0,     0,     0,     0,     0,   372,     0,     0,     0,
       0,   346,     0,     0,     0,   347,     0,   343,     0,     0,
       0,     0,     0,     0,   344,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   345,     0,     0,     0,
       0,     0,   432,     0,     0,     0,   348,     0,     0,     0,
       0,   403,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   349,   350,     0,   351,   352,   353,     0,   354,
     355,   356,     0,   357,   358,   359,   360,   361,     0,   362,
     363,   364,   365,   366,   367,   368,     0,   346,   369,   370,
     371,   347,   401,   343,     0,     0,     0,     0,     0,   372,
     344,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   345,   254,     0,     0,     0,     0,   167,     0,
       0,     0,   348,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,     0,   357,
     358,   359,   360,   361,     0,   362,   363,   364,   365,   366,
     367,   368,     0,   346,   369,   370,   371,   347,     0,   343,
       0,     0,     0,     0,     0,   372,   344,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   345,   575,
       0,     0,     0,     0,     0,     0,     0,     0,   348,     0,
       0,     0,     0,   576,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   349,   350,     0,   351,   352,   353,
       0,   354,   355,   356,     0,   357,   358,   359,   360,   361,
       0,   362,   363,   364,   365,   366,   367,   368,     0,   346,
     369,   370,   371,   347,   401,   343,     0,     0,     0,     0,
       0,   372,   344,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   345,   577,     0,     0,     0,     0,
       0,     0,     0,     0,   348,     0,     0,     0,     0,   578,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     349,   350,     0,   351,   352,   353,     0,   354,   355,   356,
       0,   357,   358,   359,   360,   361,     0,   362,   363,   364,
     365,   366,   367,   368,     0,   346,   369,   370,   371,   347,
       0,   343,     0,     0,     0,     0,     0,   372,   344,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     345,   743,     0,     0,     0,     0,     0,     0,     0,     0,
     348,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,     0,   357,   358,   359,
     360,   361,     0,   362,   363,   364,   365,   366,   367,   368,
       0,   346,   369,   370,   371,   347,     0,   343,     0,     0,
       0,     0,     0,   372,   344,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   348,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   349,   350,     0,   351,   352,   353,     0,   354,
     355,   356,     0,   357,   358,   359,   360,   361,     0,   362,
     363,   364,   365,   366,   367,   368,     0,   346,   369,   370,
     371,   347,   401,     0,     0,     0,     0,   343,     0,   372,
     760,     0,     0,     0,   344,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   345,     0,     0,     0,
       0,     0,   348,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,     0,   357,
     358,   359,   360,   361,     0,   362,   363,   364,   365,   366,
     367,   368,     0,     0,   369,   370,   371,   346,   401,     0,
       0,   347,     0,   343,     0,   372,     0,     0,     0,     0,
     344,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   345,     0,     0,     0,     0,   932,   432,     0,
       0,     0,   348,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   933,   357,
     358,   359,   360,   361,     0,   362,   363,   364,   365,   366,
     367,   368,     0,   346,   369,   370,   371,   347,     0,   343,
       0,     0,     0,   798,     0,   372,   344,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   345,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   348,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   349,   350,     0,   351,   352,   353,
       0,   354,   355,   356,     0,   357,   358,   359,   360,   361,
       0,   362,   363,   364,   365,   366,   367,   368,     0,   346,
     369,   370,   371,   347,     0,   343,     0,     0,     0,     0,
       0,   372,   344,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   348,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     349,   350,     0,   351,   352,   353,     0,   354,   355,   356,
       0,   357,   358,   359,   360,   361,     0,   362,   363,   364,
     365,   366,   367,   368,     0,   346,   369,   370,   371,   347,
     474,     0,     0,     0,     0,   343,     0,   372,     0,     0,
       0,     0,   344,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   345,     0,     0,     0,     0,     0,
     348,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,     0,   357,   358,   359,
     360,   361,     0,   362,   363,   364,   365,   366,   367,   368,
       0,   478,   369,   370,   371,   346,     0,     0,     0,   347,
       0,     0,     0,   372,     0,   343,     0,     0,     0,     0,
       0,     0,   344,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   345,     0,     0,     0,     0,     0,
     348,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,     0,   357,   358,   359,
     360,   361,     0,   362,   363,   364,   365,   366,   367,   368,
       0,   480,   369,   370,   371,   346,     0,     0,     0,   347,
       0,     0,     0,   372,     0,   343,     0,     0,     0,     0,
       0,     0,   344,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   345,     0,     0,     0,     0,     0,
     348,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,     0,   357,   358,   359,
     360,   361,     0,   362,   363,   364,   365,   366,   367,   368,
       0,   482,   369,   370,   371,   346,     0,     0,     0,   347,
       0,     0,     0,   372,     0,   343,     0,     0,     0,     0,
       0,     0,   344,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   345,     0,     0,     0,     0,     0,
     348,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,     0,   357,   358,   359,
     360,   361,     0,   362,   363,   364,   365,   366,   367,   368,
       0,   487,   369,   370,   371,   346,     0,     0,     0,   347,
       0,   343,     0,   372,     0,     0,     0,     0,   344,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     345,     0,     0,     0,     0,     0,     0,     0,     0,   504,
     348,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,     0,   357,   358,   359,
     360,   361,     0,   362,   363,   364,   365,   366,   367,   368,
       0,   346,   369,   370,   371,   347,     0,     0,     0,     0,
       0,   343,     0,   372,   579,     0,     0,     0,   344,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     345,     0,     0,     0,     0,     0,   348,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   349,   350,     0,   351,   352,   353,     0,   354,
     355,   356,     0,   357,   358,   359,   360,   361,     0,   362,
     363,   364,   365,   366,   367,   368,     0,     0,   369,   370,
     371,   346,     0,     0,     0,   347,     0,   343,     0,   372,
       0,     0,     0,     0,   344,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   348,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   349,   350,     0,   351,   352,   353,     0,   354,
     355,   356,     0,   357,   358,   359,   360,   361,     0,   362,
     363,   364,   365,   366,   367,   368,     0,   346,   369,   370,
     371,   347,     0,   343,     0,   667,     0,     0,     0,   372,
     344,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   348,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,     0,   357,
     358,   359,   360,   361,  -509,   362,   363,   364,   365,   366,
     367,   368,     0,   346,   369,   370,   371,   347,     0,   343,
       0,     0,     0,     0,     0,   372,   344,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   345,     0,
       0,     0,     0,     0,   167,     0,     0,     0,   348,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   349,   350,     0,   351,   352,   353,
       0,   354,   355,   356,     0,   357,   358,   359,   360,   361,
       0,   362,   363,   364,   365,   366,   367,   368,     0,   346,
     369,   370,   371,   347,     0,     0,     0,     0,     0,   343,
     755,   372,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   345,     0,
       0,     0,     0,     0,   348,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     349,   350,     0,   351,   352,   353,     0,   354,   355,   356,
       0,   357,   358,   359,   360,   361,     0,   362,   363,   364,
     365,   366,   367,   368,     0,   728,   369,   370,   371,   346,
       0,     0,     0,   347,     0,   343,     0,   372,     0,     0,
       0,     0,   344,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   345,     0,     0,   758,     0,     0,
       0,     0,     0,     0,   348,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     349,   350,     0,   351,   352,   353,     0,   354,   355,   356,
       0,   357,   358,   359,   360,   361,     0,   362,   363,   364,
     365,   366,   367,   368,     0,   346,   369,   370,   371,   347,
       0,   343,     0,     0,     0,     0,     0,   372,   344,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     348,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,     0,   357,   358,   359,
     360,   361,     0,   362,   363,   364,   365,   366,   367,   368,
       0,   346,   369,   370,   371,   347,     0,   343,     0,     0,
       0,     0,     0,   372,   344,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   348,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   349,   350,     0,   351,   352,   353,     0,   354,
     355,   356,     0,   357,   358,   359,   360,   361,     0,   362,
     363,   364,   365,   366,   367,   368,     0,   346,   369,   370,
     371,   347,     0,   343,     0,   782,     0,     0,     0,   372,
     344,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   348,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,     0,   357,
     358,   359,   360,   361,     0,   362,   363,   364,   365,   366,
     367,   368,     0,   346,   369,   370,   371,   347,     0,   343,
       0,   783,     0,     0,     0,   372,   344,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   345,   871,
       0,     0,     0,     0,     0,     0,     0,   801,   348,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   349,   350,     0,   351,   352,   353,
       0,   354,   355,   356,     0,   357,   358,   359,   360,   361,
       0,   362,   363,   364,   365,   366,   367,   368,     0,   346,
     369,   370,   371,   347,     0,   343,     0,     0,     0,     0,
       0,   372,   344,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   345,   874,     0,     0,     0,     0,
       0,     0,     0,     0,   348,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     349,   350,     0,   351,   352,   353,     0,   354,   355,   356,
       0,   357,   358,   359,   360,   361,     0,   362,   363,   364,
     365,   366,   367,   368,     0,   346,   369,   370,   371,   347,
       0,   343,     0,     0,     0,     0,     0,   372,   344,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     348,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,     0,   357,   358,   359,
     360,   361,     0,   362,   363,   364,   365,   366,   367,   368,
       0,   346,   369,   370,   371,   347,     0,   343,   931,     0,
       0,     0,     0,   372,   344,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   348,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   349,   350,     0,   351,   352,   353,     0,   354,
     355,   356,     0,   357,   358,   359,   360,   361,     0,   362,
     363,   364,   365,   366,   367,   368,     0,   346,   369,   370,
     371,   347,     0,   343,     0,     0,     0,   876,     0,   372,
     344,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   348,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,     0,   357,
     358,   359,   360,   361,     0,   362,   363,   364,   365,   366,
     367,   368,     0,   346,   369,   370,   371,   347,     0,     0,
       0,     0,     0,   343,     0,   372,   954,     0,     0,     0,
     344,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   345,     0,     0,     0,     0,     0,   348,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   349,   350,     0,   351,   352,   353,
       0,   354,   355,   356,     0,   357,   358,   359,   360,   361,
       0,   362,   363,   364,   365,   366,   367,   368,     0,     0,
     369,   370,   371,   346,     0,     0,     0,   347,     0,     0,
       0,   372,     0,   343,     0,     0,   957,     0,     0,     0,
     344,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   345,     0,     0,     0,     0,     0,   348,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   349,   350,     0,   351,   352,   353,
       0,   354,   355,   356,     0,   357,   358,   359,   360,   361,
       0,   362,   363,   364,   365,   366,   367,   368,     0,     0,
     369,   370,   371,   346,     0,     0,     0,   347,     0,   343,
       0,   372,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   345,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   348,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   349,   350,     0,   351,   352,   353,
       0,   354,   355,   356,     0,   357,   358,   359,   360,   361,
       0,   362,   363,   364,   365,   366,   367,   368,     0,   346,
     369,   370,   371,   347,     0,   343,     0,  1009,     0,     0,
       0,   372,   344,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   348,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     349,   350,     0,   351,   352,   353,     0,   354,   355,   356,
       0,   357,   358,   359,   360,   361,     0,   362,   363,   364,
     365,   366,   367,   368,     0,   346,   369,   370,   371,   347,
       0,     0,     0,     0,     0,  1045,   343,   372,  1062,     0,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
     348,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,     0,   357,   358,   359,
     360,   361,     0,   362,   363,   364,   365,   366,   367,   368,
       0,     0,   369,   370,   371,     0,   346,     0,     0,     0,
     347,     0,     0,   372,     0,     0,   343,     0,     0,  1063,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,   348,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,     0,   357,   358,
     359,   360,   361,     0,   362,   363,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   346,     0,     0,     0,
     347,     0,     0,     0,   372,     0,   343,     0,     0,  1064,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,   348,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,     0,   357,   358,
     359,   360,   361,     0,   362,   363,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   346,     0,     0,     0,
     347,     0,     0,     0,   372,     0,   343,     0,     0,  1065,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,   348,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,     0,   357,   358,
     359,   360,   361,     0,   362,   363,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   346,     0,     0,     0,
     347,     0,     0,     0,   372,     0,   343,     0,     0,  1066,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,   348,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,     0,   357,   358,
     359,   360,   361,     0,   362,   363,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   346,     0,     0,     0,
     347,     0,     0,     0,   372,     0,     0,     0,     0,  1067,
       0,     0,     0,   343,     0,     0,     0,     0,     0,     0,
     344,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   348,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,  1091,   357,   358,
     359,   360,   361,     0,   362,   363,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,     0,     0,     0,     0,
       0,     0,     0,   346,   372,     0,     0,   347,     0,   343,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   345,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   348,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   349,   350,     0,   351,   352,   353,
       0,   354,   355,   356,     0,   357,   358,   359,   360,   361,
       0,   362,   363,   364,   365,   366,   367,   368,     0,   346,
     369,   370,   371,   347,     0,   343,     0,     0,     0,     0,
       0,   372,   344,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   348,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     349,   350,     0,   351,   352,   353,     0,   354,   355,   356,
       0,   357,   358,   359,   360,   361,     0,   362,   363,   364,
     365,   366,   367,   368,     0,   346,   369,   370,   371,   347,
       0,   343,     0,     0,     0,  1120,     0,   372,   344,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     348,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,     0,   357,   358,   359,
     360,   361,     0,   362,   363,   364,   365,   366,   367,   368,
       0,   716,   369,   370,   371,   347,     0,     0,     0,     0,
       0,     0,     0,   372,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   348,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   349,   350,     0,   351,   352,   353,     0,   354,
     355,   356,     0,   357,   358,   359,   360,   361,     0,   362,
     363,   364,   365,   366,   367,   368,     0,   269,   369,   370,
     371,    22,    23,     0,     0,     0,     0,     0,     0,   372,
     270,   269,    30,   271,     0,    22,    23,    35,     0,     0,
       0,     0,     0,    40,   270,     0,    30,   271,     0,     0,
       0,    35,     0,     0,     0,     0,     0,    40,     0,     0,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,   937,     0,     0,   938,     0,   272,     0,    63,    53,
       0,    55,     0,    57,     0,   937,     0,     0,   938,   269,
     272,     0,    63,    22,    23,     0,     0,    79,     0,     0,
      81,     0,   270,    83,    30,   271,     0,     0,     0,    35,
       0,    79,     0,     0,    81,    40,     0,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   345,     0,   269,
       0,     0,     0,    22,    23,     0,     0,    53,     0,    55,
       0,    57,   270,   937,    30,   271,   938,     0,   272,    35,
      63,     0,     0,    98,     0,    40,     0,     0,     0,  1021,
       0,     0,     0,     0,     0,     0,     0,    98,     0,    79,
       0,     0,    81,  1023,     0,    83,     0,    53,   346,    55,
       0,    57,   347,   937,   345,     0,   938,     0,   272,     0,
      63,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
       0,     0,    81,   348,     0,    83,     0,     0,     0,     0,
       0,     0,   345,     0,     0,    98,     0,     0,     0,   349,
     350,  1068,   351,   352,   353,   346,   354,   355,   356,   347,
     357,   358,   359,   360,     0,     0,   362,   363,   364,   365,
     366,   367,   368,     0,     0,   369,   370,   371,     0,     0,
     345,     0,     0,     0,     0,    98,   372,     0,     0,     0,
     348,  1069,     0,   346,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,     0,   357,   358,   359,
     360,     0,     0,   362,   363,   364,   365,   366,     0,   368,
       0,   346,   369,   370,   371,   347,     0,     0,     0,     0,
       0,     0,     0,   372,   349,   350,     0,   351,   352,   353,
       0,   354,   355,   356,     0,   357,   358,   359,   360,     0,
       0,   362,   363,   364,   365,   366,     0,   368,     0,     0,
     369,   370,   371,     0,     0,     0,     0,     0,     0,     0,
       0,   372,   349,   350,     0,   351,   352,   353,     0,   354,
     355,   356,     0,     0,   358,   359,   360,     0,     0,   362,
     363,   364,   365,     0,     0,   368,     0,   269,   369,   370,
     371,    22,    23,     0,     0,     0,     0,     0,     0,   372,
     270,     0,    30,   271,     0,     0,     0,    35,     0,     0,
       0,     0,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   693,     0,   223,     0,
       0,     0,     0,     0,     0,    53,     0,    55,     0,   327,
       0,   937,   224,   225,   938,   226,   272,     0,    63,     0,
     227,     0,     0,     0,     0,     0,     0,     0,   228,     0,
       0,     0,     0,     0,   229,  -220,     0,    79,     0,   230,
      81,     0,   231,    83,     0,   232,     0,     0,     0,  -220,
    -220,     0,  -220,   233,     0,     0,     0,  -220,     0,   234,
     235,     0,     0,     0,     0,  -220,   236,     0,     0,     0,
       0,  -220,     0,     0,     0,   237,  -220,     0,     0,  -220,
       0,     0,  -220,     0,   238,   239,     0,   240,     0,   241,
    -220,   242,     0,    98,   243,     0,  -220,  -220,   244,     0,
       0,   245,     0,  -220,   246,     0,     0,     0,     0,     0,
       0,     0,  -220,     0,     0,  -230,     0,     0,     0,     0,
       0,  -220,  -220,     0,  -220,     0,  -220,     0,  -220,  -230,
    -230,  -220,  -230,     0,     0,  -220,     0,  -230,  -220,     0,
       0,  -220,     0,     0,     0,  -230,     0,     0,     0,     0,
       0,  -230,   223,     0,     0,     0,  -230,     0,     0,  -230,
       0,     0,  -230,     0,     0,     0,   224,   225,     0,   226,
    -230,     0,     0,     0,   227,     0,  -230,  -230,     0,     0,
       0,     0,   228,  -230,     0,     0,     0,     0,   229,     0,
       0,     0,  -230,   230,     0,     0,   231,     0,     0,   232,
       0,  -230,  -230,     0,  -230,     0,  -230,   233,  -230,     0,
    -221,  -230,     0,   234,   235,  -230,     0,     0,  -230,     0,
     236,  -230,     0,     0,  -221,  -221,     0,  -221,     0,   237,
       0,     0,  -221,     0,     0,     0,     0,     0,   238,   239,
    -221,   240,     0,   241,     0,   242,  -221,  -152,   243,     0,
       0,  -221,   244,     0,  -221,   245,     0,  -221,   246,     0,
       0,  -152,  -152,     0,  -152,  -221,     0,     0,     0,  -152,
       0,  -221,  -221,     0,     0,     0,     0,  -152,  -221,     0,
       0,     0,     0,  -152,     0,     0,     0,  -221,  -152,     0,
       0,  -152,     0,     0,  -152,     0,  -221,  -221,     0,  -221,
       0,  -221,  -152,  -221,     0,  -148,  -221,     0,  -152,  -152,
    -221,     0,     0,  -221,     0,  -152,  -221,     0,     0,  -148,
    -148,     0,  -148,     0,  -152,     0,     0,  -148,     0,     0,
       0,     0,     0,  -152,  -152,  -148,  -152,     0,  -152,     0,
    -152,  -148,     0,  -152,     0,     0,  -148,  -152,     0,  -148,
    -152,     0,  -148,  -152,     0,     0,     0,     0,     0,     0,
    -148,     0,     0,     0,     0,     0,  -148,  -148,     0,     0,
       0,     0,     0,  -148,     0,     0,     0,     0,     0,     0,
       0,     0,  -148,     0,     0,     0,     0,     0,     0,     0,
       0,  -148,  -148,     0,  -148,     0,  -148,     0,  -148,     0,
       0,  -148,     0,     0,     0,  -148,     0,     0,  -148,     0,
       0,  -148
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1015)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      12,   152,    65,    64,    16,   332,    60,   777,    20,    19,
     483,   394,   216,   193,    26,   222,     2,   605,    30,    31,
     690,    33,    34,    35,    36,   702,   449,   689,    46,   209,
      22,     1,   987,    45,     3,    47,     1,    85,   101,   807,
      52,   102,    54,    46,    56,   452,    13,    32,    38,     1,
      32,   906,    64,    65,    66,    67,    68,    69,   639,   457,
     267,    73,    68,    75,    18,    77,    78,  1081,   861,   862,
      82,    32,    32,    85,    86,     1,    88,    85,    90,  1031,
      83,    93,    94,   290,    96,   483,   113,   138,   100,   101,
     102,   103,    32,   113,    46,   113,   866,   593,    68,    56,
     596,    46,     0,   903,   110,    46,  1120,    46,    33,    34,
     158,    46,   158,   440,     3,   163,    46,    71,   322,   323,
     181,   139,    46,     3,     1,   713,   113,    96,   113,  1081,
      56,   113,   139,    56,   715,    89,   148,   907,    83,  1098,
     110,   168,   880,   194,   882,  1100,   297,   494,   168,   496,
     168,  1006,   106,   118,   111,   163,     3,  1116,   164,    85,
     172,   168,    85,   175,   176,   177,   118,   179,   180,   181,
      77,    78,   940,   138,   139,    98,   976,   102,   163,    56,
      83,   163,   192,   390,   391,   111,   138,   139,   111,   201,
     139,   158,    31,   774,   164,   963,    85,   965,   139,   139,
     139,   166,   163,   163,   139,   975,   158,    96,    85,   139,
     222,   201,   138,   205,   268,   139,    96,   166,   210,  1012,
    1013,    98,   605,   163,   962,   217,   964,   168,     3,   168,
     139,   638,   222,   168,   111,   158,   732,   163,   168,   735,
     163,   205,     8,    82,   111,    20,   210,    86,    60,   139,
     118,   165,  1020,   139,  1022,   267,    93,   166,    33,   271,
    1030,   138,    27,   933,   118,   139,    78,   279,   280,   476,
     932,   139,   139,   950,   165,   143,   166,   267,   290,   139,
     166,   158,   118,    58,   139,   139,   163,   165,   133,   143,
     165,    45,   166,    51,   133,   107,   141,   158,   166,   138,
     290,   889,   141,   164,   164,   317,   145,    72,   144,   164,
     146,   147,   166,    67,   150,   151,   379,   380,    83,    77,
     332,    96,   385,    77,    78,   388,   389,   102,   314,   341,
     713,   343,   344,   345,   346,   347,   348,   102,   350,   351,
     352,   353,   354,   766,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   768,   139,   690,   163,   355,   427,   379,   380,   420,
     139,   139,   139,   385,   425,   139,   388,   389,   390,   391,
     392,   393,   394,   118,   807,   165,    78,   167,   139,   166,
     158,   403,   139,   139,    27,   133,   408,   166,   410,   166,
     390,   391,   166,   141,   107,   609,   610,   158,   143,   158,
     139,   158,   158,   158,    56,   427,   157,   429,   430,   164,
      27,   346,   347,   435,   436,   101,   133,   439,   440,   158,
     442,   494,   444,   496,   141,    46,   158,   191,    27,    72,
     913,    48,    27,    85,    82,    56,   158,   459,    86,   149,
      83,   463,   464,   514,   139,   445,    98,   469,   139,   213,
     214,   215,   216,    70,   476,    72,   220,   392,   393,   111,
     674,   675,   139,   158,    85,   108,    83,   158,   403,   491,
     158,   158,   494,    72,   496,   665,   476,    72,   500,   501,
     502,   158,   504,   505,    83,    82,   158,   158,    83,    86,
     111,   108,   514,   257,   158,   430,   889,   118,   906,   263,
     165,   436,   167,   102,   165,   913,   158,   102,   165,   108,
     167,   163,   945,   163,   510,   201,   589,   158,   139,   133,
     112,   165,   286,   167,   138,    81,   158,   141,    81,   165,
     963,   145,   965,   165,   165,   167,   222,   158,   165,   165,
     304,   702,   163,   158,   118,   166,   165,   165,   167,   158,
       8,    60,   138,   575,   576,   577,   578,   579,   580,   581,
     163,   141,   582,   583,   158,   500,   118,   589,   166,   591,
     139,    27,    46,   644,     2,   648,   158,   599,   165,   601,
      34,   267,   166,   605,    12,   607,    27,  1020,   139,  1022,
      46,   591,   118,    49,   166,   166,   933,    25,  1006,    27,
    1036,   166,    21,   139,   290,    46,   168,   168,    49,   163,
     165,   158,   158,    69,   158,   158,    72,   158,    74,   168,
     642,   643,   644,  1031,   166,   158,   648,    83,    69,   139,
     158,    72,    34,    74,   158,   158,   166,  1045,   139,   164,
     404,   576,    83,   578,   166,    73,   102,    56,   164,  1085,
    1086,  1087,  1088,  1089,  1090,   118,    83,    32,   166,    32,
     163,   102,   168,    31,   686,   163,   688,   689,   690,    34,
     434,   138,   100,  1081,   438,    83,    85,   105,   168,   165,
     165,   138,   158,   139,   168,   168,   158,   164,   141,    98,
    1098,   713,    31,   141,   716,   717,   718,   914,   915,   721,
     165,   141,   111,    27,   390,   391,   141,   133,  1116,   141,
     166,   141,  1120,   133,    82,   786,   141,   141,    86,   741,
     141,   743,    46,   133,   141,    49,   748,   141,   133,   751,
     133,    34,   163,   755,   141,   158,   758,   166,    98,   133,
     168,   163,   158,    82,   133,    69,   139,    86,    72,   158,
      74,   166,   329,   166,   163,   166,   157,   157,    24,    83,
     298,     2,   168,   790,   786,   133,   134,   810,   136,   137,
     138,    12,   140,   141,   796,   950,   798,   145,   102,   801,
     945,   716,   717,   469,    25,   153,    27,   809,   766,   950,
     476,   159,   160,   161,   133,   134,   631,   136,   137,   138,
    1100,   140,   141,   142,   813,   491,   145,  1030,   494,  1058,
     496,  1091,  1084,   152,   153,   501,    35,   156,   460,   247,
     159,   160,   161,    30,   591,    -1,   254,    -1,    -1,    -1,
      -1,   170,    73,    -1,   856,    -1,    77,    78,    -1,   603,
     604,    -1,    -1,    -1,    -1,   609,   610,    -1,   870,   871,
      -1,   873,   874,   875,   876,    -1,    -1,   621,    -1,   100,
      -1,    -1,   884,   885,   105,   887,    -1,   889,    -1,   891,
     892,    -1,    -1,   895,   938,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,   314,    -1,    -1,    56,
     912,   913,   914,   915,    -1,    -1,    -1,   661,   920,    -1,
     922,   329,    -1,   589,   926,   591,   977,    -1,    -1,   931,
     932,   933,    -1,    -1,   914,   915,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,    -1,
      -1,    -1,   954,   955,   956,   957,   958,   959,   960,   961,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,   977,    -1,    -1,    -1,    -1,
     982,    -1,   726,   727,   986,     2,    -1,    -1,    -1,    -1,
      31,    -1,   139,   401,   738,    12,   740,    -1,    -1,    -1,
      -1,    -1,  1053,   747,    -1,    -1,   750,    -1,    25,   753,
      27,   158,    -1,    -1,    -1,    -1,   163,    -1,    -1,   166,
      -1,   429,  1024,  1025,  1026,  1027,   247,   435,    -1,    -1,
      -1,    31,    -1,   254,   442,    -1,    -1,    -1,    -1,    -1,
      31,    82,    -1,  1045,    -1,    86,    -1,  1049,  1050,   793,
      -1,  1053,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,
    1062,  1063,  1064,  1065,  1066,  1067,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1080,  1081,
      -1,  1083,    82,   100,    -1,    -1,    86,    -1,   105,    -1,
     498,    82,   133,   314,   502,    86,    -1,   138,    -1,   140,
     141,     2,   510,    -1,   145,    -1,    -1,    -1,   329,    -1,
      -1,    12,   153,    -1,  1116,    -1,    -1,    -1,  1120,    -1,
     161,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   133,   134,    -1,   145,   137,   138,    -1,   140,
     141,   168,   152,   153,   145,    -1,   156,    -1,    -1,   159,
     160,   161,   153,    -1,   572,    -1,    -1,    46,   159,   160,
     161,    -1,    73,    -1,   918,     2,    -1,    56,    -1,    -1,
     401,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,   100,
      27,    -1,    -1,    -1,   105,    -1,    85,    -1,   429,    -1,
      -1,    -1,   620,    -1,   435,    -1,    -1,    -1,    -1,    -1,
      -1,   442,    -1,   967,   968,    -1,    -1,   971,   972,    -1,
     247,    -1,   111,    -1,    -1,    -1,    -1,   254,    -1,   118,
     906,    -1,    -1,   987,    -1,    -1,    73,    -1,   914,   915,
      -1,   995,    -1,   997,    -1,   999,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,   673,    -1,   168,    -1,    -1,
      -1,   679,    -1,   100,    -1,    -1,    -1,   498,   105,   158,
       2,   502,    -1,    -1,   163,    -1,    -1,   166,    -1,   510,
      12,    -1,    -1,    -1,   702,    -1,    -1,   314,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,   716,   717,
      -1,    -1,   329,    -1,    -1,    -1,   982,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   741,    -1,   743,    -1,    -1,    -1,    -1,
     748,   168,    -1,   751,    -1,    -1,   247,    -1,    -1,   757,
      -1,    73,   760,   254,    -1,    -1,  1100,    -1,    -1,    -1,
      -1,     1,    -1,    -1,    -1,  1031,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   785,   100,    -1,
      -1,    -1,    22,   105,   401,    -1,    26,    27,   796,    -1,
     798,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,   620,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      -1,    51,   429,   314,    -1,  1081,    -1,    -1,   435,    82,
     247,    -1,    -1,    86,    -1,   442,    -1,   254,   329,    -1,
      70,    -1,    72,    31,    74,    -1,    76,    77,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,   168,    -1,    -1,    -1,
      -1,    -1,   673,    -1,  1120,    -1,    -1,    -1,   679,    -1,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,
     133,   134,    -1,    -1,    -1,   138,   884,   140,   141,    -1,
      -1,   498,   145,   891,    82,   502,    -1,   314,    86,    -1,
     153,    -1,    -1,   510,   902,    -1,   159,   160,   161,    -1,
     401,    -1,   329,    -1,    -1,    -1,    -1,    -1,   916,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   925,   158,    -1,
     741,   929,   743,    -1,   164,   247,    -1,   748,   429,    -1,
     751,    -1,   254,    -1,   435,   133,   757,    -1,    -1,   760,
     138,   442,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,     3,
      -1,   159,   160,   161,   785,   973,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,   401,   796,    20,   798,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   314,    -1,    -1,    -1,    -1,   498,    -1,    -1,
      44,   502,   429,   620,    -1,    -1,    50,   329,   435,   510,
      -1,    -1,    -1,    -1,    58,   442,  1024,  1025,  1026,  1027,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     2,    80,    -1,    -1,    -1,
      -1,  1049,  1050,    -1,    -1,    12,    -1,    -1,    92,    -1,
      -1,    -1,    96,    -1,    -1,    -1,   673,    -1,    25,   103,
      27,    -1,   679,   884,    -1,   109,    -1,    -1,     2,    -1,
     891,   498,    -1,    -1,    -1,   502,    -1,    -1,    12,   401,
      -1,   902,    -1,   510,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,   916,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   925,    -1,    73,   429,   929,    -1,
      -1,    -1,    -1,   435,    -1,    -1,    -1,    -1,    -1,   620,
     442,    -1,    -1,    -1,   741,    -1,   743,    -1,    -1,    -1,
      -1,   748,    -1,   100,   751,    -1,    -1,    -1,   105,    73,
     757,    22,    -1,   760,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,   973,    -1,    35,    -1,    37,    38,    -1,    -1,
      42,    42,    -1,    -1,    -1,    -1,   100,    48,   785,    -1,
      51,   105,   673,    -1,    -1,    -1,   498,    -1,   679,   796,
     502,   798,    -1,    -1,    -1,    -1,    -1,    -1,   510,    70,
      -1,    72,    -1,   620,    -1,    -1,    77,    79,    -1,    -1,
      81,   168,    83,  1024,  1025,  1026,  1027,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,   107,   108,  1049,  1050,
      -1,    -1,    -1,    -1,   168,    -1,    -1,    -1,     2,    -1,
     741,    -1,   743,    -1,    -1,    -1,   673,   748,    12,   131,
     751,   133,   679,    -1,    -1,    -1,   757,    -1,    -1,   760,
      -1,    25,    -1,    27,    -1,    -1,    -1,   884,    -1,    -1,
      -1,    -1,    -1,    -1,   891,    -1,    -1,    -1,    -1,    -1,
     247,    -1,    -1,    -1,   785,   902,    -1,   254,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   796,    -1,   798,   620,   916,
      -1,    -1,    22,    -1,    -1,    -1,    26,    27,   925,    73,
      -1,   193,   929,   247,   741,    35,   743,    37,    38,    -1,
     254,   748,    42,    -1,   751,    -1,    -1,   209,    48,   211,
     757,    -1,    -1,   760,    -1,    -1,   100,    -1,    -1,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,   314,    -1,    -1,
      70,   673,    72,    -1,    -1,    -1,   973,   679,   785,    -1,
      -1,    81,   329,    83,    67,    -1,    -1,    -1,   250,   796,
      -1,   798,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     314,    -1,   102,   884,    -1,   105,    -1,    -1,   108,    -1,
     891,    -1,    -1,   275,    -1,   329,    -1,    -1,    -1,    -1,
      -1,   902,    -1,    -1,   168,    -1,    -1,  1024,  1025,  1026,
    1027,    -1,    -1,    -1,    -1,   916,    -1,    -1,    -1,   741,
      -1,   743,    -1,    -1,   925,    -1,   748,    -1,   929,   751,
      -1,    -1,  1049,  1050,   401,   757,    -1,    -1,   760,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   884,    -1,    -1,
      -1,    -1,   429,   785,   891,    -1,    -1,   401,   435,    -1,
      -1,    -1,   973,    -1,   796,   902,   798,    -1,    -1,    -1,
      -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,   916,
     254,    -1,    -1,    -1,    -1,   429,    -1,    -1,   925,    -1,
      -1,   435,   929,    -1,    -1,    -1,    -1,    -1,   442,    -1,
     213,   214,   215,   216,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1024,  1025,  1026,  1027,    -1,    -1,    -1,
      -1,   498,    -1,    -1,    -1,   502,   418,    -1,    -1,    -1,
     422,    -1,    -1,   510,    -1,    -1,   973,    -1,  1049,  1050,
     314,    -1,    -1,    -1,   257,    -1,    -1,    -1,    -1,    -1,
     263,    -1,   884,    -1,   498,   329,    -1,    -1,   502,   891,
     452,    -1,    -1,    -1,    -1,    -1,   510,    -1,    -1,    -1,
     902,    -1,    -1,   286,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   916,    -1,    -1,  1024,  1025,  1026,
    1027,   304,    -1,   925,    -1,    -1,    -1,   929,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1049,  1050,    -1,    -1,    -1,    -1,    -1,   511,
      -1,    -1,    -1,    -1,   516,    -1,    -1,   401,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   973,    -1,   620,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   429,    -1,    -1,    -1,    -1,
      -1,   435,    -1,    -1,    -1,    -1,    -1,    -1,   442,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   620,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   404,  1024,  1025,  1026,  1027,   673,    -1,    -1,    -1,
     592,    -1,   679,   595,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1049,  1050,    -1,
      -1,   434,    -1,    -1,   498,   438,    -1,    -1,   502,   673,
      -1,    -1,    -1,    -1,    -1,   679,   510,    -1,    -1,    -1,
      -1,    -1,   634,    -1,    -1,    -1,   638,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,     3,    -1,   741,    -1,   743,    -1,    -1,   661,
      -1,   748,    -1,   665,   751,    -1,    17,    18,   149,    20,
     757,    -1,    -1,   760,    25,    -1,    -1,    -1,    -1,    -1,
      -1,   162,    33,    -1,   165,    -1,   167,   741,    39,   743,
      -1,    -1,    -1,    44,   748,    -1,    47,   751,   785,    50,
     702,    -1,    -1,   757,    -1,    -1,   760,    58,    -1,   796,
      -1,   798,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      -1,   785,    -1,    -1,    -1,    -1,   620,    -1,    89,    90,
      -1,    92,   796,    94,   798,    96,    -1,    -1,    99,    -1,
      -1,    -1,   103,   104,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,   765,    -1,    -1,   768,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     603,   604,    -1,    -1,    -1,    -1,   609,   610,   790,   673,
      -1,    -1,    -1,    -1,    -1,   679,    -1,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,   810,    -1,
      35,    -1,    37,    38,   165,   902,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,   916,
     884,    -1,    -1,    -1,   836,    -1,    -1,   891,   925,    -1,
      -1,    -1,   929,    -1,    -1,    70,    -1,    72,   902,    74,
      -1,    76,    -1,    -1,    79,    -1,    81,   741,    83,   743,
      -1,    -1,   916,    -1,   748,    -1,    -1,   751,    -1,    -1,
      -1,   925,    -1,   757,    -1,   929,   760,   102,    -1,    -1,
     105,    -1,    -1,   108,    -1,    -1,   973,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   785,    -1,   726,   727,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   796,    -1,   798,   738,    -1,    -1,    -1,   973,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   750,    -1,    -1,
     753,    -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,   950,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
    1024,  1025,  1026,  1027,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    47,    -1,    -1,    50,  1049,  1050,    -1,    54,    55,
     884,    -1,    58,    59,    -1,    61,    62,   891,    64,    65,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    73,   902,    75,
      -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,   916,    89,    90,    91,    92,    93,    94,    -1,
      96,   925,    -1,    99,    -1,   929,   102,   103,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,   973,
      -1,    -1,    -1,    -1,    -1,   918,   152,    -1,    -1,   155,
     156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,
      -1,   167,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1024,  1025,  1026,  1027,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     0,     1,    -1,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,  1049,  1050,    14,    15,    16,
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
     157,   158,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,
     167,    -1,     1,   170,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    18,
      19,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      -1,    50,    51,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    62,    -1,    64,    65,    66,    -1,    -1,
      -1,    70,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    -1,    96,    97,    -1,
      99,   100,   101,   102,   103,    -1,   105,   106,   107,   108,
     109,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,   163,   164,   165,    -1,   167,    -1,
       1,   170,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    -1,    50,
      51,    -1,    53,    54,    55,    -1,    57,    58,    59,    60,
      61,    62,    -1,    64,    65,    66,    -1,    -1,    -1,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    -1,    83,    84,    85,    -1,    87,    88,    89,    90,
      91,    92,    93,    94,    -1,    96,    97,    -1,    99,   100,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
      -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,   158,    -1,    -1,
     161,    -1,   163,   164,   165,    -1,   167,    -1,     1,   170,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    -1,    50,    51,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    62,
      -1,    64,    65,    66,    -1,    -1,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    -1,
      83,    84,    85,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    -1,    96,    97,    -1,    99,   100,   101,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,    -1,   112,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,   158,    -1,    -1,   161,    -1,
     163,   164,   165,    -1,   167,    -1,     1,   170,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    -1,    50,    51,    -1,    53,    54,
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
     155,   156,   157,   158,    -1,    -1,   161,    -1,   163,   164,
     165,    -1,   167,    -1,     1,   170,     3,    -1,     5,     6,
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
     157,   158,    -1,    -1,   161,    -1,   163,   164,   165,    -1,
     167,    -1,     1,   170,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      -1,    50,    51,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    62,    -1,    64,    65,    66,    -1,    -1,
      -1,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    -1,    83,    84,    85,    -1,    87,    88,
      89,    90,    91,    92,    93,    94,    -1,    96,    97,    -1,
      99,   100,   101,   102,   103,    -1,   105,   106,   107,   108,
     109,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,   158,
      -1,    -1,   161,    -1,   163,   164,   165,    -1,   167,    -1,
       1,   170,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    -1,    50,
      51,    -1,    53,    54,    55,    -1,    57,    58,    59,    60,
      61,    62,    -1,    64,    65,    66,    -1,    -1,    -1,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    -1,    83,    84,    85,    -1,    87,    88,    89,    90,
      91,    92,    93,    94,    -1,    96,    97,    -1,    99,   100,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
      -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,   158,    -1,    -1,
     161,    -1,   163,    -1,   165,    -1,   167,    -1,     1,   170,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
     163,    -1,   165,    -1,   167,    -1,     1,   170,     3,    -1,
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
     155,   156,   157,   158,    -1,    -1,   161,    -1,   163,    -1,
     165,    -1,   167,    -1,     1,   170,     3,    -1,     5,     6,
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
     157,   158,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,
     167,    -1,     1,   170,     3,    -1,     5,     6,     7,     8,
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
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,
       1,   170,     3,    -1,     5,     6,     7,     8,     9,    10,
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
     161,    -1,   163,    -1,   165,    -1,   167,    -1,     1,   170,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
     163,    -1,   165,    -1,   167,    -1,     1,   170,     3,    -1,
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
     155,   156,   157,   158,    -1,    -1,   161,    -1,   163,    -1,
     165,    -1,   167,    -1,     1,   170,     3,    -1,     5,     6,
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
     157,   158,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,
     167,    -1,     1,   170,     3,    -1,     5,     6,     7,     8,
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
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,
       1,   170,     3,    -1,     5,     6,     7,     8,     9,    10,
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
     161,    -1,   163,    -1,   165,    -1,   167,    -1,     1,   170,
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
     163,    -1,   165,    -1,   167,   168,     1,   170,     3,     4,
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
     165,    -1,   167,    -1,     3,   170,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    26,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,
      -1,   170,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
      61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    96,    -1,    -1,    99,   100,
     101,    -1,   103,   104,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,
     161,    -1,   163,    -1,   165,    -1,   167,    -1,    -1,   170,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,   100,   101,    -1,
     103,   104,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,
     163,    -1,   165,   166,   167,    -1,    -1,   170,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,
      65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,
      75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    96,    -1,    -1,    99,   100,   101,    -1,   103,   104,
      -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
     155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,
     165,   166,   167,    -1,     3,   170,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,
       3,   170,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,   158,    -1,    -1,   161,    -1,
     163,    -1,   165,    -1,   167,    -1,    -1,   170,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,
      65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,
      75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    96,    -1,    -1,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
     155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,
     165,    -1,   167,    -1,     3,   170,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,
       3,   170,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,   115,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,
     163,    -1,   165,    -1,   167,    -1,    -1,   170,     3,     4,
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
     115,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
     155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,
     165,    -1,   167,    -1,     3,   170,     5,     6,     7,     8,
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
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,
      -1,   170,     3,     4,     5,     6,     7,     8,     9,    10,
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
     161,    -1,   163,   164,   165,    -1,   167,    -1,    -1,   170,
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
     163,    -1,   165,    -1,   167,   168,    -1,   170,     3,     4,
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
     165,    -1,   167,   168,     3,   170,     5,     6,     7,     8,
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
     109,    -1,    -1,    -1,    -1,    -1,   115,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,
       3,   170,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    32,
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
     163,    -1,   165,    -1,   167,    -1,    -1,   170,     3,     4,
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
     165,    -1,   167,    -1,     3,   170,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,    -1,    -1,   102,   103,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,
       3,   170,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
     163,    -1,   165,    -1,   167,   168,    -1,   170,     3,     4,
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
     165,    -1,   167,    -1,    -1,   170,     3,     4,     5,     6,
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
     167,    -1,    -1,   170,     3,     4,     5,     6,     7,     8,
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
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,
       3,   170,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
     163,    -1,   165,    -1,   167,    -1,     3,   170,     5,     6,
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
     167,    -1,     3,   170,     5,     6,     7,     8,     9,    10,
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
     161,    -1,   163,    -1,   165,    -1,   167,    -1,     3,   170,
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
     165,    -1,   167,    -1,     3,   170,     5,     6,     7,     8,
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
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,
       3,   170,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
     163,    -1,   165,    -1,   167,    -1,     3,   170,     5,     6,
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
     167,    -1,     3,   170,     5,     6,     7,     8,     9,    10,
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
     161,    -1,   163,    -1,   165,    -1,   167,    -1,     3,   170,
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
     165,    -1,   167,    -1,     3,   170,     5,     6,     7,     8,
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
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,
       3,   170,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
     163,    -1,   165,    -1,   167,    -1,     3,   170,     5,     6,
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
     167,    -1,     3,   170,     5,     6,     7,     8,     9,    10,
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
     161,    -1,   163,    -1,   165,    -1,   167,    -1,     3,   170,
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
     165,    -1,   167,    -1,     3,   170,     5,     6,     7,     8,
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
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,
       3,   170,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    12,   142,
      -1,    -1,    -1,    17,    18,    19,    20,    -1,    -1,   152,
      -1,    25,   155,   156,   157,    -1,    -1,    -1,   161,    33,
      -1,    -1,   165,    -1,   167,    39,    -1,   170,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    92,    -1,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,   120,   121,   122,   123,
      -1,    -1,   126,   127,   128,   129,    -1,   131,   132,   133,
     134,   135,   136,   137,    -1,    -1,   140,    -1,    -1,    -1,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,    -1,   156,    -1,    -1,   159,   160,   161,   162,    -1,
       3,   165,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   162,   156,   157,   165,    -1,   167,   161,    -1,
     163,    -1,   165,    -1,   167,     3,     4,     5,     6,     7,
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
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,    -1,    -1,    -1,   163,    -1,   165,   166,   167,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    56,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    98,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,   135,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,   157,   158,    -1,    -1,    -1,    -1,
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
      -1,   133,     3,   135,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,   157,    -1,    -1,    -1,    -1,
      -1,   163,    33,   165,    35,   167,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
      61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,   135,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,   157,    -1,    -1,    -1,
      -1,    -1,   163,    33,   165,    35,   167,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,     3,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    17,    18,    99,
      20,    -1,    -1,   103,    -1,    25,   106,    -1,    -1,   109,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    -1,   135,    -1,    -1,    58,    -1,
      -1,     3,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,
      80,    -1,    -1,    25,    -1,   165,    -1,   167,    -1,    89,
      90,    33,    92,    -1,    94,    -1,    96,    39,     3,    99,
      -1,    -1,    44,   103,    -1,    47,   106,    -1,    50,   109,
      -1,    -1,    17,    18,    -1,    20,    58,    -1,    -1,    -1,
      25,    -1,    64,    65,    -1,    -1,    -1,    -1,    33,    71,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    80,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    89,    90,    -1,
      92,    -1,    94,    58,    96,    -1,    -1,    99,    -1,    64,
      65,   103,    -1,   163,   106,   165,    71,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    -1,    92,    -1,    94,
      -1,    96,    -1,    -1,    99,    -1,     3,    -1,   103,    -1,
      -1,   106,    -1,    -1,   109,    12,    -1,    -1,    -1,    -1,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    64,    65,    -1,
     165,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    -1,    92,    12,    94,    -1,    96,
      -1,    -1,    99,    19,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   118,   119,   120,   121,   122,   123,    -1,    -1,   126,
     127,   128,   129,    -1,   131,   132,   133,   134,   135,   136,
     137,    -1,    -1,   140,    -1,    -1,    -1,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,   156,
      -1,    -1,   159,   160,   161,   162,    82,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    95,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
     126,   127,   128,   129,    -1,   131,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    82,   163,    -1,    -1,
      86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,    95,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
     126,   127,   128,   129,    -1,   131,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    82,   163,    -1,    -1,
      86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,    95,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
     126,   127,   128,   129,    -1,   131,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    82,   163,    -1,    -1,
      86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,    95,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
     126,   127,   128,   129,    -1,   131,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    82,   163,    -1,    -1,
      86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,    95,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
     126,   127,   128,   129,    -1,   131,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    82,   163,    -1,    -1,
      86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,    95,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
     126,   127,   128,   129,    -1,   131,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    82,   163,    -1,    -1,
      86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,    95,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
     126,   127,   128,   129,    -1,   131,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    82,   163,    -1,    -1,
      86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,    95,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
     126,   127,   128,   129,    -1,   131,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    82,   163,    -1,    -1,
      86,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,   117,   118,   119,   120,   121,   122,   123,    31,    32,
     126,   127,   128,   129,    -1,   131,   132,   133,   134,    -1,
     136,   137,   138,    46,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    12,   163,    -1,    -1,
      -1,    -1,    -1,    19,   170,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    -1,    86,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    82,   140,   141,   142,
      86,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,    -1,   159,   160,   161,    -1,
     163,    12,    -1,    -1,    -1,    -1,    -1,   170,    19,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    36,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    67,   163,    12,    13,
      -1,    -1,    -1,    -1,   170,    19,    -1,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    -1,    86,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    82,   140,
     141,   142,    86,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,   158,   159,   160,
     161,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,   170,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    -1,    86,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    82,   159,   160,
     161,    86,   163,    12,    -1,    -1,    -1,    -1,    -1,   170,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    82,   159,   160,   161,    86,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   170,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    82,
     159,   160,   161,    86,   163,    12,    -1,    -1,    -1,    -1,
      -1,   170,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,    82,   159,   160,   161,    86,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   170,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,    82,   159,   160,   161,    86,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   170,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    82,   159,   160,
     161,    86,   163,    -1,    -1,    -1,    -1,    12,    -1,   170,
      95,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    82,   163,    -1,
      -1,    86,    -1,    12,    -1,   170,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,   113,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    67,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    82,   159,   160,   161,    86,    -1,    12,
      -1,    -1,    -1,   168,    -1,   170,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    82,
     159,   160,   161,    86,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   170,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,    82,   159,   160,   161,    86,
     163,    -1,    -1,    -1,    -1,    12,    -1,   170,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,    82,    -1,    -1,    -1,    86,
      -1,    -1,    -1,   170,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,    82,    -1,    -1,    -1,    86,
      -1,    -1,    -1,   170,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,    82,    -1,    -1,    -1,    86,
      -1,    -1,    -1,   170,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,    82,    -1,    -1,    -1,    86,
      -1,    12,    -1,   170,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,    82,   159,   160,   161,    86,    -1,    -1,    -1,    -1,
      -1,    12,    -1,   170,    95,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    -1,   159,   160,
     161,    82,    -1,    -1,    -1,    86,    -1,    12,    -1,   170,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    82,   159,   160,
     161,    86,    -1,    12,    -1,   166,    -1,    -1,    -1,   170,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,    -1,    82,   159,   160,   161,    86,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   170,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    82,
     159,   160,   161,    86,    -1,    -1,    -1,    -1,    -1,    12,
      13,   170,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,    82,
      -1,    -1,    -1,    86,    -1,    12,    -1,   170,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,    82,   159,   160,   161,    86,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   170,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,    82,   159,   160,   161,    86,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   170,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    82,   159,   160,
     161,    86,    -1,    12,    -1,   166,    -1,    -1,    -1,   170,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    82,   159,   160,   161,    86,    -1,    12,
      -1,   166,    -1,    -1,    -1,   170,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    82,
     159,   160,   161,    86,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   170,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,    82,   159,   160,   161,    86,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   170,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,    82,   159,   160,   161,    86,    -1,    12,    13,    -1,
      -1,    -1,    -1,   170,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    82,   159,   160,
     161,    86,    -1,    12,    -1,    -1,    -1,   168,    -1,   170,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    82,   159,   160,   161,    86,    -1,    -1,
      -1,    -1,    -1,    12,    -1,   170,    95,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,    82,    -1,    -1,    -1,    86,    -1,    -1,
      -1,   170,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,    82,    -1,    -1,    -1,    86,    -1,    12,
      -1,   170,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    82,
     159,   160,   161,    86,    -1,    12,    -1,   166,    -1,    -1,
      -1,   170,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,    82,   159,   160,   161,    86,
      -1,    -1,    -1,    -1,    -1,   168,    12,   170,    95,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,    -1,   159,   160,   161,    -1,    82,    -1,    -1,    -1,
      86,    -1,    -1,   170,    -1,    -1,    12,    -1,    -1,    95,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    82,    -1,    -1,    -1,
      86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,    95,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    82,    -1,    -1,    -1,
      86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,    95,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    82,    -1,    -1,    -1,
      86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,    95,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    82,    -1,    -1,    -1,
      86,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    56,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,   170,    -1,    -1,    86,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    82,
     159,   160,   161,    86,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   170,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,    82,   159,   160,   161,    86,
      -1,    12,    -1,    -1,    -1,   168,    -1,   170,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,    82,   159,   160,   161,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    22,   159,   160,
     161,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      35,    22,    37,    38,    -1,    26,    27,    42,    -1,    -1,
      -1,    -1,    -1,    48,    35,    -1,    37,    38,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,
      -1,    76,    -1,    -1,    79,    -1,    81,    -1,    83,    70,
      -1,    72,    -1,    74,    -1,    76,    -1,    -1,    79,    22,
      81,    -1,    83,    26,    27,    -1,    -1,   102,    -1,    -1,
     105,    -1,    35,   108,    37,    38,    -1,    -1,    -1,    42,
      -1,   102,    -1,    -1,   105,    48,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,    70,    -1,    72,
      -1,    74,    35,    76,    37,    38,    79,    -1,    81,    42,
      83,    -1,    -1,   158,    -1,    48,    -1,    -1,    -1,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,   102,
      -1,    -1,   105,   164,    -1,   108,    -1,    70,    82,    72,
      -1,    74,    86,    76,    31,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,   105,   117,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,   158,    -1,    -1,    -1,   133,
     134,   164,   136,   137,   138,    82,   140,   141,   142,    86,
     144,   145,   146,   147,    -1,    -1,   150,   151,   152,   153,
     154,   155,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      31,    -1,    -1,    -1,    -1,   158,   170,    -1,    -1,    -1,
     117,   164,    -1,    82,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,    -1,    -1,   150,   151,   152,   153,   154,    -1,   156,
      -1,    82,   159,   160,   161,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,    -1,
      -1,   150,   151,   152,   153,   154,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,    -1,   145,   146,   147,    -1,    -1,   150,
     151,   152,   153,    -1,    -1,   156,    -1,    22,   159,   160,
     161,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      35,    -1,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,
      -1,    76,    17,    18,    79,    20,    81,    -1,    83,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,     3,    -1,   102,    -1,    44,
     105,    -1,    47,   108,    -1,    50,    -1,    -1,    -1,    17,
      18,    -1,    20,    58,    -1,    -1,    -1,    25,    -1,    64,
      65,    -1,    -1,    -1,    -1,    33,    71,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    80,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    89,    90,    -1,    92,    -1,    94,
      58,    96,    -1,   158,    99,    -1,    64,    65,   103,    -1,
      -1,   106,    -1,    71,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    89,    90,    -1,    92,    -1,    94,    -1,    96,    17,
      18,    99,    20,    -1,    -1,   103,    -1,    25,   106,    -1,
      -1,   109,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,     3,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    17,    18,    -1,    20,
      58,    -1,    -1,    -1,    25,    -1,    64,    65,    -1,    -1,
      -1,    -1,    33,    71,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    80,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    89,    90,    -1,    92,    -1,    94,    58,    96,    -1,
       3,    99,    -1,    64,    65,   103,    -1,    -1,   106,    -1,
      71,   109,    -1,    -1,    17,    18,    -1,    20,    -1,    80,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    89,    90,
      33,    92,    -1,    94,    -1,    96,    39,     3,    99,    -1,
      -1,    44,   103,    -1,    47,   106,    -1,    50,   109,    -1,
      -1,    17,    18,    -1,    20,    58,    -1,    -1,    -1,    25,
      -1,    64,    65,    -1,    -1,    -1,    -1,    33,    71,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    80,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    89,    90,    -1,    92,
      -1,    94,    58,    96,    -1,     3,    99,    -1,    64,    65,
     103,    -1,    -1,   106,    -1,    71,   109,    -1,    -1,    17,
      18,    -1,    20,    -1,    80,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    89,    90,    33,    92,    -1,    94,    -1,
      96,    39,    -1,    99,    -1,    -1,    44,   103,    -1,    47,
     106,    -1,    50,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    64,    65,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    -1,    92,    -1,    94,    -1,    96,    -1,
      -1,    99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,   109
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   175,   176,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,    35,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    47,
      48,    50,    53,    54,    55,    57,    58,    59,    61,    62,
      64,    65,    66,    70,    71,    72,    73,    74,    75,    76,
      79,    80,    81,    83,    84,    85,    87,    88,    89,    90,
      91,    92,    93,    94,    96,    97,    99,   100,   101,   102,
     103,   105,   106,   108,   109,   112,   114,   117,   133,   134,
     135,   136,   137,   142,   152,   155,   156,   157,   158,   161,
     163,   165,   167,   170,   177,   178,   179,   180,   181,   182,
     184,   189,   190,   191,   192,   193,   197,   199,   202,   203,
     205,   206,   207,   212,   213,   215,   216,   217,   221,   222,
     225,   226,   229,   230,   233,   236,   237,   262,   265,   266,
     286,   287,   288,   289,   290,   291,   292,   300,   301,   302,
     303,   304,   307,   308,   309,   310,   311,   312,   313,   315,
     316,   317,   318,   319,   158,   179,   304,   113,   293,   294,
       3,   194,    14,    22,    35,    40,    41,    43,    81,    94,
     163,   167,   233,   286,   291,   302,   303,   304,   307,   309,
     310,   293,   304,   102,   265,    83,   194,   179,   280,   304,
     179,   165,     8,    81,   304,   305,     8,    11,    81,   102,
     305,    72,   115,   214,   304,   214,   304,    26,   266,   304,
     304,   197,   165,     3,    17,    18,    20,    25,    33,    39,
      44,    47,    50,    58,    64,    65,    71,    80,    89,    90,
      92,    94,    96,    99,   103,   106,   109,   196,   198,   234,
     165,   196,   267,   268,    32,   184,   201,   304,   304,    18,
      71,    89,   106,   304,   304,   304,     8,   165,   204,    22,
      35,    38,    81,   205,     4,   157,   256,   279,   304,   100,
     101,   158,   304,   306,   304,   201,   304,   304,   304,    93,
     165,   179,   304,   304,   184,   193,   304,   307,   184,   193,
     304,   196,   263,   304,   304,   304,   304,   304,   304,   304,
     304,     1,   164,   177,   185,   279,   104,   143,   256,   281,
     282,   306,   214,   279,   304,   314,   304,    74,   179,   163,
      78,   183,   107,   196,   196,    51,    77,   251,   267,   157,
     158,   149,   304,    12,    19,    31,    82,    86,   117,   133,
     134,   136,   137,   138,   140,   141,   142,   144,   145,   146,
     147,   148,   150,   151,   152,   153,   154,   155,   156,   159,
     160,   161,   170,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   162,   243,
     165,   167,    82,    86,   165,   179,   158,   304,   304,   304,
     279,   163,   184,    46,   293,   263,   267,   158,   139,   158,
     112,   197,   256,   283,   284,   285,   306,    81,   210,   237,
     265,    81,   208,   263,   237,   265,   196,   165,   201,    32,
      46,   201,   113,   201,   296,    32,    46,   201,   296,    36,
      67,   158,    95,   184,   243,    13,   158,   283,   179,   165,
     245,   104,   165,   196,   269,   270,     1,   138,   274,    46,
     139,   179,   201,   165,   165,   201,   283,   205,   196,   139,
     158,   304,   304,   158,   163,   201,   165,   283,   158,   218,
     158,   218,   158,   118,   264,   158,   201,   158,   164,   164,
     177,   139,   164,   304,   139,   166,   139,   166,   168,   296,
      46,   139,   168,   296,   116,   139,   168,     8,     1,   164,
     185,    60,   186,   304,   138,   227,   163,   238,   158,   304,
     304,   304,   304,   214,   304,   214,   304,   304,   304,   304,
     304,   304,   304,    20,    33,    58,   102,   197,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   306,   306,   306,   306,   306,
     283,   283,   214,   304,   214,   304,    27,    46,    83,   108,
     295,   298,   299,   304,   319,    32,    46,    32,    46,    95,
      46,   168,   214,   304,   201,   158,   158,   304,   304,   118,
     166,   139,   211,   196,   267,   209,   196,   158,   267,    46,
     279,    43,   304,   214,   304,   165,   201,    43,   304,   214,
     304,   201,   161,   186,   188,   304,   186,   187,   179,   304,
      34,   304,   197,   166,    27,    46,    49,    69,    72,    83,
     102,   178,   246,   247,   248,   249,   235,   270,   139,   166,
      33,    47,    90,    94,   167,   200,   275,   287,   118,   271,
     304,   268,   304,   304,   166,   256,   304,     1,   223,   283,
     166,    21,   219,   275,   287,   139,   164,   166,   166,   281,
     166,   281,   179,   168,   214,   304,   168,   179,   304,   168,
     304,   168,   304,   164,   164,   196,   139,   158,    13,    36,
      67,   279,   163,     1,   196,   231,   232,    27,    72,    83,
     102,   240,   250,   165,   158,   158,   158,   158,   158,   166,
     168,    46,    83,   139,   166,   286,    82,    82,    43,   214,
     304,    43,   214,   304,   214,   304,   293,   293,   158,   256,
     306,   285,   196,   227,   158,   196,   227,   158,   304,   166,
     304,    32,   201,    32,   201,   297,   298,   304,    32,   201,
     296,    32,   201,   296,   158,    13,   158,    34,    34,   179,
      95,   184,   158,    46,    83,   248,   139,   166,   165,   196,
      27,    72,    83,   102,   252,   166,   270,   274,     1,   279,
      63,   306,   166,   166,   164,    68,   110,   164,   224,   166,
     165,   184,   196,   220,   263,   179,   168,   296,   168,   296,
     179,   116,   304,   304,   188,   187,     1,   228,   164,   118,
     139,   164,    83,   239,     1,     3,    12,    17,    19,    20,
      25,    39,    44,    50,    52,    58,    64,    65,    80,    92,
      96,    99,   103,   109,   133,   134,   135,   136,   137,   140,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   156,   159,   160,   161,   162,   165,   195,   196,   198,
     241,   242,   243,   286,   166,   298,   274,   286,   286,   304,
      32,    32,   304,    32,    32,   168,   168,   201,   201,   227,
     163,   227,   163,   201,    95,    43,   304,    43,   304,   139,
     166,    95,    43,   304,   201,    43,   304,   201,   304,   179,
     304,   179,    34,   196,   247,   270,   138,   278,    83,   274,
     271,   168,    46,   168,   165,   165,    32,   179,   279,   220,
     138,   184,    43,   179,   304,   168,    43,   179,   304,   168,
     304,    13,    36,    67,   158,   158,   164,    76,    79,   164,
     178,   203,   304,   232,   252,   165,   244,   196,   304,   133,
     141,   244,   244,   271,    95,    43,    43,    95,    43,    43,
      43,    43,   163,   228,   163,   228,   304,   304,   304,   298,
     304,   304,   304,    34,   179,   278,   166,   167,   200,   256,
     277,   287,   143,   257,   271,    56,   111,   258,   304,   275,
     287,   283,   283,   179,   201,   166,   304,   304,   179,   304,
     179,   304,   188,   187,   203,     1,   138,   273,   246,   166,
       3,    96,   242,   243,   304,   304,   304,   304,   304,   304,
     228,   164,   228,   164,    95,    95,    95,    95,   179,   271,
     278,   168,   279,   256,   304,     3,    85,    96,   259,   260,
     261,   304,   184,   202,   255,   168,   166,   166,   184,    95,
      95,   158,   158,   167,   200,   272,   287,    98,   253,   166,
     244,   244,    95,    95,    95,    95,    95,    95,   164,   164,
     304,   304,   304,   304,   257,   271,   256,   276,   277,   287,
      46,   168,   261,   111,   139,   118,   144,   146,   147,   150,
     151,    56,   287,   157,   157,   304,   304,     1,   168,   279,
     258,   304,   276,   277,   304,   260,   261,   261,   261,   261,
     261,   261,   259,   168,   272,   287,   168,   158,   254,   255,
     168,   272,   287,   276
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
#line 499 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 512 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 517 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 518 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 540 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 542 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 543 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 544 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 545 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 546 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 547 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 548 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 549 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 550 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 551 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 552 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 553 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 558 "chapel.ypp"
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
#line 571 "chapel.ypp"
    { (yyvsp[(1) - (3)].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (3)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (3)]).prevModule;  // restore previous module name
    }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = (yyvsp[(3) - (4)].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 588 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 589 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 590 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 594 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 595 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 609 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 610 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 615 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 616 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 621 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 626 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 631 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 635 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 651 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 660 "chapel.ypp"
    { (yyval.b) = (currentModuleType != MOD_INTERNAL ? true : false); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 661 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 662 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 666 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (4)].ponlylist), (yyvsp[(1) - (4)].b)); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 667 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), "", (yyvsp[(5) - (6)].ponlylist), true, (yyvsp[(1) - (6)].b)); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 668 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), (yyvsp[(5) - (8)].pexpr), (yyvsp[(7) - (8)].ponlylist), true, (yyvsp[(1) - (8)].b)); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 669 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), "", (yyvsp[(5) - (6)].ponlylist), false, (yyvsp[(1) - (6)].b)); }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 670 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), (yyvsp[(5) - (8)].pexpr), (yyvsp[(7) - (8)].ponlylist), false, (yyvsp[(1) - (8)].b)); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 674 "chapel.ypp"
    { (yyval.pblockstmt) = buildImportStmt(new UnresolvedSymExpr((yyvsp[(2) - (3)].pch))); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 676 "chapel.ypp"
    { (yyval.pblockstmt) = buildImportStmt(new UnresolvedSymExpr((yyvsp[(2) - (5)].pch)), (yyvsp[(4) - (5)].pch)); }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 680 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 685 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 687 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 704 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 705 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 706 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 707 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 708 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 709 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 712 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 713 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 714 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 715 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 716 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 717 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 729 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 730 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 742 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 743 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 744 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 745 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 748 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 749 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 758 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 759 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 763 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 764 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 765 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 766 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 767 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 768 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 770 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 771 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 772 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 773 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 780 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 781 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 782 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 783 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 787 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 788 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 792 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 793 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 797 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 799 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 809 "chapel.ypp"
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 810 "chapel.ypp"
    { parsingPrivate=(yyvsp[(2) - (3)].b); applyPrivateToBlock((yyvsp[(3) - (3)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(3) - (3)].pblockstmt); }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 824 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 825 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 826 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 827 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 832 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 837 "chapel.ypp"
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
#line 847 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 852 "chapel.ypp"
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
#line 863 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 868 "chapel.ypp"
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
#line 878 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 883 "chapel.ypp"
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
#line 894 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 898 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 903 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 911 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 922 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 928 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 929 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 930 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 931 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 932 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 933 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 934 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), true, false); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 935 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 936 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), true, false); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 937 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 938 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 939 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 940 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 941 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 942 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 943 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 944 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 945 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 947 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 953 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 959 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 965 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 971 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 978 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 985 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 989 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 995 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 999 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 1000 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 1001 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 1002 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1003 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 1005 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1007 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1009 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1014 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1017 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1018 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1022 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1026 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1031 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1032 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1033 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1037 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1038 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1042 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1046 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1048 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1052 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1053 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1058 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1060 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1062 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1069 "chapel.ypp"
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
#line 1079 "chapel.ypp"
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
#line 1092 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1097 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1102 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1111 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1116 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1118 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1120 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1125 "chapel.ypp"
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
#line 1141 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1148 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1157 "chapel.ypp"
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
#line 1165 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1169 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1175 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1176 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1181 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1186 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1191 "chapel.ypp"
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
#line 1213 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1216 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1220 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1228 "chapel.ypp"
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
#line 1238 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1245 "chapel.ypp"
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
#line 1271 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1275 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1280 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1284 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1289 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1307 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1308 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1309 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1312 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1313 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1314 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1317 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1318 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1319 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1320 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1321 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1322 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1323 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1324 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1325 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1326 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1327 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1331 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1332 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1333 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1335 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1336 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1337 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1338 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1340 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1341 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1342 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1346 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1347 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1351 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1356 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1357 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1364 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1366 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1370 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1374 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1375 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1379 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1380 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1381 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1382 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1383 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1384 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1386 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1387 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1391 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1392 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1393 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1394 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1395 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1396 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1400 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1401 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1405 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1406 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1407 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1408 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1409 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1410 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1414 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1415 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1418 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1424 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1428 "chapel.ypp"
    { 
      DefExpr * de = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch)));
      USR_WARN(de, "support for space in query expressions is deprecated");
      (yyval.pexpr) = de;
  }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1437 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1438 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1439 "chapel.ypp"
    { if (DefExpr* def = toDefExpr((yyvsp[(2) - (2)].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
                         }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1447 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1449 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1451 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1453 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1455 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1460 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1462 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1466 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1467 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1468 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1469 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1470 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1471 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1472 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1479 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1486 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1488 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1493 "chapel.ypp"
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
#line 1506 "chapel.ypp"
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
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1526 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1532 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1534 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1539 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1545 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1554 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1564 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1569 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1571 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1573 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1578 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1580 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1582 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1588 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1598 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1600 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1604 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1610 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1614 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1621 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1622 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1623 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1624 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1625 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1630 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1631 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1633 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1634 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1659 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1663 "chapel.ypp"
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
#line 1671 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1677 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1678 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1679 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1684 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1686 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1692 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1694 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1696 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1701 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1703 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1704 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1710 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1711 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1712 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1713 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1722 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1724 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1728 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1729 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1733 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1738 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1739 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1743 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1744 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1745 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1746 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1750 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1751 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1763 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1765 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1767 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1778 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1780 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1782 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1784 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1786 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1788 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1790 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1793 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1795 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1797 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1799 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1801 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1803 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1805 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1807 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1810 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1812 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1814 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1819 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1821 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1823 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1825 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1827 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1829 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1831 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1833 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1835 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1837 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1839 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1841 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1843 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1850 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1856 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1862 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1868 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1877 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1886 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1902 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1906 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1911 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1915 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1916 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1920 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1924 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1925 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1930 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1934 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1938 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1944 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1945 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1946 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1947 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1948 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1949 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1955 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1960 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1962 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1966 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1970 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1974 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1982 "chapel.ypp"
    { (yyvsp[(1) - (2)].pcallexpr)->insertAtTail((yyvsp[(2) - (2)].pexpr));
      (yyval.pexpr) = (yyvsp[(1) - (2)].pcallexpr); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1987 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1993 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1999 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 2006 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 2015 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtOwned->symbol, (yyvsp[(4) - (5)].pexpr))); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 2017 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtShared->symbol, (yyvsp[(4) - (5)].pexpr))); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 2022 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 2038 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 2040 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 2042 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 2044 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), false, true); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 2059 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 2061 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 2063 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr), true); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 2065 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 2069 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 2070 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 2074 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 2075 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 2076 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 2094 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 2095 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 2096 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr));}
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 2097 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 2101 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 2102 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 2103 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 2107 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 2108 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 2109 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 2110 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 2111 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 2119 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2120 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2121 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2122 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 2126 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2127 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 2132 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2133 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2134 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2135 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2136 "chapel.ypp"
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2137 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2138 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2139 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2140 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2141 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2142 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2144 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2148 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2155 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2156 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2160 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2161 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 584:
/* Line 1792 of yacc.c  */
#line 2162 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 585:
/* Line 1792 of yacc.c  */
#line 2163 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 586:
/* Line 1792 of yacc.c  */
#line 2164 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 587:
/* Line 1792 of yacc.c  */
#line 2165 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 588:
/* Line 1792 of yacc.c  */
#line 2166 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 589:
/* Line 1792 of yacc.c  */
#line 2167 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 590:
/* Line 1792 of yacc.c  */
#line 2168 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 591:
/* Line 1792 of yacc.c  */
#line 2169 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 592:
/* Line 1792 of yacc.c  */
#line 2170 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 593:
/* Line 1792 of yacc.c  */
#line 2171 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 594:
/* Line 1792 of yacc.c  */
#line 2172 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 595:
/* Line 1792 of yacc.c  */
#line 2173 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 596:
/* Line 1792 of yacc.c  */
#line 2174 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 597:
/* Line 1792 of yacc.c  */
#line 2175 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 598:
/* Line 1792 of yacc.c  */
#line 2176 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 599:
/* Line 1792 of yacc.c  */
#line 2177 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 600:
/* Line 1792 of yacc.c  */
#line 2178 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 601:
/* Line 1792 of yacc.c  */
#line 2179 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 602:
/* Line 1792 of yacc.c  */
#line 2180 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 603:
/* Line 1792 of yacc.c  */
#line 2181 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 604:
/* Line 1792 of yacc.c  */
#line 2182 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 605:
/* Line 1792 of yacc.c  */
#line 2186 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 606:
/* Line 1792 of yacc.c  */
#line 2187 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 607:
/* Line 1792 of yacc.c  */
#line 2188 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 608:
/* Line 1792 of yacc.c  */
#line 2189 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 609:
/* Line 1792 of yacc.c  */
#line 2190 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 610:
/* Line 1792 of yacc.c  */
#line 2191 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 611:
/* Line 1792 of yacc.c  */
#line 2192 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 612:
/* Line 1792 of yacc.c  */
#line 2196 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 613:
/* Line 1792 of yacc.c  */
#line 2197 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 614:
/* Line 1792 of yacc.c  */
#line 2198 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 615:
/* Line 1792 of yacc.c  */
#line 2199 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 616:
/* Line 1792 of yacc.c  */
#line 2203 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 617:
/* Line 1792 of yacc.c  */
#line 2204 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 618:
/* Line 1792 of yacc.c  */
#line 2205 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 619:
/* Line 1792 of yacc.c  */
#line 2206 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 620:
/* Line 1792 of yacc.c  */
#line 2211 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 621:
/* Line 1792 of yacc.c  */
#line 2212 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 622:
/* Line 1792 of yacc.c  */
#line 2213 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 623:
/* Line 1792 of yacc.c  */
#line 2214 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 624:
/* Line 1792 of yacc.c  */
#line 2215 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 625:
/* Line 1792 of yacc.c  */
#line 2216 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 626:
/* Line 1792 of yacc.c  */
#line 2217 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 10439 "bison-chapel.cpp"
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


