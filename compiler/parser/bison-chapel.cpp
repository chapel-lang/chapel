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
    Flag                      flag;
    ProcIter                  procIter;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<OnlyRename*>* ponlylist;
  };

  #endif

/* Line 387 of yacc.c  */
#line 131 "chapel.ypp"

  #ifndef _BISON_CHAPEL_DEFINES_2_
  #define _BISON_CHAPEL_DEFINES_2_

  struct YYLTYPE {
    int         first_line;
    int         first_column;
    int         last_line;
    int         last_column;
    const char* comment;
  };

  #define YYLTYPE_IS_DECLARED 1
  #define YYLTYPE_IS_TRIVIAL  1

  #endif

/* Line 387 of yacc.c  */
#line 153 "chapel.ypp"

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
#line 235 "bison-chapel.cpp"

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
     TBORROWED = 270,
     TBY = 271,
     TCATCH = 272,
     TCLASS = 273,
     TCOBEGIN = 274,
     TCOFORALL = 275,
     TCONFIG = 276,
     TCONST = 277,
     TCONTINUE = 278,
     TDEFER = 279,
     TDELETE = 280,
     TDMAPPED = 281,
     TDO = 282,
     TDOMAIN = 283,
     TELSE = 284,
     TENUM = 285,
     TEXCEPT = 286,
     TEXPORT = 287,
     TEXTERN = 288,
     TFOR = 289,
     TFORALL = 290,
     TFORWARDING = 291,
     TIF = 292,
     TIN = 293,
     TINDEX = 294,
     TINLINE = 295,
     TINOUT = 296,
     TITER = 297,
     TLABEL = 298,
     TLAMBDA = 299,
     TLET = 300,
     TLOCAL = 301,
     TMINUSMINUS = 302,
     TMODULE = 303,
     TNEW = 304,
     TNIL = 305,
     TNOINIT = 306,
     TON = 307,
     TONLY = 308,
     TOTHERWISE = 309,
     TOUT = 310,
     TOVERRIDE = 311,
     TOWNED = 312,
     TPARAM = 313,
     TPLUSPLUS = 314,
     TPRAGMA = 315,
     TPRIMITIVE = 316,
     TPRIVATE = 317,
     TPROC = 318,
     TPROTOTYPE = 319,
     TPUBLIC = 320,
     TRECORD = 321,
     TREDUCE = 322,
     TREF = 323,
     TREQUIRE = 324,
     TRETURN = 325,
     TSCAN = 326,
     TSELECT = 327,
     TSERIAL = 328,
     TSHARED = 329,
     TSINGLE = 330,
     TSPARSE = 331,
     TSUBDOMAIN = 332,
     TSYNC = 333,
     TTHEN = 334,
     TTHROW = 335,
     TTHROWS = 336,
     TTRY = 337,
     TTRYBANG = 338,
     TTYPE = 339,
     TUNDERSCORE = 340,
     TUNION = 341,
     TUNMANAGED = 342,
     TUSE = 343,
     TVAR = 344,
     TWHEN = 345,
     TWHERE = 346,
     TWHILE = 347,
     TWITH = 348,
     TYIELD = 349,
     TZIP = 350,
     TALIAS = 351,
     TAND = 352,
     TASSIGN = 353,
     TASSIGNBAND = 354,
     TASSIGNBOR = 355,
     TASSIGNBXOR = 356,
     TASSIGNDIVIDE = 357,
     TASSIGNEXP = 358,
     TASSIGNLAND = 359,
     TASSIGNLOR = 360,
     TASSIGNMINUS = 361,
     TASSIGNMOD = 362,
     TASSIGNMULTIPLY = 363,
     TASSIGNPLUS = 364,
     TASSIGNSL = 365,
     TASSIGNSR = 366,
     TBAND = 367,
     TBNOT = 368,
     TBOR = 369,
     TBXOR = 370,
     TCOLON = 371,
     TCOMMA = 372,
     TDIVIDE = 373,
     TDOT = 374,
     TDOTDOT = 375,
     TDOTDOTDOT = 376,
     TEQUAL = 377,
     TEXP = 378,
     TGREATER = 379,
     TGREATEREQUAL = 380,
     THASH = 381,
     TLESS = 382,
     TLESSEQUAL = 383,
     TMINUS = 384,
     TMOD = 385,
     TNOT = 386,
     TNOTEQUAL = 387,
     TOR = 388,
     TPLUS = 389,
     TQUESTION = 390,
     TSEMI = 391,
     TSHIFTLEFT = 392,
     TSHIFTRIGHT = 393,
     TSTAR = 394,
     TSWAP = 395,
     TASSIGNREDUCE = 396,
     TIO = 397,
     TLCBR = 398,
     TRCBR = 399,
     TLP = 400,
     TRP = 401,
     TLSBR = 402,
     TRSBR = 403,
     TNOELSE = 404,
     TUMINUS = 405,
     TUPLUS = 406
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
#line 184 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);


/* Line 387 of yacc.c  */
#line 451 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 458 "bison-chapel.cpp"
/* Unqualified %code blocks.  */
/* Line 391 of yacc.c  */
#line 38 "chapel.ypp"

  #include <string>
  int         captureTokens;
  std::string captureString;

