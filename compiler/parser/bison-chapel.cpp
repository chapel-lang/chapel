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
#define YYLAST   19177

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  174
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  146
/* YYNRULES -- Number of rules.  */
#define YYNRULES  627
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1128

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
     217,   222,   229,   237,   241,   246,   251,   256,   261,   266,
     267,   269,   271,   273,   275,   277,   279,   281,   283,   285,
     287,   289,   291,   293,   295,   297,   299,   301,   303,   305,
     307,   309,   311,   313,   315,   317,   319,   321,   323,   325,
     327,   329,   331,   333,   335,   337,   339,   341,   343,   345,
     347,   349,   351,   353,   355,   357,   359,   361,   363,   365,
     367,   370,   372,   375,   379,   381,   383,   386,   387,   391,
     393,   395,   397,   399,   401,   403,   405,   409,   415,   421,
     424,   425,   434,   435,   445,   446,   455,   456,   466,   470,
     474,   480,   486,   489,   495,   499,   506,   513,   518,   524,
     530,   534,   538,   545,   551,   558,   564,   571,   575,   580,
     584,   589,   596,   604,   611,   619,   624,   630,   635,   641,
     646,   651,   655,   662,   668,   675,   681,   690,   698,   701,
     705,   709,   712,   715,   719,   723,   724,   727,   730,   734,
     740,   742,   746,   750,   756,   762,   763,   766,   770,   773,
     777,   784,   791,   793,   795,   797,   798,   801,   802,   805,
     809,   815,   821,   823,   825,   828,   832,   834,   838,   839,
     840,   849,   850,   852,   854,   855,   856,   867,   871,   875,
     881,   887,   891,   893,   897,   899,   902,   904,   906,   908,
     910,   912,   914,   916,   918,   920,   922,   924,   926,   928,
     930,   932,   934,   936,   938,   940,   942,   944,   946,   948,
     950,   952,   955,   957,   959,   961,   963,   965,   967,   969,
     971,   973,   975,   977,   979,   980,   984,   988,   989,   991,
     995,  1000,  1006,  1011,  1018,  1025,  1026,  1028,  1030,  1032,
    1034,  1036,  1039,  1042,  1044,  1046,  1048,  1049,  1051,  1053,
    1056,  1058,  1060,  1062,  1064,  1065,  1067,  1070,  1072,  1074,
    1076,  1077,  1079,  1081,  1083,  1085,  1087,  1090,  1092,  1094,
    1097,  1100,  1101,  1104,  1107,  1112,  1117,  1119,  1123,  1127,
    1131,  1135,  1139,  1143,  1147,  1150,  1152,  1154,  1158,  1163,
    1168,  1171,  1176,  1177,  1180,  1183,  1185,  1187,  1189,  1192,
    1194,  1199,  1203,  1205,  1209,  1213,  1219,  1221,  1223,  1227,
    1229,  1232,  1236,  1237,  1240,  1243,  1247,  1250,  1255,  1259,
    1263,  1268,  1272,  1273,  1276,  1279,  1282,  1284,  1285,  1288,
    1291,  1294,  1296,  1301,  1306,  1313,  1317,  1318,  1320,  1322,
    1326,  1331,  1335,  1340,  1347,  1348,  1351,  1354,  1357,  1360,
    1362,  1364,  1368,  1372,  1374,  1378,  1380,  1382,  1384,  1388,
    1392,  1393,  1395,  1397,  1401,  1405,  1409,  1411,  1413,  1415,
    1417,  1419,  1422,  1424,  1426,  1428,  1430,  1432,  1434,  1437,
    1442,  1447,  1452,  1458,  1461,  1464,  1466,  1469,  1471,  1474,
    1476,  1479,  1481,  1484,  1486,  1488,  1495,  1502,  1507,  1517,
    1527,  1535,  1542,  1549,  1554,  1564,  1574,  1582,  1587,  1594,
    1601,  1611,  1621,  1628,  1630,  1632,  1634,  1636,  1638,  1640,
    1642,  1644,  1648,  1649,  1651,  1656,  1658,  1662,  1667,  1669,
    1673,  1678,  1682,  1686,  1688,  1690,  1693,  1695,  1698,  1700,
    1702,  1706,  1708,  1711,  1714,  1717,  1720,  1723,  1732,  1741,
    1751,  1761,  1767,  1773,  1778,  1780,  1782,  1784,  1786,  1788,
    1790,  1792,  1794,  1799,  1803,  1807,  1811,  1814,  1817,  1820,
    1822,  1823,  1825,  1828,  1831,  1833,  1835,  1837,  1839,  1841,
    1843,  1846,  1849,  1851,  1856,  1861,  1866,  1870,  1874,  1878,
    1882,  1888,  1892,  1897,  1901,  1906,  1908,  1910,  1912,  1914,
    1916,  1918,  1920,  1922,  1924,  1926,  1930,  1935,  1939,  1944,
    1948,  1953,  1957,  1963,  1967,  1971,  1975,  1979,  1983,  1987,
    1991,  1995,  1999,  2003,  2007,  2011,  2015,  2019,  2023,  2027,
    2031,  2035,  2039,  2043,  2047,  2051,  2055,  2058,  2061,  2064,
    2067,  2070,  2073,  2076,  2080,  2084,  2088,  2092,  2096,  2100,
    2104,  2108,  2110,  2112,  2114,  2116,  2118,  2120
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
     304,    13,   304,    67,   187,   158,    -1,   189,    45,   304,
     158,    -1,   189,    45,   304,    13,   197,   158,    -1,   189,
      45,   304,   141,   163,   186,   164,    -1,    84,   279,   158,
      -1,   307,   243,   306,   158,    -1,   307,   162,   306,   158,
      -1,   307,   130,   306,   158,    -1,   307,   124,   306,   158,
      -1,   307,   125,   306,   158,    -1,    -1,     3,    -1,     3,
      -1,    64,    -1,    96,    -1,    39,    -1,    99,    -1,   198,
      -1,     3,    -1,    64,    -1,    96,    -1,    39,    -1,    99,
      -1,   198,    -1,     3,    -1,    96,    -1,    17,    -1,    50,
      -1,   103,    -1,    80,    -1,    44,    -1,    25,    -1,    20,
      -1,    92,    -1,    94,    -1,    90,    -1,    71,    -1,    89,
      -1,    18,    -1,   106,    -1,    33,    -1,    47,    -1,    58,
      -1,    65,    -1,   109,    -1,    17,    -1,    35,    -1,    50,
      -1,   103,    -1,    80,    -1,    44,    -1,    25,    -1,    20,
      -1,    92,    -1,    58,    -1,    65,    -1,   109,    -1,    94,
      -1,    90,    -1,    33,    -1,    47,    -1,    32,   179,    -1,
     184,    -1,    85,   158,    -1,    85,   306,   158,    -1,   158,
      -1,   205,    -1,    79,   205,    -1,    -1,    76,   204,   205,
      -1,   237,    -1,   266,    -1,   229,    -1,   262,    -1,   225,
      -1,   206,    -1,   207,    -1,    42,   304,   158,    -1,    42,
     304,    36,   188,   158,    -1,    42,   304,    67,   187,   158,
      -1,    42,   266,    -1,    -1,    38,    81,   208,   196,   227,
     163,   228,   164,    -1,    -1,    38,     8,    81,   209,   196,
     227,   163,   228,   164,    -1,    -1,    37,    81,   210,   196,
     227,   163,   228,   164,    -1,    -1,    37,     8,    81,   211,
     196,   227,   163,   228,   164,    -1,    38,   305,   237,    -1,
      37,   305,   237,    -1,    38,   305,   265,   267,   158,    -1,
      37,   305,   265,   267,   158,    -1,    38,    11,    -1,    32,
     179,   112,   304,   158,    -1,   112,   304,   201,    -1,    24,
     304,    46,   304,   293,   201,    -1,    24,   304,    46,   214,
     293,   201,    -1,    24,   304,   293,   201,    -1,    40,   304,
      46,   304,   201,    -1,    40,   304,    46,   214,   201,    -1,
      40,   304,   201,    -1,    40,   214,   201,    -1,    40,    72,
     196,    46,   304,   201,    -1,    41,   304,    46,   304,   201,
      -1,    41,   304,    46,   304,   296,   201,    -1,    41,   304,
      46,   214,   201,    -1,    41,   304,    46,   214,   296,   201,
      -1,    41,   304,   201,    -1,    41,   304,   296,   201,    -1,
      41,   214,   201,    -1,    41,   214,   296,   201,    -1,   167,
     279,    46,   304,   168,   179,    -1,   167,   279,    46,   304,
     296,   168,   179,    -1,   167,   279,    46,   214,   168,   179,
      -1,   167,   279,    46,   214,   296,   168,   179,    -1,   167,
     279,   168,   179,    -1,   167,   279,   296,   168,   179,    -1,
     167,   214,   168,   179,    -1,   167,   214,   296,   168,   179,
      -1,   115,   165,   279,   166,    -1,    43,   304,    95,   179,
      -1,    43,   304,   184,    -1,    43,   304,    95,   179,    34,
     179,    -1,    43,   304,   184,    34,   179,    -1,    43,   304,
     243,   304,    95,   179,    -1,    43,   304,   243,   304,   184,
      -1,    43,   304,   243,   304,    95,   179,    34,   179,    -1,
      43,   304,   243,   304,   184,    34,   179,    -1,    29,   179,
      -1,   100,   304,   158,    -1,   101,   304,   158,    -1,   100,
     193,    -1,   101,   193,    -1,   100,   184,   218,    -1,   101,
     184,   218,    -1,    -1,   218,   219,    -1,    21,   184,    -1,
      21,   220,   184,    -1,    21,   165,   220,   166,   184,    -1,
     196,    -1,   196,   138,   304,    -1,    97,   304,   158,    -1,
      87,   304,   163,   223,   164,    -1,    87,   304,   163,     1,
     164,    -1,    -1,   223,   224,    -1,   110,   279,   201,    -1,
      68,   179,    -1,    68,    32,   179,    -1,   226,   196,   227,
     163,   228,   164,    -1,   226,   196,   227,   163,     1,   164,
      -1,    22,    -1,    81,    -1,   105,    -1,    -1,   138,   279,
      -1,    -1,   228,   203,    -1,   228,   178,   203,    -1,   230,
     196,   163,   231,   164,    -1,   230,   196,   163,     1,   164,
      -1,    35,    -1,   232,    -1,   231,   139,    -1,   231,   139,
     232,    -1,   196,    -1,   196,   118,   304,    -1,    -1,    -1,
      54,   234,   245,   235,   252,   274,   258,   255,    -1,    -1,
      48,    -1,    70,    -1,    -1,    -1,   236,   251,   238,   240,
     239,   252,   273,   253,   258,   254,    -1,   250,   242,   244,
      -1,   250,   243,   244,    -1,   250,   241,   141,   242,   244,
      -1,   250,   241,   141,   243,   244,    -1,   250,     1,   244,
      -1,   286,    -1,   165,   304,   166,    -1,   195,    -1,   135,
     196,    -1,   134,    -1,   136,    -1,   137,    -1,   135,    -1,
     144,    -1,   154,    -1,   151,    -1,   147,    -1,   150,    -1,
     146,    -1,   156,    -1,   152,    -1,   161,    -1,   140,    -1,
     159,    -1,   160,    -1,   153,    -1,   145,    -1,   133,    -1,
      19,    -1,   148,    -1,    12,    -1,   162,    -1,   149,    -1,
      52,    -1,   196,   133,    -1,   118,    -1,   129,    -1,   126,
      -1,   128,    -1,   122,    -1,   127,    -1,   123,    -1,   119,
      -1,   120,    -1,   121,    -1,   132,    -1,   131,    -1,    -1,
     165,   246,   166,    -1,   165,   246,   166,    -1,    -1,   247,
      -1,   246,   139,   247,    -1,   248,   196,   278,   271,    -1,
     178,   248,   196,   278,   271,    -1,   248,   196,   278,   257,
      -1,   248,   165,   270,   166,   278,   271,    -1,   248,   165,
     270,   166,   278,   257,    -1,    -1,   249,    -1,    46,    -1,
      49,    -1,    69,    -1,    27,    -1,    27,    46,    -1,    27,
      83,    -1,    72,    -1,    83,    -1,   102,    -1,    -1,    72,
      -1,    83,    -1,    27,    83,    -1,    27,    -1,   102,    -1,
      77,    -1,    51,    -1,    -1,    27,    -1,    27,    83,    -1,
      83,    -1,    72,    -1,   102,    -1,    -1,    98,    -1,   158,
      -1,   255,    -1,   184,    -1,   202,    -1,   157,   196,    -1,
       4,    -1,   143,    -1,   143,   304,    -1,   143,   256,    -1,
      -1,   111,   304,    -1,    56,   259,    -1,   111,   304,    56,
     259,    -1,    56,   259,   111,   304,    -1,   260,    -1,   259,
     139,   260,    -1,   261,   118,   261,    -1,   261,   150,   261,
      -1,   261,   151,   261,    -1,   261,   144,   261,    -1,   261,
     146,   261,    -1,   261,   147,   261,    -1,    85,   261,    -1,
       3,    -1,    96,    -1,   102,   263,   158,    -1,    26,   102,
     263,   158,    -1,    38,   102,   263,   158,    -1,   196,   264,
      -1,   196,   264,   139,   263,    -1,    -1,   118,   287,    -1,
     118,   275,    -1,    72,    -1,    27,    -1,    83,    -1,    27,
      83,    -1,   108,    -1,    26,   265,   267,   158,    -1,   265,
     267,   158,    -1,   268,    -1,   267,   139,   268,    -1,   196,
     274,   271,    -1,   165,   270,   166,   274,   271,    -1,   104,
      -1,   196,    -1,   165,   270,   166,    -1,   269,    -1,   269,
     139,    -1,   269,   139,   270,    -1,    -1,   118,    63,    -1,
     118,   306,    -1,   167,   168,   287,    -1,   167,   168,    -1,
     167,   279,   168,   287,    -1,   167,   279,   168,    -1,   167,
     168,   272,    -1,   167,   279,   168,   272,    -1,   167,     1,
     168,    -1,    -1,   138,   287,    -1,   138,   272,    -1,   138,
     200,    -1,     1,    -1,    -1,   138,   287,    -1,   138,   275,
      -1,   138,   200,    -1,     1,    -1,   167,   279,   168,   287,
      -1,   167,   279,   168,   275,    -1,   167,   279,    46,   304,
     168,   287,    -1,   167,     1,   168,    -1,    -1,   287,    -1,
     256,    -1,   167,   168,   276,    -1,   167,   279,   168,   276,
      -1,   167,   168,   277,    -1,   167,   279,   168,   277,    -1,
     167,   279,    46,   304,   168,   276,    -1,    -1,   138,   287,
      -1,   138,   256,    -1,   138,   200,    -1,   138,   277,    -1,
     304,    -1,   256,    -1,   279,   139,   304,    -1,   279,   139,
     256,    -1,   304,    -1,   280,   139,   304,    -1,   104,    -1,
     306,    -1,   256,    -1,   281,   139,   281,    -1,   282,   139,
     281,    -1,    -1,   284,    -1,   285,    -1,   284,   139,   285,
      -1,   197,   118,   256,    -1,   197,   118,   306,    -1,   256,
      -1,   306,    -1,   197,    -1,   199,    -1,   288,    -1,   288,
     157,    -1,   157,    -1,   291,    -1,   307,    -1,   290,    -1,
     316,    -1,   315,    -1,    90,   304,    -1,    47,   165,   283,
     166,    -1,    33,   165,   283,   166,    -1,    93,   165,   283,
     166,    -1,    91,    93,   165,   283,   166,    -1,    14,   304,
      -1,    94,   304,    -1,    71,    -1,    71,   304,    -1,   106,
      -1,   106,   304,    -1,    89,    -1,    89,   304,    -1,    18,
      -1,    18,   304,    -1,    22,    -1,    81,    -1,    40,   304,
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
     674,   675,   676,   680,   684,   686,   688,   690,   692,   699,
     700,   704,   705,   706,   707,   708,   709,   712,   713,   714,
     715,   716,   717,   729,   730,   741,   742,   743,   744,   745,
     746,   747,   748,   749,   750,   751,   752,   753,   754,   755,
     756,   757,   758,   759,   763,   764,   765,   766,   767,   768,
     769,   770,   771,   772,   773,   774,   781,   782,   783,   784,
     788,   789,   793,   794,   798,   799,   800,   810,   810,   815,
     816,   817,   818,   819,   820,   821,   825,   826,   827,   828,
     833,   832,   848,   847,   864,   863,   879,   878,   894,   898,
     903,   911,   922,   929,   930,   931,   932,   933,   934,   935,
     936,   937,   938,   939,   940,   941,   942,   943,   944,   945,
     946,   947,   953,   959,   965,   971,   978,   985,   989,   996,
    1000,  1001,  1002,  1003,  1004,  1006,  1008,  1010,  1015,  1018,
    1019,  1020,  1021,  1022,  1023,  1027,  1028,  1032,  1033,  1034,
    1038,  1039,  1043,  1046,  1048,  1053,  1054,  1058,  1060,  1062,
    1069,  1079,  1093,  1098,  1103,  1111,  1112,  1117,  1118,  1120,
    1125,  1141,  1148,  1157,  1165,  1169,  1176,  1177,  1182,  1187,
    1181,  1214,  1217,  1221,  1229,  1239,  1228,  1271,  1275,  1280,
    1284,  1289,  1296,  1297,  1301,  1302,  1303,  1304,  1305,  1306,
    1307,  1308,  1309,  1310,  1311,  1312,  1313,  1314,  1315,  1316,
    1317,  1318,  1319,  1320,  1321,  1322,  1323,  1324,  1325,  1326,
    1327,  1328,  1332,  1333,  1334,  1335,  1336,  1337,  1338,  1339,
    1340,  1341,  1342,  1343,  1347,  1348,  1352,  1356,  1357,  1358,
    1362,  1364,  1366,  1368,  1370,  1375,  1376,  1380,  1381,  1382,
    1383,  1384,  1385,  1386,  1387,  1388,  1392,  1393,  1394,  1395,
    1396,  1397,  1401,  1402,  1406,  1407,  1408,  1409,  1410,  1411,
    1415,  1416,  1419,  1420,  1424,  1425,  1429,  1434,  1438,  1439,
    1440,  1448,  1449,  1451,  1453,  1455,  1460,  1462,  1467,  1468,
    1469,  1470,  1471,  1472,  1473,  1477,  1479,  1484,  1486,  1488,
    1493,  1506,  1523,  1524,  1526,  1531,  1532,  1533,  1534,  1535,
    1539,  1545,  1553,  1554,  1562,  1564,  1569,  1571,  1573,  1578,
    1580,  1582,  1589,  1590,  1591,  1596,  1598,  1600,  1604,  1608,
    1610,  1614,  1622,  1623,  1624,  1625,  1626,  1631,  1632,  1633,
    1634,  1635,  1655,  1659,  1663,  1671,  1678,  1679,  1680,  1684,
    1686,  1692,  1694,  1696,  1701,  1702,  1703,  1704,  1705,  1711,
    1712,  1713,  1714,  1718,  1719,  1723,  1724,  1725,  1729,  1730,
    1734,  1735,  1739,  1740,  1744,  1745,  1746,  1747,  1751,  1752,
    1763,  1765,  1767,  1773,  1774,  1775,  1776,  1777,  1778,  1780,
    1782,  1784,  1786,  1788,  1790,  1793,  1795,  1797,  1799,  1801,
    1803,  1805,  1807,  1810,  1812,  1817,  1819,  1821,  1823,  1825,
    1827,  1829,  1831,  1833,  1835,  1837,  1839,  1841,  1848,  1854,
    1860,  1866,  1875,  1885,  1893,  1894,  1895,  1896,  1897,  1898,
    1899,  1900,  1905,  1906,  1910,  1914,  1915,  1919,  1923,  1924,
    1928,  1932,  1936,  1943,  1944,  1945,  1946,  1947,  1948,  1952,
    1953,  1958,  1960,  1964,  1968,  1972,  1980,  1985,  1991,  1997,
    2004,  2013,  2017,  2024,  2032,  2033,  2034,  2035,  2036,  2037,
    2038,  2039,  2040,  2042,  2044,  2046,  2061,  2063,  2065,  2067,
    2072,  2073,  2077,  2078,  2079,  2083,  2084,  2085,  2086,  2097,
    2098,  2099,  2100,  2104,  2105,  2106,  2110,  2111,  2112,  2113,
    2114,  2122,  2123,  2124,  2125,  2129,  2130,  2134,  2135,  2136,
    2137,  2138,  2139,  2140,  2141,  2142,  2143,  2144,  2145,  2146,
    2150,  2158,  2159,  2163,  2164,  2165,  2166,  2167,  2168,  2169,
    2170,  2171,  2172,  2173,  2174,  2175,  2176,  2177,  2178,  2179,
    2180,  2181,  2182,  2183,  2184,  2185,  2189,  2190,  2191,  2192,
    2193,  2194,  2195,  2199,  2200,  2201,  2202,  2206,  2207,  2208,
    2209,  2214,  2215,  2216,  2217,  2218,  2219,  2220
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
     191,   191,   191,   192,   193,   193,   193,   193,   193,   194,
     194,   195,   195,   195,   195,   195,   195,   196,   196,   196,
     196,   196,   196,   197,   197,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   200,   200,   200,   200,
     201,   201,   202,   202,   203,   203,   203,   204,   203,   205,
     205,   205,   205,   205,   205,   205,   206,   206,   206,   206,
     208,   207,   209,   207,   210,   207,   211,   207,   207,   207,
     207,   207,   212,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   214,
     215,   215,   215,   215,   215,   215,   215,   215,   216,   217,
     217,   217,   217,   217,   217,   218,   218,   219,   219,   219,
     220,   220,   221,   222,   222,   223,   223,   224,   224,   224,
     225,   225,   226,   226,   226,   227,   227,   228,   228,   228,
     229,   229,   230,   231,   231,   231,   232,   232,   234,   235,
     233,   236,   236,   236,   238,   239,   237,   240,   240,   240,
     240,   240,   241,   241,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   244,   244,   245,   246,   246,   246,
     247,   247,   247,   247,   247,   248,   248,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   250,   250,   250,   250,
     250,   250,   251,   251,   252,   252,   252,   252,   252,   252,
     253,   253,   254,   254,   255,   255,   256,   256,   257,   257,
     257,   258,   258,   258,   258,   258,   259,   259,   260,   260,
     260,   260,   260,   260,   260,   261,   261,   262,   262,   262,
     263,   263,   264,   264,   264,   265,   265,   265,   265,   265,
     266,   266,   267,   267,   268,   268,   269,   269,   269,   270,
     270,   270,   271,   271,   271,   272,   272,   272,   272,   272,
     272,   272,   273,   273,   273,   273,   273,   274,   274,   274,
     274,   274,   275,   275,   275,   275,   276,   276,   276,   277,
     277,   277,   277,   277,   278,   278,   278,   278,   278,   279,
     279,   279,   279,   280,   280,   281,   281,   281,   282,   282,
     283,   283,   284,   284,   285,   285,   285,   285,   286,   286,
     287,   287,   287,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   290,   291,   292,   292,   292,   292,   292,   292,
     292,   292,   293,   293,   294,   295,   295,   296,   297,   297,
     298,   298,   298,   299,   299,   299,   299,   299,   299,   300,
     300,   301,   301,   301,   301,   301,   302,   302,   302,   302,
     302,   302,   302,   303,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     305,   305,   306,   306,   306,   307,   307,   307,   307,   308,
     308,   308,   308,   309,   309,   309,   310,   310,   310,   310,
     310,   311,   311,   311,   311,   312,   312,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   314,   314,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   316,   316,   316,   316,
     316,   316,   316,   317,   317,   317,   317,   318,   318,   318,
     318,   319,   319,   319,   319,   319,   319,   319
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
       4,     6,     7,     3,     4,     4,     4,     4,     4,     0,
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
       4,     3,     6,     5,     6,     5,     8,     7,     2,     3,
       3,     2,     2,     3,     3,     0,     2,     2,     3,     5,
       1,     3,     3,     5,     5,     0,     2,     3,     2,     3,
       6,     6,     1,     1,     1,     0,     2,     0,     2,     3,
       5,     5,     1,     1,     2,     3,     1,     3,     0,     0,
       8,     0,     1,     1,     0,     0,    10,     3,     3,     5,
       5,     3,     1,     3,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     3,     3,     0,     1,     3,
       4,     5,     4,     6,     6,     0,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     0,     1,     1,     2,
       1,     1,     1,     1,     0,     1,     2,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     2,     1,     1,     2,
       2,     0,     2,     2,     4,     4,     1,     3,     3,     3,
       3,     3,     3,     3,     2,     1,     1,     3,     4,     4,
       2,     4,     0,     2,     2,     1,     1,     1,     2,     1,
       4,     3,     1,     3,     3,     5,     1,     1,     3,     1,
       2,     3,     0,     2,     2,     3,     2,     4,     3,     3,
       4,     3,     0,     2,     2,     2,     1,     0,     2,     2,
       2,     1,     4,     4,     6,     3,     0,     1,     1,     3,
       4,     3,     4,     6,     0,     2,     2,     2,     2,     1,
       1,     3,     3,     1,     3,     1,     1,     1,     3,     3,
       0,     1,     1,     3,     3,     3,     1,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     2,     4,
       4,     4,     5,     2,     2,     1,     2,     1,     2,     1,
       2,     1,     2,     1,     1,     6,     6,     4,     9,     9,
       7,     6,     6,     4,     9,     9,     7,     4,     6,     6,
       9,     9,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     0,     1,     4,     1,     3,     4,     1,     3,
       4,     3,     3,     1,     1,     2,     1,     2,     1,     1,
       3,     1,     2,     2,     2,     2,     2,     8,     8,     9,
       9,     5,     5,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     3,     3,     2,     2,     2,     1,
       0,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     2,     1,     4,     4,     4,     3,     3,     3,     3,
       5,     3,     4,     3,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     4,     3,     4,     3,
       4,     3,     5,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    93,   568,   569,   570,   571,
     572,   573,     0,   492,    79,   114,   461,   121,   463,   492,
       0,   120,     0,   366,    79,     0,     0,     0,     0,   115,
     540,   540,   565,     0,     0,     0,     0,   119,     0,   242,
     116,     0,   238,     0,     0,   123,     0,   511,   483,   574,
     124,     0,   243,   455,   365,     0,     0,     0,   137,    45,
     118,   464,   367,     0,     0,     0,     0,   459,     0,     0,
     122,     0,     0,    94,     0,   566,     0,     0,     0,   117,
     224,   457,   369,   125,     0,     0,   623,     0,   625,     0,
     626,   627,   539,     0,   624,   621,   442,   134,   622,     0,
       0,     0,     0,     4,     0,     5,     0,     9,    47,    10,
       0,    11,    12,    13,    15,   438,   439,    23,    14,   135,
     144,   145,    16,    18,    17,    20,    21,    22,    19,   143,
       0,   141,     0,   529,     0,   139,   142,     0,   140,   545,
     525,   440,   526,   445,   443,     0,     0,     0,   530,   531,
       0,   444,     0,   546,   547,   548,   567,   524,   447,   446,
     527,   528,     0,    39,    25,   453,     0,     0,   493,    80,
       0,     0,   463,   115,     0,     0,     0,   464,     0,     0,
       0,   529,   545,   443,   530,   531,   462,   444,   546,   547,
       0,   492,     0,     0,   368,     0,   198,     0,   423,     0,
     430,   571,   464,   541,   241,   571,   162,   464,     0,   241,
       0,     0,     0,     0,     0,     0,     0,   149,     0,     0,
     430,    87,    95,   107,   101,   100,   109,    90,    99,   110,
      96,   111,    88,   112,   105,    98,   106,   104,   102,   103,
      89,    91,    97,   108,   113,     0,    92,     0,     0,     0,
       0,   372,     0,   131,    33,     0,   608,   515,   512,   513,
     514,     0,   456,   609,     7,   430,   241,   222,   232,   540,
     223,   136,   337,   442,   420,     0,   419,     0,     0,   132,
     544,     0,     0,    36,     0,   460,   448,     0,   430,    37,
     454,     0,   205,   201,     0,   444,   205,   202,     0,   362,
       0,   458,     0,     0,   610,   612,   537,   607,   606,     0,
      49,    52,     0,     0,   425,     0,   427,     0,     0,   426,
       0,     0,   419,     0,   538,     0,     6,     0,    48,     0,
       0,     0,   225,     0,   323,   322,   244,     0,   441,    24,
       0,   516,     0,     0,     0,     0,     0,     0,   611,     0,
       0,     0,     0,     0,     0,   536,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   282,   289,   290,   291,   286,   288,     0,     0,
     284,   287,   285,   283,     0,   293,   292,     0,     0,   430,
     430,     0,     0,     0,    26,    27,     0,     0,     0,     0,
       0,    28,     0,     0,     0,     0,    29,     0,    30,     0,
     438,   436,     0,   431,   432,   437,   156,     0,   159,     0,
     152,     0,     0,   158,     0,     0,     0,   171,     0,     0,
     170,     0,   179,     0,     0,     0,   177,     0,     0,    58,
     146,     0,   191,     0,     0,    31,   305,   239,   376,     0,
     377,   379,     0,   401,     0,   382,     0,     0,   130,    32,
       0,     0,    34,     0,   138,   336,     0,    73,   542,   543,
     133,     0,    35,   430,     0,   212,   203,   199,   204,   200,
       0,   360,   357,   164,    38,    51,    50,    53,     0,   575,
       0,     0,   561,     0,   563,     0,     0,     0,     0,     0,
       0,     0,     0,   579,     8,     0,    41,     0,     0,     0,
       0,    54,     0,     0,     0,   316,   371,   491,   603,   602,
     605,   614,   613,   618,   617,   599,   596,   597,   598,   533,
     586,     0,   558,   559,   557,   556,   534,   590,   601,   595,
     593,   604,   594,   592,   584,   589,   591,   600,   583,   587,
     588,   585,   535,     0,     0,     0,     0,     0,     0,     0,
     616,   615,   620,   619,   503,   504,   506,   508,     0,   495,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   577,
     492,   492,   167,   358,   370,   424,     0,     0,   450,     0,
       0,   225,     0,     0,   225,   359,     0,     0,     0,     0,
     467,     0,     0,     0,   180,     0,   473,     0,     0,   178,
     622,    61,     0,    54,    59,     0,   190,     0,     0,     0,
     449,   310,   307,   308,   309,   313,   314,   315,   305,     0,
     298,     0,   306,   324,     0,   380,     0,   128,   129,   127,
     126,     0,   400,   399,   525,     0,   374,   523,   373,     0,
       0,   555,   422,   421,     0,     0,     0,   451,     0,   206,
     364,   525,     0,   576,   532,   562,   428,   564,   429,   187,
       0,     0,     0,   578,   185,   477,     0,   581,   580,     0,
      43,    42,    40,     0,     0,    70,     0,    65,     0,     0,
      58,   226,     0,     0,   236,     0,   233,   320,   317,   318,
     321,   245,     0,     0,    77,    78,    76,    75,    74,   553,
     554,   505,   507,     0,   494,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   163,   434,
     435,   433,   225,     0,   161,   225,     0,   160,     0,   189,
       0,     0,   169,     0,   168,     0,   498,     0,     0,   175,
       0,     0,   173,     0,   147,     0,   148,     0,     0,   193,
       0,   195,   311,   312,     0,   305,   296,     0,   414,   325,
     328,   327,   329,     0,   378,   381,   382,     0,     0,   383,
     384,   521,   522,   214,     0,     0,   213,   216,   452,     0,
     207,   210,     0,   361,   188,     0,     0,     0,     0,   186,
       0,     0,     0,    56,    55,     0,     0,     0,   241,   231,
       0,   234,   230,   319,   324,   294,    81,   277,    95,   275,
     101,   100,    84,    99,    96,   280,   111,    82,   112,    98,
     102,    83,    85,    97,   113,   274,   256,   259,   257,   258,
     269,   260,   273,   265,   263,   276,   279,   264,   262,   267,
     272,   261,   266,   270,   271,   268,   278,     0,   254,     0,
      86,     0,   294,   294,   252,   560,   496,   382,   545,   545,
       0,     0,     0,     0,     0,     0,     0,     0,   166,   165,
       0,   227,     0,   227,   172,     0,     0,   466,     0,   465,
       0,   497,     0,     0,   472,   176,     0,   471,   174,    55,
     192,   482,   194,     0,   414,   299,     0,     0,   382,   326,
     341,   375,   405,     0,   577,   430,   430,     0,   218,     0,
       0,     0,   208,     0,   183,   479,     0,     0,   181,   478,
       0,   582,    71,     0,     0,     0,    58,    66,    68,   221,
     137,   241,   220,   241,   228,   237,   235,     0,   305,   251,
     255,     0,   281,     0,   247,   248,   500,     0,     0,     0,
       0,     0,     0,     0,     0,   227,   241,   227,   241,   470,
       0,     0,   499,   476,     0,     0,     0,   197,   382,   414,
       0,   417,   416,   418,   525,   338,   302,   300,     0,     0,
       0,     0,   403,   525,     0,     0,   219,   217,     0,   211,
       0,   184,     0,   182,    72,    57,     0,     0,   229,   396,
       0,   330,     0,   253,    81,    83,   294,   294,     0,     0,
       0,     0,     0,     0,   241,   155,   241,   151,     0,     0,
       0,     0,   196,   301,   382,   406,     0,   340,   339,   355,
       0,   356,   343,   346,     0,   342,   334,   335,   240,     0,
     517,   518,   209,     0,     0,    67,    69,     0,   395,   394,
     525,   331,   341,   295,   249,   250,     0,     0,     0,     0,
       0,     0,   157,   153,   469,   468,   475,   474,   304,   303,
     408,   409,   411,   525,     0,   577,   354,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   525,   519,   520,   481,
     480,     0,   386,     0,     0,     0,   410,   412,   345,   347,
     348,   351,   352,   353,   349,   350,   344,   391,   389,   525,
     577,   332,   246,   333,   406,   390,   525,   413
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   311,   104,   616,   106,   107,   108,   329,
     109,   312,   611,   615,   612,   110,   111,   112,   113,   114,
     170,   858,   249,   115,   246,   116,   642,   254,   117,   118,
     266,   119,   120,   121,   421,   593,   417,   590,   122,   123,
     724,   124,   125,   126,   476,   659,   792,   127,   128,   655,
     787,   129,   130,   513,   808,   131,   132,   695,   696,   181,
     247,   633,   134,   135,   515,   814,   701,   861,   862,   443,
     949,   447,   629,   630,   631,   632,   702,   336,   773,  1062,
    1122,  1048,   274,   986,   990,  1042,  1043,  1044,   136,   300,
     481,   137,   138,   250,   251,   451,   452,   646,  1059,  1011,
     455,   643,  1081,   983,   908,   313,   197,   317,   318,   412,
     413,   414,   182,   140,   141,   142,   143,   183,   145,   167,
     168,   568,   433,   745,   569,   570,   146,   147,   184,   185,
     150,   209,   415,   187,   152,   188,   189,   155,   156,   157,
     323,   158,   159,   160,   161,   162
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1019
static const yytype_int16 yypact[] =
{
   -1019,   114,  3096, -1019,   -95, -1019, -1019, -1019, -1019, -1019,
   -1019, -1019,  4440,    71,   174, -1019, 13537, -1019, 18786,    71,
   10296, -1019,   338,   120,   174,  4440, 10296,  4440,   124, 18844,
   10462,  7790, -1019,  7956,  8960,  6454, 10296, -1019,   133, -1019,
   -1019, 18881, -1019,  2728,  9126, -1019, 10296,   115, -1019, -1019,
   -1019, 10296, -1019, 13537, -1019, 10296,   231,   171,   385,  2768,
   -1019, 18939, -1019,  9294,  7290, 10296,  9126, 13537, 10296,   277,
   -1019,   208,  4440, -1019, 10296, -1019, 10628, 10628, 18881, -1019,
   -1019, 13537, -1019, -1019, 10296, 10296, -1019, 10296, -1019, 10296,
   -1019, -1019, 13076, 10296, -1019, 10296, -1019, -1019, -1019,  3432,
    6622,  8124, 10296, -1019,  4272, -1019,   220, -1019,   318, -1019,
      33, -1019, -1019, -1019, -1019, -1019, -1019, -1019, -1019, -1019,
   -1019, -1019, -1019, -1019, -1019, -1019, -1019, -1019, -1019, -1019,
   18881, -1019, 18881,   354,    75, -1019, -1019,  2728, -1019,   254,
   -1019,   279, -1019, -1019,   280,   316,   334, 10296,   330,   347,
   18274,  2075,   -55,   351,   357, -1019, -1019, -1019, -1019, -1019,
   -1019, -1019,   342, -1019, -1019, 18274,   353,  4440, -1019, -1019,
     364, 10296, -1019, -1019, 10296, 10296, 10296, -1019, 10296,  9294,
    9294,   119, -1019, -1019, -1019, -1019,   150,   134, -1019, -1019,
     366, 15086, 18881,  2728, -1019,   387, -1019,   149, 18274,   431,
    7458,   466, 18976, 18274,   192,   469, -1019, 19034, 18881,   192,
   18881,   393,    61, 14765,    18, 14708,   367, -1019, 14860, 13975,
    7458, -1019, -1019, -1019, -1019, -1019, -1019, -1019, -1019, -1019,
   -1019, -1019, -1019, -1019, -1019, -1019, -1019, -1019, -1019, -1019,
   -1019, -1019, -1019, -1019, -1019,  4440, -1019,   395,  2246,    56,
      -1, -1019,  4440, -1019, -1019, 15162,  1005, -1019,   409,   416,
   -1019, 15162,   150,  1005, -1019,  7458, 19069, -1019, -1019,  9460,
   -1019, -1019, -1019, 18881, -1019,   246, 18274, 10296, 10296, -1019,
   18274,   434, 15778, -1019, 15162,   150, 18274,   435,  7458, -1019,
   18274, 15854, -1019, -1019, 15934,  2313, -1019, -1019, 16014,   481,
     448,   150, 15162, 16094,   361,   361,  2236,  1005,  1005,   298,
   -1019, -1019,  3600,    67, -1019, 10296, -1019,    86,   111, -1019,
     -34,   -15, 16174,   -36,  2236,   601, -1019,  3768, -1019,   550,
   10296, 10296,   473,   453, -1019, -1019, -1019,   295,   155, -1019,
   10296,   476, 10296, 10296, 10296,  8960,  8960, 10296,   236, 10296,
   10296, 10296, 10296, 10296,   399, 13076, 10296, 10296, 10296, 10296,
   10296, 10296, 10296, 10296, 10296, 10296, 10296, 10296, 10296, 10296,
   10296, 10296, -1019, -1019, -1019, -1019, -1019, -1019,  8290,  8290,
   -1019, -1019, -1019, -1019,  8290, -1019, -1019,  8290,  8290,  7458,
    7458,  8960,  8960,  7124, -1019, -1019, 15238, 15314, 16250,    62,
    3936, -1019,  8960,    61,   463,   333, -1019, 10296, -1019, 10296,
     505, -1019,   460,   493, -1019, -1019, -1019, 18881, -1019,  2728,
   -1019, 18881,   479, -1019,  2728,   593,  9294, -1019,  4608,  8960,
   -1019,   480, -1019,    61,  4776,  8960, -1019,    61, 10794, 10296,
   -1019,  4440,   607, 10296,   482, -1019,   221, -1019, -1019,  2246,
   -1019,   507,   483, -1019, 10960,   533, 10296,  2728, -1019, -1019,
   10296, 10296, -1019,   486, -1019, -1019,  9294, -1019, 18274, 18274,
   -1019,    29, -1019,  7458,   487, -1019,   637, -1019,   637, -1019,
   11126,   525, -1019, -1019, -1019, -1019, -1019, -1019,  8458, -1019,
   16330,  6790, -1019,  6958, -1019,  4440,   497,  8960,  8626,  3264,
     498, 10296,  9626, -1019, -1019,   340, -1019,  4104, 18881, 15390,
     372, 14917,  9294,   506, 18749,   222, -1019, 16406, 18515, 18515,
     445, -1019,   445, -1019,   445, 18595,   502,   574,  1186,   150,
     361,   514, -1019, -1019, -1019, -1019,  2236,  2888,   445,   671,
     671, 18515,   671,   671,   944,   361,  2888, 18555,   944,  1005,
    1005,   361,  2236,   512,   513,   522,   524,   528,   517,   504,
   -1019,   445, -1019,   445,    28, -1019, -1019, -1019,   112, -1019,
    1586, 18350,   391, 11292,  8960, 11458,  8960, 10296,  8960, 13799,
      71, 16482, -1019, -1019, -1019, 18274, 16558,  7458, -1019,  7458,
   18881,   473,   381, 18881,   473, -1019,   396, 10296,   130, 10296,
   18274,    64, 15470,  7124, -1019, 10296, 18274,    63, 15010, -1019,
     530,   545,   531, 16638,   545,   532,   658, 16714,  4440, 15546,
   -1019,   199, -1019, -1019, -1019, -1019, -1019, -1019,   585,   148,
   -1019,  2911, -1019,   424,   529,  2246,    56,   124,   133, 10296,
   10296,  6288, -1019, -1019,   733,  7624, -1019, 18274, -1019, 16790,
   16866, -1019, -1019, 18274,   535,    36,   547, -1019,  2483, -1019,
   -1019,   418, 18881, -1019, -1019, -1019, -1019, -1019, -1019, -1019,
    4440,     2, 15626, -1019, -1019, 18274,  4440, 18274, -1019, 16942,
   -1019, -1019, -1019,    46,    26, -1019, 10296, -1019, 10296, 10794,
   10296,   577,  1127,   553,   600,   142, -1019,   638, -1019, -1019,
   -1019, -1019, 12915,   554, -1019, -1019, -1019, -1019, -1019, -1019,
   -1019, -1019, -1019,  7124, -1019,    19,  8960,  8960, 10296,   690,
   17018, 10296,   698, 17094,   564, 17170,    61,    61, -1019, -1019,
   -1019, -1019,   473,   575, -1019,   473,   578, -1019, 15162, -1019,
   14055,  4944, -1019,  5112, -1019,   203, -1019, 14135,  5280, -1019,
      61,  5448, -1019,    61, -1019, 10296, -1019,  4440, 10296, -1019,
    4440,   706, -1019, -1019, 18881,   654, -1019,  2246,   604,   661,
   -1019, -1019, -1019,    98, -1019, -1019,   533,   579,    74, -1019,
   -1019,   580,   583, -1019,  5616,  9294, -1019, -1019, -1019, 18881,
   -1019,   611,   366, -1019, -1019,  5784,   582,  5952,   587, -1019,
   10296,   603, 10296, 17246, 15702,   609,   610,   594,  1469, -1019,
   10296, 18881, -1019, -1019,   424,   586,   319, -1019,   618, -1019,
     621,   624,   636,   631,   633, -1019,   634,   643,   639,   640,
     641,   415,   651,   645,   646, -1019, -1019, 18881, -1019, -1019,
   -1019, -1019, -1019, -1019, -1019, -1019, -1019, -1019, -1019, -1019,
   -1019, -1019, -1019, -1019, -1019, -1019, -1019, 10296, -1019,   660,
     663,   650,   586,   586, -1019, -1019, -1019,   533,   223,   225,
   17322, 11624, 11790, 17402, 11956, 12122, 12288, 12454, -1019, -1019,
     647, -1019,   652, -1019, -1019,  4440, 10296, 18274, 10296, 18274,
    7124, -1019,  4440, 10296, 18274, -1019, 10296, 18274, -1019, 18274,
   -1019, 18274,   760,  4440,   604, -1019,   635,  9794,   -13, -1019,
      45, -1019, -1019,  8960, 13668,  7458,  7458,  4440, -1019,    60,
     648, 10296, -1019, 10296, -1019, 18274,  4440, 10296, -1019, 18274,
    4440, 18274, -1019,   235, 10296, 10794, 10296, -1019, -1019, -1019,
   -1019, 19069, -1019,  2592, -1019, 18274, -1019,   122,   221, -1019,
   -1019, 17482, -1019, 13894, -1019, -1019, -1019, 10296, 10296, 10296,
   10296, 10296, 10296, 10296, 10296, -1019,  1767, -1019,  1977, 16714,
   14215, 14295, -1019, 16714, 14375, 14455,  4440, -1019,   533,   604,
    8794, -1019, -1019, -1019,   243,  9294, -1019, -1019,   162, 10296,
     -14, 17558, -1019,  1115,   653,   655,   431, -1019,   366, 18274,
   14535, -1019, 14615, -1019, -1019, 18274,   642,   644, -1019, -1019,
   12620,   699,   232, -1019,   673,   684,   586,   586, 17634, 17715,
   17795, 17875, 17955, 18035, 18486, -1019, 18500, -1019,  4440,  4440,
    4440,  4440, -1019, -1019,   -13,  9962,    95, -1019, 18274, -1019,
      73, -1019,   -53, -1019,   390, 18122, -1019, -1019, -1019, 12454,
     668,   672, -1019,  4440,  4440, -1019, -1019,  6120, -1019, -1019,
     386, -1019,    45, -1019, -1019, -1019, 10296, 10296, 10296, 10296,
   10296, 10296, -1019, -1019, 16714, 16714, 16714, 16714, -1019, -1019,
   -1019, -1019, -1019,   245,  8960, 13241, -1019, 10296,   162,    73,
      73,    73,    73,    73,    73,   162,  1126, -1019, -1019, 16714,
   16714,   665, 12786,   105,    24, 18198, -1019, -1019, 18274, -1019,
   -1019, -1019, -1019, -1019, -1019, -1019,   681, -1019, -1019,   457,
   13406, -1019, -1019, -1019, 10130, -1019,   461, -1019
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1019, -1019, -1019,     3,  -330,  2156, -1019, -1019, -1019, -1019,
     389,   501,  -322,  -665,  -663, -1019, -1019, -1019, -1019,   520,
     811, -1019,  2089,  -152,  -685, -1019,  -867,   704,  -929,  -770,
   -1019,   -56, -1019, -1019, -1019, -1019, -1019, -1019, -1019, -1019,
     178, -1019, -1019, -1019,   540, -1019,    48, -1019, -1019, -1019,
   -1019, -1019, -1019,  -500,  -640, -1019, -1019, -1019,    35,   967,
   -1019, -1019, -1019,   204, -1019, -1019, -1019, -1019,  -114,  -150,
    -826, -1019,  -108,    82,   224, -1019, -1019, -1019,    40, -1019,
   -1019,  -256,   -73,  -185,  -212,  -240,  -232,  -781, -1019,  -180,
   -1019,    13,   822,  -127,   401, -1019,  -438,  -760,  -874, -1019,
    -594,  -473, -1018,  -935,  -819,   -19, -1019,    34, -1019,  -218,
   -1019,   272,   526,  -325, -1019, -1019, -1019,  1158, -1019,    14,
   -1019, -1019,  -202, -1019,  -588, -1019, -1019, -1019,  1356,  1395,
     -12,   836,    80,   928, -1019,  1728,  1965, -1019, -1019, -1019,
   -1019, -1019, -1019, -1019, -1019,  -387
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -578
static const yytype_int16 yytable[] =
{
     165,   388,   444,   271,   186,   103,   572,   660,   191,   510,
     337,   634,   404,   437,   198,   746,   911,   860,   203,   203,
     453,   213,   215,   218,   219,   806,   805,   316,   422,     5,
     654,   497,   255,   190,   256,   193,   954,   955,   944,   261,
     981,   262,   776,   263,   275,   456,   531,   463,   410,     5,
     252,   276,   280,   282,   284,   285,   286,   453,  1087,   532,
     290,  1047,   291,   163,   294,   298,   405,  1106,   410,   301,
     474,    64,   302,   303,   711,   304,  1039,   305,   330,   431,
     306,   307,   321,   308,   533,   978,  1088,   276,   280,   322,
     324,   733,   252,   252,   736,   748,   741,  -215,   431,   453,
    1082,   988,  -397,   502,   784,   645,  1127,   956,   578,    64,
     389,   712,   390,   410,     3,   431,   628,   614,   496,   500,
     913,   867,    73,  1009,   498,   866,   334,   411,   534,   644,
     985,   431,   503,   257,   495,   341,   410,  -397,   457,  -215,
     331,  1084,    73,  1058,   281,   388,   785,   411,   987,   400,
    1107,   578,   335,   499,  -397,   661,   989,   454,  -397,   165,
    1034,   399,   396,   397,   398,  1039,   290,   276,   322,  1041,
     795,   558,   559,  1008,  -397,  1047,   431,   169,  -392,   910,
     319,   400,  1121,  -397,   166,  -397,   258,   400,   280,   802,
    1064,  1065,   411,  -215,   454,  -397,   944,   775,   944,   466,
     786,   498,   535,   194,   259,   403,   488,  -392,   280,  -397,
     464,   212,   214,   498,  -397,   411,   572,   419,  1033,    23,
    -392,   260,   424,   400,   400,   491,   400,   400,  1118,   193,
     579,   489,   880,  -392,   498,   882,   454,   410,   410,   264,
      39,   966,   914,   968,   498,   762,  1125,  1040,   621,   697,
     493,   713,   492,   280,   944,   656,   944,   203,  1041,  1086,
    1010,  -397,    52,  1085,    54,   468,   469,   622,   860,   466,
     623,  1007,  1006,  1120,  1079,    62,   280,   494,   714,   320,
    -392,   811,   763,   348,  -552,  -392,  -552,   765,   407,   200,
     624,   354,   592,   625,   698,    56,   739,   596,   220,  -549,
      82,  -549,   972,   490,   626,   699,   812,   408,  1110,  1111,
    1112,  1113,  1114,  1115,   766,   487,   411,   411,   509,   511,
    -551,   410,  -551,   627,   700,  1024,   572,  1026,   517,   906,
     518,   519,   520,   522,   524,   525,   265,   526,   527,   528,
     529,   530,   890,   536,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   551,   552,
    -297,  -415,  -502,  -407,  -501,    23,   280,   280,   614,   891,
     287,   765,   280,   288,   686,   280,   280,   280,   280,   561,
     563,   571,  -415,   327,  -407,   466,  -415,  -297,  -407,  -502,
     581,  -501,   344,   652,    23,   585,   328,   586,  1063,  1004,
     411,  -550,     5,  -550,   467,   750,   753,   598,   418,  -415,
      54,  -407,  -485,   423,   276,   652,   600,   602,   316,   531,
     316,    62,   606,   608,   391,   652,   613,   613,   392,   617,
     -64,   619,   532,   253,   457,   628,   338,   410,  -484,    54,
     192,   992,  -393,   345,   647,   -46,    82,   346,   649,   650,
      62,   769,   -87,   516,   653,   253,   163,   533,   553,   554,
     -93,   280,   485,   -46,   555,   292,   296,   556,   557,   796,
     798,  -393,   457,   717,   339,    82,   653,   392,   943,   280,
     933,   280,   793,   340,  -393,   672,   653,   675,  -489,   677,
     679,   584,   -64,   691,   348,    73,   770,  -393,   163,   352,
     276,   534,   354,   572,   680,  -490,   357,   771,  1089,  -487,
     487,   686,  -488,  -385,   729,  -486,   411,  -387,   393,  -552,
     457,  -552,   395,   521,   523,   666,   772,   668,   139,   400,
     687,   801,   535,   344,  1090,   457,  1091,  1092,   139,   734,
    1093,  1094,  -385,   409,  -393,   406,  -387,   416,   -89,  -393,
     420,   139,   863,   139,   737,  -385,   -94,  -363,   426,  -387,
     446,   600,   720,   606,   723,   617,   725,   675,  -385,   560,
     562,   319,  -387,   319,   460,   280,  -363,   280,   348,   401,
     580,   461,   984,   352,   345,   738,   354,   740,   346,   993,
     357,   571,   470,   747,   726,   727,   293,   297,   139,   480,
     473,   253,   253,   253,   253,   344,   482,   601,   442,   504,
     508,   512,   621,   607,   614,  -385,   514,   354,   628,  -387,
    -385,   583,   778,   587,  -387,   139,   588,   286,   290,   322,
     139,   622,   589,   280,   623,   348,   943,   595,   943,   597,
     352,   618,   353,   354,   253,   603,   635,   357,   620,   636,
     253,   645,   651,   657,   624,   364,   345,   625,   658,   325,
     346,   368,   369,   370,   662,   670,   676,   730,   626,   692,
     704,   705,   710,   253,   803,   671,   804,   613,   613,   703,
     706,   621,   707,   709,   686,  1060,   708,   627,   -60,   754,
     756,   253,   757,   139,   943,   774,   943,   994,   995,   783,
     622,   571,   344,   623,   522,   561,   870,   348,   349,   873,
    1083,   351,   352,   788,   353,   354,   466,   809,   810,   357,
     865,   813,   871,   624,  1096,   780,   625,   364,    56,   887,
     874,   889,   876,   368,   369,   370,   894,   626,   881,   897,
     903,   883,   907,   899,   909,   915,   901,   912,   916,   921,
     926,   948,   719,   345,   722,   930,   627,   346,   939,  -114,
    1083,   932,  -121,   410,   410,  -120,   919,   937,   938,   -90,
     283,   139,  -119,   276,  -116,  -123,   -88,  1119,   139,  -398,
    -124,  -118,  -122,   925,   -91,   929,  -117,  -125,   931,  -398,
     613,   953,   253,   952,   976,  1126,   -92,  1061,   945,  1083,
    1055,   979,  1056,  1017,   348,   349,   -87,   350,   351,   352,
     965,   353,   354,   355,   998,   967,   357,   -89,  -398,  1050,
    1088,  1051,   253,   363,   364,  1097,   253,   367,   507,  1098,
     368,   369,   370,  1117,   982,   195,   478,   920,   139,  1016,
    1012,   371,   411,   411,  -398,   951,   946,   905,  1123,  1078,
    1104,  -398,   764,   139,   947,  1116,  1109,   217,   648,   887,
     889,   731,   894,   897,   925,   929,   204,     0,     0,     0,
       0,     0,  -398,   969,   970,     0,   971,     0,   571,     0,
     973,   974,     0,     0,   975,   271,     0,     0,     0,     0,
       0,  -398,     0,     0,   521,   560,  -398,     0,     0,  -398,
       0,   991,   675,   280,   280,     0,     0,     0,     0,   999,
       0,  1000,  1037,     0,     0,  1002,   427,   430,   432,   436,
       0,     0,  1005,   613,   613,     0,   139,     0,     0,     0,
     151,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     151,     0,     0,     0,     0,   969,  1018,  1019,   973,  1020,
    1021,  1022,  1023,   151,   139,   151,     0,     0,     0,   459,
     139,  1036,  1080,     0,     0,   462,     0,   139,   322,   133,
       0,     0,     0,  1038,     0,   344,     0,  1045,     0,   133,
       0,     0,     0,     0,     0,     0,     0,     0,   472,     0,
     253,   253,   133,     0,   133,     0,   253,   253,     0,     0,
     151,     0,     0,     0,   295,   295,   483,     0,   761,     0,
       0,     0,  1080,     0,     0,     0,  1074,  1075,  1076,  1077,
       0,   139,     0,     0,     0,   139,   345,   151,     0,     0,
     346,     0,   151,   139,     0,     0,   344,   929,  1103,   133,
       0,  1099,  1100,     0,     0,   322,     0,   790,     0,     0,
       0,  1080,     0,     0,  1074,  1075,  1076,  1077,  1099,  1100,
       0,     0,     0,     0,     0,     0,   133,     0,     0,     0,
       0,   133,  1105,   675,     0,  1108,     0,   348,   349,     0,
     350,   351,   352,     0,   353,   354,     0,   345,     0,   357,
       0,   346,     0,     0,     0,   151,   715,   364,     0,     0,
       0,     0,     0,   368,   369,   370,     0,   582,   675,     0,
       0,     0,   929,     0,     0,   253,   253,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   253,   807,   442,
       0,     0,     0,     0,   133,     0,   442,   604,   348,   253,
       0,   609,   253,   352,   139,   353,   354,     0,     0,  -227,
     357,     0,     0,  -227,  -227,     0,     0,     0,   364,     0,
     144,  -402,  -227,     0,  -227,  -227,   370,     0,     0,  -227,
     144,  -402,  -404,   151,     0,  -227,     0,     0,  -227,     0,
     151,   922,  -404,   144,     0,   144,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   139,  -227,     0,  -227,
    -402,  -227,   139,  -227,  -227,     0,  -227,     0,  -227,     0,
    -227,  -404,   133,     0,     0,     0,     0,   344,     0,   133,
       0,     0,     0,     0,     0,     0,  -402,     0,   864,  -227,
     144,     0,  -227,  -402,     0,  -227,     0,  -404,     0,     0,
     151,     0,   868,   869,  -404,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -402,   151,     0,   144,     0,     0,
       0,     0,   144,     0,     0,  -404,     0,   139,   345,   139,
       0,     0,   346,  -402,   139,     0,     0,   139,  -402,   133,
       0,  -402,     0,   139,  -404,  -227,   139,     0,     0,  -404,
       0,  -227,  -404,     0,   133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   742,   744,     0,   253,     0,
     139,   749,   752,     0,     0,     0,     0,     0,     0,   348,
     349,   139,     0,   139,   352,   144,   353,   354,   151,     0,
       0,   357,     0,     0,     0,     0,     0,     0,     0,   364,
       0,     0,     0,     0,     0,   368,   369,   370,     0,     0,
       0,     0,     0,     0,     0,     0,   151,     0,   148,   442,
     442,     0,   151,   442,   442,     0,     0,   133,   148,   151,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1046,
       0,   148,     0,   148,     0,     0,     0,  1052,     0,   442,
       0,   442,     0,     0,     0,   133,     0,   149,     0,     0,
       0,   133,     0,   144,     0,     0,     0,   149,   133,     0,
     144,   139,     0,     0,     0,     0,     0,     0,   139,     0,
     149,     0,   149,   151,     0,     0,     0,   151,   148,   139,
     878,   879,     0,     0,     0,   151,     0,     0,     0,     0,
       0,     0,   884,   139,     0,     0,     0,     0,     0,     0,
       0,     0,   139,     0,   895,   148,   139,   898,     0,     0,
     148,     0,   133,     0,     0,     0,   133,   149,     0,     0,
     144,     0,     0,     0,   133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   144,     0,     0,     0,     0,
       0,   267,     0,  1046,   149,    22,    23,     0,     0,   149,
       0,     0,   139,     0,   268,     0,    30,   269,     0,     0,
       0,    35,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,   148,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,    54,     0,    56,     0,   940,   151,     0,   941,     0,
     270,     0,    62,     0,   139,   139,   139,   139,   144,     0,
       0,     0,   149,     0,     0,     0,     0,     0,     0,     0,
       0,    78,     0,     0,    80,     0,     0,    82,     0,   139,
     139,     0,     0,     0,     0,   133,   144,     0,     0,     5,
       0,     0,   144,     0,     0,     0,     0,     0,   151,   144,
       0,   148,     0,    15,   151,     0,    17,     0,   148,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   173,     0,   997,     0,     0,     0,    97,     0,     0,
      37,     0,     0,   942,     0,     0,    40,   133,     0,     0,
     149,     0,     0,   133,    45,     0,     0,   149,     0,     0,
       0,    50,     0,   144,     0,     0,     0,   144,     0,     0,
       0,     0,     0,     0,     0,   144,    60,     0,   148,   151,
       0,   151,     0,     0,     0,     0,   151,     0,    70,   151,
       0,     0,    73,   148,     0,   151,     0,     0,   151,    79,
       0,     0,     0,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   149,   133,     0,
     133,     0,   151,     0,     0,   133,     0,     0,   133,     0,
       0,     0,   149,   151,   133,   151,     0,   133,     0,     0,
     153,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     153,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   133,     0,   153,     0,   153,   148,     0,     0,     0,
       0,     0,   133,     0,   133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   144,     0,     0,     0,
       0,     0,     0,     0,   148,     0,     0,     0,     0,   267,
     148,     0,     0,    22,    23,   149,     0,   148,     0,     0,
     153,     0,   268,     0,    30,   269,     0,     0,     0,    35,
       0,     0,     0,   151,     0,    39,     0,     0,     0,     0,
     151,     0,     0,   149,     0,     0,     0,   153,   144,   149,
       0,   151,   153,     0,   144,     0,   149,    52,     0,    54,
       0,    56,     0,   940,     0,   151,   941,     0,   270,     0,
      62,   148,   133,     0,   151,   148,     0,     0,   151,   133,
       0,     0,     0,   148,     0,     0,     0,     0,     0,    78,
     133,     0,    80,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,   133,     0,     0,     0,     0,     0,
     149,     0,     0,   133,   149,   153,     0,   133,     0,   144,
       0,   144,   149,     0,   151,     0,   144,     0,     0,   144,
       0,     0,     0,     0,     0,   144,     0,     0,   144,     0,
       0,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,  1025,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   144,   133,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   144,     0,   144,   151,   151,   151,   151,
       0,     0,     0,     0,     0,     0,     0,   154,     0,     0,
       0,     0,     0,   153,   148,     0,     0,   154,     0,     0,
     153,   151,   151,     0,     0,     0,     0,     0,     0,     0,
     154,     0,   154,     0,     0,   133,   133,   133,   133,   267,
       0,     0,     0,    22,    23,     0,     0,     0,     0,     0,
       0,     0,   268,   149,    30,   269,     0,     0,     0,    35,
     133,   133,     0,     0,     0,    39,   148,     0,     0,     0,
       0,     0,   148,     0,     0,     0,     0,   154,     0,     0,
     153,     0,     0,   144,     0,     0,     0,    52,     0,    54,
     144,    56,     0,   940,     0,   153,   941,     0,   270,     0,
      62,   144,     0,     0,   154,   149,     0,     0,     0,   154,
       0,   149,     0,     0,     0,   144,     0,     0,     0,    78,
       0,     0,    80,     0,   144,    82,     0,     0,   144,     0,
       0,     0,     0,     0,     0,     0,     0,   148,     0,   148,
       0,     0,     0,     0,   148,     0,     0,   148,     0,     0,
       0,     0,     0,   148,     0,     0,   148,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   153,     0,
     245,     0,   154,     0,   144,    97,   149,     0,   149,     0,
     148,  1027,     0,   149,     0,     0,   149,     0,     0,     0,
       0,   148,   149,   148,     0,   149,   153,     0,   105,     0,
       0,     0,   153,     0,     0,     0,     0,   299,   164,   153,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   149,
       0,   196,     0,   199,     0,     0,   144,   144,   144,   144,
     149,     0,   149,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,     0,     0,
     154,   144,   144,     0,     0,     0,     0,   154,     0,   332,
       0,   333,     0,   153,  -509,     0,     0,   153,   289,     0,
       0,     0,     0,     0,     0,   153,     0,   387,     0,     0,
    -549,   148,  -549,     0,     0,     0,     0,     0,   148,   221,
       0,     0,     0,     0,     0,   105,     0,     0,     0,   148,
     326,     0,     0,   222,   223,     0,   224,   344,     0,     0,
       0,   225,     0,   148,     0,     0,     0,   154,     0,   226,
     149,   299,   148,     0,     0,   227,   148,   149,     0,     0,
     228,     0,   154,   229,     0,     0,   230,   299,   149,   425,
       0,     0,     0,     0,   231,     0,     0,     0,     0,     0,
     232,   233,   149,     0,     0,     0,     0,   234,   345,     0,
       0,   149,   346,   394,     0,   149,   235,     0,     0,     0,
       0,     0,   148,     0,     0,   236,   237,   450,   238,     0,
     239,     0,   240,     0,     0,   241,   153,     0,     0,   242,
     448,     0,   243,     0,     0,   244,     0,     0,     0,     0,
       0,     0,   465,     0,     0,   154,     0,     0,     0,   348,
     349,   149,   350,   351,   352,     0,   353,   354,     0,     0,
       0,   357,     0,     0,   148,   148,   148,   148,   363,   364,
       0,     0,   367,   154,     0,   368,   369,   370,   153,   154,
       0,   445,     0,     0,   153,     0,   154,     0,   458,   148,
     148,   449,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   149,   149,   149,   149,     0,     0,     0,
       0,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,     0,     0,   149,   149,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     154,     0,     0,     0,   154,     0,     0,     0,   105,   153,
       0,   153,   154,     0,     0,   387,   153,     0,  -549,   153,
    -549,     0,     0,   105,     0,   153,   221,     0,   153,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,   223,     0,   224,     0,     0,   591,     0,   225,     0,
     594,     0,   153,     0,     0,     0,   226,     0,     0,     0,
       0,     0,   227,   153,     0,   153,     0,   228,     0,     0,
     229,     0,     0,   230,     0,     0,     0,     0,   450,     0,
       0,   231,     0,     0,     0,     0,     0,   232,   233,     0,
       0,     0,     0,     0,   234,     0,   105,     0,     0,     0,
       0,     0,     0,   235,     0,     0,     0,     0,     0,     0,
       0,     0,   236,   237,     0,   238,     0,   239,     0,   240,
       0,     0,   241,   154,   458,     0,   242,     0,     0,   243,
     458,     0,   244,     0,     0,     0,     0,   682,     0,     0,
       0,     0,     0,   694,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   153,   267,     0,     0,     0,    22,    23,
     153,     0,     0,     0,     0,     0,     0,   268,     0,    30,
     269,   153,     0,     0,    35,   154,     0,     0,     0,     0,
      39,   154,     0,     0,     0,   153,   400,     0,   789,     0,
       0,   669,     0,     0,   153,   674,     0,     0,   153,     0,
       0,     0,    52,   105,    54,     0,   325,     0,   940,     0,
       0,   941,     0,   270,     0,    62,     0,     0,     0,   732,
       0,     0,   735,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    78,     0,     0,    80,     0,     0,
      82,     0,     0,     0,   153,     0,   154,     0,   154,     0,
       0,     0,     0,   154,     0,     0,   154,     0,     0,     0,
     768,     0,   154,     0,   450,   154,     0,     0,     0,     0,
       0,   221,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   222,   223,   791,   224,   154,
      97,   299,     0,   225,     0,     0,   153,   153,   153,   153,
     154,   226,   154,     0,     0,     0,     0,   227,     0,     0,
       0,     0,   228,     0,   759,   229,     0,     0,   230,     0,
       0,   153,   153,     0,     0,     0,   231,     0,     0,     0,
     267,   859,   232,   233,    22,    23,     0,     0,     0,   234,
       0,     0,     0,   268,     0,    30,   269,     0,   235,     0,
      35,     0,     0,   -63,     0,     0,    39,   236,   237,  -241,
     238,     0,   239,     0,   240,     0,   794,   241,     0,     0,
       0,   242,   799,     0,   243,     0,     0,   244,    52,     0,
      54,     0,     0,     0,     0,  -241,     0,     0,     0,   270,
     154,    62,     0,   904,     0,     0,   450,   154,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   154,     0,
      78,     0,     0,    80,     0,   -63,    82,     0,   791,     0,
       0,     0,   154,     0,     0,     0,     0,     0,     0,     0,
       0,   154,     0,   248,     0,   154,     0,   458,     0,   458,
     694,     0,     0,     0,   458,     0,     0,   458,     0,     0,
       0,     0,     0,   900,   221,     0,   902,     0,     0,   344,
       0,     0,     0,     0,     0,     0,   950,     0,   222,   223,
       0,   224,     0,     0,     0,     0,   225,     0,     0,     0,
     918,   154,     0,     0,   226,     0,     0,     0,     0,     0,
     227,   924,     0,   928,     0,   228,     0,     0,   229,     0,
       0,   230,     0,     0,     0,     0,     0,     0,     0,   231,
     345,     0,     0,     0,   346,   232,   233,     0,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,   235,     0,   154,   154,   154,   154,     0,     0,     0,
     236,   237,     0,   238,     0,   239,     0,   240,     0,     0,
     241,     0,     0,     0,   242,     0,     0,   243,   154,   154,
     244,   348,   349,     0,   350,   351,   352,     0,   353,   354,
     355,     0,     0,   357,   358,   359,     0,     0,   361,   362,
     363,   364,   859,     0,   367,     0,     0,   368,   369,   370,
       0,     0,     0,     0,     0,     0,     0,     0,   371,   977,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   996,     0,     0,   767,     0,     0,     0,
       0,     0,  1001,     0,     0,     0,  1003,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    -2,     4,     0,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,  1032,    30,    31,    32,    33,    34,    35,    36,
      37,   -62,     0,    38,    39,     0,    40,  -241,     0,    41,
      42,    43,     0,    44,    45,    46,   -44,    47,    48,     0,
      49,    50,    51,     0,     0,     0,    52,    53,    54,    55,
      56,    57,    58,  -241,   -44,    59,    60,    61,     0,    62,
      63,    64,     0,    65,    66,    67,    68,    69,    70,    71,
      72,     0,    73,    74,     0,    75,    76,    77,    78,    79,
       0,    80,    81,   -62,    82,    83,     0,     0,    84,     0,
      85,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,    97,     0,     0,    98,     0,    99,
       0,   100,     0,   101,     0,     4,   102,     5,     0,     6,
       7,     8,     9,    10,    11,     0,  -577,     0,    12,    13,
      14,    15,    16,  -577,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,  -577,    27,    28,  -577,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,   -62,
       0,    38,    39,     0,    40,  -241,     0,    41,    42,    43,
       0,    44,    45,    46,   -44,    47,    48,     0,    49,    50,
      51,     0,     0,     0,    52,    53,    54,    55,     0,    57,
      58,  -241,   -44,    59,    60,    61,  -577,    62,    63,    64,
    -577,    65,    66,    67,    68,    69,    70,    71,    72,     0,
      73,    74,     0,    75,    76,    77,    78,    79,     0,    80,
      81,   -62,    82,    83,     0,     0,    84,     0,    85,     0,
       0,  -577,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -577,  -577,    89,
    -577,  -577,  -577,  -577,  -577,  -577,  -577,     0,  -577,  -577,
    -577,  -577,  -577,     0,  -577,  -577,  -577,  -577,  -577,  -577,
    -577,  -577,    97,  -577,  -577,  -577,     0,    99,  -577,   100,
       0,   101,     0,   309,  -577,     5,   272,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,   -62,     0,    38,
      39,     0,    40,  -241,     0,    41,    42,    43,     0,    44,
      45,    46,   -44,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,    56,    57,    58,  -241,
     -44,    59,    60,    61,     0,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
       0,    75,    76,    77,    78,    79,     0,    80,    81,   -62,
      82,    83,     0,     0,    84,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,   273,
      97,     0,     0,    98,     0,    99,   310,   100,     0,   101,
       0,     4,   102,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,   -62,     0,    38,    39,     0,
      40,  -241,     0,    41,    42,    43,     0,    44,    45,    46,
     -44,    47,    48,     0,    49,    50,    51,     0,     0,     0,
      52,    53,    54,    55,    56,    57,    58,  -241,   -44,    59,
      60,    61,     0,    62,    63,    64,     0,    65,    66,    67,
      68,    69,    70,    71,    72,     0,    73,    74,     0,    75,
      76,    77,    78,    79,     0,    80,    81,   -62,    82,    83,
       0,     0,    84,     0,    85,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,    97,     0,
       0,    98,     0,    99,   486,   100,     0,   101,     0,   505,
     102,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,   -62,     0,    38,    39,     0,    40,  -241,
       0,    41,    42,    43,     0,    44,    45,    46,   -44,    47,
      48,     0,    49,    50,    51,     0,     0,     0,    52,    53,
      54,    55,    56,    57,    58,  -241,   -44,    59,    60,    61,
       0,    62,    63,    64,     0,    65,    66,    67,    68,    69,
      70,    71,    72,     0,    73,    74,     0,    75,    76,    77,
      78,    79,     0,    80,    81,   -62,    82,    83,     0,     0,
      84,     0,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,    97,     0,     0,    98,
       0,    99,   506,   100,     0,   101,     0,   309,   102,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,   -62,     0,    38,    39,     0,    40,  -241,     0,    41,
      42,    43,     0,    44,    45,    46,   -44,    47,    48,     0,
      49,    50,    51,     0,     0,     0,    52,    53,    54,    55,
      56,    57,    58,  -241,   -44,    59,    60,    61,     0,    62,
      63,    64,     0,    65,    66,    67,    68,    69,    70,    71,
      72,     0,    73,    74,     0,    75,    76,    77,    78,    79,
       0,    80,    81,   -62,    82,    83,     0,     0,    84,     0,
      85,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,    97,     0,     0,    98,     0,    99,
     310,   100,     0,   101,     0,     4,   102,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,   -62,
       0,    38,    39,     0,    40,  -241,     0,    41,    42,    43,
       0,    44,    45,    46,   -44,    47,    48,     0,    49,    50,
      51,     0,     0,     0,    52,    53,    54,    55,    56,    57,
      58,  -241,   -44,    59,    60,    61,     0,    62,    63,    64,
       0,    65,    66,    67,    68,    69,    70,    71,    72,     0,
      73,    74,     0,    75,    76,    77,    78,    79,     0,    80,
      81,   -62,    82,    83,     0,     0,    84,     0,    85,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,    97,     0,     0,    98,     0,    99,   681,   100,
       0,   101,     0,     4,   102,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,   -62,     0,    38,
      39,     0,    40,  -241,     0,    41,    42,    43,     0,    44,
      45,    46,   -44,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,   325,    57,    58,  -241,
     -44,    59,    60,    61,     0,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
       0,    75,    76,    77,    78,    79,     0,    80,    81,   -62,
      82,    83,     0,     0,    84,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,     0,   100,     0,   101,
       0,     4,   102,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,   -62,     0,    38,    39,     0,
      40,  -241,     0,    41,    42,    43,     0,    44,    45,    46,
     -44,    47,    48,     0,    49,    50,    51,     0,     0,     0,
      52,    53,    54,    55,     0,    57,    58,  -241,   -44,    59,
      60,    61,     0,    62,    63,    64,     0,    65,    66,    67,
      68,    69,    70,    71,    72,     0,    73,    74,     0,    75,
      76,    77,    78,    79,     0,    80,    81,   -62,    82,    83,
       0,     0,    84,     0,    85,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,    97,     0,
       0,    98,     0,    99,     0,   100,     0,   101,     0,     4,
     102,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,   599,    37,   -62,     0,    38,    39,     0,    40,  -241,
       0,    41,    42,    43,     0,    44,    45,    46,   -44,    47,
      48,     0,    49,    50,    51,     0,     0,     0,    52,    53,
      54,    55,     0,    57,    58,  -241,   -44,    59,    60,    61,
       0,    62,    63,    64,     0,    65,    66,    67,    68,    69,
      70,    71,    72,     0,    73,    74,     0,    75,    76,    77,
      78,    79,     0,    80,    81,   -62,    82,    83,     0,     0,
      84,     0,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,    97,     0,     0,    98,
       0,    99,     0,   100,     0,   101,     0,     4,   102,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,   605,
      37,   -62,     0,    38,    39,     0,    40,  -241,     0,    41,
      42,    43,     0,    44,    45,    46,   -44,    47,    48,     0,
      49,    50,    51,     0,     0,     0,    52,    53,    54,    55,
       0,    57,    58,  -241,   -44,    59,    60,    61,     0,    62,
      63,    64,     0,    65,    66,    67,    68,    69,    70,    71,
      72,     0,    73,    74,     0,    75,    76,    77,    78,    79,
       0,    80,    81,   -62,    82,    83,     0,     0,    84,     0,
      85,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,    97,     0,     0,    98,     0,    99,
       0,   100,     0,   101,     0,     4,   102,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,   886,    37,   -62,
       0,    38,    39,     0,    40,  -241,     0,    41,    42,    43,
       0,    44,    45,    46,   -44,    47,    48,     0,    49,    50,
      51,     0,     0,     0,    52,    53,    54,    55,     0,    57,
      58,  -241,   -44,    59,    60,    61,     0,    62,    63,    64,
       0,    65,    66,    67,    68,    69,    70,    71,    72,     0,
      73,    74,     0,    75,    76,    77,    78,    79,     0,    80,
      81,   -62,    82,    83,     0,     0,    84,     0,    85,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,    97,     0,     0,    98,     0,    99,     0,   100,
       0,   101,     0,     4,   102,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,   888,    37,   -62,     0,    38,
      39,     0,    40,  -241,     0,    41,    42,    43,     0,    44,
      45,    46,   -44,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,     0,    57,    58,  -241,
     -44,    59,    60,    61,     0,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
       0,    75,    76,    77,    78,    79,     0,    80,    81,   -62,
      82,    83,     0,     0,    84,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,     0,   100,     0,   101,
       0,     4,   102,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,   893,    37,   -62,     0,    38,    39,     0,
      40,  -241,     0,    41,    42,    43,     0,    44,    45,    46,
     -44,    47,    48,     0,    49,    50,    51,     0,     0,     0,
      52,    53,    54,    55,     0,    57,    58,  -241,   -44,    59,
      60,    61,     0,    62,    63,    64,     0,    65,    66,    67,
      68,    69,    70,    71,    72,     0,    73,    74,     0,    75,
      76,    77,    78,    79,     0,    80,    81,   -62,    82,    83,
       0,     0,    84,     0,    85,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,    97,     0,
       0,    98,     0,    99,     0,   100,     0,   101,     0,     4,
     102,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,   896,    37,   -62,     0,    38,    39,     0,    40,  -241,
       0,    41,    42,    43,     0,    44,    45,    46,   -44,    47,
      48,     0,    49,    50,    51,     0,     0,     0,    52,    53,
      54,    55,     0,    57,    58,  -241,   -44,    59,    60,    61,
       0,    62,    63,    64,     0,    65,    66,    67,    68,    69,
      70,    71,    72,     0,    73,    74,     0,    75,    76,    77,
      78,    79,     0,    80,    81,   -62,    82,    83,     0,     0,
      84,     0,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,    97,     0,     0,    98,
       0,    99,     0,   100,     0,   101,     0,     4,   102,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,   917,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,   -62,     0,    38,    39,     0,    40,  -241,     0,    41,
      42,    43,     0,    44,    45,    46,   -44,    47,    48,     0,
      49,    50,    51,     0,     0,     0,    52,    53,    54,    55,
       0,    57,    58,  -241,   -44,    59,    60,    61,     0,    62,
      63,    64,     0,    65,    66,    67,    68,    69,    70,    71,
      72,     0,    73,    74,     0,    75,    76,    77,    78,    79,
       0,    80,    81,   -62,    82,    83,     0,     0,    84,     0,
      85,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,    97,     0,     0,    98,     0,    99,
       0,   100,     0,   101,     0,     4,   102,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,   923,    37,   -62,
       0,    38,    39,     0,    40,  -241,     0,    41,    42,    43,
       0,    44,    45,    46,   -44,    47,    48,     0,    49,    50,
      51,     0,     0,     0,    52,    53,    54,    55,     0,    57,
      58,  -241,   -44,    59,    60,    61,     0,    62,    63,    64,
       0,    65,    66,    67,    68,    69,    70,    71,    72,     0,
      73,    74,     0,    75,    76,    77,    78,    79,     0,    80,
      81,   -62,    82,    83,     0,     0,    84,     0,    85,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,    97,     0,     0,    98,     0,    99,     0,   100,
       0,   101,     0,     4,   102,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,   927,    37,   -62,     0,    38,
      39,     0,    40,  -241,     0,    41,    42,    43,     0,    44,
      45,    46,   -44,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,     0,    57,    58,  -241,
     -44,    59,    60,    61,     0,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
       0,    75,    76,    77,    78,    79,     0,    80,    81,   -62,
      82,    83,     0,     0,    84,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,     0,   100,     0,   101,
       0,  1101,   102,     5,   272,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   177,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   178,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,   273,     0,     0,
       0,    98,     0,   179,     0,   100,     0,   180,  1102,   777,
     102,     5,   272,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   176,    37,     0,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   177,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   178,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,   273,     0,     0,     0,    98,
       0,   179,     0,   100,     0,   180,     0,     5,   102,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
     216,    23,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   176,    37,     0,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,    54,    55,     0,    57,
       0,     0,     0,     0,    60,   177,     0,    62,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   178,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,    82,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,   179,     0,   100,
       0,   180,     0,     0,   102,     5,   272,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   171,     0,     0,    15,
      16,     0,    17,     0,   172,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   173,     0,     0,
       0,    32,   174,   175,     0,   176,    37,     0,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   177,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   178,     0,    73,     0,
       0,    75,   277,   278,     0,    79,   314,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,   315,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,   273,
       0,     0,     0,    98,     0,   179,     0,   100,     0,   180,
       0,     0,   102,     5,   272,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   177,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   178,     0,    73,     0,     0,    75,
     277,   278,     0,    79,   314,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,   273,     0,     0,
       0,    98,     0,   179,     0,   100,   665,   180,     0,     0,
     102,     5,   272,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   176,    37,     0,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   177,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   178,     0,    73,     0,     0,    75,   277,   278,
       0,    79,   314,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,   273,     0,     0,     0,    98,
       0,   179,     0,   100,   667,   180,     0,     5,   102,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,   564,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   176,    37,     0,
     565,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   177,     0,   566,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   178,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,   567,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,   179,     0,   100,
       0,   180,     0,     5,   102,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   177,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   178,     0,    73,     0,     0,    75,
     277,   278,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,   279,     0,
       0,    98,     0,   179,     0,   100,     0,   180,     0,     0,
     102,     5,   272,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   176,    37,     0,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   177,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   178,     0,    73,     0,     0,    75,   277,   278,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,   273,     0,     0,     0,    98,
       0,   179,     0,   100,     0,   180,     0,     5,   102,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   176,    37,     0,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,   779,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   177,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   178,     0,
      73,     0,     0,    75,   277,   278,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,   179,     0,   100,
       0,   180,     0,     5,   102,     6,     7,     8,   205,    10,
      11,   206,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   207,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   178,     0,    73,     0,     0,    75,
       0,     0,   208,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   179,     0,   100,     0,   180,     0,     5,
     102,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,   210,    55,
       0,    57,     0,     0,     0,     0,    60,   177,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     178,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,   211,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   179,
       0,   100,     0,   180,     0,     0,   102,     5,   272,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   176,    37,     0,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   177,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   178,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,   211,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,   273,     0,     0,     0,    98,     0,   179,     0,   100,
       0,   180,     0,     5,   102,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   177,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   178,     0,    73,     0,     0,    75,
     277,   278,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   179,     0,   100,     0,   180,     0,     0,
     102,     5,   272,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   176,    37,     0,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   177,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   178,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,   273,     0,     0,     0,    98,
       0,   179,   663,   100,     0,   180,     0,     0,   102,     5,
     272,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   177,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     178,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,   273,     0,     0,     0,    98,     0,   179,
       0,   100,     0,   180,   673,     0,   102,     5,   272,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   176,    37,     0,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   177,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   178,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,   273,     0,     0,     0,    98,     0,   179,     0,   100,
       0,   180,  1035,     5,   102,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   177,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   178,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,   211,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   179,     0,   100,     0,   180,     0,     5,
     102,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,   252,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   177,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     178,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,    99,
       0,   100,     0,   180,     0,     0,   102,     5,   272,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   176,    37,     0,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   177,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   178,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,   273,     0,     0,     0,    98,     0,   179,     0,   100,
       0,   180,     0,     5,   102,     6,     7,     8,   205,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   207,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   178,     0,    73,     0,     0,    75,
       0,     0,   208,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   179,     0,   100,     0,   180,     0,     5,
     102,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   177,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     178,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   179,
       0,   100,     0,   180,   678,     0,   102,     5,   272,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   637,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   176,    37,     0,
       0,   638,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   177,     0,     0,     0,     0,
       0,     0,     0,    67,   639,    69,    70,    71,   640,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,   273,     0,     0,     0,    98,     0,   179,     0,   100,
       0,   980,     0,     0,   102,     5,   272,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   171,     0,     0,    15,
      16,     0,    17,     0,   172,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   173,     0,     0,
       0,    32,   174,   175,     0,   176,    37,     0,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   177,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   178,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,   273,
       0,     0,     0,    98,     0,   179,     0,   100,     0,   980,
       0,     0,   102,     5,   272,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   964,    37,     0,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   177,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   178,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,   273,     0,     0,
       0,    98,     0,   179,     0,   100,     0,   180,     0,     5,
     102,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   177,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     178,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   179,
       0,   100,     0,   180,     0,     5,   102,     6,     7,     8,
     201,    10,    11,     0,     0,     0,   171,     0,     0,    15,
      16,     0,    17,     0,   172,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   173,     0,     0,
       0,    32,   174,   175,     0,   176,    37,     0,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   202,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   178,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,   179,     0,   100,     0,   180,
       0,     5,   102,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   176,    37,     0,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   177,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   178,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,    99,     0,   100,     0,   180,     0,     5,   102,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   176,    37,     0,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   177,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   178,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,   610,     0,   179,     0,   100,
       0,   180,     0,     5,   102,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   637,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,   638,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   177,     0,     0,     0,     0,     0,     0,     0,    67,
     639,    69,    70,    71,   640,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   179,     0,   100,     0,   641,     0,     5,
     102,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   177,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     178,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   179,
       0,   100,     0,   641,     0,     5,   102,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   171,     0,     0,    15,
      16,     0,    17,     0,   172,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   173,     0,     0,
       0,    32,   174,   175,     0,   718,    37,     0,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   177,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   178,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,   179,     0,   100,     0,   180,
       0,     5,   102,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   721,    37,     0,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   177,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   178,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   179,     0,   100,     0,   180,     0,     5,   102,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   958,    37,     0,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   177,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   178,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,   179,     0,   100,
       0,   180,     0,     5,   102,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   959,    37,     0,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   177,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   178,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   179,     0,   100,     0,   180,     0,     5,
     102,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   961,
      37,     0,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   177,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     178,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   179,
       0,   100,     0,   180,     0,     5,   102,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   171,     0,     0,    15,
      16,     0,    17,     0,   172,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   173,     0,     0,
       0,    32,   174,   175,     0,   962,    37,     0,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   177,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   178,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,   179,     0,   100,     0,   180,
       0,     5,   102,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   963,    37,     0,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   177,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   178,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   179,     0,   100,     0,   180,     0,     5,   102,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   964,    37,     0,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   177,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   178,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,   179,     0,   100,
       0,   180,     0,     5,   102,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   637,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,   638,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   177,     0,     0,     0,     0,     0,     0,     0,    67,
     639,    69,    70,    71,   640,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   179,     0,   100,     0,  1057,     0,     5,
     102,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   177,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     178,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   815,     0,   816,    87,
      88,    89,    90,    91,     0,     0,     0,   817,    92,     0,
       0,     0,   818,   223,   819,   820,     0,     0,    93,     0,
     821,    94,    95,    96,     0,     0,     0,    98,   226,     0,
     173,   100,     0,  1057,   822,     0,   102,     0,     0,   823,
       0,     0,   229,     0,     0,   824,     0,   825,     0,     0,
       0,     0,     0,   826,     0,     0,     0,     0,     0,   827,
     828,     0,     0,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,   829,     0,     0,     0,     0,
       0,     0,     0,     0,   236,   237,     0,   830,     0,   239,
       0,   831,     0,     0,   832,     0,     0,     0,   833,     0,
       0,   243,     0,     0,   834,     0,     0,     0,     0,     0,
       0,     0,     0,   372,   373,   374,   375,   376,   377,     0,
       0,   380,   381,   382,   383,     0,   385,   386,   835,   836,
     837,   838,   839,     0,     0,   840,     0,     0,     0,   841,
     842,   843,   844,   845,   846,   847,   848,   849,   850,   851,
       0,   852,     0,     0,   853,   854,   855,   856,     0,     5,
     857,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   177,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     178,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,    95,    96,     0,     0,     0,    98,     0,   179,
       0,   100,     0,   180,     5,   272,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
       0,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   176,    37,     0,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   177,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   178,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,     0,  -406,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
    -406,     0,     0,     0,  -406,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   273,     0,
       0,     0,     0,     0,   179,     0,   100,  -406,   980,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,  -388,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   177,     0,     0,
       0,  -388,     0,     0,     0,    67,    68,    69,    70,    71,
     178,     0,    73,     0,  -388,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,  -388,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,    89,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   171,     0,     0,    15,    16,     0,    17,     0,   172,
       0,     0,    21,    96,  -388,     0,     0,     0,     0,  -388,
      28,   100,   173,  1057,     0,     0,    32,   174,   175,     0,
     176,    37,     0,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   177,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   178,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,     5,    89,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,    96,     0,     0,     0,     0,     0,
     179,    28,   100,   173,   180,     0,     0,    32,   174,   175,
       0,   176,    37,     0,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   177,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   178,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,    89,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   171,     0,     0,    15,    16,     0,    17,
       0,   172,     0,     0,    21,    96,     0,     0,     0,     0,
       0,   179,    28,   100,   173,   641,     0,     0,    32,   174,
     175,     0,   176,    37,     0,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     177,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   178,     0,    73,     0,  1014,    75,     0,
       0,     0,    79,     0,     0,    81,   817,     0,    83,     0,
       0,   222,   223,   819,   224,     0,     0,     0,     0,   225,
       0,     0,     0,     0,     0,     0,     0,   226,     0,     0,
       0,     0,     0,   822,    89,     0,     0,     0,   228,     0,
       0,   229,     0,     0,   230,     0,   825,     0,     0,     0,
       0,     0,   231,     0,     0,     0,     0,     0,   827,   233,
       0,     0,     0,     0,   100,   234,   180,     0,     0,     0,
       0,     0,     0,     0,   235,     0,     0,     0,     0,     0,
       0,     0,     0,   236,   237,     0,   238,   342,   239,     0,
    1015,     0,     0,   832,   343,     0,     0,   242,     0,     0,
     243,     0,     0,   244,     0,     0,   344,     0,     0,     0,
       0,     0,   372,   373,   374,   375,   376,   377,     0,     0,
     380,   381,   382,   383,     0,   385,   386,   835,   836,   837,
     838,   839,     0,     0,   840,     0,     0,     0,   841,   842,
     843,   844,   845,   846,   847,   848,   849,   850,   851,     0,
     852,     0,     0,   853,   854,   855,   856,   345,     0,     0,
       0,   346,     0,     0,     0,     0,     0,   342,     0,     0,
     441,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,   347,   372,   373,   374,   375,   376,   377,     0,
       0,   380,   381,   382,   383,     0,   385,   386,   348,   349,
       0,   350,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,   359,   360,     0,   361,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,   345,   400,     0,
       0,   346,     0,     0,     0,   371,     0,   342,     0,     0,
     885,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,   347,   372,   373,   374,   375,   376,   377,     0,
       0,   380,   381,   382,   383,     0,   385,   386,   348,   349,
       0,   350,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,   359,   360,     0,   361,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,   345,   400,     0,
       0,   346,     0,     0,     0,   371,     0,   342,     0,     0,
     892,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,   347,   372,   373,   374,   375,   376,   377,     0,
       0,   380,   381,   382,   383,     0,   385,   386,   348,   349,
       0,   350,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,   359,   360,     0,   361,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,   345,   400,     0,
       0,   346,     0,     0,     0,   371,     0,   342,     0,     0,
    1028,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,   347,   372,   373,   374,   375,   376,   377,     0,
       0,   380,   381,   382,   383,     0,   385,   386,   348,   349,
       0,   350,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,   359,   360,     0,   361,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,   345,   400,     0,
       0,   346,     0,     0,     0,   371,     0,   342,     0,     0,
    1029,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,   347,   372,   373,   374,   375,   376,   377,     0,
       0,   380,   381,   382,   383,     0,   385,   386,   348,   349,
       0,   350,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,   359,   360,     0,   361,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,   345,   400,     0,
       0,   346,     0,     0,     0,   371,     0,   342,     0,     0,
    1030,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,   347,   372,   373,   374,   375,   376,   377,     0,
       0,   380,   381,   382,   383,     0,   385,   386,   348,   349,
       0,   350,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,   359,   360,     0,   361,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,   345,   400,     0,
       0,   346,     0,     0,     0,   371,     0,   342,     0,     0,
    1031,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,   347,   372,   373,   374,   375,   376,   377,     0,
       0,   380,   381,   382,   383,     0,   385,   386,   348,   349,
       0,   350,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,   359,   360,     0,   361,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,   345,   400,     0,
       0,   346,     0,     0,     0,   371,     0,   342,     0,     0,
    1053,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,   347,   372,   373,   374,   375,   376,   377,     0,
       0,   380,   381,   382,   383,     0,   385,   386,   348,   349,
       0,   350,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,   359,   360,     0,   361,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,   345,   400,     0,
       0,   346,     0,     0,     0,   371,     0,     0,     0,     0,
    1054,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     342,     0,     0,     0,     0,     0,     0,   343,     0,     0,
       0,     0,   347,   372,   373,   374,   375,   376,   377,   344,
     434,   380,   381,   382,   383,     0,   385,   386,   348,   349,
       0,   350,   351,   352,   435,   353,   354,   355,     0,   356,
     357,   358,   359,   360,     0,   361,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,   342,   400,     0,
       0,     0,     0,     0,   343,   371,     0,     0,     0,     0,
     345,     0,     0,     0,   346,     0,   344,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   348,   349,     0,   350,   351,   352,   345,   353,   354,
     355,   346,   356,   357,   358,   359,   360,     0,   361,   362,
     363,   364,   365,   366,   367,     0,     0,   368,   369,   370,
       0,   400,   342,     0,     0,     0,     0,     0,   371,   343,
       0,     0,   347,     0,     0,     0,     0,     0,     0,     0,
       0,   344,     0,     0,     0,     0,   438,     0,   348,   349,
       0,   350,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,   359,   360,     0,   361,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,   439,   400,   342,
     688,     0,     0,     0,     0,   371,   343,     0,     0,     0,
       0,     0,   345,     0,     0,     0,   346,     0,   344,     0,
       0,     0,     0,   689,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,   690,     0,     0,     0,     0,     0,
       0,     0,     0,   348,   349,     0,   350,   351,   352,   345,
     353,   354,   355,   346,   356,   357,   358,   359,   360,     0,
     361,   362,   363,   364,   365,   366,   367,     0,   440,   368,
     369,   370,   342,     0,     0,     0,     0,     0,     0,   343,
     371,     0,     0,     0,   347,     0,     0,     0,     0,     0,
       0,   344,   751,     0,     0,     0,     0,     0,     0,     0,
     348,   349,     0,   350,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,   359,   360,     0,   361,   362,   363,
     364,   365,   366,   367,     0,     0,   368,   369,   370,     0,
       0,     0,     0,     0,     0,     0,     0,   371,     0,     0,
       0,     0,   345,     0,     0,     0,   346,     0,   342,     0,
       0,     0,     0,     0,     0,   343,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,     0,     0,
       0,     0,     0,   431,     0,     0,     0,   347,     0,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   348,   349,     0,   350,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,   359,   360,     0,
     361,   362,   363,   364,   365,   366,   367,     0,   345,   368,
     369,   370,   346,   400,   342,     0,     0,     0,     0,     0,
     371,   343,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   344,   252,     0,     0,     0,     0,   166,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   348,
     349,     0,   350,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,   359,   360,     0,   361,   362,   363,   364,
     365,   366,   367,     0,   345,   368,   369,   370,   346,     0,
     342,     0,     0,     0,     0,     0,   371,   343,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   344,
     573,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,   574,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   348,   349,     0,   350,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,   359,
     360,     0,   361,   362,   363,   364,   365,   366,   367,     0,
     345,   368,   369,   370,   346,   400,   342,     0,     0,     0,
       0,     0,   371,   343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,   575,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
     576,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   348,   349,     0,   350,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,   359,   360,     0,   361,   362,
     363,   364,   365,   366,   367,     0,   345,   368,   369,   370,
     346,     0,   342,   683,     0,     0,     0,     0,   371,   343,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   348,   349,     0,
     350,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,   359,   360,     0,   361,   362,   363,   364,   365,   366,
     367,     0,   345,   368,   369,   370,   346,     0,     0,     0,
       0,     0,   342,     0,   371,     0,     0,     0,     0,   343,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,   743,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   348,   349,     0,   350,   351,   352,     0,
     353,   684,   355,     0,   356,   357,   358,   359,   360,     0,
     361,   362,   363,   364,   365,   366,   367,     0,   685,   368,
     369,   370,   345,     0,     0,     0,   346,     0,   342,     0,
     371,     0,     0,     0,     0,   343,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   348,   349,     0,   350,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,   359,   360,     0,
     361,   362,   363,   364,   365,   366,   367,     0,   345,   368,
     369,   370,   346,   400,     0,     0,     0,     0,   342,     0,
     371,   760,     0,     0,     0,   343,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   348,
     349,     0,   350,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,   359,   360,     0,   361,   362,   363,   364,
     365,   366,   367,     0,     0,   368,   369,   370,   345,   400,
       0,     0,   346,     0,   342,     0,   371,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   344,     0,     0,     0,     0,   935,   431,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   348,
     349,     0,   350,   351,   352,     0,   353,   354,   355,   936,
     356,   357,   358,   359,   360,     0,   361,   362,   363,   364,
     365,   366,   367,     0,   345,   368,   369,   370,   346,     0,
     342,     0,     0,     0,   797,     0,   371,   343,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   344,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   348,   349,     0,   350,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,   359,
     360,     0,   361,   362,   363,   364,   365,   366,   367,     0,
     345,   368,   369,   370,   346,     0,   342,     0,     0,     0,
       0,     0,   371,   343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   348,   349,     0,   350,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,   359,   360,     0,   361,   362,
     363,   364,   365,   366,   367,     0,   345,   368,   369,   370,
     346,   471,     0,     0,     0,     0,   342,     0,   371,     0,
       0,     0,     0,   343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   348,   349,     0,
     350,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,   359,   360,     0,   361,   362,   363,   364,   365,   366,
     367,     0,   475,   368,   369,   370,   345,     0,     0,     0,
     346,     0,     0,     0,   371,     0,   342,     0,     0,     0,
       0,     0,     0,   343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   348,   349,     0,
     350,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,   359,   360,     0,   361,   362,   363,   364,   365,   366,
     367,     0,   477,   368,   369,   370,   345,     0,     0,     0,
     346,     0,     0,     0,   371,     0,   342,     0,     0,     0,
       0,     0,     0,   343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   348,   349,     0,
     350,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,   359,   360,     0,   361,   362,   363,   364,   365,   366,
     367,     0,   479,   368,   369,   370,   345,     0,     0,     0,
     346,     0,     0,     0,   371,     0,   342,     0,     0,     0,
       0,     0,     0,   343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   348,   349,     0,
     350,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,   359,   360,     0,   361,   362,   363,   364,   365,   366,
     367,     0,   484,   368,   369,   370,   345,     0,     0,     0,
     346,     0,   342,     0,   371,     0,     0,     0,     0,   343,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,     0,     0,     0,     0,     0,     0,     0,     0,
     501,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   348,   349,     0,
     350,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,   359,   360,     0,   361,   362,   363,   364,   365,   366,
     367,     0,   345,   368,   369,   370,   346,     0,     0,     0,
       0,     0,   342,     0,   371,   577,     0,     0,     0,   343,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   348,   349,     0,   350,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,   359,   360,     0,
     361,   362,   363,   364,   365,   366,   367,     0,     0,   368,
     369,   370,   345,     0,     0,     0,   346,     0,   342,     0,
     371,     0,     0,     0,     0,   343,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   348,   349,     0,   350,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,   359,   360,     0,
     361,   362,   363,   364,   365,   366,   367,     0,   345,   368,
     369,   370,   346,     0,   342,     0,   664,     0,     0,     0,
     371,   343,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   348,
     349,     0,   350,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,   359,   360,  -510,   361,   362,   363,   364,
     365,   366,   367,     0,   345,   368,   369,   370,   346,     0,
     342,     0,     0,     0,     0,     0,   371,   343,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   344,
       0,     0,     0,     0,     0,   166,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   348,   349,     0,   350,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,   359,
     360,     0,   361,   362,   363,   364,   365,   366,   367,     0,
     345,   368,   369,   370,   346,     0,     0,     0,     0,     0,
     342,   755,   371,     0,     0,     0,     0,   343,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   344,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   348,   349,     0,   350,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,   359,   360,     0,   361,   362,
     363,   364,   365,   366,   367,     0,   728,   368,   369,   370,
     345,     0,     0,     0,   346,     0,   342,     0,   371,     0,
       0,     0,     0,   343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,     0,     0,   758,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   348,   349,     0,   350,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,   359,   360,     0,   361,   362,
     363,   364,   365,   366,   367,     0,   345,   368,   369,   370,
     346,     0,   342,     0,     0,     0,     0,     0,   371,   343,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   348,   349,     0,
     350,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,   359,   360,     0,   361,   362,   363,   364,   365,   366,
     367,     0,   345,   368,   369,   370,   346,     0,   342,     0,
       0,     0,     0,     0,   371,   343,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   348,   349,     0,   350,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,   359,   360,     0,
     361,   362,   363,   364,   365,   366,   367,     0,   345,   368,
     369,   370,   346,     0,   342,     0,   781,     0,     0,     0,
     371,   343,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   348,
     349,     0,   350,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,   359,   360,     0,   361,   362,   363,   364,
     365,   366,   367,     0,   345,   368,   369,   370,   346,     0,
     342,     0,   782,     0,     0,     0,   371,   343,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   344,
     872,     0,     0,     0,     0,     0,     0,     0,   800,   347,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   348,   349,     0,   350,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,   359,
     360,     0,   361,   362,   363,   364,   365,   366,   367,     0,
     345,   368,   369,   370,   346,     0,   342,     0,     0,     0,
       0,     0,   371,   343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,   875,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   348,   349,     0,   350,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,   359,   360,     0,   361,   362,
     363,   364,   365,   366,   367,     0,   345,   368,   369,   370,
     346,     0,   342,     0,     0,     0,     0,     0,   371,   343,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   348,   349,     0,
     350,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,   359,   360,     0,   361,   362,   363,   364,   365,   366,
     367,     0,   345,   368,   369,   370,   346,     0,   342,   934,
       0,     0,     0,     0,   371,   343,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   348,   349,     0,   350,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,   359,   360,     0,
     361,   362,   363,   364,   365,   366,   367,     0,   345,   368,
     369,   370,   346,     0,   342,     0,     0,     0,   877,     0,
     371,   343,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   348,
     349,     0,   350,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,   359,   360,     0,   361,   362,   363,   364,
     365,   366,   367,     0,   345,   368,   369,   370,   346,     0,
       0,     0,     0,     0,   342,     0,   371,   957,     0,     0,
       0,   343,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   344,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   348,   349,     0,   350,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,   359,
     360,     0,   361,   362,   363,   364,   365,   366,   367,     0,
       0,   368,   369,   370,   345,     0,     0,     0,   346,     0,
       0,     0,   371,     0,   342,     0,     0,   960,     0,     0,
       0,   343,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   344,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   348,   349,     0,   350,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,   359,
     360,     0,   361,   362,   363,   364,   365,   366,   367,     0,
       0,   368,   369,   370,   345,     0,     0,     0,   346,     0,
     342,     0,   371,     0,     0,     0,     0,   343,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   344,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   348,   349,     0,   350,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,   359,
     360,     0,   361,   362,   363,   364,   365,   366,   367,     0,
     345,   368,   369,   370,   346,     0,   342,     0,  1013,     0,
       0,     0,   371,   343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   348,   349,     0,   350,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,   359,   360,     0,   361,   362,
     363,   364,   365,   366,   367,     0,   345,   368,   369,   370,
     346,     0,     0,     0,     0,     0,  1049,   342,   371,  1066,
       0,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   348,   349,     0,
     350,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,   359,   360,     0,   361,   362,   363,   364,   365,   366,
     367,     0,     0,   368,   369,   370,     0,   345,     0,     0,
       0,   346,     0,     0,   371,     0,     0,   342,     0,     0,
    1067,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,   347,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   348,   349,
       0,   350,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,   359,   360,     0,   361,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,   345,     0,     0,
       0,   346,     0,     0,     0,   371,     0,   342,     0,     0,
    1068,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,   347,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   348,   349,
       0,   350,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,   359,   360,     0,   361,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,   345,     0,     0,
       0,   346,     0,     0,     0,   371,     0,   342,     0,     0,
    1069,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,   347,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   348,   349,
       0,   350,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,   359,   360,     0,   361,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,   345,     0,     0,
       0,   346,     0,     0,     0,   371,     0,   342,     0,     0,
    1070,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,   347,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   348,   349,
       0,   350,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,   359,   360,     0,   361,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,   345,     0,     0,
       0,   346,     0,     0,     0,   371,     0,     0,     0,     0,
    1071,     0,     0,     0,   342,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   347,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   348,   349,
       0,   350,   351,   352,     0,   353,   354,   355,  1095,   356,
     357,   358,   359,   360,     0,   361,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,     0,     0,     0,
       0,     0,     0,     0,   345,   371,     0,     0,   346,     0,
     342,     0,     0,     0,     0,     0,     0,   343,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   344,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   348,   349,     0,   350,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,   359,
     360,     0,   361,   362,   363,   364,   365,   366,   367,     0,
     345,   368,   369,   370,   346,     0,   342,     0,     0,     0,
       0,     0,   371,   343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   348,   349,     0,   350,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,   359,   360,     0,   361,   362,
     363,   364,   365,   366,   367,     0,   345,   368,   369,   370,
     346,     0,   342,     0,     0,     0,  1124,     0,   371,   343,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   348,   349,     0,
     350,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,   359,   360,     0,   361,   362,   363,   364,   365,   366,
     367,     0,   716,   368,   369,   370,   346,     0,     0,     0,
       0,     0,     0,     0,   371,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   348,   349,     0,   350,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,   359,   360,     0,
     361,   362,   363,   364,   365,   366,   367,     0,   267,   368,
     369,   370,    22,    23,     0,     0,     0,     0,     0,     0,
     371,   268,   267,    30,   269,     0,    22,    23,    35,     0,
       0,     0,     0,     0,    39,   268,     0,    30,   269,     0,
       0,     0,    35,     0,     0,     0,   344,     0,    39,     0,
       0,     0,     0,     0,     0,     0,    52,     0,    54,     0,
      56,     0,   940,     0,     0,   941,     0,   270,     0,    62,
      52,     0,    54,     0,    56,     0,   940,     0,     0,   941,
       0,   270,     0,    62,     0,     0,   344,     0,    78,     0,
       0,    80,     0,     0,    82,     0,     0,   345,     0,     0,
       0,   346,    78,     0,     0,    80,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,   347,     0,     0,     0,     0,   345,     0,     0,
       0,   346,     0,     0,    97,     0,     0,     0,   348,   349,
    1072,   350,   351,   352,     0,   353,   354,   355,    97,   356,
     357,   358,   359,     0,  1073,   361,   362,   363,   364,   365,
     366,   367,   347,     0,   368,   369,   370,   345,     0,     0,
       0,   346,     0,     0,     0,   371,     0,     0,   348,   349,
       0,   350,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,   359,     0,     0,   361,   362,   363,   364,   365,
       0,   367,     0,     0,   368,   369,   370,     0,     0,     0,
       0,     0,     0,     0,     0,   371,     0,     0,   348,   349,
       0,   350,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,   359,     0,     0,   361,   362,   363,   364,   365,
     693,   367,   221,     0,   368,   369,   370,     0,     0,     0,
       0,     0,     0,     0,     0,   371,   222,   223,     0,   224,
       0,     0,     0,     0,   225,     0,     0,     0,     0,     0,
       0,     0,   226,     0,     0,     0,     0,     0,   227,  -222,
       0,     0,     0,   228,     0,     0,   229,     0,     0,   230,
       0,     0,     0,  -222,  -222,     0,  -222,   231,     0,     0,
       0,  -222,     0,   232,   233,     0,     0,     0,     0,  -222,
     234,     0,     0,     0,     0,  -222,     0,     0,     0,   235,
    -222,     0,     0,  -222,     0,     0,  -222,     0,   236,   237,
       0,   238,     0,   239,  -222,   240,     0,  -232,   241,     0,
    -222,  -222,   242,     0,     0,   243,     0,  -222,   244,     0,
       0,  -232,  -232,     0,  -232,     0,  -222,     0,     0,  -232,
       0,     0,     0,     0,     0,  -222,  -222,  -232,  -222,     0,
    -222,     0,  -222,  -232,   221,  -222,     0,     0,  -232,  -222,
       0,  -232,  -222,     0,  -232,  -222,     0,     0,   222,   223,
       0,   224,  -232,     0,     0,     0,   225,     0,  -232,  -232,
       0,     0,     0,     0,   226,  -232,     0,     0,     0,     0,
     227,     0,     0,     0,  -232,   228,     0,     0,   229,     0,
       0,   230,     0,  -232,  -232,     0,  -232,     0,  -232,   231,
    -232,     0,  -223,  -232,     0,   232,   233,  -232,     0,     0,
    -232,     0,   234,  -232,     0,     0,  -223,  -223,     0,  -223,
       0,   235,     0,     0,  -223,     0,     0,     0,     0,     0,
     236,   237,  -223,   238,     0,   239,     0,   240,  -223,  -154,
     241,     0,     0,  -223,   242,     0,  -223,   243,     0,  -223,
     244,     0,     0,  -154,  -154,     0,  -154,  -223,     0,     0,
       0,  -154,     0,  -223,  -223,     0,     0,     0,     0,  -154,
    -223,     0,     0,     0,     0,  -154,     0,     0,     0,  -223,
    -154,     0,     0,  -154,     0,     0,  -154,     0,  -223,  -223,
       0,  -223,     0,  -223,  -154,  -223,     0,  -150,  -223,     0,
    -154,  -154,  -223,     0,     0,  -223,     0,  -154,  -223,     0,
       0,  -150,  -150,     0,  -150,     0,  -154,     0,     0,  -150,
       0,     0,     0,     0,     0,  -154,  -154,  -150,  -154,     0,
    -154,     0,  -154,  -150,     0,  -154,     0,     0,  -150,  -154,
       0,  -150,  -154,     0,  -150,  -154,     0,     0,     0,     0,
       0,   267,  -150,     0,     0,    22,    23,     0,  -150,  -150,
       0,     0,     0,     0,   268,  -150,    30,   269,     0,     0,
       0,    35,     0,     0,  -150,     0,     0,    39,     0,     0,
       0,     0,     0,  -150,  -150,     0,  -150,     0,  -150,     0,
    -150,     0,     0,  -150,     0,     0,     0,  -150,     0,    52,
    -150,    54,     0,  -150,     0,     0,     0,     0,     0,     0,
     270,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    78,     0,     0,    80,     0,     0,    82
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1019)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      12,   151,   220,    59,    16,     2,   393,   480,    20,   331,
     137,   449,   192,   215,    26,   603,   776,   702,    30,    31,
       1,    33,    34,    35,    36,   690,   689,   100,   208,     3,
       1,    46,    44,    19,    46,    22,   862,   863,   808,    51,
     907,    53,   636,    55,    63,    46,    20,   265,   200,     3,
      32,    63,    64,    65,    66,    67,    68,     1,   111,    33,
      72,   990,    74,   158,    76,    77,   193,  1085,   220,    81,
     288,    85,    84,    85,    46,    87,     3,    89,    45,   113,
      92,    93,   101,    95,    58,   904,   139,    99,   100,   101,
     102,   591,    32,    32,   594,    32,    32,    68,   113,     1,
    1035,    56,    46,   139,    68,   118,  1124,   867,    46,    85,
     165,    83,   167,   265,     0,   113,   446,   439,   320,   321,
      46,   715,    96,     1,   139,   713,    51,   200,   102,   454,
     143,   113,   168,    18,   168,   147,   288,   118,   139,   110,
     107,    46,    96,  1010,    64,   295,   110,   220,   908,   163,
    1085,    46,    77,   168,    56,   480,   111,   138,   139,   171,
     979,   180,   174,   175,   176,     3,   178,   179,   180,    96,
     168,   389,   390,   943,   118,  1104,   113,     3,    56,   773,
     100,   163,   158,    85,   113,   166,    71,   163,   200,   163,
    1016,  1017,   265,   164,   138,   139,   966,   635,   968,   139,
     164,   139,   354,    83,    89,   191,   139,    85,   220,   111,
     266,    33,    34,   139,   158,   288,   603,   204,   978,    27,
      98,   106,   209,   163,   163,   139,   163,   163,  1102,   216,
     168,   164,   732,   111,   139,   735,   138,   389,   390,     8,
      48,   881,   168,   883,   139,    46,  1120,    85,    27,    27,
     139,   139,   166,   265,  1024,   473,  1026,   269,    96,  1040,
     138,   163,    70,   168,    72,   277,   278,    46,   953,   139,
      49,   936,   935,   168,  1034,    83,   288,   166,   166,   101,
     158,   139,    83,   133,   165,   163,   167,   139,   139,   165,
      69,   141,   419,    72,    72,    74,   166,   424,   165,   165,
     108,   167,   890,   315,    83,    83,   164,   158,  1089,  1090,
    1091,  1092,  1093,  1094,   166,   312,   389,   390,   330,   331,
     165,   473,   167,   102,   102,   965,   713,   967,   340,   767,
     342,   343,   344,   345,   346,   347,   165,   349,   350,   351,
     352,   353,   139,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     139,   118,   139,   118,   139,    27,   378,   379,   690,   166,
      93,   139,   384,   165,   139,   387,   388,   389,   390,   391,
     392,   393,   139,   163,   139,   139,   143,   166,   143,   166,
     402,   166,    31,   466,    27,   407,    78,   409,   166,   164,
     473,   165,     3,   167,   158,   607,   608,   426,   204,   166,
      72,   166,   158,   209,   426,   488,   428,   429,   491,    20,
     493,    83,   434,   435,    82,   498,   438,   439,    86,   441,
      45,   443,    33,    44,   139,   765,   157,   589,   158,    72,
     102,   914,    56,    82,   456,    60,   108,    86,   460,   461,
      83,    27,   133,   158,   466,    66,   158,    58,   378,   379,
     141,   473,   164,    78,   384,    76,    77,   387,   388,   671,
     672,    85,   139,    82,   158,   108,   488,    86,   808,   491,
     802,   493,   662,   149,    98,   497,   498,   499,   158,   501,
     502,   158,   107,   512,   133,    96,    72,   111,   158,   138,
     512,   102,   141,   890,   164,   158,   145,    83,   118,   158,
     507,   139,   158,    56,   587,   158,   589,    56,   165,   165,
     139,   167,   158,   345,   346,   491,   102,   493,     2,   163,
     158,   683,   684,    31,   144,   139,   146,   147,    12,   158,
     150,   151,    85,   112,   158,   158,    85,    81,   133,   163,
      81,    25,   702,    27,   158,    98,   141,   139,   165,    98,
     165,   573,   574,   575,   576,   577,   578,   579,   111,   391,
     392,   491,   111,   493,   165,   587,   158,   589,   133,   190,
     402,   165,   907,   138,    82,   597,   141,   599,    86,   914,
     145,   603,   158,   605,   580,   581,    76,    77,    72,   118,
     165,   212,   213,   214,   215,    31,   158,   429,   219,     8,
      60,   138,    27,   435,   936,   158,   163,   141,   948,   158,
     163,   158,   641,   118,   163,    99,   166,   639,   640,   641,
     104,    46,   139,   645,    49,   133,   966,   158,   968,    46,
     138,    34,   140,   141,   255,   165,   139,   145,   166,   166,
     261,   118,   166,   166,    69,   153,    82,    72,    21,    74,
      86,   159,   160,   161,   139,   168,   168,   587,    83,   163,
     158,   158,   168,   284,   686,   497,   688,   689,   690,   165,
     158,    27,   158,   166,   139,  1010,   158,   102,   158,   158,
     158,   302,    34,   167,  1024,   166,  1026,   915,   916,   164,
      46,   713,    31,    49,   716,   717,   718,   133,   134,   721,
    1035,   137,   138,   166,   140,   141,   139,   164,   118,   145,
     166,    83,    32,    69,  1049,   645,    72,   153,    74,   741,
      32,   743,   168,   159,   160,   161,   748,    83,   163,   751,
      34,   163,   138,   755,    83,   165,   758,   168,   165,   138,
     168,   165,   574,    82,   576,   168,   102,    86,   164,   141,
    1085,   158,   141,   915,   916,   141,   785,   158,   158,   133,
      66,   245,   141,   785,   141,   141,   133,  1102,   252,    46,
     141,   141,   141,   795,   133,   797,   141,   141,   800,    56,
     802,   141,   403,   133,    34,  1120,   133,    98,   810,  1124,
     158,   166,   158,   953,   133,   134,   133,   136,   137,   138,
     163,   140,   141,   142,   166,   163,   145,   133,    85,   166,
     139,   166,   433,   152,   153,   157,   437,   156,   327,   157,
     159,   160,   161,   168,   907,    24,   296,   789,   312,   953,
     948,   170,   915,   916,   111,   857,   811,   765,  1104,  1034,
    1062,   118,   628,   327,   814,  1095,  1088,    35,   457,   871,
     872,   589,   874,   875,   876,   877,    30,    -1,    -1,    -1,
      -1,    -1,   139,   885,   886,    -1,   888,    -1,   890,    -1,
     892,   893,    -1,    -1,   896,   941,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   716,   717,   163,    -1,    -1,   166,
      -1,   913,   914,   915,   916,    -1,    -1,    -1,    -1,   921,
      -1,   923,   985,    -1,    -1,   927,   212,   213,   214,   215,
      -1,    -1,   934,   935,   936,    -1,   400,    -1,    -1,    -1,
       2,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,   957,   958,   959,   960,   961,
     962,   963,   964,    25,   428,    27,    -1,    -1,    -1,   255,
     434,   980,  1035,    -1,    -1,   261,    -1,   441,   980,     2,
      -1,    -1,    -1,   985,    -1,    31,    -1,   989,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   284,    -1,
     601,   602,    25,    -1,    27,    -1,   607,   608,    -1,    -1,
      72,    -1,    -1,    -1,    76,    77,   302,    -1,   619,    -1,
      -1,    -1,  1085,    -1,    -1,    -1,  1028,  1029,  1030,  1031,
      -1,   495,    -1,    -1,    -1,   499,    82,    99,    -1,    -1,
      86,    -1,   104,   507,    -1,    -1,    31,  1049,  1057,    72,
      -1,  1053,  1054,    -1,    -1,  1057,    -1,   658,    -1,    -1,
      -1,  1124,    -1,    -1,  1066,  1067,  1068,  1069,  1070,  1071,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,
      -1,   104,  1084,  1085,    -1,  1087,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,    -1,    82,    -1,   145,
      -1,    86,    -1,    -1,    -1,   167,   570,   153,    -1,    -1,
      -1,    -1,    -1,   159,   160,   161,    -1,   403,  1120,    -1,
      -1,    -1,  1124,    -1,    -1,   726,   727,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   738,     1,   740,
      -1,    -1,    -1,    -1,   167,    -1,   747,   433,   133,   750,
      -1,   437,   753,   138,   618,   140,   141,    -1,    -1,    22,
     145,    -1,    -1,    26,    27,    -1,    -1,    -1,   153,    -1,
       2,    46,    35,    -1,    37,    38,   161,    -1,    -1,    42,
      12,    56,    46,   245,    -1,    48,    -1,    -1,    51,    -1,
     252,   792,    56,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   670,    70,    -1,    72,
      85,    74,   676,    76,    77,    -1,    79,    -1,    81,    -1,
      83,    85,   245,    -1,    -1,    -1,    -1,    31,    -1,   252,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,   702,   102,
      72,    -1,   105,   118,    -1,   108,    -1,   111,    -1,    -1,
     312,    -1,   716,   717,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   327,    -1,    99,    -1,    -1,
      -1,    -1,   104,    -1,    -1,   139,    -1,   741,    82,   743,
      -1,    -1,    86,   158,   748,    -1,    -1,   751,   163,   312,
      -1,   166,    -1,   757,   158,   158,   760,    -1,    -1,   163,
      -1,   164,   166,    -1,   327,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   601,   602,    -1,   919,    -1,
     784,   607,   608,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   795,    -1,   797,   138,   167,   140,   141,   400,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,    -1,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   428,    -1,     2,   970,
     971,    -1,   434,   974,   975,    -1,    -1,   400,    12,   441,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   990,
      -1,    25,    -1,    27,    -1,    -1,    -1,   998,    -1,  1000,
      -1,  1002,    -1,    -1,    -1,   428,    -1,     2,    -1,    -1,
      -1,   434,    -1,   245,    -1,    -1,    -1,    12,   441,    -1,
     252,   885,    -1,    -1,    -1,    -1,    -1,    -1,   892,    -1,
      25,    -1,    27,   495,    -1,    -1,    -1,   499,    72,   903,
     726,   727,    -1,    -1,    -1,   507,    -1,    -1,    -1,    -1,
      -1,    -1,   738,   917,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   926,    -1,   750,    99,   930,   753,    -1,    -1,
     104,    -1,   495,    -1,    -1,    -1,   499,    72,    -1,    -1,
     312,    -1,    -1,    -1,   507,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   327,    -1,    -1,    -1,    -1,
      -1,    22,    -1,  1104,    99,    26,    27,    -1,    -1,   104,
      -1,    -1,   976,    -1,    35,    -1,    37,    38,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    76,   618,    -1,    79,    -1,
      81,    -1,    83,    -1,  1028,  1029,  1030,  1031,   400,    -1,
      -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,  1053,
    1054,    -1,    -1,    -1,    -1,   618,   428,    -1,    -1,     3,
      -1,    -1,   434,    -1,    -1,    -1,    -1,    -1,   670,   441,
      -1,   245,    -1,    17,   676,    -1,    20,    -1,   252,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,   919,    -1,    -1,    -1,   158,    -1,    -1,
      44,    -1,    -1,   164,    -1,    -1,    50,   670,    -1,    -1,
     245,    -1,    -1,   676,    58,    -1,    -1,   252,    -1,    -1,
      -1,    65,    -1,   495,    -1,    -1,    -1,   499,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   507,    80,    -1,   312,   741,
      -1,   743,    -1,    -1,    -1,    -1,   748,    -1,    92,   751,
      -1,    -1,    96,   327,    -1,   757,    -1,    -1,   760,   103,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   312,   741,    -1,
     743,    -1,   784,    -1,    -1,   748,    -1,    -1,   751,    -1,
      -1,    -1,   327,   795,   757,   797,    -1,   760,    -1,    -1,
       2,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   784,    -1,    25,    -1,    27,   400,    -1,    -1,    -1,
      -1,    -1,   795,    -1,   797,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   618,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   428,    -1,    -1,    -1,    -1,    22,
     434,    -1,    -1,    26,    27,   400,    -1,   441,    -1,    -1,
      72,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,    42,
      -1,    -1,    -1,   885,    -1,    48,    -1,    -1,    -1,    -1,
     892,    -1,    -1,   428,    -1,    -1,    -1,    99,   670,   434,
      -1,   903,   104,    -1,   676,    -1,   441,    70,    -1,    72,
      -1,    74,    -1,    76,    -1,   917,    79,    -1,    81,    -1,
      83,   495,   885,    -1,   926,   499,    -1,    -1,   930,   892,
      -1,    -1,    -1,   507,    -1,    -1,    -1,    -1,    -1,   102,
     903,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   917,    -1,    -1,    -1,    -1,    -1,
     495,    -1,    -1,   926,   499,   167,    -1,   930,    -1,   741,
      -1,   743,   507,    -1,   976,    -1,   748,    -1,    -1,   751,
      -1,    -1,    -1,    -1,    -1,   757,    -1,    -1,   760,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,
      -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   784,   976,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   795,    -1,   797,  1028,  1029,  1030,  1031,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,
      -1,    -1,    -1,   245,   618,    -1,    -1,    12,    -1,    -1,
     252,  1053,  1054,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,  1028,  1029,  1030,  1031,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,   618,    37,    38,    -1,    -1,    -1,    42,
    1053,  1054,    -1,    -1,    -1,    48,   670,    -1,    -1,    -1,
      -1,    -1,   676,    -1,    -1,    -1,    -1,    72,    -1,    -1,
     312,    -1,    -1,   885,    -1,    -1,    -1,    70,    -1,    72,
     892,    74,    -1,    76,    -1,   327,    79,    -1,    81,    -1,
      83,   903,    -1,    -1,    99,   670,    -1,    -1,    -1,   104,
      -1,   676,    -1,    -1,    -1,   917,    -1,    -1,    -1,   102,
      -1,    -1,   105,    -1,   926,   108,    -1,    -1,   930,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   741,    -1,   743,
      -1,    -1,    -1,    -1,   748,    -1,    -1,   751,    -1,    -1,
      -1,    -1,    -1,   757,    -1,    -1,   760,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   400,    -1,
      41,    -1,   167,    -1,   976,   158,   741,    -1,   743,    -1,
     784,   164,    -1,   748,    -1,    -1,   751,    -1,    -1,    -1,
      -1,   795,   757,   797,    -1,   760,   428,    -1,     2,    -1,
      -1,    -1,   434,    -1,    -1,    -1,    -1,    78,    12,   441,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   784,
      -1,    25,    -1,    27,    -1,    -1,  1028,  1029,  1030,  1031,
     795,    -1,   797,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,    -1,    -1,
     245,  1053,  1054,    -1,    -1,    -1,    -1,   252,    -1,   130,
      -1,   132,    -1,   495,   149,    -1,    -1,   499,    72,    -1,
      -1,    -1,    -1,    -1,    -1,   507,    -1,   162,    -1,    -1,
     165,   885,   167,    -1,    -1,    -1,    -1,    -1,   892,     3,
      -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,   903,
     104,    -1,    -1,    17,    18,    -1,    20,    31,    -1,    -1,
      -1,    25,    -1,   917,    -1,    -1,    -1,   312,    -1,    33,
     885,   192,   926,    -1,    -1,    39,   930,   892,    -1,    -1,
      44,    -1,   327,    47,    -1,    -1,    50,   208,   903,   210,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      64,    65,   917,    -1,    -1,    -1,    -1,    71,    82,    -1,
      -1,   926,    86,   167,    -1,   930,    80,    -1,    -1,    -1,
      -1,    -1,   976,    -1,    -1,    89,    90,   248,    92,    -1,
      94,    -1,    96,    -1,    -1,    99,   618,    -1,    -1,   103,
     104,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,   273,    -1,    -1,   400,    -1,    -1,    -1,   133,
     134,   976,   136,   137,   138,    -1,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,  1028,  1029,  1030,  1031,   152,   153,
      -1,    -1,   156,   428,    -1,   159,   160,   161,   670,   434,
      -1,   245,    -1,    -1,   676,    -1,   441,    -1,   252,  1053,
    1054,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1028,  1029,  1030,  1031,    -1,    -1,    -1,
      -1,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,  1053,  1054,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     495,    -1,    -1,    -1,   499,    -1,    -1,    -1,   312,   741,
      -1,   743,   507,    -1,    -1,   162,   748,    -1,   165,   751,
     167,    -1,    -1,   327,    -1,   757,     3,    -1,   760,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,    -1,    -1,   417,    -1,    25,    -1,
     421,    -1,   784,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,   795,    -1,   797,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    -1,   449,    -1,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    64,    65,    -1,
      -1,    -1,    -1,    -1,    71,    -1,   400,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    -1,    92,    -1,    94,    -1,    96,
      -1,    -1,    99,   618,   428,    -1,   103,    -1,    -1,   106,
     434,    -1,   109,    -1,    -1,    -1,    -1,   508,    -1,    -1,
      -1,    -1,    -1,   514,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   885,    22,    -1,    -1,    -1,    26,    27,
     892,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,
      38,   903,    -1,    -1,    42,   670,    -1,    -1,    -1,    -1,
      48,   676,    -1,    -1,    -1,   917,   163,    -1,   165,    -1,
      -1,   495,    -1,    -1,   926,   499,    -1,    -1,   930,    -1,
      -1,    -1,    70,   507,    72,    -1,    74,    -1,    76,    -1,
      -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,   590,
      -1,    -1,   593,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
     108,    -1,    -1,    -1,   976,    -1,   741,    -1,   743,    -1,
      -1,    -1,    -1,   748,    -1,    -1,   751,    -1,    -1,    -1,
     631,    -1,   757,    -1,   635,   760,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,   658,    20,   784,
     158,   662,    -1,    25,    -1,    -1,  1028,  1029,  1030,  1031,
     795,    33,   797,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    -1,   618,    47,    -1,    -1,    50,    -1,
      -1,  1053,  1054,    -1,    -1,    -1,    58,    -1,    -1,    -1,
      22,   702,    64,    65,    26,    27,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    80,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    89,    90,    51,
      92,    -1,    94,    -1,    96,    -1,   670,    99,    -1,    -1,
      -1,   103,   676,    -1,   106,    -1,    -1,   109,    70,    -1,
      72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    81,
     885,    83,    -1,   764,    -1,    -1,   767,   892,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   903,    -1,
     102,    -1,    -1,   105,    -1,   107,   108,    -1,   789,    -1,
      -1,    -1,   917,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   926,    -1,   165,    -1,   930,    -1,   741,    -1,   743,
     811,    -1,    -1,    -1,   748,    -1,    -1,   751,    -1,    -1,
      -1,    -1,    -1,   757,     3,    -1,   760,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,   837,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
     784,   976,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,   795,    -1,   797,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      82,    -1,    -1,    -1,    86,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,  1028,  1029,  1030,  1031,    -1,    -1,    -1,
      89,    90,    -1,    92,    -1,    94,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,   103,    -1,    -1,   106,  1053,  1054,
     109,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,    -1,   145,   146,   147,    -1,    -1,   150,   151,
     152,   153,   953,    -1,   156,    -1,    -1,   159,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,   903,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   917,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   926,    -1,    -1,    -1,   930,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     0,     1,    -1,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,   976,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    -1,    50,    51,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    62,    -1,
      64,    65,    66,    -1,    -1,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      84,    85,    -1,    87,    88,    89,    90,    91,    92,    93,
      94,    -1,    96,    97,    -1,    99,   100,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,    -1,    -1,   112,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,   156,   157,   158,    -1,    -1,   161,    -1,   163,
      -1,   165,    -1,   167,    -1,     1,   170,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    18,    19,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    -1,    50,    51,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    62,    -1,    64,    65,
      66,    -1,    -1,    -1,    70,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    -1,
      96,    97,    -1,    99,   100,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,    -1,    -1,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,   163,   164,   165,
      -1,   167,    -1,     1,   170,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    -1,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    -1,    64,    65,    66,    -1,
      -1,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    -1,    83,    84,    85,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    -1,    96,    97,
      -1,    99,   100,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,
     158,    -1,    -1,   161,    -1,   163,   164,   165,    -1,   167,
      -1,     1,   170,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    -1,
      50,    51,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    62,    -1,    64,    65,    66,    -1,    -1,    -1,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    87,    88,    89,
      90,    91,    92,    93,    94,    -1,    96,    97,    -1,    99,
     100,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
      -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,   158,    -1,
      -1,   161,    -1,   163,   164,   165,    -1,   167,    -1,     1,
     170,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,   163,   164,   165,    -1,   167,    -1,     1,   170,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    -1,    50,    51,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    62,    -1,
      64,    65,    66,    -1,    -1,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      84,    85,    -1,    87,    88,    89,    90,    91,    92,    93,
      94,    -1,    96,    97,    -1,    99,   100,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,    -1,    -1,   112,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,   156,   157,   158,    -1,    -1,   161,    -1,   163,
     164,   165,    -1,   167,    -1,     1,   170,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    -1,    50,    51,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    62,    -1,    64,    65,
      66,    -1,    -1,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    -1,    83,    84,    85,
      -1,    87,    88,    89,    90,    91,    92,    93,    94,    -1,
      96,    97,    -1,    99,   100,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,    -1,    -1,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,   158,    -1,    -1,   161,    -1,   163,   164,   165,
      -1,   167,    -1,     1,   170,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    -1,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    -1,    64,    65,    66,    -1,
      -1,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    -1,    83,    84,    85,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    -1,    96,    97,
      -1,    99,   100,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,
     158,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,
      -1,     1,   170,     3,    -1,     5,     6,     7,     8,     9,
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
      -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,     1,
     170,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,   163,    -1,   165,    -1,   167,    -1,     1,   170,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
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
      -1,   165,    -1,   167,    -1,     1,   170,     3,    -1,     5,
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
     156,   157,   158,    -1,    -1,   161,    -1,   163,    -1,   165,
      -1,   167,    -1,     1,   170,     3,    -1,     5,     6,     7,
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
     158,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,
      -1,     1,   170,     3,    -1,     5,     6,     7,     8,     9,
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
      -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,     1,
     170,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,   163,    -1,   165,    -1,   167,    -1,     1,   170,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
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
      -1,   165,    -1,   167,    -1,     1,   170,     3,    -1,     5,
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
     156,   157,   158,    -1,    -1,   161,    -1,   163,    -1,   165,
      -1,   167,    -1,     1,   170,     3,    -1,     5,     6,     7,
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
     158,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,
      -1,     1,   170,     3,     4,     5,     6,     7,     8,     9,
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
     170,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,   163,    -1,   165,    -1,   167,    -1,     3,   170,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      26,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,    65,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    -1,    75,
      -1,    -1,    -1,    -1,    80,    81,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,
     106,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,
      -1,   167,    -1,    -1,   170,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,
      58,    59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,
      -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    91,    92,    93,    94,    -1,    96,    -1,
      -1,    99,   100,   101,    -1,   103,   104,    -1,   106,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,
      -1,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,
      -1,    -1,   170,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
     100,   101,    -1,   103,   104,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,   161,    -1,   163,    -1,   165,   166,   167,    -1,    -1,
     170,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,
      62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,
      -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    96,    -1,    -1,    99,   100,   101,
      -1,   103,   104,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,
      -1,   163,    -1,   165,   166,   167,    -1,     3,   170,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      46,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,    65,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,
      -1,    -1,    -1,    -1,    80,    81,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,
     106,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,
      -1,   167,    -1,     3,   170,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,   158,    -1,
      -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,    -1,
     170,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,
      62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,
      -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    96,    -1,    -1,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,
      -1,   163,    -1,   165,    -1,   167,    -1,     3,   170,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    61,    62,    63,    64,    65,
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
      -1,   167,    -1,     3,   170,     5,     6,     7,     8,     9,
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
      -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,     3,
     170,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,   115,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,
      -1,   165,    -1,   167,    -1,    -1,   170,     3,     4,     5,
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
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,   115,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,
      -1,   167,    -1,     3,   170,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,    -1,
     170,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,   163,   164,   165,    -1,   167,    -1,    -1,   170,     3,
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
      -1,   165,    -1,   167,   168,    -1,   170,     3,     4,     5,
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
      -1,   167,   168,     3,   170,     5,     6,     7,     8,     9,
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
      -1,    -1,    -1,    -1,    -1,   115,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,     3,
     170,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
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
      -1,   165,    -1,   167,    -1,    -1,   170,     3,     4,     5,
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
      -1,   167,    -1,     3,   170,     5,     6,     7,     8,     9,
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
      -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,     3,
     170,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
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
      -1,   165,    -1,   167,   168,    -1,   170,     3,     4,     5,
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
      -1,   167,    -1,    -1,   170,     3,     4,     5,     6,     7,
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
      -1,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,
      -1,    -1,   170,     3,     4,     5,     6,     7,     8,     9,
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
      -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,     3,
     170,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
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
      -1,   165,    -1,   167,    -1,     3,   170,     5,     6,     7,
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
      -1,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,
      -1,     3,   170,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,   163,    -1,   165,    -1,   167,    -1,     3,   170,     5,
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
      -1,   167,    -1,     3,   170,     5,     6,     7,     8,     9,
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
      -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,     3,
     170,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
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
      -1,   165,    -1,   167,    -1,     3,   170,     5,     6,     7,
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
      -1,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,
      -1,     3,   170,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,   163,    -1,   165,    -1,   167,    -1,     3,   170,     5,
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
      -1,   167,    -1,     3,   170,     5,     6,     7,     8,     9,
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
      -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,     3,
     170,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
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
      -1,   165,    -1,   167,    -1,     3,   170,     5,     6,     7,
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
      -1,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,
      -1,     3,   170,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,   163,    -1,   165,    -1,   167,    -1,     3,   170,     5,
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
      -1,   167,    -1,     3,   170,     5,     6,     7,     8,     9,
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
      -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,     3,
     170,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    12,   142,    -1,
      -1,    -1,    17,    18,    19,    20,    -1,    -1,   152,    -1,
      25,   155,   156,   157,    -1,    -1,    -1,   161,    33,    -1,
      35,   165,    -1,   167,    39,    -1,   170,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    64,
      65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    -1,    92,    -1,    94,
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,   120,   121,   122,   123,    -1,
      -1,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,    -1,    -1,   140,    -1,    -1,    -1,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
      -1,   156,    -1,    -1,   159,   160,   161,   162,    -1,     3,
     165,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,    -1,   156,   157,    -1,    -1,    -1,   161,    -1,   163,
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
      99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
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
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,     3,   135,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,   157,    -1,    -1,    -1,    -1,    -1,
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
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    96,    -1,     3,    99,    -1,
      -1,    -1,   103,    -1,    -1,   106,    12,    -1,   109,    -1,
      -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,   135,    -1,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    64,    65,
      -1,    -1,    -1,    -1,   165,    71,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    -1,    92,    12,    94,    -1,
      96,    -1,    -1,    99,    19,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   118,   119,   120,   121,   122,   123,    -1,    -1,
     126,   127,   128,   129,    -1,   131,   132,   133,   134,   135,
     136,   137,    -1,    -1,   140,    -1,    -1,    -1,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
     156,    -1,    -1,   159,   160,   161,   162,    82,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      95,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   117,   118,   119,   120,   121,   122,   123,    -1,
      -1,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    82,   163,    -1,
      -1,    86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,
      95,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   117,   118,   119,   120,   121,   122,   123,    -1,
      -1,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    82,   163,    -1,
      -1,    86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,
      95,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   117,   118,   119,   120,   121,   122,   123,    -1,
      -1,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    82,   163,    -1,
      -1,    86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,
      95,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   117,   118,   119,   120,   121,   122,   123,    -1,
      -1,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    82,   163,    -1,
      -1,    86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,
      95,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   117,   118,   119,   120,   121,   122,   123,    -1,
      -1,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    82,   163,    -1,
      -1,    86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,
      95,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   117,   118,   119,   120,   121,   122,   123,    -1,
      -1,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    82,   163,    -1,
      -1,    86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,
      95,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   117,   118,   119,   120,   121,   122,   123,    -1,
      -1,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    82,   163,    -1,
      -1,    86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,
      95,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   117,   118,   119,   120,   121,   122,   123,    -1,
      -1,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    82,   163,    -1,
      -1,    86,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,   117,   118,   119,   120,   121,   122,   123,    31,
      32,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
      -1,   136,   137,   138,    46,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    12,   163,    -1,
      -1,    -1,    -1,    -1,    19,   170,    -1,    -1,    -1,    -1,
      82,    -1,    -1,    -1,    86,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    82,   140,   141,
     142,    86,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      -1,   163,    12,    -1,    -1,    -1,    -1,    -1,   170,    19,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    67,   163,    12,
      13,    -1,    -1,    -1,    -1,   170,    19,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    -1,    86,    -1,    31,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    82,
     140,   141,   142,    86,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,   158,   159,
     160,   161,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
     170,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,    -1,   159,   160,   161,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    -1,    86,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    82,   159,
     160,   161,    86,   163,    12,    -1,    -1,    -1,    -1,    -1,
     170,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,    -1,    82,   159,   160,   161,    86,    -1,
      12,    -1,    -1,    -1,    -1,    -1,   170,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      82,   159,   160,   161,    86,   163,    12,    -1,    -1,    -1,
      -1,    -1,   170,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    82,   159,   160,   161,
      86,    -1,    12,    13,    -1,    -1,    -1,    -1,   170,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    82,   159,   160,   161,    86,    -1,    -1,    -1,
      -1,    -1,    12,    -1,   170,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,   158,   159,
     160,   161,    82,    -1,    -1,    -1,    86,    -1,    12,    -1,
     170,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    82,   159,
     160,   161,    86,   163,    -1,    -1,    -1,    -1,    12,    -1,
     170,    95,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,    -1,    -1,   159,   160,   161,    82,   163,
      -1,    -1,    86,    -1,    12,    -1,   170,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,   113,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    67,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,    -1,    82,   159,   160,   161,    86,    -1,
      12,    -1,    -1,    -1,   168,    -1,   170,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      82,   159,   160,   161,    86,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   170,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    82,   159,   160,   161,
      86,   163,    -1,    -1,    -1,    -1,    12,    -1,   170,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,   158,   159,   160,   161,    82,    -1,    -1,    -1,
      86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,   158,   159,   160,   161,    82,    -1,    -1,    -1,
      86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,   158,   159,   160,   161,    82,    -1,    -1,    -1,
      86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,   158,   159,   160,   161,    82,    -1,    -1,    -1,
      86,    -1,    12,    -1,   170,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    82,   159,   160,   161,    86,    -1,    -1,    -1,
      -1,    -1,    12,    -1,   170,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,    -1,    -1,    -1,    86,    -1,    12,    -1,
     170,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    82,   159,
     160,   161,    86,    -1,    12,    -1,   166,    -1,    -1,    -1,
     170,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,    -1,    82,   159,   160,   161,    86,    -1,
      12,    -1,    -1,    -1,    -1,    -1,   170,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      82,   159,   160,   161,    86,    -1,    -1,    -1,    -1,    -1,
      12,    13,   170,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,   158,   159,   160,   161,
      82,    -1,    -1,    -1,    86,    -1,    12,    -1,   170,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    82,   159,   160,   161,
      86,    -1,    12,    -1,    -1,    -1,    -1,    -1,   170,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    82,   159,   160,   161,    86,    -1,    12,    -1,
      -1,    -1,    -1,    -1,   170,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    82,   159,
     160,   161,    86,    -1,    12,    -1,   166,    -1,    -1,    -1,
     170,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,    -1,    82,   159,   160,   161,    86,    -1,
      12,    -1,   166,    -1,    -1,    -1,   170,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      82,   159,   160,   161,    86,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   170,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    82,   159,   160,   161,
      86,    -1,    12,    -1,    -1,    -1,    -1,    -1,   170,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    82,   159,   160,   161,    86,    -1,    12,    13,
      -1,    -1,    -1,    -1,   170,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    82,   159,
     160,   161,    86,    -1,    12,    -1,    -1,    -1,   168,    -1,
     170,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,    -1,    82,   159,   160,   161,    86,    -1,
      -1,    -1,    -1,    -1,    12,    -1,   170,    95,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      -1,   159,   160,   161,    82,    -1,    -1,    -1,    86,    -1,
      -1,    -1,   170,    -1,    12,    -1,    -1,    95,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      -1,   159,   160,   161,    82,    -1,    -1,    -1,    86,    -1,
      12,    -1,   170,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      82,   159,   160,   161,    86,    -1,    12,    -1,   166,    -1,
      -1,    -1,   170,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    82,   159,   160,   161,
      86,    -1,    -1,    -1,    -1,    -1,   168,    12,   170,    95,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    -1,    82,    -1,    -1,
      -1,    86,    -1,    -1,   170,    -1,    -1,    12,    -1,    -1,
      95,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    82,    -1,    -1,
      -1,    86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,
      95,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    82,    -1,    -1,
      -1,    86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,
      95,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    82,    -1,    -1,
      -1,    86,    -1,    -1,    -1,   170,    -1,    12,    -1,    -1,
      95,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    82,    -1,    -1,
      -1,    86,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    56,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,   170,    -1,    -1,    86,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      82,   159,   160,   161,    86,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   170,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    82,   159,   160,   161,
      86,    -1,    12,    -1,    -1,    -1,   168,    -1,   170,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    82,   159,   160,   161,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    22,   159,
     160,   161,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
     170,    35,    22,    37,    38,    -1,    26,    27,    42,    -1,
      -1,    -1,    -1,    -1,    48,    35,    -1,    37,    38,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    31,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,
      74,    -1,    76,    -1,    -1,    79,    -1,    81,    -1,    83,
      70,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    31,    -1,   102,    -1,
      -1,   105,    -1,    -1,   108,    -1,    -1,    82,    -1,    -1,
      -1,    86,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    82,    -1,    -1,
      -1,    86,    -1,    -1,   158,    -1,    -1,    -1,   133,   134,
     164,   136,   137,   138,    -1,   140,   141,   142,   158,   144,
     145,   146,   147,    -1,   164,   150,   151,   152,   153,   154,
     155,   156,   117,    -1,   159,   160,   161,    82,    -1,    -1,
      -1,    86,    -1,    -1,    -1,   170,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,    -1,    -1,   150,   151,   152,   153,   154,
      -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,    -1,    -1,   150,   151,   152,   153,   154,
       1,   156,     3,    -1,   159,   160,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    17,    18,    -1,    20,
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
      -1,    -1,    -1,    89,    90,    -1,    92,    -1,    94,    -1,
      96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    70,
     106,    72,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,   108
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   175,   176,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,    35,
      37,    38,    39,    40,    41,    42,    43,    44,    47,    48,
      50,    53,    54,    55,    57,    58,    59,    61,    62,    64,
      65,    66,    70,    71,    72,    73,    74,    75,    76,    79,
      80,    81,    83,    84,    85,    87,    88,    89,    90,    91,
      92,    93,    94,    96,    97,    99,   100,   101,   102,   103,
     105,   106,   108,   109,   112,   114,   117,   133,   134,   135,
     136,   137,   142,   152,   155,   156,   157,   158,   161,   163,
     165,   167,   170,   177,   178,   179,   180,   181,   182,   184,
     189,   190,   191,   192,   193,   197,   199,   202,   203,   205,
     206,   207,   212,   213,   215,   216,   217,   221,   222,   225,
     226,   229,   230,   233,   236,   237,   262,   265,   266,   286,
     287,   288,   289,   290,   291,   292,   300,   301,   302,   303,
     304,   307,   308,   309,   310,   311,   312,   313,   315,   316,
     317,   318,   319,   158,   179,   304,   113,   293,   294,     3,
     194,    14,    22,    35,    40,    41,    43,    81,    94,   163,
     167,   233,   286,   291,   302,   303,   304,   307,   309,   310,
     293,   304,   102,   265,    83,   194,   179,   280,   304,   179,
     165,     8,    81,   304,   305,     8,    11,    81,   102,   305,
      72,   115,   214,   304,   214,   304,    26,   266,   304,   304,
     165,     3,    17,    18,    20,    25,    33,    39,    44,    47,
      50,    58,    64,    65,    71,    80,    89,    90,    92,    94,
      96,    99,   103,   106,   109,   196,   198,   234,   165,   196,
     267,   268,    32,   184,   201,   304,   304,    18,    71,    89,
     106,   304,   304,   304,     8,   165,   204,    22,    35,    38,
      81,   205,     4,   157,   256,   279,   304,   100,   101,   158,
     304,   306,   304,   201,   304,   304,   304,    93,   165,   179,
     304,   304,   184,   193,   304,   307,   184,   193,   304,   196,
     263,   304,   304,   304,   304,   304,   304,   304,   304,     1,
     164,   177,   185,   279,   104,   143,   256,   281,   282,   306,
     214,   279,   304,   314,   304,    74,   179,   163,    78,   183,
      45,   107,   196,   196,    51,    77,   251,   267,   157,   158,
     149,   304,    12,    19,    31,    82,    86,   117,   133,   134,
     136,   137,   138,   140,   141,   142,   144,   145,   146,   147,
     148,   150,   151,   152,   153,   154,   155,   156,   159,   160,
     161,   170,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   162,   243,   165,
     167,    82,    86,   165,   179,   158,   304,   304,   304,   279,
     163,   184,    46,   293,   263,   267,   158,   139,   158,   112,
     197,   256,   283,   284,   285,   306,    81,   210,   237,   265,
      81,   208,   263,   237,   265,   196,   165,   201,    32,    46,
     201,   113,   201,   296,    32,    46,   201,   296,    36,    67,
     158,    95,   184,   243,   283,   179,   165,   245,   104,   165,
     196,   269,   270,     1,   138,   274,    46,   139,   179,   201,
     165,   165,   201,   283,   205,   196,   139,   158,   304,   304,
     158,   163,   201,   165,   283,   158,   218,   158,   218,   158,
     118,   264,   158,   201,   158,   164,   164,   177,   139,   164,
     304,   139,   166,   139,   166,   168,   296,    46,   139,   168,
     296,   116,   139,   168,     8,     1,   164,   185,    60,   304,
     186,   304,   138,   227,   163,   238,   158,   304,   304,   304,
     304,   214,   304,   214,   304,   304,   304,   304,   304,   304,
     304,    20,    33,    58,   102,   197,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   306,   306,   306,   306,   306,   283,   283,
     214,   304,   214,   304,    27,    46,    83,   108,   295,   298,
     299,   304,   319,    32,    46,    32,    46,    95,    46,   168,
     214,   304,   201,   158,   158,   304,   304,   118,   166,   139,
     211,   196,   267,   209,   196,   158,   267,    46,   279,    43,
     304,   214,   304,   165,   201,    43,   304,   214,   304,   201,
     161,   186,   188,   304,   186,   187,   179,   304,    34,   304,
     166,    27,    46,    49,    69,    72,    83,   102,   178,   246,
     247,   248,   249,   235,   270,   139,   166,    33,    47,    90,
      94,   167,   200,   275,   287,   118,   271,   304,   268,   304,
     304,   166,   256,   304,     1,   223,   283,   166,    21,   219,
     275,   287,   139,   164,   166,   166,   281,   166,   281,   179,
     168,   214,   304,   168,   179,   304,   168,   304,   168,   304,
     164,   164,   196,    13,   141,   158,   139,   158,    13,    36,
      67,   279,   163,     1,   196,   231,   232,    27,    72,    83,
     102,   240,   250,   165,   158,   158,   158,   158,   158,   166,
     168,    46,    83,   139,   166,   286,    82,    82,    43,   214,
     304,    43,   214,   304,   214,   304,   293,   293,   158,   256,
     306,   285,   196,   227,   158,   196,   227,   158,   304,   166,
     304,    32,   201,    32,   201,   297,   298,   304,    32,   201,
     296,    32,   201,   296,   158,    13,   158,    34,    34,   179,
      95,   184,    46,    83,   248,   139,   166,   165,   196,    27,
      72,    83,   102,   252,   166,   270,   274,     1,   279,    63,
     306,   166,   166,   164,    68,   110,   164,   224,   166,   165,
     184,   196,   220,   263,   179,   168,   296,   168,   296,   179,
     116,   197,   163,   304,   304,   188,   187,     1,   228,   164,
     118,   139,   164,    83,   239,     1,     3,    12,    17,    19,
      20,    25,    39,    44,    50,    52,    58,    64,    65,    80,
      92,    96,    99,   103,   109,   133,   134,   135,   136,   137,
     140,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   156,   159,   160,   161,   162,   165,   195,   196,
     198,   241,   242,   243,   286,   166,   298,   274,   286,   286,
     304,    32,    32,   304,    32,    32,   168,   168,   201,   201,
     227,   163,   227,   163,   201,    95,    43,   304,    43,   304,
     139,   166,    95,    43,   304,   201,    43,   304,   201,   304,
     179,   304,   179,    34,   196,   247,   270,   138,   278,    83,
     274,   271,   168,    46,   168,   165,   165,    32,   179,   279,
     220,   138,   184,    43,   179,   304,   168,    43,   179,   304,
     168,   304,   158,   186,    13,    36,    67,   158,   158,   164,
      76,    79,   164,   178,   203,   304,   232,   252,   165,   244,
     196,   304,   133,   141,   244,   244,   271,    95,    43,    43,
      95,    43,    43,    43,    43,   163,   228,   163,   228,   304,
     304,   304,   298,   304,   304,   304,    34,   179,   278,   166,
     167,   200,   256,   277,   287,   143,   257,   271,    56,   111,
     258,   304,   275,   287,   283,   283,   179,   201,   166,   304,
     304,   179,   304,   179,   164,   304,   188,   187,   203,     1,
     138,   273,   246,   166,     3,    96,   242,   243,   304,   304,
     304,   304,   304,   304,   228,   164,   228,   164,    95,    95,
      95,    95,   179,   271,   278,   168,   279,   256,   304,     3,
      85,    96,   259,   260,   261,   304,   184,   202,   255,   168,
     166,   166,   184,    95,    95,   158,   158,   167,   200,   272,
     287,    98,   253,   166,   244,   244,    95,    95,    95,    95,
      95,    95,   164,   164,   304,   304,   304,   304,   257,   271,
     256,   276,   277,   287,    46,   168,   261,   111,   139,   118,
     144,   146,   147,   150,   151,    56,   287,   157,   157,   304,
     304,     1,   168,   279,   258,   304,   276,   277,   304,   260,
     261,   261,   261,   261,   261,   261,   259,   168,   272,   287,
     168,   158,   254,   255,   168,   272,   287,   276
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
    { (yyval.pblockstmt) = buildImportStmt((yyvsp[(3) - (4)].pexpr), (yyvsp[(1) - (4)].b)); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 675 "chapel.ypp"
    { (yyval.pblockstmt) = buildImportStmt((yyvsp[(3) - (6)].pexpr), (yyvsp[(5) - (6)].pch), (yyvsp[(1) - (6)].b)); }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 676 "chapel.ypp"
    { (yyval.pblockstmt) = buildImportStmt((yyvsp[(3) - (7)].pexpr), (yyvsp[(6) - (7)].ponlylist), (yyvsp[(1) - (7)].b)); }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 680 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 685 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 687 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 704 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 705 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 706 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 707 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 708 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 709 "chapel.ypp"
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
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 714 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 715 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 716 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 717 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 729 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 730 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 742 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 743 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 744 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 745 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 748 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 749 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 758 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 759 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 763 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 764 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 765 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 766 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 767 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 768 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 770 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 771 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 772 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 773 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 774 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 781 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 782 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 783 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 784 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 788 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 789 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 793 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 794 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 798 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 800 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 810 "chapel.ypp"
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 811 "chapel.ypp"
    { parsingPrivate=(yyvsp[(2) - (3)].b); applyPrivateToBlock((yyvsp[(3) - (3)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(3) - (3)].pblockstmt); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 825 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 826 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 827 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 828 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 833 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 838 "chapel.ypp"
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

  case 152:
/* Line 1792 of yacc.c  */
#line 848 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 853 "chapel.ypp"
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

  case 154:
/* Line 1792 of yacc.c  */
#line 864 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 869 "chapel.ypp"
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

  case 156:
/* Line 1792 of yacc.c  */
#line 879 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 884 "chapel.ypp"
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

  case 158:
/* Line 1792 of yacc.c  */
#line 895 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 899 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 904 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 912 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 923 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 929 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 930 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 931 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 932 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 933 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 934 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 935 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), true, false); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 936 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 937 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), true, false); }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 938 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 939 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 940 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 941 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 942 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 943 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 944 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 945 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 946 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 948 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 954 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 960 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 966 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 972 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 979 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 986 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 990 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 996 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 1000 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 1001 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1002 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 1003 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1004 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1006 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1008 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1010 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1018 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1022 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1023 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1028 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1032 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1033 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1034 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1038 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1039 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1043 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1047 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1049 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1053 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1054 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1059 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1061 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1063 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1070 "chapel.ypp"
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

  case 221:
/* Line 1792 of yacc.c  */
#line 1080 "chapel.ypp"
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

  case 222:
/* Line 1792 of yacc.c  */
#line 1093 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1098 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1103 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1111 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1112 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1117 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1119 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1121 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1126 "chapel.ypp"
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

  case 231:
/* Line 1792 of yacc.c  */
#line 1142 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1149 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1158 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1166 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1170 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1176 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1177 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1182 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1187 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1192 "chapel.ypp"
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

  case 241:
/* Line 1792 of yacc.c  */
#line 1214 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1217 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1221 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1229 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1239 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1246 "chapel.ypp"
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

  case 247:
/* Line 1792 of yacc.c  */
#line 1272 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1276 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1281 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1285 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1290 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1307 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1308 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1309 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1312 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1313 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1314 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1317 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1318 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1319 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1320 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1321 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1322 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1323 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1324 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1325 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1326 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1327 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1328 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1332 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1333 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1335 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1336 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1337 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1338 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1340 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1341 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1342 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1343 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1347 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1348 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1352 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1356 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1357 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1358 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1363 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1365 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1367 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1371 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1375 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1376 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1380 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1381 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1382 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1383 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1384 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1386 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1387 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1388 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1392 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1393 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1394 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1395 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1396 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1397 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1401 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1402 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1406 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1407 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1408 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1409 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1410 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1411 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1415 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1416 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1419 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1425 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1429 "chapel.ypp"
    { 
      DefExpr * de = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch)));
      USR_WARN(de, "support for space in query expressions is deprecated");
      (yyval.pexpr) = de;
  }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1434 "chapel.ypp"
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1438 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1439 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1440 "chapel.ypp"
    { if (DefExpr* def = toDefExpr((yyvsp[(2) - (2)].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
                         }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1448 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1450 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1452 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1454 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1456 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1461 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1463 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1467 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1468 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1469 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1470 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1471 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1472 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1473 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1480 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1487 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1494 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1507 "chapel.ypp"
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

  case 362:
/* Line 1792 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1527 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1532 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1534 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1540 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1546 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1555 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1563 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1565 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1570 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1572 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1574 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1579 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1583 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1591 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1597 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1601 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1605 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1609 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1611 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1615 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1622 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1623 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1624 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1625 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1626 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1631 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1633 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1634 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1635 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1656 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1660 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1664 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1672 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1678 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1679 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1680 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1685 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1687 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1693 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1695 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1697 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1701 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1703 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1704 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1705 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1711 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1712 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1713 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1719 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1724 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1725 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1729 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1730 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1734 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1739 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1740 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1744 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1745 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1746 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1747 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1751 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1752 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1764 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1766 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1768 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1779 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1781 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1783 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1785 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1787 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1789 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1791 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1794 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1796 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1798 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1800 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1802 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1804 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1806 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1808 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1811 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1813 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1818 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1820 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1822 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1824 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1826 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1828 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1830 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1832 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1834 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1836 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1838 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1840 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1842 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1849 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1855 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1861 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1867 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1876 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1885 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1901 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1905 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1910 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1914 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1915 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1919 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1923 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1924 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1929 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1933 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1937 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1943 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1944 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1945 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1946 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1947 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 1948 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1954 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1959 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1961 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1965 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1969 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1973 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1981 "chapel.ypp"
    { (yyvsp[(1) - (2)].pcallexpr)->insertAtTail((yyvsp[(2) - (2)].pexpr));
      (yyval.pexpr) = (yyvsp[(1) - (2)].pcallexpr); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1986 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1992 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1998 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 2005 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 2014 "chapel.ypp"
    { USR_WARN((yyvsp[(4) - (5)].pexpr), "the syntax 'new owned(an expression)' is deprecated");
      USR_PRINT((yyvsp[(4) - (5)].pexpr), "instead use 'owned.create(an expression)'");
      (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtOwned->symbol, (yyvsp[(4) - (5)].pexpr))); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 2018 "chapel.ypp"
    { USR_WARN((yyvsp[(4) - (5)].pexpr), "the syntax 'new shared(an expression)' is deprecated");
      USR_PRINT((yyvsp[(4) - (5)].pexpr), "instead use 'shared.create(an expression)'");
      (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtShared->symbol, (yyvsp[(4) - (5)].pexpr))); }
    break;

  case 523:
/* Line 1792 of yacc.c  */
#line 2025 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 2041 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 2043 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 2045 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 2047 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), false, true); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 2062 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 2064 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 2066 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr), true); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 2068 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 2072 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 2073 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 2077 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 2078 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 2079 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 2097 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 2098 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 2099 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr));}
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 2100 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 2104 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 2105 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 2106 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 2110 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 2111 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 2112 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 2113 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 2114 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2122 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2123 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2124 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 2125 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2129 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 2130 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2135 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2136 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2137 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2138 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2139 "chapel.ypp"
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2140 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2141 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2142 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2143 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2144 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2145 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2147 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2151 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2158 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2159 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2163 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 584:
/* Line 1792 of yacc.c  */
#line 2164 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 585:
/* Line 1792 of yacc.c  */
#line 2165 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 586:
/* Line 1792 of yacc.c  */
#line 2166 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 587:
/* Line 1792 of yacc.c  */
#line 2167 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 588:
/* Line 1792 of yacc.c  */
#line 2168 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 589:
/* Line 1792 of yacc.c  */
#line 2169 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 590:
/* Line 1792 of yacc.c  */
#line 2170 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 591:
/* Line 1792 of yacc.c  */
#line 2171 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 592:
/* Line 1792 of yacc.c  */
#line 2172 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 593:
/* Line 1792 of yacc.c  */
#line 2173 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 594:
/* Line 1792 of yacc.c  */
#line 2174 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 595:
/* Line 1792 of yacc.c  */
#line 2175 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 596:
/* Line 1792 of yacc.c  */
#line 2176 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 597:
/* Line 1792 of yacc.c  */
#line 2177 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 598:
/* Line 1792 of yacc.c  */
#line 2178 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 599:
/* Line 1792 of yacc.c  */
#line 2179 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 600:
/* Line 1792 of yacc.c  */
#line 2180 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 601:
/* Line 1792 of yacc.c  */
#line 2181 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 602:
/* Line 1792 of yacc.c  */
#line 2182 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 603:
/* Line 1792 of yacc.c  */
#line 2183 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 604:
/* Line 1792 of yacc.c  */
#line 2184 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 605:
/* Line 1792 of yacc.c  */
#line 2185 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 606:
/* Line 1792 of yacc.c  */
#line 2189 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 607:
/* Line 1792 of yacc.c  */
#line 2190 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 608:
/* Line 1792 of yacc.c  */
#line 2191 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 609:
/* Line 1792 of yacc.c  */
#line 2192 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 610:
/* Line 1792 of yacc.c  */
#line 2193 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 611:
/* Line 1792 of yacc.c  */
#line 2194 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 612:
/* Line 1792 of yacc.c  */
#line 2195 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 613:
/* Line 1792 of yacc.c  */
#line 2199 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 614:
/* Line 1792 of yacc.c  */
#line 2200 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 615:
/* Line 1792 of yacc.c  */
#line 2201 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 616:
/* Line 1792 of yacc.c  */
#line 2202 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 617:
/* Line 1792 of yacc.c  */
#line 2206 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 618:
/* Line 1792 of yacc.c  */
#line 2207 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 619:
/* Line 1792 of yacc.c  */
#line 2208 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 620:
/* Line 1792 of yacc.c  */
#line 2209 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 621:
/* Line 1792 of yacc.c  */
#line 2214 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 622:
/* Line 1792 of yacc.c  */
#line 2215 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 623:
/* Line 1792 of yacc.c  */
#line 2216 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 624:
/* Line 1792 of yacc.c  */
#line 2217 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 625:
/* Line 1792 of yacc.c  */
#line 2218 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 626:
/* Line 1792 of yacc.c  */
#line 2219 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 627:
/* Line 1792 of yacc.c  */
#line 2220 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 10478 "bison-chapel.cpp"
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


