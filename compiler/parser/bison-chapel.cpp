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
#define YYLAST   13775

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  152
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  125
/* YYNRULES -- Number of rules.  */
#define YYNRULES  518
/* YYNRULES -- Number of states.  */
#define YYNSTATES  971

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
     277,   278,   280,   282,   284,   286,   289,   291,   294,   298,
     300,   302,   305,   308,   310,   312,   314,   316,   318,   320,
     324,   330,   336,   339,   342,   348,   352,   359,   366,   371,
     377,   383,   387,   391,   398,   404,   411,   417,   424,   428,
     433,   440,   448,   455,   463,   468,   474,   479,   484,   488,
     495,   501,   508,   514,   523,   531,   534,   538,   542,   545,
     548,   552,   556,   557,   560,   563,   567,   573,   575,   579,
     583,   589,   595,   596,   599,   603,   606,   610,   617,   626,
     633,   642,   644,   646,   648,   649,   652,   653,   656,   660,
     666,   672,   674,   676,   679,   683,   685,   689,   690,   691,
     700,   701,   703,   706,   709,   711,   712,   713,   724,   728,
     732,   738,   744,   746,   750,   752,   755,   757,   759,   761,
     763,   765,   767,   769,   771,   773,   775,   777,   779,   781,
     783,   785,   787,   789,   791,   793,   795,   797,   799,   801,
     803,   805,   807,   809,   811,   813,   815,   817,   819,   821,
     823,   825,   827,   828,   832,   836,   837,   839,   843,   848,
     854,   859,   866,   873,   874,   876,   878,   880,   882,   884,
     887,   890,   892,   894,   896,   897,   899,   901,   904,   906,
     908,   910,   912,   913,   915,   918,   920,   922,   924,   925,
     927,   929,   931,   933,   935,   938,   940,   941,   943,   946,
     949,   950,   953,   957,   962,   967,   970,   975,   976,   979,
     982,   987,   992,   997,  1003,  1008,  1009,  1011,  1013,  1015,
    1019,  1023,  1029,  1031,  1033,  1037,  1039,  1042,  1046,  1047,
    1050,  1053,  1054,  1057,  1060,  1063,  1066,  1069,  1072,  1075,
    1078,  1081,  1083,  1088,  1093,  1100,  1104,  1105,  1107,  1109,
    1113,  1118,  1122,  1127,  1134,  1135,  1138,  1141,  1144,  1147,
    1150,  1153,  1156,  1159,  1162,  1165,  1167,  1169,  1173,  1177,
    1179,  1183,  1185,  1187,  1189,  1193,  1197,  1198,  1200,  1202,
    1206,  1210,  1214,  1216,  1218,  1220,  1222,  1224,  1226,  1228,
    1230,  1233,  1238,  1243,  1248,  1254,  1257,  1260,  1263,  1266,
    1269,  1272,  1279,  1286,  1291,  1301,  1311,  1319,  1326,  1333,
    1338,  1348,  1358,  1366,  1371,  1378,  1385,  1395,  1405,  1412,
    1414,  1416,  1418,  1420,  1422,  1424,  1426,  1428,  1432,  1433,
    1435,  1440,  1442,  1446,  1451,  1453,  1457,  1462,  1466,  1470,
    1472,  1474,  1477,  1479,  1482,  1484,  1486,  1490,  1493,  1498,
    1500,  1502,  1504,  1506,  1508,  1510,  1512,  1514,  1519,  1523,
    1527,  1530,  1533,  1535,  1538,  1541,  1543,  1545,  1547,  1549,
    1551,  1553,  1555,  1560,  1565,  1570,  1574,  1578,  1582,  1586,
    1591,  1595,  1600,  1602,  1604,  1606,  1608,  1610,  1614,  1619,
    1623,  1628,  1632,  1637,  1641,  1647,  1651,  1655,  1659,  1663,
    1667,  1671,  1675,  1679,  1683,  1687,  1691,  1695,  1699,  1703,
    1707,  1711,  1715,  1719,  1723,  1727,  1731,  1735,  1739,  1742,
    1745,  1748,  1751,  1754,  1757,  1761,  1765,  1769,  1773,  1777,
    1781,  1785,  1789,  1791,  1793,  1795,  1797,  1799,  1801
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     153,     0,    -1,   154,    -1,    -1,   154,   155,    -1,   157,
      -1,   156,   157,    -1,    60,     7,    -1,   156,    60,     7,
      -1,   158,    -1,   161,    -1,   166,    -1,   167,    -1,   174,
      -1,   168,    -1,   177,    -1,   180,    -1,   178,    -1,   187,
      -1,   181,    -1,   182,    -1,   186,    -1,   173,    -1,   252,
     136,    -1,    12,   157,    -1,    13,   253,   157,    -1,    14,
     169,   136,    -1,    19,   253,   161,    -1,    23,   169,   136,
      -1,    25,   241,   136,    -1,    43,   170,   157,    -1,    46,
     263,   172,    -1,    46,   172,    -1,    52,   263,   172,    -1,
      73,   263,   172,    -1,    73,   172,    -1,    78,   157,    -1,
      94,   263,   136,    -1,     1,   136,    -1,   159,   160,    48,
     170,   143,   144,    -1,   159,   160,    48,   170,   143,   162,
     144,    -1,   159,   160,    48,   170,   143,     1,   144,    -1,
      -1,    65,    -1,    62,    -1,    -1,    64,    -1,   143,   144,
      -1,   143,   162,   144,    -1,   143,     1,   144,    -1,   155,
      -1,   162,   155,    -1,   263,    -1,   263,    11,   263,    -1,
     163,   117,   263,    -1,   163,   117,   263,    11,   263,    -1,
      -1,   163,    -1,   139,    -1,   163,    -1,    88,   240,   136,
      -1,    88,   263,    31,   165,   136,    -1,    88,   263,    53,
     164,   136,    -1,    69,   240,   136,    -1,   265,    98,   264,
     136,    -1,   265,   109,   264,   136,    -1,   265,   106,   264,
     136,    -1,   265,   108,   264,   136,    -1,   265,   102,   264,
     136,    -1,   265,   107,   264,   136,    -1,   265,   103,   264,
     136,    -1,   265,    99,   264,   136,    -1,   265,   100,   264,
     136,    -1,   265,   101,   264,   136,    -1,   265,   111,   264,
     136,    -1,   265,   110,   264,   136,    -1,   265,   140,   264,
     136,    -1,   265,   141,   264,   136,    -1,   265,   104,   264,
     136,    -1,   265,   105,   264,   136,    -1,    -1,   170,    -1,
       3,    -1,   169,    -1,     7,    -1,    27,   157,    -1,   161,
      -1,    70,   136,    -1,    70,   264,   136,    -1,   136,    -1,
     175,    -1,    65,   175,    -1,    62,   175,    -1,   194,    -1,
     225,    -1,   190,    -1,   176,    -1,   202,    -1,   228,    -1,
      36,   263,   136,    -1,    36,   263,    31,   165,   136,    -1,
      36,   263,    53,   164,   136,    -1,    36,   228,    -1,    33,
       9,    -1,    27,   157,    92,   263,   136,    -1,    92,   263,
     172,    -1,    20,   263,    38,   263,   253,   172,    -1,    20,
     263,    38,   179,   253,   172,    -1,    20,   263,   253,   172,
      -1,    34,   263,    38,   263,   172,    -1,    34,   263,    38,
     179,   172,    -1,    34,   263,   172,    -1,    34,   179,   172,
      -1,    34,    58,   170,    38,   263,   172,    -1,    35,   263,
      38,   263,   172,    -1,    35,   263,    38,   263,   256,   172,
      -1,    35,   263,    38,   179,   172,    -1,    35,   263,    38,
     179,   256,   172,    -1,    35,   263,   172,    -1,    35,   263,
     256,   172,    -1,   147,   240,    38,   263,   148,   157,    -1,
     147,   240,    38,   263,   256,   148,   157,    -1,   147,   240,
      38,   179,   148,   157,    -1,   147,   240,    38,   179,   256,
     148,   157,    -1,   147,   240,   148,   157,    -1,   147,   240,
     256,   148,   157,    -1,    95,   145,   240,   146,    -1,    37,
     263,    79,   157,    -1,    37,   263,   161,    -1,    37,   263,
      79,   157,    29,   157,    -1,    37,   263,   161,    29,   157,
      -1,    37,   263,   208,   263,    79,   157,    -1,    37,   263,
     208,   263,   161,    -1,    37,   263,   208,   263,    79,   157,
      29,   157,    -1,    37,   263,   208,   263,   161,    29,   157,
      -1,    24,   157,    -1,    82,   263,   136,    -1,    83,   263,
     136,    -1,    82,   168,    -1,    83,   168,    -1,    82,   161,
     183,    -1,    83,   161,   183,    -1,    -1,   183,   184,    -1,
      17,   161,    -1,    17,   185,   161,    -1,    17,   145,   185,
     146,   161,    -1,   170,    -1,   170,   116,   263,    -1,    80,
     263,   136,    -1,    72,   263,   143,   188,   144,    -1,    72,
     263,   143,     1,   144,    -1,    -1,   188,   189,    -1,    90,
     240,   172,    -1,    54,   157,    -1,    54,    27,   157,    -1,
     191,   170,   192,   143,   193,   144,    -1,    33,   171,   191,
     170,   192,   143,   193,   144,    -1,   191,   170,   192,   143,
       1,   144,    -1,    33,   171,   191,   170,   192,   143,     1,
     144,    -1,    18,    -1,    66,    -1,    86,    -1,    -1,   116,
     240,    -1,    -1,   193,   174,    -1,   193,   156,   174,    -1,
     195,   170,   143,   196,   144,    -1,   195,   170,   143,     1,
     144,    -1,    30,    -1,   197,    -1,   196,   117,    -1,   196,
     117,   197,    -1,   170,    -1,   170,    98,   263,    -1,    -1,
      -1,    44,   199,   210,   200,   217,   235,   224,   220,    -1,
      -1,    40,    -1,    32,   171,    -1,    33,   171,    -1,    56,
      -1,    -1,    -1,   201,   216,   203,   205,   204,   217,   235,
     218,   224,   219,    -1,   215,   207,   209,    -1,   215,   208,
     209,    -1,   215,   206,   119,   207,   209,    -1,   215,   206,
     119,   208,   209,    -1,   247,    -1,   145,   263,   146,    -1,
     170,    -1,   113,   170,    -1,   112,    -1,   114,    -1,   115,
      -1,   113,    -1,   122,    -1,   132,    -1,   128,    -1,   125,
      -1,   127,    -1,   124,    -1,   134,    -1,   129,    -1,   139,
      -1,   118,    -1,   137,    -1,   138,    -1,   130,    -1,   123,
      -1,   131,    -1,    16,    -1,   126,    -1,    10,    -1,   140,
      -1,   142,    -1,    98,    -1,   109,    -1,   106,    -1,   108,
      -1,   102,    -1,   107,    -1,   103,    -1,    99,    -1,   100,
      -1,   101,    -1,   111,    -1,   110,    -1,    -1,   145,   211,
     146,    -1,   145,   211,   146,    -1,    -1,   212,    -1,   211,
     117,   212,    -1,   213,   170,   239,   234,    -1,   156,   213,
     170,   239,   234,    -1,   213,   170,   239,   223,    -1,   213,
     145,   233,   146,   239,   234,    -1,   213,   145,   233,   146,
     239,   223,    -1,    -1,   214,    -1,    38,    -1,    41,    -1,
      55,    -1,    22,    -1,    22,    38,    -1,    22,    68,    -1,
      58,    -1,    68,    -1,    84,    -1,    -1,    58,    -1,    68,
      -1,    22,    68,    -1,    22,    -1,    84,    -1,    63,    -1,
      42,    -1,    -1,    22,    -1,    22,    68,    -1,    68,    -1,
      58,    -1,    84,    -1,    -1,    81,    -1,   136,    -1,   220,
      -1,   161,    -1,   173,    -1,   135,   170,    -1,   135,    -1,
      -1,   221,    -1,   121,   263,    -1,   121,   222,    -1,    -1,
      91,   263,    -1,    84,   226,   136,    -1,    21,    84,   226,
     136,    -1,    33,    84,   226,   136,    -1,   170,   227,    -1,
     170,   227,   117,   226,    -1,    -1,    98,   248,    -1,    98,
     236,    -1,   229,    58,   230,   136,    -1,   229,    22,   230,
     136,    -1,   229,    68,   230,   136,    -1,   229,    22,    68,
     230,   136,    -1,   229,    89,   230,   136,    -1,    -1,    21,
      -1,    33,    -1,   231,    -1,   230,   117,   231,    -1,   170,
     235,   234,    -1,   145,   233,   146,   235,   234,    -1,    85,
      -1,   170,    -1,   145,   233,   146,    -1,   232,    -1,   232,
     117,    -1,   232,   117,   233,    -1,    -1,    98,    51,    -1,
      98,   264,    -1,    -1,   116,   248,    -1,   116,   236,    -1,
     116,    28,    -1,   116,    75,    -1,   116,    78,    -1,   116,
      15,    -1,   116,    57,    -1,   116,    87,    -1,   116,    74,
      -1,     1,    -1,   147,   240,   148,   248,    -1,   147,   240,
     148,   236,    -1,   147,   240,    38,   263,   148,   248,    -1,
     147,     1,   148,    -1,    -1,   248,    -1,   221,    -1,   147,
     148,   237,    -1,   147,   240,   148,   237,    -1,   147,   148,
     238,    -1,   147,   240,   148,   238,    -1,   147,   240,    38,
     263,   148,   237,    -1,    -1,   116,   248,    -1,   116,   221,
      -1,   116,    28,    -1,   116,    75,    -1,   116,    78,    -1,
     116,    15,    -1,   116,    57,    -1,   116,    87,    -1,   116,
      74,    -1,   116,   238,    -1,   263,    -1,   221,    -1,   240,
     117,   263,    -1,   240,   117,   221,    -1,   263,    -1,   241,
     117,   263,    -1,    85,    -1,   264,    -1,   221,    -1,   242,
     117,   242,    -1,   243,   117,   242,    -1,    -1,   245,    -1,
     246,    -1,   245,   117,   246,    -1,   170,    98,   221,    -1,
     170,    98,   264,    -1,   221,    -1,   264,    -1,   170,    -1,
     251,    -1,   265,    -1,   250,    -1,   273,    -1,   272,    -1,
      75,   263,    -1,    39,   145,   244,   146,    -1,    28,   145,
     244,   146,    -1,    77,   145,   244,   146,    -1,    76,    77,
     145,   244,   146,    -1,    12,   263,    -1,    78,   263,    -1,
      15,   263,    -1,    57,   263,    -1,    87,   263,    -1,    74,
     263,    -1,    34,   263,    38,   263,    27,   263,    -1,    34,
     263,    38,   179,    27,   263,    -1,    34,   263,    27,   263,
      -1,    34,   263,    38,   263,    27,    37,   263,    79,   263,
      -1,    34,   263,    38,   179,    27,    37,   263,    79,   263,
      -1,    34,   263,    27,    37,   263,    79,   263,    -1,    35,
     263,    38,   263,    27,   263,    -1,    35,   263,    38,   179,
      27,   263,    -1,    35,   263,    27,   263,    -1,    35,   263,
      38,   263,    27,    37,   263,    79,   263,    -1,    35,   263,
      38,   179,    27,    37,   263,    79,   263,    -1,    35,   263,
      27,    37,   263,    79,   263,    -1,   147,   240,   148,   263,
      -1,   147,   240,    38,   263,   148,   263,    -1,   147,   240,
      38,   179,   148,   263,    -1,   147,   240,    38,   263,   148,
      37,   263,    79,   263,    -1,   147,   240,    38,   179,   148,
      37,   263,    79,   263,    -1,    37,   263,    79,   263,    29,
     263,    -1,    50,    -1,   251,    -1,   247,    -1,   268,    -1,
     267,    -1,   198,    -1,   261,    -1,   262,    -1,   260,   142,
     263,    -1,    -1,   254,    -1,    93,   145,   255,   146,    -1,
     258,    -1,   255,   117,   258,    -1,    93,   145,   257,   146,
      -1,   258,    -1,   257,   117,   258,    -1,   259,   247,   235,
     234,    -1,   276,    67,   247,    -1,   263,    67,   247,    -1,
      22,    -1,    38,    -1,    22,    38,    -1,    68,    -1,    22,
      68,    -1,    89,    -1,   265,    -1,   260,   142,   263,    -1,
      49,   263,    -1,    45,   230,    38,   263,    -1,   270,    -1,
     248,    -1,   249,    -1,   274,    -1,   275,    -1,   198,    -1,
     261,    -1,   262,    -1,   145,   121,   263,   146,    -1,   263,
     116,   263,    -1,   263,   120,   263,    -1,   263,   120,    -1,
     120,   263,    -1,   120,    -1,    82,   263,    -1,    83,   263,
      -1,   263,    -1,   247,    -1,   267,    -1,   268,    -1,   269,
      -1,   265,    -1,   198,    -1,   266,   145,   244,   146,    -1,
     266,   147,   244,   148,    -1,    61,   145,   244,   146,    -1,
     263,   119,   170,    -1,   263,   119,    84,    -1,   263,   119,
      28,    -1,   145,   242,   146,    -1,   145,   242,   117,   146,
      -1,   145,   243,   146,    -1,   145,   243,   117,   146,    -1,
       4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,    -1,
     143,   240,   144,    -1,   143,   240,   117,   144,    -1,   147,
     240,   148,    -1,   147,   240,   117,   148,    -1,   147,   271,
     148,    -1,   147,   271,   117,   148,    -1,   263,    96,   263,
      -1,   271,   117,   263,    96,   263,    -1,   263,   134,   263,
      -1,   263,   129,   263,    -1,   263,   139,   263,    -1,   263,
     118,   263,    -1,   263,   137,   263,    -1,   263,   138,   263,
      -1,   263,   130,   263,    -1,   263,   122,   263,    -1,   263,
     132,   263,    -1,   263,   128,   263,    -1,   263,   125,   263,
      -1,   263,   127,   263,    -1,   263,   124,   263,    -1,   263,
     112,   263,    -1,   263,   114,   263,    -1,   263,   115,   263,
      -1,   263,    97,   263,    -1,   263,   133,   263,    -1,   263,
     123,   263,    -1,   263,    16,   263,    -1,   263,    10,   263,
      -1,   263,   126,   263,    -1,   263,    26,   263,    -1,   134,
     263,    -1,   129,   263,    -1,    47,   263,    -1,    59,   263,
      -1,   131,   263,    -1,   113,   263,    -1,   263,    67,   263,
      -1,   263,    67,   179,    -1,   276,    67,   263,    -1,   276,
      67,   179,    -1,   263,    71,   263,    -1,   263,    71,   179,
      -1,   276,    71,   263,    -1,   276,    71,   179,    -1,   134,
      -1,   139,    -1,    97,    -1,   133,    -1,   112,    -1,   114,
      -1,   115,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   446,   446,   451,   452,   458,   459,   464,   465,   470,
     471,   472,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   496,   497,   498,   499,   503,
     506,   508,   514,   515,   516,   520,   521,   534,   535,   536,
     541,   542,   547,   552,   557,   561,   568,   573,   577,   582,
     586,   587,   588,   592,   596,   598,   600,   602,   604,   606,
     608,   610,   612,   614,   616,   618,   620,   622,   624,   626,
     631,   632,   636,   640,   641,   645,   646,   650,   651,   655,
     656,   657,   658,   659,   660,   661,   662,   666,   667,   671,
     672,   673,   674,   678,   693,   694,   695,   696,   697,   698,
     699,   700,   701,   702,   703,   704,   705,   706,   707,   708,
     709,   715,   721,   727,   733,   740,   750,   754,   755,   756,
     757,   758,   760,   762,   764,   769,   772,   773,   774,   775,
     776,   777,   781,   782,   786,   787,   788,   792,   793,   797,
     800,   802,   807,   808,   812,   814,   816,   823,   833,   843,
     853,   866,   871,   876,   884,   885,   890,   891,   893,   898,
     914,   921,   930,   938,   942,   949,   950,   955,   960,   954,
     985,   991,   998,  1006,  1014,  1025,  1031,  1024,  1059,  1063,
    1068,  1072,  1080,  1081,  1085,  1086,  1087,  1088,  1089,  1090,
    1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,
    1101,  1102,  1103,  1104,  1105,  1106,  1107,  1108,  1109,  1110,
    1114,  1115,  1116,  1117,  1118,  1119,  1120,  1121,  1122,  1123,
    1124,  1125,  1129,  1130,  1134,  1138,  1139,  1140,  1144,  1146,
    1148,  1150,  1152,  1157,  1158,  1162,  1163,  1164,  1165,  1166,
    1167,  1168,  1169,  1170,  1174,  1175,  1176,  1177,  1178,  1179,
    1183,  1184,  1188,  1189,  1190,  1191,  1192,  1193,  1197,  1198,
    1201,  1202,  1206,  1207,  1211,  1213,  1218,  1219,  1223,  1224,
    1228,  1229,  1233,  1235,  1237,  1242,  1255,  1272,  1273,  1275,
    1280,  1288,  1296,  1304,  1313,  1323,  1324,  1325,  1329,  1330,
    1338,  1340,  1345,  1347,  1349,  1354,  1356,  1358,  1365,  1366,
    1367,  1371,  1372,  1373,  1374,  1375,  1376,  1377,  1378,  1379,
    1380,  1381,  1401,  1405,  1409,  1417,  1424,  1425,  1426,  1430,
    1432,  1438,  1440,  1442,  1447,  1448,  1449,  1450,  1451,  1452,
    1453,  1454,  1455,  1456,  1457,  1463,  1464,  1465,  1466,  1470,
    1471,  1475,  1476,  1477,  1481,  1482,  1486,  1487,  1491,  1492,
    1496,  1497,  1498,  1499,  1503,  1514,  1515,  1516,  1517,  1518,
    1519,  1521,  1523,  1525,  1527,  1529,  1531,  1533,  1535,  1537,
    1539,  1544,  1546,  1548,  1550,  1552,  1554,  1556,  1558,  1560,
    1562,  1564,  1566,  1568,  1575,  1581,  1587,  1593,  1602,  1612,
    1620,  1621,  1622,  1623,  1624,  1625,  1626,  1627,  1632,  1633,
    1637,  1641,  1642,  1646,  1650,  1651,  1655,  1659,  1663,  1670,
    1671,  1672,  1673,  1674,  1675,  1679,  1680,  1685,  1690,  1698,
    1699,  1700,  1701,  1702,  1703,  1704,  1705,  1706,  1708,  1710,
    1712,  1714,  1716,  1721,  1722,  1723,  1726,  1727,  1728,  1729,
    1740,  1741,  1745,  1746,  1747,  1751,  1752,  1753,  1761,  1762,
    1763,  1764,  1768,  1769,  1770,  1771,  1772,  1773,  1774,  1775,
    1776,  1777,  1781,  1789,  1790,  1794,  1795,  1796,  1797,  1798,
    1799,  1800,  1801,  1802,  1803,  1804,  1805,  1806,  1807,  1808,
    1809,  1810,  1811,  1812,  1813,  1814,  1815,  1816,  1820,  1821,
    1822,  1823,  1824,  1825,  1829,  1830,  1831,  1832,  1836,  1837,
    1838,  1839,  1844,  1845,  1846,  1847,  1848,  1849,  1850
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
  "require_stmt", "assignment_stmt", "opt_ident", "ident",
  "opt_ident_or_string", "do_stmt", "return_stmt", "class_level_stmt",
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
  "opt_where_part", "type_alias_decl_stmt", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_stmt", "opt_config", "var_decl_stmt_inner_ls",
  "var_decl_stmt_inner", "tuple_var_decl_component",
  "tuple_var_decl_stmt_inner_ls", "opt_init_expr", "opt_type",
  "array_type", "opt_formal_array_elt_type", "formal_array_type",
  "opt_formal_type", "expr_ls", "simple_expr_ls", "tuple_component",
  "tuple_expr_ls", "opt_actual_ls", "actual_ls", "actual_expr",
  "ident_expr", "type_level_expr", "for_expr", "cond_expr", "nil_expr",
  "stmt_level_expr", "opt_task_intent_ls", "task_intent_clause",
  "task_intent_ls", "forall_intent_clause", "forall_intent_ls",
  "intent_expr", "shadow_var_prefix", "io_expr", "new_expr", "let_expr",
  "expr", "opt_try_expr", "lhs_expr", "fun_expr", "call_expr", "dot_expr",
  "parenthesized_expr", "literal_expr", "assoc_expr_ls", "binary_op_expr",
  "unary_op_expr", "reduce_expr", "scan_expr", "reduce_scan_op_expr", YY_NULL
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
     234,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   236,   236,   236,   236,   237,   237,   237,   238,
     238,   238,   238,   238,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   240,   240,   240,   240,   241,
     241,   242,   242,   242,   243,   243,   244,   244,   245,   245,
     246,   246,   246,   246,   247,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   250,   251,
     252,   252,   252,   252,   252,   252,   252,   252,   253,   253,
     254,   255,   255,   256,   257,   257,   258,   258,   258,   259,
     259,   259,   259,   259,   259,   260,   260,   261,   262,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   264,   264,   264,   265,   265,   265,   265,
     266,   266,   267,   267,   267,   268,   268,   268,   269,   269,
     269,   269,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   271,   271,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   273,   273,
     273,   273,   273,   273,   274,   274,   274,   274,   275,   275,
     275,   275,   276,   276,   276,   276,   276,   276,   276
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
       0,     1,     1,     1,     1,     2,     1,     2,     3,     1,
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
       2,     0,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     4,     4,     6,     3,     0,     1,     1,     3,
       4,     3,     4,     6,     0,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     1,     1,     3,     3,     1,
       3,     1,     1,     1,     3,     3,     0,     1,     1,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     4,     4,     4,     5,     2,     2,     2,     2,     2,
       2,     6,     6,     4,     9,     9,     7,     6,     6,     4,
       9,     9,     7,     4,     6,     6,     9,     9,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       4,     1,     3,     4,     1,     3,     4,     3,     3,     1,
       1,     2,     1,     2,     1,     1,     3,     2,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     3,     3,
       2,     2,     1,     2,     2,     1,     1,     1,     1,     1,
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
       3,     0,     0,     1,     0,    82,   462,   463,   464,   465,
     466,     0,   408,    80,     0,   161,   408,     0,   296,    80,
       0,     0,     0,     0,   171,    80,    80,     0,     0,   295,
       0,     0,   181,     0,   177,     0,     0,     0,     0,   399,
       0,   184,     0,     0,     0,     0,   295,   295,   162,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   163,     0,     0,     0,     0,   514,   516,     0,
     517,   518,   442,     0,     0,   515,   512,    89,   513,     0,
       0,     0,     4,     0,     5,     9,    45,    10,    11,    12,
      14,   364,    22,    13,    90,    96,    15,    17,    16,    19,
      20,    21,    18,    95,     0,    93,     0,   434,     0,    97,
      94,    98,     0,   446,   430,   431,   367,   365,     0,     0,
     435,   436,     0,   366,     0,   447,   448,   449,   429,   369,
     368,   432,   433,     0,    38,    24,   375,     0,     0,   409,
       0,    81,     0,     0,     0,     0,     0,     0,     0,   434,
     446,   365,   435,   436,   377,   366,   447,   448,     0,   408,
       0,     0,   135,     0,   349,     0,   356,    84,    83,   182,
     103,     0,   183,     0,     0,     0,     0,     0,   296,   297,
     102,     0,     0,   356,     0,     0,     0,     0,     0,   298,
       0,    86,    32,     0,   500,   427,     0,   378,   501,     7,
     356,   297,    92,    91,   275,   346,     0,   345,     0,     0,
      87,   445,     0,     0,    35,     0,   380,   370,     0,   356,
      36,   376,     0,   142,   138,     0,   366,   142,   139,     0,
     287,     0,   379,     0,   345,     0,     0,   503,   441,   499,
     502,   498,     0,    47,    50,     0,     0,   351,     0,   353,
       0,     0,   352,     0,   345,     0,     0,     6,    46,     0,
     164,     0,   261,   260,   185,     0,     0,     0,     0,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   440,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   356,   356,     0,     0,     0,
      25,    26,     0,     0,     0,     0,     0,    27,     0,     0,
       0,    28,     0,    29,     0,   364,   362,     0,   357,   358,
     363,     0,     0,     0,     0,   112,     0,     0,   111,     0,
       0,     0,   118,     0,     0,    56,    99,     0,   220,   227,
     228,   229,   224,   226,   222,   225,   223,   221,   231,   230,
     128,     0,     0,    30,   235,   178,   302,     0,   303,   305,
       0,   321,     0,   308,     0,     0,    85,    31,    33,     0,
     183,   274,     0,    63,   443,   444,    88,     0,    34,   356,
       0,   149,   140,   136,   141,   137,     0,   285,   282,    60,
       0,    56,   105,    37,    49,    48,    51,     0,   467,     0,
       0,   458,     0,   460,     0,     0,     0,     0,     0,     0,
     471,     8,     0,     0,     0,     0,   254,     0,     0,     0,
       0,     0,   407,   495,   494,   497,   505,   504,   509,   508,
     491,   488,   489,   490,   438,   478,   457,   456,   455,   439,
     482,   493,   487,   485,   496,   486,   484,   476,   481,   483,
     492,   475,   479,   480,   477,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   506,   511,   510,   419,   420,   422,
     424,     0,   411,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   469,   408,   408,   108,   283,   350,     0,     0,
     372,     0,   284,   164,     0,     0,     0,   383,     0,     0,
       0,   389,     0,     0,     0,   119,   513,    59,     0,    52,
      57,     0,   127,     0,     0,     0,   371,   248,   245,   246,
     247,   251,   252,   253,   243,     0,   236,     0,   244,   262,
       0,   306,     0,   317,   314,   318,   320,   315,   316,   319,
       0,   313,   430,     0,   300,   428,   299,   454,   348,   347,
       0,     0,     0,   373,     0,   143,   289,   430,     0,     0,
       0,   468,   437,   459,   354,   461,   355,     0,     0,   470,
     124,   393,     0,   473,   472,     0,     0,   165,     0,     0,
     175,     0,   172,   258,   255,   256,   259,   186,     0,     0,
     291,   290,   292,   294,    64,    71,    72,    73,    68,    70,
      78,    79,    66,    69,    67,    65,    75,    74,    76,    77,
     452,   453,   421,   423,     0,   410,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   104,
     360,   361,   359,     0,     0,   126,     0,     0,   110,     0,
     109,     0,     0,   116,     0,     0,   114,     0,     0,   414,
       0,   100,     0,   101,     0,     0,   130,     0,   132,   249,
     250,     0,   243,   234,     0,   334,   263,   266,   265,   267,
       0,   304,   307,   308,     0,     0,   309,   310,   151,     0,
       0,   150,   153,   374,     0,   144,   147,     0,   286,    61,
      62,     0,     0,     0,     0,   125,     0,     0,     0,   295,
     170,     0,   173,   169,   257,   262,   217,   215,   196,   199,
     197,   198,   209,   200,   213,   205,   203,   216,   204,   202,
     207,   212,   214,   201,   206,   210,   211,   208,   218,   219,
       0,   194,     0,   232,   232,   192,   293,   412,   308,   446,
     446,     0,     0,     0,     0,     0,     0,     0,     0,   107,
     106,     0,   113,     0,     0,   382,     0,   381,     0,     0,
     388,   117,     0,   387,   115,     0,   413,    54,    53,   129,
     398,   131,     0,   334,   237,     0,     0,   308,   264,   280,
     301,   325,     0,   469,     0,   155,     0,     0,     0,   145,
       0,   122,   395,     0,     0,   120,   394,     0,   474,     0,
      39,     0,   159,    80,   295,   295,   157,   295,   167,   176,
     174,     0,   195,     0,     0,   235,   188,   189,   416,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   295,   386,
       0,     0,   392,     0,     0,   415,     0,     0,   134,   308,
     334,   340,   337,   341,   343,   338,   339,   342,     0,   336,
     344,   430,   276,   240,   238,     0,     0,     0,   323,   430,
     156,   154,     0,   148,     0,   123,     0,   121,    41,    40,
     168,   268,   193,   194,   232,   232,     0,     0,     0,     0,
       0,     0,     0,   160,   158,     0,     0,     0,     0,    55,
     133,   239,   308,   326,     0,   277,   279,   278,   281,   272,
     273,   179,     0,   146,     0,     0,   269,   280,   190,   191,
     233,     0,     0,     0,     0,     0,     0,   385,   384,   391,
     390,   242,   241,   328,   329,   331,   430,     0,   469,   430,
     397,   396,     0,     0,   330,   332,   270,   187,   271,   326,
     333
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   244,    83,   542,    85,    86,   259,    87,
     245,   537,   541,   538,    88,    89,    90,   168,    91,   172,
     192,    92,    93,    94,    95,    96,    97,   655,    98,    99,
     100,   402,   585,   717,   101,   102,   581,   712,   103,   104,
     434,   729,   105,   106,   611,   612,   149,   185,   559,   108,
     109,   436,   735,   617,   762,   763,   371,   846,   375,   555,
     556,   557,   558,   618,   264,   700,   937,   967,   931,   205,
     926,   883,   886,   110,   231,   407,   111,   112,   188,   189,
     379,   380,   574,   383,   571,   954,   880,   807,   246,   163,
     250,   251,   337,   338,   339,   150,   114,   115,   116,   151,
     118,   138,   139,   501,   353,   678,   502,   503,   119,   152,
     153,   211,   340,   155,   124,   156,   157,   127,   128,   255,
     129,   130,   131,   132,   133
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -855
static const yytype_int16 yypact[] =
{
    -855,   147,  2637,  -855,   -34,  -855,  -855,  -855,  -855,  -855,
    -855,  3949,    59,   164,  8077,  -855,    59,  8077,   145,   164,
    3949,  8077,  3949,    90,  -855,   146,   370,  6752,  8077,  6872,
    8077,   105,  -855,   164,  -855,    41,  7257,  8077,  8077,  -855,
    8077,  -855,  8077,  8077,   250,   118,  1200,  1289,  -855,  7377,
    6367,  8077,  7257,  8077,  8077,   183,   154,  3949,  8077,  8197,
    8197,   164,  -855,  8077,  7377,  8077,  8077,  -855,  -855,  8077,
    -855,  -855,  9637,  8077,  8077,  -855,  8077,  -855,  -855,  3079,
    5812,  7377,  -855,  3804,  -855,  -855,   255,  -855,  -855,  -855,
    -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,
    -855,  -855,  -855,  -855,   164,  -855,   164,   319,   196,  -855,
    -855,  -855,   242,   170,  -855,  -855,  -855,   210,   248,   199,
     271,   283, 13416,  2244,   216,   293,   301,  -855,  -855,  -855,
    -855,  -855,  -855,   334,  -855,  -855, 13416,   312,  3949,  -855,
     313,  -855,  8077,  8077,  8077,  8077,  8077,  7377,  7377,   231,
    -855,  -855,  -855,  -855,   344,   333,  -855,  -855,   325, 11736,
     164,   331,  -855,   204, 13416,   380,  6487,  -855,  -855,  -855,
    -855,   164,    68,   164,   330,    18, 11277, 11243,  -855,  -855,
    -855, 11661, 10494,  6487,  3949,   339,    63,    67,    60,  -855,
    3949,  -855,  -855, 11327,   195,   344, 11327,   344,   195,  -855,
    6487,   188,  -855,  -855,   164,  -855,   239, 13416,  8077,  8077,
    -855, 13416,   337, 11536,  -855, 11327,   344, 13416,   340,  6487,
    -855, 13416, 11945,  -855,  -855, 11991,  2327,  -855,  -855, 12070,
     381,   352,   344,   282, 11786, 11327, 12116,   272,   886,   195,
     272,   195,   161,  -855,  -855,  3224,    95,  -855,  8077,  -855,
      -7,   101,  -855,    83, 12195,   -18,   483,  -855,  -855,   443,
     379,   354,  -855,  -855,  -855,    27,    41,    41,    41,  -855,
    8077,  8077,  8077,  8077,  7522,  7522,  8077,  8077,  8077,  8077,
    8077,  8077,   129,  9637,  8077,  8077,  8077,  8077,  8077,  8077,
    8077,  8077,  8077,  8077,  8077,  8077,  8077,  8077,  8077,  6992,
    6992,  6992,  6992,  6992,  6992,  6992,  6992,  6992,  6992,  6992,
    6992,  6992,  6992,  6992,  6992,  6487,  6487,  7522,  7522,  6247,
    -855,  -855, 11861, 11911, 12259,    62,  3369,  -855,  7522,    18,
     362,  -855,  8077,  -855,  8077,   402,  -855,   357,   394,  -855,
    -855,   382,   164,   474,  7377,  -855,  4094,  7522,  -855,  4239,
    7522,   371,  -855,    18,  8317,  8077,  -855,  3949,  -855,  -855,
    -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,
     486,  8077,   373,  -855,    51,  -855,  -855,    63,  -855,   400,
     374,  -855,  8437,   425,  8077,    41,  -855,  -855,  -855,   384,
    -855,  -855,  7377,  -855, 13416, 13416,  -855,    36,  -855,  6487,
     387,  -855,   508,  -855,   508,  -855,  8557,   409,  -855,  -855,
    8317,  8077,  -855,  -855,  -855,  -855,  -855,  7112,  -855, 10361,
    5957,  -855,  6102,  -855,  7522,  5374,  2934,   383,  8077,  5666,
    -855,  -855,   164,  7377,   393,   386,   165,    41,   299,   300,
     304,   316, 11611, 13526, 13526,   346,  -855,   346,  -855,   346,
   13590,   969,   467,   623,   344,   272,  -855,  -855,  -855,   886,
   13636,   346,  2127,  2127, 13526,  2127,  2127,   525,   272, 13636,
   13555,   525,   195,   195,   272,   401,   404,   407,   408,   410,
     412,   414,   416,   418,   419,   420,   421,   433,   436,   437,
     438,   399,   428,  -855,   346,  -855,   346,   166,  -855,  -855,
    -855,   102,  -855,   164, 13480,   376,  8677,  7522,  8797,  7522,
    8077,  7522,  9926,    59, 12332,  -855,  -855, 13416, 12396,  6487,
    -855,  6487,  -855,   379,  8077,   109,  8077, 13416,    49, 11452,
    8077, 13416,    38, 11402,  6247,  -855,   441,   422,   451, 12469,
     422,   452,   498, 12533,  3949, 11486,  -855,   173,  -855,  -855,
    -855,  -855,  -855,  -855,   638,   110,  -855,    57,  -855,   328,
     445,    63,    67,  8077,    90,  8077,  8077,  8077,  8077,  8077,
    2785,  -855,  1039,  6632,  -855, 13416,  -855,  -855,  -855, 13416,
     449,    70,   453,  -855,    28,  -855,  -855,   317,   164,   462,
     466,  -855,  -855,  -855,  -855,  -855,  -855,     3, 10152,  -855,
    -855, 13416,  3949, 13416,  -855, 12602,   446,   490,  1345,   459,
     511,   163,  -855,   542,  -855,  -855,  -855,  -855,  9979,   324,
    -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,
    -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,
    -855,  -855,  -855,  -855,  6247,  -855,    52,  7522,  7522,  8077,
     584, 12666,  8077,   585, 12735,   481, 10190,    18,    18,  -855,
    -855,  -855,  -855,   487, 11327,  -855, 10572,  4384,  -855,  4529,
    -855, 10650,  4674,  -855,    18,  4819,  -855,    18,   115,  -855,
    8077,  -855,  8077,  -855,  3949,  8077,  -855,  3949,   602,  -855,
    -855,   164,   747,  -855,    63,   516,   565,  -855,  -855,  -855,
      47,  -855,  -855,   425,   503,    76,  -855,  -855,  -855,  4964,
    7377,  -855,  -855,  -855,   164,  -855,   518,   325,  -855,  -855,
    -855,  5109,   504,  5254,   505,  -855,  8077,  3514,   494,  1450,
    -855,  8077,   164,  -855,  -855,   328,  -855,  -855,  -855,   164,
    -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,
    -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,
    8077,   535,   537,   512,   512,  -855,  -855,  -855,   425,   187,
     191, 12799,  8917,  9037, 12872,  9157,  9277,  9397,  9517,  -855,
    -855,  2465,  -855,  3949,  8077, 13416,  8077, 13416,  3949,  8077,
   13416,  -855,  8077, 13416,  -855,  6247,  -855, 12936, 13416,  -855,
   13416,   629,  3949,   516,  -855,   515,  7642,   124,  -855,   575,
    -855,  -855,  7522,  9849,  3949,  -855,    12,   522,  8077,  -855,
    8077,  -855, 13416,  3949,  8077,  -855, 13416,  3949, 13416,   176,
    -855,  3659,  -855,   403,   538,   538,  -855,  1563,  -855, 13416,
    -855,    42,  -855, 10399, 10027,    51,  -855,  -855,  -855,  8077,
    8077,  8077,  8077,  8077,  8077,  8077,  8077,   527,  2056, 12533,
   10728, 10806, 12533, 10884, 10962,  -855,  8077,  3949,  -855,   425,
     516,  8077,    90,  8077,  8077,  8077,  8077,  8077,  5520,  -855,
    -855,   119,  7377,  -855,  -855,  8077,    23, 10228,  -855,  1045,
     380,  -855,   325, 13416, 11040,  -855, 11118,  -855,  -855,  -855,
    -855,   588,  -855,  -855,   512,   512,   205, 13005, 13069, 13142,
   13206, 13279, 13343,  -855,  -855,  3949,  3949,  3949,  3949, 13416,
    -855,  -855,   124,  7787,    80,  -855,  -855, 13416, 13416,  -855,
    -855,  -855,  9517,  -855,  3949,  3949,  -855,   575,  -855,  -855,
    -855,  8077,  8077,  8077,  8077,  8077,  8077, 12533, 12533, 12533,
   12533,  -855,  -855,  -855,  -855,  -855,   251,  7522,  9728,  1126,
   12533, 12533,    58, 10323,  -855,  -855,  -855,  -855,  -855,  7932,
    -855
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -855,  -855,  -855,    -1,  -369,  1733,  -855,  -855,  -855,  1109,
     -55,  -286,   263,   267,  -855,  -855,   385,    88,  2197,     8,
     306,  -792,  -638,   -43,  -855,  -855,  -855,    53,  -855,  -855,
    -855,   454,  -855,   -30,  -855,  -855,  -855,  -855,  -855,   514,
     168,   -94,  -855,  -855,  -855,   -44,   889,  -855,  -855,  -855,
    -855,  -855,  -855,  -855,  -855,  -152,  -607,  -689,  -855,  -150,
       5,   148,  -855,  -855,  -855,   -36,  -855,  -855,  -259,   -63,
    -855,  -218,  -232,  -855,  -158,  -855,   681,  -855,  -183,   326,
    -855,  -356,  -680,  -538,  -399,  -854,  -836,  -706,   -35,  -855,
      74,  -855,  -141,  -855,   192,   363,  -374,  -855,  -855,   986,
    -855,     0,  -855,  -855,  -231,  -855,  -528,  -855,  -855,  1127,
    1337,    -2,   314,   391,  -855,  1382,  1506,  -855,  -855,  -855,
    -855,  -855,  -855,  -855,  -309
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -470
static const yytype_int16 yytable[] =
{
     122,    82,   330,   202,   203,   554,   679,   586,   572,   136,
     505,   764,   154,   341,   206,   159,   158,   249,   122,   164,
     122,   560,   427,   810,   703,   176,   177,   181,   182,   233,
       5,     5,   587,   169,   193,   194,   195,   580,   196,   190,
     197,   198,   372,   381,     5,   190,   253,   207,   381,   213,
     215,   216,   217,   381,  -243,   221,   222,   225,   229,   389,
       5,   232,   234,   235,   236,   672,     5,   237,   381,   540,
     238,   239,   240,   547,   241,   847,   667,   207,   400,   254,
     175,   122,   438,   439,   440,   441,    15,   955,   848,   548,
    -152,   838,   549,    50,   930,   437,   351,   869,   384,   429,
     511,   140,   134,   336,   964,  -311,   550,   161,   768,   551,
     420,    44,  -311,   325,   812,   970,   767,  -311,   957,   552,
     336,   424,   965,  -311,   709,   540,  -152,   884,    50,   392,
     430,   351,     5,  -311,    48,   553,   122,   336,  -311,   421,
     136,   322,   323,   324,   221,   207,   254,     3,   376,     5,
    -311,   721,   137,   167,    62,   326,   336,   456,   382,   329,
     710,   326,   809,   382,   922,  -311,   326,     5,   382,  -311,
     930,   326,   186,   714,   491,   492,   351,   385,  -311,   425,
    -152,   326,   122,   382,  -311,  -311,   186,   613,   122,   921,
    -311,     5,   326,   425,   966,   167,  -243,   425,  -311,   900,
     425,   326,   694,  -311,   642,   702,   394,   395,   377,   390,
     512,   689,   417,   457,   711,   938,   939,  -335,   422,   644,
     838,   273,   573,   614,   813,   505,   392,   692,   958,   160,
     -80,   426,   795,   615,   643,   166,  -335,   905,   262,   418,
    -335,   690,   952,   122,   416,   882,   419,   423,   645,   616,
     183,   -80,   336,   336,   619,   665,   693,   199,   582,   263,
     218,   796,   274,   200,   265,  -335,   275,   865,   442,   443,
     444,   445,   447,   449,   450,   451,   452,   453,   454,   455,
     732,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   474,   134,   273,   219,
     266,   674,   677,   901,  -418,   414,  -401,   733,  -417,   525,
     267,   280,   134,   281,   282,   494,   496,   504,   285,   258,
     898,   332,   692,   554,   122,   292,   514,   446,   448,   578,
     517,   268,   518,  -418,   298,   505,   336,  -417,   805,   274,
     333,   270,   207,   275,   527,   529,  -400,   531,   533,  -327,
     696,   940,   539,   539,   578,   543,   392,   249,   214,   249,
     837,   315,   578,   316,   212,   113,   722,   724,  -327,   545,
     493,   495,  -327,     5,   113,   393,  -451,   167,  -451,   170,
     122,   513,   575,   113,   269,   113,   697,   609,   280,     5,
     579,   282,  -297,   123,   252,   285,   698,  -327,   607,   392,
     528,   317,   123,   532,   122,   318,     5,  -405,   539,   539,
     167,   123,   699,   123,   888,   579,   385,   385,   409,  -406,
     113,   385,   598,   579,   601,  -297,   603,   605,  -297,  -403,
     718,   207,   881,   385,  -288,   620,   621,  -402,  -297,   889,
     622,   385,   113,   648,   224,   228,   113,   318,   123,   321,
     226,   226,   623,  -288,   171,  -404,   660,   319,   336,  -297,
     766,  -297,   280,   282,  -451,   282,  -451,   331,   326,   285,
     123,  -297,   334,   396,   123,   344,   554,   597,  -450,   406,
    -450,   345,   348,   352,   374,   399,   505,   171,   408,   837,
     431,   432,  -297,   273,   594,   433,   596,   435,   516,   387,
     519,   113,   388,   520,   527,   651,   531,   654,   543,   656,
     601,   521,   524,   657,   658,   544,   534,   561,   522,   546,
     562,   398,   664,   573,   666,   584,   588,   684,   671,   123,
     577,   602,   504,   583,   274,   705,   608,   624,   275,   680,
     625,   412,   122,   626,   627,   640,   628,   113,   629,   956,
     630,   273,   631,   113,   632,   633,   634,   635,   959,   178,
     650,   154,   653,   197,   216,   217,   221,   232,   254,   636,
      25,   201,   637,   638,   639,   123,   641,   -58,    32,   277,
    -180,   123,   279,   280,   956,   281,   282,   681,   683,   727,
     285,   701,   274,   708,    41,   956,   275,   292,   719,   713,
     122,  -180,   720,   730,   296,   297,   298,   392,   113,   731,
     734,   772,   775,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,   777,
     781,   802,   806,   808,   818,   515,   123,   277,   832,   278,
     279,   280,   504,   281,   282,   447,   494,   771,   285,   273,
     774,   811,   823,   827,  -364,   292,   844,   845,   867,   535,
     547,   870,   296,   297,   298,   785,   885,   787,   892,   936,
     790,   913,   831,   793,   590,   816,   548,   589,   797,   549,
     798,   404,   122,   800,   817,   122,   342,   858,   840,   113,
     274,   663,   904,   550,   275,   906,   551,   804,   256,   841,
     446,   493,   691,   968,   951,   962,   552,   122,   207,   113,
     180,   576,   113,   662,     0,     0,     0,   123,     0,   822,
     113,   826,   553,     0,   828,   122,     0,     0,     0,   839,
       0,     0,     0,     0,   252,   277,   252,   123,     0,   280,
     123,   281,   282,   879,     0,     0,   285,     0,   123,     0,
       0,     0,     0,   292,     0,     0,     0,     0,   843,     0,
     296,   297,   298,     0,     0,     0,     0,     0,     0,   547,
     785,   787,     0,   790,   793,   822,   826,     0,     0,     0,
       0,   859,   860,     0,   861,   548,   862,   863,   549,   113,
     864,   202,   203,   504,     0,     0,     0,     0,     0,     0,
     122,     0,   550,     0,   122,   551,     0,    44,     0,     0,
     887,   601,   122,     0,     0,   552,   893,   123,   894,   925,
       0,   122,   896,     0,     0,   122,     0,     0,     0,   122,
     416,   553,     0,   661,   668,   670,     0,     0,   673,   676,
       0,     0,     0,   924,     0,     0,     0,   859,   907,   908,
     862,   909,   910,   911,   912,     0,     0,     0,     0,     0,
     953,     0,     0,     0,   919,   122,   646,     0,     0,   154,
       0,   197,   216,   217,   221,   232,   254,     0,     0,     0,
     927,     0,     0,   928,     0,     0,     0,   707,     0,     0,
       0,   107,     0,     0,     0,   953,     0,     0,     0,     0,
     107,     0,     0,     0,     0,     0,   953,   113,     0,   107,
       0,   107,   273,   947,   948,   949,   950,     0,     0,     0,
       0,   122,     0,     0,     0,     0,     0,     0,     0,     0,
     826,     0,   960,   961,     0,   123,     0,     0,     0,   947,
     948,   949,   950,   960,   961,     0,   107,     0,     0,     0,
       0,     0,     0,   274,     0,   963,   601,   275,     0,     0,
       0,     0,     0,   779,   780,   113,     0,   826,   107,     0,
     782,     0,   107,     0,     0,     0,     0,     0,     0,     0,
     791,   765,     0,   794,     0,     0,     0,     0,   117,     0,
       0,     0,     0,   123,     0,   273,     0,   117,   277,     0,
     278,   279,   280,     0,   281,   282,   117,     0,   117,   285,
     769,   770,     0,     0,     0,   291,   292,     0,     0,     0,
     295,     0,     0,   296,   297,   298,     0,   107,     0,     0,
     113,     0,   113,     0,     0,   113,   274,     0,   113,     0,
     275,     0,     0,   117,     0,     0,     0,   113,     0,     0,
     113,     0,     0,     0,     0,     0,     0,     0,   123,     0,
     123,     0,     0,   123,     0,   117,   123,     0,     0,   117,
       0,     0,   113,   107,     0,   123,     0,  -312,   123,   107,
       0,     0,     0,  -322,   113,   280,   113,   281,   282,     0,
     113,     0,   285,     0,     0,     0,     0,     0,     0,   292,
     123,     0,     0,     0,     0,     0,   296,   297,   298,  -312,
       0,     0,   123,     0,   123,  -322,     0,     0,   123,     0,
    -312,     0,   891,     0,   117,     0,  -322,     0,     0,   120,
    -312,     0,     0,     0,   107,     0,  -322,  -312,   120,     0,
       0,     0,     0,  -322,     0,   191,   113,   120,     0,   120,
       0,   113,     0,     0,     0,     0,  -312,     0,     0,     0,
       0,   191,  -322,     0,  -324,   113,     0,     0,   223,   227,
     117,     0,     0,     0,   123,  -312,   117,   113,     0,   123,
       0,  -322,  -312,     0,   120,  -312,   113,     0,  -322,     0,
     113,  -322,     0,   123,   113,     0,  -324,     0,     0,     0,
       0,     0,     0,     0,     0,   123,   120,  -324,     0,     0,
     120,     0,     0,     0,   123,   107,     0,  -324,   123,     0,
       0,   178,   123,     0,  -324,     0,     0,     0,     0,     0,
     113,   117,    25,   201,     0,   107,     0,     0,   107,     0,
      32,     0,  -180,  -324,     0,     0,   107,     0,   -44,     0,
       0,     0,     0,     0,     0,     0,    41,     0,   123,     0,
       0,     0,  -324,  -180,   -44,   120,     0,   327,     0,  -324,
       0,     0,  -324,     0,     0,     0,     0,     0,   113,   113,
     113,   113,     0,     0,   191,   191,   191,     0,     0,     0,
       0,   370,     0,     0,     0,     0,     0,   113,   113,     0,
       0,     0,   191,     0,     0,   191,   123,   123,   123,   123,
     178,   120,   117,     0,     0,   107,     0,   120,     0,     0,
       0,    25,   201,     0,   191,   123,   123,     0,     0,    32,
       0,  -180,   117,     0,     0,   117,     0,   -43,     0,   121,
       0,     0,     0,   117,   191,    41,   728,     0,   121,     0,
       0,     0,  -180,   -43,     0,     0,     0,   121,     0,   121,
       0,     0,     0,  -166,     0,     0,  -166,  -166,     0,     0,
       0,     0,   120,     0,     0,  -166,     0,  -166,  -166,     0,
       0,  -166,     0,     0,   125,  -166,     0,  -166,     0,     0,
       0,     0,     0,   125,   121,     0,     0,     0,     0,     0,
       0,  -166,   125,  -166,   125,  -166,     0,  -166,  -166,     0,
    -166,  -166,   117,  -166,     0,     0,   121,     0,     0,     0,
     121,     0,     0,     0,     0,     0,     0,     0,     0,  -166,
       0,  -166,     0,   107,  -166,     0,     0,     0,   191,   125,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   120,     0,     0,     0,     0,     0,     0,
       0,   125,   191,     0,     0,   125,     0,     0,    15,     0,
       0,    18,     0,   120,     0,   121,   120,     0,     0,     0,
      24,  -166,    25,   833,   120,     0,    29,     0,     0,  -166,
      32,   107,  -180,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    41,     0,   126,     0,
      44,     0,   834,  -180,     0,   835,    48,   126,     0,     0,
     125,   121,     0,     0,     0,     0,   126,   121,   126,     0,
     117,     0,     0,     0,    61,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   120,     0,     0,   107,     0,   107,     0,
       0,   107,     0,   126,   107,     0,   125,     0,     0,     0,
       0,     0,   125,   107,     0,     0,   107,     0,     0,     0,
       0,    15,   121,     0,    18,   126,    77,     0,   117,   126,
       0,     0,     0,    24,   836,    25,   833,     0,   107,    29,
       0,     0,     0,    32,     0,  -180,     0,     0,     0,     0,
     107,     0,   107,     0,     0,     0,   107,     0,     0,    41,
       0,     0,     0,   256,     0,   834,  -180,   125,   835,    48,
       0,     0,     0,     0,     0,     0,     0,   191,   191,     0,
       0,   191,   191,     0,   126,     0,     0,    61,     0,    62,
       0,     0,     0,   117,   688,   117,     0,     0,   117,     0,
       0,   117,     0,   121,     0,     0,     0,     0,     0,     0,
     117,   120,   107,   117,     0,     0,     0,   107,     0,     0,
       0,     0,     0,   121,     0,     0,   121,     0,     0,     0,
     126,   107,     0,   715,   121,   117,   126,     0,     0,    77,
       0,     0,     0,   107,     0,     0,     0,   117,   125,   117,
       0,     0,   107,   117,     0,     0,   107,     0,     0,     0,
     107,     0,     0,     0,     0,     0,     0,     0,   125,   120,
       0,   125,     0,     0,     0,    84,     0,     0,     0,   125,
       0,     0,     0,     0,   135,     0,     0,     0,     0,     0,
       0,   126,     0,   162,     0,   165,   107,     0,     0,     0,
       0,     0,     0,   121,     0,     0,   191,   191,     0,   117,
       0,     0,     0,   191,   117,   370,     0,     0,     0,     0,
     370,     0,     0,   191,     0,     0,   191,     0,   117,     0,
     220,     0,     0,     0,   120,     0,   120,     0,     0,   120,
     117,     0,   120,     0,   107,   107,   107,   107,   125,   117,
       0,   120,    84,   117,   120,     0,   257,   117,     0,     0,
       0,     0,     0,   107,   107,     0,   819,     0,     0,     0,
       0,     0,   126,     0,     0,     0,   120,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   120,     0,
     120,     0,   126,   117,   120,   126,     0,     0,     0,     0,
       0,     0,     0,   126,     0,     0,     0,     0,     0,     0,
       0,   320,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   121,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   117,   117,   117,   117,     0,     0,     0,     0,     0,
     120,     0,     0,     0,     0,   120,     0,   373,     0,     0,
     117,   117,     0,   386,     0,   191,   125,     0,     0,   120,
       0,     0,   126,     0,     0,     0,     0,     0,     0,   121,
       0,   120,     0,     0,     0,     0,     0,     0,     0,     0,
     120,     0,     0,     0,   120,     0,     0,     0,   120,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   370,
     370,     0,   370,   370,     0,     0,     0,     0,    84,     0,
       0,     0,     0,     0,   125,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   120,   929,     0,     0,     0,     0,
       0,   933,     0,   370,   121,   370,   121,     0,     0,   121,
       0,     0,   121,     0,     0,     0,     0,     0,     0,     0,
       0,   121,     0,     0,   121,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   120,   120,   120,   120,   121,     0,     0,   125,
     126,   125,     0,     0,   125,     0,     0,   125,   121,    84,
     121,   120,   120,     0,   121,     0,   125,     0,     0,   125,
       0,   929,     0,     0,    15,     0,     0,    18,     0,   386,
       0,     0,   386,     0,     0,     0,    24,     0,    25,   833,
       0,   125,    29,     0,     0,     0,    32,     0,  -180,     0,
       0,     0,     0,   125,     0,   125,     0,     0,   126,   125,
       0,     0,    41,     0,     0,     0,    44,     0,   834,  -180,
     121,   835,    48,     0,     0,   121,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   121,
      61,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,   121,     0,   273,     0,     0,     0,     0,     0,   600,
     121,     0,     0,     0,   121,   125,     0,     0,   121,     0,
     125,     0,     0,   126,     0,   126,     0,     0,   126,     0,
       0,   126,     0,     0,   125,     0,     0,     0,     0,     0,
     126,     0,    77,   126,   274,     0,   125,     0,   275,     0,
     914,     0,     0,     0,   121,   125,     0,     0,     0,   125,
     141,     0,     0,   125,     0,   126,   141,     0,     0,     0,
       0,     0,   141,   141,     0,     0,     0,   126,     0,   126,
     184,     0,   187,   126,     0,     0,     0,     0,     0,   277,
       0,   278,   279,   280,     0,   281,   282,   283,     0,   125,
     285,     0,   121,   121,   121,   121,   291,   292,   230,     0,
       0,   295,     0,     0,   296,   297,   298,     0,     0,     0,
       0,   121,   121,     0,     0,     0,     0,   686,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   126,
       0,     0,     0,     0,   126,     0,     0,   125,   125,   125,
     125,   260,     0,   261,     0,     0,     0,     0,   126,     0,
       0,     0,     0,     0,     0,     0,   125,   125,     0,     0,
     126,     0,     0,     0,     0,     0,     0,     0,     0,   126,
       0,     0,     0,   126,     0,   725,     0,   126,     0,     0,
       0,     0,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,     0,   230,     0,     0,
       0,     0,     0,   335,     0,     0,     0,     0,   230,     0,
     343,     0,     0,   126,     0,     0,     0,     0,     0,     0,
     335,     0,     0,   378,   313,   314,  -425,     0,     0,  -450,
       0,  -450,     0,     0,     0,     0,     0,   335,   141,     0,
     386,   391,   386,     0,     0,   386,     0,     0,   386,     0,
       0,     0,     0,     0,     0,     0,   335,   799,     0,     0,
     801,   126,   126,   126,   126,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,     0,
     126,   126,   815,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   821,     0,   825,     0,     0,     0,
      84,     0,   187,   187,   187,   187,   857,   313,   314,     0,
       0,     0,  -450,     0,  -450,     0,     0,     0,     0,   458,
       0,     0,     0,  -166,     0,     0,  -166,  -166,     0,     0,
       0,     0,     0,     0,     0,  -166,     0,  -166,  -166,     0,
       0,  -166,     0,     0,     0,  -166,     0,  -166,     0,     0,
       0,     0,   335,   335,     0,     0,     0,     0,     0,     0,
       0,  -166,     0,  -166,     0,  -166,     0,  -166,  -166,     0,
    -166,  -166,     0,  -166,     0,   868,     0,     0,     0,   523,
       0,     0,     0,     0,     0,     0,     0,   890,     0,  -166,
       0,  -166,     0,     0,  -166,     0,   895,     0,     0,     0,
     897,     0,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,     0,     0,     0,     0,
       0,     0,   187,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   335,     0,     0,     0,
     920,  -166,     0,     0,     0,     0,     0,     0,     0,  -166,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   606,
       0,     0,   610,     0,   187,     0,     0,    -2,     4,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,     0,     0,    15,    16,    17,    18,  -295,
      19,    20,    21,     0,    22,    23,     0,    24,     0,    25,
      26,    27,    28,    29,    30,     0,    31,    32,     0,  -180,
      33,    34,    35,    36,    37,   -42,    38,    39,     0,    40,
       0,     0,     0,    41,    42,  -295,    43,    44,    45,    46,
    -180,   -42,    47,    48,     0,  -295,    49,    50,     0,    51,
      52,    53,    54,    55,    56,    57,     0,    58,   335,    59,
      60,    61,     0,    62,    63,    64,  -295,     0,     0,    65,
       0,    66,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,   695,     0,     0,    72,   378,     0,
       0,     0,     0,     0,     0,     0,    73,     0,    74,     0,
      75,    76,     0,    77,     0,     0,    78,     0,     0,     0,
      79,   716,    80,     0,    81,   230,   704,     0,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   142,     0,     0,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,     0,   761,     0,     0,     0,   143,
     144,     0,   145,     0,    31,     0,     0,     0,     0,    34,
      35,     0,    37,     0,    38,    39,     0,     0,     0,     0,
       0,     0,    42,     0,    43,     0,    45,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    53,
      54,    55,    56,   146,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,     0,     0,     0,     0,     0,   803,     0,
       0,   378,     0,     0,     0,     0,     0,    68,    69,    70,
      71,     0,     0,     0,     0,    72,     0,     0,     0,     0,
       0,   716,     0,     0,    73,     0,    74,     0,    75,    76,
     204,     0,     0,     0,    78,     0,     0,     0,   147,   610,
      80,     0,   148,     0,     0,     4,   842,     5,     6,     7,
       8,     9,    10,     0,  -469,     0,    11,    12,    13,    14,
    -469,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
    -469,    22,    23,  -469,    24,     0,    25,    26,    27,    28,
      29,    30,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,     0,    45,    46,  -180,   -42,    47,
      48,  -469,  -295,    49,    50,  -469,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
     141,  -469,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   903,     0,     0,     0,     0,  -469,    69,  -469,  -469,
    -469,  -469,  -469,  -469,  -469,     0,  -469,  -469,  -469,  -469,
    -469,  -469,  -469,  -469,  -469,    74,  -469,  -469,  -469,     0,
      77,  -469,  -469,  -469,     0,     0,     0,    79,  -469,    80,
     242,    81,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -295,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,    30,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,    42,  -295,    43,    44,
      45,    46,  -180,   -42,    47,    48,     0,  -295,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    57,     0,    58,
       0,    59,    60,    61,     0,    62,    63,    64,  -295,     0,
       0,    65,     0,    66,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,     0,
      74,     0,    75,    76,   204,    77,     0,     0,    78,     0,
       0,     0,    79,   243,    80,     4,    81,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
       0,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,    30,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,    44,    45,    46,  -180,   -42,    47,
      48,     0,  -295,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,     0,
      77,     0,     0,    78,     0,     0,     0,    79,   415,    80,
     242,    81,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -295,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,    30,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,    42,  -295,    43,    44,
      45,    46,  -180,   -42,    47,    48,     0,  -295,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    57,     0,    58,
       0,    59,    60,    61,     0,    62,    63,    64,  -295,     0,
       0,    65,     0,    66,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,     0,
      74,     0,    75,    76,     0,    77,     0,     0,    78,     0,
       0,     0,    79,   243,    80,   829,    81,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
       0,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,    30,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,    44,    45,    46,  -180,   -42,    47,
      48,     0,  -295,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,     0,
      77,     0,     0,    78,     0,     0,     0,    79,   830,    80,
       4,    81,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -295,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,    30,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,    42,  -295,    43,    44,
      45,    46,  -180,   -42,    47,    48,     0,  -295,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    57,     0,    58,
       0,    59,    60,    61,     0,    62,    63,    64,  -295,     0,
       0,    65,     0,    66,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,     0,
      74,     0,    75,    76,     0,    77,     0,     0,    78,     0,
       0,     0,    79,   899,    80,     4,    81,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
       0,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,    30,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,   256,    45,    46,  -180,   -42,    47,
      48,     0,  -295,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,     0,
      77,     0,     0,    78,     0,     0,     0,    79,     0,    80,
       4,    81,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -295,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,    30,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,    42,  -295,    43,     0,
      45,    46,  -180,   -42,    47,    48,     0,  -295,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    57,     0,    58,
       0,    59,    60,    61,     0,    62,    63,    64,  -295,     0,
       0,    65,     0,    66,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,     0,
      74,     0,    75,    76,     0,    77,     0,     0,    78,     0,
       0,     0,    79,     0,    80,     4,    81,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
       0,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,   526,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,     0,    45,    46,  -180,   -42,    47,
      48,     0,  -295,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,     0,
      77,     0,     0,    78,     0,     0,     0,    79,     0,    80,
       4,    81,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -295,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,   530,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,    42,  -295,    43,     0,
      45,    46,  -180,   -42,    47,    48,     0,  -295,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    57,     0,    58,
       0,    59,    60,    61,     0,    62,    63,    64,  -295,     0,
       0,    65,     0,    66,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,     0,
      74,     0,    75,    76,     0,    77,     0,     0,    78,     0,
       0,     0,    79,     0,    80,     4,    81,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
       0,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,   784,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,     0,    45,    46,  -180,   -42,    47,
      48,     0,  -295,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,     0,
      77,     0,     0,    78,     0,     0,     0,    79,     0,    80,
       4,    81,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -295,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,   786,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,    42,  -295,    43,     0,
      45,    46,  -180,   -42,    47,    48,     0,  -295,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    57,     0,    58,
       0,    59,    60,    61,     0,    62,    63,    64,  -295,     0,
       0,    65,     0,    66,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,     0,
      74,     0,    75,    76,     0,    77,     0,     0,    78,     0,
       0,     0,    79,     0,    80,     4,    81,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
       0,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,   789,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,     0,    45,    46,  -180,   -42,    47,
      48,     0,  -295,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,     0,
      77,     0,     0,    78,     0,     0,     0,    79,     0,    80,
       4,    81,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -295,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,   792,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,    42,  -295,    43,     0,
      45,    46,  -180,   -42,    47,    48,     0,  -295,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    57,     0,    58,
       0,    59,    60,    61,     0,    62,    63,    64,  -295,     0,
       0,    65,     0,    66,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,     0,
      74,     0,    75,    76,     0,    77,     0,     0,    78,     0,
       0,     0,    79,     0,    80,     4,    81,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
       0,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
       0,   814,    23,     0,    24,     0,    25,    26,    27,    28,
      29,    30,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,     0,    45,    46,  -180,   -42,    47,
      48,     0,  -295,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,     0,
      77,     0,     0,    78,     0,     0,     0,    79,     0,    80,
       4,    81,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -295,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,   820,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,    42,  -295,    43,     0,
      45,    46,  -180,   -42,    47,    48,     0,  -295,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    57,     0,    58,
       0,    59,    60,    61,     0,    62,    63,    64,  -295,     0,
       0,    65,     0,    66,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,     0,
      74,     0,    75,    76,     0,    77,     0,     0,    78,     0,
       0,     0,    79,     0,    80,     4,    81,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
       0,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,   824,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,     0,    45,    46,  -180,   -42,    47,
      48,     0,  -295,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     5,     6,     7,
       8,     9,    10,    73,     0,    74,   142,    75,    76,    14,
      77,     0,     0,    78,     0,     0,     0,    79,     0,    80,
       0,    81,    23,     0,     0,     0,     0,     0,   143,   144,
       0,   145,     0,    31,     0,     0,     0,     0,    34,    35,
       0,    37,     0,    38,    39,     0,     0,     0,     0,     0,
       0,    42,     0,    43,     0,    45,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    53,    54,
      55,    56,   146,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,   204,
       0,     0,     0,    78,     0,     0,     0,   147,     0,    80,
       0,   148,   599,     5,     6,     7,     8,     9,    10,     0,
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
       0,    74,     0,    75,    76,   204,     0,     0,     0,    78,
       0,     0,     0,   147,     0,    80,     0,   148,   923,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   142,     0,
       0,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     0,     0,     0,
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
      76,     0,     0,     0,     0,    78,     0,     0,     0,   147,
       0,    80,     0,   148,   604,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   142,     0,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,   208,   209,     0,   247,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,   248,     0,     0,     0,     0,     0,     0,
       0,    73,     0,    74,     0,    75,    76,   204,     0,     0,
       0,    78,     0,     0,     0,   147,     0,    80,     0,   148,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   142,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,   208,
     209,     0,   247,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,    73,     0,    74,     0,
      75,    76,   204,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,   593,   148,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   142,     0,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,   208,   209,     0,   247,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     0,     0,     0,     0,     0,
       0,    73,     0,    74,     0,    75,    76,   204,     0,     0,
       0,    78,     0,     0,     0,   147,     0,    80,   595,   148,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   142,
       0,     0,    14,     0,     0,     0,     0,     0,     0,   497,
       0,     0,     0,     0,     0,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,   498,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,   499,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,   500,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,   208,
     209,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,   210,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,   208,
     209,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,    73,     0,    74,     0,
      75,    76,   204,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   142,     0,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,   706,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,   208,   209,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     5,     6,     7,     8,     9,
      10,    73,     0,    74,   142,    75,    76,    14,     0,     0,
       0,    78,     0,     0,     0,   147,     0,    80,     0,   148,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
     173,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,   174,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     5,     6,     7,     8,     9,
      10,    73,     0,    74,   142,    75,    76,    14,     0,     0,
       0,    78,     0,   178,     0,   147,     0,    80,     0,   148,
      23,     0,     0,     0,     0,   179,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     5,     6,     7,     8,     9,
      10,    73,     0,    74,   142,    75,    76,    14,     0,     0,
       0,    78,     0,     0,     0,   147,     0,    80,     0,   148,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,   208,   209,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     5,     6,     7,     8,     9,
      10,    73,     0,    74,   142,    75,    76,    14,     0,     0,
       0,    78,     0,     0,     0,   147,     0,    80,     0,   148,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     0,     0,     0,     0,     0,
       0,    73,     0,    74,     0,    75,    76,   204,     0,     0,
       0,    78,     0,     0,     0,   147,   591,    80,     0,   148,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   142,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   190,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
      79,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,    73,     0,    74,     0,
      75,    76,   204,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   142,     0,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,   174,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     5,     6,     7,     8,     9,
      10,    73,     0,    74,   142,    75,    76,   871,     0,     0,
       0,    78,     0,     0,     0,   147,     0,    80,     0,   148,
     872,     0,     0,     0,     0,     0,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,   873,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   874,   875,    55,    56,
     876,     0,     0,     0,     0,     0,     0,     0,     0,   877,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     0,     0,     0,     0,     0,
       0,    73,     0,    74,     0,    75,    76,   204,     0,     0,
       0,    78,     0,     0,     0,   147,     0,    80,     0,   878,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   142,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,    73,     0,    74,     0,
      75,    76,   204,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   878,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   142,     0,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   856,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     0,     0,     0,     0,     0,
       0,    73,     0,    74,     0,    75,    76,   204,     0,     0,
       0,    78,     0,     0,     0,   147,     0,    80,     0,   148,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   142,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
      79,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,   563,     0,     0,     0,   536,     0,     0,     0,
     147,     0,    80,     0,   148,   564,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,   565,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   566,   567,    55,    56,   568,     0,     0,     0,     0,
       0,     0,     0,     0,   569,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   570,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   570,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   649,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   652,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   850,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   851,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   853,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   854,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   855,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   856,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     142,     0,     0,    14,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,    23,     0,     0,     0,
       0,     0,   143,   144,     0,   145,    73,    31,    74,     0,
       0,    76,    34,    35,     0,    37,    78,    38,    39,     0,
     147,     0,    80,     0,   148,    42,     0,    43,     0,    45,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    54,    55,    56,   146,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -326,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,     0,  -326,     0,     0,     0,  -326,
       0,     0,     5,     6,     7,     8,     9,    10,     0,    74,
       0,   142,     0,   204,    14,     0,     0,     0,     0,     0,
       0,   147,     0,    80,  -326,   878,     0,    23,     0,     0,
       0,     0,     0,   143,   144,     0,   145,     0,    31,     0,
       0,     0,     0,    34,    35,     0,    37,     0,    38,    39,
       0,     0,     0,     0,     0,     0,    42,     0,    43,     0,
      45,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    53,    54,    55,    56,   146,     0,     5,
       6,     7,     8,     9,    10,     0,    63,     0,   142,     0,
       0,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     0,     0,     0,
     143,   144,    69,   145,     0,    31,     0,     0,     0,     0,
      34,    35,     0,    37,     0,    38,    39,     0,     0,     0,
      74,     0,     5,    42,     0,    43,     0,    45,     0,   736,
       0,     0,   147,     0,    80,   737,   570,     0,     0,     0,
      53,    54,    55,    56,   146,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     0,     0,     0,     0,     0,     0,   736,     0,    69,
       0,     0,     0,   737,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,   148,     0,     0,     0,   358,   359,   360,
     361,   362,   363,     0,     0,   364,   365,   366,   367,   368,
     369,   738,   739,   740,   741,     0,     0,   742,     0,     0,
       0,   743,   744,   745,   746,   747,   748,   749,   750,   751,
     752,   753,     0,   754,     0,     0,   755,   756,   757,   758,
       0,   759,     0,     0,   760,   358,   359,   360,   361,   362,
     363,     0,     0,   364,   365,   366,   367,   368,   369,   738,
     739,   740,   741,     0,     0,   742,     0,     0,     0,   743,
     744,   745,   746,   747,   748,   749,   750,   751,   752,   753,
       0,   754,   271,     0,   755,   756,   757,   758,   272,   759,
       0,     0,     0,     0,     0,     0,     0,     0,   273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     271,     0,     0,     0,     0,     0,   272,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   273,     0,     0,   274,
       0,     0,     0,   275,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   271,     0,
       0,     0,     0,     0,   272,   351,     0,     0,     0,   276,
       0,     0,     0,     0,   273,     0,     0,   274,     0,     0,
       0,   275,     0,     0,   277,     0,   278,   279,   280,     0,
     281,   282,   283,     0,   284,   285,   286,   287,   288,   289,
     290,   291,   292,     0,   293,   294,   295,   276,     0,   296,
     297,   298,     0,     0,     0,   274,     0,     0,     0,   275,
     723,     0,   277,     0,   278,   279,   280,     0,   281,   282,
     283,     0,   284,   285,   286,   287,   288,   289,   290,   291,
     292,     0,   293,   294,   295,   276,     0,   296,   297,   298,
       0,     0,     0,   271,     0,     0,     0,     0,   778,   272,
     277,     0,   278,   279,   280,     0,   281,   282,   283,   273,
     284,   285,   286,   287,   288,   289,   290,   291,   292,     0,
     293,   294,   295,     0,     0,   296,   297,   298,     0,     0,
       0,   271,     0,     0,     0,     0,   932,   272,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   273,     0,     0,
     274,     0,     0,     0,   275,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   271,
       0,     0,     0,     0,     0,   272,     0,     0,     0,     0,
     276,     0,     0,     0,     0,   273,     0,     0,   274,     0,
       0,     0,   275,     0,     0,   277,     0,   278,   279,   280,
       0,   281,   282,   283,     0,   284,   285,   286,   287,   288,
     289,   290,   291,   292,     0,   293,   294,   295,   276,     0,
     296,   297,   298,     0,     0,     0,   274,     0,     0,     0,
     275,   969,     0,   277,     0,   278,   279,   280,     0,   281,
     282,   283,     0,   284,   285,   286,   287,   288,   289,   290,
     291,   292,     0,   293,   294,   295,   276,     0,   296,   297,
     298,     0,     0,     0,   271,     0,     0,   592,     0,     0,
     272,   277,     0,   278,   279,   280,     0,   281,   282,   283,
     273,   284,   285,   286,   287,   288,   289,   290,   291,   292,
       0,   293,   294,   295,     0,     0,   296,   297,   298,     0,
       0,     0,     0,     0,     0,   902,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   274,     0,     0,     0,   275,     0,     0,     0,     0,
       0,     0,     0,   357,     0,     0,     0,     0,     0,     0,
       0,     0,   271,     0,     0,     0,     0,     0,   272,     0,
       0,   276,   358,   359,   360,   361,   362,   363,   273,     0,
     364,   365,   366,   367,   368,   369,   277,     0,   278,   279,
     280,     0,   281,   282,   283,     0,   284,   285,   286,   287,
     288,   289,   290,   291,   292,     0,   293,   294,   295,     0,
       0,   296,   297,   298,     0,     0,     0,   326,     0,   274,
       0,     0,     0,   275,     0,     0,     0,     0,     0,     0,
       0,   783,     0,     0,     0,     0,     0,     0,     0,     0,
     271,     0,     0,     0,     0,     0,   272,     0,     0,   276,
     358,   359,   360,   361,   362,   363,   273,     0,   364,   365,
     366,   367,   368,   369,   277,     0,   278,   279,   280,     0,
     281,   282,   283,     0,   284,   285,   286,   287,   288,   289,
     290,   291,   292,     0,   293,   294,   295,     0,     0,   296,
     297,   298,     0,     0,     0,   326,     0,   274,     0,     0,
       0,   275,     0,     0,     0,     0,     0,     0,     0,   788,
       0,     0,     0,     0,     0,     0,     0,     0,   271,     0,
       0,     0,     0,     0,   272,     0,     0,   276,   358,   359,
     360,   361,   362,   363,   273,     0,   364,   365,   366,   367,
     368,   369,   277,     0,   278,   279,   280,     0,   281,   282,
     283,     0,   284,   285,   286,   287,   288,   289,   290,   291,
     292,     0,   293,   294,   295,     0,     0,   296,   297,   298,
       0,     0,     0,   326,     0,   274,     0,     0,     0,   275,
       0,     0,     0,     0,     0,     0,     0,   915,     0,     0,
       0,     0,     0,     0,     0,     0,   271,     0,     0,     0,
       0,     0,   272,     0,     0,   276,   358,   359,   360,   361,
     362,   363,   273,     0,   364,   365,   366,   367,   368,   369,
     277,     0,   278,   279,   280,     0,   281,   282,   283,     0,
     284,   285,   286,   287,   288,   289,   290,   291,   292,     0,
     293,   294,   295,     0,     0,   296,   297,   298,     0,     0,
       0,   326,     0,   274,     0,     0,     0,   275,     0,     0,
       0,     0,     0,     0,     0,   916,     0,     0,     0,     0,
       0,     0,     0,     0,   271,     0,     0,     0,     0,     0,
     272,     0,     0,   276,   358,   359,   360,   361,   362,   363,
     273,     0,   364,   365,   366,   367,   368,   369,   277,     0,
     278,   279,   280,     0,   281,   282,   283,     0,   284,   285,
     286,   287,   288,   289,   290,   291,   292,     0,   293,   294,
     295,     0,     0,   296,   297,   298,     0,     0,     0,   326,
       0,   274,     0,     0,     0,   275,     0,     0,     0,     0,
       0,     0,     0,   917,     0,     0,     0,     0,     0,     0,
       0,     0,   271,     0,     0,     0,     0,     0,   272,     0,
       0,   276,   358,   359,   360,   361,   362,   363,   273,     0,
     364,   365,   366,   367,   368,   369,   277,     0,   278,   279,
     280,     0,   281,   282,   283,     0,   284,   285,   286,   287,
     288,   289,   290,   291,   292,     0,   293,   294,   295,     0,
       0,   296,   297,   298,     0,     0,     0,   326,     0,   274,
       0,     0,     0,   275,     0,     0,     0,     0,     0,     0,
       0,   918,     0,     0,     0,     0,     0,     0,     0,     0,
     271,     0,     0,     0,     0,     0,   272,     0,     0,   276,
     358,   359,   360,   361,   362,   363,   273,     0,   364,   365,
     366,   367,   368,   369,   277,     0,   278,   279,   280,     0,
     281,   282,   283,     0,   284,   285,   286,   287,   288,   289,
     290,   291,   292,     0,   293,   294,   295,     0,     0,   296,
     297,   298,     0,     0,     0,   326,     0,   274,     0,     0,
       0,   275,     0,     0,     0,     0,     0,     0,     0,   934,
       0,     0,     0,     0,     0,     0,     0,     0,   271,     0,
       0,     0,     0,     0,   272,     0,     0,   276,   358,   359,
     360,   361,   362,   363,   273,     0,   364,   365,   366,   367,
     368,   369,   277,     0,   278,   279,   280,     0,   281,   282,
     283,     0,   284,   285,   286,   287,   288,   289,   290,   291,
     292,     0,   293,   294,   295,     0,     0,   296,   297,   298,
       0,     0,     0,   326,     0,   274,     0,     0,     0,   275,
       0,     0,     0,     0,     0,     0,     0,   935,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   276,   358,   359,   360,   361,
     362,   363,     0,     0,   364,   365,   366,   367,   368,   369,
     277,     0,   278,   279,   280,     0,   281,   282,   283,     0,
     284,   285,   286,   287,   288,   289,   290,   291,   292,     0,
     293,   294,   295,   271,     0,   296,   297,   298,     0,   272,
       0,   326,     0,     0,     0,     0,     0,     0,     0,   273,
     349,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   350,     0,     0,     0,     0,     0,   271,     0,     0,
       0,     0,     0,   272,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   273,   346,     0,     0,     0,     0,     0,
     274,     0,     0,     0,   275,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,   271,     0,     0,
     276,     0,     0,   272,   274,     0,     0,     0,   275,     0,
       0,     0,     0,   273,   190,   277,     0,   278,   279,   280,
       0,   281,   282,   283,     0,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   276,   293,   294,   295,     0,     0,
     296,   297,   298,     0,     0,     0,   326,     0,     0,   277,
       0,   278,   279,   280,   274,   281,   282,   283,   275,   284,
     285,   286,   287,   288,   289,   290,   291,   292,     0,   293,
     294,   295,   271,     0,   296,   297,   298,     0,   272,     0,
     326,     0,     0,     0,   276,     0,     0,     0,   273,   675,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,   278,   279,   280,     0,   281,   282,   283,     0,   284,
     285,   286,   287,   288,   289,   290,   291,   292,     0,   293,
     294,   295,   271,     0,   296,   297,   298,     0,   272,   274,
     326,     0,     0,   275,     0,     0,     0,     0,   273,   669,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   351,   271,     0,     0,   276,
       0,     0,   272,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   273,     0,   277,     0,   278,   279,   280,   274,
     281,   282,   283,   275,   284,   285,   286,   287,   288,   289,
     290,   291,   292,     0,   293,   294,   295,     0,     0,   296,
     297,   298,     0,     0,     0,   326,   271,     0,     0,   276,
       0,     0,   272,   274,     0,     0,     0,   275,     0,     0,
       0,     0,   273,     0,   277,   687,   278,   279,   280,     0,
     281,   282,   283,     0,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   276,   293,   294,   295,     0,     0,   296,
     297,   298,     0,     0,     0,   326,     0,     0,   277,     0,
     278,   279,   280,   274,   281,   282,   283,   275,   284,   285,
     286,   287,   288,   289,   290,   291,   292,     0,   293,   294,
     295,   271,     0,   296,   297,   298,     0,   272,     0,   326,
       0,     0,     0,   276,     0,     0,     0,   273,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
     278,   279,   280,     0,   281,   282,   283,     0,   284,   285,
     286,   287,   288,   289,   290,   291,   292,     0,   293,   294,
     295,   271,     0,   296,   297,   298,     0,   272,   274,   397,
       0,     0,   275,     0,     0,     0,     0,   273,     0,     0,
       0,     0,   354,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   276,     0,
       0,     0,     0,     0,   355,     0,     0,     0,     0,     0,
       0,     0,     0,   277,     0,   278,   279,   280,   274,   281,
     282,   283,   275,   284,   285,   286,   287,   288,   289,   290,
     291,   292,     0,   293,   294,   295,   271,     0,   296,   297,
     298,     0,   272,  -426,     0,     0,     0,     0,   276,     0,
       0,     0,   273,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   277,   328,   278,   279,   280,     0,   281,
     282,   283,     0,   284,   285,   286,   287,   288,   289,   290,
     291,   292,     0,   293,   294,   295,   271,   356,   296,   297,
     298,     0,   272,   274,     0,     0,     0,   275,     0,     0,
       0,     0,   273,     0,     0,     0,     0,   410,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   137,
       0,     0,     0,   276,     0,     0,     0,     0,     0,   411,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
     278,   279,   280,   274,   281,   282,   283,   275,   284,   285,
     286,   287,   288,   289,   290,   291,   292,     0,   293,   294,
     295,   271,     0,   296,   297,   298,     0,   272,     0,     0,
       0,     0,     0,   276,     0,     0,     0,   273,   506,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,   507,
     278,   279,   280,     0,   281,   282,   283,     0,   284,   285,
     286,   287,   288,   289,   290,   291,   292,     0,   293,   294,
     295,   271,     0,   296,   297,   298,     0,   272,   274,     0,
       0,     0,   275,     0,     0,     0,     0,   273,   508,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   509,
       0,     0,     0,     0,     0,   271,     0,     0,   276,     0,
       0,   272,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   273,     0,   277,     0,   278,   279,   280,   274,   281,
     282,   283,   275,   284,   285,   286,   287,   288,   289,   290,
     291,   292,     0,   293,   294,   295,     0,     0,   296,   297,
     298,   271,     0,     0,     0,     0,     0,   272,   276,     0,
       0,     0,   274,     0,     0,     0,   275,   273,     0,     0,
       0,     0,     0,   277,     0,   278,   279,   280,     0,   281,
     282,   283,     0,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   276,   293,   294,   295,     0,     0,   296,   297,
     298,     0,     0,     0,     0,     0,     0,   277,   274,   278,
     279,   280,   275,   281,   282,   283,     0,   284,   285,   286,
     287,   288,   289,   290,   291,   292,     0,   293,   294,   295,
     271,   401,   296,   297,   298,     0,   272,     0,   276,     0,
       0,     0,     0,     0,     0,     0,   273,     0,     0,     0,
       0,     0,     0,   277,     0,   278,   279,   280,     0,   281,
     282,   283,     0,   284,   285,   286,   287,   288,   289,   290,
     291,   292,     0,   293,   294,   295,   271,   403,   296,   297,
     298,     0,   272,     0,     0,     0,     0,   274,     0,     0,
       0,   275,   273,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   276,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,   274,   278,   279,   280,   275,   281,   282,
     283,     0,   284,   285,   286,   287,   288,   289,   290,   291,
     292,     0,   293,   294,   295,   271,   405,   296,   297,   298,
       0,   272,     0,   276,     0,     0,     0,     0,     0,     0,
       0,   273,     0,     0,     0,     0,     0,     0,   277,     0,
     278,   279,   280,     0,   281,   282,   283,     0,   284,   285,
     286,   287,   288,   289,   290,   291,   292,     0,   293,   294,
     295,     0,   413,   296,   297,   298,     0,     0,     0,     0,
       0,     0,   274,     0,     0,     0,   275,     0,     0,   271,
       0,     0,     0,     0,     0,   272,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   273,     0,     0,     0,     0,
       0,   428,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,     0,   278,
     279,   280,     0,   281,   282,   283,     0,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   274,   293,   294,   295,
     275,     0,   296,   297,   298,     0,     0,     0,   510,     0,
       0,     0,   271,     0,     0,     0,     0,     0,   272,     0,
       0,     0,     0,     0,     0,     0,   276,     0,   273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,   278,   279,   280,     0,   281,   282,   283,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
       0,   293,   294,   295,     0,     0,   296,   297,   298,   274,
       0,     0,     0,   275,     0,     0,   271,     0,     0,     0,
       0,     0,   272,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   273,     0,     0,   137,     0,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,   278,   279,   280,     0,
     281,   282,   283,     0,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   274,   293,   294,   295,   275,     0,   296,
     297,   298,     0,     0,     0,     0,     0,     0,     0,   271,
     682,     0,     0,     0,     0,   272,     0,     0,     0,     0,
       0,     0,     0,   276,     0,   273,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
     278,   279,   280,     0,   281,   282,   283,     0,   284,   285,
     286,   287,   288,   289,   290,   291,   292,     0,   293,   294,
     295,     0,   659,   296,   297,   298,   274,     0,     0,     0,
     275,     0,     0,   271,     0,     0,     0,     0,     0,   272,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   273,
       0,     0,   685,     0,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,   278,   279,   280,     0,   281,   282,   283,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     274,   293,   294,   295,   275,     0,   296,   297,   298,     0,
       0,     0,   271,     0,     0,     0,     0,     0,   272,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   273,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,     0,   278,   279,   280,
       0,   281,   282,   283,     0,   284,   285,   286,   287,   288,
     289,   290,   291,   292,     0,   293,   294,   295,     0,   274,
     296,   297,   298,   275,     0,     0,   271,     0,     0,     0,
       0,     0,   272,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   273,   773,     0,     0,     0,     0,   726,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,   278,   279,   280,     0,
     281,   282,   283,     0,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   274,   293,   294,   295,   275,     0,   296,
     297,   298,     0,     0,     0,   271,     0,     0,     0,     0,
       0,   272,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   273,   776,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
     278,   279,   280,     0,   281,   282,   283,     0,   284,   285,
     286,   287,   288,   289,   290,   291,   292,     0,   293,   294,
     295,     0,   274,   296,   297,   298,   275,     0,     0,   271,
       0,     0,     0,     0,     0,   272,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   273,     0,     0,     0,     0,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,     0,   278,
     279,   280,     0,   281,   282,   283,     0,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   274,   293,   294,   295,
     275,     0,   296,   297,   298,     0,     0,     0,   849,     0,
       0,     0,   271,     0,     0,     0,     0,     0,   272,     0,
       0,     0,     0,     0,     0,     0,   276,     0,   273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,   278,   279,   280,     0,   281,   282,   283,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
       0,   293,   294,   295,     0,     0,   296,   297,   298,   274,
       0,     0,     0,   275,     0,     0,   271,   866,     0,     0,
       0,   852,   272,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   273,     0,     0,     0,     0,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,   278,   279,   280,     0,
     281,   282,   283,     0,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   274,   293,   294,   295,   275,     0,   296,
     297,   298,     0,     0,     0,   271,     0,     0,     0,     0,
       0,   272,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   273,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
     278,   279,   280,     0,   281,   282,   283,     0,   284,   285,
     286,   287,   288,   289,   290,   291,   292,     0,   293,   294,
     295,     0,   274,   296,   297,   298,   275,     0,     0,   271,
       0,     0,     0,     0,   941,   272,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   273,     0,     0,     0,     0,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,     0,   278,
     279,   280,     0,   281,   282,   283,     0,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   274,   293,   294,   295,
     275,     0,   296,   297,   298,     0,     0,     0,   942,     0,
       0,     0,   271,     0,     0,     0,     0,     0,   272,     0,
       0,     0,     0,     0,     0,     0,   276,     0,   273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,   278,   279,   280,     0,   281,   282,   283,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
       0,   293,   294,   295,     0,     0,   296,   297,   298,   274,
       0,     0,     0,   275,     0,     0,   271,     0,     0,     0,
       0,   943,   272,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   273,     0,     0,     0,     0,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,   278,   279,   280,     0,
     281,   282,   283,     0,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   274,   293,   294,   295,   275,     0,   296,
     297,   298,     0,     0,     0,   944,     0,     0,     0,   271,
       0,     0,     0,     0,     0,   272,     0,     0,     0,     0,
       0,     0,     0,   276,     0,   273,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
     278,   279,   280,     0,   281,   282,   283,     0,   284,   285,
     286,   287,   288,   289,   290,   291,   292,     0,   293,   294,
     295,     0,     0,   296,   297,   298,   274,     0,     0,     0,
     275,     0,     0,   271,     0,     0,     0,     0,   945,   272,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   273,
       0,     0,     0,     0,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,   278,   279,   280,     0,   281,   282,   283,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     274,   293,   294,   295,   275,     0,   296,   297,   298,     0,
       0,     0,   946,     0,     0,     0,   271,     0,     0,     0,
       0,     0,   272,     0,     0,     0,     0,     0,     0,     0,
     276,     0,   273,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,     0,   278,   279,   280,
       0,   281,   282,   283,     0,   284,   285,   286,   287,   288,
     289,   290,   291,   292,     0,   293,   294,   295,     0,     0,
     296,   297,   298,   274,     0,     0,     0,   275,     0,     0,
     271,     0,     0,     0,     0,     0,   272,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   273,     0,     0,     0,
       0,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
     278,   279,   280,     0,   281,   282,   283,     0,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   647,   293,   294,
     295,   275,   273,   296,   297,   298,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   276,     0,     0,
       0,   273,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,   274,   278,   279,   280,   275,   281,   282,
     283,     0,   284,   285,   286,   287,   288,   289,   290,   291,
     292,     0,   293,   294,   295,     0,   273,   296,   297,   298,
       0,     0,   274,   276,     0,     0,   275,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
     278,   279,   280,     0,   281,   282,   283,     0,   284,   285,
     286,   287,   276,   289,   290,   291,   292,   274,   293,   294,
     295,   275,   273,   296,   297,   298,     0,   277,     0,   278,
     279,   280,     0,   281,   282,   283,     0,   284,   285,   286,
     287,     0,   289,   290,   291,   292,     0,   293,     0,   295,
       0,     0,   296,   297,   298,     0,     0,     0,     0,     0,
       0,     0,   277,   274,   278,   279,   280,   275,   281,   282,
     283,     0,   284,   285,   286,   287,     0,   289,   290,   291,
     292,     0,   293,     0,   295,     0,     0,   296,   297,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
     278,   279,   280,     0,   281,   282,   283,     0,     0,   285,
     286,   287,     0,   289,   290,   291,   292,     0,     0,     0,
     295,     0,     0,   296,   297,   298
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-855)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       2,     2,   160,    46,    47,   374,   534,   406,   382,    11,
     319,   618,    14,   171,    49,    17,    16,    80,    20,    21,
      22,   377,   253,   703,   562,    27,    28,    29,    30,    64,
       3,     3,   406,    25,    36,    37,    38,     1,    40,    27,
      42,    43,   183,     1,     3,    27,    81,    49,     1,    51,
      52,    53,    54,     1,     3,    57,    58,    59,    60,   200,
       3,    63,    64,    65,    66,    27,     3,    69,     1,   355,
      72,    73,    74,    22,    76,   764,    27,    79,   219,    81,
      27,    83,   265,   266,   267,   268,    18,   923,   768,    38,
      54,   729,    41,    70,   886,    68,    93,   803,    38,   117,
      38,    13,   136,   166,   958,    38,    55,    19,   646,    58,
     117,    60,    70,   148,    38,   969,   644,    70,    38,    68,
     183,    38,   958,    81,    54,   411,    90,   807,    70,   117,
     148,    93,     3,    91,    66,    84,   138,   200,    91,   146,
     142,   143,   144,   145,   146,   147,   148,     0,    85,     3,
      98,   148,    93,     7,    86,   143,   219,    28,   116,   159,
      90,   143,   700,   116,   870,    98,   143,     3,   116,   117,
     962,   143,   145,   145,   315,   316,    93,   117,   136,   117,
     144,   143,   184,   116,   117,   143,   145,    22,   190,   869,
     143,     3,   143,   117,   136,     7,   145,   117,   146,   837,
     117,   143,   145,   136,    38,   561,   208,   209,   145,   201,
     148,    38,   117,    84,   144,   904,   905,    98,   117,   117,
     858,    26,    98,    58,   148,   534,   117,   117,   148,    84,
      42,   148,   117,    68,    68,   145,   117,   844,    42,   144,
     121,    68,   922,   245,   245,   121,   248,   146,   146,    84,
     145,    63,   315,   316,   437,   146,   146,     7,   399,    63,
      77,   146,    67,   145,    22,   146,    71,   795,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     117,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   136,    26,   145,
      58,   532,   533,   841,   117,   144,   136,   144,   117,   344,
      68,   116,   136,   118,   119,   317,   318,   319,   123,    64,
     144,   117,   117,   692,   326,   130,   328,   274,   275,   392,
     332,    89,   334,   146,   139,   644,   399,   146,   694,    67,
     136,   142,   344,    71,   346,   347,   136,   349,   350,    98,
      22,   146,   354,   355,   417,   357,   117,   420,    52,   422,
     729,   145,   425,   147,    50,     2,   597,   598,   117,   371,
     317,   318,   121,     3,    11,   136,   145,     7,   147,     9,
     382,   328,   384,    20,   136,    22,    58,     1,   116,     3,
     392,   119,    22,     2,    80,   123,    68,   146,   433,   117,
     347,    67,    11,   350,   406,    71,     3,   136,   410,   411,
       7,    20,    84,    22,   813,   417,   117,   117,   136,   136,
      57,   117,   424,   425,   426,    22,   428,   429,    58,   136,
     588,   433,   806,   117,   117,   136,   136,   136,    68,   813,
     136,   117,    79,    67,    59,    60,    83,    71,    57,   136,
      59,    60,   136,   136,    84,   136,   519,   145,   521,    89,
     136,    58,   116,   119,   145,   119,   147,   136,   143,   123,
      79,    68,    92,   136,    83,   145,   845,   424,   145,    98,
     147,   175,   176,   177,   145,   145,   795,    84,   136,   858,
       7,    48,    89,    26,   420,   116,   422,   143,   136,   193,
      98,   138,   196,   146,   506,   507,   508,   509,   510,   511,
     512,   117,    38,   513,   514,    29,   145,   117,   136,   146,
     146,   215,   524,    98,   526,    17,   117,    29,   530,   138,
     146,   148,   534,   146,    67,   570,   143,   136,    71,   117,
     136,   235,   544,   136,   136,   146,   136,   184,   136,   923,
     136,    26,   136,   190,   136,   136,   136,   136,   932,    21,
     507,   563,   509,   565,   566,   567,   568,   569,   570,   136,
      32,    33,   136,   136,   136,   184,   148,   136,    40,   112,
      42,   190,   115,   116,   958,   118,   119,   136,   136,   143,
     123,   146,    67,   144,    56,   969,    71,   130,   136,   146,
     602,    63,   136,   144,   137,   138,   139,   117,   245,    98,
      68,    27,    27,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,   314,   148,
     143,    29,   116,    68,   116,   329,   245,   112,   144,   114,
     115,   116,   644,   118,   119,   647,   648,   649,   123,    26,
     652,   148,   148,   148,   119,   130,   119,   145,    29,   353,
      22,   146,   137,   138,   139,   667,    91,   669,   146,    81,
     672,   144,   727,   675,   411,   710,    38,   410,   680,    41,
     682,   227,   684,   685,   714,   687,   172,   781,   732,   326,
      67,   523,   844,    55,    71,   845,    58,   692,    60,   735,
     647,   648,   554,   962,   922,   937,    68,   709,   710,   346,
      29,   385,   349,   521,    -1,    -1,    -1,   326,    -1,   721,
     357,   723,    84,    -1,   726,   727,    -1,    -1,    -1,   731,
      -1,    -1,    -1,    -1,   420,   112,   422,   346,    -1,   116,
     349,   118,   119,   806,    -1,    -1,   123,    -1,   357,    -1,
      -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,   760,    -1,
     137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    22,
     772,   773,    -1,   775,   776,   777,   778,    -1,    -1,    -1,
      -1,   783,   784,    -1,   786,    38,   788,   789,    41,   426,
     792,   834,   835,   795,    -1,    -1,    -1,    -1,    -1,    -1,
     802,    -1,    55,    -1,   806,    58,    -1,    60,    -1,    -1,
     812,   813,   814,    -1,    -1,    68,   818,   426,   820,   882,
      -1,   823,   824,    -1,    -1,   827,    -1,    -1,    -1,   831,
     831,    84,    -1,   519,   528,   529,    -1,    -1,   532,   533,
      -1,    -1,    -1,   878,    -1,    -1,    -1,   849,   850,   851,
     852,   853,   854,   855,   856,    -1,    -1,    -1,    -1,    -1,
     923,    -1,    -1,    -1,   866,   867,   503,    -1,    -1,   871,
      -1,   873,   874,   875,   876,   877,   878,    -1,    -1,    -1,
     882,    -1,    -1,   885,    -1,    -1,    -1,   573,    -1,    -1,
      -1,     2,    -1,    -1,    -1,   958,    -1,    -1,    -1,    -1,
      11,    -1,    -1,    -1,    -1,    -1,   969,   544,    -1,    20,
      -1,    22,    26,   915,   916,   917,   918,    -1,    -1,    -1,
      -1,   923,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     932,    -1,   934,   935,    -1,   544,    -1,    -1,    -1,   941,
     942,   943,   944,   945,   946,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    -1,   957,   958,    71,    -1,    -1,
      -1,    -1,    -1,   657,   658,   602,    -1,   969,    79,    -1,
     664,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     674,   618,    -1,   677,    -1,    -1,    -1,    -1,     2,    -1,
      -1,    -1,    -1,   602,    -1,    26,    -1,    11,   112,    -1,
     114,   115,   116,    -1,   118,   119,    20,    -1,    22,   123,
     647,   648,    -1,    -1,    -1,   129,   130,    -1,    -1,    -1,
     134,    -1,    -1,   137,   138,   139,    -1,   138,    -1,    -1,
     667,    -1,   669,    -1,    -1,   672,    67,    -1,   675,    -1,
      71,    -1,    -1,    57,    -1,    -1,    -1,   684,    -1,    -1,
     687,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   667,    -1,
     669,    -1,    -1,   672,    -1,    79,   675,    -1,    -1,    83,
      -1,    -1,   709,   184,    -1,   684,    -1,    38,   687,   190,
      -1,    -1,    -1,    38,   721,   116,   723,   118,   119,    -1,
     727,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     709,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,    70,
      -1,    -1,   721,    -1,   723,    70,    -1,    -1,   727,    -1,
      81,    -1,   816,    -1,   138,    -1,    81,    -1,    -1,     2,
      91,    -1,    -1,    -1,   245,    -1,    91,    98,    11,    -1,
      -1,    -1,    -1,    98,    -1,    36,   783,    20,    -1,    22,
      -1,   788,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    52,   117,    -1,    38,   802,    -1,    -1,    59,    60,
     184,    -1,    -1,    -1,   783,   136,   190,   814,    -1,   788,
      -1,   136,   143,    -1,    57,   146,   823,    -1,   143,    -1,
     827,   146,    -1,   802,   831,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   814,    79,    81,    -1,    -1,
      83,    -1,    -1,    -1,   823,   326,    -1,    91,   827,    -1,
      -1,    21,   831,    -1,    98,    -1,    -1,    -1,    -1,    -1,
     867,   245,    32,    33,    -1,   346,    -1,    -1,   349,    -1,
      40,    -1,    42,   117,    -1,    -1,   357,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,   867,    -1,
      -1,    -1,   136,    63,    64,   138,    -1,   158,    -1,   143,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,   915,   916,
     917,   918,    -1,    -1,   175,   176,   177,    -1,    -1,    -1,
      -1,   182,    -1,    -1,    -1,    -1,    -1,   934,   935,    -1,
      -1,    -1,   193,    -1,    -1,   196,   915,   916,   917,   918,
      21,   184,   326,    -1,    -1,   426,    -1,   190,    -1,    -1,
      -1,    32,    33,    -1,   215,   934,   935,    -1,    -1,    40,
      -1,    42,   346,    -1,    -1,   349,    -1,    48,    -1,     2,
      -1,    -1,    -1,   357,   235,    56,     1,    -1,    11,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    20,    -1,    22,
      -1,    -1,    -1,    18,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,   245,    -1,    -1,    30,    -1,    32,    33,    -1,
      -1,    36,    -1,    -1,     2,    40,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    11,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    20,    58,    22,    60,    -1,    62,    63,    -1,
      65,    66,   426,    68,    -1,    -1,    79,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    86,    -1,   544,    89,    -1,    -1,    -1,   329,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   326,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,   353,    -1,    -1,    83,    -1,    -1,    18,    -1,
      -1,    21,    -1,   346,    -1,   138,   349,    -1,    -1,    -1,
      30,   136,    32,    33,   357,    -1,    36,    -1,    -1,   144,
      40,   602,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,     2,    -1,
      60,    -1,    62,    63,    -1,    65,    66,    11,    -1,    -1,
     138,   184,    -1,    -1,    -1,    -1,    20,   190,    22,    -1,
     544,    -1,    -1,    -1,    84,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   426,    -1,    -1,   667,    -1,   669,    -1,
      -1,   672,    -1,    57,   675,    -1,   184,    -1,    -1,    -1,
      -1,    -1,   190,   684,    -1,    -1,   687,    -1,    -1,    -1,
      -1,    18,   245,    -1,    21,    79,   136,    -1,   602,    83,
      -1,    -1,    -1,    30,   144,    32,    33,    -1,   709,    36,
      -1,    -1,    -1,    40,    -1,    42,    -1,    -1,    -1,    -1,
     721,    -1,   723,    -1,    -1,    -1,   727,    -1,    -1,    56,
      -1,    -1,    -1,    60,    -1,    62,    63,   245,    65,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   528,   529,    -1,
      -1,   532,   533,    -1,   138,    -1,    -1,    84,    -1,    86,
      -1,    -1,    -1,   667,   545,   669,    -1,    -1,   672,    -1,
      -1,   675,    -1,   326,    -1,    -1,    -1,    -1,    -1,    -1,
     684,   544,   783,   687,    -1,    -1,    -1,   788,    -1,    -1,
      -1,    -1,    -1,   346,    -1,    -1,   349,    -1,    -1,    -1,
     184,   802,    -1,   584,   357,   709,   190,    -1,    -1,   136,
      -1,    -1,    -1,   814,    -1,    -1,    -1,   721,   326,   723,
      -1,    -1,   823,   727,    -1,    -1,   827,    -1,    -1,    -1,
     831,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   346,   602,
      -1,   349,    -1,    -1,    -1,     2,    -1,    -1,    -1,   357,
      -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      -1,   245,    -1,    20,    -1,    22,   867,    -1,    -1,    -1,
      -1,    -1,    -1,   426,    -1,    -1,   657,   658,    -1,   783,
      -1,    -1,    -1,   664,   788,   666,    -1,    -1,    -1,    -1,
     671,    -1,    -1,   674,    -1,    -1,   677,    -1,   802,    -1,
      57,    -1,    -1,    -1,   667,    -1,   669,    -1,    -1,   672,
     814,    -1,   675,    -1,   915,   916,   917,   918,   426,   823,
      -1,   684,    79,   827,   687,    -1,    83,   831,    -1,    -1,
      -1,    -1,    -1,   934,   935,    -1,   717,    -1,    -1,    -1,
      -1,    -1,   326,    -1,    -1,    -1,   709,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   721,    -1,
     723,    -1,   346,   867,   727,   349,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   357,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   544,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   915,   916,   917,   918,    -1,    -1,    -1,    -1,    -1,
     783,    -1,    -1,    -1,    -1,   788,    -1,   184,    -1,    -1,
     934,   935,    -1,   190,    -1,   816,   544,    -1,    -1,   802,
      -1,    -1,   426,    -1,    -1,    -1,    -1,    -1,    -1,   602,
      -1,   814,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     823,    -1,    -1,    -1,   827,    -1,    -1,    -1,   831,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   860,
     861,    -1,   863,   864,    -1,    -1,    -1,    -1,   245,    -1,
      -1,    -1,    -1,    -1,   602,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   867,   886,    -1,    -1,    -1,    -1,
      -1,   892,    -1,   894,   667,   896,   669,    -1,    -1,   672,
      -1,    -1,   675,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   684,    -1,    -1,   687,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   915,   916,   917,   918,   709,    -1,    -1,   667,
     544,   669,    -1,    -1,   672,    -1,    -1,   675,   721,   326,
     723,   934,   935,    -1,   727,    -1,   684,    -1,    -1,   687,
      -1,   962,    -1,    -1,    18,    -1,    -1,    21,    -1,   346,
      -1,    -1,   349,    -1,    -1,    -1,    30,    -1,    32,    33,
      -1,   709,    36,    -1,    -1,    -1,    40,    -1,    42,    -1,
      -1,    -1,    -1,   721,    -1,   723,    -1,    -1,   602,   727,
      -1,    -1,    56,    -1,    -1,    -1,    60,    -1,    62,    63,
     783,    65,    66,    -1,    -1,   788,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   802,
      84,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   814,    -1,    26,    -1,    -1,    -1,    -1,    -1,   426,
     823,    -1,    -1,    -1,   827,   783,    -1,    -1,   831,    -1,
     788,    -1,    -1,   667,    -1,   669,    -1,    -1,   672,    -1,
      -1,   675,    -1,    -1,   802,    -1,    -1,    -1,    -1,    -1,
     684,    -1,   136,   687,    67,    -1,   814,    -1,    71,    -1,
     144,    -1,    -1,    -1,   867,   823,    -1,    -1,    -1,   827,
      13,    -1,    -1,   831,    -1,   709,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    -1,    -1,    -1,   721,    -1,   723,
      33,    -1,    35,   727,    -1,    -1,    -1,    -1,    -1,   112,
      -1,   114,   115,   116,    -1,   118,   119,   120,    -1,   867,
     123,    -1,   915,   916,   917,   918,   129,   130,    61,    -1,
      -1,   134,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,
      -1,   934,   935,    -1,    -1,    -1,    -1,   544,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   783,
      -1,    -1,    -1,    -1,   788,    -1,    -1,   915,   916,   917,
     918,   104,    -1,   106,    -1,    -1,    -1,    -1,   802,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   934,   935,    -1,    -1,
     814,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   823,
      -1,    -1,    -1,   827,    -1,   602,    -1,   831,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   160,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,   171,    -1,
     173,    -1,    -1,   867,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,   186,   140,   141,   142,    -1,    -1,   145,
      -1,   147,    -1,    -1,    -1,    -1,    -1,   200,   201,    -1,
     667,   204,   669,    -1,    -1,   672,    -1,    -1,   675,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   219,   684,    -1,    -1,
     687,   915,   916,   917,   918,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,    -1,
     934,   935,   709,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   721,    -1,   723,    -1,    -1,    -1,
     727,    -1,   265,   266,   267,   268,     1,   140,   141,    -1,
      -1,    -1,   145,    -1,   147,    -1,    -1,    -1,    -1,   282,
      -1,    -1,    -1,    18,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    32,    33,    -1,
      -1,    36,    -1,    -1,    -1,    40,    -1,    42,    -1,    -1,
      -1,    -1,   315,   316,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    58,    -1,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    68,    -1,   802,    -1,    -1,    -1,   342,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   814,    -1,    84,
      -1,    86,    -1,    -1,    89,    -1,   823,    -1,    -1,    -1,
     827,    -1,    -1,    -1,   831,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   377,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   385,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   399,    -1,    -1,    -1,
     867,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   432,
      -1,    -1,   435,    -1,   437,    -1,    -1,     0,     1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    -1,    -1,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    27,    28,    -1,    30,    -1,    32,
      33,    34,    35,    36,    37,    -1,    39,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    -1,    52,
      -1,    -1,    -1,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    -1,    68,    69,    70,    -1,    72,
      73,    74,    75,    76,    77,    78,    -1,    80,   521,    82,
      83,    84,    -1,    86,    87,    88,    89,    -1,    -1,    92,
      -1,    94,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,   557,    -1,    -1,   120,   561,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,
     133,   134,    -1,   136,    -1,    -1,   139,    -1,    -1,    -1,
     143,   584,   145,    -1,   147,   588,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,   618,    -1,    -1,    -1,    34,
      35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    -1,    59,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,   691,    -1,
      -1,   694,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,
     115,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,   714,    -1,    -1,   129,    -1,   131,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,   732,
     145,    -1,   147,    -1,    -1,     1,   739,     3,     4,     5,
       6,     7,     8,    -1,    10,    -1,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
     833,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   844,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,   120,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
     136,   137,   138,   139,    -1,    -1,    -1,   143,   144,   145,
       1,   147,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,   133,   134,   135,   136,    -1,    -1,   139,    -1,
      -1,    -1,   143,   144,   145,     1,   147,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,
     136,    -1,    -1,   139,    -1,    -1,    -1,   143,   144,   145,
       1,   147,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,
      -1,    -1,   143,   144,   145,     1,   147,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,
     136,    -1,    -1,   139,    -1,    -1,    -1,   143,   144,   145,
       1,   147,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,
      -1,    -1,   143,   144,   145,     1,   147,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,
     136,    -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,
       1,   147,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,
      -1,    -1,   143,    -1,   145,     1,   147,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,
     136,    -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,
       1,   147,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,
      -1,    -1,   143,    -1,   145,     1,   147,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,
     136,    -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,
       1,   147,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,
      -1,    -1,   143,    -1,   145,     1,   147,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,
     136,    -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,
       1,   147,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,
      -1,    -1,   143,    -1,   145,     1,   147,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,
     136,    -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,
       1,   147,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,
      -1,    -1,   143,    -1,   145,     1,   147,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,     3,     4,     5,
       6,     7,     8,   129,    -1,   131,    12,   133,   134,    15,
     136,    -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,
      -1,   147,    28,    -1,    -1,    -1,    -1,    -1,    34,    35,
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
      -1,   131,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,   143,    -1,   145,    -1,   147,   148,     3,
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
     134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,
      -1,   145,    -1,   147,   148,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    82,    83,    -1,    85,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,   131,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    82,
      83,    -1,    85,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,   146,   147,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    82,    83,    -1,    85,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,   131,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,   145,   146,   147,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    38,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    82,
      83,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,   136,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    82,
      83,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    51,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    82,    83,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,     3,     4,     5,     6,     7,
       8,   129,    -1,   131,    12,   133,   134,    15,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,     3,     4,     5,     6,     7,
       8,   129,    -1,   131,    12,   133,   134,    15,    -1,    -1,
      -1,   139,    -1,    21,    -1,   143,    -1,   145,    -1,   147,
      28,    -1,    -1,    -1,    -1,    33,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,     3,     4,     5,     6,     7,
       8,   129,    -1,   131,    12,   133,   134,    15,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    82,    83,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,     3,     4,     5,     6,     7,
       8,   129,    -1,   131,    12,   133,   134,    15,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,   131,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,   144,   145,    -1,   147,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,     3,     4,     5,     6,     7,
       8,   129,    -1,   131,    12,   133,   134,    15,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,   131,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,   131,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    34,    35,    -1,    37,   129,    39,   131,    -1,
      -1,   134,    44,    45,    -1,    47,   139,    49,    50,    -1,
     143,    -1,   145,    -1,   147,    57,    -1,    59,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,   121,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   131,
      -1,    12,    -1,   135,    15,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,   145,   146,   147,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    34,    35,    -1,    37,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    47,    -1,    49,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    59,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    75,    76,    77,    78,    -1,     3,
       4,     5,     6,     7,     8,    -1,    87,    -1,    12,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      34,    35,   113,    37,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,
     131,    -1,     3,    57,    -1,    59,    -1,    61,    -1,    10,
      -1,    -1,   143,    -1,   145,    16,   147,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,   113,
      -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    -1,   147,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,    -1,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,    -1,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,    -1,   134,    -1,    -1,   137,   138,   139,   140,
      -1,   142,    -1,    -1,   145,    98,    99,   100,   101,   102,
     103,    -1,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,    -1,    -1,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
      -1,   134,    10,    -1,   137,   138,   139,   140,    16,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    67,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    16,    93,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    67,    -1,    -1,
      -1,    71,    -1,    -1,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,   132,   133,   134,    97,    -1,   137,
     138,   139,    -1,    -1,    -1,    67,    -1,    -1,    -1,    71,
     148,    -1,   112,    -1,   114,   115,   116,    -1,   118,   119,
     120,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,    -1,   132,   133,   134,    97,    -1,   137,   138,   139,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,   148,    16,
     112,    -1,   114,   115,   116,    -1,   118,   119,   120,    26,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    -1,
     132,   133,   134,    -1,    -1,   137,   138,   139,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,   148,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,
      67,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    26,    -1,    -1,    67,    -1,
      -1,    -1,    71,    -1,    -1,   112,    -1,   114,   115,   116,
      -1,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,   132,   133,   134,    97,    -1,
     137,   138,   139,    -1,    -1,    -1,    67,    -1,    -1,    -1,
      71,   148,    -1,   112,    -1,   114,   115,   116,    -1,   118,
     119,   120,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    -1,   132,   133,   134,    97,    -1,   137,   138,
     139,    -1,    -1,    -1,    10,    -1,    -1,   146,    -1,    -1,
      16,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      26,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    -1,    -1,   137,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,
      -1,    97,    98,    99,   100,   101,   102,   103,    26,    -1,
     106,   107,   108,   109,   110,   111,   112,    -1,   114,   115,
     116,    -1,   118,   119,   120,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,   132,   133,   134,    -1,
      -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,    67,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,    26,    -1,   106,   107,
     108,   109,   110,   111,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,   132,   133,   134,    -1,    -1,   137,
     138,   139,    -1,    -1,    -1,   143,    -1,    67,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    16,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,    26,    -1,   106,   107,   108,   109,
     110,   111,   112,    -1,   114,   115,   116,    -1,   118,   119,
     120,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,    -1,   132,   133,   134,    -1,    -1,   137,   138,   139,
      -1,    -1,    -1,   143,    -1,    67,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    16,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,    26,    -1,   106,   107,   108,   109,   110,   111,
     112,    -1,   114,   115,   116,    -1,   118,   119,   120,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    -1,
     132,   133,   134,    -1,    -1,   137,   138,   139,    -1,    -1,
      -1,   143,    -1,    67,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      16,    -1,    -1,    97,    98,    99,   100,   101,   102,   103,
      26,    -1,   106,   107,   108,   109,   110,   111,   112,    -1,
     114,   115,   116,    -1,   118,   119,   120,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,   132,   133,
     134,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
      -1,    67,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,
      -1,    97,    98,    99,   100,   101,   102,   103,    26,    -1,
     106,   107,   108,   109,   110,   111,   112,    -1,   114,   115,
     116,    -1,   118,   119,   120,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,   132,   133,   134,    -1,
      -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,    67,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,    26,    -1,   106,   107,
     108,   109,   110,   111,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,   132,   133,   134,    -1,    -1,   137,
     138,   139,    -1,    -1,    -1,   143,    -1,    67,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    16,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,    26,    -1,   106,   107,   108,   109,
     110,   111,   112,    -1,   114,   115,   116,    -1,   118,   119,
     120,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,    -1,   132,   133,   134,    -1,    -1,   137,   138,   139,
      -1,    -1,    -1,   143,    -1,    67,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,   106,   107,   108,   109,   110,   111,
     112,    -1,   114,   115,   116,    -1,   118,   119,   120,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    -1,
     132,   133,   134,    10,    -1,   137,   138,   139,    -1,    16,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    71,    38,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    10,    -1,    -1,
      97,    -1,    -1,    16,    67,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    26,    27,   112,    -1,   114,   115,   116,
      -1,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    97,   132,   133,   134,    -1,    -1,
     137,   138,   139,    -1,    -1,    -1,   143,    -1,    -1,   112,
      -1,   114,   115,   116,    67,   118,   119,   120,    71,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    -1,   132,
     133,   134,    10,    -1,   137,   138,   139,    -1,    16,    -1,
     143,    -1,    -1,    -1,    97,    -1,    -1,    -1,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,   114,   115,   116,    -1,   118,   119,   120,    -1,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    -1,   132,
     133,   134,    10,    -1,   137,   138,   139,    -1,    16,    67,
     143,    -1,    -1,    71,    -1,    -1,    -1,    -1,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    10,    -1,    -1,    97,
      -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   112,    -1,   114,   115,   116,    67,
     118,   119,   120,    71,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,   132,   133,   134,    -1,    -1,   137,
     138,   139,    -1,    -1,    -1,   143,    10,    -1,    -1,    97,
      -1,    -1,    16,    67,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    26,    -1,   112,    79,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    97,   132,   133,   134,    -1,    -1,   137,
     138,   139,    -1,    -1,    -1,   143,    -1,    -1,   112,    -1,
     114,   115,   116,    67,   118,   119,   120,    71,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,   132,   133,
     134,    10,    -1,   137,   138,   139,    -1,    16,    -1,   143,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
     114,   115,   116,    -1,   118,   119,   120,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,   132,   133,
     134,    10,    -1,   137,   138,   139,    -1,    16,    67,   143,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,   114,   115,   116,    67,   118,
     119,   120,    71,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    -1,   132,   133,   134,    10,    -1,   137,   138,
     139,    -1,    16,   142,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    38,   114,   115,   116,    -1,   118,
     119,   120,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    -1,   132,   133,   134,    10,   136,   137,   138,
     139,    -1,    16,    67,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
     114,   115,   116,    67,   118,   119,   120,    71,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,   132,   133,
     134,    10,    -1,   137,   138,   139,    -1,    16,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    38,
     114,   115,   116,    -1,   118,   119,   120,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,   132,   133,
     134,    10,    -1,   137,   138,   139,    -1,    16,    67,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    97,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   112,    -1,   114,   115,   116,    67,   118,
     119,   120,    71,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    -1,   132,   133,   134,    -1,    -1,   137,   138,
     139,    10,    -1,    -1,    -1,    -1,    -1,    16,    97,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    71,    26,    -1,    -1,
      -1,    -1,    -1,   112,    -1,   114,   115,   116,    -1,   118,
     119,   120,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    97,   132,   133,   134,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,    -1,    -1,    -1,   112,    67,   114,
     115,   116,    71,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    -1,   132,   133,   134,
      10,   136,   137,   138,   139,    -1,    16,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,   114,   115,   116,    -1,   118,
     119,   120,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    -1,   132,   133,   134,    10,   136,   137,   138,
     139,    -1,    16,    -1,    -1,    -1,    -1,    67,    -1,    -1,
      -1,    71,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    67,   114,   115,   116,    71,   118,   119,
     120,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,    -1,   132,   133,   134,    10,   136,   137,   138,   139,
      -1,    16,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
     114,   115,   116,    -1,   118,   119,   120,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,   132,   133,
     134,    -1,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    71,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    93,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    67,   132,   133,   134,    71,    -1,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      11,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
     114,   115,   116,    -1,   118,   119,   120,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,   132,   133,
     134,    -1,   136,   137,   138,   139,    67,    -1,    -1,    -1,
      71,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    29,    -1,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      67,   132,   133,   134,    71,    -1,   137,   138,   139,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,
      -1,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,   132,   133,   134,    -1,    67,
     137,   138,   139,    71,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    67,   132,   133,   134,    71,    -1,   137,
     138,   139,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    27,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
     114,   115,   116,    -1,   118,   119,   120,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,   132,   133,
     134,    -1,    67,   137,   138,   139,    71,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    71,    -1,    -1,    10,    11,    -1,    -1,
      -1,    79,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    97,
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
      71,    -1,    -1,    10,    -1,    -1,    -1,    -1,    79,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      67,   132,   133,   134,    71,    -1,   137,   138,   139,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,
      -1,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,   132,   133,   134,    -1,    -1,
     137,   138,   139,    67,    -1,    -1,    -1,    71,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
     114,   115,   116,    -1,   118,   119,   120,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    67,   132,   133,
     134,    71,    26,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    67,   114,   115,   116,    71,   118,   119,
     120,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,    -1,   132,   133,   134,    -1,    26,   137,   138,   139,
      -1,    -1,    67,    97,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
     114,   115,   116,    -1,   118,   119,   120,    -1,   122,   123,
     124,   125,    97,   127,   128,   129,   130,    67,   132,   133,
     134,    71,    26,   137,   138,   139,    -1,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,    -1,   132,    -1,   134,
      -1,    -1,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    67,   114,   115,   116,    71,   118,   119,
     120,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,    -1,   132,    -1,   134,    -1,    -1,   137,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
     114,   115,   116,    -1,   118,   119,   120,    -1,    -1,   123,
     124,   125,    -1,   127,   128,   129,   130,    -1,    -1,    -1,
     134,    -1,    -1,   137,   138,   139
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
     225,   228,   229,   247,   248,   249,   250,   251,   252,   260,
     261,   262,   263,   265,   266,   267,   268,   269,   270,   272,
     273,   274,   275,   276,   136,   157,   263,    93,   253,   254,
     169,   170,    12,    34,    35,    37,    78,   143,   147,   198,
     247,   251,   261,   262,   263,   265,   267,   268,   253,   263,
      84,   169,   157,   241,   263,   157,   145,     7,   169,   171,
       9,    84,   171,    58,    95,   179,   263,   263,    21,    33,
     228,   263,   263,   145,   170,   199,   145,   170,   230,   231,
      27,   161,   172,   263,   263,   263,   263,   263,   263,     7,
     145,    33,   175,   175,   135,   221,   240,   263,    82,    83,
     136,   263,   264,   263,   172,   263,   263,   263,    77,   145,
     157,   263,   263,   161,   168,   263,   265,   161,   168,   263,
     170,   226,   263,   240,   263,   263,   263,   263,   263,   263,
     263,   263,     1,   144,   155,   162,   240,    85,   121,   221,
     242,   243,   264,   240,   263,   271,    60,   157,    64,   160,
     170,   170,    42,    63,   216,    22,    58,    68,    89,   136,
     142,    10,    16,    26,    67,    71,    97,   112,   114,   115,
     116,   118,   119,   120,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   132,   133,   134,   137,   138,   139,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   140,   141,   145,   147,    67,    71,   145,
     157,   136,   263,   263,   263,   240,   143,   161,    38,   253,
     226,   136,   117,   136,    92,   170,   221,   244,   245,   246,
     264,   226,   191,   170,   145,   172,    27,    38,   172,    27,
      38,    93,   172,   256,    31,    53,   136,    79,    98,    99,
     100,   101,   102,   103,   106,   107,   108,   109,   110,   111,
     161,   208,   244,   157,   145,   210,    85,   145,   170,   232,
     233,     1,   116,   235,    38,   117,   157,   172,   172,   244,
     171,   170,   117,   136,   263,   263,   136,   143,   172,   145,
     244,   136,   183,   136,   183,   136,    98,   227,   136,   136,
      31,    53,   172,   136,   144,   144,   155,   117,   144,   263,
     117,   146,   117,   146,    38,   117,   148,   256,    96,   117,
     148,     7,    48,   116,   192,   143,   203,    68,   230,   230,
     230,   230,   263,   263,   263,   263,   179,   263,   179,   263,
     263,   263,   263,   263,   263,   263,    28,    84,   170,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   244,   244,   179,   263,   179,   263,    22,    38,    68,
      89,   255,   258,   259,   263,   276,    27,    38,    27,    38,
      79,    38,   148,   179,   263,   172,   136,   263,   263,    98,
     146,   117,   136,   170,    38,   240,    37,   263,   179,   263,
      37,   263,   179,   263,   145,   172,   139,   163,   165,   263,
     163,   164,   157,   263,    29,   263,   146,    22,    38,    41,
      55,    58,    68,    84,   156,   211,   212,   213,   214,   200,
     233,   117,   146,    15,    28,    57,    74,    75,    78,    87,
     147,   236,   248,    98,   234,   263,   231,   146,   221,   263,
       1,   188,   244,   146,    17,   184,   236,   248,   117,   165,
     164,   144,   146,   146,   242,   146,   242,   179,   263,   148,
     157,   263,   148,   263,   148,   263,   170,   240,   143,     1,
     170,   196,   197,    22,    58,    68,    84,   205,   215,   230,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     146,   148,    38,    68,   117,   146,   247,    67,    67,    37,
     179,   263,    37,   179,   263,   179,   263,   253,   253,   136,
     221,   264,   246,   192,   263,   146,   263,    27,   172,    27,
     172,   263,    27,   172,   256,    27,   172,   256,   257,   258,
     117,   136,    11,   136,    29,    29,   157,    79,   161,    38,
      68,   213,   117,   146,   145,   170,    22,    58,    68,    84,
     217,   146,   233,   235,     1,   240,    51,   264,   144,    54,
      90,   144,   189,   146,   145,   161,   170,   185,   226,   136,
     136,   148,   256,   148,   256,   157,    96,   143,     1,   193,
     144,    98,   117,   144,    68,   204,    10,    16,   112,   113,
     114,   115,   118,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   134,   137,   138,   139,   140,   142,
     145,   170,   206,   207,   208,   247,   136,   258,   235,   247,
     247,   263,    27,    27,   263,    27,    27,   148,   148,   172,
     172,   143,   172,    79,    37,   263,    37,   263,    79,    37,
     263,   172,    37,   263,   172,   117,   146,   263,   263,   157,
     263,   157,    29,   170,   212,   233,   116,   239,    68,   235,
     234,   148,    38,   148,    27,   157,   240,   185,   116,   161,
      37,   157,   263,   148,    37,   157,   263,   148,   263,     1,
     144,   162,   144,    33,    62,    65,   144,   156,   174,   263,
     197,   217,   170,   263,   119,   145,   209,   209,   234,    79,
      37,    37,    79,    37,    37,    37,    37,     1,   193,   263,
     263,   263,   263,   263,   263,   258,    11,    29,   157,   239,
     146,    15,    28,    57,    74,    75,    78,    87,   147,   221,
     238,   248,   121,   223,   234,    91,   224,   263,   236,   248,
     157,   172,   146,   263,   263,   157,   263,   157,   144,   144,
     174,   235,   146,   170,   207,   208,   211,   263,   263,   263,
     263,   263,   263,   144,   144,    79,    79,    79,    79,   263,
     157,   234,   239,   148,   240,   221,   222,   263,   263,   161,
     173,   220,   148,   161,    79,    79,    81,   218,   209,   209,
     146,    79,    79,    79,    79,    79,    79,   263,   263,   263,
     263,   223,   234,   221,   237,   238,   248,    38,   148,   248,
     263,   263,   224,   263,   237,   238,   136,   219,   220,   148,
     237
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
#line 446 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 451 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 452 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 459 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 464 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 465 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 484 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 485 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 486 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 487 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 488 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 489 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 490 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 491 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 492 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 493 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 494 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 495 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 496 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 497 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 498 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 499 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (6)].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[(1) - (6)].b), (yyvsp[(2) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 507 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType, (yyvsp[(6) - (7)].pblockstmt), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 509 "chapel.ypp"
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 514 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 515 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 516 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 520 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 521 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 534 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 535 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 542 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 547 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 552 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 557 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 561 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 568 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 588 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 592 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 597 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 599 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 601 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 603 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 605 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 607 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 609 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 611 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 613 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 615 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 617 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 619 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 621 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 623 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 625 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 627 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 631 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 636 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 645 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 646 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 650 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 651 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 655 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 657 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 658 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 671 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 672 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 673 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 674 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 679 "chapel.ypp"
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
#line 693 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 694 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 698 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 702 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 703 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 704 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 705 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 706 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 707 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 708 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 710 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 716 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 722 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 728 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 734 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 741 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 758 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 760 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 762 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 764 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 772 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 773 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 774 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 775 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 776 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 777 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 781 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 782 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 786 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 787 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 788 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 792 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 793 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 797 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 801 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 803 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 807 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 808 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 813 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 815 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 817 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 824 "chapel.ypp"
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
#line 834 "chapel.ypp"
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
#line 844 "chapel.ypp"
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
#line 854 "chapel.ypp"
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
#line 866 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 871 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 876 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 884 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 885 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 890 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 892 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 894 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 899 "chapel.ypp"
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
#line 915 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 922 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 931 "chapel.ypp"
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
#line 939 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 943 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 949 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 950 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 955 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 960 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 965 "chapel.ypp"
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
#line 985 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 991 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 998 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 1006 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 1014 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 1025 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 1031 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 1038 "chapel.ypp"
    {
      FnSymbol* fn = (yyvsp[(4) - (10)].pfnsymbol);

      fn->copyFlags((yyvsp[(1) - (10)].pfnsymbol));
      // The user explicitly named this function (controls mangling).
      if (*(yyvsp[(1) - (10)].pfnsymbol)->name)
        fn->cname = (yyvsp[(1) - (10)].pfnsymbol)->name;

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
#line 1060 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 1064 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 1069 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 1073 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 1081 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1086 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1087 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1089 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1090 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1091 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1092 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1094 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1096 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1097 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1099 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1101 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1102 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1103 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1104 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1105 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1106 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1107 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1108 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1114 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1115 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1116 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1117 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1118 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1119 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1120 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1121 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1122 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1123 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1124 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1125 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1129 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1130 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1134 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1139 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1140 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1145 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1147 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1149 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1151 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1153 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1157 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1158 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1162 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1163 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1164 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1165 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1166 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1167 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1168 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1169 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1170 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1174 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1175 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1176 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1177 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1178 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1179 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1183 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1184 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1188 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1190 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1191 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1192 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1193 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1197 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1198 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1201 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1207 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1214 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1218 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1228 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1234 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1236 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1238 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1243 "chapel.ypp"
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
#line 1256 "chapel.ypp"
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
#line 1272 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1276 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1281 "chapel.ypp"
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
#line 1289 "chapel.ypp"
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
#line 1297 "chapel.ypp"
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
#line 1305 "chapel.ypp"
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
#line 1314 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1323 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1324 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1325 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1331 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1341 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1346 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1348 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1350 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1357 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1365 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1366 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1367 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1374 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1375 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1376 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1377 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1378 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1379 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1380 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1381 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1402 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1406 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1410 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1418 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1424 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1425 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1426 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1431 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1439 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1441 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1443 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1447 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1448 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1449 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1450 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1451 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1452 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1453 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1454 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1455 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1456 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1457 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1463 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1464 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1465 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1466 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1470 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1471 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1476 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1481 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1486 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1492 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1496 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1497 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1498 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1499 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1526 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1532 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1534 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1536 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1553 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1555 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1557 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1559 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1561 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1563 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1565 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1569 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1576 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1582 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1588 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1594 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1612 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1637 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1641 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1642 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1646 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1650 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1651 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1656 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1660 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1664 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1670 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1671 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1672 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1673 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1674 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1681 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1686 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1691 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1707 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1709 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1711 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1713 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1715 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1721 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1722 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1745 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1746 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1747 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1751 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1752 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1753 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1761 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1762 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1763 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1764 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1768 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1769 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1770 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1771 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1772 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1773 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1775 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1776 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1778 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1782 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1789 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1790 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1794 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1795 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1796 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1797 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1798 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1799 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1800 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1801 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1802 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1803 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1804 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1805 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1806 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1807 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1808 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1809 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1810 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1811 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1812 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1813 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1814 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1815 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1816 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1820 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1821 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1822 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1823 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1824 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1825 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1829 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1830 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1831 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1832 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 1836 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1837 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1838 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1839 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1844 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1845 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1846 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1847 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1848 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1849 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1850 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 8487 "bison-chapel.cpp"
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