/* Line 391 of yacc.c  */
#line 192 "chapel.ypp"

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
    setupError(__FILE__, __LINE__, 3);

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
#line 537 "bison-chapel.cpp"

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
#define YYLAST   14482

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  152
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  128
/* YYNRULES -- Number of rules.  */
#define YYNRULES  538
/* YYNRULES -- Number of states.  */
#define YYNSTATES  992

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   406

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
     145,   146,   147,   148,   149,   150,   151
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
     114,   122,   130,   131,   133,   135,   136,   138,   141,   145,
     149,   151,   154,   156,   160,   164,   170,   171,   173,   175,
     177,   181,   187,   193,   197,   202,   207,   212,   217,   222,
     227,   232,   237,   242,   247,   252,   257,   262,   267,   272,
     277,   278,   280,   282,   283,   285,   288,   290,   293,   297,
     299,   301,   304,   307,   309,   311,   313,   315,   317,   319,
     323,   329,   335,   338,   341,   347,   351,   358,   365,   370,
     376,   382,   386,   390,   397,   403,   410,   416,   423,   427,
     432,   439,   447,   454,   462,   467,   473,   478,   483,   487,
     494,   500,   507,   513,   522,   530,   533,   537,   541,   544,
     547,   551,   555,   556,   559,   562,   566,   572,   574,   578,
     582,   588,   594,   595,   598,   602,   605,   609,   616,   625,
     632,   641,   643,   645,   647,   648,   651,   652,   655,   659,
     665,   671,   673,   675,   678,   682,   684,   688,   689,   690,
     699,   700,   702,   705,   708,   710,   711,   712,   723,   727,
     731,   737,   743,   745,   749,   751,   754,   756,   758,   760,
     762,   764,   766,   768,   770,   772,   774,   776,   778,   780,
     782,   784,   786,   788,   790,   792,   794,   796,   798,   800,
     802,   804,   806,   808,   810,   812,   814,   816,   818,   820,
     822,   824,   826,   827,   831,   835,   836,   838,   842,   847,
     853,   858,   865,   872,   873,   875,   877,   879,   881,   883,
     886,   889,   891,   893,   895,   896,   898,   900,   903,   905,
     907,   909,   911,   912,   914,   917,   919,   921,   923,   924,
     926,   928,   930,   932,   934,   937,   939,   940,   942,   945,
     948,   949,   952,   956,   961,   966,   969,   974,   975,   978,
     981,   986,   991,   996,  1002,  1007,  1008,  1010,  1012,  1014,
    1018,  1022,  1028,  1030,  1032,  1036,  1038,  1041,  1045,  1046,
    1049,  1052,  1056,  1059,  1064,  1068,  1072,  1077,  1081,  1082,
    1085,  1088,  1091,  1094,  1097,  1100,  1103,  1106,  1109,  1111,
    1112,  1115,  1118,  1121,  1124,  1127,  1130,  1133,  1136,  1139,
    1141,  1146,  1151,  1158,  1162,  1163,  1165,  1167,  1171,  1176,
    1180,  1185,  1192,  1193,  1196,  1199,  1202,  1205,  1208,  1211,
    1214,  1217,  1220,  1223,  1225,  1227,  1231,  1235,  1237,  1241,
    1243,  1245,  1247,  1251,  1255,  1256,  1258,  1260,  1264,  1268,
    1272,  1274,  1276,  1278,  1280,  1282,  1284,  1286,  1288,  1291,
    1296,  1301,  1306,  1312,  1315,  1318,  1321,  1324,  1327,  1330,
    1337,  1344,  1349,  1359,  1369,  1377,  1384,  1391,  1396,  1406,
    1416,  1424,  1429,  1436,  1443,  1453,  1463,  1470,  1472,  1474,
    1476,  1478,  1480,  1482,  1484,  1486,  1490,  1491,  1493,  1498,
    1500,  1504,  1509,  1511,  1515,  1520,  1524,  1528,  1530,  1532,
    1535,  1537,  1540,  1542,  1544,  1548,  1551,  1556,  1558,  1560,
    1562,  1564,  1566,  1568,  1570,  1572,  1577,  1581,  1585,  1588,
    1591,  1593,  1594,  1596,  1599,  1602,  1604,  1606,  1608,  1610,
    1612,  1614,  1616,  1621,  1626,  1631,  1635,  1639,  1643,  1647,
    1652,  1656,  1661,  1663,  1665,  1667,  1669,  1671,  1675,  1680,
    1684,  1689,  1693,  1698,  1702,  1708,  1712,  1716,  1720,  1724,
    1728,  1732,  1736,  1740,  1744,  1748,  1752,  1756,  1760,  1764,
    1768,  1772,  1776,  1780,  1784,  1788,  1792,  1796,  1800,  1803,
    1806,  1809,  1812,  1815,  1818,  1822,  1826,  1830,  1834,  1838,
    1842,  1846,  1850,  1852,  1854,  1856,  1858,  1860,  1862
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     153,     0,    -1,   154,    -1,    -1,   154,   155,    -1,   157,
      -1,   156,   157,    -1,    60,     7,    -1,   156,    60,     7,
      -1,   158,    -1,   161,    -1,   166,    -1,   167,    -1,   174,
      -1,   168,    -1,   177,    -1,   180,    -1,   178,    -1,   187,
      -1,   181,    -1,   182,    -1,   186,    -1,   173,    -1,   254,
     136,    -1,    12,   157,    -1,    13,   255,   157,    -1,    14,
     169,   136,    -1,    19,   255,   161,    -1,    23,   169,   136,
      -1,    25,   243,   136,    -1,    43,   170,   157,    -1,    46,
     265,   172,    -1,    46,   172,    -1,    52,   265,   172,    -1,
      73,   265,   172,    -1,    73,   172,    -1,    78,   157,    -1,
      94,   265,   136,    -1,     1,   136,    -1,   159,   160,    48,
     170,   143,   144,    -1,   159,   160,    48,   170,   143,   162,
     144,    -1,   159,   160,    48,   170,   143,     1,   144,    -1,
      -1,    65,    -1,    62,    -1,    -1,    64,    -1,   143,   144,
      -1,   143,   162,   144,    -1,   143,     1,   144,    -1,   155,
      -1,   162,   155,    -1,   265,    -1,   265,    11,   265,    -1,
     163,   117,   265,    -1,   163,   117,   265,    11,   265,    -1,
      -1,   163,    -1,   139,    -1,   163,    -1,    88,   242,   136,
      -1,    88,   265,    31,   165,   136,    -1,    88,   265,    53,
     164,   136,    -1,    69,   242,   136,    -1,   268,    98,   267,
     136,    -1,   268,   109,   267,   136,    -1,   268,   106,   267,
     136,    -1,   268,   108,   267,   136,    -1,   268,   102,   267,
     136,    -1,   268,   107,   267,   136,    -1,   268,   103,   267,
     136,    -1,   268,    99,   267,   136,    -1,   268,   100,   267,
     136,    -1,   268,   101,   267,   136,    -1,   268,   111,   267,
     136,    -1,   268,   110,   267,   136,    -1,   268,   140,   267,
     136,    -1,   268,   141,   267,   136,    -1,   268,   104,   267,
     136,    -1,   268,   105,   267,   136,    -1,    -1,   170,    -1,
       3,    -1,    -1,     7,    -1,    27,   157,    -1,   161,    -1,
      70,   136,    -1,    70,   267,   136,    -1,   136,    -1,   175,
      -1,    65,   175,    -1,    62,   175,    -1,   194,    -1,   225,
      -1,   190,    -1,   176,    -1,   202,    -1,   228,    -1,    36,
     265,   136,    -1,    36,   265,    31,   165,   136,    -1,    36,
     265,    53,   164,   136,    -1,    36,   228,    -1,    33,     9,
      -1,    27,   157,    92,   265,   136,    -1,    92,   265,   172,
      -1,    20,   265,    38,   265,   255,   172,    -1,    20,   265,
      38,   179,   255,   172,    -1,    20,   265,   255,   172,    -1,
      34,   265,    38,   265,   172,    -1,    34,   265,    38,   179,
     172,    -1,    34,   265,   172,    -1,    34,   179,   172,    -1,
      34,    58,   170,    38,   265,   172,    -1,    35,   265,    38,
     265,   172,    -1,    35,   265,    38,   265,   258,   172,    -1,
      35,   265,    38,   179,   172,    -1,    35,   265,    38,   179,
     258,   172,    -1,    35,   265,   172,    -1,    35,   265,   258,
     172,    -1,   147,   242,    38,   265,   148,   157,    -1,   147,
     242,    38,   265,   258,   148,   157,    -1,   147,   242,    38,
     179,   148,   157,    -1,   147,   242,    38,   179,   258,   148,
     157,    -1,   147,   242,   148,   157,    -1,   147,   242,   258,
     148,   157,    -1,    95,   145,   242,   146,    -1,    37,   265,
      79,   157,    -1,    37,   265,   161,    -1,    37,   265,    79,
     157,    29,   157,    -1,    37,   265,   161,    29,   157,    -1,
      37,   265,   208,   265,    79,   157,    -1,    37,   265,   208,
     265,   161,    -1,    37,   265,   208,   265,    79,   157,    29,
     157,    -1,    37,   265,   208,   265,   161,    29,   157,    -1,
      24,   157,    -1,    82,   265,   136,    -1,    83,   265,   136,
      -1,    82,   168,    -1,    83,   168,    -1,    82,   161,   183,
      -1,    83,   161,   183,    -1,    -1,   183,   184,    -1,    17,
     161,    -1,    17,   185,   161,    -1,    17,   145,   185,   146,
     161,    -1,   170,    -1,   170,   116,   265,    -1,    80,   265,
     136,    -1,    72,   265,   143,   188,   144,    -1,    72,   265,
     143,     1,   144,    -1,    -1,   188,   189,    -1,    90,   242,
     172,    -1,    54,   157,    -1,    54,    27,   157,    -1,   191,
     170,   192,   143,   193,   144,    -1,    33,   171,   191,   170,
     192,   143,   193,   144,    -1,   191,   170,   192,   143,     1,
     144,    -1,    33,   171,   191,   170,   192,   143,     1,   144,
      -1,    18,    -1,    66,    -1,    86,    -1,    -1,   116,   242,
      -1,    -1,   193,   174,    -1,   193,   156,   174,    -1,   195,
     170,   143,   196,   144,    -1,   195,   170,   143,     1,   144,
      -1,    30,    -1,   197,    -1,   196,   117,    -1,   196,   117,
     197,    -1,   170,    -1,   170,    98,   265,    -1,    -1,    -1,
      44,   199,   210,   200,   217,   237,   224,   220,    -1,    -1,
      40,    -1,    32,   266,    -1,    33,   266,    -1,    56,    -1,
      -1,    -1,   201,   216,   203,   205,   204,   217,   236,   218,
     224,   219,    -1,   215,   207,   209,    -1,   215,   208,   209,
      -1,   215,   206,   119,   207,   209,    -1,   215,   206,   119,
     208,   209,    -1,   249,    -1,   145,   265,   146,    -1,   170,
      -1,   113,   170,    -1,   112,    -1,   114,    -1,   115,    -1,
     113,    -1,   122,    -1,   132,    -1,   128,    -1,   125,    -1,
     127,    -1,   124,    -1,   134,    -1,   129,    -1,   139,    -1,
     118,    -1,   137,    -1,   138,    -1,   130,    -1,   123,    -1,
     131,    -1,    16,    -1,   126,    -1,    10,    -1,   140,    -1,
     142,    -1,    98,    -1,   109,    -1,   106,    -1,   108,    -1,
     102,    -1,   107,    -1,   103,    -1,    99,    -1,   100,    -1,
     101,    -1,   111,    -1,   110,    -1,    -1,   145,   211,   146,
      -1,   145,   211,   146,    -1,    -1,   212,    -1,   211,   117,
     212,    -1,   213,   170,   241,   234,    -1,   156,   213,   170,
     241,   234,    -1,   213,   170,   241,   223,    -1,   213,   145,
     233,   146,   241,   234,    -1,   213,   145,   233,   146,   241,
     223,    -1,    -1,   214,    -1,    38,    -1,    41,    -1,    55,
      -1,    22,    -1,    22,    38,    -1,    22,    68,    -1,    58,
      -1,    68,    -1,    84,    -1,    -1,    58,    -1,    68,    -1,
      22,    68,    -1,    22,    -1,    84,    -1,    63,    -1,    42,
      -1,    -1,    22,    -1,    22,    68,    -1,    68,    -1,    58,
      -1,    84,    -1,    -1,    81,    -1,   136,    -1,   220,    -1,
     161,    -1,   173,    -1,   135,   170,    -1,   135,    -1,    -1,
     221,    -1,   121,   265,    -1,   121,   222,    -1,    -1,    91,
     265,    -1,    84,   226,   136,    -1,    21,    84,   226,   136,
      -1,    33,    84,   226,   136,    -1,   170,   227,    -1,   170,
     227,   117,   226,    -1,    -1,    98,   250,    -1,    98,   238,
      -1,   229,    58,   230,   136,    -1,   229,    22,   230,   136,
      -1,   229,    68,   230,   136,    -1,   229,    22,    68,   230,
     136,    -1,   229,    89,   230,   136,    -1,    -1,    21,    -1,
      33,    -1,   231,    -1,   230,   117,   231,    -1,   170,   237,
     234,    -1,   145,   233,   146,   237,   234,    -1,    85,    -1,
     170,    -1,   145,   233,   146,    -1,   232,    -1,   232,   117,
      -1,   232,   117,   233,    -1,    -1,    98,    51,    -1,    98,
     267,    -1,   147,   148,   250,    -1,   147,   148,    -1,   147,
     242,   148,   250,    -1,   147,   242,   148,    -1,   147,   148,
     235,    -1,   147,   242,   148,   235,    -1,   147,     1,   148,
      -1,    -1,   116,   250,    -1,   116,   235,    -1,   116,    28,
      -1,   116,    75,    -1,   116,    78,    -1,   116,    15,    -1,
     116,    57,    -1,   116,    87,    -1,   116,    74,    -1,     1,
      -1,    -1,   116,   250,    -1,   116,   238,    -1,   116,    28,
      -1,   116,    75,    -1,   116,    78,    -1,   116,    15,    -1,
     116,    57,    -1,   116,    87,    -1,   116,    74,    -1,     1,
      -1,   147,   242,   148,   250,    -1,   147,   242,   148,   238,
      -1,   147,   242,    38,   265,   148,   250,    -1,   147,     1,
     148,    -1,    -1,   250,    -1,   221,    -1,   147,   148,   239,
      -1,   147,   242,   148,   239,    -1,   147,   148,   240,    -1,
     147,   242,   148,   240,    -1,   147,   242,    38,   265,   148,
     239,    -1,    -1,   116,   250,    -1,   116,   221,    -1,   116,
      28,    -1,   116,    75,    -1,   116,    78,    -1,   116,    15,
      -1,   116,    57,    -1,   116,    87,    -1,   116,    74,    -1,
     116,   240,    -1,   265,    -1,   221,    -1,   242,   117,   265,
      -1,   242,   117,   221,    -1,   265,    -1,   243,   117,   265,
      -1,    85,    -1,   267,    -1,   221,    -1,   244,   117,   244,
      -1,   245,   117,   244,    -1,    -1,   247,    -1,   248,    -1,
     247,   117,   248,    -1,   170,    98,   221,    -1,   170,    98,
     267,    -1,   221,    -1,   267,    -1,   170,    -1,   253,    -1,
     268,    -1,   252,    -1,   276,    -1,   275,    -1,    75,   265,
      -1,    39,   145,   246,   146,    -1,    28,   145,   246,   146,
      -1,    77,   145,   246,   146,    -1,    76,    77,   145,   246,
     146,    -1,    12,   265,    -1,    78,   265,    -1,    15,   265,
      -1,    57,   265,    -1,    87,   265,    -1,    74,   265,    -1,
      34,   265,    38,   265,    27,   265,    -1,    34,   265,    38,
     179,    27,   265,    -1,    34,   265,    27,   265,    -1,    34,
     265,    38,   265,    27,    37,   265,    79,   265,    -1,    34,
     265,    38,   179,    27,    37,   265,    79,   265,    -1,    34,
     265,    27,    37,   265,    79,   265,    -1,    35,   265,    38,
     265,    27,   265,    -1,    35,   265,    38,   179,    27,   265,
      -1,    35,   265,    27,   265,    -1,    35,   265,    38,   265,
      27,    37,   265,    79,   265,    -1,    35,   265,    38,   179,
      27,    37,   265,    79,   265,    -1,    35,   265,    27,    37,
     265,    79,   265,    -1,   147,   242,   148,   265,    -1,   147,
     242,    38,   265,   148,   265,    -1,   147,   242,    38,   179,
     148,   265,    -1,   147,   242,    38,   265,   148,    37,   265,
      79,   265,    -1,   147,   242,    38,   179,   148,    37,   265,
      79,   265,    -1,    37,   265,    79,   265,    29,   265,    -1,
      50,    -1,   253,    -1,   249,    -1,   271,    -1,   270,    -1,
     198,    -1,   263,    -1,   264,    -1,   262,   142,   265,    -1,
      -1,   256,    -1,    93,   145,   257,   146,    -1,   260,    -1,
     257,   117,   260,    -1,    93,   145,   259,   146,    -1,   260,
      -1,   259,   117,   260,    -1,   261,   249,   237,   234,    -1,
     279,    67,   249,    -1,   265,    67,   249,    -1,    22,    -1,
      38,    -1,    22,    38,    -1,    68,    -1,    22,    68,    -1,
      89,    -1,   268,    -1,   262,   142,   265,    -1,    49,   265,
      -1,    45,   230,    38,   265,    -1,   273,    -1,   250,    -1,
     251,    -1,   277,    -1,   278,    -1,   198,    -1,   263,    -1,
     264,    -1,   145,   121,   265,   146,    -1,   265,   116,   265,
      -1,   265,   120,   265,    -1,   265,   120,    -1,   120,   265,
      -1,   120,    -1,    -1,   265,    -1,    82,   265,    -1,    83,
     265,    -1,   265,    -1,   249,    -1,   270,    -1,   271,    -1,
     272,    -1,   268,    -1,   198,    -1,   269,   145,   246,   146,
      -1,   269,   147,   246,   148,    -1,    61,   145,   246,   146,
      -1,   265,   119,   170,    -1,   265,   119,    84,    -1,   265,
     119,    28,    -1,   145,   244,   146,    -1,   145,   244,   117,
     146,    -1,   145,   245,   146,    -1,   145,   245,   117,   146,
      -1,     4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,
      -1,   143,   242,   144,    -1,   143,   242,   117,   144,    -1,
     147,   242,   148,    -1,   147,   242,   117,   148,    -1,   147,
     274,   148,    -1,   147,   274,   117,   148,    -1,   265,    96,
     265,    -1,   274,   117,   265,    96,   265,    -1,   265,   134,
     265,    -1,   265,   129,   265,    -1,   265,   139,   265,    -1,
     265,   118,   265,    -1,   265,   137,   265,    -1,   265,   138,
     265,    -1,   265,   130,   265,    -1,   265,   122,   265,    -1,
     265,   132,   265,    -1,   265,   128,   265,    -1,   265,   125,
     265,    -1,   265,   127,   265,    -1,   265,   124,   265,    -1,
     265,   112,   265,    -1,   265,   114,   265,    -1,   265,   115,
     265,    -1,   265,    97,   265,    -1,   265,   133,   265,    -1,
     265,   123,   265,    -1,   265,    16,   265,    -1,   265,    10,
     265,    -1,   265,   126,   265,    -1,   265,    26,   265,    -1,
     134,   265,    -1,   129,   265,    -1,    47,   265,    -1,    59,
     265,    -1,   131,   265,    -1,   113,   265,    -1,   265,    67,
     265,    -1,   265,    67,   179,    -1,   279,    67,   265,    -1,
     279,    67,   179,    -1,   265,    71,   265,    -1,   265,    71,
     179,    -1,   279,    71,   265,    -1,   279,    71,   179,    -1,
     134,    -1,   139,    -1,    97,    -1,   133,    -1,   112,    -1,
     114,    -1,   115,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   447,   447,   452,   453,   459,   460,   465,   466,   471,
     472,   473,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   504,
     507,   509,   515,   516,   517,   521,   522,   535,   536,   537,
     542,   543,   548,   553,   558,   562,   569,   574,   578,   583,
     587,   588,   589,   593,   597,   599,   601,   603,   605,   607,
     609,   611,   613,   615,   617,   619,   621,   623,   625,   627,
     632,   633,   637,   641,   642,   646,   647,   651,   652,   656,
     657,   658,   659,   660,   661,   662,   663,   667,   668,   672,
     673,   674,   675,   679,   694,   695,   696,   697,   698,   699,
     700,   701,   702,   703,   704,   705,   706,   707,   708,   709,
     710,   716,   722,   728,   734,   741,   751,   755,   756,   757,
     758,   759,   761,   763,   765,   770,   773,   774,   775,   776,
     777,   778,   782,   783,   787,   788,   789,   793,   794,   798,
     801,   803,   808,   809,   813,   815,   817,   824,   834,   844,
     854,   867,   872,   877,   885,   886,   891,   892,   894,   899,
     915,   922,   931,   939,   943,   950,   951,   956,   961,   955,
     986,   992,   999,  1005,  1011,  1022,  1028,  1021,  1060,  1064,
    1069,  1073,  1081,  1082,  1086,  1087,  1088,  1089,  1090,  1091,
    1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,  1101,
    1102,  1103,  1104,  1105,  1106,  1107,  1108,  1109,  1110,  1111,
    1115,  1116,  1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,
    1125,  1126,  1130,  1131,  1135,  1139,  1140,  1141,  1145,  1147,
    1149,  1151,  1153,  1158,  1159,  1163,  1164,  1165,  1166,  1167,
    1168,  1169,  1170,  1171,  1175,  1176,  1177,  1178,  1179,  1180,
    1184,  1185,  1189,  1190,  1191,  1192,  1193,  1194,  1198,  1199,
    1202,  1203,  1207,  1208,  1212,  1214,  1219,  1220,  1224,  1225,
    1229,  1230,  1234,  1236,  1238,  1243,  1256,  1273,  1274,  1276,
    1281,  1289,  1297,  1305,  1314,  1324,  1325,  1326,  1330,  1331,
    1339,  1341,  1346,  1348,  1350,  1355,  1357,  1359,  1366,  1367,
    1368,  1373,  1375,  1377,  1381,  1385,  1387,  1391,  1399,  1400,
    1401,  1402,  1403,  1404,  1405,  1406,  1407,  1408,  1409,  1414,
    1415,  1416,  1417,  1418,  1419,  1420,  1421,  1422,  1423,  1424,
    1444,  1448,  1452,  1460,  1467,  1468,  1469,  1473,  1475,  1481,
    1483,  1485,  1490,  1491,  1492,  1493,  1494,  1495,  1496,  1497,
    1498,  1499,  1500,  1506,  1507,  1508,  1509,  1513,  1514,  1518,
    1519,  1520,  1524,  1525,  1529,  1530,  1534,  1535,  1539,  1540,
    1541,  1542,  1546,  1557,  1558,  1559,  1560,  1561,  1562,  1564,
    1566,  1568,  1570,  1572,  1574,  1576,  1578,  1580,  1582,  1587,
    1589,  1591,  1593,  1595,  1597,  1599,  1601,  1603,  1605,  1607,
    1609,  1611,  1618,  1624,  1630,  1636,  1645,  1655,  1663,  1664,
    1665,  1666,  1667,  1668,  1669,  1670,  1675,  1676,  1680,  1684,
    1685,  1689,  1693,  1694,  1698,  1702,  1706,  1713,  1714,  1715,
    1716,  1717,  1718,  1722,  1723,  1728,  1733,  1741,  1742,  1743,
    1744,  1745,  1746,  1747,  1748,  1749,  1751,  1753,  1755,  1757,
    1759,  1764,  1765,  1768,  1769,  1770,  1773,  1774,  1775,  1776,
    1787,  1788,  1792,  1793,  1794,  1798,  1799,  1800,  1808,  1809,
    1810,  1811,  1815,  1816,  1817,  1818,  1819,  1820,  1821,  1822,
    1823,  1824,  1828,  1836,  1837,  1841,  1842,  1843,  1844,  1845,
    1846,  1847,  1848,  1849,  1850,  1851,  1852,  1853,  1854,  1855,
    1856,  1857,  1858,  1859,  1860,  1861,  1862,  1863,  1867,  1868,
    1869,  1870,  1871,  1872,  1876,  1877,  1878,  1879,  1883,  1884,
    1885,  1886,  1891,  1892,  1893,  1894,  1895,  1896,  1897
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "INTLITERAL", "REALLITERAL",
  "IMAGLITERAL", "STRINGLITERAL", "CSTRINGLITERAL", "EXTERNCODE", "TALIGN",
  "TAS", "TATOMIC", "TBEGIN", "TBREAK", "TBORROWED", "TBY", "TCATCH",
  "TCLASS", "TCOBEGIN", "TCOFORALL", "TCONFIG", "TCONST", "TCONTINUE",
  "TDEFER", "TDELETE", "TDMAPPED", "TDO", "TDOMAIN", "TELSE", "TENUM",
  "TEXCEPT", "TEXPORT", "TEXTERN", "TFOR", "TFORALL", "TFORWARDING", "TIF",
  "TIN", "TINDEX", "TINLINE", "TINOUT", "TITER", "TLABEL", "TLAMBDA",
  "TLET", "TLOCAL", "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT",
  "TON", "TONLY", "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED", "TPARAM",
  "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE",
  "TPUBLIC", "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN",
  "TSELECT", "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE", "TSUBDOMAIN",
  "TSYNC", "TTHEN", "TTHROW", "TTHROWS", "TTRY", "TTRYBANG", "TTYPE",
  "TUNDERSCORE", "TUNION", "TUNMANAGED", "TUSE", "TVAR", "TWHEN", "TWHERE",
  "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN",
  "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
  "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNSL", "TASSIGNSR", "TBAND",
  "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT",
  "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL",
  "THASH", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOT", "TNOTEQUAL",
  "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT",
  "TSTAR", "TSWAP", "TASSIGNREDUCE", "TIO", "TLCBR", "TRCBR", "TLP", "TRP",
  "TLSBR", "TRSBR", "TNOELSE", "TUMINUS", "TUPLUS", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_stmt", "access_control", "opt_prototype", "block_stmt",
  "stmt_ls", "only_ls", "opt_only_ls", "except_ls", "use_stmt",
  "require_stmt", "assignment_stmt", "opt_ident", "ident", "opt_string",
  "do_stmt", "return_stmt", "class_level_stmt", "private_decl",
  "forwarding_stmt", "extern_block_stmt", "loop_stmt", "zippered_iterator",
  "if_stmt", "defer_stmt", "try_stmt", "catch_stmt_ls", "catch_stmt",
  "catch_expr", "throw_stmt", "select_stmt", "when_stmt_ls", "when_stmt",
  "class_decl_stmt", "class_tag", "opt_inherit", "class_level_stmt_ls",
  "enum_decl_stmt", "enum_header", "enum_ls", "enum_item",
  "lambda_decl_expr", "$@1", "$@2", "linkage_spec", "fn_decl_stmt", "$@3",
  "$@4", "fn_decl_stmt_inner", "fn_decl_receiver_expr", "fn_ident",
  "assignop_ident", "opt_formal_ls", "req_formal_ls", "formal_ls",
  "formal", "opt_intent_tag", "required_intent_tag", "opt_this_intent_tag",
  "proc_or_iter", "opt_ret_tag", "opt_throws_error",
  "opt_function_body_stmt", "function_body_stmt", "query_expr",
  "opt_query_expr", "var_arg_expr", "opt_where_part",
  "type_alias_decl_stmt", "type_alias_decl_stmt_inner", "opt_init_type",
  "var_decl_stmt", "opt_config", "var_decl_stmt_inner_ls",
  "var_decl_stmt_inner", "tuple_var_decl_component",
  "tuple_var_decl_stmt_inner_ls", "opt_init_expr", "ret_array_type",
  "opt_ret_type", "opt_type", "array_type", "opt_formal_array_elt_type",
  "formal_array_type", "opt_formal_type", "expr_ls", "simple_expr_ls",
  "tuple_component", "tuple_expr_ls", "opt_actual_ls", "actual_ls",
  "actual_expr", "ident_expr", "type_level_expr", "for_expr", "cond_expr",
  "nil_expr", "stmt_level_expr", "opt_task_intent_ls",
  "task_intent_clause", "task_intent_ls", "forall_intent_clause",
  "forall_intent_ls", "intent_expr", "shadow_var_prefix", "io_expr",
  "new_expr", "let_expr", "expr", "opt_expr", "opt_try_expr", "lhs_expr",
  "fun_expr", "call_expr", "dot_expr", "parenthesized_expr",
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
     405,   406
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   152,   153,   154,   154,   155,   155,   156,   156,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   158,
     158,   158,   159,   159,   159,   160,   160,   161,   161,   161,
     162,   162,   163,   163,   163,   163,   164,   164,   165,   165,
     166,   166,   166,   167,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     169,   169,   170,   171,   171,   172,   172,   173,   173,   174,
     174,   174,   174,   174,   174,   174,   174,   175,   175,   176,
     176,   176,   176,   177,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   179,   180,   180,   180,
     180,   180,   180,   180,   180,   181,   182,   182,   182,   182,
     182,   182,   183,   183,   184,   184,   184,   185,   185,   186,
     187,   187,   188,   188,   189,   189,   189,   190,   190,   190,
     190,   191,   191,   191,   192,   192,   193,   193,   193,   194,
     194,   195,   196,   196,   196,   197,   197,   199,   200,   198,
     201,   201,   201,   201,   201,   203,   204,   202,   205,   205,
     205,   205,   206,   206,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   209,   209,   210,   211,   211,   211,   212,   212,
     212,   212,   212,   213,   213,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   215,   215,   215,   215,   215,   215,
     216,   216,   217,   217,   217,   217,   217,   217,   218,   218,
     219,   219,   220,   220,   221,   221,   222,   222,   223,   223,
     224,   224,   225,   225,   225,   226,   226,   227,   227,   227,
     228,   228,   228,   228,   228,   229,   229,   229,   230,   230,
     231,   231,   232,   232,   232,   233,   233,   233,   234,   234,
     234,   235,   235,   235,   235,   235,   235,   235,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     238,   238,   238,   238,   239,   239,   239,   240,   240,   240,
     240,   240,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   242,   242,   242,   242,   243,   243,   244,
     244,   244,   245,   245,   246,   246,   247,   247,   248,   248,
     248,   248,   249,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   252,   253,   254,   254,
     254,   254,   254,   254,   254,   254,   255,   255,   256,   257,
     257,   258,   259,   259,   260,   260,   260,   261,   261,   261,
     261,   261,   261,   262,   262,   263,   264,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   266,   266,   267,   267,   267,   268,   268,   268,   268,
     269,   269,   270,   270,   270,   271,   271,   271,   272,   272,
     272,   272,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   274,   274,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   276,   276,
     276,   276,   276,   276,   277,   277,   277,   277,   278,   278,
     278,   278,   279,   279,   279,   279,   279,   279,   279
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     3,     2,     2,     3,     2,     6,
       7,     7,     0,     1,     1,     0,     1,     2,     3,     3,
       1,     2,     1,     3,     3,     5,     0,     1,     1,     1,
       3,     5,     5,     3,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       0,     1,     1,     0,     1,     2,     1,     2,     3,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     3,
       5,     5,     2,     2,     5,     3,     6,     6,     4,     5,
       5,     3,     3,     6,     5,     6,     5,     6,     3,     4,
       6,     7,     6,     7,     4,     5,     4,     4,     3,     6,
       5,     6,     5,     8,     7,     2,     3,     3,     2,     2,
       3,     3,     0,     2,     2,     3,     5,     1,     3,     3,
       5,     5,     0,     2,     3,     2,     3,     6,     8,     6,
       8,     1,     1,     1,     0,     2,     0,     2,     3,     5,
       5,     1,     1,     2,     3,     1,     3,     0,     0,     8,
       0,     1,     2,     2,     1,     0,     0,    10,     3,     3,
       5,     5,     1,     3,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     3,     3,     0,     1,     3,     4,     5,
       4,     6,     6,     0,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     0,     1,     1,     2,     1,     1,
       1,     1,     0,     1,     2,     1,     1,     1,     0,     1,
       1,     1,     1,     1,     2,     1,     0,     1,     2,     2,
       0,     2,     3,     4,     4,     2,     4,     0,     2,     2,
       4,     4,     4,     5,     4,     0,     1,     1,     1,     3,
       3,     5,     1,     1,     3,     1,     2,     3,     0,     2,
       2,     3,     2,     4,     3,     3,     4,     3,     0,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     0,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       4,     4,     6,     3,     0,     1,     1,     3,     4,     3,
       4,     6,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     1,     1,     3,     3,     1,     3,     1,
       1,     1,     3,     3,     0,     1,     1,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     4,
       4,     4,     5,     2,     2,     2,     2,     2,     2,     6,
       6,     4,     9,     9,     7,     6,     6,     4,     9,     9,
       7,     4,     6,     6,     9,     9,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     0,     1,     4,     1,
       3,     4,     1,     3,     4,     3,     3,     1,     1,     2,
       1,     2,     1,     1,     3,     2,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     3,     3,     2,     2,
       1,     0,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     4,     4,     4,     3,     3,     3,     3,     4,
       3,     4,     1,     1,     1,     1,     1,     3,     4,     3,
       4,     3,     4,     3,     5,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   482,   483,   484,   485,
     486,     0,   426,    80,     0,   161,   426,     0,   296,    80,
       0,     0,     0,     0,   171,   461,   297,     0,     0,   295,
       0,     0,   181,     0,   177,     0,     0,     0,     0,   417,
       0,   184,     0,     0,     0,     0,   295,   295,   162,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   163,     0,     0,     0,     0,   534,   536,     0,
     537,   538,   460,     0,     0,   535,   532,    89,   533,     0,
       0,     0,     4,     0,     5,     9,    45,    10,    11,    12,
      14,   382,    22,    13,    90,    96,    15,    17,    16,    19,
      20,    21,    18,    95,     0,    93,     0,   452,     0,    97,
      94,    98,     0,   466,   448,   449,   385,   383,     0,     0,
     453,   454,     0,   384,     0,   467,   468,   469,   447,   387,
     386,   450,   451,     0,    38,    24,   393,     0,     0,   427,
       0,    81,     0,     0,     0,     0,     0,     0,     0,   452,
     466,   383,   453,   454,   395,   384,   467,   468,     0,   426,
       0,     0,   135,     0,   367,     0,   374,   462,   182,   485,
     103,     0,     0,   183,     0,     0,     0,     0,     0,   296,
     297,   102,     0,     0,   374,     0,     0,     0,     0,     0,
     298,     0,    86,    32,     0,   520,   445,     0,   396,   521,
       7,   374,   297,    92,    91,   275,   364,     0,   363,     0,
       0,    87,   465,     0,     0,    35,     0,   398,   388,     0,
     374,    36,   394,     0,   142,   138,     0,   384,   142,   139,
       0,   287,     0,   397,     0,   363,     0,     0,   523,   459,
     519,   522,   518,     0,    47,    50,     0,     0,   369,     0,
     371,     0,     0,   370,     0,   363,     0,     0,     6,    46,
       0,   164,     0,   261,   260,   185,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   458,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   374,   374,     0,     0,
       0,    25,    26,     0,     0,     0,     0,     0,    27,     0,
       0,     0,    28,     0,    29,     0,   382,   380,     0,   375,
     376,   381,     0,     0,     0,     0,   112,     0,     0,   111,
       0,     0,     0,   118,     0,     0,    56,    99,     0,   220,
     227,   228,   229,   224,   226,   222,   225,   223,   221,   231,
     230,   128,     0,     0,    30,   235,   178,   302,     0,   303,
     305,     0,   339,     0,   308,     0,     0,    85,    31,    33,
       0,   274,     0,    63,   463,   464,    88,     0,    34,   374,
       0,   149,   140,   136,   141,   137,     0,   285,   282,    60,
       0,    56,   105,    37,    49,    48,    51,     0,   487,     0,
       0,   478,     0,   480,     0,     0,     0,     0,     0,     0,
     491,     8,     0,     0,     0,     0,   254,     0,     0,     0,
       0,     0,   425,   515,   514,   517,   525,   524,   529,   528,
     511,   508,   509,   510,   456,   498,   477,   476,   475,   457,
     502,   513,   507,   505,   516,   506,   504,   496,   501,   503,
     512,   495,   499,   500,   497,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   527,   526,   531,   530,   437,   438,   440,
     442,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   489,   426,   426,   108,   283,   368,     0,     0,
     390,     0,   284,   164,     0,     0,     0,   401,     0,     0,
       0,   407,     0,     0,     0,   119,   533,    59,     0,    52,
      57,     0,   127,     0,     0,     0,   389,   248,   245,   246,
     247,   251,   252,   253,   243,     0,   236,     0,   244,   262,
       0,   306,     0,   335,   332,   336,   338,   333,   334,   337,
       0,   331,   448,     0,   300,   446,   299,   474,   366,   365,
       0,     0,     0,   391,     0,   143,   289,   448,     0,     0,
       0,   488,   455,   479,   372,   481,   373,     0,     0,   490,
     124,   411,     0,   493,   492,     0,     0,   165,     0,     0,
     175,     0,   172,   258,   255,   256,   259,   186,     0,     0,
     291,   290,   292,   294,    64,    71,    72,    73,    68,    70,
      78,    79,    66,    69,    67,    65,    75,    74,    76,    77,
     472,   473,   439,   441,     0,   428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   104,
     378,   379,   377,     0,     0,   126,     0,     0,   110,     0,
     109,     0,     0,   116,     0,     0,   114,     0,     0,   432,
       0,   100,     0,   101,     0,     0,   130,     0,   132,   249,
     250,     0,   243,   234,     0,   352,   263,   266,   265,   267,
       0,   304,   307,   308,     0,     0,   309,   310,   151,     0,
       0,   150,   153,   392,     0,   144,   147,     0,   286,    61,
      62,     0,     0,     0,     0,   125,     0,     0,     0,   295,
     170,     0,   173,   169,   257,   262,   217,   215,   196,   199,
     197,   198,   209,   200,   213,   205,   203,   216,   204,   202,
     207,   212,   214,   201,   206,   210,   211,   208,   218,   219,
       0,   194,     0,   232,   232,   192,   293,   430,   308,   466,
     466,     0,     0,     0,     0,     0,     0,     0,     0,   107,
     106,     0,   113,     0,     0,   400,     0,   399,     0,     0,
     406,   117,     0,   405,   115,     0,   431,    54,    53,   129,
     416,   131,     0,   352,   237,     0,     0,   308,   264,   280,
     301,   343,     0,   489,     0,   155,     0,     0,     0,   145,
       0,   122,   413,     0,     0,   120,   412,     0,   494,     0,
      39,     0,   159,   297,   295,   295,   157,   295,   167,   176,
     174,     0,   195,     0,     0,   235,   188,   189,   434,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   295,   404,
       0,     0,   410,     0,     0,   433,     0,     0,   134,   308,
     352,   358,   355,   359,   361,   356,   357,   360,     0,   354,
     362,   448,   276,   240,   238,     0,     0,     0,   341,   448,
     156,   154,     0,   148,     0,   123,     0,   121,    41,    40,
     168,   328,     0,   268,   193,   194,   232,   232,     0,     0,
       0,     0,     0,     0,     0,   160,   158,     0,     0,     0,
       0,    55,   133,   239,   308,   344,     0,   277,   279,   278,
     281,   272,   273,   179,     0,   146,     0,     0,   324,   321,
     325,   327,   322,   323,   326,     0,   320,   448,   269,   280,
     190,   191,   233,     0,     0,     0,     0,     0,     0,   403,
     402,   409,   408,   242,   241,   346,   347,   349,   448,     0,
     489,   448,   415,   414,     0,   312,     0,     0,     0,   348,
     350,   317,   315,   448,   489,   270,   187,   271,   344,   316,
     448,   351
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   245,    83,   542,    85,    86,   260,    87,
     246,   537,   541,   538,    88,    89,    90,   140,    91,   172,
     193,    92,    93,    94,    95,    96,    97,   655,    98,    99,
     100,   402,   585,   717,   101,   102,   581,   712,   103,   104,
     434,   729,   105,   106,   611,   612,   149,   186,   559,   108,
     109,   436,   735,   617,   762,   763,   372,   846,   376,   555,
     556,   557,   558,   618,   265,   700,   949,   986,   933,   206,
     928,   883,   886,   110,   232,   407,   111,   112,   189,   190,
     380,   381,   574,   946,   903,   384,   571,   966,   880,   807,
     247,   163,   251,   252,   338,   339,   340,   150,   114,   115,
     116,   151,   118,   138,   139,   501,   354,   678,   502,   503,
     119,   152,   153,   212,   173,   341,   155,   124,   156,   157,
     127,   128,   256,   129,   130,   131,   132,   133
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -883
static const yytype_int16 yypact[] =
{
    -883,   199,  2933,  -883,   -41,  -883,  -883,  -883,  -883,  -883,
    -883,  4093,    30,   228,  8581,  -883,    30,  8581,    25,   228,
    4093,  8581,  4093,    98,  -883,  8581,  5956,  7136,  8581,  7256,
    8581,   108,  -883,   228,  -883,    57,  7761,  8581,  8581,  -883,
    8581,  -883,  8581,  8581,   241,   120,  1528,  1612,  -883,  7881,
    6751,  8581,  7761,  8581,  8581,   267,   160,  4093,  8581,  8701,
    8701,   228,  -883,  8581,  7881,  8581,  8581,  -883,  -883,  8581,
    -883,  -883, 10381,  8581,  8581,  -883,  8581,  -883,  -883,  3223,
    6196,  7881,  -883,  3948,  -883,  -883,   204,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,   228,  -883,   228,   292,   191,  -883,
    -883,  -883,   192,   243,  -883,  -883,  -883,   251,   274,   264,
     294,   296, 14169,  1290,   155,   305,   307,  -883,  -883,  -883,
    -883,  -883,  -883,   270,  -883,  -883, 14169,   301,  4093,  -883,
     316,  -883,  8581,  8581,  8581,  8581,  8581,  7881,  7881,   221,
    -883,  -883,  -883,  -883,   337,   239,  -883,  -883,   314, 12489,
     228,   322,  -883,   106, 14169,   367,  6871, 14169,  -883,   153,
    -883,   228,   281,  -883,   228,   317,    49, 12030, 11996,  -883,
    -883,  -883, 12414, 11247,  6871,  4093,   318,    65,    88,    44,
    -883,  4093,  -883,  -883, 12080,   617,   337, 12080,   337,   617,
    -883,  6871,  7376,  -883,  -883,   228,  -883,   110, 14169,  8581,
    8581,  -883, 14169,   325, 12289,  -883, 12080,   337, 14169,   319,
    6871,  -883, 14169, 12698,  -883,  -883, 12744, 10587,  -883,  -883,
   12823,   368,   329,   337,   115, 12539, 12080, 12869,   185,  1191,
     617,   185,   617,   122,  -883,  -883,  3368,   -13,  -883,  8581,
    -883,   -21,   -19,  -883,    61, 12948,   -32,   460,  -883,  -883,
     420,   353,   332,  -883,  -883,  -883,    29,    57,    57,    57,
    -883,  8581,  8581,  8581,  8581,  8026,  8026,  8581,  8581,  8581,
    8581,  8581,  8581,   100, 10381,  8581,  8581,  8581,  8581,  8581,
    8581,  8581,  8581,  8581,  8581,  8581,  8581,  8581,  8581,  8581,
    7496,  7496,  7496,  7496,  7496,  7496,  7496,  7496,  7496,  7496,
    7496,  7496,  7496,  7496,  7496,  7496,  6871,  6871,  8026,  8026,
    6631,  -883,  -883, 12614, 12664, 13012,    70,  3513,  -883,  8026,
      49,   336,  -883,  8581,  -883,  8581,   384,  -883,   338,   366,
    -883,  -883,   350,   228,   449,  7881,  -883,  4238,  8026,  -883,
    4383,  8026,   343,  -883,    49,  8821,  8581,  -883,  4093,  -883,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,   461,  8581,   345,  -883,   117,  -883,  -883,    65,  -883,
     377,   365,  -883,  8941,   398,  8581,    57,  -883,  -883,  -883,
     370,  -883,  7881,  -883, 14169, 14169,  -883,    15,  -883,  6871,
     373,  -883,   496,  -883,   496,  -883,  9061,   397,  -883,  -883,
    8821,  8581,  -883,  -883,  -883,  -883,  -883,  7616,  -883, 11114,
    6341,  -883,  6486,  -883,  8026,  5518,  3078,   372,  8581,  5810,
    -883,  -883,   228,  7881,   378,   404,   245,    57,   121,   123,
     203,   233, 12364, 14279, 14279,   326,  -883,   326,  -883,   326,
   14343,   362,  1459,  1492,   337,   185,  -883,  -883,  -883,  1191,
     860,   326,  1050,  1050, 14279,  1050,  1050,  1241,   185,   860,
   14308,  1241,   617,   617,   185,   387,   390,   391,   393,   394,
     395,   403,   408,   409,   410,   411,   412,   422,   423,   424,
     426,   388,   385,  -883,   326,  -883,   326,    53,  -883,  -883,
    -883,   -17,  -883,   228, 14233,   291,  9181,  8026,  9301,  8026,
    8581,  8026, 10784,    30, 13085,  -883,  -883, 14169, 13149,  6871,
    -883,  6871,  -883,   353,  8581,    95,  8581, 14169,    63, 12205,
    8581, 14169,    60, 12155,  6631,  -883,   433,   439,   434, 13222,
     439,   437,   511, 13286,  4093, 12239,  -883,    56,  -883,  -883,
    -883,  -883,  -883,  -883,   457,   103,  -883,    72,  -883,   333,
     429,    65,    88,  8581,    98,  8581,  8581,  8581,  8581,  8581,
    2705,  -883,   304,  7016,  -883, 14169,  -883,  -883,  -883, 14169,
     435,    32,   431,  -883,    51,  -883,  -883,   235,   228,   442,
     444,  -883,  -883,  -883,  -883,  -883,  -883,     9,  2165,  -883,
    -883, 14169,  4093, 14169,  -883, 13355,   441,   465,  2083,   443,
     490,   -10,  -883,   522,  -883,  -883,  -883,  -883,  2755,   244,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,  6631,  -883,    16,  8026,  8026,  8581,
     564, 13419,  8581,   565, 13488,   446, 10917,    49,    49,  -883,
    -883,  -883,  -883,   454, 12080,  -883, 11325,  4528,  -883,  4673,
    -883, 11403,  4818,  -883,    49,  4963,  -883,    49,   109,  -883,
    8581,  -883,  8581,  -883,  4093,  8581,  -883,  4093,   569,  -883,
    -883,   228,   841,  -883,    65,   485,   534,  -883,  -883,  -883,
      82,  -883,  -883,   398,   455,    73,  -883,  -883,  -883,  5108,
    7881,  -883,  -883,  -883,   228,  -883,   488,   314,  -883,  -883,
    -883,  5253,   458,  5398,   459,  -883,  8581,  3658,   466,   553,
    -883,  8581,   228,  -883,  -883,   333,  -883,  -883,  -883,   228,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    8581,   492,   493,   469,   469,  -883,  -883,  -883,   398,   118,
     189, 13552,  9421,  9541, 13625,  9661,  9781,  9901, 10021,  -883,
    -883, 10846,  -883,  4093,  8581, 14169,  8581, 14169,  4093,  8581,
   14169,  -883,  8581, 14169,  -883,  6631,  -883, 13689, 14169,  -883,
   14169,   591,  4093,   485,  -883,   475,  8146,    14,  -883,   532,
    -883,  -883,  8026, 10707,  4093,  -883,    39,   478,  8581,  -883,
    8581,  -883, 14169,  4093,  8581,  -883, 14169,  4093, 14169,   215,
    -883,  3803,  -883,  6076,   706,   706,  -883,  1653,  -883, 14169,
    -883,    79,  -883, 11183, 10839,   117,  -883,  -883,  -883,  8581,
    8581,  8581,  8581,  8581,  8581,  8581,  8581,   481,  1967, 13286,
   11481, 11559, 13286, 11637, 11715,  -883,  8581,  4093,  -883,   398,
     485,  8581,    98,  8581,  8581,  8581,  8581,  8581,  5664,  -883,
    -883,   211,  7881,  -883,  -883,  8581,    23, 10981,  -883,   570,
     367,  -883,   314, 14169, 11793,  -883, 11871,  -883,  -883,  -883,
    -883,  -883, 10141,   546,  -883,  -883,   469,   469,   190, 13758,
   13822, 13895, 13959, 14032, 14096,  -883,  -883,  4093,  4093,  4093,
    4093, 14169,  -883,  -883,    14,  8291,    80,  -883,  -883, 14169,
   14169,  -883,  -883,  -883, 10021,  -883,  4093,  4093,  8581,    98,
    8581,  8581,  8581,  8581,  8581,  2541,  -883,   249,  -883,   532,
    -883,  -883,  -883,  8581,  8581,  8581,  8581,  8581,  8581, 13286,
   13286, 13286, 13286,  -883,  -883,  -883,  -883,  -883,   217,  8026,
   10472,   921, 13286, 13286,   480, 10261,   113,    93, 11050,  -883,
    -883,  -883,  -883,   308, 10593,  -883,  -883,  -883,  8436,  -883,
     312,  -883
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -883,  -883,  -883,    -1,  -369,  1542,  -883,  -883,  -883,   360,
     -98,  -218,   219,   222,  -883,  -883,   313,   615,  2211,  -883,
     878,  -847,  -670,   -43,  -883,  -883,  -883,    46,  -883,  -883,
    -883,   413,  -883,   -78,  -883,  -883,  -883,  -883,  -883,   472,
     125,  -143,  -883,  -883,  -883,   -83,   824,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,  -883,  -192,  -607,  -742,  -883,  -190,
     -39,   104,  -883,  -883,  -883,   -73,  -883,  -883,  -320,   234,
    -883,  -261,  -285,  -883,  -150,  -883,   637,  -883,  -224,   283,
    -883,  -370,  -690,  -865,  -883,  -531,  -399,  -882,  -833,  -702,
     -35,  -883,    -9,  -883,  -136,  -883,   150,   700,  -353,  -883,
    -883,  1098,  -883,   -11,  -883,  -883,  -221,  -883,  -497,  -883,
    -883,  1128,  1266,    -2,   647,  1159,   414,  -883,  1526,  1782,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -318
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -490
static const yytype_int16 yytable[] =
{
     122,    82,   505,   203,   204,   158,   554,   586,   560,   136,
     331,   764,   154,   810,   207,   159,   580,   382,   122,   164,
     122,   342,   847,   167,   167,   177,   178,   182,   183,   234,
     572,   703,     5,   427,   194,   195,   196,   679,   197,   932,
     198,   199,   438,   439,   440,   441,   254,   208,   373,   214,
     216,   217,   218,   587,     5,   222,   223,   226,   230,   838,
       5,   233,   235,   236,   237,   390,   191,   238,     5,  -152,
     239,   240,   241,   176,   242,     5,   191,   208,   848,   255,
     901,   122,   385,   382,   400,   429,   709,   672,   979,   382,
     667,   642,   967,    50,   689,   134,   420,   437,   422,   424,
     644,   869,   352,     5,   417,  -152,   991,   732,   511,   160,
     982,   812,   573,   326,  -329,   768,   430,   884,   969,   989,
    -243,   643,   710,   137,   690,   421,  -329,   423,   456,   645,
     932,   418,   383,  -329,   733,   882,   122,   980,   540,   547,
     136,   323,   324,   325,   222,   208,   255,   767,   330,  -318,
     377,   511,  -329,   352,   352,   548,   392,   721,   549,  -152,
    -318,   386,  -329,    50,   950,   951,   327,   900,   924,   809,
    -318,   -84,   550,  -329,   187,   551,   711,    44,   425,   923,
     491,   492,   327,   122,   457,   552,  -329,   425,   838,   122,
     425,   702,   327,   540,   327,   902,   714,   425,   383,     3,
     167,   553,   187,   327,   383,  -329,   327,   394,   395,   426,
     378,   274,   392,   619,   266,  -318,   505,   694,   512,   -84,
     692,   813,  -318,   333,  -329,  -329,   795,   392,   970,   985,
     425,     5,   392,   263,   964,  -436,   327,   907,   386,   -84,
     386,   665,   334,   166,   122,   416,   393,   419,   200,   693,
     267,   409,   275,   184,   264,   796,   276,   620,   134,   621,
     268,   984,  -243,   582,  -436,   201,   414,   613,   259,   442,
     443,   444,   445,   447,   449,   450,   451,   452,   453,   454,
     455,   269,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   865,    15,
     316,   281,   317,   614,   283,   220,  -435,   692,   286,  -353,
     525,   674,   677,   615,   250,  -345,   494,   496,   504,  -319,
     386,   446,   448,   554,   805,   122,   505,   514,  -353,   616,
    -319,   517,  -353,   518,  -345,  -435,   952,   318,  -345,   622,
    -319,   319,  -330,   208,   219,   527,   529,    48,   531,   533,
     386,   134,  -288,   539,   539,   696,   543,  -353,   648,   898,
     837,   386,   319,  -345,   493,   495,  -471,    62,  -471,   623,
     545,  -288,   225,   229,  -330,   513,   722,   724,  -311,  -419,
     766,   122,  -313,   575,  -470,  -319,  -470,  -418,   274,  -311,
     579,   697,  -319,  -313,   528,  -330,   192,   532,   607,  -311,
     337,   698,  -330,  -313,   122,   609,   271,     5,   539,   539,
     270,   594,   192,   596,   888,   579,   123,   699,   337,   224,
     228,  -330,   598,   579,   601,   123,   603,   605,  -422,   275,
    -423,   208,  -424,   276,   123,   337,   123,  -471,   718,  -471,
    -330,  -421,   281,  -420,  -311,   283,   320,  -330,  -313,   286,
    -330,  -311,   322,   881,   337,  -313,   283,   327,   332,   335,
     889,   396,   345,   375,   399,   408,   406,   431,   432,   433,
     597,   123,   516,   227,   227,   435,   554,   505,   281,   547,
     282,   283,   519,   521,   520,   286,   522,   524,   534,   837,
     544,   546,   293,   123,   561,   548,   573,   123,   549,   297,
     298,   299,   657,   658,   527,   651,   531,   654,   543,   656,
     601,   562,   550,   584,   588,   551,   577,   257,   328,   583,
     602,   608,   664,   624,   666,   552,   625,   626,   671,   627,
     628,   629,   504,   641,   640,   705,   192,   192,   192,   630,
     684,   553,   122,   371,   631,   632,   633,   634,   635,   947,
     337,   337,   123,   650,   192,   653,   680,   192,   636,   637,
     638,   154,   639,   198,   217,   218,   222,   233,   255,   -58,
     681,    15,   968,   683,    18,   701,   192,   713,   719,   708,
     720,   971,   392,    24,   727,    25,   833,   730,   731,    29,
     734,   772,   775,    32,   777,  -180,   192,   781,   802,   123,
     122,   806,   808,   811,   818,   123,   823,   827,  -340,    41,
     832,  -382,   844,    44,   845,   834,  -180,   968,   835,    48,
     867,   870,   983,   885,   892,   915,   578,   948,   981,   831,
     590,   990,   589,   337,   161,   968,   817,    61,   858,    62,
    -340,   404,   504,   274,   343,   447,   494,   771,   663,   840,
     774,   578,   906,   804,   250,   908,   250,   987,   691,   578,
     123,  -340,   841,   963,   977,   785,   181,   787,  -340,   576,
     790,   662,   168,   793,     0,   816,     0,     0,   797,     0,
     798,     0,   122,   800,   275,   122,     0,  -340,   276,    77,
     192,     0,     0,   446,   493,     0,     0,   836,     0,     0,
       0,     0,   113,     0,     0,     0,  -340,   122,   208,     0,
       0,   113,     0,  -340,   192,     0,  -340,     0,     0,   822,
     113,   826,   113,     0,   828,   122,     0,   179,     0,   839,
       0,     0,     0,   281,     0,   282,   283,     0,    25,   202,
     286,   123,     0,     0,     0,     0,    32,   293,  -180,     0,
       0,     0,     0,   660,     0,   337,   299,   113,   843,     0,
       0,   123,    41,     0,   123,     0,     0,     0,     0,  -180,
     785,   787,   123,   790,   793,   822,   826,     0,     0,   113,
       0,   859,   860,   113,   861,     0,   862,   863,     0,     0,
     864,   203,   204,   504,     0,     0,     0,     0,     0,     0,
     122,     0,     0,     0,   122,     0,     0,     0,     0,     0,
     887,   601,   122,     0,     0,     0,   893,     0,   894,     0,
       0,   122,   896,     0,     0,   122,   107,     0,     0,   122,
     416,   167,     0,     0,     0,   107,     0,     0,   113,     0,
     123,     0,     0,   926,   107,     0,   107,   859,   909,   910,
     862,   911,   912,   913,   914,     0,     0,     0,     0,     0,
       0,     0,     0,   547,   921,   122,     0,     0,     0,   154,
       0,   198,   217,   218,   222,   233,   255,     0,     0,   548,
     929,   107,   549,   930,     0,   113,   274,     0,   192,   192,
       0,   113,   192,   192,     0,     0,   550,     0,     0,   551,
     122,    44,     0,   107,     0,   688,     0,   107,     0,   552,
     976,     0,     0,     0,     0,   959,   960,   961,   962,     0,
       0,     0,     0,   122,     0,   553,     0,   275,     0,     0,
     215,   276,   826,     0,   972,   973,   154,     0,   198,   217,
     218,   222,   233,   255,   715,     0,   113,     0,     0,     0,
       0,   959,   960,   961,   962,   972,   973,     0,   123,  -342,
       0,     0,   107,     0,     0,     0,     0,   978,   601,     0,
       0,     0,   278,   122,   279,   280,   281,     0,   282,   283,
     284,     0,   601,   286,   287,   288,   826,   290,   291,   292,
     293,  -342,     0,     0,   296,     0,     0,   297,   298,   299,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   107,
       0,     0,  -342,     0,     0,   107,   123,   192,   192,  -342,
       0,     0,     0,     0,   192,     0,   371,   113,     0,     0,
       0,   371,     0,     0,   192,     0,     0,   192,  -342,     0,
     879,     0,     0,     0,     0,     0,     0,   113,     0,     0,
     113,     0,     0,     0,   346,   349,   353,  -342,   113,     0,
       0,     0,     0,     0,  -342,     0,     0,  -342,     0,     0,
     107,     0,   388,     0,     0,   389,   274,   819,     0,     0,
       0,   123,     0,   123,     0,     0,   123,     0,     0,   123,
       0,     0,     0,     0,   398,     0,     0,     0,   123,     0,
     117,   123,     0,     0,     0,     0,     0,     0,     0,   117,
       0,     0,     0,     0,   412,     0,   927,   275,   117,     0,
     117,   276,     0,   123,     0,     0,   113,     0,     0,     0,
     120,     0,     0,     0,     0,   123,     0,   123,     0,   120,
       0,   123,     0,     0,     0,     0,     0,     0,   120,     0,
     120,   107,     0,     0,     0,   117,     0,     0,     0,   965,
       0,     0,   278,     0,   279,   280,   281,     0,   282,   283,
     284,   107,     0,   286,   107,     0,   192,   117,     0,   292,
     293,   117,   107,     0,   296,   120,     0,   297,   298,   299,
       0,     0,     0,     0,     0,     0,     0,   123,     0,     0,
       0,     0,   123,   646,   965,     0,     0,   120,   515,   213,
       0,   120,     0,     0,     0,     0,   123,   274,     0,     0,
     371,   371,   965,   371,   371,     0,     0,     0,   123,     0,
       0,     0,   535,     0,     0,     0,   117,   123,     0,   253,
       0,   123,     0,     0,   113,   123,   931,     0,     0,     0,
     107,     0,   935,     0,   371,     0,   371,     0,   275,     0,
       0,     0,   276,     0,     0,     0,   120,   274,   121,     0,
       0,     0,     0,     0,     0,     0,     0,   121,     0,     0,
       0,   123,     0,   117,     0,     0,   121,     0,   121,   117,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,   278,     0,   279,   280,   281,   275,   282,
     283,     0,   276,   120,   286,     0,     0,     0,   765,   120,
     292,   293,     0,   121,     0,   296,     0,     0,   297,   298,
     299,   123,   123,   123,   123,     0,     0,   931,     0,     0,
       0,     0,     0,     0,   117,   121,     0,   769,   770,   121,
     123,   123,     0,   278,     0,   279,   280,   281,     0,   282,
     283,     0,     0,     0,   286,     0,     0,   113,   107,   113,
       0,   293,   113,     0,   120,   113,     0,     0,   297,   298,
     299,     0,     0,     0,   113,     0,     0,   113,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,     0,     0,   121,     0,   668,   670,     0,   113,
     673,   676,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   113,     0,   113,     0,   117,   107,   113,     0,     0,
     314,   315,  -443,     0,     0,  -470,     0,  -470,     0,     0,
       0,     0,     0,     0,     0,   117,     0,     0,   117,     0,
       0,   121,     0,     0,     0,   120,   117,   121,     0,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   120,     0,     0,   120,     0,
       0,     0,     0,   113,     0,   274,   120,     0,   113,     0,
       0,   107,     0,   107,     0,     0,   107,     0,     0,   107,
       0,     0,   113,     0,     0,     0,     0,     0,   107,     0,
       0,   107,   121,     0,   113,     0,     0,     0,   274,     0,
       0,     0,     0,   113,   117,     0,   275,   113,   125,     0,
     276,   113,     0,   107,     0,   779,   780,   125,     0,     0,
       0,     0,   782,     0,    84,   107,   125,   107,   125,   179,
       0,   107,   791,   135,   120,   794,     0,     0,     0,   275,
      25,   202,   162,   276,   165,     0,     0,   113,    32,     0,
    -180,   278,     0,     0,   280,   281,   -44,   282,   283,   253,
       0,   253,   286,   125,    41,     0,     0,     0,     0,   293,
       0,  -180,   -44,   121,     0,     0,   297,   298,   299,   221,
       0,     0,     0,     0,   278,   125,     0,   107,   281,   125,
     282,   283,   107,   121,     0,   286,   121,   113,   113,   113,
     113,    84,   293,     0,   121,   258,   107,     0,     0,   297,
     298,   299,     0,   179,     0,     0,   113,   113,   107,     0,
       0,     0,   117,     0,    25,   202,     0,   107,     0,     0,
       0,   107,    32,     0,  -180,   107,     0,     0,     0,     0,
     -43,     0,     0,     0,   125,     0,     0,     0,    41,     0,
       0,    15,   120,     0,    18,  -180,   -43,     0,   661,     0,
     321,     0,     0,    24,     0,    25,   833,     0,     0,    29,
       0,   107,   121,    32,   891,  -180,     0,     0,     0,     0,
     117,     0,     0,     0,     0,     0,     0,     0,     0,    41,
       0,   125,     0,   257,     0,   834,  -180,   125,   835,    48,
       0,     0,     0,     0,     0,     0,     0,   374,     0,     0,
     120,     0,   707,   387,     0,     0,     0,    61,     0,    62,
       0,   107,   107,   107,   107,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     107,   107,     0,     0,     0,   117,     0,   117,     0,     0,
     117,     0,   125,   117,     0,     0,     0,     0,     0,     0,
       0,     0,   117,     0,   126,   117,     0,     0,    84,    77,
       0,     0,     0,   126,     0,   120,     0,   120,     0,     0,
     120,     0,   126,   120,   126,     0,     0,   117,     0,     0,
     121,     0,   120,     0,     0,   120,     0,     0,     0,   117,
       0,   117,     0,     0,     0,   117,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   120,     0,   126,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   120,
       0,   120,     0,   125,     0,   120,     0,     0,     0,     0,
       0,   126,     0,     0,     0,   126,     0,     0,   121,    84,
       0,     0,     0,   125,     0,     0,   125,     0,     0,     0,
       0,   117,     0,     0,   125,     0,   117,     0,     0,   387,
       0,     0,   387,     0,     0,     0,     0,     0,     0,     0,
     117,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   120,   117,     0,     0,     0,   120,     0,     0,     0,
     126,   117,     0,     0,     0,   117,     0,     0,     0,   117,
     120,     0,     0,   121,     0,   121,     0,     0,   121,     0,
       0,   121,   120,     0,     0,     0,     0,     0,     0,     0,
     121,   120,   125,   121,     0,   120,     0,     0,     0,   120,
       0,     0,     0,     0,     0,   117,     0,   126,   600,     0,
       0,     0,     0,   126,     0,   121,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    15,     0,   121,    18,   121,
       0,     0,     0,   121,     0,   120,     0,    24,     0,    25,
     833,     0,     0,    29,     0,     0,     0,    32,     0,  -180,
       0,     0,     0,     0,     0,   117,   117,   117,   117,     0,
       0,     0,     0,    41,     0,     0,     0,    44,   126,   834,
    -180,     0,   835,    48,   117,   117,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   120,   120,   120,   120,   121,
       0,    61,     0,    62,   121,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   120,   120,     0,     0,   121,     0,
     125,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     121,     0,     0,     0,   728,     0,   686,     0,     0,   121,
       0,     0,     0,   121,     0,     0,     0,   121,     0,     0,
       0,  -166,     0,    77,  -166,  -166,     0,     0,     0,   126,
       0,   916,     0,  -166,     0,  -166,  -166,     0,     0,  -166,
       0,     0,     0,  -166,     0,  -166,     0,     0,   125,   126,
       0,     0,   126,   121,     0,     0,     0,     0,     0,  -166,
     126,  -166,     0,  -166,   725,  -166,  -166,     0,  -166,  -166,
       0,  -166,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -166,     0,  -166,
       0,     0,  -166,     0,     0,   272,     0,     0,     0,     0,
       0,   273,     0,   121,   121,   121,   121,     0,     0,     0,
       0,   274,     0,   125,     0,   125,     0,     0,   125,     0,
       0,   125,   121,   121,     0,     0,     0,     0,   126,   387,
     125,   387,     0,   125,   387,     0,     0,   387,     0,  -166,
       0,     0,     0,     0,   141,     0,   799,  -166,     0,   801,
     141,     0,   275,     0,     0,   125,   276,     0,     0,     0,
       0,     0,     0,     0,   185,     0,   188,   125,     0,   125,
       0,   815,     0,   125,     0,     0,     0,     0,   352,     0,
       0,     0,   277,   821,     0,   825,     0,     0,     0,    84,
       0,     0,   231,     0,     0,     0,     0,   278,     0,   279,
     280,   281,     0,   282,   283,   284,     0,   285,   286,   287,
     288,   289,   290,   291,   292,   293,     0,   294,   295,   296,
       0,     0,   297,   298,   299,     0,     0,     0,     0,   125,
       0,     0,     0,   723,   125,   261,     0,   262,     0,     0,
       0,     0,     0,     0,     0,     0,   126,     0,   125,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     125,     0,     0,     0,   868,     0,     0,     0,     0,   125,
       0,     0,     0,   125,     0,     0,   890,   125,     0,     0,
       0,     0,     0,     0,     0,   895,     0,     0,     0,   897,
       0,   231,     0,    84,     0,     0,     0,   336,     0,     0,
       0,     0,   231,     0,   126,   344,     0,     0,     0,     0,
       0,     0,     0,   125,     0,   336,     0,     0,   379,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   922,
       0,     0,   336,     0,     0,     0,   391,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   336,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   125,   125,   125,   125,     0,     0,   126,
       0,   126,     0,     0,   126,     0,     0,   126,     0,     0,
       0,     0,   125,   125,     0,     0,   126,     0,     0,   126,
       0,     0,     0,     0,     0,     0,     0,   188,   188,   188,
     188,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   126,     0,     0,   458,     0,     0,     0,     0,     0,
       0,     0,     0,   126,     0,   126,     0,     0,     0,   126,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   336,   336,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   974,     0,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   142,   523,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,   126,     0,     0,     0,    23,
     126,     0,     0,     0,     0,   143,   144,     0,   145,     0,
      31,     0,     0,     0,   126,    34,    35,     0,    37,   379,
      38,    39,     0,     0,     0,     0,   126,   188,    42,     0,
      43,     0,    45,     0,     0,   126,     0,     0,     0,   126,
     336,     0,     0,   126,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,   606,     0,     0,   610,     0,   188,   126,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
      73,     0,    74,     0,    75,    76,   205,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   148,   975,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   126,
     126,   126,   126,     0,     0,     0,   704,     0,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   142,   126,   126,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   336,    23,     0,     0,     0,     0,     0,   143,
     144,     0,   145,     0,    31,     0,     0,     0,     0,    34,
      35,     0,    37,     0,    38,    39,     0,     0,     5,     0,
       0,     0,    42,     0,    43,   736,    45,     0,   695,     0,
       0,   737,   379,     0,     0,     0,     0,     0,     0,    53,
      54,    55,    56,   146,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,     0,   716,     0,     0,     0,   231,
       0,     0,    67,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,     0,     0,     0,     0,    72,     0,     0,     0,   761,
       0,     0,     0,     0,    73,     0,    74,     0,    75,    76,
     205,     0,     0,     0,    78,     0,     0,     0,   147,     0,
      80,     0,   148,   359,   360,   361,   362,   363,   364,     0,
       0,   365,   366,   367,   368,   369,   370,   738,   739,   740,
     741,     0,     0,   742,     0,     0,     0,   743,   744,   745,
     746,   747,   748,   749,   750,   751,   752,   753,     0,   754,
       0,     0,   755,   756,   757,   758,     0,   759,     0,     0,
     760,     0,   803,     0,     0,   379,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   716,     0,     0,     0,     0,
       0,     0,     0,    -2,     4,     0,     5,     6,     7,     8,
       9,    10,     0,   610,     0,    11,    12,    13,    14,     0,
     842,    15,    16,    17,    18,  -295,    19,    20,    21,     0,
      22,    23,     0,    24,     0,    25,    26,    27,    28,    29,
      30,     0,    31,    32,     0,  -180,    33,    34,    35,    36,
      37,   -42,    38,    39,     0,    40,     0,     0,     0,    41,
      42,  -295,    43,    44,    45,    46,  -180,   -42,    47,    48,
       0,  -295,    49,    50,     0,    51,    52,    53,    54,    55,
      56,    57,     0,    58,     0,    59,    60,    61,     0,    62,
      63,    64,  -295,     0,     0,    65,     0,    66,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,     0,
       0,     0,     0,    72,     0,   905,     0,     0,     0,     0,
       0,     0,    73,     0,    74,     0,    75,    76,     0,    77,
       0,     0,    78,     0,     0,     0,    79,     0,    80,     4,
      81,     5,     6,     7,     8,     9,    10,     0,  -489,     0,
      11,    12,    13,    14,  -489,     0,    15,    16,    17,    18,
    -295,    19,    20,    21,  -489,    22,    23,  -489,    24,     0,
      25,    26,    27,    28,    29,    30,     0,    31,    32,     0,
    -180,    33,    34,    35,    36,    37,   -42,    38,    39,     0,
      40,     0,     0,     0,    41,    42,  -295,    43,     0,    45,
      46,  -180,   -42,    47,    48,  -489,  -295,    49,    50,  -489,
      51,    52,    53,    54,    55,    56,    57,     0,    58,     0,
      59,    60,    61,     0,    62,    63,    64,  -295,     0,     0,
      65,     0,    66,     0,     0,  -489,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -489,    69,  -489,  -489,  -489,  -489,  -489,  -489,  -489,     0,
    -489,  -489,  -489,  -489,  -489,  -489,  -489,  -489,  -489,    74,
    -489,  -489,  -489,     0,    77,  -489,  -489,  -489,     0,     0,
       0,    79,  -489,    80,   243,    81,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,     0,
       0,    15,    16,    17,    18,  -295,    19,    20,    21,     0,
      22,    23,     0,    24,     0,    25,    26,    27,    28,    29,
      30,     0,    31,    32,     0,  -180,    33,    34,    35,    36,
      37,   -42,    38,    39,     0,    40,     0,     0,     0,    41,
      42,  -295,    43,    44,    45,    46,  -180,   -42,    47,    48,
       0,  -295,    49,    50,     0,    51,    52,    53,    54,    55,
      56,    57,     0,    58,     0,    59,    60,    61,     0,    62,
      63,    64,  -295,     0,     0,    65,     0,    66,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,     0,
       0,     0,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,    73,     0,    74,     0,    75,    76,   205,    77,
       0,     0,    78,     0,     0,     0,    79,   244,    80,     4,
      81,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,     0,     0,    15,    16,    17,    18,
    -295,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,    30,     0,    31,    32,     0,
    -180,    33,    34,    35,    36,    37,   -42,    38,    39,     0,
      40,     0,     0,     0,    41,    42,  -295,    43,    44,    45,
      46,  -180,   -42,    47,    48,     0,  -295,    49,    50,     0,
      51,    52,    53,    54,    55,    56,    57,     0,    58,     0,
      59,    60,    61,     0,    62,    63,    64,  -295,     0,     0,
      65,     0,    66,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,     0,     0,     0,     0,    72,     0,
       0,     0,     0,     0,     0,     0,     0,    73,     0,    74,
       0,    75,    76,     0,    77,     0,     0,    78,     0,     0,
       0,    79,   415,    80,   243,    81,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,     0,
       0,    15,    16,    17,    18,  -295,    19,    20,    21,     0,
      22,    23,     0,    24,     0,    25,    26,    27,    28,    29,
      30,     0,    31,    32,     0,  -180,    33,    34,    35,    36,
      37,   -42,    38,    39,     0,    40,     0,     0,     0,    41,
      42,  -295,    43,    44,    45,    46,  -180,   -42,    47,    48,
       0,  -295,    49,    50,     0,    51,    52,    53,    54,    55,
      56,    57,     0,    58,     0,    59,    60,    61,     0,    62,
      63,    64,  -295,     0,     0,    65,     0,    66,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,     0,
       0,     0,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,    73,     0,    74,     0,    75,    76,     0,    77,
       0,     0,    78,     0,     0,     0,    79,   244,    80,   829,
      81,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,     0,     0,    15,    16,    17,    18,
    -295,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,    30,     0,    31,    32,     0,
    -180,    33,    34,    35,    36,    37,   -42,    38,    39,     0,
      40,     0,     0,     0,    41,    42,  -295,    43,    44,    45,
      46,  -180,   -42,    47,    48,     0,  -295,    49,    50,     0,
      51,    52,    53,    54,    55,    56,    57,     0,    58,     0,
      59,    60,    61,     0,    62,    63,    64,  -295,     0,     0,
      65,     0,    66,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,     0,     0,     0,     0,    72,     0,
       0,     0,     0,     0,     0,     0,     0,    73,     0,    74,
       0,    75,    76,     0,    77,     0,     0,    78,     0,     0,
       0,    79,   830,    80,     4,    81,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,     0,
       0,    15,    16,    17,    18,  -295,    19,    20,    21,     0,
      22,    23,     0,    24,     0,    25,    26,    27,    28,    29,
      30,     0,    31,    32,     0,  -180,    33,    34,    35,    36,
      37,   -42,    38,    39,     0,    40,     0,     0,     0,    41,
      42,  -295,    43,    44,    45,    46,  -180,   -42,    47,    48,
       0,  -295,    49,    50,     0,    51,    52,    53,    54,    55,
      56,    57,     0,    58,     0,    59,    60,    61,     0,    62,
      63,    64,  -295,     0,     0,    65,     0,    66,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,     0,
       0,     0,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,    73,     0,    74,     0,    75,    76,     0,    77,
       0,     0,    78,     0,     0,     0,    79,   899,    80,     4,
      81,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,     0,     0,    15,    16,    17,    18,
    -295,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,    30,     0,    31,    32,     0,
    -180,    33,    34,    35,    36,    37,   -42,    38,    39,     0,
      40,     0,     0,     0,    41,    42,  -295,    43,   257,    45,
      46,  -180,   -42,    47,    48,     0,  -295,    49,    50,     0,
      51,    52,    53,    54,    55,    56,    57,     0,    58,     0,
      59,    60,    61,     0,    62,    63,    64,  -295,     0,     0,
      65,     0,    66,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,     0,     0,     0,     0,    72,     0,
       0,     0,     0,     0,     0,     0,     0,    73,     0,    74,
       0,    75,    76,     0,    77,     0,     0,    78,     0,     0,
       0,    79,     0,    80,     4,    81,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,     0,
       0,    15,    16,    17,    18,  -295,    19,    20,    21,     0,
      22,    23,     0,    24,     0,    25,    26,    27,    28,    29,
      30,     0,    31,    32,     0,  -180,    33,    34,    35,    36,
      37,   -42,    38,    39,     0,    40,     0,     0,     0,    41,
      42,  -295,    43,     0,    45,    46,  -180,   -42,    47,    48,
       0,  -295,    49,    50,     0,    51,    52,    53,    54,    55,
      56,    57,     0,    58,     0,    59,    60,    61,     0,    62,
      63,    64,  -295,     0,     0,    65,     0,    66,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,     0,
       0,     0,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,    73,     0,    74,     0,    75,    76,     0,    77,
       0,     0,    78,     0,     0,     0,    79,     0,    80,     4,
      81,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,     0,     0,    15,    16,    17,    18,
    -295,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,   526,     0,    31,    32,     0,
    -180,    33,    34,    35,    36,    37,   -42,    38,    39,     0,
      40,     0,     0,     0,    41,    42,  -295,    43,     0,    45,
      46,  -180,   -42,    47,    48,     0,  -295,    49,    50,     0,
      51,    52,    53,    54,    55,    56,    57,     0,    58,     0,
      59,    60,    61,     0,    62,    63,    64,  -295,     0,     0,
      65,     0,    66,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,     0,     0,     0,     0,    72,     0,
       0,     0,     0,     0,     0,     0,     0,    73,     0,    74,
       0,    75,    76,     0,    77,     0,     0,    78,     0,     0,
       0,    79,     0,    80,     4,    81,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,     0,
       0,    15,    16,    17,    18,  -295,    19,    20,    21,     0,
      22,    23,     0,    24,     0,    25,    26,    27,    28,    29,
     530,     0,    31,    32,     0,  -180,    33,    34,    35,    36,
      37,   -42,    38,    39,     0,    40,     0,     0,     0,    41,
      42,  -295,    43,     0,    45,    46,  -180,   -42,    47,    48,
       0,  -295,    49,    50,     0,    51,    52,    53,    54,    55,
      56,    57,     0,    58,     0,    59,    60,    61,     0,    62,
      63,    64,  -295,     0,     0,    65,     0,    66,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,     0,
       0,     0,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,    73,     0,    74,     0,    75,    76,     0,    77,
       0,     0,    78,     0,     0,     0,    79,     0,    80,     4,
      81,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,     0,     0,    15,    16,    17,    18,
    -295,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,   784,     0,    31,    32,     0,
    -180,    33,    34,    35,    36,    37,   -42,    38,    39,     0,
      40,     0,     0,     0,    41,    42,  -295,    43,     0,    45,
      46,  -180,   -42,    47,    48,     0,  -295,    49,    50,     0,
      51,    52,    53,    54,    55,    56,    57,     0,    58,     0,
      59,    60,    61,     0,    62,    63,    64,  -295,     0,     0,
      65,     0,    66,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,     0,     0,     0,     0,    72,     0,
       0,     0,     0,     0,     0,     0,     0,    73,     0,    74,
       0,    75,    76,     0,    77,     0,     0,    78,     0,     0,
       0,    79,     0,    80,     4,    81,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,     0,
       0,    15,    16,    17,    18,  -295,    19,    20,    21,     0,
      22,    23,     0,    24,     0,    25,    26,    27,    28,    29,
     786,     0,    31,    32,     0,  -180,    33,    34,    35,    36,
      37,   -42,    38,    39,     0,    40,     0,     0,     0,    41,
      42,  -295,    43,     0,    45,    46,  -180,   -42,    47,    48,
       0,  -295,    49,    50,     0,    51,    52,    53,    54,    55,
      56,    57,     0,    58,     0,    59,    60,    61,     0,    62,
      63,    64,  -295,     0,     0,    65,     0,    66,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,     0,
       0,     0,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,    73,     0,    74,     0,    75,    76,     0,    77,
       0,     0,    78,     0,     0,     0,    79,     0,    80,     4,
      81,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,     0,     0,    15,    16,    17,    18,
    -295,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,   789,     0,    31,    32,     0,
    -180,    33,    34,    35,    36,    37,   -42,    38,    39,     0,
      40,     0,     0,     0,    41,    42,  -295,    43,     0,    45,
      46,  -180,   -42,    47,    48,     0,  -295,    49,    50,     0,
      51,    52,    53,    54,    55,    56,    57,     0,    58,     0,
      59,    60,    61,     0,    62,    63,    64,  -295,     0,     0,
      65,     0,    66,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,     0,     0,     0,     0,    72,     0,
       0,     0,     0,     0,     0,     0,     0,    73,     0,    74,
       0,    75,    76,     0,    77,     0,     0,    78,     0,     0,
       0,    79,     0,    80,     4,    81,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,     0,
       0,    15,    16,    17,    18,  -295,    19,    20,    21,     0,
      22,    23,     0,    24,     0,    25,    26,    27,    28,    29,
     792,     0,    31,    32,     0,  -180,    33,    34,    35,    36,
      37,   -42,    38,    39,     0,    40,     0,     0,     0,    41,
      42,  -295,    43,     0,    45,    46,  -180,   -42,    47,    48,
       0,  -295,    49,    50,     0,    51,    52,    53,    54,    55,
      56,    57,     0,    58,     0,    59,    60,    61,     0,    62,
      63,    64,  -295,     0,     0,    65,     0,    66,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,     0,
       0,     0,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,    73,     0,    74,     0,    75,    76,     0,    77,
       0,     0,    78,     0,     0,     0,    79,     0,    80,     4,
      81,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,     0,     0,    15,    16,    17,    18,
    -295,    19,    20,    21,     0,   814,    23,     0,    24,     0,
      25,    26,    27,    28,    29,    30,     0,    31,    32,     0,
    -180,    33,    34,    35,    36,    37,   -42,    38,    39,     0,
      40,     0,     0,     0,    41,    42,  -295,    43,     0,    45,
      46,  -180,   -42,    47,    48,     0,  -295,    49,    50,     0,
      51,    52,    53,    54,    55,    56,    57,     0,    58,     0,
      59,    60,    61,     0,    62,    63,    64,  -295,     0,     0,
      65,     0,    66,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,     0,     0,     0,     0,    72,     0,
       0,     0,     0,     0,     0,     0,     0,    73,     0,    74,
       0,    75,    76,     0,    77,     0,     0,    78,     0,     0,
       0,    79,     0,    80,     4,    81,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,     0,
       0,    15,    16,    17,    18,  -295,    19,    20,    21,     0,
      22,    23,     0,    24,     0,    25,    26,    27,    28,    29,
     820,     0,    31,    32,     0,  -180,    33,    34,    35,    36,
      37,   -42,    38,    39,     0,    40,     0,     0,     0,    41,
      42,  -295,    43,     0,    45,    46,  -180,   -42,    47,    48,
       0,  -295,    49,    50,     0,    51,    52,    53,    54,    55,
      56,    57,     0,    58,     0,    59,    60,    61,     0,    62,
      63,    64,  -295,     0,     0,    65,     0,    66,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,     0,
       0,     0,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,    73,     0,    74,     0,    75,    76,     0,    77,
       0,     0,    78,     0,     0,     0,    79,     0,    80,     4,
      81,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,     0,     0,    15,    16,    17,    18,
    -295,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,   824,     0,    31,    32,     0,
    -180,    33,    34,    35,    36,    37,   -42,    38,    39,     0,
      40,     0,     0,     0,    41,    42,  -295,    43,     0,    45,
      46,  -180,   -42,    47,    48,     0,  -295,    49,    50,     0,
      51,    52,    53,    54,    55,    56,    57,     0,    58,     0,
      59,    60,    61,     0,    62,    63,    64,  -295,     0,     0,
      65,     0,    66,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,     0,     0,     0,     0,    72,     0,
       0,     5,     6,     7,     8,     9,    10,    73,     0,    74,
     142,    75,    76,    14,    77,     0,     0,    78,     0,     0,
       0,    79,     0,    80,     0,    81,    23,     0,     0,     0,
       0,     0,   143,   144,     0,   145,     0,    31,     0,     0,
       0,     0,    34,    35,     0,    37,     0,    38,    39,     0,
       0,     0,     0,     0,     0,    42,     0,    43,     0,    45,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    54,    55,    56,   146,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,     0,     0,     0,     0,    72,     0,
       0,     0,     0,     0,     0,     0,     0,    73,     0,    74,
       0,    75,    76,   205,     0,     0,     0,    78,     0,     0,
       0,   147,     0,    80,     0,   148,   599,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   142,     0,     0,    14,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    23,     0,     0,     0,     0,     0,   143,   144,
       0,   145,     0,    31,     0,     0,     0,     0,    34,    35,
       0,    37,     0,    38,    39,     0,     0,     0,     0,     0,
       0,    42,     0,    43,     0,    45,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    53,    54,
      55,    56,   146,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,   205,
       0,     0,     0,    78,     0,     0,     0,   147,     0,    80,
       0,   148,   925,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   142,     0,     0,    14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    23,     0,
       0,     0,     0,     0,   143,   144,     0,   145,     0,    31,
       0,     0,     0,     0,    34,    35,     0,    37,     0,    38,
      39,     0,     0,     0,     0,     0,     0,    42,     0,    43,
       0,    45,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,    54,    55,    56,   146,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    67,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,     0,     0,     0,     0,
      72,     0,     0,     0,     0,     0,     0,     0,     0,    73,
       0,    74,     0,    75,    76,     0,     0,     0,     0,    78,
       0,     0,     0,   147,     0,    80,     0,   148,   604,     5,
       6,     7,     8,   169,    10,   170,     0,     0,   142,     0,
       0,    14,     0,     0,   -83,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     0,     0,     0,
     143,   144,     0,   145,     0,    31,     0,     0,  -461,     0,
      34,    35,     0,    37,     0,    38,    39,     0,     0,     0,
       0,     0,     0,    42,     0,    43,     0,    45,     0,  -461,
       0,     0,   -83,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,    56,   146,     0,     0,     0,     0,     0,
     171,     0,   -83,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,    72,     0,     0,     5,
       6,     7,     8,   169,    10,    73,     0,    74,   142,    75,
      76,    14,     0,     0,   -83,    78,     0,     0,     0,   147,
       0,    80,     0,   148,    23,     0,     0,     0,     0,     0,
     143,   144,     0,   145,     0,    31,     0,     0,  -461,     0,
      34,    35,     0,    37,     0,    38,    39,     0,     0,     0,
       0,     0,     0,    42,     0,    43,     0,    45,     0,  -461,
       0,     0,   -83,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,    56,   146,     0,     0,     0,     0,     0,
     171,     0,   -83,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,    72,     0,     0,     5,
       6,     7,     8,     9,    10,    73,     0,    74,   142,    75,
      76,    14,     0,     0,     0,    78,     0,     0,     0,   147,
       0,    80,     0,   148,    23,     0,     0,     0,     0,     0,
     143,   144,     0,   145,     0,    31,     0,     0,     0,     0,
      34,    35,     0,    37,     0,    38,    39,     0,     0,     0,
       0,     0,     0,    42,     0,    43,     0,    45,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,    56,   146,     0,     0,     0,   209,   210,
       0,   248,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,    72,   249,     0,     0,
       0,     0,     0,     0,     0,    73,     0,    74,     0,    75,
      76,   205,     0,     0,     0,    78,     0,     0,     0,   147,
       0,    80,     0,   148,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   142,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   145,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,   209,   210,     0,   248,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
      73,     0,    74,     0,    75,    76,   205,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,   593,   148,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   142,     0,
       0,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     0,     0,     0,
     143,   144,     0,   145,     0,    31,     0,     0,     0,     0,
      34,    35,     0,    37,     0,    38,    39,     0,     0,     0,
       0,     0,     0,    42,     0,    43,     0,    45,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,    56,   146,     0,     0,     0,   209,   210,
       0,   248,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,    72,     0,     0,     0,
       0,     0,     0,     0,     0,    73,     0,    74,     0,    75,
      76,   205,     0,     0,     0,    78,     0,     0,     0,   147,
       0,    80,   595,   148,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   142,     0,     0,    14,     0,     0,     0,
       0,     0,     0,   497,     0,     0,     0,     0,     0,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   145,   498,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,   499,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
     500,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,    14,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   148,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   145,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,   209,   210,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,    14,   211,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   148,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   145,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,   209,   210,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
      73,     0,    74,     0,    75,    76,   205,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   148,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   142,     0,
       0,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     0,     0,     0,
     143,   144,     0,   145,     0,    31,     0,     0,     0,     0,
      34,    35,     0,    37,     0,    38,    39,   706,     0,     0,
       0,     0,     0,    42,     0,    43,     0,    45,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,    56,   146,     0,     0,     0,   209,   210,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,    72,     0,     0,     5,
       6,     7,     8,     9,    10,    73,     0,    74,   142,    75,
      76,    14,     0,     0,     0,    78,     0,     0,     0,   147,
       0,    80,     0,   148,    23,     0,     0,     0,     0,     0,
     143,   144,     0,   145,     0,    31,     0,     0,     0,     0,
      34,    35,     0,    37,     0,    38,    39,     0,     0,     0,
       0,     0,     0,    42,   174,    43,     0,    45,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,    56,   146,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,   175,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,    72,     0,     0,     5,
       6,     7,     8,     9,    10,    73,     0,    74,   142,    75,
      76,    14,     0,     0,     0,    78,     0,   179,     0,   147,
       0,    80,     0,   148,    23,     0,     0,     0,     0,   180,
     143,   144,     0,   145,     0,    31,     0,     0,     0,     0,
      34,    35,     0,    37,     0,    38,    39,     0,     0,     0,
       0,     0,     0,    42,     0,    43,     0,    45,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,    56,   146,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,    72,     0,     0,     5,
       6,     7,     8,     9,    10,    73,     0,    74,   142,    75,
      76,    14,     0,     0,     0,    78,     0,     0,     0,   147,
       0,    80,     0,   148,    23,     0,     0,     0,     0,     0,
     143,   144,     0,   145,     0,    31,     0,     0,  -461,     0,
      34,    35,     0,    37,     0,    38,    39,     0,     0,     0,
       0,     0,     0,    42,     0,    43,     0,    45,     0,  -461,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,    56,   146,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,    72,     0,     0,     5,
       6,     7,     8,     9,    10,    73,     0,    74,   142,    75,
      76,    14,     0,     0,     0,    78,     0,     0,     0,   147,
       0,    80,     0,   148,    23,     0,     0,     0,     0,     0,
     143,   144,     0,   145,     0,    31,     0,     0,     0,     0,
      34,    35,     0,    37,     0,    38,    39,     0,     0,     0,
       0,     0,     0,    42,     0,    43,     0,    45,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,    56,   146,     0,     0,     0,   209,   210,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,    72,     0,     0,     5,
       6,     7,     8,     9,    10,    73,     0,    74,   142,    75,
      76,    14,     0,     0,     0,    78,     0,     0,     0,   147,
       0,    80,     0,   148,    23,     0,     0,     0,     0,     0,
     143,   144,     0,   145,     0,    31,     0,     0,     0,     0,
      34,    35,     0,    37,     0,    38,    39,     0,     0,     0,
       0,     0,     0,    42,     0,    43,     0,    45,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,    56,   146,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,    72,     0,     0,     0,
       0,     0,     0,     0,     0,    73,     0,    74,     0,    75,
      76,   205,     0,     0,     0,    78,     0,     0,     0,   147,
     591,    80,     0,   148,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   142,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   191,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   145,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,    14,     0,     0,     0,
      78,     0,     0,     0,    79,     0,    80,     0,   148,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   145,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
      73,     0,    74,     0,    75,    76,   205,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   148,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   142,     0,
       0,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     0,     0,     0,
     143,   144,     0,   145,     0,    31,     0,     0,     0,     0,
      34,    35,     0,    37,     0,    38,    39,     0,     0,     0,
       0,     0,     0,    42,     0,    43,     0,    45,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,    56,   146,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,   175,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,    72,     0,     0,     5,
       6,     7,     8,     9,    10,    73,     0,    74,   142,    75,
      76,   871,     0,     0,     0,    78,     0,     0,     0,   147,
       0,    80,     0,   148,   872,     0,     0,     0,     0,     0,
     143,   144,     0,   145,     0,    31,     0,     0,     0,     0,
      34,    35,     0,    37,     0,    38,    39,     0,     0,     0,
       0,     0,     0,   873,     0,    43,     0,    45,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     874,   875,    55,    56,   876,     0,     0,     0,     0,     0,
       0,     0,     0,   877,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,    72,     0,     0,     0,
       0,     0,     0,     0,     0,    73,     0,    74,     0,    75,
      76,   205,     0,     0,     0,    78,     0,     0,     0,   147,
       0,    80,     0,   878,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   142,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   145,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
      73,     0,    74,     0,    75,    76,   205,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   878,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   142,     0,
       0,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     0,     0,     0,
     143,   144,     0,   856,     0,    31,     0,     0,     0,     0,
      34,    35,     0,    37,     0,    38,    39,     0,     0,     0,
       0,     0,     0,    42,     0,    43,     0,    45,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,    56,   146,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,    72,     0,     0,     0,
       0,     0,     0,     0,     0,    73,     0,    74,     0,    75,
      76,   205,     0,     0,     0,    78,     0,     0,     0,   147,
       0,    80,     0,   148,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   142,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   145,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,    14,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   148,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   145,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,    14,     0,     0,     0,
      78,     0,     0,     0,    79,     0,    80,     0,   148,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   145,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,   563,     0,     0,     0,
     536,     0,     0,     0,   147,     0,    80,     0,   148,   564,
       0,     0,     0,     0,     0,   143,   144,     0,   145,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,   565,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   566,   567,    55,    56,   568,
       0,     0,     0,     0,     0,     0,     0,     0,   569,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,    14,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   570,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   145,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,    14,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   570,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   649,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,    14,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   148,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   652,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,    14,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   148,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   850,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,    14,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   148,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   851,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,    14,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   148,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   853,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,    14,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   148,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   854,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,    14,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   148,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   855,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,    14,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   148,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   856,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,   938,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   148,   939,
       0,     0,     0,     0,     0,   143,   144,     0,   145,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,   940,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   941,   942,    55,    56,   943,
       0,     0,     0,     0,     0,     0,     0,     0,   944,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,    14,     0,     0,     0,
      78,     0,     0,     0,   147,     0,    80,     0,   945,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   145,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
      73,     0,    74,   142,    75,    76,    14,     0,     0,     0,
      78,     0,     0,     0,     0,     0,    80,     0,   945,    23,
       0,     0,     0,     0,     0,   143,   144,     0,   145,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   142,     0,     0,    14,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   145,
      73,    31,    74,     0,     0,    76,    34,    35,     0,    37,
      78,    38,    39,     0,   147,     0,    80,     0,   148,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -344,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,     0,     0,     0,  -344,
       0,     0,     0,  -344,     0,     0,     5,     6,     7,     8,
       9,    10,     0,    74,     0,   142,     0,   205,    14,     0,
       0,     0,     0,     0,     0,   147,     0,    80,  -344,   878,
       0,    23,     0,     0,     0,     0,     0,   143,   144,     0,
     145,     0,    31,     0,     0,     0,     0,    34,    35,     0,
      37,     0,    38,    39,     0,     0,     0,     0,     0,     0,
      42,     0,    43,     0,    45,     0,     0,     0,     0,     0,
       0,     0,     0,  -314,     0,     0,     0,    53,    54,    55,
      56,   146,     0,     0,  -314,     0,     0,     0,     0,     0,
      63,     0,     0,     0,  -314,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,     0,
       0,     0,     0,     0,     0,     0,    69,     0,     0,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   142,
       0,     0,    14,     0,    74,     0,     0,   314,   315,  -314,
       0,     0,  -470,     0,  -470,    23,  -314,     0,    80,     0,
     945,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     5,     6,     7,
       8,     9,    10,     0,    63,     0,   142,     0,     0,    14,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    23,     0,     0,     0,     0,     0,   143,   144,
      69,   145,     0,    31,     0,     0,     0,     0,    34,    35,
       0,    37,     0,    38,    39,     0,     0,     0,    74,     0,
       0,    42,     5,    43,     0,    45,     0,   857,     0,   736,
     147,     0,    80,     0,   570,   737,     0,     0,    53,    54,
      55,    56,   146,     0,  -166,     0,     0,  -166,  -166,     0,
       0,    63,     0,     0,     0,     0,  -166,     0,  -166,  -166,
       0,     0,  -166,     0,     0,     0,  -166,     0,  -166,     0,
       0,     0,     0,     0,     0,     0,     0,    69,     0,     0,
       0,     0,  -166,     0,  -166,     0,  -166,     0,  -166,  -166,
       0,  -166,  -166,     0,  -166,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   272,     0,    80,
    -166,   148,  -166,   273,     0,  -166,     0,   359,   360,   361,
     362,   363,   364,   274,     0,   365,   366,   367,   368,   369,
     370,   738,   739,   740,   741,     0,     0,   742,     0,     0,
       0,   743,   744,   745,   746,   747,   748,   749,   750,   751,
     752,   753,     0,   754,     0,     0,   755,   756,   757,   758,
       0,   759,  -166,     0,   275,     0,     0,     0,   276,     0,
    -166,   272,     0,     0,     0,     0,     0,   273,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   274,     0,     0,
       0,     0,     0,     0,   277,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   278,
       0,   279,   280,   281,     0,   282,   283,   284,     0,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   275,   294,
     295,   296,   276,     0,   297,   298,   299,     0,     0,     0,
     272,     0,     0,     0,     0,   778,   273,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   274,     0,   277,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   278,     0,   279,   280,   281,     0,   282,
     283,   284,     0,   285,   286,   287,   288,   289,   290,   291,
     292,   293,     0,   294,   295,   296,     0,   275,   297,   298,
     299,   276,     0,     0,   272,     0,     0,     0,     0,   934,
     273,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274,     0,     0,     0,     0,     0,     0,   277,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   278,     0,   279,   280,   281,     0,   282,   283,
     284,     0,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   275,   294,   295,   296,   276,     0,   297,   298,   299,
       0,     0,     0,   272,     0,     0,     0,     0,   988,   273,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   274,
       0,   277,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   278,     0,   279,   280,
     281,     0,   282,   283,   284,     0,   285,   286,   287,   288,
     289,   290,   291,   292,   293,     0,   294,   295,   296,     0,
     275,   297,   298,   299,   276,     0,     0,   272,     0,     0,
     592,     0,     0,   273,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   274,     0,     0,     0,     0,     0,     0,
     277,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   278,     0,   279,   280,   281,
       0,   282,   283,   284,     0,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   275,   294,   295,   296,   276,     0,
     297,   298,   299,     0,     0,     0,   358,     0,     0,   904,
       0,     0,     0,     0,     0,   272,     0,     0,     0,     0,
       0,   273,     0,     0,   277,   359,   360,   361,   362,   363,
     364,   274,     0,   365,   366,   367,   368,   369,   370,   278,
       0,   279,   280,   281,     0,   282,   283,   284,     0,   285,
     286,   287,   288,   289,   290,   291,   292,   293,     0,   294,
     295,   296,     0,     0,   297,   298,   299,     0,     0,     0,
     327,     0,   275,     0,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,   783,     0,     0,     0,     0,     0,
       0,     0,     0,   272,     0,     0,     0,     0,     0,   273,
       0,     0,   277,   359,   360,   361,   362,   363,   364,   274,
       0,   365,   366,   367,   368,   369,   370,   278,     0,   279,
     280,   281,     0,   282,   283,   284,     0,   285,   286,   287,
     288,   289,   290,   291,   292,   293,     0,   294,   295,   296,
       0,     0,   297,   298,   299,     0,     0,     0,   327,     0,
     275,     0,     0,     0,   276,     0,     0,     0,     0,     0,
       0,     0,   788,     0,     0,     0,     0,     0,     0,     0,
       0,   272,     0,     0,     0,     0,     0,   273,     0,     0,
     277,   359,   360,   361,   362,   363,   364,   274,     0,   365,
     366,   367,   368,   369,   370,   278,     0,   279,   280,   281,
       0,   282,   283,   284,     0,   285,   286,   287,   288,   289,
     290,   291,   292,   293,     0,   294,   295,   296,     0,     0,
     297,   298,   299,     0,     0,     0,   327,     0,   275,     0,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
     917,     0,     0,     0,     0,     0,     0,     0,     0,   272,
       0,     0,     0,     0,     0,   273,     0,     0,   277,   359,
     360,   361,   362,   363,   364,   274,     0,   365,   366,   367,
     368,   369,   370,   278,     0,   279,   280,   281,     0,   282,
     283,   284,     0,   285,   286,   287,   288,   289,   290,   291,
     292,   293,     0,   294,   295,   296,     0,     0,   297,   298,
     299,     0,     0,     0,   327,     0,   275,     0,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,   918,     0,
       0,     0,     0,     0,     0,     0,     0,   272,     0,     0,
       0,     0,     0,   273,     0,     0,   277,   359,   360,   361,
     362,   363,   364,   274,     0,   365,   366,   367,   368,   369,
     370,   278,     0,   279,   280,   281,     0,   282,   283,   284,
       0,   285,   286,   287,   288,   289,   290,   291,   292,   293,
       0,   294,   295,   296,     0,     0,   297,   298,   299,     0,
       0,     0,   327,     0,   275,     0,     0,     0,   276,     0,
       0,     0,     0,     0,     0,     0,   919,     0,     0,     0,
       0,     0,     0,     0,     0,   272,     0,     0,     0,     0,
       0,   273,     0,     0,   277,   359,   360,   361,   362,   363,
     364,   274,     0,   365,   366,   367,   368,   369,   370,   278,
       0,   279,   280,   281,     0,   282,   283,   284,     0,   285,
     286,   287,   288,   289,   290,   291,   292,   293,     0,   294,
     295,   296,     0,     0,   297,   298,   299,     0,     0,     0,
     327,     0,   275,     0,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,   920,     0,     0,     0,     0,     0,
       0,     0,     0,   272,     0,     0,     0,     0,     0,   273,
       0,     0,   277,   359,   360,   361,   362,   363,   364,   274,
       0,   365,   366,   367,   368,   369,   370,   278,     0,   279,
     280,   281,     0,   282,   283,   284,     0,   285,   286,   287,
     288,   289,   290,   291,   292,   293,     0,   294,   295,   296,
       0,     0,   297,   298,   299,     0,     0,     0,   327,     0,
     275,     0,     0,     0,   276,     0,     0,     0,     0,     0,
       0,     0,   936,     0,     0,     0,     0,     0,     0,     0,
       0,   272,     0,     0,     0,     0,     0,   273,     0,     0,
     277,   359,   360,   361,   362,   363,   364,   274,     0,   365,
     366,   367,   368,   369,   370,   278,     0,   279,   280,   281,
       0,   282,   283,   284,     0,   285,   286,   287,   288,   289,
     290,   291,   292,   293,     0,   294,   295,   296,     0,     0,
     297,   298,   299,     0,     0,     0,   327,     0,   275,     0,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
     937,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,   359,
     360,   361,   362,   363,   364,     0,     0,   365,   366,   367,
     368,   369,   370,   278,     0,   279,   280,   281,     0,   282,
     283,   284,     0,   285,   286,   287,   288,   289,   290,   291,
     292,   293,     0,   294,   295,   296,   272,     0,   297,   298,
     299,     0,   273,     0,   327,     0,     0,     0,     0,     0,
       0,     0,   274,   350,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,     0,     0,     0,     0,     0,
     272,     0,     0,     0,     0,     0,   273,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   274,   347,     0,     0,
       0,     0,     0,   275,     0,     0,     0,   276,   348,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   352,
     272,     0,     0,   277,     0,     0,   273,   275,     0,     0,
       0,   276,     0,     0,     0,     0,   274,   191,   278,     0,
     279,   280,   281,     0,   282,   283,   284,     0,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   277,   294,   295,
     296,     0,     0,   297,   298,   299,     0,     0,     0,   327,
       0,     0,   278,     0,   279,   280,   281,   275,   282,   283,
     284,   276,   285,   286,   287,   288,   289,   290,   291,   292,
     293,     0,   294,   295,   296,   272,     0,   297,   298,   299,
       0,   273,     0,   327,     0,     0,     0,   277,     0,     0,
       0,   274,   675,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   278,     0,   279,   280,   281,     0,   282,   283,
     284,     0,   285,   286,   287,   288,   289,   290,   291,   292,
     293,     0,   294,   295,   296,   272,     0,   297,   298,   299,
       0,   273,   275,   327,     0,     0,   276,     0,     0,     0,
       0,   274,   669,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   352,   272,
       0,     0,   277,     0,     0,   273,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   274,     0,   278,     0,   279,
     280,   281,   275,   282,   283,   284,   276,   285,   286,   287,
     288,   289,   290,   291,   292,   293,     0,   294,   295,   296,
       0,     0,   297,   298,   299,     0,     0,     0,   327,   272,
       0,     0,   277,     0,     0,   273,   275,     0,     0,     0,
     276,     0,     0,     0,     0,   274,     0,   278,   687,   279,
     280,   281,     0,   282,   283,   284,     0,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   277,   294,   295,   296,
       0,     0,   297,   298,   299,     0,     0,     0,   327,     0,
       0,   278,     0,   279,   280,   281,   275,   282,   283,   284,
     276,   285,   286,   287,   288,   289,   290,   291,   292,   293,
       0,   294,   295,   296,   272,     0,   297,   298,   299,     0,
     273,     0,   327,     0,     0,     0,   277,     0,     0,     0,
     274,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   278,     0,   279,   280,   281,     0,   282,   283,   284,
       0,   285,   286,   287,   288,   289,   290,   291,   292,   293,
       0,   294,   295,   296,   272,     0,   297,   298,   299,     0,
     273,   275,   397,     0,     0,   276,     0,     0,     0,     0,
     274,     0,     0,     0,     0,   355,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,     0,     0,     0,     0,   356,     0,     0,
       0,     0,     0,     0,     0,     0,   278,     0,   279,   280,
     281,   275,   282,   283,   284,   276,   285,   286,   287,   288,
     289,   290,   291,   292,   293,     0,   294,   295,   296,   272,
       0,   297,   298,   299,     0,   273,  -444,     0,     0,     0,
       0,   277,     0,     0,     0,   274,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   278,   329,   279,   280,
     281,     0,   282,   283,   284,     0,   285,   286,   287,   288,
     289,   290,   291,   292,   293,     0,   294,   295,   296,   272,
     357,   297,   298,   299,     0,   273,   275,     0,     0,     0,
     276,     0,     0,     0,     0,   274,     0,     0,     0,     0,
     410,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   137,     0,     0,     0,   277,     0,     0,     0,
       0,     0,   411,     0,     0,     0,     0,     0,     0,     0,
       0,   278,     0,   279,   280,   281,   275,   282,   283,   284,
     276,   285,   286,   287,   288,   289,   290,   291,   292,   293,
       0,   294,   295,   296,   272,     0,   297,   298,   299,     0,
     273,     0,     0,     0,     0,     0,   277,     0,     0,     0,
     274,   506,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   278,   507,   279,   280,   281,     0,   282,   283,   284,
       0,   285,   286,   287,   288,   289,   290,   291,   292,   293,
       0,   294,   295,   296,   272,     0,   297,   298,   299,     0,
     273,   275,     0,     0,     0,   276,     0,     0,     0,     0,
     274,   508,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,     0,     0,     0,     0,     0,   272,     0,
       0,   277,     0,     0,   273,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   274,     0,   278,     0,   279,   280,
     281,   275,   282,   283,   284,   276,   285,   286,   287,   288,
     289,   290,   291,   292,   293,     0,   294,   295,   296,     0,
       0,   297,   298,   299,   272,     0,     0,     0,     0,     0,
     273,   277,     0,     0,     0,   275,     0,     0,     0,   276,
     274,     0,     0,     0,     0,     0,   278,     0,   279,   280,
     281,     0,   282,   283,   284,     0,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   277,   294,   295,   296,     0,
       0,   297,   298,   299,     0,     0,     0,     0,     0,     0,
     278,   275,   279,   280,   281,   276,   282,   283,   284,     0,
     285,   286,   287,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,   272,   401,   297,   298,   299,     0,   273,
       0,   277,     0,     0,     0,     0,     0,     0,     0,   274,
       0,     0,     0,     0,     0,     0,   278,     0,   279,   280,
     281,     0,   282,   283,   284,     0,   285,   286,   287,   288,
     289,   290,   291,   292,   293,     0,   294,   295,   296,   272,
     403,   297,   298,   299,     0,   273,     0,     0,     0,     0,
     275,     0,     0,     0,   276,   274,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   278,   275,   279,   280,   281,
     276,   282,   283,   284,     0,   285,   286,   287,   288,   289,
     290,   291,   292,   293,     0,   294,   295,   296,   272,   405,
     297,   298,   299,     0,   273,     0,   277,     0,     0,     0,
       0,     0,     0,     0,   274,     0,     0,     0,     0,     0,
       0,   278,     0,   279,   280,   281,     0,   282,   283,   284,
       0,   285,   286,   287,   288,   289,   290,   291,   292,   293,
       0,   294,   295,   296,     0,   413,   297,   298,   299,     0,
       0,     0,     0,     0,     0,   275,     0,     0,     0,   276,
       0,     0,   272,     0,     0,     0,     0,     0,   273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   274,     0,
       0,     0,     0,     0,   428,   277,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     278,     0,   279,   280,   281,     0,   282,   283,   284,     0,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   275,
     294,   295,   296,   276,     0,   297,   298,   299,     0,     0,
       0,   510,     0,     0,     0,   272,     0,     0,     0,     0,
       0,   273,     0,     0,     0,     0,     0,     0,     0,   277,
       0,   274,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   278,     0,   279,   280,   281,     0,
     282,   283,   284,     0,   285,   286,   287,   288,   289,   290,
     291,   292,   293,     0,   294,   295,   296,     0,     0,   297,
     298,   299,   275,     0,     0,     0,   276,     0,     0,   272,
       0,     0,     0,     0,     0,   273,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   274,     0,     0,   137,     0,
       0,     0,   277,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   278,     0,   279,
     280,   281,     0,   282,   283,   284,     0,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   275,   294,   295,   296,
     276,     0,   297,   298,   299,     0,     0,     0,     0,     0,
       0,     0,   272,   682,     0,     0,     0,     0,   273,     0,
       0,     0,     0,     0,     0,     0,   277,     0,   274,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   278,     0,   279,   280,   281,     0,   282,   283,   284,
       0,   285,   286,   287,   288,   289,   290,   291,   292,   293,
       0,   294,   295,   296,     0,   659,   297,   298,   299,   275,
       0,     0,     0,   276,     0,     0,   272,     0,     0,     0,
       0,     0,   273,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   274,     0,     0,   685,     0,     0,     0,   277,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   278,     0,   279,   280,   281,     0,
     282,   283,   284,     0,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   275,   294,   295,   296,   276,     0,   297,
     298,   299,     0,     0,     0,   272,     0,     0,     0,     0,
       0,   273,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   274,     0,   277,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   278,     0,
     279,   280,   281,     0,   282,   283,   284,     0,   285,   286,
     287,   288,   289,   290,   291,   292,   293,     0,   294,   295,
     296,     0,   275,   297,   298,   299,   276,     0,     0,   272,
       0,     0,     0,     0,     0,   273,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   274,   773,     0,     0,     0,
       0,   726,   277,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   278,     0,   279,
     280,   281,     0,   282,   283,   284,     0,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   275,   294,   295,   296,
     276,     0,   297,   298,   299,     0,     0,     0,   272,     0,
       0,     0,     0,     0,   273,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   274,   776,   277,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   278,     0,   279,   280,   281,     0,   282,   283,   284,
       0,   285,   286,   287,   288,   289,   290,   291,   292,   293,
       0,   294,   295,   296,     0,   275,   297,   298,   299,   276,
       0,     0,   272,     0,     0,     0,     0,     0,   273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   274,     0,
       0,     0,     0,     0,     0,   277,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     278,     0,   279,   280,   281,     0,   282,   283,   284,     0,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   275,
     294,   295,   296,   276,     0,   297,   298,   299,     0,     0,
       0,   849,     0,     0,     0,   272,     0,     0,     0,     0,
       0,   273,     0,     0,     0,     0,     0,     0,     0,   277,
       0,   274,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   278,     0,   279,   280,   281,     0,
     282,   283,   284,     0,   285,   286,   287,   288,   289,   290,
     291,   292,   293,     0,   294,   295,   296,     0,     0,   297,
     298,   299,   275,     0,     0,     0,   276,     0,     0,   272,
     866,     0,     0,     0,   852,   273,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   274,     0,     0,     0,     0,
       0,     0,   277,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   278,     0,   279,
     280,   281,     0,   282,   283,   284,     0,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   275,   294,   295,   296,
     276,     0,   297,   298,   299,     0,     0,     0,   272,     0,
       0,     0,     0,     0,   273,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   274,     0,   277,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   278,     0,   279,   280,   281,     0,   282,   283,   284,
       0,   285,   286,   287,   288,   289,   290,   291,   292,   293,
       0,   294,   295,   296,     0,   275,   297,   298,   299,   276,
       0,     0,   272,     0,     0,     0,     0,   953,   273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   274,     0,
       0,     0,     0,     0,     0,   277,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     278,     0,   279,   280,   281,     0,   282,   283,   284,     0,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   275,
     294,   295,   296,   276,     0,   297,   298,   299,     0,     0,
       0,   954,     0,     0,     0,   272,     0,     0,     0,     0,
       0,   273,     0,     0,     0,     0,     0,     0,     0,   277,
       0,   274,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   278,     0,   279,   280,   281,     0,
     282,   283,   284,     0,   285,   286,   287,   288,   289,   290,
     291,   292,   293,     0,   294,   295,   296,     0,     0,   297,
     298,   299,   275,     0,     0,     0,   276,     0,     0,   272,
       0,     0,     0,     0,   955,   273,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   274,     0,     0,     0,     0,
       0,     0,   277,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   278,     0,   279,
     280,   281,     0,   282,   283,   284,     0,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   275,   294,   295,   296,
     276,     0,   297,   298,   299,     0,     0,     0,   956,     0,
       0,     0,   272,     0,     0,     0,     0,     0,   273,     0,
       0,     0,     0,     0,     0,     0,   277,     0,   274,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   278,     0,   279,   280,   281,     0,   282,   283,   284,
       0,   285,   286,   287,   288,   289,   290,   291,   292,   293,
       0,   294,   295,   296,     0,     0,   297,   298,   299,   275,
       0,     0,     0,   276,     0,     0,   272,     0,     0,     0,
       0,   957,   273,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   274,     0,     0,     0,     0,     0,     0,   277,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   278,     0,   279,   280,   281,     0,
     282,   283,   284,     0,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   275,   294,   295,   296,   276,     0,   297,
     298,   299,     0,     0,     0,   958,     0,     0,     0,   272,
       0,     0,     0,     0,     0,   273,     0,     0,     0,     0,
       0,     0,     0,   277,     0,   274,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   278,     0,
     279,   280,   281,     0,   282,   283,   284,     0,   285,   286,
     287,   288,   289,   290,   291,   292,   293,     0,   294,   295,
     296,     0,     0,   297,   298,   299,   275,     0,     0,     0,
     276,     0,     0,   272,     0,     0,     0,     0,     0,   273,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   274,
       0,     0,     0,     0,     0,     0,   277,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   278,     0,   279,   280,   281,     0,   282,   283,   284,
       0,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     647,   294,   295,   296,   276,   274,   297,   298,   299,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,     0,     0,     0,   274,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   278,   275,   279,   280,   281,
     276,   282,   283,   284,     0,   285,   286,   287,   288,   289,
     290,   291,   292,   293,     0,   294,   295,   296,     0,   274,
     297,   298,   299,     0,     0,   275,   277,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   278,     0,   279,   280,   281,     0,   282,   283,   284,
       0,   285,   286,   287,   288,   277,   290,   291,   292,   293,
     275,   294,   295,   296,   276,     0,   297,   298,   299,     0,
     278,     0,   279,   280,   281,     0,   282,   283,   284,     0,
     285,   286,   287,   288,     0,   290,   291,   292,   293,     0,
     294,     0,   296,     0,     0,   297,   298,   299,     0,     0,
       0,     0,     0,     0,     0,   278,     0,   279,   280,   281,
       0,   282,   283,   284,     0,   285,   286,   287,   288,     0,
     290,   291,   292,   293,     0,   294,     0,   296,     0,     0,
     297,   298,   299
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-883)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       2,     2,   320,    46,    47,    16,   375,   406,   378,    11,
     160,   618,    14,   703,    49,    17,     1,     1,    20,    21,
      22,   171,   764,    25,    26,    27,    28,    29,    30,    64,
     383,   562,     3,   254,    36,    37,    38,   534,    40,   886,
      42,    43,   266,   267,   268,   269,    81,    49,   184,    51,
      52,    53,    54,   406,     3,    57,    58,    59,    60,   729,
       3,    63,    64,    65,    66,   201,    27,    69,     3,    54,
      72,    73,    74,    27,    76,     3,    27,    79,   768,    81,
       1,    83,    38,     1,   220,   117,    54,    27,   970,     1,
      27,    38,   925,    70,    38,   136,   117,    68,   117,    38,
     117,   803,    93,     3,   117,    90,   988,   117,    38,    84,
     975,    38,    98,   148,    98,   646,   148,   807,    38,   984,
       3,    68,    90,    93,    68,   146,    38,   146,    28,   146,
     977,   144,   116,   117,   144,   121,   138,   970,   356,    22,
     142,   143,   144,   145,   146,   147,   148,   644,   159,    70,
      85,    38,    70,    93,    93,    38,   117,   148,    41,   144,
      81,   117,   146,    70,   906,   907,   143,   837,   870,   700,
      91,    18,    55,    91,   145,    58,   144,    60,   117,   869,
     316,   317,   143,   185,    84,    68,    98,   117,   858,   191,
     117,   561,   143,   411,   143,   116,   145,   117,   116,     0,
     202,    84,   145,   143,   116,   117,   143,   209,   210,   148,
     145,    26,   117,   437,    22,   136,   534,   145,   148,    66,
     117,   148,   143,   117,   136,   143,   117,   117,   148,   136,
     117,     3,   117,    42,   924,   117,   143,   844,   117,    86,
     117,   146,   136,   145,   246,   246,   136,   249,     7,   146,
      58,   136,    67,   145,    63,   146,    71,   136,   136,   136,
      68,   148,   145,   399,   146,   145,   144,    22,    64,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,    89,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   795,    18,
     145,   116,   147,    58,   119,   145,   117,   117,   123,    98,
     345,   532,   533,    68,    80,    98,   318,   319,   320,    70,
     117,   275,   276,   692,   694,   327,   644,   329,   117,    84,
      81,   333,   121,   335,   117,   146,   146,    67,   121,   136,
      91,    71,    38,   345,    77,   347,   348,    66,   350,   351,
     117,   136,   117,   355,   356,    22,   358,   146,    67,   144,
     729,   117,    71,   146,   318,   319,   145,    86,   147,   136,
     372,   136,    59,    60,    70,   329,   597,   598,    70,   136,
     136,   383,    70,   385,   145,   136,   147,   136,    26,    81,
     392,    58,   143,    81,   348,    91,    36,   351,   433,    91,
     166,    68,    98,    91,   406,     1,   142,     3,   410,   411,
     136,   420,    52,   422,   813,   417,     2,    84,   184,    59,
      60,   117,   424,   425,   426,    11,   428,   429,   136,    67,
     136,   433,   136,    71,    20,   201,    22,   145,   588,   147,
     136,   136,   116,   136,   136,   119,   145,   143,   136,   123,
     146,   143,   136,   806,   220,   143,   119,   143,   136,    92,
     813,   136,   145,   145,   145,   136,    98,     7,    48,   116,
     424,    57,   136,    59,    60,   143,   845,   795,   116,    22,
     118,   119,    98,   117,   146,   123,   136,    38,   145,   858,
      29,   146,   130,    79,   117,    38,    98,    83,    41,   137,
     138,   139,   513,   514,   506,   507,   508,   509,   510,   511,
     512,   146,    55,    17,   117,    58,   146,    60,   158,   146,
     148,   143,   524,   136,   526,    68,   136,   136,   530,   136,
     136,   136,   534,   148,   146,   570,   176,   177,   178,   136,
      29,    84,   544,   183,   136,   136,   136,   136,   136,   902,
     316,   317,   138,   507,   194,   509,   117,   197,   136,   136,
     136,   563,   136,   565,   566,   567,   568,   569,   570,   136,
     136,    18,   925,   136,    21,   146,   216,   146,   136,   144,
     136,   934,   117,    30,   143,    32,    33,   144,    98,    36,
      68,    27,    27,    40,   148,    42,   236,   143,    29,   185,
     602,   116,    68,   148,   116,   191,   148,   148,    38,    56,
     144,   119,   119,    60,   145,    62,    63,   970,    65,    66,
      29,   146,   975,    91,   146,   144,   392,    81,   148,   727,
     411,   984,   410,   399,    19,   988,   714,    84,   781,    86,
      70,   228,   644,    26,   172,   647,   648,   649,   523,   732,
     652,   417,   844,   692,   420,   845,   422,   977,   554,   425,
     246,    91,   735,   924,   949,   667,    29,   669,    98,   386,
     672,   521,    25,   675,    -1,   710,    -1,    -1,   680,    -1,
     682,    -1,   684,   685,    67,   687,    -1,   117,    71,   136,
     330,    -1,    -1,   647,   648,    -1,    -1,   144,    -1,    -1,
      -1,    -1,     2,    -1,    -1,    -1,   136,   709,   710,    -1,
      -1,    11,    -1,   143,   354,    -1,   146,    -1,    -1,   721,
      20,   723,    22,    -1,   726,   727,    -1,    21,    -1,   731,
      -1,    -1,    -1,   116,    -1,   118,   119,    -1,    32,    33,
     123,   327,    -1,    -1,    -1,    -1,    40,   130,    42,    -1,
      -1,    -1,    -1,   519,    -1,   521,   139,    57,   760,    -1,
      -1,   347,    56,    -1,   350,    -1,    -1,    -1,    -1,    63,
     772,   773,   358,   775,   776,   777,   778,    -1,    -1,    79,
      -1,   783,   784,    83,   786,    -1,   788,   789,    -1,    -1,
     792,   834,   835,   795,    -1,    -1,    -1,    -1,    -1,    -1,
     802,    -1,    -1,    -1,   806,    -1,    -1,    -1,    -1,    -1,
     812,   813,   814,    -1,    -1,    -1,   818,    -1,   820,    -1,
      -1,   823,   824,    -1,    -1,   827,     2,    -1,    -1,   831,
     831,   833,    -1,    -1,    -1,    11,    -1,    -1,   138,    -1,
     426,    -1,    -1,   878,    20,    -1,    22,   849,   850,   851,
     852,   853,   854,   855,   856,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,   866,   867,    -1,    -1,    -1,   871,
      -1,   873,   874,   875,   876,   877,   878,    -1,    -1,    38,
     882,    57,    41,   885,    -1,   185,    26,    -1,   528,   529,
      -1,   191,   532,   533,    -1,    -1,    55,    -1,    -1,    58,
     902,    60,    -1,    79,    -1,   545,    -1,    83,    -1,    68,
     945,    -1,    -1,    -1,    -1,   917,   918,   919,   920,    -1,
      -1,    -1,    -1,   925,    -1,    84,    -1,    67,    -1,    -1,
      52,    71,   934,    -1,   936,   937,   938,    -1,   940,   941,
     942,   943,   944,   945,   584,    -1,   246,    -1,    -1,    -1,
      -1,   953,   954,   955,   956,   957,   958,    -1,   544,    38,
      -1,    -1,   138,    -1,    -1,    -1,    -1,   969,   970,    -1,
      -1,    -1,   112,   975,   114,   115,   116,    -1,   118,   119,
     120,    -1,   984,   123,   124,   125,   988,   127,   128,   129,
     130,    70,    -1,    -1,   134,    -1,    -1,   137,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
      -1,    -1,    91,    -1,    -1,   191,   602,   657,   658,    98,
      -1,    -1,    -1,    -1,   664,    -1,   666,   327,    -1,    -1,
      -1,   671,    -1,    -1,   674,    -1,    -1,   677,   117,    -1,
     806,    -1,    -1,    -1,    -1,    -1,    -1,   347,    -1,    -1,
     350,    -1,    -1,    -1,   176,   177,   178,   136,   358,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,   146,    -1,    -1,
     246,    -1,   194,    -1,    -1,   197,    26,   717,    -1,    -1,
      -1,   667,    -1,   669,    -1,    -1,   672,    -1,    -1,   675,
      -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,   684,    -1,
       2,   687,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,
      -1,    -1,    -1,    -1,   236,    -1,   882,    67,    20,    -1,
      22,    71,    -1,   709,    -1,    -1,   426,    -1,    -1,    -1,
       2,    -1,    -1,    -1,    -1,   721,    -1,   723,    -1,    11,
      -1,   727,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      22,   327,    -1,    -1,    -1,    57,    -1,    -1,    -1,   925,
      -1,    -1,   112,    -1,   114,   115,   116,    -1,   118,   119,
     120,   347,    -1,   123,   350,    -1,   816,    79,    -1,   129,
     130,    83,   358,    -1,   134,    57,    -1,   137,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   783,    -1,    -1,
      -1,    -1,   788,   503,   970,    -1,    -1,    79,   330,    50,
      -1,    83,    -1,    -1,    -1,    -1,   802,    26,    -1,    -1,
     860,   861,   988,   863,   864,    -1,    -1,    -1,   814,    -1,
      -1,    -1,   354,    -1,    -1,    -1,   138,   823,    -1,    80,
      -1,   827,    -1,    -1,   544,   831,   886,    -1,    -1,    -1,
     426,    -1,   892,    -1,   894,    -1,   896,    -1,    67,    -1,
      -1,    -1,    71,    -1,    -1,    -1,   138,    26,     2,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,
      -1,   867,    -1,   185,    -1,    -1,    20,    -1,    22,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   602,   112,    -1,   114,   115,   116,    67,   118,
     119,    -1,    71,   185,   123,    -1,    -1,    -1,   618,   191,
     129,   130,    -1,    57,    -1,   134,    -1,    -1,   137,   138,
     139,   917,   918,   919,   920,    -1,    -1,   977,    -1,    -1,
      -1,    -1,    -1,    -1,   246,    79,    -1,   647,   648,    83,
     936,   937,    -1,   112,    -1,   114,   115,   116,    -1,   118,
     119,    -1,    -1,    -1,   123,    -1,    -1,   667,   544,   669,
      -1,   130,   672,    -1,   246,   675,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   684,    -1,    -1,   687,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,    -1,   138,    -1,   528,   529,    -1,   709,
     532,   533,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   721,    -1,   723,    -1,   327,   602,   727,    -1,    -1,
     140,   141,   142,    -1,    -1,   145,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   347,    -1,    -1,   350,    -1,
      -1,   185,    -1,    -1,    -1,   327,   358,   191,    -1,   300,
     301,   302,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   312,   313,   314,   315,   347,    -1,    -1,   350,    -1,
      -1,    -1,    -1,   783,    -1,    26,   358,    -1,   788,    -1,
      -1,   667,    -1,   669,    -1,    -1,   672,    -1,    -1,   675,
      -1,    -1,   802,    -1,    -1,    -1,    -1,    -1,   684,    -1,
      -1,   687,   246,    -1,   814,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    -1,   823,   426,    -1,    67,   827,     2,    -1,
      71,   831,    -1,   709,    -1,   657,   658,    11,    -1,    -1,
      -1,    -1,   664,    -1,     2,   721,    20,   723,    22,    21,
      -1,   727,   674,    11,   426,   677,    -1,    -1,    -1,    67,
      32,    33,    20,    71,    22,    -1,    -1,   867,    40,    -1,
      42,   112,    -1,    -1,   115,   116,    48,   118,   119,   420,
      -1,   422,   123,    57,    56,    -1,    -1,    -1,    -1,   130,
      -1,    63,    64,   327,    -1,    -1,   137,   138,   139,    57,
      -1,    -1,    -1,    -1,   112,    79,    -1,   783,   116,    83,
     118,   119,   788,   347,    -1,   123,   350,   917,   918,   919,
     920,    79,   130,    -1,   358,    83,   802,    -1,    -1,   137,
     138,   139,    -1,    21,    -1,    -1,   936,   937,   814,    -1,
      -1,    -1,   544,    -1,    32,    33,    -1,   823,    -1,    -1,
      -1,   827,    40,    -1,    42,   831,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,   138,    -1,    -1,    -1,    56,    -1,
      -1,    18,   544,    -1,    21,    63,    64,    -1,   519,    -1,
     138,    -1,    -1,    30,    -1,    32,    33,    -1,    -1,    36,
      -1,   867,   426,    40,   816,    42,    -1,    -1,    -1,    -1,
     602,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,   185,    -1,    60,    -1,    62,    63,   191,    65,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,
     602,    -1,   573,   191,    -1,    -1,    -1,    84,    -1,    86,
      -1,   917,   918,   919,   920,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     936,   937,    -1,    -1,    -1,   667,    -1,   669,    -1,    -1,
     672,    -1,   246,   675,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   684,    -1,     2,   687,    -1,    -1,   246,   136,
      -1,    -1,    -1,    11,    -1,   667,    -1,   669,    -1,    -1,
     672,    -1,    20,   675,    22,    -1,    -1,   709,    -1,    -1,
     544,    -1,   684,    -1,    -1,   687,    -1,    -1,    -1,   721,
      -1,   723,    -1,    -1,    -1,   727,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   709,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   721,
      -1,   723,    -1,   327,    -1,   727,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    83,    -1,    -1,   602,   327,
      -1,    -1,    -1,   347,    -1,    -1,   350,    -1,    -1,    -1,
      -1,   783,    -1,    -1,   358,    -1,   788,    -1,    -1,   347,
      -1,    -1,   350,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     802,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   783,   814,    -1,    -1,    -1,   788,    -1,    -1,    -1,
     138,   823,    -1,    -1,    -1,   827,    -1,    -1,    -1,   831,
     802,    -1,    -1,   667,    -1,   669,    -1,    -1,   672,    -1,
      -1,   675,   814,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     684,   823,   426,   687,    -1,   827,    -1,    -1,    -1,   831,
      -1,    -1,    -1,    -1,    -1,   867,    -1,   185,   426,    -1,
      -1,    -1,    -1,   191,    -1,   709,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    -1,   721,    21,   723,
      -1,    -1,    -1,   727,    -1,   867,    -1,    30,    -1,    32,
      33,    -1,    -1,    36,    -1,    -1,    -1,    40,    -1,    42,
      -1,    -1,    -1,    -1,    -1,   917,   918,   919,   920,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    60,   246,    62,
      63,    -1,    65,    66,   936,   937,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   917,   918,   919,   920,   783,
      -1,    84,    -1,    86,   788,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   936,   937,    -1,    -1,   802,    -1,
     544,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     814,    -1,    -1,    -1,     1,    -1,   544,    -1,    -1,   823,
      -1,    -1,    -1,   827,    -1,    -1,    -1,   831,    -1,    -1,
      -1,    18,    -1,   136,    21,    22,    -1,    -1,    -1,   327,
      -1,   144,    -1,    30,    -1,    32,    33,    -1,    -1,    36,
      -1,    -1,    -1,    40,    -1,    42,    -1,    -1,   602,   347,
      -1,    -1,   350,   867,    -1,    -1,    -1,    -1,    -1,    56,
     358,    58,    -1,    60,   602,    62,    63,    -1,    65,    66,
      -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    86,
      -1,    -1,    89,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,   917,   918,   919,   920,    -1,    -1,    -1,
      -1,    26,    -1,   667,    -1,   669,    -1,    -1,   672,    -1,
      -1,   675,   936,   937,    -1,    -1,    -1,    -1,   426,   667,
     684,   669,    -1,   687,   672,    -1,    -1,   675,    -1,   136,
      -1,    -1,    -1,    -1,    13,    -1,   684,   144,    -1,   687,
      19,    -1,    67,    -1,    -1,   709,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,   721,    -1,   723,
      -1,   709,    -1,   727,    -1,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    97,   721,    -1,   723,    -1,    -1,    -1,   727,
      -1,    -1,    61,    -1,    -1,    -1,    -1,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    -1,   132,   133,   134,
      -1,    -1,   137,   138,   139,    -1,    -1,    -1,    -1,   783,
      -1,    -1,    -1,   148,   788,   104,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   544,    -1,   802,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     814,    -1,    -1,    -1,   802,    -1,    -1,    -1,    -1,   823,
      -1,    -1,    -1,   827,    -1,    -1,   814,   831,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   823,    -1,    -1,    -1,   827,
      -1,   160,    -1,   831,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,   171,    -1,   602,   174,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   867,    -1,   184,    -1,    -1,   187,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   867,
      -1,    -1,   201,    -1,    -1,    -1,   205,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   917,   918,   919,   920,    -1,    -1,   667,
      -1,   669,    -1,    -1,   672,    -1,    -1,   675,    -1,    -1,
      -1,    -1,   936,   937,    -1,    -1,   684,    -1,    -1,   687,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   266,   267,   268,
     269,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   709,    -1,    -1,   283,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   721,    -1,   723,    -1,    -1,    -1,   727,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   316,   317,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,   343,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   783,    -1,    -1,    -1,    28,
     788,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,   802,    44,    45,    -1,    47,   378,
      49,    50,    -1,    -1,    -1,    -1,   814,   386,    57,    -1,
      59,    -1,    61,    -1,    -1,   823,    -1,    -1,    -1,   827,
     399,    -1,    -1,   831,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,   432,    -1,    -1,   435,    -1,   437,   867,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,   131,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   917,
     918,   919,   920,    -1,    -1,    -1,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,   936,   937,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   521,    28,    -1,    -1,    -1,    -1,    -1,    34,
      35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    47,    -1,    49,    50,    -1,    -1,     3,    -1,
      -1,    -1,    57,    -1,    59,    10,    61,    -1,   557,    -1,
      -1,    16,   561,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,   584,    -1,    -1,    -1,   588,
      -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,
     115,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,   618,
      -1,    -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,    -1,
     145,    -1,   147,    98,    99,   100,   101,   102,   103,    -1,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,    -1,    -1,   118,    -1,    -1,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,    -1,   134,
      -1,    -1,   137,   138,   139,   140,    -1,   142,    -1,    -1,
     145,    -1,   691,    -1,    -1,   694,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   714,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     0,     1,    -1,     3,     4,     5,     6,
       7,     8,    -1,   732,    -1,    12,    13,    14,    15,    -1,
     739,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    -1,    30,    -1,    32,    33,    34,    35,    36,
      37,    -1,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    70,    -1,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    -1,    82,    83,    84,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,   120,    -1,   844,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,   136,
      -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,     1,
     147,     3,     4,     5,     6,     7,     8,    -1,    10,    -1,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    34,    35,    36,    37,    -1,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    -1,
      82,    83,    84,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,   116,   117,   118,   119,   120,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,   137,   138,   139,    -1,    -1,
      -1,   143,   144,   145,     1,   147,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    -1,    30,    -1,    32,    33,    34,    35,    36,
      37,    -1,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    70,    -1,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    -1,    82,    83,    84,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,   131,    -1,   133,   134,   135,   136,
      -1,    -1,   139,    -1,    -1,    -1,   143,   144,   145,     1,
     147,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    -1,    30,    -1,
      32,    33,    34,    35,    36,    37,    -1,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      52,    -1,    -1,    -1,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    -1,
      82,    83,    84,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,
      -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,    -1,
      -1,   143,   144,   145,     1,   147,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    -1,    30,    -1,    32,    33,    34,    35,    36,
      37,    -1,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    70,    -1,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    -1,    82,    83,    84,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,   136,
      -1,    -1,   139,    -1,    -1,    -1,   143,   144,   145,     1,
     147,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    -1,    30,    -1,
      32,    33,    34,    35,    36,    37,    -1,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      52,    -1,    -1,    -1,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    -1,
      82,    83,    84,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,
      -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,    -1,
      -1,   143,   144,   145,     1,   147,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    -1,    30,    -1,    32,    33,    34,    35,    36,
      37,    -1,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    70,    -1,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    -1,    82,    83,    84,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,   136,
      -1,    -1,   139,    -1,    -1,    -1,   143,   144,   145,     1,
     147,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    -1,    30,    -1,
      32,    33,    34,    35,    36,    37,    -1,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      52,    -1,    -1,    -1,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    -1,
      82,    83,    84,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,
      -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,    -1,
      -1,   143,    -1,   145,     1,   147,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    -1,    30,    -1,    32,    33,    34,    35,    36,
      37,    -1,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    70,    -1,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    -1,    82,    83,    84,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,   136,
      -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,     1,
     147,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    -1,    30,    -1,
      32,    33,    34,    35,    36,    37,    -1,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    -1,
      82,    83,    84,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,
      -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,    -1,
      -1,   143,    -1,   145,     1,   147,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    -1,    30,    -1,    32,    33,    34,    35,    36,
      37,    -1,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    70,    -1,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    -1,    82,    83,    84,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,   136,
      -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,     1,
     147,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    -1,    30,    -1,
      32,    33,    34,    35,    36,    37,    -1,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    -1,
      82,    83,    84,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,
      -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,    -1,
      -1,   143,    -1,   145,     1,   147,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    -1,    30,    -1,    32,    33,    34,    35,    36,
      37,    -1,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    70,    -1,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    -1,    82,    83,    84,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,   136,
      -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,     1,
     147,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    -1,    30,    -1,
      32,    33,    34,    35,    36,    37,    -1,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    -1,
      82,    83,    84,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,
      -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,    -1,
      -1,   143,    -1,   145,     1,   147,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    -1,    30,    -1,    32,    33,    34,    35,    36,
      37,    -1,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    70,    -1,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    -1,    82,    83,    84,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,   136,
      -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,     1,
     147,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    -1,    30,    -1,
      32,    33,    34,    35,    36,    37,    -1,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    -1,
      82,    83,    84,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,
      -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,    -1,
      -1,   143,    -1,   145,     1,   147,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    -1,    30,    -1,    32,    33,    34,    35,    36,
      37,    -1,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    70,    -1,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    -1,    82,    83,    84,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,   136,
      -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,     1,
     147,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    -1,    30,    -1,
      32,    33,    34,    35,    36,    37,    -1,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    -1,
      82,    83,    84,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,
      -1,     3,     4,     5,     6,     7,     8,   129,    -1,   131,
      12,   133,   134,    15,   136,    -1,    -1,   139,    -1,    -1,
      -1,   143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,
      -1,    -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,   143,    -1,   145,    -1,   147,   148,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    34,    35,
      -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    47,    -1,    49,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    -1,    59,    -1,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    75,
      76,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,
      -1,   147,   148,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,    49,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    59,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
      -1,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,   143,    -1,   145,    -1,   147,   148,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    12,    -1,
      -1,    15,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      34,    35,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,
      44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,    63,
      -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,     3,
       4,     5,     6,     7,     8,   129,    -1,   131,    12,   133,
     134,    15,    -1,    -1,    18,   139,    -1,    -1,    -1,   143,
      -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,    -1,
      34,    35,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,
      44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,    63,
      -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,     3,
       4,     5,     6,     7,     8,   129,    -1,   131,    12,   133,
     134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,
      -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,    -1,
      34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    82,    83,
      -1,    85,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,
      -1,   145,    -1,   147,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    82,    83,    -1,    85,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,   131,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,   146,   147,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    82,    83,
      -1,    85,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,
      -1,   145,   146,   147,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    38,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    82,    83,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,   136,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    82,    83,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,   131,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    47,    -1,    49,    50,    51,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    82,    83,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,     3,
       4,     5,     6,     7,     8,   129,    -1,   131,    12,   133,
     134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,
      -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,    -1,
      34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    58,    59,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,     3,
       4,     5,     6,     7,     8,   129,    -1,   131,    12,   133,
     134,    15,    -1,    -1,    -1,   139,    -1,    21,    -1,   143,
      -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,    33,
      34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,     3,
       4,     5,     6,     7,     8,   129,    -1,   131,    12,   133,
     134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,
      -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,    -1,
      34,    35,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,
      44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,     3,
       4,     5,     6,     7,     8,   129,    -1,   131,    12,   133,
     134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,
      -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,    -1,
      34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    82,    83,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,     3,
       4,     5,     6,     7,     8,   129,    -1,   131,    12,   133,
     134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,
      -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,    -1,
      34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,
     144,   145,    -1,   147,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,   131,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,     3,
       4,     5,     6,     7,     8,   129,    -1,   131,    12,   133,
     134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,
      -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,    -1,
      34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,
      -1,   145,    -1,   147,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,   131,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,
      -1,   145,    -1,   147,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,     3,     4,     5,     6,     7,     8,
     129,    -1,   131,    12,   133,   134,    15,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,   145,    -1,   147,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
     129,    39,   131,    -1,    -1,   134,    44,    45,    -1,    47,
     139,    49,    50,    -1,   143,    -1,   145,    -1,   147,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,   117,
      -1,    -1,    -1,   121,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,   131,    -1,    12,    -1,   135,    15,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,   145,   146,   147,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,
      37,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      47,    -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    74,    75,    76,
      77,    78,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    91,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    -1,   131,    -1,    -1,   140,   141,   136,
      -1,    -1,   145,    -1,   147,    28,   143,    -1,   145,    -1,
     147,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,     3,     4,     5,
       6,     7,     8,    -1,    87,    -1,    12,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    34,    35,
     113,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    47,    -1,    49,    50,    -1,    -1,    -1,   131,    -1,
      -1,    57,     3,    59,    -1,    61,    -1,     1,    -1,    10,
     143,    -1,   145,    -1,   147,    16,    -1,    -1,    74,    75,
      76,    77,    78,    -1,    18,    -1,    -1,    21,    22,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    30,    -1,    32,    33,
      -1,    -1,    36,    -1,    -1,    -1,    40,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    56,    -1,    58,    -1,    60,    -1,    62,    63,
      -1,    65,    66,    -1,    68,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,   145,
      84,   147,    86,    16,    -1,    89,    -1,    98,    99,   100,
     101,   102,   103,    26,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,    -1,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,    -1,   134,    -1,    -1,   137,   138,   139,   140,
      -1,   142,   136,    -1,    67,    -1,    -1,    -1,    71,    -1,
     144,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,   114,   115,   116,    -1,   118,   119,   120,    -1,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    67,   132,
     133,   134,    71,    -1,   137,   138,   139,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,   148,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,   114,   115,   116,    -1,   118,
     119,   120,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    -1,   132,   133,   134,    -1,    67,   137,   138,
     139,    71,    -1,    -1,    10,    -1,    -1,    -1,    -1,   148,
      16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    -1,   114,   115,   116,    -1,   118,   119,
     120,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,    67,   132,   133,   134,    71,    -1,   137,   138,   139,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,   148,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,   115,
     116,    -1,   118,   119,   120,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,   132,   133,   134,    -1,
      67,   137,   138,   139,    71,    -1,    -1,    10,    -1,    -1,
     146,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,
      -1,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    67,   132,   133,   134,    71,    -1,
     137,   138,   139,    -1,    -1,    -1,    79,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,    26,    -1,   106,   107,   108,   109,   110,   111,   112,
      -1,   114,   115,   116,    -1,   118,   119,   120,    -1,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    -1,   132,
     133,   134,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,
     143,    -1,    67,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,    26,
      -1,   106,   107,   108,   109,   110,   111,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    -1,   132,   133,   134,
      -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,
      67,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,    26,    -1,   106,
     107,   108,   109,   110,   111,   112,    -1,   114,   115,   116,
      -1,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,   132,   133,   134,    -1,    -1,
     137,   138,   139,    -1,    -1,    -1,   143,    -1,    67,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    26,    -1,   106,   107,   108,
     109,   110,   111,   112,    -1,   114,   115,   116,    -1,   118,
     119,   120,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    -1,   132,   133,   134,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,    -1,    67,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    16,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    26,    -1,   106,   107,   108,   109,   110,
     111,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    -1,    -1,   137,   138,   139,    -1,
      -1,    -1,   143,    -1,    67,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,    26,    -1,   106,   107,   108,   109,   110,   111,   112,
      -1,   114,   115,   116,    -1,   118,   119,   120,    -1,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    -1,   132,
     133,   134,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,
     143,    -1,    67,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,    26,
      -1,   106,   107,   108,   109,   110,   111,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    -1,   132,   133,   134,
      -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,
      67,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,    26,    -1,   106,
     107,   108,   109,   110,   111,   112,    -1,   114,   115,   116,
      -1,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,   132,   133,   134,    -1,    -1,
     137,   138,   139,    -1,    -1,    -1,   143,    -1,    67,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,   106,   107,   108,
     109,   110,   111,   112,    -1,   114,   115,   116,    -1,   118,
     119,   120,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    -1,   132,   133,   134,    10,    -1,   137,   138,
     139,    -1,    16,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    71,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      10,    -1,    -1,    97,    -1,    -1,    16,    67,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    26,    27,   112,    -1,
     114,   115,   116,    -1,   118,   119,   120,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    97,   132,   133,
     134,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
      -1,    -1,   112,    -1,   114,   115,   116,    67,   118,   119,
     120,    71,   122,   123,   124,   125,   126,   127,   128,   129,
     130,    -1,   132,   133,   134,    10,    -1,   137,   138,   139,
      -1,    16,    -1,   143,    -1,    -1,    -1,    97,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    -1,   114,   115,   116,    -1,   118,   119,
     120,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,    -1,   132,   133,   134,    10,    -1,   137,   138,   139,
      -1,    16,    67,   143,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    10,
      -1,    -1,    97,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   112,    -1,   114,
     115,   116,    67,   118,   119,   120,    71,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    -1,   132,   133,   134,
      -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,    10,
      -1,    -1,    97,    -1,    -1,    16,    67,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    26,    -1,   112,    79,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    97,   132,   133,   134,
      -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,
      -1,   112,    -1,   114,   115,   116,    67,   118,   119,   120,
      71,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    10,    -1,   137,   138,   139,    -1,
      16,    -1,   143,    -1,    -1,    -1,    97,    -1,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    10,    -1,   137,   138,   139,    -1,
      16,    67,   143,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,   115,
     116,    67,   118,   119,   120,    71,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,   132,   133,   134,    10,
      -1,   137,   138,   139,    -1,    16,   142,    -1,    -1,    -1,
      -1,    97,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    38,   114,   115,
     116,    -1,   118,   119,   120,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,   132,   133,   134,    10,
     136,   137,   138,   139,    -1,    16,    67,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    67,   118,   119,   120,
      71,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    10,    -1,   137,   138,   139,    -1,
      16,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    38,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    10,    -1,   137,   138,   139,    -1,
      16,    67,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    97,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,   112,    -1,   114,   115,
     116,    67,   118,   119,   120,    71,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,   132,   133,   134,    -1,
      -1,   137,   138,   139,    10,    -1,    -1,    -1,    -1,    -1,
      16,    97,    -1,    -1,    -1,    67,    -1,    -1,    -1,    71,
      26,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,   115,
     116,    -1,   118,   119,   120,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    97,   132,   133,   134,    -1,
      -1,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    67,   114,   115,   116,    71,   118,   119,   120,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    -1,
     132,   133,   134,    10,   136,   137,   138,   139,    -1,    16,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,   115,
     116,    -1,   118,   119,   120,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,   132,   133,   134,    10,
     136,   137,   138,   139,    -1,    16,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    71,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    67,   114,   115,   116,
      71,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,   132,   133,   134,    10,   136,
     137,   138,   139,    -1,    16,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    -1,   136,   137,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    71,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,   114,   115,   116,    -1,   118,   119,   120,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    67,
     132,   133,   134,    71,    -1,   137,   138,   139,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,   132,   133,   134,    -1,    -1,   137,
     138,   139,    67,    -1,    -1,    -1,    71,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    67,   132,   133,   134,
      71,    -1,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    11,    -1,    -1,    -1,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    -1,   136,   137,   138,   139,    67,
      -1,    -1,    -1,    71,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    29,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    67,   132,   133,   134,    71,    -1,   137,
     138,   139,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
     114,   115,   116,    -1,   118,   119,   120,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,   132,   133,
     134,    -1,    67,   137,   138,   139,    71,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    67,   132,   133,   134,
      71,    -1,   137,   138,   139,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    27,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    -1,    67,   137,   138,   139,    71,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,   114,   115,   116,    -1,   118,   119,   120,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    67,
     132,   133,   134,    71,    -1,   137,   138,   139,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,   132,   133,   134,    -1,    -1,   137,
     138,   139,    67,    -1,    -1,    -1,    71,    -1,    -1,    10,
      11,    -1,    -1,    -1,    79,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    67,   132,   133,   134,
      71,    -1,   137,   138,   139,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    -1,    67,   137,   138,   139,    71,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    79,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,   114,   115,   116,    -1,   118,   119,   120,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    67,
     132,   133,   134,    71,    -1,   137,   138,   139,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,   132,   133,   134,    -1,    -1,   137,
     138,   139,    67,    -1,    -1,    -1,    71,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    79,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    67,   132,   133,   134,
      71,    -1,   137,   138,   139,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    -1,    -1,   137,   138,   139,    67,
      -1,    -1,    -1,    71,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    79,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    67,   132,   133,   134,    71,    -1,   137,
     138,   139,    -1,    -1,    -1,    79,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
     114,   115,   116,    -1,   118,   119,   120,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,   132,   133,
     134,    -1,    -1,   137,   138,   139,    67,    -1,    -1,    -1,
      71,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      67,   132,   133,   134,    71,    26,   137,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    67,   114,   115,   116,
      71,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,   132,   133,   134,    -1,    26,
     137,   138,   139,    -1,    -1,    67,    97,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,    97,   127,   128,   129,   130,
      67,   132,   133,   134,    71,    -1,   137,   138,   139,    -1,
     112,    -1,   114,   115,   116,    -1,   118,   119,   120,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,    -1,
     132,    -1,   134,    -1,    -1,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,
      -1,   118,   119,   120,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,    -1,   132,    -1,   134,    -1,    -1,
     137,   138,   139
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   153,   154,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    15,    18,    19,    20,    21,    23,
      24,    25,    27,    28,    30,    32,    33,    34,    35,    36,
      37,    39,    40,    43,    44,    45,    46,    47,    49,    50,
      52,    56,    57,    59,    60,    61,    62,    65,    66,    69,
      70,    72,    73,    74,    75,    76,    77,    78,    80,    82,
      83,    84,    86,    87,    88,    92,    94,    97,   112,   113,
     114,   115,   120,   129,   131,   133,   134,   136,   139,   143,
     145,   147,   155,   156,   157,   158,   159,   161,   166,   167,
     168,   170,   173,   174,   175,   176,   177,   178,   180,   181,
     182,   186,   187,   190,   191,   194,   195,   198,   201,   202,
     225,   228,   229,   249,   250,   251,   252,   253,   254,   262,
     263,   264,   265,   268,   269,   270,   271,   272,   273,   275,
     276,   277,   278,   279,   136,   157,   265,    93,   255,   256,
     169,   170,    12,    34,    35,    37,    78,   143,   147,   198,
     249,   253,   263,   264,   265,   268,   270,   271,   255,   265,
      84,   169,   157,   243,   265,   157,   145,   265,   266,     7,
       9,    84,   171,   266,    58,    95,   179,   265,   265,    21,
      33,   228,   265,   265,   145,   170,   199,   145,   170,   230,
     231,    27,   161,   172,   265,   265,   265,   265,   265,   265,
       7,   145,    33,   175,   175,   135,   221,   242,   265,    82,
      83,   136,   265,   267,   265,   172,   265,   265,   265,    77,
     145,   157,   265,   265,   161,   168,   265,   268,   161,   168,
     265,   170,   226,   265,   242,   265,   265,   265,   265,   265,
     265,   265,   265,     1,   144,   155,   162,   242,    85,   121,
     221,   244,   245,   267,   242,   265,   274,    60,   157,    64,
     160,   170,   170,    42,    63,   216,    22,    58,    68,    89,
     136,   142,    10,    16,    26,    67,    71,    97,   112,   114,
     115,   116,   118,   119,   120,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   132,   133,   134,   137,   138,   139,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   140,   141,   145,   147,    67,    71,
     145,   157,   136,   265,   265,   265,   242,   143,   161,    38,
     255,   226,   136,   117,   136,    92,   170,   221,   246,   247,
     248,   267,   226,   191,   170,   145,   172,    27,    38,   172,
      27,    38,    93,   172,   258,    31,    53,   136,    79,    98,
      99,   100,   101,   102,   103,   106,   107,   108,   109,   110,
     111,   161,   208,   246,   157,   145,   210,    85,   145,   170,
     232,   233,     1,   116,   237,    38,   117,   157,   172,   172,
     246,   170,   117,   136,   265,   265,   136,   143,   172,   145,
     246,   136,   183,   136,   183,   136,    98,   227,   136,   136,
      31,    53,   172,   136,   144,   144,   155,   117,   144,   265,
     117,   146,   117,   146,    38,   117,   148,   258,    96,   117,
     148,     7,    48,   116,   192,   143,   203,    68,   230,   230,
     230,   230,   265,   265,   265,   265,   179,   265,   179,   265,
     265,   265,   265,   265,   265,   265,    28,    84,   170,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   246,   246,   179,   265,   179,   265,    22,    38,    68,
      89,   257,   260,   261,   265,   279,    27,    38,    27,    38,
      79,    38,   148,   179,   265,   172,   136,   265,   265,    98,
     146,   117,   136,   170,    38,   242,    37,   265,   179,   265,
      37,   265,   179,   265,   145,   172,   139,   163,   165,   265,
     163,   164,   157,   265,    29,   265,   146,    22,    38,    41,
      55,    58,    68,    84,   156,   211,   212,   213,   214,   200,
     233,   117,   146,    15,    28,    57,    74,    75,    78,    87,
     147,   238,   250,    98,   234,   265,   231,   146,   221,   265,
       1,   188,   246,   146,    17,   184,   238,   250,   117,   165,
     164,   144,   146,   146,   244,   146,   244,   179,   265,   148,
     157,   265,   148,   265,   148,   265,   170,   242,   143,     1,
     170,   196,   197,    22,    58,    68,    84,   205,   215,   230,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     146,   148,    38,    68,   117,   146,   249,    67,    67,    37,
     179,   265,    37,   179,   265,   179,   265,   255,   255,   136,
     221,   267,   248,   192,   265,   146,   265,    27,   172,    27,
     172,   265,    27,   172,   258,    27,   172,   258,   259,   260,
     117,   136,    11,   136,    29,    29,   157,    79,   161,    38,
      68,   213,   117,   146,   145,   170,    22,    58,    68,    84,
     217,   146,   233,   237,     1,   242,    51,   267,   144,    54,
      90,   144,   189,   146,   145,   161,   170,   185,   226,   136,
     136,   148,   258,   148,   258,   157,    96,   143,     1,   193,
     144,    98,   117,   144,    68,   204,    10,    16,   112,   113,
     114,   115,   118,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   134,   137,   138,   139,   140,   142,
     145,   170,   206,   207,   208,   249,   136,   260,   237,   249,
     249,   265,    27,    27,   265,    27,    27,   148,   148,   172,
     172,   143,   172,    79,    37,   265,    37,   265,    79,    37,
     265,   172,    37,   265,   172,   117,   146,   265,   265,   157,
     265,   157,    29,   170,   212,   233,   116,   241,    68,   237,
     234,   148,    38,   148,    27,   157,   242,   185,   116,   161,
      37,   157,   265,   148,    37,   157,   265,   148,   265,     1,
     144,   162,   144,    33,    62,    65,   144,   156,   174,   265,
     197,   217,   170,   265,   119,   145,   209,   209,   234,    79,
      37,    37,    79,    37,    37,    37,    37,     1,   193,   265,
     265,   265,   265,   265,   265,   260,    11,    29,   157,   241,
     146,    15,    28,    57,    74,    75,    78,    87,   147,   221,
     240,   250,   121,   223,   234,    91,   224,   265,   238,   250,
     157,   172,   146,   265,   265,   157,   265,   157,   144,   144,
     174,     1,   116,   236,   146,   170,   207,   208,   211,   265,
     265,   265,   265,   265,   265,   144,   144,    79,    79,    79,
      79,   265,   157,   234,   241,   148,   242,   221,   222,   265,
     265,   161,   173,   220,   148,   161,    79,    79,    15,    28,
      57,    74,    75,    78,    87,   147,   235,   250,    81,   218,
     209,   209,   146,    79,    79,    79,    79,    79,    79,   265,
     265,   265,   265,   223,   234,   221,   239,   240,   250,    38,
     148,   250,   265,   265,     1,   148,   242,   224,   265,   239,
     240,   148,   235,   250,   148,   136,   219,   220,   148,   235,
     250,   239
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
#line 447 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 452 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 453 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 460 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 465 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 466 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 485 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 486 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 487 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 488 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 489 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 490 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 491 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 492 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 493 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 494 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 495 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 496 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 497 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 498 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 499 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 500 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (6)].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[(1) - (6)].b), (yyvsp[(2) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 508 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType, (yyvsp[(6) - (7)].pblockstmt), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 510 "chapel.ypp"
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 515 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 516 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 517 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 521 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 522 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 535 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 542 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 543 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 548 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 553 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 558 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 562 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 569 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 578 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 588 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 589 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 593 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 598 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 600 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 602 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 610 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 616 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 618 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 620 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 622 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 624 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 626 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 628 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 632 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 637 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 646 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 647 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 651 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 652 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 656 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 658 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 659 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 672 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 673 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 674 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 675 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 680 "chapel.ypp"
    {
#ifdef HAVE_LLVM
      if (externC) {
        (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
      } else {
        USR_FATAL(new BlockStmt(), "extern block syntax is turned off. Use --extern-c flag to turn on.");
      }
#else
      USR_FATAL(new BlockStmt(), "Chapel must be built with llvm in order to use the extern block syntax");
#endif
    }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 694 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 698 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 702 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 703 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 704 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 705 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 706 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 707 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 708 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 709 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 711 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 717 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 723 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 729 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 735 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 742 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 758 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 759 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 761 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 763 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 765 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 770 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 773 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 774 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 775 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 776 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 777 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 778 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 782 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 783 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 787 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 788 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 789 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 793 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 794 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 798 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 802 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 804 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 808 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 809 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 814 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 816 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 818 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 825 "chapel.ypp"
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

  case 158:
/* Line 1792 of yacc.c  */
#line 835 "chapel.ypp"
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

  case 159:
/* Line 1792 of yacc.c  */
#line 845 "chapel.ypp"
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

  case 160:
/* Line 1792 of yacc.c  */
#line 855 "chapel.ypp"
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

  case 161:
/* Line 1792 of yacc.c  */
#line 867 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 872 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 877 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 885 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 886 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 891 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 893 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 895 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 900 "chapel.ypp"
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

  case 170:
/* Line 1792 of yacc.c  */
#line 916 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 923 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 932 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 940 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 944 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 950 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 951 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 956 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 961 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 966 "chapel.ypp"
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
      if ((yyvsp[(7) - (8)].pexpr))
        (yyvsp[(3) - (8)].pfnsymbol)->where = new BlockStmt((yyvsp[(7) - (8)].pexpr));
      (yyvsp[(3) - (8)].pfnsymbol)->insertAtTail((yyvsp[(8) - (8)].pblockstmt));
      (yyval.pexpr) = new DefExpr(buildLambda((yyvsp[(3) - (8)].pfnsymbol)));
    }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 986 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 992 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 999 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[(2) - (2)].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 1005 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[(2) - (2)].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 1011 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 1022 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 1028 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 1035 "chapel.ypp"
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

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[(4) - (10)].pfnsymbol), (yyvsp[(6) - (10)].retTag), (yyvsp[(7) - (10)].pexpr), (yyvsp[(8) - (10)].b), (yyvsp[(9) - (10)].pexpr), (yyvsp[(10) - (10)].pblockstmt), (yylsp[(1) - (10)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 1061 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 1065 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 1070 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 1074 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 1082 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1087 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1089 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1090 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1091 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1092 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1094 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1096 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1097 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1099 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1101 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1102 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1103 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1104 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1105 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1106 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1107 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1108 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1111 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1115 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1116 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1117 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1118 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1119 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1120 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1121 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1122 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1123 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1124 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1125 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1126 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1130 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1135 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1139 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1140 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1141 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1146 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1148 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1150 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1152 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1154 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1158 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1159 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1163 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1164 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1165 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1166 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1167 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1168 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1169 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1170 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1171 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1175 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1176 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1177 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1178 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1179 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1180 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1184 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1185 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1190 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1191 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1192 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1193 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1194 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1198 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1199 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1208 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1213 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1215 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1219 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1225 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1230 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1235 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1237 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1239 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1244 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1257 "chapel.ypp"
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

  case 287:
/* Line 1792 of yacc.c  */
#line 1273 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1277 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1282 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1290 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1298 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1306 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1324 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1325 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1326 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1332 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1340 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1342 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1347 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1349 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1351 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1356 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1358 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1366 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1367 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1374 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1376 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1378 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1382 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1386 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1388 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1392 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1399 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1400 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1401 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1402 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1403 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1404 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1405 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1406 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1407 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1408 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1409 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1414 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1415 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1416 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1417 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1418 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1419 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1420 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1421 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1422 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1423 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1424 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1445 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1449 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1453 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1461 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1467 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1469 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1474 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1476 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1486 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1490 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1492 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1493 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1494 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1496 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1497 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1498 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1499 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1500 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1518 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1534 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1541 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1542 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1546 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1563 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1565 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1569 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1571 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1573 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1575 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1577 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1579 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1583 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1588 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1592 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1594 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1598 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1600 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1602 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1604 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1606 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1610 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1612 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1619 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1625 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1631 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1637 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1646 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1671 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1680 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1684 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1685 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1693 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1694 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1699 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1703 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1707 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1713 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1715 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1724 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1729 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1734 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1750 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1752 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1754 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1756 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1758 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1760 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1764 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1765 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1768 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1769 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1770 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1792 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1793 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1794 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1798 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1799 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1800 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1808 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1809 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1810 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1811 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1815 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1816 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1817 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1818 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1819 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1820 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1821 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1822 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1823 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1825 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1829 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1836 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1837 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1841 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1842 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1843 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1844 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1845 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1846 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1847 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1848 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1849 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1850 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1851 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1852 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1853 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 1854 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1855 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1856 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1857 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1858 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1859 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1860 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1861 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1862 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1863 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1867 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1868 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1869 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 1870 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 1871 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 523:
/* Line 1792 of yacc.c  */
#line 1872 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 1876 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 1877 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 1878 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 1879 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 1883 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 1884 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 1885 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 1886 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 1891 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 1892 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 1893 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 1894 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 1895 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 1896 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 1897 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 8778 "bison-chapel.cpp"
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


