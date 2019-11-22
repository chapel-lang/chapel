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
#line 146 "chapel.ypp"

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
#line 169 "chapel.ypp"

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
     TUPLUS = 424,
     TLNOT = 425
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
#line 200 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);


/* Line 387 of yacc.c  */
#line 486 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 493 "bison-chapel.cpp"
/* Unqualified %code blocks.  */
/* Line 391 of yacc.c  */
#line 38 "chapel.ypp"

  #include <string>
  int         captureTokens;
  std::string captureString;

/* Line 391 of yacc.c  */
#line 208 "chapel.ypp"

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
#line 570 "bison-chapel.cpp"

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
#define YYLAST   17803

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  171
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  144
/* YYNRULES -- Number of rules.  */
#define YYNRULES  617
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1103

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   425

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
     165,   166,   167,   168,   169,   170
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
     371,   377,   380,   381,   390,   391,   401,   402,   411,   412,
     422,   426,   430,   436,   442,   445,   451,   455,   462,   469,
     474,   480,   486,   490,   494,   501,   507,   514,   520,   527,
     531,   536,   540,   545,   552,   560,   567,   575,   580,   586,
     591,   597,   602,   607,   611,   618,   624,   631,   637,   646,
     654,   657,   661,   665,   668,   671,   675,   679,   680,   683,
     686,   690,   696,   698,   702,   706,   712,   718,   719,   722,
     726,   729,   733,   740,   747,   749,   751,   753,   754,   757,
     758,   761,   765,   771,   777,   779,   781,   784,   788,   790,
     794,   795,   796,   805,   806,   808,   810,   811,   812,   823,
     827,   831,   837,   843,   847,   849,   853,   855,   858,   860,
     862,   864,   866,   868,   870,   872,   874,   876,   878,   880,
     882,   884,   886,   888,   890,   892,   894,   896,   898,   900,
     902,   904,   906,   908,   911,   913,   915,   917,   919,   921,
     923,   925,   927,   929,   931,   933,   935,   936,   940,   944,
     945,   947,   951,   956,   962,   967,   974,   981,   982,   984,
     986,   988,   990,   992,   995,   998,  1000,  1002,  1004,  1005,
    1007,  1009,  1012,  1014,  1016,  1018,  1020,  1021,  1023,  1026,
    1028,  1030,  1032,  1033,  1035,  1037,  1039,  1041,  1043,  1046,
    1048,  1051,  1054,  1055,  1058,  1061,  1066,  1071,  1073,  1077,
    1081,  1085,  1089,  1093,  1097,  1101,  1104,  1106,  1108,  1112,
    1117,  1122,  1125,  1130,  1131,  1134,  1137,  1139,  1141,  1143,
    1146,  1148,  1153,  1157,  1159,  1163,  1167,  1173,  1175,  1177,
    1181,  1183,  1186,  1190,  1191,  1194,  1197,  1201,  1204,  1209,
    1213,  1217,  1222,  1226,  1227,  1230,  1233,  1236,  1238,  1239,
    1242,  1245,  1248,  1250,  1255,  1260,  1267,  1271,  1272,  1274,
    1276,  1280,  1285,  1289,  1294,  1301,  1302,  1305,  1308,  1311,
    1314,  1316,  1318,  1322,  1326,  1328,  1332,  1334,  1336,  1338,
    1342,  1346,  1347,  1349,  1351,  1355,  1359,  1363,  1365,  1367,
    1369,  1371,  1373,  1376,  1378,  1380,  1382,  1384,  1386,  1388,
    1391,  1396,  1401,  1406,  1412,  1415,  1418,  1420,  1423,  1425,
    1428,  1430,  1433,  1435,  1438,  1440,  1442,  1444,  1451,  1458,
    1463,  1473,  1483,  1491,  1498,  1505,  1510,  1520,  1530,  1538,
    1543,  1550,  1557,  1567,  1577,  1584,  1586,  1588,  1590,  1592,
    1594,  1596,  1598,  1600,  1604,  1605,  1607,  1612,  1614,  1618,
    1623,  1625,  1629,  1634,  1638,  1642,  1644,  1646,  1649,  1651,
    1654,  1656,  1658,  1662,  1664,  1667,  1670,  1673,  1676,  1679,
    1688,  1697,  1707,  1717,  1723,  1729,  1734,  1736,  1738,  1740,
    1742,  1744,  1746,  1748,  1750,  1755,  1759,  1763,  1766,  1769,
    1771,  1772,  1774,  1777,  1780,  1782,  1784,  1786,  1788,  1790,
    1792,  1795,  1798,  1800,  1805,  1810,  1815,  1819,  1823,  1827,
    1831,  1837,  1841,  1846,  1850,  1855,  1857,  1859,  1861,  1863,
    1865,  1867,  1869,  1871,  1873,  1875,  1879,  1884,  1888,  1893,
    1897,  1902,  1906,  1912,  1916,  1920,  1924,  1928,  1932,  1936,
    1940,  1944,  1948,  1952,  1956,  1960,  1964,  1968,  1972,  1976,
    1980,  1984,  1988,  1992,  1996,  2000,  2004,  2007,  2010,  2013,
    2016,  2019,  2022,  2025,  2029,  2033,  2037,  2041,  2045,  2049,
    2053,  2057,  2059,  2061,  2063,  2065,  2067,  2069
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     172,     0,    -1,   173,    -1,    -1,   173,   174,    -1,   176,
      -1,   175,   176,    -1,    72,     7,    -1,   175,    72,     7,
      -1,   178,    -1,   181,    -1,   187,    -1,   188,    -1,   199,
      -1,   189,    -1,   207,    -1,   210,    -1,   208,    -1,   217,
      -1,   211,    -1,   212,    -1,   216,    -1,   198,    -1,   287,
     156,    -1,    13,   176,    -1,    14,   288,   176,    -1,    15,
     190,   156,    -1,    22,   288,   181,    -1,    27,   190,   156,
      -1,    29,   275,   156,    -1,    51,   192,   176,    -1,    55,
     299,   197,    -1,    55,   197,    -1,    64,   299,   197,    -1,
      86,   299,   197,    -1,    86,   197,    -1,    92,   176,    -1,
     112,   299,   156,    -1,     1,   156,    -1,   179,   180,    58,
     192,    -1,   177,   161,   162,    -1,   177,   161,   182,   162,
      -1,   177,   161,     1,   162,    -1,    -1,    77,    -1,    74,
      -1,    -1,    76,    -1,   161,   162,    -1,   161,   182,   162,
      -1,   161,     1,   162,    -1,   174,    -1,   182,   174,    -1,
     299,    -1,   299,    12,   299,    -1,   183,   137,   299,    -1,
     183,   137,   299,    12,   299,    -1,    -1,   183,    -1,   159,
      -1,   183,    -1,    -1,    77,    -1,    74,    -1,   186,   105,
     274,   156,    -1,   186,   105,   299,    35,   185,   156,    -1,
     186,   105,   299,    65,   184,   156,    -1,    82,   274,   156,
      -1,   302,   238,   301,   156,    -1,   302,   160,   301,   156,
      -1,   302,   128,   301,   156,    -1,   302,   122,   301,   156,
      -1,   302,   123,   301,   156,    -1,    -1,     3,    -1,     3,
      -1,    62,    -1,    94,    -1,    38,    -1,    97,    -1,   194,
      -1,     3,    -1,    62,    -1,    94,    -1,    38,    -1,    97,
      -1,   194,    -1,     3,    -1,    94,    -1,    16,    -1,    48,
      -1,   101,    -1,    78,    -1,    43,    -1,    24,    -1,    19,
      -1,    90,    -1,    92,    -1,    88,    -1,    69,    -1,    87,
      -1,    17,    -1,   104,    -1,    32,    -1,    45,    -1,    56,
      -1,    63,    -1,   107,    -1,    16,    -1,    48,    -1,   101,
      -1,    78,    -1,    43,    -1,    24,    -1,    19,    -1,    90,
      -1,    56,    -1,    63,    -1,   107,    -1,    92,    -1,    88,
      -1,    32,    -1,    45,    -1,    31,   176,    -1,   181,    -1,
      83,   156,    -1,    83,   301,   156,    -1,   156,    -1,   200,
      -1,    77,   200,    -1,    74,   200,    -1,   232,    -1,   261,
      -1,   224,    -1,   257,    -1,   220,    -1,   201,    -1,   202,
      -1,    41,   299,   156,    -1,    41,   299,    35,   185,   156,
      -1,    41,   299,    65,   184,   156,    -1,    41,   261,    -1,
      -1,    37,    79,   203,   192,   222,   161,   223,   162,    -1,
      -1,    37,     7,    79,   204,   192,   222,   161,   223,   162,
      -1,    -1,    36,    79,   205,   192,   222,   161,   223,   162,
      -1,    -1,    36,     7,    79,   206,   192,   222,   161,   223,
     162,    -1,    37,   300,   232,    -1,    36,   300,   232,    -1,
      37,   300,   260,   262,   156,    -1,    36,   300,   260,   262,
     156,    -1,    37,    10,    -1,    31,   176,   110,   299,   156,
      -1,   110,   299,   197,    -1,    23,   299,    44,   299,   288,
     197,    -1,    23,   299,    44,   209,   288,   197,    -1,    23,
     299,   288,   197,    -1,    39,   299,    44,   299,   197,    -1,
      39,   299,    44,   209,   197,    -1,    39,   299,   197,    -1,
      39,   209,   197,    -1,    39,    70,   192,    44,   299,   197,
      -1,    40,   299,    44,   299,   197,    -1,    40,   299,    44,
     299,   291,   197,    -1,    40,   299,    44,   209,   197,    -1,
      40,   299,    44,   209,   291,   197,    -1,    40,   299,   197,
      -1,    40,   299,   291,   197,    -1,    40,   209,   197,    -1,
      40,   209,   291,   197,    -1,   165,   274,    44,   299,   166,
     176,    -1,   165,   274,    44,   299,   291,   166,   176,    -1,
     165,   274,    44,   209,   166,   176,    -1,   165,   274,    44,
     209,   291,   166,   176,    -1,   165,   274,   166,   176,    -1,
     165,   274,   291,   166,   176,    -1,   165,   209,   166,   176,
      -1,   165,   209,   291,   166,   176,    -1,   113,   163,   274,
     164,    -1,    42,   299,    93,   176,    -1,    42,   299,   181,
      -1,    42,   299,    93,   176,    33,   176,    -1,    42,   299,
     181,    33,   176,    -1,    42,   299,   238,   299,    93,   176,
      -1,    42,   299,   238,   299,   181,    -1,    42,   299,   238,
     299,    93,   176,    33,   176,    -1,    42,   299,   238,   299,
     181,    33,   176,    -1,    28,   176,    -1,    98,   299,   156,
      -1,    99,   299,   156,    -1,    98,   189,    -1,    99,   189,
      -1,    98,   181,   213,    -1,    99,   181,   213,    -1,    -1,
     213,   214,    -1,    20,   181,    -1,    20,   215,   181,    -1,
      20,   163,   215,   164,   181,    -1,   192,    -1,   192,   136,
     299,    -1,    95,   299,   156,    -1,    85,   299,   161,   218,
     162,    -1,    85,   299,   161,     1,   162,    -1,    -1,   218,
     219,    -1,   108,   274,   197,    -1,    66,   176,    -1,    66,
      31,   176,    -1,   221,   192,   222,   161,   223,   162,    -1,
     221,   192,   222,   161,     1,   162,    -1,    21,    -1,    79,
      -1,   103,    -1,    -1,   136,   274,    -1,    -1,   223,   199,
      -1,   223,   175,   199,    -1,   225,   192,   161,   226,   162,
      -1,   225,   192,   161,     1,   162,    -1,    34,    -1,   227,
      -1,   226,   137,    -1,   226,   137,   227,    -1,   192,    -1,
     192,   116,   299,    -1,    -1,    -1,    52,   229,   240,   230,
     247,   269,   253,   250,    -1,    -1,    46,    -1,    68,    -1,
      -1,    -1,   231,   246,   233,   235,   234,   247,   268,   248,
     253,   249,    -1,   245,   237,   239,    -1,   245,   238,   239,
      -1,   245,   236,   139,   237,   239,    -1,   245,   236,   139,
     238,   239,    -1,   245,     1,   239,    -1,   281,    -1,   163,
     299,   164,    -1,   191,    -1,   133,   192,    -1,   132,    -1,
     134,    -1,   135,    -1,   133,    -1,   142,    -1,   152,    -1,
     149,    -1,   145,    -1,   148,    -1,   144,    -1,   154,    -1,
     150,    -1,   159,    -1,   138,    -1,   157,    -1,   158,    -1,
     151,    -1,   143,    -1,   131,    -1,    18,    -1,   146,    -1,
      11,    -1,   160,    -1,   147,    -1,    50,    -1,   192,   131,
      -1,   116,    -1,   127,    -1,   124,    -1,   126,    -1,   120,
      -1,   125,    -1,   121,    -1,   117,    -1,   118,    -1,   119,
      -1,   130,    -1,   129,    -1,    -1,   163,   241,   164,    -1,
     163,   241,   164,    -1,    -1,   242,    -1,   241,   137,   242,
      -1,   243,   192,   273,   266,    -1,   175,   243,   192,   273,
     266,    -1,   243,   192,   273,   252,    -1,   243,   163,   265,
     164,   273,   266,    -1,   243,   163,   265,   164,   273,   252,
      -1,    -1,   244,    -1,    44,    -1,    47,    -1,    67,    -1,
      26,    -1,    26,    44,    -1,    26,    81,    -1,    70,    -1,
      81,    -1,   100,    -1,    -1,    70,    -1,    81,    -1,    26,
      81,    -1,    26,    -1,   100,    -1,    75,    -1,    49,    -1,
      -1,    26,    -1,    26,    81,    -1,    81,    -1,    70,    -1,
     100,    -1,    -1,    96,    -1,   156,    -1,   250,    -1,   181,
      -1,   198,    -1,   155,   192,    -1,   141,    -1,   141,   299,
      -1,   141,   251,    -1,    -1,   109,   299,    -1,    54,   254,
      -1,   109,   299,    54,   254,    -1,    54,   254,   109,   299,
      -1,   255,    -1,   254,   137,   255,    -1,   256,   116,   256,
      -1,   256,   148,   256,    -1,   256,   149,   256,    -1,   256,
     142,   256,    -1,   256,   144,   256,    -1,   256,   145,   256,
      -1,    83,   256,    -1,     3,    -1,    94,    -1,   100,   258,
     156,    -1,    25,   100,   258,   156,    -1,    37,   100,   258,
     156,    -1,   192,   259,    -1,   192,   259,   137,   258,    -1,
      -1,   116,   282,    -1,   116,   270,    -1,    70,    -1,    26,
      -1,    81,    -1,    26,    81,    -1,   106,    -1,    25,   260,
     262,   156,    -1,   260,   262,   156,    -1,   263,    -1,   262,
     137,   263,    -1,   192,   269,   266,    -1,   163,   265,   164,
     269,   266,    -1,   102,    -1,   192,    -1,   163,   265,   164,
      -1,   264,    -1,   264,   137,    -1,   264,   137,   265,    -1,
      -1,   116,    61,    -1,   116,   301,    -1,   165,   166,   282,
      -1,   165,   166,    -1,   165,   274,   166,   282,    -1,   165,
     274,   166,    -1,   165,   166,   267,    -1,   165,   274,   166,
     267,    -1,   165,     1,   166,    -1,    -1,   136,   282,    -1,
     136,   267,    -1,   136,   196,    -1,     1,    -1,    -1,   136,
     282,    -1,   136,   270,    -1,   136,   196,    -1,     1,    -1,
     165,   274,   166,   282,    -1,   165,   274,   166,   270,    -1,
     165,   274,    44,   299,   166,   282,    -1,   165,     1,   166,
      -1,    -1,   282,    -1,   251,    -1,   165,   166,   271,    -1,
     165,   274,   166,   271,    -1,   165,   166,   272,    -1,   165,
     274,   166,   272,    -1,   165,   274,    44,   299,   166,   271,
      -1,    -1,   136,   282,    -1,   136,   251,    -1,   136,   196,
      -1,   136,   272,    -1,   299,    -1,   251,    -1,   274,   137,
     299,    -1,   274,   137,   251,    -1,   299,    -1,   275,   137,
     299,    -1,   102,    -1,   301,    -1,   251,    -1,   276,   137,
     276,    -1,   277,   137,   276,    -1,    -1,   279,    -1,   280,
      -1,   279,   137,   280,    -1,   193,   116,   251,    -1,   193,
     116,   301,    -1,   251,    -1,   301,    -1,   193,    -1,   195,
      -1,   283,    -1,   283,   155,    -1,   155,    -1,   286,    -1,
     302,    -1,   285,    -1,   311,    -1,   310,    -1,    88,   299,
      -1,    45,   163,   278,   164,    -1,    32,   163,   278,   164,
      -1,    91,   163,   278,   164,    -1,    89,    91,   163,   278,
     164,    -1,    13,   299,    -1,    92,   299,    -1,    69,    -1,
      69,   299,    -1,   104,    -1,   104,   299,    -1,    87,    -1,
      87,   299,    -1,    17,    -1,    17,   299,    -1,    21,    -1,
      79,    -1,    34,    -1,    39,   299,    44,   299,    31,   299,
      -1,    39,   299,    44,   209,    31,   299,    -1,    39,   299,
      31,   299,    -1,    39,   299,    44,   299,    31,    42,   299,
      93,   299,    -1,    39,   299,    44,   209,    31,    42,   299,
      93,   299,    -1,    39,   299,    31,    42,   299,    93,   299,
      -1,    40,   299,    44,   299,    31,   299,    -1,    40,   299,
      44,   209,    31,   299,    -1,    40,   299,    31,   299,    -1,
      40,   299,    44,   299,    31,    42,   299,    93,   299,    -1,
      40,   299,    44,   209,    31,    42,   299,    93,   299,    -1,
      40,   299,    31,    42,   299,    93,   299,    -1,   165,   274,
     166,   299,    -1,   165,   274,    44,   299,   166,   299,    -1,
     165,   274,    44,   209,   166,   299,    -1,   165,   274,    44,
     299,   166,    42,   299,    93,   299,    -1,   165,   274,    44,
     209,   166,    42,   299,    93,   299,    -1,    42,   299,    93,
     299,    33,   299,    -1,    60,    -1,   286,    -1,   281,    -1,
     305,    -1,   304,    -1,   228,    -1,   297,    -1,   298,    -1,
     295,   147,   299,    -1,    -1,   289,    -1,   111,   163,   290,
     164,    -1,   293,    -1,   290,   137,   293,    -1,   111,   163,
     292,   164,    -1,   293,    -1,   292,   137,   293,    -1,   294,
     281,   269,   266,    -1,   314,    80,   281,    -1,   299,    80,
     281,    -1,    26,    -1,    44,    -1,    26,    44,    -1,    81,
      -1,    26,    81,    -1,   106,    -1,   302,    -1,   295,   147,
     299,    -1,    59,    -1,    59,    69,    -1,    59,    87,    -1,
      59,   104,    -1,    59,    17,    -1,   296,   299,    -1,    59,
      69,   163,   299,   164,   163,   278,   164,    -1,    59,    87,
     163,   299,   164,   163,   278,   164,    -1,    59,    69,   163,
     299,   164,   163,   278,   164,   155,    -1,    59,    87,   163,
     299,   164,   163,   278,   164,   155,    -1,    59,    69,   163,
     299,   164,    -1,    59,    87,   163,   299,   164,    -1,    53,
     262,    44,   299,    -1,   308,    -1,   282,    -1,   284,    -1,
     312,    -1,   313,    -1,   228,    -1,   297,    -1,   298,    -1,
     163,   141,   299,   164,    -1,   299,   136,   299,    -1,   299,
     140,   299,    -1,   299,   140,    -1,   140,   299,    -1,   140,
      -1,    -1,   299,    -1,    98,   299,    -1,    99,   299,    -1,
     299,    -1,   281,    -1,   304,    -1,   305,    -1,   306,    -1,
     302,    -1,   299,   131,    -1,   283,   155,    -1,   228,    -1,
     303,   163,   278,   164,    -1,   303,   165,   278,   166,    -1,
      73,   163,   278,   164,    -1,   299,   139,   193,    -1,   299,
     139,   100,    -1,   299,   139,    32,    -1,   299,   139,    56,
      -1,   299,   139,    19,   163,   164,    -1,   163,   276,   164,
      -1,   163,   276,   137,   164,    -1,   163,   277,   164,    -1,
     163,   277,   137,   164,    -1,    38,    -1,    97,    -1,   307,
      -1,     4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,
      -1,     9,    -1,    62,    -1,   161,   274,   162,    -1,   161,
     274,   137,   162,    -1,   165,   274,   166,    -1,   165,   274,
     137,   166,    -1,   165,   309,   166,    -1,   165,   309,   137,
     166,    -1,   299,   114,   299,    -1,   309,   137,   299,   114,
     299,    -1,   299,   154,   299,    -1,   299,   150,   299,    -1,
     299,   159,   299,    -1,   299,   138,   299,    -1,   299,   157,
     299,    -1,   299,   158,   299,    -1,   299,   151,   299,    -1,
     299,   142,   299,    -1,   299,   152,   299,    -1,   299,   149,
     299,    -1,   299,   145,   299,    -1,   299,   148,   299,    -1,
     299,   144,   299,    -1,   299,   132,   299,    -1,   299,   134,
     299,    -1,   299,   135,   299,    -1,   299,   115,   299,    -1,
     299,   153,   299,    -1,   299,   143,   299,    -1,   299,    18,
     299,    -1,   299,    11,   299,    -1,   299,   146,   299,    -1,
     299,    30,   299,    -1,   154,   299,    -1,   150,   299,    -1,
      57,   299,    -1,    71,   299,    -1,   131,   299,    -1,   299,
     131,    -1,   133,   299,    -1,   299,    80,   299,    -1,   299,
      80,   209,    -1,   314,    80,   299,    -1,   314,    80,   209,
      -1,   299,    84,   299,    -1,   299,    84,   209,    -1,   314,
      84,   299,    -1,   314,    84,   209,    -1,   154,    -1,   159,
      -1,   115,    -1,   153,    -1,   132,    -1,   134,    -1,   135,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   495,   495,   500,   501,   507,   508,   513,   514,   519,
     520,   521,   522,   523,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
     540,   541,   542,   543,   544,   545,   546,   547,   548,   552,
     565,   570,   575,   583,   584,   585,   589,   590,   603,   604,
     605,   610,   611,   616,   621,   626,   630,   637,   642,   646,
     651,   655,   656,   657,   661,   662,   663,   667,   671,   673,
     675,   677,   679,   686,   687,   691,   692,   693,   694,   695,
     696,   699,   700,   701,   702,   703,   704,   716,   717,   728,
     729,   730,   731,   732,   733,   734,   735,   736,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   750,   751,
     752,   753,   754,   755,   756,   757,   758,   759,   760,   767,
     768,   769,   770,   774,   775,   779,   780,   784,   785,   786,
     787,   791,   792,   793,   794,   795,   796,   797,   801,   802,
     803,   804,   809,   808,   824,   823,   840,   839,   855,   854,
     870,   874,   879,   887,   898,   905,   906,   907,   908,   909,
     910,   911,   912,   913,   914,   915,   916,   917,   918,   919,
     920,   921,   922,   923,   929,   935,   941,   947,   954,   961,
     965,   972,   976,   977,   978,   979,   980,   982,   984,   986,
     991,   994,   995,   996,   997,   998,   999,  1003,  1004,  1008,
    1009,  1010,  1014,  1015,  1019,  1022,  1024,  1029,  1030,  1034,
    1036,  1038,  1045,  1055,  1069,  1074,  1079,  1087,  1088,  1093,
    1094,  1096,  1101,  1117,  1124,  1133,  1141,  1145,  1152,  1153,
    1158,  1163,  1157,  1190,  1193,  1197,  1205,  1215,  1204,  1247,
    1251,  1256,  1260,  1265,  1272,  1273,  1277,  1278,  1279,  1280,
    1281,  1282,  1283,  1284,  1285,  1286,  1287,  1288,  1289,  1290,
    1291,  1292,  1293,  1294,  1295,  1296,  1297,  1298,  1299,  1300,
    1301,  1302,  1303,  1304,  1308,  1309,  1310,  1311,  1312,  1313,
    1314,  1315,  1316,  1317,  1318,  1319,  1323,  1324,  1328,  1332,
    1333,  1334,  1338,  1340,  1342,  1344,  1346,  1351,  1352,  1356,
    1357,  1358,  1359,  1360,  1361,  1362,  1363,  1364,  1368,  1369,
    1370,  1371,  1372,  1373,  1377,  1378,  1382,  1383,  1384,  1385,
    1386,  1387,  1391,  1392,  1395,  1396,  1400,  1401,  1405,  1410,
    1411,  1412,  1420,  1421,  1423,  1425,  1427,  1432,  1434,  1439,
    1440,  1441,  1442,  1443,  1444,  1445,  1449,  1451,  1456,  1458,
    1460,  1465,  1478,  1495,  1496,  1498,  1503,  1504,  1505,  1506,
    1507,  1511,  1517,  1525,  1526,  1534,  1536,  1541,  1543,  1545,
    1550,  1552,  1554,  1561,  1562,  1563,  1568,  1570,  1572,  1576,
    1580,  1582,  1586,  1594,  1595,  1596,  1597,  1598,  1603,  1604,
    1605,  1606,  1607,  1627,  1631,  1635,  1643,  1650,  1651,  1652,
    1656,  1658,  1664,  1666,  1668,  1673,  1674,  1675,  1676,  1677,
    1683,  1684,  1685,  1686,  1690,  1691,  1695,  1696,  1697,  1701,
    1702,  1706,  1707,  1711,  1712,  1716,  1717,  1718,  1719,  1723,
    1724,  1735,  1737,  1739,  1745,  1746,  1747,  1748,  1749,  1750,
    1752,  1754,  1756,  1758,  1760,  1762,  1765,  1767,  1769,  1771,
    1773,  1775,  1777,  1779,  1782,  1784,  1786,  1791,  1793,  1795,
    1797,  1799,  1801,  1803,  1805,  1807,  1809,  1811,  1813,  1815,
    1822,  1828,  1834,  1840,  1849,  1859,  1867,  1868,  1869,  1870,
    1871,  1872,  1873,  1874,  1879,  1880,  1884,  1888,  1889,  1893,
    1897,  1898,  1902,  1906,  1910,  1917,  1918,  1919,  1920,  1921,
    1922,  1926,  1927,  1932,  1934,  1938,  1942,  1946,  1954,  1959,
    1965,  1971,  1978,  1987,  1989,  1994,  2002,  2003,  2004,  2005,
    2006,  2007,  2008,  2009,  2010,  2012,  2014,  2016,  2018,  2020,
    2025,  2026,  2030,  2031,  2032,  2036,  2037,  2038,  2039,  2050,
    2051,  2052,  2053,  2057,  2058,  2059,  2063,  2064,  2065,  2066,
    2067,  2075,  2076,  2077,  2078,  2082,  2083,  2087,  2088,  2089,
    2090,  2091,  2092,  2093,  2094,  2095,  2096,  2097,  2098,  2099,
    2103,  2111,  2112,  2116,  2117,  2118,  2119,  2120,  2121,  2122,
    2123,  2124,  2125,  2126,  2127,  2128,  2129,  2130,  2131,  2132,
    2133,  2134,  2135,  2136,  2137,  2138,  2142,  2143,  2144,  2145,
    2146,  2147,  2148,  2152,  2153,  2154,  2155,  2159,  2160,  2161,
    2162,  2167,  2168,  2169,  2170,  2171,  2172,  2173
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
  "TRP", "TLSBR", "TRSBR", "TNOELSE", "TUMINUS", "TUPLUS", "TLNOT",
  "$accept", "program", "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls",
  "stmt", "module_decl_start", "module_decl_stmt", "access_control",
  "opt_prototype", "block_stmt", "stmt_ls", "only_ls", "opt_only_ls",
  "except_ls", "use_access_control", "use_stmt", "require_stmt",
  "assignment_stmt", "opt_label_ident", "ident_fn_def", "ident_def",
  "ident_use", "internal_type_ident_def", "scalar_type",
  "reserved_type_ident_use", "do_stmt", "return_stmt", "class_level_stmt",
  "private_decl", "forwarding_stmt", "extern_export_decl_stmt", "$@1",
  "$@2", "$@3", "$@4", "extern_block_stmt", "loop_stmt",
  "zippered_iterator", "if_stmt", "defer_stmt", "try_stmt",
  "catch_stmt_ls", "catch_stmt", "catch_expr", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt", "enum_header",
  "enum_ls", "enum_item", "lambda_decl_expr", "$@5", "$@6", "linkage_spec",
  "fn_decl_stmt", "$@7", "$@8", "fn_decl_stmt_inner",
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
     425
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   171,   172,   173,   173,   174,   174,   175,   175,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   177,
     178,   178,   178,   179,   179,   179,   180,   180,   181,   181,
     181,   182,   182,   183,   183,   183,   183,   184,   184,   185,
     185,   186,   186,   186,   187,   187,   187,   188,   189,   189,
     189,   189,   189,   190,   190,   191,   191,   191,   191,   191,
     191,   192,   192,   192,   192,   192,   192,   193,   193,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   196,
     196,   196,   196,   197,   197,   198,   198,   199,   199,   199,
     199,   200,   200,   200,   200,   200,   200,   200,   201,   201,
     201,   201,   203,   202,   204,   202,   205,   202,   206,   202,
     202,   202,   202,   202,   207,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   209,   210,   210,   210,   210,   210,   210,   210,   210,
     211,   212,   212,   212,   212,   212,   212,   213,   213,   214,
     214,   214,   215,   215,   216,   217,   217,   218,   218,   219,
     219,   219,   220,   220,   221,   221,   221,   222,   222,   223,
     223,   223,   224,   224,   225,   226,   226,   226,   227,   227,
     229,   230,   228,   231,   231,   231,   233,   234,   232,   235,
     235,   235,   235,   235,   236,   236,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   239,   239,   240,   241,
     241,   241,   242,   242,   242,   242,   242,   243,   243,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   245,   245,
     245,   245,   245,   245,   246,   246,   247,   247,   247,   247,
     247,   247,   248,   248,   249,   249,   250,   250,   251,   252,
     252,   252,   253,   253,   253,   253,   253,   254,   254,   255,
     255,   255,   255,   255,   255,   255,   256,   256,   257,   257,
     257,   258,   258,   259,   259,   259,   260,   260,   260,   260,
     260,   261,   261,   262,   262,   263,   263,   264,   264,   264,
     265,   265,   265,   266,   266,   266,   267,   267,   267,   267,
     267,   267,   267,   268,   268,   268,   268,   268,   269,   269,
     269,   269,   269,   270,   270,   270,   270,   271,   271,   271,
     272,   272,   272,   272,   272,   273,   273,   273,   273,   273,
     274,   274,   274,   274,   275,   275,   276,   276,   276,   277,
     277,   278,   278,   279,   279,   280,   280,   280,   280,   281,
     281,   282,   282,   282,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   285,   286,   287,   287,   287,   287,
     287,   287,   287,   287,   288,   288,   289,   290,   290,   291,
     292,   292,   293,   293,   293,   294,   294,   294,   294,   294,
     294,   295,   295,   296,   296,   296,   296,   296,   297,   297,
     297,   297,   297,   297,   297,   298,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     300,   300,   301,   301,   301,   302,   302,   302,   302,   303,
     303,   303,   303,   304,   304,   304,   305,   305,   305,   305,
     305,   306,   306,   306,   306,   307,   307,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   309,   309,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   311,   311,   311,   311,
     311,   311,   311,   312,   312,   312,   312,   313,   313,   313,
     313,   314,   314,   314,   314,   314,   314,   314
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
       5,     2,     0,     8,     0,     9,     0,     8,     0,     9,
       3,     3,     5,     5,     2,     5,     3,     6,     6,     4,
       5,     5,     3,     3,     6,     5,     6,     5,     6,     3,
       4,     3,     4,     6,     7,     6,     7,     4,     5,     4,
       5,     4,     4,     3,     6,     5,     6,     5,     8,     7,
       2,     3,     3,     2,     2,     3,     3,     0,     2,     2,
       3,     5,     1,     3,     3,     5,     5,     0,     2,     3,
       2,     3,     6,     6,     1,     1,     1,     0,     2,     0,
       2,     3,     5,     5,     1,     1,     2,     3,     1,     3,
       0,     0,     8,     0,     1,     1,     0,     0,    10,     3,
       3,     5,     5,     3,     1,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     3,     3,     0,
       1,     3,     4,     5,     4,     6,     6,     0,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     0,     1,
       1,     2,     1,     1,     1,     1,     0,     1,     2,     1,
       1,     1,     0,     1,     1,     1,     1,     1,     2,     1,
       2,     2,     0,     2,     2,     4,     4,     1,     3,     3,
       3,     3,     3,     3,     3,     2,     1,     1,     3,     4,
       4,     2,     4,     0,     2,     2,     1,     1,     1,     2,
       1,     4,     3,     1,     3,     3,     5,     1,     1,     3,
       1,     2,     3,     0,     2,     2,     3,     2,     4,     3,
       3,     4,     3,     0,     2,     2,     2,     1,     0,     2,
       2,     2,     1,     4,     4,     6,     3,     0,     1,     1,
       3,     4,     3,     4,     6,     0,     2,     2,     2,     2,
       1,     1,     3,     3,     1,     3,     1,     1,     1,     3,
       3,     0,     1,     1,     3,     3,     3,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     2,
       4,     4,     4,     5,     2,     2,     1,     2,     1,     2,
       1,     2,     1,     2,     1,     1,     1,     6,     6,     4,
       9,     9,     7,     6,     6,     4,     9,     9,     7,     4,
       6,     6,     9,     9,     6,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     1,     4,     1,     3,     4,
       1,     3,     4,     3,     3,     1,     1,     2,     1,     2,
       1,     1,     3,     1,     2,     2,     2,     2,     2,     8,
       8,     9,     9,     5,     5,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     3,     3,     2,     2,     1,
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
       3,     0,     0,     1,     0,    87,   558,   559,   560,   561,
     562,   563,     0,   484,    73,   108,   452,   114,   454,   484,
       0,   113,     0,   357,    73,     0,     0,     0,     0,   456,
     530,   530,   555,     0,     0,     0,     0,   112,     0,   234,
     109,     0,   230,     0,     0,   116,     0,   503,   475,   564,
     117,     0,   235,   446,   356,     0,     0,     0,    45,    44,
     111,   455,   358,     0,     0,     0,     0,   450,     0,     0,
     115,     0,     0,    88,     0,   556,     0,     0,     0,   110,
     216,   448,   360,   118,     0,     0,   613,     0,   615,     0,
     616,   617,   529,     0,   614,   611,   433,   127,   612,     0,
       0,     0,     4,     0,     5,     0,     9,    46,    10,     0,
      11,    12,    14,   429,   430,    22,    13,   128,   136,   137,
      15,    17,    16,    19,    20,    21,    18,   135,     0,   133,
       0,   521,     0,   131,   134,     0,   132,   535,   517,   431,
     518,   436,   434,     0,     0,     0,   522,   523,     0,   435,
       0,   536,   537,   538,   557,   516,   438,   437,   519,   520,
       0,    38,    24,   444,     0,     0,   485,    74,     0,     0,
     454,   456,     0,     0,     0,   455,     0,     0,     0,   521,
     535,   434,   522,   523,   453,   435,   536,   537,     0,   484,
       0,     0,   359,     0,   190,     0,   414,     0,   421,   561,
     455,   531,   233,   561,   154,   455,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   141,     0,     0,   421,    81,
      89,   101,    95,    94,   103,    84,    93,   104,    90,   105,
      82,   106,    99,    92,   100,    98,    96,    97,    83,    85,
      91,   102,   107,     0,    86,     0,     0,     0,     0,   363,
       0,   124,    32,     0,   598,   507,   504,   505,   506,     0,
     447,   599,     7,   421,   214,   224,   530,   215,   130,   129,
     433,   411,     0,   410,     0,     0,   125,   534,     0,     0,
      35,     0,   451,   439,     0,   421,    36,   445,     0,   197,
     193,     0,   435,   197,   194,     0,   353,     0,   449,     0,
       0,   600,   602,   528,   597,   596,     0,    48,    51,     0,
       0,   416,     0,   418,     0,     0,   417,     0,     0,   410,
       0,     0,     6,     0,    47,     0,     0,   217,     0,   315,
     314,   236,     0,   432,    23,     0,   508,     0,     0,     0,
       0,     0,     0,   601,     0,     0,     0,     0,     0,     0,
     527,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   274,   281,   282,   283,
     278,   280,     0,     0,   276,   279,   277,   275,     0,   285,
     284,     0,     0,   421,   421,     0,     0,     0,    25,    26,
       0,     0,     0,     0,     0,    27,     0,     0,     0,     0,
      28,     0,    29,     0,   429,   427,     0,   422,   423,   428,
     148,     0,   151,     0,   144,     0,     0,   150,     0,     0,
       0,   163,     0,     0,   162,     0,   171,     0,     0,     0,
     169,     0,     0,    57,   138,     0,   183,     0,     0,    30,
     297,   231,   367,     0,   368,   370,     0,   392,     0,   373,
       0,     0,   123,    31,     0,     0,    33,     0,   328,     0,
      67,   532,   533,   126,     0,    34,   421,     0,   204,   195,
     191,   196,   192,     0,   351,   348,   156,    37,    50,    49,
      52,     0,   565,     0,     0,   551,     0,   553,     0,     0,
       0,     0,     0,     0,     0,     0,   569,     8,     0,    40,
       0,     0,     0,   410,     0,     0,     0,   308,   362,   483,
     593,   592,   595,   604,   603,   608,   607,   589,   586,   587,
     588,   525,   576,     0,   548,   549,   547,   546,   526,   580,
     591,   585,   583,   594,   584,   582,   574,   579,   581,   590,
     573,   577,   578,   575,     0,     0,     0,     0,     0,     0,
       0,   606,   605,   610,   609,   495,   496,   498,   500,     0,
     487,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     567,   484,   484,   159,   349,   361,   415,     0,     0,   441,
       0,     0,   217,     0,     0,   217,   350,     0,     0,     0,
       0,   459,     0,     0,     0,   172,     0,   465,     0,     0,
     170,   612,    60,     0,    53,    58,     0,   182,     0,     0,
       0,   440,   302,   299,   300,   301,   305,   306,   307,   297,
       0,   290,     0,   298,   316,     0,   371,     0,   121,   122,
     120,   119,     0,   391,   390,   517,     0,   365,   515,   364,
       0,     0,   545,   413,   412,     0,     0,     0,   442,     0,
     198,   355,   517,     0,   566,   524,   552,   419,   554,   420,
     179,     0,     0,     0,   568,   177,   469,     0,   571,   570,
       0,    42,    41,    39,    64,     0,    57,   218,     0,     0,
     228,     0,   225,   312,   309,   310,   313,   237,     0,     0,
      71,    72,    70,    69,    68,   543,   544,   497,   499,     0,
     486,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   155,   425,   426,   424,   217,     0,
     153,   217,     0,   152,     0,   181,     0,     0,   161,     0,
     160,     0,   490,     0,     0,   167,     0,     0,   165,     0,
       0,   139,     0,   140,     0,     0,   185,     0,   187,   303,
     304,     0,   297,   288,     0,   405,   317,   320,   319,   321,
       0,   369,   372,   373,     0,     0,   374,   375,   513,   514,
     206,     0,     0,   205,   208,   443,     0,   199,   202,     0,
     352,   180,     0,     0,     0,     0,   178,     0,     0,     0,
       0,   233,   223,     0,   226,   222,   311,   316,   286,    75,
     269,    89,   267,    95,    94,    78,    93,    90,   272,   105,
      76,   106,    92,    96,    77,    79,    91,   107,   266,   248,
     251,   249,   250,   261,   252,   265,   257,   255,   268,   271,
     256,   254,   259,   264,   253,   258,   262,   263,   260,   270,
       0,   246,     0,    80,     0,   286,   286,   244,   550,   488,
     373,   535,   535,     0,     0,     0,     0,     0,     0,     0,
       0,   158,   157,     0,   219,     0,   219,   164,     0,     0,
     458,     0,   457,     0,   489,     0,     0,   464,   168,     0,
     463,   166,    55,    54,   184,   474,   186,     0,   405,   291,
       0,     0,   373,   318,   332,   366,   396,     0,   567,   421,
     421,     0,   210,     0,     0,     0,   200,     0,   175,   471,
       0,     0,   173,   470,     0,   572,    65,    66,   213,   233,
     233,   212,   233,   220,   229,   227,     0,   297,   243,   247,
       0,   273,     0,   239,   240,   492,     0,     0,     0,     0,
       0,     0,     0,     0,   219,   233,   219,   233,   462,     0,
       0,   491,   468,     0,     0,     0,     0,   189,   373,   405,
       0,   408,   407,   409,   517,   329,   294,   292,     0,     0,
       0,     0,   394,   517,     0,     0,   211,   209,     0,   203,
       0,   176,     0,   174,   221,   387,     0,   322,     0,   245,
      75,    77,   286,   286,     0,     0,     0,     0,     0,     0,
     233,   147,   233,   143,     0,     0,     0,     0,    56,   188,
     293,   373,   397,     0,   331,   330,   346,     0,   347,   334,
     337,     0,   333,   326,   327,   232,     0,   509,   510,   201,
       0,     0,     0,   386,   385,   517,   323,   332,   287,   241,
     242,     0,     0,     0,     0,     0,     0,   149,   145,   461,
     460,   467,   466,   296,   295,   399,   400,   402,   517,     0,
     567,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   517,   511,   512,   473,   472,     0,   377,     0,     0,
       0,   401,   403,   336,   338,   339,   342,   343,   344,   340,
     341,   335,   382,   380,   517,   567,   324,   238,   325,   397,
     381,   517,   404
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   308,   103,   607,   105,   106,   107,   325,
     108,   309,   602,   606,   603,   109,   110,   111,   112,   168,
     841,   247,   113,   244,   114,   633,   252,   115,   116,   117,
     118,   119,   415,   584,   411,   581,   120,   121,   710,   122,
     123,   124,   469,   650,   779,   125,   126,   646,   774,   127,
     128,   505,   791,   129,   130,   681,   682,   179,   245,   624,
     132,   133,   507,   797,   687,   844,   845,   437,   928,   441,
     620,   621,   622,   623,   688,   331,   760,  1037,  1097,  1025,
     271,   966,   970,  1019,  1020,  1021,   134,   297,   474,   135,
     136,   248,   249,   445,   446,   637,  1034,   987,   449,   634,
    1056,   963,   892,   310,   195,   314,   315,   406,   407,   408,
     180,   138,   139,   140,   141,   181,   143,   165,   166,   559,
     427,   731,   560,   561,   144,   145,   182,   183,   148,   207,
     409,   185,   150,   186,   187,   153,   154,   155,   320,   156,
     157,   158,   159,   160
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -993
static const yytype_int16 yypact[] =
{
    -993,   103,  2980,  -993,   -77,  -993,  -993,  -993,  -993,  -993,
    -993,  -993,  4450,     1,   129,  -993, 13245,  -993, 15164,     1,
    9496,  -993,   189,   195,   129,  4450,  9496,  4450,    56, 16572,
    9659,  7876,  -993,  8039,  8681,  6735,  9496,  -993,   220,  -993,
    -993, 17525,  -993,  2474,  8844,  -993,  9496,    49,  -993,  -993,
    -993,  9496,  -993, 13245,  -993,  9496,   393,   240,  8525, 15548,
    -993, 17582,  -993,  9822,  7550,  9496,  8844, 13245,  9496,   318,
    -993,   261,  4450,  -993,  9496,  -993,  9985,  9985, 17525,  -993,
    -993, 13245,  -993,  -993,  9496,  9496,  -993,  9496,  -993,  9496,
    -993,  -993, 12756,  9496,  -993,  9496,  -993,  -993,  -993,  3472,
    6898,  9007,  -993,  4287,  -993,   251,  -993,   357,  -993,   406,
    -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,
    -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993, 17525,  -993,
   17525,   379,    21,  -993,  -993,  2474,  -993,   310,  -993,   368,
    -993,  -993,   362,   371,   372,  9496,   374,   378, 17264, 13242,
     292,   381,   390,  -993,  -993,  -993,  -993,  -993,  -993,  -993,
     313,  -993,  -993, 17264,   375,  4450,  -993,  -993,   414,  9496,
    -993,  -993,  9496,  9496,  9496,  -993,  9496,  9822,  9822,   308,
    -993,  -993,  -993,  -993,   168,   366,  -993,  -993,   412, 15237,
   17525,  2474,  -993,   430,  -993,   -46, 17264,   480,  8202,   513,
   17639, 17264,   653,   514,  -993, 17696, 17525,   653, 17525,   431,
       2, 14771,    15, 14739,   295,  -993, 15180, 13987,  8202,  -993,
    -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,
    -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,
    -993,  -993,  -993,  4450,  -993,   433,  2211,    62,   -16,  -993,
    4450,  -993,  -993, 14915,    93,  -993,   436,   439,  -993, 14915,
     168,    93,  -993,  8202,  -993,  -993,  9170,  -993,  -993,  -993,
   17525,  -993,   222, 17264,  9496,  9496,  -993, 17264,   447, 15148,
    -993, 14915,   168, 17264,   443,  8202,  -993, 17264, 15324,  -993,
    -993, 15589, 13371,  -993,  -993, 15638,   492,   456,   168, 14915,
   15733,   283,   283, 13699,    93,    93,   -70,  -993,  -993,  3635,
      75,  -993,  9496,  -993,    79,   116,  -993,   -35,   135, 15782,
     137,   608,  -993,  3798,  -993,   558,  9822,   481,   460,  -993,
    -993,  -993,   250,   382,  -993,  9496,   484,  9496,  9496,  9496,
    8681,  8681,  9496,   396,  9496,  9496,  9496,  9496,  9496,   449,
   12756,  9496,  9496,  9496,  9496,  9496,  9496,  9496,  9496,  9496,
    9496,  9496,  9496,  9496,  9496,  9496,  -993,  -993,  -993,  -993,
    -993,  -993,  8365,  8365,  -993,  -993,  -993,  -993,  8365,  -993,
    -993,  8365,  8365,  8202,  8202,  8681,  8681,  7061,  -993,  -993,
   15381, 15413, 15814,    57,  3961,  -993,  8681,     2,   469,   276,
    -993,  9496,  -993,  9496,   511,  -993,   466,   494,  -993,  -993,
    -993, 17525,  -993,  2474,  -993, 17525,   476,  -993,  2474,   592,
    9822,  -993,  4613,  8681,  -993,   477,  -993,     2,  4776,  8681,
    -993,     2, 10148,  9496,  -993,  4450,   610,  9496,   485,  -993,
      58,  -993,  -993,  2211,  -993,   504,   490,  -993, 10311,   532,
    9496,  2474,  -993,  -993,  9496,  9496,  -993,   491,  -993,  9822,
    -993, 17264, 17264,  -993,    43,  -993,  8202,   495,  -993,   636,
    -993,   636,  -993, 10474,   525,  -993,  -993,  -993,  -993,  -993,
    -993,  9333,  -993, 13745,  7224,  -993,  7387,  -993,  4450,   499,
    8681,  6243,  3309,   500,  9496,  6407,  -993,  -993,   149,  -993,
    4124, 17525,   290, 15557,  9822,   507, 16396,   298,  -993, 15958,
    2208,  2208,   249,  -993,   249,  -993,   249, 12911,   664,   828,
    2493,   168,   283,   508,  -993,  -993,  -993,  -993, 13699,  2794,
     249, 13070, 13070,  2208, 13070, 13070, 14675,   283,  2794, 17381,
   14675,    93,    93,   283,   520,   522,   524,   526,   527,   521,
     523,  -993,   249,  -993,   249,    27,  -993,  -993,  -993,   124,
    -993,  2678, 17340,   417, 10637,  8681, 10800,  8681,  9496,  8681,
   13509,     1, 16003,  -993,  -993,  -993, 17264, 16044,  8202,  -993,
    8202, 17525,   481,   311, 17525,   481,  -993,   319,  9496,   128,
    9496, 17264,    11, 14972,  7061,  -993,  9496, 17264,    26, 14828,
    -993,   537,   551,   540, 16155,   551,   545,   669, 16195,  4450,
   15004,  -993,   112,  -993,  -993,  -993,  -993,  -993,  -993,   600,
     138,  -993,  2623,  -993,   364,   543,  2211,    62,    56,   220,
    9496,  9496,  6080,  -993,  -993,   361,  7713,  -993, 17264,  -993,
   13802, 13843,  -993,  -993, 17264,   549,    72,   548,  -993,  2395,
    -993,  -993,   354, 17525,  -993,  -993,  -993,  -993,  -993,  -993,
    -993,  4450,   -14,  2454,  -993,  -993, 17264,  4450, 17264,  -993,
   16236,  -993,  -993,  -993,  -993, 10148,  9496,   577,  1541,   554,
     604,    96,  -993,   645,  -993,  -993,  -993,  -993,  8520,   571,
    -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,  7061,
    -993,    84,  8681,  8681,  9496,   706, 16380,  9496,   710, 16412,
     579,  2707,     2,     2,  -993,  -993,  -993,  -993,   481,   582,
    -993,   481,   590,  -993, 14915,  -993, 14063,  4939,  -993,  5102,
    -993,   142,  -993, 14139,  5265,  -993,     2,  5428,  -993,     2,
    9496,  -993,  9496,  -993,  4450,  9496,  -993,  4450,   720,  -993,
    -993, 17525,   680,  -993,  2211,   618,   676,  -993,  -993,  -993,
      46,  -993,  -993,   532,   596,    90,  -993,  -993,   595,   601,
    -993,  5591,  9822,  -993,  -993,  -993, 17525,  -993,   627,   412,
    -993,  -993,  5754,   599,  5917,   602,  -993,  9496,   613,   617,
     614,  1131,  -993,  9496, 17525,  -993,  -993,   364,   603,   373,
    -993,   638,  -993,   639,   643,   656,   649,   650,  -993,   651,
     661,   654,   660,   662,   377,   674,   667,   670,  -993,  -993,
   17525,  -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,
    -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,
    9496,  -993,   677,   679,   681,   603,   603,  -993,  -993,  -993,
     532,   167,   217, 16556, 10963, 11126, 16588, 11289, 11452, 11615,
   11778,  -993,  -993,   655,  -993,   658,  -993,  -993,  4450,  9496,
   17264,  9496, 17264,  7061,  -993,  4450,  9496, 17264,  -993,  9496,
   17264,  -993, 16645, 17264,  -993, 17264,   791,  4450,   618,  -993,
     663, 11941,   171,  -993,    45,  -993,  -993,  8681, 13377,  8202,
    8202,  4450,  -993,     9,   666,  9496,  -993,  9496,  -993, 17264,
    4450,  9496,  -993, 17264,  4450, 17264,  -993,  -993,  -993, 12796,
   12796,  -993,  2126,  -993, 17264,  -993,   121,    58,  -993,  -993,
   13889,  -993, 13601,  -993,  -993,  -993,  9496,  9496,  9496,  9496,
    9496,  9496,  9496,  9496,  -993,  1318,  -993,  1701, 16195, 14215,
   14291,  -993, 16195, 14367, 14443,  9496,  4450,  -993,   532,   618,
    6571,  -993,  -993,  -993,   387,  9822,  -993,  -993,   166,  9496,
     -34,  2748,  -993,   630,   668,   671,   480,  -993,   412, 17264,
   14519,  -993, 14595,  -993,  -993,  -993, 12104,   729,   218,  -993,
     695,   700,   603,   603, 16732, 16808, 16884, 16960, 17036, 17112,
    1788,  -993,  1931,  -993,  4450,  4450,  4450,  4450, 17264,  -993,
    -993,   171, 12267,    98,  -993, 17264,  -993,   144,  -993,    76,
    -993,   465, 17188,  -993,  -993,  -993, 11778,   678,   682,  -993,
    4450,  4450,  3143,  -993,  -993,   201,  -993,    45,  -993,  -993,
    -993,  9496,  9496,  9496,  9496,  9496,  9496,  -993,  -993, 16195,
   16195, 16195, 16195,  -993,  -993,  -993,  -993,  -993,   434,  8681,
   13082,  -993,  9496,   166,   144,   144,   144,   144,   144,   144,
     166,  1025,  -993,  -993, 16195, 16195,   672, 12593,   130,   -45,
   13657,  -993,  -993, 17264,  -993,  -993,  -993,  -993,  -993,  -993,
    -993,   699,  -993,  -993,   353, 12919,  -993,  -993,  -993, 12430,
    -993,   411,  -993
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -993,  -993,  -993,    -1,  -350,  2026,  -993,  -993,  -993,  -993,
     352,   516,  -408,   174,   176,  -993,  -993,  -993,   363,   829,
    -993,    20,  -181,  -682,  -993,  -798,   105,  -939,  -755,   -47,
    -993,  -993,  -993,  -993,  -993,  -993,  -993,  -993,   110,  -993,
    -993,  -993,   559,  -993,    78,  -993,  -993,  -993,  -993,  -993,
    -993,  -537,  -675,  -993,  -993,  -993,    61,  1014,  -993,  -993,
    -993,    91,  -993,  -993,  -993,  -993,   -72,  -147,  -751,  -993,
     -65,   113,   247,  -993,  -993,  -993,    71,  -993,  -993,  -210,
     586,  -141,  -163,  -195,  -186,  -580,  -993,  -177,  -993,    -6,
     843,  -132,   428,  -993,  -417,  -733,  -786,  -993,  -577,  -468,
    -992,  -945,  -810,   -43,  -993,    99,  -993,  -144,  -993,   300,
     488,  -438,  -993,  -993,  -993,  1103,  -993,     8,  -993,  -993,
    -204,  -993,  -579,  -993,  -993,  -993,  1182,  1214,   -12,   851,
     256,   633,  -993,  1653,  1961,  -993,  -993,  -993,  -993,  -993,
    -993,  -993,  -993,  -380
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -568
static const yytype_int16 yytable[] =
{
     163,   102,   382,   332,   184,   651,   843,   563,   189,   431,
     635,   268,   269,   398,   196,   732,   191,   404,   201,   201,
     272,   211,   213,   216,   217,   605,   625,   188,   450,   416,
     895,  1024,   253,   250,   254,   652,   923,   404,    64,   259,
     250,   260,   727,   261,   645,   719,   250,   447,   722,    64,
     763,   273,   277,   279,   281,   282,   283,   734,   318,   399,
     287,   243,   288,   447,   291,   295,   255,  1057,  1081,   298,
     329,   697,   299,   300,   438,   301,   425,   302,   958,   161,
     303,   304,   404,   305,   612,   447,   161,   273,   277,   319,
     619,   401,   478,   961,   933,   934,   330,   425,   296,   968,
    -388,   569,   613,     3,   404,   614,  -388,  1102,   698,  -207,
     402,  1096,   164,   489,   493,  1082,   394,   935,   256,   457,
     849,   451,   985,   339,   850,   615,   425,   394,   616,  -388,
      56,   488,   167,   336,   897,   393,   257,   425,   771,   617,
    1024,   467,  1059,   210,   212,   382,   459,  1016,   327,  1011,
     328,  -207,   782,   258,   969,  -388,   749,   163,   618,   967,
     390,   391,   392,   394,   287,   273,   319,   984,   527,  1016,
     394,   280,   394,   340,   569,  -383,   394,   341,  -388,   490,
     772,   863,   448,   894,   865,  1062,   277,   394,  1033,   945,
     923,   947,   923,   750,   491,  -289,   413,   397,   448,  -388,
    -388,   418,   404,   404,  -383,  -207,   277,  -388,   191,   762,
     296,   317,   481,  1063,   563,    23,   484,  -383,  -388,   198,
     448,  -388,  -289,   570,   343,  1010,   296,   491,   419,   347,
    -383,   348,   349,   794,   773,   491,   352,   482,  1018,   549,
     550,  1039,  1040,   485,   359,   923,   425,   923,  -388,  1017,
     843,   277,   365,   486,   201,  -384,   898,   986,   795,    54,
    1018,   699,   461,   462,  1060,   459,   444,   491,   605,  1000,
      62,  1002,   491,   277,   495,   752,   192,  -383,  1054,   873,
     487,   583,  -383,   502,  -384,   404,   587,   636,   700,   190,
     458,  1093,   725,   412,   951,    82,  1095,  -384,   417,   343,
     483,   492,   753,   496,  -494,   161,   874,   349,   480,  1100,
    -384,   671,   965,   339,   503,   421,   424,   426,   430,   563,
     278,    23,   647,   509,   683,   510,   511,   512,   514,   516,
     517,  -494,   518,   519,   520,   521,   522,   890,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
     540,   541,   542,   543,  -493,   752,   316,  -384,   453,   459,
     277,   277,  -384,   340,   456,    54,   277,   341,   684,   277,
     277,   277,   277,   552,   554,   562,    62,   589,   460,   685,
     343,  -493,  1038,   218,   572,   347,   465,   451,   349,   576,
     756,   577,   352,   385,   736,   739,   251,   386,   686,   404,
     262,    82,   619,   263,   476,  -389,   508,  -376,   273,   284,
     591,   593,   323,   451,   343,  -389,   597,   599,   251,   347,
     604,   604,   349,   608,   285,   610,   352,   459,   289,   293,
     972,   582,   575,   324,   757,   585,  -376,  1061,   638,   290,
     294,   922,   640,   641,  -389,   758,   674,   644,   451,  -376,
     513,   515,     5,   964,   277,   383,   451,   384,   783,   785,
     973,   677,  -376,   444,   759,  -378,  -477,   720,   523,   644,
    -389,  -542,   277,  -542,   277,   723,   780,  -389,   663,   644,
     666,   524,   668,   670,  1085,  1086,  1087,  1088,  1089,  1090,
     137,  -354,   273,   563,  -378,   551,   553,   703,  -389,   480,
     137,   386,   573,  -406,   -81,   525,   571,  -378,   -83,  -376,
    -354,   326,   -87,   137,  -376,   137,   -88,  -389,  -476,   335,
    -378,   673,  -389,   333,  -406,  -389,   680,   334,  -406,  -539,
    -481,  -539,   595,   592,  -482,  -480,   600,  -479,   387,   598,
     395,   846,  -542,    73,  -542,  -541,  -478,  -541,  1035,   526,
    -398,  -406,   591,   706,   597,   709,   608,   711,   666,  -540,
     137,  -540,   251,   251,   251,   251,   277,  -378,   277,   436,
     389,  -398,  -378,   394,  1058,  -398,   724,   619,   726,   712,
     713,  1064,   562,   657,   733,   659,   400,   137,  1071,   765,
     403,   137,   410,   414,   420,   922,   440,   922,  -398,   454,
     662,   718,   455,   463,   721,   251,   466,  1065,   473,  1066,
    1067,   251,   475,  1068,  1069,   497,   501,   504,   283,   287,
     319,   506,  1058,   349,   277,   574,   612,   578,   544,   545,
     579,   580,   586,   251,   546,   149,   588,   547,   548,  1094,
     594,   626,   755,   609,   613,   149,   444,   614,   636,   611,
     922,   251,   922,   137,   627,   642,   649,  1101,   149,   648,
     149,  1058,   653,   604,   604,   661,   667,   615,   678,   778,
     616,   689,   321,   296,  -393,   705,   690,   708,   691,    23,
     692,   617,   693,   694,  -393,   695,   313,   562,   740,   696,
     514,   552,   853,   -59,   339,   856,   741,   728,   730,    39,
     618,   743,   744,   735,   738,   149,   612,   761,   842,   292,
     292,   770,   775,  -393,   459,   870,   792,   872,   404,   404,
     793,    52,   877,    54,   613,   880,   796,   614,   882,   903,
     883,   137,   149,   885,    62,   848,   149,   854,   137,  -393,
     316,   857,   316,   864,   340,   859,  -393,   615,   341,   251,
     616,   866,    56,   887,   891,   974,   975,   893,   899,    82,
     273,   617,   896,   905,   900,   910,   927,  -393,   914,   916,
     909,   888,   913,   917,   444,   915,   918,  -108,  -114,   251,
     618,   924,  -113,   251,   405,   993,  -393,   -84,  -112,  -109,
    -116,  -393,   -82,  -117,  -393,   343,   778,   137,   149,  -111,
     347,  -115,   348,   349,   405,   -85,  -110,   352,   931,  -118,
     -86,   137,   513,   551,   680,   359,   944,   861,   862,   946,
     932,   363,   364,   365,   956,  1036,   -81,   959,   930,   867,
     978,   -83,  1027,  1072,   716,  1028,  1063,  1073,  1092,   500,
     929,   878,   870,   872,   881,   877,   880,   909,   913,   405,
     789,   788,   471,   193,   904,   925,   948,   949,   339,   950,
     992,   562,   988,   952,   953,   889,   751,   954,   926,  1098,
    1053,   405,   268,   269,  1079,  1091,   149,  1084,   215,   639,
     717,   202,   137,   149,     0,   971,   666,   277,   277,     0,
       0,     0,   767,   979,     0,   980,     0,     0,     0,   982,
       0,     0,     0,     0,     0,     0,     0,     0,   340,     0,
     137,     0,   341,     0,     0,     0,   137,  1013,     0,     0,
       0,     0,     0,   137,   948,   994,   995,   952,   996,   997,
     998,   999,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   149,  1008,   251,   251,     0,     0,   319,     0,
     251,   251,   842,  1015,     0,     0,   149,  1022,     0,   343,
     344,     0,   748,   346,   347,     0,   348,   349,     0,   405,
     405,   352,     0,     0,     0,     0,   137,     0,     0,   359,
     137,     0,     0,     0,     0,   363,   364,   365,   137,  1078,
       0,     0,  1049,  1050,  1051,  1052,     0,     0,     0,     0,
       0,   777,     0,     0,     0,     0,     0,     0,   977,     0,
       0,     0,     0,     0,   913,     0,   131,     0,  1074,  1075,
     319,     0,     0,     0,     0,     0,   131,   149,     0,  1049,
    1050,  1051,  1052,  1074,  1075,     0,     0,     0,     0,   131,
       0,   131,     0,     0,     0,   643,     0,  1080,   666,   701,
    1083,     0,   405,     0,     0,   149,     0,     0,     0,     0,
       0,   149,     0,     0,   251,   251,     0,   643,   149,  -395,
     313,     0,   313,     0,     0,     0,   251,   643,   436,  -395,
       0,     0,     0,   666,     0,   436,   131,   913,   251,     0,
       0,   251,     0,     0,     0,     0,     0,   137,     0,     0,
       0,     0,     0,     0,     0,   142,     0,     0,  -395,     0,
       0,     0,     0,   131,     0,   142,     0,   131,     0,     0,
       0,   149,     0,     0,     0,   149,     0,     0,   142,     0,
     142,   906,     0,   149,  -395,     0,     0,     0,     0,     0,
       0,  -395,     0,     0,     0,     0,     0,     0,     0,   137,
       0,     0,   264,     0,     0,   137,    22,    23,     0,     0,
       0,     0,  -395,     0,   715,   265,   405,    30,   266,     0,
       0,     0,    35,     0,     0,   142,   847,    39,     0,   131,
       0,  -395,     0,     0,   146,     0,  -395,     0,     0,  -395,
     851,   852,     0,     0,   146,     0,     0,     0,     0,    52,
       0,    54,   142,    56,     0,   919,   142,   146,   920,   146,
     267,     0,    62,     0,     0,   137,   147,   137,     0,     0,
       0,     0,   137,     0,     0,   137,   147,     0,     0,     0,
       0,    78,   137,     0,    80,   137,     0,    82,     0,   147,
       0,   147,   149,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   146,   251,     0,   131,     0,   137,
       0,     0,     0,     0,   131,     0,     0,     0,   142,     0,
     137,     0,   137,     0,     0,     0,     0,     0,     0,     0,
       0,   146,     0,     0,     0,   146,   147,    97,     0,     0,
       0,     0,     0,   921,   149,     0,     0,     0,     0,     0,
     149,   436,   436,     0,     0,   436,   436,     0,     0,     0,
       0,     0,     0,   147,     0,     0,     0,   147,     0,     0,
       0,     0,  1023,   131,     0,     0,     0,     0,     0,     0,
    1029,     0,   436,     0,   436,     0,     0,   131,     0,   264,
       0,     0,     0,    22,    23,     0,   142,   146,     0,     0,
       0,     0,   265,   142,    30,   266,   137,     0,     0,    35,
     149,     0,   149,   137,    39,     0,     0,   149,     0,     0,
     149,     0,     0,     0,     0,   137,     0,   149,     0,   147,
     149,     0,     0,     0,     0,     0,    52,     0,    54,   137,
      56,     0,   919,     0,     0,   920,     0,   267,   137,    62,
       0,     0,   137,     0,   149,     0,     0,     0,   131,     0,
       0,     0,   142,     0,     0,   149,     0,   149,    78,     0,
       0,    80,     0,     0,    82,   146,   142,     0,     0,     0,
       0,  1023,   146,     0,     0,     0,   131,     0,     0,     0,
       0,     0,   131,     0,   137,     0,     0,     0,     0,   131,
       0,     0,     0,     0,     0,     0,     0,   147,     0,     0,
       0,     0,     0,     0,   147,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,   962,     0,     0,
    1001,     0,     0,     0,     0,   405,   405,     0,     0,     0,
       0,   146,   137,   137,   137,   137,     0,   142,     0,     0,
       0,   149,   131,     0,     0,   146,   131,     0,   149,     0,
       0,     0,     0,     0,   131,     0,     0,     0,   137,   137,
     149,     0,     0,   147,     0,   142,     0,     0,     0,     0,
       0,   142,     0,     0,   149,     0,     0,   147,   142,     0,
       0,     0,   790,   149,     0,     0,     0,   149,     0,     0,
       0,  1014,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -219,     0,     0,     0,  -219,  -219,     0,     0,
       0,     0,     0,     0,     0,  -219,   146,  -219,  -219,     0,
       0,     0,  -219,     0,     0,     0,     0,  -219,     0,   149,
    -219,   142,     0,     0,     0,   142,     0,     0,  1055,     0,
       0,     0,     0,   142,   146,     0,     0,     0,   147,  -219,
     146,  -219,     0,  -219,     0,  -219,  -219,   146,  -219,     0,
    -219,     0,  -219,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   147,   149,   149,   149,
     149,  -219,   147,     0,  -219,     0,  1055,  -219,     0,   147,
       0,     0,     0,     0,     0,   151,     0,     0,     0,     0,
       0,     0,     0,   149,   149,   151,     0,     0,     0,     0,
     146,     0,     0,     0,   146,   131,     0,     0,   151,     0,
     151,   131,   146,     0,     0,  1055,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -219,     0,     0,
       0,     0,   147,  -219,     0,     0,   147,     0,     0,     0,
       0,     0,   142,     0,   147,     0,     0,     0,     0,     0,
       0,     0,   264,     0,     0,   151,    22,    23,     0,     0,
       0,     0,     0,     0,     0,   265,     0,    30,   266,     0,
       0,   131,    35,   131,     0,     0,     0,    39,   131,     0,
       0,   131,   151,     0,     0,     0,   151,     0,   131,     0,
       0,   131,     0,     0,   142,     0,     0,     0,     0,    52,
     142,    54,     0,    56,     0,   919,     0,     0,   920,     0,
     267,     0,    62,     0,     0,   131,     0,     0,     0,     0,
       0,   146,     0,     0,     0,     0,   131,     0,   131,     0,
       0,    78,     0,     0,    80,     0,     0,    82,     0,   264,
       0,     0,     0,    22,    23,     0,     0,     0,   151,     0,
       0,     0,   265,   147,    30,   266,     0,     0,     0,    35,
     142,     0,   142,     0,    39,     0,     0,   142,     0,     0,
     142,     0,     0,   146,     0,     0,     0,   142,     0,   146,
     142,     0,     0,     0,     0,     0,    52,    97,    54,     0,
      56,     0,   919,  1003,     0,   920,     0,   267,     0,    62,
       0,     0,     0,     0,   142,   147,     0,     0,     0,     0,
       0,   147,   131,     0,     0,   142,     0,   142,    78,   131,
       0,    80,     0,     0,    82,     0,   151,     0,     0,     0,
       0,   131,     0,   151,     0,     0,     0,     0,     0,   146,
       0,   146,     0,     0,     0,   131,   146,     0,     0,   146,
       0,     0,     0,     0,   131,     0,   146,     0,   131,   146,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   147,     0,   147,    97,     0,     0,     0,   147,     0,
    1047,   147,   264,   146,     0,     0,    22,    23,   147,     0,
       0,   147,   151,   152,   146,   265,   146,    30,   266,     0,
     131,   142,    35,   152,     0,     0,   151,    39,   142,     0,
       0,     0,     0,     0,     0,   147,   152,     0,   152,     0,
     142,     0,     0,     0,     0,     0,   147,     0,   147,    52,
       0,    54,     0,    56,   142,   919,     0,     0,   920,     0,
     267,     0,    62,   142,     0,     0,     0,   142,   131,   131,
     131,   131,     0,     0,     0,     0,     0,     0,   104,     0,
       0,    78,     0,   152,    80,     0,     0,    82,   162,     0,
       0,     0,     0,     0,   131,   131,     0,   151,     0,     0,
     146,   194,     0,   197,     0,     0,     0,   146,     0,   142,
     152,     0,     0,     0,   152,     0,     0,     0,     0,   146,
       0,     0,     0,     0,     0,   151,     0,     0,     0,     0,
       0,   151,   147,   146,     0,     0,     0,    97,   151,   147,
       0,     0,   146,  1048,     0,     0,   146,     0,   286,     0,
       0,   147,     0,     0,     0,     0,     0,   142,   142,   142,
     142,     0,     0,     0,     0,   147,     0,     0,     0,     0,
       0,     0,     0,     0,   147,   104,   152,     0,   147,   322,
       0,     0,     0,   142,   142,     0,     0,     0,   146,     0,
       0,   151,     0,     0,     0,   151,     0,   264,     0,     0,
       0,    22,    23,   151,     0,     0,     0,     0,     0,     0,
     265,     0,    30,   266,     0,     0,     0,    35,     0,     0,
     147,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   146,   146,   146,   146,
       0,   388,     0,     0,    52,     0,    54,     0,   321,     0,
     919,     0,     0,   920,   152,   267,     0,    62,     0,     0,
       0,   152,   146,   146,   219,     0,     0,     0,   147,   147,
     147,   147,     0,     0,     0,     0,    78,   220,   221,    80,
     222,     0,    82,     0,     0,   223,     0,     0,   339,     0,
       0,     0,     0,   224,   147,   147,     0,     0,     0,   225,
       0,     0,     0,     0,   226,     0,   227,     0,     0,   228,
       0,     0,   151,     0,     0,     0,     0,   229,     0,   439,
     152,     0,     0,   230,   231,     0,   452,     0,     0,     0,
     232,     0,    97,     0,   152,     0,     0,     0,   340,   233,
       0,     0,   341,     0,     0,     0,     0,     0,   234,   235,
       0,   236,     0,   237,     0,   238,     0,     0,   239,     0,
       0,     0,   240,   442,   151,   241,     0,     0,   242,     0,
     151,     0,     0,   342,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   104,     0,     0,     0,   343,
     344,     0,   345,   346,   347,     0,   348,   349,   350,   104,
     351,   352,   353,   354,     0,   152,   356,   357,   358,   359,
     360,   361,   362,     0,     0,   363,   364,   365,     0,     0,
       0,     0,     0,     0,   443,     0,     0,     0,     0,     0,
     151,     0,   151,   152,     0,     0,     0,   151,     0,   152,
     151,     0,     0,     0,     0,     0,   152,   151,   219,     0,
     151,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   220,   221,     0,   222,     0,     0,     0,     0,   223,
     104,     0,     0,     0,   151,     0,     0,   224,     0,     0,
       0,     0,     0,   225,     0,   151,     0,   151,   226,     0,
     227,     0,     0,   228,     0,     0,     0,     0,   452,   152,
       0,   229,     0,   152,   452,     0,     0,   230,   231,     0,
       0,   152,     0,     0,   232,   337,     0,     0,     0,     0,
       0,     0,   338,   233,     0,     0,     0,   219,     0,     0,
       0,     0,   234,   235,   339,   236,     0,   237,     0,   238,
     220,   221,   239,   222,     0,     0,   240,     0,   223,   241,
       0,     0,   242,     0,     0,     0,   224,     0,     0,     0,
       0,     0,   225,     0,   660,     0,     0,   226,   665,   227,
       0,   151,   228,   339,     0,     0,   104,     0,   151,     0,
     229,     0,     0,     0,   340,     0,   230,   231,   341,     0,
     151,     0,     0,   232,     0,     0,     0,     0,     0,     0,
       0,     0,   233,     0,   151,     0,   394,     0,   776,     0,
       0,   234,   235,   151,   236,   425,   237,   151,   238,   342,
     152,   239,     0,   340,     0,   240,     0,   341,   241,     0,
       0,   242,     0,     0,     0,   343,   344,     0,   345,   346,
     347,     0,   348,   349,   350,     0,   351,   352,   353,   354,
     355,     0,   356,   357,   358,   359,   360,   361,   362,   151,
       0,   363,   364,   365,     0,     0,     0,     0,     0,     0,
     784,     0,   152,     0,   343,   344,   219,     0,   152,   347,
       0,   348,   349,     0,     0,   746,   352,   246,     0,   220,
     221,     0,   222,     0,   359,     0,     0,   223,     0,     0,
     363,   364,   365,     0,     0,   224,     0,   151,   151,   151,
     151,   225,     0,     0,     0,     0,   226,     0,   227,     0,
       0,   228,     0,     0,     0,     0,     0,     0,     0,   229,
       0,     5,     0,   151,   151,   230,   231,   781,   152,     0,
     152,     0,   232,   786,    15,   152,     0,    17,   152,     0,
       0,   233,    21,     0,     0,   152,     0,     0,   152,     0,
     234,   235,     0,   236,     0,   237,     0,   238,   337,     0,
     239,    37,     0,     0,   240,   338,    40,   241,     0,     0,
     242,     0,   152,     0,    45,     0,     0,   339,     0,     0,
       0,    50,     0,   152,     0,   152,     0,     0,     0,     0,
       0,     0,     0,   452,     0,   452,    60,     0,     0,   337,
     452,     0,     0,   452,     0,     0,   338,     0,    70,     0,
     884,     0,    73,   886,     0,     0,     0,     0,   339,    79,
       0,     0,     0,     0,     0,    83,   754,   340,     0,     0,
       0,   341,     0,     0,     0,     0,     0,   902,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   908,     0,
     912,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,   339,     0,     0,     0,   340,   152,
       0,     0,   341,     0,     0,     0,   152,     0,   343,   344,
       0,   345,   346,   347,     0,   348,   349,   350,   152,   351,
     352,   353,   354,   355,     0,   356,   357,   358,   359,   360,
     361,   362,   152,   342,   363,   364,   365,     0,     0,     0,
       0,   152,     0,   860,   340,   152,     0,     0,   341,   343,
     344,     0,   345,   346,   347,     0,   348,   349,   350,     0,
     351,   352,   353,   354,   355,     0,   356,   357,   358,   359,
     360,   361,   362,     0,     0,   363,   364,   365,     0,     0,
       0,     0,     0,   957,  1026,     0,     0,   152,     0,     0,
       0,     0,     0,     0,     0,   343,   344,   976,   345,   346,
     347,     0,   348,   349,   350,     0,   981,   352,   353,   354,
     983,     0,   356,   357,   358,   359,     0,     0,   362,     0,
       0,   363,   364,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   152,   152,   152,   152,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      -2,     4,  1009,     5,     6,     7,     8,     9,    10,    11,
       0,   152,   152,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,     0,    38,    39,     0,    40,  -233,
       0,    41,    42,    43,     0,    44,    45,    46,   -43,    47,
      48,     0,    49,    50,    51,     0,     0,     0,    52,    53,
      54,    55,    56,    57,    58,  -233,   -43,    59,    60,    61,
       0,    62,    63,    64,     0,    65,    66,    67,    68,    69,
      70,    71,    72,     0,    73,    74,     0,    75,    76,    77,
      78,    79,     0,    80,    81,   -61,    82,    83,     0,     0,
      84,     0,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,    97,     0,     0,    98,
       0,    99,     0,   100,  1076,   101,     5,     6,     7,     8,
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
       0,     0,     0,    93,     0,     0,    94,    95,   270,     0,
       0,     0,    98,     0,   177,     0,   100,     0,   178,  1077,
       4,     0,     5,     6,     7,     8,     9,    10,    11,     0,
    -567,     0,    12,    13,    14,    15,    16,  -567,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,  -567,
      27,    28,  -567,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,     0,    38,    39,     0,    40,  -233,     0,
      41,    42,    43,     0,    44,    45,    46,   -43,    47,    48,
       0,    49,    50,    51,     0,     0,     0,    52,    53,    54,
      55,     0,    57,    58,  -233,   -43,    59,    60,    61,  -567,
      62,    63,    64,  -567,    65,    66,    67,    68,    69,    70,
      71,    72,     0,    73,    74,     0,    75,    76,    77,    78,
      79,     0,    80,    81,   -61,    82,    83,     0,     0,    84,
       0,    85,     0,     0,  -567,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -567,  -567,    89,  -567,  -567,  -567,  -567,  -567,  -567,  -567,
       0,  -567,  -567,  -567,  -567,  -567,     0,  -567,  -567,  -567,
    -567,  -567,  -567,  -567,  -567,    97,  -567,  -567,  -567,     0,
      99,  -567,   100,   306,   101,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,     0,    38,    39,     0,
      40,  -233,     0,    41,    42,    43,     0,    44,    45,    46,
     -43,    47,    48,     0,    49,    50,    51,     0,     0,     0,
      52,    53,    54,    55,    56,    57,    58,  -233,   -43,    59,
      60,    61,     0,    62,    63,    64,     0,    65,    66,    67,
      68,    69,    70,    71,    72,     0,    73,    74,     0,    75,
      76,    77,    78,    79,     0,    80,    81,   -61,    82,    83,
       0,     0,    84,     0,    85,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,   270,    97,     0,
       0,    98,     0,    99,   307,   100,     4,   101,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,     0,
      38,    39,     0,    40,  -233,     0,    41,    42,    43,     0,
      44,    45,    46,   -43,    47,    48,     0,    49,    50,    51,
       0,     0,     0,    52,    53,    54,    55,    56,    57,    58,
    -233,   -43,    59,    60,    61,     0,    62,    63,    64,     0,
      65,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      74,     0,    75,    76,    77,    78,    79,     0,    80,    81,
     -61,    82,    83,     0,     0,    84,     0,    85,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,    97,     0,     0,    98,     0,    99,   479,   100,   498,
     101,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,     0,    38,    39,     0,    40,  -233,     0,    41,
      42,    43,     0,    44,    45,    46,   -43,    47,    48,     0,
      49,    50,    51,     0,     0,     0,    52,    53,    54,    55,
      56,    57,    58,  -233,   -43,    59,    60,    61,     0,    62,
      63,    64,     0,    65,    66,    67,    68,    69,    70,    71,
      72,     0,    73,    74,     0,    75,    76,    77,    78,    79,
       0,    80,    81,   -61,    82,    83,     0,     0,    84,     0,
      85,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,    97,     0,     0,    98,     0,    99,
     499,   100,   306,   101,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,     0,    38,    39,     0,    40,
    -233,     0,    41,    42,    43,     0,    44,    45,    46,   -43,
      47,    48,     0,    49,    50,    51,     0,     0,     0,    52,
      53,    54,    55,    56,    57,    58,  -233,   -43,    59,    60,
      61,     0,    62,    63,    64,     0,    65,    66,    67,    68,
      69,    70,    71,    72,     0,    73,    74,     0,    75,    76,
      77,    78,    79,     0,    80,    81,   -61,    82,    83,     0,
       0,    84,     0,    85,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,    97,     0,     0,
      98,     0,    99,   307,   100,     4,   101,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,     0,    38,
      39,     0,    40,  -233,     0,    41,    42,    43,     0,    44,
      45,    46,   -43,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,    56,    57,    58,  -233,
     -43,    59,    60,    61,     0,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
       0,    75,    76,    77,    78,    79,     0,    80,    81,   -61,
      82,    83,     0,     0,    84,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,   672,   100,     4,   101,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,    39,     0,    40,  -233,     0,    41,    42,
      43,     0,    44,    45,    46,   -43,    47,    48,     0,    49,
      50,    51,     0,     0,     0,    52,    53,    54,    55,   321,
      57,    58,  -233,   -43,    59,    60,    61,     0,    62,    63,
      64,     0,    65,    66,    67,    68,    69,    70,    71,    72,
       0,    73,    74,     0,    75,    76,    77,    78,    79,     0,
      80,    81,   -61,    82,    83,     0,     0,    84,     0,    85,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,    97,     0,     0,    98,     0,    99,     0,
     100,     4,   101,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,     0,    38,    39,     0,    40,  -233,
       0,    41,    42,    43,     0,    44,    45,    46,   -43,    47,
      48,     0,    49,    50,    51,     0,     0,     0,    52,    53,
      54,    55,     0,    57,    58,  -233,   -43,    59,    60,    61,
       0,    62,    63,    64,     0,    65,    66,    67,    68,    69,
      70,    71,    72,     0,    73,    74,     0,    75,    76,    77,
      78,    79,     0,    80,    81,   -61,    82,    83,     0,     0,
      84,     0,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,    97,     0,     0,    98,
       0,    99,     0,   100,     4,   101,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,   590,    37,     0,    38,    39,
       0,    40,  -233,     0,    41,    42,    43,     0,    44,    45,
      46,   -43,    47,    48,     0,    49,    50,    51,     0,     0,
       0,    52,    53,    54,    55,     0,    57,    58,  -233,   -43,
      59,    60,    61,     0,    62,    63,    64,     0,    65,    66,
      67,    68,    69,    70,    71,    72,     0,    73,    74,     0,
      75,    76,    77,    78,    79,     0,    80,    81,   -61,    82,
      83,     0,     0,    84,     0,    85,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,    97,
       0,     0,    98,     0,    99,     0,   100,     4,   101,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,   596,    37,
       0,    38,    39,     0,    40,  -233,     0,    41,    42,    43,
       0,    44,    45,    46,   -43,    47,    48,     0,    49,    50,
      51,     0,     0,     0,    52,    53,    54,    55,     0,    57,
      58,  -233,   -43,    59,    60,    61,     0,    62,    63,    64,
       0,    65,    66,    67,    68,    69,    70,    71,    72,     0,
      73,    74,     0,    75,    76,    77,    78,    79,     0,    80,
      81,   -61,    82,    83,     0,     0,    84,     0,    85,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,    97,     0,     0,    98,     0,    99,     0,   100,
       4,   101,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,   869,    37,     0,    38,    39,     0,    40,  -233,     0,
      41,    42,    43,     0,    44,    45,    46,   -43,    47,    48,
       0,    49,    50,    51,     0,     0,     0,    52,    53,    54,
      55,     0,    57,    58,  -233,   -43,    59,    60,    61,     0,
      62,    63,    64,     0,    65,    66,    67,    68,    69,    70,
      71,    72,     0,    73,    74,     0,    75,    76,    77,    78,
      79,     0,    80,    81,   -61,    82,    83,     0,     0,    84,
       0,    85,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,    97,     0,     0,    98,     0,
      99,     0,   100,     4,   101,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,   871,    37,     0,    38,    39,     0,
      40,  -233,     0,    41,    42,    43,     0,    44,    45,    46,
     -43,    47,    48,     0,    49,    50,    51,     0,     0,     0,
      52,    53,    54,    55,     0,    57,    58,  -233,   -43,    59,
      60,    61,     0,    62,    63,    64,     0,    65,    66,    67,
      68,    69,    70,    71,    72,     0,    73,    74,     0,    75,
      76,    77,    78,    79,     0,    80,    81,   -61,    82,    83,
       0,     0,    84,     0,    85,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,    97,     0,
       0,    98,     0,    99,     0,   100,     4,   101,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,   876,    37,     0,
      38,    39,     0,    40,  -233,     0,    41,    42,    43,     0,
      44,    45,    46,   -43,    47,    48,     0,    49,    50,    51,
       0,     0,     0,    52,    53,    54,    55,     0,    57,    58,
    -233,   -43,    59,    60,    61,     0,    62,    63,    64,     0,
      65,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      74,     0,    75,    76,    77,    78,    79,     0,    80,    81,
     -61,    82,    83,     0,     0,    84,     0,    85,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,    97,     0,     0,    98,     0,    99,     0,   100,     4,
     101,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
     879,    37,     0,    38,    39,     0,    40,  -233,     0,    41,
      42,    43,     0,    44,    45,    46,   -43,    47,    48,     0,
      49,    50,    51,     0,     0,     0,    52,    53,    54,    55,
       0,    57,    58,  -233,   -43,    59,    60,    61,     0,    62,
      63,    64,     0,    65,    66,    67,    68,    69,    70,    71,
      72,     0,    73,    74,     0,    75,    76,    77,    78,    79,
       0,    80,    81,   -61,    82,    83,     0,     0,    84,     0,
      85,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,    97,     0,     0,    98,     0,    99,
       0,   100,     4,   101,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,   901,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,     0,    38,    39,     0,    40,
    -233,     0,    41,    42,    43,     0,    44,    45,    46,   -43,
      47,    48,     0,    49,    50,    51,     0,     0,     0,    52,
      53,    54,    55,     0,    57,    58,  -233,   -43,    59,    60,
      61,     0,    62,    63,    64,     0,    65,    66,    67,    68,
      69,    70,    71,    72,     0,    73,    74,     0,    75,    76,
      77,    78,    79,     0,    80,    81,   -61,    82,    83,     0,
       0,    84,     0,    85,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,    97,     0,     0,
      98,     0,    99,     0,   100,     4,   101,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,   907,    37,     0,    38,
      39,     0,    40,  -233,     0,    41,    42,    43,     0,    44,
      45,    46,   -43,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,     0,    57,    58,  -233,
     -43,    59,    60,    61,     0,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
       0,    75,    76,    77,    78,    79,     0,    80,    81,   -61,
      82,    83,     0,     0,    84,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,     0,   100,     4,   101,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,   911,
      37,     0,    38,    39,     0,    40,  -233,     0,    41,    42,
      43,     0,    44,    45,    46,   -43,    47,    48,     0,    49,
      50,    51,     0,     0,     0,    52,    53,    54,    55,     0,
      57,    58,  -233,   -43,    59,    60,    61,     0,    62,    63,
      64,     0,    65,    66,    67,    68,    69,    70,    71,    72,
       0,    73,    74,     0,    75,    76,    77,    78,    79,     0,
      80,    81,   -61,    82,    83,     0,     0,    84,     0,    85,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,    97,     0,     0,    98,     0,    99,     0,
     100,   764,   101,     5,     6,     7,     8,     9,    10,    11,
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
      93,     0,     0,    94,    95,   270,     0,     0,     0,    98,
       0,   177,     0,   100,     0,   178,     5,     6,     7,     8,
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
       0,     0,     0,    93,     0,     0,    94,    95,   270,     0,
       0,     0,    98,     0,   177,     0,   100,     0,   178,   664,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
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
     100,     0,   178,   669,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   169,     0,     0,    15,    16,     0,
      17,     0,   170,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   171,     0,     0,     0,    32,
     172,   173,     0,   174,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     175,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   176,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,   270,     0,     0,     0,
      98,     0,   177,     0,   100,     0,   178,  1012,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   169,     0,
       0,    15,    16,     0,    17,     0,   170,     0,     0,    21,
     214,    23,     0,     0,     0,     0,     0,    28,     0,   171,
       0,     0,     0,    32,   172,   173,     0,   174,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,    54,    55,     0,    57,     0,
       0,     0,     0,    60,   175,     0,    62,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   176,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,    82,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   177,     0,   100,     0,
     178,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   169,     0,     0,    15,    16,     0,    17,     0,   170,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   171,     0,     0,     0,    32,   172,   173,     0,
     174,    37,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   175,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     176,     0,    73,     0,     0,    75,   274,   275,     0,    79,
     311,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,   312,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,   270,     0,     0,     0,    98,     0,   177,
       0,   100,     0,   178,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   169,     0,     0,    15,    16,     0,
      17,     0,   170,     0,     0,    21,     0,   555,     0,     0,
       0,     0,     0,    28,     0,   171,     0,     0,     0,    32,
     172,   173,     0,   174,    37,   556,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     175,     0,   557,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   176,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,   558,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,   177,     0,   100,     0,   178,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   169,     0,     0,
      15,    16,     0,    17,     0,   170,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   171,     0,
       0,     0,    32,   172,   173,     0,   174,    37,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   175,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   176,     0,    73,     0,
       0,    75,   274,   275,     0,    79,   311,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,   270,
       0,     0,     0,    98,     0,   177,     0,   100,   656,   178,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     169,     0,     0,    15,    16,     0,    17,     0,   170,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   171,     0,     0,     0,    32,   172,   173,     0,   174,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   175,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   176,
       0,    73,     0,     0,    75,   274,   275,     0,    79,   311,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,   270,     0,     0,     0,    98,     0,   177,     0,
     100,   658,   178,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   171,     0,     0,     0,    32,   172,
     173,     0,   174,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   175,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   176,     0,    73,     0,     0,    75,   274,   275,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,   276,     0,     0,    98,
       0,   177,     0,   100,     0,   178,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   169,     0,     0,    15,
      16,     0,    17,     0,   170,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   171,     0,     0,
       0,    32,   172,   173,     0,   174,    37,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,   766,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   175,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   176,     0,    73,     0,     0,
      75,   274,   275,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   177,     0,   100,     0,   178,     5,
       6,     7,     8,   203,    10,    11,   204,     0,     0,   169,
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
       0,   178,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   169,     0,     0,    15,    16,     0,    17,     0,
     170,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   171,     0,     0,     0,    32,   172,   173,
       0,   174,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,   208,
      55,     0,    57,     0,     0,     0,     0,    60,   175,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   176,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,   209,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     177,     0,   100,     0,   178,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   169,     0,     0,    15,    16,
       0,    17,     0,   170,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   171,     0,     0,     0,
      32,   172,   173,     0,   174,    37,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   175,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   176,     0,    73,     0,     0,    75,
     274,   275,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,   270,     0,     0,
       0,    98,     0,   177,     0,   100,     0,   178,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   169,     0,
       0,    15,    16,     0,    17,     0,   170,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   171,
       0,     0,     0,    32,   172,   173,     0,   174,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   175,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   176,     0,    73,
       0,     0,    75,   274,   275,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,   798,     0,   799,    98,     0,   177,     0,   100,     0,
     178,   800,     0,     0,     0,     0,   801,   221,   802,   803,
       0,     0,     0,     0,   804,     0,   264,     0,     0,     0,
      22,    23,   224,     0,     0,     0,     0,     0,   805,   265,
       0,    30,   266,   806,     0,   227,    35,     0,   807,     0,
     808,    39,     0,     0,  -233,     0,   809,     0,     0,     0,
       0,     0,   810,   811,     0,     0,     0,     0,     0,   232,
       0,     0,     0,    52,     0,    54,     0,     0,   812,     0,
    -233,     0,     0,     0,   267,     0,    62,   234,   235,     0,
     813,     0,   237,     0,   814,     0,     0,   815,     0,     0,
       0,   816,     0,     0,   241,    78,     0,   817,    80,     0,
     -63,    82,     0,     0,     0,     0,   366,   367,   368,   369,
     370,   371,     0,     0,   374,   375,   376,   377,     0,   379,
     380,   818,   819,   820,   821,   822,     0,     0,   823,     0,
       0,     0,   824,   825,   826,   827,   828,   829,   830,   831,
     832,   833,   834,     0,   835,     0,     0,   836,   837,   838,
     839,     0,     0,   840,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   169,     0,     0,    15,    16,     0,
      17,     0,   170,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   171,     0,     0,     0,    32,
     172,   173,     0,   174,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     175,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   176,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,   209,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,   177,     0,   100,     0,   178,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   169,     0,     0,
      15,    16,     0,    17,     0,   170,     0,     0,    21,     0,
       0,     0,     0,     0,     0,   250,    28,     0,   171,     0,
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
       0,     0,     0,    98,     0,    99,     0,   100,     0,   178,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
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
     209,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,   270,     0,     0,     0,    98,     0,   177,     0,
     100,     0,   178,     5,     6,     7,     8,   203,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   171,     0,     0,     0,    32,   172,
     173,     0,   174,    37,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   205,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   176,     0,    73,     0,     0,    75,     0,     0,
     206,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   177,     0,   100,     0,   178,     5,     6,     7,     8,
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
       0,     0,     0,    93,     0,     0,    94,    95,   270,     0,
       0,     0,    98,     0,   177,   654,   100,     0,   178,     5,
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
      95,    96,     0,     0,     0,    98,     0,   177,     0,   100,
       0,   178,     5,     6,     7,     8,   199,    10,    11,     0,
       0,     0,   169,     0,     0,    15,    16,     0,    17,     0,
     170,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   171,     0,     0,     0,    32,   172,   173,
       0,   174,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   200,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   176,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     177,     0,   100,     0,   178,     5,     6,     7,     8,     9,
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
       0,     0,    93,     0,     0,    94,    95,   270,     0,     0,
       0,    98,     0,   177,     0,   100,     0,   178,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   169,     0,
       0,    15,    16,     0,    17,     0,   170,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   171,
       0,     0,     0,    32,   172,   173,     0,   174,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   175,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   176,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,    99,     0,   100,     0,
     178,     5,     6,     7,     8,     9,    10,    11,     0,     0,
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
       0,    94,    95,    96,     0,     0,     0,   601,     0,   177,
       0,   100,     0,   178,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   169,     0,     0,    15,    16,     0,
      17,     0,   170,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   628,     0,   171,     0,     0,     0,    32,
     172,   173,     0,   174,    37,     0,   629,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     175,     0,     0,     0,     0,     0,     0,     0,    67,   630,
      69,    70,    71,   631,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,   177,     0,   100,     0,   632,     5,     6,     7,
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
       0,     0,     0,    98,     0,   177,     0,   100,     0,   632,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     169,     0,     0,    15,    16,     0,    17,     0,   170,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   171,     0,     0,     0,    32,   172,   173,     0,   704,
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
     100,     0,   178,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   171,     0,     0,     0,    32,   172,
     173,     0,   707,    37,     0,    38,     0,     0,    40,     0,
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
       0,   177,     0,   100,     0,   178,     5,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   169,     0,     0,    15,
      16,     0,    17,     0,   170,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   171,     0,     0,
       0,    32,   172,   173,     0,   937,    37,     0,    38,     0,
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
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   177,     0,   100,     0,   178,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   169,
       0,     0,    15,    16,     0,    17,     0,   170,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     171,     0,     0,     0,    32,   172,   173,     0,   938,    37,
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
       0,   178,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   169,     0,     0,    15,    16,     0,    17,     0,
     170,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   171,     0,     0,     0,    32,   172,   173,
       0,   940,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   175,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   176,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     177,     0,   100,     0,   178,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   169,     0,     0,    15,    16,
       0,    17,     0,   170,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   171,     0,     0,     0,
      32,   172,   173,     0,   941,    37,     0,    38,     0,     0,
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
       0,    98,     0,   177,     0,   100,     0,   178,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   169,     0,
       0,    15,    16,     0,    17,     0,   170,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   171,
       0,     0,     0,    32,   172,   173,     0,   942,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   175,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   176,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   177,     0,   100,     0,
     178,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   169,     0,     0,    15,    16,     0,    17,     0,   170,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   171,     0,     0,     0,    32,   172,   173,     0,
     943,    37,     0,    38,     0,     0,    40,     0,     0,     0,
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
       0,   100,     0,   178,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   169,     0,     0,    15,    16,     0,
      17,     0,   170,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   628,     0,   171,     0,     0,     0,    32,
     172,   173,     0,   174,    37,     0,   629,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     175,     0,     0,     0,     0,     0,     0,     0,    67,   630,
      69,    70,    71,   631,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,   270,     0,     0,     0,
      98,     0,   177,     0,   100,     0,   960,     5,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   169,     0,     0,
      15,    16,     0,    17,     0,   170,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,   628,     0,   171,     0,
       0,     0,    32,   172,   173,     0,   174,    37,     0,   629,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   175,     0,     0,     0,     0,     0,     0,
       0,    67,   630,    69,    70,    71,   631,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,   177,     0,   100,     0,  1032,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
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
      94,    95,   270,     0,     0,     0,    98,     0,   177,     0,
     100,     0,   960,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   169,     0,     0,    15,    16,     0,    17,
       0,   170,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   171,     0,     0,     0,    32,   172,
     173,     0,   943,    37,     0,    38,     0,     0,    40,     0,
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
      93,     0,     0,    94,    95,   270,     0,     0,     0,    98,
       0,   177,     0,   100,     0,   178,     5,     6,     7,     8,
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
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,     0,     0,   100,     0,  1032,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   169,
       0,     0,    15,    16,     0,    17,     0,   170,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     171,     0,     0,     0,    32,   172,   173,     0,   174,    37,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,   264,    49,    50,
       0,    22,    23,     0,     0,    53,     0,    55,     0,    57,
     265,     0,    30,   266,    60,   175,     0,    35,     0,     0,
       0,     0,    39,    67,    68,    69,    70,    71,   176,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,    52,     0,    54,     0,     0,     0,
       0,     0,     0,     0,     0,   267,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    78,     0,     0,    80,
       0,     0,    82,     0,     0,     0,    93,     0,     0,     0,
      95,    96,     0,     0,     0,    98,     0,   177,     0,   100,
       0,   178,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   169,     0,     0,    15,    16,     0,    17,     0,
     170,   339,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   171,     0,     0,     0,    32,   172,   173,
       0,   174,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,  -379,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,   340,    57,     0,     0,   341,     0,    60,   175,     0,
       0,     0,  -379,     0,     0,     0,    67,    68,    69,    70,
      71,   176,     0,    73,     0,  -379,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,  -379,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,   344,     0,   345,   346,   347,     0,   348,
     349,   350,    89,   351,   352,   353,   354,     0,     0,   356,
     357,   358,   359,   360,     0,   362,     0,     0,   363,   364,
     365,     0,     0,     0,    96,  -379,     0,     0,     0,     0,
    -379,     0,   100,     0,  1032,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   169,     0,     0,    15,    16,
     339,    17,     0,   170,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   171,     0,     0,     0,
      32,   172,   173,     0,   174,    37,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
     340,    53,     0,    55,   341,    57,     0,     0,     0,     0,
      60,   175,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   176,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,     0,  -397,     0,
       0,   343,   344,     0,   345,   346,   347,     0,   348,   349,
     350,     0,     0,   352,     0,    89,     0,     0,     0,  -397,
     358,   359,     0,  -397,   362,     0,     0,   363,   364,   365,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
       0,     0,     0,   177,     0,   100,  -397,   960,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   169,     0,
       0,    15,    16,     0,    17,     0,   170,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   171,
       0,     0,     0,    32,   172,   173,     0,   174,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   175,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   176,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,     0,     0,     0,    87,     0,    89,     0,
       5,     6,     7,     8,     9,    10,    11,     0,     0,  -501,
     169,     0,     0,    15,    16,     0,    17,     0,   170,     0,
      96,    21,   381,     0,     0,  -539,   177,  -539,   100,    28,
     178,   171,     0,     0,     0,    32,   172,   173,     0,   174,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   175,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   176,
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,     0,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     5,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   169,     0,     0,    15,    16,     0,    17,     0,
     170,   381,    96,    21,  -539,     0,  -539,     0,   177,     0,
     100,    28,   632,   171,     0,     0,     0,    32,   172,   173,
       0,   174,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   175,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   176,     0,    73,   990,     0,    75,     0,     0,     0,
      79,     0,   800,    81,     0,     0,    83,   220,   221,   802,
     222,     0,     0,     0,     0,   223,     0,     0,     0,     0,
       0,     0,     0,   224,     0,     0,     0,     0,     0,   805,
       0,     0,    89,     0,   226,     0,   227,     0,     0,   228,
       0,   808,     0,     0,     0,     0,     0,   229,     0,     0,
       0,     0,     0,   810,   231,     0,     0,     0,   337,     0,
     232,     0,   100,     0,   178,   338,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   339,   234,   235,
       0,   236,     0,   237,     0,   991,     0,     0,   815,     0,
       0,     0,   240,     0,     0,   241,     0,     0,   242,     0,
       0,     0,     0,     0,     0,     0,     0,   366,   367,   368,
     369,   370,   371,     0,     0,   374,   375,   376,   377,   339,
     379,   380,   818,   819,   820,   821,   822,   340,     0,   823,
       0,   341,     0,   824,   825,   826,   827,   828,   829,   830,
     831,   832,   833,   834,     0,   835,   337,     0,   836,   837,
     838,   839,     0,   338,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,   339,     0,     0,     0,   340,
       0,     0,     0,   341,     0,     0,     0,     0,   343,   344,
       0,   345,   346,   347,     0,   348,   349,   350,     0,   351,
     352,   353,   354,   355,     0,   356,   357,   358,   359,   360,
     361,   362,     0,   337,   363,   364,   365,     0,     0,     0,
     338,     0,     0,  1099,     0,   340,     0,     0,     0,   341,
     343,   344,   339,   345,   346,   347,     0,   348,   349,     0,
       0,     0,   352,     0,     0,     0,     0,     0,     0,   358,
     359,     0,     0,   362,   337,     0,   363,   364,   365,     0,
     342,   338,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   339,     0,     0,   343,   344,     0,   345,
     346,   347,   340,   348,   349,   350,   341,   351,   352,   353,
     354,   355,     0,   356,   357,   358,   359,   360,   361,   362,
     337,     0,   363,   364,   365,     0,     0,   338,     0,   655,
       0,     0,     0,     0,     0,     0,     0,   342,     0,   339,
       0,     0,     0,   340,     0,     0,     0,   341,     0,     0,
       0,     0,     0,   343,   344,     0,   345,   346,   347,     0,
     348,   349,   350,     0,   351,   352,   353,   354,   355,     0,
     356,   357,   358,   359,   360,   361,   362,     0,   342,   363,
     364,   365,     0,     0,     0,     0,   768,     0,     0,   340,
       0,     0,     0,   341,   343,   344,     0,   345,   346,   347,
       0,   348,   349,   350,     0,   351,   352,   353,   354,   355,
       0,   356,   357,   358,   359,   360,   361,   362,   337,     0,
     363,   364,   365,     0,   342,   338,     0,   769,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   339,     0,     0,
     343,   344,     0,   345,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,   354,   355,     0,   356,   357,   358,
     359,   360,   361,   362,     0,     0,   363,   364,   365,     0,
       0,     0,     0,   989,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   340,     0,     0,
       0,   341,     0,     0,   337,     0,     0,     0,     0,     0,
     435,   338,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   339,     0,     0,     0,     0,     0,     0,
       0,     0,   342,   366,   367,   368,   369,   370,   371,     0,
       0,   374,   375,   376,   377,     0,   379,   380,   343,   344,
       0,   345,   346,   347,     0,   348,   349,   350,     0,   351,
     352,   353,   354,   355,     0,   356,   357,   358,   359,   360,
     361,   362,     0,   340,   363,   364,   365,   341,   394,     0,
     337,     0,     0,     0,     0,     0,   868,   338,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   339,
       0,     0,     0,     0,     0,     0,     0,     0,   342,   366,
     367,   368,   369,   370,   371,     0,     0,   374,   375,   376,
     377,     0,   379,   380,   343,   344,     0,   345,   346,   347,
       0,   348,   349,   350,     0,   351,   352,   353,   354,   355,
       0,   356,   357,   358,   359,   360,   361,   362,     0,   340,
     363,   364,   365,   341,   394,     0,   337,     0,     0,     0,
       0,     0,   875,   338,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   339,     0,     0,     0,     0,
       0,     0,     0,     0,   342,   366,   367,   368,   369,   370,
     371,     0,     0,   374,   375,   376,   377,     0,   379,   380,
     343,   344,     0,   345,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,   354,   355,     0,   356,   357,   358,
     359,   360,   361,   362,     0,   340,   363,   364,   365,   341,
     394,     0,   337,     0,     0,     0,     0,     0,  1004,   338,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   339,     0,     0,     0,     0,     0,     0,     0,     0,
     342,   366,   367,   368,   369,   370,   371,     0,     0,   374,
     375,   376,   377,     0,   379,   380,   343,   344,     0,   345,
     346,   347,     0,   348,   349,   350,     0,   351,   352,   353,
     354,   355,     0,   356,   357,   358,   359,   360,   361,   362,
       0,   340,   363,   364,   365,   341,   394,     0,   337,     0,
       0,     0,     0,     0,  1005,   338,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   339,     0,     0,
       0,     0,     0,     0,     0,     0,   342,   366,   367,   368,
     369,   370,   371,     0,     0,   374,   375,   376,   377,     0,
     379,   380,   343,   344,     0,   345,   346,   347,     0,   348,
     349,   350,     0,   351,   352,   353,   354,   355,     0,   356,
     357,   358,   359,   360,   361,   362,     0,   340,   363,   364,
     365,   341,   394,     0,   337,     0,     0,     0,     0,     0,
    1006,   338,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   339,     0,     0,     0,     0,     0,     0,
       0,     0,   342,   366,   367,   368,   369,   370,   371,     0,
       0,   374,   375,   376,   377,     0,   379,   380,   343,   344,
       0,   345,   346,   347,     0,   348,   349,   350,     0,   351,
     352,   353,   354,   355,     0,   356,   357,   358,   359,   360,
     361,   362,     0,   340,   363,   364,   365,   341,   394,     0,
     337,     0,     0,     0,     0,     0,  1007,   338,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   339,
       0,     0,     0,     0,     0,     0,     0,     0,   342,   366,
     367,   368,   369,   370,   371,     0,     0,   374,   375,   376,
     377,     0,   379,   380,   343,   344,     0,   345,   346,   347,
       0,   348,   349,   350,     0,   351,   352,   353,   354,   355,
       0,   356,   357,   358,   359,   360,   361,   362,     0,   340,
     363,   364,   365,   341,   394,     0,   337,     0,     0,     0,
       0,     0,  1030,   338,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   339,     0,     0,     0,     0,
       0,     0,     0,     0,   342,   366,   367,   368,   369,   370,
     371,     0,     0,   374,   375,   376,   377,     0,   379,   380,
     343,   344,     0,   345,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,   354,   355,     0,   356,   357,   358,
     359,   360,   361,   362,     0,   340,   363,   364,   365,   341,
     394,     0,     0,     0,     0,     0,     0,     0,  1031,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   339,     0,     0,     0,     0,
     342,   366,   367,   368,   369,   370,   371,     0,     0,   374,
     375,   376,   377,     0,   379,   380,   343,   344,     0,   345,
     346,   347,     0,   348,   349,   350,     0,   351,   352,   353,
     354,   355,     0,   356,   357,   358,   359,   360,   361,   362,
     337,     0,   363,   364,   365,   340,   394,   338,     0,   341,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   339,
     428,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   337,   429,     0,     0,     0,     0,     0,   338,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   339,   422,     0,     0,     0,   343,   344,     0,   345,
     346,   347,     0,   348,   349,   423,     0,     0,   352,   340,
       0,     0,     0,   341,     0,     0,   359,     0,     0,     0,
       0,     0,   363,   364,   365,     0,     0,     0,     0,   337,
       0,     0,     0,     0,     0,     0,   338,     0,     0,     0,
     425,   340,     0,     0,   342,   341,     0,     0,   339,   737,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     343,   344,     0,   345,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,   354,   355,   342,   356,   357,   358,
     359,   360,   361,   362,     0,     0,   363,   364,   365,     0,
     394,     0,   343,   344,     0,   345,   346,   347,   340,   348,
     349,   350,   341,   351,   352,   353,   354,   355,     0,   356,
     357,   358,   359,   360,   361,   362,   337,     0,   363,   364,
     365,     0,   394,   338,     0,     0,     0,     0,     0,   425,
       0,     0,     0,   342,     0,   339,   250,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
     344,     0,   345,   346,   347,     0,   348,   349,   350,     0,
     351,   352,   353,   354,   355,     0,   356,   357,   358,   359,
     360,   361,   362,   337,     0,   363,   364,   365,     0,   394,
     338,     0,     0,     0,     0,   340,     0,     0,     0,   341,
       0,     0,   339,   729,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   337,     0,     0,     0,     0,
       0,     0,   338,     0,     0,     0,     0,     0,     0,     0,
     342,     0,     0,     0,   339,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,   344,     0,   345,
     346,   347,   340,   348,   349,   350,   341,   351,   352,   353,
     354,   355,     0,   356,   357,   358,   359,   360,   361,   362,
       0,     0,   363,   364,   365,     0,   394,     0,     0,     0,
       0,     0,     0,     0,   340,     0,     0,   342,   341,     0,
       0,     0,     0,     0,     0,     0,     0,   747,     0,     0,
       0,     0,     0,   343,   344,     0,   345,   346,   347,     0,
     348,   349,   350,     0,   351,   352,   353,   354,   355,   342,
     356,   357,   358,   359,   360,   361,   362,     0,     0,   363,
     364,   365,     0,   394,     0,   343,   344,     0,   345,   346,
     347,     0,   348,   349,   350,     0,   351,   352,   353,   354,
     355,     0,   356,   357,   358,   359,   360,   361,   362,   337,
       0,   363,   364,   365,     0,   394,   338,  -214,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   339,     0,
    -214,  -214,     0,  -214,     0,     0,     0,     0,  -214,     0,
       0,   337,     0,     0,     0,     0,  -214,     0,   338,     0,
       0,     0,  -214,     0,     0,     0,     0,  -214,     0,  -214,
     339,     0,  -214,     0,     0,   432,     0,     0,     0,     0,
    -214,     0,     0,     0,     0,     0,  -214,  -214,   340,     0,
       0,     0,   341,  -214,     0,     0,     0,     0,     0,     0,
       0,     0,  -214,     0,     0,   433,     0,     0,   337,     0,
       0,  -214,  -214,     0,  -214,   338,  -214,     0,  -214,     0,
     340,  -214,     0,   342,   341,  -214,     0,   339,  -214,     0,
       0,  -214,     0,     0,     0,     0,     0,     0,     0,   343,
     344,   396,   345,   346,   347,     0,   348,   349,   350,     0,
     351,   352,   353,   354,   355,   342,   356,   357,   358,   359,
     360,   361,   362,     0,     0,   363,   364,   365,     0,   464,
       0,   343,   344,     0,   345,   346,   347,   340,   348,   349,
     350,   341,   351,   352,   353,   354,   355,     0,   356,   357,
     358,   359,   360,   361,   362,   337,   434,   363,   364,   365,
       0,     0,   338,     0,     0,     0,     0,     0,   164,     0,
       0,     0,   342,     0,   339,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   343,   344,
       0,   345,   346,   347,     0,   348,   349,   350,     0,   351,
     352,   353,   354,   355,     0,   356,   357,   358,   359,   360,
     361,   362,   337,     0,   363,   364,   365,     0,     0,   338,
       0,     0,     0,     0,   340,     0,     0,     0,   341,     0,
       0,   339,   564,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   337,   565,     0,     0,     0,     0,
       0,   338,     0,     0,     0,     0,     0,     0,     0,   342,
       0,     0,     0,   339,   566,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   343,   344,   567,   345,   346,
     347,   340,   348,   349,   350,   341,   351,   352,   353,   354,
     355,     0,   356,   357,   358,   359,   360,   361,   362,     0,
     468,   363,   364,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   340,     0,     0,   342,   341,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,   344,     0,   345,   346,   347,     0,   348,
     349,   350,     0,   351,   352,   353,   354,   355,   342,   356,
     357,   358,   359,   360,   361,   362,     0,     0,   363,   364,
     365,     0,     0,     0,   343,   344,     0,   345,   346,   347,
       0,   348,   349,   350,     0,   351,   352,   353,   354,   355,
       0,   356,   357,   358,   359,   360,   361,   362,   337,   264,
     363,   364,   365,    22,    23,   338,     0,     0,     0,     0,
       0,     0,   265,     0,    30,   266,     0,   339,     0,    35,
       0,     0,   675,     0,    39,     0,     0,  -233,     0,     0,
     337,     0,     0,     0,     0,     0,     0,   338,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,    54,   339,
       0,     0,   676,  -233,     0,     0,     0,   267,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   340,     0,     0,
       0,   341,     0,     0,     0,     0,     0,     0,    78,   337,
       0,    80,     0,   -62,    82,     0,   338,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   339,   340,
       0,     0,   342,   341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   343,   344,
       0,   345,   346,   347,     0,   348,   349,   350,     0,   351,
     352,   353,   354,   355,   342,   356,   357,   358,   359,   360,
     361,   362,     0,     0,   363,   364,   365,     0,   340,     0,
     343,   344,   341,   345,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,   354,   355,     0,   356,   357,   358,
     359,   360,   361,   362,   337,   470,   363,   364,   365,     0,
       0,   338,     0,   342,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   339,     0,     0,     0,     0,     0,   343,
     344,     0,   345,   346,   347,     0,   348,   349,   350,     0,
     351,   352,   353,   354,   355,     0,   356,   357,   358,   359,
     360,   361,   362,   337,   472,   363,   364,   365,     0,     0,
     338,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   339,   340,     0,     0,     0,   341,     0,     0,
       0,     0,     0,     0,     0,   337,     0,     0,     0,     0,
       0,     0,   338,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   339,     0,     0,     0,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   340,     0,   343,   344,   341,   345,   346,   347,
       0,   348,   349,   350,     0,   351,   352,   353,   354,   355,
       0,   356,   357,   358,   359,   360,   361,   362,     0,   477,
     363,   364,   365,     0,   340,     0,   494,   342,   341,     0,
       0,     0,     0,     0,     0,     0,     0,   568,     0,     0,
       0,     0,     0,   343,   344,     0,   345,   346,   347,     0,
     348,   349,   350,     0,   351,   352,   353,   354,   355,   342,
     356,   357,   358,   359,   360,   361,   362,     0,     0,   363,
     364,   365,     0,     0,     0,   343,   344,     0,   345,   346,
     347,     0,   348,   349,   350,     0,   351,   352,   353,   354,
     355,     0,   356,   357,   358,   359,   360,   361,   362,   337,
       0,   363,   364,   365,     0,     0,   338,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   339,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   337,     0,     0,     0,     0,     0,
       0,   338,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   339,     0,     0,     0,     0,   340,     0,
       0,     0,   341,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   337,     0,     0,     0,     0,
       0,     0,   338,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   342,   339,     0,     0,     0,     0,     0,
       0,     0,     0,   340,     0,     0,     0,   341,     0,   343,
     344,     0,   345,   346,   347,     0,   348,   349,   350,     0,
     351,   352,   353,   354,   355,  -502,   356,   357,   358,   359,
     360,   361,   362,     0,   164,   363,   364,   365,   342,     0,
       0,     0,     0,     0,   340,     0,     0,     0,   341,     0,
       0,     0,     0,     0,   343,   344,     0,   345,   346,   347,
       0,   348,   349,   350,     0,   351,   352,   353,   354,   355,
       0,   356,   357,   358,   359,   360,   361,   362,     0,   342,
     363,   364,   365,     0,     0,     0,   337,   742,     0,     0,
       0,     0,     0,   338,     0,   343,   344,     0,   345,   346,
     347,     0,   348,   349,   350,   339,   351,   352,   353,   354,
     355,     0,   356,   357,   358,   359,   360,   361,   362,     0,
     714,   363,   364,   365,     0,     0,   337,     0,     0,     0,
       0,     0,     0,   338,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   339,     0,     0,   745,     0,
       0,     0,     0,     0,     0,   340,     0,     0,     0,   341,
       0,     0,     0,     0,     0,     0,     0,   337,     0,     0,
       0,     0,     0,     0,   338,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   339,     0,     0,     0,
     342,     0,     0,     0,     0,   340,     0,     0,     0,   341,
       0,     0,     0,     0,     0,     0,   343,   344,     0,   345,
     346,   347,     0,   348,   349,   350,     0,   351,   352,   353,
     354,   355,     0,   356,   357,   358,   359,   360,   361,   362,
     342,     0,   363,   364,   365,     0,   340,     0,     0,     0,
     341,     0,     0,     0,     0,     0,   343,   344,     0,   345,
     346,   347,     0,   348,   349,   350,     0,   351,   352,   353,
     354,   355,     0,   356,   357,   358,   359,   360,   361,   362,
     787,   342,   363,   364,   365,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   343,   344,     0,
     345,   346,   347,     0,   348,   349,   350,     0,   351,   352,
     353,   354,   355,     0,   356,   357,   358,   359,   360,   361,
     362,   337,     0,   363,   364,   365,     0,   679,   338,   219,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     339,   855,   220,   221,     0,   222,     0,     0,     0,     0,
     223,     0,     0,   337,     0,     0,     0,     0,   224,     0,
     338,     0,     0,     0,   225,     0,     0,     0,     0,   226,
       0,   227,   339,   858,   228,     0,     0,     0,     0,     0,
       0,     0,   229,     0,     0,     0,     0,     0,   230,   231,
     340,     0,     0,     0,   341,   232,     0,     0,     0,     0,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,     0,   234,   235,     0,   236,     0,   237,     0,
     238,     0,   340,   239,     0,   342,   341,   240,     0,     0,
     241,     0,     0,   242,     0,     0,     0,     0,     0,     0,
       0,   343,   344,     0,   345,   346,   347,     0,   348,   349,
     350,     0,   351,   352,   353,   354,   355,   342,   356,   357,
     358,   359,   360,   361,   362,     0,     0,   363,   364,   365,
       0,     0,     0,   343,   344,     0,   345,   346,   347,     0,
     348,   349,   350,     0,   351,   352,   353,   354,   355,     0,
     356,   357,   358,   359,   360,   361,   362,   337,     0,   363,
     364,   365,     0,     0,   338,  -224,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   339,     0,  -224,  -224,
       0,  -224,     0,     0,     0,     0,  -224,     0,     0,   337,
       0,     0,     0,     0,  -224,     0,   338,     0,     0,     0,
    -224,     0,     0,     0,     0,  -224,     0,  -224,   339,     0,
    -224,     0,     0,     0,     0,     0,     0,     0,  -224,     0,
       0,     0,     0,     0,  -224,  -224,   340,     0,     0,     0,
     341,  -224,     0,     0,     0,     0,     0,     0,     0,   936,
    -224,     0,     0,     0,     0,     0,   337,   955,     0,  -224,
    -224,     0,  -224,   338,  -224,     0,  -224,     0,   340,  -224,
       0,   342,   341,  -224,     0,   339,  -224,     0,     0,  -224,
       0,   939,     0,     0,     0,     0,     0,   343,   344,     0,
     345,   346,   347,     0,   348,   349,   350,     0,   351,   352,
     353,   354,   355,   342,   356,   357,   358,   359,   360,   361,
     362,     0,     0,   363,   364,   365,     0,     0,     0,   343,
     344,     0,   345,   346,   347,   340,   348,   349,   350,   341,
     351,   352,   353,   354,   355,     0,   356,   357,   358,   359,
     360,   361,   362,   337,     0,   363,   364,   365,     0,     0,
     338,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     342,     0,   339,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,   344,     0,   345,
     346,   347,     0,   348,   349,   350,     0,   351,   352,   353,
     354,   355,     0,   356,   357,   358,   359,   360,   361,   362,
       0,     0,   363,   364,   365,     0,     0,     0,     0,     0,
       0,     0,   340,     0,     0,     0,   341,     0,     0,   337,
       0,     0,     0,     0,     0,  1041,   338,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   339,     0,
       0,     0,     0,     0,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   343,   344,     0,   345,   346,   347,     0,
     348,   349,   350,     0,   351,   352,   353,   354,   355,     0,
     356,   357,   358,   359,   360,   361,   362,     0,   340,   363,
     364,   365,   341,     0,     0,   337,     0,     0,     0,     0,
       0,  1042,   338,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   339,     0,     0,     0,     0,     0,
       0,     0,     0,   342,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
     344,     0,   345,   346,   347,     0,   348,   349,   350,     0,
     351,   352,   353,   354,   355,     0,   356,   357,   358,   359,
     360,   361,   362,     0,   340,   363,   364,   365,   341,     0,
       0,   337,     0,     0,     0,     0,     0,  1043,   338,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     339,     0,     0,     0,     0,     0,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   343,   344,     0,   345,   346,
     347,     0,   348,   349,   350,     0,   351,   352,   353,   354,
     355,     0,   356,   357,   358,   359,   360,   361,   362,     0,
     340,   363,   364,   365,   341,     0,     0,   337,     0,     0,
       0,     0,     0,  1044,   338,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   339,     0,     0,     0,
       0,     0,     0,     0,     0,   342,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,   344,     0,   345,   346,   347,     0,   348,   349,
     350,     0,   351,   352,   353,   354,   355,     0,   356,   357,
     358,   359,   360,   361,   362,     0,   340,   363,   364,   365,
     341,     0,     0,   337,     0,     0,     0,     0,     0,  1045,
     338,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   339,     0,     0,     0,     0,     0,     0,     0,
       0,   342,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   343,   344,     0,
     345,   346,   347,     0,   348,   349,   350,     0,   351,   352,
     353,   354,   355,     0,   356,   357,   358,   359,   360,   361,
     362,     0,   340,   363,   364,   365,   341,     0,     0,   337,
       0,     0,     0,     0,     0,  1046,   338,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   339,     0,
       0,     0,     0,     0,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1070,   343,   344,     0,   345,   346,   347,     0,
     348,   349,   350,     0,   351,   352,   353,   354,   355,     0,
     356,   357,   358,   359,   360,   361,   362,     0,   340,   363,
     364,   365,   341,     0,     0,   337,     0,     0,     0,     0,
       0,     0,   338,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   339,     0,     0,     0,     0,     0,
       0,     0,     0,   342,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
     344,     0,   345,   346,   347,     0,   348,   349,   350,     0,
     351,   352,   353,   354,   355,     0,   356,   357,   358,   359,
     360,   361,   362,     0,   340,   363,   364,   365,   341,     0,
       0,   337,     0,     0,     0,     0,     0,     0,   338,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     339,     0,     0,     0,     0,     0,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   343,   344,     0,   345,   346,
     347,     0,   348,   349,   350,     0,   351,   352,   353,   354,
     355,   339,   356,   357,   358,   359,   360,   361,   362,     0,
     702,   363,   364,   365,   341,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   342,     0,     0,     0,     0,
       0,   340,     0,     0,     0,   341,     0,     0,     0,     0,
       0,   343,   344,     0,   345,   346,   347,     0,   348,   349,
     350,     0,   351,   352,   353,   354,   355,     0,   356,   357,
     358,   359,   360,   361,   362,     0,   342,   363,   364,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,   344,     0,   345,   346,   347,     0,   348,
     349,   350,     0,   351,   352,   353,   354,     0,   219,   356,
     357,   358,   359,   360,     0,   362,     0,     0,   363,   364,
     365,   220,   221,     0,   222,     0,     0,     0,     0,   223,
       0,     0,     0,     0,     0,     0,     0,   224,     0,     0,
       0,     0,     0,   225,     0,     0,     0,     0,   226,     0,
     227,     0,     0,   228,     0,     0,     0,     0,     0,     0,
       0,   229,     0,     0,     0,  -215,     0,   230,   231,     0,
       0,     0,     0,     0,   232,     0,     0,     0,  -215,  -215,
       0,  -215,     0,   233,     0,     0,  -215,     0,     0,     0,
       0,     0,   234,   235,  -215,   236,     0,   237,     0,   238,
    -215,     0,   239,     0,     0,  -215,   240,  -215,     0,   241,
    -215,     0,   242,     0,     0,     0,     0,     0,  -215,     0,
       0,     0,  -146,     0,  -215,  -215,     0,     0,     0,     0,
       0,  -215,     0,     0,     0,  -146,  -146,     0,  -146,     0,
    -215,     0,     0,  -146,     0,     0,     0,     0,     0,  -215,
    -215,  -146,  -215,     0,  -215,     0,  -215,  -146,     0,  -215,
       0,     0,  -146,  -215,  -146,     0,  -215,  -146,     0,  -215,
       0,     0,     0,     0,     0,  -146,     0,     0,     0,  -142,
       0,  -146,  -146,     0,     0,     0,     0,     0,  -146,     0,
       0,     0,  -142,  -142,     0,  -142,     0,  -146,     0,     0,
    -142,     0,     0,     0,     0,     0,  -146,  -146,  -142,  -146,
       0,  -146,     0,  -146,  -142,     0,  -146,     0,     0,  -142,
    -146,  -142,     0,  -146,  -142,     0,  -146,     0,     0,     0,
       0,     0,  -142,     0,     0,     0,     0,     0,  -142,  -142,
       0,     0,     0,     0,     0,  -142,     0,     0,     0,     0,
       0,     0,     0,     0,  -142,     0,     0,     0,     0,     0,
       0,     0,     0,  -142,  -142,     0,  -142,     0,  -142,     0,
    -142,     0,     0,  -142,     0,     0,     0,  -142,     0,     0,
    -142,     0,     0,  -142
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-993)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      12,     2,   149,   135,    16,   473,   688,   387,    20,   213,
     448,    58,    59,   190,    26,   594,    22,   198,    30,    31,
      63,    33,    34,    35,    36,   433,   443,    19,    44,   206,
     763,   970,    44,    31,    46,   473,   791,   218,    83,    51,
      31,    53,    31,    55,     1,   582,    31,     1,   585,    83,
     627,    63,    64,    65,    66,    67,    68,    31,   101,   191,
      72,    41,    74,     1,    76,    77,    17,  1012,  1060,    81,
      49,    44,    84,    85,   218,    87,   111,    89,   888,   156,
      92,    93,   263,    95,    26,     1,   156,    99,   100,   101,
     440,   137,   162,   891,   845,   846,    75,   111,    78,    54,
      54,    44,    44,     0,   285,    47,    44,  1099,    81,    66,
     156,   156,   111,   317,   318,  1060,   161,   850,    69,   263,
     699,   137,     1,    30,   701,    67,   111,   161,    70,    83,
      72,   166,     3,   145,    44,   178,    87,   111,    66,    81,
    1079,   285,    44,    33,    34,   292,   137,     3,   128,   959,
     130,   108,   166,   104,   109,   109,    44,   169,   100,   892,
     172,   173,   174,   161,   176,   177,   178,   922,   349,     3,
     161,    66,   161,    80,    44,    54,   161,    84,   116,    44,
     108,   718,   136,   760,   721,   109,   198,   161,   986,   864,
     945,   866,   947,    81,   137,   137,   202,   189,   136,   137,
     116,   207,   383,   384,    83,   162,   218,   161,   214,   626,
     190,   101,   137,   137,   594,    26,   137,    96,   156,   163,
     136,   137,   164,   166,   131,   958,   206,   137,   208,   136,
     109,   138,   139,   137,   162,   137,   143,   162,    94,   383,
     384,   992,   993,   164,   151,  1000,   111,  1002,   164,    83,
     932,   263,   159,   137,   266,    54,   166,   136,   162,    70,
      94,   137,   274,   275,   166,   137,   246,   137,   676,   944,
      81,   946,   137,   285,   137,   137,    81,   156,  1011,   137,
     164,   413,   161,   326,    83,   466,   418,   116,   164,   100,
     270,  1077,   164,   202,   873,   106,   166,    96,   207,   131,
     312,   166,   164,   166,   137,   156,   164,   139,   309,  1095,
     109,   162,   141,    30,   326,   210,   211,   212,   213,   699,
      64,    26,   466,   335,    26,   337,   338,   339,   340,   341,
     342,   164,   344,   345,   346,   347,   348,   754,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   137,   137,   100,   156,   253,   137,
     372,   373,   161,    80,   259,    70,   378,    84,    70,   381,
     382,   383,   384,   385,   386,   387,    81,   420,   156,    81,
     131,   164,   164,   163,   396,   136,   281,   137,   139,   401,
      26,   403,   143,    80,   598,   599,    44,    84,   100,   580,
       7,   106,   752,   163,   299,    44,   156,    54,   420,    91,
     422,   423,   161,   137,   131,    54,   428,   429,    66,   136,
     432,   433,   139,   435,   163,   437,   143,   137,    76,    77,
     898,   411,   156,    76,    70,   415,    83,  1017,   450,    76,
      77,   791,   454,   455,    83,    81,   156,   459,   137,    96,
     340,   341,     3,   891,   466,   163,   137,   165,   662,   663,
     898,   504,   109,   443,   100,    54,   156,   156,    19,   481,
     109,   163,   484,   165,   486,   156,   653,   116,   490,   491,
     492,    32,   494,   495,  1064,  1065,  1066,  1067,  1068,  1069,
       2,   137,   504,   873,    83,   385,   386,    80,   137,   500,
      12,    84,   397,   116,   131,    56,   396,    96,   131,   156,
     156,   105,   139,    25,   161,    27,   139,   156,   156,   147,
     109,   501,   161,   155,   137,   164,   506,   156,   141,   163,
     156,   165,   427,   423,   156,   156,   431,   156,   163,   429,
     188,   688,   163,    94,   165,   163,   156,   165,   986,   100,
     116,   164,   564,   565,   566,   567,   568,   569,   570,   163,
      72,   165,   210,   211,   212,   213,   578,   156,   580,   217,
     156,   137,   161,   161,  1012,   141,   588,   927,   590,   571,
     572,   116,   594,   484,   596,   486,   156,    99,  1026,   632,
     110,   103,    79,    79,   163,   945,   163,   947,   164,   163,
     490,   581,   163,   156,   584,   253,   163,   142,   116,   144,
     145,   259,   156,   148,   149,     7,    58,   136,   630,   631,
     632,   161,  1060,   139,   636,   156,    26,   116,   372,   373,
     164,   137,   156,   281,   378,     2,    44,   381,   382,  1077,
     163,   137,   622,    33,    44,    12,   626,    47,   116,   164,
    1000,   299,  1002,   165,   164,   164,    20,  1095,    25,   164,
      27,  1099,   137,   675,   676,   166,   166,    67,   161,   649,
      70,   163,    72,   653,    44,   565,   156,   567,   156,    26,
     156,    81,   156,   156,    54,   164,   100,   699,   137,   166,
     702,   703,   704,   156,    30,   707,   156,   592,   593,    46,
     100,   156,    33,   598,   599,    72,    26,   164,   688,    76,
      77,   162,   164,    83,   137,   727,   162,   729,   899,   900,
     116,    68,   734,    70,    44,   737,    81,    47,   740,   772,
     742,   243,    99,   745,    81,   164,   103,    31,   250,   109,
     484,    31,   486,   161,    80,   166,   116,    67,    84,   397,
      70,   161,    72,    33,   136,   899,   900,    81,   163,   106,
     772,    81,   166,   136,   163,   166,   163,   137,   166,   156,
     782,   751,   784,   156,   754,   787,   162,   139,   139,   427,
     100,   793,   139,   431,   198,   932,   156,   131,   139,   139,
     139,   161,   131,   139,   164,   131,   776,   309,   165,   139,
     136,   139,   138,   139,   218,   131,   139,   143,   131,   139,
     131,   323,   702,   703,   794,   151,   161,   712,   713,   161,
     139,   157,   158,   159,    33,    96,   131,   164,   840,   724,
     164,   131,   164,   155,   578,   164,   137,   155,   166,   323,
     820,   736,   854,   855,   739,   857,   858,   859,   860,   263,
     676,   675,   293,    24,   776,   794,   868,   869,    30,   871,
     932,   873,   927,   875,   876,   752,   619,   879,   797,  1079,
    1011,   285,   919,   920,  1037,  1070,   243,  1063,    35,   451,
     580,    30,   394,   250,    -1,   897,   898,   899,   900,    -1,
      -1,    -1,   636,   905,    -1,   907,    -1,    -1,    -1,   911,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,
     422,    -1,    84,    -1,    -1,    -1,   428,   960,    -1,    -1,
      -1,    -1,    -1,   435,   936,   937,   938,   939,   940,   941,
     942,   943,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   309,   955,   592,   593,    -1,    -1,   960,    -1,
     598,   599,   932,   965,    -1,    -1,   323,   969,    -1,   131,
     132,    -1,   610,   135,   136,    -1,   138,   139,    -1,   383,
     384,   143,    -1,    -1,    -1,    -1,   488,    -1,    -1,   151,
     492,    -1,    -1,    -1,    -1,   157,   158,   159,   500,  1032,
      -1,    -1,  1004,  1005,  1006,  1007,    -1,    -1,    -1,    -1,
      -1,   649,    -1,    -1,    -1,    -1,    -1,    -1,   903,    -1,
      -1,    -1,    -1,    -1,  1026,    -1,     2,    -1,  1030,  1031,
    1032,    -1,    -1,    -1,    -1,    -1,    12,   394,    -1,  1041,
    1042,  1043,  1044,  1045,  1046,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,   459,    -1,  1059,  1060,   561,
    1062,    -1,   466,    -1,    -1,   422,    -1,    -1,    -1,    -1,
      -1,   428,    -1,    -1,   712,   713,    -1,   481,   435,    44,
     484,    -1,   486,    -1,    -1,    -1,   724,   491,   726,    54,
      -1,    -1,    -1,  1095,    -1,   733,    72,  1099,   736,    -1,
      -1,   739,    -1,    -1,    -1,    -1,    -1,   609,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    99,    -1,    12,    -1,   103,    -1,    -1,
      -1,   488,    -1,    -1,    -1,   492,    -1,    -1,    25,    -1,
      27,   779,    -1,   500,   109,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   661,
      -1,    -1,    21,    -1,    -1,   667,    25,    26,    -1,    -1,
      -1,    -1,   137,    -1,   578,    34,   580,    36,    37,    -1,
      -1,    -1,    41,    -1,    -1,    72,   688,    46,    -1,   165,
      -1,   156,    -1,    -1,     2,    -1,   161,    -1,    -1,   164,
     702,   703,    -1,    -1,    12,    -1,    -1,    -1,    -1,    68,
      -1,    70,    99,    72,    -1,    74,   103,    25,    77,    27,
      79,    -1,    81,    -1,    -1,   727,     2,   729,    -1,    -1,
      -1,    -1,   734,    -1,    -1,   737,    12,    -1,    -1,    -1,
      -1,   100,   744,    -1,   103,   747,    -1,   106,    -1,    25,
      -1,    27,   609,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,   903,    -1,   243,    -1,   771,
      -1,    -1,    -1,    -1,   250,    -1,    -1,    -1,   165,    -1,
     782,    -1,   784,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,    -1,    -1,   103,    72,   156,    -1,    -1,
      -1,    -1,    -1,   162,   661,    -1,    -1,    -1,    -1,    -1,
     667,   949,   950,    -1,    -1,   953,   954,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,
      -1,    -1,   970,   309,    -1,    -1,    -1,    -1,    -1,    -1,
     978,    -1,   980,    -1,   982,    -1,    -1,   323,    -1,    21,
      -1,    -1,    -1,    25,    26,    -1,   243,   165,    -1,    -1,
      -1,    -1,    34,   250,    36,    37,   868,    -1,    -1,    41,
     727,    -1,   729,   875,    46,    -1,    -1,   734,    -1,    -1,
     737,    -1,    -1,    -1,    -1,   887,    -1,   744,    -1,   165,
     747,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,   901,
      72,    -1,    74,    -1,    -1,    77,    -1,    79,   910,    81,
      -1,    -1,   914,    -1,   771,    -1,    -1,    -1,   394,    -1,
      -1,    -1,   309,    -1,    -1,   782,    -1,   784,   100,    -1,
      -1,   103,    -1,    -1,   106,   243,   323,    -1,    -1,    -1,
      -1,  1079,   250,    -1,    -1,    -1,   422,    -1,    -1,    -1,
      -1,    -1,   428,    -1,   956,    -1,    -1,    -1,    -1,   435,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   243,    -1,    -1,
      -1,    -1,    -1,    -1,   250,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   891,    -1,    -1,
     162,    -1,    -1,    -1,    -1,   899,   900,    -1,    -1,    -1,
      -1,   309,  1004,  1005,  1006,  1007,    -1,   394,    -1,    -1,
      -1,   868,   488,    -1,    -1,   323,   492,    -1,   875,    -1,
      -1,    -1,    -1,    -1,   500,    -1,    -1,    -1,  1030,  1031,
     887,    -1,    -1,   309,    -1,   422,    -1,    -1,    -1,    -1,
      -1,   428,    -1,    -1,   901,    -1,    -1,   323,   435,    -1,
      -1,    -1,     1,   910,    -1,    -1,    -1,   914,    -1,    -1,
      -1,   965,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,   394,    36,    37,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    46,    -1,   956,
      49,   488,    -1,    -1,    -1,   492,    -1,    -1,  1012,    -1,
      -1,    -1,    -1,   500,   422,    -1,    -1,    -1,   394,    68,
     428,    70,    -1,    72,    -1,    74,    75,   435,    77,    -1,
      79,    -1,    81,   609,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   422,  1004,  1005,  1006,
    1007,   100,   428,    -1,   103,    -1,  1060,   106,    -1,   435,
      -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1030,  1031,    12,    -1,    -1,    -1,    -1,
     488,    -1,    -1,    -1,   492,   661,    -1,    -1,    25,    -1,
      27,   667,   500,    -1,    -1,  1099,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
      -1,    -1,   488,   162,    -1,    -1,   492,    -1,    -1,    -1,
      -1,    -1,   609,    -1,   500,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    72,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    37,    -1,
      -1,   727,    41,   729,    -1,    -1,    -1,    46,   734,    -1,
      -1,   737,    99,    -1,    -1,    -1,   103,    -1,   744,    -1,
      -1,   747,    -1,    -1,   661,    -1,    -1,    -1,    -1,    68,
     667,    70,    -1,    72,    -1,    74,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,   771,    -1,    -1,    -1,    -1,
      -1,   609,    -1,    -1,    -1,    -1,   782,    -1,   784,    -1,
      -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,    21,
      -1,    -1,    -1,    25,    26,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    34,   609,    36,    37,    -1,    -1,    -1,    41,
     727,    -1,   729,    -1,    46,    -1,    -1,   734,    -1,    -1,
     737,    -1,    -1,   661,    -1,    -1,    -1,   744,    -1,   667,
     747,    -1,    -1,    -1,    -1,    -1,    68,   156,    70,    -1,
      72,    -1,    74,   162,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,   771,   661,    -1,    -1,    -1,    -1,
      -1,   667,   868,    -1,    -1,   782,    -1,   784,   100,   875,
      -1,   103,    -1,    -1,   106,    -1,   243,    -1,    -1,    -1,
      -1,   887,    -1,   250,    -1,    -1,    -1,    -1,    -1,   727,
      -1,   729,    -1,    -1,    -1,   901,   734,    -1,    -1,   737,
      -1,    -1,    -1,    -1,   910,    -1,   744,    -1,   914,   747,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   727,    -1,   729,   156,    -1,    -1,    -1,   734,    -1,
     162,   737,    21,   771,    -1,    -1,    25,    26,   744,    -1,
      -1,   747,   309,     2,   782,    34,   784,    36,    37,    -1,
     956,   868,    41,    12,    -1,    -1,   323,    46,   875,    -1,
      -1,    -1,    -1,    -1,    -1,   771,    25,    -1,    27,    -1,
     887,    -1,    -1,    -1,    -1,    -1,   782,    -1,   784,    68,
      -1,    70,    -1,    72,   901,    74,    -1,    -1,    77,    -1,
      79,    -1,    81,   910,    -1,    -1,    -1,   914,  1004,  1005,
    1006,  1007,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,
      -1,   100,    -1,    72,   103,    -1,    -1,   106,    12,    -1,
      -1,    -1,    -1,    -1,  1030,  1031,    -1,   394,    -1,    -1,
     868,    25,    -1,    27,    -1,    -1,    -1,   875,    -1,   956,
      99,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,   887,
      -1,    -1,    -1,    -1,    -1,   422,    -1,    -1,    -1,    -1,
      -1,   428,   868,   901,    -1,    -1,    -1,   156,   435,   875,
      -1,    -1,   910,   162,    -1,    -1,   914,    -1,    72,    -1,
      -1,   887,    -1,    -1,    -1,    -1,    -1,  1004,  1005,  1006,
    1007,    -1,    -1,    -1,    -1,   901,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   910,    99,   165,    -1,   914,   103,
      -1,    -1,    -1,  1030,  1031,    -1,    -1,    -1,   956,    -1,
      -1,   488,    -1,    -1,    -1,   492,    -1,    21,    -1,    -1,
      -1,    25,    26,   500,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    37,    -1,    -1,    -1,    41,    -1,    -1,
     956,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1004,  1005,  1006,  1007,
      -1,   165,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,
      74,    -1,    -1,    77,   243,    79,    -1,    81,    -1,    -1,
      -1,   250,  1030,  1031,     3,    -1,    -1,    -1,  1004,  1005,
    1006,  1007,    -1,    -1,    -1,    -1,   100,    16,    17,   103,
      19,    -1,   106,    -1,    -1,    24,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    32,  1030,  1031,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    45,    -1,    -1,    48,
      -1,    -1,   609,    -1,    -1,    -1,    -1,    56,    -1,   243,
     309,    -1,    -1,    62,    63,    -1,   250,    -1,    -1,    -1,
      69,    -1,   156,    -1,   323,    -1,    -1,    -1,    80,    78,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    -1,    -1,    97,    -1,
      -1,    -1,   101,   102,   661,   104,    -1,    -1,   107,    -1,
     667,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   309,    -1,    -1,    -1,   131,
     132,    -1,   134,   135,   136,    -1,   138,   139,   140,   323,
     142,   143,   144,   145,    -1,   394,   148,   149,   150,   151,
     152,   153,   154,    -1,    -1,   157,   158,   159,    -1,    -1,
      -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,
     727,    -1,   729,   422,    -1,    -1,    -1,   734,    -1,   428,
     737,    -1,    -1,    -1,    -1,    -1,   435,   744,     3,    -1,
     747,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    16,    17,    -1,    19,    -1,    -1,    -1,    -1,    24,
     394,    -1,    -1,    -1,   771,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,   782,    -1,   784,    43,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    -1,   422,   488,
      -1,    56,    -1,   492,   428,    -1,    -1,    62,    63,    -1,
      -1,   500,    -1,    -1,    69,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    78,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    87,    88,    30,    90,    -1,    92,    -1,    94,
      16,    17,    97,    19,    -1,    -1,   101,    -1,    24,   104,
      -1,    -1,   107,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,   488,    -1,    -1,    43,   492,    45,
      -1,   868,    48,    30,    -1,    -1,   500,    -1,   875,    -1,
      56,    -1,    -1,    -1,    80,    -1,    62,    63,    84,    -1,
     887,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,   901,    -1,   161,    -1,   163,    -1,
      -1,    87,    88,   910,    90,   111,    92,   914,    94,   115,
     609,    97,    -1,    80,    -1,   101,    -1,    84,   104,    -1,
      -1,   107,    -1,    -1,    -1,   131,   132,    -1,   134,   135,
     136,    -1,   138,   139,   140,    -1,   142,   143,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,   956,
      -1,   157,   158,   159,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,   661,    -1,   131,   132,     3,    -1,   667,   136,
      -1,   138,   139,    -1,    -1,   609,   143,   163,    -1,    16,
      17,    -1,    19,    -1,   151,    -1,    -1,    24,    -1,    -1,
     157,   158,   159,    -1,    -1,    32,    -1,  1004,  1005,  1006,
    1007,    38,    -1,    -1,    -1,    -1,    43,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,     3,    -1,  1030,  1031,    62,    63,   661,   727,    -1,
     729,    -1,    69,   667,    16,   734,    -1,    19,   737,    -1,
      -1,    78,    24,    -1,    -1,   744,    -1,    -1,   747,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    11,    -1,
      97,    43,    -1,    -1,   101,    18,    48,   104,    -1,    -1,
     107,    -1,   771,    -1,    56,    -1,    -1,    30,    -1,    -1,
      -1,    63,    -1,   782,    -1,   784,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   727,    -1,   729,    78,    -1,    -1,    11,
     734,    -1,    -1,   737,    -1,    -1,    18,    -1,    90,    -1,
     744,    -1,    94,   747,    -1,    -1,    -1,    -1,    30,   101,
      -1,    -1,    -1,    -1,    -1,   107,   163,    80,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,   771,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   782,    -1,
     784,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    30,    -1,    -1,    -1,    80,   868,
      -1,    -1,    84,    -1,    -1,    -1,   875,    -1,   131,   132,
      -1,   134,   135,   136,    -1,   138,   139,   140,   887,   142,
     143,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,   901,   115,   157,   158,   159,    -1,    -1,    -1,
      -1,   910,    -1,   166,    80,   914,    -1,    -1,    84,   131,
     132,    -1,   134,   135,   136,    -1,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,    -1,    -1,   157,   158,   159,    -1,    -1,
      -1,    -1,    -1,   887,   166,    -1,    -1,   956,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,   901,   134,   135,
     136,    -1,   138,   139,   140,    -1,   910,   143,   144,   145,
     914,    -1,   148,   149,   150,   151,    -1,    -1,   154,    -1,
      -1,   157,   158,   159,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1004,  1005,  1006,  1007,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       0,     1,   956,     3,     4,     5,     6,     7,     8,     9,
      -1,  1030,  1031,    13,    14,    15,    16,    17,    -1,    19,
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
     150,    -1,    -1,   153,   154,   155,   156,    -1,    -1,   159,
      -1,   161,    -1,   163,     1,   165,     3,     4,     5,     6,
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
       1,    -1,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    45,    46,    -1,    48,    49,    -1,
      51,    52,    53,    -1,    55,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    -1,    -1,    -1,    68,    69,    70,
      71,    -1,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
     101,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,    -1,
     161,   162,   163,     1,   165,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    13,    14,    15,    16,    17,
      -1,    19,    -1,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    -1,    31,    32,    -1,    34,    -1,    36,    37,
      38,    39,    40,    41,    42,    43,    -1,    45,    46,    -1,
      48,    49,    -1,    51,    52,    53,    -1,    55,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    -1,    -1,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
      -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,    -1,   153,   154,   155,   156,    -1,
      -1,   159,    -1,   161,   162,   163,     1,   165,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    13,    14,
      15,    16,    17,    -1,    19,    -1,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,    31,    32,    -1,    34,
      -1,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      45,    46,    -1,    48,    49,    -1,    51,    52,    53,    -1,
      55,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      -1,    -1,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      85,    86,    87,    88,    89,    90,    91,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,   101,    -1,   103,   104,
     105,   106,   107,    -1,    -1,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,
     155,   156,    -1,    -1,   159,    -1,   161,   162,   163,     1,
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
      -1,   150,    -1,    -1,   153,   154,   155,   156,    -1,    -1,
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
      -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,
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
     153,   154,   155,   156,    -1,    -1,   159,    -1,   161,    -1,
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
     150,    -1,    -1,   153,   154,   155,   156,    -1,    -1,   159,
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
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,   156,
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
     154,   155,   156,    -1,    -1,   159,    -1,   161,    -1,   163,
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
      -1,    -1,   153,   154,   155,   156,    -1,    -1,   159,    -1,
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
      -1,    -1,   150,    -1,    -1,   153,   154,   155,   156,    -1,
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
     155,   156,    -1,    -1,   159,    -1,   161,    -1,   163,     1,
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
      -1,   153,   154,   155,   156,    -1,    -1,   159,    -1,   161,
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
      -1,   150,    -1,    -1,   153,   154,   155,   156,    -1,    -1,
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
      -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,
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
     153,   154,   155,   156,    -1,    -1,   159,    -1,   161,    -1,
     163,     1,   165,     3,     4,     5,     6,     7,     8,     9,
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
     153,   154,   155,    -1,    -1,    -1,   159,    -1,   161,    -1,
     163,    -1,   165,   166,     3,     4,     5,     6,     7,     8,
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
     159,    -1,   161,    -1,   163,    -1,   165,   166,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,
      -1,    16,    17,    -1,    19,    -1,    21,    -1,    -1,    24,
      25,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,
      -1,    56,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    -1,    -1,    78,    79,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    91,    92,    -1,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,   104,
      -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      92,    -1,    94,    -1,    -1,    97,    98,    99,    -1,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,
      -1,   153,   154,   155,    -1,    -1,    -1,   159,    -1,   161,
      -1,   163,    -1,   165,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,    -1,
      19,    -1,    21,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    34,    -1,    -1,    -1,    38,
      39,    40,    -1,    42,    43,    44,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    -1,    56,    57,    -1,
      59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    71,    -1,    73,    -1,    -1,    -1,    -1,    78,
      79,    -1,    81,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    91,    92,    -1,    94,    -1,    -1,    97,    -1,
      -1,    -1,   101,    -1,    -1,   104,    -1,   106,   107,    -1,
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
      -1,    97,    98,    99,    -1,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,
      -1,    -1,    -1,   159,    -1,   161,    -1,   163,   164,   165,
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
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,    -1,    -1,   153,   154,   155,   156,    -1,    -1,   159,
      -1,   161,    -1,   163,    -1,   165,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,    16,
      17,    -1,    19,    -1,    21,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    34,    -1,    -1,
      -1,    38,    39,    40,    -1,    42,    43,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    53,    -1,    -1,    56,
      57,    -1,    59,    60,    61,    62,    63,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    71,    -1,    73,    -1,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    89,    90,    91,    92,    -1,    94,    -1,    -1,
      97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,    -1,
      -1,    -1,   159,    -1,   161,    -1,   163,    -1,   165,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    13,
      -1,    -1,    16,    17,    -1,    19,    -1,    21,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      34,    -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    -1,    56,    57,    -1,    59,    60,    -1,    62,    63,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    -1,    73,
      -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    89,    90,    91,    92,    -1,
      94,    -1,    -1,    97,    -1,    -1,   100,   101,    -1,    -1,
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
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    -1,    -1,    78,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
     101,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,   113,    -1,   115,    -1,    -1,    -1,    -1,    -1,
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
      98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,    -1,   153,   154,   155,    -1,    -1,
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
      -1,    -1,    97,    98,    99,    -1,   101,    -1,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,
     155,     1,    -1,     3,   159,    -1,   161,    -1,   163,    -1,
     165,    11,    -1,    -1,    -1,    -1,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    24,    -1,    21,    -1,    -1,    -1,
      25,    26,    32,    -1,    -1,    -1,    -1,    -1,    38,    34,
      -1,    36,    37,    43,    -1,    45,    41,    -1,    48,    -1,
      50,    46,    -1,    -1,    49,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    68,    -1,    70,    -1,    -1,    78,    -1,
      75,    -1,    -1,    -1,    79,    -1,    81,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,
      -1,   101,    -1,    -1,   104,   100,    -1,   107,   103,    -1,
     105,   106,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,   121,    -1,    -1,   124,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,    -1,    -1,   138,    -1,
      -1,    -1,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,   154,    -1,    -1,   157,   158,   159,
     160,    -1,    -1,   163,     3,     4,     5,     6,     7,     8,
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
      -1,    -1,    -1,    -1,   113,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,    -1,   153,   154,   155,    -1,    -1,    -1,
     159,    -1,   161,    -1,   163,    -1,   165,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,
      16,    17,    -1,    19,    -1,    21,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    34,    -1,
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
     113,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     100,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,    -1,    -1,   153,   154,   155,    -1,    -1,    -1,   159,
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
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,    -1,
      -1,    -1,   159,    -1,   161,   162,   163,    -1,   165,     3,
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
      -1,    -1,   150,    -1,    -1,   153,   154,   155,    -1,    -1,
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
     150,    -1,    -1,   153,   154,   155,    -1,    -1,    -1,   159,
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
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,    -1,
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
      -1,    -1,   150,    -1,    -1,   153,   154,   155,    -1,    -1,
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
     150,    -1,    -1,   153,   154,   155,    -1,    -1,    -1,   159,
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
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,    -1,
      -1,    -1,   159,    -1,    -1,    -1,   163,    -1,   165,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    -1,    13,
      -1,    -1,    16,    17,    -1,    19,    -1,    21,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      34,    -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    -1,    56,    57,    -1,    59,    60,    21,    62,    63,
      -1,    25,    26,    -1,    -1,    69,    -1,    71,    -1,    73,
      34,    -1,    36,    37,    78,    79,    -1,    41,    -1,    -1,
      -1,    -1,    46,    87,    88,    89,    90,    91,    92,    -1,
      94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,
     104,    -1,    -1,   107,    68,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,   100,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   150,    -1,    -1,    -1,
     154,   155,    -1,    -1,    -1,   159,    -1,   161,    -1,   163,
      -1,   165,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    -1,    -1,    16,    17,    -1,    19,    -1,
      21,    30,    -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    34,    -1,    -1,    -1,    38,    39,    40,
      -1,    42,    43,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    53,    54,    -1,    56,    57,    -1,    59,    60,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      71,    80,    73,    -1,    -1,    84,    -1,    78,    79,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    87,    88,    89,    90,
      91,    92,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
     101,    -1,    -1,   104,    -1,    -1,   107,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,   138,
     139,   140,   133,   142,   143,   144,   145,    -1,    -1,   148,
     149,   150,   151,   152,    -1,   154,    -1,    -1,   157,   158,
     159,    -1,    -1,    -1,   155,   156,    -1,    -1,    -1,    -1,
     161,    -1,   163,    -1,   165,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,
      30,    19,    -1,    21,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    34,    -1,    -1,    -1,
      38,    39,    40,    -1,    42,    43,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    53,    -1,    -1,    56,    57,
      -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,
      80,    69,    -1,    71,    84,    73,    -1,    -1,    -1,    -1,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    -1,    94,    -1,    -1,    97,
      -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      -1,   131,   132,    -1,   134,   135,   136,    -1,   138,   139,
     140,    -1,    -1,   143,    -1,   133,    -1,    -1,    -1,   137,
     150,   151,    -1,   141,   154,    -1,    -1,   157,   158,   159,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
      -1,    -1,    -1,   161,    -1,   163,   164,   165,     3,     4,
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
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,    -1,   131,    -1,   133,    -1,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,   147,
      13,    -1,    -1,    16,    17,    -1,    19,    -1,    21,    -1,
     155,    24,   160,    -1,    -1,   163,   161,   165,   163,    32,
     165,    34,    -1,    -1,    -1,    38,    39,    40,    -1,    42,
      43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      53,    -1,    -1,    56,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    -1,
      73,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    -1,    -1,    16,    17,    -1,    19,    -1,
      21,   160,   155,    24,   163,    -1,   165,    -1,   161,    -1,
     163,    32,   165,    34,    -1,    -1,    -1,    38,    39,    40,
      -1,    42,    43,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    -1,    56,    57,    -1,    59,    60,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      71,    -1,    73,    -1,    -1,    -1,    -1,    78,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      91,    92,    -1,    94,     3,    -1,    97,    -1,    -1,    -1,
     101,    -1,    11,   104,    -1,    -1,   107,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,   133,    -1,    43,    -1,    45,    -1,    -1,    48,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    62,    63,    -1,    -1,    -1,    11,    -1,
      69,    -1,   163,    -1,   165,    18,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    -1,    -1,    97,    -1,
      -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,   121,    -1,    -1,   124,   125,   126,   127,    30,
     129,   130,   131,   132,   133,   134,   135,    80,    -1,   138,
      -1,    84,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,   154,    11,    -1,   157,   158,
     159,   160,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    30,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,   131,   132,
      -1,   134,   135,   136,    -1,   138,   139,   140,    -1,   142,
     143,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,    -1,    11,   157,   158,   159,    -1,    -1,    -1,
      18,    -1,    -1,   166,    -1,    80,    -1,    -1,    -1,    84,
     131,   132,    30,   134,   135,   136,    -1,   138,   139,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,   150,
     151,    -1,    -1,   154,    11,    -1,   157,   158,   159,    -1,
     115,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    80,   138,   139,   140,    84,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
      11,    -1,   157,   158,   159,    -1,    -1,    18,    -1,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    30,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,    -1,   115,   157,
     158,   159,    -1,    -1,    -1,    -1,   164,    -1,    -1,    80,
      -1,    -1,    -1,    84,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    11,    -1,
     157,   158,   159,    -1,   115,    18,    -1,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
     131,   132,    -1,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,    -1,    -1,   157,   158,   159,    -1,
      -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    84,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      93,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,   120,   121,    -1,
      -1,   124,   125,   126,   127,    -1,   129,   130,   131,   132,
      -1,   134,   135,   136,    -1,   138,   139,   140,    -1,   142,
     143,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,    -1,    80,   157,   158,   159,    84,   161,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    93,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
     117,   118,   119,   120,   121,    -1,    -1,   124,   125,   126,
     127,    -1,   129,   130,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    -1,    80,
     157,   158,   159,    84,   161,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    93,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,   120,
     121,    -1,    -1,   124,   125,   126,   127,    -1,   129,   130,
     131,   132,    -1,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,    -1,    80,   157,   158,   159,    84,
     161,    -1,    11,    -1,    -1,    -1,    -1,    -1,    93,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,   117,   118,   119,   120,   121,    -1,    -1,   124,
     125,   126,   127,    -1,   129,   130,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
      -1,    80,   157,   158,   159,    84,   161,    -1,    11,    -1,
      -1,    -1,    -1,    -1,    93,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,
     119,   120,   121,    -1,    -1,   124,   125,   126,   127,    -1,
     129,   130,   131,   132,    -1,   134,   135,   136,    -1,   138,
     139,   140,    -1,   142,   143,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,    -1,    80,   157,   158,
     159,    84,   161,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      93,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,   120,   121,    -1,
      -1,   124,   125,   126,   127,    -1,   129,   130,   131,   132,
      -1,   134,   135,   136,    -1,   138,   139,   140,    -1,   142,
     143,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,    -1,    80,   157,   158,   159,    84,   161,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    93,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
     117,   118,   119,   120,   121,    -1,    -1,   124,   125,   126,
     127,    -1,   129,   130,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    -1,    80,
     157,   158,   159,    84,   161,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    93,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,   120,
     121,    -1,    -1,   124,   125,   126,   127,    -1,   129,   130,
     131,   132,    -1,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,    -1,    80,   157,   158,   159,    84,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
     115,   116,   117,   118,   119,   120,   121,    -1,    -1,   124,
     125,   126,   127,    -1,   129,   130,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
      11,    -1,   157,   158,   159,    80,   161,    18,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    44,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    31,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,    44,    -1,    -1,   143,    80,
      -1,    -1,    -1,    84,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,   157,   158,   159,    -1,    -1,    -1,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
     111,    80,    -1,    -1,   115,    84,    -1,    -1,    30,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,   132,    -1,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,   115,   148,   149,   150,
     151,   152,   153,   154,    -1,    -1,   157,   158,   159,    -1,
     161,    -1,   131,   132,    -1,   134,   135,   136,    80,   138,
     139,   140,    84,   142,   143,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,    11,    -1,   157,   158,
     159,    -1,   161,    18,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,   115,    -1,    30,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,    -1,   134,   135,   136,    -1,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,    11,    -1,   157,   158,   159,    -1,   161,
      18,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,
      -1,    -1,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    80,   138,   139,   140,    84,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
      -1,    -1,   157,   158,   159,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,   115,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,   115,
     148,   149,   150,   151,   152,   153,   154,    -1,    -1,   157,
     158,   159,    -1,   161,    -1,   131,   132,    -1,   134,   135,
     136,    -1,   138,   139,   140,    -1,   142,   143,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,    11,
      -1,   157,   158,   159,    -1,   161,    18,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      16,    17,    -1,    19,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    11,    -1,    -1,    -1,    -1,    32,    -1,    18,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    45,
      30,    -1,    48,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    62,    63,    80,    -1,
      -1,    -1,    84,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    65,    -1,    -1,    11,    -1,
      -1,    87,    88,    -1,    90,    18,    92,    -1,    94,    -1,
      80,    97,    -1,   115,    84,   101,    -1,    30,   104,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,    44,   134,   135,   136,    -1,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,   115,   148,   149,   150,   151,
     152,   153,   154,    -1,    -1,   157,   158,   159,    -1,   161,
      -1,   131,   132,    -1,   134,   135,   136,    80,   138,   139,
     140,    84,   142,   143,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,    11,   156,   157,   158,   159,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,   115,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
      -1,   134,   135,   136,    -1,   138,   139,   140,    -1,   142,
     143,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,    11,    -1,   157,   158,   159,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,
      -1,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    11,    44,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    30,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,    44,   134,   135,
     136,    80,   138,   139,   140,    84,   142,   143,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,   115,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,   138,
     139,   140,    -1,   142,   143,   144,   145,   146,   115,   148,
     149,   150,   151,   152,   153,   154,    -1,    -1,   157,   158,
     159,    -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    11,    21,
     157,   158,   159,    25,    26,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    37,    -1,    30,    -1,    41,
      -1,    -1,    35,    -1,    46,    -1,    -1,    49,    -1,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    30,
      -1,    -1,    65,    75,    -1,    -1,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,   100,    11,
      -1,   103,    -1,   105,   106,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    80,
      -1,    -1,   115,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
      -1,   134,   135,   136,    -1,   138,   139,   140,    -1,   142,
     143,   144,   145,   146,   115,   148,   149,   150,   151,   152,
     153,   154,    -1,    -1,   157,   158,   159,    -1,    80,    -1,
     131,   132,    84,   134,   135,   136,    -1,   138,   139,   140,
      -1,   142,   143,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,    11,   156,   157,   158,   159,    -1,
      -1,    18,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,   131,
     132,    -1,   134,   135,   136,    -1,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,    11,   156,   157,   158,   159,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    80,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,   131,   132,    84,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,    -1,    80,    -1,   114,   115,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,   115,
     148,   149,   150,   151,   152,   153,   154,    -1,    -1,   157,
     158,   159,    -1,    -1,    -1,   131,   132,    -1,   134,   135,
     136,    -1,   138,   139,   140,    -1,   142,   143,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,    11,
      -1,   157,   158,   159,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,   131,
     132,    -1,   134,   135,   136,    -1,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,    -1,   111,   157,   158,   159,   115,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,
      -1,   138,   139,   140,    -1,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,    -1,   115,
     157,   158,   159,    -1,    -1,    -1,    11,    12,    -1,    -1,
      -1,    -1,    -1,    18,    -1,   131,   132,    -1,   134,   135,
     136,    -1,   138,   139,   140,    30,   142,   143,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,    -1,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     115,    -1,   157,   158,   159,    -1,    80,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     114,   115,   157,   158,   159,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,
     134,   135,   136,    -1,   138,   139,   140,    -1,   142,   143,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,    11,    -1,   157,   158,   159,    -1,     1,    18,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    31,    16,    17,    -1,    19,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    11,    -1,    -1,    -1,    -1,    32,    -1,
      18,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    45,    30,    31,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    62,    63,
      80,    -1,    -1,    -1,    84,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    -1,    80,    97,    -1,   115,    84,   101,    -1,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,    -1,   134,   135,   136,    -1,   138,   139,
     140,    -1,   142,   143,   144,   145,   146,   115,   148,   149,
     150,   151,   152,   153,   154,    -1,    -1,   157,   158,   159,
      -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,    11,    -1,   157,
     158,   159,    -1,    -1,    18,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    16,    17,
      -1,    19,    -1,    -1,    -1,    -1,    24,    -1,    -1,    11,
      -1,    -1,    -1,    -1,    32,    -1,    18,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    45,    30,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    62,    63,    80,    -1,    -1,    -1,
      84,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      78,    -1,    -1,    -1,    -1,    -1,    11,    12,    -1,    87,
      88,    -1,    90,    18,    92,    -1,    94,    -1,    80,    97,
      -1,   115,    84,   101,    -1,    30,   104,    -1,    -1,   107,
      -1,    93,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,
     134,   135,   136,    -1,   138,   139,   140,    -1,   142,   143,
     144,   145,   146,   115,   148,   149,   150,   151,   152,   153,
     154,    -1,    -1,   157,   158,   159,    -1,    -1,    -1,   131,
     132,    -1,   134,   135,   136,    80,   138,   139,   140,    84,
     142,   143,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,    11,    -1,   157,   158,   159,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,    -1,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
      -1,    -1,   157,   158,   159,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    93,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,    -1,    80,   157,
     158,   159,    84,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    93,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,    -1,   134,   135,   136,    -1,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,    -1,    80,   157,   158,   159,    84,    -1,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    93,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,   135,
     136,    -1,   138,   139,   140,    -1,   142,   143,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,    -1,
      80,   157,   158,   159,    84,    -1,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    93,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,    -1,   134,   135,   136,    -1,   138,   139,
     140,    -1,   142,   143,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,    -1,    80,   157,   158,   159,
      84,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    93,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,
     134,   135,   136,    -1,   138,   139,   140,    -1,   142,   143,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,    -1,    80,   157,   158,   159,    84,    -1,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    93,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,   131,   132,    -1,   134,   135,   136,    -1,
     138,   139,   140,    -1,   142,   143,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,    -1,    80,   157,
     158,   159,    84,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,    -1,   134,   135,   136,    -1,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,    -1,    80,   157,   158,   159,    84,    -1,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,   135,
     136,    -1,   138,   139,   140,    -1,   142,   143,   144,   145,
     146,    30,   148,   149,   150,   151,   152,   153,   154,    -1,
      80,   157,   158,   159,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,   131,   132,    -1,   134,   135,   136,    -1,   138,   139,
     140,    -1,   142,   143,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,    -1,   115,   157,   158,   159,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,    -1,   134,   135,   136,    -1,   138,
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
      -1,    -1,     3,    -1,    62,    63,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    16,    17,    -1,    19,    -1,
      78,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    87,
      88,    32,    90,    -1,    92,    -1,    94,    38,    -1,    97,
      -1,    -1,    43,   101,    45,    -1,   104,    48,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,     3,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    16,    17,    -1,    19,    -1,    78,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    87,    88,    32,    90,
      -1,    92,    -1,    94,    38,    -1,    97,    -1,    -1,    43,
     101,    45,    -1,   104,    48,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    62,    63,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,
     104,    -1,    -1,   107
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   172,   173,     0,     1,     3,     4,     5,     6,     7,
       8,     9,    13,    14,    15,    16,    17,    19,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    31,    32,    34,
      36,    37,    38,    39,    40,    41,    42,    43,    45,    46,
      48,    51,    52,    53,    55,    56,    57,    59,    60,    62,
      63,    64,    68,    69,    70,    71,    72,    73,    74,    77,
      78,    79,    81,    82,    83,    85,    86,    87,    88,    89,
      90,    91,    92,    94,    95,    97,    98,    99,   100,   101,
     103,   104,   106,   107,   110,   112,   115,   131,   132,   133,
     134,   135,   140,   150,   153,   154,   155,   156,   159,   161,
     163,   165,   174,   175,   176,   177,   178,   179,   181,   186,
     187,   188,   189,   193,   195,   198,   199,   200,   201,   202,
     207,   208,   210,   211,   212,   216,   217,   220,   221,   224,
     225,   228,   231,   232,   257,   260,   261,   281,   282,   283,
     284,   285,   286,   287,   295,   296,   297,   298,   299,   302,
     303,   304,   305,   306,   307,   308,   310,   311,   312,   313,
     314,   156,   176,   299,   111,   288,   289,     3,   190,    13,
      21,    34,    39,    40,    42,    79,    92,   161,   165,   228,
     281,   286,   297,   298,   299,   302,   304,   305,   288,   299,
     100,   260,    81,   190,   176,   275,   299,   176,   163,     7,
      79,   299,   300,     7,    10,    79,   100,   300,    70,   113,
     209,   299,   209,   299,    25,   261,   299,   299,   163,     3,
      16,    17,    19,    24,    32,    38,    43,    45,    48,    56,
      62,    63,    69,    78,    87,    88,    90,    92,    94,    97,
     101,   104,   107,   192,   194,   229,   163,   192,   262,   263,
      31,   181,   197,   299,   299,    17,    69,    87,   104,   299,
     299,   299,     7,   163,    21,    34,    37,    79,   200,   200,
     155,   251,   274,   299,    98,    99,   156,   299,   301,   299,
     197,   299,   299,   299,    91,   163,   176,   299,   299,   181,
     189,   299,   302,   181,   189,   299,   192,   258,   299,   299,
     299,   299,   299,   299,   299,   299,     1,   162,   174,   182,
     274,   102,   141,   251,   276,   277,   301,   209,   274,   299,
     309,    72,   176,   161,    76,   180,   105,   192,   192,    49,
      75,   246,   262,   155,   156,   147,   299,    11,    18,    30,
      80,    84,   115,   131,   132,   134,   135,   136,   138,   139,
     140,   142,   143,   144,   145,   146,   148,   149,   150,   151,
     152,   153,   154,   157,   158,   159,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   160,   238,   163,   165,    80,    84,   163,   176,   156,
     299,   299,   299,   274,   161,   181,    44,   288,   258,   262,
     156,   137,   156,   110,   193,   251,   278,   279,   280,   301,
      79,   205,   232,   260,    79,   203,   258,   232,   260,   192,
     163,   197,    31,    44,   197,   111,   197,   291,    31,    44,
     197,   291,    35,    65,   156,    93,   181,   238,   278,   176,
     163,   240,   102,   163,   192,   264,   265,     1,   136,   269,
      44,   137,   176,   197,   163,   163,   197,   278,   192,   137,
     156,   299,   299,   156,   161,   197,   163,   278,   156,   213,
     156,   213,   156,   116,   259,   156,   197,   156,   162,   162,
     174,   137,   162,   299,   137,   164,   137,   164,   166,   291,
      44,   137,   166,   291,   114,   137,   166,     7,     1,   162,
     182,    58,   274,   299,   136,   222,   161,   233,   156,   299,
     299,   299,   299,   209,   299,   209,   299,   299,   299,   299,
     299,   299,   299,    19,    32,    56,   100,   193,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   301,   301,   301,   301,   301,   278,
     278,   209,   299,   209,   299,    26,    44,    81,   106,   290,
     293,   294,   299,   314,    31,    44,    31,    44,    93,    44,
     166,   209,   299,   197,   156,   156,   299,   299,   116,   164,
     137,   206,   192,   262,   204,   192,   156,   262,    44,   274,
      42,   299,   209,   299,   163,   197,    42,   299,   209,   299,
     197,   159,   183,   185,   299,   183,   184,   176,   299,    33,
     299,   164,    26,    44,    47,    67,    70,    81,   100,   175,
     241,   242,   243,   244,   230,   265,   137,   164,    32,    45,
      88,    92,   165,   196,   270,   282,   116,   266,   299,   263,
     299,   299,   164,   251,   299,     1,   218,   278,   164,    20,
     214,   270,   282,   137,   162,   164,   164,   276,   164,   276,
     176,   166,   209,   299,   166,   176,   299,   166,   299,   166,
     299,   162,   162,   192,   156,    35,    65,   274,   161,     1,
     192,   226,   227,    26,    70,    81,   100,   235,   245,   163,
     156,   156,   156,   156,   156,   164,   166,    44,    81,   137,
     164,   281,    80,    80,    42,   209,   299,    42,   209,   299,
     209,   299,   288,   288,   156,   251,   301,   280,   192,   222,
     156,   192,   222,   156,   299,   164,   299,    31,   197,    31,
     197,   292,   293,   299,    31,   197,   291,    31,   197,   291,
     137,   156,    12,   156,    33,    33,   176,    93,   181,    44,
      81,   243,   137,   164,   163,   192,    26,    70,    81,   100,
     247,   164,   265,   269,     1,   274,    61,   301,   164,   164,
     162,    66,   108,   162,   219,   164,   163,   181,   192,   215,
     258,   176,   166,   291,   166,   291,   176,   114,   185,   184,
       1,   223,   162,   116,   137,   162,    81,   234,     1,     3,
      11,    16,    18,    19,    24,    38,    43,    48,    50,    56,
      62,    63,    78,    90,    94,    97,   101,   107,   131,   132,
     133,   134,   135,   138,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   154,   157,   158,   159,   160,
     163,   191,   192,   194,   236,   237,   238,   281,   164,   293,
     269,   281,   281,   299,    31,    31,   299,    31,    31,   166,
     166,   197,   197,   222,   161,   222,   161,   197,    93,    42,
     299,    42,   299,   137,   164,    93,    42,   299,   197,    42,
     299,   197,   299,   299,   176,   299,   176,    33,   192,   242,
     265,   136,   273,    81,   269,   266,   166,    44,   166,   163,
     163,    31,   176,   274,   215,   136,   181,    42,   176,   299,
     166,    42,   176,   299,   166,   299,   156,   156,   162,    74,
      77,   162,   175,   199,   299,   227,   247,   163,   239,   192,
     299,   131,   139,   239,   239,   266,    93,    42,    42,    93,
      42,    42,    42,    42,   161,   223,   161,   223,   299,   299,
     299,   293,   299,   299,   299,    12,    33,   176,   273,   164,
     165,   196,   251,   272,   282,   141,   252,   266,    54,   109,
     253,   299,   270,   282,   278,   278,   176,   197,   164,   299,
     299,   176,   299,   176,   199,     1,   136,   268,   241,   164,
       3,    94,   237,   238,   299,   299,   299,   299,   299,   299,
     223,   162,   223,   162,    93,    93,    93,    93,   299,   176,
     266,   273,   166,   274,   251,   299,     3,    83,    94,   254,
     255,   256,   299,   181,   198,   250,   166,   164,   164,   181,
      93,    93,   165,   196,   267,   282,    96,   248,   164,   239,
     239,    93,    93,    93,    93,    93,    93,   162,   162,   299,
     299,   299,   299,   252,   266,   251,   271,   272,   282,    44,
     166,   256,   109,   137,   116,   142,   144,   145,   148,   149,
      54,   282,   155,   155,   299,   299,     1,   166,   274,   253,
     299,   271,   272,   299,   255,   256,   256,   256,   256,   256,
     256,   254,   166,   267,   282,   166,   156,   249,   250,   166,
     267,   282,   271
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
#line 495 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 500 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 508 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 513 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 514 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 533 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 534 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 535 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 540 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 542 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 543 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 544 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 545 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 546 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 547 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 548 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 553 "chapel.ypp"
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
#line 566 "chapel.ypp"
    { (yyvsp[(1) - (3)].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (3)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (3)]).prevModule;  // restore previous module name
    }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 571 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = (yyvsp[(3) - (4)].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 583 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 584 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 585 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 589 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 590 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 603 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 605 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 610 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 611 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 616 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 621 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 626 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 630 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 637 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 646 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 655 "chapel.ypp"
    { (yyval.b) = (currentModuleType != MOD_INTERNAL ? true : false); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 656 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 657 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 661 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (4)].pcallexpr), (yyvsp[(1) - (4)].b)); }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 662 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), (yyvsp[(5) - (6)].ponlylist), true, (yyvsp[(1) - (6)].b)); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 663 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), (yyvsp[(5) - (6)].ponlylist), false, (yyvsp[(1) - (6)].b)); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 667 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 672 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 674 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 676 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 678 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 680 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 686 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 687 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 694 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 702 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 703 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 704 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 716 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 717 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 728 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 729 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 730 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 731 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 732 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 733 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 734 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 737 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 738 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 739 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 740 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 742 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 743 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 744 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 745 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 758 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 759 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 760 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 767 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 768 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 770 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 774 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 775 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 779 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 780 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 784 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 786 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 787 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 801 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 802 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 803 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 804 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 809 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 814 "chapel.ypp"
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

  case 144:
