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
    std::vector<OnlyRename*>* ponlylist;
  };

  #endif

/* Line 387 of yacc.c  */
#line 130 "chapel.ypp"

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
#line 152 "chapel.ypp"

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
#line 234 "bison-chapel.cpp"

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
     TBY = 270,
     TCATCH = 271,
     TCLASS = 272,
     TCOBEGIN = 273,
     TCOFORALL = 274,
     TCONFIG = 275,
     TCONST = 276,
     TCONTINUE = 277,
     TDEFER = 278,
     TDELETE = 279,
     TDMAPPED = 280,
     TDO = 281,
     TDOMAIN = 282,
     TELSE = 283,
     TENUM = 284,
     TEXCEPT = 285,
     TEXPORT = 286,
     TEXTERN = 287,
     TFOR = 288,
     TFORALL = 289,
     TFORWARDING = 290,
     TIF = 291,
     TIN = 292,
     TINDEX = 293,
     TINLINE = 294,
     TINOUT = 295,
     TITER = 296,
     TLABEL = 297,
     TLAMBDA = 298,
     TLET = 299,
     TLOCAL = 300,
     TMINUSMINUS = 301,
     TMODULE = 302,
     TNEW = 303,
     TNIL = 304,
     TNOINIT = 305,
     TON = 306,
     TONLY = 307,
     TOTHERWISE = 308,
     TOUT = 309,
     TOWNED = 310,
     TPARAM = 311,
     TPLUSPLUS = 312,
     TPRAGMA = 313,
     TPRIMITIVE = 314,
     TPRIVATE = 315,
     TPROC = 316,
     TPROTOTYPE = 317,
     TPUBLIC = 318,
     TRECORD = 319,
     TREDUCE = 320,
     TREF = 321,
     TREQUIRE = 322,
     TRETURN = 323,
     TSCAN = 324,
     TSELECT = 325,
     TSERIAL = 326,
     TSHARED = 327,
     TSINGLE = 328,
     TSPARSE = 329,
     TSUBDOMAIN = 330,
     TSYNC = 331,
     TTHEN = 332,
     TTHROW = 333,
     TTHROWS = 334,
     TTRY = 335,
     TTRYBANG = 336,
     TTYPE = 337,
     TUNDERSCORE = 338,
     TUNION = 339,
     TUNMANAGED = 340,
     TUSE = 341,
     TVAR = 342,
     TWHEN = 343,
     TWHERE = 344,
     TWHILE = 345,
     TWITH = 346,
     TYIELD = 347,
     TZIP = 348,
     TALIAS = 349,
     TAND = 350,
     TASSIGN = 351,
     TASSIGNBAND = 352,
     TASSIGNBOR = 353,
     TASSIGNBXOR = 354,
     TASSIGNDIVIDE = 355,
     TASSIGNEXP = 356,
     TASSIGNLAND = 357,
     TASSIGNLOR = 358,
     TASSIGNMINUS = 359,
     TASSIGNMOD = 360,
     TASSIGNMULTIPLY = 361,
     TASSIGNPLUS = 362,
     TASSIGNSL = 363,
     TASSIGNSR = 364,
     TBAND = 365,
     TBNOT = 366,
     TBOR = 367,
     TBXOR = 368,
     TCOLON = 369,
     TCOMMA = 370,
     TDIVIDE = 371,
     TDOT = 372,
     TDOTDOT = 373,
     TDOTDOTDOT = 374,
     TEQUAL = 375,
     TEXP = 376,
     TGREATER = 377,
     TGREATEREQUAL = 378,
     THASH = 379,
     TLESS = 380,
     TLESSEQUAL = 381,
     TMINUS = 382,
     TMOD = 383,
     TNOT = 384,
     TNOTEQUAL = 385,
     TOR = 386,
     TPLUS = 387,
     TQUESTION = 388,
     TSEMI = 389,
     TSHIFTLEFT = 390,
     TSHIFTRIGHT = 391,
     TSTAR = 392,
     TSWAP = 393,
     TASSIGNREDUCE = 394,
     TIO = 395,
     TLCBR = 396,
     TRCBR = 397,
     TLP = 398,
     TRP = 399,
     TLSBR = 400,
     TRSBR = 401,
     TNOELSE = 402,
     TUMINUS = 403,
     TUPLUS = 404
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
#line 183 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);


/* Line 387 of yacc.c  */
#line 448 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 455 "bison-chapel.cpp"
/* Unqualified %code blocks.  */
/* Line 391 of yacc.c  */
#line 38 "chapel.ypp"

  #include <string>
  int         captureTokens;
  std::string captureString;

