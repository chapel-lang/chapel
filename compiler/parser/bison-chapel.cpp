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
     TFOR = 291,
     TFORALL = 292,
     TFORWARDING = 293,
     TIF = 294,
     TIMAG = 295,
     TIN = 296,
     TINDEX = 297,
     TINLINE = 298,
     TINOUT = 299,
     TINT = 300,
     TITER = 301,
     TLABEL = 302,
     TLAMBDA = 303,
     TLET = 304,
     TLOCAL = 305,
     TMINUSMINUS = 306,
     TMODULE = 307,
     TNEW = 308,
     TNIL = 309,
     TNOINIT = 310,
     TON = 311,
     TONLY = 312,
     TOTHERWISE = 313,
     TOUT = 314,
     TOVERRIDE = 315,
     TOWNED = 316,
     TPARAM = 317,
     TPLUSPLUS = 318,
     TPRAGMA = 319,
     TPRIMITIVE = 320,
     TPRIVATE = 321,
     TPROC = 322,
     TPROTOTYPE = 323,
     TPUBLIC = 324,
     TREAL = 325,
     TRECORD = 326,
     TREDUCE = 327,
     TREF = 328,
     TREQUIRE = 329,
     TRETURN = 330,
     TSCAN = 331,
     TSELECT = 332,
     TSERIAL = 333,
     TSHARED = 334,
     TSINGLE = 335,
     TSPARSE = 336,
     TSTRING = 337,
     TSUBDOMAIN = 338,
     TSYNC = 339,
     TTHEN = 340,
     TTHROW = 341,
     TTHROWS = 342,
     TTRY = 343,
     TTRYBANG = 344,
     TTYPE = 345,
     TUINT = 346,
     TUNDERSCORE = 347,
     TUNION = 348,
     TUNMANAGED = 349,
     TUSE = 350,
     TVAR = 351,
     TWHEN = 352,
     TWHERE = 353,
     TWHILE = 354,
     TWITH = 355,
     TYIELD = 356,
     TZIP = 357,
     TALIAS = 358,
     TAND = 359,
     TASSIGN = 360,
     TASSIGNBAND = 361,
     TASSIGNBOR = 362,
     TASSIGNBXOR = 363,
     TASSIGNDIVIDE = 364,
     TASSIGNEXP = 365,
     TASSIGNLAND = 366,
     TASSIGNLOR = 367,
     TASSIGNMINUS = 368,
     TASSIGNMOD = 369,
     TASSIGNMULTIPLY = 370,
     TASSIGNPLUS = 371,
     TASSIGNSL = 372,
     TASSIGNSR = 373,
     TBAND = 374,
     TBNOT = 375,
     TBOR = 376,
     TBXOR = 377,
     TCOLON = 378,
     TCOMMA = 379,
     TDIVIDE = 380,
     TDOT = 381,
     TDOTDOT = 382,
     TDOTDOTDOT = 383,
     TEQUAL = 384,
     TEXP = 385,
     TGREATER = 386,
     TGREATEREQUAL = 387,
     THASH = 388,
     TLESS = 389,
     TLESSEQUAL = 390,
     TMINUS = 391,
     TMOD = 392,
     TNOT = 393,
     TNOTEQUAL = 394,
     TOR = 395,
     TPLUS = 396,
     TQUESTION = 397,
     TSEMI = 398,
     TSHIFTLEFT = 399,
     TSHIFTRIGHT = 400,
     TSTAR = 401,
     TSWAP = 402,
     TASSIGNREDUCE = 403,
     TIO = 404,
     TLCBR = 405,
     TRCBR = 406,
     TLP = 407,
     TRP = 408,
     TLSBR = 409,
     TRSBR = 410,
     TNOELSE = 411,
     TUMINUS = 412,
     TUPLUS = 413
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
#line 458 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 465 "bison-chapel.cpp"
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
#line 544 "bison-chapel.cpp"

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
#define YYLAST   15447

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  159
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  131
/* YYNRULES -- Number of rules.  */
#define YYNRULES  554
/* YYNRULES -- Number of states.  */
#define YYNSTATES  998

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   413

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
     155,   156,   157,   158
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
     223,   225,   227,   229,   231,   233,   235,   237,   239,   241,
     243,   245,   247,   249,   251,   253,   255,   257,   259,   261,
     262,   264,   267,   269,   272,   276,   278,   280,   283,   286,
     288,   290,   292,   294,   296,   298,   302,   308,   314,   317,
     320,   326,   330,   337,   344,   349,   355,   361,   365,   369,
     376,   382,   389,   395,   402,   406,   411,   418,   426,   433,
     441,   446,   452,   457,   462,   466,   473,   479,   486,   492,
     501,   509,   512,   516,   520,   523,   526,   530,   534,   535,
     538,   541,   545,   551,   553,   557,   559,   563,   567,   573,
     579,   580,   583,   587,   590,   594,   601,   608,   617,   626,
     633,   642,   644,   646,   648,   649,   652,   653,   656,   660,
     666,   672,   674,   676,   679,   683,   685,   689,   690,   691,
     700,   701,   703,   706,   709,   711,   712,   713,   724,   728,
     732,   738,   744,   748,   750,   754,   756,   758,   761,   763,
     765,   767,   769,   771,   773,   775,   777,   779,   781,   783,
     785,   787,   789,   791,   793,   795,   797,   799,   801,   803,
     805,   807,   809,   811,   813,   815,   817,   819,   821,   823,
     825,   827,   829,   831,   833,   834,   838,   842,   843,   845,
     849,   854,   860,   865,   872,   879,   884,   885,   887,   889,
     891,   893,   895,   898,   901,   903,   905,   907,   908,   910,
     912,   915,   917,   919,   921,   923,   924,   926,   929,   931,
     933,   935,   936,   938,   940,   942,   944,   946,   949,   951,
     952,   954,   957,   960,   961,   964,   968,   973,   978,   981,
     984,   989,   990,   993,   996,  1001,  1006,  1011,  1017,  1022,
    1023,  1025,  1027,  1029,  1033,  1037,  1043,  1047,  1049,  1051,
    1055,  1057,  1060,  1064,  1065,  1068,  1071,  1075,  1078,  1083,
    1087,  1091,  1096,  1100,  1101,  1104,  1107,  1110,  1113,  1116,
    1119,  1122,  1125,  1128,  1130,  1131,  1134,  1137,  1140,  1143,
    1146,  1149,  1152,  1155,  1158,  1160,  1165,  1170,  1177,  1181,
    1182,  1184,  1186,  1190,  1195,  1199,  1204,  1211,  1212,  1215,
    1218,  1221,  1224,  1227,  1230,  1233,  1236,  1239,  1242,  1244,
    1246,  1250,  1254,  1256,  1260,  1262,  1264,  1266,  1270,  1274,
    1275,  1277,  1279,  1283,  1287,  1291,  1293,  1295,  1297,  1299,
    1301,  1303,  1305,  1307,  1310,  1315,  1320,  1326,  1329,  1332,
    1335,  1338,  1341,  1344,  1351,  1358,  1363,  1373,  1383,  1391,
    1398,  1405,  1410,  1420,  1430,  1438,  1443,  1450,  1457,  1467,
    1477,  1484,  1486,  1488,  1490,  1492,  1494,  1496,  1498,  1500,
    1504,  1505,  1507,  1512,  1514,  1518,  1523,  1525,  1529,  1534,
    1538,  1542,  1544,  1546,  1549,  1551,  1554,  1556,  1558,  1562,
    1565,  1570,  1572,  1574,  1576,  1578,  1580,  1582,  1584,  1586,
    1591,  1595,  1599,  1603,  1607,  1610,  1613,  1615,  1616,  1618,
    1621,  1624,  1626,  1628,  1630,  1632,  1634,  1636,  1638,  1643,
    1648,  1653,  1657,  1661,  1665,  1669,  1674,  1678,  1683,  1685,
    1687,  1689,  1691,  1693,  1697,  1702,  1706,  1711,  1715,  1720,
    1724,  1730,  1734,  1738,  1742,  1746,  1750,  1754,  1758,  1762,
    1766,  1770,  1774,  1778,  1782,  1786,  1790,  1794,  1798,  1802,
    1806,  1810,  1814,  1818,  1822,  1825,  1828,  1831,  1834,  1837,
    1840,  1844,  1848,  1852,  1856,  1860,  1864,  1868,  1872,  1874,
    1876,  1878,  1880,  1882,  1884
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     160,     0,    -1,   161,    -1,    -1,   161,   162,    -1,   164,
      -1,   163,   164,    -1,    64,     7,    -1,   163,    64,     7,
      -1,   165,    -1,   168,    -1,   173,    -1,   174,    -1,   184,
      -1,   175,    -1,   187,    -1,   190,    -1,   188,    -1,   197,
      -1,   191,    -1,   192,    -1,   196,    -1,   183,    -1,   264,
     143,    -1,    12,   164,    -1,    13,   265,   164,    -1,    14,
     176,   143,    -1,    20,   265,   168,    -1,    25,   176,   143,
      -1,    27,   253,   143,    -1,    47,   177,   164,    -1,    50,
     275,   182,    -1,    50,   182,    -1,    56,   275,   182,    -1,
      78,   275,   182,    -1,    78,   182,    -1,    84,   164,    -1,
     101,   275,   143,    -1,     1,   143,    -1,   166,   167,    52,
     177,   150,   151,    -1,   166,   167,    52,   177,   150,   169,
     151,    -1,   166,   167,    52,   177,   150,     1,   151,    -1,
      -1,    69,    -1,    66,    -1,    -1,    68,    -1,   150,   151,
      -1,   150,   169,   151,    -1,   150,     1,   151,    -1,   162,
      -1,   169,   162,    -1,   275,    -1,   275,    11,   275,    -1,
     170,   124,   275,    -1,   170,   124,   275,    11,   275,    -1,
      -1,   170,    -1,   146,    -1,   170,    -1,    95,   252,   143,
      -1,    95,   275,    33,   172,   143,    -1,    95,   275,    57,
     171,   143,    -1,    74,   252,   143,    -1,   278,   218,   277,
     143,    -1,   278,   147,   277,   143,    -1,   278,   148,   277,
     143,    -1,   278,   111,   277,   143,    -1,   278,   112,   277,
     143,    -1,    -1,   177,    -1,     3,    -1,   178,    -1,    15,
      -1,    45,    -1,    91,    -1,    70,    -1,    40,    -1,    22,
      -1,    82,    -1,    42,    -1,    84,    -1,    80,    -1,    61,
      -1,    79,    -1,    16,    -1,    94,    -1,    30,    -1,   179,
      -1,    -1,     7,    -1,    29,   164,    -1,   168,    -1,    75,
     143,    -1,    75,   277,   143,    -1,   143,    -1,   185,    -1,
      69,   185,    -1,    66,   185,    -1,   204,    -1,   235,    -1,
     200,    -1,   186,    -1,   212,    -1,   238,    -1,    38,   275,
     143,    -1,    38,   275,    33,   172,   143,    -1,    38,   275,
      57,   171,   143,    -1,    38,   238,    -1,    35,     9,    -1,
      29,   164,    99,   275,   143,    -1,    99,   275,   182,    -1,
      21,   275,    41,   275,   265,   182,    -1,    21,   275,    41,
     189,   265,   182,    -1,    21,   275,   265,   182,    -1,    36,
     275,    41,   275,   182,    -1,    36,   275,    41,   189,   182,
      -1,    36,   275,   182,    -1,    36,   189,   182,    -1,    36,
      62,   177,    41,   275,   182,    -1,    37,   275,    41,   275,
     182,    -1,    37,   275,    41,   275,   268,   182,    -1,    37,
     275,    41,   189,   182,    -1,    37,   275,    41,   189,   268,
     182,    -1,    37,   275,   182,    -1,    37,   275,   268,   182,
      -1,   154,   252,    41,   275,   155,   164,    -1,   154,   252,
      41,   275,   268,   155,   164,    -1,   154,   252,    41,   189,
     155,   164,    -1,   154,   252,    41,   189,   268,   155,   164,
      -1,   154,   252,   155,   164,    -1,   154,   252,   268,   155,
     164,    -1,   102,   152,   252,   153,    -1,    39,   275,    85,
     164,    -1,    39,   275,   168,    -1,    39,   275,    85,   164,
      31,   164,    -1,    39,   275,   168,    31,   164,    -1,    39,
     275,   218,   275,    85,   164,    -1,    39,   275,   218,   275,
     168,    -1,    39,   275,   218,   275,    85,   164,    31,   164,
      -1,    39,   275,   218,   275,   168,    31,   164,    -1,    26,
     164,    -1,    88,   275,   143,    -1,    89,   275,   143,    -1,
      88,   175,    -1,    89,   175,    -1,    88,   168,   193,    -1,
      89,   168,   193,    -1,    -1,   193,   194,    -1,    18,   168,
      -1,    18,   195,   168,    -1,    18,   152,   195,   153,   168,
      -1,   177,    -1,   177,   123,   275,    -1,   180,    -1,   180,
     123,   275,    -1,    86,   275,   143,    -1,    77,   275,   150,
     198,   151,    -1,    77,   275,   150,     1,   151,    -1,    -1,
     198,   199,    -1,    97,   252,   182,    -1,    58,   164,    -1,
      58,    29,   164,    -1,   201,   177,   202,   150,   203,   151,
      -1,   201,   180,   202,   150,   203,   151,    -1,    35,   181,
     201,   180,   202,   150,   203,   151,    -1,    35,   181,   201,
     177,   202,   150,   203,   151,    -1,   201,   177,   202,   150,
       1,   151,    -1,    35,   181,   201,   177,   202,   150,     1,
     151,    -1,    19,    -1,    71,    -1,    93,    -1,    -1,   123,
     252,    -1,    -1,   203,   184,    -1,   203,   163,   184,    -1,
     205,   177,   150,   206,   151,    -1,   205,   177,   150,     1,
     151,    -1,    32,    -1,   207,    -1,   206,   124,    -1,   206,
     124,   207,    -1,   177,    -1,   177,   105,   275,    -1,    -1,
      -1,    48,   209,   220,   210,   227,   247,   234,   230,    -1,
      -1,    43,    -1,    34,   276,    -1,    35,   276,    -1,    60,
      -1,    -1,    -1,   211,   226,   213,   215,   214,   227,   246,
     228,   234,   229,    -1,   225,   217,   219,    -1,   225,   218,
     219,    -1,   225,   216,   126,   217,   219,    -1,   225,   216,
     126,   218,   219,    -1,   225,     1,   219,    -1,   259,    -1,
     152,   275,   153,    -1,   177,    -1,   180,    -1,   120,   177,
      -1,   119,    -1,   121,    -1,   122,    -1,   120,    -1,   129,
      -1,   139,    -1,   135,    -1,   132,    -1,   134,    -1,   131,
      -1,   141,    -1,   136,    -1,   146,    -1,   125,    -1,   144,
      -1,   145,    -1,   137,    -1,   130,    -1,   138,    -1,    17,
      -1,   133,    -1,    10,    -1,   147,    -1,   149,    -1,   105,
      -1,   116,    -1,   113,    -1,   115,    -1,   109,    -1,   114,
      -1,   110,    -1,   106,    -1,   107,    -1,   108,    -1,   118,
      -1,   117,    -1,    -1,   152,   221,   153,    -1,   152,   221,
     153,    -1,    -1,   222,    -1,   221,   124,   222,    -1,   223,
     177,   251,   244,    -1,   163,   223,   177,   251,   244,    -1,
     223,   177,   251,   233,    -1,   223,   152,   243,   153,   251,
     244,    -1,   223,   152,   243,   153,   251,   233,    -1,   223,
     180,   251,   244,    -1,    -1,   224,    -1,    41,    -1,    44,
      -1,    59,    -1,    24,    -1,    24,    41,    -1,    24,    73,
      -1,    62,    -1,    73,    -1,    90,    -1,    -1,    62,    -1,
      73,    -1,    24,    73,    -1,    24,    -1,    90,    -1,    67,
      -1,    46,    -1,    -1,    24,    -1,    24,    73,    -1,    73,
      -1,    62,    -1,    90,    -1,    -1,    87,    -1,   143,    -1,
     230,    -1,   168,    -1,   183,    -1,   142,   177,    -1,   142,
      -1,    -1,   231,    -1,   128,   275,    -1,   128,   232,    -1,
      -1,    98,   275,    -1,    90,   236,   143,    -1,    23,    90,
     236,   143,    -1,    35,    90,   236,   143,    -1,   177,   237,
      -1,   180,   237,    -1,   177,   237,   124,   236,    -1,    -1,
     105,   260,    -1,   105,   248,    -1,   239,    62,   240,   143,
      -1,   239,    24,   240,   143,    -1,   239,    73,   240,   143,
      -1,   239,    24,    73,   240,   143,    -1,   239,    96,   240,
     143,    -1,    -1,    23,    -1,    35,    -1,   241,    -1,   240,
     124,   241,    -1,   177,   247,   244,    -1,   152,   243,   153,
     247,   244,    -1,   180,   247,   244,    -1,    92,    -1,   177,
      -1,   152,   243,   153,    -1,   242,    -1,   242,   124,    -1,
     242,   124,   243,    -1,    -1,   105,    55,    -1,   105,   277,
      -1,   154,   155,   260,    -1,   154,   155,    -1,   154,   252,
     155,   260,    -1,   154,   252,   155,    -1,   154,   155,   245,
      -1,   154,   252,   155,   245,    -1,   154,     1,   155,    -1,
      -1,   123,   260,    -1,   123,   245,    -1,   123,    30,    -1,
     123,    80,    -1,   123,    84,    -1,   123,    16,    -1,   123,
      61,    -1,   123,    94,    -1,   123,    79,    -1,     1,    -1,
      -1,   123,   260,    -1,   123,   248,    -1,   123,    30,    -1,
     123,    80,    -1,   123,    84,    -1,   123,    16,    -1,   123,
      61,    -1,   123,    94,    -1,   123,    79,    -1,     1,    -1,
     154,   252,   155,   260,    -1,   154,   252,   155,   248,    -1,
     154,   252,    41,   275,   155,   260,    -1,   154,     1,   155,
      -1,    -1,   260,    -1,   231,    -1,   154,   155,   249,    -1,
     154,   252,   155,   249,    -1,   154,   155,   250,    -1,   154,
     252,   155,   250,    -1,   154,   252,    41,   275,   155,   249,
      -1,    -1,   123,   260,    -1,   123,   231,    -1,   123,    30,
      -1,   123,    80,    -1,   123,    84,    -1,   123,    16,    -1,
     123,    61,    -1,   123,    94,    -1,   123,    79,    -1,   123,
     250,    -1,   275,    -1,   231,    -1,   252,   124,   275,    -1,
     252,   124,   231,    -1,   275,    -1,   253,   124,   275,    -1,
      92,    -1,   277,    -1,   231,    -1,   254,   124,   254,    -1,
     255,   124,   254,    -1,    -1,   257,    -1,   258,    -1,   257,
     124,   258,    -1,   177,   105,   231,    -1,   177,   105,   277,
      -1,   231,    -1,   277,    -1,   177,    -1,   263,    -1,   278,
      -1,   262,    -1,   286,    -1,   285,    -1,    80,   275,    -1,
      30,   152,   256,   153,    -1,    83,   152,   256,   153,    -1,
      81,    83,   152,   256,   153,    -1,    12,   275,    -1,    84,
     275,    -1,    16,   275,    -1,    61,   275,    -1,    94,   275,
      -1,    79,   275,    -1,    36,   275,    41,   275,    29,   275,
      -1,    36,   275,    41,   189,    29,   275,    -1,    36,   275,
      29,   275,    -1,    36,   275,    41,   275,    29,    39,   275,
      85,   275,    -1,    36,   275,    41,   189,    29,    39,   275,
      85,   275,    -1,    36,   275,    29,    39,   275,    85,   275,
      -1,    37,   275,    41,   275,    29,   275,    -1,    37,   275,
      41,   189,    29,   275,    -1,    37,   275,    29,   275,    -1,
      37,   275,    41,   275,    29,    39,   275,    85,   275,    -1,
      37,   275,    41,   189,    29,    39,   275,    85,   275,    -1,
      37,   275,    29,    39,   275,    85,   275,    -1,   154,   252,
     155,   275,    -1,   154,   252,    41,   275,   155,   275,    -1,
     154,   252,    41,   189,   155,   275,    -1,   154,   252,    41,
     275,   155,    39,   275,    85,   275,    -1,   154,   252,    41,
     189,   155,    39,   275,    85,   275,    -1,    39,   275,    85,
     275,    31,   275,    -1,    54,    -1,   263,    -1,   259,    -1,
     281,    -1,   280,    -1,   208,    -1,   273,    -1,   274,    -1,
     272,   149,   275,    -1,    -1,   266,    -1,   100,   152,   267,
     153,    -1,   270,    -1,   267,   124,   270,    -1,   100,   152,
     269,   153,    -1,   270,    -1,   269,   124,   270,    -1,   271,
     259,   247,   244,    -1,   289,    72,   259,    -1,   275,    72,
     259,    -1,    24,    -1,    41,    -1,    24,    41,    -1,    73,
      -1,    24,    73,    -1,    96,    -1,   278,    -1,   272,   149,
     275,    -1,    53,   275,    -1,    49,   240,    41,   275,    -1,
     283,    -1,   260,    -1,   261,    -1,   287,    -1,   288,    -1,
     208,    -1,   273,    -1,   274,    -1,   152,   128,   275,   153,
      -1,   275,   123,   275,    -1,   275,   123,    16,    -1,   275,
     123,    94,    -1,   275,   127,   275,    -1,   275,   127,    -1,
     127,   275,    -1,   127,    -1,    -1,   275,    -1,    88,   275,
      -1,    89,   275,    -1,   275,    -1,   259,    -1,   280,    -1,
     281,    -1,   282,    -1,   278,    -1,   208,    -1,   279,   152,
     256,   153,    -1,   279,   154,   256,   155,    -1,    65,   152,
     256,   153,    -1,   275,   126,   177,    -1,   275,   126,    90,
      -1,   275,   126,    30,    -1,   152,   254,   153,    -1,   152,
     254,   124,   153,    -1,   152,   255,   153,    -1,   152,   255,
     124,   153,    -1,     4,    -1,     5,    -1,     6,    -1,     7,
      -1,     8,    -1,   150,   252,   151,    -1,   150,   252,   124,
     151,    -1,   154,   252,   155,    -1,   154,   252,   124,   155,
      -1,   154,   284,   155,    -1,   154,   284,   124,   155,    -1,
     275,   103,   275,    -1,   284,   124,   275,   103,   275,    -1,
     275,   141,   275,    -1,   275,   136,   275,    -1,   275,   146,
     275,    -1,   275,   125,   275,    -1,   275,   144,   275,    -1,
     275,   145,   275,    -1,   275,   137,   275,    -1,   275,   129,
     275,    -1,   275,   139,   275,    -1,   275,   135,   275,    -1,
     275,   132,   275,    -1,   275,   134,   275,    -1,   275,   131,
     275,    -1,   275,   119,   275,    -1,   275,   121,   275,    -1,
     275,   122,   275,    -1,   275,   104,   275,    -1,   275,   140,
     275,    -1,   275,   130,   275,    -1,   275,    17,   275,    -1,
     275,    10,   275,    -1,   275,   133,   275,    -1,   275,    28,
     275,    -1,   141,   275,    -1,   136,   275,    -1,    51,   275,
      -1,    63,   275,    -1,   138,   275,    -1,   120,   275,    -1,
     275,    72,   275,    -1,   275,    72,   189,    -1,   289,    72,
     275,    -1,   289,    72,   189,    -1,   275,    76,   275,    -1,
     275,    76,   189,    -1,   289,    76,   275,    -1,   289,    76,
     189,    -1,   141,    -1,   146,    -1,   104,    -1,   140,    -1,
     119,    -1,   121,    -1,   122,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   449,   449,   454,   455,   461,   462,   467,   468,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   506,
     509,   511,   517,   518,   519,   523,   524,   537,   538,   539,
     544,   545,   550,   555,   560,   564,   571,   576,   580,   585,
     589,   590,   591,   595,   599,   601,   603,   605,   607,   612,
     613,   617,   618,   630,   631,   632,   633,   634,   635,   636,
     637,   641,   642,   643,   644,   645,   646,   647,   658,   665,
     666,   670,   671,   675,   676,   680,   681,   682,   683,   684,
     685,   686,   687,   691,   692,   696,   697,   698,   699,   703,
     710,   711,   712,   713,   714,   715,   716,   717,   718,   719,
     720,   721,   722,   723,   724,   725,   726,   732,   738,   744,
     750,   757,   767,   771,   772,   773,   774,   775,   777,   779,
     781,   786,   789,   790,   791,   792,   793,   794,   798,   799,
     803,   804,   805,   809,   810,   811,   812,   816,   819,   821,
     826,   827,   831,   833,   835,   842,   852,   862,   872,   882,
     892,   905,   910,   915,   923,   924,   929,   930,   932,   937,
     953,   960,   969,   977,   981,   988,   989,   994,   999,   993,
    1024,  1030,  1037,  1043,  1049,  1060,  1066,  1059,  1098,  1102,
    1107,  1111,  1116,  1123,  1124,  1128,  1129,  1130,  1131,  1132,
    1133,  1134,  1135,  1136,  1137,  1138,  1139,  1140,  1141,  1142,
    1143,  1144,  1145,  1146,  1147,  1148,  1149,  1150,  1151,  1152,
    1153,  1154,  1158,  1159,  1160,  1161,  1162,  1163,  1164,  1165,
    1166,  1167,  1168,  1169,  1173,  1174,  1178,  1182,  1183,  1184,
    1188,  1190,  1192,  1194,  1196,  1198,  1203,  1204,  1208,  1209,
    1210,  1211,  1212,  1213,  1214,  1215,  1216,  1220,  1221,  1222,
    1223,  1224,  1225,  1229,  1230,  1234,  1235,  1236,  1237,  1238,
    1239,  1243,  1244,  1247,  1248,  1252,  1253,  1257,  1259,  1264,
    1265,  1269,  1270,  1274,  1275,  1279,  1281,  1283,  1288,  1301,
    1315,  1332,  1333,  1335,  1340,  1348,  1356,  1364,  1373,  1383,
    1384,  1385,  1389,  1390,  1398,  1400,  1402,  1407,  1409,  1411,
    1416,  1418,  1420,  1427,  1428,  1429,  1434,  1436,  1438,  1442,
    1446,  1448,  1452,  1460,  1461,  1462,  1463,  1464,  1465,  1466,
    1467,  1468,  1469,  1470,  1475,  1476,  1477,  1478,  1479,  1480,
    1481,  1482,  1483,  1484,  1485,  1505,  1509,  1513,  1521,  1528,
    1529,  1530,  1534,  1536,  1542,  1544,  1546,  1551,  1552,  1553,
    1554,  1555,  1556,  1557,  1558,  1559,  1560,  1561,  1567,  1568,
    1569,  1570,  1574,  1575,  1579,  1580,  1581,  1585,  1586,  1590,
    1591,  1595,  1596,  1600,  1601,  1602,  1603,  1607,  1618,  1619,
    1620,  1621,  1622,  1623,  1627,  1629,  1631,  1633,  1635,  1637,
    1639,  1641,  1643,  1648,  1650,  1652,  1654,  1656,  1658,  1660,
    1662,  1664,  1666,  1668,  1670,  1672,  1679,  1685,  1691,  1697,
    1706,  1716,  1724,  1725,  1726,  1727,  1728,  1729,  1730,  1731,
    1736,  1737,  1741,  1745,  1746,  1750,  1754,  1755,  1759,  1763,
    1767,  1774,  1775,  1776,  1777,  1778,  1779,  1783,  1784,  1789,
    1794,  1802,  1803,  1804,  1805,  1806,  1807,  1808,  1809,  1810,
    1812,  1814,  1816,  1818,  1820,  1822,  1824,  1829,  1830,  1833,
    1834,  1835,  1838,  1839,  1840,  1841,  1852,  1853,  1857,  1858,
    1859,  1863,  1864,  1865,  1873,  1874,  1875,  1876,  1880,  1881,
    1882,  1883,  1884,  1885,  1886,  1887,  1888,  1889,  1893,  1901,
    1902,  1906,  1907,  1908,  1909,  1910,  1911,  1912,  1913,  1914,
    1915,  1916,  1917,  1918,  1919,  1920,  1921,  1922,  1923,  1924,
    1925,  1926,  1927,  1928,  1932,  1933,  1934,  1935,  1936,  1937,
    1941,  1942,  1943,  1944,  1948,  1949,  1950,  1951,  1956,  1957,
    1958,  1959,  1960,  1961,  1962
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
  "TELSE", "TENUM", "TEXCEPT", "TEXPORT", "TEXTERN", "TFOR", "TFORALL",
  "TFORWARDING", "TIF", "TIMAG", "TIN", "TINDEX", "TINLINE", "TINOUT",
  "TINT", "TITER", "TLABEL", "TLAMBDA", "TLET", "TLOCAL", "TMINUSMINUS",
  "TMODULE", "TNEW", "TNIL", "TNOINIT", "TON", "TONLY", "TOTHERWISE",
  "TOUT", "TOVERRIDE", "TOWNED", "TPARAM", "TPLUSPLUS", "TPRAGMA",
  "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC", "TREAL",
  "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT",
  "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE", "TSTRING", "TSUBDOMAIN",
  "TSYNC", "TTHEN", "TTHROW", "TTHROWS", "TTRY", "TTRYBANG", "TTYPE",
  "TUINT", "TUNDERSCORE", "TUNION", "TUNMANAGED", "TUSE", "TVAR", "TWHEN",
  "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND",
  "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
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
  "internal_type_ident", "reserved_type_ident",
  "reserved_type_error_ident", "opt_string", "do_stmt", "return_stmt",
  "class_level_stmt", "private_decl", "forwarding_stmt",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "defer_stmt", "try_stmt", "catch_stmt_ls", "catch_stmt", "catch_expr",
  "throw_stmt", "select_stmt", "when_stmt_ls", "when_stmt",
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
     405,   406,   407,   408,   409,   410,   411,   412,   413
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   159,   160,   161,   161,   162,   162,   163,   163,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   165,
     165,   165,   166,   166,   166,   167,   167,   168,   168,   168,
     169,   169,   170,   170,   170,   170,   171,   171,   172,   172,
     173,   173,   173,   174,   175,   175,   175,   175,   175,   176,
     176,   177,   177,   178,   178,   178,   178,   178,   178,   178,
     178,   179,   179,   179,   179,   179,   179,   179,   180,   181,
     181,   182,   182,   183,   183,   184,   184,   184,   184,   184,
     184,   184,   184,   185,   185,   186,   186,   186,   186,   187,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   189,   190,   190,   190,   190,   190,   190,   190,
     190,   191,   192,   192,   192,   192,   192,   192,   193,   193,
     194,   194,   194,   195,   195,   195,   195,   196,   197,   197,
     198,   198,   199,   199,   199,   200,   200,   200,   200,   200,
     200,   201,   201,   201,   202,   202,   203,   203,   203,   204,
     204,   205,   206,   206,   206,   207,   207,   209,   210,   208,
     211,   211,   211,   211,   211,   213,   214,   212,   215,   215,
     215,   215,   215,   216,   216,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   219,   219,   220,   221,   221,   221,
     222,   222,   222,   222,   222,   222,   223,   223,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   225,   225,   225,
     225,   225,   225,   226,   226,   227,   227,   227,   227,   227,
     227,   228,   228,   229,   229,   230,   230,   231,   231,   232,
     232,   233,   233,   234,   234,   235,   235,   235,   236,   236,
     236,   237,   237,   237,   238,   238,   238,   238,   238,   239,
     239,   239,   240,   240,   241,   241,   241,   242,   242,   242,
     243,   243,   243,   244,   244,   244,   245,   245,   245,   245,
     245,   245,   245,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   248,   248,   248,   248,   249,
     249,   249,   250,   250,   250,   250,   250,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   252,   252,
     252,   252,   253,   253,   254,   254,   254,   255,   255,   256,
     256,   257,   257,   258,   258,   258,   258,   259,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     262,   263,   264,   264,   264,   264,   264,   264,   264,   264,
     265,   265,   266,   267,   267,   268,   269,   269,   270,   270,
     270,   271,   271,   271,   271,   271,   271,   272,   272,   273,
     274,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   276,   276,   277,
     277,   277,   278,   278,   278,   278,   279,   279,   280,   280,
     280,   281,   281,   281,   282,   282,   282,   282,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   284,
     284,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   286,   286,   286,   286,   286,   286,
     287,   287,   287,   287,   288,   288,   288,   288,   289,   289,
     289,   289,   289,   289,   289
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
       3,     5,     5,     3,     4,     4,     4,     4,     4,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     2,     1,     2,     3,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     3,     5,     5,     2,     2,
       5,     3,     6,     6,     4,     5,     5,     3,     3,     6,
       5,     6,     5,     6,     3,     4,     6,     7,     6,     7,
       4,     5,     4,     4,     3,     6,     5,     6,     5,     8,
       7,     2,     3,     3,     2,     2,     3,     3,     0,     2,
       2,     3,     5,     1,     3,     1,     3,     3,     5,     5,
       0,     2,     3,     2,     3,     6,     6,     8,     8,     6,
       8,     1,     1,     1,     0,     2,     0,     2,     3,     5,
       5,     1,     1,     2,     3,     1,     3,     0,     0,     8,
       0,     1,     2,     2,     1,     0,     0,    10,     3,     3,
       5,     5,     3,     1,     3,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     3,     3,     0,     1,     3,
       4,     5,     4,     6,     6,     4,     0,     1,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     0,     1,     1,
       2,     1,     1,     1,     1,     0,     1,     2,     1,     1,
       1,     0,     1,     1,     1,     1,     1,     2,     1,     0,
       1,     2,     2,     0,     2,     3,     4,     4,     2,     2,
       4,     0,     2,     2,     4,     4,     4,     5,     4,     0,
       1,     1,     1,     3,     3,     5,     3,     1,     1,     3,
       1,     2,     3,     0,     2,     2,     3,     2,     4,     3,
       3,     4,     3,     0,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     1,     0,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     4,     4,     6,     3,     0,
       1,     1,     3,     4,     3,     4,     6,     0,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     1,
       3,     3,     1,     3,     1,     1,     1,     3,     3,     0,
       1,     1,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     4,     4,     5,     2,     2,     2,
       2,     2,     2,     6,     6,     4,     9,     9,     7,     6,
       6,     4,     9,     9,     7,     4,     6,     6,     9,     9,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     1,     4,     1,     3,     4,     1,     3,     4,     3,
       3,     1,     1,     2,     1,     2,     1,     1,     3,     2,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       3,     3,     3,     3,     2,     2,     1,     0,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       4,     3,     3,     3,     3,     4,     3,     4,     1,     1,
       1,     1,     1,     3,     4,     3,     4,     3,     4,     3,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    71,   498,   499,   500,   501,
     502,     0,   440,    69,    73,     0,   171,   440,     0,    78,
     310,    69,     0,     0,     0,     0,   181,   477,   311,     0,
       0,   309,     0,    77,    80,   191,    74,     0,   187,     0,
       0,     0,     0,   431,     0,   194,     0,     0,     0,     0,
     309,   309,    76,   172,     0,     0,     0,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,    75,   173,
       0,     0,     0,     0,   550,   552,     0,   553,   554,   476,
       0,     0,   551,   548,    95,   549,     0,     0,     0,     4,
       0,     5,     9,    45,    10,    11,    12,    14,   397,    72,
      22,    13,    96,   102,    15,    17,    16,    19,    20,    21,
      18,   101,     0,    99,     0,   466,     0,   103,   100,   104,
       0,   482,   462,   463,   400,   398,     0,     0,   467,   468,
       0,   399,     0,   483,   484,   485,   461,   402,   401,   464,
     465,     0,    38,    24,   407,     0,     0,   441,     0,    70,
       0,     0,     0,     0,     0,     0,     0,   466,   482,   398,
     467,   468,   409,   399,   483,   484,     0,   440,     0,     0,
     141,     0,   382,     0,   389,   478,   192,   501,   109,     0,
       0,   193,     0,     0,     0,     0,     0,   310,   311,   108,
       0,     0,     0,     0,    85,    87,    83,    84,    82,    81,
      86,     0,     0,    88,     0,     0,   312,     0,    92,    32,
       0,   536,   459,     0,   410,   537,     7,   389,   311,    98,
      97,   288,   379,     0,   378,     0,     0,    93,   481,     0,
       0,    35,     0,   412,   403,     0,   389,    36,   408,     0,
     148,   144,     0,   399,   148,   145,     0,   301,   301,     0,
     411,     0,   378,     0,     0,   539,   475,   535,   538,   534,
       0,    47,    50,     0,     0,   384,     0,   386,     0,     0,
     385,     0,   378,     0,     0,     6,    46,     0,   174,   174,
       0,   274,   273,   195,     0,     0,     0,     0,    23,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   474,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   232,   239,
     240,   241,   236,   238,     0,     0,   234,   237,   235,   233,
     243,   242,     0,     0,     0,   389,   389,     0,     0,     0,
      25,    26,     0,     0,     0,     0,     0,    27,     0,     0,
       0,    28,     0,    29,     0,   397,   395,     0,   390,   391,
     396,     0,     0,     0,     0,   118,     0,     0,   117,     0,
       0,     0,   124,     0,     0,    56,   105,     0,   134,     0,
      30,   256,   188,   317,     0,   318,   320,     0,   354,     0,
     323,   323,     0,     0,    91,    31,    33,     0,   287,     0,
      63,   479,   480,    94,     0,    34,   389,     0,   157,   146,
     142,   147,   143,     0,   298,   299,   295,    60,     0,    56,
     111,    37,    49,    48,    51,     0,   503,     0,     0,   494,
       0,   496,     0,     0,     0,     0,     0,     0,   507,     8,
       0,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,   439,   531,   530,   533,   541,   540,   545,   544,   527,
     524,   525,   526,   471,   472,   470,   514,   493,   492,   491,
     473,   518,   529,   523,   521,   532,   522,   520,   512,   517,
     519,   528,   511,   515,   516,   513,     0,     0,     0,     0,
       0,     0,     0,   543,   542,   547,   546,   451,   452,   454,
     456,     0,   443,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   505,   440,   440,   114,   296,   383,     0,     0,
     404,     0,   297,   174,   174,     0,     0,     0,   415,     0,
       0,     0,   421,     0,     0,     0,   125,   549,    59,     0,
      52,    57,     0,   133,     0,     0,     0,   261,   258,   259,
     260,   264,   265,   266,   256,     0,   248,     0,   257,   275,
       0,   321,     0,   350,   347,   351,   353,   348,   349,   352,
       0,   346,   462,     0,   314,   316,   460,   313,   490,   381,
     380,     0,     0,     0,   405,     0,   149,   303,   462,     0,
       0,     0,   504,   469,   495,   387,   497,   388,     0,     0,
     506,   130,   425,     0,   509,   508,     0,     0,   175,     0,
     176,     0,   185,     0,   182,   271,   268,   269,   272,   196,
       0,     0,   305,   304,   306,   308,    67,    68,    65,    66,
      64,   488,   489,   453,   455,     0,   442,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,   393,   394,   392,     0,     0,     0,   132,     0,     0,
     116,     0,   115,     0,     0,   122,     0,     0,   120,     0,
       0,   446,     0,   106,     0,   107,     0,     0,   136,     0,
     138,   262,   263,     0,   256,   246,     0,   367,   367,   276,
     279,   278,   280,     0,   319,   322,   323,     0,     0,   324,
     325,   159,     0,     0,   158,   161,   406,     0,   150,   153,
     155,     0,   300,    61,    62,     0,     0,     0,     0,   131,
       0,     0,     0,   309,   309,   180,     0,   183,   179,   270,
     275,   244,   229,   227,   208,   211,   209,   210,   221,   212,
     225,   217,   215,   228,   216,   214,   219,   224,   226,   213,
     218,   222,   223,   220,   230,   231,     0,   205,   206,     0,
     244,   244,   203,   307,   444,   323,   482,   482,     0,     0,
       0,     0,     0,     0,     0,     0,   113,   112,     0,   176,
     119,     0,     0,   414,     0,   413,     0,     0,   420,   123,
       0,   419,   121,     0,   445,    54,    53,   135,   430,   137,
       0,   367,   249,     0,     0,   323,   323,   277,   293,   315,
     358,     0,   505,     0,   163,     0,     0,     0,     0,   151,
       0,   128,   427,     0,     0,   126,   426,     0,   510,     0,
      39,     0,   169,   311,   309,   309,   165,   309,   177,   166,
     186,   184,     0,   256,   202,   207,     0,     0,   198,   199,
     448,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     309,   309,   418,     0,     0,   424,     0,     0,   447,     0,
       0,   140,   323,   367,   373,   370,   374,   376,   371,   372,
     375,     0,   369,   377,   462,   289,   252,   250,   255,     0,
       0,     0,   356,   462,   164,   162,     0,   154,   156,     0,
     129,     0,   127,    41,    40,   178,   343,     0,   281,     0,
     204,   205,   244,   244,     0,     0,     0,     0,     0,     0,
     170,   168,   167,     0,     0,     0,     0,    55,   139,   251,
     323,   359,     0,   290,   292,   291,   294,   285,   286,   189,
       0,   152,     0,     0,   339,   336,   340,   342,   337,   338,
     341,     0,   335,   462,   282,   293,   245,   200,   201,     0,
       0,     0,     0,     0,     0,   417,   416,   423,   422,   254,
     253,   361,   362,   364,   462,     0,   505,   462,   429,   428,
       0,   327,     0,     0,     0,   363,   365,   332,   330,   462,
     505,   283,   197,   284,   359,   331,   462,   366
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   262,    90,   543,    92,    93,   277,    94,
     263,   538,   542,   539,    95,    96,    97,   148,    98,    99,
     203,   204,   180,   209,   100,   101,   102,   103,   104,   105,
     646,   106,   107,   108,   409,   586,   711,   109,   110,   582,
     705,   111,   112,   442,   723,   113,   114,   613,   614,   157,
     193,   559,   116,   117,   445,   730,   619,   759,   760,   379,
     844,   382,   555,   556,   557,   558,   620,   283,   693,   955,
     992,   939,   222,   934,   886,   890,   118,   249,   414,   119,
     120,   205,   206,   386,   387,   574,   952,   908,   390,   571,
     972,   883,   805,   264,   171,   268,   269,   357,   358,   359,
     158,   122,   123,   124,   159,   126,   146,   147,   501,   373,
     670,   502,   503,   127,   160,   161,   130,   181,   360,   163,
     132,   164,   165,   135,   136,   273,   137,   138,   139,   140,
     141
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -888
static const yytype_int16 yypact[] =
{
    -888,    97,  2860,  -888,   -39,  -888,  -888,  -888,  -888,  -888,
    -888,  4231,    14,   599,  -888,  9286,  -888,    14,  9286,  -888,
      39,   599,  4231,  9286,  4231,    20,  -888,  9286,  6362,  7739,
    9286,  7891,  9286,  -888,  -888,  -888,  -888,   599,  -888,  2115,
    8424,  9286,  9286,  -888,  9286,  -888,  9286,  9286,   174,    86,
     743,  1176,  -888,  -888,  8551,  7308,  9286,  8424,  9286,  9286,
     183,  -888,   124,  4231,  9286,  9413,  9413,  2525,  -888,  -888,
    9286,  8551,  9286,  9286,  -888,  -888,  9286,  -888,  -888,  1442,
    9286,  9286,  -888,  9286,  -888,  -888,  3319,  6725,  8551,  -888,
    4079,  -888,  -888,   130,  -888,  -888,  -888,  -888,  -888,  -888,
    -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
    -888,  -888,  2525,  -888,   599,   302,    46,  -888,  -888,  -888,
     194,   193,  -888,  -888,  -888,   197,   202,   201,   203,   214,
   15086, 11412,   -43,   224,   230,  -888,  -888,  -888,  -888,  -888,
    -888,   177,  -888,  -888, 15086,   199,  4231,  -888,   242,  -888,
    9286,  9286,  9286,  9286,  9286,  8551,  8551,   277,  -888,  -888,
    -888,  -888,   147,   312,  -888,  -888,   248, 13511,  2525,   256,
    -888,   109, 15086,   310,  7460, 15086,  -888,    83,  -888,  2525,
     149,  -888,   599,   260,    21, 12946, 12869,  -888,  -888,  -888,
   13459, 12230,  4231,   261,  -888,  -888,  -888,  -888,  -888,  -888,
    -888,   113,   111,  -888,   111,    43,  -888,  4231,  -888,  -888,
   13094,   891,   147, 13094,   147,   891,  -888,  7460,  8018,  -888,
    -888,   599,  -888,   121, 15086,  9286,  9286,  -888, 15086,   275,
   13307,  -888, 13094,   147, 15086,   280,  7460,  -888, 15086, 13728,
    -888,  -888, 13814, 15293,  -888,  -888, 13866,   341,   341,   306,
     147,   134, 13596, 13094, 13946,   348, 11712,   891,   348,   891,
     231,  -888,  -888,  3471,   -31,  -888,  9286,  -888,    88,   107,
    -888,     1, 13998,   -28,   450,  -888,  -888,   416,   347,   347,
     326,  -888,  -888,  -888,  1107,  2115,  2115,  2115,  -888,  9286,
    9286,  9286,  9286,  8703,  8703,  9286,  9286,  9286,  9286,  9540,
    9286,  1160,  1442,  9286,  9286,  9286,  9286,  9286,  9286,  9286,
    9286,  9286,  9286,  9286,  9286,  9286,  9286,  9286,  -888,  -888,
    -888,  -888,  -888,  -888,  8145,  8145,  -888,  -888,  -888,  -888,
    -888,  -888,  8145,  8145,  8145,  7460,  7460,  8703,  8703,  7181,
    -888,  -888, 13648, 13780, 14078,    22,  3623,  -888,  8703,    21,
     346,  -888,  9286,  -888,  9286,   376,  -888,   340,   370,  -888,
    -888,   361,  2525,   466,  8551,  -888,  4383,  8703,  -888,  4535,
    8703,   363,  -888,    21,  9667,  9286,  -888,  4231,   482,  9286,
    -888,   581,  -888,  -888,   113,  -888,   393,   365,  -888,  9794,
     414,   414,  9286,  2115,  -888,  -888,  -888,   368,  -888,  8551,
    -888, 15086, 15086,  -888,    40,  -888,  7460,   373,  -888,   505,
    -888,   505,  -888,  9921,   403,  -888,  -888,  -888,  9667,  9286,
    -888,  -888,  -888,  -888,  -888,  8272,  -888, 12088,  6877,  -888,
    7029,  -888,  8703,  5903,  3167,   377,  9286,  6209,  -888,  -888,
     599,  8551,   378,   383,   993,   146,  2115,   191,   211,   228,
     251, 13378,  2363,  2363,   357,  -888,   357,  -888,   357, 13136,
     623,   598,   762, 11532, 11532,   147,   348,  -888,  -888,  -888,
   11712, 15240,   357,  2698,  2698,  2363,  2698,  2698,   314,   348,
   15240,  1957,   314,   891,   891,   348,   392,   394,   399,   400,
     404,   395,   381,  -888,   357,  -888,   357,    48,  -888,  -888,
    -888,   108,  -888,   599, 15125,   187, 10048,  8703, 10175,  8703,
    9286,  8703, 11716,    14, 14130,  -888,  -888, 15086, 14164,  7460,
    -888,  7460,  -888,   347,   347,  9286,   115,  9286, 15086,    53,
   13165,  9286, 15086,    10, 13017,  7181,  -888,   408,   429,   418,
   14216,   429,   420,   534, 14296,  4231, 13236,    65,  -888,  -888,
    -888,  -888,  -888,  -888,  1007,   185,  -888,  2267,  -888,   338,
     413,   113,   111,  9286,    20,  9286,  9286,  9286,  9286,  9286,
    5751,  -888,   436,  7612,  -888,  -888, 15086,  -888,  -888,  -888,
   15086,   417,     8,   419,  -888,  1885,  -888,  -888,   267,  2525,
     427,   428,  -888,  -888,  -888,  -888,  -888,  -888,    18,  2621,
    -888,  -888, 15086,  4231, 15086,  -888, 14348,   423,   453,  1237,
    -888,   430,   469,    62,  -888,   507,  -888,  -888,  -888,  -888,
    6448,   272,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
    -888,  -888,  -888,  -888,  -888,  7181,  -888,    56,  8703,  8703,
    9286,   549, 14382,  9286,   555, 14514,   432, 11875,    21,    21,
    -888,  -888,  -888,  -888,   438,   440, 13094,  -888, 12301,  4687,
    -888,  4839,  -888, 12372,  4991,  -888,    21,  5143,  -888,    21,
     186,  -888,  9286,  -888,  9286,  -888,  4231,  9286,  -888,  4231,
     561,  -888,  -888,   599,  1055,  -888,   113,   471,   471,   523,
    -888,  -888,  -888,    94,  -888,  -888,   414,   442,    23,  -888,
    -888,  -888,  5295,  8551,  -888,  -888,  -888,  2525,  -888,   476,
     478,   248,  -888,  -888,  -888,  5447,   449,  5599,   451,  -888,
    9286,  3775,   456,  1580,  2646,  -888,  9286,   599,  -888,  -888,
     338,   461,  -888,  -888,  -888,   599,  -888,  -888,  -888,  -888,
    -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
    -888,  -888,  -888,  -888,  -888,  -888,  9286,   491,  -888,   493,
     461,   461,  -888,  -888,  -888,   414,   196,   207, 14553, 10302,
   10429, 14592, 10556, 10683, 10810, 10937,  -888,  -888,  2307,  -888,
    -888,  4231,  9286, 15086,  9286, 15086,  4231,  9286, 15086,  -888,
    9286, 15086,  -888,  7181,  -888, 14692, 15086,  -888, 15086,   577,
    4231,   471,  -888,   467,  8830,   123,   414,  -888,   525,  -888,
    -888,  8703, 11624,  4231,  -888,    -2,   477,  9286,  9286,  -888,
    9286,  -888, 15086,  4231,  9286,  -888, 15086,  4231, 15086,   254,
    -888,  3927,  -888,  6598,  1116,  1116,  -888, 15254,  -888,  -888,
   15086,  -888,    87,   581,  -888,  -888, 12159, 11799,  -888,  -888,
    -888,  9286,  9286,  9286,  9286,  9286,  9286,  9286,  9286,   483,
   14518, 15244, 14296, 12443, 12514, 14296, 12585, 12656,  -888,  9286,
    4231,  -888,   414,   471,  9286,    20,  9286,  9286,  9286,  9286,
    9286,  6056,  -888,  -888,   119,  8551,  -888,  -888,  -888,  9286,
      15, 11946,  -888,   774,   310,  -888,   248, 15086, 15086, 12727,
    -888, 12798,  -888,  -888,  -888,  -888,  -888, 11064,   546,   208,
    -888,  -888,   461,   461, 14731, 14770, 14870, 14909, 14948, 15048,
    -888,  -888,  -888,  4231,  4231,  4231,  4231, 15086,  -888,  -888,
     123,  8982,    33,  -888,  -888, 15086, 15086,  -888,  -888,  -888,
   10937,  -888,  4231,  4231,  9286,    20,  9286,  9286,  9286,  9286,
    9286,  3012,  -888,    32,  -888,   525,  -888,  -888,  -888,  9286,
    9286,  9286,  9286,  9286,  9286, 14296, 14296, 14296, 14296,  -888,
    -888,  -888,  -888,  -888,   122,  8703, 11318,   952, 14296, 14296,
     481, 11191,    42,   -17, 12017,  -888,  -888,  -888,  -888,   305,
   11413,  -888,  -888,  -888,  9134,  -888,   329,  -888
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -888,  -888,  -888,    -1,  -376,  1918,  -888,  -888,  -888,   425,
     -84,  -288,   223,   229,  -888,  -888,   264,   628,  2143,  -888,
    -888,   -45,  -888,    16,  -819,  -638,   -42,  -888,  -888,  -888,
     245,  -888,  -888,  -888,   415,  -888,   -55,  -888,  -888,  -888,
    -888,  -888,   480,  -253,  -585,  -888,  -888,  -888,   -69,   966,
    -888,  -888,  -888,  -888,  -888,  -888,  -888,  -888,  -180,  -128,
    -723,  -888,  -175,    -9,   125,  -888,  -888,  -888,   -57,  -888,
    -888,  -307,   170,  -888,  -250,  -272,  -888,  -155,   445,   653,
    -888,  -206,   293,  -888,  -374,  -363,  -887,  -888,  -190,  -411,
    -877,  -840,  -677,   -48,  -888,    58,  -888,  -185,  -888,   166,
     439,    49,  -888,  -888,  1131,  -888,    17,  -888,  -888,  -215,
    -888,  -486,  -888,  -888,  1323,  1348,   -11,   661,    45,   880,
    -888,  1731,  1859,  -888,  -888,  -888,  -888,  -888,  -888,  -888,
    -324
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -506
static const yytype_int16 yytable[] =
{
     144,    89,   587,   334,   162,   554,   223,   167,   219,   220,
     560,   806,   172,   350,   391,   505,   175,   175,   185,   186,
     190,   191,   248,   251,   361,   724,   443,   207,   575,   210,
     211,   212,   397,   213,   166,   214,   215,   848,   849,   664,
     271,   581,   432,   224,   228,   230,   232,   233,   234,   671,
     207,   407,   238,   239,   242,   246,   435,   388,    55,   250,
     252,   253,   254,   511,   811,   255,   702,   279,   256,   257,
     258,   938,   259,   231,   975,   224,   228,   272,   447,   448,
     449,   450,   659,   511,   392,   838,   838,   541,   906,   633,
      55,   973,   281,   425,   988,   388,   437,     3,  -160,   985,
     229,   371,   -90,   995,   142,   703,   681,  -334,   345,   335,
     371,   336,   388,   282,   145,   334,     5,   997,   371,  -334,
     426,   634,   399,   248,   872,   433,   991,   438,    14,   168,
    -334,   541,   270,   346,   248,    19,   986,  -160,   682,   144,
     342,   343,   344,   238,   224,   272,   433,   433,   346,   764,
     491,   492,  -344,    33,   -90,    34,   434,   433,    36,   704,
     346,  -344,  -333,   228,   938,   346,   433,   393,    16,  -344,
     615,   346,   174,   715,  -333,  -334,   -90,   512,   812,   389,
    -344,   216,  -334,    52,   349,  -333,   727,   695,   976,   957,
     958,  -160,  -344,   860,   861,    61,   930,   990,   276,   905,
     365,   368,   372,   346,    68,   383,   228,   175,   616,  -344,
     907,   505,   428,   728,   401,   402,  -344,   389,   284,   617,
      53,   583,   838,   838,  -368,   228,   395,  -360,   573,   396,
    -333,   430,   635,   352,   389,  -344,   618,  -333,   217,   399,
     621,   429,    69,  -368,  -344,   399,  -360,  -368,   405,   337,
    -360,   885,   353,   338,  -344,   427,   285,   267,   399,   639,
     431,   636,   424,   338,   400,   384,   235,   286,   657,   420,
     654,   655,  -368,   301,   184,  -360,   236,   417,   451,   452,
     453,   454,   456,   458,   459,   460,   461,   462,   465,   466,
     287,   470,   471,   472,   473,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   868,   554,   684,
     793,   505,   803,   228,   228,   393,   526,   524,   666,   669,
    -450,   228,   228,   228,   228,   228,   494,   496,   504,   241,
     245,  -449,   684,   809,   622,   393,  -433,   514,   685,   794,
    -432,   517,   292,   518,   356,   288,  -437,   837,   837,  -450,
     289,   339,   393,   224,   623,   528,   530,  -438,   532,   534,
    -449,   956,   689,   540,   540,   515,   544,  -435,   546,   486,
     487,   624,   696,  -434,   142,   393,   292,   488,   489,   490,
    -326,   576,   422,   716,   718,   341,   293,   356,   580,   536,
     294,  -302,  -326,   608,   625,   228,   393,   142,   346,   351,
     690,   892,   850,  -326,  -328,   903,   356,   540,   540,   354,
    -302,   691,   364,   381,   580,   763,  -328,   228,   403,   228,
     293,   599,   580,   602,   294,   604,   606,  -328,   692,  -487,
     224,  -487,   406,   296,   712,   297,   298,   299,   572,   300,
     301,   121,   887,   888,   304,  -436,   413,   765,  -326,   416,
     121,   311,   162,   250,  -487,  -326,  -487,   439,   315,   316,
     317,   121,   588,   121,  -486,   208,  -486,   554,   440,   505,
     441,   299,  -328,   270,   301,   270,   444,  -345,   304,  -328,
     299,   519,   208,   301,   837,   837,   595,   304,   597,   516,
     240,   244,   761,   520,   521,   528,   642,   532,   645,   544,
     647,   602,   121,   808,   522,   356,   356,   525,   228,   929,
     228,  -345,   688,   545,   656,   535,   658,   561,   562,   573,
     663,   578,   698,   585,   504,   121,   584,   589,   609,   121,
     648,   649,   603,   610,  -345,   626,   632,   627,   455,   457,
     710,  -345,   628,   629,   248,   660,   662,   630,   631,   665,
     668,   -58,   162,   672,   214,   233,   234,   238,   250,   272,
    -345,   673,   228,   675,   652,   676,   694,   970,   701,   579,
     713,   714,   706,   721,   726,   758,   356,   399,   769,  -345,
     729,   725,   493,   495,   772,   121,  -345,   774,   778,  -345,
     779,   347,   800,   513,   804,   579,   807,   810,   267,   817,
     267,   818,     5,   579,   823,   547,   827,   832,   870,   208,
     208,   208,   529,   843,    14,   533,   378,  -397,   700,   847,
     873,    19,   548,   889,   504,   549,   292,   456,   494,   768,
     896,   121,   771,   954,   920,   208,   987,   831,   208,    33,
     550,    34,   591,   551,    36,    48,   121,   590,   783,   169,
     785,   292,   816,   788,   552,   815,   791,   208,   841,   411,
     362,   795,   710,   796,   776,   777,   798,   912,   909,    52,
     293,   553,   780,   842,   294,   802,   993,   598,   208,   683,
     969,    61,   789,   983,   189,   792,   577,   653,   176,   651,
      68,   356,   224,   415,     0,   293,     0,     0,     0,   294,
       0,     0,   121,     0,   822,  -247,   826,     0,     0,   828,
       0,     0,     0,     0,     0,   840,     0,   296,     0,   913,
     298,   299,     0,   300,   301,     0,     0,     0,   304,     0,
       0,     0,     0,     0,  -247,   311,     0,     0,     0,     0,
       0,     0,   315,   316,   317,   846,   299,     0,   300,   301,
       0,     0,   641,   304,   644,     0,     0,     0,   783,   785,
     311,   788,   791,   822,   826,     0,   187,   315,   316,   317,
     862,   863,     0,   864,   208,   865,   866,    27,   218,   867,
       0,     0,   504,     0,     0,   121,    35,     0,     0,  -190,
     292,     0,   219,   220,     0,   -44,     0,     0,   208,     0,
     891,   602,   758,    45,     0,   121,   897,   898,   121,   899,
    -190,   -44,     0,   901,     0,  -355,   121,     0,     0,     0,
       0,     0,   175,     0,     0,     0,     0,     0,     0,     0,
     424,   895,     0,   932,   293,     0,     0,     0,   294,     0,
     862,   914,   915,   865,   916,   917,   918,   919,     0,  -355,
       0,     0,     0,   884,     0,     0,     0,     0,   927,     0,
       0,   893,     0,   162,     0,   214,   233,   234,   238,   250,
     272,     0,  -355,   121,   935,     0,     0,     0,   936,  -355,
       0,   296,   131,   455,   493,   299,     0,   300,   301,     0,
       0,   131,   304,     0,     0,     0,     0,     0,  -355,   311,
       0,     0,   131,   982,   131,     0,   315,   316,   317,     0,
       0,     0,   965,   966,   967,   968,     0,  -355,     0,   292,
       0,     0,     0,     0,  -355,     0,     0,  -355,     0,   826,
       0,   978,   979,   162,     0,   214,   233,   234,   238,   250,
     272,     0,   637,   131,     0,   243,   243,     0,   965,   966,
     967,   968,   978,   979,   208,   208,   953,     0,   208,   208,
       0,     0,     0,   293,   984,   602,   131,   294,   115,     0,
     131,   680,     0,     0,   882,     0,     0,   115,     0,   602,
     974,     0,     0,   826,   121,     0,     0,     0,   115,   977,
     115,     0,     0,  -357,   611,     0,     5,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    14,     0,
     708,     0,     0,     0,   299,    19,   300,   301,     0,     0,
       0,   304,     0,     0,     0,   974,   131,  -357,   311,   115,
     989,   547,     0,    33,     0,    34,     0,   317,    36,   996,
       0,     0,   121,   974,     0,     0,     0,     0,   548,     0,
    -357,   549,   115,     0,     0,   933,   115,  -357,     0,   762,
       0,     0,     0,    52,     0,     0,   550,     0,     0,   551,
       0,   274,   131,   208,   208,    61,  -357,   766,   767,   547,
     552,   208,     0,   378,    68,     0,     0,   131,   378,     0,
       0,   208,     0,     0,   208,  -357,   548,   553,   121,   549,
     121,   971,  -357,   121,     0,  -357,   121,     0,     0,     0,
       5,     0,   115,     0,   550,   121,     0,   551,   121,    48,
       0,     0,    14,   194,     0,     0,     0,     0,   552,    19,
       0,     0,     0,   125,     0,     0,   819,   195,     0,   187,
       0,   121,   125,   131,     0,   553,   971,    33,     0,    34,
      27,   218,    36,   125,   121,   125,   121,     0,   115,    35,
     121,     0,  -190,     5,   971,     0,     0,     0,   196,     0,
       0,     0,     0,   115,     0,    14,    45,    52,     0,     0,
     446,     0,    19,  -190,     0,     0,   197,   198,     0,    61,
     467,   199,     0,     0,   125,     0,     0,     0,    68,   187,
      33,   200,    34,     0,     0,    36,     0,     0,     0,     0,
      27,   218,     0,     0,     0,     0,     0,   125,     0,    35,
     121,   125,  -190,     0,     0,   121,   131,     0,   -43,   115,
      52,     0,     0,     0,     0,     0,    45,     0,   722,   121,
     208,     0,    61,  -190,   -43,     0,   131,     0,     0,   131,
     468,    68,   121,     0,     0,     0,  -176,   131,     0,   201,
    -176,  -176,   121,     0,     0,     0,   121,     0,     0,  -176,
     121,  -176,  -176,     0,     0,  -176,     0,   125,     0,     0,
    -176,     0,     0,  -176,     0,     0,     0,     0,   378,   378,
       0,   378,   378,     0,     0,     0,     0,  -176,     0,  -176,
       0,  -176,     0,  -176,  -176,     0,  -176,     0,  -176,   121,
    -176,     0,   115,     0,   131,   937,     0,     0,     0,     0,
       0,   941,     0,   125,   378,   128,   378,  -176,     0,     0,
    -176,     0,   115,  -176,   128,   115,     0,     0,   125,     0,
       0,     0,     0,   115,     0,   128,     0,   128,     0,     0,
     129,     0,     0,     0,     0,     0,     0,     0,     0,   129,
       0,     0,   121,   121,   121,   121,     0,     0,     0,     0,
     129,     0,   129,     0,     0,     0,     0,     0,     0,     0,
    -176,   121,   121,     0,     0,     0,   128,     0,  -176,     0,
       0,     0,     0,     0,   125,     0,     0,     0,     0,     0,
     115,     0,     0,     0,     0,     0,     0,     0,   937,   128,
       0,   129,     0,   128,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   131,     0,     0,     0,     0,
       0,     0,     0,     0,   129,     0,     0,     0,   129,     0,
       0,     0,     0,     0,     0,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   150,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,   128,
       0,     0,    25,     0,     0,     0,     0,   125,   151,   152,
       0,   153,    33,   131,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,   129,    42,    43,   125,     0,     0,
     125,     0,     0,    46,     0,    47,     0,    49,   125,     0,
       0,   115,    52,     0,     0,   128,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   154,     0,     0,     0,
     128,     0,     0,    68,     0,     0,    70,     0,     0,   131,
     129,   131,     0,     0,   131,     0,     0,   131,     0,     0,
       0,     0,     0,     0,     0,   129,   131,     0,     0,   131,
       0,    75,    76,    77,    78,   125,     0,     0,     0,   115,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,   131,    83,     0,     0,   128,     0,    85,     0,
       0,     0,   155,     0,    87,   131,   156,   131,     0,    16,
       0,   131,     0,    20,     0,     0,     0,     0,     0,     0,
       0,   129,    26,     0,    27,   833,     0,     0,    31,     0,
       0,     0,     0,    35,     0,   115,  -190,   115,     0,     0,
     115,     0,     0,   115,     0,     0,     0,     0,     0,     0,
      45,     0,   115,     0,    48,   115,   834,  -190,     0,   835,
       0,    53,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   131,     0,     0,     0,     0,   131,     0,   115,   128,
      67,     0,     0,    69,     0,     0,   125,     0,     0,     0,
     131,   115,     0,   115,     0,     0,     0,   115,     0,   128,
       0,     0,   128,   131,   129,     0,     0,     0,     0,     0,
     128,     0,     0,   131,     0,     0,     0,   131,     0,     0,
       0,   131,     0,     0,   129,     0,     0,   129,     0,     0,
       0,     0,     0,    84,     0,   129,     0,     0,     0,     0,
       0,   836,     0,   133,   125,     0,     0,     0,     0,     0,
       0,     0,   133,     0,     0,     0,     0,   115,     0,     0,
     131,     0,   115,   133,     0,   133,     0,   128,     0,     0,
       0,     0,     0,     0,     0,     0,   115,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   115,
       0,     0,   129,     0,     0,     0,     0,     0,     0,   115,
     125,     0,   125,   115,   133,   125,     0,   115,   125,     0,
       0,     0,     0,   131,   131,   131,   131,   125,     0,     0,
     125,     0,     0,     0,     0,     0,     0,   133,     0,     0,
       0,   133,   131,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   125,     0,     0,   115,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   125,     0,   125,     0,
       0,     0,   125,     0,     0,     0,     0,     0,     0,     0,
       0,   134,     0,     0,     0,     0,     0,     0,   128,     0,
     134,     0,     0,     0,     0,     0,     0,   133,     0,     0,
       0,   134,     0,   134,     0,     0,     0,     0,     5,   115,
     115,   115,   115,   129,     0,     0,     0,     0,     0,     0,
      14,   194,     0,     0,     0,     0,     0,    19,   115,   115,
       0,     0,   125,     0,     0,   195,     0,   125,     0,     0,
      91,     0,   134,   133,     0,    33,   128,    34,     0,   143,
      36,   125,     0,     0,     0,     0,     0,     0,   133,     0,
     170,     0,   173,     0,   125,   134,   196,     0,     0,   134,
       0,   129,     0,     0,   125,    52,     0,     0,   125,     0,
       0,     0,   125,     0,   197,   198,     0,    61,     0,   199,
       0,     0,     0,     0,     0,     0,    68,     0,     0,   200,
       0,   237,   128,     0,   128,   292,     0,   128,     0,     0,
     128,     0,     0,     0,   133,     0,     0,     0,     0,   128,
       0,   125,   128,     0,    91,   134,     0,   129,   275,   129,
       0,     0,   129,     0,     0,   129,     0,     0,     0,     0,
       0,     0,     0,     0,   129,   128,     0,   129,     0,   293,
       0,     0,     0,   294,     0,   346,     0,   707,   128,     0,
     128,     0,     0,     0,   128,     0,     0,     0,     0,     0,
     129,   134,     0,     0,   125,   125,   125,   125,     0,     0,
       0,   295,     0,   129,   340,   129,   134,     0,     0,   129,
       0,     0,     0,   125,   125,     0,   296,   133,   297,   298,
     299,     0,   300,   301,   302,     0,   303,   304,   305,   306,
       0,   308,   309,   310,   311,     0,   312,   133,   314,     0,
     133,   315,   316,   317,   128,     0,     0,     0,   133,   128,
     380,     0,     0,     0,     0,     0,     0,     0,     5,     0,
       0,     0,   134,   128,     0,   394,     0,     0,     0,   129,
      14,   194,     0,     0,   129,     0,   128,    19,     0,     0,
       0,     0,     0,     0,     0,   195,   128,     0,   129,     0,
     128,     0,     0,     0,   128,    33,   149,    34,     0,     0,
      36,   129,     0,     0,   149,   133,     0,     0,     0,     0,
       0,   129,     0,     0,     0,   129,   196,     0,     0,   129,
     192,    91,   202,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   128,   197,   198,     0,    61,     0,   199,
       0,     0,     0,     0,     0,   134,    68,     0,     0,   200,
     247,     0,     0,     0,     0,     0,     0,     0,   129,     0,
       0,     0,     0,     0,     0,   134,     0,     0,   134,     0,
       0,     0,     0,     0,     0,     0,   134,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   128,   128,   128,   128,
       0,     0,     0,     0,     0,   278,     0,   280,     0,     0,
       0,     0,     0,     0,    91,   128,   128,   201,     0,     0,
       5,   129,   129,   129,   129,     0,   133,     0,     0,     0,
       0,     0,    14,   194,   394,     0,     0,   394,     0,    19,
     129,   129,     0,   134,     0,     0,     0,   195,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    33,   859,    34,
       0,   247,    36,     0,     0,     0,     0,   355,     0,     0,
       0,     0,   247,     0,     0,   363,  -176,     0,   196,     0,
    -176,  -176,     0,     0,   133,     0,     0,    52,     0,  -176,
       0,  -176,  -176,     0,   385,  -176,   197,   198,     0,    61,
    -176,   199,   601,  -176,     0,     0,     0,     0,    68,     0,
     355,   200,     0,     0,   398,     0,     0,  -176,     0,  -176,
       0,  -176,     0,  -176,  -176,     0,  -176,     0,  -176,   355,
    -176,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     133,   292,   133,     0,     0,   133,     0,  -176,   133,     0,
    -176,     0,     0,  -176,   134,     0,     0,   133,     0,     0,
     133,     0,     0,     0,     0,     0,     0,     0,     0,   686,
       0,     0,     0,     0,     0,     0,     0,   202,   202,   202,
     202,     0,     0,   133,     0,   293,     0,     0,     0,   294,
       0,     0,     0,     0,   469,     0,   133,     0,   133,     0,
    -176,     0,   133,     0,     0,     0,     0,     0,  -176,     0,
       0,     0,   134,   678,     0,     0,     0,   295,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   355,   355,
       0,     0,   296,     0,   297,   298,   299,     0,   300,   301,
     302,     0,   303,   304,   305,   306,     0,   308,   309,   310,
     311,     0,   312,   313,   314,   523,     0,   315,   316,   317,
       0,     0,   133,     0,     0,     0,     0,   133,   134,     0,
     134,   719,     0,   134,     0,     0,   134,   385,     5,     0,
       0,   133,     0,     0,     0,   134,   202,     0,   134,     0,
      14,   194,     0,     0,   133,     0,     0,    19,     0,   355,
       0,     0,     0,     0,   133,   195,     0,     0,   133,     0,
       0,   134,   133,     0,     0,    33,     0,    34,     0,     0,
      36,     0,     0,     0,   134,     0,   134,   394,     0,   394,
     134,     0,   394,   607,     0,   394,   196,   612,     0,   202,
       0,     0,     0,     0,   797,    52,     0,   799,     0,     0,
       0,   133,     0,     0,   197,   198,     0,    61,     0,   199,
       0,     0,     0,     0,     0,     0,    68,     0,     0,   200,
     814,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   290,     0,   821,     0,   825,     0,     0,   291,    91,
     134,     0,     0,     0,     0,   134,     0,     0,     0,   292,
       0,     0,     0,     0,   133,   133,   133,   133,     0,   134,
       0,     0,     0,     0,   355,    16,     0,     0,     0,    20,
       0,     0,   134,   133,   133,     0,     0,     0,    26,     0,
      27,   833,   134,     0,    31,     0,   134,     0,     0,    35,
     134,     0,  -190,   293,     0,     0,     0,   294,     0,     0,
     687,     0,     0,     0,   385,     0,    45,     0,     0,     0,
      48,     0,   834,  -190,     0,   835,     0,    53,   871,     0,
       0,   371,     0,     0,     0,   295,   292,     0,   709,   134,
       0,   894,   247,     0,     0,     0,    67,     0,     0,    69,
     296,   900,   297,   298,   299,   902,   300,   301,   302,    91,
     303,   304,   305,   306,   307,   308,   309,   310,   311,     0,
     312,   313,   314,   757,     0,   315,   316,   317,     0,     0,
     293,     0,     0,     0,   294,     0,   717,     0,     0,     0,
       0,     0,   134,   134,   134,   134,     0,     0,   928,    84,
       0,     0,     0,     0,     0,     0,     0,   839,     0,     0,
       0,   134,   134,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   296,     0,   297,
     298,   299,     0,   300,   301,   302,   801,     0,   304,   385,
       0,     0,     0,     0,   310,   311,     0,     0,     0,   314,
       0,     0,   315,   316,   317,     0,     0,     0,     0,     0,
     709,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      -2,     4,     0,     5,     6,     7,     8,     9,    10,     0,
     612,     0,    11,    12,    13,    14,    15,     0,   845,    16,
      17,    18,    19,    20,  -309,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,    32,
      33,     0,    34,    35,     0,    36,  -190,    37,    38,    39,
      40,    41,   -42,    42,    43,     0,    44,     0,     0,     0,
      45,    46,  -309,    47,    48,    49,    50,  -190,   -42,    51,
      52,    53,     0,  -309,    54,    55,     0,    56,    57,    58,
      59,    60,    61,    62,    63,     0,    64,     0,    65,    66,
      67,    68,     0,    69,    70,    71,  -309,     0,     0,    72,
       0,    73,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
     911,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,     0,    84,     0,     0,    85,     0,     0,     0,
      86,     0,    87,   980,    88,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   150,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,   151,   152,
       0,   153,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   154,     0,     0,     0,
       0,     0,     0,    68,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,   221,     0,     0,     0,    85,     0,
       0,     0,   155,     0,    87,     0,   156,   981,     4,     0,
       5,     6,     7,     8,     9,    10,     0,  -505,     0,    11,
      12,    13,    14,    15,  -505,     0,    16,    17,    18,    19,
      20,  -309,    21,    22,    23,  -505,    24,    25,  -505,    26,
       0,    27,    28,    29,    30,    31,    32,    33,     0,    34,
      35,     0,    36,  -190,    37,    38,    39,    40,    41,   -42,
      42,    43,     0,    44,     0,     0,     0,    45,    46,  -309,
      47,     0,    49,    50,  -190,   -42,    51,    52,    53,  -505,
    -309,    54,    55,  -505,    56,    57,    58,    59,    60,    61,
      62,    63,     0,    64,     0,    65,    66,    67,    68,     0,
      69,    70,    71,  -309,     0,     0,    72,     0,    73,     0,
       0,  -505,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -505,    76,  -505,  -505,
    -505,  -505,  -505,  -505,  -505,     0,  -505,  -505,  -505,  -505,
    -505,  -505,  -505,  -505,  -505,    81,  -505,  -505,  -505,     0,
      84,  -505,  -505,  -505,     0,     0,     0,    86,  -505,    87,
     260,    88,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,  -309,    21,    22,    23,     0,    24,    25,
       0,    26,     0,    27,    28,    29,    30,    31,    32,    33,
       0,    34,    35,     0,    36,  -190,    37,    38,    39,    40,
      41,   -42,    42,    43,     0,    44,     0,     0,     0,    45,
      46,  -309,    47,    48,    49,    50,  -190,   -42,    51,    52,
      53,     0,  -309,    54,    55,     0,    56,    57,    58,    59,
      60,    61,    62,    63,     0,    64,     0,    65,    66,    67,
      68,     0,    69,    70,    71,  -309,     0,     0,    72,     0,
      73,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    81,     0,    82,
      83,   221,    84,     0,     0,    85,     0,     0,     0,    86,
     261,    87,     4,    88,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,  -309,    21,    22,    23,     0,
      24,    25,     0,    26,     0,    27,    28,    29,    30,    31,
      32,    33,     0,    34,    35,     0,    36,  -190,    37,    38,
      39,    40,    41,   -42,    42,    43,     0,    44,     0,     0,
       0,    45,    46,  -309,    47,    48,    49,    50,  -190,   -42,
      51,    52,    53,     0,  -309,    54,    55,     0,    56,    57,
      58,    59,    60,    61,    62,    63,     0,    64,     0,    65,
      66,    67,    68,     0,    69,    70,    71,  -309,     0,     0,
      72,     0,    73,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     0,     0,     0,     0,     0,     0,    80,     0,    81,
       0,    82,    83,     0,    84,     0,     0,    85,     0,     0,
       0,    86,   423,    87,   260,    88,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,  -309,    21,    22,
      23,     0,    24,    25,     0,    26,     0,    27,    28,    29,
      30,    31,    32,    33,     0,    34,    35,     0,    36,  -190,
      37,    38,    39,    40,    41,   -42,    42,    43,     0,    44,
       0,     0,     0,    45,    46,  -309,    47,    48,    49,    50,
    -190,   -42,    51,    52,    53,     0,  -309,    54,    55,     0,
      56,    57,    58,    59,    60,    61,    62,    63,     0,    64,
       0,    65,    66,    67,    68,     0,    69,    70,    71,  -309,
       0,     0,    72,     0,    73,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     0,     0,     0,     0,     0,     0,    80,
       0,    81,     0,    82,    83,     0,    84,     0,     0,    85,
       0,     0,     0,    86,   261,    87,   829,    88,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,  -309,
      21,    22,    23,     0,    24,    25,     0,    26,     0,    27,
      28,    29,    30,    31,    32,    33,     0,    34,    35,     0,
      36,  -190,    37,    38,    39,    40,    41,   -42,    42,    43,
       0,    44,     0,     0,     0,    45,    46,  -309,    47,    48,
      49,    50,  -190,   -42,    51,    52,    53,     0,  -309,    54,
      55,     0,    56,    57,    58,    59,    60,    61,    62,    63,
       0,    64,     0,    65,    66,    67,    68,     0,    69,    70,
      71,  -309,     0,     0,    72,     0,    73,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,    81,     0,    82,    83,     0,    84,     0,
       0,    85,     0,     0,     0,    86,   830,    87,     4,    88,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,  -309,    21,    22,    23,     0,    24,    25,     0,    26,
       0,    27,    28,    29,    30,    31,    32,    33,     0,    34,
      35,     0,    36,  -190,    37,    38,    39,    40,    41,   -42,
      42,    43,     0,    44,     0,     0,     0,    45,    46,  -309,
      47,    48,    49,    50,  -190,   -42,    51,    52,    53,     0,
    -309,    54,    55,     0,    56,    57,    58,    59,    60,    61,
      62,    63,     0,    64,     0,    65,    66,    67,    68,     0,
      69,    70,    71,  -309,     0,     0,    72,     0,    73,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
       0,     0,     0,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,    80,     0,    81,     0,    82,    83,     0,
      84,     0,     0,    85,     0,     0,     0,    86,   904,    87,
       4,    88,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,  -309,    21,    22,    23,     0,    24,    25,
       0,    26,     0,    27,    28,    29,    30,    31,    32,    33,
       0,    34,    35,     0,    36,  -190,    37,    38,    39,    40,
      41,   -42,    42,    43,     0,    44,     0,     0,     0,    45,
      46,  -309,    47,   274,    49,    50,  -190,   -42,    51,    52,
      53,     0,  -309,    54,    55,     0,    56,    57,    58,    59,
      60,    61,    62,    63,     0,    64,     0,    65,    66,    67,
      68,     0,    69,    70,    71,  -309,     0,     0,    72,     0,
      73,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    81,     0,    82,
      83,     0,    84,     0,     0,    85,     0,     0,     0,    86,
       0,    87,     4,    88,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,  -309,    21,    22,    23,     0,
      24,    25,     0,    26,     0,    27,    28,    29,    30,    31,
      32,    33,     0,    34,    35,     0,    36,  -190,    37,    38,
      39,    40,    41,   -42,    42,    43,     0,    44,     0,     0,
       0,    45,    46,  -309,    47,     0,    49,    50,  -190,   -42,
      51,    52,    53,     0,  -309,    54,    55,     0,    56,    57,
      58,    59,    60,    61,    62,    63,     0,    64,     0,    65,
      66,    67,    68,     0,    69,    70,    71,  -309,     0,     0,
      72,     0,    73,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     0,     0,     0,     0,     0,     0,    80,     0,    81,
       0,    82,    83,     0,    84,     0,     0,    85,     0,     0,
       0,    86,     0,    87,     4,    88,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,  -309,    21,    22,
      23,     0,    24,    25,     0,    26,     0,    27,    28,    29,
      30,    31,   527,    33,     0,    34,    35,     0,    36,  -190,
      37,    38,    39,    40,    41,   -42,    42,    43,     0,    44,
       0,     0,     0,    45,    46,  -309,    47,     0,    49,    50,
    -190,   -42,    51,    52,    53,     0,  -309,    54,    55,     0,
      56,    57,    58,    59,    60,    61,    62,    63,     0,    64,
       0,    65,    66,    67,    68,     0,    69,    70,    71,  -309,
       0,     0,    72,     0,    73,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     0,     0,     0,     0,     0,     0,    80,
       0,    81,     0,    82,    83,     0,    84,     0,     0,    85,
       0,     0,     0,    86,     0,    87,     4,    88,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,  -309,
      21,    22,    23,     0,    24,    25,     0,    26,     0,    27,
      28,    29,    30,    31,   531,    33,     0,    34,    35,     0,
      36,  -190,    37,    38,    39,    40,    41,   -42,    42,    43,
       0,    44,     0,     0,     0,    45,    46,  -309,    47,     0,
      49,    50,  -190,   -42,    51,    52,    53,     0,  -309,    54,
      55,     0,    56,    57,    58,    59,    60,    61,    62,    63,
       0,    64,     0,    65,    66,    67,    68,     0,    69,    70,
      71,  -309,     0,     0,    72,     0,    73,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,    81,     0,    82,    83,     0,    84,     0,
       0,    85,     0,     0,     0,    86,     0,    87,     4,    88,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,  -309,    21,    22,    23,     0,    24,    25,     0,    26,
       0,    27,    28,    29,    30,    31,   782,    33,     0,    34,
      35,     0,    36,  -190,    37,    38,    39,    40,    41,   -42,
      42,    43,     0,    44,     0,     0,     0,    45,    46,  -309,
      47,     0,    49,    50,  -190,   -42,    51,    52,    53,     0,
    -309,    54,    55,     0,    56,    57,    58,    59,    60,    61,
      62,    63,     0,    64,     0,    65,    66,    67,    68,     0,
      69,    70,    71,  -309,     0,     0,    72,     0,    73,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
       0,     0,     0,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,    80,     0,    81,     0,    82,    83,     0,
      84,     0,     0,    85,     0,     0,     0,    86,     0,    87,
       4,    88,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,  -309,    21,    22,    23,     0,    24,    25,
       0,    26,     0,    27,    28,    29,    30,    31,   784,    33,
       0,    34,    35,     0,    36,  -190,    37,    38,    39,    40,
      41,   -42,    42,    43,     0,    44,     0,     0,     0,    45,
      46,  -309,    47,     0,    49,    50,  -190,   -42,    51,    52,
      53,     0,  -309,    54,    55,     0,    56,    57,    58,    59,
      60,    61,    62,    63,     0,    64,     0,    65,    66,    67,
      68,     0,    69,    70,    71,  -309,     0,     0,    72,     0,
      73,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    81,     0,    82,
      83,     0,    84,     0,     0,    85,     0,     0,     0,    86,
       0,    87,     4,    88,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,  -309,    21,    22,    23,     0,
      24,    25,     0,    26,     0,    27,    28,    29,    30,    31,
     787,    33,     0,    34,    35,     0,    36,  -190,    37,    38,
      39,    40,    41,   -42,    42,    43,     0,    44,     0,     0,
       0,    45,    46,  -309,    47,     0,    49,    50,  -190,   -42,
      51,    52,    53,     0,  -309,    54,    55,     0,    56,    57,
      58,    59,    60,    61,    62,    63,     0,    64,     0,    65,
      66,    67,    68,     0,    69,    70,    71,  -309,     0,     0,
      72,     0,    73,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     0,     0,     0,     0,     0,     0,    80,     0,    81,
       0,    82,    83,     0,    84,     0,     0,    85,     0,     0,
       0,    86,     0,    87,     4,    88,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,  -309,    21,    22,
      23,     0,    24,    25,     0,    26,     0,    27,    28,    29,
      30,    31,   790,    33,     0,    34,    35,     0,    36,  -190,
      37,    38,    39,    40,    41,   -42,    42,    43,     0,    44,
       0,     0,     0,    45,    46,  -309,    47,     0,    49,    50,
    -190,   -42,    51,    52,    53,     0,  -309,    54,    55,     0,
      56,    57,    58,    59,    60,    61,    62,    63,     0,    64,
       0,    65,    66,    67,    68,     0,    69,    70,    71,  -309,
       0,     0,    72,     0,    73,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     0,     0,     0,     0,     0,     0,    80,
       0,    81,     0,    82,    83,     0,    84,     0,     0,    85,
       0,     0,     0,    86,     0,    87,     4,    88,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,  -309,
      21,    22,    23,     0,   813,    25,     0,    26,     0,    27,
      28,    29,    30,    31,    32,    33,     0,    34,    35,     0,
      36,  -190,    37,    38,    39,    40,    41,   -42,    42,    43,
       0,    44,     0,     0,     0,    45,    46,  -309,    47,     0,
      49,    50,  -190,   -42,    51,    52,    53,     0,  -309,    54,
      55,     0,    56,    57,    58,    59,    60,    61,    62,    63,
       0,    64,     0,    65,    66,    67,    68,     0,    69,    70,
      71,  -309,     0,     0,    72,     0,    73,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,    81,     0,    82,    83,     0,    84,     0,
       0,    85,     0,     0,     0,    86,     0,    87,     4,    88,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,  -309,    21,    22,    23,     0,    24,    25,     0,    26,
       0,    27,    28,    29,    30,    31,   820,    33,     0,    34,
      35,     0,    36,  -190,    37,    38,    39,    40,    41,   -42,
      42,    43,     0,    44,     0,     0,     0,    45,    46,  -309,
      47,     0,    49,    50,  -190,   -42,    51,    52,    53,     0,
    -309,    54,    55,     0,    56,    57,    58,    59,    60,    61,
      62,    63,     0,    64,     0,    65,    66,    67,    68,     0,
      69,    70,    71,  -309,     0,     0,    72,     0,    73,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
       0,     0,     0,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,    80,     0,    81,     0,    82,    83,     0,
      84,     0,     0,    85,     0,     0,     0,    86,     0,    87,
       4,    88,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,  -309,    21,    22,    23,     0,    24,    25,
       0,    26,     0,    27,    28,    29,    30,    31,   824,    33,
       0,    34,    35,     0,    36,  -190,    37,    38,    39,    40,
      41,   -42,    42,    43,     0,    44,     0,     0,     0,    45,
      46,  -309,    47,     0,    49,    50,  -190,   -42,    51,    52,
      53,     0,  -309,    54,    55,     0,    56,    57,    58,    59,
      60,    61,    62,    63,     0,    64,     0,    65,    66,    67,
      68,     0,    69,    70,    71,  -309,     0,     0,    72,     0,
      73,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    81,     0,    82,
      83,     0,    84,     0,     0,    85,     0,     0,     0,    86,
       0,    87,   697,    88,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   150,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,   151,   152,     0,
     153,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,    62,   154,     0,     0,     0,     0,
       0,     0,    68,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     0,     0,     0,     0,     0,     0,    80,     0,    81,
       0,    82,    83,   221,     0,     0,     0,    85,     0,     0,
       0,   155,     0,    87,     0,   156,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   150,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,   151,
     152,     0,   153,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,   154,     0,     0,
       0,     0,     0,     0,    68,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     0,     0,     0,     0,     0,     0,    80,
       0,    81,     0,    82,    83,   221,     0,     0,     0,    85,
       0,     0,     0,   155,     0,    87,     0,   156,   600,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   150,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,   151,   152,     0,   153,    33,     0,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,    46,     0,    47,
       0,    49,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     154,     0,     0,     0,     0,     0,     0,    68,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,    80,     0,    81,     0,    82,    83,   221,     0,
       0,     0,    85,     0,     0,     0,   155,     0,    87,     0,
     156,   931,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   150,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,   151,   152,     0,   153,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
      46,     0,    47,     0,    49,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,   154,     0,     0,     0,     0,     0,     0,
      68,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    81,     0,    82,
      83,     0,     0,     0,     0,    85,     0,     0,     0,   155,
       0,    87,     0,   156,   605,     5,     6,     7,     8,   177,
      10,   178,     0,     0,   150,     0,     0,    14,    15,     0,
       0,   -89,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,   151,   152,
       0,   153,    33,     0,    34,     0,     0,    36,  -477,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,  -477,
       0,     0,    52,   -89,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   154,     0,     0,   731,
       0,     5,   179,    68,     0,   -89,    70,     0,   732,     0,
       0,     0,     0,    14,   194,   733,    74,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,   195,     0,
       0,    75,    76,    77,    78,     0,     0,     0,    33,    79,
      34,     0,     0,    36,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,     0,     0,     0,     0,    85,   196,
       0,     0,   155,     0,    87,     0,   156,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,   197,   198,     0,
      61,     0,   199,     0,     0,     0,     0,     0,     0,    68,
       0,     0,   200,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   318,   319,   320,   321,   322,   323,     0,
       0,   326,   327,   328,   329,   330,   331,   734,   735,   736,
     737,     0,     0,   738,     0,     0,     0,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,     0,   750,
       0,     0,   751,   752,   753,   754,     0,   755,     0,     0,
     756,     5,     6,     7,     8,   177,    10,     0,     0,     0,
     150,     0,     0,    14,    15,     0,     0,   -89,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,   151,   152,     0,   153,    33,     0,
      34,     0,     0,    36,  -477,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,  -477,     0,     0,    52,   -89,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   154,     0,     0,     0,     0,     0,   179,    68,
       0,   -89,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     5,     6,
       7,     8,     9,    10,    80,     0,    81,   150,    82,    83,
      14,    15,     0,     0,    85,     0,     0,    19,   155,     0,
      87,     0,   156,     0,     0,    25,     0,     0,     0,     0,
       0,   151,   152,     0,   153,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,     0,    42,    43,
       0,     0,     0,     0,     0,     0,    46,     0,    47,     0,
      49,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,    62,   154,
       0,     0,     0,   225,   226,     0,    68,   265,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,   266,     0,     0,     0,     0,     0,     0,
       0,    80,     0,    81,     0,    82,    83,   221,     0,     0,
       0,    85,     0,     0,     0,   155,     0,    87,     0,   156,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   150,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,   151,   152,     0,   153,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,    46,     0,
      47,     0,    49,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,   154,     0,     0,     0,   225,   226,     0,    68,   265,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
       0,     0,     0,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,    80,     0,    81,     0,    82,    83,   221,
       0,     0,     0,    85,     0,     0,     0,   155,     0,    87,
     594,   156,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   150,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,   151,   152,     0,   153,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
      46,     0,    47,     0,    49,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,   154,     0,     0,     0,   225,   226,     0,
      68,   265,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    81,     0,    82,
      83,   221,     0,     0,     0,    85,     0,     0,     0,   155,
       0,    87,   596,   156,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   150,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,   497,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,   151,   152,     0,
     153,    33,   498,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,   499,     0,     0,     0,     0,     0,
      58,    59,    60,    61,    62,   154,     0,     0,     0,     0,
       0,     0,    68,     0,     0,    70,     0,   500,     0,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     5,     6,     7,     8,     9,    10,    80,     0,    81,
     150,    82,    83,    14,    15,     0,     0,    85,     0,     0,
      19,   155,     0,    87,     0,   156,     0,     0,    25,     0,
       0,     0,     0,     0,   151,   152,     0,   153,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   154,     0,     0,     0,   225,   226,     0,    68,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
       0,   227,     0,     0,    85,     0,     0,     0,   155,     0,
      87,     0,   156,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   150,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,   151,   152,     0,   153,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,    46,     0,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,    62,   154,     0,     0,     0,   225,   226,
       0,    68,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,   221,     0,     0,     0,    85,     0,     0,     0,
     155,     0,    87,     0,   156,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   150,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,   151,   152,
       0,   153,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,   699,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   154,     0,     0,     0,
     225,   226,     0,    68,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     5,     6,     7,     8,     9,    10,    80,     0,
      81,   150,    82,    83,    14,    15,     0,     0,    85,     0,
       0,    19,   155,     0,    87,     0,   156,     0,     0,    25,
       0,     0,     0,     0,     0,   151,   152,     0,   153,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
      46,   182,    47,     0,    49,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,   154,     0,     0,     0,     0,     0,     0,
      68,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,   183,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    81,     0,    82,
      83,     0,     0,     0,     0,    85,     0,     0,     0,   155,
       0,    87,     0,   156,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   150,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,   187,     0,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     0,   188,   151,   152,     0,
     153,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,    62,   154,     0,     0,     0,     0,
       0,     0,    68,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     5,     6,     7,     8,     9,    10,    80,     0,    81,
     150,    82,    83,    14,    15,     0,     0,    85,     0,     0,
      19,   155,     0,    87,     0,   156,     0,     0,    25,     0,
       0,     0,     0,     0,   151,   152,     0,   153,    33,     0,
      34,     0,     0,    36,  -477,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,  -477,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   154,     0,     0,     0,     0,     0,     0,    68,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     5,     6,
       7,     8,     9,    10,    80,     0,    81,   150,    82,    83,
      14,    15,     0,     0,    85,     0,     0,    19,   155,     0,
      87,     0,   156,     0,     0,    25,     0,     0,     0,     0,
       0,   151,   152,     0,   153,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,     0,    42,    43,
       0,     0,     0,     0,     0,     0,    46,     0,    47,     0,
      49,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,    62,   154,
       0,     0,     0,   225,   226,     0,    68,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,     0,     0,     5,     6,     7,     8,     9,
      10,    80,     0,    81,   150,    82,    83,    14,    15,     0,
       0,    85,     0,     0,    19,   155,     0,    87,     0,   156,
       0,     0,    25,     0,     0,     0,     0,     0,   151,   152,
       0,   153,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   154,     0,     0,     0,
       0,     0,     0,    68,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,   221,     0,     0,     0,    85,     0,
       0,     0,   155,   592,    87,     0,   156,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   150,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,   207,    25,     0,     0,     0,     0,     0,
     151,   152,     0,   153,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,     0,    42,    43,     0,
       0,     0,     0,     0,     0,    46,     0,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,   154,     0,
       0,     0,     0,     0,     0,    68,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     5,     6,     7,     8,     9,    10,
      80,     0,    81,   150,    82,    83,    14,    15,     0,     0,
      85,     0,     0,    19,    86,     0,    87,     0,   156,     0,
       0,    25,     0,     0,     0,     0,     0,   151,   152,     0,
     153,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,    62,   154,     0,     0,     0,     0,
       0,     0,    68,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     0,     0,     0,     0,     0,     0,    80,     0,    81,
       0,    82,    83,   221,     0,     0,     0,    85,     0,     0,
       0,   155,     0,    87,     0,   156,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   150,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,   151,
     152,     0,   153,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,   154,     0,     0,
       0,     0,     0,     0,    68,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,   183,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     5,     6,     7,     8,     9,    10,    80,
       0,    81,   150,    82,    83,    14,   874,     0,     0,    85,
       0,     0,    19,   155,     0,    87,     0,   156,     0,     0,
     875,     0,     0,     0,     0,     0,   151,   152,     0,   153,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,   876,     0,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,   877,
     878,    60,    61,    62,   879,     0,     0,     0,     0,     0,
       0,    68,     0,     0,   880,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,   221,     0,     0,     0,    85,     0,     0,     0,
     155,     0,    87,     0,   881,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   150,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,   151,   152,
       0,   153,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   154,     0,     0,     0,
       0,     0,     0,    68,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,   221,     0,     0,     0,    85,     0,
       0,     0,   155,     0,    87,     0,   881,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   150,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
     151,   152,     0,   858,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,     0,    42,    43,     0,
       0,     0,     0,     0,     0,    46,     0,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,   154,     0,
       0,     0,     0,     0,     0,    68,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,   221,     0,     0,     0,
      85,     0,     0,     0,   155,     0,    87,     0,   156,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   150,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,   151,   152,     0,   153,    33,     0,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,    46,     0,    47,
       0,    49,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     154,     0,     0,     0,     0,     0,     0,    68,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     5,     6,     7,     8,
       9,    10,    80,     0,    81,   150,    82,    83,    14,    15,
       0,     0,    85,     0,     0,    19,   155,     0,    87,     0,
     156,     0,     0,    25,     0,     0,     0,     0,     0,   151,
     152,     0,   153,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,   154,     0,     0,
       0,     0,     0,     0,    68,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     5,     6,     7,     8,     9,    10,    80,
       0,    81,   150,    82,    83,    14,   463,     0,     0,    85,
       0,     0,    19,    86,     0,    87,     0,   156,     0,     0,
      25,     0,     0,     0,     0,     0,   151,   152,     0,   153,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,    46,     0,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,    62,   154,     0,     0,     0,     0,     0,
       0,    68,     0,     0,   464,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       5,     6,     7,     8,     9,    10,    80,     0,    81,   150,
      82,    83,    14,    15,     0,     0,    85,     0,     0,    19,
     155,     0,    87,     0,   156,     0,     0,    25,     0,     0,
       0,     0,     0,   151,   152,     0,   153,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,    46,     0,
      47,     0,    49,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,   154,     0,     0,     0,     0,     0,     0,    68,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
       0,     0,     0,     0,    79,     0,     0,     5,     6,     7,
       8,     9,    10,    80,     0,    81,   150,    82,    83,    14,
     563,     0,     0,   537,     0,     0,    19,   155,     0,    87,
       0,   156,     0,     0,   564,     0,     0,     0,     0,     0,
     151,   152,     0,   153,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,     0,    42,    43,     0,
       0,     0,     0,     0,     0,   565,     0,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,   566,   567,    60,    61,    62,   568,     0,
       0,     0,     0,     0,     0,    68,     0,     0,   569,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     5,     6,     7,     8,     9,    10,
      80,     0,    81,   150,    82,    83,    14,    15,     0,     0,
      85,     0,     0,    19,   155,     0,    87,     0,   570,     0,
       0,    25,     0,     0,     0,     0,     0,   151,   152,     0,
     153,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,    62,   154,     0,     0,     0,     0,
       0,     0,    68,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     5,     6,     7,     8,     9,    10,    80,     0,    81,
     150,    82,    83,    14,    15,     0,     0,    85,     0,     0,
      19,   155,     0,    87,     0,   570,     0,     0,    25,     0,
       0,     0,     0,     0,   151,   152,     0,   640,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   154,     0,     0,     0,     0,     0,     0,    68,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     5,     6,
       7,     8,     9,    10,    80,     0,    81,   150,    82,    83,
      14,    15,     0,     0,    85,     0,     0,    19,   155,     0,
      87,     0,   156,     0,     0,    25,     0,     0,     0,     0,
       0,   151,   152,     0,   643,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,     0,    42,    43,
       0,     0,     0,     0,     0,     0,    46,     0,    47,     0,
      49,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,    62,   154,
       0,     0,     0,     0,     0,     0,    68,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,     0,     0,     5,     6,     7,     8,     9,
      10,    80,     0,    81,   150,    82,    83,    14,    15,     0,
       0,    85,     0,     0,    19,   155,     0,    87,     0,   156,
       0,     0,    25,     0,     0,     0,     0,     0,   151,   152,
       0,   852,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   154,     0,     0,     0,
       0,     0,     0,    68,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     5,     6,     7,     8,     9,    10,    80,     0,
      81,   150,    82,    83,    14,    15,     0,     0,    85,     0,
       0,    19,   155,     0,    87,     0,   156,     0,     0,    25,
       0,     0,     0,     0,     0,   151,   152,     0,   853,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
      46,     0,    47,     0,    49,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,   154,     0,     0,     0,     0,     0,     0,
      68,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     5,
       6,     7,     8,     9,    10,    80,     0,    81,   150,    82,
      83,    14,    15,     0,     0,    85,     0,     0,    19,   155,
       0,    87,     0,   156,     0,     0,    25,     0,     0,     0,
       0,     0,   151,   152,     0,   855,    33,     0,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,    46,     0,    47,
       0,    49,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     154,     0,     0,     0,     0,     0,     0,    68,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     5,     6,     7,     8,
       9,    10,    80,     0,    81,   150,    82,    83,    14,    15,
       0,     0,    85,     0,     0,    19,   155,     0,    87,     0,
     156,     0,     0,    25,     0,     0,     0,     0,     0,   151,
     152,     0,   856,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,   154,     0,     0,
       0,     0,     0,     0,    68,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     5,     6,     7,     8,     9,    10,    80,
       0,    81,   150,    82,    83,    14,    15,     0,     0,    85,
       0,     0,    19,   155,     0,    87,     0,   156,     0,     0,
      25,     0,     0,     0,     0,     0,   151,   152,     0,   857,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,    46,     0,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,    62,   154,     0,     0,     0,     0,     0,
       0,    68,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       5,     6,     7,     8,     9,    10,    80,     0,    81,   150,
      82,    83,    14,    15,     0,     0,    85,     0,     0,    19,
     155,     0,    87,     0,   156,     0,     0,    25,     0,     0,
       0,     0,     0,   151,   152,     0,   858,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,    46,     0,
      47,     0,    49,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,   154,     0,     0,     0,     0,     0,     0,    68,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
       0,     0,     0,     0,    79,     0,     0,     5,     6,     7,
       8,     9,    10,    80,     0,    81,   150,    82,    83,    14,
     944,     0,     0,    85,     0,     0,    19,   155,     0,    87,
       0,   156,     0,     0,   945,     0,     0,     0,     0,     0,
     151,   152,     0,   153,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,     0,    42,    43,     0,
       0,     0,     0,     0,     0,   946,     0,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,   947,   948,    60,    61,    62,   949,     0,
       0,     0,     0,     0,     0,    68,     0,     0,   950,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     5,     6,     7,     8,     9,    10,
      80,     0,    81,   150,    82,    83,    14,    15,     0,     0,
      85,     0,     0,    19,   155,     0,    87,     0,   951,     0,
       0,    25,     0,     0,     0,     0,     0,   151,   152,     0,
     153,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,    62,   154,     0,     0,     0,     0,
       0,     0,    68,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     5,     6,     7,     8,     9,    10,    80,     0,    81,
     150,    82,    83,    14,    15,     0,     0,    85,     0,     0,
      19,     0,     0,    87,     0,   951,     0,     0,    25,     0,
       0,     0,     0,     0,   151,   152,     0,   153,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   154,     0,     0,     0,     0,     0,     0,    68,
       0,     0,    70,     0,     0,     0,     5,     6,     7,     8,
       9,    10,     0,  -359,     0,   150,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,    76,     0,
       0,     0,  -359,    25,     0,     0,  -359,     0,     0,   151,
     152,     0,   153,    33,     0,    34,    81,     0,    36,     0,
     221,    38,    39,     0,    41,     0,    42,    43,   155,     0,
      87,  -359,   881,     0,    46,     0,    47,     0,    49,     0,
       0,     0,     0,    52,     0,     0,     0,     0,  -329,     0,
       0,     0,    58,    59,    60,    61,    62,   154,     0,     0,
    -329,     0,     0,     0,    68,     0,     0,    70,     0,     0,
       0,  -329,     0,     0,     0,     0,     0,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,     0,     0,    76,     0,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   150,     0,     0,    14,    15,     0,
       0,    81,     0,     0,    19,     0,  -329,     0,     0,   332,
     333,  -457,    25,  -329,  -486,    87,  -486,   951,   151,   152,
       0,   153,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   154,     0,     0,     0,
       0,     0,     0,    68,     0,     0,    70,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   150,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,    76,     0,    25,     0,     0,     0,     0,     0,
     151,   152,     0,   153,    33,     0,    34,     0,     0,    36,
      81,     0,    38,    39,     0,    41,     0,    42,    43,     0,
       0,     0,   155,     0,    87,    46,   156,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,   154,     0,
       0,     0,     0,     0,     0,    68,     0,     0,    70,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   150,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
     292,     0,     0,     0,    76,     0,    25,     0,     0,     0,
       0,     0,   151,   152,     0,   153,    33,     0,    34,     0,
       0,    36,    81,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,   155,     0,    87,    46,   570,    47,
       0,    49,     0,     0,   293,     0,    52,     0,   294,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     154,     0,     5,     0,     0,     0,     0,    68,     0,   732,
      70,     0,     0,     0,    14,   194,   733,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,   195,
       0,   296,     0,   297,   298,   299,    76,   300,   301,    33,
       0,    34,   304,     0,    36,     0,     0,     0,   310,   311,
       0,     0,     0,   314,    81,     0,   315,   316,   317,     0,
     196,     0,     0,     0,     0,     0,     0,     0,    87,    52,
     156,     0,     0,     0,     0,     0,     0,     0,   197,   198,
       0,    61,     0,   199,     0,   290,     0,     0,     0,     0,
      68,     0,   291,   200,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   292,   318,   319,   320,   321,   322,   323,
       0,     0,   326,   327,   328,   329,   330,   331,   734,   735,
     736,   737,     0,     0,   738,     0,     0,     0,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   749,     0,
     750,     0,     0,   751,   752,   753,   754,   293,   755,     0,
       0,   294,     0,     0,     0,     0,   290,     0,     0,     0,
       0,     0,     0,   291,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   292,     0,     0,     0,     0,   295,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   296,     0,   297,   298,   299,     0,
     300,   301,   302,     0,   303,   304,   305,   306,   307,   308,
     309,   310,   311,     0,   312,   313,   314,     0,   293,   315,
     316,   317,   294,     0,     0,     0,     0,   290,     0,     0,
     775,     0,     0,     0,   291,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   292,     0,     0,     0,     0,
     295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,     0,   297,   298,   299,
       0,   300,   301,   302,     0,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,     0,   293,
     315,   316,   317,   294,     0,     0,     0,     0,   290,     0,
       0,   940,     0,     0,     0,   291,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   292,     0,     0,     0,
       0,   295,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   296,     0,   297,   298,
     299,     0,   300,   301,   302,     0,   303,   304,   305,   306,
     307,   308,   309,   310,   311,     0,   312,   313,   314,     0,
     293,   315,   316,   317,   294,     0,     0,     0,     0,   290,
       0,     0,   994,     0,     0,     0,   291,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   292,     0,     0,
       0,     0,   295,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   296,     0,   297,
     298,   299,     0,   300,   301,   302,     0,   303,   304,   305,
     306,   307,   308,   309,   310,   311,     0,   312,   313,   314,
       0,   293,   315,   316,   317,   294,     0,     0,     0,     0,
     290,   593,     0,     0,     0,     0,     0,   291,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   292,     0,
       0,     0,     0,   295,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   296,     0,
     297,   298,   299,     0,   300,   301,   302,     0,   303,   304,
     305,   306,   307,   308,   309,   310,   311,     0,   312,   313,
     314,     0,   293,   315,   316,   317,   294,     0,     0,     0,
       0,   290,   910,     0,     0,   377,     0,     0,   291,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   292,
       0,     0,     0,     0,   295,   318,   319,   320,   321,   322,
     323,     0,     0,   326,   327,   328,   329,   330,   331,   296,
       0,   297,   298,   299,     0,   300,   301,   302,     0,   303,
     304,   305,   306,   307,   308,   309,   310,   311,     0,   312,
     313,   314,     0,   293,   315,   316,   317,   294,     0,     0,
     346,     0,   290,     0,     0,     0,   781,     0,     0,   291,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     292,     0,     0,     0,     0,   295,   318,   319,   320,   321,
     322,   323,     0,     0,   326,   327,   328,   329,   330,   331,
     296,     0,   297,   298,   299,     0,   300,   301,   302,     0,
     303,   304,   305,   306,   307,   308,   309,   310,   311,     0,
     312,   313,   314,     0,   293,   315,   316,   317,   294,     0,
       0,   346,     0,   290,     0,     0,     0,   786,     0,     0,
     291,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   292,     0,     0,     0,     0,   295,   318,   319,   320,
     321,   322,   323,     0,     0,   326,   327,   328,   329,   330,
     331,   296,     0,   297,   298,   299,     0,   300,   301,   302,
       0,   303,   304,   305,   306,   307,   308,   309,   310,   311,
       0,   312,   313,   314,     0,   293,   315,   316,   317,   294,
       0,     0,   346,     0,   290,     0,     0,     0,   923,     0,
       0,   291,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   292,     0,     0,     0,     0,   295,   318,   319,
     320,   321,   322,   323,     0,     0,   326,   327,   328,   329,
     330,   331,   296,     0,   297,   298,   299,     0,   300,   301,
     302,     0,   303,   304,   305,   306,   307,   308,   309,   310,
     311,     0,   312,   313,   314,     0,   293,   315,   316,   317,
     294,     0,     0,   346,     0,   290,     0,     0,     0,   924,
       0,     0,   291,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   292,     0,     0,     0,     0,   295,   318,
     319,   320,   321,   322,   323,     0,     0,   326,   327,   328,
     329,   330,   331,   296,     0,   297,   298,   299,     0,   300,
     301,   302,     0,   303,   304,   305,   306,   307,   308,   309,
     310,   311,     0,   312,   313,   314,     0,   293,   315,   316,
     317,   294,     0,     0,   346,     0,   290,     0,     0,     0,
     925,     0,     0,   291,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   292,     0,     0,     0,     0,   295,
     318,   319,   320,   321,   322,   323,     0,     0,   326,   327,
     328,   329,   330,   331,   296,     0,   297,   298,   299,     0,
     300,   301,   302,     0,   303,   304,   305,   306,   307,   308,
     309,   310,   311,     0,   312,   313,   314,     0,   293,   315,
     316,   317,   294,     0,     0,   346,     0,   290,     0,     0,
       0,   926,     0,     0,   291,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   292,     0,     0,     0,     0,
     295,   318,   319,   320,   321,   322,   323,     0,     0,   326,
     327,   328,   329,   330,   331,   296,     0,   297,   298,   299,
       0,   300,   301,   302,     0,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,     0,   293,
     315,   316,   317,   294,     0,     0,   346,     0,   290,     0,
       0,     0,   942,     0,     0,   291,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   292,     0,     0,     0,
       0,   295,   318,   319,   320,   321,   322,   323,     0,     0,
     326,   327,   328,   329,   330,   331,   296,     0,   297,   298,
     299,     0,   300,   301,   302,     0,   303,   304,   305,   306,
     307,   308,   309,   310,   311,     0,   312,   313,   314,     0,
     293,   315,   316,   317,   294,     0,     0,   346,     0,   290,
       0,     0,     0,   943,     0,     0,   291,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   292,   369,     0,
       0,     0,   295,   318,   319,   320,   321,   322,   323,     0,
     370,   326,   327,   328,   329,   330,   331,   296,     0,   297,
     298,   299,     0,   300,   301,   302,     0,   303,   304,   305,
     306,   307,   308,   309,   310,   311,     0,   312,   313,   314,
       0,   293,   315,   316,   317,   294,     0,     0,   346,     0,
       0,     0,     0,     0,     0,     0,   290,     0,     0,     0,
       0,     0,     0,   291,     0,     0,     0,     0,     0,   371,
       0,     0,     0,   295,   292,   366,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,   296,     0,
     297,   298,   299,     0,   300,   301,   302,     0,   303,   304,
     305,   306,   307,   308,   309,   310,   311,     0,   312,   313,
     314,     0,     0,   315,   316,   317,     0,     0,   293,   346,
       0,     0,   294,     0,     0,     0,     0,   290,     0,     0,
       0,     0,     0,     0,   291,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   292,   667,     0,     0,     0,
     295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,     0,   297,   298,   299,
       0,   300,   301,   302,     0,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,     0,   293,
     315,   316,   317,   294,     0,     0,   346,     0,     0,     0,
       0,     0,     0,     0,   290,     0,     0,     0,     0,     0,
       0,   291,     0,     0,     0,     0,     0,   371,     0,     0,
       0,   295,   292,   207,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   296,     0,   297,   298,
     299,     0,   300,   301,   302,     0,   303,   304,   305,   306,
     307,   308,   309,   310,   311,     0,   312,   313,   314,     0,
       0,   315,   316,   317,   292,     0,   293,   346,     0,     0,
     294,     0,     0,     0,     0,   290,     0,     0,     0,     0,
       0,     0,   291,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   292,   661,     0,     0,     0,   295,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   293,     0,
       0,     0,   294,   296,     0,   297,   298,   299,     0,   300,
     301,   302,     0,   303,   304,   305,   306,   307,   308,   309,
     310,   311,     0,   312,   313,   314,     0,   293,   315,   316,
     317,   294,     0,     0,   346,     0,   290,     0,     0,     0,
       0,     0,     0,   291,     0,   296,     0,   297,   298,   299,
       0,   300,   301,   302,   292,   303,   304,   305,   306,   295,
     308,   309,   310,   311,     0,   312,     0,   314,     0,     0,
     315,   316,   317,     0,   296,     0,   297,   298,   299,     0,
     300,   301,   302,     0,   303,   304,   305,   306,   307,   308,
     309,   310,   311,     0,   312,   313,   314,     0,   293,   315,
     316,   317,   294,     0,     0,   346,     0,   290,     0,     0,
       0,   679,     0,     0,   291,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   292,     0,     0,     0,     0,
     295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,     0,   297,   298,   299,
       0,   300,   301,   302,     0,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,     0,   293,
     315,   316,   317,   294,     0,     0,   346,     0,   290,     0,
       0,     0,     0,     0,     0,   291,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   292,     0,     0,     0,
       0,   295,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   296,     0,   297,   298,
     299,     0,   300,   301,   302,     0,   303,   304,   305,   306,
     307,   308,   309,   310,   311,     0,   312,   313,   314,     0,
     293,   315,   316,   317,   294,     0,     0,   404,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   290,
       0,     0,     0,     0,     0,     0,   291,     0,     0,     0,
       0,     0,   295,     0,     0,     0,     0,   292,     0,     0,
       0,     0,   374,     0,     0,     0,     0,   296,     0,   297,
     298,   299,     0,   300,   301,   302,     0,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   375,   312,   313,   314,
       0,   290,   315,   316,   317,     0,     0,  -458,   291,     0,
       0,   293,     0,     0,     0,   294,     0,     0,     0,   292,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   348,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   295,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   296,     0,
     297,   298,   299,   293,   300,   301,   302,   294,   303,   304,
     305,   306,   307,   308,   309,   310,   311,     0,   312,   313,
     314,     0,   376,   315,   316,   317,   290,     0,     0,     0,
       0,   145,     0,   291,     0,   295,     0,     0,     0,     0,
       0,     0,     0,     0,   292,     0,     0,     0,     0,   418,
     296,     0,   297,   298,   299,     0,   300,   301,   302,     0,
     303,   304,   305,   306,   307,   308,   309,   310,   311,     0,
     312,   313,   314,   419,     0,   315,   316,   317,   290,     0,
       0,     0,     0,     0,     0,   291,     0,     0,   293,     0,
       0,     0,   294,     0,     0,     0,   292,   506,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   507,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,     0,   297,   298,   299,
     293,   300,   301,   302,   294,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,   290,     0,
     315,   316,   317,     0,     0,   291,     0,     0,     0,     0,
       0,     0,   295,     0,     0,     0,   292,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   296,     0,   297,
     298,   299,     0,   300,   301,   302,     0,   303,   304,   305,
     306,   307,   308,   309,   310,   311,     0,   312,   313,   314,
     290,     0,   315,   316,   317,     0,     0,   291,     0,     0,
     293,     0,     0,     0,   294,     0,     0,     0,   292,   508,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   509,     0,     0,   290,     0,     0,     0,     0,     0,
       0,   291,   295,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   292,     0,     0,     0,     0,   296,     0,   297,
     298,   299,   293,   300,   301,   302,   294,   303,   304,   305,
     306,   307,   308,   309,   310,   311,     0,   312,   313,   314,
       0,   408,   315,   316,   317,     0,   290,     0,     0,     0,
       0,     0,     0,   291,   295,     0,   293,     0,     0,     0,
     294,     0,     0,     0,   292,     0,     0,     0,     0,   296,
       0,   297,   298,   299,     0,   300,   301,   302,     0,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   295,   312,
     313,   314,     0,     0,   315,   316,   317,     0,     0,     0,
       0,     0,     0,   296,     0,   297,   298,   299,   293,   300,
     301,   302,   294,   303,   304,   305,   306,   307,   308,   309,
     310,   311,     0,   312,   313,   314,   290,   410,   315,   316,
     317,     0,     0,   291,     0,     0,     0,     0,     0,     0,
     295,     0,     0,     0,   292,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,     0,   297,   298,   299,
       0,   300,   301,   302,     0,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,   290,   412,
     315,   316,   317,     0,     0,   291,     0,     0,   293,     0,
       0,     0,   294,     0,     0,     0,   292,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,     0,   297,   298,   299,
     293,   300,   301,   302,   294,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,   290,   421,
     315,   316,   317,     0,     0,   291,     0,     0,     0,     0,
       0,   436,   295,     0,     0,     0,   292,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   296,     0,   297,
     298,   299,     0,   300,   301,   302,     0,   303,   304,   305,
     306,   307,   308,   309,   310,   311,     0,   312,   313,   314,
     290,     0,   315,   316,   317,     0,     0,   291,     0,     0,
     293,     0,     0,     0,   294,     0,     0,     0,   292,     0,
       0,     0,     0,   510,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   290,     0,     0,     0,     0,     0,
       0,   291,   295,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   292,     0,     0,     0,     0,   296,     0,   297,
     298,   299,   293,   300,   301,   302,   294,   303,   304,   305,
     306,   307,   308,   309,   310,   311,     0,   312,   313,   314,
       0,     0,   315,   316,   317,     0,   290,   674,     0,     0,
     145,     0,     0,   291,   295,     0,   293,     0,     0,     0,
     294,     0,     0,     0,   292,     0,     0,     0,     0,   296,
       0,   297,   298,   299,     0,   300,   301,   302,     0,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   295,   312,
     313,   314,     0,     0,   315,   316,   317,     0,     0,     0,
       0,     0,     0,   296,     0,   297,   298,   299,   293,   300,
     301,   302,   294,   303,   304,   305,   306,   307,   308,   309,
     310,   311,     0,   312,   313,   314,   290,   650,   315,   316,
     317,     0,     0,   291,     0,     0,     0,     0,     0,     0,
     295,     0,     0,     0,   292,     0,     0,   677,     0,     0,
       0,     0,     0,     0,     0,   296,     0,   297,   298,   299,
       0,   300,   301,   302,     0,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,   290,     0,
     315,   316,   317,     0,     0,   291,     0,     0,   293,     0,
       0,     0,   294,     0,     0,     0,   292,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   290,     0,     0,     0,     0,     0,     0,   291,
     295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     292,   770,     0,     0,     0,   296,     0,   297,   298,   299,
     293,   300,   301,   302,   294,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,     0,     0,
     315,   316,   317,     0,     0,     0,     0,     0,     0,     0,
       0,   720,   295,     0,   293,     0,     0,     0,   294,     0,
       0,     0,     0,     0,     0,     0,     0,   296,     0,   297,
     298,   299,     0,   300,   301,   302,     0,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   295,   312,   313,   314,
       0,     0,   315,   316,   317,     0,     0,     0,     0,     0,
       0,   296,     0,   297,   298,   299,     0,   300,   301,   302,
       0,   303,   304,   305,   306,   307,   308,   309,   310,   311,
       0,   312,   313,   314,   290,     0,   315,   316,   317,     0,
       0,   291,     0,     0,     0,     0,     0,    16,     0,     0,
       0,    20,   292,   773,     0,     0,     0,     0,     0,     0,
      26,     0,    27,   833,     0,     0,    31,     0,     0,     0,
       0,    35,     0,   290,  -190,     0,     0,     0,     0,     0,
     291,     0,     0,     0,     0,     0,     0,     0,    45,     0,
       0,   292,    48,     0,   834,  -190,   293,   835,     0,    53,
     294,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   290,     0,     0,     0,     0,     0,    67,   291,
       0,    69,     0,     0,     0,     0,     0,     0,   295,     0,
     292,     0,     0,     0,     0,   293,     0,     0,     0,   294,
       0,     0,     0,   296,     0,   297,   298,   299,   851,   300,
     301,   302,     0,   303,   304,   305,   306,   307,   308,   309,
     310,   311,     0,   312,   313,   314,     0,   295,   315,   316,
     317,    84,     0,     0,   293,     0,     0,     0,   294,   921,
       0,     0,   296,     0,   297,   298,   299,   854,   300,   301,
     302,     0,   303,   304,   305,   306,   307,   308,   309,   310,
     311,     0,   312,   313,   314,     0,   295,   315,   316,   317,
       0,     0,   290,   869,     0,     0,     0,     0,     0,   291,
       0,   296,     0,   297,   298,   299,     0,   300,   301,   302,
     292,   303,   304,   305,   306,   307,   308,   309,   310,   311,
       0,   312,   313,   314,     0,     0,   315,   316,   317,     0,
       0,   290,     0,     0,     0,     0,     0,     0,   291,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   292,
       0,     0,     0,     0,   293,     0,     0,     0,   294,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     290,     0,     0,     0,     0,     0,     0,   291,     0,     0,
       0,     0,     0,     0,     0,     0,   295,     0,   292,     0,
       0,     0,     0,   293,     0,     0,     0,   294,     0,     0,
       0,   296,     0,   297,   298,   299,   959,   300,   301,   302,
       0,   303,   304,   305,   306,   307,   308,   309,   310,   311,
       0,   312,   313,   314,     0,   295,   315,   316,   317,     0,
       0,     0,   293,     0,     0,     0,   294,     0,     0,     0,
     296,     0,   297,   298,   299,   960,   300,   301,   302,     0,
     303,   304,   305,   306,   307,   308,   309,   310,   311,     0,
     312,   313,   314,     0,   295,   315,   316,   317,     0,     0,
     290,     0,     0,     0,     0,     0,     0,   291,     0,   296,
       0,   297,   298,   299,     0,   300,   301,   302,   292,   303,
     304,   305,   306,   307,   308,   309,   310,   311,     0,   312,
     313,   314,     0,     0,   315,   316,   317,     0,     0,   290,
       0,     0,     0,     0,     0,     0,   291,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   292,     0,     0,
       0,     0,   293,     0,     0,     0,   294,     0,     0,     0,
       0,     0,     0,     0,     0,   961,     0,     0,   290,     0,
       0,     0,     0,     0,     0,   291,     0,     0,     0,     0,
       0,     0,     0,     0,   295,     0,   292,     0,     0,     0,
       0,   293,     0,     0,     0,   294,     0,     0,     0,   296,
       0,   297,   298,   299,   962,   300,   301,   302,     0,   303,
     304,   305,   306,   307,   308,   309,   310,   311,     0,   312,
     313,   314,     0,   295,   315,   316,   317,     0,     0,     0,
     293,     0,     0,     0,   294,     0,     0,     0,   296,     0,
     297,   298,   299,   963,   300,   301,   302,     0,   303,   304,
     305,   306,   307,   308,   309,   310,   311,     0,   312,   313,
     314,     0,   295,   315,   316,   317,     0,     0,   290,     0,
       0,     0,     0,     0,     0,   291,     0,   296,     0,   297,
     298,   299,     0,   300,   301,   302,   292,   303,   304,   305,
     306,   307,   308,   309,   310,   311,     0,   312,   313,   314,
       0,     0,   315,   316,   317,     0,   290,     0,     0,     0,
       0,     0,     0,   291,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   292,     0,     0,     0,     0,     0,
     293,     0,     0,     0,   294,     0,     0,     0,     0,     0,
       0,     0,     0,   964,     0,   290,     0,     0,     0,     0,
       0,     0,   291,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   295,   292,     0,     0,     0,     0,   293,     0,
       0,     0,   294,     0,     0,     0,     0,   296,     0,   297,
     298,   299,     0,   300,   301,   302,     0,   303,   304,   305,
     306,   307,   308,   309,   310,   311,     0,   312,   313,   314,
     295,     0,   315,   316,   317,     0,     0,   638,     0,     0,
       0,   294,     0,     0,     0,   296,     0,   297,   298,   299,
       0,   300,   301,   302,     0,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,     0,   295,
     315,   316,   317,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   296,     0,   297,   298,   299,     0,
     300,   301,   302,     0,   303,   304,   305,   306,   307,   308,
     309,   310,   311,    16,   312,   313,   314,    20,   292,   315,
     316,   317,     0,    16,     0,     0,    26,    20,    27,   833,
       0,     0,    31,     0,     0,     0,    26,    35,    27,   833,
    -190,     0,    31,     0,     0,     0,     0,    35,     0,     0,
    -190,     0,     0,     0,    45,     0,     0,     0,    48,     0,
     834,  -190,   293,   835,    45,    53,   294,     0,   274,     0,
     834,  -190,     0,   835,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,    69,     0,     0,
       0,     0,     0,     0,    67,     0,     0,    69,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   296,
       0,   297,   298,   299,     0,   300,   301,   302,     0,     0,
     304,   305,   306,     0,   308,   309,   310,   311,     0,     0,
       0,   314,     0,     0,   315,   316,   317,    84,     0,     0,
       0,     0,     0,     0,     0,   922,     0,    84,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     332,   333,     0,     0,     0,  -486,     0,  -486
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-888)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,     2,   413,   131,    15,   381,    54,    18,    50,    51,
     384,   688,    23,   168,   204,   339,    27,    28,    29,    30,
      31,    32,    67,    71,   179,   610,   279,    29,   391,    40,
      41,    42,   217,    44,    17,    46,    47,   760,   761,    29,
      88,     1,    41,    54,    55,    56,    57,    58,    59,   535,
      29,   236,    63,    64,    65,    66,   271,     1,    75,    70,
      71,    72,    73,    41,    41,    76,    58,   112,    79,    80,
      81,   890,    83,    57,    41,    86,    87,    88,   284,   285,
     286,   287,    29,    41,    41,   723,   724,   375,     1,    41,
      75,   931,    46,   124,   981,     1,   124,     0,    58,   976,
      55,   100,    19,   990,   143,    97,    41,    75,   156,   152,
     100,   154,     1,    67,   100,   243,     3,   994,   100,    87,
     151,    73,   124,   168,   801,   124,   143,   155,    15,    90,
      98,   419,    87,   150,   179,    22,   976,    97,    73,   150,
     151,   152,   153,   154,   155,   156,   124,   124,   150,   635,
     335,   336,    41,    40,    71,    42,   155,   124,    45,   151,
     150,   105,    75,   174,   983,   150,   124,   124,    19,    75,
      24,   150,   152,   155,    87,   143,    93,   155,   155,   123,
     124,     7,   150,    70,   167,    98,   124,   561,   155,   912,
     913,   151,    98,   778,   779,    82,   873,   155,    68,   837,
     184,   185,   186,   150,    91,    92,   217,   218,    62,   153,
     123,   535,   124,   151,   225,   226,   105,   123,    24,    73,
      71,   406,   860,   861,   105,   236,   210,   105,   105,   213,
     143,   124,   124,   124,   123,   124,    90,   150,   152,   124,
     446,   153,    93,   124,   150,   124,   124,   128,   232,    72,
     128,   128,   143,    76,   143,   266,    62,    87,   124,    72,
     153,   153,   263,    76,   143,   152,    83,    73,   153,   253,
     523,   524,   153,   126,    29,   153,   152,   143,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
      96,   302,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   793,   684,   124,
     124,   635,   686,   324,   325,   124,   364,   362,   533,   534,
     124,   332,   333,   334,   335,   336,   337,   338,   339,    65,
      66,   124,   124,   696,   143,   124,   143,   348,   153,   153,
     143,   352,    28,   354,   174,   143,   143,   723,   724,   153,
     149,   152,   124,   364,   143,   366,   367,   143,   369,   370,
     153,   153,    24,   374,   375,   349,   377,   143,   379,   324,
     325,   143,   562,   143,   143,   124,    28,   332,   333,   334,
      75,   392,   151,   598,   599,   143,    72,   217,   399,   373,
      76,   124,    87,   441,   143,   406,   124,   143,   150,   143,
      62,   812,   765,    98,    75,   151,   236,   418,   419,    99,
     143,    73,   152,   152,   425,   143,    87,   428,   143,   430,
      72,   432,   433,   434,    76,   436,   437,    98,    90,   152,
     441,   154,   152,   119,   589,   121,   122,   123,   389,   125,
     126,     2,   805,   806,   130,   143,   105,   637,   143,   143,
      11,   137,   463,   464,   152,   150,   154,     7,   144,   145,
     146,    22,   413,    24,   152,    40,   154,   843,    52,   793,
     123,   123,   143,   428,   126,   430,   150,    41,   130,   150,
     123,   105,    57,   126,   860,   861,   428,   130,   430,   143,
      65,    66,   620,   153,   124,   506,   507,   508,   509,   510,
     511,   512,    63,   693,   143,   335,   336,    41,   519,   872,
     521,    75,   557,    31,   525,   152,   527,   124,   153,   105,
     531,   153,   570,    18,   535,    86,   153,   124,   150,    90,
     513,   514,   155,   150,    98,   143,   155,   143,   293,   294,
     585,   105,   143,   143,   589,   529,   530,   143,   153,   533,
     534,   143,   563,   124,   565,   566,   567,   568,   569,   570,
     124,   143,   573,   143,   519,    31,   153,   930,   151,   399,
     143,   143,   153,   150,   105,   620,   406,   124,    29,   143,
      73,   151,   337,   338,    29,   146,   150,   155,   150,   153,
     150,   166,    31,   348,   123,   425,    73,   155,   428,   123,
     430,   123,     3,   433,   155,    24,   155,   151,    31,   184,
     185,   186,   367,   152,    15,   370,   191,   126,   573,   126,
     153,    22,    41,    98,   635,    44,    28,   638,   639,   640,
     153,   192,   643,    87,   151,   210,   155,   721,   213,    40,
      59,    42,   419,    62,    45,    64,   207,   418,   659,    21,
     661,    28,   707,   664,    73,   703,   667,   232,   727,   244,
     180,   672,   707,   674,   648,   649,   677,   847,   843,    70,
      72,    90,   656,   730,    76,   684,   983,   432,   253,   554,
     930,    82,   666,   955,    31,   669,   393,   521,    27,   519,
      91,   521,   703,   248,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,   263,    -1,   715,   124,   717,    -1,    -1,   720,
      -1,    -1,    -1,    -1,    -1,   726,    -1,   119,    -1,   847,
     122,   123,    -1,   125,   126,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    -1,   153,   137,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,   756,   123,    -1,   125,   126,
      -1,    -1,   507,   130,   509,    -1,    -1,    -1,   769,   770,
     137,   772,   773,   774,   775,    -1,    23,   144,   145,   146,
     781,   782,    -1,   784,   349,   786,   787,    34,    35,   790,
      -1,    -1,   793,    -1,    -1,   346,    43,    -1,    -1,    46,
      28,    -1,   834,   835,    -1,    52,    -1,    -1,   373,    -1,
     811,   812,   847,    60,    -1,   366,   817,   818,   369,   820,
      67,    68,    -1,   824,    -1,    41,   377,    -1,    -1,    -1,
      -1,    -1,   833,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     831,   815,    -1,   881,    72,    -1,    -1,    -1,    76,    -1,
     851,   852,   853,   854,   855,   856,   857,   858,    -1,    75,
      -1,    -1,    -1,   804,    -1,    -1,    -1,    -1,   869,    -1,
      -1,   812,    -1,   874,    -1,   876,   877,   878,   879,   880,
     881,    -1,    98,   434,   885,    -1,    -1,    -1,   889,   105,
      -1,   119,     2,   638,   639,   123,    -1,   125,   126,    -1,
      -1,    11,   130,    -1,    -1,    -1,    -1,    -1,   124,   137,
      -1,    -1,    22,   951,    24,    -1,   144,   145,   146,    -1,
      -1,    -1,   923,   924,   925,   926,    -1,   143,    -1,    28,
      -1,    -1,    -1,    -1,   150,    -1,    -1,   153,    -1,   940,
      -1,   942,   943,   944,    -1,   946,   947,   948,   949,   950,
     951,    -1,   503,    63,    -1,    65,    66,    -1,   959,   960,
     961,   962,   963,   964,   529,   530,   907,    -1,   533,   534,
      -1,    -1,    -1,    72,   975,   976,    86,    76,     2,    -1,
      90,   546,    -1,    -1,   804,    -1,    -1,    11,    -1,   990,
     931,    -1,    -1,   994,   545,    -1,    -1,    -1,    22,   940,
      24,    -1,    -1,    41,     1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
     585,    -1,    -1,    -1,   123,    22,   125,   126,    -1,    -1,
      -1,   130,    -1,    -1,    -1,   976,   146,    75,   137,    63,
     981,    24,    -1,    40,    -1,    42,    -1,   146,    45,   990,
      -1,    -1,   603,   994,    -1,    -1,    -1,    -1,    41,    -1,
      98,    44,    86,    -1,    -1,   885,    90,   105,    -1,   620,
      -1,    -1,    -1,    70,    -1,    -1,    59,    -1,    -1,    62,
      -1,    64,   192,   648,   649,    82,   124,   638,   639,    24,
      73,   656,    -1,   658,    91,    -1,    -1,   207,   663,    -1,
      -1,   666,    -1,    -1,   669,   143,    41,    90,   659,    44,
     661,   931,   150,   664,    -1,   153,   667,    -1,    -1,    -1,
       3,    -1,   146,    -1,    59,   676,    -1,    62,   679,    64,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    73,    22,
      -1,    -1,    -1,     2,    -1,    -1,   711,    30,    -1,    23,
      -1,   702,    11,   263,    -1,    90,   976,    40,    -1,    42,
      34,    35,    45,    22,   715,    24,   717,    -1,   192,    43,
     721,    -1,    46,     3,   994,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    -1,   207,    -1,    15,    60,    70,    -1,    -1,
      73,    -1,    22,    67,    -1,    -1,    79,    80,    -1,    82,
      30,    84,    -1,    -1,    63,    -1,    -1,    -1,    91,    23,
      40,    94,    42,    -1,    -1,    45,    -1,    -1,    -1,    -1,
      34,    35,    -1,    -1,    -1,    -1,    -1,    86,    -1,    43,
     781,    90,    46,    -1,    -1,   786,   346,    -1,    52,   263,
      70,    -1,    -1,    -1,    -1,    -1,    60,    -1,     1,   800,
     815,    -1,    82,    67,    68,    -1,   366,    -1,    -1,   369,
      90,    91,   813,    -1,    -1,    -1,    19,   377,    -1,   152,
      23,    24,   823,    -1,    -1,    -1,   827,    -1,    -1,    32,
     831,    34,    35,    -1,    -1,    38,    -1,   146,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    -1,   863,   864,
      -1,   866,   867,    -1,    -1,    -1,    -1,    60,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    -1,    71,   870,
      73,    -1,   346,    -1,   434,   890,    -1,    -1,    -1,    -1,
      -1,   896,    -1,   192,   899,     2,   901,    90,    -1,    -1,
      93,    -1,   366,    96,    11,   369,    -1,    -1,   207,    -1,
      -1,    -1,    -1,   377,    -1,    22,    -1,    24,    -1,    -1,
       2,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,
      -1,    -1,   923,   924,   925,   926,    -1,    -1,    -1,    -1,
      22,    -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,   942,   943,    -1,    -1,    -1,    63,    -1,   151,    -1,
      -1,    -1,    -1,    -1,   263,    -1,    -1,    -1,    -1,    -1,
     434,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   983,    86,
      -1,    63,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   545,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,   146,
      -1,    -1,    30,    -1,    -1,    -1,    -1,   346,    36,    37,
      -1,    39,    40,   603,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,   146,    53,    54,   366,    -1,    -1,
     369,    -1,    -1,    61,    -1,    63,    -1,    65,   377,    -1,
      -1,   545,    70,    -1,    -1,   192,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
     207,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,   659,
     192,   661,    -1,    -1,   664,    -1,    -1,   667,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   207,   676,    -1,    -1,   679,
      -1,   119,   120,   121,   122,   434,    -1,    -1,    -1,   603,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
     138,    -1,   702,   141,    -1,    -1,   263,    -1,   146,    -1,
      -1,    -1,   150,    -1,   152,   715,   154,   717,    -1,    19,
      -1,   721,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   263,    32,    -1,    34,    35,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   659,    46,   661,    -1,    -1,
     664,    -1,    -1,   667,    -1,    -1,    -1,    -1,    -1,    -1,
      60,    -1,   676,    -1,    64,   679,    66,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   781,    -1,    -1,    -1,    -1,   786,    -1,   702,   346,
      90,    -1,    -1,    93,    -1,    -1,   545,    -1,    -1,    -1,
     800,   715,    -1,   717,    -1,    -1,    -1,   721,    -1,   366,
      -1,    -1,   369,   813,   346,    -1,    -1,    -1,    -1,    -1,
     377,    -1,    -1,   823,    -1,    -1,    -1,   827,    -1,    -1,
      -1,   831,    -1,    -1,   366,    -1,    -1,   369,    -1,    -1,
      -1,    -1,    -1,   143,    -1,   377,    -1,    -1,    -1,    -1,
      -1,   151,    -1,     2,   603,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    -1,    -1,    -1,    -1,   781,    -1,    -1,
     870,    -1,   786,    22,    -1,    24,    -1,   434,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   800,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   813,
      -1,    -1,   434,    -1,    -1,    -1,    -1,    -1,    -1,   823,
     659,    -1,   661,   827,    63,   664,    -1,   831,   667,    -1,
      -1,    -1,    -1,   923,   924,   925,   926,   676,    -1,    -1,
     679,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    90,   942,   943,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   702,    -1,    -1,   870,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   715,    -1,   717,    -1,
      -1,    -1,   721,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,   545,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    22,    -1,    24,    -1,    -1,    -1,    -1,     3,   923,
     924,   925,   926,   545,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,   942,   943,
      -1,    -1,   781,    -1,    -1,    30,    -1,   786,    -1,    -1,
       2,    -1,    63,   192,    -1,    40,   603,    42,    -1,    11,
      45,   800,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,
      22,    -1,    24,    -1,   813,    86,    61,    -1,    -1,    90,
      -1,   603,    -1,    -1,   823,    70,    -1,    -1,   827,    -1,
      -1,    -1,   831,    -1,    79,    80,    -1,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,
      -1,    63,   659,    -1,   661,    28,    -1,   664,    -1,    -1,
     667,    -1,    -1,    -1,   263,    -1,    -1,    -1,    -1,   676,
      -1,   870,   679,    -1,    86,   146,    -1,   659,    90,   661,
      -1,    -1,   664,    -1,    -1,   667,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   676,   702,    -1,   679,    -1,    72,
      -1,    -1,    -1,    76,    -1,   150,    -1,   152,   715,    -1,
     717,    -1,    -1,    -1,   721,    -1,    -1,    -1,    -1,    -1,
     702,   192,    -1,    -1,   923,   924,   925,   926,    -1,    -1,
      -1,   104,    -1,   715,   146,   717,   207,    -1,    -1,   721,
      -1,    -1,    -1,   942,   943,    -1,   119,   346,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
      -1,   134,   135,   136,   137,    -1,   139,   366,   141,    -1,
     369,   144,   145,   146,   781,    -1,    -1,    -1,   377,   786,
     192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,   263,   800,    -1,   207,    -1,    -1,    -1,   781,
      15,    16,    -1,    -1,   786,    -1,   813,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,   823,    -1,   800,    -1,
     827,    -1,    -1,    -1,   831,    40,    13,    42,    -1,    -1,
      45,   813,    -1,    -1,    21,   434,    -1,    -1,    -1,    -1,
      -1,   823,    -1,    -1,    -1,   827,    61,    -1,    -1,   831,
      37,   263,    39,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   870,    79,    80,    -1,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,   346,    91,    -1,    -1,    94,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   870,    -1,
      -1,    -1,    -1,    -1,    -1,   366,    -1,    -1,   369,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   377,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   923,   924,   925,   926,
      -1,    -1,    -1,    -1,    -1,   112,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,   346,   942,   943,   152,    -1,    -1,
       3,   923,   924,   925,   926,    -1,   545,    -1,    -1,    -1,
      -1,    -1,    15,    16,   366,    -1,    -1,   369,    -1,    22,
     942,   943,    -1,   434,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,     1,    42,
      -1,   168,    45,    -1,    -1,    -1,    -1,   174,    -1,    -1,
      -1,    -1,   179,    -1,    -1,   182,    19,    -1,    61,    -1,
      23,    24,    -1,    -1,   603,    -1,    -1,    70,    -1,    32,
      -1,    34,    35,    -1,   201,    38,    79,    80,    -1,    82,
      43,    84,   434,    46,    -1,    -1,    -1,    -1,    91,    -1,
     217,    94,    -1,    -1,   221,    -1,    -1,    60,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    -1,    71,   236,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     659,    28,   661,    -1,    -1,   664,    -1,    90,   667,    -1,
      93,    -1,    -1,    96,   545,    -1,    -1,   676,    -1,    -1,
     679,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   284,   285,   286,
     287,    -1,    -1,   702,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,   301,    -1,   715,    -1,   717,    -1,
     143,    -1,   721,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,   603,   545,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   335,   336,
      -1,    -1,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,    -1,   134,   135,   136,
     137,    -1,   139,   140,   141,   362,    -1,   144,   145,   146,
      -1,    -1,   781,    -1,    -1,    -1,    -1,   786,   659,    -1,
     661,   603,    -1,   664,    -1,    -1,   667,   384,     3,    -1,
      -1,   800,    -1,    -1,    -1,   676,   393,    -1,   679,    -1,
      15,    16,    -1,    -1,   813,    -1,    -1,    22,    -1,   406,
      -1,    -1,    -1,    -1,   823,    30,    -1,    -1,   827,    -1,
      -1,   702,   831,    -1,    -1,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    -1,   715,    -1,   717,   659,    -1,   661,
     721,    -1,   664,   440,    -1,   667,    61,   444,    -1,   446,
      -1,    -1,    -1,    -1,   676,    70,    -1,   679,    -1,    -1,
      -1,   870,    -1,    -1,    79,    80,    -1,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,
     702,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,   715,    -1,   717,    -1,    -1,    17,   721,
     781,    -1,    -1,    -1,    -1,   786,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,   923,   924,   925,   926,    -1,   800,
      -1,    -1,    -1,    -1,   521,    19,    -1,    -1,    -1,    23,
      -1,    -1,   813,   942,   943,    -1,    -1,    -1,    32,    -1,
      34,    35,   823,    -1,    38,    -1,   827,    -1,    -1,    43,
     831,    -1,    46,    72,    -1,    -1,    -1,    76,    -1,    -1,
     557,    -1,    -1,    -1,   561,    -1,    60,    -1,    -1,    -1,
      64,    -1,    66,    67,    -1,    69,    -1,    71,   800,    -1,
      -1,   100,    -1,    -1,    -1,   104,    28,    -1,   585,   870,
      -1,   813,   589,    -1,    -1,    -1,    90,    -1,    -1,    93,
     119,   823,   121,   122,   123,   827,   125,   126,   127,   831,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,   620,    -1,   144,   145,   146,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,   155,    -1,    -1,    -1,
      -1,    -1,   923,   924,   925,   926,    -1,    -1,   870,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,   942,   943,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,   683,    -1,   130,   686,
      -1,    -1,    -1,    -1,   136,   137,    -1,    -1,    -1,   141,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
     707,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       0,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
     727,    -1,    12,    13,    14,    15,    16,    -1,   735,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    -1,    42,    43,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    -1,    56,    -1,    -1,    -1,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    -1,    86,    -1,    88,    89,
      90,    91,    -1,    93,    94,    95,    96,    -1,    -1,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
     847,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,
     140,   141,    -1,   143,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,     1,   154,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
     138,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,    -1,   150,    -1,   152,    -1,   154,   155,     1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    10,    -1,    12,
      13,    14,    15,    16,    17,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      43,    -1,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    -1,    56,    -1,    -1,    -1,    60,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    -1,    86,    -1,    88,    89,    90,    91,    -1,
      93,    94,    95,    96,    -1,    -1,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
     123,   124,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,    -1,
     143,   144,   145,   146,    -1,    -1,    -1,   150,   151,   152,
       1,   154,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    43,    -1,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    -1,    56,    -1,    -1,    -1,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    -1,    86,    -1,    88,    89,    90,
      91,    -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,
     141,   142,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,
     151,   152,     1,   154,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    43,    -1,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    -1,    56,    -1,    -1,
      -1,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    -1,    86,    -1,    88,
      89,    90,    91,    -1,    93,    94,    95,    96,    -1,    -1,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,
      -1,   140,   141,    -1,   143,    -1,    -1,   146,    -1,    -1,
      -1,   150,   151,   152,     1,   154,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    -1,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    -1,    56,
      -1,    -1,    -1,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    -1,    86,
      -1,    88,    89,    90,    91,    -1,    93,    94,    95,    96,
      -1,    -1,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
      -1,   138,    -1,   140,   141,    -1,   143,    -1,    -1,   146,
      -1,    -1,    -1,   150,   151,   152,     1,   154,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    -1,    42,    43,    -1,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      -1,    56,    -1,    -1,    -1,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      -1,    86,    -1,    88,    89,    90,    91,    -1,    93,    94,
      95,    96,    -1,    -1,    99,    -1,   101,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,   138,    -1,   140,   141,    -1,   143,    -1,
      -1,   146,    -1,    -1,    -1,   150,   151,   152,     1,   154,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      43,    -1,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    -1,    56,    -1,    -1,    -1,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    -1,    86,    -1,    88,    89,    90,    91,    -1,
      93,    94,    95,    96,    -1,    -1,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,    -1,
     143,    -1,    -1,   146,    -1,    -1,    -1,   150,   151,   152,
       1,   154,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    43,    -1,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    -1,    56,    -1,    -1,    -1,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    -1,    86,    -1,    88,    89,    90,
      91,    -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,
     141,    -1,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,   152,     1,   154,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    43,    -1,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    -1,    56,    -1,    -1,
      -1,    60,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    -1,    86,    -1,    88,
      89,    90,    91,    -1,    93,    94,    95,    96,    -1,    -1,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,
      -1,   140,   141,    -1,   143,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,     1,   154,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    -1,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    -1,    56,
      -1,    -1,    -1,    60,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    -1,    86,
      -1,    88,    89,    90,    91,    -1,    93,    94,    95,    96,
      -1,    -1,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
      -1,   138,    -1,   140,   141,    -1,   143,    -1,    -1,   146,
      -1,    -1,    -1,   150,    -1,   152,     1,   154,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    -1,    42,    43,    -1,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      -1,    56,    -1,    -1,    -1,    60,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      -1,    86,    -1,    88,    89,    90,    91,    -1,    93,    94,
      95,    96,    -1,    -1,    99,    -1,   101,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,   138,    -1,   140,   141,    -1,   143,    -1,
      -1,   146,    -1,    -1,    -1,   150,    -1,   152,     1,   154,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      43,    -1,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    -1,    56,    -1,    -1,    -1,    60,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    -1,    86,    -1,    88,    89,    90,    91,    -1,
      93,    94,    95,    96,    -1,    -1,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,    -1,
     143,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,
       1,   154,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    43,    -1,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    -1,    56,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    -1,    86,    -1,    88,    89,    90,
      91,    -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,
     141,    -1,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,   152,     1,   154,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    43,    -1,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    -1,    56,    -1,    -1,
      -1,    60,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    -1,    86,    -1,    88,
      89,    90,    91,    -1,    93,    94,    95,    96,    -1,    -1,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,
      -1,   140,   141,    -1,   143,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,     1,   154,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    -1,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    -1,    56,
      -1,    -1,    -1,    60,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    -1,    86,
      -1,    88,    89,    90,    91,    -1,    93,    94,    95,    96,
      -1,    -1,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
      -1,   138,    -1,   140,   141,    -1,   143,    -1,    -1,   146,
      -1,    -1,    -1,   150,    -1,   152,     1,   154,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    -1,    42,    43,    -1,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      -1,    56,    -1,    -1,    -1,    60,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      -1,    86,    -1,    88,    89,    90,    91,    -1,    93,    94,
      95,    96,    -1,    -1,    99,    -1,   101,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,   138,    -1,   140,   141,    -1,   143,    -1,
      -1,   146,    -1,    -1,    -1,   150,    -1,   152,     1,   154,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      43,    -1,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    -1,    56,    -1,    -1,    -1,    60,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    -1,    86,    -1,    88,    89,    90,    91,    -1,
      93,    94,    95,    96,    -1,    -1,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,    -1,
     143,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,
       1,   154,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    43,    -1,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    -1,    56,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    -1,    86,    -1,    88,    89,    90,
      91,    -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,
     141,    -1,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,   152,     1,   154,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
      -1,   138,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,   150,    -1,   152,    -1,   154,   155,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,   138,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,
     154,   155,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,
     141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,   152,    -1,   154,   155,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    19,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    46,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    67,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,     1,
      -1,     3,    90,    91,    -1,    93,    94,    -1,    10,    -1,
      -1,    -1,    -1,    15,    16,    17,   104,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    40,   127,
      42,    -1,    -1,    45,    -1,    -1,    -1,    -1,   136,    -1,
     138,    -1,   140,   141,    -1,    -1,    -1,    -1,   146,    61,
      -1,    -1,   150,    -1,   152,    -1,   154,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    -1,
      82,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,   107,   108,   109,   110,    -1,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    -1,    -1,   125,    -1,    -1,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,   141,
      -1,    -1,   144,   145,   146,   147,    -1,   149,    -1,    -1,
     152,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    19,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    46,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    67,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,
       5,     6,     7,     8,   136,    -1,   138,    12,   140,   141,
      15,    16,    -1,    -1,   146,    -1,    -1,    22,   150,    -1,
     152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      -1,    -1,    -1,    88,    89,    -1,    91,    92,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,   138,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    88,    89,    -1,    91,    92,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,
     153,   154,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    88,    89,    -1,
      91,    92,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,   152,   153,   154,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    41,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    94,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,     3,     4,     5,     6,     7,     8,   136,    -1,   138,
      12,   140,   141,    15,    16,    -1,    -1,   146,    -1,    -1,
      22,   150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    88,    89,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,
      -1,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,
     152,    -1,   154,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    88,    89,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,    -1,   154,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    55,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      88,    89,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,     3,     4,     5,     6,     7,     8,   136,    -1,
     138,    12,   140,   141,    15,    16,    -1,    -1,   146,    -1,
      -1,    22,   150,    -1,   152,    -1,   154,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,
     141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,   152,    -1,   154,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    35,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,     3,     4,     5,     6,     7,     8,   136,    -1,   138,
      12,   140,   141,    15,    16,    -1,    -1,   146,    -1,    -1,
      22,   150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    46,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    67,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,
       5,     6,     7,     8,   136,    -1,   138,    12,   140,   141,
      15,    16,    -1,    -1,   146,    -1,    -1,    22,   150,    -1,
     152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      -1,    -1,    -1,    88,    89,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,     7,
       8,   136,    -1,   138,    12,   140,   141,    15,    16,    -1,
      -1,   146,    -1,    -1,    22,   150,    -1,   152,    -1,   154,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
     138,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,    -1,   150,   151,   152,    -1,   154,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,     3,     4,     5,     6,     7,     8,
     136,    -1,   138,    12,   140,   141,    15,    16,    -1,    -1,
     146,    -1,    -1,    22,   150,    -1,   152,    -1,   154,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,     3,     4,     5,     6,     7,     8,   136,
      -1,   138,    12,   140,   141,    15,    16,    -1,    -1,   146,
      -1,    -1,    22,   150,    -1,   152,    -1,   154,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,    -1,   154,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
     138,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,    -1,   150,    -1,   152,    -1,   154,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,   138,    -1,   140,   141,   142,    -1,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,
       7,     8,   136,    -1,   138,    12,   140,   141,    15,    16,
      -1,    -1,   146,    -1,    -1,    22,   150,    -1,   152,    -1,
     154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,     3,     4,     5,     6,     7,     8,   136,
      -1,   138,    12,   140,   141,    15,    16,    -1,    -1,   146,
      -1,    -1,    22,   150,    -1,   152,    -1,   154,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
       3,     4,     5,     6,     7,     8,   136,    -1,   138,    12,
     140,   141,    15,    16,    -1,    -1,   146,    -1,    -1,    22,
     150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,
       6,     7,     8,   136,    -1,   138,    12,   140,   141,    15,
      16,    -1,    -1,   146,    -1,    -1,    22,   150,    -1,   152,
      -1,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,     3,     4,     5,     6,     7,     8,
     136,    -1,   138,    12,   140,   141,    15,    16,    -1,    -1,
     146,    -1,    -1,    22,   150,    -1,   152,    -1,   154,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,     3,     4,     5,     6,     7,     8,   136,    -1,   138,
      12,   140,   141,    15,    16,    -1,    -1,   146,    -1,    -1,
      22,   150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,
       5,     6,     7,     8,   136,    -1,   138,    12,   140,   141,
      15,    16,    -1,    -1,   146,    -1,    -1,    22,   150,    -1,
     152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,     7,
       8,   136,    -1,   138,    12,   140,   141,    15,    16,    -1,
      -1,   146,    -1,    -1,    22,   150,    -1,   152,    -1,   154,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,     3,     4,     5,     6,     7,     8,   136,    -1,
     138,    12,   140,   141,    15,    16,    -1,    -1,   146,    -1,
      -1,    22,   150,    -1,   152,    -1,   154,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,
       4,     5,     6,     7,     8,   136,    -1,   138,    12,   140,
     141,    15,    16,    -1,    -1,   146,    -1,    -1,    22,   150,
      -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,
       7,     8,   136,    -1,   138,    12,   140,   141,    15,    16,
      -1,    -1,   146,    -1,    -1,    22,   150,    -1,   152,    -1,
     154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,     3,     4,     5,     6,     7,     8,   136,
      -1,   138,    12,   140,   141,    15,    16,    -1,    -1,   146,
      -1,    -1,    22,   150,    -1,   152,    -1,   154,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
       3,     4,     5,     6,     7,     8,   136,    -1,   138,    12,
     140,   141,    15,    16,    -1,    -1,   146,    -1,    -1,    22,
     150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,
       6,     7,     8,   136,    -1,   138,    12,   140,   141,    15,
      16,    -1,    -1,   146,    -1,    -1,    22,   150,    -1,   152,
      -1,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,     3,     4,     5,     6,     7,     8,
     136,    -1,   138,    12,   140,   141,    15,    16,    -1,    -1,
     146,    -1,    -1,    22,   150,    -1,   152,    -1,   154,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,     3,     4,     5,     6,     7,     8,   136,    -1,   138,
      12,   140,   141,    15,    16,    -1,    -1,   146,    -1,    -1,
      22,    -1,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,   105,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,   120,    -1,
      -1,    -1,   124,    30,    -1,    -1,   128,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,   138,    -1,    45,    -1,
     142,    48,    49,    -1,    51,    -1,    53,    54,   150,    -1,
     152,   153,   154,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      87,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    -1,    -1,   120,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,   138,    -1,    -1,    22,    -1,   143,    -1,    -1,   147,
     148,   149,    30,   150,   152,   152,   154,   154,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    94,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
     138,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,   150,    -1,   152,    61,   154,    63,    -1,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      28,    -1,    -1,    -1,   120,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,   138,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,   150,    -1,   152,    61,   154,    63,
      -1,    65,    -1,    -1,    72,    -1,    70,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,     3,    -1,    -1,    -1,    -1,    91,    -1,    10,
      94,    -1,    -1,    -1,    15,    16,    17,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,   119,    -1,   121,   122,   123,   120,   125,   126,    40,
      -1,    42,   130,    -1,    45,    -1,    -1,    -1,   136,   137,
      -1,    -1,    -1,   141,   138,    -1,   144,   145,   146,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    70,
     154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      -1,    82,    -1,    84,    -1,    10,    -1,    -1,    -1,    -1,
      91,    -1,    17,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,   105,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    -1,    -1,   125,    -1,    -1,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,    -1,
     141,    -1,    -1,   144,   145,   146,   147,    72,   149,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,    72,   144,
     145,   146,    76,    -1,    -1,    -1,    -1,    10,    -1,    -1,
     155,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,    72,
     144,   145,   146,    76,    -1,    -1,    -1,    -1,    10,    -1,
      -1,   155,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
      72,   144,   145,   146,    76,    -1,    -1,    -1,    -1,    10,
      -1,    -1,   155,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      -1,    72,   144,   145,   146,    76,    -1,    -1,    -1,    -1,
      10,   153,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,    72,   144,   145,   146,    76,    -1,    -1,    -1,
      -1,    10,   153,    -1,    -1,    85,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,   104,   105,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,   117,   118,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,    72,   144,   145,   146,    76,    -1,    -1,
     150,    -1,    10,    -1,    -1,    -1,    85,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,   104,   105,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,   117,   118,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,    72,   144,   145,   146,    76,    -1,
      -1,   150,    -1,    10,    -1,    -1,    -1,    85,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   104,   105,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,   117,
     118,   119,    -1,   121,   122,   123,    -1,   125,   126,   127,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,   139,   140,   141,    -1,    72,   144,   145,   146,    76,
      -1,    -1,   150,    -1,    10,    -1,    -1,    -1,    85,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,   104,   105,   106,
     107,   108,   109,   110,    -1,    -1,   113,   114,   115,   116,
     117,   118,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,    72,   144,   145,   146,
      76,    -1,    -1,   150,    -1,    10,    -1,    -1,    -1,    85,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   104,   105,
     106,   107,   108,   109,   110,    -1,    -1,   113,   114,   115,
     116,   117,   118,   119,    -1,   121,   122,   123,    -1,   125,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    -1,    72,   144,   145,
     146,    76,    -1,    -1,   150,    -1,    10,    -1,    -1,    -1,
      85,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   104,
     105,   106,   107,   108,   109,   110,    -1,    -1,   113,   114,
     115,   116,   117,   118,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,    72,   144,
     145,   146,    76,    -1,    -1,   150,    -1,    10,    -1,    -1,
      -1,    85,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     104,   105,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,   117,   118,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,    72,
     144,   145,   146,    76,    -1,    -1,   150,    -1,    10,    -1,
      -1,    -1,    85,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   104,   105,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,   117,   118,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
      72,   144,   145,   146,    76,    -1,    -1,   150,    -1,    10,
      -1,    -1,    -1,    85,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,
      -1,    -1,   104,   105,   106,   107,   108,   109,   110,    -1,
      41,   113,   114,   115,   116,   117,   118,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      -1,    72,   144,   145,   146,    76,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,   100,
      -1,    -1,    -1,   104,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,    -1,   144,   145,   146,    -1,    -1,    72,   150,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,    72,
     144,   145,   146,    76,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,   100,    -1,    -1,
      -1,   104,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
      -1,   144,   145,   146,    28,    -1,    72,   150,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    76,   119,    -1,   121,   122,   123,    -1,   125,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    -1,    72,   144,   145,
     146,    76,    -1,    -1,   150,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    28,   129,   130,   131,   132,   104,
     134,   135,   136,   137,    -1,   139,    -1,   141,    -1,    -1,
     144,   145,   146,    -1,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,    72,   144,
     145,   146,    76,    -1,    -1,   150,    -1,    10,    -1,    -1,
      -1,    85,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,    72,
     144,   145,   146,    76,    -1,    -1,   150,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
      72,   144,   145,   146,    76,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    57,   139,   140,   141,
      -1,    10,   144,   145,   146,    -1,    -1,   149,    17,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    72,   125,   126,   127,    76,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,   146,    10,    -1,    -1,    -1,
      -1,   100,    -1,    17,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    33,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    57,    -1,   144,   145,   146,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      72,   125,   126,   127,    76,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    10,    -1,
     144,   145,   146,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      10,    -1,   144,   145,   146,    -1,    -1,    17,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    28,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    72,   125,   126,   127,    76,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,   146,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,   104,    -1,    72,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   104,   139,
     140,   141,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,   122,   123,    72,   125,
     126,   127,    76,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    10,   143,   144,   145,
     146,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    10,   143,
     144,   145,   146,    -1,    -1,    17,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      72,   125,   126,   127,    76,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    10,   143,
     144,   145,   146,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,   103,   104,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      10,    -1,   144,   145,   146,    -1,    -1,    17,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    72,   125,   126,   127,    76,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      -1,    -1,   144,   145,   146,    -1,    10,    11,    -1,    -1,
     100,    -1,    -1,    17,   104,    -1,    72,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   104,   139,
     140,   141,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,   122,   123,    72,   125,
     126,   127,    76,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    10,   143,   144,   145,
     146,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    28,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    10,    -1,
     144,   145,   146,    -1,    -1,    17,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    29,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      72,   125,   126,   127,    76,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,    -1,    72,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   104,   139,   140,   141,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,   121,   122,   123,    -1,   125,   126,   127,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,   139,   140,   141,    10,    -1,   144,   145,   146,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    23,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    34,    35,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    10,    46,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    28,    64,    -1,    66,    67,    72,    69,    -1,    71,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    90,    17,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      28,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,   119,    -1,   121,   122,   123,    85,   125,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    -1,   104,   144,   145,
     146,   143,    -1,    -1,    72,    -1,    -1,    -1,    76,   151,
      -1,    -1,   119,    -1,   121,   122,   123,    85,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,   104,   144,   145,   146,
      -1,    -1,    10,    11,    -1,    -1,    -1,    -1,    -1,    17,
      -1,   119,    -1,   121,   122,   123,    -1,   125,   126,   127,
      28,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,   139,   140,   141,    -1,    -1,   144,   145,   146,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    28,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,   119,    -1,   121,   122,   123,    85,   125,   126,   127,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,   139,   140,   141,    -1,   104,   144,   145,   146,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    85,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,   104,   144,   145,   146,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    28,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,    -1,   144,   145,   146,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    28,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,   119,
      -1,   121,   122,   123,    85,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,   104,   144,   145,   146,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    85,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,   104,   144,   145,   146,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    28,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      -1,    -1,   144,   145,   146,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    28,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
     104,    -1,   144,   145,   146,    -1,    -1,    72,    -1,    -1,
      -1,    76,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,   104,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    19,   139,   140,   141,    23,    28,   144,
     145,   146,    -1,    19,    -1,    -1,    32,    23,    34,    35,
      -1,    -1,    38,    -1,    -1,    -1,    32,    43,    34,    35,
      46,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,
      66,    67,    72,    69,    60,    71,    76,    -1,    64,    -1,
      66,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,    -1,
     130,   131,   132,    -1,   134,   135,   136,   137,    -1,    -1,
      -1,   141,    -1,    -1,   144,   145,   146,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,    -1,   143,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,    -1,    -1,    -1,   152,    -1,   154
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   160,   161,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    15,    16,    19,    20,    21,    22,
      23,    25,    26,    27,    29,    30,    32,    34,    35,    36,
      37,    38,    39,    40,    42,    43,    45,    47,    48,    49,
      50,    51,    53,    54,    56,    60,    61,    63,    64,    65,
      66,    69,    70,    71,    74,    75,    77,    78,    79,    80,
      81,    82,    83,    84,    86,    88,    89,    90,    91,    93,
      94,    95,    99,   101,   104,   119,   120,   121,   122,   127,
     136,   138,   140,   141,   143,   146,   150,   152,   154,   162,
     163,   164,   165,   166,   168,   173,   174,   175,   177,   178,
     183,   184,   185,   186,   187,   188,   190,   191,   192,   196,
     197,   200,   201,   204,   205,   208,   211,   212,   235,   238,
     239,   259,   260,   261,   262,   263,   264,   272,   273,   274,
     275,   278,   279,   280,   281,   282,   283,   285,   286,   287,
     288,   289,   143,   164,   275,   100,   265,   266,   176,   177,
      12,    36,    37,    39,    84,   150,   154,   208,   259,   263,
     273,   274,   275,   278,   280,   281,   265,   275,    90,   176,
     164,   253,   275,   164,   152,   275,   276,     7,     9,    90,
     181,   276,    62,   102,   189,   275,   275,    23,    35,   238,
     275,   275,   177,   209,    16,    30,    61,    79,    80,    84,
      94,   152,   177,   179,   180,   240,   241,    29,   168,   182,
     275,   275,   275,   275,   275,   275,     7,   152,    35,   185,
     185,   142,   231,   252,   275,    88,    89,   143,   275,   277,
     275,   182,   275,   275,   275,    83,   152,   164,   275,   275,
     168,   175,   275,   278,   168,   175,   275,   177,   180,   236,
     275,   252,   275,   275,   275,   275,   275,   275,   275,   275,
       1,   151,   162,   169,   252,    92,   128,   231,   254,   255,
     277,   252,   275,   284,    64,   164,    68,   167,   177,   180,
     177,    46,    67,   226,    24,    62,    73,    96,   143,   149,
      10,    17,    28,    72,    76,   104,   119,   121,   122,   123,
     125,   126,   127,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   139,   140,   141,   144,   145,   146,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   147,   148,   218,   152,   154,    72,    76,   152,
     164,   143,   275,   275,   275,   252,   150,   168,    41,   265,
     236,   143,   124,   143,    99,   177,   231,   256,   257,   258,
     277,   236,   201,   177,   152,   182,    29,    41,   182,    29,
      41,   100,   182,   268,    33,    57,   143,    85,   168,   218,
     164,   152,   220,    92,   152,   177,   242,   243,     1,   123,
     247,   247,    41,   124,   164,   182,   182,   256,   177,   124,
     143,   275,   275,   143,   150,   182,   152,   256,   143,   193,
     143,   193,   143,   105,   237,   237,   143,   143,    33,    57,
     182,   143,   151,   151,   162,   124,   151,   275,   124,   153,
     124,   153,    41,   124,   155,   268,   103,   124,   155,     7,
      52,   123,   202,   202,   150,   213,    73,   240,   240,   240,
     240,   275,   275,   275,   275,   189,   275,   189,   275,   275,
     275,   275,   275,    16,    94,   275,   275,    30,    90,   177,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   277,   277,   277,   277,
     277,   256,   256,   189,   275,   189,   275,    24,    41,    73,
      96,   267,   270,   271,   275,   289,    29,    41,    29,    41,
      85,    41,   155,   189,   275,   182,   143,   275,   275,   105,
     153,   124,   143,   177,   180,    41,   252,    39,   275,   189,
     275,    39,   275,   189,   275,   152,   182,   146,   170,   172,
     275,   170,   171,   164,   275,    31,   275,    24,    41,    44,
      59,    62,    73,    90,   163,   221,   222,   223,   224,   210,
     243,   124,   153,    16,    30,    61,    79,    80,    84,    94,
     154,   248,   260,   105,   244,   244,   275,   241,   153,   231,
     275,     1,   198,   256,   153,    18,   194,   248,   260,   124,
     172,   171,   151,   153,   153,   254,   153,   254,   189,   275,
     155,   164,   275,   155,   275,   155,   275,   177,   252,   150,
     150,     1,   177,   206,   207,    24,    62,    73,    90,   215,
     225,   240,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   153,   155,    41,    73,   124,   153,   259,    72,    72,
      39,   189,   275,    39,   189,   275,   189,   275,   265,   265,
     143,   231,   277,   258,   202,   202,   275,   153,   275,    29,
     182,    29,   182,   275,    29,   182,   268,    29,   182,   268,
     269,   270,   124,   143,    11,   143,    31,    31,   164,    85,
     168,    41,    73,   223,   124,   153,   152,   177,   180,    24,
      62,    73,    90,   227,   153,   243,   247,     1,   252,    55,
     277,   151,    58,    97,   151,   199,   153,   152,   168,   177,
     180,   195,   236,   143,   143,   155,   268,   155,   268,   164,
     103,   150,     1,   203,   203,   151,   105,   124,   151,    73,
     214,     1,    10,    17,   119,   120,   121,   122,   125,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     141,   144,   145,   146,   147,   149,   152,   177,   180,   216,
     217,   218,   259,   143,   270,   247,   259,   259,   275,    29,
      29,   275,    29,    29,   155,   155,   182,   182,   150,   150,
     182,    85,    39,   275,    39,   275,    85,    39,   275,   182,
      39,   275,   182,   124,   153,   275,   275,   164,   275,   164,
      31,   177,   222,   243,   123,   251,   251,    73,   247,   244,
     155,    41,   155,    29,   164,   252,   195,   123,   123,   168,
      39,   164,   275,   155,    39,   164,   275,   155,   275,     1,
     151,   169,   151,    35,    66,    69,   151,   163,   184,   151,
     275,   207,   227,   152,   219,   177,   275,   126,   219,   219,
     244,    85,    39,    39,    85,    39,    39,    39,    39,     1,
     203,   203,   275,   275,   275,   275,   275,   275,   270,    11,
      31,   164,   251,   153,    16,    30,    61,    79,    80,    84,
      94,   154,   231,   250,   260,   128,   233,   244,   244,    98,
     234,   275,   248,   260,   164,   182,   153,   275,   275,   275,
     164,   275,   164,   151,   151,   184,     1,   123,   246,   221,
     153,   177,   217,   218,   275,   275,   275,   275,   275,   275,
     151,   151,   151,    85,    85,    85,    85,   275,   164,   244,
     251,   155,   252,   231,   232,   275,   275,   168,   183,   230,
     155,   168,    85,    85,    16,    30,    61,    79,    80,    84,
      94,   154,   245,   260,    87,   228,   153,   219,   219,    85,
      85,    85,    85,    85,    85,   275,   275,   275,   275,   233,
     244,   231,   249,   250,   260,    41,   155,   260,   275,   275,
       1,   155,   252,   234,   275,   249,   250,   155,   245,   260,
     155,   143,   229,   230,   155,   245,   260,   249
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
#line 449 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 454 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 455 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 462 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 467 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 468 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 487 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 488 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 489 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 490 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 491 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 492 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 493 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 494 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 495 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 496 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 497 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 498 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 499 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 500 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 502 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 507 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (6)].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[(1) - (6)].b), (yyvsp[(2) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType, (yyvsp[(6) - (7)].pblockstmt), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 512 "chapel.ypp"
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 517 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 518 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 519 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 523 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 524 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 544 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 545 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 550 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 555 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 560 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 564 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 571 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 580 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 589 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 590 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 591 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 595 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 600 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 602 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 617 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 618 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 630 "chapel.ypp"
    { (yyval.pch) = astrBool; }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 631 "chapel.ypp"
    { (yyval.pch) = astrInt; }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 632 "chapel.ypp"
    { (yyval.pch) = astrUint; }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 633 "chapel.ypp"
    { (yyval.pch) = astrReal; }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 634 "chapel.ypp"
    { (yyval.pch) = astrImag; }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 635 "chapel.ypp"
    { (yyval.pch) = astrComplex; }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 636 "chapel.ypp"
    { (yyval.pch) = astrString; }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 637 "chapel.ypp"
    { (yyval.pch) = astrIndex; }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pch) = astr("sync"); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyval.pch) = astr("single"); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 643 "chapel.ypp"
    { (yyval.pch) = astr("owned"); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 644 "chapel.ypp"
    { (yyval.pch) = astr("shared"); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 645 "chapel.ypp"
    { (yyval.pch) = astr("borrowed"); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 646 "chapel.ypp"
    { (yyval.pch) = astr("unmanaged"); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 647 "chapel.ypp"
    { (yyval.pch) = astr("domain"); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 659 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch);
      yyerror(NULL, context, "variable declaration uses reserved word");
    }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 665 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 666 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 670 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 671 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 675 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 676 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 680 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 682 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 683 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 698 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 704 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 710 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 711 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 712 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 713 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 714 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 715 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 716 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 717 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 718 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 719 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 720 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 721 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 722 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 723 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 724 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 725 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 727 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 733 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 739 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 745 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 758 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 767 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 771 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 772 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 773 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 774 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 775 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 777 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 779 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 781 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 786 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 789 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 790 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 791 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 792 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 793 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 794 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 798 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 799 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 803 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 804 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 805 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 809 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 810 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 811 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 812 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 816 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 820 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 822 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 826 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 827 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 832 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 834 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 836 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 843 "chapel.ypp"
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

  case 166:
/* Line 1792 of yacc.c  */
#line 853 "chapel.ypp"
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

  case 167:
/* Line 1792 of yacc.c  */
#line 863 "chapel.ypp"
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

  case 168:
/* Line 1792 of yacc.c  */
#line 873 "chapel.ypp"
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

  case 169:
/* Line 1792 of yacc.c  */
#line 883 "chapel.ypp"
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

  case 170:
/* Line 1792 of yacc.c  */
#line 893 "chapel.ypp"
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

  case 171:
/* Line 1792 of yacc.c  */
#line 905 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 910 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 915 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 923 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 924 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 929 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 931 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 933 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 938 "chapel.ypp"
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

  case 180:
/* Line 1792 of yacc.c  */
#line 954 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 961 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 970 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 978 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 982 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 988 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 989 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 994 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 999 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 1004 "chapel.ypp"
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

  case 190:
/* Line 1792 of yacc.c  */
#line 1024 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 1030 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1037 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[(2) - (2)].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 1043 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[(2) - (2)].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1049 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1060 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1066 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1073 "chapel.ypp"
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

  case 198:
/* Line 1792 of yacc.c  */
#line 1099 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1103 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1108 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1112 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1117 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1124 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1128 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1129 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1130 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1132 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1133 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1134 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1135 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1136 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1137 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1139 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1140 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1141 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1142 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1143 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1144 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1145 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1146 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1147 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1148 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1149 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1150 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1151 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1152 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1153 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1154 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1158 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1159 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1160 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1161 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1162 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1163 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1164 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1165 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1166 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1167 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1168 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1169 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1173 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1174 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1178 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1182 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1183 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1184 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1191 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1193 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1195 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1197 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1199 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1203 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1204 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1208 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1209 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1210 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1213 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1214 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1215 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1216 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1220 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1221 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1225 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1230 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1234 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1235 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1236 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1237 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1238 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1239 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1243 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1244 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1247 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1253 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1258 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1260 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1264 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1269 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1270 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1284 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1289 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
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

  case 301:
/* Line 1792 of yacc.c  */
#line 1332 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1336 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1341 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1349 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1357 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1365 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1374 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1383 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1384 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1391 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1399 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1401 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1403 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1408 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1410 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1412 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1417 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1419 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1421 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1427 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1428 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1429 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1435 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1437 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1439 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1443 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1447 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1449 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1453 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1460 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1461 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1462 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1463 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1464 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1465 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1466 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1467 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1469 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1470 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1476 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1479 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1480 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1481 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1514 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1522 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1543 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1552 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1553 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1554 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1555 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1556 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1557 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1559 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1560 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1561 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1568 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1569 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1570 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1574 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1575 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1579 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1580 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1586 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1600 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1601 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1602 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1607 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1624 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1630 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1634 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1636 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1638 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1640 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1642 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1644 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1649 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1651 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1653 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1657 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1659 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1661 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1663 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1665 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1667 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1669 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1671 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1673 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1680 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1686 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1692 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1698 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1707 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1732 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1736 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1741 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1745 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1746 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1750 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1754 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1755 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1760 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1764 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1768 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1775 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1776 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1777 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1778 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1779 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1785 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1790 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1795 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1811 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1813 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1815 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1817 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1819 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1821 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1823 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1825 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1829 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1830 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1833 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1834 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1835 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1857 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1858 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1859 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1863 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1864 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1865 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1873 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1874 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1875 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1876 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1880 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1881 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1882 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1883 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1884 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1885 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1886 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1887 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1888 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1890 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 1894 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1901 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1902 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1906 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1907 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1908 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1909 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1910 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1911 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1912 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1913 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1914 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1915 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 1916 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 1917 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 523:
/* Line 1792 of yacc.c  */
#line 1918 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 1919 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 1920 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 1921 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 1922 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 1923 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 1924 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 1925 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 1926 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 1927 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 1928 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 1932 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 1933 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 1934 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 1935 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 1936 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 1937 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 1941 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 1942 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 1943 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 1944 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 1948 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 1949 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 1950 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 1951 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 1956 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 1957 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 1958 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 1959 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 1960 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 1961 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 1962 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 9118 "bison-chapel.cpp"
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