/* Line 1792 of yacc.c  */
#line 824 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 829 "chapel.ypp"
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

  case 146:
/* Line 1792 of yacc.c  */
#line 840 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 845 "chapel.ypp"
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

  case 148:
/* Line 1792 of yacc.c  */
#line 855 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 860 "chapel.ypp"
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

  case 150:
/* Line 1792 of yacc.c  */
#line 871 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 875 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 880 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 888 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 899 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 905 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 906 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 907 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 908 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 909 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 910 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 911 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 912 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 913 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 914 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 915 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 916 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 917 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 918 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 919 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 920 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 921 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 922 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 924 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 930 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 936 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 942 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 948 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 955 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 962 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 966 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 972 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 976 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 977 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 978 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 979 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 980 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 982 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 984 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 986 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 991 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 994 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 995 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 996 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 997 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 998 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 999 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1003 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1004 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1008 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1009 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1010 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1014 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1023 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1025 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1029 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1030 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1035 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1037 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1039 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1046 "chapel.ypp"
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

  case 213:
/* Line 1792 of yacc.c  */
#line 1056 "chapel.ypp"
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

  case 214:
/* Line 1792 of yacc.c  */
#line 1069 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1074 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1079 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1087 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1097 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1102 "chapel.ypp"
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

  case 223:
