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
     TLOCALE = 306,
     TMINUSMINUS = 307,
     TMODULE = 308,
     TNEW = 309,
     TNIL = 310,
     TNOINIT = 311,
     TON = 312,
     TONLY = 313,
     TOTHERWISE = 314,
     TOUT = 315,
     TOVERRIDE = 316,
     TOWNED = 317,
     TPARAM = 318,
     TPLUSPLUS = 319,
     TPRAGMA = 320,
     TPRIMITIVE = 321,
     TPRIVATE = 322,
     TPROC = 323,
     TPROTOTYPE = 324,
     TPUBLIC = 325,
     TREAL = 326,
     TRECORD = 327,
     TREDUCE = 328,
     TREF = 329,
     TREQUIRE = 330,
     TRETURN = 331,
     TSCAN = 332,
     TSELECT = 333,
     TSERIAL = 334,
     TSHARED = 335,
     TSINGLE = 336,
     TSPARSE = 337,
     TSTRING = 338,
     TSUBDOMAIN = 339,
     TSYNC = 340,
     TTHEN = 341,
     TTHIS = 342,
     TTHROW = 343,
     TTHROWS = 344,
     TTRY = 345,
     TTRYBANG = 346,
     TTYPE = 347,
     TUINT = 348,
     TUNDERSCORE = 349,
     TUNION = 350,
     TUNMANAGED = 351,
     TUSE = 352,
     TVAR = 353,
     TWHEN = 354,
     TWHERE = 355,
     TWHILE = 356,
     TWITH = 357,
     TYIELD = 358,
     TZIP = 359,
     TALIAS = 360,
     TAND = 361,
     TASSIGN = 362,
     TASSIGNBAND = 363,
     TASSIGNBOR = 364,
     TASSIGNBXOR = 365,
     TASSIGNDIVIDE = 366,
     TASSIGNEXP = 367,
     TASSIGNLAND = 368,
     TASSIGNLOR = 369,
     TASSIGNMINUS = 370,
     TASSIGNMOD = 371,
     TASSIGNMULTIPLY = 372,
     TASSIGNPLUS = 373,
     TASSIGNREDUCE = 374,
     TASSIGNSL = 375,
     TASSIGNSR = 376,
     TBAND = 377,
     TBNOT = 378,
     TBOR = 379,
     TBXOR = 380,
     TCOLON = 381,
     TCOMMA = 382,
     TDIVIDE = 383,
     TDOT = 384,
     TDOTDOT = 385,
     TDOTDOTDOT = 386,
     TEQUAL = 387,
     TEXP = 388,
     TGREATER = 389,
     TGREATEREQUAL = 390,
     THASH = 391,
     TIO = 392,
     TLESS = 393,
     TLESSEQUAL = 394,
     TMINUS = 395,
     TMOD = 396,
     TNOT = 397,
     TNOTEQUAL = 398,
     TOR = 399,
     TPLUS = 400,
     TQUESTION = 401,
     TSEMI = 402,
     TSHIFTLEFT = 403,
     TSHIFTRIGHT = 404,
     TSTAR = 405,
     TSWAP = 406,
     TLCBR = 407,
     TRCBR = 408,
     TLP = 409,
     TRP = 410,
     TLSBR = 411,
     TRSBR = 412,
     TNOELSE = 413,
     TUMINUS = 414,
     TUPLUS = 415
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
#line 460 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 467 "bison-chapel.cpp"
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
#line 546 "bison-chapel.cpp"

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
#define YYLAST   15792

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  161
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  133
/* YYNRULES -- Number of rules.  */
#define YYNRULES  553
/* YYNRULES -- Number of states.  */
#define YYNSTATES  995

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   415

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
     155,   156,   157,   158,   159,   160
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
     263,   265,   267,   269,   271,   273,   275,   277,   279,   281,
     283,   285,   287,   289,   291,   293,   295,   297,   299,   301,
     303,   305,   306,   308,   311,   313,   316,   320,   322,   324,
     327,   330,   332,   334,   336,   338,   340,   342,   346,   352,
     358,   361,   364,   370,   374,   381,   388,   393,   399,   405,
     409,   413,   420,   426,   433,   439,   446,   450,   455,   462,
     470,   477,   485,   490,   496,   501,   506,   510,   517,   523,
     530,   536,   545,   553,   556,   560,   564,   567,   570,   574,
     578,   579,   582,   585,   589,   595,   597,   601,   605,   611,
     617,   618,   621,   625,   628,   632,   639,   648,   655,   664,
     666,   668,   670,   671,   674,   675,   678,   682,   688,   694,
     696,   698,   701,   705,   707,   711,   712,   713,   722,   723,
     725,   728,   731,   733,   734,   735,   746,   750,   754,   760,
     766,   770,   772,   776,   778,   781,   783,   785,   787,   789,
     791,   793,   795,   797,   799,   801,   803,   805,   807,   809,
     811,   813,   815,   817,   819,   821,   823,   825,   827,   829,
     831,   833,   835,   837,   839,   841,   843,   845,   847,   849,
     851,   853,   854,   858,   862,   863,   865,   869,   874,   880,
     885,   892,   899,   900,   902,   904,   906,   908,   910,   913,
     916,   918,   920,   922,   923,   925,   927,   930,   932,   934,
     936,   938,   939,   941,   944,   946,   948,   950,   951,   953,
     955,   957,   959,   961,   964,   966,   967,   969,   972,   975,
     976,   979,   983,   988,   993,   996,  1001,  1002,  1005,  1008,
    1013,  1018,  1023,  1029,  1034,  1035,  1037,  1039,  1041,  1045,
    1049,  1055,  1057,  1059,  1063,  1065,  1068,  1072,  1073,  1076,
    1079,  1083,  1086,  1091,  1095,  1099,  1104,  1108,  1109,  1112,
    1115,  1118,  1120,  1121,  1124,  1127,  1130,  1132,  1137,  1142,
    1149,  1153,  1154,  1156,  1158,  1162,  1167,  1171,  1176,  1183,
    1184,  1187,  1190,  1193,  1196,  1198,  1200,  1204,  1208,  1210,
    1214,  1216,  1218,  1220,  1224,  1228,  1229,  1231,  1233,  1237,
    1241,  1245,  1247,  1249,  1251,  1253,  1255,  1257,  1259,  1261,
    1263,  1266,  1271,  1276,  1281,  1287,  1290,  1293,  1296,  1299,
    1302,  1305,  1312,  1319,  1324,  1334,  1344,  1352,  1359,  1366,
    1371,  1381,  1391,  1399,  1404,  1411,  1418,  1428,  1438,  1445,
    1447,  1449,  1451,  1453,  1455,  1457,  1459,  1461,  1465,  1466,
    1468,  1473,  1475,  1479,  1484,  1486,  1490,  1495,  1499,  1503,
    1505,  1507,  1510,  1512,  1515,  1517,  1519,  1523,  1526,  1531,
    1533,  1535,  1537,  1539,  1541,  1543,  1545,  1547,  1552,  1556,
    1560,  1564,  1568,  1571,  1574,  1576,  1577,  1579,  1582,  1585,
    1587,  1589,  1591,  1593,  1595,  1597,  1599,  1604,  1609,  1614,
    1618,  1622,  1626,  1630,  1634,  1639,  1643,  1648,  1650,  1652,
    1654,  1656,  1658,  1662,  1667,  1671,  1676,  1680,  1685,  1689,
    1695,  1699,  1703,  1707,  1711,  1715,  1719,  1723,  1727,  1731,
    1735,  1739,  1743,  1747,  1751,  1755,  1759,  1763,  1767,  1771,
    1775,  1779,  1783,  1787,  1790,  1793,  1796,  1799,  1802,  1805,
    1809,  1813,  1817,  1821,  1825,  1829,  1833,  1837,  1839,  1841,
    1843,  1845,  1847,  1849
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     162,     0,    -1,   163,    -1,    -1,   163,   164,    -1,   166,
      -1,   165,   166,    -1,    65,     7,    -1,   165,    65,     7,
      -1,   167,    -1,   170,    -1,   175,    -1,   176,    -1,   188,
      -1,   177,    -1,   191,    -1,   194,    -1,   192,    -1,   201,
      -1,   195,    -1,   196,    -1,   200,    -1,   187,    -1,   268,
     147,    -1,    12,   166,    -1,    13,   269,   166,    -1,    14,
     178,   147,    -1,    20,   269,   170,    -1,    25,   178,   147,
      -1,    27,   257,   147,    -1,    47,   180,   166,    -1,    50,
     279,   186,    -1,    50,   186,    -1,    57,   279,   186,    -1,
      79,   279,   186,    -1,    79,   186,    -1,    85,   166,    -1,
     103,   279,   147,    -1,     1,   147,    -1,   168,   169,    53,
     180,   152,   153,    -1,   168,   169,    53,   180,   152,   171,
     153,    -1,   168,   169,    53,   180,   152,     1,   153,    -1,
      -1,    70,    -1,    67,    -1,    -1,    69,    -1,   152,   153,
      -1,   152,   171,   153,    -1,   152,     1,   153,    -1,   164,
      -1,   171,   164,    -1,   279,    -1,   279,    11,   279,    -1,
     172,   127,   279,    -1,   172,   127,   279,    11,   279,    -1,
      -1,   172,    -1,   150,    -1,   172,    -1,    97,   256,   147,
      -1,    97,   279,    33,   174,   147,    -1,    97,   279,    58,
     173,   147,    -1,    75,   256,   147,    -1,   282,   222,   281,
     147,    -1,   282,   151,   281,   147,    -1,   282,   119,   281,
     147,    -1,   282,   113,   281,   147,    -1,   282,   114,   281,
     147,    -1,    -1,     3,    -1,     3,    -1,    87,    -1,   182,
      -1,     3,    -1,    87,    -1,   182,    -1,     3,    -1,    87,
      -1,    15,    -1,    45,    -1,    93,    -1,    71,    -1,    40,
      -1,    22,    -1,    83,    -1,    85,    -1,    81,    -1,    62,
      -1,    80,    -1,    16,    -1,    96,    -1,    30,    -1,    42,
      -1,    51,    -1,    15,    -1,    45,    -1,    93,    -1,    71,
      -1,    40,    -1,    22,    -1,    83,    -1,    51,    -1,    85,
      -1,    81,    -1,    62,    -1,    80,    -1,    16,    -1,    96,
      -1,    30,    -1,    42,    -1,    -1,     7,    -1,    29,   166,
      -1,   170,    -1,    76,   147,    -1,    76,   281,   147,    -1,
     147,    -1,   189,    -1,    70,   189,    -1,    67,   189,    -1,
     208,    -1,   239,    -1,   204,    -1,   190,    -1,   216,    -1,
     242,    -1,    38,   279,   147,    -1,    38,   279,    33,   174,
     147,    -1,    38,   279,    58,   173,   147,    -1,    38,   242,
      -1,    35,     9,    -1,    29,   166,   101,   279,   147,    -1,
     101,   279,   186,    -1,    21,   279,    41,   279,   269,   186,
      -1,    21,   279,    41,   193,   269,   186,    -1,    21,   279,
     269,   186,    -1,    36,   279,    41,   279,   186,    -1,    36,
     279,    41,   193,   186,    -1,    36,   279,   186,    -1,    36,
     193,   186,    -1,    36,    63,   180,    41,   279,   186,    -1,
      37,   279,    41,   279,   186,    -1,    37,   279,    41,   279,
     272,   186,    -1,    37,   279,    41,   193,   186,    -1,    37,
     279,    41,   193,   272,   186,    -1,    37,   279,   186,    -1,
      37,   279,   272,   186,    -1,   156,   256,    41,   279,   157,
     166,    -1,   156,   256,    41,   279,   272,   157,   166,    -1,
     156,   256,    41,   193,   157,   166,    -1,   156,   256,    41,
     193,   272,   157,   166,    -1,   156,   256,   157,   166,    -1,
     156,   256,   272,   157,   166,    -1,   104,   154,   256,   155,
      -1,    39,   279,    86,   166,    -1,    39,   279,   170,    -1,
      39,   279,    86,   166,    31,   166,    -1,    39,   279,   170,
      31,   166,    -1,    39,   279,   222,   279,    86,   166,    -1,
      39,   279,   222,   279,   170,    -1,    39,   279,   222,   279,
      86,   166,    31,   166,    -1,    39,   279,   222,   279,   170,
      31,   166,    -1,    26,   166,    -1,    90,   279,   147,    -1,
      91,   279,   147,    -1,    90,   177,    -1,    91,   177,    -1,
      90,   170,   197,    -1,    91,   170,   197,    -1,    -1,   197,
     198,    -1,    18,   170,    -1,    18,   199,   170,    -1,    18,
     154,   199,   155,   170,    -1,   180,    -1,   180,   126,   279,
      -1,    88,   279,   147,    -1,    78,   279,   152,   202,   153,
      -1,    78,   279,   152,     1,   153,    -1,    -1,   202,   203,
      -1,    99,   256,   186,    -1,    59,   166,    -1,    59,    29,
     166,    -1,   205,   180,   206,   152,   207,   153,    -1,    35,
     185,   205,   180,   206,   152,   207,   153,    -1,   205,   180,
     206,   152,     1,   153,    -1,    35,   185,   205,   180,   206,
     152,     1,   153,    -1,    19,    -1,    72,    -1,    95,    -1,
      -1,   126,   256,    -1,    -1,   207,   188,    -1,   207,   165,
     188,    -1,   209,   180,   152,   210,   153,    -1,   209,   180,
     152,     1,   153,    -1,    32,    -1,   211,    -1,   210,   127,
      -1,   210,   127,   211,    -1,   180,    -1,   180,   107,   279,
      -1,    -1,    -1,    48,   213,   224,   214,   231,   251,   238,
     234,    -1,    -1,    43,    -1,    34,   280,    -1,    35,   280,
      -1,    61,    -1,    -1,    -1,   215,   230,   217,   219,   218,
     231,   250,   232,   238,   233,    -1,   229,   221,   223,    -1,
     229,   222,   223,    -1,   229,   220,   129,   221,   223,    -1,
     229,   220,   129,   222,   223,    -1,   229,     1,   223,    -1,
     263,    -1,   154,   279,   155,    -1,   179,    -1,   123,   180,
      -1,   122,    -1,   124,    -1,   125,    -1,   123,    -1,   132,
      -1,   143,    -1,   139,    -1,   135,    -1,   138,    -1,   134,
      -1,   145,    -1,   140,    -1,   150,    -1,   128,    -1,   148,
      -1,   149,    -1,   141,    -1,   133,    -1,   142,    -1,    17,
      -1,   136,    -1,    10,    -1,   151,    -1,   137,    -1,   107,
      -1,   118,    -1,   115,    -1,   117,    -1,   111,    -1,   116,
      -1,   112,    -1,   108,    -1,   109,    -1,   110,    -1,   121,
      -1,   120,    -1,    -1,   154,   225,   155,    -1,   154,   225,
     155,    -1,    -1,   226,    -1,   225,   127,   226,    -1,   227,
     180,   255,   248,    -1,   165,   227,   180,   255,   248,    -1,
     227,   180,   255,   237,    -1,   227,   154,   247,   155,   255,
     248,    -1,   227,   154,   247,   155,   255,   237,    -1,    -1,
     228,    -1,    41,    -1,    44,    -1,    60,    -1,    24,    -1,
      24,    41,    -1,    24,    74,    -1,    63,    -1,    74,    -1,
      92,    -1,    -1,    63,    -1,    74,    -1,    24,    74,    -1,
      24,    -1,    92,    -1,    68,    -1,    46,    -1,    -1,    24,
      -1,    24,    74,    -1,    74,    -1,    63,    -1,    92,    -1,
      -1,    89,    -1,   147,    -1,   234,    -1,   170,    -1,   187,
      -1,   146,   180,    -1,   146,    -1,    -1,   235,    -1,   131,
     279,    -1,   131,   236,    -1,    -1,   100,   279,    -1,    92,
     240,   147,    -1,    23,    92,   240,   147,    -1,    35,    92,
     240,   147,    -1,   180,   241,    -1,   180,   241,   127,   240,
      -1,    -1,   107,   264,    -1,   107,   252,    -1,   243,    63,
     244,   147,    -1,   243,    24,   244,   147,    -1,   243,    74,
     244,   147,    -1,   243,    24,    74,   244,   147,    -1,   243,
      98,   244,   147,    -1,    -1,    23,    -1,    35,    -1,   245,
      -1,   244,   127,   245,    -1,   180,   251,   248,    -1,   154,
     247,   155,   251,   248,    -1,    94,    -1,   180,    -1,   154,
     247,   155,    -1,   246,    -1,   246,   127,    -1,   246,   127,
     247,    -1,    -1,   107,    56,    -1,   107,   281,    -1,   156,
     157,   264,    -1,   156,   157,    -1,   156,   256,   157,   264,
      -1,   156,   256,   157,    -1,   156,   157,   249,    -1,   156,
     256,   157,   249,    -1,   156,     1,   157,    -1,    -1,   126,
     264,    -1,   126,   249,    -1,   126,   184,    -1,     1,    -1,
      -1,   126,   264,    -1,   126,   252,    -1,   126,   184,    -1,
       1,    -1,   156,   256,   157,   264,    -1,   156,   256,   157,
     252,    -1,   156,   256,    41,   279,   157,   264,    -1,   156,
       1,   157,    -1,    -1,   264,    -1,   235,    -1,   156,   157,
     253,    -1,   156,   256,   157,   253,    -1,   156,   157,   254,
      -1,   156,   256,   157,   254,    -1,   156,   256,    41,   279,
     157,   253,    -1,    -1,   126,   264,    -1,   126,   235,    -1,
     126,   184,    -1,   126,   254,    -1,   279,    -1,   235,    -1,
     256,   127,   279,    -1,   256,   127,   235,    -1,   279,    -1,
     257,   127,   279,    -1,    94,    -1,   281,    -1,   235,    -1,
     258,   127,   258,    -1,   259,   127,   258,    -1,    -1,   261,
      -1,   262,    -1,   261,   127,   262,    -1,   181,   107,   235,
      -1,   181,   107,   281,    -1,   235,    -1,   281,    -1,   181,
      -1,   183,    -1,   267,    -1,   282,    -1,   266,    -1,   290,
      -1,   289,    -1,    81,   279,    -1,    42,   154,   260,   155,
      -1,    30,   154,   260,   155,    -1,    84,   154,   260,   155,
      -1,    82,    84,   154,   260,   155,    -1,    12,   279,    -1,
      85,   279,    -1,    16,   279,    -1,    62,   279,    -1,    96,
     279,    -1,    80,   279,    -1,    36,   279,    41,   279,    29,
     279,    -1,    36,   279,    41,   193,    29,   279,    -1,    36,
     279,    29,   279,    -1,    36,   279,    41,   279,    29,    39,
     279,    86,   279,    -1,    36,   279,    41,   193,    29,    39,
     279,    86,   279,    -1,    36,   279,    29,    39,   279,    86,
     279,    -1,    37,   279,    41,   279,    29,   279,    -1,    37,
     279,    41,   193,    29,   279,    -1,    37,   279,    29,   279,
      -1,    37,   279,    41,   279,    29,    39,   279,    86,   279,
      -1,    37,   279,    41,   193,    29,    39,   279,    86,   279,
      -1,    37,   279,    29,    39,   279,    86,   279,    -1,   156,
     256,   157,   279,    -1,   156,   256,    41,   279,   157,   279,
      -1,   156,   256,    41,   193,   157,   279,    -1,   156,   256,
      41,   279,   157,    39,   279,    86,   279,    -1,   156,   256,
      41,   193,   157,    39,   279,    86,   279,    -1,    39,   279,
      86,   279,    31,   279,    -1,    55,    -1,   267,    -1,   263,
      -1,   285,    -1,   284,    -1,   212,    -1,   277,    -1,   278,
      -1,   276,   137,   279,    -1,    -1,   270,    -1,   102,   154,
     271,   155,    -1,   274,    -1,   271,   127,   274,    -1,   102,
     154,   273,   155,    -1,   274,    -1,   273,   127,   274,    -1,
     275,   263,   251,   248,    -1,   293,    73,   263,    -1,   279,
      73,   263,    -1,    24,    -1,    41,    -1,    24,    41,    -1,
      74,    -1,    24,    74,    -1,    98,    -1,   282,    -1,   276,
     137,   279,    -1,    54,   279,    -1,    49,   244,    41,   279,
      -1,   287,    -1,   264,    -1,   265,    -1,   291,    -1,   292,
      -1,   212,    -1,   277,    -1,   278,    -1,   154,   131,   279,
     155,    -1,   279,   126,   279,    -1,   279,   126,    16,    -1,
     279,   126,    96,    -1,   279,   130,   279,    -1,   279,   130,
      -1,   130,   279,    -1,   130,    -1,    -1,   279,    -1,    90,
     279,    -1,    91,   279,    -1,   279,    -1,   263,    -1,   284,
      -1,   285,    -1,   286,    -1,   282,    -1,   212,    -1,   283,
     154,   260,   155,    -1,   283,   156,   260,   157,    -1,    66,
     154,   260,   155,    -1,   279,   129,   181,    -1,   279,   129,
      92,    -1,   279,   129,    30,    -1,   279,   129,    51,    -1,
     154,   258,   155,    -1,   154,   258,   127,   155,    -1,   154,
     259,   155,    -1,   154,   259,   127,   155,    -1,     4,    -1,
       5,    -1,     6,    -1,     7,    -1,     8,    -1,   152,   256,
     153,    -1,   152,   256,   127,   153,    -1,   156,   256,   157,
      -1,   156,   256,   127,   157,    -1,   156,   288,   157,    -1,
     156,   288,   127,   157,    -1,   279,   105,   279,    -1,   288,
     127,   279,   105,   279,    -1,   279,   145,   279,    -1,   279,
     140,   279,    -1,   279,   150,   279,    -1,   279,   128,   279,
      -1,   279,   148,   279,    -1,   279,   149,   279,    -1,   279,
     141,   279,    -1,   279,   132,   279,    -1,   279,   143,   279,
      -1,   279,   139,   279,    -1,   279,   135,   279,    -1,   279,
     138,   279,    -1,   279,   134,   279,    -1,   279,   122,   279,
      -1,   279,   124,   279,    -1,   279,   125,   279,    -1,   279,
     106,   279,    -1,   279,   144,   279,    -1,   279,   133,   279,
      -1,   279,    17,   279,    -1,   279,    10,   279,    -1,   279,
     136,   279,    -1,   279,    28,   279,    -1,   145,   279,    -1,
     140,   279,    -1,    52,   279,    -1,    64,   279,    -1,   142,
     279,    -1,   123,   279,    -1,   279,    73,   279,    -1,   279,
      73,   193,    -1,   293,    73,   279,    -1,   293,    73,   193,
      -1,   279,    77,   279,    -1,   279,    77,   193,    -1,   293,
      77,   279,    -1,   293,    77,   193,    -1,   145,    -1,   150,
      -1,   106,    -1,   144,    -1,   122,    -1,   124,    -1,   125,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   466,   466,   471,   472,   478,   479,   484,   485,   490,
     491,   492,   493,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,   517,   518,   519,   523,
     526,   528,   534,   535,   536,   540,   541,   554,   555,   556,
     561,   562,   567,   572,   577,   581,   588,   593,   597,   602,
     606,   607,   608,   612,   616,   618,   620,   622,   624,   631,
     632,   636,   637,   638,   641,   642,   643,   655,   656,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   677,
     678,   679,   680,   681,   682,   686,   687,   688,   689,   690,
     691,   692,   693,   700,   701,   702,   703,   704,   705,   706,
     707,   712,   713,   717,   718,   722,   723,   727,   728,   729,
     730,   731,   732,   733,   734,   738,   739,   743,   744,   745,
     746,   750,   757,   758,   759,   760,   761,   762,   763,   764,
     765,   766,   767,   768,   769,   770,   771,   772,   773,   779,
     785,   791,   797,   804,   814,   818,   819,   820,   821,   822,
     824,   826,   828,   833,   836,   837,   838,   839,   840,   841,
     845,   846,   850,   851,   852,   856,   857,   861,   864,   866,
     871,   872,   876,   878,   880,   887,   897,   907,   917,   930,
     935,   940,   948,   949,   954,   955,   957,   962,   978,   985,
     994,  1002,  1006,  1013,  1014,  1019,  1024,  1018,  1049,  1055,
    1062,  1068,  1074,  1085,  1091,  1084,  1123,  1127,  1132,  1136,
    1141,  1148,  1149,  1153,  1154,  1155,  1156,  1157,  1158,  1159,
    1160,  1161,  1162,  1163,  1164,  1165,  1166,  1167,  1168,  1169,
    1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,  1178,  1182,
    1183,  1184,  1185,  1186,  1187,  1188,  1189,  1190,  1191,  1192,
    1193,  1197,  1198,  1202,  1206,  1207,  1208,  1212,  1214,  1216,
    1218,  1220,  1225,  1226,  1230,  1231,  1232,  1233,  1234,  1235,
    1236,  1237,  1238,  1242,  1243,  1244,  1245,  1246,  1247,  1251,
    1252,  1256,  1257,  1258,  1259,  1260,  1261,  1265,  1266,  1269,
    1270,  1274,  1275,  1279,  1281,  1286,  1287,  1291,  1292,  1296,
    1297,  1301,  1303,  1305,  1310,  1323,  1340,  1341,  1343,  1348,
    1356,  1364,  1372,  1381,  1391,  1392,  1393,  1397,  1398,  1406,
    1408,  1413,  1415,  1417,  1422,  1424,  1426,  1433,  1434,  1435,
    1440,  1442,  1444,  1448,  1452,  1454,  1458,  1466,  1467,  1468,
    1469,  1470,  1475,  1476,  1477,  1478,  1479,  1499,  1503,  1507,
    1515,  1522,  1523,  1524,  1528,  1530,  1536,  1538,  1540,  1545,
    1546,  1547,  1548,  1549,  1555,  1556,  1557,  1558,  1562,  1563,
    1567,  1568,  1569,  1573,  1574,  1578,  1579,  1583,  1584,  1588,
    1589,  1590,  1591,  1595,  1596,  1607,  1608,  1609,  1610,  1611,
    1612,  1614,  1616,  1618,  1620,  1622,  1624,  1626,  1628,  1630,
    1632,  1637,  1639,  1641,  1643,  1645,  1647,  1649,  1651,  1653,
    1655,  1657,  1659,  1661,  1668,  1674,  1680,  1686,  1695,  1705,
    1713,  1714,  1715,  1716,  1717,  1718,  1719,  1720,  1725,  1726,
    1730,  1734,  1735,  1739,  1743,  1744,  1748,  1752,  1756,  1763,
    1764,  1765,  1766,  1767,  1768,  1772,  1773,  1778,  1783,  1791,
    1792,  1793,  1794,  1795,  1796,  1797,  1798,  1799,  1801,  1803,
    1805,  1807,  1809,  1811,  1813,  1818,  1819,  1822,  1823,  1824,
    1827,  1828,  1829,  1830,  1841,  1842,  1846,  1847,  1848,  1852,
    1853,  1854,  1855,  1863,  1864,  1865,  1866,  1870,  1871,  1872,
    1873,  1874,  1875,  1876,  1877,  1878,  1879,  1883,  1891,  1892,
    1896,  1897,  1898,  1899,  1900,  1901,  1902,  1903,  1904,  1905,
    1906,  1907,  1908,  1909,  1910,  1911,  1912,  1913,  1914,  1915,
    1916,  1917,  1918,  1922,  1923,  1924,  1925,  1926,  1927,  1931,
    1932,  1933,  1934,  1938,  1939,  1940,  1941,  1946,  1947,  1948,
    1949,  1950,  1951,  1952
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
  "TINT", "TITER", "TLABEL", "TLAMBDA", "TLET", "TLOCAL", "TLOCALE",
  "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT", "TON", "TONLY",
  "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED", "TPARAM", "TPLUSPLUS",
  "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC",
  "TREAL", "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN",
  "TSELECT", "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE", "TSTRING",
  "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS", "TTHROW", "TTHROWS", "TTRY",
  "TTRYBANG", "TTYPE", "TUINT", "TUNDERSCORE", "TUNION", "TUNMANAGED",
  "TUSE", "TVAR", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP",
  "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR",
  "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR",
  "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY", "TASSIGNPLUS",
  "TASSIGNREDUCE", "TASSIGNSL", "TASSIGNSR", "TBAND", "TBNOT", "TBOR",
  "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT",
  "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL", "THASH", "TIO", "TLESS",
  "TLESSEQUAL", "TMINUS", "TMOD", "TNOT", "TNOTEQUAL", "TOR", "TPLUS",
  "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP",
  "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR", "TRSBR", "TNOELSE", "TUMINUS",
  "TUPLUS", "$accept", "program", "toplevel_stmt_ls", "toplevel_stmt",
  "pragma_ls", "stmt", "module_decl_stmt", "access_control",
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
  "opt_where_part", "type_alias_decl_stmt", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_stmt", "opt_config", "var_decl_stmt_inner_ls",
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
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   161,   162,   163,   163,   164,   164,   165,   165,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   167,
     167,   167,   168,   168,   168,   169,   169,   170,   170,   170,
     171,   171,   172,   172,   172,   172,   173,   173,   174,   174,
     175,   175,   175,   176,   177,   177,   177,   177,   177,   178,
     178,   179,   179,   179,   180,   180,   180,   181,   181,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   183,   183,   183,   183,   183,
     183,   183,   183,   184,   184,   184,   184,   184,   184,   184,
     184,   185,   185,   186,   186,   187,   187,   188,   188,   188,
     188,   188,   188,   188,   188,   189,   189,   190,   190,   190,
     190,   191,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   193,   194,   194,   194,   194,   194,
     194,   194,   194,   195,   196,   196,   196,   196,   196,   196,
     197,   197,   198,   198,   198,   199,   199,   200,   201,   201,
     202,   202,   203,   203,   203,   204,   204,   204,   204,   205,
     205,   205,   206,   206,   207,   207,   207,   208,   208,   209,
     210,   210,   210,   211,   211,   213,   214,   212,   215,   215,
     215,   215,   215,   217,   218,   216,   219,   219,   219,   219,
     219,   220,   220,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   223,   223,   224,   225,   225,   225,   226,   226,   226,
     226,   226,   227,   227,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   229,   229,   229,   229,   229,   229,   230,
     230,   231,   231,   231,   231,   231,   231,   232,   232,   233,
     233,   234,   234,   235,   235,   236,   236,   237,   237,   238,
     238,   239,   239,   239,   240,   240,   241,   241,   241,   242,
     242,   242,   242,   242,   243,   243,   243,   244,   244,   245,
     245,   246,   246,   246,   247,   247,   247,   248,   248,   248,
     249,   249,   249,   249,   249,   249,   249,   250,   250,   250,
     250,   250,   251,   251,   251,   251,   251,   252,   252,   252,
     252,   253,   253,   253,   254,   254,   254,   254,   254,   255,
     255,   255,   255,   255,   256,   256,   256,   256,   257,   257,
     258,   258,   258,   259,   259,   260,   260,   261,   261,   262,
     262,   262,   262,   263,   263,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   266,   267,
     268,   268,   268,   268,   268,   268,   268,   268,   269,   269,
     270,   271,   271,   272,   273,   273,   274,   274,   274,   275,
     275,   275,   275,   275,   275,   276,   276,   277,   278,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   280,   280,   281,   281,   281,
     282,   282,   282,   282,   283,   283,   284,   284,   284,   285,
     285,   285,   285,   286,   286,   286,   286,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   288,   288,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   290,   290,   290,   290,   290,   290,   291,
     291,   291,   291,   292,   292,   292,   292,   293,   293,   293,
     293,   293,   293,   293
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     2,     1,     2,     3,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     3,     5,     5,
       2,     2,     5,     3,     6,     6,     4,     5,     5,     3,
       3,     6,     5,     6,     5,     6,     3,     4,     6,     7,
       6,     7,     4,     5,     4,     4,     3,     6,     5,     6,
       5,     8,     7,     2,     3,     3,     2,     2,     3,     3,
       0,     2,     2,     3,     5,     1,     3,     3,     5,     5,
       0,     2,     3,     2,     3,     6,     8,     6,     8,     1,
       1,     1,     0,     2,     0,     2,     3,     5,     5,     1,
       1,     2,     3,     1,     3,     0,     0,     8,     0,     1,
       2,     2,     1,     0,     0,    10,     3,     3,     5,     5,
       3,     1,     3,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     3,     0,     1,     3,     4,     5,     4,
       6,     6,     0,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     0,     1,     2,     1,     1,     1,     0,     1,     1,
       1,     1,     1,     2,     1,     0,     1,     2,     2,     0,
       2,     3,     4,     4,     2,     4,     0,     2,     2,     4,
       4,     4,     5,     4,     0,     1,     1,     1,     3,     3,
       5,     1,     1,     3,     1,     2,     3,     0,     2,     2,
       3,     2,     4,     3,     3,     4,     3,     0,     2,     2,
       2,     1,     0,     2,     2,     2,     1,     4,     4,     6,
       3,     0,     1,     1,     3,     4,     3,     4,     6,     0,
       2,     2,     2,     2,     1,     1,     3,     3,     1,     3,
       1,     1,     1,     3,     3,     0,     1,     1,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     4,     4,     4,     5,     2,     2,     2,     2,     2,
       2,     6,     6,     4,     9,     9,     7,     6,     6,     4,
       9,     9,     7,     4,     6,     6,     9,     9,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       4,     1,     3,     4,     1,     3,     4,     3,     3,     1,
       1,     2,     1,     2,     1,     1,     3,     2,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     3,     3,
       3,     3,     2,     2,     1,     0,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     4,     3,
       3,     3,     3,     3,     4,     3,     4,     1,     1,     1,
       1,     1,     3,     4,     3,     4,     3,     4,     3,     5,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    77,   497,   498,   499,   500,
     501,     0,   438,    69,    95,     0,   189,   438,     0,   100,
     325,    69,     0,     0,     0,     0,   199,   475,   326,     0,
       0,   324,     0,    99,     0,   209,    96,     0,   205,     0,
       0,   102,     0,     0,   429,     0,   212,     0,     0,     0,
       0,   324,   324,    98,   190,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,    78,     0,     0,     0,     0,
      97,   191,     0,     0,     0,     0,   549,   551,     0,   552,
     553,   474,     0,     0,   550,   547,   117,   548,     0,     0,
       0,     4,     0,     5,     9,    45,    10,    11,    12,    14,
     393,   394,    22,    13,   118,   124,    15,    17,    16,    19,
      20,    21,    18,   123,     0,   121,     0,   464,     0,   125,
     122,   126,     0,   480,   460,   461,   397,   395,     0,     0,
     465,   466,     0,   396,     0,   481,   482,   483,   459,   399,
     398,   462,   463,     0,    38,    24,   405,     0,     0,   439,
      70,     0,     0,     0,     0,     0,     0,     0,     0,   464,
     480,   395,   465,   466,   407,   396,   481,   482,     0,   438,
       0,     0,   163,     0,   378,     0,   385,   476,   210,   500,
     131,     0,     0,   211,     0,     0,     0,     0,     0,   325,
     326,   130,     0,     0,   385,    74,    79,    90,    84,    92,
      83,    93,    80,    94,    88,    82,    89,    87,    85,    86,
      75,    81,    91,     0,    76,     0,     0,     0,     0,   327,
       0,   114,    32,     0,   535,   457,     0,   408,   536,     7,
     385,   326,   120,   119,   304,   375,     0,   374,     0,     0,
     115,   479,     0,     0,    35,     0,   410,   400,     0,   385,
      36,   406,     0,   170,   166,     0,   396,   170,   167,     0,
     316,     0,   409,     0,   374,     0,     0,   538,   473,   534,
     537,   533,     0,    47,    50,     0,     0,   380,     0,   382,
       0,     0,   381,     0,   374,     0,     0,     6,    46,     0,
     192,     0,   290,   289,   213,     0,     0,     0,     0,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   472,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   249,
     256,   257,   258,   253,   255,     0,     0,   251,   254,   252,
     250,     0,   260,   259,     0,     0,   385,   385,     0,     0,
       0,    25,    26,     0,     0,     0,     0,     0,    27,     0,
       0,     0,    28,     0,    29,     0,   393,   391,     0,   386,
     387,   392,     0,     0,     0,     0,   140,     0,     0,   139,
       0,     0,     0,   146,     0,     0,    56,   127,     0,   156,
       0,     0,    30,   272,   206,   331,     0,   332,   334,     0,
     356,     0,   337,     0,     0,   113,    31,    33,     0,   303,
       0,    63,   477,   478,   116,     0,    34,   385,     0,   177,
     168,   164,   169,   165,     0,   314,   311,    60,     0,    56,
     133,    37,    49,    48,    51,     0,   502,     0,     0,   493,
       0,   495,     0,     0,     0,     0,     0,     0,   506,     8,
       0,     0,     0,     0,   283,     0,     0,     0,     0,     0,
     437,   530,   529,   532,   540,   539,   544,   543,   526,   523,
     524,   525,   469,   470,   468,   513,   491,   492,   490,   489,
     471,   517,   528,   522,   520,   531,   521,   519,   511,   516,
     518,   527,   510,   514,   515,   512,     0,     0,     0,     0,
       0,     0,     0,   542,   541,   546,   545,   449,   450,   452,
     454,     0,   441,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,   438,   438,   136,   312,   379,     0,     0,
     402,     0,   313,   192,     0,     0,     0,   413,     0,     0,
       0,   419,     0,     0,     0,   147,   548,    59,     0,    52,
      57,     0,   155,     0,     0,     0,   401,   277,   274,   275,
     276,   280,   281,   282,   272,     0,   265,     0,   273,   291,
       0,   335,     0,   107,   109,   110,   105,   106,   104,   103,
     108,     0,   355,   354,   460,     0,   329,   458,   328,   488,
     377,   376,     0,     0,     0,   403,     0,   171,   318,   460,
       0,     0,     0,   503,   467,   494,   383,   496,   384,     0,
       0,   505,   152,   423,     0,   508,   507,     0,     0,   193,
       0,     0,   203,     0,   200,   287,   284,   285,   288,   214,
       0,     0,   320,   319,   321,   323,    67,    68,    66,    65,
      64,   486,   487,   451,   453,     0,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     132,   389,   390,   388,     0,     0,   154,     0,     0,   138,
       0,   137,     0,     0,   144,     0,     0,   142,     0,     0,
     444,     0,   128,     0,   129,     0,     0,   158,     0,   160,
     278,   279,     0,   272,   263,     0,   369,   292,   295,   294,
     296,     0,   333,   336,   337,     0,     0,   338,   339,   179,
       0,     0,   178,   181,   404,     0,   172,   175,     0,   315,
      61,    62,     0,     0,     0,     0,   153,     0,     0,     0,
     324,   198,     0,   201,   197,   286,   291,   261,    71,   246,
      79,   244,    84,    83,    80,    94,    82,    85,    72,    81,
     225,   228,   226,   227,   238,   229,   242,   234,   232,   245,
     248,   233,   231,   236,   241,   243,   230,   235,   239,   240,
     237,   247,     0,   223,    73,     0,   261,   261,   221,   322,
     442,   337,   480,   480,     0,     0,     0,     0,     0,     0,
       0,     0,   135,   134,     0,   141,     0,     0,   412,     0,
     411,     0,     0,   418,   145,     0,   417,   143,     0,   443,
      54,    53,   157,   428,   159,     0,   369,   266,     0,     0,
     337,   293,   309,   330,   360,     0,   504,     0,   183,     0,
       0,     0,   173,     0,   150,   425,     0,     0,   148,   424,
       0,   509,     0,    39,     0,   187,   326,   324,   324,   185,
     324,   195,   204,   202,     0,   272,   220,   224,     0,     0,
     216,   217,   446,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   324,   416,     0,     0,   422,     0,     0,   445,
       0,     0,   162,   337,   369,     0,   372,   371,   373,   460,
     305,   269,   267,     0,     0,     0,   358,   460,   184,   182,
       0,   176,     0,   151,     0,   149,    41,    40,   196,   351,
       0,   297,     0,   222,    71,    72,   261,   261,     0,     0,
       0,     0,     0,     0,   188,   186,     0,     0,     0,     0,
      55,   161,   268,   337,   361,     0,   306,   308,   307,   310,
     301,   302,   207,     0,   174,     0,     0,     0,   350,   349,
     460,   298,   309,   262,   218,   219,     0,     0,     0,     0,
       0,     0,   415,   414,   421,   420,   271,   270,   363,   364,
     366,   460,     0,   504,   460,   427,   426,     0,   341,     0,
       0,     0,   365,   367,   346,   344,   460,   504,   299,   215,
     300,   361,   345,   460,   368
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   274,    92,   552,    94,    95,   289,    96,
     275,   547,   551,   548,    97,    98,    99,   151,   773,   217,
     100,   214,   101,   582,   182,   222,   102,   103,   104,   105,
     106,   107,   656,   108,   109,   110,   420,   597,   718,   111,
     112,   593,   713,   113,   114,   452,   730,   115,   116,   623,
     624,   159,   215,   569,   118,   119,   454,   736,   629,   775,
     776,   390,   856,   394,   565,   566,   567,   568,   630,   294,
     701,   952,   989,   942,   235,   937,   891,   894,   120,   261,
     425,   121,   122,   218,   219,   398,   399,   586,   949,   911,
     402,   583,   969,   888,   820,   276,   173,   280,   281,   368,
     369,   370,   160,   124,   125,   126,   161,   128,   148,   149,
     511,   384,   679,   512,   513,   129,   162,   163,   132,   183,
     371,   165,   134,   166,   167,   137,   138,   285,   139,   140,
     141,   142,   143
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -855
static const yytype_int16 yypact[] =
{
    -855,   156,  2519,  -855,   -24,  -855,  -855,  -855,  -855,  -855,
    -855,  3908,    65,   258,  -855,  9048,  -855,    65,  9048,  -855,
     145,   258,  3908,  9048,  3908,   134,  -855,  9048,  6067,  7469,
    9048,  7623,  9048,  -855,   166,  -855,  -855, 14860,  -855, 11770,
    8170,  -855,  9048,  9048,  -855,  9048,  -855,  9048,  9048,   200,
     178,   620,  1006,  -855,  -855,  8301,  7030,  9048,  8170,  9048,
    9048,   188,  -855,   190,  3908,  -855,  9048,  9179,  9179, 14860,
    -855,  -855,  9048,  8301,  9048,  9048,  -855,  -855,  9048,  -855,
    -855, 11144,  9048,  9048,  -855,  9048,  -855,  -855,  2984,  6437,
    8301,  -855,  3754,  -855,  -855,   218,  -855,  -855,  -855,  -855,
    -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,
    -855,  -855,  -855,  -855, 14860,  -855, 14860,   269,    56,  -855,
    -855,  -855,   142,   198,  -855,  -855,  -855,   233,   241,   253,
     254,   268, 15426, 11365,    97,   275,   281,  -855,  -855,  -855,
    -855,  -855,  -855,   273,  -855,  -855, 15426,   246,  3908,  -855,
    -855,   287,  9048,  9048,  9048,  9048,  9048,  8301,  8301,   251,
    -855,  -855,  -855,  -855,   291,   265,  -855,  -855,   285, 13536,
   14860,   298,  -855,    77, 15426,   346,  7184, 15426,  -855,   100,
    -855, 14860,   159,  -855, 14860,   284,    47, 13103, 13063,  -855,
    -855,  -855, 13496, 12335,  7184,  -855,  -855,  -855,  -855,  -855,
    -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,
    -855,  -855,  -855,  3908,  -855,   296,  2166,    27,    -2,  -855,
    3908,  -855,  -855, 13279,   337,   291, 13279,   291,   337,  -855,
    7184,  7754,  -855,  -855, 14860,  -855,    79, 15426,  9048,  9048,
    -855, 15426,   304, 13360,  -855, 13279,   291, 15426,   302,  7184,
    -855, 15426, 13753,  -855,  -855, 13889, 15636,  -855,  -855, 13920,
     347,   310,   291,   112, 13672, 13279, 14056,   427,  1411,   337,
     427,   337,   -61,  -855,  -855,  3138,   109,  -855,  9048,  -855,
     103,   114,  -855,    13, 14102,   -40,   451,  -855,  -855,   406,
     334,   315,  -855,  -855,  -855,  2295, 11770, 11770, 11770,  -855,
    9048,  9048,  9048,  9048,  8455,  8455,  9048,  9048,  9048,  9048,
    9310,  9048,   352, 11144,  9048,  9048,  9048,  9048,  9048,  9048,
    9048,  9048,  9048,  9048,  9048,  9048,  9048,  9048,  9048,  -855,
    -855,  -855,  -855,  -855,  -855,  7885,  7885,  -855,  -855,  -855,
    -855,  7885,  -855,  -855,  7885,  7885,  7184,  7184,  8455,  8455,
    6899,  -855,  -855, 13703, 13839, 14143,    25,  3292,  -855,  8455,
      47,   322,  -855,  9048,  -855,  9048,   366,  -855,   325,   350,
    -855,  -855,   335, 14860,   440,  8301,  -855,  4062,  8455,  -855,
    4216,  8455,   329,  -855,    47,  9441,  9048,  -855,  3908,   454,
    9048,   336,  -855,   662,  -855,  -855,  2166,  -855,   362,   340,
    -855,  9572,   385,  9048, 11770,  -855,  -855,  -855,   341,  -855,
    8301,  -855, 15426, 15426,  -855,    22,  -855,  7184,   342,  -855,
     475,  -855,   475,  -855,  9703,   371,  -855,  -855,  9441,  9048,
    -855,  -855,  -855,  -855,  -855,  8016,  -855, 12187,  6591,  -855,
    6745,  -855,  8455,  5602,  2830,   355,  9048,  5912,  -855,  -855,
   14860,  8301,   351, 14076,   172, 11770,   139,   204,   226,   229,
   14238, 15552, 15552,   320,  -855,   320,  -855,   320, 14357,   955,
    1185,  1309, 11492, 11492,   291,   427,  -855,  -855,  -855,  -855,
    1411,  2142,   320,  1862,  1862, 15552,  1862,  1862,   739,   427,
    2142, 15592,   739,   337,   337,   427,   367,   370,   374,   375,
     379,   358,   382,  -855,   320,  -855,   320,    42,  -855,  -855,
    -855,   121,  -855,  1105, 15512,   295,  9834,  8455,  9965,  8455,
    9048,  8455, 11682,    65, 14288,  -855,  -855, 15426, 14319,  7184,
    -855,  7184,  -855,   334,  9048,   122,  9048, 15426,    51, 13310,
    9048, 15426,    36, 13143,  6899,  -855,   384,   405,   389, 14460,
     405,   393,   510, 14491,  3908, 13446,  -855,   149,  -855,  -855,
    -855,  -855,  -855,  -855,  1097,   123,  -855, 11830,  -855,   189,
     387,  2166,    27,  9048,   134,   166,  9048,  9048,  9048,  9048,
    9048,  5448,  -855,  -855,   118,  7338,  -855, 15426,  -855,  -855,
    -855, 15426,   390,    61,   391,  -855, 11710,  -855,  -855,   231,
   14860,   397,   398,  -855,  -855,  -855,  -855,  -855,  -855,    -9,
    2332,  -855,  -855, 15426,  3908, 15426,  -855, 14541,   395,   425,
    1063,   401,   450,   115,  -855,   487,  -855,  -855,  -855,  -855,
    6154,   234,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,
    -855,  -855,  -855,  -855,  -855,  6899,  -855,    24,  8455,  8455,
    9048,   534, 14627,  9048,   542, 14677,   415, 11975,    47,    47,
    -855,  -855,  -855,  -855,   424, 13279,  -855, 12409,  4370,  -855,
    4524,  -855, 12483,  4678,  -855,    47,  4832,  -855,    47,   128,
    -855,  9048,  -855,  9048,  -855,  3908,  9048,  -855,  3908,   547,
    -855,  -855, 14860,  1304,  -855,  2166,   455,   506,  -855,  -855,
    -855,    37,  -855,  -855,   385,   428,    44,  -855,  -855,  -855,
    4986,  8301,  -855,  -855,  -855, 14860,  -855,   456,   285,  -855,
    -855,  -855,  5140,   429,  5294,   430,  -855,  9048,  3446,   431,
   13087,  -855,  9048, 14860,  -855,  -855,   189,   435,   462,  -855,
     466,  -855,   467,   469,   472,   473,   474,   476,   477,   478,
    -855, 14860,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,
    -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,
    -855,  -855,  9048,  -855,  -855,   480,   435,   435,  -855,  -855,
    -855,   385,   192,   202, 14708, 10096, 10227, 14844, 10358, 10489,
   10620, 10751,  -855,  -855,  1902,  -855,  3908,  9048, 15426,  9048,
   15426,  3908,  9048, 15426,  -855,  9048, 15426,  -855,  6899,  -855,
   14875, 15426,  -855, 15426,   584,  3908,   455,  -855,   461,  8586,
      -4,  -855,   519,  -855,  -855,  8455, 11587,  3908,  -855,    46,
     465,  9048,  -855,  9048,  -855, 15426,  3908,  9048,  -855, 15426,
    3908, 15426,   174,  -855,  3600,  -855,  6306,   565,   565,  -855,
     966,  -855, 15426,  -855,    29,   662,  -855,  -855, 12261, 11890,
    -855,  -855,  -855,  9048,  9048,  9048,  9048,  9048,  9048,  9048,
    9048,   468, 13294, 14491, 12557, 12631, 14491, 12705, 12779,  -855,
    9048,  3908,  -855,   385,   455,  5757,  -855,  -855,  -855,   -23,
    8301,  -855,  -855,  9048,   -16, 12044,  -855,   372,   346,  -855,
     285, 15426, 12853,  -855, 12927,  -855,  -855,  -855,  -855,  -855,
   10882,   536,   216,  -855,  -855,  -855,   435,   435, 15011, 15042,
   15178, 15209, 15345, 15376,  -855,  -855,  3908,  3908,  3908,  3908,
   15426,  -855,  -855,    -4,  8740,    53,  -855,  -855, 15426, 15426,
    -855,  -855,  -855, 10751,  -855,  3908,  3908,  2673,  -855,  -855,
     132,  -855,   519,  -855,  -855,  -855,  9048,  9048,  9048,  9048,
    9048,  9048, 14491, 14491, 14491, 14491,  -855,  -855,  -855,  -855,
    -855,   256,  8455, 11275,   497, 14491, 14491,   471, 11013,    60,
      82, 12118,  -855,  -855,  -855,  -855,   133, 11371,  -855,  -855,
    -855,  8894,  -855,   171,  -855
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -855,  -855,  -855,    -1,  -371,  1671,  -855,  -855,  -855,  1526,
    -105,  -296,   201,   203,  -855,  -855,   101,   608,  -855,  1393,
    -135,  -615,  -855,  -761,  -855,   867,  -854,  -688,   -49,  -855,
    -855,  -855,    93,  -855,  -855,  -855,   394,  -855,   -83,  -855,
    -855,  -855,  -855,  -855,   479,   117,  -146,  -855,  -855,  -855,
     -80,   553,  -855,  -855,  -855,  -855,  -855,  -855,  -855,  -855,
    -203,  -128,  -725,  -855,  -197,   -33,   104,  -855,  -855,  -855,
     -69,  -855,  -855,  -311,   300,  -855,  -262,  -276,  -855,  -157,
    -855,   646,  -855,  -198,   274,  -855,  -369,  -690,  -594,  -855,
    -537,  -414,  -776,  -838,  -727,   -47,  -855,  -331,  -855,  -161,
    -855,   148,   338,  -351,  -855,  -855,   902,  -855,    -8,  -855,
    -855,  -201,  -855,  -533,  -855,  -855,   929,  1231,   -11,   653,
      50,   526,  -855,  1328,  1425,  -855,  -855,  -855,  -855,  -855,
    -855,  -855,  -344
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -505
static const yytype_int16 yytable[] =
{
     146,    91,   232,   233,   164,   345,   515,   169,   236,   168,
     598,   680,   174,   361,   823,   774,   177,   177,   187,   188,
     192,   193,   564,   592,   372,   400,   263,   570,   400,   223,
     909,   224,   225,   391,   226,   704,   227,   228,   400,   403,
     941,   366,   851,   283,   237,   241,   243,   245,   246,   247,
     584,   860,   861,   251,   442,   252,   255,   259,   886,   366,
      56,   262,   264,   265,   266,   673,   521,   267,  -352,   408,
     268,   269,   270,   599,   271,   220,   220,   237,   241,   284,
     668,  -180,   445,   643,  -370,   825,   144,   447,   418,   883,
     550,   862,   432,   382,   972,   366,   970,   456,   457,   458,
     459,   521,   292,   585,  -370,  -347,   242,   606,  -370,   608,
     781,   356,   780,  -352,   366,   382,   644,   448,  -347,  -112,
     710,  -180,   186,   144,   293,   404,   941,   890,   345,  -347,
     892,  -352,  -370,   550,  -352,   983,   357,  -352,   382,   282,
     443,   146,   353,   354,   355,   251,   237,   284,   722,   948,
     401,  -352,   443,   401,  -352,   910,     3,   933,    56,  -353,
     711,   360,   908,   401,   822,   241,   295,   147,   254,   258,
     444,   443,  -112,   410,  -352,  -180,  -347,   479,    16,  -352,
     443,  -347,   522,   241,   851,   501,   502,   443,   357,  -352,
     690,   954,   955,   932,  -353,  -112,   625,   982,   357,   357,
     515,   826,   703,   357,   363,   296,   410,   229,  -348,  -340,
     973,   366,   366,   697,   712,   994,   297,   987,  -353,   241,
     177,  -348,  -340,   691,   364,  -353,   411,   412,   413,   988,
     438,    54,  -348,  -340,   357,   626,   435,   170,   241,   410,
     298,   440,   733,   967,   774,  -353,   627,  -342,   645,   410,
     693,   346,   698,   347,    71,   808,   594,   631,   439,   427,
    -342,   150,   436,   699,   628,  -353,   404,   437,   734,   441,
    -353,  -342,   248,  -353,   434,   879,   646,   666,   694,  -348,
    -340,   700,   366,   809,  -348,  -340,   632,   288,   176,   460,
     461,   462,   463,   465,   467,   468,   469,   470,   471,   474,
     475,   515,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,  -342,  -448,
     194,   144,   564,  -342,   241,   241,   818,   906,   535,  -447,
     241,   404,   230,   241,   241,   241,   241,   504,   506,   514,
     123,   675,   678,   693,   249,  -431,   348,  -448,   524,   123,
     349,   633,   527,   404,   528,     5,   404,  -447,  -317,   850,
     123,   404,   123,  -362,   237,   303,   537,   539,   649,   541,
     543,   953,   349,   634,   549,   549,   635,   553,  -317,   555,
    -430,   779,   476,  -362,   985,   496,   497,  -362,   299,   279,
     300,   498,   587,   992,   499,   500,   366,   464,   466,   591,
     350,  -435,   123,   477,   619,  -485,   241,  -485,   723,   725,
     304,  -362,   896,  -357,   305,  -436,  -434,   549,   549,  -484,
     312,  -484,  -433,  -485,   591,  -485,   123,   241,  -432,   241,
     123,   610,   591,   613,   352,   615,   617,   357,   375,    65,
     237,   503,   505,   719,   478,   362,   310,   365,  -357,   312,
     393,   414,   523,   315,   424,   303,   417,   426,   449,   450,
     451,   164,   262,   310,   515,   311,   312,   453,   889,   526,
     315,   538,  -357,   529,   542,   897,   367,   531,   322,  -357,
     530,   534,   532,   544,   564,   554,   123,   328,   282,   571,
     282,   556,   585,   596,   367,   572,   589,   595,   600,  -357,
     304,   850,   777,   620,   305,   537,   652,   541,   655,   553,
     657,   613,   614,   641,   636,   658,   659,   637,   241,  -357,
     241,   638,   639,   665,  -357,   667,   640,  -357,   133,   672,
     367,   -58,   681,   514,   706,   609,   682,   133,  -359,   642,
     684,   685,   702,   709,   720,   721,   714,   728,   133,   367,
     133,   123,   410,   310,   731,   117,   312,   732,   123,   950,
     315,   735,   164,   785,   117,   227,   246,   247,   251,   262,
     284,   788,   790,  -359,   241,   117,   794,   117,   815,   662,
     821,   819,   831,   971,   845,   824,   836,   840,   189,   855,
     133,   -77,   974,   256,   256,   -95,  -100,  -359,   -99,    27,
     231,   -96,  -102,   -98,  -359,  -101,   -78,   -97,    35,   859,
     651,  -208,   654,   123,   133,   881,   884,   117,   133,   893,
     900,   924,   971,   844,  -359,   951,    46,   986,   984,   171,
     602,   601,   830,  -208,   514,   708,   993,   465,   504,   784,
     971,   117,   787,   189,  -359,   117,   367,   367,   872,  -359,
     664,   422,  -359,   853,    27,   231,   916,   798,   912,   800,
     817,   373,   803,    35,   829,   806,  -208,   854,   692,   990,
     810,   966,   811,   -44,   133,   813,   980,   191,   588,   663,
     178,    46,     0,     0,     0,     0,   557,     0,  -208,   -44,
       0,     0,     0,     0,     0,   123,     0,     0,     0,     0,
     237,   117,     0,   558,     0,     0,   559,     0,     0,     0,
     590,   835,     0,   839,     0,   123,   841,   367,   123,     0,
       0,   852,   560,     0,     0,   561,   123,    49,     0,     0,
       0,   917,     0,     0,     0,   590,   562,     0,   279,   133,
     279,   464,   503,   590,     0,     0,   133,     0,     0,     0,
       0,     0,     0,     0,   563,     0,     0,     0,     0,     0,
       0,   858,     0,     0,     0,     0,   117,   303,     0,     0,
       0,     0,     0,   117,   798,   800,     0,   803,   806,   835,
     839,     0,   123,     0,     0,   873,   874,     0,   875,  -264,
     876,   877,     0,     0,   878,     0,     0,   514,   232,   233,
       0,   133,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,     0,   895,   613,   305,  -264,     0,     0,
     901,     0,   902,     0,     0,     0,   904,     0,   117,   661,
       0,   367,     0,     0,     0,   177,     0,     0,   935,     0,
       0,     0,     0,   434,     0,     0,     0,     0,     0,     0,
       0,   647,   873,   918,   919,   876,   920,   921,   922,   923,
       0,   307,     0,   308,   309,   310,     0,   311,   312,   930,
       0,     0,   315,     0,   284,     0,     0,     0,     0,   938,
     322,     0,   939,   133,     0,     0,     0,   326,   327,   328,
       0,     0,   123,     0,     0,     0,     0,     0,     0,     0,
     979,     0,     0,   133,   127,     0,   133,     0,     0,     0,
     117,     0,     0,   127,   133,   962,   963,   964,   965,     0,
       0,     0,     0,     0,   127,   244,   127,     0,     0,     0,
     117,   130,   839,   117,   975,   976,   284,     0,     0,     0,
     130,   117,     0,     0,     0,   962,   963,   964,   965,   975,
     976,   130,   123,   130,     0,     0,     0,     0,     0,     0,
       0,   981,   613,     0,     0,     0,   127,     0,   778,     0,
     133,     0,     0,     0,     0,     0,   613,     0,     0,     0,
     839,     0,     0,   303,     0,    16,   782,   783,     0,    20,
     127,     0,     0,   130,   127,     0,     0,   117,    26,     0,
      27,   846,     0,     0,    31,     0,   123,     0,   123,    35,
       0,   123,  -208,     0,   123,     0,     0,   130,     0,     0,
       0,   130,     0,   123,     0,     0,   123,    46,   304,   189,
       0,   286,   305,   847,  -208,     0,   848,     0,    54,     0,
      27,   231,     0,     0,     0,     0,     0,     0,   123,    35,
     127,     0,  -208,   376,   379,   383,     0,     0,    69,   -43,
     123,    71,   123,     0,   729,     0,   123,    46,     0,     0,
       0,     0,     0,     0,  -208,   -43,     0,   130,     0,     0,
     133,   310,  -194,   311,   312,     0,  -194,  -194,   315,     0,
     406,     0,     0,   407,     0,  -194,   322,  -194,  -194,     0,
       0,  -194,     0,   326,   327,   328,  -194,   117,     5,  -194,
       0,     0,   416,    86,     0,   127,     0,     0,     0,   887,
      14,   557,   127,     0,  -194,     0,  -194,    19,  -194,     0,
    -194,  -194,   430,  -194,   123,  -194,     0,  -194,   558,   123,
     133,   559,   130,     0,     0,    33,     0,     0,     0,   130,
      36,     0,     0,   123,     0,  -194,    41,   560,  -194,     0,
     561,  -194,   286,     0,     0,   123,     0,   117,     0,     0,
       0,   562,     0,     0,   123,     0,    53,   127,   123,     0,
       0,     0,   123,     0,     0,     0,     0,     0,    62,   563,
     936,     0,    65,     0,   133,     0,   133,     0,    70,   133,
       0,     0,   133,     0,   130,     0,     0,     0,     0,     0,
    -194,   133,     0,   303,   133,     0,  -194,     0,     0,   123,
       0,   117,     0,   117,     0,     0,   117,   525,     0,   117,
       0,     0,     0,   131,   968,     0,   133,     0,   117,     0,
       0,   117,   131,     0,     0,     0,     0,     0,   133,     0,
     133,   545,     0,   131,   133,   131,     0,     0,   304,   127,
       0,     0,   305,   117,   123,   123,   123,   123,     0,     0,
       0,     0,     0,   968,     0,   117,     0,   117,     0,   127,
       0,   117,   127,   123,   123,     0,   130,     0,     0,     0,
     127,   968,     0,     0,     0,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   130,   307,     0,   130,
     309,   310,     0,   311,   312,     0,     0,   130,   315,   131,
       0,     0,   133,   131,     0,     0,   322,   133,   557,     0,
     135,     0,     0,   326,   327,   328,     0,   303,     0,   135,
       0,   133,     0,     0,     0,   558,   127,     0,   559,   117,
     135,     0,   135,   133,   117,     0,     0,     0,     0,     0,
       0,     0,   133,     0,   560,     0,   133,   561,   117,    49,
     133,     0,     0,   130,     0,     0,     0,     0,   562,   131,
     117,     0,   304,     0,     0,     0,   305,     0,     0,   117,
       0,     0,   135,   117,     0,     0,   563,   117,     0,     0,
       0,     0,     0,     0,     0,   669,   671,   133,     0,   674,
     677,     0,     0,     0,     0,     0,   135,     0,     0,     0,
     135,     0,     0,     0,     0,     0,     0,   136,     0,     0,
     213,   307,     0,     0,   117,   310,   136,   311,   312,   303,
       0,     0,   315,     0,   131,     0,     0,   136,     0,   136,
     322,   131,   133,   133,   133,   133,   127,   326,   327,   328,
       0,     0,   260,     0,     0,     0,     0,     0,     0,     0,
       0,   133,   133,     0,     0,     0,   135,     0,     0,   117,
     117,   117,   117,   130,   304,     0,     0,     0,   305,   136,
       0,     0,     0,     0,     0,     0,     0,     0,   117,   117,
       0,     0,     0,     0,     0,     0,   131,   290,     0,   291,
       0,     0,     0,   136,     0,     0,   127,   136,     0,     0,
       0,     0,     0,     0,     0,   792,   793,     0,     0,     0,
       0,     0,   795,   307,     0,   308,   309,   310,     0,   311,
     312,   135,   804,   130,   315,   807,     0,     0,   135,     0,
       0,   321,   322,     0,     0,     0,   325,     0,     0,   326,
     327,   328,     0,   260,     0,     0,   221,     0,     0,     0,
     127,     0,   127,   136,   260,   127,     0,   374,   127,     0,
       0,     0,     0,     0,   221,     0,     0,   127,   131,     0,
     127,     0,     0,   253,   257,     0,     0,   130,     0,   130,
       0,     0,   130,   135,     0,   130,     0,     0,   131,   397,
       0,   131,   127,     0,   130,     0,     0,   130,     0,   131,
       0,     0,     0,     0,   127,     0,   127,   409,     0,     0,
     127,     0,     0,     0,     0,     0,     0,     0,   136,   130,
       0,     0,     0,     0,     0,   136,     0,     0,     0,     0,
       0,   130,     0,   130,     0,     0,     0,   130,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,   131,     0,     0,     0,     0,
       0,     0,   145,     0,     0,   135,     0,     0,     0,     0,
       0,     0,     0,   172,   358,   175,   899,     0,   127,     0,
     136,     0,     0,   127,     0,   135,     0,     0,   135,     0,
       0,     0,   221,   221,   221,     0,   135,   127,     0,   389,
       0,     0,     0,     0,     0,   130,     0,     0,     0,   127,
     130,     0,     0,     0,     0,   250,     0,     0,   127,     0,
       0,     0,   127,     0,   130,     0,   127,     0,     0,   221,
       0,     0,   221,     0,     0,     0,   130,     0,     0,    93,
       0,     0,     0,   287,     0,   130,   533,     0,     0,   130,
       0,   221,   135,   130,     0,     0,     0,     0,     0,     0,
       0,     0,   136,   127,     0,   131,     0,     0,     0,   397,
       0,   221,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   136,     0,     0,   136,     0,     0,     0,     0,
     130,     0,     0,   136,     0,     0,     0,     0,     0,   351,
       0,     0,     0,     0,     0,     0,     0,     0,   127,   127,
     127,   127,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   618,     0,   131,   622,   127,   127,     0,
       0,     0,     0,     0,     0,   130,   130,   130,   130,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   136,
       0,     0,     0,     0,   130,   130,     0,     0,     0,     0,
       0,     0,   135,     0,   392,     0,   221,     0,     0,     0,
     303,   405,     0,     0,     0,     0,     0,     0,     0,   131,
       0,   131,     0,   871,   131,     0,     0,   131,     0,     0,
     221,     0,     0,     0,     0,     0,   131,     0,     0,   131,
       0,  -194,     0,     0,     0,  -194,  -194,     0,     0,     0,
       0,     0,     0,     0,  -194,   304,  -194,  -194,     0,   305,
    -194,   131,   135,     0,     0,  -194,    93,     0,  -194,     0,
       0,     0,     0,   131,     0,   131,     0,     0,     0,   131,
     696,     0,     0,  -194,   397,  -194,     0,  -194,     0,  -194,
    -194,     0,  -194,     0,  -194,     0,  -194,     0,     0,   136,
       0,     0,     0,     0,   307,     0,   308,   309,   310,   717,
     311,   312,   313,   260,  -194,   315,   135,  -194,   135,     0,
    -194,   135,   321,   322,   135,     0,     0,   325,     0,     0,
     326,   327,   328,   135,     0,     0,   135,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   131,    93,     0,
       0,     0,   131,     0,     0,     0,     0,     0,   135,   136,
       0,     0,     0,     0,     0,     0,   131,     0,   405,  -194,
     135,   405,   135,     0,     0,  -194,   135,     0,   131,     0,
       0,     0,     0,     0,   221,   221,     0,   131,   221,   221,
       0,   131,     0,     0,     0,   131,     0,     0,     0,     0,
       0,   689,     0,     0,     0,   816,     0,     0,   397,     0,
       0,     0,     0,   136,     0,   136,     0,     0,   136,     0,
       0,   136,     0,     0,     0,     0,     0,     0,   717,     0,
     136,     0,   131,   136,     0,   612,     0,     0,     0,     0,
       0,     0,   716,     0,   135,     0,   622,     0,     0,   135,
       0,     0,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,   135,   857,     0,     0,   136,     0,   136,
       0,     0,     0,   136,     0,   135,     0,   131,   131,   131,
     131,     0,     0,     0,   135,     0,     0,     0,   135,   195,
     303,     0,   135,     0,     0,     0,   131,   131,     0,     0,
       0,   196,   197,     0,   221,   221,     0,     0,   198,     0,
       0,   221,     0,   389,     0,     0,   199,     0,   389,     0,
       0,   221,     0,     0,   221,     0,   200,     0,   201,   135,
       0,   202,     0,     0,     0,   304,     0,   203,     0,   305,
       0,   136,     0,     0,     0,   687,   136,     0,   204,     0,
       0,     0,     0,     0,     0,     0,     0,   205,     0,     0,
     136,     0,     0,     0,   832,     0,   206,   207,     0,   208,
       0,   209,   136,   210,   135,   135,   135,   135,     0,   211,
     395,   136,   212,     0,   307,   136,   308,   309,   310,   136,
     311,   312,   313,   135,   135,   315,   316,   317,     0,     0,
     319,   320,   321,   322,     0,   726,     0,   325,     0,     0,
     326,   327,   328,     0,     0,     0,     0,     0,   195,     0,
       0,     0,     0,     0,     0,     0,   136,     0,     0,     0,
     196,   197,     0,     0,     0,     0,     0,   198,     0,     0,
     396,     0,     0,     0,     0,   199,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   200,     0,   201,     0,   405,
     202,   405,   301,     0,   405,     0,   203,   405,     0,   302,
       0,   136,   136,   136,   136,   221,   812,   204,     0,   814,
     303,     0,     0,     0,     0,     0,   205,     0,     0,   455,
     136,   136,     0,     0,     0,   206,   207,     0,   208,     0,
     209,   828,   210,     0,     0,     0,     0,     0,   211,     0,
       0,   212,     0,   834,     0,   838,     0,     0,     0,    93,
     389,   389,     0,   389,   389,   304,     0,     0,     0,   305,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     940,     0,     0,     0,     0,     0,   944,     0,   389,     0,
     389,     0,     0,     0,   382,     0,     0,     0,   306,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   216,
       0,     0,     0,     0,   307,     0,   308,   309,   310,     0,
     311,   312,   313,     0,   314,   315,   316,   317,   318,     0,
     319,   320,   321,   322,     0,   323,   324,   325,     0,     0,
     326,   327,   328,     0,     0,     0,   882,     0,     0,   724,
       0,     0,     0,     0,     0,     0,     0,     0,   898,     0,
       0,     0,     0,     0,     0,     0,   940,   903,     0,     0,
       0,   905,     0,     0,     0,    93,     0,     0,     0,    -2,
       4,     0,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,  -324,    21,    22,    23,     0,    24,    25,
       0,    26,   931,    27,    28,    29,    30,    31,    32,    33,
       0,    34,    35,     0,    36,  -208,    37,    38,    39,    40,
      41,    42,   -42,    43,    44,     0,    45,     0,     0,     0,
      46,    47,  -324,    48,    49,    50,    51,  -208,   -42,    52,
      53,    54,     0,  -324,    55,    56,     0,    57,    58,    59,
      60,    61,    62,    63,    64,     0,    65,    66,     0,    67,
      68,    69,    70,     0,    71,    72,    73,  -324,     0,     0,
      74,     0,    75,     0,     0,    76,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    77,    78,    79,    80,     0,     0,     0,     0,    81,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,    83,     0,    84,    85,     0,    86,     0,     0,    87,
       0,    88,     0,    89,   977,    90,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   152,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,   153,
     154,     0,   155,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,    42,     0,    43,    44,     0,
       0,     0,     0,     0,     0,    47,     0,    48,     0,    50,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,    61,    62,    63,   156,     0,
      65,     0,     0,     0,     0,     0,    70,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    76,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    77,    78,    79,    80,     0,
       0,     0,     0,    81,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    82,     0,    83,     0,    84,    85,   234,
       0,     0,     0,    87,     0,   157,     0,    89,     0,   158,
     978,     4,     0,     5,     6,     7,     8,     9,    10,     0,
    -504,     0,    11,    12,    13,    14,    15,  -504,     0,    16,
      17,    18,    19,    20,  -324,    21,    22,    23,  -504,    24,
      25,  -504,    26,     0,    27,    28,    29,    30,    31,    32,
      33,     0,    34,    35,     0,    36,  -208,    37,    38,    39,
      40,    41,    42,   -42,    43,    44,     0,    45,     0,     0,
       0,    46,    47,  -324,    48,     0,    50,    51,  -208,   -42,
      52,    53,    54,  -504,  -324,    55,    56,  -504,    57,    58,
      59,    60,    61,    62,    63,    64,     0,    65,    66,     0,
      67,    68,    69,    70,     0,    71,    72,    73,  -324,     0,
       0,    74,     0,    75,     0,     0,  -504,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -504,    78,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,     0,  -504,  -504,  -504,  -504,  -504,     0,  -504,  -504,
    -504,  -504,    83,  -504,  -504,  -504,     0,    86,  -504,  -504,
    -504,     0,    88,  -504,    89,   272,    90,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -324,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,    32,    33,     0,    34,    35,     0,    36,
    -208,    37,    38,    39,    40,    41,    42,   -42,    43,    44,
       0,    45,     0,     0,     0,    46,    47,  -324,    48,    49,
      50,    51,  -208,   -42,    52,    53,    54,     0,  -324,    55,
      56,     0,    57,    58,    59,    60,    61,    62,    63,    64,
       0,    65,    66,     0,    67,    68,    69,    70,     0,    71,
      72,    73,  -324,     0,     0,    74,     0,    75,     0,     0,
      76,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,    78,    79,    80,
       0,     0,     0,     0,    81,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,    83,     0,    84,    85,
     234,    86,     0,     0,    87,     0,    88,   273,    89,     4,
      90,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -324,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,    32,    33,     0,
      34,    35,     0,    36,  -208,    37,    38,    39,    40,    41,
      42,   -42,    43,    44,     0,    45,     0,     0,     0,    46,
      47,  -324,    48,    49,    50,    51,  -208,   -42,    52,    53,
      54,     0,  -324,    55,    56,     0,    57,    58,    59,    60,
      61,    62,    63,    64,     0,    65,    66,     0,    67,    68,
      69,    70,     0,    71,    72,    73,  -324,     0,     0,    74,
       0,    75,     0,     0,    76,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      77,    78,    79,    80,     0,     0,     0,     0,    81,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
      83,     0,    84,    85,     0,    86,     0,     0,    87,     0,
      88,   433,    89,   272,    90,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,  -324,    21,    22,    23,
       0,    24,    25,     0,    26,     0,    27,    28,    29,    30,
      31,    32,    33,     0,    34,    35,     0,    36,  -208,    37,
      38,    39,    40,    41,    42,   -42,    43,    44,     0,    45,
       0,     0,     0,    46,    47,  -324,    48,    49,    50,    51,
    -208,   -42,    52,    53,    54,     0,  -324,    55,    56,     0,
      57,    58,    59,    60,    61,    62,    63,    64,     0,    65,
      66,     0,    67,    68,    69,    70,     0,    71,    72,    73,
    -324,     0,     0,    74,     0,    75,     0,     0,    76,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    77,    78,    79,    80,     0,     0,
       0,     0,    81,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,    83,     0,    84,    85,     0,    86,
       0,     0,    87,     0,    88,   273,    89,   842,    90,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
    -324,    21,    22,    23,     0,    24,    25,     0,    26,     0,
      27,    28,    29,    30,    31,    32,    33,     0,    34,    35,
       0,    36,  -208,    37,    38,    39,    40,    41,    42,   -42,
      43,    44,     0,    45,     0,     0,     0,    46,    47,  -324,
      48,    49,    50,    51,  -208,   -42,    52,    53,    54,     0,
    -324,    55,    56,     0,    57,    58,    59,    60,    61,    62,
      63,    64,     0,    65,    66,     0,    67,    68,    69,    70,
       0,    71,    72,    73,  -324,     0,     0,    74,     0,    75,
       0,     0,    76,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    77,    78,
      79,    80,     0,     0,     0,     0,    81,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,    83,     0,
      84,    85,     0,    86,     0,     0,    87,     0,    88,   843,
      89,     4,    90,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -324,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,    32,
      33,     0,    34,    35,     0,    36,  -208,    37,    38,    39,
      40,    41,    42,   -42,    43,    44,     0,    45,     0,     0,
       0,    46,    47,  -324,    48,    49,    50,    51,  -208,   -42,
      52,    53,    54,     0,  -324,    55,    56,     0,    57,    58,
      59,    60,    61,    62,    63,    64,     0,    65,    66,     0,
      67,    68,    69,    70,     0,    71,    72,    73,  -324,     0,
       0,    74,     0,    75,     0,     0,    76,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    77,    78,    79,    80,     0,     0,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,    83,     0,    84,    85,     0,    86,     0,     0,
      87,     0,    88,   907,    89,     4,    90,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -324,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,    32,    33,     0,    34,    35,     0,    36,
    -208,    37,    38,    39,    40,    41,    42,   -42,    43,    44,
       0,    45,     0,     0,     0,    46,    47,  -324,    48,   286,
      50,    51,  -208,   -42,    52,    53,    54,     0,  -324,    55,
      56,     0,    57,    58,    59,    60,    61,    62,    63,    64,
       0,    65,    66,     0,    67,    68,    69,    70,     0,    71,
      72,    73,  -324,     0,     0,    74,     0,    75,     0,     0,
      76,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,    78,    79,    80,
       0,     0,     0,     0,    81,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,    83,     0,    84,    85,
       0,    86,     0,     0,    87,     0,    88,     0,    89,     4,
      90,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -324,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,    32,    33,     0,
      34,    35,     0,    36,  -208,    37,    38,    39,    40,    41,
      42,   -42,    43,    44,     0,    45,     0,     0,     0,    46,
      47,  -324,    48,     0,    50,    51,  -208,   -42,    52,    53,
      54,     0,  -324,    55,    56,     0,    57,    58,    59,    60,
      61,    62,    63,    64,     0,    65,    66,     0,    67,    68,
      69,    70,     0,    71,    72,    73,  -324,     0,     0,    74,
       0,    75,     0,     0,    76,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      77,    78,    79,    80,     0,     0,     0,     0,    81,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
      83,     0,    84,    85,     0,    86,     0,     0,    87,     0,
      88,     0,    89,     4,    90,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,  -324,    21,    22,    23,
       0,    24,    25,     0,    26,     0,    27,    28,    29,    30,
      31,   536,    33,     0,    34,    35,     0,    36,  -208,    37,
      38,    39,    40,    41,    42,   -42,    43,    44,     0,    45,
       0,     0,     0,    46,    47,  -324,    48,     0,    50,    51,
    -208,   -42,    52,    53,    54,     0,  -324,    55,    56,     0,
      57,    58,    59,    60,    61,    62,    63,    64,     0,    65,
      66,     0,    67,    68,    69,    70,     0,    71,    72,    73,
    -324,     0,     0,    74,     0,    75,     0,     0,    76,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    77,    78,    79,    80,     0,     0,
       0,     0,    81,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,    83,     0,    84,    85,     0,    86,
       0,     0,    87,     0,    88,     0,    89,     4,    90,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
    -324,    21,    22,    23,     0,    24,    25,     0,    26,     0,
      27,    28,    29,    30,    31,   540,    33,     0,    34,    35,
       0,    36,  -208,    37,    38,    39,    40,    41,    42,   -42,
      43,    44,     0,    45,     0,     0,     0,    46,    47,  -324,
      48,     0,    50,    51,  -208,   -42,    52,    53,    54,     0,
    -324,    55,    56,     0,    57,    58,    59,    60,    61,    62,
      63,    64,     0,    65,    66,     0,    67,    68,    69,    70,
       0,    71,    72,    73,  -324,     0,     0,    74,     0,    75,
       0,     0,    76,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    77,    78,
      79,    80,     0,     0,     0,     0,    81,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,    83,     0,
      84,    85,     0,    86,     0,     0,    87,     0,    88,     0,
      89,     4,    90,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -324,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,   797,
      33,     0,    34,    35,     0,    36,  -208,    37,    38,    39,
      40,    41,    42,   -42,    43,    44,     0,    45,     0,     0,
       0,    46,    47,  -324,    48,     0,    50,    51,  -208,   -42,
      52,    53,    54,     0,  -324,    55,    56,     0,    57,    58,
      59,    60,    61,    62,    63,    64,     0,    65,    66,     0,
      67,    68,    69,    70,     0,    71,    72,    73,  -324,     0,
       0,    74,     0,    75,     0,     0,    76,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    77,    78,    79,    80,     0,     0,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,    83,     0,    84,    85,     0,    86,     0,     0,
      87,     0,    88,     0,    89,     4,    90,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -324,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,   799,    33,     0,    34,    35,     0,    36,
    -208,    37,    38,    39,    40,    41,    42,   -42,    43,    44,
       0,    45,     0,     0,     0,    46,    47,  -324,    48,     0,
      50,    51,  -208,   -42,    52,    53,    54,     0,  -324,    55,
      56,     0,    57,    58,    59,    60,    61,    62,    63,    64,
       0,    65,    66,     0,    67,    68,    69,    70,     0,    71,
      72,    73,  -324,     0,     0,    74,     0,    75,     0,     0,
      76,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,    78,    79,    80,
       0,     0,     0,     0,    81,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,    83,     0,    84,    85,
       0,    86,     0,     0,    87,     0,    88,     0,    89,     4,
      90,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -324,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,   802,    33,     0,
      34,    35,     0,    36,  -208,    37,    38,    39,    40,    41,
      42,   -42,    43,    44,     0,    45,     0,     0,     0,    46,
      47,  -324,    48,     0,    50,    51,  -208,   -42,    52,    53,
      54,     0,  -324,    55,    56,     0,    57,    58,    59,    60,
      61,    62,    63,    64,     0,    65,    66,     0,    67,    68,
      69,    70,     0,    71,    72,    73,  -324,     0,     0,    74,
       0,    75,     0,     0,    76,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      77,    78,    79,    80,     0,     0,     0,     0,    81,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
      83,     0,    84,    85,     0,    86,     0,     0,    87,     0,
      88,     0,    89,     4,    90,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,  -324,    21,    22,    23,
       0,    24,    25,     0,    26,     0,    27,    28,    29,    30,
      31,   805,    33,     0,    34,    35,     0,    36,  -208,    37,
      38,    39,    40,    41,    42,   -42,    43,    44,     0,    45,
       0,     0,     0,    46,    47,  -324,    48,     0,    50,    51,
    -208,   -42,    52,    53,    54,     0,  -324,    55,    56,     0,
      57,    58,    59,    60,    61,    62,    63,    64,     0,    65,
      66,     0,    67,    68,    69,    70,     0,    71,    72,    73,
    -324,     0,     0,    74,     0,    75,     0,     0,    76,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    77,    78,    79,    80,     0,     0,
       0,     0,    81,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,    83,     0,    84,    85,     0,    86,
       0,     0,    87,     0,    88,     0,    89,     4,    90,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
    -324,    21,    22,    23,     0,   827,    25,     0,    26,     0,
      27,    28,    29,    30,    31,    32,    33,     0,    34,    35,
       0,    36,  -208,    37,    38,    39,    40,    41,    42,   -42,
      43,    44,     0,    45,     0,     0,     0,    46,    47,  -324,
      48,     0,    50,    51,  -208,   -42,    52,    53,    54,     0,
    -324,    55,    56,     0,    57,    58,    59,    60,    61,    62,
      63,    64,     0,    65,    66,     0,    67,    68,    69,    70,
       0,    71,    72,    73,  -324,     0,     0,    74,     0,    75,
       0,     0,    76,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    77,    78,
      79,    80,     0,     0,     0,     0,    81,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,    83,     0,
      84,    85,     0,    86,     0,     0,    87,     0,    88,     0,
      89,     4,    90,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -324,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,   833,
      33,     0,    34,    35,     0,    36,  -208,    37,    38,    39,
      40,    41,    42,   -42,    43,    44,     0,    45,     0,     0,
       0,    46,    47,  -324,    48,     0,    50,    51,  -208,   -42,
      52,    53,    54,     0,  -324,    55,    56,     0,    57,    58,
      59,    60,    61,    62,    63,    64,     0,    65,    66,     0,
      67,    68,    69,    70,     0,    71,    72,    73,  -324,     0,
       0,    74,     0,    75,     0,     0,    76,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    77,    78,    79,    80,     0,     0,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,    83,     0,    84,    85,     0,    86,     0,     0,
      87,     0,    88,     0,    89,     4,    90,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -324,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,   837,    33,     0,    34,    35,     0,    36,
    -208,    37,    38,    39,    40,    41,    42,   -42,    43,    44,
       0,    45,     0,     0,     0,    46,    47,  -324,    48,     0,
      50,    51,  -208,   -42,    52,    53,    54,     0,  -324,    55,
      56,     0,    57,    58,    59,    60,    61,    62,    63,    64,
       0,    65,    66,     0,    67,    68,    69,    70,     0,    71,
      72,    73,  -324,     0,     0,    74,     0,    75,     0,     0,
      76,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,    78,    79,    80,
       0,     0,     0,     0,    81,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,    83,     0,    84,    85,
       0,    86,     0,     0,    87,     0,    88,     0,    89,   705,
      90,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     152,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,   153,   154,     0,   155,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
      42,     0,    43,    44,     0,     0,     0,     0,     0,     0,
      47,     0,    48,     0,    50,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
      61,    62,    63,   156,     0,    65,     0,     0,     0,     0,
       0,    70,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    76,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      77,    78,    79,    80,     0,     0,     0,     0,    81,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
      83,     0,    84,    85,   234,     0,     0,     0,    87,     0,
     157,     0,    89,     0,   158,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   152,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,   153,   154,
       0,   155,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,    42,     0,    43,    44,     0,     0,
       0,     0,     0,     0,    47,     0,    48,     0,    50,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,     0,    59,    60,    61,    62,    63,   156,     0,    65,
       0,     0,     0,     0,     0,    70,     0,     0,    72,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    76,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    77,    78,    79,    80,     0,     0,
       0,     0,    81,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,    83,     0,    84,    85,   234,     0,
       0,     0,    87,     0,   157,     0,    89,     0,   158,   611,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   152,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,   153,   154,     0,   155,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,    42,
       0,    43,    44,     0,     0,     0,     0,     0,     0,    47,
       0,    48,     0,    50,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,     0,    59,    60,    61,
      62,    63,   156,     0,    65,     0,     0,     0,     0,     0,
      70,     0,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    76,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    77,
      78,    79,    80,     0,     0,     0,     0,    81,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,    83,
       0,    84,    85,   234,     0,     0,     0,    87,     0,   157,
       0,    89,     0,   158,   934,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   152,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,   153,   154,
       0,   155,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,    42,     0,    43,    44,     0,     0,
       0,     0,     0,     0,    47,     0,    48,     0,    50,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,     0,    59,    60,    61,    62,    63,   156,     0,    65,
       0,     0,     0,     0,     0,    70,     0,     0,    72,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    76,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    77,    78,    79,    80,     0,     0,
       0,     0,    81,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,    83,     0,    84,    85,     0,     0,
       0,     0,    87,     0,   157,     0,    89,     0,   158,   616,
       5,     6,     7,     8,   179,    10,   180,     0,     0,   152,
       0,     0,    14,    15,     0,     0,  -111,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,   153,   154,     0,   155,    33,     0,    34,
       0,     0,    36,  -475,     0,    38,    39,     0,    41,    42,
       0,    43,    44,     0,     0,     0,     0,     0,     0,    47,
       0,    48,     0,    50,     0,  -475,     0,     0,    53,  -111,
       0,     0,     0,     0,     0,     0,     0,    59,    60,    61,
      62,    63,   156,     0,    65,   737,     0,   738,     0,   181,
      70,     0,  -111,    72,   739,     0,     0,     0,     0,   740,
     197,   741,     0,    76,     0,     0,   742,     0,     0,     0,
       0,     0,     0,     0,   199,     0,     0,     0,     0,    77,
      78,    79,    80,     0,   743,     0,   201,    81,     0,   744,
       0,     0,     0,     0,     0,   745,     0,    82,     0,    83,
       0,    84,    85,     0,     0,     0,   204,    87,     0,   157,
       0,    89,     0,   158,     0,   746,     0,     0,     0,     0,
       0,     0,     0,     0,   206,   207,     0,   747,     0,   209,
       0,   748,     0,     0,     0,     0,     0,   749,     0,     0,
     212,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   329,   330,   331,   332,   333,   334,     0,     0,   337,
     338,   339,   340,     0,   342,   343,   750,   751,   752,   753,
       0,     0,   754,     0,     0,     0,   755,   756,   757,   758,
     759,   760,   761,   762,   763,   764,   765,   766,     0,   767,
       0,     0,   768,   769,   770,   771,     0,     0,   772,     5,
       6,     7,     8,   179,    10,     0,     0,     0,   152,     0,
       0,    14,    15,     0,     0,  -111,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,   153,   154,     0,   155,    33,     0,    34,     0,
       0,    36,  -475,     0,    38,    39,     0,    41,    42,     0,
      43,    44,     0,     0,     0,     0,     0,     0,    47,     0,
      48,     0,    50,     0,  -475,     0,     0,    53,  -111,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,    62,
      63,   156,     0,    65,     0,     0,     0,     0,   181,    70,
       0,  -111,    72,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    76,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    77,    78,
      79,    80,     0,     0,     0,     0,    81,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    82,     0,    83,   152,
      84,    85,    14,    15,     0,     0,    87,     0,   157,    19,
      89,     0,   158,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,   153,   154,     0,   155,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,    42,
       0,    43,    44,     0,     0,     0,     0,     0,     0,    47,
       0,    48,     0,    50,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,     0,    59,    60,    61,
      62,    63,   156,     0,    65,     0,     0,   238,   239,     0,
      70,   277,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    76,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    77,
      78,    79,    80,     0,     0,     0,     0,    81,   278,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,    83,
       0,    84,    85,   234,     0,     0,     0,    87,     0,   157,
       0,    89,     0,   158,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   152,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,   153,   154,     0,
     155,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,    42,     0,    43,    44,     0,     0,     0,
       0,     0,     0,    47,     0,    48,     0,    50,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,    62,    63,   156,     0,    65,     0,
       0,   238,   239,     0,    70,   277,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    76,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    77,    78,    79,    80,     0,     0,     0,
       0,    81,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,    83,     0,    84,    85,   234,     0,     0,
       0,    87,     0,   157,     0,    89,   605,   158,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   152,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,   153,   154,     0,   155,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,    42,     0,    43,
      44,     0,     0,     0,     0,     0,     0,    47,     0,    48,
       0,    50,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,    62,    63,
     156,     0,    65,     0,     0,   238,   239,     0,    70,   277,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    76,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    77,    78,    79,
      80,     0,     0,     0,     0,    81,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,    83,     0,    84,
      85,   234,     0,     0,     0,    87,     0,   157,     0,    89,
     607,   158,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   152,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,   507,     0,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,   153,   154,     0,   155,    33,
     508,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,    42,     0,    43,    44,     0,     0,     0,     0,     0,
       0,    47,     0,    48,     0,    50,     0,     0,     0,     0,
      53,     0,     0,   509,     0,     0,     0,     0,     0,    59,
      60,    61,    62,    63,   156,     0,    65,     0,     0,     0,
       0,     0,    70,     0,     0,    72,     0,   510,     0,     0,
       0,     0,     0,     0,     0,    76,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    77,    78,    79,    80,     0,     0,     0,     0,    81,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    82,
       0,    83,   152,    84,    85,    14,    15,     0,     0,    87,
       0,   157,    19,    89,     0,   158,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,   153,   154,     0,   155,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,    42,     0,    43,    44,     0,     0,     0,     0,
       0,     0,    47,     0,    48,     0,    50,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,     0,
      59,    60,    61,    62,    63,   156,     0,    65,     0,     0,
     238,   239,     0,    70,     0,     0,    72,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    76,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    77,    78,    79,    80,     0,     0,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,    83,     0,    84,    85,     0,   240,     0,     0,
      87,     0,   157,     0,    89,     0,   158,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   152,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
     153,   154,     0,   155,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,    42,     0,    43,    44,
       0,     0,     0,     0,     0,     0,    47,     0,    48,     0,
      50,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,    61,    62,    63,   156,
       0,    65,     0,     0,   238,   239,     0,    70,     0,     0,
      72,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      76,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,    78,    79,    80,
       0,     0,     0,     0,    81,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,    83,     0,    84,    85,
     234,     0,     0,     0,    87,     0,   157,     0,    89,     0,
     158,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     152,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,   153,   154,     0,   155,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
      42,     0,    43,    44,   707,     0,     0,     0,     0,     0,
      47,     0,    48,     0,    50,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
      61,    62,    63,   156,     0,    65,     0,     0,   238,   239,
       0,    70,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    76,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      77,    78,    79,    80,     0,     0,     0,     0,    81,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    82,     0,
      83,   152,    84,    85,    14,    15,     0,     0,    87,     0,
     157,    19,    89,     0,   158,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,   153,   154,     0,   155,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,    42,     0,    43,    44,     0,     0,     0,     0,     0,
       0,    47,   184,    48,     0,    50,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,    63,   156,     0,    65,     0,     0,     0,
       0,     0,    70,     0,     0,    72,     0,     0,     0,     0,
       0,     0,     0,   185,     0,    76,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    77,    78,    79,    80,     0,     0,     0,     0,    81,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,    83,     0,    84,    85,     0,     0,     0,     0,    87,
       0,   157,     0,    89,     0,   158,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   152,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,   189,     0,     0,     0,
       0,     0,     0,    25,     0,     0,     0,     0,   190,   153,
     154,     0,   155,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,    42,     0,    43,    44,     0,
       0,     0,     0,     0,     0,    47,     0,    48,     0,    50,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,    61,    62,    63,   156,     0,
      65,     0,     0,     0,     0,     0,    70,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    76,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    77,    78,    79,    80,     0,
       0,     0,     0,    81,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    82,     0,    83,   152,    84,    85,    14,
      15,     0,     0,    87,     0,   157,    19,    89,     0,   158,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
     153,   154,     0,   155,    33,     0,    34,     0,     0,    36,
    -475,     0,    38,    39,     0,    41,    42,     0,    43,    44,
       0,     0,     0,     0,     0,     0,    47,     0,    48,     0,
      50,     0,  -475,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,    61,    62,    63,   156,
       0,    65,     0,     0,     0,     0,     0,    70,     0,     0,
      72,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      76,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,    78,    79,    80,
       0,     0,     0,     0,    81,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    82,     0,    83,   152,    84,    85,
      14,    15,     0,     0,    87,     0,   157,    19,    89,     0,
     158,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,   153,   154,     0,   155,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,    42,     0,    43,
      44,     0,     0,     0,     0,     0,     0,    47,     0,    48,
       0,    50,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,    62,    63,
     156,     0,    65,     0,     0,   238,   239,     0,    70,     0,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    76,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    77,    78,    79,
      80,     0,     0,     0,     0,    81,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    82,     0,    83,   152,    84,
      85,    14,    15,     0,     0,    87,     0,   157,    19,    89,
       0,   158,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,   153,   154,     0,   155,    33,     0,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,    42,     0,
      43,    44,     0,     0,     0,     0,     0,     0,    47,     0,
      48,     0,    50,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,    62,
      63,   156,     0,    65,     0,     0,     0,     0,     0,    70,
       0,     0,    72,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    76,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    77,    78,
      79,    80,     0,     0,     0,     0,    81,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,    83,     0,
      84,    85,   234,     0,     0,     0,    87,     0,   157,   603,
      89,     0,   158,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   152,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,   220,
      25,     0,     0,     0,     0,     0,   153,   154,     0,   155,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,    42,     0,    43,    44,     0,     0,     0,     0,
       0,     0,    47,     0,    48,     0,    50,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,     0,
      59,    60,    61,    62,    63,   156,     0,    65,     0,     0,
       0,     0,     0,    70,     0,     0,    72,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    76,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    77,    78,    79,    80,     0,     0,     0,     0,
      81,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      82,     0,    83,   152,    84,    85,    14,    15,     0,     0,
      87,     0,    88,    19,    89,     0,   158,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,   153,   154,     0,
     155,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,    42,     0,    43,    44,     0,     0,     0,
       0,     0,     0,    47,     0,    48,     0,    50,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,    62,    63,   156,     0,    65,     0,
       0,     0,     0,     0,    70,     0,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    76,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    77,    78,    79,    80,     0,     0,     0,
       0,    81,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,    83,     0,    84,    85,   234,     0,     0,
       0,    87,     0,   157,     0,    89,     0,   158,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   152,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,   153,   154,     0,   155,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,    42,     0,    43,
      44,     0,     0,     0,     0,     0,     0,    47,     0,    48,
       0,    50,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,    62,    63,
     156,     0,    65,     0,     0,     0,     0,     0,    70,     0,
       0,    72,     0,     0,     0,     0,     0,     0,     0,   185,
       0,    76,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    77,    78,    79,
      80,     0,     0,     0,     0,    81,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    82,     0,    83,   152,    84,
      85,    14,   573,     0,     0,    87,     0,   157,    19,    89,
       0,   158,     0,     0,     0,     0,   574,     0,     0,     0,
       0,     0,   153,   154,     0,   155,    33,     0,   575,     0,
       0,    36,     0,     0,    38,    39,     0,    41,    42,     0,
      43,    44,     0,     0,     0,     0,     0,     0,   576,     0,
      48,     0,    50,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,     0,   577,   578,    61,    62,
      63,   579,     0,    65,     0,     0,     0,     0,     0,    70,
       0,     0,   580,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    76,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    77,    78,
      79,    80,     0,     0,     0,     0,    81,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,    83,     0,
      84,    85,   234,     0,     0,     0,    87,     0,   157,     0,
      89,     0,   885,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   152,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,   153,   154,     0,   155,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,    42,     0,    43,    44,     0,     0,     0,     0,
       0,     0,    47,     0,    48,     0,    50,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,     0,
      59,    60,    61,    62,    63,   156,     0,    65,     0,     0,
       0,     0,     0,    70,     0,     0,    72,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    76,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    77,    78,    79,    80,     0,     0,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,    83,     0,    84,    85,   234,     0,     0,     0,
      87,     0,   157,     0,    89,     0,   885,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   152,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
     153,   154,     0,   870,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,    42,     0,    43,    44,
       0,     0,     0,     0,     0,     0,    47,     0,    48,     0,
      50,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,    61,    62,    63,   156,
       0,    65,     0,     0,     0,     0,     0,    70,     0,     0,
      72,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      76,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,    78,    79,    80,
       0,     0,     0,     0,    81,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,    83,     0,    84,    85,
     234,     0,     0,     0,    87,     0,   157,     0,    89,     0,
     158,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     152,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,   153,   154,     0,   155,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
      42,     0,    43,    44,     0,     0,     0,     0,     0,     0,
      47,     0,    48,     0,    50,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
      61,    62,    63,   156,     0,    65,     0,     0,     0,     0,
       0,    70,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    76,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      77,    78,    79,    80,     0,     0,     0,     0,    81,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    82,     0,
      83,   152,    84,    85,    14,    15,     0,     0,    87,     0,
     157,    19,    89,     0,   158,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,   153,   154,     0,   155,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,    42,     0,    43,    44,     0,     0,     0,     0,     0,
       0,    47,     0,    48,     0,    50,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,    63,   156,     0,    65,     0,     0,     0,
       0,     0,    70,     0,     0,    72,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    76,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    77,    78,    79,    80,     0,     0,     0,     0,    81,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    82,
       0,    83,   152,    84,    85,    14,   472,     0,     0,    87,
       0,    88,    19,    89,     0,   158,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,   153,   154,     0,   155,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,    42,     0,    43,    44,     0,     0,     0,     0,
       0,     0,    47,     0,    48,     0,    50,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,     0,
      59,    60,    61,    62,    63,   156,     0,    65,     0,     0,
       0,     0,     0,    70,     0,     0,   473,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    76,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    77,    78,    79,    80,     0,     0,     0,     0,
      81,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      82,     0,    83,   152,    84,    85,    14,    15,     0,     0,
      87,     0,   157,    19,    89,     0,   158,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,   153,   154,     0,
     155,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,    42,     0,    43,    44,     0,     0,     0,
       0,     0,     0,    47,     0,    48,     0,    50,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,    62,    63,   156,     0,    65,     0,
       0,     0,     0,     0,    70,     0,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    76,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    77,    78,    79,    80,     0,     0,     0,
       0,    81,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    82,     0,    83,   152,    84,    85,    14,   573,     0,
       0,   546,     0,   157,    19,    89,     0,   158,     0,     0,
       0,     0,   574,     0,     0,     0,     0,     0,   153,   154,
       0,   155,    33,     0,   575,     0,     0,    36,     0,     0,
      38,    39,     0,    41,    42,     0,    43,    44,     0,     0,
       0,     0,     0,     0,   576,     0,    48,     0,    50,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,     0,   577,   578,    61,    62,    63,   579,     0,    65,
       0,     0,     0,     0,     0,    70,     0,     0,   580,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    76,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    77,    78,    79,    80,     0,     0,
       0,     0,    81,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    82,     0,    83,   152,    84,    85,    14,    15,
       0,     0,    87,     0,   157,    19,    89,     0,   581,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,   153,
     154,     0,   155,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,    42,     0,    43,    44,     0,
       0,     0,     0,     0,     0,    47,     0,    48,     0,    50,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,    61,    62,    63,   156,     0,
      65,     0,     0,     0,     0,     0,    70,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    76,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    77,    78,    79,    80,     0,
       0,     0,     0,    81,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    82,     0,    83,   152,    84,    85,    14,
      15,     0,     0,    87,     0,   157,    19,    89,     0,   581,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
     153,   154,     0,   650,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,    42,     0,    43,    44,
       0,     0,     0,     0,     0,     0,    47,     0,    48,     0,
      50,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,    61,    62,    63,   156,
       0,    65,     0,     0,     0,     0,     0,    70,     0,     0,
      72,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      76,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,    78,    79,    80,
       0,     0,     0,     0,    81,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    82,     0,    83,   152,    84,    85,
      14,    15,     0,     0,    87,     0,   157,    19,    89,     0,
     158,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,   153,   154,     0,   653,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,    42,     0,    43,
      44,     0,     0,     0,     0,     0,     0,    47,     0,    48,
       0,    50,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,    62,    63,
     156,     0,    65,     0,     0,     0,     0,     0,    70,     0,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    76,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    77,    78,    79,
      80,     0,     0,     0,     0,    81,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    82,     0,    83,   152,    84,
      85,    14,    15,     0,     0,    87,     0,   157,    19,    89,
       0,   158,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,   153,   154,     0,   864,    33,     0,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,    42,     0,
      43,    44,     0,     0,     0,     0,     0,     0,    47,     0,
      48,     0,    50,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,    62,
      63,   156,     0,    65,     0,     0,     0,     0,     0,    70,
       0,     0,    72,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    76,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    77,    78,
      79,    80,     0,     0,     0,     0,    81,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    82,     0,    83,   152,
      84,    85,    14,    15,     0,     0,    87,     0,   157,    19,
      89,     0,   158,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,   153,   154,     0,   865,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,    42,
       0,    43,    44,     0,     0,     0,     0,     0,     0,    47,
       0,    48,     0,    50,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,     0,    59,    60,    61,
      62,    63,   156,     0,    65,     0,     0,     0,     0,     0,
      70,     0,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    76,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    77,
      78,    79,    80,     0,     0,     0,     0,    81,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    82,     0,    83,
     152,    84,    85,    14,    15,     0,     0,    87,     0,   157,
      19,    89,     0,   158,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,   153,   154,     0,   867,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
      42,     0,    43,    44,     0,     0,     0,     0,     0,     0,
      47,     0,    48,     0,    50,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
      61,    62,    63,   156,     0,    65,     0,     0,     0,     0,
       0,    70,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    76,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      77,    78,    79,    80,     0,     0,     0,     0,    81,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    82,     0,
      83,   152,    84,    85,    14,    15,     0,     0,    87,     0,
     157,    19,    89,     0,   158,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,   153,   154,     0,   868,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,    42,     0,    43,    44,     0,     0,     0,     0,     0,
       0,    47,     0,    48,     0,    50,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,    63,   156,     0,    65,     0,     0,     0,
       0,     0,    70,     0,     0,    72,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    76,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    77,    78,    79,    80,     0,     0,     0,     0,    81,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    82,
       0,    83,   152,    84,    85,    14,    15,     0,     0,    87,
       0,   157,    19,    89,     0,   158,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,   153,   154,     0,   869,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,    42,     0,    43,    44,     0,     0,     0,     0,
       0,     0,    47,     0,    48,     0,    50,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,     0,
      59,    60,    61,    62,    63,   156,     0,    65,     0,     0,
       0,     0,     0,    70,     0,     0,    72,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    76,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    77,    78,    79,    80,     0,     0,     0,     0,
      81,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      82,     0,    83,   152,    84,    85,    14,    15,     0,     0,
      87,     0,   157,    19,    89,     0,   158,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,   153,   154,     0,
     870,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,    42,     0,    43,    44,     0,     0,     0,
       0,     0,     0,    47,     0,    48,     0,    50,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,    62,    63,   156,     0,    65,     0,
       0,     0,     0,     0,    70,     0,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    76,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    77,    78,    79,    80,     0,     0,     0,
       0,    81,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    82,     0,    83,   152,    84,    85,    14,   573,     0,
       0,    87,     0,   157,    19,    89,     0,   158,     0,     0,
       0,     0,   574,     0,     0,     0,     0,     0,   153,   154,
       0,   155,    33,     0,   575,     0,     0,    36,     0,     0,
      38,    39,     0,    41,    42,     0,    43,    44,     0,     0,
       0,     0,     0,     0,   576,     0,    48,     0,    50,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,     0,   577,   578,    61,    62,    63,   579,     0,    65,
       0,     0,     0,     0,     0,    70,     0,     0,   580,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    76,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    77,    78,    79,    80,     0,     0,
       0,     0,    81,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    82,     0,    83,   152,    84,    85,    14,    15,
       0,     0,    87,     0,   157,    19,    89,     0,   947,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,   153,
     154,     0,   155,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,    42,     0,    43,    44,     0,
       0,     0,     0,     0,     0,    47,     0,    48,     0,    50,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,    61,    62,    63,   156,     0,
      65,     0,     0,     0,     0,     0,    70,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    76,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    77,    78,    79,    80,     0,
       0,     0,     0,    81,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    82,     0,    83,   152,    84,    85,    14,
      15,     0,     0,    87,     0,     0,    19,    89,     0,   947,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
     153,   154,     0,   155,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,    42,     0,    43,    44,
       0,     0,     0,     0,     0,     0,    47,     0,    48,     0,
      50,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,    61,    62,    63,   156,
       0,    65,     0,     0,     0,     0,     0,    70,     0,     0,
      72,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,    78,    79,    80,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    82,     0,    83,   152,     0,    85,
      14,    15,     0,     0,    87,     0,   157,    19,    89,     0,
     158,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,   153,   154,     0,   155,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,    42,     0,    43,
      44,     0,     0,     0,     0,     0,     0,    47,     0,    48,
       0,    50,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,    62,    63,
     156,     0,    65,     0,     0,     0,     0,     0,    70,     0,
       0,    72,     0,     0,     5,     6,     7,     8,     9,    10,
       0,     0,  -361,   152,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,    78,     0,
       0,    25,  -361,     0,     0,     0,  -361,   153,   154,     0,
     155,    33,     0,    34,     0,     0,    36,    83,     0,    38,
      39,   234,    41,    42,     0,    43,    44,   157,     0,    89,
    -361,   885,     0,    47,     0,    48,     0,    50,     0,     0,
       0,     0,    53,     0,     0,     0,     0,  -343,     0,     0,
       0,    59,    60,    61,    62,    63,   156,     0,    65,     0,
    -343,     0,     0,     0,    70,     0,     0,    72,     0,     0,
       0,  -343,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,     0,     0,     0,
       0,     0,     0,     0,    78,     5,     6,     7,     8,     9,
      10,     0,  -455,     0,   152,     0,     0,    14,    15,     0,
       0,     0,     0,    83,    19,     0,   344,     0,  -343,  -484,
       0,  -484,    25,  -343,     0,    89,     0,   947,   153,   154,
       0,   155,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,    42,     0,    43,    44,     0,     0,
       0,     0,     0,     0,    47,     0,    48,     0,    50,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,     0,    59,    60,    61,    62,    63,   156,     0,    65,
       0,     0,     0,     0,     0,    70,     0,     0,    72,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   152,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,    78,     0,    25,     0,     0,
       0,     0,     0,   153,   154,     0,   155,    33,     0,    34,
       0,     0,    36,     0,    83,    38,    39,     0,    41,    42,
       0,    43,    44,     0,   157,     0,    89,     0,   158,    47,
       0,    48,     0,    50,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,     0,    59,    60,    61,
      62,    63,   156,     0,    65,     0,     0,     0,     0,     0,
      70,     0,     0,    72,     0,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   152,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
      78,     0,    25,   195,     0,     0,     0,     0,   153,   154,
       0,   155,    33,     0,    34,   196,   197,    36,     0,    83,
      38,    39,   198,    41,    42,     0,    43,    44,     0,   157,
     199,    89,     0,   581,    47,     0,    48,     0,    50,     0,
     200,     0,   201,    53,     0,   202,     0,     0,     0,     0,
       0,   203,    59,    60,    61,    62,    63,   156,     0,    65,
       0,     0,   204,   195,     0,    70,     0,     0,    72,     0,
       0,   205,     0,     0,     0,   196,   197,     0,     0,     0,
     206,   207,   198,   208,     0,   209,     0,   210,     0,     0,
     199,     0,     0,   211,     0,    78,   212,     0,     0,     0,
     200,     0,   201,     0,     0,   202,     0,     0,     0,     0,
       0,   203,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,   204,   195,     0,     0,    89,     0,   158,     0,
       0,   205,     0,     0,     0,   196,   197,     0,     0,     0,
     206,   207,   198,   208,     0,   209,     0,   210,     0,     0,
     199,     0,   357,   211,   715,     0,   212,     0,     0,     0,
     200,     0,   201,     0,     0,   202,     0,     0,     0,     0,
       0,   203,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   204,   914,     0,     0,     0,     0,     0,     0,
     739,   205,     0,     0,     0,   196,   197,   741,     0,     0,
     206,   207,   198,   208,     0,   209,     0,   210,     0,     0,
     199,     0,     0,   211,   216,     0,   212,     0,     0,     0,
     200,     0,   201,     0,     0,   202,     0,     0,     0,     0,
       0,   203,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   204,     0,     0,     0,     0,     0,     0,     0,
       0,   205,     0,     0,     0,     0,     0,     0,     0,     0,
     206,   207,     0,   208,     0,   209,     0,   915,     0,     0,
       0,     0,     0,   211,   695,   301,   212,     0,     0,     0,
       0,     0,   302,     0,     0,     0,     0,   329,   330,   331,
     332,   333,   334,   303,     0,   337,   338,   339,   340,     0,
     342,   343,   750,   751,   752,   753,     0,     0,   754,     0,
       0,     0,   755,   756,   757,   758,   759,   760,   761,   762,
     763,   764,   765,   766,     0,   767,     0,     0,   768,   769,
     770,   771,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,   305,     0,   301,     0,     0,     0,     0,     0,
       0,   302,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   303,     0,     0,     0,     0,     0,     0,     0,
       0,   306,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   307,     0,   308,
     309,   310,     0,   311,   312,   313,     0,   314,   315,   316,
     317,   318,     0,   319,   320,   321,   322,   304,   323,   324,
     325,   305,     0,   326,   327,   328,     0,     0,   301,     0,
       0,     0,   791,     0,     0,   302,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   303,     0,     0,     0,
     306,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   307,     0,   308,   309,
     310,     0,   311,   312,   313,     0,   314,   315,   316,   317,
     318,     0,   319,   320,   321,   322,     0,   323,   324,   325,
       0,   304,   326,   327,   328,   305,     0,   301,     0,     0,
       0,   943,     0,     0,   302,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   303,     0,     0,     0,     0,
       0,     0,     0,     0,   306,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     307,     0,   308,   309,   310,     0,   311,   312,   313,     0,
     314,   315,   316,   317,   318,     0,   319,   320,   321,   322,
     304,   323,   324,   325,   305,     0,   326,   327,   328,     0,
       0,   301,     0,     0,     0,   991,     0,     0,   302,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   303,
       0,     0,     0,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   307,
       0,   308,   309,   310,     0,   311,   312,   313,     0,   314,
     315,   316,   317,   318,     0,   319,   320,   321,   322,     0,
     323,   324,   325,     0,   304,   326,   327,   328,   305,     0,
       0,     0,   604,     0,     0,   301,     0,     0,     0,     0,
       0,     0,   302,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   303,     0,     0,     0,   306,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   307,     0,   308,   309,   310,     0,   311,
     312,   313,     0,   314,   315,   316,   317,   318,     0,   319,
     320,   321,   322,     0,   323,   324,   325,     0,   304,   326,
     327,   328,   305,     0,     0,     0,   913,     0,     0,   301,
       0,   388,     0,     0,     0,     0,   302,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   303,     0,     0,
       0,   306,   329,   330,   331,   332,   333,   334,     0,     0,
     337,   338,   339,   340,     0,   342,   343,   307,     0,   308,
     309,   310,     0,   311,   312,   313,     0,   314,   315,   316,
     317,   318,     0,   319,   320,   321,   322,     0,   323,   324,
     325,     0,   304,   326,   327,   328,   305,   357,     0,     0,
       0,     0,     0,   301,     0,   796,     0,     0,     0,     0,
     302,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   303,     0,     0,     0,   306,   329,   330,   331,   332,
     333,   334,     0,     0,   337,   338,   339,   340,     0,   342,
     343,   307,     0,   308,   309,   310,     0,   311,   312,   313,
       0,   314,   315,   316,   317,   318,     0,   319,   320,   321,
     322,     0,   323,   324,   325,     0,   304,   326,   327,   328,
     305,   357,     0,     0,     0,     0,     0,   301,     0,   801,
       0,     0,     0,     0,   302,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   303,     0,     0,     0,   306,
     329,   330,   331,   332,   333,   334,     0,     0,   337,   338,
     339,   340,     0,   342,   343,   307,     0,   308,   309,   310,
       0,   311,   312,   313,     0,   314,   315,   316,   317,   318,
       0,   319,   320,   321,   322,     0,   323,   324,   325,     0,
     304,   326,   327,   328,   305,   357,     0,     0,     0,     0,
       0,   301,     0,   926,     0,     0,     0,     0,   302,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   303,
       0,     0,     0,   306,   329,   330,   331,   332,   333,   334,
       0,     0,   337,   338,   339,   340,     0,   342,   343,   307,
       0,   308,   309,   310,     0,   311,   312,   313,     0,   314,
     315,   316,   317,   318,     0,   319,   320,   321,   322,     0,
     323,   324,   325,     0,   304,   326,   327,   328,   305,   357,
       0,     0,     0,     0,     0,   301,     0,   927,     0,     0,
       0,     0,   302,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   303,     0,     0,     0,   306,   329,   330,
     331,   332,   333,   334,     0,     0,   337,   338,   339,   340,
       0,   342,   343,   307,     0,   308,   309,   310,     0,   311,
     312,   313,     0,   314,   315,   316,   317,   318,     0,   319,
     320,   321,   322,     0,   323,   324,   325,     0,   304,   326,
     327,   328,   305,   357,     0,     0,     0,     0,     0,   301,
       0,   928,     0,     0,     0,     0,   302,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   303,     0,     0,
       0,   306,   329,   330,   331,   332,   333,   334,     0,     0,
     337,   338,   339,   340,     0,   342,   343,   307,     0,   308,
     309,   310,     0,   311,   312,   313,     0,   314,   315,   316,
     317,   318,     0,   319,   320,   321,   322,     0,   323,   324,
     325,     0,   304,   326,   327,   328,   305,   357,     0,     0,
       0,     0,     0,   301,     0,   929,     0,     0,     0,     0,
     302,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   303,     0,     0,     0,   306,   329,   330,   331,   332,
     333,   334,     0,     0,   337,   338,   339,   340,     0,   342,
     343,   307,     0,   308,   309,   310,     0,   311,   312,   313,
       0,   314,   315,   316,   317,   318,     0,   319,   320,   321,
     322,     0,   323,   324,   325,     0,   304,   326,   327,   328,
     305,   357,     0,     0,     0,     0,     0,   301,     0,   945,
       0,     0,     0,     0,   302,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   303,     0,     0,     0,   306,
     329,   330,   331,   332,   333,   334,     0,     0,   337,   338,
     339,   340,     0,   342,   343,   307,     0,   308,   309,   310,
       0,   311,   312,   313,     0,   314,   315,   316,   317,   318,
       0,   319,   320,   321,   322,     0,   323,   324,   325,     0,
     304,   326,   327,   328,   305,   357,     0,     0,     0,     0,
       0,     0,     0,   946,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   306,   329,   330,   331,   332,   333,   334,
       0,     0,   337,   338,   339,   340,     0,   342,   343,   307,
       0,   308,   309,   310,     0,   311,   312,   313,     0,   314,
     315,   316,   317,   318,     0,   319,   320,   321,   322,     0,
     323,   324,   325,   301,     0,   326,   327,   328,     0,   357,
     302,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   303,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   381,     0,    16,     0,     0,     0,
      20,     0,     0,   301,     0,     0,     0,     0,     0,    26,
     302,    27,   846,     0,     0,    31,     0,     0,     0,     0,
      35,   303,   377,  -208,     0,     0,   304,     0,     0,     0,
     305,     0,     0,     0,   378,     0,     0,     0,    46,     0,
       0,     0,    49,   301,   847,  -208,     0,   848,     0,    54,
     302,     0,     0,     0,     0,   382,     0,     0,     0,   306,
       0,   303,   676,     0,     0,     0,   304,     0,     0,    69,
     305,     0,    71,     0,     0,   307,     0,   308,   309,   310,
       0,   311,   312,   313,     0,   314,   315,   316,   317,   318,
       0,   319,   320,   321,   322,     0,   323,   324,   325,   306,
       0,   326,   327,   328,     0,   357,   304,     0,     0,     0,
     305,     0,     0,     0,     0,   307,     0,   308,   309,   310,
       0,   311,   312,   313,    86,   314,   315,   316,   317,   318,
     849,   319,   320,   321,   322,   382,   323,   324,   325,   306,
       0,   326,   327,   328,     0,   357,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   307,     0,   308,   309,   310,
       0,   311,   312,   313,     0,   314,   315,   316,   317,   318,
       0,   319,   320,   321,   322,     0,   323,   324,   325,   301,
       0,   326,   327,   328,     0,   357,   302,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   303,   220,     0,
       0,     0,     0,    16,     0,     0,     0,    20,     0,     0,
     301,     0,     0,     0,     0,     0,    26,   302,    27,   846,
       0,     0,    31,     0,     0,     0,     0,    35,   303,   670,
    -208,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,     0,     0,    46,   305,     0,     0,    49,
       0,   847,  -208,     0,   848,     0,    54,     0,     0,     0,
     301,     0,     0,     0,     0,     0,     0,   302,     0,     0,
       0,     0,     0,   304,     0,   306,    69,   305,   303,    71,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   307,     0,   308,   309,   310,     0,   311,   312,   313,
       0,   314,   315,   316,   317,   318,   306,   319,   320,   321,
     322,     0,   323,   324,   325,     0,     0,   326,   327,   328,
       0,   357,   307,   304,   308,   309,   310,   305,   311,   312,
     313,    86,   314,   315,   316,   317,   318,   925,   319,   320,
     321,   322,     0,   323,   324,   325,   301,     0,   326,   327,
     328,     0,   357,   302,     0,     0,   306,     0,     0,     0,
       0,     0,     0,     0,   303,     0,     0,     0,     0,     0,
       0,     0,   307,     0,   308,   309,   310,     0,   311,   312,
     313,     0,   314,   315,   316,   317,   318,     0,   319,   320,
     321,   322,     0,   323,   324,   325,   301,     0,   326,   327,
     328,     0,   415,   302,     0,     0,     0,     0,     0,   304,
       0,     0,     0,   305,   303,     0,     0,     0,     0,   385,
       0,     0,   688,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   301,     0,     0,     0,
       0,     0,   306,   302,   386,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   303,     0,     0,     0,   307,   304,
     308,   309,   310,   305,   311,   312,   313,   359,   314,   315,
     316,   317,   318,     0,   319,   320,   321,   322,     0,   323,
     324,   325,     0,     0,   326,   327,   328,     0,   357,     0,
       0,     0,   306,     0,     0,     0,     0,     0,     0,   304,
       0,     0,     0,   305,     0,     0,     0,     0,   307,     0,
     308,   309,   310,     0,   311,   312,   313,     0,   314,   315,
     316,   317,   318,     0,   319,   320,   321,   322,   147,   323,
     324,   325,   306,   387,   326,   327,   328,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   307,     0,
     308,   309,   310,     0,   311,   312,   313,     0,   314,   315,
     316,   317,   318,     0,   319,   320,   321,   322,     0,   323,
     324,   325,   301,     0,   326,   327,   328,     0,     0,   302,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     303,     0,     0,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,   301,     0,     0,     0,     0,     0,     0,
     302,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,   303,   516,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   517,   304,     0,     0,     0,   305,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   301,     0,     0,     0,     0,     0,     0,
     302,     0,     0,     0,     0,     0,   304,     0,   306,     0,
     305,   303,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   307,     0,   308,   309,   310,     0,
     311,   312,   313,     0,   314,   315,   316,   317,   318,   306,
     319,   320,   321,   322,     0,   323,   324,   325,     0,     0,
     326,   327,   328,     0,     0,   307,   304,   308,   309,   310,
     305,   311,   312,   313,     0,   314,   315,   316,   317,   318,
       0,   319,   320,   321,   322,     0,   323,   324,   325,   301,
       0,   326,   327,   328,     0,     0,   302,     0,     0,   306,
       0,     0,     0,     0,     0,     0,     0,   303,   518,     0,
       0,     0,     0,     0,     0,   307,     0,   308,   309,   310,
     519,   311,   312,   313,     0,   314,   315,   316,   317,   318,
       0,   319,   320,   321,   322,     0,   323,   324,   325,   301,
     419,   326,   327,   328,     0,     0,   302,     0,     0,     0,
       0,     0,   304,     0,     0,     0,   305,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     301,     0,     0,     0,     0,     0,     0,   302,     0,     0,
       0,     0,     0,     0,     0,   306,     0,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   307,   304,   308,   309,   310,   305,   311,   312,   313,
       0,   314,   315,   316,   317,   318,     0,   319,   320,   321,
     322,     0,   323,   324,   325,     0,     0,   326,   327,   328,
       0,     0,     0,   304,     0,   306,     0,   305,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   307,     0,   308,   309,   310,     0,   311,   312,   313,
       0,   314,   315,   316,   317,   318,   306,   319,   320,   321,
     322,     0,   323,   324,   325,     0,   421,   326,   327,   328,
       0,     0,   307,     0,   308,   309,   310,     0,   311,   312,
     313,     0,   314,   315,   316,   317,   318,     0,   319,   320,
     321,   322,     0,   323,   324,   325,   301,   423,   326,   327,
     328,     0,     0,   302,     0,     0,     0,   621,     0,   195,
       0,     0,     0,     0,   303,     0,     0,     0,     0,     0,
       0,   196,   197,     0,     0,     0,     0,     0,   198,     0,
       0,     0,     0,     0,     0,     0,   199,     0,     0,     0,
       0,     0,   301,     0,     0,     0,   200,     0,   201,   302,
       0,   202,     0,     0,     0,     0,     0,   203,     0,   304,
     303,     0,     0,   305,     0,     0,     0,     0,   204,     0,
       0,     0,     0,     0,     0,     0,     0,   205,     0,     0,
       0,     0,     0,   301,     0,     0,   206,   207,     0,   208,
     302,   209,   306,   210,     0,     0,     0,     0,     0,   211,
       0,   303,   212,     0,     0,   304,     0,     0,   307,   305,
     308,   309,   310,     0,   311,   312,   313,     0,   314,   315,
     316,   317,   318,     0,   319,   320,   321,   322,     0,   323,
     324,   325,     0,   431,   326,   327,   328,   446,   306,     0,
       0,     0,     0,     0,     0,     0,   304,     0,     0,     0,
     305,     0,     0,     0,   307,     0,   308,   309,   310,   520,
     311,   312,   313,     0,   314,   315,   316,   317,   318,     0,
     319,   320,   321,   322,     0,   323,   324,   325,   301,   306,
     326,   327,   328,     0,     0,   302,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   307,   303,   308,   309,   310,
       0,   311,   312,   313,     0,   314,   315,   316,   317,   318,
       0,   319,   320,   321,   322,     0,   323,   324,   325,     0,
       0,   326,   327,   328,     0,     0,     0,     0,   301,     0,
       0,     0,     0,     0,     0,   302,     0,     0,     0,     0,
       0,   304,     0,     0,     0,   305,   303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   301,
       0,     0,     0,     0,     0,     0,   302,     0,     0,     0,
       0,     0,     0,     0,   306,     0,     0,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     307,   304,   308,   309,   310,   305,   311,   312,   313,     0,
     314,   315,   316,   317,   318,  -456,   319,   320,   321,   322,
       0,   323,   324,   325,     0,   303,   326,   327,   328,     0,
     147,     0,   304,     0,   306,     0,   305,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     307,     0,   308,   309,   310,     0,   311,   312,   313,     0,
     314,   315,   316,   317,   318,   306,   319,   320,   321,   322,
     304,   323,   324,   325,   305,     0,   326,   327,   328,     0,
       0,   307,     0,   308,   309,   310,     0,   311,   312,   313,
       0,   314,   315,   316,   317,   318,     0,   319,   320,   321,
     322,     0,   323,   324,   325,     0,   660,   326,   327,   328,
     301,   683,     0,     0,     0,     0,     0,   302,     0,   307,
       0,   308,   309,   310,     0,   311,   312,   313,   303,   314,
     315,   316,   317,     0,     0,   319,   320,   321,   322,     0,
     323,   301,   325,     0,     0,   326,   327,   328,   302,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   303,
       0,     0,   686,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   304,     0,     0,     0,   305,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   301,     0,     0,     0,     0,     0,     0,   302,     0,
       0,     0,     0,     0,   304,     0,   306,     0,   305,   303,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   307,     0,   308,   309,   310,     0,   311,   312,
     313,     0,   314,   315,   316,   317,   318,   306,   319,   320,
     321,   322,     0,   323,   324,   325,     0,     0,   326,   327,
     328,     0,     0,   307,   304,   308,   309,   310,   305,   311,
     312,   313,     0,   314,   315,   316,   317,   318,     0,   319,
     320,   321,   322,     0,   323,   324,   325,   301,     0,   326,
     327,   328,     0,     0,   302,     0,   727,   306,     0,     0,
       0,     0,     0,     0,     0,   303,   786,     0,     0,     0,
       0,     0,     0,   307,     0,   308,   309,   310,     0,   311,
     312,   313,     0,   314,   315,   316,   317,   318,     0,   319,
     320,   321,   322,     0,   323,   324,   325,   301,     0,   326,
     327,   328,     0,     0,   302,     0,     0,     0,     0,     0,
     304,     0,     0,     0,   305,   303,   789,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   301,     0,
       0,     0,     0,     0,     0,   302,     0,     0,     0,     0,
       0,     0,     0,   306,     0,     0,   303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   307,
     304,   308,   309,   310,   305,   311,   312,   313,     0,   314,
     315,   316,   317,   318,     0,   319,   320,   321,   322,     0,
     323,   324,   325,     0,     0,   326,   327,   328,     0,     0,
       0,   304,     0,   306,     0,   305,     0,     0,     0,     0,
       0,     0,     0,     0,   863,     0,     0,     0,     0,   307,
       0,   308,   309,   310,     0,   311,   312,   313,     0,   314,
     315,   316,   317,   318,   306,   319,   320,   321,   322,     0,
     323,   324,   325,     0,     0,   326,   327,   328,     0,     0,
     307,     0,   308,   309,   310,     0,   311,   312,   313,     0,
     314,   315,   316,   317,   318,     0,   319,   320,   321,   322,
       0,   323,   324,   325,   301,     0,   326,   327,   328,     0,
       0,   302,     0,   195,     0,     0,     0,     0,     0,     0,
       0,     0,   303,     0,     0,   196,   197,     0,     0,     0,
       0,     0,   198,     0,     0,   301,   880,     0,     0,     0,
     199,     0,   302,     0,     0,     0,     0,     0,     0,     0,
     200,     0,   201,   303,     0,   202,     0,     0,     0,     0,
       0,   203,     0,     0,     0,     0,     0,   304,     0,     0,
       0,   305,   204,     0,     0,     0,     0,     0,     0,     0,
     866,   205,     0,     0,     0,     0,     0,     0,     0,     0,
     206,   207,     0,   208,     0,   209,     0,   210,   304,     0,
     306,     0,   305,   211,     0,     0,   212,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   307,     0,   308,   309,
     310,     0,   311,   312,   313,     0,   314,   315,   316,   317,
     318,   306,   319,   320,   321,   322,     0,   323,   324,   325,
       0,     0,   326,   327,   328,     0,     0,   307,     0,   308,
     309,   310,     0,   311,   312,   313,     0,   314,   315,   316,
     317,   318,     0,   319,   320,   321,   322,     0,   323,   324,
     325,   301,     0,   326,   327,   328,     0,     0,   302,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   303,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   301,     0,     0,     0,     0,     0,     0,   302,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,     0,     0,     0,   305,     0,
       0,     0,     0,     0,     0,     0,     0,   956,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   304,     0,   306,     0,   305,
       0,     0,     0,     0,     0,     0,     0,     0,   957,     0,
       0,     0,     0,   307,     0,   308,   309,   310,     0,   311,
     312,   313,     0,   314,   315,   316,   317,   318,   306,   319,
     320,   321,   322,     0,   323,   324,   325,     0,     0,   326,
     327,   328,     0,     0,   307,     0,   308,   309,   310,     0,
     311,   312,   313,     0,   314,   315,   316,   317,   318,     0,
     319,   320,   321,   322,     0,   323,   324,   325,   301,     0,
     326,   327,   328,     0,     0,   302,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   301,
       0,     0,     0,     0,     0,     0,   302,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,     0,     0,     0,   305,     0,     0,     0,     0,
       0,     0,     0,     0,   958,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,     0,   306,     0,   305,     0,     0,     0,
       0,     0,     0,     0,     0,   959,     0,     0,     0,     0,
     307,     0,   308,   309,   310,     0,   311,   312,   313,     0,
     314,   315,   316,   317,   318,   306,   319,   320,   321,   322,
       0,   323,   324,   325,     0,     0,   326,   327,   328,     0,
       0,   307,     0,   308,   309,   310,     0,   311,   312,   313,
       0,   314,   315,   316,   317,   318,     0,   319,   320,   321,
     322,     0,   323,   324,   325,   301,     0,   326,   327,   328,
       0,     0,   302,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   303,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   301,     0,     0,     0,
       0,     0,     0,   302,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   303,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,   305,     0,     0,     0,     0,     0,     0,     0,
       0,   960,     0,     0,     0,     0,   301,     0,     0,     0,
       0,     0,     0,   302,     0,     0,     0,     0,     0,   304,
       0,   306,     0,   305,   303,     0,     0,     0,     0,     0,
       0,     0,   961,     0,     0,     0,     0,   307,     0,   308,
     309,   310,     0,   311,   312,   313,     0,   314,   315,   316,
     317,   318,   306,   319,   320,   321,   322,     0,   323,   324,
     325,     0,     0,   326,   327,   328,     0,     0,   307,   304,
     308,   309,   310,   305,   311,   312,   313,     0,   314,   315,
     316,   317,   318,     0,   319,   320,   321,   322,     0,   323,
     324,   325,   301,     0,   326,   327,   328,     0,     0,   302,
       0,     0,   306,     0,     0,     0,     0,     0,     0,     0,
     303,     0,     0,     0,     0,     0,     0,     0,   307,     0,
     308,   309,   310,     0,   311,   312,   313,     0,   314,   315,
     316,   317,   318,     0,   319,   320,   321,   322,     0,   323,
     324,   325,     0,     0,   326,   327,   328,     0,     0,     0,
     303,     0,     0,     0,     0,   648,     0,     0,     0,   305,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   306,     0,
     303,     0,     0,     0,     0,   304,     0,     0,     0,   305,
       0,     0,     0,     0,   307,     0,   308,   309,   310,     0,
     311,   312,   313,     0,   314,   315,   316,   317,   318,     0,
     319,   320,   321,   322,     0,   323,   324,   325,   306,     0,
     326,   327,   328,     0,     0,   304,     0,     0,     0,   305,
       0,     0,     0,     0,   307,     0,   308,   309,   310,     0,
     311,   312,   313,     0,   314,   315,   316,   317,     0,     0,
     319,   320,   321,   322,     0,   323,   324,   325,   306,     0,
     326,   327,   328,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   307,     0,   308,   309,   310,     0,
     311,   312,   313,     0,   314,   315,   316,   317,     0,     0,
     319,   320,   321,   322,     0,   323,     0,   325,     0,     0,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,     0,     0,
    -484,     0,  -484
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-855)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,     2,    51,    52,    15,   133,   350,    18,    55,    17,
     424,   544,    23,   170,   704,   630,    27,    28,    29,    30,
      31,    32,   393,     1,   181,     1,    73,   396,     1,    40,
       1,    42,    43,   194,    45,   572,    47,    48,     1,    41,
     894,   176,   730,    90,    55,    56,    57,    58,    59,    60,
     401,   776,   777,    64,    41,    66,    67,    68,   819,   194,
      76,    72,    73,    74,    75,    29,    41,    78,    41,   230,
      81,    82,    83,   424,    85,    29,    29,    88,    89,    90,
      29,    59,   283,    41,   107,    41,   147,   127,   249,   816,
     386,   781,   153,   102,    41,   230,   934,   295,   296,   297,
     298,    41,    46,   107,   127,    76,    56,   438,   131,   440,
     647,   158,   645,    76,   249,   102,    74,   157,    89,    19,
      59,    99,    29,   147,    68,   127,   980,   131,   256,   100,
     820,   107,   155,   429,   107,   973,   152,   100,   102,    89,
     127,   152,   153,   154,   155,   156,   157,   158,   157,   910,
     126,   127,   127,   126,   127,   126,     0,   884,    76,    41,
      99,   169,   850,   126,   701,   176,    24,   102,    67,    68,
     157,   127,    72,   127,   147,   153,   147,   312,    19,   155,
     127,   152,   157,   194,   872,   346,   347,   127,   152,   152,
      41,   916,   917,   883,    76,    95,    24,   973,   152,   152,
     544,   157,   571,   152,   127,    63,   127,     7,    76,    76,
     157,   346,   347,    24,   153,   991,    74,   157,   100,   230,
     231,    89,    89,    74,   147,   107,   147,   238,   239,   147,
     127,    72,   100,   100,   152,    63,   127,    92,   249,   127,
      98,   127,   127,   933,   859,   127,    74,    76,   127,   127,
     127,   154,    63,   156,    95,   127,   417,   455,   155,   147,
      89,     3,   153,    74,    92,   147,   127,   278,   153,   155,
     152,   100,    84,   155,   275,   808,   155,   155,   155,   147,
     147,    92,   417,   155,   152,   152,   147,    69,   154,   300,
     301,   302,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   645,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   147,   127,
     154,   147,   693,   152,   335,   336,   695,   153,   375,   127,
     341,   127,   154,   344,   345,   346,   347,   348,   349,   350,
       2,   542,   543,   127,   154,   147,    73,   155,   359,    11,
      77,   147,   363,   127,   365,     3,   127,   155,   127,   730,
      22,   127,    24,   107,   375,    28,   377,   378,    73,   380,
     381,   155,    77,   147,   385,   386,   147,   388,   147,   390,
     147,   147,    30,   127,   978,   335,   336,   131,   147,    89,
     137,   341,   403,   987,   344,   345,   531,   304,   305,   410,
     154,   147,    64,    51,   451,   154,   417,   156,   609,   610,
      73,   155,   826,    41,    77,   147,   147,   428,   429,   154,
     129,   156,   147,   154,   435,   156,    88,   438,   147,   440,
      92,   442,   443,   444,   147,   446,   447,   152,   154,    87,
     451,   348,   349,   600,    92,   147,   126,   101,    76,   129,
     154,   147,   359,   133,   107,    28,   154,   147,     7,    53,
     126,   472,   473,   126,   808,   128,   129,   152,   819,   147,
     133,   378,   100,   107,   381,   826,   176,   127,   141,   107,
     155,    41,   147,   154,   855,    31,   148,   150,   438,   127,
     440,   155,   107,    18,   194,   155,   155,   155,   127,   127,
      73,   872,   630,   152,    77,   516,   517,   518,   519,   520,
     521,   522,   157,   155,   147,   523,   524,   147,   529,   147,
     531,   147,   147,   534,   152,   536,   147,   155,     2,   540,
     230,   147,   127,   544,   581,   442,   147,    11,    41,   157,
     147,    31,   155,   153,   147,   147,   155,   152,    22,   249,
      24,   213,   127,   126,   153,     2,   129,   107,   220,   910,
     133,    74,   573,    29,    11,   576,   577,   578,   579,   580,
     581,    29,   157,    76,   585,    22,   152,    24,    31,   529,
      74,   126,   126,   934,   153,   157,   157,   157,    23,   154,
      64,   129,   943,    67,    68,   129,   129,   100,   129,    34,
      35,   129,   129,   129,   107,   129,   129,   129,    43,   129,
     517,    46,   519,   275,    88,    31,   155,    64,    92,   100,
     155,   153,   973,   728,   127,    89,    61,   978,   157,    21,
     429,   428,   715,    68,   645,   585,   987,   648,   649,   650,
     991,    88,   653,    23,   147,    92,   346,   347,   794,   152,
     533,   257,   155,   733,    34,    35,   859,   668,   855,   670,
     693,   182,   673,    43,   711,   676,    46,   736,   564,   980,
     681,   933,   683,    53,   148,   686,   952,    31,   404,   531,
      27,    61,    -1,    -1,    -1,    -1,    24,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,   357,    -1,    -1,    -1,    -1,
     711,   148,    -1,    41,    -1,    -1,    44,    -1,    -1,    -1,
     410,   722,    -1,   724,    -1,   377,   727,   417,   380,    -1,
      -1,   732,    60,    -1,    -1,    63,   388,    65,    -1,    -1,
      -1,   859,    -1,    -1,    -1,   435,    74,    -1,   438,   213,
     440,   648,   649,   443,    -1,    -1,   220,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,   772,    -1,    -1,    -1,    -1,   213,    28,    -1,    -1,
      -1,    -1,    -1,   220,   785,   786,    -1,   788,   789,   790,
     791,    -1,   444,    -1,    -1,   796,   797,    -1,   799,   127,
     801,   802,    -1,    -1,   805,    -1,    -1,   808,   847,   848,
      -1,   275,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    -1,   825,   826,    77,   155,    -1,    -1,
     831,    -1,   833,    -1,    -1,    -1,   837,    -1,   275,   529,
      -1,   531,    -1,    -1,    -1,   846,    -1,    -1,   885,    -1,
      -1,    -1,    -1,   844,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   513,   863,   864,   865,   866,   867,   868,   869,   870,
      -1,   122,    -1,   124,   125,   126,    -1,   128,   129,   880,
      -1,    -1,   133,    -1,   885,    -1,    -1,    -1,    -1,   890,
     141,    -1,   893,   357,    -1,    -1,    -1,   148,   149,   150,
      -1,    -1,   554,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     947,    -1,    -1,   377,     2,    -1,   380,    -1,    -1,    -1,
     357,    -1,    -1,    11,   388,   926,   927,   928,   929,    -1,
      -1,    -1,    -1,    -1,    22,    58,    24,    -1,    -1,    -1,
     377,     2,   943,   380,   945,   946,   947,    -1,    -1,    -1,
      11,   388,    -1,    -1,    -1,   956,   957,   958,   959,   960,
     961,    22,   614,    24,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   972,   973,    -1,    -1,    -1,    64,    -1,   630,    -1,
     444,    -1,    -1,    -1,    -1,    -1,   987,    -1,    -1,    -1,
     991,    -1,    -1,    28,    -1,    19,   648,   649,    -1,    23,
      88,    -1,    -1,    64,    92,    -1,    -1,   444,    32,    -1,
      34,    35,    -1,    -1,    38,    -1,   668,    -1,   670,    43,
      -1,   673,    46,    -1,   676,    -1,    -1,    88,    -1,    -1,
      -1,    92,    -1,   685,    -1,    -1,   688,    61,    73,    23,
      -1,    65,    77,    67,    68,    -1,    70,    -1,    72,    -1,
      34,    35,    -1,    -1,    -1,    -1,    -1,    -1,   710,    43,
     148,    -1,    46,   186,   187,   188,    -1,    -1,    92,    53,
     722,    95,   724,    -1,     1,    -1,   728,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    68,    69,    -1,   148,    -1,    -1,
     554,   126,    19,   128,   129,    -1,    23,    24,   133,    -1,
     223,    -1,    -1,   226,    -1,    32,   141,    34,    35,    -1,
      -1,    38,    -1,   148,   149,   150,    43,   554,     3,    46,
      -1,    -1,   245,   147,    -1,   213,    -1,    -1,    -1,   819,
      15,    24,   220,    -1,    61,    -1,    63,    22,    65,    -1,
      67,    68,   265,    70,   796,    72,    -1,    74,    41,   801,
     614,    44,   213,    -1,    -1,    40,    -1,    -1,    -1,   220,
      45,    -1,    -1,   815,    -1,    92,    51,    60,    95,    -1,
      63,    98,    65,    -1,    -1,   827,    -1,   614,    -1,    -1,
      -1,    74,    -1,    -1,   836,    -1,    71,   275,   840,    -1,
      -1,    -1,   844,    -1,    -1,    -1,    -1,    -1,    83,    92,
     890,    -1,    87,    -1,   668,    -1,   670,    -1,    93,   673,
      -1,    -1,   676,    -1,   275,    -1,    -1,    -1,    -1,    -1,
     147,   685,    -1,    28,   688,    -1,   153,    -1,    -1,   881,
      -1,   668,    -1,   670,    -1,    -1,   673,   360,    -1,   676,
      -1,    -1,    -1,     2,   934,    -1,   710,    -1,   685,    -1,
      -1,   688,    11,    -1,    -1,    -1,    -1,    -1,   722,    -1,
     724,   384,    -1,    22,   728,    24,    -1,    -1,    73,   357,
      -1,    -1,    77,   710,   926,   927,   928,   929,    -1,    -1,
      -1,    -1,    -1,   973,    -1,   722,    -1,   724,    -1,   377,
      -1,   728,   380,   945,   946,    -1,   357,    -1,    -1,    -1,
     388,   991,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   377,   122,    -1,   380,
     125,   126,    -1,   128,   129,    -1,    -1,   388,   133,    88,
      -1,    -1,   796,    92,    -1,    -1,   141,   801,    24,    -1,
       2,    -1,    -1,   148,   149,   150,    -1,    28,    -1,    11,
      -1,   815,    -1,    -1,    -1,    41,   444,    -1,    44,   796,
      22,    -1,    24,   827,   801,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   836,    -1,    60,    -1,   840,    63,   815,    65,
     844,    -1,    -1,   444,    -1,    -1,    -1,    -1,    74,   148,
     827,    -1,    73,    -1,    -1,    -1,    77,    -1,    -1,   836,
      -1,    -1,    64,   840,    -1,    -1,    92,   844,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   538,   539,   881,    -1,   542,
     543,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,
      37,   122,    -1,    -1,   881,   126,    11,   128,   129,    28,
      -1,    -1,   133,    -1,   213,    -1,    -1,    22,    -1,    24,
     141,   220,   926,   927,   928,   929,   554,   148,   149,   150,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   945,   946,    -1,    -1,    -1,   148,    -1,    -1,   926,
     927,   928,   929,   554,    73,    -1,    -1,    -1,    77,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   945,   946,
      -1,    -1,    -1,    -1,    -1,    -1,   275,   114,    -1,   116,
      -1,    -1,    -1,    88,    -1,    -1,   614,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   658,   659,    -1,    -1,    -1,
      -1,    -1,   665,   122,    -1,   124,   125,   126,    -1,   128,
     129,   213,   675,   614,   133,   678,    -1,    -1,   220,    -1,
      -1,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,   148,
     149,   150,    -1,   170,    -1,    -1,    40,    -1,    -1,    -1,
     668,    -1,   670,   148,   181,   673,    -1,   184,   676,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,   685,   357,    -1,
     688,    -1,    -1,    67,    68,    -1,    -1,   668,    -1,   670,
      -1,    -1,   673,   275,    -1,   676,    -1,    -1,   377,   216,
      -1,   380,   710,    -1,   685,    -1,    -1,   688,    -1,   388,
      -1,    -1,    -1,    -1,   722,    -1,   724,   234,    -1,    -1,
     728,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,   710,
      -1,    -1,    -1,    -1,    -1,   220,    -1,    -1,    -1,    -1,
      -1,   722,    -1,   724,    -1,    -1,    -1,   728,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     2,    -1,   444,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    -1,    -1,   357,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,   168,    24,   829,    -1,   796,    -1,
     275,    -1,    -1,   801,    -1,   377,    -1,    -1,   380,    -1,
      -1,    -1,   186,   187,   188,    -1,   388,   815,    -1,   193,
      -1,    -1,    -1,    -1,    -1,   796,    -1,    -1,    -1,   827,
     801,    -1,    -1,    -1,    -1,    64,    -1,    -1,   836,    -1,
      -1,    -1,   840,    -1,   815,    -1,   844,    -1,    -1,   223,
      -1,    -1,   226,    -1,    -1,    -1,   827,    -1,    -1,    88,
      -1,    -1,    -1,    92,    -1,   836,   373,    -1,    -1,   840,
      -1,   245,   444,   844,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   357,   881,    -1,   554,    -1,    -1,    -1,   396,
      -1,   265,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   377,    -1,    -1,   380,    -1,    -1,    -1,    -1,
     881,    -1,    -1,   388,    -1,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   926,   927,
     928,   929,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   450,    -1,   614,   453,   945,   946,    -1,
      -1,    -1,    -1,    -1,    -1,   926,   927,   928,   929,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   444,
      -1,    -1,    -1,    -1,   945,   946,    -1,    -1,    -1,    -1,
      -1,    -1,   554,    -1,   213,    -1,   360,    -1,    -1,    -1,
      28,   220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   668,
      -1,   670,    -1,     1,   673,    -1,    -1,   676,    -1,    -1,
     384,    -1,    -1,    -1,    -1,    -1,   685,    -1,    -1,   688,
      -1,    19,    -1,    -1,    -1,    23,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    73,    34,    35,    -1,    77,
      38,   710,   614,    -1,    -1,    43,   275,    -1,    46,    -1,
      -1,    -1,    -1,   722,    -1,   724,    -1,    -1,    -1,   728,
     567,    -1,    -1,    61,   571,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    -1,    72,    -1,    74,    -1,    -1,   554,
      -1,    -1,    -1,    -1,   122,    -1,   124,   125,   126,   596,
     128,   129,   130,   600,    92,   133,   668,    95,   670,    -1,
      98,   673,   140,   141,   676,    -1,    -1,   145,    -1,    -1,
     148,   149,   150,   685,    -1,    -1,   688,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   796,   357,    -1,
      -1,    -1,   801,    -1,    -1,    -1,    -1,    -1,   710,   614,
      -1,    -1,    -1,    -1,    -1,    -1,   815,    -1,   377,   147,
     722,   380,   724,    -1,    -1,   153,   728,    -1,   827,    -1,
      -1,    -1,    -1,    -1,   538,   539,    -1,   836,   542,   543,
      -1,   840,    -1,    -1,    -1,   844,    -1,    -1,    -1,    -1,
      -1,   555,    -1,    -1,    -1,   692,    -1,    -1,   695,    -1,
      -1,    -1,    -1,   668,    -1,   670,    -1,    -1,   673,    -1,
      -1,   676,    -1,    -1,    -1,    -1,    -1,    -1,   715,    -1,
     685,    -1,   881,   688,    -1,   444,    -1,    -1,    -1,    -1,
      -1,    -1,   596,    -1,   796,    -1,   733,    -1,    -1,   801,
      -1,    -1,    -1,    -1,    -1,   710,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   815,   751,    -1,    -1,   722,    -1,   724,
      -1,    -1,    -1,   728,    -1,   827,    -1,   926,   927,   928,
     929,    -1,    -1,    -1,   836,    -1,    -1,    -1,   840,     3,
      28,    -1,   844,    -1,    -1,    -1,   945,   946,    -1,    -1,
      -1,    15,    16,    -1,   658,   659,    -1,    -1,    22,    -1,
      -1,   665,    -1,   667,    -1,    -1,    30,    -1,   672,    -1,
      -1,   675,    -1,    -1,   678,    -1,    40,    -1,    42,   881,
      -1,    45,    -1,    -1,    -1,    73,    -1,    51,    -1,    77,
      -1,   796,    -1,    -1,    -1,   554,   801,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,
     815,    -1,    -1,    -1,   718,    -1,    80,    81,    -1,    83,
      -1,    85,   827,    87,   926,   927,   928,   929,    -1,    93,
      94,   836,    96,    -1,   122,   840,   124,   125,   126,   844,
     128,   129,   130,   945,   946,   133,   134,   135,    -1,    -1,
     138,   139,   140,   141,    -1,   614,    -1,   145,    -1,    -1,
     148,   149,   150,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   881,    -1,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
     154,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    42,    -1,   668,
      45,   670,    10,    -1,   673,    -1,    51,   676,    -1,    17,
      -1,   926,   927,   928,   929,   829,   685,    62,    -1,   688,
      28,    -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,    74,
     945,   946,    -1,    -1,    -1,    80,    81,    -1,    83,    -1,
      85,   710,    87,    -1,    -1,    -1,    -1,    -1,    93,    -1,
      -1,    96,    -1,   722,    -1,   724,    -1,    -1,    -1,   728,
     874,   875,    -1,   877,   878,    73,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     894,    -1,    -1,    -1,    -1,    -1,   900,    -1,   902,    -1,
     904,    -1,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      -1,    -1,    -1,    -1,   122,    -1,   124,   125,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,   135,   136,    -1,
     138,   139,   140,   141,    -1,   143,   144,   145,    -1,    -1,
     148,   149,   150,    -1,    -1,    -1,   815,    -1,    -1,   157,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   827,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   980,   836,    -1,    -1,
      -1,   840,    -1,    -1,    -1,   844,    -1,    -1,    -1,     0,
       1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,   881,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    43,    -1,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    57,    -1,    -1,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    -1,    74,    75,    76,    -1,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    -1,    90,
      91,    92,    93,    -1,    95,    96,    97,    98,    -1,    -1,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,   142,    -1,   144,   145,    -1,   147,    -1,    -1,   150,
      -1,   152,    -1,   154,     1,   156,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    52,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    64,    -1,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    81,    82,    83,    84,    85,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,    -1,
      -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,   142,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,   152,    -1,   154,    -1,   156,
     157,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      10,    -1,    12,    13,    14,    15,    16,    17,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    -1,    42,    43,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    -1,    57,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    -1,
      90,    91,    92,    93,    -1,    95,    96,    97,    98,    -1,
      -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,    -1,   132,   133,   134,   135,   136,    -1,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,   147,   148,   149,
     150,    -1,   152,   153,   154,     1,   156,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      -1,    57,    -1,    -1,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    -1,    74,    75,
      76,    -1,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    -1,    90,    91,    92,    93,    -1,    95,
      96,    97,    98,    -1,    -1,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,   142,    -1,   144,   145,
     146,   147,    -1,    -1,   150,    -1,   152,   153,   154,     1,
     156,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    57,    -1,    -1,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    -1,    74,    75,    76,    -1,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    -1,    90,    91,
      92,    93,    -1,    95,    96,    97,    98,    -1,    -1,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
     142,    -1,   144,   145,    -1,   147,    -1,    -1,   150,    -1,
     152,   153,   154,     1,   156,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    -1,    42,    43,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    57,
      -1,    -1,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    -1,    74,    75,    76,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    -1,    90,    91,    92,    93,    -1,    95,    96,    97,
      98,    -1,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,   142,    -1,   144,   145,    -1,   147,
      -1,    -1,   150,    -1,   152,   153,   154,     1,   156,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    43,
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    -1,    57,    -1,    -1,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    -1,
      74,    75,    76,    -1,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    -1,    90,    91,    92,    93,
      -1,    95,    96,    97,    98,    -1,    -1,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,    -1,
     144,   145,    -1,   147,    -1,    -1,   150,    -1,   152,   153,
     154,     1,   156,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    -1,    42,    43,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    -1,    57,    -1,    -1,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    -1,    74,    75,    76,    -1,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    -1,
      90,    91,    92,    93,    -1,    95,    96,    97,    98,    -1,
      -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,   142,    -1,   144,   145,    -1,   147,    -1,    -1,
     150,    -1,   152,   153,   154,     1,   156,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      -1,    57,    -1,    -1,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    -1,    74,    75,
      76,    -1,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    -1,    90,    91,    92,    93,    -1,    95,
      96,    97,    98,    -1,    -1,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,   142,    -1,   144,   145,
      -1,   147,    -1,    -1,   150,    -1,   152,    -1,   154,     1,
     156,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    57,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    -1,    74,    75,    76,    -1,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    -1,    90,    91,
      92,    93,    -1,    95,    96,    97,    98,    -1,    -1,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
     142,    -1,   144,   145,    -1,   147,    -1,    -1,   150,    -1,
     152,    -1,   154,     1,   156,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    -1,    42,    43,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    57,
      -1,    -1,    -1,    61,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    -1,    74,    75,    76,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    -1,    90,    91,    92,    93,    -1,    95,    96,    97,
      98,    -1,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,   142,    -1,   144,   145,    -1,   147,
      -1,    -1,   150,    -1,   152,    -1,   154,     1,   156,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    43,
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    -1,    57,    -1,    -1,    -1,    61,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    -1,
      74,    75,    76,    -1,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    -1,    90,    91,    92,    93,
      -1,    95,    96,    97,    98,    -1,    -1,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,    -1,
     144,   145,    -1,   147,    -1,    -1,   150,    -1,   152,    -1,
     154,     1,   156,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    -1,    42,    43,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    -1,    57,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    -1,    74,    75,    76,    -1,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    -1,
      90,    91,    92,    93,    -1,    95,    96,    97,    98,    -1,
      -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,   142,    -1,   144,   145,    -1,   147,    -1,    -1,
     150,    -1,   152,    -1,   154,     1,   156,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      -1,    57,    -1,    -1,    -1,    61,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    -1,    74,    75,
      76,    -1,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    -1,    90,    91,    92,    93,    -1,    95,
      96,    97,    98,    -1,    -1,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,   142,    -1,   144,   145,
      -1,   147,    -1,    -1,   150,    -1,   152,    -1,   154,     1,
     156,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    57,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    -1,    74,    75,    76,    -1,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    -1,    90,    91,
      92,    93,    -1,    95,    96,    97,    98,    -1,    -1,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
     142,    -1,   144,   145,    -1,   147,    -1,    -1,   150,    -1,
     152,    -1,   154,     1,   156,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    -1,    42,    43,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    57,
      -1,    -1,    -1,    61,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    -1,    74,    75,    76,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    -1,    90,    91,    92,    93,    -1,    95,    96,    97,
      98,    -1,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,   142,    -1,   144,   145,    -1,   147,
      -1,    -1,   150,    -1,   152,    -1,   154,     1,   156,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    43,
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    -1,    57,    -1,    -1,    -1,    61,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    -1,
      74,    75,    76,    -1,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    -1,    90,    91,    92,    93,
      -1,    95,    96,    97,    98,    -1,    -1,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,    -1,
     144,   145,    -1,   147,    -1,    -1,   150,    -1,   152,    -1,
     154,     1,   156,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    -1,    42,    43,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    -1,    57,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    -1,    74,    75,    76,    -1,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    -1,
      90,    91,    92,    93,    -1,    95,    96,    97,    98,    -1,
      -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,   142,    -1,   144,   145,    -1,   147,    -1,    -1,
     150,    -1,   152,    -1,   154,     1,   156,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      -1,    57,    -1,    -1,    -1,    61,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    -1,    74,    75,
      76,    -1,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    -1,    90,    91,    92,    93,    -1,    95,
      96,    97,    98,    -1,    -1,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,   142,    -1,   144,   145,
      -1,   147,    -1,    -1,   150,    -1,   152,    -1,   154,     1,
     156,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    64,    -1,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,
      82,    83,    84,    85,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
     142,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
     152,    -1,   154,    -1,   156,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    52,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    64,    -1,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    81,    82,    83,    84,    85,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,   142,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,   152,    -1,   154,    -1,   156,   157,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    64,    -1,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,
      83,    84,    85,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
     123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,   152,
      -1,   154,    -1,   156,   157,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    52,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    64,    -1,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    81,    82,    83,    84,    85,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,   142,    -1,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,   152,    -1,   154,    -1,   156,   157,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    19,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    46,    -1,    48,    49,    -1,    51,    52,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    64,    -1,    66,    -1,    68,    -1,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,
      83,    84,    85,    -1,    87,     1,    -1,     3,    -1,    92,
      93,    -1,    95,    96,    10,    -1,    -1,    -1,    -1,    15,
      16,    17,    -1,   106,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,   122,
     123,   124,   125,    -1,    40,    -1,    42,   130,    -1,    45,
      -1,    -1,    -1,    -1,    -1,    51,    -1,   140,    -1,   142,
      -1,   144,   145,    -1,    -1,    -1,    62,   150,    -1,   152,
      -1,   154,    -1,   156,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    81,    -1,    83,    -1,    85,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,   111,   112,    -1,    -1,   115,
     116,   117,   118,    -1,   120,   121,   122,   123,   124,   125,
      -1,    -1,   128,    -1,    -1,    -1,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,    -1,   145,
      -1,    -1,   148,   149,   150,   151,    -1,    -1,   154,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    19,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    46,    -1,    48,    49,    -1,    51,    52,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      64,    -1,    66,    -1,    68,    -1,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,    83,
      84,    85,    -1,    87,    -1,    -1,    -1,    -1,    92,    93,
      -1,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   140,    -1,   142,    12,
     144,   145,    15,    16,    -1,    -1,   150,    -1,   152,    22,
     154,    -1,   156,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    64,    -1,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,
      83,    84,    85,    -1,    87,    -1,    -1,    90,    91,    -1,
      93,    94,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
     123,   124,   125,    -1,    -1,    -1,    -1,   130,   131,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,   152,
      -1,   154,    -1,   156,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    64,    -1,    66,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    81,    82,    83,    84,    85,    -1,    87,    -1,
      -1,    90,    91,    -1,    93,    94,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,   142,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,   155,   156,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,
      -1,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    81,    82,    83,    84,
      85,    -1,    87,    -1,    -1,    90,    91,    -1,    93,    94,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,   142,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,
     155,   156,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      41,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    -1,    64,    -1,    66,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    80,
      81,    82,    83,    84,    85,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    93,    -1,    -1,    96,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   140,
      -1,   142,    12,   144,   145,    15,    16,    -1,    -1,   150,
      -1,   152,    22,   154,    -1,   156,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    64,    -1,    66,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    81,    82,    83,    84,    85,    -1,    87,    -1,    -1,
      90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,   142,    -1,   144,   145,    -1,   147,    -1,    -1,
     150,    -1,   152,    -1,   154,    -1,   156,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    52,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,    -1,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    81,    82,    83,    84,    85,
      -1,    87,    -1,    -1,    90,    91,    -1,    93,    -1,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,   142,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,    -1,
     156,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      52,    -1,    54,    55,    56,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    64,    -1,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,
      82,    83,    84,    85,    -1,    87,    -1,    -1,    90,    91,
      -1,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   140,    -1,
     142,    12,   144,   145,    15,    16,    -1,    -1,   150,    -1,
     152,    22,   154,    -1,   156,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      81,    82,    83,    84,    85,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,   142,    -1,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,   152,    -1,   154,    -1,   156,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    35,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    52,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    64,    -1,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    81,    82,    83,    84,    85,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,    -1,
      -1,    -1,    -1,   130,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,   140,    -1,   142,    12,   144,   145,    15,
      16,    -1,    -1,   150,    -1,   152,    22,   154,    -1,   156,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      46,    -1,    48,    49,    -1,    51,    52,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,    -1,
      66,    -1,    68,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    81,    82,    83,    84,    85,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   140,    -1,   142,    12,   144,   145,
      15,    16,    -1,    -1,   150,    -1,   152,    22,   154,    -1,
     156,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,
      -1,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    81,    82,    83,    84,
      85,    -1,    87,    -1,    -1,    90,    91,    -1,    93,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,   140,    -1,   142,    12,   144,
     145,    15,    16,    -1,    -1,   150,    -1,   152,    22,   154,
      -1,   156,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      64,    -1,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,    83,
      84,    85,    -1,    87,    -1,    -1,    -1,    -1,    -1,    93,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,   152,   153,
     154,    -1,   156,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    64,    -1,    66,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    81,    82,    83,    84,    85,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,
     130,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     140,    -1,   142,    12,   144,   145,    15,    16,    -1,    -1,
     150,    -1,   152,    22,   154,    -1,   156,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    64,    -1,    66,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    81,    82,    83,    84,    85,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,   142,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,    -1,   156,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,
      -1,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    81,    82,    83,    84,
      85,    -1,    87,    -1,    -1,    -1,    -1,    -1,    93,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,   140,    -1,   142,    12,   144,
     145,    15,    16,    -1,    -1,   150,    -1,   152,    22,   154,
      -1,   156,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      64,    -1,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,    83,
      84,    85,    -1,    87,    -1,    -1,    -1,    -1,    -1,    93,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,
     154,    -1,   156,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    64,    -1,    66,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    81,    82,    83,    84,    85,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,   142,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,   152,    -1,   154,    -1,   156,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    52,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,    -1,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    81,    82,    83,    84,    85,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,   142,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,    -1,
     156,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    64,    -1,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,
      82,    83,    84,    85,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   140,    -1,
     142,    12,   144,   145,    15,    16,    -1,    -1,   150,    -1,
     152,    22,   154,    -1,   156,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    -1,    64,    -1,    66,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      81,    82,    83,    84,    85,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   140,
      -1,   142,    12,   144,   145,    15,    16,    -1,    -1,   150,
      -1,   152,    22,   154,    -1,   156,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    64,    -1,    66,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    81,    82,    83,    84,    85,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,
     130,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     140,    -1,   142,    12,   144,   145,    15,    16,    -1,    -1,
     150,    -1,   152,    22,   154,    -1,   156,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    64,    -1,    66,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    81,    82,    83,    84,    85,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,   140,    -1,   142,    12,   144,   145,    15,    16,    -1,
      -1,   150,    -1,   152,    22,   154,    -1,   156,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    52,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    64,    -1,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    81,    82,    83,    84,    85,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   140,    -1,   142,    12,   144,   145,    15,    16,
      -1,    -1,   150,    -1,   152,    22,   154,    -1,   156,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    52,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    64,    -1,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    81,    82,    83,    84,    85,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,    -1,
      -1,    -1,    -1,   130,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,   140,    -1,   142,    12,   144,   145,    15,
      16,    -1,    -1,   150,    -1,   152,    22,   154,    -1,   156,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    52,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,    -1,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    81,    82,    83,    84,    85,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   140,    -1,   142,    12,   144,   145,
      15,    16,    -1,    -1,   150,    -1,   152,    22,   154,    -1,
     156,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,
      -1,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    81,    82,    83,    84,
      85,    -1,    87,    -1,    -1,    -1,    -1,    -1,    93,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,   140,    -1,   142,    12,   144,
     145,    15,    16,    -1,    -1,   150,    -1,   152,    22,   154,
      -1,   156,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      64,    -1,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,    83,
      84,    85,    -1,    87,    -1,    -1,    -1,    -1,    -1,    93,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   140,    -1,   142,    12,
     144,   145,    15,    16,    -1,    -1,   150,    -1,   152,    22,
     154,    -1,   156,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    64,    -1,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,
      83,    84,    85,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
     123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,   140,    -1,   142,
      12,   144,   145,    15,    16,    -1,    -1,   150,    -1,   152,
      22,   154,    -1,   156,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    64,    -1,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,
      82,    83,    84,    85,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   140,    -1,
     142,    12,   144,   145,    15,    16,    -1,    -1,   150,    -1,
     152,    22,   154,    -1,   156,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    -1,    64,    -1,    66,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      81,    82,    83,    84,    85,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   140,
      -1,   142,    12,   144,   145,    15,    16,    -1,    -1,   150,
      -1,   152,    22,   154,    -1,   156,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    64,    -1,    66,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    81,    82,    83,    84,    85,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,
     130,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     140,    -1,   142,    12,   144,   145,    15,    16,    -1,    -1,
     150,    -1,   152,    22,   154,    -1,   156,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    64,    -1,    66,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    81,    82,    83,    84,    85,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,   140,    -1,   142,    12,   144,   145,    15,    16,    -1,
      -1,   150,    -1,   152,    22,   154,    -1,   156,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    52,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    64,    -1,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    81,    82,    83,    84,    85,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   140,    -1,   142,    12,   144,   145,    15,    16,
      -1,    -1,   150,    -1,   152,    22,   154,    -1,   156,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    52,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    64,    -1,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    81,    82,    83,    84,    85,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,    -1,
      -1,    -1,    -1,   130,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,   140,    -1,   142,    12,   144,   145,    15,
      16,    -1,    -1,   150,    -1,    -1,    22,   154,    -1,   156,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    52,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,    -1,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    81,    82,    83,    84,    85,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   140,    -1,   142,    12,    -1,   145,
      15,    16,    -1,    -1,   150,    -1,   152,    22,   154,    -1,
     156,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,
      -1,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    81,    82,    83,    84,
      85,    -1,    87,    -1,    -1,    -1,    -1,    -1,    93,    -1,
      -1,    96,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,   107,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    30,   127,    -1,    -1,    -1,   131,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,   142,    -1,    48,
      49,   146,    51,    52,    -1,    54,    55,   152,    -1,   154,
     155,   156,    -1,    62,    -1,    64,    -1,    66,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    80,    81,    82,    83,    84,    85,    -1,    87,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,   100,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,     3,     4,     5,     6,     7,
       8,    -1,   137,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,   142,    22,    -1,   151,    -1,   147,   154,
      -1,   156,    30,   152,    -1,   154,    -1,   156,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    52,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    64,    -1,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    81,    82,    83,    84,    85,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,    96,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,   142,    48,    49,    -1,    51,    52,
      -1,    54,    55,    -1,   152,    -1,   154,    -1,   156,    62,
      -1,    64,    -1,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,
      83,    84,    85,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      93,    -1,    -1,    96,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    30,     3,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    15,    16,    45,    -1,   142,
      48,    49,    22,    51,    52,    -1,    54,    55,    -1,   152,
      30,   154,    -1,   156,    62,    -1,    64,    -1,    66,    -1,
      40,    -1,    42,    71,    -1,    45,    -1,    -1,    -1,    -1,
      -1,    51,    80,    81,    82,    83,    84,    85,    -1,    87,
      -1,    -1,    62,     3,    -1,    93,    -1,    -1,    96,    -1,
      -1,    71,    -1,    -1,    -1,    15,    16,    -1,    -1,    -1,
      80,    81,    22,    83,    -1,    85,    -1,    87,    -1,    -1,
      30,    -1,    -1,    93,    -1,   123,    96,    -1,    -1,    -1,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,     3,    -1,    -1,   154,    -1,   156,    -1,
      -1,    71,    -1,    -1,    -1,    15,    16,    -1,    -1,    -1,
      80,    81,    22,    83,    -1,    85,    -1,    87,    -1,    -1,
      30,    -1,   152,    93,   154,    -1,    96,    -1,    -1,    -1,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    71,    -1,    -1,    -1,    15,    16,    17,    -1,    -1,
      80,    81,    22,    83,    -1,    85,    -1,    87,    -1,    -1,
      30,    -1,    -1,    93,   154,    -1,    96,    -1,    -1,    -1,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    81,    -1,    83,    -1,    85,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    93,   154,    10,    96,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,   111,   112,    28,    -1,   115,   116,   117,   118,    -1,
     120,   121,   122,   123,   124,   125,    -1,    -1,   128,    -1,
      -1,    -1,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,    -1,   145,    -1,    -1,   148,   149,
     150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,
      -1,    -1,    77,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,    -1,   128,   129,   130,    -1,   132,   133,   134,
     135,   136,    -1,   138,   139,   140,   141,    73,   143,   144,
     145,    77,    -1,   148,   149,   150,    -1,    -1,    10,    -1,
      -1,    -1,   157,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,   125,
     126,    -1,   128,   129,   130,    -1,   132,   133,   134,   135,
     136,    -1,   138,   139,   140,   141,    -1,   143,   144,   145,
      -1,    73,   148,   149,   150,    77,    -1,    10,    -1,    -1,
      -1,   157,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,   124,   125,   126,    -1,   128,   129,   130,    -1,
     132,   133,   134,   135,   136,    -1,   138,   139,   140,   141,
      73,   143,   144,   145,    77,    -1,   148,   149,   150,    -1,
      -1,    10,    -1,    -1,    -1,   157,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,   124,   125,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,   135,   136,    -1,   138,   139,   140,   141,    -1,
     143,   144,   145,    -1,    73,   148,   149,   150,    77,    -1,
      -1,    -1,   155,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,   124,   125,   126,    -1,   128,
     129,   130,    -1,   132,   133,   134,   135,   136,    -1,   138,
     139,   140,   141,    -1,   143,   144,   145,    -1,    73,   148,
     149,   150,    77,    -1,    -1,    -1,   155,    -1,    -1,    10,
      -1,    86,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,   106,   107,   108,   109,   110,   111,   112,    -1,    -1,
     115,   116,   117,   118,    -1,   120,   121,   122,    -1,   124,
     125,   126,    -1,   128,   129,   130,    -1,   132,   133,   134,
     135,   136,    -1,   138,   139,   140,   141,    -1,   143,   144,
     145,    -1,    73,   148,   149,   150,    77,   152,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    86,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     111,   112,    -1,    -1,   115,   116,   117,   118,    -1,   120,
     121,   122,    -1,   124,   125,   126,    -1,   128,   129,   130,
      -1,   132,   133,   134,   135,   136,    -1,   138,   139,   140,
     141,    -1,   143,   144,   145,    -1,    73,   148,   149,   150,
      77,   152,    -1,    -1,    -1,    -1,    -1,    10,    -1,    86,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   111,   112,    -1,    -1,   115,   116,
     117,   118,    -1,   120,   121,   122,    -1,   124,   125,   126,
      -1,   128,   129,   130,    -1,   132,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,    -1,   143,   144,   145,    -1,
      73,   148,   149,   150,    77,   152,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    86,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   111,   112,
      -1,    -1,   115,   116,   117,   118,    -1,   120,   121,   122,
      -1,   124,   125,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,   135,   136,    -1,   138,   139,   140,   141,    -1,
     143,   144,   145,    -1,    73,   148,   149,   150,    77,   152,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    86,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   111,   112,    -1,    -1,   115,   116,   117,   118,
      -1,   120,   121,   122,    -1,   124,   125,   126,    -1,   128,
     129,   130,    -1,   132,   133,   134,   135,   136,    -1,   138,
     139,   140,   141,    -1,   143,   144,   145,    -1,    73,   148,
     149,   150,    77,   152,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    86,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,   106,   107,   108,   109,   110,   111,   112,    -1,    -1,
     115,   116,   117,   118,    -1,   120,   121,   122,    -1,   124,
     125,   126,    -1,   128,   129,   130,    -1,   132,   133,   134,
     135,   136,    -1,   138,   139,   140,   141,    -1,   143,   144,
     145,    -1,    73,   148,   149,   150,    77,   152,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    86,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     111,   112,    -1,    -1,   115,   116,   117,   118,    -1,   120,
     121,   122,    -1,   124,   125,   126,    -1,   128,   129,   130,
      -1,   132,   133,   134,   135,   136,    -1,   138,   139,   140,
     141,    -1,   143,   144,   145,    -1,    73,   148,   149,   150,
      77,   152,    -1,    -1,    -1,    -1,    -1,    10,    -1,    86,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   111,   112,    -1,    -1,   115,   116,
     117,   118,    -1,   120,   121,   122,    -1,   124,   125,   126,
      -1,   128,   129,   130,    -1,   132,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,    -1,   143,   144,   145,    -1,
      73,   148,   149,   150,    77,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   111,   112,
      -1,    -1,   115,   116,   117,   118,    -1,   120,   121,   122,
      -1,   124,   125,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,   135,   136,    -1,   138,   139,   140,   141,    -1,
     143,   144,   145,    10,    -1,   148,   149,   150,    -1,   152,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    19,    -1,    -1,    -1,
      23,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    32,
      17,    34,    35,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    28,    29,    46,    -1,    -1,    73,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    41,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    65,    10,    67,    68,    -1,    70,    -1,    72,
      17,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,   106,
      -1,    28,    29,    -1,    -1,    -1,    73,    -1,    -1,    92,
      77,    -1,    95,    -1,    -1,   122,    -1,   124,   125,   126,
      -1,   128,   129,   130,    -1,   132,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,    -1,   143,   144,   145,   106,
      -1,   148,   149,   150,    -1,   152,    73,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,   122,    -1,   124,   125,   126,
      -1,   128,   129,   130,   147,   132,   133,   134,   135,   136,
     153,   138,   139,   140,   141,   102,   143,   144,   145,   106,
      -1,   148,   149,   150,    -1,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,   124,   125,   126,
      -1,   128,   129,   130,    -1,   132,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,    -1,   143,   144,   145,    10,
      -1,   148,   149,   150,    -1,   152,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    23,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    32,    17,    34,    35,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    28,    29,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    61,    77,    -1,    -1,    65,
      -1,    67,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    73,    -1,   106,    92,    77,    28,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,   124,   125,   126,    -1,   128,   129,   130,
      -1,   132,   133,   134,   135,   136,   106,   138,   139,   140,
     141,    -1,   143,   144,   145,    -1,    -1,   148,   149,   150,
      -1,   152,   122,    73,   124,   125,   126,    77,   128,   129,
     130,   147,   132,   133,   134,   135,   136,   153,   138,   139,
     140,   141,    -1,   143,   144,   145,    10,    -1,   148,   149,
     150,    -1,   152,    17,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   124,   125,   126,    -1,   128,   129,
     130,    -1,   132,   133,   134,   135,   136,    -1,   138,   139,
     140,   141,    -1,   143,   144,   145,    10,    -1,   148,   149,
     150,    -1,   152,    17,    -1,    -1,    -1,    -1,    -1,    73,
      -1,    -1,    -1,    77,    28,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,   106,    17,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,   122,    73,
     124,   125,   126,    77,   128,   129,   130,    41,   132,   133,
     134,   135,   136,    -1,   138,   139,   140,   141,    -1,   143,
     144,   145,    -1,    -1,   148,   149,   150,    -1,   152,    -1,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    73,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,   122,    -1,
     124,   125,   126,    -1,   128,   129,   130,    -1,   132,   133,
     134,   135,   136,    -1,   138,   139,   140,   141,   102,   143,
     144,   145,   106,   147,   148,   149,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
     124,   125,   126,    -1,   128,   129,   130,    -1,   132,   133,
     134,   135,   136,    -1,   138,   139,   140,   141,    -1,   143,
     144,   145,    10,    -1,   148,   149,   150,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    73,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    73,    -1,   106,    -1,
      77,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   124,   125,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,   135,   136,   106,
     138,   139,   140,   141,    -1,   143,   144,   145,    -1,    -1,
     148,   149,   150,    -1,    -1,   122,    73,   124,   125,   126,
      77,   128,   129,   130,    -1,   132,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,    -1,   143,   144,   145,    10,
      -1,   148,   149,   150,    -1,    -1,    17,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,   124,   125,   126,
      41,   128,   129,   130,    -1,   132,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,    -1,   143,   144,   145,    10,
     147,   148,   149,   150,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    -1,    77,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    73,   124,   125,   126,    77,   128,   129,   130,
      -1,   132,   133,   134,   135,   136,    -1,   138,   139,   140,
     141,    -1,   143,   144,   145,    -1,    -1,   148,   149,   150,
      -1,    -1,    -1,    73,    -1,   106,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,   124,   125,   126,    -1,   128,   129,   130,
      -1,   132,   133,   134,   135,   136,   106,   138,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
      -1,    -1,   122,    -1,   124,   125,   126,    -1,   128,   129,
     130,    -1,   132,   133,   134,   135,   136,    -1,   138,   139,
     140,   141,    -1,   143,   144,   145,    10,   147,   148,   149,
     150,    -1,    -1,    17,    -1,    -1,    -1,     1,    -1,     3,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    40,    -1,    42,    17,
      -1,    45,    -1,    -1,    -1,    -1,    -1,    51,    -1,    73,
      28,    -1,    -1,    77,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    80,    81,    -1,    83,
      17,    85,   106,    87,    -1,    -1,    -1,    -1,    -1,    93,
      -1,    28,    96,    -1,    -1,    73,    -1,    -1,   122,    77,
     124,   125,   126,    -1,   128,   129,   130,    -1,   132,   133,
     134,   135,   136,    -1,   138,   139,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,   150,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,
      77,    -1,    -1,    -1,   122,    -1,   124,   125,   126,    86,
     128,   129,   130,    -1,   132,   133,   134,   135,   136,    -1,
     138,   139,   140,   141,    -1,   143,   144,   145,    10,   106,
     148,   149,   150,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    28,   124,   125,   126,
      -1,   128,   129,   130,    -1,   132,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,    -1,   143,   144,   145,    -1,
      -1,   148,   149,   150,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    73,    -1,    -1,    -1,    77,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    73,   124,   125,   126,    77,   128,   129,   130,    -1,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
      -1,   143,   144,   145,    -1,    28,   148,   149,   150,    -1,
     102,    -1,    73,    -1,   106,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,   124,   125,   126,    -1,   128,   129,   130,    -1,
     132,   133,   134,   135,   136,   106,   138,   139,   140,   141,
      73,   143,   144,   145,    77,    -1,   148,   149,   150,    -1,
      -1,   122,    -1,   124,   125,   126,    -1,   128,   129,   130,
      -1,   132,   133,   134,   135,   136,    -1,   138,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
      10,    11,    -1,    -1,    -1,    -1,    -1,    17,    -1,   122,
      -1,   124,   125,   126,    -1,   128,   129,   130,    28,   132,
     133,   134,   135,    -1,    -1,   138,   139,   140,   141,    -1,
     143,    10,   145,    -1,    -1,   148,   149,   150,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    73,    -1,   106,    -1,    77,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   124,   125,   126,    -1,   128,   129,
     130,    -1,   132,   133,   134,   135,   136,   106,   138,   139,
     140,   141,    -1,   143,   144,   145,    -1,    -1,   148,   149,
     150,    -1,    -1,   122,    73,   124,   125,   126,    77,   128,
     129,   130,    -1,   132,   133,   134,   135,   136,    -1,   138,
     139,   140,   141,    -1,   143,   144,   145,    10,    -1,   148,
     149,   150,    -1,    -1,    17,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,   124,   125,   126,    -1,   128,
     129,   130,    -1,   132,   133,   134,   135,   136,    -1,   138,
     139,   140,   141,    -1,   143,   144,   145,    10,    -1,   148,
     149,   150,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      73,    -1,    -1,    -1,    77,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      73,   124,   125,   126,    77,   128,   129,   130,    -1,   132,
     133,   134,   135,   136,    -1,   138,   139,   140,   141,    -1,
     143,   144,   145,    -1,    -1,   148,   149,   150,    -1,    -1,
      -1,    73,    -1,   106,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,   122,
      -1,   124,   125,   126,    -1,   128,   129,   130,    -1,   132,
     133,   134,   135,   136,   106,   138,   139,   140,   141,    -1,
     143,   144,   145,    -1,    -1,   148,   149,   150,    -1,    -1,
     122,    -1,   124,   125,   126,    -1,   128,   129,   130,    -1,
     132,   133,   134,   135,   136,    -1,   138,   139,   140,   141,
      -1,   143,   144,   145,    10,    -1,   148,   149,   150,    -1,
      -1,    17,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    10,    11,    -1,    -1,    -1,
      30,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    42,    28,    -1,    45,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,
      -1,    77,    62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    81,    -1,    83,    -1,    85,    -1,    87,    73,    -1,
     106,    -1,    77,    93,    -1,    -1,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,   125,
     126,    -1,   128,   129,   130,    -1,   132,   133,   134,   135,
     136,   106,   138,   139,   140,   141,    -1,   143,   144,   145,
      -1,    -1,   148,   149,   150,    -1,    -1,   122,    -1,   124,
     125,   126,    -1,   128,   129,   130,    -1,   132,   133,   134,
     135,   136,    -1,   138,   139,   140,   141,    -1,   143,   144,
     145,    10,    -1,   148,   149,   150,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    -1,   106,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,   122,    -1,   124,   125,   126,    -1,   128,
     129,   130,    -1,   132,   133,   134,   135,   136,   106,   138,
     139,   140,   141,    -1,   143,   144,   145,    -1,    -1,   148,
     149,   150,    -1,    -1,   122,    -1,   124,   125,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,   135,   136,    -1,
     138,   139,   140,   141,    -1,   143,   144,   145,    10,    -1,
     148,   149,   150,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    -1,   106,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
     122,    -1,   124,   125,   126,    -1,   128,   129,   130,    -1,
     132,   133,   134,   135,   136,   106,   138,   139,   140,   141,
      -1,   143,   144,   145,    -1,    -1,   148,   149,   150,    -1,
      -1,   122,    -1,   124,   125,   126,    -1,   128,   129,   130,
      -1,   132,   133,   134,   135,   136,    -1,   138,   139,   140,
     141,    -1,   143,   144,   145,    10,    -1,   148,   149,   150,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    73,
      -1,   106,    -1,    77,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,    -1,   128,   129,   130,    -1,   132,   133,   134,
     135,   136,   106,   138,   139,   140,   141,    -1,   143,   144,
     145,    -1,    -1,   148,   149,   150,    -1,    -1,   122,    73,
     124,   125,   126,    77,   128,   129,   130,    -1,   132,   133,
     134,   135,   136,    -1,   138,   139,   140,   141,    -1,   143,
     144,   145,    10,    -1,   148,   149,   150,    -1,    -1,    17,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
     124,   125,   126,    -1,   128,   129,   130,    -1,   132,   133,
     134,   135,   136,    -1,   138,   139,   140,   141,    -1,   143,
     144,   145,    -1,    -1,   148,   149,   150,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      28,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,   122,    -1,   124,   125,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,   135,   136,    -1,
     138,   139,   140,   141,    -1,   143,   144,   145,   106,    -1,
     148,   149,   150,    -1,    -1,    73,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,   122,    -1,   124,   125,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,   135,    -1,    -1,
     138,   139,   140,   141,    -1,   143,   144,   145,   106,    -1,
     148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   124,   125,   126,    -1,
     128,   129,   130,    -1,   132,   133,   134,   135,    -1,    -1,
     138,   139,   140,   141,    -1,   143,    -1,   145,    -1,    -1,
     148,   149,   150,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,
     154,    -1,   156
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   162,   163,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    15,    16,    19,    20,    21,    22,
      23,    25,    26,    27,    29,    30,    32,    34,    35,    36,
      37,    38,    39,    40,    42,    43,    45,    47,    48,    49,
      50,    51,    52,    54,    55,    57,    61,    62,    64,    65,
      66,    67,    70,    71,    72,    75,    76,    78,    79,    80,
      81,    82,    83,    84,    85,    87,    88,    90,    91,    92,
      93,    95,    96,    97,   101,   103,   106,   122,   123,   124,
     125,   130,   140,   142,   144,   145,   147,   150,   152,   154,
     156,   164,   165,   166,   167,   168,   170,   175,   176,   177,
     181,   183,   187,   188,   189,   190,   191,   192,   194,   195,
     196,   200,   201,   204,   205,   208,   209,   212,   215,   216,
     239,   242,   243,   263,   264,   265,   266,   267,   268,   276,
     277,   278,   279,   282,   283,   284,   285,   286,   287,   289,
     290,   291,   292,   293,   147,   166,   279,   102,   269,   270,
       3,   178,    12,    36,    37,    39,    85,   152,   156,   212,
     263,   267,   277,   278,   279,   282,   284,   285,   269,   279,
      92,   178,   166,   257,   279,   166,   154,   279,   280,     7,
       9,    92,   185,   280,    63,   104,   193,   279,   279,    23,
      35,   242,   279,   279,   154,     3,    15,    16,    22,    30,
      40,    42,    45,    51,    62,    71,    80,    81,    83,    85,
      87,    93,    96,   180,   182,   213,   154,   180,   244,   245,
      29,   170,   186,   279,   279,   279,   279,   279,   279,     7,
     154,    35,   189,   189,   146,   235,   256,   279,    90,    91,
     147,   279,   281,   279,   186,   279,   279,   279,    84,   154,
     166,   279,   279,   170,   177,   279,   282,   170,   177,   279,
     180,   240,   279,   256,   279,   279,   279,   279,   279,   279,
     279,   279,     1,   153,   164,   171,   256,    94,   131,   235,
     258,   259,   281,   256,   279,   288,    65,   166,    69,   169,
     180,   180,    46,    68,   230,    24,    63,    74,    98,   147,
     137,    10,    17,    28,    73,    77,   106,   122,   124,   125,
     126,   128,   129,   130,   132,   133,   134,   135,   136,   138,
     139,   140,   141,   143,   144,   145,   148,   149,   150,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   151,   222,   154,   156,    73,    77,
     154,   166,   147,   279,   279,   279,   256,   152,   170,    41,
     269,   240,   147,   127,   147,   101,   181,   235,   260,   261,
     262,   281,   240,   205,   180,   154,   186,    29,    41,   186,
      29,    41,   102,   186,   272,    33,    58,   147,    86,   170,
     222,   260,   166,   154,   224,    94,   154,   180,   246,   247,
       1,   126,   251,    41,   127,   166,   186,   186,   260,   180,
     127,   147,   279,   279,   147,   152,   186,   154,   260,   147,
     197,   147,   197,   147,   107,   241,   147,   147,    33,    58,
     186,   147,   153,   153,   164,   127,   153,   279,   127,   155,
     127,   155,    41,   127,   157,   272,   105,   127,   157,     7,
      53,   126,   206,   152,   217,    74,   244,   244,   244,   244,
     279,   279,   279,   279,   193,   279,   193,   279,   279,   279,
     279,   279,    16,    96,   279,   279,    30,    51,    92,   181,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   281,   281,   281,   281,
     281,   260,   260,   193,   279,   193,   279,    24,    41,    74,
      98,   271,   274,   275,   279,   293,    29,    41,    29,    41,
      86,    41,   157,   193,   279,   186,   147,   279,   279,   107,
     155,   127,   147,   180,    41,   256,    39,   279,   193,   279,
      39,   279,   193,   279,   154,   186,   150,   172,   174,   279,
     172,   173,   166,   279,    31,   279,   155,    24,    41,    44,
      60,    63,    74,    92,   165,   225,   226,   227,   228,   214,
     247,   127,   155,    16,    30,    42,    62,    80,    81,    85,
      96,   156,   184,   252,   264,   107,   248,   279,   245,   155,
     235,   279,     1,   202,   260,   155,    18,   198,   252,   264,
     127,   174,   173,   153,   155,   155,   258,   155,   258,   193,
     279,   157,   166,   279,   157,   279,   157,   279,   180,   256,
     152,     1,   180,   210,   211,    24,    63,    74,    92,   219,
     229,   244,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   155,   157,    41,    74,   127,   155,   263,    73,    73,
      39,   193,   279,    39,   193,   279,   193,   279,   269,   269,
     147,   235,   281,   262,   206,   279,   155,   279,    29,   186,
      29,   186,   279,    29,   186,   272,    29,   186,   272,   273,
     274,   127,   147,    11,   147,    31,    31,   166,    86,   170,
      41,    74,   227,   127,   155,   154,   180,    24,    63,    74,
      92,   231,   155,   247,   251,     1,   256,    56,   281,   153,
      59,    99,   153,   203,   155,   154,   170,   180,   199,   240,
     147,   147,   157,   272,   157,   272,   166,   105,   152,     1,
     207,   153,   107,   127,   153,    74,   218,     1,     3,    10,
      15,    17,    22,    40,    45,    51,    71,    83,    87,    93,
     122,   123,   124,   125,   128,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   145,   148,   149,
     150,   151,   154,   179,   182,   220,   221,   222,   263,   147,
     274,   251,   263,   263,   279,    29,    29,   279,    29,    29,
     157,   157,   186,   186,   152,   186,    86,    39,   279,    39,
     279,    86,    39,   279,   186,    39,   279,   186,   127,   155,
     279,   279,   166,   279,   166,    31,   180,   226,   247,   126,
     255,    74,   251,   248,   157,    41,   157,    29,   166,   256,
     199,   126,   170,    39,   166,   279,   157,    39,   166,   279,
     157,   279,     1,   153,   171,   153,    35,    67,    70,   153,
     165,   188,   279,   211,   231,   154,   223,   180,   279,   129,
     223,   223,   248,    86,    39,    39,    86,    39,    39,    39,
      39,     1,   207,   279,   279,   279,   279,   279,   279,   274,
      11,    31,   166,   255,   155,   156,   184,   235,   254,   264,
     131,   237,   248,   100,   238,   279,   252,   264,   166,   186,
     155,   279,   279,   166,   279,   166,   153,   153,   188,     1,
     126,   250,   225,   155,     3,    87,   221,   222,   279,   279,
     279,   279,   279,   279,   153,   153,    86,    86,    86,    86,
     279,   166,   248,   255,   157,   256,   235,   236,   279,   279,
     170,   187,   234,   157,   170,    86,    86,   156,   184,   249,
     264,    89,   232,   155,   223,   223,    86,    86,    86,    86,
      86,    86,   279,   279,   279,   279,   237,   248,   235,   253,
     254,   264,    41,   157,   264,   279,   279,     1,   157,   256,
     238,   279,   253,   254,   157,   249,   264,   157,   147,   233,
     234,   157,   249,   264,   253
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
#line 466 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 471 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 472 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 479 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 484 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 485 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 507 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 508 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 509 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 511 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 512 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 513 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 514 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 515 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 516 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 517 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 518 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 519 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 524 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (6)].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[(1) - (6)].b), (yyvsp[(2) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 527 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType, (yyvsp[(6) - (7)].pblockstmt), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 529 "chapel.ypp"
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 534 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 535 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 540 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 554 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 555 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 556 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 561 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 562 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 567 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 572 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 577 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 581 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 588 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 597 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 607 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 617 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 619 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 621 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 623 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 625 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 631 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 632 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 636 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 637 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 638 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError("this"); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 643 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 655 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 656 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 667 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 668 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 669 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 670 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 671 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 672 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 673 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 674 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 675 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 676 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 677 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 678 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 679 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 680 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 681 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 682 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 686 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 687 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 690 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 702 "chapel.ypp"
    { (yyval.pch) = "_owned"; }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 703 "chapel.ypp"
    { (yyval.pch) = "_shared"; }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 704 "chapel.ypp"
    { (yyval.pch) = "_borrowed"; }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 705 "chapel.ypp"
    { (yyval.pch) = "_unmanaged"; }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 706 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 707 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 712 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 713 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 717 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 718 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 722 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 723 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 727 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 729 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 730 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 743 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 744 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 745 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 758 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 759 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 760 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 761 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 762 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 763 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 764 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 765 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 766 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 767 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 768 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 770 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 771 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 772 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 774 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 780 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 786 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 792 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 798 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 805 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 814 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 818 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 819 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 820 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 821 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 822 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 824 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 826 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 828 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 833 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 836 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 837 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 838 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 839 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 840 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 841 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 845 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 846 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 850 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 851 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 852 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 856 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 857 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 861 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 865 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 867 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 871 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 872 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 877 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 879 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 881 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 888 "chapel.ypp"
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

  case 186:
/* Line 1792 of yacc.c  */
#line 898 "chapel.ypp"
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

  case 187:
/* Line 1792 of yacc.c  */
#line 908 "chapel.ypp"
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

  case 188:
/* Line 1792 of yacc.c  */
#line 918 "chapel.ypp"
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

  case 189:
/* Line 1792 of yacc.c  */
#line 930 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 935 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 940 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 948 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 949 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 954 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 956 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 958 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 963 "chapel.ypp"
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

  case 198:
/* Line 1792 of yacc.c  */
#line 979 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 986 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 995 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1003 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1007 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1013 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1014 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1019 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1024 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1029 "chapel.ypp"
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

  case 208:
/* Line 1792 of yacc.c  */
#line 1049 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1055 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1062 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[(2) - (2)].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1068 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[(2) - (2)].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1074 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1085 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1091 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1098 "chapel.ypp"
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

  case 216:
/* Line 1792 of yacc.c  */
#line 1124 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1128 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1133 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1137 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1142 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1149 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1153 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1154 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1155 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1156 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1157 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1158 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1159 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1160 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1161 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1162 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1163 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1164 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1165 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1166 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1167 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1168 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1169 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1170 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1171 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1172 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1173 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1174 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1175 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1176 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1177 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1178 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1182 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1183 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1184 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1185 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1186 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1187 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1188 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1190 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1191 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1192 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1193 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1197 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1198 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1206 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1207 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1208 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1213 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1215 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1217 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1219 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1221 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1225 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1226 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1230 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1231 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1232 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1234 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1235 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1236 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1237 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1238 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1242 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1243 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1244 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1245 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1246 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1247 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1251 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1252 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1256 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1257 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1258 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1259 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1260 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1261 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1265 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1266 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1269 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1286 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1311 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1324 "chapel.ypp"
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

  case 316:
/* Line 1792 of yacc.c  */
#line 1340 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1342 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1344 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1349 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1357 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1365 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1373 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1382 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1391 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1392 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1393 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1399 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1407 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1409 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1414 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1416 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1418 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1423 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1425 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1427 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1434 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1435 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1441 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1443 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1445 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1449 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1453 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1455 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1459 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1466 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1467 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1469 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1470 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1476 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1479 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1500 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1508 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1516 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1541 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1546 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1548 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1555 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1556 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1557 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1563 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1568 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1569 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1573 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1574 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1578 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1583 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1584 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1588 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1591 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1613 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1615 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1617 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1619 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1621 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1623 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1625 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1627 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1629 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1631 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1633 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1638 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1640 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1642 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1644 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1646 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1648 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1650 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1652 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1654 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1656 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1658 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1660 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1662 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1669 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1675 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1681 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1687 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1696 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1705 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1721 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1725 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1730 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1734 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1735 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1739 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1743 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1744 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1749 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1753 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1757 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1763 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1764 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1765 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1766 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1767 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1768 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1779 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1784 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1800 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1802 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1804 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1806 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1808 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1810 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1812 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1814 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1818 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1819 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1822 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1823 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1824 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1846 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1847 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1848 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1852 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1853 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1854 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1855 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1863 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1864 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1865 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1866 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1870 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1871 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1872 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1873 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1874 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1875 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1876 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1877 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1878 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1880 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1884 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 1891 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1892 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1896 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1897 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1898 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1899 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1900 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1901 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1902 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1903 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1904 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1905 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1906 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 1907 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 1908 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 523:
/* Line 1792 of yacc.c  */
#line 1909 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 1910 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 1911 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 1912 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 1913 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 1914 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 1915 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 1916 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 1917 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 1918 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 1922 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 1923 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 1924 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 1925 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 1926 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 1927 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 1931 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 1932 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 1933 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 1934 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 1938 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 1939 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 1940 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 1941 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 1946 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 1947 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 1948 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 1949 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 1950 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 1951 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 1952 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 9159 "bison-chapel.cpp"
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