/* Line 391 of yacc.c  */
#line 191 "chapel.ypp"

  #include "build.h"
  #include "CatchStmt.h"
  #include "DeferStmt.h"
  #include "DoWhileStmt.h"
  #include "driver.h"
  #include "flex-chapel.h"
  #include "ForallStmt.h"
  #include "ForLoop.h"
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
#line 533 "bison-chapel.cpp"

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
#define YYLAST   13668

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  150
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  124
/* YYNRULES -- Number of rules.  */
#define YYNRULES  501
/* YYNRULES -- Number of states.  */
#define YYNSTATES  947

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   404

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
     145,   146,   147,   148,   149
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
    1046,  1049,  1052,  1054,  1059,  1064,  1071,  1075,  1076,  1078,
    1080,  1084,  1089,  1093,  1098,  1105,  1106,  1109,  1112,  1115,
    1118,  1121,  1124,  1127,  1130,  1133,  1135,  1137,  1141,  1145,
    1147,  1151,  1153,  1155,  1157,  1161,  1165,  1166,  1168,  1170,
    1174,  1178,  1182,  1184,  1186,  1188,  1190,  1192,  1194,  1196,
    1198,  1201,  1206,  1211,  1216,  1222,  1225,  1228,  1231,  1234,
    1237,  1244,  1251,  1256,  1266,  1276,  1284,  1291,  1298,  1303,
    1313,  1323,  1331,  1336,  1343,  1350,  1360,  1370,  1377,  1379,
    1381,  1383,  1385,  1387,  1389,  1391,  1393,  1397,  1398,  1400,
    1405,  1407,  1411,  1416,  1418,  1422,  1425,  1429,  1433,  1435,
    1439,  1442,  1447,  1449,  1451,  1453,  1455,  1457,  1459,  1461,
    1463,  1468,  1472,  1476,  1479,  1482,  1484,  1487,  1490,  1492,
    1494,  1496,  1498,  1500,  1502,  1504,  1509,  1514,  1519,  1523,
    1527,  1531,  1535,  1540,  1544,  1549,  1551,  1553,  1555,  1557,
    1559,  1563,  1568,  1572,  1577,  1581,  1586,  1590,  1596,  1600,
    1604,  1608,  1612,  1616,  1620,  1624,  1628,  1632,  1636,  1640,
    1644,  1648,  1652,  1656,  1660,  1664,  1668,  1672,  1676,  1680,
    1684,  1688,  1691,  1694,  1697,  1700,  1703,  1706,  1710,  1714,
    1718,  1722,  1726,  1730,  1734,  1738,  1740,  1742,  1744,  1746,
    1748,  1750
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     151,     0,    -1,   152,    -1,    -1,   152,   153,    -1,   155,
      -1,   154,   155,    -1,    58,     7,    -1,   154,    58,     7,
      -1,   156,    -1,   159,    -1,   164,    -1,   165,    -1,   172,
      -1,   166,    -1,   175,    -1,   178,    -1,   176,    -1,   185,
      -1,   179,    -1,   180,    -1,   184,    -1,   171,    -1,   250,
     134,    -1,    12,   155,    -1,    13,   251,   155,    -1,    14,
     167,   134,    -1,    18,   251,   159,    -1,    22,   167,   134,
      -1,    24,   239,   134,    -1,    42,   168,   155,    -1,    45,
     260,   170,    -1,    45,   170,    -1,    51,   260,   170,    -1,
      71,   260,   170,    -1,    71,   170,    -1,    76,   155,    -1,
      92,   260,   134,    -1,     1,   134,    -1,   157,   158,    47,
     168,   141,   142,    -1,   157,   158,    47,   168,   141,   160,
     142,    -1,   157,   158,    47,   168,   141,     1,   142,    -1,
      -1,    63,    -1,    60,    -1,    -1,    62,    -1,   141,   142,
      -1,   141,   160,   142,    -1,   141,     1,   142,    -1,   153,
      -1,   160,   153,    -1,   260,    -1,   260,    11,   260,    -1,
     161,   115,   260,    -1,   161,   115,   260,    11,   260,    -1,
      -1,   161,    -1,   137,    -1,   161,    -1,    86,   238,   134,
      -1,    86,   260,    30,   163,   134,    -1,    86,   260,    52,
     162,   134,    -1,    67,   238,   134,    -1,   262,    96,   261,
     134,    -1,   262,   107,   261,   134,    -1,   262,   104,   261,
     134,    -1,   262,   106,   261,   134,    -1,   262,   100,   261,
     134,    -1,   262,   105,   261,   134,    -1,   262,   101,   261,
     134,    -1,   262,    97,   261,   134,    -1,   262,    98,   261,
     134,    -1,   262,    99,   261,   134,    -1,   262,   109,   261,
     134,    -1,   262,   108,   261,   134,    -1,   262,   138,   261,
     134,    -1,   262,   139,   261,   134,    -1,   262,   102,   261,
     134,    -1,   262,   103,   261,   134,    -1,    -1,   168,    -1,
       3,    -1,   167,    -1,     7,    -1,    26,   155,    -1,   159,
      -1,    68,   134,    -1,    68,   261,   134,    -1,   134,    -1,
     173,    -1,    63,   173,    -1,    60,   173,    -1,   192,    -1,
     223,    -1,   188,    -1,   174,    -1,   200,    -1,   226,    -1,
      35,   260,   134,    -1,    35,   260,    30,   163,   134,    -1,
      35,   260,    52,   162,   134,    -1,    35,   226,    -1,    32,
       9,    -1,    26,   155,    90,   260,   134,    -1,    90,   260,
     170,    -1,    19,   260,    37,   260,   251,   170,    -1,    19,
     260,    37,   177,   251,   170,    -1,    19,   260,   251,   170,
      -1,    33,   260,    37,   260,   170,    -1,    33,   260,    37,
     177,   170,    -1,    33,   260,   170,    -1,    33,   177,   170,
      -1,    33,    56,   168,    37,   260,   170,    -1,    34,   260,
      37,   260,   170,    -1,    34,   260,    37,   260,   254,   170,
      -1,    34,   260,    37,   177,   170,    -1,    34,   260,    37,
     177,   254,   170,    -1,    34,   260,   170,    -1,    34,   260,
     254,   170,    -1,   145,   238,    37,   260,   146,   155,    -1,
     145,   238,    37,   260,   254,   146,   155,    -1,   145,   238,
      37,   177,   146,   155,    -1,   145,   238,    37,   177,   254,
     146,   155,    -1,   145,   238,   146,   155,    -1,   145,   238,
     254,   146,   155,    -1,    93,   143,   238,   144,    -1,    36,
     260,    77,   155,    -1,    36,   260,   159,    -1,    36,   260,
      77,   155,    28,   155,    -1,    36,   260,   159,    28,   155,
      -1,    36,   260,   206,   260,    77,   155,    -1,    36,   260,
     206,   260,   159,    -1,    36,   260,   206,   260,    77,   155,
      28,   155,    -1,    36,   260,   206,   260,   159,    28,   155,
      -1,    23,   155,    -1,    80,   260,   134,    -1,    81,   260,
     134,    -1,    80,   166,    -1,    81,   166,    -1,    80,   159,
     181,    -1,    81,   159,   181,    -1,    -1,   181,   182,    -1,
      16,   159,    -1,    16,   183,   159,    -1,    16,   143,   183,
     144,   159,    -1,   168,    -1,   168,   114,   260,    -1,    78,
     260,   134,    -1,    70,   260,   141,   186,   142,    -1,    70,
     260,   141,     1,   142,    -1,    -1,   186,   187,    -1,    88,
     238,   170,    -1,    53,   155,    -1,    53,    26,   155,    -1,
     189,   168,   190,   141,   191,   142,    -1,    32,   169,   189,
     168,   190,   141,   191,   142,    -1,   189,   168,   190,   141,
       1,   142,    -1,    32,   169,   189,   168,   190,   141,     1,
     142,    -1,    17,    -1,    64,    -1,    84,    -1,    -1,   114,
     238,    -1,    -1,   191,   172,    -1,   191,   154,   172,    -1,
     193,   168,   141,   194,   142,    -1,   193,   168,   141,     1,
     142,    -1,    29,    -1,   195,    -1,   194,   115,    -1,   194,
     115,   195,    -1,   168,    -1,   168,    96,   260,    -1,    -1,
      -1,    43,   197,   208,   198,   215,   233,   222,   218,    -1,
      -1,    39,    -1,    31,   169,    -1,    32,   169,    -1,    -1,
      -1,   199,   214,   201,   203,   202,   215,   233,   216,   222,
     217,    -1,   213,   205,   207,    -1,   213,   206,   207,    -1,
     213,   204,   117,   205,   207,    -1,   213,   204,   117,   206,
     207,    -1,   245,    -1,   143,   260,   144,    -1,   168,    -1,
     111,   168,    -1,   110,    -1,   112,    -1,   113,    -1,   111,
      -1,   120,    -1,   130,    -1,   126,    -1,   123,    -1,   125,
      -1,   122,    -1,   132,    -1,   127,    -1,   137,    -1,   116,
      -1,   135,    -1,   136,    -1,   128,    -1,   121,    -1,   129,
      -1,    15,    -1,   124,    -1,    10,    -1,   138,    -1,   140,
      -1,    96,    -1,   107,    -1,   104,    -1,   106,    -1,   100,
      -1,   105,    -1,   101,    -1,    97,    -1,    98,    -1,    99,
      -1,   109,    -1,   108,    -1,    -1,   143,   209,   144,    -1,
     143,   209,   144,    -1,    -1,   210,    -1,   209,   115,   210,
      -1,   211,   168,   237,   232,    -1,   211,   168,   237,   221,
      -1,   211,   143,   231,   144,   237,   232,    -1,   211,   143,
     231,   144,   237,   221,    -1,    -1,   212,    -1,    37,    -1,
      40,    -1,    54,    -1,    21,    -1,    21,    37,    -1,    21,
      66,    -1,    56,    -1,    66,    -1,    82,    -1,    -1,    56,
      -1,    66,    -1,    21,    66,    -1,    21,    -1,    82,    -1,
      61,    -1,    41,    -1,    -1,    21,    -1,    21,    66,    -1,
      66,    -1,    56,    -1,    82,    -1,    -1,    79,    -1,   134,
      -1,   218,    -1,   159,    -1,   171,    -1,   133,   168,    -1,
     133,    -1,    -1,   219,    -1,   119,   260,    -1,   119,   220,
      -1,    -1,    89,   260,    -1,    82,   224,   134,    -1,    20,
      82,   224,   134,    -1,    32,    82,   224,   134,    -1,   168,
     225,    -1,   168,   225,   115,   224,    -1,    -1,    96,   246,
      -1,    96,   234,    -1,   227,    56,   228,   134,    -1,   227,
      21,   228,   134,    -1,   227,    66,   228,   134,    -1,   227,
      21,    66,   228,   134,    -1,   227,    87,   228,   134,    -1,
      -1,    20,    -1,    32,    -1,   229,    -1,   228,   115,   229,
      -1,   168,   233,   232,    -1,   143,   231,   144,   233,   232,
      -1,    83,    -1,   168,    -1,   143,   231,   144,    -1,   230,
      -1,   230,   115,    -1,   230,   115,   231,    -1,    -1,    96,
      50,    -1,    96,   261,    -1,    -1,   114,   246,    -1,   114,
     234,    -1,     1,    -1,   145,   238,   146,   246,    -1,   145,
     238,   146,   234,    -1,   145,   238,    37,   260,   146,   246,
      -1,   145,     1,   146,    -1,    -1,   246,    -1,   219,    -1,
     145,   146,   235,    -1,   145,   238,   146,   235,    -1,   145,
     146,   236,    -1,   145,   238,   146,   236,    -1,   145,   238,
      37,   260,   146,   235,    -1,    -1,   114,   246,    -1,   114,
     219,    -1,   114,    27,    -1,   114,    73,    -1,   114,    76,
      -1,   114,    55,    -1,   114,    85,    -1,   114,    72,    -1,
     114,   236,    -1,   260,    -1,   219,    -1,   238,   115,   260,
      -1,   238,   115,   219,    -1,   260,    -1,   239,   115,   260,
      -1,    83,    -1,   261,    -1,   219,    -1,   240,   115,   240,
      -1,   241,   115,   240,    -1,    -1,   243,    -1,   244,    -1,
     243,   115,   244,    -1,   168,    96,   219,    -1,   168,    96,
     261,    -1,   219,    -1,   261,    -1,   168,    -1,   249,    -1,
     262,    -1,   248,    -1,   270,    -1,   269,    -1,    73,   260,
      -1,    38,   143,   242,   144,    -1,    27,   143,   242,   144,
      -1,    75,   143,   242,   144,    -1,    74,    75,   143,   242,
     144,    -1,    12,   260,    -1,    76,   260,    -1,    55,   260,
      -1,    85,   260,    -1,    72,   260,    -1,    33,   260,    37,
     260,    26,   260,    -1,    33,   260,    37,   177,    26,   260,
      -1,    33,   260,    26,   260,    -1,    33,   260,    37,   260,
      26,    36,   260,    77,   260,    -1,    33,   260,    37,   177,
      26,    36,   260,    77,   260,    -1,    33,   260,    26,    36,
     260,    77,   260,    -1,    34,   260,    37,   260,    26,   260,
      -1,    34,   260,    37,   177,    26,   260,    -1,    34,   260,
      26,   260,    -1,    34,   260,    37,   260,    26,    36,   260,
      77,   260,    -1,    34,   260,    37,   177,    26,    36,   260,
      77,   260,    -1,    34,   260,    26,    36,   260,    77,   260,
      -1,   145,   238,   146,   260,    -1,   145,   238,    37,   260,
     146,   260,    -1,   145,   238,    37,   177,   146,   260,    -1,
     145,   238,    37,   260,   146,    36,   260,    77,   260,    -1,
     145,   238,    37,   177,   146,    36,   260,    77,   260,    -1,
      36,   260,    77,   260,    28,   260,    -1,    49,    -1,   249,
      -1,   245,    -1,   265,    -1,   264,    -1,   196,    -1,   258,
      -1,   259,    -1,   257,   140,   260,    -1,    -1,   252,    -1,
      91,   143,   253,   144,    -1,   256,    -1,   253,   115,   256,
      -1,    91,   143,   255,   144,    -1,   256,    -1,   255,   115,
     256,    -1,   212,   245,    -1,   260,    65,   245,    -1,   273,
      65,   245,    -1,   262,    -1,   257,   140,   260,    -1,    48,
     260,    -1,    44,   228,    37,   260,    -1,   267,    -1,   246,
      -1,   247,    -1,   271,    -1,   272,    -1,   196,    -1,   258,
      -1,   259,    -1,   143,   119,   260,   144,    -1,   260,   114,
     260,    -1,   260,   118,   260,    -1,   260,   118,    -1,   118,
     260,    -1,   118,    -1,    80,   260,    -1,    81,   260,    -1,
     260,    -1,   245,    -1,   264,    -1,   265,    -1,   266,    -1,
     262,    -1,   196,    -1,   263,   143,   242,   144,    -1,   263,
     145,   242,   146,    -1,    59,   143,   242,   144,    -1,   260,
     117,   168,    -1,   260,   117,    82,    -1,   260,   117,    27,
      -1,   143,   240,   144,    -1,   143,   240,   115,   144,    -1,
     143,   241,   144,    -1,   143,   241,   115,   144,    -1,     4,
      -1,     5,    -1,     6,    -1,     7,    -1,     8,    -1,   141,
     238,   142,    -1,   141,   238,   115,   142,    -1,   145,   238,
     146,    -1,   145,   238,   115,   146,    -1,   145,   268,   146,
      -1,   145,   268,   115,   146,    -1,   260,    94,   260,    -1,
     268,   115,   260,    94,   260,    -1,   260,   132,   260,    -1,
     260,   127,   260,    -1,   260,   137,   260,    -1,   260,   116,
     260,    -1,   260,   135,   260,    -1,   260,   136,   260,    -1,
     260,   128,   260,    -1,   260,   120,   260,    -1,   260,   130,
     260,    -1,   260,   126,   260,    -1,   260,   123,   260,    -1,
     260,   125,   260,    -1,   260,   122,   260,    -1,   260,   110,
     260,    -1,   260,   112,   260,    -1,   260,   113,   260,    -1,
     260,    95,   260,    -1,   260,   131,   260,    -1,   260,   121,
     260,    -1,   260,    15,   260,    -1,   260,    10,   260,    -1,
     260,   124,   260,    -1,   260,    25,   260,    -1,   132,   260,
      -1,   127,   260,    -1,    46,   260,    -1,    57,   260,    -1,
     129,   260,    -1,   111,   260,    -1,   260,    65,   260,    -1,
     260,    65,   177,    -1,   273,    65,   260,    -1,   273,    65,
     177,    -1,   260,    69,   260,    -1,   260,    69,   177,    -1,
     273,    69,   260,    -1,   273,    69,   177,    -1,   132,    -1,
     137,    -1,    95,    -1,   131,    -1,   110,    -1,   112,    -1,
     113,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   443,   443,   448,   449,   455,   456,   461,   462,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   496,   500,
     503,   505,   511,   512,   513,   517,   518,   531,   532,   533,
     538,   539,   544,   549,   554,   558,   565,   570,   574,   579,
     583,   584,   585,   589,   593,   595,   597,   599,   601,   603,
     605,   607,   609,   611,   613,   615,   617,   619,   621,   623,
     628,   629,   633,   637,   638,   642,   643,   647,   648,   652,
     653,   654,   655,   656,   657,   658,   659,   663,   664,   668,
     669,   670,   671,   675,   690,   691,   692,   693,   694,   695,
     696,   697,   698,   699,   700,   701,   702,   703,   704,   705,
     706,   712,   718,   724,   730,   737,   747,   751,   752,   753,
     754,   755,   757,   759,   761,   766,   769,   770,   771,   772,
     773,   774,   778,   779,   783,   784,   785,   789,   790,   794,
     797,   799,   804,   805,   809,   811,   813,   820,   830,   840,
     850,   863,   868,   873,   881,   882,   887,   888,   890,   895,
     911,   918,   927,   935,   939,   946,   947,   952,   957,   951,
     982,   988,   995,  1003,  1015,  1021,  1014,  1049,  1053,  1058,
    1062,  1070,  1071,  1075,  1076,  1077,  1078,  1079,  1080,  1081,
    1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,  1090,  1091,
    1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,  1104,
    1105,  1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,
    1115,  1119,  1120,  1124,  1128,  1129,  1130,  1134,  1136,  1138,
    1140,  1145,  1146,  1150,  1151,  1152,  1153,  1154,  1155,  1156,
    1157,  1158,  1162,  1163,  1164,  1165,  1166,  1167,  1171,  1172,
    1176,  1177,  1178,  1179,  1180,  1181,  1185,  1186,  1189,  1190,
    1194,  1195,  1199,  1201,  1206,  1207,  1211,  1212,  1216,  1217,
    1221,  1223,  1225,  1230,  1243,  1260,  1261,  1263,  1268,  1276,
    1284,  1292,  1301,  1311,  1312,  1313,  1317,  1318,  1326,  1328,
    1333,  1335,  1337,  1342,  1344,  1346,  1353,  1354,  1355,  1359,
    1360,  1361,  1362,  1382,  1386,  1390,  1398,  1405,  1406,  1407,
    1411,  1413,  1419,  1421,  1423,  1428,  1429,  1430,  1431,  1432,
    1433,  1434,  1435,  1436,  1437,  1443,  1444,  1445,  1446,  1450,
    1451,  1455,  1456,  1457,  1461,  1462,  1466,  1467,  1471,  1472,
    1476,  1477,  1478,  1479,  1483,  1494,  1495,  1496,  1497,  1498,
    1499,  1501,  1503,  1505,  1507,  1509,  1511,  1513,  1515,  1517,
    1522,  1524,  1526,  1528,  1530,  1532,  1534,  1536,  1538,  1540,
    1542,  1544,  1546,  1553,  1559,  1565,  1571,  1580,  1590,  1598,
    1599,  1600,  1601,  1602,  1603,  1604,  1605,  1610,  1611,  1615,
    1619,  1622,  1627,  1631,  1634,  1639,  1642,  1645,  1651,  1652,
    1657,  1662,  1670,  1671,  1672,  1673,  1674,  1675,  1676,  1677,
    1678,  1680,  1682,  1684,  1686,  1688,  1693,  1694,  1695,  1698,
    1699,  1700,  1701,  1712,  1713,  1717,  1718,  1719,  1723,  1724,
    1725,  1733,  1734,  1735,  1736,  1740,  1741,  1742,  1743,  1744,
    1745,  1746,  1747,  1748,  1749,  1753,  1761,  1762,  1766,  1767,
    1768,  1769,  1770,  1771,  1772,  1773,  1774,  1775,  1776,  1777,
    1778,  1779,  1780,  1781,  1782,  1783,  1784,  1785,  1786,  1787,
    1788,  1792,  1793,  1794,  1795,  1796,  1797,  1801,  1802,  1803,
    1804,  1808,  1809,  1810,  1811,  1816,  1817,  1818,  1819,  1820,
    1821,  1822
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "INTLITERAL", "REALLITERAL",
  "IMAGLITERAL", "STRINGLITERAL", "CSTRINGLITERAL", "EXTERNCODE", "TALIGN",
  "TAS", "TATOMIC", "TBEGIN", "TBREAK", "TBY", "TCATCH", "TCLASS",
  "TCOBEGIN", "TCOFORALL", "TCONFIG", "TCONST", "TCONTINUE", "TDEFER",
  "TDELETE", "TDMAPPED", "TDO", "TDOMAIN", "TELSE", "TENUM", "TEXCEPT",
  "TEXPORT", "TEXTERN", "TFOR", "TFORALL", "TFORWARDING", "TIF", "TIN",
  "TINDEX", "TINLINE", "TINOUT", "TITER", "TLABEL", "TLAMBDA", "TLET",
  "TLOCAL", "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT", "TON",
  "TONLY", "TOTHERWISE", "TOUT", "TOWNED", "TPARAM", "TPLUSPLUS",
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
  "intent_expr", "io_expr", "new_expr", "let_expr", "expr", "opt_try_expr",
  "lhs_expr", "fun_expr", "call_expr", "dot_expr", "parenthesized_expr",
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
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   150,   151,   152,   152,   153,   153,   154,   154,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   156,
     156,   156,   157,   157,   157,   158,   158,   159,   159,   159,
     160,   160,   161,   161,   161,   161,   162,   162,   163,   163,
     164,   164,   164,   165,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     167,   167,   168,   169,   169,   170,   170,   171,   171,   172,
     172,   172,   172,   172,   172,   172,   172,   173,   173,   174,
     174,   174,   174,   175,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   177,   178,   178,   178,
     178,   178,   178,   178,   178,   179,   180,   180,   180,   180,
     180,   180,   181,   181,   182,   182,   182,   183,   183,   184,
     185,   185,   186,   186,   187,   187,   187,   188,   188,   188,
     188,   189,   189,   189,   190,   190,   191,   191,   191,   192,
     192,   193,   194,   194,   194,   195,   195,   197,   198,   196,
     199,   199,   199,   199,   201,   202,   200,   203,   203,   203,
     203,   204,   204,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   207,   207,   208,   209,   209,   209,   210,   210,   210,
     210,   211,   211,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   213,   213,   213,   213,   213,   213,   214,   214,
     215,   215,   215,   215,   215,   215,   216,   216,   217,   217,
     218,   218,   219,   219,   220,   220,   221,   221,   222,   222,
     223,   223,   223,   224,   224,   225,   225,   225,   226,   226,
     226,   226,   226,   227,   227,   227,   228,   228,   229,   229,
     230,   230,   230,   231,   231,   231,   232,   232,   232,   233,
     233,   233,   233,   234,   234,   234,   234,   235,   235,   235,
     236,   236,   236,   236,   236,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   238,   238,   238,   238,   239,
     239,   240,   240,   240,   241,   241,   242,   242,   243,   243,
     244,   244,   244,   244,   245,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   248,   249,   250,
     250,   250,   250,   250,   250,   250,   250,   251,   251,   252,
     253,   253,   254,   255,   255,   256,   256,   256,   257,   257,
     258,   259,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   261,   261,   261,   262,
     262,   262,   262,   263,   263,   264,   264,   264,   265,   265,
     265,   266,   266,   266,   266,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   268,   268,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   270,   270,   270,   270,   270,   270,   271,   271,   271,
     271,   272,   272,   272,   272,   273,   273,   273,   273,   273,
     273,   273
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
       2,     2,     1,     4,     4,     6,     3,     0,     1,     1,
       3,     4,     3,     4,     6,     0,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     1,     1,     3,     3,     1,
       3,     1,     1,     1,     3,     3,     0,     1,     1,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     4,     4,     4,     5,     2,     2,     2,     2,     2,
       6,     6,     4,     9,     9,     7,     6,     6,     4,     9,
       9,     7,     4,     6,     6,     9,     9,     6,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     1,     4,
       1,     3,     4,     1,     3,     2,     3,     3,     1,     3,
       2,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     3,     3,     2,     2,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     4,     3,     3,
       3,     3,     4,     3,     4,     1,     1,     1,     1,     1,
       3,     4,     3,     4,     3,     4,     3,     5,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   445,   446,   447,   448,
     449,     0,   397,    80,   161,   397,     0,   294,    80,     0,
       0,     0,     0,   171,    80,    80,     0,     0,   293,     0,
       0,   181,     0,   177,     0,     0,     0,     0,   388,     0,
       0,     0,     0,     0,   293,   293,   162,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     163,     0,     0,     0,     0,   497,   499,     0,   500,   501,
     425,     0,     0,   498,   495,    89,   496,     0,     0,     0,
       4,     0,     5,     9,    45,    10,    11,    12,    14,   354,
      22,    13,    90,    96,    15,    17,    16,    19,    20,    21,
      18,    95,     0,    93,     0,   417,     0,    97,    94,    98,
       0,   429,   413,   414,   357,   355,     0,     0,   418,   419,
       0,   356,     0,   430,   431,   432,   412,   359,   358,   415,
     416,     0,    38,    24,   365,     0,     0,   398,     0,    81,
       0,     0,     0,     0,     0,     0,     0,     0,   417,   429,
     355,   418,   419,   397,   356,   430,   431,     0,     0,   135,
       0,   339,     0,   346,    84,    83,   182,   103,     0,   183,
       0,     0,     0,     0,     0,   294,   295,   102,     0,     0,
     346,     0,     0,     0,     0,     0,   296,     0,    86,    32,
       0,   483,   410,     0,   367,   484,     7,   346,   295,    92,
      91,   273,   336,     0,   335,     0,     0,    87,   428,     0,
       0,    35,     0,   369,   360,     0,   346,    36,   366,     0,
     142,   138,     0,   356,   142,   139,     0,   285,     0,   368,
       0,   335,     0,     0,   486,   424,   482,   485,   481,     0,
      47,    50,     0,     0,   341,     0,   343,     0,     0,   342,
       0,   335,     0,     0,     6,    46,     0,   164,     0,   259,
     258,   184,     0,     0,     0,     0,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     423,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   346,   346,     0,     0,     0,    25,    26,     0,
      27,     0,     0,     0,     0,     0,     0,     0,    28,     0,
      29,     0,   354,   352,     0,   347,   348,   353,     0,     0,
       0,     0,   112,     0,     0,   111,     0,     0,     0,   118,
       0,     0,    56,    99,     0,   219,   226,   227,   228,   223,
     225,   221,   224,   222,   220,   230,   229,   128,     0,     0,
      30,   234,   178,   300,     0,   301,   303,     0,   312,     0,
     306,     0,     0,    85,    31,    33,     0,   183,   272,     0,
      63,   426,   427,    88,     0,    34,   346,     0,   149,   140,
     136,   141,   137,     0,   283,   280,    60,     0,    56,   105,
      37,    49,    48,    51,     0,   450,     0,     0,   441,     0,
     443,     0,     0,     0,     0,     0,     0,   454,     8,     0,
       0,     0,     0,   252,     0,     0,     0,     0,     0,   396,
     478,   477,   480,   488,   487,   492,   491,   474,   471,   472,
     473,   421,   461,   440,   439,   438,   422,   465,   476,   470,
     468,   479,   469,   467,   459,   464,   466,   475,   458,   462,
     463,   460,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     490,   489,   494,   493,   246,   243,   244,   245,   249,   250,
     251,     0,     0,   400,     0,     0,     0,     0,     0,     0,
       0,     0,   452,   397,   397,   108,   281,   340,     0,     0,
     362,     0,   282,   164,     0,     0,     0,   372,     0,     0,
       0,   378,     0,     0,     0,   119,   496,    59,     0,    52,
      57,     0,   127,     0,     0,     0,   361,     0,   235,     0,
     242,   260,     0,   304,     0,     0,   311,   413,     0,   298,
     411,   297,   437,   338,   337,     0,     0,     0,   363,     0,
     143,   287,   413,     0,     0,     0,   451,   420,   442,   344,
     444,   345,     0,     0,   453,   124,   382,     0,   456,   455,
       0,     0,   165,     0,     0,   175,     0,   172,   256,   253,
     254,   257,   185,     0,     0,   289,   288,   290,   292,    64,
      71,    72,    73,    68,    70,    78,    79,    66,    69,    67,
      65,    75,    74,    76,    77,   435,   436,   247,   248,   405,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   104,   350,   351,   349,     0,     0,
     126,     0,     0,   110,     0,   109,     0,     0,   116,     0,
       0,   114,     0,     0,   403,     0,   100,     0,   101,     0,
       0,   130,     0,   132,   241,   233,     0,   325,   261,   264,
     263,   265,     0,   302,   305,   306,     0,     0,   307,   308,
     151,     0,     0,   150,   153,   364,     0,   144,   147,     0,
     284,    61,    62,     0,     0,     0,     0,   125,     0,     0,
       0,   293,   170,     0,   173,   169,   255,   260,   216,   214,
     195,   198,   196,   197,   208,   199,   212,   204,   202,   215,
     203,   201,   206,   211,   213,   200,   205,   209,   210,   207,
     217,   218,     0,   193,     0,   231,   231,   191,   291,   401,
     429,   429,     0,     0,     0,     0,     0,     0,     0,     0,
     107,   106,     0,   113,     0,     0,   371,     0,   370,     0,
       0,   377,   117,     0,   376,   115,     0,   402,    54,    53,
     129,   387,   131,     0,   236,     0,     0,   306,   262,   278,
     299,   316,     0,   452,     0,   155,     0,     0,     0,   145,
       0,   122,   384,     0,     0,   120,   383,     0,   457,     0,
      39,     0,   159,    80,   293,   293,   157,   293,   167,   176,
     174,     0,   194,     0,     0,   234,   187,   188,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   293,   375,     0,
       0,   381,     0,     0,   404,     0,     0,   134,   325,   328,
     331,   333,   329,   330,   332,     0,   327,   334,   413,   274,
     238,   237,     0,     0,     0,   314,   413,   156,   154,     0,
     148,     0,   123,     0,   121,    41,    40,   168,   266,   192,
     193,   231,   231,     0,     0,     0,     0,     0,     0,     0,
     160,   158,     0,     0,     0,     0,    55,   133,   306,   317,
       0,   275,   277,   276,   279,   270,   271,   179,     0,   146,
       0,     0,   267,   278,   189,   190,   232,     0,     0,     0,
       0,     0,     0,   374,   373,   380,   379,   240,   239,   319,
     320,   322,   413,     0,   452,   413,   386,   385,     0,     0,
     321,   323,   268,   186,   269,   317,   324
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   241,    81,   542,    83,    84,   256,    85,
     242,   537,   541,   538,    86,    87,    88,   165,    89,   169,
     189,    90,    91,    92,    93,    94,    95,   640,    96,    97,
      98,   399,   570,   699,    99,   100,   566,   694,   101,   102,
     431,   711,   103,   104,   596,   597,   148,   182,   551,   106,
     107,   433,   717,   602,   744,   745,   368,   826,   372,   547,
     548,   549,   501,   603,   261,   682,   913,   943,   907,   202,
     902,   860,   863,   108,   228,   404,   109,   110,   185,   186,
     376,   377,   559,   380,   556,   930,   857,   787,   243,   160,
     247,   248,   334,   335,   336,   149,   112,   113,   114,   150,
     116,   136,   137,   502,   350,   663,   503,   117,   151,   152,
     208,   337,   154,   122,   155,   156,   125,   126,   252,   127,
     128,   129,   130,   131
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -821
static const yytype_int16 yypact[] =
{
    -821,   100,  2848,  -821,   -19,  -821,  -821,  -821,  -821,  -821,
    -821,  3992,    47,   160,  -821,    47,  8428,    91,   160,  3992,
    8428,  3992,   155,  -821,   245,   436,  6998,  8428,  7141,  8428,
     173,  -821,   160,  -821,    29,  7570,  8428,  8428,  -821,  8428,
    8428,  8428,   189,   215,   827,   928,  -821,  7713,  6569,  8428,
    7570,  8428,  8428,   163,   226,  3992,  8428,  8571,  8571,   160,
    -821,  8428,  7713,  8428,  8428,  -821,  -821,  8428,  -821,  -821,
   10144,  8428,  8428,  -821,  8428,  -821,  -821,  3134,  6140,  7713,
    -821,  3849,  -821,  -821,   270,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,   160,  -821,   160,   176,    61,  -821,  -821,  -821,
     287,   122,  -821,  -821,  -821,   212,   250,   185,   266,   274,
   13417,  2295,   -38,   279,   291,  -821,  -821,  -821,  -821,  -821,
    -821,   299,  -821,  -821, 13417,   290,  3992,  -821,   302,  -821,
     305,  8428,  8428,  8428,  8428,  8428,  7713,  7713,   179,  -821,
    -821,  -821,  -821, 11855,   271,  -821,  -821,   160,   308,  -821,
      56, 13417,   361,  6712,  -821,  -821,  -821,  -821,   160,   103,
     160,   309,    25, 11302, 11268,  -821,  -821,  -821, 11777, 10597,
    6712,  3992,   310,    27,    92,    36,  -821,  3992,  -821,  -821,
   11455,   487,   337, 11455,   337,   487,  -821,  6712,   296,  -821,
    -821,   160,  -821,   129, 13417,  8428,  8428,  -821, 13417,   322,
   11647,  -821, 11455,   337, 13417,   316,  6712,  -821, 13417, 12102,
    -821,  -821, 12140,  1377,  -821,  -821, 12186,   365,   329,   337,
     194, 11932, 11455, 12263,   269,  1588,   487,   269,   487,   108,
    -821,  -821,  3277,    15,  -821,  8428,  -821,   -17,   -16,  -821,
      69, 12309,   -29,   461,  -821,  -821,   441,   355,   348,  -821,
    -821,  -821,    38,    29,    29,    29,  -821,  8428,  8428,  8428,
    8428,  7856,  7856,  8428,  8428,  8428,  8428,  8428,  8428,    64,
   10144,  8428,  8428,  8428,  8428,  8428,  8428,  8428,  8428,  8428,
    8428,  8428,  8428,  8428,  8428,  8428,  7284,  7284,  7284,  7284,
    7284,  7284,  7284,  7284,  7284,  7284,  7284,  7284,  7284,  7284,
    7284,  7284,  6712,  6712,  7856,  7856,  5997,  -821,  -821,  3420,
    -821, 11978, 12056, 12347,    53,  7856,    25,   360,  -821,  8428,
    -821,  8428,   400,  -821,   353,   384,  -821,  -821,   364,   160,
     463,  7713,  -821,  4135,  7856,  -821,  4278,  7856,   358,  -821,
      25,  8714,  8428,  -821,  3992,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,   483,  8428,   371,
    -821,    55,  -821,  -821,    27,  -821,   402,   376,  -821,  8857,
     425,  8428,    29,  -821,  -821,  -821,   381,  -821,  -821,  7713,
    -821, 13417, 13417,  -821,    35,  -821,  6712,   385,  -821,   511,
    -821,   511,  -821,  8857,   416,  -821,  -821,  8714,  8428,  -821,
    -821,  -821,  -821,  -821,  7427,  -821, 10469,  6283,  -821,  6426,
    -821,  7856,  5565,  2991,   387,  8428,  5853,  -821,  -821,   160,
    7713,   396,   429,   153,    29,   247,   255,   263,   265, 11731,
   13531, 13531,   301,  -821,   301,  -821,   301, 10201,   422,  1552,
     911,   337,   269,  -821,  -821,  -821,  1588,  1046,   301,  5392,
    5392, 13531,  5392,  5392,  1735,   269,  1046,  2708,  1735,   487,
     487,   269,   412,   413,   414,   417,   420,   426,   427,   431,
     432,   434,   437,   438,   439,   440,   442,   443,   419,   444,
    -821,   301,  -821,   301,    96,  -821,  -821,  -821,  -821,  -821,
    -821,   160,    93,  -821, 13481,   307,  9000,  7856,  9143,  7856,
    8428,  7856, 10231,    47, 12428,  -821,  -821, 13417, 12466,  6712,
    -821,  6712,  -821,   355,  8428,    95,  8428, 13417,    52, 11574,
    8428, 13417,    45, 11421,  5997,  -821,   446,   467,   449, 12556,
     467,   450,   550, 12594,  3992, 11608,  -821,   115,  -821,    40,
    -821,   195,   447,    27,    92,  5422,  -821,   314,  6855,  -821,
   13417,  -821,  -821,  -821, 13417,   445,    13,   451,  -821,    39,
    -821,  -821,   277,   160,   454,   459,  -821,  -821,  -821,  -821,
    -821,  -821,   -27,  1288,  -821,  -821, 13417,  3992, 13417,  -821,
   12633,   456,   491,  1732,   470,   502,    34,  -821,   543,  -821,
    -821,  -821,  -821, 10281,   283,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    5997,  -821,  7856,  7856,  8428,   590, 12752,  8428,   594, 12786,
     476,  2662,    25,    25,  -821,  -821,  -821,  -821,   486, 11455,
    -821, 10666,  4421,  -821,  4564,  -821, 10735,  4707,  -821,    25,
    4850,  -821,    25,   116,  -821,  8428,  -821,  8428,  -821,  3992,
    8428,  -821,  3992,   601,   180,  -821,    27,   519,   568,  -821,
    -821,  -821,    86,  -821,  -821,   425,   490,    76,  -821,  -821,
    -821,  4993,  7713,  -821,  -821,  -821,   160,  -821,   524,   305,
    -821,  -821,  -821,  5136,   493,  5279,   495,  -821,  8428,  3563,
     505,   582,  -821,  8428,   160,  -821,  -821,   195,  -821,  -821,
    -821,   160,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  8428,   520,   527,   506,   506,  -821,  -821,  -821,
     182,   191, 12876,  9286,  9429, 12910,  9572,  9715,  9858, 10001,
    -821,  -821,  2410,  -821,  3992,  8428, 13417,  8428, 13417,  3992,
    8428, 13417,  -821,  8428, 13417,  -821,  5997,  -821, 12949, 13417,
    -821, 13417,   620,  3992,  -821,   507,  7999,    -7,  -821,   565,
    -821,  -821,  7856,  1490,  3992,  -821,    20,   512,  8428,  -821,
    8428,  -821, 13417,  3992,  8428,  -821, 13417,  3992, 13417,   113,
    -821,  3706,  -821,   523,   654,   654,  -821,  2102,  -821, 13417,
    -821,    80,  -821, 10533,  1938,    55,  -821,  -821,  8428,  8428,
    8428,  8428,  8428,  8428,  8428,  8428,   515,  2473, 12594, 10804,
   10873, 12594, 10942, 11011,  -821,  8428,  3992,  -821,   519,   155,
    8428,  8428,  8428,  8428,  8428,  5709,  -821,  -821,   244,  7713,
    -821,  -821,  8428,     6, 10330,  -821,   466,   361,  -821,   305,
   13417, 11080,  -821, 11149,  -821,  -821,  -821,  -821,   580,  -821,
    -821,   506,   506,   203, 13033, 13097, 13161, 13225, 13289, 13353,
    -821,  -821,  3992,  3992,  3992,  3992, 13417,  -821,    -7,  8142,
      87,  -821,  -821, 13417, 13417,  -821,  -821,  -821, 10001,  -821,
    3992,  3992,  -821,   565,  -821,  -821,  -821,  8428,  8428,  8428,
    8428,  8428,  8428, 12594, 12594, 12594, 12594,  -821,  -821,  -821,
    -821,  -821,   252,  7856,  2625,   746, 12594, 12594,    84, 10405,
    -821,  -821,  -821,  -821,  -821,  8285,  -821
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -821,  -821,  -821,    -1,  -648,  1945,  -821,  -821,  -821,  1727,
     -48,  -258,   254,   264,  -821,  -821,   392,   318,  2143,     7,
     576,  -806,  -659,   -42,  -821,  -821,  -821,   -14,  -821,  -821,
    -821,   448,  -821,   -21,  -821,  -821,  -821,  -821,  -821,   509,
     157,   -86,  -821,  -821,  -821,   -33,   658,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -142,  -592,  -717,  -821,  -141,
       9,  -821,  -367,  -821,  -821,   -30,  -821,  -821,  -250,    48,
    -821,  -207,  -221,  -821,  -147,  -821,   668,  -821,  -180,   317,
    -821,  -361,  -669,  -526,  -398,  -820,  -803,  -150,   -39,  -821,
    -222,  -821,  -100,  -821,   183,   354,  -359,  -821,  -821,   943,
    -821,     0,  -821,  -821,  -228,  -821,  -527,  -821,  1039,  1318,
      -2,   175,   383,  -821,  1437,  1570,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -310
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -453
static const yytype_int16 yytable[] =
{
     120,    80,   199,   200,   550,   571,   505,   664,   203,   134,
     327,   746,   172,   552,   153,   140,   790,   120,   161,   120,
     557,   338,   424,   230,   173,   174,   178,   179,   685,   827,
       5,   166,     5,   190,   191,   192,   565,   193,   194,   195,
     250,     5,     5,     5,   572,   204,   187,   210,   212,   213,
     214,   187,   818,   218,   219,   222,   226,   906,  -241,   229,
     231,   232,   233,   817,   348,   234,   691,     5,   235,   236,
     237,   657,   238,   381,    48,   204,   494,   251,   652,   120,
     369,   378,   435,   436,   437,   438,   426,   378,  -152,   558,
     511,   453,   495,   378,   540,   496,   931,   386,   417,   419,
       3,   692,   259,   749,   434,   312,   421,   313,   324,   497,
     373,   498,   859,   792,   940,   132,   397,   427,   861,   703,
      14,   499,   260,  -152,   933,   946,   246,   418,   420,  -309,
     414,   941,   906,   627,   120,   389,   348,   500,   135,   134,
     321,   322,   323,   218,   204,   251,   454,   319,  -309,   714,
     540,   382,    48,   326,  -309,   693,   789,   415,   877,  -309,
     348,   319,   628,     5,   914,   915,   319,    46,   422,  -309,
     374,   329,   183,   157,   598,  -309,   715,  -152,   818,   120,
     319,   183,   696,   676,   422,   120,   319,    60,  -309,   817,
     330,   422,   684,   319,   379,   579,   196,   581,  -241,   512,
     379,   494,   422,   391,   392,   387,   379,  -309,   630,   599,
     389,   333,   488,   489,  -309,   423,   678,   495,   942,   600,
     496,  -309,   793,   209,   505,   319,  -309,  -309,   333,   928,
     674,   776,   882,   934,   497,   601,   498,   631,   215,   650,
     120,   413,   132,   416,   389,   333,   499,   132,     5,   844,
     411,   679,   164,   249,   604,   875,  -390,   443,   445,   675,
     777,   680,   500,   390,   333,   439,   440,   441,   442,   444,
     446,   447,   448,   449,   450,   451,   452,   681,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   270,   878,   567,  -406,   163,     5,
     490,   492,   525,   164,   659,   662,  -407,   550,   262,   389,
    -393,   513,   491,   493,   504,   785,   180,   120,   674,  -434,
     505,  -434,  -434,   514,  -434,   267,  -406,   517,   406,   518,
     528,   138,   255,   532,   271,  -407,   158,   -80,   272,   204,
    -326,   527,   529,   263,   531,   533,  -389,   916,  -318,   539,
     539,  -310,   543,   264,   704,   706,   111,   -80,   197,  -326,
     333,   333,   382,  -326,   314,   111,   545,  -318,   315,   216,
     382,  -318,   633,   111,   265,   111,   315,   120,   382,   560,
     382,   605,  -310,   277,   266,   121,   279,   564,  -326,   606,
     282,   592,  -286,  -310,   121,   865,  -318,   607,   382,   608,
    -394,   120,   121,  -310,   121,   539,   539,   582,  -395,   111,
    -310,  -286,   564,  -392,  -433,   277,  -433,   748,   279,   583,
     564,   586,   282,   588,   590,  -391,   700,   858,   204,  -310,
     594,   111,     5,   316,   866,   111,   318,   563,   121,     5,
     223,   223,   328,   164,   333,   167,   319,   270,  -310,   221,
     225,   331,   341,   371,   279,  -310,   393,  -295,   550,   396,
     121,   403,   563,   405,   121,   246,   505,   246,   428,   430,
     563,   472,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   271,   429,   432,
     111,   272,  -295,   635,   516,   638,   519,   520,   522,   521,
     524,   534,  -295,  -313,   527,   636,   531,   639,   543,   641,
     586,   544,   270,   642,   643,   546,   687,   553,   168,   121,
     554,   558,   649,  -295,   651,   562,     5,   569,   656,   568,
     164,   573,   504,   587,  -313,   111,   277,   593,   278,   279,
     932,   111,   120,   282,  -295,  -313,   609,   610,   611,   935,
     289,   612,   271,   251,   613,  -313,   272,   293,   294,   295,
     614,   615,  -313,   625,   121,   616,   617,   645,   618,   333,
     121,   619,   620,   621,   622,   932,   623,   624,   669,  -295,
     -58,  -313,   665,   666,   668,   120,   932,   690,   701,  -295,
     626,   683,   249,   702,   249,   695,   111,   709,   713,    14,
    -313,   277,    17,   278,   279,   168,   389,  -313,   282,   716,
    -295,    23,   712,    24,   813,   289,   753,    28,   443,   490,
     756,    31,   758,  -180,   295,   121,   211,   762,   504,   783,
     444,   491,   752,   786,   788,   755,   791,  -354,   798,   803,
      42,   807,   814,  -180,   824,   815,    46,   812,   846,   825,
     766,   848,   768,   796,   862,   771,   869,   890,   774,   912,
     105,   811,   575,   778,    59,   779,    60,   120,   781,   105,
     120,   574,   401,   111,   175,   797,   837,   105,   339,   105,
     648,   820,   881,   784,   883,    24,   198,   821,   944,   120,
     204,   927,   938,    31,   646,  -180,   177,   111,   898,   561,
     111,   802,   121,   806,   647,     0,   808,   120,   111,     0,
       0,   819,     0,   105,     0,  -180,    75,     0,     0,     0,
       0,     0,     0,     0,   816,     0,   121,     0,     0,   121,
       0,     0,     0,   689,     0,   105,     0,   121,     0,   105,
     823,     0,     0,     0,     0,     0,     0,     0,   342,   345,
     349,   766,   768,     0,   771,   774,   802,   806,     0,     0,
       0,     0,   838,   839,     0,   840,   384,   841,   842,   385,
       0,   843,   199,   200,   504,     0,     0,   111,     0,     0,
       0,   120,     0,  -315,   120,     0,     0,     0,   395,     0,
     864,   586,   120,     0,   105,     0,   870,     0,   871,     0,
       0,   120,   873,     0,     0,   120,   121,     0,   409,   120,
     413,     0,     0,     0,  -315,     0,   900,     0,     0,     0,
       0,     0,     0,     0,     0,  -315,   838,   884,   885,   841,
     886,   887,   888,   889,   856,  -315,     0,     0,     0,   105,
       0,     0,  -315,   896,   120,   105,     0,   175,   194,   213,
     214,   218,   229,   251,     0,   629,     0,   903,    24,   198,
     904,  -315,     0,     0,     0,     0,    31,     0,  -180,     0,
       0,     0,     0,     0,   -44,     0,     0,     0,     0,     0,
    -315,     0,     0,     0,     0,     0,     0,  -315,  -180,   -44,
     923,   924,   925,   926,     0,     0,     0,   120,   111,     0,
     105,     0,   515,     0,     0,     0,   806,   901,   936,   937,
       0,     0,     0,     0,     0,   923,   924,   925,   926,   936,
     937,     0,     0,     0,     0,     0,   535,   121,     0,     0,
       0,   939,   586,     0,     0,     0,   270,     0,     0,     0,
       0,   111,     0,   806,     0,   115,     0,   929,   175,     0,
       0,     0,     0,     0,   115,     0,     0,   747,     0,    24,
     198,     0,   115,     0,   115,     0,     0,    31,     0,  -180,
     121,     0,     0,     0,     0,   -43,   271,   105,     0,     0,
     272,     0,   929,     0,     0,     0,   750,   751,     0,  -180,
     -43,     0,     0,   929,     0,     0,     0,     0,   115,     0,
       0,   105,     0,     0,   105,     0,   111,     0,   111,     0,
       0,   111,   105,     0,   111,     0,     0,     0,     0,     0,
     115,   274,     0,   111,   115,   277,   111,   278,   279,     0,
       0,     0,   282,     0,     0,   121,     0,   121,     0,   289,
     121,   118,     0,   121,     0,   111,   293,   294,   295,     0,
     118,     0,   121,     0,     0,   121,     0,   111,   118,   111,
     118,     0,     0,   111,     0,     0,     0,     0,     0,     0,
       0,   270,     0,     0,   121,     0,     0,     0,     0,   115,
       0,   105,     0,     0,     0,     0,   121,     0,   121,     0,
       0,     0,   121,     0,   118,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   653,   655,     0,     0,   658,   661,
       0,   271,     0,     0,     0,   272,   118,     0,   111,     0,
     118,     0,     0,   111,   115,     0,     0,     0,     0,     0,
     115,     0,     0,     0,     0,     0,     0,   111,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   121,   111,     0,
       0,     0,   121,     0,     0,     0,   274,   111,   275,   276,
     277,   111,   278,   279,   280,   111,   121,   282,   283,   284,
       0,   286,   287,   288,   289,   118,     0,   121,   292,     0,
       0,   293,   294,   295,     0,   115,   121,     0,     0,     0,
     121,     0,     0,     0,   121,     0,     0,     0,     0,     0,
     111,     0,   105,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   760,   761,
     118,     0,     0,     0,     0,   763,   118,     0,     0,   121,
       0,     0,     0,     0,     0,   772,     0,     0,   775,     0,
       0,     0,     0,     0,     0,   105,   111,   111,   111,   111,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   115,     0,   111,   111,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   121,   121,   121,   121,     0,
       0,   118,     0,     0,     0,     0,   115,     0,     0,   115,
       0,     0,     0,   121,   121,     0,     0,   115,   268,     0,
       0,     0,     0,   269,     0,     0,     0,     0,     0,     0,
     105,     0,   105,   270,     0,   105,     0,     0,   105,     0,
     119,     0,     0,     0,     0,     0,     0,   105,     0,   119,
     105,     0,     0,     0,     0,     0,     0,   119,     0,   119,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   105,
       0,     0,     0,   271,     0,     0,     0,   272,   118,     0,
       0,   105,     0,   105,     0,     0,   115,   105,     0,     0,
       0,     0,   868,   119,     0,     0,     0,     0,     0,   348,
       0,     0,   118,   273,     0,   118,     0,     0,     0,     0,
       0,     0,     0,   118,     0,   119,     0,     0,   274,   119,
     275,   276,   277,     0,   278,   279,   280,     0,   281,   282,
     283,   284,   285,   286,   287,   288,   289,     0,   290,   291,
     292,     0,   105,   293,   294,   295,     0,   105,     0,     0,
       0,     0,     0,     0,   705,     0,     0,     0,     0,   123,
       0,   105,     0,     0,     0,     0,     0,     0,   123,     0,
       0,     0,   105,     0,   119,     0,   123,     0,   123,     0,
       0,   105,   118,     0,     0,   105,     0,     0,     0,   105,
       0,     0,     0,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,   308,   309,   115,     0,     0,
       0,     0,   123,     5,     6,     7,     8,     9,    10,   119,
       0,     0,   141,     0,   105,   119,     0,     0,     0,     0,
       0,     0,     0,     0,   123,   310,   311,    22,   123,     0,
    -433,     0,  -433,   142,   143,     0,   144,     0,    30,     0,
     115,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,    40,     0,    41,     0,    43,
     105,   105,   105,   105,     0,     0,     0,     0,     0,     0,
     119,     0,    51,    52,    53,    54,   145,     0,   105,   105,
       0,     0,   124,   123,     0,    61,     0,   270,     0,     0,
       0,   124,     0,   118,     0,     0,     0,     0,     0,   124,
       0,   124,     0,     0,     0,   115,     0,   115,     0,     0,
     115,    67,     0,   115,     0,     0,     0,     0,     0,     0,
       0,     0,   115,   270,     0,   115,     0,   271,   123,    72,
       0,   272,     0,     0,   123,   124,   118,     0,     0,     0,
       0,   146,     0,    78,   115,   555,     0,   119,     0,     0,
       0,     0,     0,     0,     0,     0,   115,   124,   115,     0,
       0,   124,   115,   271,     0,     0,     0,   272,     0,     0,
       0,   119,   274,     0,   119,   276,   277,     0,   278,   279,
       0,     0,   119,   282,     0,     0,     0,     0,     0,   123,
     289,     0,     0,     0,     0,     0,     0,   293,   294,   295,
       0,   118,     0,   118,     0,     0,   118,     0,   274,   118,
     275,   276,   277,     0,   278,   279,   124,   115,   118,   282,
       0,   118,   115,     0,     0,   288,   289,     0,     0,     0,
     292,     0,     0,   293,   294,   295,   115,     0,     0,     0,
     118,     0,     0,   710,     0,     0,     0,   115,     0,     0,
       0,   119,   118,     0,   118,     0,   115,     0,   118,  -166,
     115,   124,  -166,  -166,   115,     0,   123,   124,     0,     0,
     270,  -166,   188,  -166,  -166,     0,     0,  -166,     0,     0,
       0,  -166,     0,  -166,     0,     0,     0,   188,     0,     0,
     123,     0,     0,   123,   220,   224,     0,     0,  -166,   115,
    -166,   123,  -166,  -166,     0,  -166,  -166,     0,  -166,     0,
     271,     0,     0,   118,   272,     0,     0,     0,   118,     0,
       0,     0,   124,     0,  -166,     0,  -166,     0,     0,  -166,
       0,     0,   118,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   118,     0,   115,   115,   115,   115,     0,
       0,     0,   118,     0,     0,   274,   118,   275,   276,   277,
     118,   278,   279,   115,   115,     0,   282,     0,     0,     0,
     123,     0,   119,   289,     0,     0,  -166,   320,     0,     0,
     293,   294,   295,     0,  -166,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   118,     0,     0,     0,   124,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   188,
     188,   188,     0,     0,     0,   119,   367,     0,     0,     0,
       0,     0,     0,   124,     0,     0,   124,   188,     0,     0,
     188,     0,     0,     0,   124,     0,     0,     0,     0,     0,
       0,   118,   118,   118,   118,     0,     0,     0,     0,   188,
       0,     5,     0,     0,     0,     0,     0,    82,   718,   118,
     118,     0,     0,   719,     0,     0,   133,     0,     0,   188,
       0,     0,     0,     0,   159,     0,   162,     0,     0,     0,
     119,     0,   119,     0,     0,   119,     0,     0,   119,     0,
       0,   123,     0,     0,     0,     0,     0,   119,     0,     0,
     119,     0,     0,   124,     0,     0,     0,     0,     0,     0,
     217,     0,     0,     0,     0,     0,     0,     0,     0,   119,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   119,    82,   119,   123,     0,   254,   119,     0,     0,
       0,     0,     0,     0,   355,   356,   357,   358,   359,   360,
       0,     0,   361,   362,   363,   364,   365,   366,   720,   721,
     722,   723,     0,   188,   724,     0,     0,     0,   725,   726,
     727,   728,   729,   730,   731,   732,   733,   734,   735,     0,
     736,     0,     0,   737,   738,   739,   740,   188,   741,     0,
       0,   317,   119,     0,     0,     0,     0,   119,     0,   123,
       0,   123,     0,     0,   123,     0,     0,   123,     0,     0,
       0,   119,     0,     0,     0,     0,   123,     0,     0,   123,
       0,     0,   119,     0,   124,     0,     0,     0,     0,    14,
       0,   119,    17,     0,     0,   119,   370,     0,   123,   119,
       0,    23,   383,    24,   813,     0,     0,    28,     0,     0,
     123,    31,   123,  -180,     0,     0,   123,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   139,   124,     0,     0,
     253,   139,   814,  -180,   119,   815,    46,   139,   139,     0,
       0,     0,     0,     0,     0,   181,     0,   184,     0,     0,
       0,     0,     0,     0,    59,     0,    60,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   123,   227,     0,     0,     0,   123,     0,     0,     0,
     119,   119,   119,   119,     0,     0,     0,     0,     0,     0,
     123,     0,   124,     0,   124,     0,     0,   124,   119,   119,
     124,   123,     0,     0,     0,     0,    75,     0,     0,   124,
     123,     0,   124,     0,   123,   257,     0,   258,   123,     0,
       0,     0,     0,     0,     0,   188,   188,     0,     0,   188,
     188,   124,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,   673,   124,     0,   124,     0,     0,     0,   124,
       0,     0,     0,   123,     0,     0,     0,     0,   383,     0,
       0,   383,     0,     0,     0,     0,   697,     0,     0,     0,
     227,     0,     0,     0,     0,     0,   332,     0,     0,     0,
       0,   227,     0,   340,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   332,     0,     0,   375,     0,     0,   123,
     123,   123,   123,     0,   124,     0,     0,     0,     0,   124,
     332,   139,     0,     0,   388,     0,     0,   123,   123,     0,
       0,     0,     0,   124,     0,     0,     0,     0,     0,   332,
       0,     0,     0,     0,   124,     0,     0,     0,   585,   188,
     188,     0,     0,   124,     0,     0,   188,   124,   367,     0,
       0,   124,     0,   367,     0,     0,   188,     0,     0,   188,
       0,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   184,   184,   184,   184,     0,
       0,   836,     0,     0,     0,     0,   124,     0,     0,     0,
       0,     0,   455,     0,     0,     0,   799,  -166,     0,     0,
    -166,  -166,     0,   310,   311,  -408,     0,     0,  -433,  -166,
    -433,  -166,  -166,     0,     0,  -166,     0,     0,     0,  -166,
       0,  -166,     0,     0,     0,   332,   332,     0,     0,     0,
       0,     0,   124,   124,   124,   124,  -166,     0,  -166,     0,
    -166,  -166,     0,  -166,  -166,     0,  -166,     0,     0,     0,
     124,   124,   523,     0,     0,     0,     0,     0,     0,   671,
      14,     0,  -166,    17,  -166,     0,     0,  -166,     0,     0,
       0,     0,    23,     0,    24,   813,     0,     0,    28,     0,
       0,     0,    31,     0,  -180,     0,     0,   375,     0,     0,
       0,     0,     0,   188,     0,   184,     0,     0,     0,     0,
       0,    42,   707,   814,  -180,     0,   815,    46,     0,   332,
       0,     0,     0,     0,  -166,     0,     0,     0,     0,     0,
       0,     0,  -166,     0,     0,    59,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,   367,   367,     0,   367,
     367,     0,   591,     0,     0,   595,     0,   184,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     905,     0,     0,     0,     0,     0,   909,   383,   367,   383,
     367,     0,   383,     0,     0,   383,     0,    75,     0,     0,
       0,     0,     0,     0,   780,   891,     0,   782,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,    10,     0,     0,   795,   141,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   801,     0,
     805,     0,    22,     0,    82,     0,     0,     0,   142,   143,
       0,   144,     0,    30,   332,   905,     0,     0,    33,    34,
       0,    36,   268,    37,    38,     0,     0,   269,     0,     0,
      40,     0,    41,     0,    43,     0,     0,   270,     0,     0,
       0,     0,   677,     0,     0,     0,   375,    51,    52,    53,
      54,   145,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,   698,     0,     0,     0,   227,     0,     0,     0,
       0,  -317,     0,     0,     0,     0,     0,   271,   847,     0,
       0,   272,     0,   270,     0,     0,    67,     0,     0,   867,
    -317,     0,     0,     0,  -317,     0,   743,     0,   872,     0,
       0,     0,   874,     0,    72,     0,    82,   273,   201,     0,
       0,     0,     0,     0,     0,     0,   146,     0,    78,  -317,
     855,     0,   274,   271,   275,   276,   277,   272,   278,   279,
     280,     0,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   897,   290,   291,   292,     0,     0,   293,   294,   295,
       0,     0,     0,   273,     0,     0,     0,     0,   759,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   274,   375,
     275,   276,   277,     0,   278,   279,   280,     0,   281,   282,
     283,   284,     0,   286,   287,   288,   289,     0,   290,   698,
     292,     0,     0,   293,   294,   295,     0,     0,    -2,     4,
       0,     5,     6,     7,     8,     9,    10,   595,     0,     0,
      11,    12,    13,     0,   822,    14,    15,    16,    17,  -293,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,    29,     0,    30,    31,     0,  -180,
      32,    33,    34,    35,    36,   -42,    37,    38,     0,    39,
       0,     0,     0,    40,  -293,    41,    42,    43,    44,  -180,
     -42,    45,    46,     0,  -293,    47,    48,     0,    49,    50,
      51,    52,    53,    54,    55,     0,    56,     0,    57,    58,
      59,     0,    60,    61,    62,  -293,     0,     0,    63,     0,
      64,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   139,     0,    66,    67,
      68,    69,     0,     0,     0,     0,    70,   880,     0,     0,
       0,     0,     0,     0,     0,    71,     0,    72,     0,    73,
      74,     0,    75,     0,     0,    76,     0,     0,     0,    77,
       0,    78,     4,    79,     5,     6,     7,     8,     9,    10,
       0,  -452,     0,    11,    12,    13,  -452,     0,    14,    15,
      16,    17,  -293,    18,    19,    20,  -452,    21,    22,  -452,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -180,    32,    33,    34,    35,    36,   -42,    37,
      38,     0,    39,     0,     0,     0,    40,  -293,    41,     0,
      43,    44,  -180,   -42,    45,    46,  -452,  -293,    47,    48,
    -452,    49,    50,    51,    52,    53,    54,    55,     0,    56,
       0,    57,    58,    59,     0,    60,    61,    62,  -293,     0,
       0,    63,     0,    64,     0,     0,  -452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -452,    67,  -452,  -452,  -452,  -452,  -452,  -452,  -452,
       0,  -452,  -452,  -452,  -452,  -452,  -452,  -452,  -452,  -452,
      72,  -452,  -452,  -452,     0,    75,  -452,  -452,  -452,     0,
       0,     0,    77,  -452,    78,   239,    79,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
       0,    14,    15,    16,    17,  -293,    18,    19,    20,     0,
      21,    22,     0,    23,     0,    24,    25,    26,    27,    28,
      29,     0,    30,    31,     0,  -180,    32,    33,    34,    35,
      36,   -42,    37,    38,     0,    39,     0,     0,     0,    40,
    -293,    41,    42,    43,    44,  -180,   -42,    45,    46,     0,
    -293,    47,    48,     0,    49,    50,    51,    52,    53,    54,
      55,     0,    56,     0,    57,    58,    59,     0,    60,    61,
      62,  -293,     0,     0,    63,     0,    64,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,     0,     0,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,    71,     0,    72,     0,    73,    74,   201,    75,     0,
       0,    76,     0,     0,     0,    77,   240,    78,     4,    79,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,     0,    14,    15,    16,    17,  -293,    18,
      19,    20,     0,    21,    22,     0,    23,     0,    24,    25,
      26,    27,    28,    29,     0,    30,    31,     0,  -180,    32,
      33,    34,    35,    36,   -42,    37,    38,     0,    39,     0,
       0,     0,    40,  -293,    41,    42,    43,    44,  -180,   -42,
      45,    46,     0,  -293,    47,    48,     0,    49,    50,    51,
      52,    53,    54,    55,     0,    56,     0,    57,    58,    59,
       0,    60,    61,    62,  -293,     0,     0,    63,     0,    64,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,     0,     0,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,    71,     0,    72,     0,    73,    74,
       0,    75,     0,     0,    76,     0,     0,     0,    77,   412,
      78,   239,    79,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -293,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -180,    32,    33,    34,    35,    36,   -42,    37,    38,
       0,    39,     0,     0,     0,    40,  -293,    41,    42,    43,
      44,  -180,   -42,    45,    46,     0,  -293,    47,    48,     0,
      49,    50,    51,    52,    53,    54,    55,     0,    56,     0,
      57,    58,    59,     0,    60,    61,    62,  -293,     0,     0,
      63,     0,    64,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,     0,     0,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,    71,     0,    72,
       0,    73,    74,     0,    75,     0,     0,    76,     0,     0,
       0,    77,   240,    78,   809,    79,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,     0,
      14,    15,    16,    17,  -293,    18,    19,    20,     0,    21,
      22,     0,    23,     0,    24,    25,    26,    27,    28,    29,
       0,    30,    31,     0,  -180,    32,    33,    34,    35,    36,
     -42,    37,    38,     0,    39,     0,     0,     0,    40,  -293,
      41,    42,    43,    44,  -180,   -42,    45,    46,     0,  -293,
      47,    48,     0,    49,    50,    51,    52,    53,    54,    55,
       0,    56,     0,    57,    58,    59,     0,    60,    61,    62,
    -293,     0,     0,    63,     0,    64,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,     0,     0,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
      71,     0,    72,     0,    73,    74,     0,    75,     0,     0,
      76,     0,     0,     0,    77,   810,    78,     4,    79,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,     0,    14,    15,    16,    17,  -293,    18,    19,
      20,     0,    21,    22,     0,    23,     0,    24,    25,    26,
      27,    28,    29,     0,    30,    31,     0,  -180,    32,    33,
      34,    35,    36,   -42,    37,    38,     0,    39,     0,     0,
       0,    40,  -293,    41,    42,    43,    44,  -180,   -42,    45,
      46,     0,  -293,    47,    48,     0,    49,    50,    51,    52,
      53,    54,    55,     0,    56,     0,    57,    58,    59,     0,
      60,    61,    62,  -293,     0,     0,    63,     0,    64,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
       0,     0,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,    71,     0,    72,     0,    73,    74,     0,
      75,     0,     0,    76,     0,     0,     0,    77,   876,    78,
       4,    79,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -293,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
    -180,    32,    33,    34,    35,    36,   -42,    37,    38,     0,
      39,     0,     0,     0,    40,  -293,    41,   253,    43,    44,
    -180,   -42,    45,    46,     0,  -293,    47,    48,     0,    49,
      50,    51,    52,    53,    54,    55,     0,    56,     0,    57,
      58,    59,     0,    60,    61,    62,  -293,     0,     0,    63,
       0,    64,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,     0,     0,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,    71,     0,    72,     0,
      73,    74,     0,    75,     0,     0,    76,     0,     0,     0,
      77,     0,    78,     4,    79,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,     0,    14,
      15,    16,    17,  -293,    18,    19,    20,     0,    21,    22,
       0,    23,     0,    24,    25,    26,    27,    28,    29,     0,
      30,    31,     0,  -180,    32,    33,    34,    35,    36,   -42,
      37,    38,     0,    39,     0,     0,     0,    40,  -293,    41,
       0,    43,    44,  -180,   -42,    45,    46,     0,  -293,    47,
      48,     0,    49,    50,    51,    52,    53,    54,    55,     0,
      56,     0,    57,    58,    59,     0,    60,    61,    62,  -293,
       0,     0,    63,     0,    64,     0,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,     0,     0,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,    71,
       0,    72,     0,    73,    74,     0,    75,     0,     0,    76,
       0,     0,     0,    77,     0,    78,     4,    79,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   526,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
      40,  -293,    41,     0,    43,    44,  -180,   -42,    45,    46,
       0,  -293,    47,    48,     0,    49,    50,    51,    52,    53,
      54,    55,     0,    56,     0,    57,    58,    59,     0,    60,
      61,    62,  -293,     0,     0,    63,     0,    64,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,    69,     0,
       0,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,    71,     0,    72,     0,    73,    74,     0,    75,
       0,     0,    76,     0,     0,     0,    77,     0,    78,     4,
      79,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -293,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,   530,     0,    30,    31,     0,  -180,
      32,    33,    34,    35,    36,   -42,    37,    38,     0,    39,
       0,     0,     0,    40,  -293,    41,     0,    43,    44,  -180,
     -42,    45,    46,     0,  -293,    47,    48,     0,    49,    50,
      51,    52,    53,    54,    55,     0,    56,     0,    57,    58,
      59,     0,    60,    61,    62,  -293,     0,     0,    63,     0,
      64,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,     0,     0,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,    71,     0,    72,     0,    73,
      74,     0,    75,     0,     0,    76,     0,     0,     0,    77,
       0,    78,     4,    79,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -293,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,   765,     0,    30,
      31,     0,  -180,    32,    33,    34,    35,    36,   -42,    37,
      38,     0,    39,     0,     0,     0,    40,  -293,    41,     0,
      43,    44,  -180,   -42,    45,    46,     0,  -293,    47,    48,
       0,    49,    50,    51,    52,    53,    54,    55,     0,    56,
       0,    57,    58,    59,     0,    60,    61,    62,  -293,     0,
       0,    63,     0,    64,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,     0,     0,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,    71,     0,
      72,     0,    73,    74,     0,    75,     0,     0,    76,     0,
       0,     0,    77,     0,    78,     4,    79,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
       0,    14,    15,    16,    17,  -293,    18,    19,    20,     0,
      21,    22,     0,    23,     0,    24,    25,    26,    27,    28,
     767,     0,    30,    31,     0,  -180,    32,    33,    34,    35,
      36,   -42,    37,    38,     0,    39,     0,     0,     0,    40,
    -293,    41,     0,    43,    44,  -180,   -42,    45,    46,     0,
    -293,    47,    48,     0,    49,    50,    51,    52,    53,    54,
      55,     0,    56,     0,    57,    58,    59,     0,    60,    61,
      62,  -293,     0,     0,    63,     0,    64,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,     0,     0,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,    71,     0,    72,     0,    73,    74,     0,    75,     0,
       0,    76,     0,     0,     0,    77,     0,    78,     4,    79,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,     0,    14,    15,    16,    17,  -293,    18,
      19,    20,     0,    21,    22,     0,    23,     0,    24,    25,
      26,    27,    28,   770,     0,    30,    31,     0,  -180,    32,
      33,    34,    35,    36,   -42,    37,    38,     0,    39,     0,
       0,     0,    40,  -293,    41,     0,    43,    44,  -180,   -42,
      45,    46,     0,  -293,    47,    48,     0,    49,    50,    51,
      52,    53,    54,    55,     0,    56,     0,    57,    58,    59,
       0,    60,    61,    62,  -293,     0,     0,    63,     0,    64,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,     0,     0,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,    71,     0,    72,     0,    73,    74,
       0,    75,     0,     0,    76,     0,     0,     0,    77,     0,
      78,     4,    79,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -293,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,   773,     0,    30,    31,
       0,  -180,    32,    33,    34,    35,    36,   -42,    37,    38,
       0,    39,     0,     0,     0,    40,  -293,    41,     0,    43,
      44,  -180,   -42,    45,    46,     0,  -293,    47,    48,     0,
      49,    50,    51,    52,    53,    54,    55,     0,    56,     0,
      57,    58,    59,     0,    60,    61,    62,  -293,     0,     0,
      63,     0,    64,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,     0,     0,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,    71,     0,    72,
       0,    73,    74,     0,    75,     0,     0,    76,     0,     0,
       0,    77,     0,    78,     4,    79,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,     0,
      14,    15,    16,    17,  -293,    18,    19,    20,     0,   794,
      22,     0,    23,     0,    24,    25,    26,    27,    28,    29,
       0,    30,    31,     0,  -180,    32,    33,    34,    35,    36,
     -42,    37,    38,     0,    39,     0,     0,     0,    40,  -293,
      41,     0,    43,    44,  -180,   -42,    45,    46,     0,  -293,
      47,    48,     0,    49,    50,    51,    52,    53,    54,    55,
       0,    56,     0,    57,    58,    59,     0,    60,    61,    62,
    -293,     0,     0,    63,     0,    64,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,     0,     0,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
      71,     0,    72,     0,    73,    74,     0,    75,     0,     0,
      76,     0,     0,     0,    77,     0,    78,     4,    79,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,     0,    14,    15,    16,    17,  -293,    18,    19,
      20,     0,    21,    22,     0,    23,     0,    24,    25,    26,
      27,    28,   800,     0,    30,    31,     0,  -180,    32,    33,
      34,    35,    36,   -42,    37,    38,     0,    39,     0,     0,
       0,    40,  -293,    41,     0,    43,    44,  -180,   -42,    45,
      46,     0,  -293,    47,    48,     0,    49,    50,    51,    52,
      53,    54,    55,     0,    56,     0,    57,    58,    59,     0,
      60,    61,    62,  -293,     0,     0,    63,     0,    64,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
       0,     0,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,    71,     0,    72,     0,    73,    74,     0,
      75,     0,     0,    76,     0,     0,     0,    77,     0,    78,
       4,    79,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -293,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,   804,     0,    30,    31,     0,
    -180,    32,    33,    34,    35,    36,   -42,    37,    38,     0,
      39,     0,     0,     0,    40,  -293,    41,     0,    43,    44,
    -180,   -42,    45,    46,     0,  -293,    47,    48,     0,    49,
      50,    51,    52,    53,    54,    55,     0,    56,     0,    57,
      58,    59,     0,    60,    61,    62,  -293,     0,     0,    63,
       0,    64,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,     0,     0,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,    71,     0,    72,     0,
      73,    74,     0,    75,     0,     0,    76,   270,     0,     0,
      77,     0,    78,   686,    79,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   141,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,     0,     0,     0,   142,   143,   271,   144,     0,
      30,   272,     0,     0,     0,    33,    34,     0,    36,     0,
      37,    38,     0,     0,     0,     0,     0,    40,     0,    41,
       0,    43,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,    52,    53,    54,   145,     0,
       0,     0,   274,     0,   275,   276,   277,    61,   278,   279,
     280,     0,     0,   282,     0,     0,     0,    65,     0,   288,
     289,     0,     0,     0,   292,     0,     0,   293,   294,   295,
       0,     0,    66,    67,    68,    69,     0,     0,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,    71,
       0,    72,     0,    73,    74,   201,     0,     0,     0,    76,
       0,     0,     0,   146,     0,    78,     0,   147,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   141,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,     0,   142,   143,
       0,   144,     0,    30,     0,     0,     0,     0,    33,    34,
       0,    36,     0,    37,    38,     0,     0,     0,     0,     0,
      40,     0,    41,     0,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,    52,    53,
      54,   145,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,    69,     0,
       0,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,    71,     0,    72,     0,    73,    74,   201,     0,
       0,     0,    76,     0,     0,     0,   146,     0,    78,     0,
     147,   584,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   141,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,   142,   143,     0,   144,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,    40,     0,    41,     0,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,    52,    53,    54,   145,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,     0,     0,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,    71,     0,    72,     0,
      73,    74,   201,     0,     0,     0,    76,     0,     0,     0,
     146,     0,    78,     0,   147,   899,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   142,   143,     0,   144,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,    40,     0,
      41,     0,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,    52,    53,    54,   145,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,     0,     0,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
      71,     0,    72,     0,    73,    74,     0,     0,     0,     0,
      76,     0,     0,     0,   146,     0,    78,     0,   147,   589,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   141,
       0,     0,     0,     0,     0,     0,     0,     0,   494,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,     0,
     142,   143,     0,   144,   495,    30,     0,   496,     0,     0,
      33,    34,     0,    36,     0,    37,    38,     0,     0,     0,
       0,   497,    40,   498,    41,     0,    43,     0,     0,     0,
       0,     0,     0,   499,     0,     0,     0,     0,     0,    51,
      52,    53,    54,   145,     0,     0,     0,     0,     0,   500,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,     0,     0,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,    71,     0,    72,     0,    73,    74,
       0,     0,     0,     0,    76,     0,     0,     0,   146,     0,
      78,     0,   147,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,   142,   143,     0,   144,     0,    30,     0,
       0,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,    40,     0,    41,     0,    43,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,    52,    53,    54,   145,     0,     0,     0,
     205,   206,     0,   244,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,     0,     0,     0,     0,    70,   245,
       0,     0,     0,     0,     0,     0,     0,    71,     0,    72,
       0,    73,    74,   201,     0,     0,     0,    76,     0,     0,
       0,   146,     0,    78,     0,   147,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   142,   143,     0,   144,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,    40,     0,
      41,     0,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,    52,    53,    54,   145,
       0,     0,     0,   205,   206,     0,   244,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,     0,     0,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
      71,     0,    72,     0,    73,    74,   201,     0,     0,     0,
      76,     0,     0,     0,   146,     0,    78,   578,   147,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   141,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   142,
     143,     0,   144,     0,    30,     0,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,    40,     0,    41,     0,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,    52,
      53,    54,   145,     0,     0,     0,   205,   206,     0,   244,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
       0,     0,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,    71,     0,    72,     0,    73,    74,   201,
       0,     0,     0,    76,     0,     0,     0,   146,     0,    78,
     580,   147,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   141,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,   142,   143,     0,   144,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,    40,     0,    41,     0,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,    52,    53,    54,   145,     0,     0,     0,   205,
     206,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,     0,     0,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,    71,     0,    72,     0,
      73,    74,     0,   207,     0,     0,    76,     0,     0,     0,
     146,     0,    78,     0,   147,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   141,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,     0,     0,     0,   142,   143,     0,   144,     0,
      30,     0,     0,     0,     0,    33,    34,     0,    36,     0,
      37,    38,     0,     0,     0,     0,     0,    40,     0,    41,
       0,    43,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,    52,    53,    54,   145,     0,
       0,     0,   205,   206,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,     0,     0,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,    71,
       0,    72,     0,    73,    74,   201,     0,     0,     0,    76,
       0,     0,     0,   146,     0,    78,     0,   147,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   141,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,     0,   142,   143,
       0,   144,     0,    30,     0,     0,     0,     0,    33,    34,
       0,    36,     0,    37,    38,   688,     0,     0,     0,     0,
      40,     0,    41,     0,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,    52,    53,
      54,   145,     0,     0,     0,   205,   206,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,    69,     0,
       0,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,    71,     0,    72,     0,    73,    74,     0,     0,
       0,     0,    76,     0,     0,     0,   146,     0,    78,     0,
     147,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     141,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   142,   143,     0,   144,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,    40,   170,    41,     0,    43,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,    52,    53,    54,   145,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,   171,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,     0,     0,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,    71,     0,    72,     0,    73,
      74,     0,     0,     0,     0,    76,     0,     0,     0,   146,
       0,    78,     0,   147,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   141,     0,     0,     0,     0,     0,     0,
       0,   175,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,   176,   142,   143,     0,   144,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,    40,     0,    41,     0,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,    52,    53,    54,   145,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,     0,     0,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,    71,     0,
      72,     0,    73,    74,     0,     0,     0,     0,    76,     0,
       0,     0,   146,     0,    78,     0,   147,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   141,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   142,   143,     0,
     144,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,    40,
       0,    41,     0,    43,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,    52,    53,    54,
     145,     0,     0,     0,   205,   206,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,     0,     0,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,    71,     0,    72,     0,    73,    74,     0,     0,     0,
       0,    76,     0,     0,     0,   146,     0,    78,     0,   147,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   141,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,     0,
     142,   143,     0,   144,     0,    30,     0,     0,     0,     0,
      33,    34,     0,    36,     0,    37,    38,     0,     0,     0,
       0,     0,    40,     0,    41,     0,    43,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
      52,    53,    54,   145,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,     0,     0,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,    71,     0,    72,     0,    73,    74,
     201,     0,     0,     0,    76,     0,     0,     0,   146,   576,
      78,     0,   147,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   187,    22,     0,     0,
       0,     0,     0,   142,   143,     0,   144,     0,    30,     0,
       0,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,    40,     0,    41,     0,    43,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,    52,    53,    54,   145,     0,     0,     0,
       0,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,     0,     0,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,    71,     0,    72,
       0,    73,    74,     0,     0,     0,     0,    76,     0,     0,
       0,    77,     0,    78,     0,   147,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   142,   143,     0,   144,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,    40,     0,
      41,     0,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,    52,    53,    54,   145,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,     0,     0,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
      71,     0,    72,     0,    73,    74,   201,     0,     0,     0,
      76,     0,     0,     0,   146,     0,    78,     0,   147,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   141,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   142,
     143,     0,   144,     0,    30,     0,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,    40,     0,    41,     0,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,    52,
      53,    54,   145,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,   171,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
       0,     0,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,    71,     0,    72,     0,    73,    74,     0,
       0,     0,     0,    76,     0,     0,     0,   146,     0,    78,
       0,   147,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   141,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   849,     0,     0,     0,
       0,     0,   142,   143,     0,   144,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,   850,     0,    41,     0,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   851,   852,    53,    54,   853,     0,     0,     0,     0,
       0,     0,     0,     0,   854,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,     0,     0,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,    71,     0,    72,     0,
      73,    74,   201,     0,     0,     0,    76,     0,     0,     0,
     146,     0,    78,     0,   855,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   141,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,     0,     0,     0,   142,   143,     0,   144,     0,
      30,     0,     0,     0,     0,    33,    34,     0,    36,     0,
      37,    38,     0,     0,     0,     0,     0,    40,     0,    41,
       0,    43,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,    52,    53,    54,   145,     0,
       0,     0,     0,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,     0,     0,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,    71,
       0,    72,     0,    73,    74,   201,     0,     0,     0,    76,
       0,     0,     0,   146,     0,    78,     0,   855,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   141,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,     0,   142,   143,
       0,   835,     0,    30,     0,     0,     0,     0,    33,    34,
       0,    36,     0,    37,    38,     0,     0,     0,     0,     0,
      40,     0,    41,     0,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,    52,    53,
      54,   145,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,    69,     0,
       0,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,    71,     0,    72,     0,    73,    74,   201,     0,
       0,     0,    76,     0,     0,     0,   146,     0,    78,     0,
     147,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     141,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   142,   143,     0,   144,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,    40,     0,    41,     0,    43,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,    52,    53,    54,   145,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,     0,     0,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,    71,     0,    72,     0,    73,
      74,     0,     0,     0,     0,    76,     0,     0,     0,   146,
       0,    78,     0,   147,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   141,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   142,   143,     0,   144,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,    40,     0,    41,     0,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,    52,    53,    54,   145,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,     0,     0,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,    71,     0,
      72,     0,    73,    74,     0,     0,     0,     0,    76,     0,
       0,     0,    77,     0,    78,     0,   147,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   141,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   142,   143,     0,
     144,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,    40,
       0,    41,     0,    43,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,    52,    53,    54,
     145,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,     0,     0,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,    71,     0,    72,     0,    73,    74,     0,     0,     0,
       0,   536,     0,     0,     0,   146,     0,    78,     0,   147,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   141,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,     0,
     142,   143,     0,   144,     0,    30,     0,     0,     0,     0,
      33,    34,     0,    36,     0,    37,    38,     0,     0,     0,
       0,     0,    40,     0,    41,     0,    43,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
      52,    53,    54,   145,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,     0,     0,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,    71,     0,    72,     0,    73,    74,
       0,     0,     0,     0,    76,     0,     0,     0,   146,     0,
      78,     0,   555,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,   142,   143,     0,   634,     0,    30,     0,
       0,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,    40,     0,    41,     0,    43,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,    52,    53,    54,   145,     0,     0,     0,
       0,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,     0,     0,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,    71,     0,    72,
       0,    73,    74,     0,     0,     0,     0,    76,     0,     0,
       0,   146,     0,    78,     0,   147,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   142,   143,     0,   637,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,    40,     0,
      41,     0,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,    52,    53,    54,   145,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,     0,     0,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
      71,     0,    72,     0,    73,    74,     0,     0,     0,     0,
      76,     0,     0,     0,   146,     0,    78,     0,   147,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   141,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   142,
     143,     0,   829,     0,    30,     0,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,    40,     0,    41,     0,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,    52,
      53,    54,   145,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
       0,     0,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,    71,     0,    72,     0,    73,    74,     0,
       0,     0,     0,    76,     0,     0,     0,   146,     0,    78,
       0,   147,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   141,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,   142,   143,     0,   830,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,    40,     0,    41,     0,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,    52,    53,    54,   145,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,     0,     0,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,    71,     0,    72,     0,
      73,    74,     0,     0,     0,     0,    76,     0,     0,     0,
     146,     0,    78,     0,   147,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   141,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,     0,     0,     0,   142,   143,     0,   832,     0,
      30,     0,     0,     0,     0,    33,    34,     0,    36,     0,
      37,    38,     0,     0,     0,     0,     0,    40,     0,    41,
       0,    43,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,    52,    53,    54,   145,     0,
       0,     0,     0,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,     0,     0,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,    71,
       0,    72,     0,    73,    74,     0,     0,     0,     0,    76,
       0,     0,     0,   146,     0,    78,     0,   147,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   141,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,     0,   142,   143,
       0,   833,     0,    30,     0,     0,     0,     0,    33,    34,
       0,    36,     0,    37,    38,     0,     0,     0,     0,     0,
      40,     0,    41,     0,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,    52,    53,
      54,   145,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,    69,     0,
       0,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,    71,     0,    72,     0,    73,    74,     0,     0,
       0,     0,    76,     0,     0,     0,   146,     0,    78,     0,
     147,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     141,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   142,   143,     0,   834,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,    40,     0,    41,     0,    43,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,    52,    53,    54,   145,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,     0,     0,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,    71,     0,    72,     0,    73,
      74,     0,     0,     0,     0,    76,     0,     0,     0,   146,
       0,    78,     0,   147,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   141,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   142,   143,     0,   835,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,    40,     0,    41,     0,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,    52,    53,    54,   145,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,     0,     0,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,    71,     0,
      72,     0,    73,    74,     0,     0,     0,     0,    76,     0,
       0,     0,   146,     0,    78,     0,   147,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   141,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   142,   143,     0,
     144,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,    40,
       0,    41,     0,    43,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,    52,    53,    54,
     145,     0,     0,     0,     0,     0,   270,     0,     0,    61,
       0,     0,     0,     0,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   141,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,    22,     0,
       0,     0,     0,     0,   142,   143,   271,   144,     0,    30,
     272,    71,     0,    72,    33,    34,    74,    36,     0,    37,
      38,    76,     0,     0,     5,   146,    40,    78,    41,   147,
      43,   718,     0,     0,     0,     0,   719,     0,     0,     0,
       0,     0,     0,    51,    52,    53,    54,   145,     0,     0,
       0,   274,     0,   275,   276,   277,    61,   278,   279,   280,
       0,   281,   282,   283,   284,     0,   286,   287,   288,   289,
       0,   290,     0,   292,     0,     0,   293,   294,   295,     0,
     268,     0,    67,     0,     0,   269,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   270,     0,     0,     0,     0,
      72,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    78,     0,   147,   355,   356,   357,
     358,   359,   360,     0,     0,   361,   362,   363,   364,   365,
     366,   720,   721,   722,   723,   271,     0,   724,     0,   272,
       0,   725,   726,   727,   728,   729,   730,   731,   732,   733,
     734,   735,     0,   736,     0,   268,   737,   738,   739,   740,
     269,   741,     0,     0,   742,   273,     0,     0,     0,     0,
     270,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274,     0,   275,   276,   277,     0,   278,   279,   280,     0,
     281,   282,   283,   284,   285,   286,   287,   288,   289,     0,
     290,   291,   292,     0,     0,   293,   294,   295,     0,     0,
     271,     0,     0,     0,   272,     0,   908,     0,     0,   268,
       0,     0,     0,     0,   269,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   270,     0,     0,     0,     0,     0,
     273,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   274,     0,   275,   276,   277,
       0,   278,   279,   280,     0,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   271,   290,   291,   292,   272,     0,
     293,   294,   295,   268,     0,     0,     0,     0,   269,     0,
       0,   945,     0,     0,     0,     0,     0,     0,   270,     0,
       0,     0,     0,     0,   273,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   274,
       0,   275,   276,   277,     0,   278,   279,   280,     0,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   271,   290,
     291,   292,   272,     0,   293,   294,   295,   268,     0,     0,
       0,     0,   269,   577,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,     0,     0,     0,   273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   274,     0,   275,   276,   277,     0,   278,
     279,   280,     0,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   271,   290,   291,   292,   272,     0,   293,   294,
     295,     0,     0,     0,   354,     0,   268,   879,     0,     0,
       0,   269,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   270,   273,   355,   356,   357,   358,   359,   360,     0,
       0,   361,   362,   363,   364,   365,   366,   274,     0,   275,
     276,   277,     0,   278,   279,   280,     0,   281,   282,   283,
     284,   285,   286,   287,   288,   289,     0,   290,   291,   292,
       0,   271,   293,   294,   295,   272,     0,     0,   319,     0,
       0,     0,     0,   764,     0,   268,     0,     0,     0,     0,
     269,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     270,   273,   355,   356,   357,   358,   359,   360,     0,     0,
     361,   362,   363,   364,   365,   366,   274,     0,   275,   276,
     277,     0,   278,   279,   280,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,     0,   290,   291,   292,     0,
     271,   293,   294,   295,   272,     0,     0,   319,     0,     0,
       0,     0,   769,     0,   268,     0,     0,     0,     0,   269,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   270,
     273,   355,   356,   357,   358,   359,   360,     0,     0,   361,
     362,   363,   364,   365,   366,   274,     0,   275,   276,   277,
       0,   278,   279,   280,     0,   281,   282,   283,   284,   285,
     286,   287,   288,   289,     0,   290,   291,   292,     0,   271,
     293,   294,   295,   272,     0,     0,   319,     0,     0,     0,
       0,   892,     0,   268,     0,     0,     0,     0,   269,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   270,   273,
     355,   356,   357,   358,   359,   360,     0,     0,   361,   362,
     363,   364,   365,   366,   274,     0,   275,   276,   277,     0,
     278,   279,   280,     0,   281,   282,   283,   284,   285,   286,
     287,   288,   289,     0,   290,   291,   292,     0,   271,   293,
     294,   295,   272,     0,     0,   319,     0,     0,     0,     0,
     893,     0,   268,     0,     0,     0,     0,   269,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,   273,   355,
     356,   357,   358,   359,   360,     0,     0,   361,   362,   363,
     364,   365,   366,   274,     0,   275,   276,   277,     0,   278,
     279,   280,     0,   281,   282,   283,   284,   285,   286,   287,
     288,   289,     0,   290,   291,   292,     0,   271,   293,   294,
     295,   272,     0,     0,   319,     0,     0,     0,     0,   894,
       0,   268,     0,     0,     0,     0,   269,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   270,   273,   355,   356,
     357,   358,   359,   360,     0,     0,   361,   362,   363,   364,
     365,   366,   274,     0,   275,   276,   277,     0,   278,   279,
     280,     0,   281,   282,   283,   284,   285,   286,   287,   288,
     289,     0,   290,   291,   292,     0,   271,   293,   294,   295,
     272,     0,     0,   319,     0,     0,     0,     0,   895,     0,
     268,     0,     0,     0,     0,   269,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   270,   273,   355,   356,   357,
     358,   359,   360,     0,     0,   361,   362,   363,   364,   365,
     366,   274,     0,   275,   276,   277,     0,   278,   279,   280,
       0,   281,   282,   283,   284,   285,   286,   287,   288,   289,
       0,   290,   291,   292,     0,   271,   293,   294,   295,   272,
       0,     0,   319,     0,     0,     0,     0,   910,     0,   268,
       0,     0,     0,     0,   269,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   270,   273,   355,   356,   357,   358,
     359,   360,     0,     0,   361,   362,   363,   364,   365,   366,
     274,     0,   275,   276,   277,     0,   278,   279,   280,     0,
     281,   282,   283,   284,   285,   286,   287,   288,   289,     0,
     290,   291,   292,     0,   271,   293,   294,   295,   272,     0,
       0,   319,     0,     0,     0,     0,   911,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   273,   355,   356,   357,   358,   359,
     360,     0,     0,   361,   362,   363,   364,   365,   366,   274,
       0,   275,   276,   277,     0,   278,   279,   280,     0,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   268,   290,
     291,   292,     0,   269,   293,   294,   295,     0,     0,     0,
     319,     0,     0,   270,   346,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,   268,     0,     0,     0,     0,   269,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,   343,     0,
       0,     0,     0,   271,     0,     0,     0,   272,     0,   344,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   348,
       0,     0,     0,   273,     0,     0,     0,   271,     0,     0,
       0,   272,     0,     0,     0,     0,     0,     0,   274,     0,
     275,   276,   277,     0,   278,   279,   280,     0,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   273,   290,   291,
     292,     0,     0,   293,   294,   295,     0,     0,     0,   319,
       0,     0,   274,     0,   275,   276,   277,     0,   278,   279,
     280,     0,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   268,   290,   291,   292,     0,   269,   293,   294,   295,
       0,     0,     0,   319,     0,     0,   270,   660,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   268,     0,     0,     0,     0,
     269,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     270,   187,     0,     0,     0,     0,   271,     0,     0,     0,
     272,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   348,     0,     0,     0,   273,     0,     0,     0,
     271,     0,     0,     0,   272,     0,     0,     0,     0,     0,
       0,   274,     0,   275,   276,   277,     0,   278,   279,   280,
       0,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     273,   290,   291,   292,     0,     0,   293,   294,   295,     0,
       0,     0,   319,     0,     0,   274,     0,   275,   276,   277,
       0,   278,   279,   280,     0,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   268,   290,   291,   292,     0,   269,
     293,   294,   295,     0,     0,     0,   319,     0,     0,   270,
     654,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   268,     0,
       0,     0,     0,   269,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   270,     0,     0,     0,     0,     0,   271,
       0,     0,     0,   272,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   268,     0,     0,
       0,     0,   269,     0,     0,     0,     0,     0,     0,   273,
       0,     0,   270,   271,     0,     0,     0,   272,     0,     0,
       0,     0,     0,     0,   274,   672,   275,   276,   277,     0,
     278,   279,   280,     0,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   273,   290,   291,   292,     0,     0,   293,
     294,   295,   271,     0,     0,   319,   272,     0,   274,     0,
     275,   276,   277,     0,   278,   279,   280,     0,   281,   282,
     283,   284,   285,   286,   287,   288,   289,     0,   290,   291,
     292,   268,   273,   293,   294,   295,   269,     0,     0,   319,
       0,     0,     0,     0,     0,     0,   270,   274,     0,   275,
     276,   277,     0,   278,   279,   280,     0,   281,   282,   283,
     284,   285,   286,   287,   288,   289,     0,   290,   291,   292,
       0,     0,   293,   294,   295,     0,     0,   268,   394,     0,
       0,     0,   269,     0,     0,     0,   271,     0,     0,     0,
     272,     0,   270,     0,     0,     0,     0,   351,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   273,     0,     0,   352,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   274,   271,   275,   276,   277,   272,   278,   279,   280,
       0,   281,   282,   283,   284,   285,   286,   287,   288,   289,
       0,   290,   291,   292,     0,   268,   293,   294,   295,     0,
     269,  -409,   273,     0,     0,     0,     0,     0,     0,     0,
     270,     0,     0,     0,     0,     0,     0,   274,     0,   275,
     276,   277,   325,   278,   279,   280,     0,   281,   282,   283,
     284,   285,   286,   287,   288,   289,     0,   290,   291,   292,
       0,   353,   293,   294,   295,     0,     0,     0,     0,     0,
     271,     0,     0,     0,   272,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   268,     0,     0,     0,   135,   269,     0,     0,
     273,     0,     0,     0,     0,     0,     0,   270,     0,     0,
       0,     0,   407,     0,     0,   274,     0,   275,   276,   277,
       0,   278,   279,   280,     0,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   408,   290,   291,   292,   268,     0,
     293,   294,   295,   269,     0,     0,     0,   271,     0,     0,
       0,   272,     0,   270,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   507,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   273,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   274,   271,   275,   276,   277,   272,   278,   279,
     280,     0,   281,   282,   283,   284,   285,   286,   287,   288,
     289,     0,   290,   291,   292,     0,   268,   293,   294,   295,
       0,   269,     0,   273,     0,     0,     0,     0,     0,     0,
       0,   270,   508,     0,     0,     0,     0,     0,   274,     0,
     275,   276,   277,   509,   278,   279,   280,     0,   281,   282,
     283,   284,   285,   286,   287,   288,   289,     0,   290,   291,
     292,     0,   268,   293,   294,   295,     0,   269,     0,     0,
       0,   271,     0,     0,     0,   272,     0,   270,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     268,   273,     0,     0,     0,   269,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   270,   274,   271,   275,   276,
     277,   272,   278,   279,   280,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,     0,   290,   291,   292,     0,
       0,   293,   294,   295,     0,     0,   268,   273,     0,     0,
       0,   269,     0,     0,     0,   271,     0,     0,     0,   272,
       0,   270,   274,     0,   275,   276,   277,     0,   278,   279,
     280,     0,   281,   282,   283,   284,   285,   286,   287,   288,
     289,     0,   290,   291,   292,   273,   398,   293,   294,   295,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274,   271,   275,   276,   277,   272,   278,   279,   280,     0,
     281,   282,   283,   284,   285,   286,   287,   288,   289,     0,
     290,   291,   292,   268,   400,   293,   294,   295,   269,     0,
       0,   273,     0,     0,     0,     0,     0,     0,   270,     0,
       0,     0,     0,     0,     0,     0,   274,     0,   275,   276,
     277,     0,   278,   279,   280,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,     0,   290,   291,   292,   268,
     402,   293,   294,   295,   269,     0,     0,     0,   271,     0,
       0,     0,   272,     0,   270,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   268,   273,     0,
       0,     0,   269,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,   274,   271,   275,   276,   277,   272,   278,
     279,   280,     0,   281,   282,   283,   284,   285,   286,   287,
     288,   289,     0,   290,   291,   292,     0,   410,   293,   294,
     295,     0,     0,   425,   273,     0,     0,     0,     0,     0,
       0,     0,   271,     0,     0,     0,   272,     0,     0,   274,
       0,   275,   276,   277,   510,   278,   279,   280,     0,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   268,   290,
     291,   292,   273,   269,   293,   294,   295,     0,     0,     0,
       0,     0,     0,   270,     0,     0,     0,   274,     0,   275,
     276,   277,     0,   278,   279,   280,     0,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   268,   290,   291,   292,
       0,   269,   293,   294,   295,     0,     0,     0,     0,     0,
       0,   270,     0,   271,     0,     0,     0,   272,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   135,
       0,     0,     0,   273,     0,     0,     0,     0,     0,     0,
       0,   271,     0,     0,     0,   272,     0,     0,   274,     0,
     275,   276,   277,     0,   278,   279,   280,     0,   281,   282,
     283,   284,   285,   286,   287,   288,   289,     0,   290,   291,
     292,   273,     0,   293,   294,   295,   268,   667,     0,     0,
       0,   269,     0,     0,     0,     0,   274,     0,   275,   276,
     277,   270,   278,   279,   280,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,     0,   290,   291,   292,     0,
     644,   293,   294,   295,   268,     0,     0,     0,     0,   269,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   270,
       0,   271,   670,     0,     0,   272,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   268,     0,     0,     0,     0,   269,     0,
       0,   273,     0,     0,     0,     0,     0,     0,   270,   271,
       0,     0,     0,   272,     0,     0,   274,     0,   275,   276,
     277,     0,   278,   279,   280,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,     0,   290,   291,   292,   273,
       0,   293,   294,   295,     0,     0,     0,     0,   271,     0,
       0,     0,   272,     0,   274,     0,   275,   276,   277,     0,
     278,   279,   280,     0,   281,   282,   283,   284,   285,   286,
     287,   288,   289,     0,   290,   291,   292,   708,   273,   293,
     294,   295,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   274,     0,   275,   276,   277,     0,   278,
     279,   280,     0,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   268,   290,   291,   292,     0,   269,   293,   294,
     295,     0,     0,     0,     0,     0,     0,   270,   754,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,     0,     0,     0,
       0,   269,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   270,   757,     0,     0,     0,     0,   271,     0,     0,
       0,   272,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   273,     0,     0,
       0,   271,     0,     0,     0,   272,     0,     0,     0,     0,
       0,     0,   274,     0,   275,   276,   277,     0,   278,   279,
     280,     0,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   273,   290,   291,   292,     0,   268,   293,   294,   295,
       0,   269,     0,     0,     0,     0,   274,     0,   275,   276,
     277,   270,   278,   279,   280,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,     0,   290,   291,   292,     0,
     268,   293,   294,   295,     0,   269,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   270,     0,     0,     0,     0,
       0,   271,     0,     0,     0,   272,     0,     0,     0,     0,
       0,     0,     0,   828,     0,     0,     0,     0,     0,   268,
     845,     0,     0,     0,   269,     0,     0,     0,     0,     0,
       0,   273,     0,     0,   270,   271,     0,     0,     0,   272,
       0,     0,     0,     0,     0,     0,   274,   831,   275,   276,
     277,     0,   278,   279,   280,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   273,   290,   291,   292,     0,
       0,   293,   294,   295,   271,     0,     0,     0,   272,     0,
     274,     0,   275,   276,   277,     0,   278,   279,   280,     0,
     281,   282,   283,   284,   285,   286,   287,   288,   289,     0,
     290,   291,   292,   268,   273,   293,   294,   295,   269,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   270,   274,
       0,   275,   276,   277,     0,   278,   279,   280,     0,   281,
     282,   283,   284,   285,   286,   287,   288,   289,     0,   290,
     291,   292,     0,     0,   293,   294,   295,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   271,     0,
       0,     0,   272,     0,     0,     0,     0,   268,     0,     0,
     917,     0,   269,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,     0,     0,     0,   273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   274,     0,   275,   276,   277,     0,   278,
     279,   280,     0,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   271,   290,   291,   292,   272,     0,   293,   294,
     295,   268,     0,     0,   918,     0,   269,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   270,     0,     0,     0,
       0,     0,   273,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   274,     0,   275,
     276,   277,     0,   278,   279,   280,     0,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   271,   290,   291,   292,
     272,     0,   293,   294,   295,   268,     0,     0,   919,     0,
     269,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     270,     0,     0,     0,     0,     0,   273,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   274,     0,   275,   276,   277,     0,   278,   279,   280,
       0,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     271,   290,   291,   292,   272,     0,   293,   294,   295,   268,
       0,     0,   920,     0,   269,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   270,     0,     0,     0,     0,     0,
     273,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   274,     0,   275,   276,   277,
       0,   278,   279,   280,     0,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   271,   290,   291,   292,   272,     0,
     293,   294,   295,   268,     0,     0,   921,     0,   269,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   270,     0,
       0,     0,     0,     0,   273,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   274,
       0,   275,   276,   277,     0,   278,   279,   280,     0,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   271,   290,
     291,   292,   272,     0,   293,   294,   295,   268,     0,     0,
     922,     0,   269,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,     0,     0,     0,   273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   274,     0,   275,   276,   277,     0,   278,
     279,   280,     0,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   271,   290,   291,   292,   272,     0,   293,   294,
     295,   268,     0,     0,     0,     0,   269,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   270,     0,     0,     0,
       0,     0,   273,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   274,     0,   275,
     276,   277,     0,   278,   279,   280,     0,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   632,   290,   291,   292,
     272,     0,   293,   294,   295,     0,   270,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   273,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   274,     0,   275,   276,   277,   271,   278,   279,   280,
     272,   281,   282,   283,   284,   285,   286,   287,   288,   289,
       0,   290,   291,   292,     0,     0,   293,   294,   295,     0,
       0,     0,     0,     0,     0,     0,   273,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   274,     0,   275,   276,   277,     0,   278,   279,   280,
       0,   281,   282,   283,   284,     0,   286,   287,   288,   289,
       0,   290,   291,   292,     0,     0,   293,   294,   295
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-821)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       2,     2,    44,    45,   371,   403,   316,   534,    47,    11,
     157,   603,    26,   374,    16,    15,   685,    19,    20,    21,
     379,   168,   250,    62,    26,    27,    28,    29,   554,   746,
       3,    24,     3,    35,    36,    37,     1,    39,    40,    41,
      79,     3,     3,     3,   403,    47,    26,    49,    50,    51,
      52,    26,   711,    55,    56,    57,    58,   863,     3,    61,
      62,    63,    64,   711,    91,    67,    53,     3,    70,    71,
      72,    26,    74,    37,    68,    77,    21,    79,    26,    81,
     180,     1,   262,   263,   264,   265,   115,     1,    53,    96,
      37,    27,    37,     1,   352,    40,   899,   197,   115,   115,
       0,    88,    41,   630,    66,   143,    37,   145,   147,    54,
      83,    56,   119,    37,   934,   134,   216,   146,   787,   146,
      17,    66,    61,    88,    37,   945,    78,   144,   144,    37,
     115,   934,   938,    37,   136,   115,    91,    82,    91,   141,
     142,   143,   144,   145,   146,   147,    82,   141,    68,   115,
     408,   115,    68,   153,    68,   142,   682,   142,   817,    79,
      91,   141,    66,     3,   881,   882,   141,    64,   115,    89,
     143,   115,   143,    82,    21,    89,   142,   142,   837,   181,
     141,   143,   143,   143,   115,   187,   141,    84,    96,   837,
     134,   115,   553,   141,   114,   417,     7,   419,   143,   146,
     114,    21,   115,   205,   206,   198,   114,   115,   115,    56,
     115,   163,   312,   313,   134,   146,    21,    37,   134,    66,
      40,   141,   146,    48,   534,   141,   134,   141,   180,   898,
     115,   115,   824,   146,    54,    82,    56,   144,    75,   144,
     242,   242,   134,   245,   115,   197,    66,   134,     3,   776,
     142,    56,     7,    78,   434,   142,   134,   271,   272,   144,
     144,    66,    82,   134,   216,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,    82,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,    25,   821,   396,   115,   143,     3,
     314,   315,   341,     7,   532,   533,   115,   674,    21,   115,
     134,   325,   314,   315,   316,   676,   143,   319,   115,   143,
     630,   145,   143,   325,   145,   140,   144,   329,   134,   331,
     344,    13,    62,   347,    65,   144,    18,    41,    69,   341,
      96,   343,   344,    56,   346,   347,   134,   144,    96,   351,
     352,    37,   354,    66,   582,   583,     2,    61,   143,   115,
     312,   313,   115,   119,    65,    11,   368,   115,    69,   143,
     115,   119,    65,    19,    87,    21,    69,   379,   115,   381,
     115,   134,    68,   114,   134,     2,   117,   389,   144,   134,
     121,   430,   115,    79,    11,   793,   144,   134,   115,   134,
     134,   403,    19,    89,    21,   407,   408,   421,   134,    55,
      96,   134,   414,   134,   143,   114,   145,   134,   117,   421,
     422,   423,   121,   425,   426,   134,   573,   786,   430,   115,
       1,    77,     3,   143,   793,    81,   134,   389,    55,     3,
      57,    58,   134,     7,   396,     9,   141,    25,   134,    57,
      58,    90,   143,   143,   117,   141,   134,    21,   825,   143,
      77,    96,   414,   134,    81,   417,   776,   419,     7,   114,
     422,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,    65,    47,   141,
     136,    69,    56,   507,   134,   509,    96,   144,   134,   115,
      37,   143,    66,    37,   506,   507,   508,   509,   510,   511,
     512,    28,    25,   513,   514,   144,   555,   115,    82,   136,
     144,    96,   524,    87,   526,   144,     3,    16,   530,   144,
       7,   115,   534,   146,    68,   181,   114,   141,   116,   117,
     899,   187,   544,   121,    21,    79,   134,   134,   134,   908,
     128,   134,    65,   555,   134,    89,    69,   135,   136,   137,
     134,   134,    96,   144,   181,   134,   134,   519,   134,   521,
     187,   134,   134,   134,   134,   934,   134,   134,    28,    56,
     134,   115,   115,   134,   134,   587,   945,   142,   134,    66,
     146,   144,   417,   134,   419,   144,   242,   141,    96,    17,
     134,   114,    20,   116,   117,    82,   115,   141,   121,    66,
      87,    29,   142,    31,    32,   128,    26,    35,   632,   633,
      26,    39,   146,    41,   137,   242,    50,   141,   630,    28,
     632,   633,   634,   114,    66,   637,   146,   117,   114,   146,
      58,   146,    60,    61,   117,    63,    64,   142,    28,   143,
     652,   144,   654,   692,    89,   657,   144,   142,   660,    79,
       2,   709,   408,   665,    82,   667,    84,   669,   670,    11,
     672,   407,   224,   319,    20,   696,   762,    19,   169,    21,
     523,   714,   824,   674,   825,    31,    32,   717,   938,   691,
     692,   898,   913,    39,   519,    41,    28,   343,   848,   382,
     346,   703,   319,   705,   521,    -1,   708,   709,   354,    -1,
      -1,   713,    -1,    55,    -1,    61,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,   343,    -1,    -1,   346,
      -1,    -1,    -1,   558,    -1,    77,    -1,   354,    -1,    81,
     742,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,   173,
     174,   753,   754,    -1,   756,   757,   758,   759,    -1,    -1,
      -1,    -1,   764,   765,    -1,   767,   190,   769,   770,   193,
      -1,   773,   814,   815,   776,    -1,    -1,   423,    -1,    -1,
      -1,   783,    -1,    37,   786,    -1,    -1,    -1,   212,    -1,
     792,   793,   794,    -1,   136,    -1,   798,    -1,   800,    -1,
      -1,   803,   804,    -1,    -1,   807,   423,    -1,   232,   811,
     811,    -1,    -1,    -1,    68,    -1,   855,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,   828,   829,   830,   831,
     832,   833,   834,   835,   786,    89,    -1,    -1,    -1,   181,
      -1,    -1,    96,   845,   846,   187,    -1,    20,   850,   851,
     852,   853,   854,   855,    -1,   501,    -1,   859,    31,    32,
     862,   115,    -1,    -1,    -1,    -1,    39,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,
     134,    -1,    -1,    -1,    -1,    -1,    -1,   141,    61,    62,
     892,   893,   894,   895,    -1,    -1,    -1,   899,   544,    -1,
     242,    -1,   326,    -1,    -1,    -1,   908,   859,   910,   911,
      -1,    -1,    -1,    -1,    -1,   917,   918,   919,   920,   921,
     922,    -1,    -1,    -1,    -1,    -1,   350,   544,    -1,    -1,
      -1,   933,   934,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,   587,    -1,   945,    -1,     2,    -1,   899,    20,    -1,
      -1,    -1,    -1,    -1,    11,    -1,    -1,   603,    -1,    31,
      32,    -1,    19,    -1,    21,    -1,    -1,    39,    -1,    41,
     587,    -1,    -1,    -1,    -1,    47,    65,   319,    -1,    -1,
      69,    -1,   934,    -1,    -1,    -1,   632,   633,    -1,    61,
      62,    -1,    -1,   945,    -1,    -1,    -1,    -1,    55,    -1,
      -1,   343,    -1,    -1,   346,    -1,   652,    -1,   654,    -1,
      -1,   657,   354,    -1,   660,    -1,    -1,    -1,    -1,    -1,
      77,   110,    -1,   669,    81,   114,   672,   116,   117,    -1,
      -1,    -1,   121,    -1,    -1,   652,    -1,   654,    -1,   128,
     657,     2,    -1,   660,    -1,   691,   135,   136,   137,    -1,
      11,    -1,   669,    -1,    -1,   672,    -1,   703,    19,   705,
      21,    -1,    -1,   709,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,   691,    -1,    -1,    -1,    -1,   136,
      -1,   423,    -1,    -1,    -1,    -1,   703,    -1,   705,    -1,
      -1,    -1,   709,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   528,   529,    -1,    -1,   532,   533,
      -1,    65,    -1,    -1,    -1,    69,    77,    -1,   764,    -1,
      81,    -1,    -1,   769,   181,    -1,    -1,    -1,    -1,    -1,
     187,    -1,    -1,    -1,    -1,    -1,    -1,   783,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   764,   794,    -1,
      -1,    -1,   769,    -1,    -1,    -1,   110,   803,   112,   113,
     114,   807,   116,   117,   118,   811,   783,   121,   122,   123,
      -1,   125,   126,   127,   128,   136,    -1,   794,   132,    -1,
      -1,   135,   136,   137,    -1,   242,   803,    -1,    -1,    -1,
     807,    -1,    -1,    -1,   811,    -1,    -1,    -1,    -1,    -1,
     846,    -1,   544,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   642,   643,
     181,    -1,    -1,    -1,    -1,   649,   187,    -1,    -1,   846,
      -1,    -1,    -1,    -1,    -1,   659,    -1,    -1,   662,    -1,
      -1,    -1,    -1,    -1,    -1,   587,   892,   893,   894,   895,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   319,    -1,   910,   911,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   892,   893,   894,   895,    -1,
      -1,   242,    -1,    -1,    -1,    -1,   343,    -1,    -1,   346,
      -1,    -1,    -1,   910,   911,    -1,    -1,   354,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
     652,    -1,   654,    25,    -1,   657,    -1,    -1,   660,    -1,
       2,    -1,    -1,    -1,    -1,    -1,    -1,   669,    -1,    11,
     672,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   691,
      -1,    -1,    -1,    65,    -1,    -1,    -1,    69,   319,    -1,
      -1,   703,    -1,   705,    -1,    -1,   423,   709,    -1,    -1,
      -1,    -1,   796,    55,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,   343,    95,    -1,   346,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   354,    -1,    77,    -1,    -1,   110,    81,
     112,   113,   114,    -1,   116,   117,   118,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
     132,    -1,   764,   135,   136,   137,    -1,   769,    -1,    -1,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,     2,
      -1,   783,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,
      -1,    -1,   794,    -1,   136,    -1,    19,    -1,    21,    -1,
      -1,   803,   423,    -1,    -1,   807,    -1,    -1,    -1,   811,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   544,    -1,    -1,
      -1,    -1,    55,     3,     4,     5,     6,     7,     8,   181,
      -1,    -1,    12,    -1,   846,   187,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,   138,   139,    27,    81,    -1,
     143,    -1,   145,    33,    34,    -1,    36,    -1,    38,    -1,
     587,    -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    59,
     892,   893,   894,   895,    -1,    -1,    -1,    -1,    -1,    -1,
     242,    -1,    72,    73,    74,    75,    76,    -1,   910,   911,
      -1,    -1,     2,   136,    -1,    85,    -1,    25,    -1,    -1,
      -1,    11,    -1,   544,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    21,    -1,    -1,    -1,   652,    -1,   654,    -1,    -1,
     657,   111,    -1,   660,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   669,    25,    -1,   672,    -1,    65,   181,   129,
      -1,    69,    -1,    -1,   187,    55,   587,    -1,    -1,    -1,
      -1,   141,    -1,   143,   691,   145,    -1,   319,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   703,    77,   705,    -1,
      -1,    81,   709,    65,    -1,    -1,    -1,    69,    -1,    -1,
      -1,   343,   110,    -1,   346,   113,   114,    -1,   116,   117,
      -1,    -1,   354,   121,    -1,    -1,    -1,    -1,    -1,   242,
     128,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,
      -1,   652,    -1,   654,    -1,    -1,   657,    -1,   110,   660,
     112,   113,   114,    -1,   116,   117,   136,   764,   669,   121,
      -1,   672,   769,    -1,    -1,   127,   128,    -1,    -1,    -1,
     132,    -1,    -1,   135,   136,   137,   783,    -1,    -1,    -1,
     691,    -1,    -1,     1,    -1,    -1,    -1,   794,    -1,    -1,
      -1,   423,   703,    -1,   705,    -1,   803,    -1,   709,    17,
     807,   181,    20,    21,   811,    -1,   319,   187,    -1,    -1,
      25,    29,    35,    31,    32,    -1,    -1,    35,    -1,    -1,
      -1,    39,    -1,    41,    -1,    -1,    -1,    50,    -1,    -1,
     343,    -1,    -1,   346,    57,    58,    -1,    -1,    56,   846,
      58,   354,    60,    61,    -1,    63,    64,    -1,    66,    -1,
      65,    -1,    -1,   764,    69,    -1,    -1,    -1,   769,    -1,
      -1,    -1,   242,    -1,    82,    -1,    84,    -1,    -1,    87,
      -1,    -1,   783,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   794,    -1,   892,   893,   894,   895,    -1,
      -1,    -1,   803,    -1,    -1,   110,   807,   112,   113,   114,
     811,   116,   117,   910,   911,    -1,   121,    -1,    -1,    -1,
     423,    -1,   544,   128,    -1,    -1,   134,   140,    -1,    -1,
     135,   136,   137,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   846,    -1,    -1,    -1,   319,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,
     173,   174,    -1,    -1,    -1,   587,   179,    -1,    -1,    -1,
      -1,    -1,    -1,   343,    -1,    -1,   346,   190,    -1,    -1,
     193,    -1,    -1,    -1,   354,    -1,    -1,    -1,    -1,    -1,
      -1,   892,   893,   894,   895,    -1,    -1,    -1,    -1,   212,
      -1,     3,    -1,    -1,    -1,    -1,    -1,     2,    10,   910,
     911,    -1,    -1,    15,    -1,    -1,    11,    -1,    -1,   232,
      -1,    -1,    -1,    -1,    19,    -1,    21,    -1,    -1,    -1,
     652,    -1,   654,    -1,    -1,   657,    -1,    -1,   660,    -1,
      -1,   544,    -1,    -1,    -1,    -1,    -1,   669,    -1,    -1,
     672,    -1,    -1,   423,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   691,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   703,    77,   705,   587,    -1,    81,   709,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,    -1,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    -1,   326,   116,    -1,    -1,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    -1,
     132,    -1,    -1,   135,   136,   137,   138,   350,   140,    -1,
      -1,   136,   764,    -1,    -1,    -1,    -1,   769,    -1,   652,
      -1,   654,    -1,    -1,   657,    -1,    -1,   660,    -1,    -1,
      -1,   783,    -1,    -1,    -1,    -1,   669,    -1,    -1,   672,
      -1,    -1,   794,    -1,   544,    -1,    -1,    -1,    -1,    17,
      -1,   803,    20,    -1,    -1,   807,   181,    -1,   691,   811,
      -1,    29,   187,    31,    32,    -1,    -1,    35,    -1,    -1,
     703,    39,   705,    41,    -1,    -1,   709,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,   587,    -1,    -1,
      58,    18,    60,    61,   846,    63,    64,    24,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    84,   242,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   764,    59,    -1,    -1,    -1,   769,    -1,    -1,    -1,
     892,   893,   894,   895,    -1,    -1,    -1,    -1,    -1,    -1,
     783,    -1,   652,    -1,   654,    -1,    -1,   657,   910,   911,
     660,   794,    -1,    -1,    -1,    -1,   134,    -1,    -1,   669,
     803,    -1,   672,    -1,   807,   102,    -1,   104,   811,    -1,
      -1,    -1,    -1,    -1,    -1,   528,   529,    -1,    -1,   532,
     533,   691,    -1,    -1,   319,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   545,   703,    -1,   705,    -1,    -1,    -1,   709,
      -1,    -1,    -1,   846,    -1,    -1,    -1,    -1,   343,    -1,
      -1,   346,    -1,    -1,    -1,    -1,   569,    -1,    -1,    -1,
     157,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,
      -1,   168,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,    -1,   183,    -1,    -1,   892,
     893,   894,   895,    -1,   764,    -1,    -1,    -1,    -1,   769,
     197,   198,    -1,    -1,   201,    -1,    -1,   910,   911,    -1,
      -1,    -1,    -1,   783,    -1,    -1,    -1,    -1,    -1,   216,
      -1,    -1,    -1,    -1,   794,    -1,    -1,    -1,   423,   642,
     643,    -1,    -1,   803,    -1,    -1,   649,   807,   651,    -1,
      -1,   811,    -1,   656,    -1,    -1,   659,    -1,    -1,   662,
      -1,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   262,   263,   264,   265,    -1,
      -1,     1,    -1,    -1,    -1,    -1,   846,    -1,    -1,    -1,
      -1,    -1,   279,    -1,    -1,    -1,   699,    17,    -1,    -1,
      20,    21,    -1,   138,   139,   140,    -1,    -1,   143,    29,
     145,    31,    32,    -1,    -1,    35,    -1,    -1,    -1,    39,
      -1,    41,    -1,    -1,    -1,   312,   313,    -1,    -1,    -1,
      -1,    -1,   892,   893,   894,   895,    56,    -1,    58,    -1,
      60,    61,    -1,    63,    64,    -1,    66,    -1,    -1,    -1,
     910,   911,   339,    -1,    -1,    -1,    -1,    -1,    -1,   544,
      17,    -1,    82,    20,    84,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    29,    -1,    31,    32,    -1,    -1,    35,    -1,
      -1,    -1,    39,    -1,    41,    -1,    -1,   374,    -1,    -1,
      -1,    -1,    -1,   796,    -1,   382,    -1,    -1,    -1,    -1,
      -1,    58,   587,    60,    61,    -1,    63,    64,    -1,   396,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    82,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   839,   840,    -1,   842,
     843,    -1,   429,    -1,    -1,   432,    -1,   434,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     863,    -1,    -1,    -1,    -1,    -1,   869,   652,   871,   654,
     873,    -1,   657,    -1,    -1,   660,    -1,   134,    -1,    -1,
      -1,    -1,    -1,    -1,   669,   142,    -1,   672,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,   691,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   703,    -1,
     705,    -1,    27,    -1,   709,    -1,    -1,    -1,    33,    34,
      -1,    36,    -1,    38,   521,   938,    -1,    -1,    43,    44,
      -1,    46,    10,    48,    49,    -1,    -1,    15,    -1,    -1,
      55,    -1,    57,    -1,    59,    -1,    -1,    25,    -1,    -1,
      -1,    -1,   549,    -1,    -1,    -1,   553,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,   569,    -1,    -1,    -1,   573,    -1,    -1,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    65,   783,    -1,
      -1,    69,    -1,    25,    -1,    -1,   111,    -1,    -1,   794,
     115,    -1,    -1,    -1,   119,    -1,   603,    -1,   803,    -1,
      -1,    -1,   807,    -1,   129,    -1,   811,    95,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   143,   144,
     145,    -1,   110,    65,   112,   113,   114,    69,   116,   117,
     118,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   846,   130,   131,   132,    -1,    -1,   135,   136,   137,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   676,
     112,   113,   114,    -1,   116,   117,   118,    -1,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,   130,   696,
     132,    -1,    -1,   135,   136,   137,    -1,    -1,     0,     1,
      -1,     3,     4,     5,     6,     7,     8,   714,    -1,    -1,
      12,    13,    14,    -1,   721,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    -1,    66,    67,    68,    -1,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    -1,    80,    81,
      82,    -1,    84,    85,    86,    87,    -1,    -1,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   813,    -1,   110,   111,
     112,   113,    -1,    -1,    -1,    -1,   118,   824,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,
     132,    -1,   134,    -1,    -1,   137,    -1,    -1,    -1,   141,
      -1,   143,     1,   145,     3,     4,     5,     6,     7,     8,
      -1,    10,    -1,    12,    13,    14,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    -1,    78,
      -1,    80,    81,    82,    -1,    84,    85,    86,    87,    -1,
      -1,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,    -1,   134,   135,   136,   137,    -1,
      -1,    -1,   141,   142,   143,     1,   145,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    -1,
      26,    27,    -1,    29,    -1,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    -1,    80,    81,    82,    -1,    84,    85,
      86,    87,    -1,    -1,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,   129,    -1,   131,   132,   133,   134,    -1,
      -1,   137,    -1,    -1,    -1,   141,   142,   143,     1,   145,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    -1,    17,    18,    19,    20,    21,    22,
      23,    24,    -1,    26,    27,    -1,    29,    -1,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    -1,    51,    -1,
      -1,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    75,    76,    -1,    78,    -1,    80,    81,    82,
      -1,    84,    85,    86,    87,    -1,    -1,    90,    -1,    92,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,
      -1,   134,    -1,    -1,   137,    -1,    -1,    -1,   141,   142,
     143,     1,   145,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    75,    76,    -1,    78,    -1,
      80,    81,    82,    -1,    84,    85,    86,    87,    -1,    -1,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,
      -1,   131,   132,    -1,   134,    -1,    -1,   137,    -1,    -1,
      -1,   141,   142,   143,     1,   145,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,
      17,    18,    19,    20,    21,    22,    23,    24,    -1,    26,
      27,    -1,    29,    -1,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    -1,    51,    -1,    -1,    -1,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    -1,    80,    81,    82,    -1,    84,    85,    86,
      87,    -1,    -1,    90,    -1,    92,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,   129,    -1,   131,   132,    -1,   134,    -1,    -1,
     137,    -1,    -1,    -1,   141,   142,   143,     1,   145,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    -1,    26,    27,    -1,    29,    -1,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    -1,    51,    -1,    -1,
      -1,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    -1,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    75,    76,    -1,    78,    -1,    80,    81,    82,    -1,
      84,    85,    86,    87,    -1,    -1,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,    -1,
     134,    -1,    -1,   137,    -1,    -1,    -1,   141,   142,   143,
       1,   145,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    -1,    66,    67,    68,    -1,    70,
      71,    72,    73,    74,    75,    76,    -1,    78,    -1,    80,
      81,    82,    -1,    84,    85,    86,    87,    -1,    -1,    90,
      -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,
     131,   132,    -1,   134,    -1,    -1,   137,    -1,    -1,    -1,
     141,    -1,   143,     1,   145,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    -1,    26,    27,
      -1,    29,    -1,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    -1,    51,    -1,    -1,    -1,    55,    56,    57,
      -1,    59,    60,    61,    62,    63,    64,    -1,    66,    67,
      68,    -1,    70,    71,    72,    73,    74,    75,    76,    -1,
      78,    -1,    80,    81,    82,    -1,    84,    85,    86,    87,
      -1,    -1,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,   129,    -1,   131,   132,    -1,   134,    -1,    -1,   137,
      -1,    -1,    -1,   141,    -1,   143,     1,   145,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      -1,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      75,    76,    -1,    78,    -1,    80,    81,    82,    -1,    84,
      85,    86,    87,    -1,    -1,    90,    -1,    92,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,   129,    -1,   131,   132,    -1,   134,
      -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,     1,
     145,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    57,    -1,    59,    60,    61,
      62,    63,    64,    -1,    66,    67,    68,    -1,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    -1,    80,    81,
      82,    -1,    84,    85,    86,    87,    -1,    -1,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
     112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,
     132,    -1,   134,    -1,    -1,   137,    -1,    -1,    -1,   141,
      -1,   143,     1,   145,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    -1,    66,    67,    68,
      -1,    70,    71,    72,    73,    74,    75,    76,    -1,    78,
      -1,    80,    81,    82,    -1,    84,    85,    86,    87,    -1,
      -1,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     129,    -1,   131,   132,    -1,   134,    -1,    -1,   137,    -1,
      -1,    -1,   141,    -1,   143,     1,   145,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    -1,
      26,    27,    -1,    29,    -1,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,    55,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    -1,    80,    81,    82,    -1,    84,    85,
      86,    87,    -1,    -1,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,   129,    -1,   131,   132,    -1,   134,    -1,
      -1,   137,    -1,    -1,    -1,   141,    -1,   143,     1,   145,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    -1,    17,    18,    19,    20,    21,    22,
      23,    24,    -1,    26,    27,    -1,    29,    -1,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    -1,    51,    -1,
      -1,    -1,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    75,    76,    -1,    78,    -1,    80,    81,    82,
      -1,    84,    85,    86,    87,    -1,    -1,    90,    -1,    92,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,
      -1,   134,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,
     143,     1,   145,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    57,    -1,    59,
      60,    61,    62,    63,    64,    -1,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    75,    76,    -1,    78,    -1,
      80,    81,    82,    -1,    84,    85,    86,    87,    -1,    -1,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,
      -1,   131,   132,    -1,   134,    -1,    -1,   137,    -1,    -1,
      -1,   141,    -1,   143,     1,   145,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,
      17,    18,    19,    20,    21,    22,    23,    24,    -1,    26,
      27,    -1,    29,    -1,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    -1,    51,    -1,    -1,    -1,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    -1,    80,    81,    82,    -1,    84,    85,    86,
      87,    -1,    -1,    90,    -1,    92,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,   129,    -1,   131,   132,    -1,   134,    -1,    -1,
     137,    -1,    -1,    -1,   141,    -1,   143,     1,   145,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    -1,    26,    27,    -1,    29,    -1,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    -1,    51,    -1,    -1,
      -1,    55,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    -1,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    75,    76,    -1,    78,    -1,    80,    81,    82,    -1,
      84,    85,    86,    87,    -1,    -1,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,    -1,
     134,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,
       1,   145,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    -1,    66,    67,    68,    -1,    70,
      71,    72,    73,    74,    75,    76,    -1,    78,    -1,    80,
      81,    82,    -1,    84,    85,    86,    87,    -1,    -1,    90,
      -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,
     131,   132,    -1,   134,    -1,    -1,   137,    25,    -1,    -1,
     141,    -1,   143,     1,   145,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    34,    65,    36,    -1,
      38,    69,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,
      48,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    -1,   110,    -1,   112,   113,   114,    85,   116,   117,
     118,    -1,    -1,   121,    -1,    -1,    -1,    95,    -1,   127,
     128,    -1,    -1,    -1,   132,    -1,    -1,   135,   136,   137,
      -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,   129,    -1,   131,   132,   133,    -1,    -1,    -1,   137,
      -1,    -1,    -1,   141,    -1,   143,    -1,   145,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,
      -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    57,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,   129,    -1,   131,   132,   133,    -1,
      -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,    -1,
     145,   146,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,
     131,   132,   133,    -1,    -1,    -1,   137,    -1,    -1,    -1,
     141,    -1,   143,    -1,   145,   146,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      57,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,   141,    -1,   143,    -1,   145,   146,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    37,    38,    -1,    40,    -1,    -1,
      43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,
     143,    -1,   145,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,
      -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      80,    81,    -1,    83,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,
      -1,   131,   132,   133,    -1,    -1,    -1,   137,    -1,    -1,
      -1,   141,    -1,   143,    -1,   145,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      57,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    80,    81,    -1,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,   129,    -1,   131,   132,   133,    -1,    -1,    -1,
     137,    -1,    -1,    -1,   141,    -1,   143,   144,   145,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,
      44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    57,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    80,    81,    -1,    83,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,   133,
      -1,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,
     144,   145,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,    80,
      81,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,
     131,   132,    -1,   134,    -1,    -1,   137,    -1,    -1,    -1,
     141,    -1,   143,    -1,   145,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,
      38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,
      48,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    -1,    80,    81,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,   129,    -1,   131,   132,   133,    -1,    -1,    -1,   137,
      -1,    -1,    -1,   141,    -1,   143,    -1,   145,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,
      -1,    46,    -1,    48,    49,    50,    -1,    -1,    -1,    -1,
      55,    -1,    57,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,   129,    -1,   131,   132,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,    -1,
     145,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
     112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,   141,
      -1,   143,    -1,   145,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    32,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,   141,    -1,   143,    -1,   145,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    57,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,   141,    -1,   143,    -1,   145,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    57,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,
     133,    -1,    -1,    -1,   137,    -1,    -1,    -1,   141,   142,
     143,    -1,   145,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,
      -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,
      -1,   141,    -1,   143,    -1,   145,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      57,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,   129,    -1,   131,   132,   133,    -1,    -1,    -1,
     137,    -1,    -1,    -1,   141,    -1,   143,    -1,   145,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,
      44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    57,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,
      -1,   145,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,
     131,   132,   133,    -1,    -1,    -1,   137,    -1,    -1,    -1,
     141,    -1,   143,    -1,   145,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,
      38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,
      48,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,   129,    -1,   131,   132,   133,    -1,    -1,    -1,   137,
      -1,    -1,    -1,   141,    -1,   143,    -1,   145,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,
      -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    57,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,   129,    -1,   131,   132,   133,    -1,
      -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,    -1,
     145,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    55,    -1,    57,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
     112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,   141,
      -1,   143,    -1,   145,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,   141,    -1,   143,    -1,   145,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    57,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,   141,    -1,   143,    -1,   145,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    57,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,
     143,    -1,   145,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,
      -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,
      -1,   141,    -1,   143,    -1,   145,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      57,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,   141,    -1,   143,    -1,   145,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,
      44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    57,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,
      -1,   145,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,
     131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
     141,    -1,   143,    -1,   145,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,
      38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,
      48,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,   141,    -1,   143,    -1,   145,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,
      -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    57,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,   129,    -1,   131,   132,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,    -1,
     145,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    55,    -1,    57,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
     112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,   141,
      -1,   143,    -1,   145,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,   141,    -1,   143,    -1,   145,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    57,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    85,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    65,    36,    -1,    38,
      69,   127,    -1,   129,    43,    44,   132,    46,    -1,    48,
      49,   137,    -1,    -1,     3,   141,    55,   143,    57,   145,
      59,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      -1,   110,    -1,   112,   113,   114,    85,   116,   117,   118,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,   130,    -1,   132,    -1,    -1,   135,   136,   137,    -1,
      10,    -1,   111,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,   145,    96,    97,    98,
      99,   100,   101,    -1,    -1,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    65,    -1,   116,    -1,    69,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    -1,   132,    -1,    10,   135,   136,   137,   138,
      15,   140,    -1,    -1,   143,    95,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,    -1,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,    -1,    -1,   135,   136,   137,    -1,    -1,
      65,    -1,    -1,    -1,    69,    -1,   146,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,    -1,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    65,   130,   131,   132,    69,    -1,
     135,   136,   137,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,    -1,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    65,   130,
     131,   132,    69,    -1,   135,   136,   137,    10,    -1,    -1,
      -1,    -1,    15,   144,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    65,   130,   131,   132,    69,    -1,   135,   136,
     137,    -1,    -1,    -1,    77,    -1,    10,   144,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    95,    96,    97,    98,    99,   100,   101,    -1,
      -1,   104,   105,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,   132,
      -1,    65,   135,   136,   137,    69,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    77,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    95,    96,    97,    98,    99,   100,   101,    -1,    -1,
     104,   105,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,    -1,
      65,   135,   136,   137,    69,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    77,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      95,    96,    97,    98,    99,   100,   101,    -1,    -1,   104,
     105,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,    -1,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,   130,   131,   132,    -1,    65,
     135,   136,   137,    69,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    77,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    95,
      96,    97,    98,    99,   100,   101,    -1,    -1,   104,   105,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,    -1,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    -1,   130,   131,   132,    -1,    65,   135,
     136,   137,    69,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      77,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    95,    96,
      97,    98,    99,   100,   101,    -1,    -1,   104,   105,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    -1,   130,   131,   132,    -1,    65,   135,   136,
     137,    69,    -1,    -1,   141,    -1,    -1,    -1,    -1,    77,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    95,    96,    97,
      98,    99,   100,   101,    -1,    -1,   104,   105,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,    -1,    65,   135,   136,   137,
      69,    -1,    -1,   141,    -1,    -1,    -1,    -1,    77,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    95,    96,    97,    98,
      99,   100,   101,    -1,    -1,   104,   105,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,    -1,    65,   135,   136,   137,    69,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    77,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    95,    96,    97,    98,    99,
     100,   101,    -1,    -1,   104,   105,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,    -1,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,    -1,    65,   135,   136,   137,    69,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,    -1,    -1,   104,   105,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,    -1,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    10,   130,
     131,   132,    -1,    15,   135,   136,   137,    -1,    -1,    -1,
     141,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    -1,    69,    -1,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    95,   130,   131,
     132,    -1,    -1,   135,   136,   137,    -1,    -1,    -1,   141,
      -1,    -1,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    10,   130,   131,   132,    -1,    15,   135,   136,   137,
      -1,    -1,    -1,   141,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      65,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      95,   130,   131,   132,    -1,    -1,   135,   136,   137,    -1,
      -1,    -1,   141,    -1,    -1,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,    -1,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    10,   130,   131,   132,    -1,    15,
     135,   136,   137,    -1,    -1,    -1,   141,    -1,    -1,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    25,    65,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    77,   112,   113,   114,    -1,
     116,   117,   118,    -1,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    95,   130,   131,   132,    -1,    -1,   135,
     136,   137,    65,    -1,    -1,   141,    69,    -1,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
     132,    10,    95,   135,   136,   137,    15,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    25,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,   132,
      -1,    -1,   135,   136,   137,    -1,    -1,    10,   141,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    65,    -1,    -1,    -1,
      69,    -1,    25,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    65,   112,   113,   114,    69,   116,   117,   118,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,    -1,    10,   135,   136,   137,    -1,
      15,   140,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,
     113,   114,    37,   116,   117,   118,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    91,    15,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    30,    -1,    -1,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,    -1,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    52,   130,   131,   132,    10,    -1,
     135,   136,   137,    15,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    69,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    65,   112,   113,   114,    69,   116,   117,
     118,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,    -1,    10,   135,   136,   137,
      -1,    15,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    -1,    -1,    -1,    -1,    -1,   110,    -1,
     112,   113,   114,    37,   116,   117,   118,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
     132,    -1,    10,   135,   136,   137,    -1,    15,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    69,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    95,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,   110,    65,   112,   113,
     114,    69,   116,   117,   118,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,    -1,
      -1,   135,   136,   137,    -1,    -1,    10,    95,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    65,    -1,    -1,    -1,    69,
      -1,    25,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,    95,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    65,   112,   113,   114,    69,   116,   117,   118,    -1,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,    10,   134,   135,   136,   137,    15,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,    10,
     134,   135,   136,   137,    15,    -1,    -1,    -1,    65,    -1,
      -1,    -1,    69,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    95,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,   110,    65,   112,   113,   114,    69,   116,
     117,   118,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    -1,    69,    -1,    -1,   110,
      -1,   112,   113,   114,    77,   116,   117,   118,    -1,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    10,   130,
     131,   132,    95,    15,   135,   136,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    10,   130,   131,   132,
      -1,    15,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    65,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    69,    -1,    -1,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
     132,    95,    -1,   135,   136,   137,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,   110,    -1,   112,   113,
     114,    25,   116,   117,   118,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,    -1,
     134,   135,   136,   137,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    65,    28,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    25,    65,
      -1,    -1,    -1,    69,    -1,    -1,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,    95,
      -1,   135,   136,   137,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    -1,    69,    -1,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,    -1,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    -1,   130,   131,   132,    94,    95,   135,
     136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    10,   130,   131,   132,    -1,    15,   135,   136,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    95,   130,   131,   132,    -1,    10,   135,   136,   137,
      -1,    15,    -1,    -1,    -1,    -1,   110,    -1,   112,   113,
     114,    25,   116,   117,   118,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,    -1,
      10,   135,   136,   137,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    -1,    25,    65,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    77,   112,   113,
     114,    -1,   116,   117,   118,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    95,   130,   131,   132,    -1,
      -1,   135,   136,   137,    65,    -1,    -1,    -1,    69,    -1,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,    -1,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,    10,    95,   135,   136,   137,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,    -1,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,   130,
     131,   132,    -1,    -1,   135,   136,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      77,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    65,   130,   131,   132,    69,    -1,   135,   136,
     137,    10,    -1,    -1,    77,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    65,   130,   131,   132,
      69,    -1,   135,   136,   137,    10,    -1,    -1,    77,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      65,   130,   131,   132,    69,    -1,   135,   136,   137,    10,
      -1,    -1,    77,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,    -1,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    65,   130,   131,   132,    69,    -1,
     135,   136,   137,    10,    -1,    -1,    77,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,    -1,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    65,   130,
     131,   132,    69,    -1,   135,   136,   137,    10,    -1,    -1,
      77,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    65,   130,   131,   132,    69,    -1,   135,   136,
     137,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    65,   130,   131,   132,
      69,    -1,   135,   136,   137,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,   112,   113,   114,    65,   116,   117,   118,
      69,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,    -1,    -1,   135,   136,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,   130,   131,   132,    -1,    -1,   135,   136,   137
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   151,   152,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    17,    18,    19,    20,    22,    23,
      24,    26,    27,    29,    31,    32,    33,    34,    35,    36,
      38,    39,    42,    43,    44,    45,    46,    48,    49,    51,
      55,    57,    58,    59,    60,    63,    64,    67,    68,    70,
      71,    72,    73,    74,    75,    76,    78,    80,    81,    82,
      84,    85,    86,    90,    92,    95,   110,   111,   112,   113,
     118,   127,   129,   131,   132,   134,   137,   141,   143,   145,
     153,   154,   155,   156,   157,   159,   164,   165,   166,   168,
     171,   172,   173,   174,   175,   176,   178,   179,   180,   184,
     185,   188,   189,   192,   193,   196,   199,   200,   223,   226,
     227,   245,   246,   247,   248,   249,   250,   257,   258,   259,
     260,   262,   263,   264,   265,   266,   267,   269,   270,   271,
     272,   273,   134,   155,   260,    91,   251,   252,   167,   168,
     251,    12,    33,    34,    36,    76,   141,   145,   196,   245,
     249,   258,   259,   260,   262,   264,   265,    82,   167,   155,
     239,   260,   155,   143,     7,   167,   169,     9,    82,   169,
      56,    93,   177,   260,   260,    20,    32,   226,   260,   260,
     143,   168,   197,   143,   168,   228,   229,    26,   159,   170,
     260,   260,   260,   260,   260,   260,     7,   143,    32,   173,
     173,   133,   219,   238,   260,    80,    81,   134,   260,   261,
     260,   170,   260,   260,   260,    75,   143,   155,   260,   260,
     159,   166,   260,   262,   159,   166,   260,   168,   224,   260,
     238,   260,   260,   260,   260,   260,   260,   260,   260,     1,
     142,   153,   160,   238,    83,   119,   219,   240,   241,   261,
     238,   260,   268,    58,   155,    62,   158,   168,   168,    41,
      61,   214,    21,    56,    66,    87,   134,   140,    10,    15,
      25,    65,    69,    95,   110,   112,   113,   114,   116,   117,
     118,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     130,   131,   132,   135,   136,   137,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     138,   139,   143,   145,    65,    69,   143,   155,   134,   141,
     159,   260,   260,   260,   238,    37,   251,   224,   134,   115,
     134,    90,   168,   219,   242,   243,   244,   261,   224,   189,
     168,   143,   170,    26,    37,   170,    26,    37,    91,   170,
     254,    30,    52,   134,    77,    96,    97,    98,    99,   100,
     101,   104,   105,   106,   107,   108,   109,   159,   206,   242,
     155,   143,   208,    83,   143,   168,   230,   231,     1,   114,
     233,    37,   115,   155,   170,   170,   242,   169,   168,   115,
     134,   260,   260,   134,   141,   170,   143,   242,   134,   181,
     134,   181,   134,    96,   225,   134,   134,    30,    52,   170,
     134,   142,   142,   153,   115,   142,   260,   115,   144,   115,
     144,    37,   115,   146,   254,    94,   115,   146,     7,    47,
     114,   190,   141,   201,    66,   228,   228,   228,   228,   260,
     260,   260,   260,   177,   260,   177,   260,   260,   260,   260,
     260,   260,   260,    27,    82,   168,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   242,   242,
     177,   260,   177,   260,    21,    37,    40,    54,    56,    66,
      82,   212,   253,   256,   260,   273,    26,    37,    26,    37,
      77,    37,   146,   177,   260,   170,   134,   260,   260,    96,
     144,   115,   134,   168,    37,   238,    36,   260,   177,   260,
      36,   260,   177,   260,   143,   170,   137,   161,   163,   260,
     161,   162,   155,   260,    28,   260,   144,   209,   210,   211,
     212,   198,   231,   115,   144,   145,   234,   246,    96,   232,
     260,   229,   144,   219,   260,     1,   186,   242,   144,    16,
     182,   234,   246,   115,   163,   162,   142,   144,   144,   240,
     144,   240,   177,   260,   146,   155,   260,   146,   260,   146,
     260,   168,   238,   141,     1,   168,   194,   195,    21,    56,
      66,    82,   203,   213,   228,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   144,   146,    37,    66,   245,
     115,   144,    65,    65,    36,   177,   260,    36,   177,   260,
     177,   260,   251,   251,   134,   219,   261,   244,   190,   260,
     144,   260,    26,   170,    26,   170,   260,    26,   170,   254,
      26,   170,   254,   255,   256,   115,   134,    11,   134,    28,
      28,   155,    77,   159,   115,   144,   143,   168,    21,    56,
      66,    82,   215,   144,   231,   233,     1,   238,    50,   261,
     142,    53,    88,   142,   187,   144,   143,   159,   168,   183,
     224,   134,   134,   146,   254,   146,   254,   155,    94,   141,
       1,   191,   142,    96,   115,   142,    66,   202,    10,    15,
     110,   111,   112,   113,   116,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   132,   135,   136,   137,
     138,   140,   143,   168,   204,   205,   206,   245,   134,   256,
     245,   245,   260,    26,    26,   260,    26,    26,   146,   146,
     170,   170,   141,   170,    77,    36,   260,    36,   260,    77,
      36,   260,   170,    36,   260,   170,   115,   144,   260,   260,
     155,   260,   155,    28,   210,   231,   114,   237,    66,   233,
     232,   146,    37,   146,    26,   155,   238,   183,   114,   159,
      36,   155,   260,   146,    36,   155,   260,   146,   260,     1,
     142,   160,   142,    32,    60,    63,   142,   154,   172,   260,
     195,   215,   168,   260,   117,   143,   207,   207,    77,    36,
      36,    77,    36,    36,    36,    36,     1,   191,   260,   260,
     260,   260,   260,   260,   256,    11,    28,   155,   144,    27,
      55,    72,    73,    76,    85,   145,   219,   236,   246,   119,
     221,   232,    89,   222,   260,   234,   246,   155,   170,   144,
     260,   260,   155,   260,   155,   142,   142,   172,   233,   144,
     168,   205,   206,   209,   260,   260,   260,   260,   260,   260,
     142,   142,    77,    77,    77,    77,   260,   155,   237,   146,
     238,   219,   220,   260,   260,   159,   171,   218,   146,   159,
      77,    77,    79,   216,   207,   207,   144,    77,    77,    77,
      77,    77,    77,   260,   260,   260,   260,   221,   232,   219,
     235,   236,   246,    37,   146,   246,   260,   260,   222,   260,
     235,   236,   134,   217,   218,   146,   235
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
#line 443 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 448 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 449 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 456 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 461 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 462 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 481 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 482 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 483 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 484 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 485 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 486 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 487 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 488 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 489 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 490 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 491 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 492 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 493 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 494 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 495 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 496 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (6)].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[(1) - (6)].b), (yyvsp[(2) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType, (yyvsp[(6) - (7)].pblockstmt), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 506 "chapel.ypp"
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 511 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 512 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 513 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 517 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 518 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 532 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 533 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 544 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 549 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 554 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 558 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 565 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 574 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 583 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 584 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 585 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 589 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 594 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 596 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 598 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 600 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 602 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 610 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 616 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 618 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 620 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 622 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 624 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 628 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 633 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 637 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 638 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 643 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 647 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 648 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 652 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 654 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 655 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 668 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 669 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 670 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 671 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 676 "chapel.ypp"
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
#line 690 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 694 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 698 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 702 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 703 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 704 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 705 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 707 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 713 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 719 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 725 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 731 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 738 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 755 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 759 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 761 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 766 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 770 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 771 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 772 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 773 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 774 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 778 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 779 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 783 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 784 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 785 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 789 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 790 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 794 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 798 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 800 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 804 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 805 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 810 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 812 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 814 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 821 "chapel.ypp"
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
#line 831 "chapel.ypp"
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
#line 841 "chapel.ypp"
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
#line 851 "chapel.ypp"
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
#line 863 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 868 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 873 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 881 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 882 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 887 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 889 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 891 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 896 "chapel.ypp"
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
#line 912 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 919 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 928 "chapel.ypp"
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
#line 936 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 940 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 946 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 947 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 952 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 957 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 962 "chapel.ypp"
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
#line 982 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 988 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 995 "chapel.ypp"
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
#line 1003 "chapel.ypp"
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
#line 1015 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 1021 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 1028 "chapel.ypp"
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
#line 1050 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 1054 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 1059 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 1063 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1071 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1076 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1077 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1078 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1079 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1080 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1081 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1082 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1083 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1084 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1085 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1086 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1087 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1089 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1090 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1091 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1092 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1094 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1096 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1097 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1099 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1104 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1105 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1106 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1107 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1108 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1111 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1112 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1113 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1114 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1115 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1119 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1120 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1124 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1128 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1129 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1130 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1135 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1137 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1139 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1141 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1145 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1146 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1150 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1151 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1152 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1153 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1154 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1155 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1156 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1157 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1158 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1162 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1163 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1164 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1165 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1166 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1167 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1171 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1172 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1176 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1177 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1178 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1179 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1180 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1181 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1185 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1186 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1195 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1200 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1206 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1216 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1217 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1226 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1231 "chapel.ypp"
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
#line 1244 "chapel.ypp"
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
#line 1260 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1262 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1264 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1269 "chapel.ypp"
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
#line 1277 "chapel.ypp"
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
#line 1285 "chapel.ypp"
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
#line 1293 "chapel.ypp"
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
#line 1302 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1312 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1313 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1319 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1327 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1329 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1336 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1338 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1343 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1345 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1347 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1353 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1354 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1383 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1387 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1391 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1399 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1405 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1406 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1407 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1412 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1414 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1420 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1422 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1424 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1428 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1429 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1430 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1431 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1432 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1434 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1435 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1436 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1437 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1443 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1444 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1445 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1446 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1450 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1451 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1455 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1456 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1457 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1461 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1462 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1466 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1471 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1472 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1476 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1479 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1500 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_to_unmanaged", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1518 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1527 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1541 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1543 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1547 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1554 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1560 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1566 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1572 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1606 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1610 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1615 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1620 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1623 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1627 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1635 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1639 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromArgIntent((yyvsp[(1) - (2)].pt), (yyvsp[(2) - (2)].pexpr));
    }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1642 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1645 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1653 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1658 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1663 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1679 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1681 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1683 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1685 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1687 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1693 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1694 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1695 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1719 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1724 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1725 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1733 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1734 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1735 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1736 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1740 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1741 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1742 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1743 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1744 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1745 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1746 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1747 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1748 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1750 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1754 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1761 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1762 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1766 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1767 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1768 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1769 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1770 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1771 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1772 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1773 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1775 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1776 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1777 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1778 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1779 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1780 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1781 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1782 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1783 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1784 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1785 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1786 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1787 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1788 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1792 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1793 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1794 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1795 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1796 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1797 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1801 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1802 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1803 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1804 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1808 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1809 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1810 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1811 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1816 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1817 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1818 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1819 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1820 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1821 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1822 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 8336 "bison-chapel.cpp"
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


