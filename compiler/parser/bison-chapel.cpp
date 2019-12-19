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
#line 200 "chapel.ypp"

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
#line 571 "bison-chapel.cpp"

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
#define YYLAST   18053

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  172
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  144
/* YYNRULES -- Number of rules.  */
#define YYNRULES  618
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1104

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
    1048,  1050,  1053,  1056,  1057,  1060,  1063,  1068,  1073,  1075,
    1079,  1083,  1087,  1091,  1095,  1099,  1103,  1106,  1108,  1110,
    1114,  1119,  1124,  1127,  1132,  1133,  1136,  1139,  1141,  1143,
    1145,  1148,  1150,  1155,  1159,  1161,  1165,  1169,  1175,  1177,
    1179,  1183,  1185,  1188,  1192,  1193,  1196,  1199,  1203,  1206,
    1211,  1215,  1219,  1224,  1228,  1229,  1232,  1235,  1238,  1240,
    1241,  1244,  1247,  1250,  1252,  1257,  1262,  1269,  1273,  1274,
    1276,  1278,  1282,  1287,  1291,  1296,  1303,  1304,  1307,  1310,
    1313,  1316,  1318,  1320,  1324,  1328,  1330,  1334,  1336,  1338,
    1340,  1344,  1348,  1349,  1351,  1353,  1357,  1361,  1365,  1367,
    1369,  1371,  1373,  1375,  1378,  1380,  1382,  1384,  1386,  1388,
    1390,  1393,  1398,  1403,  1408,  1414,  1417,  1420,  1422,  1425,
    1427,  1430,  1432,  1435,  1437,  1440,  1442,  1444,  1446,  1453,
    1460,  1465,  1475,  1485,  1493,  1500,  1507,  1512,  1522,  1532,
    1540,  1545,  1552,  1559,  1569,  1579,  1586,  1588,  1590,  1592,
    1594,  1596,  1598,  1600,  1602,  1606,  1607,  1609,  1614,  1616,
    1620,  1625,  1627,  1631,  1636,  1640,  1644,  1646,  1648,  1651,
    1653,  1656,  1658,  1660,  1664,  1666,  1669,  1672,  1675,  1678,
    1681,  1690,  1699,  1709,  1719,  1725,  1731,  1736,  1738,  1740,
    1742,  1744,  1746,  1748,  1750,  1752,  1757,  1761,  1765,  1768,
    1771,  1773,  1774,  1776,  1779,  1782,  1784,  1786,  1788,  1790,
    1792,  1794,  1797,  1800,  1802,  1807,  1812,  1817,  1821,  1825,
    1829,  1833,  1839,  1843,  1848,  1852,  1857,  1859,  1861,  1863,
    1865,  1867,  1869,  1871,  1873,  1875,  1877,  1881,  1886,  1890,
    1895,  1899,  1904,  1908,  1914,  1918,  1922,  1926,  1930,  1934,
    1938,  1942,  1946,  1950,  1954,  1958,  1962,  1966,  1970,  1974,
    1978,  1982,  1986,  1990,  1994,  1998,  2002,  2006,  2009,  2012,
    2015,  2018,  2021,  2024,  2027,  2031,  2035,  2039,  2043,  2047,
    2051,  2055,  2059,  2061,  2063,  2065,  2067,  2069,  2071
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     173,     0,    -1,   174,    -1,    -1,   174,   175,    -1,   177,
      -1,   176,   177,    -1,    73,     8,    -1,   176,    73,     8,
      -1,   179,    -1,   182,    -1,   188,    -1,   189,    -1,   200,
      -1,   190,    -1,   208,    -1,   211,    -1,   209,    -1,   218,
      -1,   212,    -1,   213,    -1,   217,    -1,   199,    -1,   288,
     157,    -1,    14,   177,    -1,    15,   289,   177,    -1,    16,
     191,   157,    -1,    23,   289,   182,    -1,    28,   191,   157,
      -1,    30,   276,   157,    -1,    52,   193,   177,    -1,    56,
     300,   198,    -1,    56,   198,    -1,    65,   300,   198,    -1,
      87,   300,   198,    -1,    87,   198,    -1,    93,   177,    -1,
     113,   300,   157,    -1,     1,   157,    -1,   180,   181,    59,
     193,    -1,   178,   162,   163,    -1,   178,   162,   183,   163,
      -1,   178,   162,     1,   163,    -1,    -1,    78,    -1,    75,
      -1,    -1,    77,    -1,   162,   163,    -1,   162,   183,   163,
      -1,   162,     1,   163,    -1,   175,    -1,   183,   175,    -1,
     300,    -1,   300,    13,   300,    -1,   184,   138,   300,    -1,
     184,   138,   300,    13,   300,    -1,    -1,   184,    -1,   160,
      -1,   184,    -1,    -1,    78,    -1,    75,    -1,   187,   106,
     275,   157,    -1,   187,   106,   300,    36,   186,   157,    -1,
     187,   106,   300,    66,   185,   157,    -1,    83,   275,   157,
      -1,   303,   239,   302,   157,    -1,   303,   161,   302,   157,
      -1,   303,   129,   302,   157,    -1,   303,   123,   302,   157,
      -1,   303,   124,   302,   157,    -1,    -1,     3,    -1,     3,
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
      84,   157,    -1,    84,   302,   157,    -1,   157,    -1,   201,
      -1,    78,   201,    -1,    75,   201,    -1,   233,    -1,   262,
      -1,   225,    -1,   258,    -1,   221,    -1,   202,    -1,   203,
      -1,    42,   300,   157,    -1,    42,   300,    36,   186,   157,
      -1,    42,   300,    66,   185,   157,    -1,    42,   262,    -1,
      -1,    38,    80,   204,   193,   223,   162,   224,   163,    -1,
      -1,    38,     8,    80,   205,   193,   223,   162,   224,   163,
      -1,    -1,    37,    80,   206,   193,   223,   162,   224,   163,
      -1,    -1,    37,     8,    80,   207,   193,   223,   162,   224,
     163,    -1,    38,   301,   233,    -1,    37,   301,   233,    -1,
      38,   301,   261,   263,   157,    -1,    37,   301,   261,   263,
     157,    -1,    38,    11,    -1,    32,   177,   111,   300,   157,
      -1,   111,   300,   198,    -1,    24,   300,    45,   300,   289,
     198,    -1,    24,   300,    45,   210,   289,   198,    -1,    24,
     300,   289,   198,    -1,    40,   300,    45,   300,   198,    -1,
      40,   300,    45,   210,   198,    -1,    40,   300,   198,    -1,
      40,   210,   198,    -1,    40,    71,   193,    45,   300,   198,
      -1,    41,   300,    45,   300,   198,    -1,    41,   300,    45,
     300,   292,   198,    -1,    41,   300,    45,   210,   198,    -1,
      41,   300,    45,   210,   292,   198,    -1,    41,   300,   198,
      -1,    41,   300,   292,   198,    -1,    41,   210,   198,    -1,
      41,   210,   292,   198,    -1,   166,   275,    45,   300,   167,
     177,    -1,   166,   275,    45,   300,   292,   167,   177,    -1,
     166,   275,    45,   210,   167,   177,    -1,   166,   275,    45,
     210,   292,   167,   177,    -1,   166,   275,   167,   177,    -1,
     166,   275,   292,   167,   177,    -1,   166,   210,   167,   177,
      -1,   166,   210,   292,   167,   177,    -1,   114,   164,   275,
     165,    -1,    43,   300,    94,   177,    -1,    43,   300,   182,
      -1,    43,   300,    94,   177,    34,   177,    -1,    43,   300,
     182,    34,   177,    -1,    43,   300,   239,   300,    94,   177,
      -1,    43,   300,   239,   300,   182,    -1,    43,   300,   239,
     300,    94,   177,    34,   177,    -1,    43,   300,   239,   300,
     182,    34,   177,    -1,    29,   177,    -1,    99,   300,   157,
      -1,   100,   300,   157,    -1,    99,   190,    -1,   100,   190,
      -1,    99,   182,   214,    -1,   100,   182,   214,    -1,    -1,
     214,   215,    -1,    21,   182,    -1,    21,   216,   182,    -1,
      21,   164,   216,   165,   182,    -1,   193,    -1,   193,   137,
     300,    -1,    96,   300,   157,    -1,    86,   300,   162,   219,
     163,    -1,    86,   300,   162,     1,   163,    -1,    -1,   219,
     220,    -1,   109,   275,   198,    -1,    67,   177,    -1,    67,
      32,   177,    -1,   222,   193,   223,   162,   224,   163,    -1,
     222,   193,   223,   162,     1,   163,    -1,    22,    -1,    80,
      -1,   104,    -1,    -1,   137,   275,    -1,    -1,   224,   200,
      -1,   224,   176,   200,    -1,   226,   193,   162,   227,   163,
      -1,   226,   193,   162,     1,   163,    -1,    35,    -1,   228,
      -1,   227,   138,    -1,   227,   138,   228,    -1,   193,    -1,
     193,   117,   300,    -1,    -1,    -1,    53,   230,   241,   231,
     248,   270,   254,   251,    -1,    -1,    47,    -1,    69,    -1,
      -1,    -1,   232,   247,   234,   236,   235,   248,   269,   249,
     254,   250,    -1,   246,   238,   240,    -1,   246,   239,   240,
      -1,   246,   237,   140,   238,   240,    -1,   246,   237,   140,
     239,   240,    -1,   246,     1,   240,    -1,   282,    -1,   164,
     300,   165,    -1,   192,    -1,   134,   193,    -1,   133,    -1,
     135,    -1,   136,    -1,   134,    -1,   143,    -1,   153,    -1,
     150,    -1,   146,    -1,   149,    -1,   145,    -1,   155,    -1,
     151,    -1,   160,    -1,   139,    -1,   158,    -1,   159,    -1,
     152,    -1,   144,    -1,   132,    -1,    19,    -1,   147,    -1,
      12,    -1,   161,    -1,   148,    -1,    51,    -1,   193,   132,
      -1,   117,    -1,   128,    -1,   125,    -1,   127,    -1,   121,
      -1,   126,    -1,   122,    -1,   118,    -1,   119,    -1,   120,
      -1,   131,    -1,   130,    -1,    -1,   164,   242,   165,    -1,
     164,   242,   165,    -1,    -1,   243,    -1,   242,   138,   243,
      -1,   244,   193,   274,   267,    -1,   176,   244,   193,   274,
     267,    -1,   244,   193,   274,   253,    -1,   244,   164,   266,
     165,   274,   267,    -1,   244,   164,   266,   165,   274,   253,
      -1,    -1,   245,    -1,    45,    -1,    48,    -1,    68,    -1,
      27,    -1,    27,    45,    -1,    27,    82,    -1,    71,    -1,
      82,    -1,   101,    -1,    -1,    71,    -1,    82,    -1,    27,
      82,    -1,    27,    -1,   101,    -1,    76,    -1,    50,    -1,
      -1,    27,    -1,    27,    82,    -1,    82,    -1,    71,    -1,
     101,    -1,    -1,    97,    -1,   157,    -1,   251,    -1,   182,
      -1,   199,    -1,   156,   193,    -1,     4,    -1,   142,    -1,
     142,   300,    -1,   142,   252,    -1,    -1,   110,   300,    -1,
      55,   255,    -1,   110,   300,    55,   255,    -1,    55,   255,
     110,   300,    -1,   256,    -1,   255,   138,   256,    -1,   257,
     117,   257,    -1,   257,   149,   257,    -1,   257,   150,   257,
      -1,   257,   143,   257,    -1,   257,   145,   257,    -1,   257,
     146,   257,    -1,    84,   257,    -1,     3,    -1,    95,    -1,
     101,   259,   157,    -1,    26,   101,   259,   157,    -1,    38,
     101,   259,   157,    -1,   193,   260,    -1,   193,   260,   138,
     259,    -1,    -1,   117,   283,    -1,   117,   271,    -1,    71,
      -1,    27,    -1,    82,    -1,    27,    82,    -1,   107,    -1,
      26,   261,   263,   157,    -1,   261,   263,   157,    -1,   264,
      -1,   263,   138,   264,    -1,   193,   270,   267,    -1,   164,
     266,   165,   270,   267,    -1,   103,    -1,   193,    -1,   164,
     266,   165,    -1,   265,    -1,   265,   138,    -1,   265,   138,
     266,    -1,    -1,   117,    62,    -1,   117,   302,    -1,   166,
     167,   283,    -1,   166,   167,    -1,   166,   275,   167,   283,
      -1,   166,   275,   167,    -1,   166,   167,   268,    -1,   166,
     275,   167,   268,    -1,   166,     1,   167,    -1,    -1,   137,
     283,    -1,   137,   268,    -1,   137,   197,    -1,     1,    -1,
      -1,   137,   283,    -1,   137,   271,    -1,   137,   197,    -1,
       1,    -1,   166,   275,   167,   283,    -1,   166,   275,   167,
     271,    -1,   166,   275,    45,   300,   167,   283,    -1,   166,
       1,   167,    -1,    -1,   283,    -1,   252,    -1,   166,   167,
     272,    -1,   166,   275,   167,   272,    -1,   166,   167,   273,
      -1,   166,   275,   167,   273,    -1,   166,   275,    45,   300,
     167,   272,    -1,    -1,   137,   283,    -1,   137,   252,    -1,
     137,   197,    -1,   137,   273,    -1,   300,    -1,   252,    -1,
     275,   138,   300,    -1,   275,   138,   252,    -1,   300,    -1,
     276,   138,   300,    -1,   103,    -1,   302,    -1,   252,    -1,
     277,   138,   277,    -1,   278,   138,   277,    -1,    -1,   280,
      -1,   281,    -1,   280,   138,   281,    -1,   194,   117,   252,
      -1,   194,   117,   302,    -1,   252,    -1,   302,    -1,   194,
      -1,   196,    -1,   284,    -1,   284,   156,    -1,   156,    -1,
     287,    -1,   303,    -1,   286,    -1,   312,    -1,   311,    -1,
      89,   300,    -1,    46,   164,   279,   165,    -1,    33,   164,
     279,   165,    -1,    92,   164,   279,   165,    -1,    90,    92,
     164,   279,   165,    -1,    14,   300,    -1,    93,   300,    -1,
      70,    -1,    70,   300,    -1,   105,    -1,   105,   300,    -1,
      88,    -1,    88,   300,    -1,    18,    -1,    18,   300,    -1,
      22,    -1,    80,    -1,    35,    -1,    40,   300,    45,   300,
      32,   300,    -1,    40,   300,    45,   210,    32,   300,    -1,
      40,   300,    32,   300,    -1,    40,   300,    45,   300,    32,
      43,   300,    94,   300,    -1,    40,   300,    45,   210,    32,
      43,   300,    94,   300,    -1,    40,   300,    32,    43,   300,
      94,   300,    -1,    41,   300,    45,   300,    32,   300,    -1,
      41,   300,    45,   210,    32,   300,    -1,    41,   300,    32,
     300,    -1,    41,   300,    45,   300,    32,    43,   300,    94,
     300,    -1,    41,   300,    45,   210,    32,    43,   300,    94,
     300,    -1,    41,   300,    32,    43,   300,    94,   300,    -1,
     166,   275,   167,   300,    -1,   166,   275,    45,   300,   167,
     300,    -1,   166,   275,    45,   210,   167,   300,    -1,   166,
     275,    45,   300,   167,    43,   300,    94,   300,    -1,   166,
     275,    45,   210,   167,    43,   300,    94,   300,    -1,    43,
     300,    94,   300,    34,   300,    -1,    61,    -1,   287,    -1,
     282,    -1,   306,    -1,   305,    -1,   229,    -1,   298,    -1,
     299,    -1,   296,   148,   300,    -1,    -1,   290,    -1,   112,
     164,   291,   165,    -1,   294,    -1,   291,   138,   294,    -1,
     112,   164,   293,   165,    -1,   294,    -1,   293,   138,   294,
      -1,   295,   282,   270,   267,    -1,   315,    81,   282,    -1,
     300,    81,   282,    -1,    27,    -1,    45,    -1,    27,    45,
      -1,    82,    -1,    27,    82,    -1,   107,    -1,   303,    -1,
     296,   148,   300,    -1,    60,    -1,    60,    70,    -1,    60,
      88,    -1,    60,   105,    -1,    60,    18,    -1,   297,   300,
      -1,    60,    70,   164,   300,   165,   164,   279,   165,    -1,
      60,    88,   164,   300,   165,   164,   279,   165,    -1,    60,
      70,   164,   300,   165,   164,   279,   165,   156,    -1,    60,
      88,   164,   300,   165,   164,   279,   165,   156,    -1,    60,
      70,   164,   300,   165,    -1,    60,    88,   164,   300,   165,
      -1,    54,   263,    45,   300,    -1,   309,    -1,   283,    -1,
     285,    -1,   313,    -1,   314,    -1,   229,    -1,   298,    -1,
     299,    -1,   164,   142,   300,   165,    -1,   300,   137,   300,
      -1,   300,   141,   300,    -1,   300,   141,    -1,   141,   300,
      -1,   141,    -1,    -1,   300,    -1,    99,   300,    -1,   100,
     300,    -1,   300,    -1,   282,    -1,   305,    -1,   306,    -1,
     307,    -1,   303,    -1,   300,   132,    -1,   284,   156,    -1,
     229,    -1,   304,   164,   279,   165,    -1,   304,   166,   279,
     167,    -1,    74,   164,   279,   165,    -1,   300,   140,   194,
      -1,   300,   140,   101,    -1,   300,   140,    33,    -1,   300,
     140,    57,    -1,   300,   140,    20,   164,   165,    -1,   164,
     277,   165,    -1,   164,   277,   138,   165,    -1,   164,   278,
     165,    -1,   164,   278,   138,   165,    -1,    39,    -1,    98,
      -1,   308,    -1,     5,    -1,     6,    -1,     7,    -1,     8,
      -1,     9,    -1,    10,    -1,    63,    -1,   162,   275,   163,
      -1,   162,   275,   138,   163,    -1,   166,   275,   167,    -1,
     166,   275,   138,   167,    -1,   166,   310,   167,    -1,   166,
     310,   138,   167,    -1,   300,   115,   300,    -1,   310,   138,
     300,   115,   300,    -1,   300,   155,   300,    -1,   300,   151,
     300,    -1,   300,   160,   300,    -1,   300,   139,   300,    -1,
     300,   158,   300,    -1,   300,   159,   300,    -1,   300,   152,
     300,    -1,   300,   143,   300,    -1,   300,   153,   300,    -1,
     300,   150,   300,    -1,   300,   146,   300,    -1,   300,   149,
     300,    -1,   300,   145,   300,    -1,   300,   133,   300,    -1,
     300,   135,   300,    -1,   300,   136,   300,    -1,   300,   116,
     300,    -1,   300,   154,   300,    -1,   300,   144,   300,    -1,
     300,    19,   300,    -1,   300,    12,   300,    -1,   300,   147,
     300,    -1,   300,    31,   300,    -1,   155,   300,    -1,   151,
     300,    -1,    58,   300,    -1,    72,   300,    -1,   132,   300,
      -1,   300,   132,    -1,   134,   300,    -1,   300,    81,   300,
      -1,   300,    81,   210,    -1,   315,    81,   300,    -1,   315,
      81,   210,    -1,   300,    85,   300,    -1,   300,    85,   210,
      -1,   315,    85,   300,    -1,   315,    85,   210,    -1,   155,
      -1,   160,    -1,   116,    -1,   154,    -1,   133,    -1,   135,
      -1,   136,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   496,   496,   501,   502,   508,   509,   514,   515,   520,
     521,   522,   523,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,   539,   540,
     541,   542,   543,   544,   545,   546,   547,   548,   549,   553,
     566,   571,   576,   584,   585,   586,   590,   591,   604,   605,
     606,   611,   612,   617,   622,   627,   631,   638,   643,   647,
     652,   656,   657,   658,   662,   663,   664,   668,   672,   674,
     676,   678,   680,   687,   688,   692,   693,   694,   695,   696,
     697,   700,   701,   702,   703,   704,   705,   717,   718,   729,
     730,   731,   732,   733,   734,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   751,   752,
     753,   754,   755,   756,   757,   758,   759,   760,   761,   768,
     769,   770,   771,   775,   776,   780,   781,   785,   786,   787,
     788,   792,   793,   794,   795,   796,   797,   798,   802,   803,
     804,   805,   810,   809,   825,   824,   841,   840,   856,   855,
     871,   875,   880,   888,   899,   906,   907,   908,   909,   910,
     911,   912,   913,   914,   915,   916,   917,   918,   919,   920,
     921,   922,   923,   924,   930,   936,   942,   948,   955,   962,
     966,   973,   977,   978,   979,   980,   981,   983,   985,   987,
     992,   995,   996,   997,   998,   999,  1000,  1004,  1005,  1009,
    1010,  1011,  1015,  1016,  1020,  1023,  1025,  1030,  1031,  1035,
    1037,  1039,  1046,  1056,  1070,  1075,  1080,  1088,  1089,  1094,
    1095,  1097,  1102,  1118,  1125,  1134,  1142,  1146,  1153,  1154,
    1159,  1164,  1158,  1191,  1194,  1198,  1206,  1216,  1205,  1248,
    1252,  1257,  1261,  1266,  1273,  1274,  1278,  1279,  1280,  1281,
    1282,  1283,  1284,  1285,  1286,  1287,  1288,  1289,  1290,  1291,
    1292,  1293,  1294,  1295,  1296,  1297,  1298,  1299,  1300,  1301,
    1302,  1303,  1304,  1305,  1309,  1310,  1311,  1312,  1313,  1314,
    1315,  1316,  1317,  1318,  1319,  1320,  1324,  1325,  1329,  1333,
    1334,  1335,  1339,  1341,  1343,  1345,  1347,  1352,  1353,  1357,
    1358,  1359,  1360,  1361,  1362,  1363,  1364,  1365,  1369,  1370,
    1371,  1372,  1373,  1374,  1378,  1379,  1383,  1384,  1385,  1386,
    1387,  1388,  1392,  1393,  1396,  1397,  1401,  1402,  1406,  1411,
    1415,  1416,  1417,  1425,  1426,  1428,  1430,  1432,  1437,  1439,
    1444,  1445,  1446,  1447,  1448,  1449,  1450,  1454,  1456,  1461,
    1463,  1465,  1470,  1483,  1500,  1501,  1503,  1508,  1509,  1510,
    1511,  1512,  1516,  1522,  1530,  1531,  1539,  1541,  1546,  1548,
    1550,  1555,  1557,  1559,  1566,  1567,  1568,  1573,  1575,  1577,
    1581,  1585,  1587,  1591,  1599,  1600,  1601,  1602,  1603,  1608,
    1609,  1610,  1611,  1612,  1632,  1636,  1640,  1648,  1655,  1656,
    1657,  1661,  1663,  1669,  1671,  1673,  1678,  1679,  1680,  1681,
    1682,  1688,  1689,  1690,  1691,  1695,  1696,  1700,  1701,  1702,
    1706,  1707,  1711,  1712,  1716,  1717,  1721,  1722,  1723,  1724,
    1728,  1729,  1740,  1742,  1744,  1750,  1751,  1752,  1753,  1754,
    1755,  1757,  1759,  1761,  1763,  1765,  1767,  1770,  1772,  1774,
    1776,  1778,  1780,  1782,  1784,  1787,  1789,  1791,  1796,  1798,
    1800,  1802,  1804,  1806,  1808,  1810,  1812,  1814,  1816,  1818,
    1820,  1827,  1833,  1839,  1845,  1854,  1864,  1872,  1873,  1874,
    1875,  1876,  1877,  1878,  1879,  1884,  1885,  1889,  1893,  1894,
    1898,  1902,  1903,  1907,  1911,  1915,  1922,  1923,  1924,  1925,
    1926,  1927,  1931,  1932,  1937,  1939,  1943,  1947,  1951,  1959,
    1964,  1970,  1976,  1983,  1992,  1994,  1999,  2007,  2008,  2009,
    2010,  2011,  2012,  2013,  2014,  2015,  2017,  2019,  2021,  2023,
    2025,  2030,  2031,  2035,  2036,  2037,  2041,  2042,  2043,  2044,
    2055,  2056,  2057,  2058,  2062,  2063,  2064,  2068,  2069,  2070,
    2071,  2072,  2080,  2081,  2082,  2083,  2087,  2088,  2092,  2093,
    2094,  2095,  2096,  2097,  2098,  2099,  2100,  2101,  2102,  2103,
    2104,  2108,  2116,  2117,  2121,  2122,  2123,  2124,  2125,  2126,
    2127,  2128,  2129,  2130,  2131,  2132,  2133,  2134,  2135,  2136,
    2137,  2138,  2139,  2140,  2141,  2142,  2143,  2147,  2148,  2149,
    2150,  2151,  2152,  2153,  2157,  2158,  2159,  2160,  2164,  2165,
    2166,  2167,  2172,  2173,  2174,  2175,  2176,  2177,  2178
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
     200,   201,   201,   201,   201,   201,   201,   201,   202,   202,
     202,   202,   204,   203,   205,   203,   206,   203,   207,   203,
     203,   203,   203,   203,   208,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   210,   211,   211,   211,   211,   211,   211,   211,   211,
     212,   213,   213,   213,   213,   213,   213,   214,   214,   215,
     215,   215,   216,   216,   217,   218,   218,   219,   219,   220,
     220,   220,   221,   221,   222,   222,   222,   223,   223,   224,
     224,   224,   225,   225,   226,   227,   227,   227,   228,   228,
     230,   231,   229,   232,   232,   232,   234,   235,   233,   236,
     236,   236,   236,   236,   237,   237,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   240,   240,   241,   242,
     242,   242,   243,   243,   243,   243,   243,   244,   244,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   246,   246,
     246,   246,   246,   246,   247,   247,   248,   248,   248,   248,
     248,   248,   249,   249,   250,   250,   251,   251,   252,   252,
     253,   253,   253,   254,   254,   254,   254,   254,   255,   255,
     256,   256,   256,   256,   256,   256,   256,   257,   257,   258,
     258,   258,   259,   259,   260,   260,   260,   261,   261,   261,
     261,   261,   262,   262,   263,   263,   264,   264,   265,   265,
     265,   266,   266,   266,   267,   267,   267,   268,   268,   268,
     268,   268,   268,   268,   269,   269,   269,   269,   269,   270,
     270,   270,   270,   270,   271,   271,   271,   271,   272,   272,
     272,   273,   273,   273,   273,   273,   274,   274,   274,   274,
     274,   275,   275,   275,   275,   276,   276,   277,   277,   277,
     278,   278,   279,   279,   280,   280,   281,   281,   281,   281,
     282,   282,   283,   283,   283,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   286,   287,   288,   288,   288,
     288,   288,   288,   288,   288,   289,   289,   290,   291,   291,
     292,   293,   293,   294,   294,   294,   295,   295,   295,   295,
     295,   295,   296,   296,   297,   297,   297,   297,   297,   298,
     298,   298,   298,   298,   298,   298,   299,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   301,   301,   302,   302,   302,   303,   303,   303,   303,
     304,   304,   304,   304,   305,   305,   305,   306,   306,   306,
     306,   306,   307,   307,   307,   307,   308,   308,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   310,   310,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   312,   312,   312,
     312,   312,   312,   312,   313,   313,   313,   313,   314,   314,
     314,   314,   315,   315,   315,   315,   315,   315,   315
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
       1,     2,     2,     0,     2,     2,     4,     4,     1,     3,
       3,     3,     3,     3,     3,     3,     2,     1,     1,     3,
       4,     4,     2,     4,     0,     2,     2,     1,     1,     1,
       2,     1,     4,     3,     1,     3,     3,     5,     1,     1,
       3,     1,     2,     3,     0,     2,     2,     3,     2,     4,
       3,     3,     4,     3,     0,     2,     2,     2,     1,     0,
       2,     2,     2,     1,     4,     4,     6,     3,     0,     1,
       1,     3,     4,     3,     4,     6,     0,     2,     2,     2,
       2,     1,     1,     3,     3,     1,     3,     1,     1,     1,
       3,     3,     0,     1,     1,     3,     3,     3,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       2,     4,     4,     4,     5,     2,     2,     1,     2,     1,
       2,     1,     2,     1,     2,     1,     1,     1,     6,     6,
       4,     9,     9,     7,     6,     6,     4,     9,     9,     7,
       4,     6,     6,     9,     9,     6,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     0,     1,     4,     1,     3,
       4,     1,     3,     4,     3,     3,     1,     1,     2,     1,
       2,     1,     1,     3,     1,     2,     2,     2,     2,     2,
       8,     8,     9,     9,     5,     5,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     3,     3,     2,     2,
       1,     0,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     2,     2,     1,     4,     4,     4,     3,     3,     3,
       3,     5,     3,     4,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     4,     3,     4,
       3,     4,     3,     5,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    87,   559,   560,   561,   562,
     563,   564,     0,   485,    73,   108,   453,   114,   455,   485,
       0,   113,     0,   358,    73,     0,     0,     0,     0,   457,
     531,   531,   556,     0,     0,     0,     0,   112,     0,   234,
     109,     0,   230,     0,     0,   116,     0,   504,   476,   565,
     117,     0,   235,   447,   357,     0,     0,     0,    45,    44,
     111,   456,   359,     0,     0,     0,     0,   451,     0,     0,
     115,     0,     0,    88,     0,   557,     0,     0,     0,   110,
     216,   449,   361,   118,     0,     0,   614,     0,   616,     0,
     617,   618,   530,     0,   615,   612,   434,   127,   613,     0,
       0,     0,     4,     0,     5,     0,     9,    46,    10,     0,
      11,    12,    14,   430,   431,    22,    13,   128,   136,   137,
      15,    17,    16,    19,    20,    21,    18,   135,     0,   133,
       0,   522,     0,   131,   134,     0,   132,   536,   518,   432,
     519,   437,   435,     0,     0,     0,   523,   524,     0,   436,
       0,   537,   538,   539,   558,   517,   439,   438,   520,   521,
       0,    38,    24,   445,     0,     0,   486,    74,     0,     0,
     455,   457,     0,     0,     0,   456,     0,     0,     0,   522,
     536,   435,   523,   524,   454,   436,   537,   538,     0,   485,
       0,     0,   360,     0,   190,     0,   415,     0,   422,   562,
     456,   532,   233,   562,   154,   456,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   141,     0,     0,   422,    81,
      89,   101,    95,    94,   103,    84,    93,   104,    90,   105,
      82,   106,    99,    92,   100,    98,    96,    97,    83,    85,
      91,   102,   107,     0,    86,     0,     0,     0,     0,   364,
       0,   124,    32,     0,   599,   508,   505,   506,   507,     0,
     448,   600,     7,   422,   214,   224,   531,   215,   130,   129,
     329,   434,   412,     0,   411,     0,     0,   125,   535,     0,
       0,    35,     0,   452,   440,     0,   422,    36,   446,     0,
     197,   193,     0,   436,   197,   194,     0,   354,     0,   450,
       0,     0,   601,   603,   529,   598,   597,     0,    48,    51,
       0,     0,   417,     0,   419,     0,     0,   418,     0,     0,
     411,     0,     0,     6,     0,    47,     0,     0,   217,     0,
     315,   314,   236,     0,   433,    23,     0,   509,     0,     0,
       0,     0,     0,     0,   602,     0,     0,     0,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   274,   281,   282,
     283,   278,   280,     0,     0,   276,   279,   277,   275,     0,
     285,   284,     0,     0,   422,   422,     0,     0,     0,    25,
      26,     0,     0,     0,     0,     0,    27,     0,     0,     0,
       0,    28,     0,    29,     0,   430,   428,     0,   423,   424,
     429,   148,     0,   151,     0,   144,     0,     0,   150,     0,
       0,     0,   163,     0,     0,   162,     0,   171,     0,     0,
       0,   169,     0,     0,    57,   138,     0,   183,     0,     0,
      30,   297,   231,   368,     0,   369,   371,     0,   393,     0,
     374,     0,     0,   123,    31,     0,     0,    33,     0,   328,
       0,    67,   533,   534,   126,     0,    34,   422,     0,   204,
     195,   191,   196,   192,     0,   352,   349,   156,    37,    50,
      49,    52,     0,   566,     0,     0,   552,     0,   554,     0,
       0,     0,     0,     0,     0,     0,     0,   570,     8,     0,
      40,     0,     0,     0,   411,     0,     0,     0,   308,   363,
     484,   594,   593,   596,   605,   604,   609,   608,   590,   587,
     588,   589,   526,   577,     0,   549,   550,   548,   547,   527,
     581,   592,   586,   584,   595,   585,   583,   575,   580,   582,
     591,   574,   578,   579,   576,     0,     0,     0,     0,     0,
       0,     0,   607,   606,   611,   610,   496,   497,   499,   501,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   568,   485,   485,   159,   350,   362,   416,     0,     0,
     442,     0,     0,   217,     0,     0,   217,   351,     0,     0,
       0,     0,   460,     0,     0,     0,   172,     0,   466,     0,
       0,   170,   613,    60,     0,    53,    58,     0,   182,     0,
       0,     0,   441,   302,   299,   300,   301,   305,   306,   307,
     297,     0,   290,     0,   298,   316,     0,   372,     0,   121,
     122,   120,   119,     0,   392,   391,   518,     0,   366,   516,
     365,     0,     0,   546,   414,   413,     0,     0,     0,   443,
       0,   198,   356,   518,     0,   567,   525,   553,   420,   555,
     421,   179,     0,     0,     0,   569,   177,   470,     0,   572,
     571,     0,    42,    41,    39,    64,     0,    57,   218,     0,
       0,   228,     0,   225,   312,   309,   310,   313,   237,     0,
       0,    71,    72,    70,    69,    68,   544,   545,   498,   500,
       0,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   155,   426,   427,   425,   217,
       0,   153,   217,     0,   152,     0,   181,     0,     0,   161,
       0,   160,     0,   491,     0,     0,   167,     0,     0,   165,
       0,     0,   139,     0,   140,     0,     0,   185,     0,   187,
     303,   304,     0,   297,   288,     0,   406,   317,   320,   319,
     321,     0,   370,   373,   374,     0,     0,   375,   376,   514,
     515,   206,     0,     0,   205,   208,   444,     0,   199,   202,
       0,   353,   180,     0,     0,     0,     0,   178,     0,     0,
       0,     0,   233,   223,     0,   226,   222,   311,   316,   286,
      75,   269,    89,   267,    95,    94,    78,    93,    90,   272,
     105,    76,   106,    92,    96,    77,    79,    91,   107,   266,
     248,   251,   249,   250,   261,   252,   265,   257,   255,   268,
     271,   256,   254,   259,   264,   253,   258,   262,   263,   260,
     270,     0,   246,     0,    80,     0,   286,   286,   244,   551,
     489,   374,   536,   536,     0,     0,     0,     0,     0,     0,
       0,     0,   158,   157,     0,   219,     0,   219,   164,     0,
       0,   459,     0,   458,     0,   490,     0,     0,   465,   168,
       0,   464,   166,    55,    54,   184,   475,   186,     0,   406,
     291,     0,     0,   374,   318,   333,   367,   397,     0,   568,
     422,   422,     0,   210,     0,     0,     0,   200,     0,   175,
     472,     0,     0,   173,   471,     0,   573,    65,    66,   213,
     233,   233,   212,   233,   220,   229,   227,     0,   297,   243,
     247,     0,   273,     0,   239,   240,   493,     0,     0,     0,
       0,     0,     0,     0,     0,   219,   233,   219,   233,   463,
       0,     0,   492,   469,     0,     0,     0,     0,   189,   374,
     406,     0,   409,   408,   410,   518,   330,   294,   292,     0,
       0,     0,     0,   395,   518,     0,     0,   211,   209,     0,
     203,     0,   176,     0,   174,   221,   388,     0,   322,     0,
     245,    75,    77,   286,   286,     0,     0,     0,     0,     0,
       0,   233,   147,   233,   143,     0,     0,     0,     0,    56,
     188,   293,   374,   398,     0,   332,   331,   347,     0,   348,
     335,   338,     0,   334,   326,   327,   232,     0,   510,   511,
     201,     0,     0,     0,   387,   386,   518,   323,   333,   287,
     241,   242,     0,     0,     0,     0,     0,     0,   149,   145,
     462,   461,   468,   467,   296,   295,   400,   401,   403,   518,
       0,   568,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   518,   512,   513,   474,   473,     0,   378,     0,
       0,     0,   402,   404,   337,   339,   340,   343,   344,   345,
     341,   342,   336,   383,   381,   518,   568,   324,   238,   325,
     398,   382,   518,   405
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   309,   103,   608,   105,   106,   107,   326,
     108,   310,   603,   607,   604,   109,   110,   111,   112,   168,
     842,   247,   113,   244,   114,   634,   252,   115,   116,   117,
     118,   119,   416,   585,   412,   582,   120,   121,   711,   122,
     123,   124,   470,   651,   780,   125,   126,   647,   775,   127,
     128,   506,   792,   129,   130,   682,   683,   179,   245,   625,
     132,   133,   508,   798,   688,   845,   846,   438,   929,   442,
     621,   622,   623,   624,   689,   332,   761,  1038,  1098,  1026,
     272,   967,   971,  1020,  1021,  1022,   134,   298,   475,   135,
     136,   248,   249,   446,   447,   638,  1035,   988,   450,   635,
    1057,   964,   893,   311,   195,   315,   316,   407,   408,   409,
     180,   138,   139,   140,   141,   181,   143,   165,   166,   560,
     428,   732,   561,   562,   144,   145,   182,   183,   148,   207,
     410,   185,   150,   186,   187,   153,   154,   155,   321,   156,
     157,   158,   159,   160
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -980
static const yytype_int16 yypact[] =
{
    -980,   190,  3167,  -980,   -28,  -980,  -980,  -980,  -980,  -980,
    -980,  -980,  4475,    30,   217,  -980, 13194,  -980, 16386,    30,
   10144,  -980,   218,   144,   217,  4475, 10144,  4475,    78, 17717,
   10306,  8062,  -980,  8224,  9002,  6756, 10144,  -980,   124,  -980,
    -980, 17774,  -980,  2387,  9164,  -980, 10144,   197,  -980,  -980,
    -980, 10144,  -980, 13194,  -980, 10144,   297,   195,  1361,  1636,
    -980, 17831,  -980,  9328,  7574, 10144,  9164, 13194, 10144,   216,
    -980,   286,  4475,  -980, 10144,  -980, 10468, 10468, 17774,  -980,
    -980, 13194,  -980,  -980, 10144, 10144,  -980, 10144,  -980, 10144,
    -980,  -980, 12736, 10144,  -980, 10144,  -980,  -980,  -980,  3493,
    6920,  8388,  -980,  4313,  -980,   159,  -980,   333,  -980,   358,
    -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980,
    -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980, 17774,  -980,
   17774,   351,   307,  -980,  -980,  2387,  -980,   256,  -980,   315,
    -980,  -980,   317,   338,   337, 10144,   341,   356, 17260,  1659,
     285,   374,   393,  -980,  -980,  -980,  -980,  -980,  -980,  -980,
     191,  -980,  -980, 17260,   326,  4475,  -980,  -980,   403, 10144,
    -980,  -980, 10144, 10144, 10144,  -980, 10144,  9328,  9328,   361,
    -980,  -980,  -980,  -980,   178,   366,  -980,  -980,   400, 15233,
   17774,  2387,  -980,   421,  -980,   -29, 17260,   407,  7738,   492,
   17888, 17260,   441,   508,  -980, 17945, 17774,   441, 17774,   425,
       6, 14767,    15, 14735,   284,  -980, 15176, 13983,  7738,  -980,
    -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980,
    -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980,
    -980,  -980,  -980,  4475,  -980,   428,  1478,    91,    58,  -980,
    4475,  -980,  -980, 14911,   490,  -980,   429,   431,  -980, 14911,
     178,   490,  -980,  7738,  -980,  -980,  9490,  -980,  -980,  -980,
    -980, 17774,  -980,    84, 17260, 10144, 10144,  -980, 17260,   446,
   15144,  -980, 14911,   178, 17260,   440,  7738,  -980, 17260, 15320,
    -980,  -980, 15585,  2493,  -980,  -980, 15634,   488,   453,   178,
   14911, 15729,   530,   530, 12888,   490,   490,   257,  -980,  -980,
    3657,    50,  -980, 10144,  -980,   -39,    67,  -980,   -14,     3,
   15778,   -60,   600,  -980,  3821,  -980,   554,  9328,   479,   455,
    -980,  -980,  -980,   222,   381,  -980, 10144,   486, 10144, 10144,
   10144,  9002,  9002, 10144,   385, 10144, 10144, 10144, 10144, 10144,
     263, 12736, 10144, 10144, 10144, 10144, 10144, 10144, 10144, 10144,
   10144, 10144, 10144, 10144, 10144, 10144, 10144,  -980,  -980,  -980,
    -980,  -980,  -980,  8550,  8550,  -980,  -980,  -980,  -980,  8550,
    -980,  -980,  8550,  8550,  7738,  7738,  9002,  9002,  7412,  -980,
    -980, 15377, 15409, 15810,    63,  3985,  -980,  9002,     6,   474,
     231,  -980, 10144,  -980, 10144,   515,  -980,   468,   498,  -980,
    -980,  -980, 17774,  -980,  2387,  -980, 17774,   480,  -980,  2387,
     593,  9328,  -980,  4637,  9002,  -980,   476,  -980,     6,  4799,
    9002,  -980,     6, 10630, 10144,  -980,  4475,   607, 10144,   478,
    -980,    66,  -980,  -980,  1478,  -980,   506,   481,  -980, 10792,
     531, 10144,  2387,  -980,  -980, 10144, 10144,  -980,   487,  -980,
    9328,  -980, 17260, 17260,  -980,    35,  -980,  7738,   496,  -980,
     630,  -980,   630,  -980, 10954,   528,  -980,  -980,  -980,  -980,
    -980,  -980,  8714,  -980, 13752,  7084,  -980,  7248,  -980,  4475,
     502,  9002,  6261,  3329,   504, 10144,  6591,  -980,  -980,   309,
    -980,  4149, 17774,   242, 15553,  9328,   510, 17660,   167,  -980,
   15954, 17500, 17500,   436,  -980,   436,  -980,   436, 13649,   154,
     301,   823,   178,   530,   513,  -980,  -980,  -980,  -980, 12888,
   16773,   436,  2654,  2654, 17500,  2654,  2654,   543,   530, 16773,
    2423,   543,   490,   490,   530,   527,   532,   533,   537,   547,
     535,   538,  -980,   436,  -980,   436,    61,  -980,  -980,  -980,
      85,  -980,  1289, 17336,   382, 11116,  9002, 11278,  9002, 10144,
    9002, 13454,    30, 15999,  -980,  -980,  -980, 17260, 16040,  7738,
    -980,  7738, 17774,   479,   246, 17774,   479,  -980,   249, 10144,
     108, 10144, 17260,    38, 14968,  7412,  -980, 10144, 17260,    18,
   14824,  -980,   549,   569,   553, 16151,   569,   558,   677, 16191,
    4475, 15000,  -980,   129,  -980,  -980,  -980,  -980,  -980,  -980,
     451,   114,  -980,  2635,  -980,   415,   552,  1478,    91,    78,
     124, 10144, 10144,  6097,  -980,  -980,   713,  7900,  -980, 17260,
    -980, 13793, 13839,  -980,  -980, 17260,   556,   -18,   557,  -980,
    1631,  -980,  -980,   278, 17774,  -980,  -980,  -980,  -980,  -980,
    -980,  -980,  4475,    40,  2688,  -980,  -980, 17260,  4475, 17260,
    -980, 16232,  -980,  -980,  -980,  -980, 10630, 10144,   583,   727,
     561,   610,    95,  -980,   651,  -980,  -980,  -980,  -980,  8842,
     570,  -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980,
    7412,  -980,    34,  9002,  9002, 10144,   705, 16376, 10144,   706,
   16408,   572,  2976,     6,     6,  -980,  -980,  -980,  -980,   479,
     578,  -980,   479,   579,  -980, 14911,  -980, 14059,  4961,  -980,
    5123,  -980,   139,  -980, 14135,  5285,  -980,     6,  5447,  -980,
       6, 10144,  -980, 10144,  -980,  4475, 10144,  -980,  4475,   708,
    -980,  -980, 17774,   868,  -980,  1478,   606,   662,  -980,  -980,
    -980,    65,  -980,  -980,   531,   581,    80,  -980,  -980,   586,
     588,  -980,  5609,  9328,  -980,  -980,  -980, 17774,  -980,   608,
     400,  -980,  -980,  5771,   589,  5933,   590,  -980, 10144,   598,
     602,   597,  1120,  -980, 10144, 17774,  -980,  -980,   415,   599,
     268,  -980,   626,  -980,   627,   632,   614,   635,   638,  -980,
     639,   648,   643,   647,   649,   283,   658,   652,   659,  -980,
    -980, 17774,  -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980,
    -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980,
    -980, 10144,  -980,   669,   672,   666,   599,   599,  -980,  -980,
    -980,   531,   142,   157, 16552, 11440, 11602, 16584, 11764, 11926,
   12088, 12250,  -980,  -980,   629,  -980,   646,  -980,  -980,  4475,
   10144, 17260, 10144, 17260,  7412,  -980,  4475, 10144, 17260,  -980,
   10144, 17260,  -980, 16641, 17260,  -980, 17260,   736,  4475,   606,
    -980,   654,  9654,   239,  -980,   161,  -980,  -980,  9002, 13324,
    7738,  7738,  4475,  -980,    25,   656, 10144,  -980, 10144,  -980,
   17260,  4475, 10144,  -980, 17260,  4475, 17260,  -980,  -980,  -980,
     975,   975,  -980,  2835,  -980, 17260,  -980,   100,    66,  -980,
    -980, 13896,  -980, 13551,  -980,  -980,  -980, 10144, 10144, 10144,
   10144, 10144, 10144, 10144, 10144,  -980,  2161,  -980, 15965, 16191,
   14211, 14287,  -980, 16191, 14363, 14439, 10144,  4475,  -980,   531,
     606,  6426,  -980,  -980,  -980,   -75,  9328,  -980,  -980,   186,
   10144,    -8, 13607,  -980,  1061,   657,   661,   407,  -980,   400,
   17260, 14515,  -980, 14591,  -980,  -980,  -980, 12412,   714,   200,
    -980,   680,   684,   599,   599, 16728, 16804, 16880, 16956, 17032,
   17108, 17471,  -980, 17485,  -980,  4475,  4475,  4475,  4475, 17260,
    -980,  -980,   239,  9818,    86,  -980, 17260,  -980,    43,  -980,
      -6,  -980,   394, 17184,  -980,  -980,  -980, 12250,   668,   671,
    -980,  4475,  4475,  2940,  -980,  -980,   206,  -980,   161,  -980,
    -980,  -980, 10144, 10144, 10144, 10144, 10144, 10144,  -980,  -980,
   16191, 16191, 16191, 16191,  -980,  -980,  -980,  -980,  -980,   260,
    9002, 12900,  -980, 10144,   186,    43,    43,    43,    43,    43,
      43,   186,  1084,  -980,  -980, 16191, 16191,   650, 12574,   102,
     -57, 13695,  -980,  -980, 17260,  -980,  -980,  -980,  -980,  -980,
    -980,  -980,   695,  -980,  -980,   372, 13064,  -980,  -980,  -980,
    9982,  -980,   563,  -980
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -980,  -980,  -980,    -1,  -346,  1861,  -980,  -980,  -980,  -980,
     353,   511,  -417,   160,   162,  -980,  -980,  -980,   179,   812,
    -980,  1911,  -192,  -674,  -980,  -818,   386,  -940,  -767,   -49,
    -980,  -980,  -980,  -980,  -980,  -980,  -980,  -980,    90,  -980,
    -980,  -980,   545,  -980,    64,  -980,  -980,  -980,  -980,  -980,
    -980,  -546,  -648,  -980,  -980,  -980,    45,  1107,  -980,  -980,
    -980,   287,  -980,  -980,  -980,  -980,   -91,  -147,  -750,  -980,
     -83,    97,   232,  -980,  -980,  -980,    55,  -980,  -980,  -225,
     328,  -156,  -179,  -210,  -201,  -564,  -980,  -162,  -980,     7,
     831,  -132,   417,  -980,  -432,  -734,  -631,  -980,  -583,  -463,
    -979,  -945,  -810,   -43,  -980,    51,  -980,  -202,  -980,   292,
     582,  -364,  -980,  -980,  -980,  1136,  -980,    14,  -980,  -980,
    -206,  -980,  -582,  -980,  -980,  -980,  1335,  1417,   -12,   837,
      22,  1025,  -980,  1860,  2148,  -980,  -980,  -980,  -980,  -980,
    -980,  -980,  -980,  -383
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -569
static const yytype_int16 yytable[] =
{
     163,   102,   383,   333,   184,   564,   405,   432,   189,   268,
     269,   652,   626,   733,   196,   844,   439,   606,   201,   201,
     273,   211,   213,   216,   217,   924,   405,    64,   399,   191,
     896,  1025,   253,   188,   254,   448,   646,   720,   250,   259,
     723,   260,  -407,   261,   417,   764,  1017,   250,   491,   772,
     735,   274,   278,   280,   282,   283,   284,   250,   319,   400,
     288,   458,   289,  -407,   292,   296,   448,  -407,  1058,   299,
     728,   405,   300,   301,   962,   302,    64,   303,   496,   959,
     304,   305,  1082,   306,   468,   636,   279,   274,   278,   320,
    -407,   773,   448,   613,   405,   620,   934,   935,   426,   485,
    1097,   986,  -207,   451,  1063,   395,   698,   497,   570,   402,
     653,   614,   490,   494,   615,   426,  1083,   936,   850,   851,
    -389,  1103,   317,   210,   212,   898,   486,   426,   403,   161,
     426,  1060,  1064,   337,   616,   394,  -389,   617,  1019,    56,
    1025,   492,   164,   699,  -207,   774,   383,   570,   618,  -389,
    1012,  -389,   426,   489,   395,  -384,   985,   163,   528,   968,
     391,   392,   393,   460,   288,   274,   320,   619,   395,  1034,
     493,   449,  -389,   864,   750,  -389,   866,   395,   895,   924,
     395,   924,   550,   551,  -384,   340,   278,   395,   482,  1017,
       3,   318,   405,   405,   684,   763,   452,  -384,  -207,  -389,
     395,   492,   449,   398,  -289,   487,   278,   783,  -389,   414,
    -384,   751,   564,   483,   419,   255,   969,   946,   492,   948,
     167,   191,   460,   700,   492,  1011,   192,  -389,   449,  -389,
     571,  -289,   488,   795,   924,   341,   924,   987,   685,   342,
     492,   461,   198,  1040,  1041,    23,   460,   899,  -389,   686,
     701,   278,   753,  1061,   201,   291,   295,  -384,   796,   844,
     606,  -385,  -384,   462,   463,   648,     5,   256,   687,  1096,
    1018,   970,   386,   726,   278,   405,   387,   874,  1055,   754,
    -495,  1019,   584,   524,   503,   257,   344,   588,   218,    54,
    -385,   348,   952,   349,   350,  -494,   525,  1001,   353,  1003,
      62,   484,   258,  -385,   875,   262,   360,  -495,   285,   481,
     344,    23,   364,   365,   366,   504,  -385,   564,   350,   190,
     526,   324,  -494,   891,   510,    82,   511,   512,   513,   515,
     517,   518,   340,   519,   520,   521,   522,   523,   753,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
     540,   541,   542,   543,   544,    54,   637,   330,    73,   263,
     452,   278,   278,  -385,   527,  1039,    62,   278,  -385,   452,
     278,   278,   278,   278,   553,   555,   563,  -399,   590,   509,
     460,   966,   341,   331,   452,   573,   342,   452,   576,   405,
     577,    82,   578,   737,   740,   545,   546,   251,  -399,   675,
     -81,   547,  -399,   721,   548,   549,   724,   620,   -87,   274,
     325,   592,   594,  -478,   161,   -83,  -355,   598,   600,   251,
     479,   605,   605,   -88,   609,  -399,   611,  -377,   314,   290,
     294,   514,   516,   344,   345,  -355,   973,   347,   348,   639,
     349,   350,   757,   641,   642,   353,   923,  1094,   645,   384,
     286,   385,   281,   360,  1062,   278,  -377,   784,   786,   364,
     365,   366,   678,   704,   327,  1101,   161,   387,    23,  -377,
     645,   334,   672,   278,  -477,   278,   552,   554,   613,   664,
     645,   667,  -377,   669,   671,   336,   758,   572,    39,   413,
     388,   564,   781,   274,   418,   335,   614,   759,  -482,   615,
     481,  1086,  1087,  1088,  1089,  1090,  1091,   317,  -481,   317,
      52,  1065,    54,  -483,   593,  -543,   760,  -543,   404,   616,
     599,   340,   617,    62,   322,  -543,   406,  -543,   965,  -377,
    -540,  -480,  -540,   618,  -377,   974,   658,  1066,   660,  1067,
    1068,   396,   847,  1069,  1070,  -542,   406,  -542,    82,  -541,
    -479,  -541,   619,   592,   707,   598,   710,   609,   712,   667,
     390,   340,   395,   251,   251,   251,   251,   278,   344,   278,
     437,   341,   411,   348,   340,   342,   350,   725,   401,   727,
     353,   663,   620,   563,   137,   734,   713,   714,   415,   421,
     766,   406,   441,   455,   137,   456,   422,   425,   427,   431,
     923,   717,   923,   464,   467,   474,   251,   137,   498,   137,
     476,   341,   251,   502,   406,   342,   505,   507,  -379,   284,
     288,   320,   344,  1036,   341,   278,   350,   348,   342,   349,
     350,   575,   579,   580,   353,   251,   581,   587,   589,   454,
     595,   610,   360,   612,   627,   457,   628,  -379,   637,  1059,
     366,   650,   643,   251,   137,   923,   706,   923,   709,   768,
    -379,   649,   344,  1072,   605,   605,   654,   348,   466,   662,
     350,   668,   679,  -379,   353,   344,   345,   690,   346,   347,
     348,   137,   349,   350,   691,   137,   477,   353,   563,   692,
     693,   515,   553,   854,   694,   360,   857,  1059,   975,   976,
     696,   364,   365,   366,   695,   697,   -59,   741,   405,   405,
     742,   745,   406,   406,  1095,   744,   871,   762,   873,   771,
    -379,   460,   776,   878,   793,  -379,   881,   794,   791,   883,
     904,   884,  1102,   797,   886,   849,  1059,   855,   858,   860,
     865,   867,   888,   892,   894,   906,   -84,   137,   897,  -219,
     900,   251,   901,  -219,  -219,   917,   911,   915,  -390,   918,
     919,   274,  -219,   928,  -219,  -219,  -108,  -114,  -390,  -219,
     957,   910,  -113,   914,  -219,  -112,   916,  -219,  -109,  -116,
     -82,   251,   925,  -117,   574,   251,   994,  -111,   644,  -115,
     -85,   945,  -110,   514,   552,   406,  -219,  -390,  -219,  -118,
    -219,   932,  -219,  -219,   -86,  -219,   933,  -219,   947,  -219,
     644,  1037,   -81,   314,   596,   314,   -83,  1093,   601,   960,
     644,   979,  1028,  -390,  1073,   137,  1029,  1074,  -219,   931,
    -390,  -219,   137,  1064,  -219,   501,   193,   790,   789,   472,
     926,   905,   993,   871,   873,   989,   878,   881,   910,   914,
     890,  -390,   752,   927,   340,  1099,  1054,   949,   950,  1080,
     951,  1092,   563,  1085,   953,   954,   215,   202,   955,   640,
    -390,   268,   269,   718,     0,  -390,     0,     0,  -390,     0,
       0,     0,     0,     0,  -219,     0,   972,   667,   278,   278,
    -219,     0,   137,     0,   980,   613,   981,     0,     0,     0,
     983,     0,     0,     0,   341,     0,   137,   716,   342,   406,
       0,     0,     0,   614,     0,     0,   615,     0,  1014,     0,
       0,     0,     0,     0,     0,   949,   995,   996,   953,   997,
     998,   999,  1000,     0,     0,     0,   616,     0,     0,   617,
       0,    56,     0,     0,  1009,     0,   251,   251,     0,   320,
     618,     0,   251,   251,  1016,   344,   345,     0,  1023,     0,
     348,     0,   349,   350,   749,     0,     0,   353,     0,   619,
       0,     0,     0,     0,     0,   360,     0,   137,     0,   729,
     731,   364,   365,   366,     0,   736,   739,     0,     0,     0,
    1079,     0,     0,  1050,  1051,  1052,  1053,   264,     0,     0,
       0,    22,    23,   778,     0,   137,     0,     0,     0,     0,
     265,   137,    30,   266,     0,   914,     0,    35,   137,  1075,
    1076,   320,    39,     0,     0,     0,     0,   149,     0,     0,
    1050,  1051,  1052,  1053,  1075,  1076,     0,   149,     0,     0,
       0,     0,     0,     0,    52,     0,    54,     0,  1081,   667,
     149,  1084,   149,     0,     0,   267,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,   251,   251,     0,     0,
       0,   137,     0,     0,     0,   137,    78,     0,   251,    80,
     437,     0,    82,   137,   667,     0,     0,   437,   914,     0,
     251,     0,     0,   251,     0,     0,     0,   149,     0,   862,
     863,   293,   293,     0,     0,     0,  -394,     0,     0,   131,
       0,   868,     0,     0,     0,     0,  -394,     0,     0,   131,
       0,     0,     0,   879,   149,     0,   882,     0,   149,  -396,
       0,     0,   131,   907,   131,     0,     0,     0,   142,  -396,
       0,     0,   264,     0,   702,  -394,    22,    23,   142,     0,
       0,     0,     0,     0,     0,   265,     0,    30,   266,     0,
       0,   142,    35,   142,     0,     0,     0,    39,  -396,     0,
       0,  -394,     0,     0,     0,     0,     0,     0,  -394,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
     149,    54,   137,    56,  -396,   920,     0,     0,   921,  -394,
     267,  -396,    62,     0,     0,     0,   131,     0,   142,     0,
     131,     0,     0,     0,     0,     0,     0,     0,  -394,     0,
     963,    78,  -396,  -394,    80,     0,  -394,    82,   406,   406,
       0,     0,     0,     0,     0,   142,     0,     0,     0,   142,
       0,  -396,     0,     0,   137,     0,  -396,     0,     0,  -396,
     137,     0,     0,     0,     0,     0,     0,   251,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   149,     0,
       0,   848,   131,     0,     0,   149,     0,    97,     0,     0,
       0,     0,     0,   922,     0,   852,   853,     0,     0,     0,
     978,     0,     5,     0,  1015,     0,     0,     0,     0,     0,
       0,   142,     0,   437,   437,     0,    15,   437,   437,    17,
     137,     0,   137,     0,    21,     0,     0,   137,     0,     0,
     137,     0,     0,     0,  1024,     0,     0,   137,     0,     0,
     137,     0,  1030,    37,   437,   149,   437,   146,    40,     0,
       0,  1056,     0,     0,     0,     0,    45,   146,     0,   149,
     131,     0,     0,    50,   137,     0,     0,   131,     0,     0,
     146,     0,   146,     0,     0,   137,     0,   137,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   142,
      70,     0,     0,   264,    73,     0,   142,    22,    23,  1056,
       0,    79,     0,     0,     0,     0,   265,    83,    30,   266,
       0,     0,     0,    35,     0,     0,     0,   146,    39,     0,
       0,  -233,     0,     0,     0,     0,     0,   131,     0,   147,
     149,     0,     0,     0,     0,     0,     0,     0,  1056,   147,
      52,   131,    54,  1024,   146,     0,     0,  -233,   146,     0,
       0,   267,   147,    62,   147,     0,   142,     0,   149,     0,
       0,   137,     0,     0,   149,     0,     0,     0,   137,     0,
     142,   149,    78,     0,     0,    80,     0,   -63,    82,     0,
     137,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   219,     0,     0,   137,     0,     0,     0,     0,   147,
       0,     0,     0,   137,     0,   220,   221,   137,   222,     0,
     146,     0,   131,   223,     0,     0,     0,     0,     0,     0,
       0,   224,     0,     0,   149,     0,   147,   225,   149,     0,
     147,     0,   226,     0,   227,     0,   149,   228,     0,     0,
     131,   142,     0,     0,     0,   229,   131,     0,     0,   137,
       0,   230,   231,   131,     0,     0,     0,     0,   232,     0,
       0,     0,     0,     0,     0,     0,     0,   233,     0,   142,
       0,     0,     0,     0,     0,   142,   234,   235,     0,   236,
       0,   237,   142,   238,     0,     0,   239,     0,   146,     0,
     240,   443,   147,   241,     0,   146,   242,   137,   137,   137,
     137,     0,     0,     0,     0,     0,   131,     0,     0,     0,
     131,     0,     0,     0,     0,     0,     0,     0,   131,     0,
       0,     0,     0,   137,   137,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   142,     0,     0,     0,   142,
       0,     0,     0,     0,   219,   149,     0,   142,     0,     0,
       0,     0,   444,     0,     0,   146,     0,     0,   220,   221,
       0,   222,     0,     0,     0,     0,   223,     0,   264,   146,
     147,     0,    22,    23,   224,     0,     0,   147,     0,     0,
     225,   265,     0,    30,   266,   226,     0,   227,    35,     0,
     228,     0,     0,    39,     0,     0,  -233,   149,   229,     0,
       0,     0,     0,   149,   230,   231,     0,     0,     0,     0,
       0,   232,     0,     0,     0,    52,     0,    54,     0,     0,
     233,     0,  -233,     0,     0,     0,   267,   131,    62,   234,
     235,     0,   236,     0,   237,     0,   238,   147,     0,   239,
     146,     0,     0,   240,     0,     0,   241,    78,     0,   242,
      80,   147,   -62,    82,     0,     0,   142,     0,     0,     0,
       0,     0,     0,   149,     0,   149,     0,     0,   146,     0,
     149,     0,     0,   149,   146,     0,     0,     0,     0,   131,
     149,   146,     0,   149,     0,   131,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,     0,     0,   395,     0,   777,     0,   149,   142,     0,
       0,     0,     0,     0,   142,     0,     0,  -502,   149,     0,
     149,     0,   147,     0,     0,     0,     0,     0,     0,     0,
     382,     0,     0,  -540,   146,  -540,     0,     0,   146,     0,
       0,     0,     0,     0,     0,   131,   146,   131,     0,     0,
     147,     0,   131,     0,     0,   131,   147,     0,     0,     0,
       0,     0,   131,   147,     0,   131,     0,     0,     0,     0,
       0,     0,   151,   104,   142,     0,   142,     0,     0,     0,
       0,   142,   151,   162,   142,     0,     0,     0,     0,   131,
       0,   142,     0,     0,   142,   151,   194,   151,   197,     0,
     131,     0,   131,     0,   149,     0,     0,     0,     0,     0,
       0,   149,     0,     0,     0,     0,   147,     0,   142,     0,
     147,     0,     0,   149,     0,     0,     0,     0,   147,   142,
       0,   142,     0,     0,     0,     0,     0,   149,     0,     0,
       0,     0,   151,   287,     0,     0,   149,     0,     0,     0,
     149,     0,     0,     0,     0,   146,     0,     0,     0,     0,
       0,     0,   243,     0,     0,     0,     0,     0,     0,   151,
     104,     0,     0,   151,   323,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   131,     0,     0,     0,
       0,     0,   149,   131,     0,     0,     0,     0,     0,   297,
       0,     0,     0,     0,     0,   131,     0,   146,     0,     0,
       0,     0,     0,   146,     0,   142,     0,     0,     0,   131,
       0,     0,   142,     0,     0,     0,     0,     0,   131,     0,
       0,     0,   131,     0,   142,   151,   389,   147,     0,     0,
     149,   149,   149,   149,     0,     0,     0,     0,   142,   328,
       0,   329,     0,     0,     0,     0,     0,   142,     0,     0,
       0,   142,     0,     0,     0,     0,   149,   149,     0,     0,
       0,     0,     0,   146,   131,   146,     0,     0,     0,     0,
     146,     0,     0,   146,     0,     0,     0,     0,     0,   147,
     146,     0,     0,   146,     0,   147,     0,     0,     0,     0,
       0,     0,     0,   142,     0,     0,     0,     0,     0,     0,
       0,   297,     0,   151,   440,     0,     0,   146,     0,     0,
     151,   453,   131,   131,   131,   131,     0,   297,   146,   420,
     146,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   131,   131,
       0,   142,   142,   142,   142,   147,     0,   147,     0,     0,
     152,     0,   147,     0,     0,   147,     0,   445,     0,     0,
     152,     0,   147,     0,     0,   147,     0,   142,   142,     0,
     151,   104,     0,   152,     0,   152,     0,     0,     0,     0,
       0,     0,   459,   264,   151,   104,     0,    22,    23,   147,
       0,     0,     0,     0,     0,     0,   265,     0,    30,   266,
     147,     0,   147,    35,   146,     0,     0,     0,    39,     0,
       0,   146,     0,     0,     0,     0,     0,     0,     0,     0,
     152,     0,     0,   146,     0,     0,     0,     0,     0,     0,
      52,     0,    54,     0,    56,     0,   920,   146,     0,   921,
       0,   267,     0,    62,     0,     0,   146,   152,     0,     0,
     146,   152,     0,     0,     0,   151,   104,     0,     0,     0,
       0,     0,    78,     0,     0,    80,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   151,   453,     0,   147,     0,     0,   151,
     453,     0,   146,   147,     0,     0,   151,     0,     0,     0,
       0,     0,     0,     0,     0,   147,     0,     0,     0,     0,
       0,     0,     0,   152,     0,     0,     0,     0,    97,   147,
       0,     0,     0,   583,  1002,     0,     0,   586,   147,     0,
       0,     0,   147,     0,     0,     0,     0,     0,     0,     0,
     146,   146,   146,   146,     0,     0,     0,     0,     0,   151,
     661,     0,     0,   151,   666,   445,     0,     0,     0,     0,
       0,   151,   104,     0,     0,     0,   146,   146,     0,     0,
       0,     0,     0,     0,   147,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     219,   152,     0,     0,     0,     0,     0,     0,   152,     0,
       0,     0,     0,     0,   220,   221,     0,   222,     0,     0,
       0,     0,   223,   674,     0,     0,     0,     0,   681,     0,
     224,     0,   147,   147,   147,   147,   225,     0,     0,     0,
       0,   226,     0,   227,     0,     0,   228,     0,     0,     0,
       0,     0,     0,     0,   229,     0,     0,     0,   147,   147,
     230,   231,     0,     0,   340,     0,     0,   232,   152,     0,
       0,     0,     0,     0,     0,     0,   233,     0,     0,     0,
     151,   747,   152,     0,     0,   234,   235,     0,   236,     0,
     237,     0,   238,     0,     0,   239,     0,     0,     0,   240,
       0,     0,   241,   719,     0,   242,   722,     0,     0,     0,
       0,     0,     0,     0,   341,     0,     0,     0,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   151,   782,     0,     0,     0,     0,   151,   787,
       0,     0,     0,     0,   756,     0,     0,     0,   445,   343,
       0,     0,     0,   152,     0,     0,     0,     0,     0,     0,
       0,   246,     0,     0,     0,   344,   345,     0,   346,   347,
     348,   779,   349,   350,   351,   297,   352,   353,   354,   355,
       0,   152,   357,   358,   359,   360,   361,   152,   363,     0,
       0,   364,   365,   366,   152,     0,     0,     0,   151,   453,
     151,   453,     0,     0,     0,   151,   453,     0,   151,   453,
     843,     0,     0,     0,     0,   151,   885,     0,   151,   887,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,     0,     0,     0,     0,     0,
       0,     0,   151,   903,     0,     0,     0,   152,   219,     0,
       0,   152,     0,   151,   909,   151,   913,     0,     0,   152,
       0,     0,   220,   221,   382,   222,     0,  -540,     0,  -540,
     223,     0,     0,   889,     0,     0,   445,     0,   224,     0,
       0,     0,     0,     0,   225,     0,     0,     0,     0,   226,
       0,   227,     0,     0,   228,   340,     0,     0,   779,     0,
       0,     0,   229,     0,     0,     0,     0,     0,   230,   231,
     338,     0,     0,     0,     0,   232,   681,   339,     0,     0,
       0,     0,     0,     0,   233,     0,     0,     0,     0,   340,
       0,     0,     0,   234,   235,     0,   236,     0,   237,   151,
     238,     0,   930,   239,     0,   341,   151,   240,     0,   342,
     241,     0,     0,   242,     0,     0,     0,     0,   151,   958,
       0,     0,     0,     0,     0,     0,     0,     0,   152,     0,
       0,     0,   151,   977,     0,     0,     0,     0,     0,   341,
       0,   151,   982,   342,     0,   151,   984,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,   345,     0,   346,
     347,   348,     0,   349,   350,   351,     0,     0,   353,   755,
     426,     0,     0,     0,   343,   359,   360,     0,     0,   363,
     152,     0,   364,   365,   366,     0,   152,   151,  1010,     0,
     344,   345,     0,   346,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,   355,   356,     0,   357,   358,   359,
     360,   361,   362,   363,   843,     0,   364,   365,   366,     0,
       0,     0,     0,     0,     0,   785,     0,   264,     0,     0,
       0,    22,    23,     0,     0,   151,   151,   151,   151,     0,
     265,     0,    30,   266,     0,     0,   152,    35,   152,     0,
       0,     0,    39,   152,     0,     0,   152,     0,     0,     0,
       0,   151,   151,   152,     0,     0,   152,     0,     0,     0,
       0,     0,     0,     0,    52,     0,    54,     0,   322,     0,
     920,     0,     0,   921,     0,   267,     0,    62,     0,     0,
     152,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   152,     0,   152,     0,     0,    78,     0,     0,    80,
       0,  1077,    82,     5,   270,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   169,     0,     0,    15,    16,     0,
      17,     0,   170,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   171,     0,     0,     0,    32,
     172,   173,     0,   174,    37,     0,    38,     0,   338,    40,
       0,     0,    97,    42,    43,   339,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,   340,     0,     0,
      53,     0,    55,     0,    57,     0,     0,   152,     0,    60,
     175,     0,     0,     0,   152,     0,     0,     0,    67,    68,
      69,    70,    71,   176,     0,    73,   152,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
     152,     0,     0,     0,     0,     0,    86,   341,     0,   152,
       0,   342,     0,   152,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,   343,     0,    94,    95,   271,     0,     0,     0,
      98,     0,   177,     0,   100,   152,   178,  1078,   344,   345,
       0,   346,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,   355,   356,     0,   357,   358,   359,   360,   361,
     362,   363,     0,     0,   364,   365,   366,     0,     0,     0,
       0,     0,     0,   861,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   152,   152,   152,   152,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    -2,     4,     0,
       5,     0,     6,     7,     8,     9,    10,    11,     0,   152,
     152,    12,    13,    14,    15,    16,     0,    17,     0,    18,
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
       4,   100,     5,   101,     6,     7,     8,     9,    10,    11,
       0,  -568,     0,    12,    13,    14,    15,    16,  -568,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    -568,    27,    28,  -568,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,     0,    38,    39,     0,    40,  -233,
       0,    41,    42,    43,     0,    44,    45,    46,   -43,    47,
      48,     0,    49,    50,    51,     0,     0,     0,    52,    53,
      54,    55,     0,    57,    58,  -233,   -43,    59,    60,    61,
    -568,    62,    63,    64,  -568,    65,    66,    67,    68,    69,
      70,    71,    72,     0,    73,    74,     0,    75,    76,    77,
      78,    79,     0,    80,    81,   -61,    82,    83,     0,     0,
      84,     0,    85,     0,     0,  -568,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -568,  -568,    89,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,     0,  -568,  -568,  -568,  -568,  -568,     0,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,    97,  -568,  -568,  -568,
       0,    99,  -568,   100,   307,   101,     5,   270,     6,     7,
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
       0,     0,     0,     0,    93,     0,     0,    94,    95,   271,
      97,     0,     0,    98,     0,    99,   308,   100,     4,   101,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
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
     480,   100,   499,   101,     5,     0,     6,     7,     8,     9,
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
       0,     0,    93,     0,     0,    94,    95,    96,    97,     0,
       0,    98,     0,    99,   500,   100,   307,   101,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
       0,    38,    39,     0,    40,  -233,     0,    41,    42,    43,
       0,    44,    45,    46,   -43,    47,    48,     0,    49,    50,
      51,     0,     0,     0,    52,    53,    54,    55,    56,    57,
      58,  -233,   -43,    59,    60,    61,     0,    62,    63,    64,
       0,    65,    66,    67,    68,    69,    70,    71,    72,     0,
      73,    74,     0,    75,    76,    77,    78,    79,     0,    80,
      81,   -61,    82,    83,     0,     0,    84,     0,    85,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,    97,     0,     0,    98,     0,    99,   308,   100,
       4,   101,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
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
       0,    99,   673,   100,     4,   101,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,     0,    38,
      39,     0,    40,  -233,     0,    41,    42,    43,     0,    44,
      45,    46,   -43,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,   322,    57,    58,  -233,
     -43,    59,    60,    61,     0,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
       0,    75,    76,    77,    78,    79,     0,    80,    81,   -61,
      82,    83,     0,     0,    84,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,     4,   100,     5,   101,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
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
      95,    96,    97,     0,     0,    98,     0,    99,     4,   100,
       5,   101,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
     591,    37,     0,    38,    39,     0,    40,  -233,     0,    41,
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
       4,   100,     5,   101,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,   597,    37,     0,    38,    39,     0,    40,  -233,
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
       0,    99,     4,   100,     5,   101,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,   870,    37,     0,    38,    39,     0,
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
       0,    98,     0,    99,     4,   100,     5,   101,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,   872,    37,     0,    38,
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
      97,     0,     0,    98,     0,    99,     4,   100,     5,   101,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,   877,    37,
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
      95,    96,    97,     0,     0,    98,     0,    99,     4,   100,
       5,   101,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
     880,    37,     0,    38,    39,     0,    40,  -233,     0,    41,
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
       4,   100,     5,   101,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,   902,    28,     0,    29,     0,    30,    31,    32,    33,
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
       0,    99,     4,   100,     5,   101,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,   908,    37,     0,    38,    39,     0,
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
       0,    98,     0,    99,     4,   100,     5,   101,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,   912,    37,     0,    38,
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
      97,     0,     0,    98,     0,    99,     0,   100,   765,   101,
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
       0,   100,     0,   178,     5,   270,     6,     7,     8,     9,
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
       0,    98,     0,   177,     0,   100,     0,   178,   665,     5,
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
     100,     0,   178,  1013,     5,     0,     6,     7,     8,     9,
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
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   177,     0,   100,     0,   178,   670,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     169,     0,     0,    15,    16,     0,    17,     0,   170,     0,
       0,    21,   214,    23,     0,     0,     0,     0,     0,    28,
       0,   171,     0,     0,     0,    32,   172,   173,     0,   174,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,    54,    55,     0,
      57,     0,     0,     0,     0,    60,   175,     0,    62,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   176,
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,    82,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   177,     0,
     100,     0,   178,     5,   270,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   169,     0,     0,    15,    16,     0,
      17,     0,   170,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   171,     0,     0,     0,    32,
     172,   173,     0,   174,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     175,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   176,     0,    73,     0,     0,    75,   275,
     276,     0,    79,   312,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,   313,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,   271,     0,     0,     0,
      98,     0,   177,     0,   100,     0,   178,     5,   270,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   169,     0,
       0,    15,    16,     0,    17,     0,   170,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   171,
       0,     0,     0,    32,   172,   173,     0,   174,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   175,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   176,     0,    73,
       0,     0,    75,   275,   276,     0,    79,   312,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
     271,     0,     0,     0,    98,     0,   177,     0,   100,   657,
     178,     5,   270,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   169,     0,     0,    15,    16,     0,    17,     0,
     170,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   171,     0,     0,     0,    32,   172,   173,
       0,   174,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   175,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   176,     0,    73,     0,     0,    75,   275,   276,     0,
      79,   312,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,   271,     0,     0,     0,    98,     0,
     177,     0,   100,   659,   178,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   169,     0,     0,    15,
      16,     0,    17,     0,   170,     0,     0,    21,     0,   556,
       0,     0,     0,     0,     0,    28,     0,   171,     0,     0,
       0,    32,   172,   173,     0,   174,    37,   557,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   175,     0,   558,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   176,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,   559,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   177,     0,   100,     5,   178,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   169,     0,
       0,    15,    16,     0,    17,     0,   170,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   171,
       0,     0,     0,    32,   172,   173,     0,   174,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   175,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   176,     0,    73,
       0,     0,    75,   275,   276,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,   277,     0,     0,    98,     0,   177,     0,   100,     0,
     178,     5,   270,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   169,     0,     0,    15,    16,     0,    17,     0,
     170,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   171,     0,     0,     0,    32,   172,   173,
       0,   174,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   175,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   176,     0,    73,     0,     0,    75,   275,   276,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,   271,     0,     0,     0,    98,     0,
     177,     0,   100,     5,   178,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   169,     0,     0,    15,    16,     0,
      17,     0,   170,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   171,     0,     0,     0,    32,
     172,   173,     0,   174,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,   767,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     175,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   176,     0,    73,     0,     0,    75,   275,
     276,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,   177,     0,   100,     5,   178,     6,     7,     8,
     203,    10,    11,   204,     0,     0,   169,     0,     0,    15,
      16,     0,    17,     0,   170,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   171,     0,     0,
       0,    32,   172,   173,     0,   174,    37,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   205,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   176,     0,    73,     0,     0,
      75,     0,     0,   206,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   177,     0,   100,     5,   178,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   169,     0,
       0,    15,    16,     0,    17,     0,   170,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   171,
       0,     0,     0,    32,   172,   173,     0,   174,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,   208,    55,     0,    57,     0,
       0,     0,     0,    60,   175,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   176,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,   209,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   177,     0,   100,     0,
     178,     5,   270,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   169,     0,     0,    15,    16,     0,    17,     0,
     170,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   171,     0,     0,     0,    32,   172,   173,
       0,   174,    37,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   175,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   176,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,   209,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,   271,     0,     0,     0,    98,     0,
     177,     0,   100,     5,   178,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   169,     0,     0,    15,    16,     0,
      17,     0,   170,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   171,     0,     0,     0,    32,
     172,   173,     0,   174,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     175,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   176,     0,    73,     0,     0,    75,   275,
     276,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,   177,     0,   100,     0,   178,     5,   270,     6,
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
       0,     0,     0,   799,     0,   800,    87,    88,    89,    90,
      91,     0,     0,     0,   801,    92,     0,     0,     0,   802,
     221,   803,   804,     0,     0,    93,     0,   805,    94,    95,
     271,     0,     0,     0,    98,   224,   177,   655,   100,     0,
     178,   806,     0,     0,     0,     0,   807,     0,   227,     0,
       0,   808,     0,   809,     0,     0,     0,     0,     0,   810,
       0,     0,     0,     0,     0,   811,   812,     0,     0,     0,
       0,     0,   232,     0,     0,     0,     0,     0,     0,     0,
       0,   813,     0,     0,     0,     0,     0,     0,     0,     0,
     234,   235,     0,   814,     0,   237,     0,   815,     0,     0,
     816,     0,     0,     0,   817,     0,     0,   241,     0,     0,
     818,     0,     0,     0,     0,     0,     0,     0,     0,   367,
     368,   369,   370,   371,   372,     0,     0,   375,   376,   377,
     378,     0,   380,   381,   819,   820,   821,   822,   823,     0,
       0,   824,     0,     0,     0,   825,   826,   827,   828,   829,
     830,   831,   832,   833,   834,   835,     0,   836,     0,     0,
     837,   838,   839,   840,     0,     5,   841,     6,     7,     8,
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
      83,     0,     0,     0,     0,     0,   209,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   177,     0,   100,     5,   178,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   169,     0,
       0,    15,    16,     0,    17,     0,   170,     0,     0,    21,
       0,     0,     0,     0,     0,     0,   250,    28,     0,   171,
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
     178,     5,   270,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   169,     0,     0,    15,    16,     0,    17,     0,
     170,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   171,     0,     0,     0,    32,   172,   173,
       0,   174,    37,     0,    38,     0,     0,    40,     0,     0,
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
       0,     0,    94,    95,   271,     0,     0,     0,    98,     0,
     177,     0,   100,     5,   178,     6,     7,     8,   203,    10,
      11,     0,     0,     0,   169,     0,     0,    15,    16,     0,
      17,     0,   170,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   171,     0,     0,     0,    32,
     172,   173,     0,   174,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     205,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   176,     0,    73,     0,     0,    75,     0,
       0,   206,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,   177,     0,   100,     0,   178,     5,   270,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   169,     0,
       0,    15,    16,     0,    17,     0,   170,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   629,     0,   171,
       0,     0,     0,    32,   172,   173,     0,   174,    37,     0,
     630,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   175,     0,     0,     0,     0,     0,
       0,     0,    67,   631,    69,    70,    71,   632,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
     271,     0,     0,     0,    98,     0,   177,     0,   100,     0,
     961,     5,   270,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   169,     0,     0,    15,    16,     0,    17,     0,
     170,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   171,     0,     0,     0,    32,   172,   173,
       0,   174,    37,     0,    38,     0,     0,    40,     0,     0,
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
       0,     0,    94,    95,   271,     0,     0,     0,    98,     0,
     177,     0,   100,     0,   961,     5,   270,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   169,     0,     0,    15,
      16,     0,    17,     0,   170,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   171,     0,     0,
       0,    32,   172,   173,     0,   944,    37,     0,    38,     0,
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
       0,     0,    98,     0,   177,     0,   100,     5,   178,     6,
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
      96,     0,     0,     0,    98,     0,   177,     0,   100,     5,
     178,     6,     7,     8,   199,    10,    11,     0,     0,     0,
     169,     0,     0,    15,    16,     0,    17,     0,   170,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   171,     0,     0,     0,    32,   172,   173,     0,   174,
      37,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   200,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   176,
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   177,     0,
     100,     5,   178,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   169,     0,     0,    15,    16,     0,    17,     0,
     170,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   171,     0,     0,     0,    32,   172,   173,
       0,   174,    37,     0,    38,     0,     0,    40,     0,     0,
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
      99,     0,   100,     5,   178,     6,     7,     8,     9,    10,
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
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
     602,     0,   177,     0,   100,     5,   178,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   169,     0,     0,    15,
      16,     0,    17,     0,   170,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,   629,     0,   171,     0,     0,
       0,    32,   172,   173,     0,   174,    37,     0,   630,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   175,     0,     0,     0,     0,     0,     0,     0,
      67,   631,    69,    70,    71,   632,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   177,     0,   100,     5,   633,     6,
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
      96,     0,     0,     0,    98,     0,   177,     0,   100,     5,
     633,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     169,     0,     0,    15,    16,     0,    17,     0,   170,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   171,     0,     0,     0,    32,   172,   173,     0,   705,
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
     100,     5,   178,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   169,     0,     0,    15,    16,     0,    17,     0,
     170,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   171,     0,     0,     0,    32,   172,   173,
       0,   708,    37,     0,    38,     0,     0,    40,     0,     0,
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
     177,     0,   100,     5,   178,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   169,     0,     0,    15,    16,     0,
      17,     0,   170,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   171,     0,     0,     0,    32,
     172,   173,     0,   938,    37,     0,    38,     0,     0,    40,
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
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,   177,     0,   100,     5,   178,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   169,     0,     0,    15,
      16,     0,    17,     0,   170,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   171,     0,     0,
       0,    32,   172,   173,     0,   939,    37,     0,    38,     0,
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
       0,     0,    98,     0,   177,     0,   100,     5,   178,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   169,     0,
       0,    15,    16,     0,    17,     0,   170,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   171,
       0,     0,     0,    32,   172,   173,     0,   941,    37,     0,
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
      96,     0,     0,     0,    98,     0,   177,     0,   100,     5,
     178,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     169,     0,     0,    15,    16,     0,    17,     0,   170,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   171,     0,     0,     0,    32,   172,   173,     0,   942,
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
     100,     5,   178,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   169,     0,     0,    15,    16,     0,    17,     0,
     170,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   171,     0,     0,     0,    32,   172,   173,
       0,   943,    37,     0,    38,     0,     0,    40,     0,     0,
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
     177,     0,   100,     5,   178,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   169,     0,     0,    15,    16,     0,
      17,     0,   170,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   171,     0,     0,     0,    32,
     172,   173,     0,   944,    37,     0,    38,     0,     0,    40,
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
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,   177,     0,   100,     5,   178,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   169,     0,     0,    15,
      16,     0,    17,     0,   170,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,   629,     0,   171,     0,     0,
       0,    32,   172,   173,     0,   174,    37,     0,   630,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   175,     0,     0,     0,     0,     0,     0,     0,
      67,   631,    69,    70,    71,   632,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   177,     0,   100,     5,  1033,     6,
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
      96,     0,     0,     0,    98,     0,     0,     0,   100,     5,
    1033,     6,     7,     8,     9,    10,    11,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,    95,    96,     0,     0,     0,    98,     0,   177,     0,
     100,     0,   178,     5,   270,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   169,     0,     0,    15,    16,   340,
      17,     0,   170,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   171,     0,     0,     0,    32,
     172,   173,     0,   174,    37,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,   341,
      53,     0,    55,   342,    57,     0,     0,     0,     0,    60,
     175,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   176,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,     0,  -398,     0,     0,
     344,   345,     0,   346,   347,   348,     0,   349,   350,     0,
       0,     0,   353,     0,    89,     0,     0,     0,  -398,   359,
     360,     0,  -398,   363,     0,     0,   364,   365,   366,     0,
       0,     0,     0,     0,     0,     0,   271,     0,     0,     0,
       0,     0,   177,     0,   100,  -398,   961,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   169,     0,
       0,    15,    16,     0,    17,     0,   170,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   171,
       0,     0,     0,    32,   172,   173,     0,   174,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,  -380,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   175,     0,     0,     0,  -380,     0,
       0,     0,    67,    68,    69,    70,    71,   176,     0,    73,
       0,  -380,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,  -380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,    89,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   169,     0,
       0,    15,    16,     0,    17,     0,   170,     0,     0,    21,
      96,  -380,     0,     0,     0,     0,  -380,    28,   100,   171,
    1033,     0,     0,    32,   172,   173,     0,   174,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   175,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   176,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,     5,    89,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   169,     0,
       0,    15,    16,     0,    17,     0,   170,     0,     0,    21,
      96,     0,     0,     0,     0,     0,   177,    28,   100,   171,
     178,     0,     0,    32,   172,   173,     0,   174,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   175,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   176,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,    89,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   169,     0,
       0,    15,    16,     0,    17,     0,   170,     0,     0,    21,
      96,     0,     0,     0,     0,     0,   177,    28,   100,   171,
     633,     0,     0,    32,   172,   173,     0,   174,    37,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   175,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   176,     0,    73,
       0,     0,    75,     0,   991,     0,    79,     0,     0,    81,
       0,     0,    83,   801,     0,     0,     0,     0,   220,   221,
     803,   222,     0,     0,     0,     0,   223,     0,     0,     0,
       0,     0,     0,     0,   224,     0,     0,     0,    89,     0,
     806,     0,     0,     0,     0,   226,     0,   227,     0,     0,
     228,     0,   809,     0,     0,     0,     0,     0,   229,     0,
       0,     0,     0,     0,   811,   231,     0,     0,   100,   338,
     178,   232,     0,     0,     0,     0,   339,     0,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   340,   234,
     235,     0,   236,     0,   237,     0,   992,     0,     0,   816,
       0,     0,     0,   240,     0,     0,   241,     0,     0,   242,
       0,     0,     0,     0,     0,     0,     0,     0,   367,   368,
     369,   370,   371,   372,     0,     0,   375,   376,   377,   378,
     340,   380,   381,   819,   820,   821,   822,   823,   341,     0,
     824,     0,   342,     0,   825,   826,   827,   828,   829,   830,
     831,   832,   833,   834,   835,     0,   836,   338,     0,   837,
     838,   839,   840,     0,   339,     0,     0,     0,     0,     0,
       0,     0,     0,   343,     0,     0,   340,     0,     0,     0,
     341,     0,     0,     0,   342,     0,     0,     0,     0,   344,
     345,     0,   346,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,   355,   356,     0,   357,   358,   359,   360,
     361,   362,   363,     0,   338,   364,   365,   366,     0,     0,
       0,   339,     0,     0,  1027,     0,   341,     0,     0,     0,
     342,   344,   345,   340,   346,   347,   348,     0,   349,   350,
     351,     0,   352,   353,   354,   355,     0,     0,   357,   358,
     359,   360,   361,     0,   363,   338,     0,   364,   365,   366,
       0,   343,   339,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   340,     0,     0,   344,   345,     0,
     346,   347,   348,   341,   349,   350,   351,   342,   352,   353,
     354,   355,   356,     0,   357,   358,   359,   360,   361,   362,
     363,   338,     0,   364,   365,   366,     0,     0,   339,     0,
       0,     0,  1100,     0,     0,     0,     0,     0,   343,     0,
     340,     0,     0,     0,   341,     0,     0,     0,   342,     0,
       0,     0,     0,     0,   344,   345,     0,   346,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,   355,   356,
       0,   357,   358,   359,   360,   361,   362,   363,   338,   343,
     364,   365,   366,     0,     0,   339,     0,   656,     0,     0,
     341,     0,     0,     0,   342,   344,   345,   340,   346,   347,
     348,     0,   349,   350,   351,     0,   352,   353,   354,   355,
     356,     0,   357,   358,   359,   360,   361,   362,   363,     0,
       0,   364,   365,   366,     0,   343,     0,     0,   769,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,   345,     0,   346,   347,   348,   341,   349,   350,
     351,   342,   352,   353,   354,   355,   356,     0,   357,   358,
     359,   360,   361,   362,   363,   338,     0,   364,   365,   366,
       0,     0,   339,     0,   770,     0,     0,     0,     0,     0,
       0,     0,   343,     0,   340,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   344,   345,
       0,   346,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,   355,   356,     0,   357,   358,   359,   360,   361,
     362,   363,     0,     0,   364,   365,   366,     0,     0,     0,
       0,   990,     0,     0,   341,     0,     0,     0,   342,     0,
       0,   338,     0,     0,     0,     0,     0,   436,   339,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     340,     0,     0,     0,     0,     0,     0,     0,     0,   343,
     367,   368,   369,   370,   371,   372,     0,     0,   375,   376,
     377,   378,     0,   380,   381,   344,   345,     0,   346,   347,
     348,     0,   349,   350,   351,     0,   352,   353,   354,   355,
     356,     0,   357,   358,   359,   360,   361,   362,   363,     0,
     341,   364,   365,   366,   342,   395,     0,   338,     0,     0,
       0,     0,     0,   869,   339,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   340,     0,     0,     0,
       0,     0,     0,     0,     0,   343,   367,   368,   369,   370,
     371,   372,     0,     0,   375,   376,   377,   378,     0,   380,
     381,   344,   345,     0,   346,   347,   348,     0,   349,   350,
     351,     0,   352,   353,   354,   355,   356,     0,   357,   358,
     359,   360,   361,   362,   363,     0,   341,   364,   365,   366,
     342,   395,     0,   338,     0,     0,     0,     0,     0,   876,
     339,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   340,     0,     0,     0,     0,     0,     0,     0,
       0,   343,   367,   368,   369,   370,   371,   372,     0,     0,
     375,   376,   377,   378,     0,   380,   381,   344,   345,     0,
     346,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,   355,   356,     0,   357,   358,   359,   360,   361,   362,
     363,     0,   341,   364,   365,   366,   342,   395,     0,   338,
       0,     0,     0,     0,     0,  1005,   339,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   340,     0,
       0,     0,     0,     0,     0,     0,     0,   343,   367,   368,
     369,   370,   371,   372,     0,     0,   375,   376,   377,   378,
       0,   380,   381,   344,   345,     0,   346,   347,   348,     0,
     349,   350,   351,     0,   352,   353,   354,   355,   356,     0,
     357,   358,   359,   360,   361,   362,   363,     0,   341,   364,
     365,   366,   342,   395,     0,   338,     0,     0,     0,     0,
       0,  1006,   339,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   340,     0,     0,     0,     0,     0,
       0,     0,     0,   343,   367,   368,   369,   370,   371,   372,
       0,     0,   375,   376,   377,   378,     0,   380,   381,   344,
     345,     0,   346,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,   355,   356,     0,   357,   358,   359,   360,
     361,   362,   363,     0,   341,   364,   365,   366,   342,   395,
       0,   338,     0,     0,     0,     0,     0,  1007,   339,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     340,     0,     0,     0,     0,     0,     0,     0,     0,   343,
     367,   368,   369,   370,   371,   372,     0,     0,   375,   376,
     377,   378,     0,   380,   381,   344,   345,     0,   346,   347,
     348,     0,   349,   350,   351,     0,   352,   353,   354,   355,
     356,     0,   357,   358,   359,   360,   361,   362,   363,     0,
     341,   364,   365,   366,   342,   395,     0,   338,     0,     0,
       0,     0,     0,  1008,   339,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   340,     0,     0,     0,
       0,     0,     0,     0,     0,   343,   367,   368,   369,   370,
     371,   372,     0,     0,   375,   376,   377,   378,     0,   380,
     381,   344,   345,     0,   346,   347,   348,     0,   349,   350,
     351,     0,   352,   353,   354,   355,   356,     0,   357,   358,
     359,   360,   361,   362,   363,     0,   341,   364,   365,   366,
     342,   395,     0,   338,     0,     0,     0,     0,     0,  1031,
     339,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   340,     0,     0,     0,     0,     0,     0,     0,
       0,   343,   367,   368,   369,   370,   371,   372,     0,     0,
     375,   376,   377,   378,     0,   380,   381,   344,   345,     0,
     346,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,   355,   356,     0,   357,   358,   359,   360,   361,   362,
     363,     0,   341,   364,   365,   366,   342,   395,     0,     0,
       0,     0,     0,     0,     0,  1032,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   343,   367,   368,
     369,   370,   371,   372,     0,     0,   375,   376,   377,   378,
       0,   380,   381,   344,   345,     0,   346,   347,   348,     0,
     349,   350,   351,     0,   352,   353,   354,   355,   356,     0,
     357,   358,   359,   360,   361,   362,   363,   338,     0,   364,
     365,   366,     0,   395,   339,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   340,   429,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   338,
     430,     0,     0,     0,     0,     0,   339,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   340,   423,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   424,     0,     0,     0,   341,     0,     0,     0,
     342,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   338,     0,     0,     0,
       0,     0,     0,   339,     0,     0,     0,   426,   341,     0,
       0,   343,   342,     0,     0,   340,   738,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,   345,     0,
     346,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,   355,   356,   343,   357,   358,   359,   360,   361,   362,
     363,     0,     0,   364,   365,   366,     0,   395,     0,   344,
     345,     0,   346,   347,   348,   341,   349,   350,   351,   342,
     352,   353,   354,   355,   356,     0,   357,   358,   359,   360,
     361,   362,   363,   338,     0,   364,   365,   366,     0,   395,
     339,     0,     0,     0,     0,     0,   426,     0,     0,     0,
     343,     0,   340,   250,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,   345,     0,   346,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
     355,   356,     0,   357,   358,   359,   360,   361,   362,   363,
     338,     0,   364,   365,   366,     0,   395,   339,     0,     0,
       0,     0,   341,     0,     0,     0,   342,     0,     0,   340,
     730,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   338,     0,     0,     0,     0,     0,     0,   339,
       0,     0,     0,     0,     0,     0,     0,   343,     0,     0,
       0,   340,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   344,   345,     0,   346,   347,   348,   341,
     349,   350,   351,   342,   352,   353,   354,   355,   356,     0,
     357,   358,   359,   360,   361,   362,   363,     0,     0,   364,
     365,   366,     0,   395,     0,     0,     0,     0,     0,     0,
       0,   341,     0,     0,   343,   342,     0,     0,     0,     0,
       0,     0,     0,     0,   748,     0,     0,     0,     0,     0,
     344,   345,     0,   346,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,   355,   356,   343,   357,   358,   359,
     360,   361,   362,   363,     0,     0,   364,   365,   366,     0,
     395,     0,   344,   345,     0,   346,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,   355,   356,     0,   357,
     358,   359,   360,   361,   362,   363,   338,     0,   364,   365,
     366,     0,   395,   339,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   340,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   338,     0,
       0,     0,     0,     0,     0,   339,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   340,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   341,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,     0,     0,   338,     0,     0,     0,     0,
       0,     0,   339,     0,     0,     0,     0,   341,     0,     0,
     343,   342,     0,     0,   340,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,   345,   397,   346,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
     355,   356,   343,   357,   358,   359,   360,   361,   362,   363,
       0,     0,   364,   365,   366,     0,   465,     0,   344,   345,
       0,   346,   347,   348,   341,   349,   350,   351,   342,   352,
     353,   354,   355,   356,     0,   357,   358,   359,   360,   361,
     362,   363,   338,   435,   364,   365,   366,     0,     0,   339,
       0,     0,     0,     0,     0,   164,     0,     0,     0,   343,
       0,   340,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,   345,     0,   346,   347,
     348,     0,   349,   350,   351,     0,   352,   353,   354,   355,
     356,     0,   357,   358,   359,   360,   361,   362,   363,   338,
       0,   364,   365,   366,     0,     0,   339,     0,     0,     0,
       0,   341,     0,     0,     0,   342,     0,     0,   340,   565,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   338,   566,     0,     0,     0,     0,     0,   339,     0,
       0,     0,     0,     0,     0,     0,   343,     0,     0,     0,
     340,   567,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   344,   345,   568,   346,   347,   348,   341,   349,
     350,   351,   342,   352,   353,   354,   355,   356,     0,   357,
     358,   359,   360,   361,   362,   363,     0,   469,   364,   365,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     341,     0,     0,   343,   342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   344,
     345,     0,   346,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,   355,   356,   343,   357,   358,   359,   360,
     361,   362,   363,     0,     0,   364,   365,   366,     0,     0,
       0,   344,   345,     0,   346,   347,   348,     0,   349,   350,
     351,     0,   352,   353,   354,   355,   356,     0,   357,   358,
     359,   360,   361,   362,   363,   338,     0,   364,   365,   366,
       0,     0,   339,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   340,     0,     0,     0,     0,   676,
       0,     0,     0,     0,     0,     0,     0,   338,     0,     0,
       0,     0,     0,     0,   339,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   340,     0,     0,   677,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   341,     0,     0,     0,   342,     0,
       0,     0,     0,     0,     0,     0,   338,     0,     0,     0,
       0,     0,     0,   339,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   340,   341,     0,     0,   343,
     342,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,   345,     0,   346,   347,
     348,     0,   349,   350,   351,     0,   352,   353,   354,   355,
     356,   343,   357,   358,   359,   360,   361,   362,   363,     0,
       0,   364,   365,   366,     0,   341,     0,   344,   345,   342,
     346,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,   355,   356,     0,   357,   358,   359,   360,   361,   362,
     363,   338,   471,   364,   365,   366,     0,     0,   339,     0,
     343,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     340,     0,     0,     0,     0,     0,   344,   345,     0,   346,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
     355,   356,     0,   357,   358,   359,   360,   361,   362,   363,
     338,   473,   364,   365,   366,     0,     0,   339,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   340,
     341,     0,     0,     0,   342,     0,     0,     0,     0,     0,
       0,     0,   338,     0,     0,     0,     0,     0,     0,   339,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   340,     0,     0,     0,   343,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   341,
       0,   344,   345,   342,   346,   347,   348,     0,   349,   350,
     351,     0,   352,   353,   354,   355,   356,     0,   357,   358,
     359,   360,   361,   362,   363,     0,   478,   364,   365,   366,
       0,   341,     0,   495,   343,   342,     0,     0,     0,     0,
       0,     0,     0,     0,   569,     0,     0,     0,     0,     0,
     344,   345,     0,   346,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,   355,   356,   343,   357,   358,   359,
     360,   361,   362,   363,     0,     0,   364,   365,   366,     0,
       0,     0,   344,   345,     0,   346,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,   355,   356,     0,   357,
     358,   359,   360,   361,   362,   363,   338,     0,   364,   365,
     366,     0,     0,   339,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   340,     0,   264,     0,     0,
       0,    22,    23,     0,     0,     0,     0,     0,     0,     0,
     265,     0,    30,   266,     0,     0,     0,    35,     0,     0,
       0,   338,    39,     0,     0,     0,     0,     0,   339,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     340,     0,     0,     0,    52,   341,    54,     0,    56,   342,
     920,     0,     0,   921,     0,   267,     0,    62,     0,     0,
       0,     0,   338,     0,     0,     0,     0,     0,     0,   339,
       0,     0,     0,     0,     0,     0,    78,     0,     0,    80,
     343,   340,    82,     0,     0,     0,     0,     0,     0,     0,
     341,     0,     0,     0,   342,     0,   344,   345,     0,   346,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
     355,   356,  -503,   357,   358,   359,   360,   361,   362,   363,
       0,   164,   364,   365,   366,   343,     0,     0,     0,     0,
       0,   341,    97,     0,     0,   342,     0,     0,  1004,     0,
       0,   344,   345,     0,   346,   347,   348,     0,   349,   350,
     351,     0,   352,   353,   354,   355,   356,     0,   357,   358,
     359,   360,   361,   362,   363,     0,   343,   364,   365,   366,
       0,     0,     0,   338,   743,     0,     0,     0,     0,     0,
     339,     0,   344,   345,     0,   346,   347,   348,     0,   349,
     350,   351,   340,   352,   353,   354,   355,   356,     0,   357,
     358,   359,   360,   361,   362,   363,     0,   715,   364,   365,
     366,     0,     0,   338,     0,     0,     0,     0,     0,     0,
     339,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   340,     0,     0,   746,     0,     0,     0,     0,
       0,     0,   341,     0,     0,     0,   342,     0,     0,     0,
       0,     0,     0,     0,   338,     0,     0,     0,     0,     0,
       0,   339,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   340,     0,     0,     0,   343,     0,     0,
       0,     0,   341,     0,     0,     0,   342,     0,     0,     0,
       0,     0,     0,   344,   345,     0,   346,   347,   348,     0,
     349,   350,   351,     0,   352,   353,   354,   355,   356,     0,
     357,   358,   359,   360,   361,   362,   363,   343,     0,   364,
     365,   366,     0,   341,     0,     0,     0,   342,     0,     0,
       0,     0,     0,   344,   345,     0,   346,   347,   348,     0,
     349,   350,   351,     0,   352,   353,   354,   355,   356,     0,
     357,   358,   359,   360,   361,   362,   363,   788,   343,   364,
     365,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   344,   345,     0,   346,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,   355,   356,
       0,   357,   358,   359,   360,   361,   362,   363,   338,  -214,
     364,   365,   366,     0,     0,   339,     0,     0,     0,     0,
       0,     0,     0,  -214,  -214,     0,  -214,   340,   856,     0,
       0,  -214,     0,     0,     0,     0,     0,     0,     0,  -214,
     338,     0,     0,     0,     0,  -214,     0,   339,     0,     0,
    -214,     0,  -214,     0,     0,  -214,     0,     0,     0,   340,
     859,     0,     0,  -214,     0,     0,     0,     0,     0,  -214,
    -214,     0,     0,     0,     0,     0,  -214,   341,     0,     0,
       0,   342,     0,     0,     0,  -214,     0,     0,     0,     0,
       0,     0,     0,     0,  -214,  -214,     0,  -214,     0,  -214,
       0,  -214,     0,     0,  -214,     0,     0,     0,  -214,   341,
       0,  -214,   343,   342,  -214,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   344,   345,
       0,   346,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,   355,   356,   343,   357,   358,   359,   360,   361,
     362,   363,     0,     0,   364,   365,   366,     0,     0,     0,
     344,   345,     0,   346,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,   355,   356,     0,   357,   358,   359,
     360,   361,   362,   363,   338,     0,   364,   365,   366,     0,
       0,   339,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   340,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   338,     0,     0,     0,
       0,     0,     0,   339,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   340,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   341,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,   937,     0,     0,     0,
       0,     0,     0,   338,   956,     0,     0,     0,     0,     0,
     339,     0,     0,     0,     0,   341,     0,     0,   343,   342,
       0,     0,   340,     0,     0,     0,     0,     0,   940,     0,
       0,     0,     0,     0,   344,   345,     0,   346,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,   355,   356,
     343,   357,   358,   359,   360,   361,   362,   363,     0,     0,
     364,   365,   366,     0,     0,     0,   344,   345,     0,   346,
     347,   348,   341,   349,   350,   351,   342,   352,   353,   354,
     355,   356,     0,   357,   358,   359,   360,   361,   362,   363,
     338,     0,   364,   365,   366,     0,     0,   339,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   343,     0,   340,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   344,   345,     0,   346,   347,   348,     0,
     349,   350,   351,     0,   352,   353,   354,   355,   356,     0,
     357,   358,   359,   360,   361,   362,   363,     0,     0,   364,
     365,   366,     0,     0,   340,     0,     0,     0,     0,   341,
       0,     0,     0,   342,     0,     0,   338,     0,     0,     0,
       0,     0,  1042,   339,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   340,     0,     0,     0,     0,
       0,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   341,     0,     0,     0,   342,     0,
     344,   345,     0,   346,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,   355,   356,     0,   357,   358,   359,
     360,   361,   362,   363,     0,   341,   364,   365,   366,   342,
       0,     0,   338,     0,     0,     0,     0,     0,  1043,   339,
       0,     0,     0,     0,     0,   344,   345,     0,   346,   347,
     348,   340,   349,   350,   351,     0,     0,   353,   354,   355,
     343,     0,   357,   358,   359,   360,     0,     0,   363,     0,
       0,   364,   365,   366,     0,     0,   344,   345,     0,   346,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
     355,   356,     0,   357,   358,   359,   360,   361,   362,   363,
       0,   341,   364,   365,   366,   342,     0,     0,   338,     0,
       0,     0,     0,     0,  1044,   339,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   340,     0,     0,
       0,     0,     0,     0,     0,     0,   343,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   344,   345,     0,   346,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,   355,   356,     0,   357,
     358,   359,   360,   361,   362,   363,     0,   341,   364,   365,
     366,   342,     0,     0,   338,     0,     0,     0,     0,     0,
    1045,   339,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   340,     0,     0,     0,     0,     0,     0,
       0,     0,   343,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   344,   345,
       0,   346,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,   355,   356,     0,   357,   358,   359,   360,   361,
     362,   363,     0,   341,   364,   365,   366,   342,     0,     0,
     338,     0,     0,     0,     0,     0,  1046,   339,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   340,
       0,     0,     0,     0,     0,     0,     0,     0,   343,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   344,   345,     0,   346,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,   355,   356,
       0,   357,   358,   359,   360,   361,   362,   363,     0,   341,
     364,   365,   366,   342,     0,     0,   338,     0,     0,     0,
       0,     0,  1047,   339,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   340,     0,     0,     0,     0,
       0,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1071,
     344,   345,     0,   346,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,   355,   356,     0,   357,   358,   359,
     360,   361,   362,   363,     0,   341,   364,   365,   366,   342,
       0,     0,   338,     0,     0,     0,     0,     0,     0,   339,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   340,     0,     0,     0,     0,     0,     0,     0,     0,
     343,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,   345,     0,   346,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
     355,   356,     0,   357,   358,   359,   360,   361,   362,   363,
       0,   341,   364,   365,   366,   342,     0,     0,   338,     0,
       0,     0,     0,     0,     0,   339,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   340,     0,     0,
       0,     0,     0,     0,     0,     0,   343,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   344,   345,     0,   346,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,   355,   356,     0,   357,
     358,   359,   360,   361,   362,   363,     0,   703,   364,   365,
     366,   342,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   344,   345,
       0,   346,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,   355,   356,     0,   357,   358,   359,   360,   361,
     362,   363,     0,   264,   364,   365,   366,    22,    23,     0,
       0,     0,     0,     0,     0,     0,   265,   264,    30,   266,
       0,    22,    23,    35,     0,     0,     0,     0,    39,     0,
     265,     0,    30,   266,     0,     0,     0,    35,     0,     0,
       0,   340,    39,     0,     0,     0,     0,     0,     0,     0,
      52,     0,    54,     0,    56,     0,   920,     0,     0,   921,
       0,   267,     0,    62,    52,     0,    54,     0,    56,     0,
     920,     0,     0,   921,     0,   267,     0,    62,     0,     0,
       0,     0,    78,     0,     0,    80,     0,     0,    82,     0,
       0,   341,     0,     0,     0,   342,    78,     0,     0,    80,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,     0,   344,   345,  1048,   346,   347,   348,     0,   349,
     350,   351,    97,   352,   353,   354,   355,     0,  1049,   357,
     358,   359,   360,   361,   362,   363,     0,     0,   364,   365,
     366,   680,     0,   219,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   220,   221,     0,
     222,     0,     0,     0,     0,   223,     0,     0,     0,     0,
       0,     0,     0,   224,     0,     0,     0,     0,     0,   225,
       0,     0,     0,     0,   226,     0,   227,     0,     0,   228,
       0,     0,     0,     0,     0,     0,     0,   229,     0,     0,
    -224,     0,     0,   230,   231,     0,     0,     0,     0,     0,
     232,     0,     0,     0,  -224,  -224,     0,  -224,     0,   233,
       0,     0,  -224,     0,     0,     0,     0,     0,   234,   235,
    -224,   236,     0,   237,     0,   238,  -224,     0,   239,     0,
       0,  -224,   240,  -224,     0,   241,  -224,     0,   242,     0,
       0,     0,     0,     0,  -224,     0,     0,   219,     0,     0,
    -224,  -224,     0,     0,     0,     0,     0,  -224,     0,     0,
       0,   220,   221,     0,   222,     0,  -224,     0,     0,   223,
       0,     0,     0,     0,     0,  -224,  -224,   224,  -224,     0,
    -224,     0,  -224,   225,     0,  -224,     0,     0,   226,  -224,
     227,     0,  -224,   228,     0,  -224,     0,     0,     0,     0,
       0,   229,     0,     0,  -215,     0,     0,   230,   231,     0,
       0,     0,     0,     0,   232,     0,     0,     0,  -215,  -215,
       0,  -215,     0,   233,     0,     0,  -215,     0,     0,     0,
       0,     0,   234,   235,  -215,   236,     0,   237,     0,   238,
    -215,     0,   239,     0,     0,  -215,   240,  -215,     0,   241,
    -215,     0,   242,     0,     0,     0,     0,     0,  -215,     0,
       0,  -146,     0,     0,  -215,  -215,     0,     0,     0,     0,
       0,  -215,     0,     0,     0,  -146,  -146,     0,  -146,     0,
    -215,     0,     0,  -146,     0,     0,     0,     0,     0,  -215,
    -215,  -146,  -215,     0,  -215,     0,  -215,  -146,     0,  -215,
       0,     0,  -146,  -215,  -146,     0,  -215,  -146,     0,  -215,
       0,     0,     0,     0,     0,  -146,     0,     0,  -142,     0,
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
  (!!((Yystate) == (-980)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      12,     2,   149,   135,    16,   388,   198,   213,    20,    58,
      59,   474,   444,   595,    26,   689,   218,   434,    30,    31,
      63,    33,    34,    35,    36,   792,   218,    84,   190,    22,
     764,   971,    44,    19,    46,     1,     1,   583,    32,    51,
     586,    53,   117,    55,   206,   628,     3,    32,    45,    67,
      32,    63,    64,    65,    66,    67,    68,    32,   101,   191,
      72,   263,    74,   138,    76,    77,     1,   142,  1013,    81,
      32,   263,    84,    85,   892,    87,    84,    89,   138,   889,
      92,    93,  1061,    95,   286,   449,    64,    99,   100,   101,
     165,   109,     1,    27,   286,   441,   846,   847,   112,   138,
     157,     1,    67,    45,   110,   162,    45,   167,    45,   138,
     474,    45,   318,   319,    48,   112,  1061,   851,   700,   702,
      55,  1100,   100,    33,    34,    45,   165,   112,   157,   157,
     112,    45,   138,   145,    68,   178,    45,    71,    95,    73,
    1080,   138,   112,    82,   109,   163,   293,    45,    82,    84,
     960,   117,   112,   167,   162,    55,   923,   169,   350,   893,
     172,   173,   174,   138,   176,   177,   178,   101,   162,   987,
     167,   137,   138,   719,    45,   110,   722,   162,   761,   946,
     162,   948,   384,   385,    84,    31,   198,   162,   138,     3,
       0,   101,   384,   385,    27,   627,   138,    97,   163,   165,
     162,   138,   137,   189,   138,   138,   218,   167,   117,   202,
     110,    82,   595,   163,   207,    18,    55,   865,   138,   867,
       3,   214,   138,   138,   138,   959,    82,   162,   137,   138,
     167,   165,   165,   138,  1001,    81,  1003,   137,    71,    85,
     138,   157,   164,   993,   994,    27,   138,   167,   157,    82,
     165,   263,   138,   167,   266,    76,    77,   157,   163,   933,
     677,    55,   162,   275,   276,   467,     3,    70,   101,   167,
      84,   110,    81,   165,   286,   467,    85,   138,  1012,   165,
     138,    95,   414,    20,   327,    88,   132,   419,   164,    71,
      84,   137,   874,   139,   140,   138,    33,   945,   144,   947,
      82,   313,   105,    97,   165,     8,   152,   165,    92,   310,
     132,    27,   158,   159,   160,   327,   110,   700,   140,   101,
      57,   162,   165,   755,   336,   107,   338,   339,   340,   341,
     342,   343,    31,   345,   346,   347,   348,   349,   138,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,    71,   117,    50,    95,   164,
     138,   373,   374,   157,   101,   165,    82,   379,   162,   138,
     382,   383,   384,   385,   386,   387,   388,   117,   421,   157,
     138,   142,    81,    76,   138,   397,    85,   138,   157,   581,
     402,   107,   404,   599,   600,   373,   374,    44,   138,   157,
     132,   379,   142,   157,   382,   383,   157,   753,   140,   421,
      77,   423,   424,   157,   157,   132,   138,   429,   430,    66,
     163,   433,   434,   140,   436,   165,   438,    55,   100,    76,
      77,   341,   342,   132,   133,   157,   899,   136,   137,   451,
     139,   140,    27,   455,   456,   144,   792,  1078,   460,   164,
     164,   166,    66,   152,  1018,   467,    84,   663,   664,   158,
     159,   160,   505,    81,   106,  1096,   157,    85,    27,    97,
     482,   156,   163,   485,   157,   487,   386,   387,    27,   491,
     492,   493,   110,   495,   496,   148,    71,   397,    47,   202,
     164,   874,   654,   505,   207,   157,    45,    82,   157,    48,
     501,  1065,  1066,  1067,  1068,  1069,  1070,   485,   157,   487,
      69,   117,    71,   157,   424,   164,   101,   166,   111,    68,
     430,    31,    71,    82,    73,   164,   198,   166,   892,   157,
     164,   157,   166,    82,   162,   899,   485,   143,   487,   145,
     146,   188,   689,   149,   150,   164,   218,   166,   107,   164,
     157,   166,   101,   565,   566,   567,   568,   569,   570,   571,
     157,    31,   162,   210,   211,   212,   213,   579,   132,   581,
     217,    81,    80,   137,    31,    85,   140,   589,   157,   591,
     144,   491,   928,   595,     2,   597,   572,   573,    80,   164,
     633,   263,   164,   164,    12,   164,   210,   211,   212,   213,
     946,   579,   948,   157,   164,   117,   253,    25,     8,    27,
     157,    81,   259,    59,   286,    85,   137,   162,    55,   631,
     632,   633,   132,   987,    81,   637,   140,   137,    85,   139,
     140,   157,   117,   165,   144,   282,   138,   157,    45,   253,
     164,    34,   152,   165,   138,   259,   165,    84,   117,  1013,
     160,    21,   165,   300,    72,  1001,   566,  1003,   568,   637,
      97,   165,   132,  1027,   676,   677,   138,   137,   282,   167,
     140,   167,   162,   110,   144,   132,   133,   164,   135,   136,
     137,    99,   139,   140,   157,   103,   300,   144,   700,   157,
     157,   703,   704,   705,   157,   152,   708,  1061,   900,   901,
     165,   158,   159,   160,   157,   167,   157,   138,   900,   901,
     157,    34,   384,   385,  1078,   157,   728,   165,   730,   163,
     157,   138,   165,   735,   163,   162,   738,   117,     1,   741,
     773,   743,  1096,    82,   746,   165,  1100,    32,    32,   167,
     162,   162,    34,   137,    82,   137,   132,   165,   167,    22,
     164,   398,   164,    26,    27,   157,   167,   167,    45,   157,
     163,   773,    35,   164,    37,    38,   140,   140,    55,    42,
      34,   783,   140,   785,    47,   140,   788,    50,   140,   140,
     132,   428,   794,   140,   398,   432,   933,   140,   460,   140,
     132,   162,   140,   703,   704,   467,    69,    84,    71,   140,
      73,   132,    75,    76,   132,    78,   140,    80,   162,    82,
     482,    97,   132,   485,   428,   487,   132,   167,   432,   165,
     492,   165,   165,   110,   156,   243,   165,   156,   101,   841,
     117,   104,   250,   138,   107,   324,    24,   677,   676,   294,
     795,   777,   933,   855,   856,   928,   858,   859,   860,   861,
     753,   138,   620,   798,    31,  1080,  1012,   869,   870,  1038,
     872,  1071,   874,  1064,   876,   877,    35,    30,   880,   452,
     157,   920,   921,   581,    -1,   162,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   157,    -1,   898,   899,   900,   901,
     163,    -1,   310,    -1,   906,    27,   908,    -1,    -1,    -1,
     912,    -1,    -1,    -1,    81,    -1,   324,   579,    85,   581,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,   961,    -1,
      -1,    -1,    -1,    -1,    -1,   937,   938,   939,   940,   941,
     942,   943,   944,    -1,    -1,    -1,    68,    -1,    -1,    71,
      -1,    73,    -1,    -1,   956,    -1,   593,   594,    -1,   961,
      82,    -1,   599,   600,   966,   132,   133,    -1,   970,    -1,
     137,    -1,   139,   140,   611,    -1,    -1,   144,    -1,   101,
      -1,    -1,    -1,    -1,    -1,   152,    -1,   395,    -1,   593,
     594,   158,   159,   160,    -1,   599,   600,    -1,    -1,    -1,
    1033,    -1,    -1,  1005,  1006,  1007,  1008,    22,    -1,    -1,
      -1,    26,    27,   650,    -1,   423,    -1,    -1,    -1,    -1,
      35,   429,    37,    38,    -1,  1027,    -1,    42,   436,  1031,
    1032,  1033,    47,    -1,    -1,    -1,    -1,     2,    -1,    -1,
    1042,  1043,  1044,  1045,  1046,  1047,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    71,    -1,  1060,  1061,
      25,  1063,    27,    -1,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   713,   714,    -1,    -1,
      -1,   489,    -1,    -1,    -1,   493,   101,    -1,   725,   104,
     727,    -1,   107,   501,  1096,    -1,    -1,   734,  1100,    -1,
     737,    -1,    -1,   740,    -1,    -1,    -1,    72,    -1,   713,
     714,    76,    77,    -1,    -1,    -1,    45,    -1,    -1,     2,
      -1,   725,    -1,    -1,    -1,    -1,    55,    -1,    -1,    12,
      -1,    -1,    -1,   737,    99,    -1,   740,    -1,   103,    45,
      -1,    -1,    25,   780,    27,    -1,    -1,    -1,     2,    55,
      -1,    -1,    22,    -1,   562,    84,    26,    27,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,
      -1,    25,    42,    27,    -1,    -1,    -1,    47,    84,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,   117,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,
     165,    71,   610,    73,   110,    75,    -1,    -1,    78,   138,
      80,   117,    82,    -1,    -1,    -1,    99,    -1,    72,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
     892,   101,   138,   162,   104,    -1,   165,   107,   900,   901,
      -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,   157,    -1,    -1,   662,    -1,   162,    -1,    -1,   165,
     668,    -1,    -1,    -1,    -1,    -1,    -1,   904,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   243,    -1,
      -1,   689,   165,    -1,    -1,   250,    -1,   157,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   703,   704,    -1,    -1,    -1,
     904,    -1,     3,    -1,   966,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,   950,   951,    -1,    17,   954,   955,    20,
     728,    -1,   730,    -1,    25,    -1,    -1,   735,    -1,    -1,
     738,    -1,    -1,    -1,   971,    -1,    -1,   745,    -1,    -1,
     748,    -1,   979,    44,   981,   310,   983,     2,    49,    -1,
      -1,  1013,    -1,    -1,    -1,    -1,    57,    12,    -1,   324,
     243,    -1,    -1,    64,   772,    -1,    -1,   250,    -1,    -1,
      25,    -1,    27,    -1,    -1,   783,    -1,   785,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   243,
      91,    -1,    -1,    22,    95,    -1,   250,    26,    27,  1061,
      -1,   102,    -1,    -1,    -1,    -1,    35,   108,    37,    38,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    72,    47,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,   310,    -1,     2,
     395,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1100,    12,
      69,   324,    71,  1080,    99,    -1,    -1,    76,   103,    -1,
      -1,    80,    25,    82,    27,    -1,   310,    -1,   423,    -1,
      -1,   869,    -1,    -1,   429,    -1,    -1,    -1,   876,    -1,
     324,   436,   101,    -1,    -1,   104,    -1,   106,   107,    -1,
     888,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    -1,   902,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,   911,    -1,    17,    18,   915,    20,    -1,
     165,    -1,   395,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,   489,    -1,    99,    39,   493,    -1,
     103,    -1,    44,    -1,    46,    -1,   501,    49,    -1,    -1,
     423,   395,    -1,    -1,    -1,    57,   429,    -1,    -1,   957,
      -1,    63,    64,   436,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,   423,
      -1,    -1,    -1,    -1,    -1,   429,    88,    89,    -1,    91,
      -1,    93,   436,    95,    -1,    -1,    98,    -1,   243,    -1,
     102,   103,   165,   105,    -1,   250,   108,  1005,  1006,  1007,
    1008,    -1,    -1,    -1,    -1,    -1,   489,    -1,    -1,    -1,
     493,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   501,    -1,
      -1,    -1,    -1,  1031,  1032,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   489,    -1,    -1,    -1,   493,
      -1,    -1,    -1,    -1,     3,   610,    -1,   501,    -1,    -1,
      -1,    -1,   164,    -1,    -1,   310,    -1,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    22,   324,
     243,    -1,    26,    27,    33,    -1,    -1,   250,    -1,    -1,
      39,    35,    -1,    37,    38,    44,    -1,    46,    42,    -1,
      49,    -1,    -1,    47,    -1,    -1,    50,   662,    57,    -1,
      -1,    -1,    -1,   668,    63,    64,    -1,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    69,    -1,    71,    -1,    -1,
      79,    -1,    76,    -1,    -1,    -1,    80,   610,    82,    88,
      89,    -1,    91,    -1,    93,    -1,    95,   310,    -1,    98,
     395,    -1,    -1,   102,    -1,    -1,   105,   101,    -1,   108,
     104,   324,   106,   107,    -1,    -1,   610,    -1,    -1,    -1,
      -1,    -1,    -1,   728,    -1,   730,    -1,    -1,   423,    -1,
     735,    -1,    -1,   738,   429,    -1,    -1,    -1,    -1,   662,
     745,   436,    -1,   748,    -1,   668,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,   162,    -1,   164,    -1,   772,   662,    -1,
      -1,    -1,    -1,    -1,   668,    -1,    -1,   148,   783,    -1,
     785,    -1,   395,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    -1,   164,   489,   166,    -1,    -1,   493,    -1,
      -1,    -1,    -1,    -1,    -1,   728,   501,   730,    -1,    -1,
     423,    -1,   735,    -1,    -1,   738,   429,    -1,    -1,    -1,
      -1,    -1,   745,   436,    -1,   748,    -1,    -1,    -1,    -1,
      -1,    -1,     2,     2,   728,    -1,   730,    -1,    -1,    -1,
      -1,   735,    12,    12,   738,    -1,    -1,    -1,    -1,   772,
      -1,   745,    -1,    -1,   748,    25,    25,    27,    27,    -1,
     783,    -1,   785,    -1,   869,    -1,    -1,    -1,    -1,    -1,
      -1,   876,    -1,    -1,    -1,    -1,   489,    -1,   772,    -1,
     493,    -1,    -1,   888,    -1,    -1,    -1,    -1,   501,   783,
      -1,   785,    -1,    -1,    -1,    -1,    -1,   902,    -1,    -1,
      -1,    -1,    72,    72,    -1,    -1,   911,    -1,    -1,    -1,
     915,    -1,    -1,    -1,    -1,   610,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      99,    -1,    -1,   103,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   869,    -1,    -1,    -1,
      -1,    -1,   957,   876,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,   888,    -1,   662,    -1,    -1,
      -1,    -1,    -1,   668,    -1,   869,    -1,    -1,    -1,   902,
      -1,    -1,   876,    -1,    -1,    -1,    -1,    -1,   911,    -1,
      -1,    -1,   915,    -1,   888,   165,   165,   610,    -1,    -1,
    1005,  1006,  1007,  1008,    -1,    -1,    -1,    -1,   902,   128,
      -1,   130,    -1,    -1,    -1,    -1,    -1,   911,    -1,    -1,
      -1,   915,    -1,    -1,    -1,    -1,  1031,  1032,    -1,    -1,
      -1,    -1,    -1,   728,   957,   730,    -1,    -1,    -1,    -1,
     735,    -1,    -1,   738,    -1,    -1,    -1,    -1,    -1,   662,
     745,    -1,    -1,   748,    -1,   668,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   957,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,   243,   243,    -1,    -1,   772,    -1,    -1,
     250,   250,  1005,  1006,  1007,  1008,    -1,   206,   783,   208,
     785,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1031,  1032,
      -1,  1005,  1006,  1007,  1008,   728,    -1,   730,    -1,    -1,
       2,    -1,   735,    -1,    -1,   738,    -1,   246,    -1,    -1,
      12,    -1,   745,    -1,    -1,   748,    -1,  1031,  1032,    -1,
     310,   310,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,   271,    22,   324,   324,    -1,    26,    27,   772,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,
     783,    -1,   785,    42,   869,    -1,    -1,    -1,    47,    -1,
      -1,   876,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    -1,   888,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    71,    -1,    73,    -1,    75,   902,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,   911,    99,    -1,    -1,
     915,   103,    -1,    -1,    -1,   395,   395,    -1,    -1,    -1,
      -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   423,   423,    -1,   869,    -1,    -1,   429,
     429,    -1,   957,   876,    -1,    -1,   436,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   888,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,   157,   902,
      -1,    -1,    -1,   412,   163,    -1,    -1,   416,   911,    -1,
      -1,    -1,   915,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1005,  1006,  1007,  1008,    -1,    -1,    -1,    -1,    -1,   489,
     489,    -1,    -1,   493,   493,   444,    -1,    -1,    -1,    -1,
      -1,   501,   501,    -1,    -1,    -1,  1031,  1032,    -1,    -1,
      -1,    -1,    -1,    -1,   957,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,   243,    -1,    -1,    -1,    -1,    -1,    -1,   250,    -1,
      -1,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,   502,    -1,    -1,    -1,    -1,   507,    -1,
      33,    -1,  1005,  1006,  1007,  1008,    39,    -1,    -1,    -1,
      -1,    44,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,  1031,  1032,
      63,    64,    -1,    -1,    31,    -1,    -1,    70,   310,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
     610,   610,   324,    -1,    -1,    88,    89,    -1,    91,    -1,
      93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,   102,
      -1,    -1,   105,   582,    -1,   108,   585,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   662,   662,    -1,    -1,    -1,    -1,   668,   668,
      -1,    -1,    -1,    -1,   623,    -1,    -1,    -1,   627,   116,
      -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    -1,    -1,    -1,   132,   133,    -1,   135,   136,
     137,   650,   139,   140,   141,   654,   143,   144,   145,   146,
      -1,   423,   149,   150,   151,   152,   153,   429,   155,    -1,
      -1,   158,   159,   160,   436,    -1,    -1,    -1,   728,   728,
     730,   730,    -1,    -1,    -1,   735,   735,    -1,   738,   738,
     689,    -1,    -1,    -1,    -1,   745,   745,    -1,   748,   748,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   772,   772,    -1,    -1,    -1,   489,     3,    -1,
      -1,   493,    -1,   783,   783,   785,   785,    -1,    -1,   501,
      -1,    -1,    17,    18,   161,    20,    -1,   164,    -1,   166,
      25,    -1,    -1,   752,    -1,    -1,   755,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      -1,    46,    -1,    -1,    49,    31,    -1,    -1,   777,    -1,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    63,    64,
      12,    -1,    -1,    -1,    -1,    70,   795,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    88,    89,    -1,    91,    -1,    93,   869,
      95,    -1,   821,    98,    -1,    81,   876,   102,    -1,    85,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,   888,   888,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   610,    -1,
      -1,    -1,   902,   902,    -1,    -1,    -1,    -1,    -1,    81,
      -1,   911,   911,    85,    -1,   915,   915,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,    -1,   135,
     136,   137,    -1,   139,   140,   141,    -1,    -1,   144,   164,
     112,    -1,    -1,    -1,   116,   151,   152,    -1,    -1,   155,
     662,    -1,   158,   159,   160,    -1,   668,   957,   957,    -1,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,   933,    -1,   158,   159,   160,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,  1005,  1006,  1007,  1008,    -1,
      35,    -1,    37,    38,    -1,    -1,   728,    42,   730,    -1,
      -1,    -1,    47,   735,    -1,    -1,   738,    -1,    -1,    -1,
      -1,  1031,  1032,   745,    -1,    -1,   748,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    71,    -1,    73,    -1,
      75,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
     772,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   783,    -1,   785,    -1,    -1,   101,    -1,    -1,   104,
      -1,     1,   107,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    46,    -1,    12,    49,
      -1,    -1,   157,    53,    54,    19,    -1,    57,    58,    -1,
      60,    61,    -1,    63,    64,    -1,    -1,    31,    -1,    -1,
      70,    -1,    72,    -1,    74,    -1,    -1,   869,    -1,    79,
      80,    -1,    -1,    -1,   876,    -1,    -1,    -1,    88,    89,
      90,    91,    92,    93,    -1,    95,   888,    -1,    98,    -1,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,
     902,    -1,    -1,    -1,    -1,    -1,   116,    81,    -1,   911,
      -1,    85,    -1,   915,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   116,    -1,   154,   155,   156,    -1,    -1,    -1,
     160,    -1,   162,    -1,   164,   957,   166,   167,   132,   133,
      -1,   135,   136,   137,    -1,   139,   140,   141,    -1,   143,
     144,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1005,  1006,  1007,  1008,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,     1,    -1,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,  1031,
    1032,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    -1,    46,    47,    -1,    49,    50,    -1,    52,
      53,    54,    -1,    56,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    -1,    -1,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    86,    87,    88,    89,    90,    91,    92,
      93,    -1,    95,    96,    -1,    98,    99,   100,   101,   102,
      -1,   104,   105,   106,   107,   108,    -1,    -1,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,   154,   155,   156,   157,    -1,    -1,   160,    -1,   162,
       1,   164,     3,   166,     5,     6,     7,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    46,    47,    -1,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    58,    59,    60,
      61,    -1,    63,    64,    65,    -1,    -1,    -1,    69,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    -1,    95,    96,    -1,    98,    99,   100,
     101,   102,    -1,   104,   105,   106,   107,   108,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      -1,   162,   163,   164,     1,   166,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
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
     157,    -1,    -1,   160,    -1,   162,   163,   164,     1,   166,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    -1,    46,    47,    -1,    49,    50,    -1,    52,
      53,    54,    -1,    56,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    -1,    -1,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    86,    87,    88,    89,    90,    91,    92,
      93,    -1,    95,    96,    -1,    98,    99,   100,   101,   102,
      -1,   104,   105,   106,   107,   108,    -1,    -1,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,   154,   155,   156,   157,    -1,    -1,   160,    -1,   162,
     163,   164,     1,   166,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    46,    47,    -1,
      49,    50,    -1,    52,    53,    54,    -1,    56,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    -1,    -1,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    86,    87,    88,
      89,    90,    91,    92,    93,    -1,    95,    96,    -1,    98,
      99,   100,   101,   102,    -1,   104,   105,   106,   107,   108,
      -1,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,    -1,    -1,   154,   155,   156,   157,    -1,
      -1,   160,    -1,   162,   163,   164,     1,   166,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      -1,    46,    47,    -1,    49,    50,    -1,    52,    53,    54,
      -1,    56,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    -1,    -1,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    86,    87,    88,    89,    90,    91,    92,    93,    -1,
      95,    96,    -1,    98,    99,   100,   101,   102,    -1,   104,
     105,   106,   107,   108,    -1,    -1,   111,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,
     155,   156,   157,    -1,    -1,   160,    -1,   162,   163,   164,
       1,   166,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    46,    47,    -1,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    58,    59,    60,
      61,    -1,    63,    64,    65,    -1,    -1,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    90,
      91,    92,    93,    -1,    95,    96,    -1,    98,    99,   100,
     101,   102,    -1,   104,   105,   106,   107,   108,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,    -1,    -1,   154,   155,   156,   157,    -1,    -1,   160,
      -1,   162,   163,   164,     1,   166,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
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
     157,    -1,    -1,   160,    -1,   162,     1,   164,     3,   166,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      -1,    46,    47,    -1,    49,    50,    -1,    52,    53,    54,
      -1,    56,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    -1,    -1,    -1,    69,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    86,    87,    88,    89,    90,    91,    92,    93,    -1,
      95,    96,    -1,    98,    99,   100,   101,   102,    -1,   104,
     105,   106,   107,   108,    -1,    -1,   111,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,
     155,   156,   157,    -1,    -1,   160,    -1,   162,     1,   164,
       3,   166,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    -1,    46,    47,    -1,    49,    50,    -1,    52,
      53,    54,    -1,    56,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    -1,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    86,    87,    88,    89,    90,    91,    92,
      93,    -1,    95,    96,    -1,    98,    99,   100,   101,   102,
      -1,   104,   105,   106,   107,   108,    -1,    -1,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,   154,   155,   156,   157,    -1,    -1,   160,    -1,   162,
       1,   164,     3,   166,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    46,    47,    -1,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    58,    59,    60,
      61,    -1,    63,    64,    65,    -1,    -1,    -1,    69,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    90,
      91,    92,    93,    -1,    95,    96,    -1,    98,    99,   100,
     101,   102,    -1,   104,   105,   106,   107,   108,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,    -1,    -1,   154,   155,   156,   157,    -1,    -1,   160,
      -1,   162,     1,   164,     3,   166,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    46,    47,    -1,
      49,    50,    -1,    52,    53,    54,    -1,    56,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    -1,    -1,    -1,
      69,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    86,    87,    88,
      89,    90,    91,    92,    93,    -1,    95,    96,    -1,    98,
      99,   100,   101,   102,    -1,   104,   105,   106,   107,   108,
      -1,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,    -1,    -1,   154,   155,   156,   157,    -1,
      -1,   160,    -1,   162,     1,   164,     3,   166,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    -1,    46,
      47,    -1,    49,    50,    -1,    52,    53,    54,    -1,    56,
      57,    58,    59,    60,    61,    -1,    63,    64,    65,    -1,
      -1,    -1,    69,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    86,
      87,    88,    89,    90,    91,    92,    93,    -1,    95,    96,
      -1,    98,    99,   100,   101,   102,    -1,   104,   105,   106,
     107,   108,    -1,    -1,   111,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,
     157,    -1,    -1,   160,    -1,   162,     1,   164,     3,   166,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      -1,    46,    47,    -1,    49,    50,    -1,    52,    53,    54,
      -1,    56,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    -1,    -1,    -1,    69,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    86,    87,    88,    89,    90,    91,    92,    93,    -1,
      95,    96,    -1,    98,    99,   100,   101,   102,    -1,   104,
     105,   106,   107,   108,    -1,    -1,   111,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,
     155,   156,   157,    -1,    -1,   160,    -1,   162,     1,   164,
       3,   166,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    -1,    46,    47,    -1,    49,    50,    -1,    52,
      53,    54,    -1,    56,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    -1,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    86,    87,    88,    89,    90,    91,    92,
      93,    -1,    95,    96,    -1,    98,    99,   100,   101,   102,
      -1,   104,   105,   106,   107,   108,    -1,    -1,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,   154,   155,   156,   157,    -1,    -1,   160,    -1,   162,
       1,   164,     3,   166,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    46,    47,    -1,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    58,    59,    60,
      61,    -1,    63,    64,    65,    -1,    -1,    -1,    69,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    90,
      91,    92,    93,    -1,    95,    96,    -1,    98,    99,   100,
     101,   102,    -1,   104,   105,   106,   107,   108,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,    -1,    -1,   154,   155,   156,   157,    -1,    -1,   160,
      -1,   162,     1,   164,     3,   166,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    46,    47,    -1,
      49,    50,    -1,    52,    53,    54,    -1,    56,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    -1,    -1,    -1,
      69,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    86,    87,    88,
      89,    90,    91,    92,    93,    -1,    95,    96,    -1,    98,
      99,   100,   101,   102,    -1,   104,   105,   106,   107,   108,
      -1,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,    -1,    -1,   154,   155,   156,   157,    -1,
      -1,   160,    -1,   162,     1,   164,     3,   166,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    -1,    46,
      47,    -1,    49,    50,    -1,    52,    53,    54,    -1,    56,
      57,    58,    59,    60,    61,    -1,    63,    64,    65,    -1,
      -1,    -1,    69,    70,    71,    72,    -1,    74,    75,    76,
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
      -1,   164,    -1,   166,     3,     4,     5,     6,     7,     8,
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
     164,    -1,   166,   167,     3,    -1,     5,     6,     7,     8,
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
      -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      54,    -1,    -1,    57,    58,    -1,    60,    61,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    -1,    -1,    79,    80,    -1,    82,    -1,
      -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,    93,
      -1,    95,    -1,    -1,    98,    -1,    -1,    -1,   102,    -1,
      -1,   105,    -1,   107,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
     134,   135,   136,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,
     154,   155,   156,    -1,    -1,    -1,   160,    -1,   162,    -1,
     164,    -1,   166,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,
      60,    61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,
      70,    -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,
      90,    91,    92,    93,    -1,    95,    -1,    -1,    98,    99,
     100,    -1,   102,   103,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,    -1,   154,   155,   156,    -1,    -1,    -1,
     160,    -1,   162,    -1,   164,    -1,   166,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,
      -1,    57,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    95,
      -1,    -1,    98,    99,   100,    -1,   102,   103,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,
     156,    -1,    -1,    -1,   160,    -1,   162,    -1,   164,   165,
     166,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,    61,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,
      72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,
      92,    93,    -1,    95,    -1,    -1,    98,    99,   100,    -1,
     102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,   154,   155,   156,    -1,    -1,    -1,   160,    -1,
     162,    -1,   164,   165,   166,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    45,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    54,    -1,    -1,    57,
      58,    -1,    60,    61,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    70,    -1,    72,    -1,    74,    -1,    -1,    -1,
      -1,    79,    80,    -1,    82,    -1,    -1,    -1,    -1,    -1,
      88,    89,    90,    91,    92,    93,    -1,    95,    -1,    -1,
      98,    -1,    -1,    -1,   102,    -1,    -1,   105,    -1,   107,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,    -1,
      -1,    -1,   160,    -1,   162,    -1,   164,     3,   166,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,
      -1,    57,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    95,
      -1,    -1,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,
     156,   157,    -1,    -1,   160,    -1,   162,    -1,   164,    -1,
     166,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,    61,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,
      72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,
      92,    93,    -1,    95,    -1,    -1,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,   154,   155,   156,    -1,    -1,    -1,   160,    -1,
     162,    -1,   164,     3,   166,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,
      60,    61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,
      70,    -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,
      90,    91,    92,    93,    -1,    95,    -1,    -1,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,    -1,   154,   155,   156,    -1,    -1,    -1,
     160,    -1,   162,    -1,   164,     3,   166,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    54,    -1,    -1,    57,
      58,    -1,    60,    61,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    70,    -1,    72,    -1,    74,    -1,    -1,    -1,
      -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    89,    90,    91,    92,    93,    -1,    95,    -1,    -1,
      98,    -1,    -1,   101,   102,    -1,    -1,   105,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,    -1,
      -1,    -1,   160,    -1,   162,    -1,   164,     3,   166,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,
      -1,    57,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,   114,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,
     156,    -1,    -1,    -1,   160,    -1,   162,    -1,   164,    -1,
     166,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,    61,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,
      72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,
      92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,   114,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,   154,   155,   156,    -1,    -1,    -1,   160,    -1,
     162,    -1,   164,     3,   166,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,
      60,    61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,
      70,    -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,
      90,    91,    92,    93,    -1,    95,    -1,    -1,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,    -1,   154,   155,   156,    -1,    -1,    -1,
     160,    -1,   162,    -1,   164,    -1,   166,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,
      -1,    57,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    12,   141,    -1,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,   151,    -1,    25,   154,   155,
     156,    -1,    -1,    -1,   160,    33,   162,   163,   164,    -1,
     166,    39,    -1,    -1,    -1,    -1,    44,    -1,    46,    -1,
      -1,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    89,    -1,    91,    -1,    93,    -1,    95,    -1,    -1,
      98,    -1,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,   119,   120,   121,   122,    -1,    -1,   125,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,    -1,
      -1,   139,    -1,    -1,    -1,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,    -1,   155,    -1,    -1,
     158,   159,   160,   161,    -1,     3,   164,     5,     6,     7,
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
     108,    -1,    -1,    -1,    -1,    -1,   114,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,    -1,
      -1,    -1,   160,    -1,   162,    -1,   164,     3,   166,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,
      -1,    57,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,
     156,    -1,    -1,    -1,   160,    -1,   162,    -1,   164,    -1,
     166,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,    61,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,
      72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,
      92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,   154,   155,   156,    -1,    -1,    -1,   160,    -1,
     162,    -1,   164,     3,   166,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,
      60,    61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,
      70,    -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,
      90,    91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,
      -1,   101,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,    -1,   154,   155,   156,    -1,    -1,    -1,
     160,    -1,   162,    -1,   164,    -1,   166,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,
      -1,    57,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,
     156,    -1,    -1,    -1,   160,    -1,   162,    -1,   164,    -1,
     166,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,    61,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,
      72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,
      92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,   154,   155,   156,    -1,    -1,    -1,   160,    -1,
     162,    -1,   164,    -1,   166,     3,     4,     5,     6,     7,
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
      -1,    -1,   160,    -1,   162,    -1,   164,     3,   166,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,
      -1,    57,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,
     156,    -1,    -1,    -1,   160,    -1,   162,    -1,   164,     3,
     166,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
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
     164,     3,   166,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,    61,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,
      72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,
      92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,   154,   155,   156,    -1,    -1,    -1,   160,    -1,
     162,    -1,   164,     3,   166,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,
      60,    61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,
      70,    -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,
      90,    91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,    -1,   154,   155,   156,    -1,    -1,    -1,
     160,    -1,   162,    -1,   164,     3,   166,     5,     6,     7,
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
      -1,    -1,   160,    -1,   162,    -1,   164,     3,   166,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,
      -1,    57,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,
     156,    -1,    -1,    -1,   160,    -1,   162,    -1,   164,     3,
     166,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
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
     164,     3,   166,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,    61,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,
      72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,
      92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,   154,   155,   156,    -1,    -1,    -1,   160,    -1,
     162,    -1,   164,     3,   166,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,
      60,    61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,
      70,    -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,
      90,    91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,    -1,   154,   155,   156,    -1,    -1,    -1,
     160,    -1,   162,    -1,   164,     3,   166,     5,     6,     7,
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
      -1,    -1,   160,    -1,   162,    -1,   164,     3,   166,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,
      -1,    57,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,
     156,    -1,    -1,    -1,   160,    -1,   162,    -1,   164,     3,
     166,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
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
     164,     3,   166,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,    61,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,
      72,    -1,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,
      92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,   154,   155,   156,    -1,    -1,    -1,   160,    -1,
     162,    -1,   164,     3,   166,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,
      60,    61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,
      70,    -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    79,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,
      90,    91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,    -1,   154,   155,   156,    -1,    -1,    -1,
     160,    -1,   162,    -1,   164,     3,   166,     5,     6,     7,
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
      -1,    -1,   160,    -1,   162,    -1,   164,     3,   166,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,
      -1,    57,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,
     156,    -1,    -1,    -1,   160,    -1,    -1,    -1,   164,     3,
     166,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
     134,   135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,   155,   156,    -1,    -1,    -1,   160,    -1,   162,    -1,
     164,    -1,   166,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    31,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    54,    -1,    -1,    57,    58,    -1,
      60,    61,    -1,    63,    64,    -1,    -1,    -1,    -1,    81,
      70,    -1,    72,    85,    74,    -1,    -1,    -1,    -1,    79,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,
      90,    91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,    -1,
      -1,    -1,   144,    -1,   134,    -1,    -1,    -1,   138,   151,
     152,    -1,   142,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,
      -1,    -1,   162,    -1,   164,   165,   166,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,    55,
      -1,    57,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    95,
      -1,    97,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,   108,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,   134,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
     156,   157,    -1,    -1,    -1,    -1,   162,    33,   164,    35,
     166,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,
      -1,    57,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,     3,   134,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
     156,    -1,    -1,    -1,    -1,    -1,   162,    33,   164,    35,
     166,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,
      -1,    57,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,   134,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
     156,    -1,    -1,    -1,    -1,    -1,   162,    33,   164,    35,
     166,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,
      -1,    57,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    95,
      -1,    -1,    98,    -1,     3,    -1,   102,    -1,    -1,   105,
      -1,    -1,   108,    12,    -1,    -1,    -1,    -1,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,   134,    -1,
      39,    -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    -1,
      49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,   164,    12,
     166,    70,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    88,
      89,    -1,    91,    -1,    93,    -1,    95,    -1,    -1,    98,
      -1,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
     119,   120,   121,   122,    -1,    -1,   125,   126,   127,   128,
      31,   130,   131,   132,   133,   134,   135,   136,    81,    -1,
     139,    -1,    85,    -1,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,    -1,   155,    12,    -1,   158,
     159,   160,   161,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    31,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,    -1,    12,   158,   159,   160,    -1,    -1,
      -1,    19,    -1,    -1,   167,    -1,    81,    -1,    -1,    -1,
      85,   132,   133,    31,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,   146,    -1,    -1,   149,   150,
     151,   152,   153,    -1,   155,    12,    -1,   158,   159,   160,
      -1,   116,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,   132,   133,    -1,
     135,   136,   137,    81,   139,   140,   141,    85,   143,   144,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,    12,    -1,   158,   159,   160,    -1,    -1,    19,    -1,
      -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      31,    -1,    -1,    -1,    81,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,
      -1,   139,   140,   141,    -1,   143,   144,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,   154,   155,    12,   116,
     158,   159,   160,    -1,    -1,    19,    -1,   165,    -1,    -1,
      81,    -1,    -1,    -1,    85,   132,   133,    31,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,    -1,
      -1,   158,   159,   160,    -1,   116,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,    -1,   135,   136,   137,    81,   139,   140,
     141,    85,   143,   144,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,    12,    -1,   158,   159,   160,
      -1,    -1,    19,    -1,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
      -1,   135,   136,   137,    -1,   139,   140,   141,    -1,   143,
     144,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    81,    -1,    -1,    -1,    85,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    94,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   121,   122,    -1,    -1,   125,   126,
     127,   128,    -1,   130,   131,   132,   133,    -1,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,    -1,
      81,   158,   159,   160,    85,   162,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    94,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
     121,   122,    -1,    -1,   125,   126,   127,   128,    -1,   130,
     131,   132,   133,    -1,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,    -1,    81,   158,   159,   160,
      85,   162,    -1,    12,    -1,    -1,    -1,    -1,    -1,    94,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,   121,   122,    -1,    -1,
     125,   126,   127,   128,    -1,   130,   131,   132,   133,    -1,
     135,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,    -1,    81,   158,   159,   160,    85,   162,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    94,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,   121,   122,    -1,    -1,   125,   126,   127,   128,
      -1,   130,   131,   132,   133,    -1,   135,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,   154,   155,    -1,    81,   158,
     159,   160,    85,   162,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,   121,   122,
      -1,    -1,   125,   126,   127,   128,    -1,   130,   131,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,    -1,    81,   158,   159,   160,    85,   162,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    94,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   121,   122,    -1,    -1,   125,   126,
     127,   128,    -1,   130,   131,   132,   133,    -1,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,    -1,
      81,   158,   159,   160,    85,   162,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    94,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
     121,   122,    -1,    -1,   125,   126,   127,   128,    -1,   130,
     131,   132,   133,    -1,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,    -1,    81,   158,   159,   160,
      85,   162,    -1,    12,    -1,    -1,    -1,    -1,    -1,    94,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,   121,   122,    -1,    -1,
     125,   126,   127,   128,    -1,   130,   131,   132,   133,    -1,
     135,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,    -1,    81,   158,   159,   160,    85,   162,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,   121,   122,    -1,    -1,   125,   126,   127,   128,
      -1,   130,   131,   132,   133,    -1,   135,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,   154,   155,    12,    -1,   158,
     159,   160,    -1,   162,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      45,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,   112,    81,    -1,
      -1,   116,    85,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,    -1,
     135,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,   116,   149,   150,   151,   152,   153,   154,
     155,    -1,    -1,   158,   159,   160,    -1,   162,    -1,   132,
     133,    -1,   135,   136,   137,    81,   139,   140,   141,    85,
     143,   144,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,    12,    -1,   158,   159,   160,    -1,   162,
      19,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,
     116,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,    -1,   135,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
      12,    -1,   158,   159,   160,    -1,   162,    19,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    85,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,    81,
     139,   140,   141,    85,   143,   144,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,   154,   155,    -1,    -1,   158,
     159,   160,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,   116,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,   146,   147,   116,   149,   150,   151,
     152,   153,   154,   155,    -1,    -1,   158,   159,   160,    -1,
     162,    -1,   132,   133,    -1,   135,   136,   137,    -1,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,    12,    -1,   158,   159,
     160,    -1,   162,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    81,    -1,    -1,
     116,    85,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,    45,   135,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,   116,   149,   150,   151,   152,   153,   154,   155,
      -1,    -1,   158,   159,   160,    -1,   162,    -1,   132,   133,
      -1,   135,   136,   137,    81,   139,   140,   141,    85,   143,
     144,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,    12,   157,   158,   159,   160,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,    -1,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,    12,
      -1,   158,   159,   160,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    85,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    45,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,    45,   135,   136,   137,    81,   139,
     140,   141,    85,   143,   144,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,   116,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,   146,   147,   116,   149,   150,   151,   152,
     153,   154,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   132,   133,    -1,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,    12,    -1,   158,   159,   160,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    81,    -1,    -1,   116,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,    -1,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,   146,
     147,   116,   149,   150,   151,   152,   153,   154,   155,    -1,
      -1,   158,   159,   160,    -1,    81,    -1,   132,   133,    85,
     135,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,    12,   157,   158,   159,   160,    -1,    -1,    19,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,   132,   133,    -1,   135,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
      12,   157,   158,   159,   160,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      81,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      -1,   132,   133,    85,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
      -1,    81,    -1,   115,   116,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,   146,   147,   116,   149,   150,   151,
     152,   153,   154,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   132,   133,    -1,   135,   136,   137,    -1,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,    12,    -1,   158,   159,
     160,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,
      -1,    12,    47,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    69,    81,    71,    -1,    73,    85,
      75,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,   104,
     116,    31,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    85,    -1,   132,   133,    -1,   135,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
      -1,   112,   158,   159,   160,   116,    -1,    -1,    -1,    -1,
      -1,    81,   157,    -1,    -1,    85,    -1,    -1,   163,    -1,
      -1,   132,   133,    -1,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,    -1,   116,   158,   159,   160,
      -1,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,
      19,    -1,   132,   133,    -1,   135,   136,   137,    -1,   139,
     140,   141,    31,   143,   144,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,   154,   155,   116,    -1,   158,
     159,   160,    -1,    81,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,   154,   155,   115,   116,   158,
     159,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,
      -1,   139,   140,   141,    -1,   143,   144,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,   154,   155,    12,     3,
     158,   159,   160,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    31,    32,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      12,    -1,    -1,    -1,    -1,    39,    -1,    19,    -1,    -1,
      44,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    31,
      32,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    70,    81,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    89,    -1,    91,    -1,    93,
      -1,    95,    -1,    -1,    98,    -1,    -1,    -1,   102,    81,
      -1,   105,   116,    85,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
      -1,   135,   136,   137,    -1,   139,   140,   141,    -1,   143,
     144,   145,   146,   147,   116,   149,   150,   151,   152,   153,
     154,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,    12,    -1,   158,   159,   160,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    81,    -1,    -1,   116,    85,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,
      -1,   139,   140,   141,    -1,   143,   144,   145,   146,   147,
     116,   149,   150,   151,   152,   153,   154,   155,    -1,    -1,
     158,   159,   160,    -1,    -1,    -1,   132,   133,    -1,   135,
     136,   137,    81,   139,   140,   141,    85,   143,   144,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
      12,    -1,   158,   159,   160,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,   154,   155,    -1,    -1,   158,
     159,   160,    -1,    -1,    31,    -1,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    85,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    85,    -1,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,    -1,    81,   158,   159,   160,    85,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    94,    19,
      -1,    -1,    -1,    -1,    -1,   132,   133,    -1,   135,   136,
     137,    31,   139,   140,   141,    -1,    -1,   144,   145,   146,
     116,    -1,   149,   150,   151,   152,    -1,    -1,   155,    -1,
      -1,   158,   159,   160,    -1,    -1,   132,   133,    -1,   135,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
      -1,    81,   158,   159,   160,    85,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    94,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,    -1,   135,   136,   137,    -1,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,    -1,    81,   158,   159,
     160,    85,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      94,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
      -1,   135,   136,   137,    -1,   139,   140,   141,    -1,   143,
     144,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,    -1,    81,   158,   159,   160,    85,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    94,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,
      -1,   139,   140,   141,    -1,   143,   144,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,   154,   155,    -1,    81,
     158,   159,   160,    85,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,    -1,    81,   158,   159,   160,    85,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,    -1,   135,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
      -1,    81,   158,   159,   160,    85,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,    -1,   135,   136,   137,    -1,   139,
     140,   141,    -1,   143,   144,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,    -1,    81,   158,   159,
     160,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
      -1,   135,   136,   137,    -1,   139,   140,   141,    -1,   143,
     144,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,    -1,    22,   158,   159,   160,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    22,    37,    38,
      -1,    26,    27,    42,    -1,    -1,    -1,    -1,    47,    -1,
      35,    -1,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,
      -1,    31,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    78,
      -1,    80,    -1,    82,    69,    -1,    71,    -1,    73,    -1,
      75,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,    81,    -1,    -1,    -1,    85,   101,    -1,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,    -1,   132,   133,   163,   135,   136,   137,    -1,   139,
     140,   141,   157,   143,   144,   145,   146,    -1,   163,   149,
     150,   151,   152,   153,   154,   155,    -1,    -1,   158,   159,
     160,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,
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
      -1,    57,    -1,    -1,     3,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    17,    18,
      -1,    20,    -1,    79,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    88,    89,    33,    91,    -1,    93,    -1,    95,
      39,    -1,    98,    -1,    -1,    44,   102,    46,    -1,   105,
      49,    -1,   108,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      -1,     3,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,
      79,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    88,
      89,    33,    91,    -1,    93,    -1,    95,    39,    -1,    98,
      -1,    -1,    44,   102,    46,    -1,   105,    49,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,     3,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    79,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    88,    89,    33,    91,
      -1,    93,    -1,    95,    39,    -1,    98,    -1,    -1,    44,
     102,    46,    -1,   105,    49,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    89,    -1,    91,    -1,    93,    -1,
      95,    -1,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,   108
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
     188,   189,   190,   194,   196,   199,   200,   201,   202,   203,
     208,   209,   211,   212,   213,   217,   218,   221,   222,   225,
     226,   229,   232,   233,   258,   261,   262,   282,   283,   284,
     285,   286,   287,   288,   296,   297,   298,   299,   300,   303,
     304,   305,   306,   307,   308,   309,   311,   312,   313,   314,
     315,   157,   177,   300,   112,   289,   290,     3,   191,    14,
      22,    35,    40,    41,    43,    80,    93,   162,   166,   229,
     282,   287,   298,   299,   300,   303,   305,   306,   289,   300,
     101,   261,    82,   191,   177,   276,   300,   177,   164,     8,
      80,   300,   301,     8,    11,    80,   101,   301,    71,   114,
     210,   300,   210,   300,    26,   262,   300,   300,   164,     3,
      17,    18,    20,    25,    33,    39,    44,    46,    49,    57,
      63,    64,    70,    79,    88,    89,    91,    93,    95,    98,
     102,   105,   108,   193,   195,   230,   164,   193,   263,   264,
      32,   182,   198,   300,   300,    18,    70,    88,   105,   300,
     300,   300,     8,   164,    22,    35,    38,    80,   201,   201,
       4,   156,   252,   275,   300,    99,   100,   157,   300,   302,
     300,   198,   300,   300,   300,    92,   164,   177,   300,   300,
     182,   190,   300,   303,   182,   190,   300,   193,   259,   300,
     300,   300,   300,   300,   300,   300,   300,     1,   163,   175,
     183,   275,   103,   142,   252,   277,   278,   302,   210,   275,
     300,   310,    73,   177,   162,    77,   181,   106,   193,   193,
      50,    76,   247,   263,   156,   157,   148,   300,    12,    19,
      31,    81,    85,   116,   132,   133,   135,   136,   137,   139,
     140,   141,   143,   144,   145,   146,   147,   149,   150,   151,
     152,   153,   154,   155,   158,   159,   160,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   161,   239,   164,   166,    81,    85,   164,   177,
     157,   300,   300,   300,   275,   162,   182,    45,   289,   259,
     263,   157,   138,   157,   111,   194,   252,   279,   280,   281,
     302,    80,   206,   233,   261,    80,   204,   259,   233,   261,
     193,   164,   198,    32,    45,   198,   112,   198,   292,    32,
      45,   198,   292,    36,    66,   157,    94,   182,   239,   279,
     177,   164,   241,   103,   164,   193,   265,   266,     1,   137,
     270,    45,   138,   177,   198,   164,   164,   198,   279,   193,
     138,   157,   300,   300,   157,   162,   198,   164,   279,   157,
     214,   157,   214,   157,   117,   260,   157,   198,   157,   163,
     163,   175,   138,   163,   300,   138,   165,   138,   165,   167,
     292,    45,   138,   167,   292,   115,   138,   167,     8,     1,
     163,   183,    59,   275,   300,   137,   223,   162,   234,   157,
     300,   300,   300,   300,   210,   300,   210,   300,   300,   300,
     300,   300,   300,   300,    20,    33,    57,   101,   194,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   302,   302,   302,   302,   302,
     279,   279,   210,   300,   210,   300,    27,    45,    82,   107,
     291,   294,   295,   300,   315,    32,    45,    32,    45,    94,
      45,   167,   210,   300,   198,   157,   157,   300,   300,   117,
     165,   138,   207,   193,   263,   205,   193,   157,   263,    45,
     275,    43,   300,   210,   300,   164,   198,    43,   300,   210,
     300,   198,   160,   184,   186,   300,   184,   185,   177,   300,
      34,   300,   165,    27,    45,    48,    68,    71,    82,   101,
     176,   242,   243,   244,   245,   231,   266,   138,   165,    33,
      46,    89,    93,   166,   197,   271,   283,   117,   267,   300,
     264,   300,   300,   165,   252,   300,     1,   219,   279,   165,
      21,   215,   271,   283,   138,   163,   165,   165,   277,   165,
     277,   177,   167,   210,   300,   167,   177,   300,   167,   300,
     167,   300,   163,   163,   193,   157,    36,    66,   275,   162,
       1,   193,   227,   228,    27,    71,    82,   101,   236,   246,
     164,   157,   157,   157,   157,   157,   165,   167,    45,    82,
     138,   165,   282,    81,    81,    43,   210,   300,    43,   210,
     300,   210,   300,   289,   289,   157,   252,   302,   281,   193,
     223,   157,   193,   223,   157,   300,   165,   300,    32,   198,
      32,   198,   293,   294,   300,    32,   198,   292,    32,   198,
     292,   138,   157,    13,   157,    34,    34,   177,    94,   182,
      45,    82,   244,   138,   165,   164,   193,    27,    71,    82,
     101,   248,   165,   266,   270,     1,   275,    62,   302,   165,
     165,   163,    67,   109,   163,   220,   165,   164,   182,   193,
     216,   259,   177,   167,   292,   167,   292,   177,   115,   186,
     185,     1,   224,   163,   117,   138,   163,    82,   235,     1,
       3,    12,    17,    19,    20,    25,    39,    44,    49,    51,
      57,    63,    64,    79,    91,    95,    98,   102,   108,   132,
     133,   134,   135,   136,   139,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   155,   158,   159,   160,
     161,   164,   192,   193,   195,   237,   238,   239,   282,   165,
     294,   270,   282,   282,   300,    32,    32,   300,    32,    32,
     167,   167,   198,   198,   223,   162,   223,   162,   198,    94,
      43,   300,    43,   300,   138,   165,    94,    43,   300,   198,
      43,   300,   198,   300,   300,   177,   300,   177,    34,   193,
     243,   266,   137,   274,    82,   270,   267,   167,    45,   167,
     164,   164,    32,   177,   275,   216,   137,   182,    43,   177,
     300,   167,    43,   177,   300,   167,   300,   157,   157,   163,
      75,    78,   163,   176,   200,   300,   228,   248,   164,   240,
     193,   300,   132,   140,   240,   240,   267,    94,    43,    43,
      94,    43,    43,    43,    43,   162,   224,   162,   224,   300,
     300,   300,   294,   300,   300,   300,    13,    34,   177,   274,
     165,   166,   197,   252,   273,   283,   142,   253,   267,    55,
     110,   254,   300,   271,   283,   279,   279,   177,   198,   165,
     300,   300,   177,   300,   177,   200,     1,   137,   269,   242,
     165,     3,    95,   238,   239,   300,   300,   300,   300,   300,
     300,   224,   163,   224,   163,    94,    94,    94,    94,   300,
     177,   267,   274,   167,   275,   252,   300,     3,    84,    95,
     255,   256,   257,   300,   182,   199,   251,   167,   165,   165,
     182,    94,    94,   166,   197,   268,   283,    97,   249,   165,
     240,   240,    94,    94,    94,    94,    94,    94,   163,   163,
     300,   300,   300,   300,   253,   267,   252,   272,   273,   283,
      45,   167,   257,   110,   138,   117,   143,   145,   146,   149,
     150,    55,   283,   156,   156,   300,   300,     1,   167,   275,
     254,   300,   272,   273,   300,   256,   257,   257,   257,   257,
     257,   257,   255,   167,   268,   283,   167,   157,   250,   251,
     167,   268,   283,   272
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
#line 496 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 502 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 509 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 514 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 515 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 534 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 535 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 540 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 542 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 543 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 544 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 545 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 546 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 547 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 548 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 549 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 554 "chapel.ypp"
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
#line 567 "chapel.ypp"
    { (yyvsp[(1) - (3)].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (3)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (3)]).prevModule;  // restore previous module name
    }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 572 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = (yyvsp[(3) - (4)].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 584 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 585 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 590 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 591 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 605 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 611 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 617 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 622 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 627 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 631 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 638 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 647 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 656 "chapel.ypp"
    { (yyval.b) = (currentModuleType != MOD_INTERNAL ? true : false); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 657 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 658 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 662 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (4)].pcallexpr), (yyvsp[(1) - (4)].b)); }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 663 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), (yyvsp[(5) - (6)].ponlylist), true, (yyvsp[(1) - (6)].b)); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 664 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), (yyvsp[(5) - (6)].ponlylist), false, (yyvsp[(1) - (6)].b)); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 668 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 673 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 675 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 677 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 679 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 681 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 687 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 694 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 702 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 703 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 704 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 705 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 717 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 718 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 729 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 730 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 731 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 732 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 733 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 734 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 737 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 738 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 739 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 740 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 742 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 743 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 744 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 745 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 758 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 759 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 760 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 761 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 768 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 770 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 771 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 775 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 776 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 780 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 781 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 785 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 787 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 788 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 802 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 803 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 804 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 805 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 810 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 815 "chapel.ypp"
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
#line 825 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 830 "chapel.ypp"
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
#line 841 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 846 "chapel.ypp"
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
#line 856 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 861 "chapel.ypp"
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
#line 872 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 876 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 881 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 889 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 900 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 906 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 907 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 908 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 909 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 910 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 911 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 912 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 913 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 914 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 915 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 916 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 917 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 918 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 919 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 920 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 921 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 922 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 923 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 925 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 931 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 937 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 943 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 949 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 956 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 963 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 967 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 973 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 977 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 978 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 979 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 980 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 981 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 983 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 985 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 987 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 992 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 995 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 996 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 997 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 998 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 999 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1000 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1004 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1005 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1009 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1010 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1011 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1016 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1024 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1026 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1030 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1031 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1036 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1038 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1040 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1047 "chapel.ypp"
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
#line 1057 "chapel.ypp"
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
#line 1070 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1075 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1080 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1089 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1094 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1096 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1103 "chapel.ypp"
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
#line 1119 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1126 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1135 "chapel.ypp"
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
#line 1143 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1147 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1153 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1154 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1159 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1164 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1169 "chapel.ypp"
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
#line 1191 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1194 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1198 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1206 "chapel.ypp"
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
#line 1216 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1223 "chapel.ypp"
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
#line 1249 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1253 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1258 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1262 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1267 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1283 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1284 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1286 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1288 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1298 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1309 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1312 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1313 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1314 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1317 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1318 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1319 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1320 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1324 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1325 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1329 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1333 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1335 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1340 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1342 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1344 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1346 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1348 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1352 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1353 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1357 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1358 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1363 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1364 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1365 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1370 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1374 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1378 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1379 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1383 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1384 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1386 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1387 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1388 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1392 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1393 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1396 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1402 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1406 "chapel.ypp"
    { 
      DefExpr * de = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch)));
      USR_WARN(de, "support for space in query expressions is deprecated");
      (yyval.pexpr) = de;
  }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1411 "chapel.ypp"
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1415 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1416 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1417 "chapel.ypp"
    { if (DefExpr* def = toDefExpr((yyvsp[(2) - (2)].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
                         }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1425 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1427 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1429 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1431 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1438 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1440 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1444 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1445 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1446 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1447 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1448 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1449 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1450 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1455 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1457 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1462 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1464 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1466 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1471 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1484 "chapel.ypp"
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

  case 354:
/* Line 1792 of yacc.c  */
#line 1500 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1517 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1523 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1532 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1542 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1556 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1560 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1566 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1568 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1574 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1576 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1578 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1582 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1586 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1588 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1592 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1600 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1601 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1602 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1609 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1610 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1611 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1612 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1633 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1637 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1641 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1649 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1656 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1657 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1662 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1664 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1670 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1672 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1674 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1678 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1679 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1680 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1681 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1682 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1688 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1690 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1691 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1695 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1696 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1701 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1707 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1711 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1721 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1722 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1724 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1728 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1729 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1741 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1743 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1745 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1756 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1758 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1760 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1762 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1764 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1766 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1768 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1771 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1773 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1775 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1777 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1779 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1781 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1783 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1785 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1788 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1790 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1792 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1797 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1799 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1801 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1803 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1805 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1807 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1809 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1811 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1813 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1815 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1817 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1819 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1821 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1828 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1834 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1840 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1846 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1855 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1864 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1880 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1884 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1889 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1893 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1894 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1898 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1902 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1903 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1908 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1912 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1916 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1922 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1923 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1924 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1925 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1926 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1927 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1933 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1938 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1940 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1944 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1948 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 1952 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1960 "chapel.ypp"
    { (yyvsp[(1) - (2)].pcallexpr)->insertAtTail((yyvsp[(2) - (2)].pexpr));
      (yyval.pexpr) = (yyvsp[(1) - (2)].pcallexpr); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1965 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1971 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1977 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1984 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1993 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtOwned->symbol, (yyvsp[(4) - (5)].pexpr))); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1995 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtShared->symbol, (yyvsp[(4) - (5)].pexpr))); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 2000 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 2016 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 2018 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 2020 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 2022 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 2024 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 2026 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 2030 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 2031 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 2035 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 2036 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 2037 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 2055 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 2056 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 2057 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr));}
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 2058 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 2062 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 2063 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 2064 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 2068 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 2069 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 2070 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 2071 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 2072 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 2080 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 2081 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 2082 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 2083 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 2087 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 2088 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 2093 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 2094 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2095 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2096 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2097 "chapel.ypp"
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 2098 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2099 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 2100 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 2101 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2102 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2103 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2105 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2109 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2116 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2117 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2121 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2122 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2123 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2124 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2125 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2126 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2127 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2128 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2129 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2130 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 584:
/* Line 1792 of yacc.c  */
#line 2131 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 585:
/* Line 1792 of yacc.c  */
#line 2132 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 586:
/* Line 1792 of yacc.c  */
#line 2133 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 587:
/* Line 1792 of yacc.c  */
#line 2134 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 588:
/* Line 1792 of yacc.c  */
#line 2135 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 589:
/* Line 1792 of yacc.c  */
#line 2136 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 590:
/* Line 1792 of yacc.c  */
#line 2137 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 591:
/* Line 1792 of yacc.c  */
#line 2138 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 592:
/* Line 1792 of yacc.c  */
#line 2139 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 593:
/* Line 1792 of yacc.c  */
#line 2140 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 594:
/* Line 1792 of yacc.c  */
#line 2141 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 595:
/* Line 1792 of yacc.c  */
#line 2142 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 596:
/* Line 1792 of yacc.c  */
#line 2143 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 597:
/* Line 1792 of yacc.c  */
#line 2147 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 598:
/* Line 1792 of yacc.c  */
#line 2148 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 599:
/* Line 1792 of yacc.c  */
#line 2149 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 600:
/* Line 1792 of yacc.c  */
#line 2150 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 601:
/* Line 1792 of yacc.c  */
#line 2151 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 602:
/* Line 1792 of yacc.c  */
#line 2152 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 603:
/* Line 1792 of yacc.c  */
#line 2153 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 604:
/* Line 1792 of yacc.c  */
#line 2157 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 605:
/* Line 1792 of yacc.c  */
#line 2158 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 606:
/* Line 1792 of yacc.c  */
#line 2159 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 607:
/* Line 1792 of yacc.c  */
#line 2160 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 608:
/* Line 1792 of yacc.c  */
#line 2164 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 609:
/* Line 1792 of yacc.c  */
#line 2165 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 610:
/* Line 1792 of yacc.c  */
#line 2166 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 611:
/* Line 1792 of yacc.c  */
#line 2167 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 612:
/* Line 1792 of yacc.c  */
#line 2172 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 613:
/* Line 1792 of yacc.c  */
#line 2173 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 614:
/* Line 1792 of yacc.c  */
#line 2174 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 615:
/* Line 1792 of yacc.c  */
#line 2175 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 616:
/* Line 1792 of yacc.c  */
#line 2176 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 617:
/* Line 1792 of yacc.c  */
#line 2177 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 618:
/* Line 1792 of yacc.c  */
#line 2178 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 10183 "bison-chapel.cpp"
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