/* Line 1792 of yacc.c  */
#line 1118 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1125 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1134 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1142 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1146 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1152 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1153 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1158 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1163 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1168 "chapel.ypp"
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

  case 233:
/* Line 1792 of yacc.c  */
#line 1190 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1193 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1197 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1205 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1215 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1222 "chapel.ypp"
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

  case 239:
/* Line 1792 of yacc.c  */
#line 1248 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1252 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1257 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1261 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1266 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1273 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1277 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1283 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1284 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1286 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1288 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1298 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1308 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1309 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1312 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1313 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1314 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1317 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1318 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1319 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1323 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1324 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1328 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1332 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1333 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1341 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1343 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1345 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1347 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1351 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1352 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1356 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1357 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1358 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1363 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1364 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1370 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1377 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1378 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1382 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1383 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1384 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1386 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1387 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1391 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1392 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1395 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1401 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1406 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1410 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1411 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1412 "chapel.ypp"
    { if (DefExpr* def = toDefExpr((yyvsp[(2) - (2)].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
                         }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1420 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1422 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1424 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1426 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1428 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1435 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1439 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1440 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1441 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1442 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1443 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1444 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1445 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1450 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1452 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1457 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1459 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1461 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1466 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1479 "chapel.ypp"
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

  case 353:
/* Line 1792 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1497 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1499 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1512 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1518 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1527 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1542 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1544 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1546 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1553 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1555 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1561 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1563 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1569 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1571 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1573 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1577 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1583 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1587 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1594 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1597 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1598 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1604 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1605 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1606 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1607 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1636 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1644 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1650 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1651 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1652 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1657 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1659 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1665 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1667 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1669 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1673 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1674 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1676 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1677 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1683 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1684 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1685 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1686 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1690 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1691 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1695 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1696 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1697 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1701 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1711 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1712 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1719 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1724 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1736 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1738 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1740 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1751 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1753 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1755 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1757 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1759 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1761 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1763 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1766 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1768 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1770 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1772 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1776 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1778 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1780 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1783 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1785 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1787 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1792 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1794 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1796 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1798 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1800 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1802 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1804 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1806 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1808 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1810 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1812 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1814 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1816 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1823 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1829 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1835 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1841 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1850 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1859 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1875 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1879 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1884 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1888 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1889 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1893 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1897 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1898 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1903 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1907 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1911 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1917 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1918 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1919 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1920 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1921 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1922 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1928 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1933 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1935 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1939 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1943 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1947 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 1955 "chapel.ypp"
    { (yyvsp[(1) - (2)].pcallexpr)->insertAtTail((yyvsp[(2) - (2)].pexpr));
      (yyval.pexpr) = (yyvsp[(1) - (2)].pcallexpr); }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1960 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1966 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1972 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1979 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1988 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtOwned->symbol, (yyvsp[(4) - (5)].pexpr))); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1990 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtShared->symbol, (yyvsp[(4) - (5)].pexpr))); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1995 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 2011 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 2013 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 2015 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 2017 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 2019 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 2021 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 2025 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 2026 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 2030 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 2031 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 2032 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 2050 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 2051 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 2052 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr));}
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 2053 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 2057 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 2058 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 2059 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 2063 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 2064 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 2065 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 2066 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 2067 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 2075 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 2076 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 2077 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 2078 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 2082 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 2083 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 2088 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 2089 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 2090 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2091 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2092 "chapel.ypp"
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2093 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 2094 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2095 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 2096 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 2097 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2098 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2100 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2104 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2111 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2112 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2116 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2117 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2118 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2119 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2120 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2121 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2122 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2123 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2124 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2125 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2126 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 584:
/* Line 1792 of yacc.c  */
#line 2127 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 585:
/* Line 1792 of yacc.c  */
#line 2128 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 586:
/* Line 1792 of yacc.c  */
#line 2129 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 587:
/* Line 1792 of yacc.c  */
#line 2130 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 588:
/* Line 1792 of yacc.c  */
#line 2131 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 589:
/* Line 1792 of yacc.c  */
#line 2132 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 590:
/* Line 1792 of yacc.c  */
#line 2133 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 591:
/* Line 1792 of yacc.c  */
#line 2134 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 592:
/* Line 1792 of yacc.c  */
#line 2135 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 593:
/* Line 1792 of yacc.c  */
#line 2136 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 594:
/* Line 1792 of yacc.c  */
#line 2137 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 595:
/* Line 1792 of yacc.c  */
#line 2138 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 596:
/* Line 1792 of yacc.c  */
#line 2142 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 597:
/* Line 1792 of yacc.c  */
#line 2143 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 598:
/* Line 1792 of yacc.c  */
#line 2144 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 599:
/* Line 1792 of yacc.c  */
#line 2145 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 600:
/* Line 1792 of yacc.c  */
#line 2146 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 601:
/* Line 1792 of yacc.c  */
#line 2147 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 602:
/* Line 1792 of yacc.c  */
#line 2148 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 603:
/* Line 1792 of yacc.c  */
#line 2152 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 604:
/* Line 1792 of yacc.c  */
#line 2153 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 605:
/* Line 1792 of yacc.c  */
#line 2154 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 606:
/* Line 1792 of yacc.c  */
#line 2155 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 607:
/* Line 1792 of yacc.c  */
#line 2159 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 608:
/* Line 1792 of yacc.c  */
#line 2160 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 609:
/* Line 1792 of yacc.c  */
#line 2161 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 610:
/* Line 1792 of yacc.c  */
#line 2162 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 611:
/* Line 1792 of yacc.c  */
#line 2167 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 612:
/* Line 1792 of yacc.c  */
#line 2168 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 613:
/* Line 1792 of yacc.c  */
#line 2169 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 614:
/* Line 1792 of yacc.c  */
#line 2170 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 615:
/* Line 1792 of yacc.c  */
#line 2171 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 616:
/* Line 1792 of yacc.c  */
#line 2172 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 617:
/* Line 1792 of yacc.c  */
#line 2173 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 10122 "bison-chapel.cpp"
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


