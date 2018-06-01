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
     TOWNED = 311,
     TPARAM = 312,
     TPLUSPLUS = 313,
     TPRAGMA = 314,
     TPRIMITIVE = 315,
     TPRIVATE = 316,
     TPROC = 317,
     TPROTOTYPE = 318,
     TPUBLIC = 319,
     TRECORD = 320,
     TREDUCE = 321,
     TREF = 322,
     TREQUIRE = 323,
     TRETURN = 324,
     TSCAN = 325,
     TSELECT = 326,
     TSERIAL = 327,
     TSHARED = 328,
     TSINGLE = 329,
     TSPARSE = 330,
     TSUBDOMAIN = 331,
     TSYNC = 332,
     TTHEN = 333,
     TTHROW = 334,
     TTHROWS = 335,
     TTRY = 336,
     TTRYBANG = 337,
     TTYPE = 338,
     TUNDERSCORE = 339,
     TUNION = 340,
     TUNMANAGED = 341,
     TUSE = 342,
     TVAR = 343,
     TWHEN = 344,
     TWHERE = 345,
     TWHILE = 346,
     TWITH = 347,
     TYIELD = 348,
     TZIP = 349,
     TALIAS = 350,
     TAND = 351,
     TASSIGN = 352,
     TASSIGNBAND = 353,
     TASSIGNBOR = 354,
     TASSIGNBXOR = 355,
     TASSIGNDIVIDE = 356,
     TASSIGNEXP = 357,
     TASSIGNLAND = 358,
     TASSIGNLOR = 359,
     TASSIGNMINUS = 360,
     TASSIGNMOD = 361,
     TASSIGNMULTIPLY = 362,
     TASSIGNPLUS = 363,
     TASSIGNSL = 364,
     TASSIGNSR = 365,
     TBAND = 366,
     TBNOT = 367,
     TBOR = 368,
     TBXOR = 369,
     TCOLON = 370,
     TCOMMA = 371,
     TDIVIDE = 372,
     TDOT = 373,
     TDOTDOT = 374,
     TDOTDOTDOT = 375,
     TEQUAL = 376,
     TEXP = 377,
     TGREATER = 378,
     TGREATEREQUAL = 379,
     THASH = 380,
     TLESS = 381,
     TLESSEQUAL = 382,
     TMINUS = 383,
     TMOD = 384,
     TNOT = 385,
     TNOTEQUAL = 386,
     TOR = 387,
     TPLUS = 388,
     TQUESTION = 389,
     TSEMI = 390,
     TSHIFTLEFT = 391,
     TSHIFTRIGHT = 392,
     TSTAR = 393,
     TSWAP = 394,
     TASSIGNREDUCE = 395,
     TIO = 396,
     TLCBR = 397,
     TRCBR = 398,
     TLP = 399,
     TRP = 400,
     TLSBR = 401,
     TRSBR = 402,
     TNOELSE = 403,
     TUMINUS = 404,
     TUPLUS = 405
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
#line 450 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 457 "bison-chapel.cpp"
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
#line 536 "bison-chapel.cpp"

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
#define YYLAST   13529

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  151
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  125
/* YYNRULES -- Number of rules.  */
#define YYNRULES  516
/* YYNRULES -- Number of states.  */
#define YYNSTATES  965

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   405

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
     145,   146,   147,   148,   149,   150
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
     700,   701,   703,   706,   709,   710,   711,   722,   726,   730,
     736,   742,   744,   748,   750,   753,   755,   757,   759,   761,
     763,   765,   767,   769,   771,   773,   775,   777,   779,   781,
     783,   785,   787,   789,   791,   793,   795,   797,   799,   801,
     803,   805,   807,   809,   811,   813,   815,   817,   819,   821,
     823,   825,   826,   830,   834,   835,   837,   841,   846,   851,
     858,   865,   866,   868,   870,   872,   874,   876,   879,   882,
     884,   886,   888,   889,   891,   893,   896,   898,   900,   902,
     904,   905,   907,   910,   912,   914,   916,   917,   919,   921,
     923,   925,   927,   930,   932,   933,   935,   938,   941,   942,
     945,   949,   954,   959,   962,   967,   968,   971,   974,   979,
     984,   989,   995,  1000,  1001,  1003,  1005,  1007,  1011,  1015,
    1021,  1023,  1025,  1029,  1031,  1034,  1038,  1039,  1042,  1045,
    1046,  1049,  1052,  1055,  1058,  1061,  1064,  1067,  1070,  1073,
    1075,  1080,  1085,  1092,  1096,  1097,  1099,  1101,  1105,  1110,
    1114,  1119,  1126,  1127,  1130,  1133,  1136,  1139,  1142,  1145,
    1148,  1151,  1154,  1157,  1159,  1161,  1165,  1169,  1171,  1175,
    1177,  1179,  1181,  1185,  1189,  1190,  1192,  1194,  1198,  1202,
    1206,  1208,  1210,  1212,  1214,  1216,  1218,  1220,  1222,  1225,
    1230,  1235,  1240,  1246,  1249,  1252,  1255,  1258,  1261,  1264,
    1271,  1278,  1283,  1293,  1303,  1311,  1318,  1325,  1330,  1340,
    1350,  1358,  1363,  1370,  1377,  1387,  1397,  1404,  1406,  1408,
    1410,  1412,  1414,  1416,  1418,  1420,  1424,  1425,  1427,  1432,
    1434,  1438,  1443,  1445,  1449,  1454,  1458,  1462,  1464,  1466,
    1469,  1471,  1474,  1476,  1478,  1482,  1485,  1490,  1492,  1494,
    1496,  1498,  1500,  1502,  1504,  1506,  1511,  1515,  1519,  1522,
    1525,  1527,  1530,  1533,  1535,  1537,  1539,  1541,  1543,  1545,
    1547,  1552,  1557,  1562,  1566,  1570,  1574,  1578,  1583,  1587,
    1592,  1594,  1596,  1598,  1600,  1602,  1606,  1611,  1615,  1620,
    1624,  1629,  1633,  1639,  1643,  1647,  1651,  1655,  1659,  1663,
    1667,  1671,  1675,  1679,  1683,  1687,  1691,  1695,  1699,  1703,
    1707,  1711,  1715,  1719,  1723,  1727,  1731,  1734,  1737,  1740,
    1743,  1746,  1749,  1753,  1757,  1761,  1765,  1769,  1773,  1777,
    1781,  1783,  1785,  1787,  1789,  1791,  1793
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     152,     0,    -1,   153,    -1,    -1,   153,   154,    -1,   156,
      -1,   155,   156,    -1,    59,     7,    -1,   155,    59,     7,
      -1,   157,    -1,   160,    -1,   165,    -1,   166,    -1,   173,
      -1,   167,    -1,   176,    -1,   179,    -1,   177,    -1,   186,
      -1,   180,    -1,   181,    -1,   185,    -1,   172,    -1,   251,
     135,    -1,    12,   156,    -1,    13,   252,   156,    -1,    14,
     168,   135,    -1,    19,   252,   160,    -1,    23,   168,   135,
      -1,    25,   240,   135,    -1,    43,   169,   156,    -1,    46,
     262,   171,    -1,    46,   171,    -1,    52,   262,   171,    -1,
      72,   262,   171,    -1,    72,   171,    -1,    77,   156,    -1,
      93,   262,   135,    -1,     1,   135,    -1,   158,   159,    48,
     169,   142,   143,    -1,   158,   159,    48,   169,   142,   161,
     143,    -1,   158,   159,    48,   169,   142,     1,   143,    -1,
      -1,    64,    -1,    61,    -1,    -1,    63,    -1,   142,   143,
      -1,   142,   161,   143,    -1,   142,     1,   143,    -1,   154,
      -1,   161,   154,    -1,   262,    -1,   262,    11,   262,    -1,
     162,   116,   262,    -1,   162,   116,   262,    11,   262,    -1,
      -1,   162,    -1,   138,    -1,   162,    -1,    87,   239,   135,
      -1,    87,   262,    31,   164,   135,    -1,    87,   262,    53,
     163,   135,    -1,    68,   239,   135,    -1,   264,    97,   263,
     135,    -1,   264,   108,   263,   135,    -1,   264,   105,   263,
     135,    -1,   264,   107,   263,   135,    -1,   264,   101,   263,
     135,    -1,   264,   106,   263,   135,    -1,   264,   102,   263,
     135,    -1,   264,    98,   263,   135,    -1,   264,    99,   263,
     135,    -1,   264,   100,   263,   135,    -1,   264,   110,   263,
     135,    -1,   264,   109,   263,   135,    -1,   264,   139,   263,
     135,    -1,   264,   140,   263,   135,    -1,   264,   103,   263,
     135,    -1,   264,   104,   263,   135,    -1,    -1,   169,    -1,
       3,    -1,   168,    -1,     7,    -1,    27,   156,    -1,   160,
      -1,    69,   135,    -1,    69,   263,   135,    -1,   135,    -1,
     174,    -1,    64,   174,    -1,    61,   174,    -1,   193,    -1,
     224,    -1,   189,    -1,   175,    -1,   201,    -1,   227,    -1,
      36,   262,   135,    -1,    36,   262,    31,   164,   135,    -1,
      36,   262,    53,   163,   135,    -1,    36,   227,    -1,    33,
       9,    -1,    27,   156,    91,   262,   135,    -1,    91,   262,
     171,    -1,    20,   262,    38,   262,   252,   171,    -1,    20,
     262,    38,   178,   252,   171,    -1,    20,   262,   252,   171,
      -1,    34,   262,    38,   262,   171,    -1,    34,   262,    38,
     178,   171,    -1,    34,   262,   171,    -1,    34,   178,   171,
      -1,    34,    57,   169,    38,   262,   171,    -1,    35,   262,
      38,   262,   171,    -1,    35,   262,    38,   262,   255,   171,
      -1,    35,   262,    38,   178,   171,    -1,    35,   262,    38,
     178,   255,   171,    -1,    35,   262,   171,    -1,    35,   262,
     255,   171,    -1,   146,   239,    38,   262,   147,   156,    -1,
     146,   239,    38,   262,   255,   147,   156,    -1,   146,   239,
      38,   178,   147,   156,    -1,   146,   239,    38,   178,   255,
     147,   156,    -1,   146,   239,   147,   156,    -1,   146,   239,
     255,   147,   156,    -1,    94,   144,   239,   145,    -1,    37,
     262,    78,   156,    -1,    37,   262,   160,    -1,    37,   262,
      78,   156,    29,   156,    -1,    37,   262,   160,    29,   156,
      -1,    37,   262,   207,   262,    78,   156,    -1,    37,   262,
     207,   262,   160,    -1,    37,   262,   207,   262,    78,   156,
      29,   156,    -1,    37,   262,   207,   262,   160,    29,   156,
      -1,    24,   156,    -1,    81,   262,   135,    -1,    82,   262,
     135,    -1,    81,   167,    -1,    82,   167,    -1,    81,   160,
     182,    -1,    82,   160,   182,    -1,    -1,   182,   183,    -1,
      17,   160,    -1,    17,   184,   160,    -1,    17,   144,   184,
     145,   160,    -1,   169,    -1,   169,   115,   262,    -1,    79,
     262,   135,    -1,    71,   262,   142,   187,   143,    -1,    71,
     262,   142,     1,   143,    -1,    -1,   187,   188,    -1,    89,
     239,   171,    -1,    54,   156,    -1,    54,    27,   156,    -1,
     190,   169,   191,   142,   192,   143,    -1,    33,   170,   190,
     169,   191,   142,   192,   143,    -1,   190,   169,   191,   142,
       1,   143,    -1,    33,   170,   190,   169,   191,   142,     1,
     143,    -1,    18,    -1,    65,    -1,    85,    -1,    -1,   115,
     239,    -1,    -1,   192,   173,    -1,   192,   155,   173,    -1,
     194,   169,   142,   195,   143,    -1,   194,   169,   142,     1,
     143,    -1,    30,    -1,   196,    -1,   195,   116,    -1,   195,
     116,   196,    -1,   169,    -1,   169,    97,   262,    -1,    -1,
      -1,    44,   198,   209,   199,   216,   234,   223,   219,    -1,
      -1,    40,    -1,    32,   170,    -1,    33,   170,    -1,    -1,
      -1,   200,   215,   202,   204,   203,   216,   234,   217,   223,
     218,    -1,   214,   206,   208,    -1,   214,   207,   208,    -1,
     214,   205,   118,   206,   208,    -1,   214,   205,   118,   207,
     208,    -1,   246,    -1,   144,   262,   145,    -1,   169,    -1,
     112,   169,    -1,   111,    -1,   113,    -1,   114,    -1,   112,
      -1,   121,    -1,   131,    -1,   127,    -1,   124,    -1,   126,
      -1,   123,    -1,   133,    -1,   128,    -1,   138,    -1,   117,
      -1,   136,    -1,   137,    -1,   129,    -1,   122,    -1,   130,
      -1,    16,    -1,   125,    -1,    10,    -1,   139,    -1,   141,
      -1,    97,    -1,   108,    -1,   105,    -1,   107,    -1,   101,
      -1,   106,    -1,   102,    -1,    98,    -1,    99,    -1,   100,
      -1,   110,    -1,   109,    -1,    -1,   144,   210,   145,    -1,
     144,   210,   145,    -1,    -1,   211,    -1,   210,   116,   211,
      -1,   212,   169,   238,   233,    -1,   212,   169,   238,   222,
      -1,   212,   144,   232,   145,   238,   233,    -1,   212,   144,
     232,   145,   238,   222,    -1,    -1,   213,    -1,    38,    -1,
      41,    -1,    55,    -1,    22,    -1,    22,    38,    -1,    22,
      67,    -1,    57,    -1,    67,    -1,    83,    -1,    -1,    57,
      -1,    67,    -1,    22,    67,    -1,    22,    -1,    83,    -1,
      62,    -1,    42,    -1,    -1,    22,    -1,    22,    67,    -1,
      67,    -1,    57,    -1,    83,    -1,    -1,    80,    -1,   135,
      -1,   219,    -1,   160,    -1,   172,    -1,   134,   169,    -1,
     134,    -1,    -1,   220,    -1,   120,   262,    -1,   120,   221,
      -1,    -1,    90,   262,    -1,    83,   225,   135,    -1,    21,
      83,   225,   135,    -1,    33,    83,   225,   135,    -1,   169,
     226,    -1,   169,   226,   116,   225,    -1,    -1,    97,   247,
      -1,    97,   235,    -1,   228,    57,   229,   135,    -1,   228,
      22,   229,   135,    -1,   228,    67,   229,   135,    -1,   228,
      22,    67,   229,   135,    -1,   228,    88,   229,   135,    -1,
      -1,    21,    -1,    33,    -1,   230,    -1,   229,   116,   230,
      -1,   169,   234,   233,    -1,   144,   232,   145,   234,   233,
      -1,    84,    -1,   169,    -1,   144,   232,   145,    -1,   231,
      -1,   231,   116,    -1,   231,   116,   232,    -1,    -1,    97,
      51,    -1,    97,   263,    -1,    -1,   115,   247,    -1,   115,
     235,    -1,   115,    28,    -1,   115,    74,    -1,   115,    77,
      -1,   115,    15,    -1,   115,    56,    -1,   115,    86,    -1,
     115,    73,    -1,     1,    -1,   146,   239,   147,   247,    -1,
     146,   239,   147,   235,    -1,   146,   239,    38,   262,   147,
     247,    -1,   146,     1,   147,    -1,    -1,   247,    -1,   220,
      -1,   146,   147,   236,    -1,   146,   239,   147,   236,    -1,
     146,   147,   237,    -1,   146,   239,   147,   237,    -1,   146,
     239,    38,   262,   147,   236,    -1,    -1,   115,   247,    -1,
     115,   220,    -1,   115,    28,    -1,   115,    74,    -1,   115,
      77,    -1,   115,    15,    -1,   115,    56,    -1,   115,    86,
      -1,   115,    73,    -1,   115,   237,    -1,   262,    -1,   220,
      -1,   239,   116,   262,    -1,   239,   116,   220,    -1,   262,
      -1,   240,   116,   262,    -1,    84,    -1,   263,    -1,   220,
      -1,   241,   116,   241,    -1,   242,   116,   241,    -1,    -1,
     244,    -1,   245,    -1,   244,   116,   245,    -1,   169,    97,
     220,    -1,   169,    97,   263,    -1,   220,    -1,   263,    -1,
     169,    -1,   250,    -1,   264,    -1,   249,    -1,   272,    -1,
     271,    -1,    74,   262,    -1,    39,   144,   243,   145,    -1,
      28,   144,   243,   145,    -1,    76,   144,   243,   145,    -1,
      75,    76,   144,   243,   145,    -1,    12,   262,    -1,    77,
     262,    -1,    15,   262,    -1,    56,   262,    -1,    86,   262,
      -1,    73,   262,    -1,    34,   262,    38,   262,    27,   262,
      -1,    34,   262,    38,   178,    27,   262,    -1,    34,   262,
      27,   262,    -1,    34,   262,    38,   262,    27,    37,   262,
      78,   262,    -1,    34,   262,    38,   178,    27,    37,   262,
      78,   262,    -1,    34,   262,    27,    37,   262,    78,   262,
      -1,    35,   262,    38,   262,    27,   262,    -1,    35,   262,
      38,   178,    27,   262,    -1,    35,   262,    27,   262,    -1,
      35,   262,    38,   262,    27,    37,   262,    78,   262,    -1,
      35,   262,    38,   178,    27,    37,   262,    78,   262,    -1,
      35,   262,    27,    37,   262,    78,   262,    -1,   146,   239,
     147,   262,    -1,   146,   239,    38,   262,   147,   262,    -1,
     146,   239,    38,   178,   147,   262,    -1,   146,   239,    38,
     262,   147,    37,   262,    78,   262,    -1,   146,   239,    38,
     178,   147,    37,   262,    78,   262,    -1,    37,   262,    78,
     262,    29,   262,    -1,    50,    -1,   250,    -1,   246,    -1,
     267,    -1,   266,    -1,   197,    -1,   260,    -1,   261,    -1,
     259,   141,   262,    -1,    -1,   253,    -1,    92,   144,   254,
     145,    -1,   257,    -1,   254,   116,   257,    -1,    92,   144,
     256,   145,    -1,   257,    -1,   256,   116,   257,    -1,   258,
     246,   234,   233,    -1,   275,    66,   246,    -1,   262,    66,
     246,    -1,    22,    -1,    38,    -1,    22,    38,    -1,    67,
      -1,    22,    67,    -1,    88,    -1,   264,    -1,   259,   141,
     262,    -1,    49,   262,    -1,    45,   229,    38,   262,    -1,
     269,    -1,   247,    -1,   248,    -1,   273,    -1,   274,    -1,
     197,    -1,   260,    -1,   261,    -1,   144,   120,   262,   145,
      -1,   262,   115,   262,    -1,   262,   119,   262,    -1,   262,
     119,    -1,   119,   262,    -1,   119,    -1,    81,   262,    -1,
      82,   262,    -1,   262,    -1,   246,    -1,   266,    -1,   267,
      -1,   268,    -1,   264,    -1,   197,    -1,   265,   144,   243,
     145,    -1,   265,   146,   243,   147,    -1,    60,   144,   243,
     145,    -1,   262,   118,   169,    -1,   262,   118,    83,    -1,
     262,   118,    28,    -1,   144,   241,   145,    -1,   144,   241,
     116,   145,    -1,   144,   242,   145,    -1,   144,   242,   116,
     145,    -1,     4,    -1,     5,    -1,     6,    -1,     7,    -1,
       8,    -1,   142,   239,   143,    -1,   142,   239,   116,   143,
      -1,   146,   239,   147,    -1,   146,   239,   116,   147,    -1,
     146,   270,   147,    -1,   146,   270,   116,   147,    -1,   262,
      95,   262,    -1,   270,   116,   262,    95,   262,    -1,   262,
     133,   262,    -1,   262,   128,   262,    -1,   262,   138,   262,
      -1,   262,   117,   262,    -1,   262,   136,   262,    -1,   262,
     137,   262,    -1,   262,   129,   262,    -1,   262,   121,   262,
      -1,   262,   131,   262,    -1,   262,   127,   262,    -1,   262,
     124,   262,    -1,   262,   126,   262,    -1,   262,   123,   262,
      -1,   262,   111,   262,    -1,   262,   113,   262,    -1,   262,
     114,   262,    -1,   262,    96,   262,    -1,   262,   132,   262,
      -1,   262,   122,   262,    -1,   262,    16,   262,    -1,   262,
      10,   262,    -1,   262,   125,   262,    -1,   262,    26,   262,
      -1,   133,   262,    -1,   128,   262,    -1,    47,   262,    -1,
      58,   262,    -1,   130,   262,    -1,   112,   262,    -1,   262,
      66,   262,    -1,   262,    66,   178,    -1,   275,    66,   262,
      -1,   275,    66,   178,    -1,   262,    70,   262,    -1,   262,
      70,   178,    -1,   275,    70,   262,    -1,   275,    70,   178,
      -1,   133,    -1,   138,    -1,    96,    -1,   132,    -1,   111,
      -1,   113,    -1,   114,    -1
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
     985,   991,   998,  1006,  1018,  1024,  1017,  1052,  1056,  1061,
    1065,  1073,  1074,  1078,  1079,  1080,  1081,  1082,  1083,  1084,
    1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,  1094,
    1095,  1096,  1097,  1098,  1099,  1100,  1101,  1102,  1103,  1107,
    1108,  1109,  1110,  1111,  1112,  1113,  1114,  1115,  1116,  1117,
    1118,  1122,  1123,  1127,  1131,  1132,  1133,  1137,  1139,  1141,
    1143,  1148,  1149,  1153,  1154,  1155,  1156,  1157,  1158,  1159,
    1160,  1161,  1165,  1166,  1167,  1168,  1169,  1170,  1174,  1175,
    1179,  1180,  1181,  1182,  1183,  1184,  1188,  1189,  1192,  1193,
    1197,  1198,  1202,  1204,  1209,  1210,  1214,  1215,  1219,  1220,
    1224,  1226,  1228,  1233,  1246,  1263,  1264,  1266,  1271,  1279,
    1287,  1295,  1304,  1314,  1315,  1316,  1320,  1321,  1329,  1331,
    1336,  1338,  1340,  1345,  1347,  1349,  1356,  1357,  1358,  1362,
    1363,  1364,  1365,  1366,  1367,  1368,  1369,  1370,  1371,  1372,
    1392,  1396,  1400,  1408,  1415,  1416,  1417,  1421,  1423,  1429,
    1431,  1433,  1438,  1439,  1440,  1441,  1442,  1443,  1444,  1445,
    1446,  1447,  1448,  1454,  1455,  1456,  1457,  1461,  1462,  1466,
    1467,  1468,  1472,  1473,  1477,  1478,  1482,  1483,  1487,  1488,
    1489,  1490,  1494,  1505,  1506,  1507,  1508,  1509,  1510,  1512,
    1514,  1516,  1518,  1520,  1522,  1524,  1526,  1528,  1530,  1535,
    1537,  1539,  1541,  1543,  1545,  1547,  1549,  1551,  1553,  1555,
    1557,  1559,  1566,  1572,  1578,  1584,  1593,  1603,  1611,  1612,
    1613,  1614,  1615,  1616,  1617,  1618,  1623,  1624,  1628,  1632,
    1633,  1637,  1641,  1642,  1646,  1650,  1654,  1661,  1662,  1663,
    1664,  1665,  1666,  1670,  1671,  1676,  1681,  1689,  1690,  1691,
    1692,  1693,  1694,  1695,  1696,  1697,  1699,  1701,  1703,  1705,
    1707,  1712,  1713,  1714,  1717,  1718,  1719,  1720,  1731,  1732,
    1736,  1737,  1738,  1742,  1743,  1744,  1752,  1753,  1754,  1755,
    1759,  1760,  1761,  1762,  1763,  1764,  1765,  1766,  1767,  1768,
    1772,  1780,  1781,  1785,  1786,  1787,  1788,  1789,  1790,  1791,
    1792,  1793,  1794,  1795,  1796,  1797,  1798,  1799,  1800,  1801,
    1802,  1803,  1804,  1805,  1806,  1807,  1811,  1812,  1813,  1814,
    1815,  1816,  1820,  1821,  1822,  1823,  1827,  1828,  1829,  1830,
    1835,  1836,  1837,  1838,  1839,  1840,  1841
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
  "TON", "TONLY", "TOTHERWISE", "TOUT", "TOWNED", "TPARAM", "TPLUSPLUS",
  "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC",
  "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT",
  "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE", "TSUBDOMAIN", "TSYNC",
  "TTHEN", "TTHROW", "TTHROWS", "TTRY", "TTRYBANG", "TTYPE", "TUNDERSCORE",
  "TUNION", "TUNMANAGED", "TUSE", "TVAR", "TWHEN", "TWHERE", "TWHILE",
  "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND",
  "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP",
  "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
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
     405
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   151,   152,   153,   153,   154,   154,   155,   155,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   157,
     157,   157,   158,   158,   158,   159,   159,   160,   160,   160,
     161,   161,   162,   162,   162,   162,   163,   163,   164,   164,
     165,   165,   165,   166,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     168,   168,   169,   170,   170,   171,   171,   172,   172,   173,
     173,   173,   173,   173,   173,   173,   173,   174,   174,   175,
     175,   175,   175,   176,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   178,   179,   179,   179,
     179,   179,   179,   179,   179,   180,   181,   181,   181,   181,
     181,   181,   182,   182,   183,   183,   183,   184,   184,   185,
     186,   186,   187,   187,   188,   188,   188,   189,   189,   189,
     189,   190,   190,   190,   191,   191,   192,   192,   192,   193,
     193,   194,   195,   195,   195,   196,   196,   198,   199,   197,
     200,   200,   200,   200,   202,   203,   201,   204,   204,   204,
     204,   205,   205,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   208,   208,   209,   210,   210,   210,   211,   211,   211,
     211,   212,   212,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   214,   214,   214,   214,   214,   214,   215,   215,
     216,   216,   216,   216,   216,   216,   217,   217,   218,   218,
     219,   219,   220,   220,   221,   221,   222,   222,   223,   223,
     224,   224,   224,   225,   225,   226,   226,   226,   227,   227,
     227,   227,   227,   228,   228,   228,   229,   229,   230,   230,
     231,   231,   231,   232,   232,   232,   233,   233,   233,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     235,   235,   235,   235,   236,   236,   236,   237,   237,   237,
     237,   237,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   239,   239,   239,   239,   240,   240,   241,
     241,   241,   242,   242,   243,   243,   244,   244,   245,   245,
     245,   245,   246,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   249,   250,   251,   251,
     251,   251,   251,   251,   251,   251,   252,   252,   253,   254,
     254,   255,   256,   256,   257,   257,   257,   258,   258,   258,
     258,   258,   258,   259,   259,   260,   261,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   263,   263,   263,   264,   264,   264,   264,   265,   265,
     266,   266,   266,   267,   267,   267,   268,   268,   268,   268,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   270,   270,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   272,   272,   272,   272,
     272,   272,   273,   273,   273,   273,   274,   274,   274,   274,
     275,   275,   275,   275,   275,   275,   275
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
       0,     1,     2,     2,     0,     0,    10,     3,     3,     5,
       5,     1,     3,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     3,     0,     1,     3,     4,     4,     6,
       6,     0,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     0,     1,     1,     2,     1,     1,     1,     1,
       0,     1,     2,     1,     1,     1,     0,     1,     1,     1,
       1,     1,     2,     1,     0,     1,     2,     2,     0,     2,
       3,     4,     4,     2,     4,     0,     2,     2,     4,     4,
       4,     5,     4,     0,     1,     1,     1,     3,     3,     5,
       1,     1,     3,     1,     2,     3,     0,     2,     2,     0,
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
       1,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     4,     3,     3,     3,     3,     4,     3,     4,
       1,     1,     1,     1,     1,     3,     4,     3,     4,     3,
       4,     3,     5,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   460,   461,   462,   463,
     464,     0,   406,    80,     0,   161,   406,     0,   294,    80,
       0,     0,     0,     0,   171,    80,    80,     0,     0,   293,
       0,     0,   181,     0,   177,     0,     0,     0,     0,   397,
       0,     0,     0,     0,     0,   293,   293,   162,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   163,     0,     0,     0,     0,   512,   514,     0,   515,
     516,   440,     0,     0,   513,   510,    89,   511,     0,     0,
       0,     4,     0,     5,     9,    45,    10,    11,    12,    14,
     362,    22,    13,    90,    96,    15,    17,    16,    19,    20,
      21,    18,    95,     0,    93,     0,   432,     0,    97,    94,
      98,     0,   444,   428,   429,   365,   363,     0,     0,   433,
     434,     0,   364,     0,   445,   446,   447,   427,   367,   366,
     430,   431,     0,    38,    24,   373,     0,     0,   407,     0,
      81,     0,     0,     0,     0,     0,     0,     0,   432,   444,
     363,   433,   434,   375,   364,   445,   446,     0,   406,     0,
       0,   135,     0,   347,     0,   354,    84,    83,   182,   103,
       0,   183,     0,     0,     0,     0,     0,   294,   295,   102,
       0,     0,   354,     0,     0,     0,     0,     0,   296,     0,
      86,    32,     0,   498,   425,     0,   376,   499,     7,   354,
     295,    92,    91,   273,   344,     0,   343,     0,     0,    87,
     443,     0,     0,    35,     0,   378,   368,     0,   354,    36,
     374,     0,   142,   138,     0,   364,   142,   139,     0,   285,
       0,   377,     0,   343,     0,     0,   501,   439,   497,   500,
     496,     0,    47,    50,     0,     0,   349,     0,   351,     0,
       0,   350,     0,   343,     0,     0,     6,    46,     0,   164,
       0,   259,   258,   184,     0,     0,     0,     0,    23,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   438,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   354,   354,     0,     0,     0,    25,
      26,     0,     0,     0,     0,     0,    27,     0,     0,     0,
      28,     0,    29,     0,   362,   360,     0,   355,   356,   361,
       0,     0,     0,     0,   112,     0,     0,   111,     0,     0,
       0,   118,     0,     0,    56,    99,     0,   219,   226,   227,
     228,   223,   225,   221,   224,   222,   220,   230,   229,   128,
       0,     0,    30,   234,   178,   300,     0,   301,   303,     0,
     319,     0,   306,     0,     0,    85,    31,    33,     0,   183,
     272,     0,    63,   441,   442,    88,     0,    34,   354,     0,
     149,   140,   136,   141,   137,     0,   283,   280,    60,     0,
      56,   105,    37,    49,    48,    51,     0,   465,     0,     0,
     456,     0,   458,     0,     0,     0,     0,     0,     0,   469,
       8,     0,     0,     0,     0,   252,     0,     0,     0,     0,
       0,   405,   493,   492,   495,   503,   502,   507,   506,   489,
     486,   487,   488,   436,   476,   455,   454,   453,   437,   480,
     491,   485,   483,   494,   484,   482,   474,   479,   481,   490,
     473,   477,   478,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   505,   504,   509,   508,   417,   418,   420,   422,
       0,   409,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   467,   406,   406,   108,   281,   348,     0,     0,   370,
       0,   282,   164,     0,     0,     0,   381,     0,     0,     0,
     387,     0,     0,     0,   119,   511,    59,     0,    52,    57,
       0,   127,     0,     0,     0,   369,   246,   243,   244,   245,
     249,   250,   251,     0,   235,     0,   242,   260,     0,   304,
       0,   315,   312,   316,   318,   313,   314,   317,     0,   311,
     428,     0,   298,   426,   297,   452,   346,   345,     0,     0,
       0,   371,     0,   143,   287,   428,     0,     0,     0,   466,
     435,   457,   352,   459,   353,     0,     0,   468,   124,   391,
       0,   471,   470,     0,     0,   165,     0,     0,   175,     0,
     172,   256,   253,   254,   257,   185,     0,     0,   289,   288,
     290,   292,    64,    71,    72,    73,    68,    70,    78,    79,
      66,    69,    67,    65,    75,    74,    76,    77,   450,   451,
     419,   421,     0,   408,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   104,   358,   359,
     357,     0,     0,   126,     0,     0,   110,     0,   109,     0,
       0,   116,     0,     0,   114,     0,     0,   412,     0,   100,
       0,   101,     0,     0,   130,     0,   132,   247,   248,   241,
     233,     0,   332,   261,   264,   263,   265,     0,   302,   305,
     306,     0,     0,   307,   308,   151,     0,     0,   150,   153,
     372,     0,   144,   147,     0,   284,    61,    62,     0,     0,
       0,     0,   125,     0,     0,     0,   293,   170,     0,   173,
     169,   255,   260,   216,   214,   195,   198,   196,   197,   208,
     199,   212,   204,   202,   215,   203,   201,   206,   211,   213,
     200,   205,   209,   210,   207,   217,   218,     0,   193,     0,
     231,   231,   191,   291,   410,   306,   444,   444,     0,     0,
       0,     0,     0,     0,     0,     0,   107,   106,     0,   113,
       0,     0,   380,     0,   379,     0,     0,   386,   117,     0,
     385,   115,     0,   411,    54,    53,   129,   396,   131,     0,
     236,     0,     0,   306,   262,   278,   299,   323,     0,   467,
       0,   155,     0,     0,     0,   145,     0,   122,   393,     0,
       0,   120,   392,     0,   472,     0,    39,     0,   159,    80,
     293,   293,   157,   293,   167,   176,   174,     0,   194,     0,
       0,   234,   187,   188,   414,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   293,   384,     0,     0,   390,     0,
       0,   413,     0,     0,   134,   332,   338,   335,   339,   341,
     336,   337,   340,     0,   334,   342,   428,   274,   238,   237,
       0,     0,     0,   321,   428,   156,   154,     0,   148,     0,
     123,     0,   121,    41,    40,   168,   266,   192,   193,   231,
     231,     0,     0,     0,     0,     0,     0,     0,   160,   158,
       0,     0,     0,     0,    55,   133,   306,   324,     0,   275,
     277,   276,   279,   270,   271,   179,     0,   146,     0,     0,
     267,   278,   189,   190,   232,     0,     0,     0,     0,     0,
       0,   383,   382,   389,   388,   240,   239,   326,   327,   329,
     428,     0,   467,   428,   395,   394,     0,     0,   328,   330,
     268,   186,   269,   324,   331
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   243,    82,   541,    84,    85,   258,    86,
     244,   536,   540,   537,    87,    88,    89,   167,    90,   171,
     191,    91,    92,    93,    94,    95,    96,   653,    97,    98,
      99,   401,   583,   714,   100,   101,   579,   709,   102,   103,
     433,   726,   104,   105,   609,   610,   148,   184,   557,   107,
     108,   435,   732,   615,   759,   760,   370,   842,   374,   553,
     554,   555,   556,   616,   263,   697,   931,   961,   925,   204,
     920,   878,   881,   109,   230,   406,   110,   111,   187,   188,
     378,   379,   572,   382,   569,   948,   875,   803,   245,   162,
     249,   250,   336,   337,   338,   149,   113,   114,   115,   150,
     117,   137,   138,   500,   352,   676,   501,   502,   118,   151,
     152,   210,   339,   154,   123,   155,   156,   126,   127,   254,
     128,   129,   130,   131,   132
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -834
static const yytype_int16 yypact[] =
{
    -834,   116,  2915,  -834,   -35,  -834,  -834,  -834,  -834,  -834,
    -834,  4067,    66,   211,  8165,  -834,    66,  8165,    38,   211,
    4067,  8165,  4067,    77,  -834,   346,   610,  6850,  8165,  6969,
    8165,   105,  -834,   211,  -834,    56,  7351,  8165,  8165,  -834,
    8165,  8165,  8165,   252,   166,  1082,  1176,  -834,  7470,  6468,
    8165,  7351,  8165,  8165,   184,   169,  4067,  8165,  8284,  8284,
     211,  -834,  8165,  7470,  8165,  8165,  -834,  -834,  8165,  -834,
    -834,  9712,  8165,  8165,  -834,  8165,  -834,  -834,  3203,  5917,
    7470,  -834,  3923,  -834,  -834,   235,  -834,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,  -834,   211,  -834,   211,   111,   156,  -834,  -834,
    -834,   163,   213,  -834,  -834,  -834,   220,   264,   219,   265,
     276, 13297,  1386,   178,   296,   300,  -834,  -834,  -834,  -834,
    -834,  -834,   327,  -834,  -834, 13297,   226,  4067,  -834,   308,
    -834,  8165,  8165,  8165,  8165,  8165,  7470,  7470,   225,  -834,
    -834,  -834,  -834,   269,   292,  -834,  -834,   302, 11794,   211,
     312,  -834,    49, 13297,   359,  6587,  -834,  -834,  -834,  -834,
     211,   106,   211,   326,    48, 11225, 11191,  -834,  -834,  -834,
   11670, 10515,  6587,  4067,   329,    29,    80,    52,  -834,  4067,
    -834,  -834, 11383,   544,   269, 11383,   269,   544,  -834,  6587,
     426,  -834,  -834,   211,  -834,   109, 13297,  8165,  8165,  -834,
   13297,   336, 11546,  -834, 11383,   269, 13297,   330,  6587,  -834,
   13297, 12025,  -834,  -834, 12115,  1724,  -834,  -834, 12149,   378,
     342,   269,   203, 11828, 11383, 12239,   182,  1512,   544,   182,
     544,   118,  -834,  -834,  3347,   -28,  -834,  8165,  -834,   -23,
     -22,  -834,    91, 12285,   -19,   471,  -834,  -834,   431,   365,
     340,  -834,  -834,  -834,    44,    56,    56,    56,  -834,  8165,
    8165,  8165,  8165,  7614,  7614,  8165,  8165,  8165,  8165,  8165,
    8165,    71,  9712,  8165,  8165,  8165,  8165,  8165,  8165,  8165,
    8165,  8165,  8165,  8165,  8165,  8165,  8165,  8165,  7088,  7088,
    7088,  7088,  7088,  7088,  7088,  7088,  7088,  7088,  7088,  7088,
    7088,  7088,  7088,  7088,  6587,  6587,  7614,  7614,  6349,  -834,
    -834, 11952, 11986, 12323,   100,  3491,  -834,  7614,    48,   350,
    -834,  8165,  -834,  8165,   389,  -834,   345,   376,  -834,  -834,
     360,   211,   456,  7470,  -834,  4211,  7614,  -834,  4355,  7614,
     353,  -834,    48,  8403,  8165,  -834,  4067,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,   470,
    8165,   366,  -834,    95,  -834,  -834,    29,  -834,   398,   372,
    -834,  8522,   421,  8165,    56,  -834,  -834,  -834,   374,  -834,
    -834,  7470,  -834, 13297, 13297,  -834,    13,  -834,  6587,   375,
    -834,   505,  -834,   505,  -834,  8641,   408,  -834,  -834,  8403,
    8165,  -834,  -834,  -834,  -834,  -834,  7207,  -834, 10391,  6061,
    -834,  6205,  -834,  7614,  5482,  3059,   379,  8165,  5772,  -834,
    -834,   211,  7470,   383,   332,   305,    56,   240,   242,   247,
     260, 11636,  2047,  2047,   294,  -834,   294,  -834,   294,  2572,
     847,   888,   463,   269,   182,  -834,  -834,  -834,  1512,  1042,
     294,  2751,  2751,  2047,  2751,  2751,   620,   182,  1042,  2428,
     620,   544,   544,   182,   393,   395,   397,   400,   402,   403,
     404,   405,   409,   410,   415,   418,   419,   420,   422,   425,
     411,   396,  -834,   294,  -834,   294,    63,  -834,  -834,  -834,
      -8,  -834,   211, 13391,   358,  8760,  7614,  8879,  7614,  8165,
    7614,  9995,    66, 12417,  -834,  -834, 13297, 12455,  6587,  -834,
    6587,  -834,   365,  8165,    96,  8165, 13297,    60, 11507,  8165,
   13297,    18, 11349,  6349,  -834,   433,   455,   437, 12501,   455,
     441,   550, 12579,  4067, 11473,  -834,    67,  -834,  -834,  -834,
    -834,  -834,  -834,   113,  -834,    65,  -834,   335,   438,    29,
      80,  8165,    77,  8165,  8165,  8165,  8165,  8165,  2712,  -834,
     304,  6731,  -834, 13297,  -834,  -834,  -834, 13297,   439,    23,
     442,  -834,    50,  -834,  -834,   270,   211,   454,   458,  -834,
    -834,  -834,  -834,  -834,  -834,    90, 10229,  -834,  -834, 13297,
    4067, 13297,  -834, 12625,   448,   475,  1363,   452,   499,   -12,
    -834,   530,  -834,  -834,  -834,  -834, 10057,   275,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,  6349,  -834,    10,  7614,  7614,  8165,   575, 12703,
    8165,   577, 12749,   461,  2309,    48,    48,  -834,  -834,  -834,
    -834,   467, 11383,  -834, 10584,  4499,  -834,  4643,  -834, 10653,
    4787,  -834,    48,  4931,  -834,    48,   120,  -834,  8165,  -834,
    8165,  -834,  4067,  8165,  -834,  4067,   582,  -834,  -834,   323,
    -834,    29,   497,   548,  -834,  -834,  -834,    82,  -834,  -834,
     421,   469,   119,  -834,  -834,  -834,  5075,  7470,  -834,  -834,
    -834,   211,  -834,   503,   302,  -834,  -834,  -834,  5219,   473,
    5363,   474,  -834,  8165,  3635,   479,   451,  -834,  8165,   211,
    -834,  -834,   335,  -834,  -834,  -834,   211,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  8165,   508,   509,
     484,   484,  -834,  -834,  -834,   421,   189,   192, 12787,  8998,
    9117, 12881,  9236,  9355,  9474,  9593,  -834,  -834,  2203,  -834,
    4067,  8165, 13297,  8165, 13297,  4067,  8165, 13297,  -834,  8165,
   13297,  -834,  6349,  -834, 12919, 13297,  -834, 13297,   594,  4067,
    -834,   485,  7733,    12,  -834,   543,  -834,  -834,  7614,  9919,
    4067,  -834,    59,   489,  8165,  -834,  8165,  -834, 13297,  4067,
    8165,  -834, 13297,  4067, 13297,   261,  -834,  3779,  -834,   657,
     288,   288,  -834,  1219,  -834, 13297,  -834,    84,  -834, 10437,
   10105,    95,  -834,  -834,  -834,  8165,  8165,  8165,  8165,  8165,
    8165,  8165,  8165,   494,  1011, 12579, 10722, 10791, 12579, 10860,
   10929,  -834,  8165,  4067,  -834,   497,  8165,    77,  8165,  8165,
    8165,  8165,  8165,  5627,  -834,  -834,   -25,  7470,  -834,  -834,
    8165,    34, 10267,  -834,   742,   359,  -834,   302, 13297, 10998,
    -834, 11067,  -834,  -834,  -834,  -834,   558,  -834,  -834,   484,
     484,   195, 12957, 13051, 13089, 13127, 13221, 13259,  -834,  -834,
    4067,  4067,  4067,  4067, 13297,  -834,    12,  7877,   185,  -834,
    -834, 13297, 13297,  -834,  -834,  -834,  9593,  -834,  4067,  4067,
    -834,   543,  -834,  -834,  -834,  8165,  8165,  8165,  8165,  8165,
    8165, 12579, 12579, 12579, 12579,  -834,  -834,  -834,  -834,  -834,
     134,  7614,  9799,   962, 12579, 12579,    92, 10313,  -834,  -834,
    -834,  -834,  -834,  8021,  -834
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -834,  -834,  -834,    -1,  -661,  1944,  -834,  -834,  -834,  1746,
     -85,  -296,   231,   238,  -834,  -834,   121,   243,  2308,     4,
     806,  -789,  -644,   -38,  -834,  -834,  -834,   -10,  -834,  -834,
    -834,   416,  -834,   -61,  -834,  -834,  -834,  -834,  -834,   480,
     131,  -123,  -834,  -834,  -834,   -73,   874,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  -182,  -612,  -730,  -834,  -172,
     -15,  -834,  -834,  -834,  -834,   -62,  -834,  -834,  -281,   233,
    -834,  -231,  -244,  -834,  -149,  -834,   660,  -834,  -223,   307,
    -834,  -373,  -676,  -538,  -392,  -741,  -833,  -173,    16,  -834,
      21,  -834,  -166,  -834,   174,   363,  -375,  -834,  -834,   937,
    -834,   -11,  -834,  -834,  -229,  -834,  -496,  -834,  -834,  1172,
    1319,    -2,   154,   547,  -834,  1387,  1672,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -316
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -468
static const yytype_int16 yytable[] =
{
     121,    81,   504,   558,   761,   157,   570,   201,   202,   135,
     329,   380,   153,   584,   578,   158,   371,   174,   121,   163,
     121,   340,   700,   426,   806,   175,   176,   180,   181,   168,
     585,   843,     5,   388,   192,   193,   194,   677,   195,   196,
     197,   437,   438,   439,   440,   670,   206,     5,   212,   214,
     215,   216,   399,     5,   220,   221,   224,   228,   539,     5,
     231,   233,   234,   235,   205,   833,   236,  -152,     5,   237,
     238,   239,  -333,   240,     5,   189,   206,   706,   253,   232,
     121,   380,   834,   380,   949,   380,   189,   665,   416,   844,
     383,  -333,   924,   419,   421,  -333,   252,   428,  -241,   455,
     133,   640,  -152,    49,   729,   687,   765,  -309,   642,   571,
     350,   436,   707,   375,   539,   417,     3,   546,  -309,   959,
    -333,   159,   420,   422,    15,   381,  -309,   879,   429,   423,
     641,   730,   877,   547,   688,   121,   548,   643,   510,   135,
     321,   322,   323,   220,   206,   253,   764,   328,   490,   491,
     549,  -309,   550,  -309,   456,  -309,  -152,   808,   136,   805,
     325,    49,   551,   324,  -309,   331,   708,   924,   384,   932,
     933,    47,  -309,   376,  -309,   391,   325,  -309,   552,   223,
     227,   121,   350,   350,   332,   264,   699,   121,   185,   895,
     325,    61,   325,   833,   711,   381,  -309,   381,   261,   381,
     185,   325,   325,   211,   389,   393,   394,   424,   272,   691,
     834,   958,   391,   617,     5,  -309,   424,   504,   262,  -309,
     265,   165,   964,   951,  -309,   391,  -309,   960,   900,   689,
     266,  -325,   580,   251,   325,   424,   792,   718,   425,  -241,
     946,   663,   121,   415,   392,   418,  -402,   511,   273,   182,
    -325,   267,   274,   133,  -325,  -449,   139,  -449,   690,   198,
     217,   413,   160,   445,   447,   793,   809,   441,   442,   443,
     444,   446,   448,   449,   450,   451,   452,   453,   454,  -325,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   861,   279,   257,   896,
     281,   424,   672,   675,   284,  -416,   492,   494,  -415,   177,
     199,   689,   248,   218,   493,   495,   503,   512,   801,   391,
      25,   200,   314,   121,   315,   513,   504,   611,    32,   516,
    -180,   517,   952,   607,  -416,     5,   527,  -415,   408,   531,
     934,   206,  -310,   526,   528,   546,   530,   532,  -399,     5,
    -180,   538,   538,   166,   542,  -398,   384,   693,   384,   524,
     269,   547,   612,   384,   548,   112,   719,   721,   544,  -449,
     318,  -449,   613,  -310,   112,   618,   384,   619,   549,   121,
     550,   573,   620,   112,  -310,   112,  -286,   281,   614,   577,
     551,   384,   694,   316,  -310,   621,   133,   317,   335,   268,
    -403,  -310,   695,   121,   893,  -286,   552,   538,   538,   279,
     763,  -404,   281,   595,   577,   335,   284,   883,   696,   112,
    -310,   596,   577,   599,   646,   601,   603,   876,   317,     5,
     206,  -401,   335,   166,   884,  -400,  -448,   715,  -448,  -310,
     592,   112,   594,   320,   325,   112,  -310,   330,   605,  -310,
     333,   335,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   -80,    15,
     343,   395,    18,   373,   398,   405,   504,   407,   430,   431,
     432,    24,   434,    25,   829,   515,   518,    29,   -80,   272,
     519,    32,   520,  -180,   523,   521,   648,   533,   651,   543,
     112,   655,   656,   526,   649,   530,   652,   542,   654,   599,
      43,   545,   830,  -180,   559,   831,    47,   560,   571,   575,
     581,   662,   582,   664,   586,   606,   600,   669,   622,   273,
     623,   503,   624,   274,    60,   625,    61,   626,   627,   628,
     629,   121,   950,   639,   630,   631,   112,   335,   335,   122,
     632,   953,   112,   633,   634,   635,   638,   636,   122,   153,
     637,   196,   215,   216,   220,   231,   253,   122,   -58,   122,
     272,   678,   679,   251,   276,   251,   681,   950,   279,   682,
     280,   281,   705,   698,   702,   284,    76,   710,   950,   716,
     724,   391,   291,   717,   832,   727,   728,   731,   121,   295,
     296,   297,   769,   122,   772,   225,   225,   112,   774,   778,
     273,   799,   802,     5,   274,   804,   807,   166,   814,   169,
     819,   823,   828,   863,   576,   122,  -362,   840,   841,   122,
     865,   335,  -295,   880,   887,   445,   492,   908,   930,   827,
     503,   588,   403,   446,   493,   768,   272,   587,   771,   576,
     813,   341,   248,   661,   248,   854,   836,   576,   899,   279,
       5,   280,   281,   782,   166,   784,   284,  -295,   787,   901,
     837,   790,   659,   291,   800,   962,   794,  -295,   795,  -295,
     121,   797,   297,   121,   122,   945,   273,   956,   112,   179,
     274,   574,   916,   170,   660,     0,     0,     0,  -295,     0,
       0,     0,     0,     0,   121,   206,     0,     0,   112,     0,
       0,   112,     0,     0,  -295,     0,   818,     0,   822,   112,
       0,   824,   121,   812,  -295,   704,   835,     0,     0,     0,
     122,   276,     0,   277,   278,   279,   122,   280,   281,     0,
     170,     0,   284,     0,     0,  -295,     0,     0,     0,   291,
       0,   658,     0,   335,     0,   839,   295,   296,   297,     0,
       0,     0,     0,     0,     0,     0,     0,   782,   784,     0,
     787,   790,   818,   822,     0,     0,     0,     0,   855,   856,
    -320,   857,     0,   858,   859,     0,     0,   860,   112,     0,
     503,   122,   201,   202,     0,     0,     0,   121,     0,     0,
     121,     0,     0,     0,     0,     0,   882,   599,   121,     0,
       0,  -320,   888,     0,   889,     0,     0,   121,   891,     0,
       0,   121,  -320,     0,     0,   121,   415,     0,     0,     0,
       0,     0,  -320,     0,     0,     0,     0,     0,     0,  -320,
       0,     0,     0,   855,   902,   903,   858,   904,   905,   906,
     907,     0,     0,     0,     0,     0,     0,   213,  -320,     0,
     914,   121,     0,     0,   153,   644,   196,   215,   216,   220,
     231,   253,   122,   272,     0,   921,   106,  -320,   922,     0,
       0,     0,     0,     0,  -320,   106,     0,  -320,     0,   918,
       0,     0,   122,     0,   106,   122,   106,     0,     0,     0,
       0,     0,     0,   122,     0,     0,   112,     0,   941,   942,
     943,   944,     0,   273,   272,   121,     0,   274,     0,     0,
       0,     0,     0,     0,   822,     0,   954,   955,     0,     0,
     106,     0,     0,   941,   942,   943,   944,   954,   955,   116,
       0,     0,     0,     0,     0,     0,     0,     0,   116,   957,
     599,     0,   106,     0,   273,     0,   106,   116,   274,   116,
       0,   822,   279,   112,   280,   281,     0,     0,     0,   284,
       0,     0,   122,     0,     0,     0,   291,     0,     0,   762,
     344,   347,   351,   295,   296,   297,     0,     0,     0,     0,
       0,     0,     0,   116,     0,     0,     0,     0,   386,   276,
    -322,   387,   278,   279,     0,   280,   281,     0,   766,   767,
     284,   106,     0,     0,     0,   116,     0,   291,     0,   116,
     397,     0,     0,     0,   295,   296,   297,     0,   112,    15,
     112,  -322,    18,   112,     0,   874,   112,     0,     0,     0,
     411,    24,  -322,    25,   829,   112,     0,    29,   112,     0,
       0,    32,  -322,  -180,     0,     0,     0,   106,     0,  -322,
       0,     0,     0,   106,     0,     0,     0,     0,   272,   112,
      43,     0,   830,  -180,   116,   831,    47,     0,  -322,     0,
       0,   112,     0,   112,     0,     0,     0,   112,     0,     0,
     122,     0,     0,     0,    60,     0,    61,  -322,     0,     0,
       0,     0,     0,   177,  -322,     0,     0,  -322,   273,     0,
     919,     0,   274,     0,    25,   200,     0,     0,   106,     0,
     116,     0,    32,     0,  -180,     0,   116,     0,     0,     0,
     -44,     0,     0,     0,   514,     0,     0,     0,     0,     0,
       0,     0,     0,   112,  -180,   -44,    76,   122,   112,     0,
     947,     0,     0,   276,   909,   277,   278,   279,   534,   280,
     281,   282,   112,     0,   284,   285,   286,     0,   288,   289,
     290,   291,     0,   112,   119,   294,     0,     0,   295,   296,
     297,   116,   112,   119,     0,   947,   112,     0,     0,     0,
     112,     0,   119,     0,   119,     0,   947,   177,     0,   106,
       0,     0,     0,     0,     0,     0,     0,     0,    25,   200,
       0,     0,   122,     0,   122,     0,    32,   122,  -180,   106,
     122,     0,   106,     0,   -43,     0,   112,     0,   119,   122,
     106,     0,   122,     0,     0,     0,     0,    15,  -180,   -43,
      18,     0,     0,     0,     0,     0,     0,     0,     0,    24,
     119,    25,   829,   122,   119,    29,     0,     0,     0,    32,
       0,  -180,   116,     0,     0,   122,     0,   122,     0,     0,
       0,   122,     0,   112,   112,   112,   112,     0,   255,     0,
     830,  -180,   116,   831,    47,   116,     0,     0,     0,     0,
       0,   112,   112,   116,     0,     0,     0,     0,     0,   106,
       0,     0,    60,     0,    61,     0,     0,     0,     0,   119,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   120,     0,     0,     0,     0,     0,   122,     0,     0,
     120,     0,   122,   666,   668,     0,     0,   671,   674,   120,
       0,   120,     0,     0,     0,     0,   122,     0,     0,     0,
       0,     0,     0,     0,    76,   119,     0,   122,     0,     0,
       0,   119,   116,     0,   725,     0,   122,     0,     0,     0,
     122,     0,     0,     0,   122,   120,     0,     0,     0,     0,
       0,  -166,     0,     0,  -166,  -166,     0,     0,     0,   124,
       0,     0,     0,  -166,     0,  -166,  -166,   120,   124,  -166,
       0,   120,     0,  -166,     0,  -166,     0,   124,     0,   124,
     122,     0,     0,     0,     0,     0,   119,   106,     0,     0,
    -166,     0,  -166,     0,  -166,  -166,     0,  -166,  -166,     0,
    -166,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   124,     0,     0,  -166,     0,  -166,     0,
       0,  -166,     0,     0,     0,     0,   120,   122,   122,   122,
     122,   776,   777,     0,     0,   124,     0,     0,   779,   124,
       0,     0,     0,     0,   106,   122,   122,     0,   788,     0,
     116,   791,     0,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   119,  -166,     0,
       0,     0,   120,     0,     0,     0,  -166,     0,   120,     0,
       0,     0,     0,     0,     0,     0,     0,   119,     0,     0,
     119,     0,     0,     0,   124,   312,   313,  -423,   119,     0,
    -448,     0,  -448,     0,     0,     0,     0,   116,   272,   106,
       0,   106,     0,     0,   106,     0,     0,   106,     0,     0,
       0,     0,     0,     0,     0,     0,   106,     0,     0,   106,
       0,     0,     0,   120,     0,     0,     0,     0,     0,     0,
     124,     0,     0,     0,     0,     0,   124,     0,   273,     0,
     106,     0,   274,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   106,     0,   106,     0,     0,   119,   106,     0,
       0,     0,   116,     0,   116,     0,     0,   116,     0,     0,
     116,     0,     0,     0,     0,     0,     0,     0,   886,   116,
       0,     0,   116,   276,     0,   277,   278,   279,     0,   280,
     281,   124,     0,     0,   284,     0,     0,     0,     0,     0,
     290,   291,     0,   116,   120,   294,     0,     0,   295,   296,
     297,     0,     0,     0,   106,   116,     0,   116,     0,   106,
       0,   116,     0,     0,   120,     0,     0,   120,     0,     0,
       0,     0,     0,   106,   125,   120,     0,     0,     0,     0,
       0,     0,     0,   125,   106,     0,     0,     0,     0,     0,
       0,     0,   125,   106,   125,     0,     0,   106,     0,     0,
       0,   106,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   124,     0,     0,   119,     0,   116,     0,     0,
       0,     0,   116,     0,     0,     0,     0,     0,   125,     0,
       0,     0,   124,     0,     0,   124,   116,   106,     0,     0,
       0,     0,     0,   124,   120,     0,     0,   116,     0,     0,
     125,     0,     0,     0,   125,     0,   116,     0,     0,     0,
     116,     0,     0,     0,   116,     0,     0,     0,     0,     0,
       0,     0,   119,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   190,     0,   106,   106,   106,   106,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   190,     0,     0,
     116,     0,   106,   106,   222,   226,     0,     0,     0,   125,
       0,     0,   124,     0,     0,     0,     0,     0,     0,     0,
       0,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,     0,     0,   119,     0,   119,
       0,     0,   119,     0,     0,   119,     0,   116,   116,   116,
     116,     0,     0,     0,   119,   125,     0,   119,     0,     0,
       0,   125,   120,   312,   313,   116,   116,     0,  -448,     0,
    -448,     0,     0,     0,     0,     0,     0,     0,   119,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     119,     0,   119,     0,     0,     0,   119,     0,     0,     0,
       0,     0,     0,   326,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   125,     0,     0,   120,
     190,   190,   190,     0,     0,     0,     0,   369,     0,     0,
     124,     0,     0,     0,     0,     0,     0,     0,   190,     0,
       0,   190,     0,     0,     0,     0,    83,     0,     0,     0,
       0,     0,   119,     0,     0,   134,     0,   119,     0,     0,
     190,     0,     0,     0,   161,     0,   164,     0,     0,     0,
       0,   119,     0,     0,     0,     0,     0,     0,     0,     0,
     190,     0,   119,     0,   120,     0,   120,   124,     0,   120,
       0,   119,   120,     0,     0,   119,     0,   125,     0,   119,
     219,   120,     0,     0,   120,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   125,     0,     0,
     125,     0,    83,     0,     0,   120,   256,     0,   125,     0,
       0,     0,     0,     0,     0,   119,     0,   120,     0,   120,
       0,     0,     0,   120,     0,     0,     0,     0,     0,     0,
       0,     0,   124,     0,   124,     0,     0,   124,     0,     0,
     124,     0,     0,     0,     0,     0,     0,     0,     0,   124,
       0,     0,   124,   272,   190,     0,     0,     0,     0,     0,
       0,   319,   119,   119,   119,   119,     0,     0,     0,     0,
       0,     0,     0,   124,     0,     0,     0,   125,   190,   120,
     119,   119,     0,     0,   120,   124,     0,   124,     0,     0,
       0,   124,     0,   273,     0,     0,     0,   274,   120,     0,
       0,     0,     0,     0,     0,     0,     0,   372,     0,   120,
       0,     0,     0,   385,     0,     0,     0,     0,   120,     0,
       0,     0,   120,   275,     0,     0,   120,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   276,     0,
     277,   278,   279,     0,   280,   281,   282,   124,   283,   284,
     285,   286,   124,   288,   289,   290,   291,     0,   292,   293,
     294,     0,   120,   295,   296,   297,   124,     0,    83,     0,
       0,     0,     0,     0,     0,     0,     0,   124,     0,     0,
       0,     0,     0,     0,   853,     0,   124,     0,     0,     0,
     124,     0,     0,     0,   124,   125,     0,     0,     0,     0,
       0,  -166,     0,     0,  -166,  -166,     0,     0,     0,   120,
     120,   120,   120,  -166,     0,  -166,  -166,     0,     0,  -166,
       0,     0,     0,  -166,     0,  -166,     0,   120,   120,     0,
     124,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -166,     0,  -166,     0,  -166,  -166,     0,  -166,  -166,    83,
    -166,     0,   125,   190,   190,     0,     0,   190,   190,     0,
       0,     0,     0,     0,     0,     0,  -166,     0,  -166,   385,
     686,  -166,   385,     0,     0,     0,     0,   124,   124,   124,
     124,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   124,   124,     0,     0,   270,
       0,   140,     0,     0,     0,   271,     0,   140,   712,     0,
       0,     0,     0,   140,   140,   272,     0,   125,  -166,   125,
       0,   183,   125,   186,     0,   125,  -166,     0,     0,     0,
       0,     0,     0,     0,   125,     0,     0,   125,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   229,   598,
       0,     0,     0,     0,     0,   273,     0,     0,   125,   274,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     125,     0,   125,     0,     0,     0,   125,     0,     0,     0,
       0,   190,   190,     0,     0,   275,     0,     0,   190,     0,
     369,   259,     0,   260,     0,   369,     0,     0,   190,     0,
     276,   190,   277,   278,   279,     0,   280,   281,   282,     0,
     283,   284,   285,   286,   287,   288,   289,   290,   291,     0,
     292,   293,   294,     0,     0,   295,   296,   297,     0,     0,
       0,     0,   125,     0,   272,     0,   775,   125,     0,     0,
     815,     0,     0,     0,     0,     0,     0,   229,     0,     0,
       0,   125,     0,   334,     0,     0,     0,     0,   229,     0,
     342,     0,   125,     0,     0,     0,     0,   684,     0,     0,
     334,   125,     0,   377,   273,   125,     0,     0,   274,   125,
       0,     0,     0,     0,     0,     0,     0,   334,   140,     0,
       0,   390,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   275,     0,   334,     0,     0,     0,
       0,     0,     0,     0,     0,   125,     0,     0,     0,   276,
       0,   277,   278,   279,   722,   280,   281,   282,     0,   283,
     284,   285,   286,     0,   288,   289,   290,   291,   190,   292,
       0,   294,     0,     0,   295,   296,   297,     0,     0,     0,
       0,     0,   186,   186,   186,   186,     0,     0,     0,     0,
       0,     0,   125,   125,   125,   125,     0,     0,     0,   457,
       0,     0,     0,     0,     0,     0,     0,     0,   272,     0,
     125,   125,   369,   369,     0,   369,   369,     0,     0,   385,
       0,   385,     0,     0,   385,     0,     0,   385,     0,     0,
       0,     0,   334,   334,     0,     0,   796,   923,     0,   798,
       0,     0,     0,   927,     0,   369,     0,   369,   273,     0,
       0,     0,   274,     0,     0,     0,     0,     0,     0,   522,
     811,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   817,     0,   821,     0,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   276,   377,   277,   278,   279,     0,   280,
     281,   282,   186,   283,   284,   285,   286,     0,   288,   289,
     290,   291,   923,   292,     0,   294,   334,     0,   295,   296,
     297,     0,     0,   701,     0,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   141,     0,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   604,
      23,     0,   608,   864,   186,     0,   142,   143,     0,   144,
       0,    31,     0,     0,   885,     0,    34,    35,     0,    37,
       0,    38,    39,   890,     0,     0,     0,   892,    41,     0,
      42,    83,    44,     0,     0,     0,     0,   272,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,   145,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,   915,    66,     0,
       0,     0,     0,     0,     0,     0,     0,   273,     0,     0,
       0,   274,     0,    67,    68,    69,    70,     0,   334,     0,
       0,    71,     0,     0,     0,     0,     0,     0,     0,     0,
      72,     0,    73,     0,    74,    75,   203,     0,     0,     0,
      77,     0,     0,     0,   146,     0,    79,     0,   147,     0,
       0,     0,   276,   692,   277,   278,   279,   377,   280,   281,
     282,     0,     0,   284,     0,     0,     0,     0,     0,   290,
     291,     0,     0,     0,   294,     0,     0,   295,   296,   297,
     713,     0,     0,     0,   229,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    -2,     4,     0,     5,     6,
       7,     8,     9,    10,   758,     0,     0,    11,    12,    13,
      14,     0,     0,    15,    16,    17,    18,  -293,    19,    20,
      21,     0,    22,    23,     0,    24,     0,    25,    26,    27,
      28,    29,    30,     0,    31,    32,     0,  -180,    33,    34,
      35,    36,    37,   -42,    38,    39,     0,    40,     0,     0,
       0,    41,  -293,    42,    43,    44,    45,  -180,   -42,    46,
      47,     0,  -293,    48,    49,     0,    50,    51,    52,    53,
      54,    55,    56,     0,    57,     0,    58,    59,    60,   377,
      61,    62,    63,  -293,     0,     0,    64,     0,    65,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,   713,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
       0,     0,     0,     0,    71,     0,     0,   608,     0,     0,
       0,     0,     0,    72,   838,    73,     0,    74,    75,     0,
      76,     0,     0,    77,     0,     0,     0,    78,     0,    79,
       4,    80,     5,     6,     7,     8,     9,    10,     0,  -467,
       0,    11,    12,    13,    14,  -467,     0,    15,    16,    17,
      18,  -293,    19,    20,    21,  -467,    22,    23,  -467,    24,
       0,    25,    26,    27,    28,    29,    30,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,  -293,    42,     0,    44,
      45,  -180,   -42,    46,    47,  -467,  -293,    48,    49,  -467,
      50,    51,    52,    53,    54,    55,    56,   140,    57,     0,
      58,    59,    60,     0,    61,    62,    63,  -293,   898,     0,
      64,     0,    65,     0,     0,  -467,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -467,    68,  -467,  -467,  -467,  -467,  -467,  -467,  -467,     0,
    -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,    73,
    -467,  -467,  -467,     0,    76,  -467,  -467,  -467,     0,     0,
       0,    78,  -467,    79,   241,    80,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,     0,
       0,    15,    16,    17,    18,  -293,    19,    20,    21,     0,
      22,    23,     0,    24,     0,    25,    26,    27,    28,    29,
      30,     0,    31,    32,     0,  -180,    33,    34,    35,    36,
      37,   -42,    38,    39,     0,    40,     0,     0,     0,    41,
    -293,    42,    43,    44,    45,  -180,   -42,    46,    47,     0,
    -293,    48,    49,     0,    50,    51,    52,    53,    54,    55,
      56,     0,    57,     0,    58,    59,    60,     0,    61,    62,
      63,  -293,     0,     0,    64,     0,    65,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,     0,     0,
       0,     0,    71,     0,     0,     0,     0,     0,     0,     0,
       0,    72,     0,    73,     0,    74,    75,   203,    76,     0,
       0,    77,     0,     0,     0,    78,   242,    79,     4,    80,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,     0,     0,    15,    16,    17,    18,  -293,
      19,    20,    21,     0,    22,    23,     0,    24,     0,    25,
      26,    27,    28,    29,    30,     0,    31,    32,     0,  -180,
      33,    34,    35,    36,    37,   -42,    38,    39,     0,    40,
       0,     0,     0,    41,  -293,    42,    43,    44,    45,  -180,
     -42,    46,    47,     0,  -293,    48,    49,     0,    50,    51,
      52,    53,    54,    55,    56,     0,    57,     0,    58,    59,
      60,     0,    61,    62,    63,  -293,     0,     0,    64,     0,
      65,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,     0,     0,     0,     0,    71,     0,     0,     0,
       0,     0,     0,     0,     0,    72,     0,    73,     0,    74,
      75,     0,    76,     0,     0,    77,     0,     0,     0,    78,
     414,    79,   241,    80,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,     0,     0,    15,
      16,    17,    18,  -293,    19,    20,    21,     0,    22,    23,
       0,    24,     0,    25,    26,    27,    28,    29,    30,     0,
      31,    32,     0,  -180,    33,    34,    35,    36,    37,   -42,
      38,    39,     0,    40,     0,     0,     0,    41,  -293,    42,
      43,    44,    45,  -180,   -42,    46,    47,     0,  -293,    48,
      49,     0,    50,    51,    52,    53,    54,    55,    56,     0,
      57,     0,    58,    59,    60,     0,    61,    62,    63,  -293,
       0,     0,    64,     0,    65,     0,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,     0,     0,     0,     0,
      71,     0,     0,     0,     0,     0,     0,     0,     0,    72,
       0,    73,     0,    74,    75,     0,    76,     0,     0,    77,
       0,     0,     0,    78,   242,    79,   825,    80,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,     0,     0,    15,    16,    17,    18,  -293,    19,    20,
      21,     0,    22,    23,     0,    24,     0,    25,    26,    27,
      28,    29,    30,     0,    31,    32,     0,  -180,    33,    34,
      35,    36,    37,   -42,    38,    39,     0,    40,     0,     0,
       0,    41,  -293,    42,    43,    44,    45,  -180,   -42,    46,
      47,     0,  -293,    48,    49,     0,    50,    51,    52,    53,
      54,    55,    56,     0,    57,     0,    58,    59,    60,     0,
      61,    62,    63,  -293,     0,     0,    64,     0,    65,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
       0,     0,     0,     0,    71,     0,     0,     0,     0,     0,
       0,     0,     0,    72,     0,    73,     0,    74,    75,     0,
      76,     0,     0,    77,     0,     0,     0,    78,   826,    79,
       4,    80,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -293,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,    30,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,  -293,    42,    43,    44,
      45,  -180,   -42,    46,    47,     0,  -293,    48,    49,     0,
      50,    51,    52,    53,    54,    55,    56,     0,    57,     0,
      58,    59,    60,     0,    61,    62,    63,  -293,     0,     0,
      64,     0,    65,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,     0,     0,     0,     0,    71,     0,
       0,     0,     0,     0,     0,     0,     0,    72,     0,    73,
       0,    74,    75,     0,    76,     0,     0,    77,     0,     0,
       0,    78,   894,    79,     4,    80,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,     0,
       0,    15,    16,    17,    18,  -293,    19,    20,    21,     0,
      22,    23,     0,    24,     0,    25,    26,    27,    28,    29,
      30,     0,    31,    32,     0,  -180,    33,    34,    35,    36,
      37,   -42,    38,    39,     0,    40,     0,     0,     0,    41,
    -293,    42,   255,    44,    45,  -180,   -42,    46,    47,     0,
    -293,    48,    49,     0,    50,    51,    52,    53,    54,    55,
      56,     0,    57,     0,    58,    59,    60,     0,    61,    62,
      63,  -293,     0,     0,    64,     0,    65,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,     0,     0,
       0,     0,    71,     0,     0,     0,     0,     0,     0,     0,
       0,    72,     0,    73,     0,    74,    75,     0,    76,     0,
       0,    77,     0,     0,     0,    78,     0,    79,     4,    80,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,     0,     0,    15,    16,    17,    18,  -293,
      19,    20,    21,     0,    22,    23,     0,    24,     0,    25,
      26,    27,    28,    29,    30,     0,    31,    32,     0,  -180,
      33,    34,    35,    36,    37,   -42,    38,    39,     0,    40,
       0,     0,     0,    41,  -293,    42,     0,    44,    45,  -180,
     -42,    46,    47,     0,  -293,    48,    49,     0,    50,    51,
      52,    53,    54,    55,    56,     0,    57,     0,    58,    59,
      60,     0,    61,    62,    63,  -293,     0,     0,    64,     0,
      65,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,     0,     0,     0,     0,    71,     0,     0,     0,
       0,     0,     0,     0,     0,    72,     0,    73,     0,    74,
      75,     0,    76,     0,     0,    77,     0,     0,     0,    78,
       0,    79,     4,    80,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,     0,     0,    15,
      16,    17,    18,  -293,    19,    20,    21,     0,    22,    23,
       0,    24,     0,    25,    26,    27,    28,    29,   525,     0,
      31,    32,     0,  -180,    33,    34,    35,    36,    37,   -42,
      38,    39,     0,    40,     0,     0,     0,    41,  -293,    42,
       0,    44,    45,  -180,   -42,    46,    47,     0,  -293,    48,
      49,     0,    50,    51,    52,    53,    54,    55,    56,     0,
      57,     0,    58,    59,    60,     0,    61,    62,    63,  -293,
       0,     0,    64,     0,    65,     0,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,     0,     0,     0,     0,
      71,     0,     0,     0,     0,     0,     0,     0,     0,    72,
       0,    73,     0,    74,    75,     0,    76,     0,     0,    77,
       0,     0,     0,    78,     0,    79,     4,    80,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,     0,     0,    15,    16,    17,    18,  -293,    19,    20,
      21,     0,    22,    23,     0,    24,     0,    25,    26,    27,
      28,    29,   529,     0,    31,    32,     0,  -180,    33,    34,
      35,    36,    37,   -42,    38,    39,     0,    40,     0,     0,
       0,    41,  -293,    42,     0,    44,    45,  -180,   -42,    46,
      47,     0,  -293,    48,    49,     0,    50,    51,    52,    53,
      54,    55,    56,     0,    57,     0,    58,    59,    60,     0,
      61,    62,    63,  -293,     0,     0,    64,     0,    65,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
       0,     0,     0,     0,    71,     0,     0,     0,     0,     0,
       0,     0,     0,    72,     0,    73,     0,    74,    75,     0,
      76,     0,     0,    77,     0,     0,     0,    78,     0,    79,
       4,    80,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -293,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,   781,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,  -293,    42,     0,    44,
      45,  -180,   -42,    46,    47,     0,  -293,    48,    49,     0,
      50,    51,    52,    53,    54,    55,    56,     0,    57,     0,
      58,    59,    60,     0,    61,    62,    63,  -293,     0,     0,
      64,     0,    65,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,     0,     0,     0,     0,    71,     0,
       0,     0,     0,     0,     0,     0,     0,    72,     0,    73,
       0,    74,    75,     0,    76,     0,     0,    77,     0,     0,
       0,    78,     0,    79,     4,    80,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,     0,
       0,    15,    16,    17,    18,  -293,    19,    20,    21,     0,
      22,    23,     0,    24,     0,    25,    26,    27,    28,    29,
     783,     0,    31,    32,     0,  -180,    33,    34,    35,    36,
      37,   -42,    38,    39,     0,    40,     0,     0,     0,    41,
    -293,    42,     0,    44,    45,  -180,   -42,    46,    47,     0,
    -293,    48,    49,     0,    50,    51,    52,    53,    54,    55,
      56,     0,    57,     0,    58,    59,    60,     0,    61,    62,
      63,  -293,     0,     0,    64,     0,    65,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,     0,     0,
       0,     0,    71,     0,     0,     0,     0,     0,     0,     0,
       0,    72,     0,    73,     0,    74,    75,     0,    76,     0,
       0,    77,     0,     0,     0,    78,     0,    79,     4,    80,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,     0,     0,    15,    16,    17,    18,  -293,
      19,    20,    21,     0,    22,    23,     0,    24,     0,    25,
      26,    27,    28,    29,   786,     0,    31,    32,     0,  -180,
      33,    34,    35,    36,    37,   -42,    38,    39,     0,    40,
       0,     0,     0,    41,  -293,    42,     0,    44,    45,  -180,
     -42,    46,    47,     0,  -293,    48,    49,     0,    50,    51,
      52,    53,    54,    55,    56,     0,    57,     0,    58,    59,
      60,     0,    61,    62,    63,  -293,     0,     0,    64,     0,
      65,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,     0,     0,     0,     0,    71,     0,     0,     0,
       0,     0,     0,     0,     0,    72,     0,    73,     0,    74,
      75,     0,    76,     0,     0,    77,     0,     0,     0,    78,
       0,    79,     4,    80,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,     0,     0,    15,
      16,    17,    18,  -293,    19,    20,    21,     0,    22,    23,
       0,    24,     0,    25,    26,    27,    28,    29,   789,     0,
      31,    32,     0,  -180,    33,    34,    35,    36,    37,   -42,
      38,    39,     0,    40,     0,     0,     0,    41,  -293,    42,
       0,    44,    45,  -180,   -42,    46,    47,     0,  -293,    48,
      49,     0,    50,    51,    52,    53,    54,    55,    56,     0,
      57,     0,    58,    59,    60,     0,    61,    62,    63,  -293,
       0,     0,    64,     0,    65,     0,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,     0,     0,     0,     0,
      71,     0,     0,     0,     0,     0,     0,     0,     0,    72,
       0,    73,     0,    74,    75,     0,    76,     0,     0,    77,
       0,     0,     0,    78,     0,    79,     4,    80,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,     0,     0,    15,    16,    17,    18,  -293,    19,    20,
      21,     0,   810,    23,     0,    24,     0,    25,    26,    27,
      28,    29,    30,     0,    31,    32,     0,  -180,    33,    34,
      35,    36,    37,   -42,    38,    39,     0,    40,     0,     0,
       0,    41,  -293,    42,     0,    44,    45,  -180,   -42,    46,
      47,     0,  -293,    48,    49,     0,    50,    51,    52,    53,
      54,    55,    56,     0,    57,     0,    58,    59,    60,     0,
      61,    62,    63,  -293,     0,     0,    64,     0,    65,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
       0,     0,     0,     0,    71,     0,     0,     0,     0,     0,
       0,     0,     0,    72,     0,    73,     0,    74,    75,     0,
      76,     0,     0,    77,     0,     0,     0,    78,     0,    79,
       4,    80,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -293,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,   816,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,  -293,    42,     0,    44,
      45,  -180,   -42,    46,    47,     0,  -293,    48,    49,     0,
      50,    51,    52,    53,    54,    55,    56,     0,    57,     0,
      58,    59,    60,     0,    61,    62,    63,  -293,     0,     0,
      64,     0,    65,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,     0,     0,     0,     0,    71,     0,
       0,     0,     0,     0,     0,     0,     0,    72,     0,    73,
       0,    74,    75,     0,    76,     0,     0,    77,     0,     0,
       0,    78,     0,    79,     4,    80,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,     0,
       0,    15,    16,    17,    18,  -293,    19,    20,    21,     0,
      22,    23,     0,    24,     0,    25,    26,    27,    28,    29,
     820,     0,    31,    32,     0,  -180,    33,    34,    35,    36,
      37,   -42,    38,    39,     0,    40,     0,     0,     0,    41,
    -293,    42,     0,    44,    45,  -180,   -42,    46,    47,     0,
    -293,    48,    49,     0,    50,    51,    52,    53,    54,    55,
      56,     0,    57,     0,    58,    59,    60,     0,    61,    62,
      63,  -293,     0,     0,    64,     0,    65,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,     0,     0,
       0,     0,    71,     0,     0,     5,     6,     7,     8,     9,
      10,    72,     0,    73,   141,    74,    75,    14,    76,     0,
       0,    77,     0,     0,     0,    78,     0,    79,     0,    80,
      23,     0,     0,     0,     0,     0,   142,   143,     0,   144,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,    41,     0,
      42,     0,    44,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,   145,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,     0,     0,     0,
       0,    71,     0,     0,     0,     0,     0,     0,     0,     0,
      72,     0,    73,     0,    74,    75,   203,     0,     0,     0,
      77,     0,     0,     0,   146,     0,    79,     0,   147,   597,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   141,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,     0,     0,     0,     0,
       0,   142,   143,     0,   144,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,    41,     0,    42,     0,    44,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,   145,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,     0,     0,     0,     0,    71,     0,     0,     0,
       0,     0,     0,     0,     0,    72,     0,    73,     0,    74,
      75,   203,     0,     0,     0,    77,     0,     0,     0,   146,
       0,    79,     0,   147,   917,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   141,     0,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,   142,   143,     0,   144,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,    41,     0,
      42,     0,    44,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,   145,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,     0,     0,     0,
       0,    71,     0,     0,     0,     0,     0,     0,     0,     0,
      72,     0,    73,     0,    74,    75,     0,     0,     0,     0,
      77,     0,     0,     0,   146,     0,    79,     0,   147,   602,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   141,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,     0,     0,     0,     0,
       0,   142,   143,     0,   144,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,    41,     0,    42,     0,    44,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,   145,     0,     0,     0,   207,   208,
       0,   246,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,     0,     0,     0,     0,    71,   247,     0,     0,
       0,     0,     0,     0,     0,    72,     0,    73,     0,    74,
      75,   203,     0,     0,     0,    77,     0,     0,     0,   146,
       0,    79,     0,   147,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   141,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
       0,     0,     0,     0,     0,   142,   143,     0,   144,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,    41,     0,    42,
       0,    44,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,    53,    54,    55,   145,     0,
       0,     0,   207,   208,     0,   246,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,     0,     0,     0,     0,
      71,     0,     0,     0,     0,     0,     0,     0,     0,    72,
       0,    73,     0,    74,    75,   203,     0,     0,     0,    77,
       0,     0,     0,   146,     0,    79,   591,   147,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   141,     0,     0,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,     0,     0,     0,     0,     0,   142,
     143,     0,   144,     0,    31,     0,     0,     0,     0,    34,
      35,     0,    37,     0,    38,    39,     0,     0,     0,     0,
       0,    41,     0,    42,     0,    44,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,    53,
      54,    55,   145,     0,     0,     0,   207,   208,     0,   246,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
       0,     0,     0,     0,    71,     0,     0,     0,     0,     0,
       0,     0,     0,    72,     0,    73,     0,    74,    75,   203,
       0,     0,     0,    77,     0,     0,     0,   146,     0,    79,
     593,   147,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   141,     0,     0,    14,     0,     0,     0,     0,     0,
       0,   496,     0,     0,     0,     0,     0,    23,     0,     0,
       0,     0,     0,   142,   143,     0,   144,   497,    31,     0,
       0,     0,     0,    34,    35,     0,    37,     0,    38,    39,
       0,     0,     0,     0,     0,    41,     0,    42,     0,    44,
       0,     0,     0,     0,     0,     0,   498,     0,     0,     0,
       0,     0,    52,    53,    54,    55,   145,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,   499,     0,     0,
       0,     0,     0,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,     0,     0,     0,     0,    71,     0,
       0,     5,     6,     7,     8,     9,    10,    72,     0,    73,
     141,    74,    75,    14,     0,     0,     0,    77,     0,     0,
       0,   146,     0,    79,     0,   147,    23,     0,     0,     0,
       0,     0,   142,   143,     0,   144,     0,    31,     0,     0,
       0,     0,    34,    35,     0,    37,     0,    38,    39,     0,
       0,     0,     0,     0,    41,     0,    42,     0,    44,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,   145,     0,     0,     0,   207,
     208,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,     0,     0,     0,     0,    71,     0,     0,
       5,     6,     7,     8,     9,    10,    72,     0,    73,   141,
      74,    75,    14,   209,     0,     0,    77,     0,     0,     0,
     146,     0,    79,     0,   147,    23,     0,     0,     0,     0,
       0,   142,   143,     0,   144,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,    41,     0,    42,     0,    44,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,   145,     0,     0,     0,   207,   208,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,     0,     0,     0,     0,    71,     0,     0,     0,
       0,     0,     0,     0,     0,    72,     0,    73,     0,    74,
      75,   203,     0,     0,     0,    77,     0,     0,     0,   146,
       0,    79,     0,   147,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   141,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
       0,     0,     0,     0,     0,   142,   143,     0,   144,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,   703,     0,     0,     0,     0,    41,     0,    42,
       0,    44,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,    53,    54,    55,   145,     0,
       0,     0,   207,   208,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,     0,     0,     0,     0,
      71,     0,     0,     5,     6,     7,     8,     9,    10,    72,
       0,    73,   141,    74,    75,    14,     0,     0,     0,    77,
       0,     0,     0,   146,     0,    79,     0,   147,    23,     0,
       0,     0,     0,     0,   142,   143,     0,   144,     0,    31,
       0,     0,     0,     0,    34,    35,     0,    37,     0,    38,
      39,     0,     0,     0,     0,     0,    41,   172,    42,     0,
      44,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,    53,    54,    55,   145,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   173,     0,    66,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,     0,     0,     0,     0,    71,
       0,     0,     5,     6,     7,     8,     9,    10,    72,     0,
      73,   141,    74,    75,    14,     0,     0,     0,    77,     0,
     177,     0,   146,     0,    79,     0,   147,    23,     0,     0,
       0,     0,   178,   142,   143,     0,   144,     0,    31,     0,
       0,     0,     0,    34,    35,     0,    37,     0,    38,    39,
       0,     0,     0,     0,     0,    41,     0,    42,     0,    44,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,    53,    54,    55,   145,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,     0,     0,     0,     0,    71,     0,
       0,     5,     6,     7,     8,     9,    10,    72,     0,    73,
     141,    74,    75,    14,     0,     0,     0,    77,     0,     0,
       0,   146,     0,    79,     0,   147,    23,     0,     0,     0,
       0,     0,   142,   143,     0,   144,     0,    31,     0,     0,
       0,     0,    34,    35,     0,    37,     0,    38,    39,     0,
       0,     0,     0,     0,    41,     0,    42,     0,    44,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,   145,     0,     0,     0,   207,
     208,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,     0,     0,     0,     0,    71,     0,     0,
       5,     6,     7,     8,     9,    10,    72,     0,    73,   141,
      74,    75,    14,     0,     0,     0,    77,     0,     0,     0,
     146,     0,    79,     0,   147,    23,     0,     0,     0,     0,
       0,   142,   143,     0,   144,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,    41,     0,    42,     0,    44,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,   145,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,     0,     0,     0,     0,    71,     0,     0,     0,
       0,     0,     0,     0,     0,    72,     0,    73,     0,    74,
      75,   203,     0,     0,     0,    77,     0,     0,     0,   146,
     589,    79,     0,   147,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   141,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   189,    23,
       0,     0,     0,     0,     0,   142,   143,     0,   144,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,    41,     0,    42,
       0,    44,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,    53,    54,    55,   145,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,     0,     0,     0,     0,
      71,     0,     0,     5,     6,     7,     8,     9,    10,    72,
       0,    73,   141,    74,    75,    14,     0,     0,     0,    77,
       0,     0,     0,    78,     0,    79,     0,   147,    23,     0,
       0,     0,     0,     0,   142,   143,     0,   144,     0,    31,
       0,     0,     0,     0,    34,    35,     0,    37,     0,    38,
      39,     0,     0,     0,     0,     0,    41,     0,    42,     0,
      44,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,    53,    54,    55,   145,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,     0,     0,     0,     0,    71,
       0,     0,     0,     0,     0,     0,     0,     0,    72,     0,
      73,     0,    74,    75,   203,     0,     0,     0,    77,     0,
       0,     0,   146,     0,    79,     0,   147,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   141,     0,     0,    14,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    23,     0,     0,     0,     0,     0,   142,   143,
       0,   144,     0,    31,     0,     0,     0,     0,    34,    35,
       0,    37,     0,    38,    39,     0,     0,     0,     0,     0,
      41,     0,    42,     0,    44,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,    53,    54,
      55,   145,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   173,     0,
      66,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,     0,
       0,     0,     0,    71,     0,     0,     5,     6,     7,     8,
       9,    10,    72,     0,    73,   141,    74,    75,   866,     0,
       0,     0,    77,     0,     0,     0,   146,     0,    79,     0,
     147,   867,     0,     0,     0,     0,     0,   142,   143,     0,
     144,     0,    31,     0,     0,     0,     0,    34,    35,     0,
      37,     0,    38,    39,     0,     0,     0,     0,     0,   868,
       0,    42,     0,    44,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   869,   870,    54,    55,
     871,     0,     0,     0,     0,     0,     0,     0,     0,   872,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,     0,     0,
       0,     0,    71,     0,     0,     0,     0,     0,     0,     0,
       0,    72,     0,    73,     0,    74,    75,   203,     0,     0,
       0,    77,     0,     0,     0,   146,     0,    79,     0,   873,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   141,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,     0,     0,     0,     0,
       0,   142,   143,     0,   144,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,    41,     0,    42,     0,    44,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,   145,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,     0,     0,     0,     0,    71,     0,     0,     0,
       0,     0,     0,     0,     0,    72,     0,    73,     0,    74,
      75,   203,     0,     0,     0,    77,     0,     0,     0,   146,
       0,    79,     0,   873,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   141,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
       0,     0,     0,     0,     0,   142,   143,     0,   852,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,    41,     0,    42,
       0,    44,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,    53,    54,    55,   145,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,     0,     0,     0,     0,
      71,     0,     0,     0,     0,     0,     0,     0,     0,    72,
       0,    73,     0,    74,    75,   203,     0,     0,     0,    77,
       0,     0,     0,   146,     0,    79,     0,   147,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   141,     0,     0,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,     0,     0,     0,     0,     0,   142,
     143,     0,   144,     0,    31,     0,     0,     0,     0,    34,
      35,     0,    37,     0,    38,    39,     0,     0,     0,     0,
       0,    41,     0,    42,     0,    44,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,    53,
      54,    55,   145,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
       0,     0,     0,     0,    71,     0,     0,     5,     6,     7,
       8,     9,    10,    72,     0,    73,   141,    74,    75,    14,
       0,     0,     0,    77,     0,     0,     0,   146,     0,    79,
       0,   147,    23,     0,     0,     0,     0,     0,   142,   143,
       0,   144,     0,    31,     0,     0,     0,     0,    34,    35,
       0,    37,     0,    38,    39,     0,     0,     0,     0,     0,
      41,     0,    42,     0,    44,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,    53,    54,
      55,   145,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,     0,
       0,     0,     0,    71,     0,     0,     5,     6,     7,     8,
       9,    10,    72,     0,    73,   141,    74,    75,    14,     0,
       0,     0,    77,     0,     0,     0,    78,     0,    79,     0,
     147,    23,     0,     0,     0,     0,     0,   142,   143,     0,
     144,     0,    31,     0,     0,     0,     0,    34,    35,     0,
      37,     0,    38,    39,     0,     0,     0,     0,     0,    41,
       0,    42,     0,    44,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,    53,    54,    55,
     145,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,     0,     0,
       0,     0,    71,     0,     0,     5,     6,     7,     8,     9,
      10,    72,     0,    73,   141,    74,    75,   561,     0,     0,
       0,   535,     0,     0,     0,   146,     0,    79,     0,   147,
     562,     0,     0,     0,     0,     0,   142,   143,     0,   144,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,   563,     0,
      42,     0,    44,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   564,   565,    54,    55,   566,
       0,     0,     0,     0,     0,     0,     0,     0,   567,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,     0,     0,     0,
       0,    71,     0,     0,     5,     6,     7,     8,     9,    10,
      72,     0,    73,   141,    74,    75,    14,     0,     0,     0,
      77,     0,     0,     0,   146,     0,    79,     0,   568,    23,
       0,     0,     0,     0,     0,   142,   143,     0,   144,     0,
      31,     0,     0,     0,     0,    34,    35,     0,    37,     0,
      38,    39,     0,     0,     0,     0,     0,    41,     0,    42,
       0,    44,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,    53,    54,    55,   145,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,     0,     0,     0,     0,
      71,     0,     0,     5,     6,     7,     8,     9,    10,    72,
       0,    73,   141,    74,    75,    14,     0,     0,     0,    77,
       0,     0,     0,   146,     0,    79,     0,   568,    23,     0,
       0,     0,     0,     0,   142,   143,     0,   647,     0,    31,
       0,     0,     0,     0,    34,    35,     0,    37,     0,    38,
      39,     0,     0,     0,     0,     0,    41,     0,    42,     0,
      44,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,    53,    54,    55,   145,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,     0,     0,     0,     0,    71,
       0,     0,     5,     6,     7,     8,     9,    10,    72,     0,
      73,   141,    74,    75,    14,     0,     0,     0,    77,     0,
       0,     0,   146,     0,    79,     0,   147,    23,     0,     0,
       0,     0,     0,   142,   143,     0,   650,     0,    31,     0,
       0,     0,     0,    34,    35,     0,    37,     0,    38,    39,
       0,     0,     0,     0,     0,    41,     0,    42,     0,    44,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,    53,    54,    55,   145,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,     0,     0,     0,     0,    71,     0,
       0,     5,     6,     7,     8,     9,    10,    72,     0,    73,
     141,    74,    75,    14,     0,     0,     0,    77,     0,     0,
       0,   146,     0,    79,     0,   147,    23,     0,     0,     0,
       0,     0,   142,   143,     0,   846,     0,    31,     0,     0,
       0,     0,    34,    35,     0,    37,     0,    38,    39,     0,
       0,     0,     0,     0,    41,     0,    42,     0,    44,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,   145,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,     0,     0,     0,     0,    71,     0,     0,
       5,     6,     7,     8,     9,    10,    72,     0,    73,   141,
      74,    75,    14,     0,     0,     0,    77,     0,     0,     0,
     146,     0,    79,     0,   147,    23,     0,     0,     0,     0,
       0,   142,   143,     0,   847,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,    41,     0,    42,     0,    44,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,   145,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,     0,     0,     0,     0,    71,     0,     0,     5,
       6,     7,     8,     9,    10,    72,     0,    73,   141,    74,
      75,    14,     0,     0,     0,    77,     0,     0,     0,   146,
       0,    79,     0,   147,    23,     0,     0,     0,     0,     0,
     142,   143,     0,   849,     0,    31,     0,     0,     0,     0,
      34,    35,     0,    37,     0,    38,    39,     0,     0,     0,
       0,     0,    41,     0,    42,     0,    44,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
      53,    54,    55,   145,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,     0,     0,     0,     0,    71,     0,     0,     5,     6,
       7,     8,     9,    10,    72,     0,    73,   141,    74,    75,
      14,     0,     0,     0,    77,     0,     0,     0,   146,     0,
      79,     0,   147,    23,     0,     0,     0,     0,     0,   142,
     143,     0,   850,     0,    31,     0,     0,     0,     0,    34,
      35,     0,    37,     0,    38,    39,     0,     0,     0,     0,
       0,    41,     0,    42,     0,    44,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,    53,
      54,    55,   145,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
       0,     0,     0,     0,    71,     0,     0,     5,     6,     7,
       8,     9,    10,    72,     0,    73,   141,    74,    75,    14,
       0,     0,     0,    77,     0,     0,     0,   146,     0,    79,
       0,   147,    23,     0,     0,     0,     0,     0,   142,   143,
       0,   851,     0,    31,     0,     0,     0,     0,    34,    35,
       0,    37,     0,    38,    39,     0,     0,     0,     0,     0,
      41,     0,    42,     0,    44,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,    53,    54,
      55,   145,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,     0,
       0,     0,     0,    71,     0,     0,     5,     6,     7,     8,
       9,    10,    72,     0,    73,   141,    74,    75,    14,     0,
       0,     0,    77,     0,     0,     0,   146,     0,    79,     0,
     147,    23,     0,     0,     0,     0,     0,   142,   143,     0,
     852,     0,    31,     0,     0,     0,     0,    34,    35,     0,
      37,     0,    38,    39,     0,     0,     0,     0,     0,    41,
       0,    42,     0,    44,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,    53,    54,    55,
     145,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,     0,     0,
       0,     0,    71,     0,     0,     5,     6,     7,     8,     9,
      10,    72,     0,    73,   141,    74,    75,    14,     0,     0,
       0,    77,     0,     0,     0,   146,     0,    79,     0,   147,
      23,     0,     0,     0,     0,     0,   142,   143,     0,   144,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,    41,     0,
      42,     0,    44,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,   145,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   141,     0,     0,    14,     0,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    23,     0,     0,
       0,     0,     0,   142,   143,     0,   144,     0,    31,     0,
      72,     0,    73,    34,    35,    75,    37,     0,    38,    39,
      77,     0,     0,     0,   146,    41,    79,    42,   147,    44,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,    53,    54,    55,   145,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -324,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,     0,     0,  -324,     0,     0,     0,  -324,
       0,     0,     5,     6,     7,     8,     9,    10,     0,    73,
       0,   141,     0,   203,    14,     0,     0,     0,     0,     0,
       0,   146,     0,    79,  -324,   873,     0,    23,     0,     0,
       0,     0,     0,   142,   143,     0,   144,     0,    31,     0,
       0,     0,     0,    34,    35,     0,    37,     0,    38,    39,
       0,     0,     0,     0,     0,    41,     0,    42,     0,    44,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,    53,    54,    55,   145,     0,     5,     6,
       7,     8,     9,    10,     0,    62,     0,   141,     0,     0,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,     0,     0,     0,     0,     0,   142,
     143,    68,   144,     0,    31,     0,     0,     0,     0,    34,
      35,     0,    37,     0,    38,    39,     0,     0,     0,    73,
       0,    41,     0,    42,     0,    44,     0,     0,     0,     0,
       5,   146,     0,    79,     0,   568,     0,   733,    52,    53,
      54,    55,   145,   734,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     5,     0,
       0,     0,     0,     0,     0,   733,     0,     0,     0,     0,
       0,   734,     0,     0,     0,    73,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
       0,   147,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   357,   358,   359,   360,   361,   362,
       0,     0,   363,   364,   365,   366,   367,   368,   735,   736,
     737,   738,     0,     0,   739,     0,     0,     0,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,   750,     0,
     751,     0,     0,   752,   753,   754,   755,     0,   756,     0,
       0,   757,   357,   358,   359,   360,   361,   362,     0,     0,
     363,   364,   365,   366,   367,   368,   735,   736,   737,   738,
       0,     0,   739,     0,     0,     0,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,   750,     0,   751,   270,
       0,   752,   753,   754,   755,   271,   756,     0,     0,     0,
       0,     0,     0,     0,     0,   272,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
       0,     0,     0,   271,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   272,     0,   273,     0,     0,     0,   274,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   350,     0,   270,     0,   275,     0,     0,     0,   271,
       0,     0,     0,   273,     0,     0,     0,   274,     0,   272,
     276,     0,   277,   278,   279,     0,   280,   281,   282,     0,
     283,   284,   285,   286,   287,   288,   289,   290,   291,     0,
     292,   293,   294,   275,     0,   295,   296,   297,     0,     0,
       0,     0,     0,     0,     0,     0,   720,     0,   276,   273,
     277,   278,   279,   274,   280,   281,   282,     0,   283,   284,
     285,   286,   287,   288,   289,   290,   291,     0,   292,   293,
     294,   270,     0,   295,   296,   297,     0,   271,     0,   275,
       0,     0,     0,     0,   926,     0,     0,   272,     0,     0,
       0,     0,     0,     0,   276,     0,   277,   278,   279,     0,
     280,   281,   282,     0,   283,   284,   285,   286,   287,   288,
     289,   290,   291,     0,   292,   293,   294,   270,     0,   295,
     296,   297,     0,   271,     0,     0,     0,   273,     0,     0,
     963,   274,     0,   272,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   275,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   276,   273,   277,   278,   279,   274,   280,   281,
     282,     0,   283,   284,   285,   286,   287,   288,   289,   290,
     291,     0,   292,   293,   294,   270,     0,   295,   296,   297,
       0,   271,     0,   275,     0,     0,   590,     0,     0,     0,
       0,   272,     0,     0,     0,     0,     0,     0,   276,     0,
     277,   278,   279,     0,   280,   281,   282,     0,   283,   284,
     285,   286,   287,   288,   289,   290,   291,     0,   292,   293,
     294,     0,     0,   295,   296,   297,     0,     0,     0,     0,
       0,   273,   897,     0,     0,   274,     0,     0,     0,     0,
       0,     0,     0,   356,   270,     0,     0,     0,     0,     0,
     271,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     272,   275,   357,   358,   359,   360,   361,   362,     0,     0,
     363,   364,   365,   366,   367,   368,   276,     0,   277,   278,
     279,     0,   280,   281,   282,     0,   283,   284,   285,   286,
     287,   288,   289,   290,   291,     0,   292,   293,   294,     0,
     273,   295,   296,   297,   274,     0,     0,   325,     0,     0,
       0,     0,   780,   270,     0,     0,     0,     0,     0,   271,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   272,
     275,   357,   358,   359,   360,   361,   362,     0,     0,   363,
     364,   365,   366,   367,   368,   276,     0,   277,   278,   279,
       0,   280,   281,   282,     0,   283,   284,   285,   286,   287,
     288,   289,   290,   291,     0,   292,   293,   294,     0,   273,
     295,   296,   297,   274,     0,     0,   325,     0,     0,     0,
       0,   785,   270,     0,     0,     0,     0,     0,   271,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   272,   275,
     357,   358,   359,   360,   361,   362,     0,     0,   363,   364,
     365,   366,   367,   368,   276,     0,   277,   278,   279,     0,
     280,   281,   282,     0,   283,   284,   285,   286,   287,   288,
     289,   290,   291,     0,   292,   293,   294,     0,   273,   295,
     296,   297,   274,     0,     0,   325,     0,     0,     0,     0,
     910,   270,     0,     0,     0,     0,     0,   271,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   272,   275,   357,
     358,   359,   360,   361,   362,     0,     0,   363,   364,   365,
     366,   367,   368,   276,     0,   277,   278,   279,     0,   280,
     281,   282,     0,   283,   284,   285,   286,   287,   288,   289,
     290,   291,     0,   292,   293,   294,     0,   273,   295,   296,
     297,   274,     0,     0,   325,     0,     0,     0,     0,   911,
     270,     0,     0,     0,     0,     0,   271,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   272,   275,   357,   358,
     359,   360,   361,   362,     0,     0,   363,   364,   365,   366,
     367,   368,   276,     0,   277,   278,   279,     0,   280,   281,
     282,     0,   283,   284,   285,   286,   287,   288,   289,   290,
     291,     0,   292,   293,   294,     0,   273,   295,   296,   297,
     274,     0,     0,   325,     0,     0,     0,     0,   912,   270,
       0,     0,     0,     0,     0,   271,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   272,   275,   357,   358,   359,
     360,   361,   362,     0,     0,   363,   364,   365,   366,   367,
     368,   276,     0,   277,   278,   279,     0,   280,   281,   282,
       0,   283,   284,   285,   286,   287,   288,   289,   290,   291,
       0,   292,   293,   294,     0,   273,   295,   296,   297,   274,
       0,     0,   325,     0,     0,     0,     0,   913,   270,     0,
       0,     0,     0,     0,   271,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   272,   275,   357,   358,   359,   360,
     361,   362,     0,     0,   363,   364,   365,   366,   367,   368,
     276,     0,   277,   278,   279,     0,   280,   281,   282,     0,
     283,   284,   285,   286,   287,   288,   289,   290,   291,     0,
     292,   293,   294,     0,   273,   295,   296,   297,   274,     0,
       0,   325,     0,     0,     0,     0,   928,   270,     0,     0,
       0,     0,     0,   271,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   272,   275,   357,   358,   359,   360,   361,
     362,     0,     0,   363,   364,   365,   366,   367,   368,   276,
       0,   277,   278,   279,     0,   280,   281,   282,     0,   283,
     284,   285,   286,   287,   288,   289,   290,   291,     0,   292,
     293,   294,     0,   273,   295,   296,   297,   274,     0,     0,
     325,     0,     0,     0,     0,   929,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   275,   357,   358,   359,   360,   361,   362,
       0,     0,   363,   364,   365,   366,   367,   368,   276,     0,
     277,   278,   279,     0,   280,   281,   282,     0,   283,   284,
     285,   286,   287,   288,   289,   290,   291,     0,   292,   293,
     294,   270,     0,   295,   296,   297,     0,   271,     0,   325,
       0,     0,     0,     0,     0,     0,     0,   272,   348,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   349,
       0,     0,     0,     0,     0,   270,     0,     0,     0,     0,
       0,   271,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   272,   345,     0,     0,     0,     0,   273,     0,     0,
       0,   274,     0,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   350,     0,     0,     0,   275,     0,     0,
       0,   273,     0,     0,     0,   274,     0,     0,     0,     0,
       0,     0,   276,     0,   277,   278,   279,     0,   280,   281,
     282,     0,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   275,   292,   293,   294,     0,     0,   295,   296,   297,
       0,     0,     0,   325,     0,     0,   276,     0,   277,   278,
     279,     0,   280,   281,   282,     0,   283,   284,   285,   286,
     287,   288,   289,   290,   291,     0,   292,   293,   294,   270,
       0,   295,   296,   297,     0,   271,     0,   325,     0,     0,
       0,     0,     0,     0,     0,   272,   673,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   270,     0,     0,     0,     0,     0,   271,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   272,
     189,     0,     0,     0,     0,   273,     0,     0,     0,   274,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   350,     0,     0,     0,   275,     0,     0,     0,   273,
       0,     0,     0,   274,     0,     0,     0,     0,     0,     0,
     276,     0,   277,   278,   279,     0,   280,   281,   282,     0,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   275,
     292,   293,   294,   270,     0,   295,   296,   297,     0,   271,
       0,   325,     0,     0,   276,     0,   277,   278,   279,   272,
     280,   281,   282,     0,   283,   284,   285,   286,   287,   288,
     289,   290,   291,     0,   292,   293,   294,   270,     0,   295,
     296,   297,     0,   271,     0,   325,     0,     0,     0,     0,
       0,     0,     0,   272,   667,     0,     0,     0,     0,   273,
       0,     0,     0,   274,     0,     0,     0,     0,     0,     0,
       0,   685,     0,     0,     0,     0,   270,     0,     0,     0,
       0,     0,   271,     0,     0,     0,     0,     0,     0,   275,
       0,     0,   272,   273,     0,     0,     0,   274,     0,     0,
       0,     0,     0,     0,   276,     0,   277,   278,   279,     0,
     280,   281,   282,     0,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   275,   292,   293,   294,     0,     0,   295,
     296,   297,   273,     0,     0,   325,   274,     0,   276,     0,
     277,   278,   279,     0,   280,   281,   282,     0,   283,   284,
     285,   286,   287,   288,   289,   290,   291,     0,   292,   293,
     294,     0,   275,   295,   296,   297,   270,     0,     0,   325,
       0,     0,   271,     0,     0,     0,     0,   276,     0,   277,
     278,   279,   272,   280,   281,   282,     0,   283,   284,   285,
     286,   287,   288,   289,   290,   291,     0,   292,   293,   294,
     270,     0,   295,   296,   297,     0,   271,     0,   396,     0,
       0,     0,     0,     0,     0,     0,   272,     0,     0,     0,
       0,   353,   273,     0,     0,     0,   274,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   354,     0,     0,     0,     0,     0,     0,
       0,     0,   275,     0,     0,     0,   273,     0,     0,     0,
     274,     0,     0,     0,     0,     0,     0,   276,     0,   277,
     278,   279,     0,   280,   281,   282,     0,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   275,   292,   293,   294,
       0,     0,   295,   296,   297,     0,     0,  -424,     0,     0,
       0,   276,     0,   277,   278,   279,     0,   280,   281,   282,
       0,   283,   284,   285,   286,   287,   288,   289,   290,   291,
       0,   292,   293,   294,   270,   355,   295,   296,   297,     0,
     271,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     272,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   327,     0,     0,     0,     0,     0,   270,     0,
       0,     0,     0,     0,   271,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   272,     0,     0,     0,     0,   409,
     273,     0,     0,     0,   274,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   410,     0,     0,     0,     0,   136,     0,     0,     0,
     275,     0,     0,     0,   273,     0,     0,     0,   274,     0,
       0,     0,     0,     0,     0,   276,     0,   277,   278,   279,
       0,   280,   281,   282,     0,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   275,   292,   293,   294,     0,     0,
     295,   296,   297,     0,     0,     0,     0,     0,     0,   276,
       0,   277,   278,   279,     0,   280,   281,   282,     0,   283,
     284,   285,   286,   287,   288,   289,   290,   291,     0,   292,
     293,   294,   270,     0,   295,   296,   297,     0,   271,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   272,   505,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,     0,     0,     0,     0,     0,   270,     0,     0,     0,
       0,     0,   271,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   272,   507,     0,     0,     0,     0,   273,     0,
       0,     0,   274,     0,   508,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   270,     0,     0,     0,     0,
       0,   271,     0,     0,     0,     0,     0,     0,   275,     0,
       0,   272,   273,     0,     0,     0,   274,     0,     0,     0,
       0,     0,     0,   276,     0,   277,   278,   279,     0,   280,
     281,   282,     0,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   275,   292,   293,   294,     0,     0,   295,   296,
     297,   273,     0,     0,     0,   274,     0,   276,     0,   277,
     278,   279,     0,   280,   281,   282,     0,   283,   284,   285,
     286,   287,   288,   289,   290,   291,     0,   292,   293,   294,
       0,   275,   295,   296,   297,   270,     0,     0,     0,     0,
       0,   271,     0,     0,     0,     0,   276,     0,   277,   278,
     279,   272,   280,   281,   282,     0,   283,   284,   285,   286,
     287,   288,   289,   290,   291,     0,   292,   293,   294,   270,
     400,   295,   296,   297,     0,   271,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   272,     0,     0,     0,     0,
       0,   273,     0,     0,     0,   274,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   275,     0,     0,     0,   273,     0,     0,     0,   274,
       0,     0,     0,     0,     0,     0,   276,     0,   277,   278,
     279,     0,   280,   281,   282,     0,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   275,   292,   293,   294,   270,
     402,   295,   296,   297,     0,   271,     0,     0,     0,     0,
     276,     0,   277,   278,   279,   272,   280,   281,   282,     0,
     283,   284,   285,   286,   287,   288,   289,   290,   291,     0,
     292,   293,   294,     0,   404,   295,   296,   297,     0,     0,
       0,     0,     0,     0,     0,   270,     0,     0,     0,     0,
       0,   271,     0,     0,     0,   273,     0,     0,     0,   274,
       0,   272,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   270,     0,   275,     0,     0,     0,   271,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   272,
     276,   273,   277,   278,   279,   274,   280,   281,   282,     0,
     283,   284,   285,   286,   287,   288,   289,   290,   291,     0,
     292,   293,   294,     0,   412,   295,   296,   297,     0,     0,
     427,   275,     0,     0,     0,     0,     0,     0,     0,   273,
       0,     0,     0,   274,     0,     0,   276,     0,   277,   278,
     279,   509,   280,   281,   282,     0,   283,   284,   285,   286,
     287,   288,   289,   290,   291,     0,   292,   293,   294,   275,
       0,   295,   296,   297,     0,     0,     0,   270,     0,     0,
       0,     0,     0,   271,   276,     0,   277,   278,   279,     0,
     280,   281,   282,   272,   283,   284,   285,   286,   287,   288,
     289,   290,   291,     0,   292,   293,   294,     0,     0,   295,
     296,   297,     0,     0,     0,   270,     0,     0,     0,     0,
       0,   271,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   272,     0,   273,     0,     0,     0,   274,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   136,
       0,   270,   680,   275,     0,     0,     0,   271,     0,     0,
       0,   273,     0,     0,     0,   274,     0,   272,   276,     0,
     277,   278,   279,     0,   280,   281,   282,     0,   283,   284,
     285,   286,   287,   288,   289,   290,   291,     0,   292,   293,
     294,   275,     0,   295,   296,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   276,   273,   277,   278,
     279,   274,   280,   281,   282,     0,   283,   284,   285,   286,
     287,   288,   289,   290,   291,     0,   292,   293,   294,   270,
     657,   295,   296,   297,     0,   271,     0,   275,     0,     0,
       0,     0,     0,     0,     0,   272,     0,     0,   683,     0,
       0,     0,   276,     0,   277,   278,   279,     0,   280,   281,
     282,     0,   283,   284,   285,   286,   287,   288,   289,   290,
     291,     0,   292,   293,   294,   270,     0,   295,   296,   297,
       0,   271,     0,     0,     0,   273,     0,     0,     0,   274,
       0,   272,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   275,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     276,   273,   277,   278,   279,   274,   280,   281,   282,     0,
     283,   284,   285,   286,   287,   288,   289,   290,   291,     0,
     292,   293,   294,   270,     0,   295,   296,   297,     0,   271,
     723,   275,     0,     0,     0,     0,     0,     0,     0,   272,
     770,     0,     0,     0,     0,     0,   276,     0,   277,   278,
     279,     0,   280,   281,   282,     0,   283,   284,   285,   286,
     287,   288,   289,   290,   291,     0,   292,   293,   294,   270,
       0,   295,   296,   297,     0,   271,     0,     0,     0,   273,
       0,     0,     0,   274,     0,   272,   773,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,   275,
       0,     0,     0,   271,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   272,   276,   273,   277,   278,   279,   274,
     280,   281,   282,     0,   283,   284,   285,   286,   287,   288,
     289,   290,   291,     0,   292,   293,   294,     0,     0,   295,
     296,   297,     0,     0,     0,   275,     0,     0,     0,     0,
       0,     0,     0,   273,     0,     0,     0,   274,     0,     0,
     276,     0,   277,   278,   279,   845,   280,   281,   282,     0,
     283,   284,   285,   286,   287,   288,   289,   290,   291,     0,
     292,   293,   294,   275,     0,   295,   296,   297,     0,     0,
       0,   270,     0,     0,     0,     0,     0,   271,   276,     0,
     277,   278,   279,     0,   280,   281,   282,   272,   283,   284,
     285,   286,   287,   288,   289,   290,   291,     0,   292,   293,
     294,     0,     0,   295,   296,   297,     0,     0,     0,   270,
     862,     0,     0,     0,     0,   271,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   272,     0,   273,     0,     0,
       0,   274,     0,     0,     0,     0,     0,     0,     0,   848,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
       0,     0,     0,   271,     0,     0,     0,   275,     0,     0,
       0,     0,     0,   272,     0,   273,     0,     0,     0,   274,
       0,     0,   276,     0,   277,   278,   279,     0,   280,   281,
     282,     0,   283,   284,   285,   286,   287,   288,   289,   290,
     291,     0,   292,   293,   294,   275,     0,   295,   296,   297,
       0,     0,     0,   273,     0,     0,     0,   274,     0,     0,
     276,     0,   277,   278,   279,   935,   280,   281,   282,     0,
     283,   284,   285,   286,   287,   288,   289,   290,   291,     0,
     292,   293,   294,   275,     0,   295,   296,   297,     0,     0,
       0,   270,     0,     0,     0,     0,     0,   271,   276,     0,
     277,   278,   279,     0,   280,   281,   282,   272,   283,   284,
     285,   286,   287,   288,   289,   290,   291,     0,   292,   293,
     294,     0,     0,   295,   296,   297,     0,     0,     0,   270,
       0,     0,     0,     0,     0,   271,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   272,     0,   273,     0,     0,
       0,   274,     0,     0,     0,     0,     0,     0,     0,   936,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
       0,     0,     0,   271,     0,     0,     0,   275,     0,     0,
       0,     0,     0,   272,     0,   273,     0,     0,     0,   274,
       0,     0,   276,     0,   277,   278,   279,   937,   280,   281,
     282,     0,   283,   284,   285,   286,   287,   288,   289,   290,
     291,     0,   292,   293,   294,   275,     0,   295,   296,   297,
       0,     0,     0,   273,     0,     0,     0,   274,     0,     0,
     276,     0,   277,   278,   279,   938,   280,   281,   282,     0,
     283,   284,   285,   286,   287,   288,   289,   290,   291,     0,
     292,   293,   294,   275,     0,   295,   296,   297,     0,     0,
       0,   270,     0,     0,     0,     0,     0,   271,   276,     0,
     277,   278,   279,     0,   280,   281,   282,   272,   283,   284,
     285,   286,   287,   288,   289,   290,   291,     0,   292,   293,
     294,     0,     0,   295,   296,   297,     0,     0,     0,   270,
       0,     0,     0,     0,     0,   271,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   272,     0,   273,     0,     0,
       0,   274,     0,     0,     0,     0,     0,     0,     0,   939,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
       0,     0,     0,   271,     0,     0,     0,   275,     0,     0,
       0,     0,     0,   272,     0,   273,     0,     0,     0,   274,
       0,     0,   276,     0,   277,   278,   279,   940,   280,   281,
     282,     0,   283,   284,   285,   286,   287,   288,   289,   290,
     291,     0,   292,   293,   294,   275,     0,   295,   296,   297,
       0,     0,     0,   273,     0,     0,     0,   274,     0,     0,
     276,     0,   277,   278,   279,     0,   280,   281,   282,     0,
     283,   284,   285,   286,   287,   288,   289,   290,   291,     0,
     292,   293,   294,   275,     0,   295,   296,   297,     0,     0,
       0,   270,     0,     0,     0,     0,     0,   271,   276,     0,
     277,   278,   279,     0,   280,   281,   282,   272,   283,   284,
     285,   286,   287,   288,   289,   290,   291,     0,   292,   293,
     294,     0,     0,   295,   296,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   645,     0,     0,
       0,   274,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   275,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   276,     0,   277,   278,   279,     0,   280,   281,
     282,     0,   283,   284,   285,   286,   287,   288,   289,   290,
     291,     0,   292,   293,   294,     0,     0,   295,   296,   297
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-834)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       2,     2,   318,   376,   616,    16,   381,    45,    46,    11,
     159,     1,    14,   405,     1,    17,   182,    27,    20,    21,
      22,   170,   560,   252,   700,    27,    28,    29,    30,    25,
     405,   761,     3,   199,    36,    37,    38,   533,    40,    41,
      42,   264,   265,   266,   267,    27,    48,     3,    50,    51,
      52,    53,   218,     3,    56,    57,    58,    59,   354,     3,
      62,    63,    64,    65,    48,   726,    68,    54,     3,    71,
      72,    73,    97,    75,     3,    27,    78,    54,    80,    63,
      82,     1,   726,     1,   917,     1,    27,    27,   116,   765,
      38,   116,   881,   116,   116,   120,    80,   116,     3,    28,
     135,    38,    89,    69,   116,    38,   644,    97,   116,    97,
      92,    67,    89,    84,   410,   143,     0,    22,    38,   952,
     145,    83,   145,   145,    18,   115,   116,   803,   147,    38,
      67,   143,   120,    38,    67,   137,    41,   145,    38,   141,
     142,   143,   144,   145,   146,   147,   642,   158,   314,   315,
      55,    69,    57,    69,    83,   145,   143,    38,    92,   697,
     142,    69,    67,   147,    80,   116,   143,   956,   116,   899,
     900,    65,    90,   144,    90,   116,   142,    97,    83,    58,
      59,   183,    92,    92,   135,    22,   559,   189,   144,   833,
     142,    85,   142,   854,   144,   115,   116,   115,    42,   115,
     144,   142,   142,    49,   200,   207,   208,   116,    26,   144,
     854,   952,   116,   436,     3,   135,   116,   533,    62,   135,
      57,   144,   963,    38,   142,   116,   142,   135,   840,   116,
      67,    97,   398,    79,   142,   116,   116,   147,   147,   144,
     916,   145,   244,   244,   135,   247,   135,   147,    66,   144,
     116,    88,    70,   135,   120,   144,    13,   146,   145,     7,
      76,   143,    19,   273,   274,   145,   147,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   145,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   792,   115,    63,   837,
     118,   116,   531,   532,   122,   116,   316,   317,   116,    21,
     144,   116,    79,   144,   316,   317,   318,   327,   691,   116,
      32,    33,   144,   325,   146,   327,   642,    22,    40,   331,
      42,   333,   147,     1,   145,     3,   346,   145,   135,   349,
     145,   343,    38,   345,   346,    22,   348,   349,   135,     3,
      62,   353,   354,     7,   356,   135,   116,    22,   116,   343,
     141,    38,    57,   116,    41,     2,   595,   596,   370,   144,
     144,   146,    67,    69,    11,   135,   116,   135,    55,   381,
      57,   383,   135,    20,    80,    22,   116,   118,    83,   391,
      67,   116,    57,    66,    90,   135,   135,    70,   165,   135,
     135,    97,    67,   405,   143,   135,    83,   409,   410,   115,
     135,   135,   118,   423,   416,   182,   122,   809,    83,    56,
     116,   423,   424,   425,    66,   427,   428,   802,    70,     3,
     432,   135,   199,     7,   809,   135,   144,   586,   146,   135,
     419,    78,   421,   135,   142,    82,   142,   135,   432,   145,
      91,   218,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,    42,    18,
     144,   135,    21,   144,   144,    97,   792,   135,     7,    48,
     115,    30,   142,    32,    33,   135,    97,    36,    62,    26,
     145,    40,   116,    42,    38,   135,   506,   144,   508,    29,
     137,   512,   513,   505,   506,   507,   508,   509,   510,   511,
      59,   145,    61,    62,   116,    64,    65,   145,    97,   145,
     145,   523,    17,   525,   116,   142,   147,   529,   135,    66,
     135,   533,   135,    70,    83,   135,    85,   135,   135,   135,
     135,   543,   917,   147,   135,   135,   183,   314,   315,     2,
     135,   926,   189,   135,   135,   135,   145,   135,    11,   561,
     135,   563,   564,   565,   566,   567,   568,    20,   135,    22,
      26,   116,   135,   419,   111,   421,   135,   952,   115,    29,
     117,   118,   143,   145,   568,   122,   135,   145,   963,   135,
     142,   116,   129,   135,   143,   143,    97,    67,   600,   136,
     137,   138,    27,    56,    27,    58,    59,   244,   147,   142,
      66,    29,   115,     3,    70,    67,   147,     7,   115,     9,
     147,   147,   143,    29,   391,    78,   118,   118,   144,    82,
     145,   398,    22,    90,   145,   645,   646,   143,    80,   724,
     642,   410,   226,   645,   646,   647,    26,   409,   650,   416,
     711,   171,   419,   522,   421,   778,   729,   424,   840,   115,
       3,   117,   118,   665,     7,   667,   122,    57,   670,   841,
     732,   673,   518,   129,   689,   956,   678,    67,   680,    22,
     682,   683,   138,   685,   137,   916,    66,   931,   325,    29,
      70,   384,   865,    83,   520,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,   706,   707,    -1,    -1,   345,    -1,
      -1,   348,    -1,    -1,    57,    -1,   718,    -1,   720,   356,
      -1,   723,   724,   707,    67,   571,   728,    -1,    -1,    -1,
     183,   111,    -1,   113,   114,   115,   189,   117,   118,    -1,
      83,    -1,   122,    -1,    -1,    88,    -1,    -1,    -1,   129,
      -1,   518,    -1,   520,    -1,   757,   136,   137,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   769,   770,    -1,
     772,   773,   774,   775,    -1,    -1,    -1,    -1,   780,   781,
      38,   783,    -1,   785,   786,    -1,    -1,   789,   425,    -1,
     792,   244,   830,   831,    -1,    -1,    -1,   799,    -1,    -1,
     802,    -1,    -1,    -1,    -1,    -1,   808,   809,   810,    -1,
      -1,    69,   814,    -1,   816,    -1,    -1,   819,   820,    -1,
      -1,   823,    80,    -1,    -1,   827,   827,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,   845,   846,   847,   848,   849,   850,   851,
     852,    -1,    -1,    -1,    -1,    -1,    -1,    51,   116,    -1,
     862,   863,    -1,    -1,   866,   502,   868,   869,   870,   871,
     872,   873,   325,    26,    -1,   877,     2,   135,   880,    -1,
      -1,    -1,    -1,    -1,   142,    11,    -1,   145,    -1,   873,
      -1,    -1,   345,    -1,    20,   348,    22,    -1,    -1,    -1,
      -1,    -1,    -1,   356,    -1,    -1,   543,    -1,   910,   911,
     912,   913,    -1,    66,    26,   917,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,   926,    -1,   928,   929,    -1,    -1,
      56,    -1,    -1,   935,   936,   937,   938,   939,   940,     2,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,   951,
     952,    -1,    78,    -1,    66,    -1,    82,    20,    70,    22,
      -1,   963,   115,   600,   117,   118,    -1,    -1,    -1,   122,
      -1,    -1,   425,    -1,    -1,    -1,   129,    -1,    -1,   616,
     174,   175,   176,   136,   137,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,   192,   111,
      38,   195,   114,   115,    -1,   117,   118,    -1,   645,   646,
     122,   137,    -1,    -1,    -1,    78,    -1,   129,    -1,    82,
     214,    -1,    -1,    -1,   136,   137,   138,    -1,   665,    18,
     667,    69,    21,   670,    -1,   802,   673,    -1,    -1,    -1,
     234,    30,    80,    32,    33,   682,    -1,    36,   685,    -1,
      -1,    40,    90,    42,    -1,    -1,    -1,   183,    -1,    97,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    26,   706,
      59,    -1,    61,    62,   137,    64,    65,    -1,   116,    -1,
      -1,   718,    -1,   720,    -1,    -1,    -1,   724,    -1,    -1,
     543,    -1,    -1,    -1,    83,    -1,    85,   135,    -1,    -1,
      -1,    -1,    -1,    21,   142,    -1,    -1,   145,    66,    -1,
     877,    -1,    70,    -1,    32,    33,    -1,    -1,   244,    -1,
     183,    -1,    40,    -1,    42,    -1,   189,    -1,    -1,    -1,
      48,    -1,    -1,    -1,   328,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   780,    62,    63,   135,   600,   785,    -1,
     917,    -1,    -1,   111,   143,   113,   114,   115,   352,   117,
     118,   119,   799,    -1,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,   810,     2,   133,    -1,    -1,   136,   137,
     138,   244,   819,    11,    -1,   952,   823,    -1,    -1,    -1,
     827,    -1,    20,    -1,    22,    -1,   963,    21,    -1,   325,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,   665,    -1,   667,    -1,    40,   670,    42,   345,
     673,    -1,   348,    -1,    48,    -1,   863,    -1,    56,   682,
     356,    -1,   685,    -1,    -1,    -1,    -1,    18,    62,    63,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      78,    32,    33,   706,    82,    36,    -1,    -1,    -1,    40,
      -1,    42,   325,    -1,    -1,   718,    -1,   720,    -1,    -1,
      -1,   724,    -1,   910,   911,   912,   913,    -1,    59,    -1,
      61,    62,   345,    64,    65,   348,    -1,    -1,    -1,    -1,
      -1,   928,   929,   356,    -1,    -1,    -1,    -1,    -1,   425,
      -1,    -1,    83,    -1,    85,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     2,    -1,    -1,    -1,    -1,    -1,   780,    -1,    -1,
      11,    -1,   785,   527,   528,    -1,    -1,   531,   532,    20,
      -1,    22,    -1,    -1,    -1,    -1,   799,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   135,   183,    -1,   810,    -1,    -1,
      -1,   189,   425,    -1,     1,    -1,   819,    -1,    -1,    -1,
     823,    -1,    -1,    -1,   827,    56,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    21,    22,    -1,    -1,    -1,     2,
      -1,    -1,    -1,    30,    -1,    32,    33,    78,    11,    36,
      -1,    82,    -1,    40,    -1,    42,    -1,    20,    -1,    22,
     863,    -1,    -1,    -1,    -1,    -1,   244,   543,    -1,    -1,
      57,    -1,    59,    -1,    61,    62,    -1,    64,    65,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    83,    -1,    85,    -1,
      -1,    88,    -1,    -1,    -1,    -1,   137,   910,   911,   912,
     913,   655,   656,    -1,    -1,    78,    -1,    -1,   662,    82,
      -1,    -1,    -1,    -1,   600,   928,   929,    -1,   672,    -1,
     543,   675,    -1,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   325,   135,    -1,
      -1,    -1,   183,    -1,    -1,    -1,   143,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   345,    -1,    -1,
     348,    -1,    -1,    -1,   137,   139,   140,   141,   356,    -1,
     144,    -1,   146,    -1,    -1,    -1,    -1,   600,    26,   665,
      -1,   667,    -1,    -1,   670,    -1,    -1,   673,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   682,    -1,    -1,   685,
      -1,    -1,    -1,   244,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,   189,    -1,    66,    -1,
     706,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   718,    -1,   720,    -1,    -1,   425,   724,    -1,
      -1,    -1,   665,    -1,   667,    -1,    -1,   670,    -1,    -1,
     673,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   812,   682,
      -1,    -1,   685,   111,    -1,   113,   114,   115,    -1,   117,
     118,   244,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
     128,   129,    -1,   706,   325,   133,    -1,    -1,   136,   137,
     138,    -1,    -1,    -1,   780,   718,    -1,   720,    -1,   785,
      -1,   724,    -1,    -1,   345,    -1,    -1,   348,    -1,    -1,
      -1,    -1,    -1,   799,     2,   356,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    11,   810,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,   819,    22,    -1,    -1,   823,    -1,    -1,
      -1,   827,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   325,    -1,    -1,   543,    -1,   780,    -1,    -1,
      -1,    -1,   785,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    -1,   345,    -1,    -1,   348,   799,   863,    -1,    -1,
      -1,    -1,    -1,   356,   425,    -1,    -1,   810,    -1,    -1,
      78,    -1,    -1,    -1,    82,    -1,   819,    -1,    -1,    -1,
     823,    -1,    -1,    -1,   827,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   600,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    -1,   910,   911,   912,   913,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
     863,    -1,   928,   929,    58,    59,    -1,    -1,    -1,   137,
      -1,    -1,   425,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,    -1,    -1,   665,    -1,   667,
      -1,    -1,   670,    -1,    -1,   673,    -1,   910,   911,   912,
     913,    -1,    -1,    -1,   682,   183,    -1,   685,    -1,    -1,
      -1,   189,   543,   139,   140,   928,   929,    -1,   144,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   706,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     718,    -1,   720,    -1,    -1,    -1,   724,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   244,    -1,    -1,   600,
     174,   175,   176,    -1,    -1,    -1,    -1,   181,    -1,    -1,
     543,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,    -1,
      -1,   195,    -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,
      -1,    -1,   780,    -1,    -1,    11,    -1,   785,    -1,    -1,
     214,    -1,    -1,    -1,    20,    -1,    22,    -1,    -1,    -1,
      -1,   799,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     234,    -1,   810,    -1,   665,    -1,   667,   600,    -1,   670,
      -1,   819,   673,    -1,    -1,   823,    -1,   325,    -1,   827,
      56,   682,    -1,    -1,   685,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   345,    -1,    -1,
     348,    -1,    78,    -1,    -1,   706,    82,    -1,   356,    -1,
      -1,    -1,    -1,    -1,    -1,   863,    -1,   718,    -1,   720,
      -1,    -1,    -1,   724,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   665,    -1,   667,    -1,    -1,   670,    -1,    -1,
     673,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   682,
      -1,    -1,   685,    26,   328,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   910,   911,   912,   913,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   706,    -1,    -1,    -1,   425,   352,   780,
     928,   929,    -1,    -1,   785,   718,    -1,   720,    -1,    -1,
      -1,   724,    -1,    66,    -1,    -1,    -1,    70,   799,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,   810,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,   819,    -1,
      -1,    -1,   823,    96,    -1,    -1,   827,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   780,   121,   122,
     123,   124,   785,   126,   127,   128,   129,    -1,   131,   132,
     133,    -1,   863,   136,   137,   138,   799,    -1,   244,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   810,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,   819,    -1,    -1,    -1,
     823,    -1,    -1,    -1,   827,   543,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    21,    22,    -1,    -1,    -1,   910,
     911,   912,   913,    30,    -1,    32,    33,    -1,    -1,    36,
      -1,    -1,    -1,    40,    -1,    42,    -1,   928,   929,    -1,
     863,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    -1,    59,    -1,    61,    62,    -1,    64,    65,   325,
      67,    -1,   600,   527,   528,    -1,    -1,   531,   532,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,   345,
     544,    88,   348,    -1,    -1,    -1,    -1,   910,   911,   912,
     913,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   928,   929,    -1,    -1,    10,
      -1,    13,    -1,    -1,    -1,    16,    -1,    19,   582,    -1,
      -1,    -1,    -1,    25,    26,    26,    -1,   665,   135,   667,
      -1,    33,   670,    35,    -1,   673,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   682,    -1,    -1,   685,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,   425,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,   706,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     718,    -1,   720,    -1,    -1,    -1,   724,    -1,    -1,    -1,
      -1,   655,   656,    -1,    -1,    96,    -1,    -1,   662,    -1,
     664,   103,    -1,   105,    -1,   669,    -1,    -1,   672,    -1,
     111,   675,   113,   114,   115,    -1,   117,   118,   119,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
     131,   132,   133,    -1,    -1,   136,   137,   138,    -1,    -1,
      -1,    -1,   780,    -1,    26,    -1,   147,   785,    -1,    -1,
     714,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
      -1,   799,    -1,   165,    -1,    -1,    -1,    -1,   170,    -1,
     172,    -1,   810,    -1,    -1,    -1,    -1,   543,    -1,    -1,
     182,   819,    -1,   185,    66,   823,    -1,    -1,    70,   827,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   199,   200,    -1,
      -1,   203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   863,    -1,    -1,    -1,   111,
      -1,   113,   114,   115,   600,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,   812,   131,
      -1,   133,    -1,    -1,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,   264,   265,   266,   267,    -1,    -1,    -1,    -1,
      -1,    -1,   910,   911,   912,   913,    -1,    -1,    -1,   281,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     928,   929,   856,   857,    -1,   859,   860,    -1,    -1,   665,
      -1,   667,    -1,    -1,   670,    -1,    -1,   673,    -1,    -1,
      -1,    -1,   314,   315,    -1,    -1,   682,   881,    -1,   685,
      -1,    -1,    -1,   887,    -1,   889,    -1,   891,    66,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,   341,
     706,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   718,    -1,   720,    -1,    -1,    -1,   724,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   376,   113,   114,   115,    -1,   117,
     118,   119,   384,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,   956,   131,    -1,   133,   398,    -1,   136,   137,
     138,    -1,    -1,     1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   431,
      28,    -1,   434,   799,   436,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,   810,    -1,    44,    45,    -1,    47,
      -1,    49,    50,   819,    -1,    -1,    -1,   823,    56,    -1,
      58,   827,    60,    -1,    -1,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   863,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      -1,    70,    -1,   111,   112,   113,   114,    -1,   520,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,    -1,   130,    -1,   132,   133,   134,    -1,    -1,    -1,
     138,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,    -1,
      -1,    -1,   111,   555,   113,   114,   115,   559,   117,   118,
     119,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,   128,
     129,    -1,    -1,    -1,   133,    -1,    -1,   136,   137,   138,
     582,    -1,    -1,    -1,   586,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     0,     1,    -1,     3,     4,
       5,     6,     7,     8,   616,    -1,    -1,    12,    13,    14,
      15,    -1,    -1,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    27,    28,    -1,    30,    -1,    32,    33,    34,
      35,    36,    37,    -1,    39,    40,    -1,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    -1,    52,    -1,    -1,
      -1,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,    -1,    81,    82,    83,   691,
      85,    86,    87,    88,    -1,    -1,    91,    -1,    93,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   711,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,
      -1,    -1,    -1,    -1,   119,    -1,    -1,   729,    -1,    -1,
      -1,    -1,    -1,   128,   736,   130,    -1,   132,   133,    -1,
     135,    -1,    -1,   138,    -1,    -1,    -1,   142,    -1,   144,
       1,   146,     3,     4,     5,     6,     7,     8,    -1,    10,
      -1,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,   829,    79,    -1,
      81,    82,    83,    -1,    85,    86,    87,    88,   840,    -1,
      91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,    -1,   135,   136,   137,   138,    -1,    -1,
      -1,   142,   143,   144,     1,   146,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    -1,    30,    -1,    32,    33,    34,    35,    36,
      37,    -1,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,    -1,    81,    82,    83,    -1,    85,    86,
      87,    88,    -1,    -1,    91,    -1,    93,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,   113,   114,    -1,    -1,
      -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,    -1,   130,    -1,   132,   133,   134,   135,    -1,
      -1,   138,    -1,    -1,    -1,   142,   143,   144,     1,   146,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    -1,    -1,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    27,    28,    -1,    30,    -1,    32,
      33,    34,    35,    36,    37,    -1,    39,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    -1,    52,
      -1,    -1,    -1,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    -1,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    77,    -1,    79,    -1,    81,    82,
      83,    -1,    85,    86,    87,    88,    -1,    -1,    91,    -1,
      93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
     113,   114,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,    -1,   130,    -1,   132,
     133,    -1,   135,    -1,    -1,   138,    -1,    -1,    -1,   142,
     143,   144,     1,   146,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    -1,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    27,    28,
      -1,    30,    -1,    32,    33,    34,    35,    36,    37,    -1,
      39,    40,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    -1,    52,    -1,    -1,    -1,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    77,    -1,
      79,    -1,    81,    82,    83,    -1,    85,    86,    87,    88,
      -1,    -1,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,   130,    -1,   132,   133,    -1,   135,    -1,    -1,   138,
      -1,    -1,    -1,   142,   143,   144,     1,   146,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    -1,    -1,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    27,    28,    -1,    30,    -1,    32,    33,    34,
      35,    36,    37,    -1,    39,    40,    -1,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    -1,    52,    -1,    -1,
      -1,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,    -1,    81,    82,    83,    -1,
      85,    86,    87,    88,    -1,    -1,    91,    -1,    93,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,    -1,   130,    -1,   132,   133,    -1,
     135,    -1,    -1,   138,    -1,    -1,    -1,   142,   143,   144,
       1,   146,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    -1,    67,    68,    69,    -1,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    -1,
      81,    82,    83,    -1,    85,    86,    87,    88,    -1,    -1,
      91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,   113,   114,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,   130,
      -1,   132,   133,    -1,   135,    -1,    -1,   138,    -1,    -1,
      -1,   142,   143,   144,     1,   146,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    -1,    30,    -1,    32,    33,    34,    35,    36,
      37,    -1,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,    -1,    81,    82,    83,    -1,    85,    86,
      87,    88,    -1,    -1,    91,    -1,    93,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,   113,   114,    -1,    -1,
      -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,    -1,   130,    -1,   132,   133,    -1,   135,    -1,
      -1,   138,    -1,    -1,    -1,   142,    -1,   144,     1,   146,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    -1,    -1,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    27,    28,    -1,    30,    -1,    32,
      33,    34,    35,    36,    37,    -1,    39,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    -1,    52,
      -1,    -1,    -1,    56,    57,    58,    -1,    60,    61,    62,
      63,    64,    65,    -1,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    77,    -1,    79,    -1,    81,    82,
      83,    -1,    85,    86,    87,    88,    -1,    -1,    91,    -1,
      93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
     113,   114,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,    -1,   130,    -1,   132,
     133,    -1,   135,    -1,    -1,   138,    -1,    -1,    -1,   142,
      -1,   144,     1,   146,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    -1,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    27,    28,
      -1,    30,    -1,    32,    33,    34,    35,    36,    37,    -1,
      39,    40,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    -1,    52,    -1,    -1,    -1,    56,    57,    58,
      -1,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    77,    -1,
      79,    -1,    81,    82,    83,    -1,    85,    86,    87,    88,
      -1,    -1,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,   130,    -1,   132,   133,    -1,   135,    -1,    -1,   138,
      -1,    -1,    -1,   142,    -1,   144,     1,   146,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    -1,    -1,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    27,    28,    -1,    30,    -1,    32,    33,    34,
      35,    36,    37,    -1,    39,    40,    -1,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    -1,    52,    -1,    -1,
      -1,    56,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,    -1,    81,    82,    83,    -1,
      85,    86,    87,    88,    -1,    -1,    91,    -1,    93,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,    -1,   130,    -1,   132,   133,    -1,
     135,    -1,    -1,   138,    -1,    -1,    -1,   142,    -1,   144,
       1,   146,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    -1,    67,    68,    69,    -1,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    -1,
      81,    82,    83,    -1,    85,    86,    87,    88,    -1,    -1,
      91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,   113,   114,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,   130,
      -1,   132,   133,    -1,   135,    -1,    -1,   138,    -1,    -1,
      -1,   142,    -1,   144,     1,   146,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    -1,    30,    -1,    32,    33,    34,    35,    36,
      37,    -1,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,    56,
      57,    58,    -1,    60,    61,    62,    63,    64,    65,    -1,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,    -1,    81,    82,    83,    -1,    85,    86,
      87,    88,    -1,    -1,    91,    -1,    93,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,   113,   114,    -1,    -1,
      -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,    -1,   130,    -1,   132,   133,    -1,   135,    -1,
      -1,   138,    -1,    -1,    -1,   142,    -1,   144,     1,   146,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    -1,    -1,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    27,    28,    -1,    30,    -1,    32,
      33,    34,    35,    36,    37,    -1,    39,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    -1,    52,
      -1,    -1,    -1,    56,    57,    58,    -1,    60,    61,    62,
      63,    64,    65,    -1,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    77,    -1,    79,    -1,    81,    82,
      83,    -1,    85,    86,    87,    88,    -1,    -1,    91,    -1,
      93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
     113,   114,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,    -1,   130,    -1,   132,
     133,    -1,   135,    -1,    -1,   138,    -1,    -1,    -1,   142,
      -1,   144,     1,   146,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    -1,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    27,    28,
      -1,    30,    -1,    32,    33,    34,    35,    36,    37,    -1,
      39,    40,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    -1,    52,    -1,    -1,    -1,    56,    57,    58,
      -1,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    77,    -1,
      79,    -1,    81,    82,    83,    -1,    85,    86,    87,    88,
      -1,    -1,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,   130,    -1,   132,   133,    -1,   135,    -1,    -1,   138,
      -1,    -1,    -1,   142,    -1,   144,     1,   146,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    -1,    -1,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    27,    28,    -1,    30,    -1,    32,    33,    34,
      35,    36,    37,    -1,    39,    40,    -1,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    -1,    52,    -1,    -1,
      -1,    56,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,    -1,    81,    82,    83,    -1,
      85,    86,    87,    88,    -1,    -1,    91,    -1,    93,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,    -1,   130,    -1,   132,   133,    -1,
     135,    -1,    -1,   138,    -1,    -1,    -1,   142,    -1,   144,
       1,   146,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    -1,    67,    68,    69,    -1,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    -1,
      81,    82,    83,    -1,    85,    86,    87,    88,    -1,    -1,
      91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,   113,   114,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,   130,
      -1,   132,   133,    -1,   135,    -1,    -1,   138,    -1,    -1,
      -1,   142,    -1,   144,     1,   146,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    -1,    30,    -1,    32,    33,    34,    35,    36,
      37,    -1,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,    56,
      57,    58,    -1,    60,    61,    62,    63,    64,    65,    -1,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,    -1,    81,    82,    83,    -1,    85,    86,
      87,    88,    -1,    -1,    91,    -1,    93,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,   113,   114,    -1,    -1,
      -1,    -1,   119,    -1,    -1,     3,     4,     5,     6,     7,
       8,   128,    -1,   130,    12,   132,   133,    15,   135,    -1,
      -1,   138,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      58,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,   113,   114,    -1,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,    -1,   130,    -1,   132,   133,   134,    -1,    -1,    -1,
     138,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    58,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
     113,   114,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,    -1,   130,    -1,   132,
     133,   134,    -1,    -1,    -1,   138,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,   147,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      58,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,   113,   114,    -1,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    58,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    -1,    -1,    -1,    81,    82,
      -1,    84,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
     113,   114,    -1,    -1,    -1,    -1,   119,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,    -1,   130,    -1,   132,
     133,   134,    -1,    -1,    -1,   138,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,    -1,
      -1,    -1,    81,    82,    -1,    84,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,   130,    -1,   132,   133,   134,    -1,    -1,    -1,   138,
      -1,    -1,    -1,   142,    -1,   144,   145,   146,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    34,
      35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    58,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,
      75,    76,    77,    -1,    -1,    -1,    81,    82,    -1,    84,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,    -1,   130,    -1,   132,   133,   134,
      -1,    -1,    -1,   138,    -1,    -1,    -1,   142,    -1,   144,
     145,   146,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    34,    35,    -1,    37,    38,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    47,    -1,    49,    50,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    73,    74,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,   113,   114,    -1,    -1,    -1,    -1,   119,    -1,
      -1,     3,     4,     5,     6,     7,     8,   128,    -1,   130,
      12,   132,   133,    15,    -1,    -1,    -1,   138,    -1,    -1,
      -1,   142,    -1,   144,    -1,   146,    28,    -1,    -1,    -1,
      -1,    -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    74,    75,    76,    77,    -1,    -1,    -1,    81,
      82,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,   113,   114,    -1,    -1,    -1,    -1,   119,    -1,    -1,
       3,     4,     5,     6,     7,     8,   128,    -1,   130,    12,
     132,   133,    15,   135,    -1,    -1,   138,    -1,    -1,    -1,
     142,    -1,   144,    -1,   146,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    58,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    -1,    -1,    -1,    81,    82,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
     113,   114,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,    -1,   130,    -1,   132,
     133,   134,    -1,    -1,    -1,   138,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    51,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,    -1,
      -1,    -1,    81,    82,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
     119,    -1,    -1,     3,     4,     5,     6,     7,     8,   128,
      -1,   130,    12,   132,   133,    15,    -1,    -1,    -1,   138,
      -1,    -1,    -1,   142,    -1,   144,    -1,   146,    28,    -1,
      -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,    49,
      50,    -1,    -1,    -1,    -1,    -1,    56,    57,    58,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    74,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,   113,   114,    -1,    -1,    -1,    -1,   119,
      -1,    -1,     3,     4,     5,     6,     7,     8,   128,    -1,
     130,    12,   132,   133,    15,    -1,    -1,    -1,   138,    -1,
      21,    -1,   142,    -1,   144,    -1,   146,    28,    -1,    -1,
      -1,    -1,    33,    34,    35,    -1,    37,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    47,    -1,    49,    50,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    74,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,   113,   114,    -1,    -1,    -1,    -1,   119,    -1,
      -1,     3,     4,     5,     6,     7,     8,   128,    -1,   130,
      12,   132,   133,    15,    -1,    -1,    -1,   138,    -1,    -1,
      -1,   142,    -1,   144,    -1,   146,    28,    -1,    -1,    -1,
      -1,    -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    74,    75,    76,    77,    -1,    -1,    -1,    81,
      82,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,   113,   114,    -1,    -1,    -1,    -1,   119,    -1,    -1,
       3,     4,     5,     6,     7,     8,   128,    -1,   130,    12,
     132,   133,    15,    -1,    -1,    -1,   138,    -1,    -1,    -1,
     142,    -1,   144,    -1,   146,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    58,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
     113,   114,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,    -1,   130,    -1,   132,
     133,   134,    -1,    -1,    -1,   138,    -1,    -1,    -1,   142,
     143,   144,    -1,   146,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
     119,    -1,    -1,     3,     4,     5,     6,     7,     8,   128,
      -1,   130,    12,   132,   133,    15,    -1,    -1,    -1,   138,
      -1,    -1,    -1,   142,    -1,   144,    -1,   146,    28,    -1,
      -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,    49,
      50,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    74,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,   113,   114,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,
     130,    -1,   132,   133,   134,    -1,    -1,    -1,   138,    -1,
      -1,    -1,   142,    -1,   144,    -1,   146,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    34,    35,
      -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    47,    -1,    49,    50,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,    -1,
      -1,    -1,    -1,   119,    -1,    -1,     3,     4,     5,     6,
       7,     8,   128,    -1,   130,    12,   132,   133,    15,    -1,
      -1,    -1,   138,    -1,    -1,    -1,   142,    -1,   144,    -1,
     146,    28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,
      37,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      47,    -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,   113,   114,    -1,    -1,
      -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,    -1,   130,    -1,   132,   133,   134,    -1,    -1,
      -1,   138,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    58,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
     113,   114,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,    -1,   130,    -1,   132,
     133,   134,    -1,    -1,    -1,   138,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,   130,    -1,   132,   133,   134,    -1,    -1,    -1,   138,
      -1,    -1,    -1,   142,    -1,   144,    -1,   146,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    34,
      35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    58,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,
      -1,    -1,    -1,    -1,   119,    -1,    -1,     3,     4,     5,
       6,     7,     8,   128,    -1,   130,    12,   132,   133,    15,
      -1,    -1,    -1,   138,    -1,    -1,    -1,   142,    -1,   144,
      -1,   146,    28,    -1,    -1,    -1,    -1,    -1,    34,    35,
      -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    47,    -1,    49,    50,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,    -1,
      -1,    -1,    -1,   119,    -1,    -1,     3,     4,     5,     6,
       7,     8,   128,    -1,   130,    12,   132,   133,    15,    -1,
      -1,    -1,   138,    -1,    -1,    -1,   142,    -1,   144,    -1,
     146,    28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,
      37,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      47,    -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,   113,   114,    -1,    -1,
      -1,    -1,   119,    -1,    -1,     3,     4,     5,     6,     7,
       8,   128,    -1,   130,    12,   132,   133,    15,    -1,    -1,
      -1,   138,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      58,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,   113,   114,    -1,    -1,    -1,
      -1,   119,    -1,    -1,     3,     4,     5,     6,     7,     8,
     128,    -1,   130,    12,   132,   133,    15,    -1,    -1,    -1,
     138,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
     119,    -1,    -1,     3,     4,     5,     6,     7,     8,   128,
      -1,   130,    12,   132,   133,    15,    -1,    -1,    -1,   138,
      -1,    -1,    -1,   142,    -1,   144,    -1,   146,    28,    -1,
      -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,    49,
      50,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    74,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,   113,   114,    -1,    -1,    -1,    -1,   119,
      -1,    -1,     3,     4,     5,     6,     7,     8,   128,    -1,
     130,    12,   132,   133,    15,    -1,    -1,    -1,   138,    -1,
      -1,    -1,   142,    -1,   144,    -1,   146,    28,    -1,    -1,
      -1,    -1,    -1,    34,    35,    -1,    37,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    47,    -1,    49,    50,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    74,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,   113,   114,    -1,    -1,    -1,    -1,   119,    -1,
      -1,     3,     4,     5,     6,     7,     8,   128,    -1,   130,
      12,   132,   133,    15,    -1,    -1,    -1,   138,    -1,    -1,
      -1,   142,    -1,   144,    -1,   146,    28,    -1,    -1,    -1,
      -1,    -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    74,    75,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,   113,   114,    -1,    -1,    -1,    -1,   119,    -1,    -1,
       3,     4,     5,     6,     7,     8,   128,    -1,   130,    12,
     132,   133,    15,    -1,    -1,    -1,   138,    -1,    -1,    -1,
     142,    -1,   144,    -1,   146,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    58,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
     113,   114,    -1,    -1,    -1,    -1,   119,    -1,    -1,     3,
       4,     5,     6,     7,     8,   128,    -1,   130,    12,   132,
     133,    15,    -1,    -1,    -1,   138,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,    28,    -1,    -1,    -1,    -1,    -1,
      34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    58,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,
      74,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,
     114,    -1,    -1,    -1,    -1,   119,    -1,    -1,     3,     4,
       5,     6,     7,     8,   128,    -1,   130,    12,   132,   133,
      15,    -1,    -1,    -1,   138,    -1,    -1,    -1,   142,    -1,
     144,    -1,   146,    28,    -1,    -1,    -1,    -1,    -1,    34,
      35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    58,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,
      -1,    -1,    -1,    -1,   119,    -1,    -1,     3,     4,     5,
       6,     7,     8,   128,    -1,   130,    12,   132,   133,    15,
      -1,    -1,    -1,   138,    -1,    -1,    -1,   142,    -1,   144,
      -1,   146,    28,    -1,    -1,    -1,    -1,    -1,    34,    35,
      -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    47,    -1,    49,    50,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,    -1,
      -1,    -1,    -1,   119,    -1,    -1,     3,     4,     5,     6,
       7,     8,   128,    -1,   130,    12,   132,   133,    15,    -1,
      -1,    -1,   138,    -1,    -1,    -1,   142,    -1,   144,    -1,
     146,    28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,
      37,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      47,    -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,   113,   114,    -1,    -1,
      -1,    -1,   119,    -1,    -1,     3,     4,     5,     6,     7,
       8,   128,    -1,   130,    12,   132,   133,    15,    -1,    -1,
      -1,   138,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      58,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,   113,   114,    28,    -1,    -1,
      -1,    -1,    -1,    34,    35,    -1,    37,    -1,    39,    -1,
     128,    -1,   130,    44,    45,   133,    47,    -1,    49,    50,
     138,    -1,    -1,    -1,   142,    56,   144,    58,   146,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    74,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,   120,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   130,
      -1,    12,    -1,   134,    15,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,   144,   145,   146,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    34,    35,    -1,    37,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    47,    -1,    49,    50,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    74,    75,    76,    77,    -1,     3,     4,
       5,     6,     7,     8,    -1,    86,    -1,    12,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    34,
      35,   112,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,   130,
      -1,    56,    -1,    58,    -1,    60,    -1,    -1,    -1,    -1,
       3,   142,    -1,   144,    -1,   146,    -1,    10,    73,    74,
      75,    76,    77,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,    -1,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    -1,    -1,   117,    -1,    -1,    -1,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,    -1,
     133,    -1,    -1,   136,   137,   138,   139,    -1,   141,    -1,
      -1,   144,    97,    98,    99,   100,   101,   102,    -1,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      -1,    -1,   117,    -1,    -1,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,    -1,   133,    10,
      -1,   136,   137,   138,   139,    16,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,    66,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    -1,    10,    -1,    96,    -1,    -1,    -1,    16,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    70,    -1,    26,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
     131,   132,   133,    96,    -1,   136,   137,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   111,    66,
     113,   114,   115,    70,   117,   118,   119,    -1,   121,   122,
     123,   124,   125,   126,   127,   128,   129,    -1,   131,   132,
     133,    10,    -1,   136,   137,   138,    -1,    16,    -1,    96,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,    -1,   121,   122,   123,   124,   125,   126,
     127,   128,   129,    -1,   131,   132,   133,    10,    -1,   136,
     137,   138,    -1,    16,    -1,    -1,    -1,    66,    -1,    -1,
     147,    70,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    66,   113,   114,   115,    70,   117,   118,
     119,    -1,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,   131,   132,   133,    10,    -1,   136,   137,   138,
      -1,    16,    -1,    96,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,    -1,   121,   122,
     123,   124,   125,   126,   127,   128,   129,    -1,   131,   132,
     133,    -1,    -1,   136,   137,   138,    -1,    -1,    -1,    -1,
      -1,    66,   145,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    10,    -1,    -1,    -1,    -1,    -1,
      16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
     105,   106,   107,   108,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,    -1,
      66,   136,   137,   138,    70,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    78,    10,    -1,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      96,    97,    98,    99,   100,   101,   102,    -1,    -1,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,    -1,   121,   122,   123,   124,   125,
     126,   127,   128,   129,    -1,   131,   132,   133,    -1,    66,
     136,   137,   138,    70,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    78,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,   105,   106,
     107,   108,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,    -1,   121,   122,   123,   124,   125,   126,
     127,   128,   129,    -1,   131,   132,   133,    -1,    66,   136,
     137,   138,    70,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      78,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,    -1,   121,   122,   123,   124,   125,   126,   127,
     128,   129,    -1,   131,   132,   133,    -1,    66,   136,   137,
     138,    70,    -1,    -1,   142,    -1,    -1,    -1,    -1,    78,
      10,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    96,    97,    98,
      99,   100,   101,   102,    -1,    -1,   105,   106,   107,   108,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,    -1,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,   131,   132,   133,    -1,    66,   136,   137,   138,
      70,    -1,    -1,   142,    -1,    -1,    -1,    -1,    78,    10,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    96,    97,    98,    99,
     100,   101,   102,    -1,    -1,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
      -1,   121,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,   131,   132,   133,    -1,    66,   136,   137,   138,    70,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    78,    10,    -1,
      -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    96,    97,    98,    99,   100,
     101,   102,    -1,    -1,   105,   106,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
     131,   132,   133,    -1,    66,   136,   137,   138,    70,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    78,    10,    -1,    -1,
      -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    96,    97,    98,    99,   100,   101,
     102,    -1,    -1,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,    -1,   121,
     122,   123,   124,   125,   126,   127,   128,   129,    -1,   131,
     132,   133,    -1,    66,   136,   137,   138,    70,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,   105,   106,   107,   108,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,    -1,   121,   122,
     123,   124,   125,   126,   127,   128,   129,    -1,   131,   132,
     133,    10,    -1,   136,   137,   138,    -1,    16,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      -1,    70,    -1,    38,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    96,    -1,    -1,
      -1,    66,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,    -1,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    96,   131,   132,   133,    -1,    -1,   136,   137,   138,
      -1,    -1,    -1,   142,    -1,    -1,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,    10,
      -1,   136,   137,   138,    -1,    16,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    96,    -1,    -1,    -1,    66,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    96,
     131,   132,   133,    10,    -1,   136,   137,   138,    -1,    16,
      -1,   142,    -1,    -1,   111,    -1,   113,   114,   115,    26,
     117,   118,   119,    -1,   121,   122,   123,   124,   125,   126,
     127,   128,   129,    -1,   131,   132,   133,    10,    -1,   136,
     137,   138,    -1,    16,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    66,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    -1,    26,    66,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,    -1,   121,   122,   123,   124,   125,   126,
     127,   128,   129,    96,   131,   132,   133,    -1,    -1,   136,
     137,   138,    66,    -1,    -1,   142,    70,    -1,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,    -1,   121,   122,
     123,   124,   125,   126,   127,   128,   129,    -1,   131,   132,
     133,    -1,    96,   136,   137,   138,    10,    -1,    -1,   142,
      -1,    -1,    16,    -1,    -1,    -1,    -1,   111,    -1,   113,
     114,   115,    26,   117,   118,   119,    -1,   121,   122,   123,
     124,   125,   126,   127,   128,   129,    -1,   131,   132,   133,
      10,    -1,   136,   137,   138,    -1,    16,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    31,    66,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    -1,    -1,    66,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,    -1,   121,   122,   123,
     124,   125,   126,   127,   128,   129,    96,   131,   132,   133,
      -1,    -1,   136,   137,   138,    -1,    -1,   141,    -1,    -1,
      -1,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
      -1,   121,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,   131,   132,   133,    10,   135,   136,   137,   138,    -1,
      16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    31,
      66,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      96,    -1,    -1,    -1,    66,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,    -1,   121,   122,   123,   124,   125,
     126,   127,   128,   129,    96,   131,   132,   133,    -1,    -1,
     136,   137,   138,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,    -1,   121,
     122,   123,   124,   125,   126,   127,   128,   129,    -1,   131,
     132,   133,    10,    -1,   136,   137,   138,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    -1,    70,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      -1,    26,    66,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,    -1,   121,   122,   123,   124,   125,   126,   127,
     128,   129,    96,   131,   132,   133,    -1,    -1,   136,   137,
     138,    66,    -1,    -1,    -1,    70,    -1,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,    -1,   121,   122,   123,
     124,   125,   126,   127,   128,   129,    -1,   131,   132,   133,
      -1,    96,   136,   137,   138,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,   111,    -1,   113,   114,
     115,    26,   117,   118,   119,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,    10,
     135,   136,   137,   138,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    -1,    -1,    66,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,    96,   131,   132,   133,    10,
     135,   136,   137,   138,    -1,    16,    -1,    -1,    -1,    -1,
     111,    -1,   113,   114,   115,    26,   117,   118,   119,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
     131,   132,   133,    -1,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    66,    -1,    -1,    -1,    70,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    96,    -1,    -1,    -1,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
     111,    66,   113,   114,   115,    70,   117,   118,   119,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
     131,   132,   133,    -1,   135,   136,   137,   138,    -1,    -1,
      95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    -1,    -1,    70,    -1,    -1,   111,    -1,   113,   114,
     115,    78,   117,   118,   119,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,    96,
      -1,   136,   137,   138,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    16,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,    26,   121,   122,   123,   124,   125,   126,
     127,   128,   129,    -1,   131,   132,   133,    -1,    -1,   136,
     137,   138,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,    66,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      -1,    10,    11,    96,    -1,    -1,    -1,    16,    -1,    -1,
      -1,    66,    -1,    -1,    -1,    70,    -1,    26,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,    -1,   121,   122,
     123,   124,   125,   126,   127,   128,   129,    -1,   131,   132,
     133,    96,    -1,   136,   137,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    66,   113,   114,
     115,    70,   117,   118,   119,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,    10,
     135,   136,   137,   138,    -1,    16,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    29,    -1,
      -1,    -1,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,    -1,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,   131,   132,   133,    10,    -1,   136,   137,   138,
      -1,    16,    -1,    -1,    -1,    66,    -1,    -1,    -1,    70,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    66,   113,   114,   115,    70,   117,   118,   119,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
     131,   132,   133,    10,    -1,   136,   137,   138,    -1,    16,
      95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,    -1,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,    10,
      -1,   136,   137,   138,    -1,    16,    -1,    -1,    -1,    66,
      -1,    -1,    -1,    70,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    96,
      -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,   111,    66,   113,   114,   115,    70,
     117,   118,   119,    -1,   121,   122,   123,   124,   125,   126,
     127,   128,   129,    -1,   131,   132,   133,    -1,    -1,   136,
     137,   138,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    70,    -1,    -1,
     111,    -1,   113,   114,   115,    78,   117,   118,   119,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
     131,   132,   133,    96,    -1,   136,   137,   138,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    16,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,    26,   121,   122,
     123,   124,   125,   126,   127,   128,   129,    -1,   131,   132,
     133,    -1,    -1,   136,   137,   138,    -1,    -1,    -1,    10,
      11,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    66,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    16,    -1,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    26,    -1,    66,    -1,    -1,    -1,    70,
      -1,    -1,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,    -1,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,   131,   132,   133,    96,    -1,   136,   137,   138,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    70,    -1,    -1,
     111,    -1,   113,   114,   115,    78,   117,   118,   119,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
     131,   132,   133,    96,    -1,   136,   137,   138,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    16,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,    26,   121,   122,
     123,   124,   125,   126,   127,   128,   129,    -1,   131,   132,
     133,    -1,    -1,   136,   137,   138,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    66,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    16,    -1,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    26,    -1,    66,    -1,    -1,    -1,    70,
      -1,    -1,   111,    -1,   113,   114,   115,    78,   117,   118,
     119,    -1,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,   131,   132,   133,    96,    -1,   136,   137,   138,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    70,    -1,    -1,
     111,    -1,   113,   114,   115,    78,   117,   118,   119,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
     131,   132,   133,    96,    -1,   136,   137,   138,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    16,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,    26,   121,   122,
     123,   124,   125,   126,   127,   128,   129,    -1,   131,   132,
     133,    -1,    -1,   136,   137,   138,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    66,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    16,    -1,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    26,    -1,    66,    -1,    -1,    -1,    70,
      -1,    -1,   111,    -1,   113,   114,   115,    78,   117,   118,
     119,    -1,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,   131,   132,   133,    96,    -1,   136,   137,   138,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    70,    -1,    -1,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
     131,   132,   133,    96,    -1,   136,   137,   138,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    16,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,    26,   121,   122,
     123,   124,   125,   126,   127,   128,   129,    -1,   131,   132,
     133,    -1,    -1,   136,   137,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,    -1,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,   131,   132,   133,    -1,    -1,   136,   137,   138
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   152,   153,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    15,    18,    19,    20,    21,    23,
      24,    25,    27,    28,    30,    32,    33,    34,    35,    36,
      37,    39,    40,    43,    44,    45,    46,    47,    49,    50,
      52,    56,    58,    59,    60,    61,    64,    65,    68,    69,
      71,    72,    73,    74,    75,    76,    77,    79,    81,    82,
      83,    85,    86,    87,    91,    93,    96,   111,   112,   113,
     114,   119,   128,   130,   132,   133,   135,   138,   142,   144,
     146,   154,   155,   156,   157,   158,   160,   165,   166,   167,
     169,   172,   173,   174,   175,   176,   177,   179,   180,   181,
     185,   186,   189,   190,   193,   194,   197,   200,   201,   224,
     227,   228,   246,   247,   248,   249,   250,   251,   259,   260,
     261,   262,   264,   265,   266,   267,   268,   269,   271,   272,
     273,   274,   275,   135,   156,   262,    92,   252,   253,   168,
     169,    12,    34,    35,    37,    77,   142,   146,   197,   246,
     250,   260,   261,   262,   264,   266,   267,   252,   262,    83,
     168,   156,   240,   262,   156,   144,     7,   168,   170,     9,
      83,   170,    57,    94,   178,   262,   262,    21,    33,   227,
     262,   262,   144,   169,   198,   144,   169,   229,   230,    27,
     160,   171,   262,   262,   262,   262,   262,   262,     7,   144,
      33,   174,   174,   134,   220,   239,   262,    81,    82,   135,
     262,   263,   262,   171,   262,   262,   262,    76,   144,   156,
     262,   262,   160,   167,   262,   264,   160,   167,   262,   169,
     225,   262,   239,   262,   262,   262,   262,   262,   262,   262,
     262,     1,   143,   154,   161,   239,    84,   120,   220,   241,
     242,   263,   239,   262,   270,    59,   156,    63,   159,   169,
     169,    42,    62,   215,    22,    57,    67,    88,   135,   141,
      10,    16,    26,    66,    70,    96,   111,   113,   114,   115,
     117,   118,   119,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   131,   132,   133,   136,   137,   138,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   139,   140,   144,   146,    66,    70,   144,   156,
     135,   262,   262,   262,   239,   142,   160,    38,   252,   225,
     135,   116,   135,    91,   169,   220,   243,   244,   245,   263,
     225,   190,   169,   144,   171,    27,    38,   171,    27,    38,
      92,   171,   255,    31,    53,   135,    78,    97,    98,    99,
     100,   101,   102,   105,   106,   107,   108,   109,   110,   160,
     207,   243,   156,   144,   209,    84,   144,   169,   231,   232,
       1,   115,   234,    38,   116,   156,   171,   171,   243,   170,
     169,   116,   135,   262,   262,   135,   142,   171,   144,   243,
     135,   182,   135,   182,   135,    97,   226,   135,   135,    31,
      53,   171,   135,   143,   143,   154,   116,   143,   262,   116,
     145,   116,   145,    38,   116,   147,   255,    95,   116,   147,
       7,    48,   115,   191,   142,   202,    67,   229,   229,   229,
     229,   262,   262,   262,   262,   178,   262,   178,   262,   262,
     262,   262,   262,   262,   262,    28,    83,   169,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     243,   243,   178,   262,   178,   262,    22,    38,    67,    88,
     254,   257,   258,   262,   275,    27,    38,    27,    38,    78,
      38,   147,   178,   262,   171,   135,   262,   262,    97,   145,
     116,   135,   169,    38,   239,    37,   262,   178,   262,    37,
     262,   178,   262,   144,   171,   138,   162,   164,   262,   162,
     163,   156,   262,    29,   262,   145,    22,    38,    41,    55,
      57,    67,    83,   210,   211,   212,   213,   199,   232,   116,
     145,    15,    28,    56,    73,    74,    77,    86,   146,   235,
     247,    97,   233,   262,   230,   145,   220,   262,     1,   187,
     243,   145,    17,   183,   235,   247,   116,   164,   163,   143,
     145,   145,   241,   145,   241,   178,   262,   147,   156,   262,
     147,   262,   147,   262,   169,   239,   142,     1,   169,   195,
     196,    22,    57,    67,    83,   204,   214,   229,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   145,   147,
      38,    67,   116,   145,   246,    66,    66,    37,   178,   262,
      37,   178,   262,   178,   262,   252,   252,   135,   220,   263,
     245,   191,   262,   145,   262,    27,   171,    27,   171,   262,
      27,   171,   255,    27,   171,   255,   256,   257,   116,   135,
      11,   135,    29,    29,   156,    78,   160,    38,    67,   116,
     145,   144,   169,    22,    57,    67,    83,   216,   145,   232,
     234,     1,   239,    51,   263,   143,    54,    89,   143,   188,
     145,   144,   160,   169,   184,   225,   135,   135,   147,   255,
     147,   255,   156,    95,   142,     1,   192,   143,    97,   116,
     143,    67,   203,    10,    16,   111,   112,   113,   114,   117,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   133,   136,   137,   138,   139,   141,   144,   169,   205,
     206,   207,   246,   135,   257,   234,   246,   246,   262,    27,
      27,   262,    27,    27,   147,   147,   171,   171,   142,   171,
      78,    37,   262,    37,   262,    78,    37,   262,   171,    37,
     262,   171,   116,   145,   262,   262,   156,   262,   156,    29,
     211,   232,   115,   238,    67,   234,   233,   147,    38,   147,
      27,   156,   239,   184,   115,   160,    37,   156,   262,   147,
      37,   156,   262,   147,   262,     1,   143,   161,   143,    33,
      61,    64,   143,   155,   173,   262,   196,   216,   169,   262,
     118,   144,   208,   208,   233,    78,    37,    37,    78,    37,
      37,    37,    37,     1,   192,   262,   262,   262,   262,   262,
     262,   257,    11,    29,   156,   145,    15,    28,    56,    73,
      74,    77,    86,   146,   220,   237,   247,   120,   222,   233,
      90,   223,   262,   235,   247,   156,   171,   145,   262,   262,
     156,   262,   156,   143,   143,   173,   234,   145,   169,   206,
     207,   210,   262,   262,   262,   262,   262,   262,   143,   143,
      78,    78,    78,    78,   262,   156,   238,   147,   239,   220,
     221,   262,   262,   160,   172,   219,   147,   160,    78,    78,
      80,   217,   208,   208,   145,    78,    78,    78,    78,    78,
      78,   262,   262,   262,   262,   222,   233,   220,   236,   237,
     247,    38,   147,   247,   262,   262,   223,   262,   236,   237,
     135,   218,   219,   147,   236
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
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
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
#line 1018 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 1024 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 1031 "chapel.ypp"
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

  case 187:
/* Line 1792 of yacc.c  */
#line 1053 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 1057 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 1062 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 1066 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1074 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1079 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1080 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1081 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1082 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1083 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1084 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1085 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1086 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1087 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1089 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1090 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1091 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1092 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1094 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1096 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1097 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1099 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1101 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1102 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1103 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1107 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1108 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1111 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1112 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1113 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1114 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1115 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1116 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1117 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1118 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1122 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1123 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1127 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1132 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1133 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1140 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1142 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1144 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1148 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1149 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1153 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1154 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1155 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1156 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1157 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1158 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1159 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1160 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1161 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1165 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1166 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1167 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1168 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1169 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1170 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1174 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1175 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1179 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1180 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1181 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1182 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1183 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1184 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1188 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1192 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1198 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1203 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1205 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1209 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1214 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1215 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1219 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1220 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1225 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1227 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1234 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1247 "chapel.ypp"
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

  case 285:
/* Line 1792 of yacc.c  */
#line 1263 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1265 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1267 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1272 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1280 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1288 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1305 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1314 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1322 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1330 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1332 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1337 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1341 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1346 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1348 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1350 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1356 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1357 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1358 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1363 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1364 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1365 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1366 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1367 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1370 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1393 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1397 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1401 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1409 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1415 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1416 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1417 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1422 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1424 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1430 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1432 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1434 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1438 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1439 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1440 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1441 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1442 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1443 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1444 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1445 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1446 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1447 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1448 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1454 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1455 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1456 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1457 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1461 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1462 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1466 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1467 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1472 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1473 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1487 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1488 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1490 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1494 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1517 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_to_borrowed", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1527 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_to_unmanaged", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1536 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1542 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1544 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1546 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1548 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1550 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1552 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1554 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1556 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1560 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1567 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1573 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1579 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1585 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1594 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1619 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1623 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1633 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1637 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1641 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1642 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1647 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1651 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1655 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1661 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1662 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1663 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1664 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1665 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1666 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1672 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1677 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1682 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1698 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1704 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1708 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1712 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1713 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1736 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1737 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1738 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1742 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1743 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1744 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1752 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1753 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1754 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1755 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1759 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1760 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1761 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1762 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1763 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1764 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1765 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1766 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1767 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1769 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1773 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1780 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1781 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1785 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1786 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1787 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1788 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1789 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1790 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1791 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1792 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1793 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1794 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1795 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1796 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1797 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1798 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1799 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1800 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1801 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1802 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1803 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1804 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1805 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1806 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1807 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1811 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1812 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1813 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1814 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1815 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1816 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1820 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1821 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1822 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1823 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1827 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1828 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 1829 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1830 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1835 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1836 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1837 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1838 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1839 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1840 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1841 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 8414 "bison-chapel.cpp"
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


