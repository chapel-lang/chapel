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
     TASSIGNREDUCE = 372,
     TASSIGNSL = 373,
     TASSIGNSR = 374,
     TBAND = 375,
     TBNOT = 376,
     TBOR = 377,
     TBXOR = 378,
     TCOLON = 379,
     TCOMMA = 380,
     TDIVIDE = 381,
     TDOT = 382,
     TDOTDOT = 383,
     TDOTDOTDOT = 384,
     TEQUAL = 385,
     TEXP = 386,
     TGREATER = 387,
     TGREATEREQUAL = 388,
     THASH = 389,
     TIO = 390,
     TLESS = 391,
     TLESSEQUAL = 392,
     TMINUS = 393,
     TMOD = 394,
     TNOT = 395,
     TNOTEQUAL = 396,
     TOR = 397,
     TPLUS = 398,
     TQUESTION = 399,
     TSEMI = 400,
     TSHIFTLEFT = 401,
     TSHIFTRIGHT = 402,
     TSTAR = 403,
     TSWAP = 404,
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
#define YYLAST   15810

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  159
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  131
/* YYNRULES -- Number of rules.  */
#define YYNRULES  544
/* YYNRULES -- Number of states.  */
#define YYNSTATES  984

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
     263,   265,   267,   269,   271,   273,   275,   277,   279,   281,
     283,   285,   287,   289,   290,   292,   295,   297,   300,   304,
     306,   308,   311,   314,   316,   318,   320,   322,   324,   326,
     330,   336,   342,   345,   348,   354,   358,   365,   372,   377,
     383,   389,   393,   397,   404,   410,   417,   423,   430,   434,
     439,   446,   454,   461,   469,   474,   480,   485,   490,   494,
     501,   507,   514,   520,   529,   537,   540,   544,   548,   551,
     554,   558,   562,   563,   566,   569,   573,   579,   581,   585,
     589,   595,   601,   602,   605,   609,   612,   616,   623,   632,
     639,   648,   650,   652,   654,   655,   658,   659,   662,   666,
     672,   678,   680,   682,   685,   689,   691,   695,   696,   697,
     706,   707,   709,   712,   715,   717,   718,   719,   730,   734,
     738,   744,   750,   754,   756,   760,   762,   765,   767,   769,
     771,   773,   775,   777,   779,   781,   783,   785,   787,   789,
     791,   793,   795,   797,   799,   801,   803,   805,   807,   809,
     811,   813,   815,   817,   819,   821,   823,   825,   827,   829,
     831,   833,   835,   837,   838,   842,   846,   847,   849,   853,
     858,   864,   869,   876,   883,   884,   886,   888,   890,   892,
     894,   897,   900,   902,   904,   906,   907,   909,   911,   914,
     916,   918,   920,   922,   923,   925,   928,   930,   932,   934,
     935,   937,   939,   941,   943,   945,   948,   950,   951,   953,
     956,   959,   960,   963,   967,   972,   977,   980,   985,   986,
     989,   992,   997,  1002,  1007,  1013,  1018,  1019,  1021,  1023,
    1025,  1029,  1033,  1039,  1041,  1043,  1047,  1049,  1052,  1056,
    1057,  1060,  1063,  1067,  1070,  1075,  1079,  1083,  1088,  1092,
    1093,  1096,  1099,  1102,  1104,  1105,  1108,  1111,  1114,  1116,
    1121,  1126,  1133,  1137,  1138,  1140,  1142,  1146,  1151,  1155,
    1160,  1167,  1168,  1171,  1174,  1177,  1180,  1182,  1184,  1188,
    1192,  1194,  1198,  1200,  1202,  1204,  1208,  1212,  1213,  1215,
    1217,  1221,  1225,  1229,  1231,  1233,  1235,  1237,  1239,  1241,
    1243,  1245,  1247,  1250,  1255,  1260,  1265,  1271,  1274,  1277,
    1280,  1283,  1286,  1289,  1296,  1303,  1308,  1318,  1328,  1336,
    1343,  1350,  1355,  1365,  1375,  1383,  1388,  1395,  1402,  1412,
    1422,  1429,  1431,  1433,  1435,  1437,  1439,  1441,  1443,  1445,
    1449,  1450,  1452,  1457,  1459,  1463,  1468,  1470,  1474,  1479,
    1483,  1487,  1489,  1491,  1494,  1496,  1499,  1501,  1503,  1507,
    1510,  1515,  1517,  1519,  1521,  1523,  1525,  1527,  1529,  1531,
    1536,  1540,  1544,  1548,  1552,  1555,  1558,  1560,  1561,  1563,
    1566,  1569,  1571,  1573,  1575,  1577,  1579,  1581,  1583,  1588,
    1593,  1598,  1602,  1606,  1610,  1614,  1619,  1623,  1628,  1630,
    1632,  1634,  1636,  1638,  1642,  1647,  1651,  1656,  1660,  1665,
    1669,  1675,  1679,  1683,  1687,  1691,  1695,  1699,  1703,  1707,
    1711,  1715,  1719,  1723,  1727,  1731,  1735,  1739,  1743,  1747,
    1751,  1755,  1759,  1763,  1767,  1770,  1773,  1776,  1779,  1782,
    1785,  1789,  1793,  1797,  1801,  1805,  1809,  1813,  1817,  1819,
    1821,  1823,  1825,  1827,  1829
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     160,     0,    -1,   161,    -1,    -1,   161,   162,    -1,   164,
      -1,   163,   164,    -1,    64,     7,    -1,   163,    64,     7,
      -1,   165,    -1,   168,    -1,   173,    -1,   174,    -1,   184,
      -1,   175,    -1,   187,    -1,   190,    -1,   188,    -1,   197,
      -1,   191,    -1,   192,    -1,   196,    -1,   183,    -1,   264,
     145,    -1,    12,   164,    -1,    13,   265,   164,    -1,    14,
     176,   145,    -1,    20,   265,   168,    -1,    25,   176,   145,
      -1,    27,   253,   145,    -1,    47,   177,   164,    -1,    50,
     275,   182,    -1,    50,   182,    -1,    56,   275,   182,    -1,
      78,   275,   182,    -1,    78,   182,    -1,    84,   164,    -1,
     101,   275,   145,    -1,     1,   145,    -1,   166,   167,    52,
     177,   150,   151,    -1,   166,   167,    52,   177,   150,   169,
     151,    -1,   166,   167,    52,   177,   150,     1,   151,    -1,
      -1,    69,    -1,    66,    -1,    -1,    68,    -1,   150,   151,
      -1,   150,   169,   151,    -1,   150,     1,   151,    -1,   162,
      -1,   169,   162,    -1,   275,    -1,   275,    11,   275,    -1,
     170,   125,   275,    -1,   170,   125,   275,    11,   275,    -1,
      -1,   170,    -1,   148,    -1,   170,    -1,    95,   252,   145,
      -1,    95,   275,    33,   172,   145,    -1,    95,   275,    57,
     171,   145,    -1,    74,   252,   145,    -1,   278,   218,   277,
     145,    -1,   278,   149,   277,   145,    -1,   278,   117,   277,
     145,    -1,   278,   111,   277,   145,    -1,   278,   112,   277,
     145,    -1,    -1,     3,    -1,     3,    -1,   178,    -1,    15,
      -1,    45,    -1,    91,    -1,    70,    -1,    40,    -1,    22,
      -1,    82,    -1,    84,    -1,    80,    -1,    61,    -1,    79,
      -1,    16,    -1,    94,    -1,    30,    -1,    42,    -1,    15,
      -1,    45,    -1,    91,    -1,    70,    -1,    40,    -1,    22,
      -1,    82,    -1,    84,    -1,    80,    -1,    61,    -1,    79,
      -1,    16,    -1,    94,    -1,    30,    -1,    42,    -1,    -1,
       7,    -1,    29,   164,    -1,   168,    -1,    75,   145,    -1,
      75,   277,   145,    -1,   145,    -1,   185,    -1,    69,   185,
      -1,    66,   185,    -1,   204,    -1,   235,    -1,   200,    -1,
     186,    -1,   212,    -1,   238,    -1,    38,   275,   145,    -1,
      38,   275,    33,   172,   145,    -1,    38,   275,    57,   171,
     145,    -1,    38,   238,    -1,    35,     9,    -1,    29,   164,
      99,   275,   145,    -1,    99,   275,   182,    -1,    21,   275,
      41,   275,   265,   182,    -1,    21,   275,    41,   189,   265,
     182,    -1,    21,   275,   265,   182,    -1,    36,   275,    41,
     275,   182,    -1,    36,   275,    41,   189,   182,    -1,    36,
     275,   182,    -1,    36,   189,   182,    -1,    36,    62,   177,
      41,   275,   182,    -1,    37,   275,    41,   275,   182,    -1,
      37,   275,    41,   275,   268,   182,    -1,    37,   275,    41,
     189,   182,    -1,    37,   275,    41,   189,   268,   182,    -1,
      37,   275,   182,    -1,    37,   275,   268,   182,    -1,   154,
     252,    41,   275,   155,   164,    -1,   154,   252,    41,   275,
     268,   155,   164,    -1,   154,   252,    41,   189,   155,   164,
      -1,   154,   252,    41,   189,   268,   155,   164,    -1,   154,
     252,   155,   164,    -1,   154,   252,   268,   155,   164,    -1,
     102,   152,   252,   153,    -1,    39,   275,    85,   164,    -1,
      39,   275,   168,    -1,    39,   275,    85,   164,    31,   164,
      -1,    39,   275,   168,    31,   164,    -1,    39,   275,   218,
     275,    85,   164,    -1,    39,   275,   218,   275,   168,    -1,
      39,   275,   218,   275,    85,   164,    31,   164,    -1,    39,
     275,   218,   275,   168,    31,   164,    -1,    26,   164,    -1,
      88,   275,   145,    -1,    89,   275,   145,    -1,    88,   175,
      -1,    89,   175,    -1,    88,   168,   193,    -1,    89,   168,
     193,    -1,    -1,   193,   194,    -1,    18,   168,    -1,    18,
     195,   168,    -1,    18,   152,   195,   153,   168,    -1,   177,
      -1,   177,   124,   275,    -1,    86,   275,   145,    -1,    77,
     275,   150,   198,   151,    -1,    77,   275,   150,     1,   151,
      -1,    -1,   198,   199,    -1,    97,   252,   182,    -1,    58,
     164,    -1,    58,    29,   164,    -1,   201,   177,   202,   150,
     203,   151,    -1,    35,   181,   201,   177,   202,   150,   203,
     151,    -1,   201,   177,   202,   150,     1,   151,    -1,    35,
     181,   201,   177,   202,   150,     1,   151,    -1,    19,    -1,
      71,    -1,    93,    -1,    -1,   124,   252,    -1,    -1,   203,
     184,    -1,   203,   163,   184,    -1,   205,   177,   150,   206,
     151,    -1,   205,   177,   150,     1,   151,    -1,    32,    -1,
     207,    -1,   206,   125,    -1,   206,   125,   207,    -1,   177,
      -1,   177,   105,   275,    -1,    -1,    -1,    48,   209,   220,
     210,   227,   247,   234,   230,    -1,    -1,    43,    -1,    34,
     276,    -1,    35,   276,    -1,    60,    -1,    -1,    -1,   211,
     226,   213,   215,   214,   227,   246,   228,   234,   229,    -1,
     225,   217,   219,    -1,   225,   218,   219,    -1,   225,   216,
     127,   217,   219,    -1,   225,   216,   127,   218,   219,    -1,
     225,     1,   219,    -1,   259,    -1,   152,   275,   153,    -1,
     177,    -1,   121,   177,    -1,   120,    -1,   122,    -1,   123,
      -1,   121,    -1,   130,    -1,   141,    -1,   137,    -1,   133,
      -1,   136,    -1,   132,    -1,   143,    -1,   138,    -1,   148,
      -1,   126,    -1,   146,    -1,   147,    -1,   139,    -1,   131,
      -1,   140,    -1,    17,    -1,   134,    -1,    10,    -1,   149,
      -1,   135,    -1,   105,    -1,   116,    -1,   113,    -1,   115,
      -1,   109,    -1,   114,    -1,   110,    -1,   106,    -1,   107,
      -1,   108,    -1,   119,    -1,   118,    -1,    -1,   152,   221,
     153,    -1,   152,   221,   153,    -1,    -1,   222,    -1,   221,
     125,   222,    -1,   223,   177,   251,   244,    -1,   163,   223,
     177,   251,   244,    -1,   223,   177,   251,   233,    -1,   223,
     152,   243,   153,   251,   244,    -1,   223,   152,   243,   153,
     251,   233,    -1,    -1,   224,    -1,    41,    -1,    44,    -1,
      59,    -1,    24,    -1,    24,    41,    -1,    24,    73,    -1,
      62,    -1,    73,    -1,    90,    -1,    -1,    62,    -1,    73,
      -1,    24,    73,    -1,    24,    -1,    90,    -1,    67,    -1,
      46,    -1,    -1,    24,    -1,    24,    73,    -1,    73,    -1,
      62,    -1,    90,    -1,    -1,    87,    -1,   145,    -1,   230,
      -1,   168,    -1,   183,    -1,   144,   177,    -1,   144,    -1,
      -1,   231,    -1,   129,   275,    -1,   129,   232,    -1,    -1,
      98,   275,    -1,    90,   236,   145,    -1,    23,    90,   236,
     145,    -1,    35,    90,   236,   145,    -1,   177,   237,    -1,
     177,   237,   125,   236,    -1,    -1,   105,   260,    -1,   105,
     248,    -1,   239,    62,   240,   145,    -1,   239,    24,   240,
     145,    -1,   239,    73,   240,   145,    -1,   239,    24,    73,
     240,   145,    -1,   239,    96,   240,   145,    -1,    -1,    23,
      -1,    35,    -1,   241,    -1,   240,   125,   241,    -1,   177,
     247,   244,    -1,   152,   243,   153,   247,   244,    -1,    92,
      -1,   177,    -1,   152,   243,   153,    -1,   242,    -1,   242,
     125,    -1,   242,   125,   243,    -1,    -1,   105,    55,    -1,
     105,   277,    -1,   154,   155,   260,    -1,   154,   155,    -1,
     154,   252,   155,   260,    -1,   154,   252,   155,    -1,   154,
     155,   245,    -1,   154,   252,   155,   245,    -1,   154,     1,
     155,    -1,    -1,   124,   260,    -1,   124,   245,    -1,   124,
     180,    -1,     1,    -1,    -1,   124,   260,    -1,   124,   248,
      -1,   124,   180,    -1,     1,    -1,   154,   252,   155,   260,
      -1,   154,   252,   155,   248,    -1,   154,   252,    41,   275,
     155,   260,    -1,   154,     1,   155,    -1,    -1,   260,    -1,
     231,    -1,   154,   155,   249,    -1,   154,   252,   155,   249,
      -1,   154,   155,   250,    -1,   154,   252,   155,   250,    -1,
     154,   252,    41,   275,   155,   249,    -1,    -1,   124,   260,
      -1,   124,   231,    -1,   124,   180,    -1,   124,   250,    -1,
     275,    -1,   231,    -1,   252,   125,   275,    -1,   252,   125,
     231,    -1,   275,    -1,   253,   125,   275,    -1,    92,    -1,
     277,    -1,   231,    -1,   254,   125,   254,    -1,   255,   125,
     254,    -1,    -1,   257,    -1,   258,    -1,   257,   125,   258,
      -1,     3,   105,   231,    -1,     3,   105,   277,    -1,   231,
      -1,   277,    -1,     3,    -1,   179,    -1,   263,    -1,   278,
      -1,   262,    -1,   286,    -1,   285,    -1,    80,   275,    -1,
      42,   152,   256,   153,    -1,    30,   152,   256,   153,    -1,
      83,   152,   256,   153,    -1,    81,    83,   152,   256,   153,
      -1,    12,   275,    -1,    84,   275,    -1,    16,   275,    -1,
      61,   275,    -1,    94,   275,    -1,    79,   275,    -1,    36,
     275,    41,   275,    29,   275,    -1,    36,   275,    41,   189,
      29,   275,    -1,    36,   275,    29,   275,    -1,    36,   275,
      41,   275,    29,    39,   275,    85,   275,    -1,    36,   275,
      41,   189,    29,    39,   275,    85,   275,    -1,    36,   275,
      29,    39,   275,    85,   275,    -1,    37,   275,    41,   275,
      29,   275,    -1,    37,   275,    41,   189,    29,   275,    -1,
      37,   275,    29,   275,    -1,    37,   275,    41,   275,    29,
      39,   275,    85,   275,    -1,    37,   275,    41,   189,    29,
      39,   275,    85,   275,    -1,    37,   275,    29,    39,   275,
      85,   275,    -1,   154,   252,   155,   275,    -1,   154,   252,
      41,   275,   155,   275,    -1,   154,   252,    41,   189,   155,
     275,    -1,   154,   252,    41,   275,   155,    39,   275,    85,
     275,    -1,   154,   252,    41,   189,   155,    39,   275,    85,
     275,    -1,    39,   275,    85,   275,    31,   275,    -1,    54,
      -1,   263,    -1,   259,    -1,   281,    -1,   280,    -1,   208,
      -1,   273,    -1,   274,    -1,   272,   135,   275,    -1,    -1,
     266,    -1,   100,   152,   267,   153,    -1,   270,    -1,   267,
     125,   270,    -1,   100,   152,   269,   153,    -1,   270,    -1,
     269,   125,   270,    -1,   271,   259,   247,   244,    -1,   289,
      72,   259,    -1,   275,    72,   259,    -1,    24,    -1,    41,
      -1,    24,    41,    -1,    73,    -1,    24,    73,    -1,    96,
      -1,   278,    -1,   272,   135,   275,    -1,    53,   275,    -1,
      49,   240,    41,   275,    -1,   283,    -1,   260,    -1,   261,
      -1,   287,    -1,   288,    -1,   208,    -1,   273,    -1,   274,
      -1,   152,   129,   275,   153,    -1,   275,   124,   275,    -1,
     275,   124,    16,    -1,   275,   124,    94,    -1,   275,   128,
     275,    -1,   275,   128,    -1,   128,   275,    -1,   128,    -1,
      -1,   275,    -1,    88,   275,    -1,    89,   275,    -1,   275,
      -1,   259,    -1,   280,    -1,   281,    -1,   282,    -1,   278,
      -1,   208,    -1,   279,   152,   256,   153,    -1,   279,   154,
     256,   155,    -1,    65,   152,   256,   153,    -1,   275,   127,
       3,    -1,   275,   127,    90,    -1,   275,   127,    30,    -1,
     152,   254,   153,    -1,   152,   254,   125,   153,    -1,   152,
     255,   153,    -1,   152,   255,   125,   153,    -1,     4,    -1,
       5,    -1,     6,    -1,     7,    -1,     8,    -1,   150,   252,
     151,    -1,   150,   252,   125,   151,    -1,   154,   252,   155,
      -1,   154,   252,   125,   155,    -1,   154,   284,   155,    -1,
     154,   284,   125,   155,    -1,   275,   103,   275,    -1,   284,
     125,   275,   103,   275,    -1,   275,   143,   275,    -1,   275,
     138,   275,    -1,   275,   148,   275,    -1,   275,   126,   275,
      -1,   275,   146,   275,    -1,   275,   147,   275,    -1,   275,
     139,   275,    -1,   275,   130,   275,    -1,   275,   141,   275,
      -1,   275,   137,   275,    -1,   275,   133,   275,    -1,   275,
     136,   275,    -1,   275,   132,   275,    -1,   275,   120,   275,
      -1,   275,   122,   275,    -1,   275,   123,   275,    -1,   275,
     104,   275,    -1,   275,   142,   275,    -1,   275,   131,   275,
      -1,   275,    17,   275,    -1,   275,    10,   275,    -1,   275,
     134,   275,    -1,   275,    28,   275,    -1,   143,   275,    -1,
     138,   275,    -1,    51,   275,    -1,    63,   275,    -1,   140,
     275,    -1,   121,   275,    -1,   275,    72,   275,    -1,   275,
      72,   189,    -1,   289,    72,   275,    -1,   289,    72,   189,
      -1,   275,    76,   275,    -1,   275,    76,   189,    -1,   289,
      76,   275,    -1,   289,    76,   189,    -1,   143,    -1,   148,
      -1,   104,    -1,   142,    -1,   120,    -1,   122,    -1,   123,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   465,   465,   470,   471,   477,   478,   483,   484,   489,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   517,   518,   522,
     525,   527,   533,   534,   535,   539,   540,   553,   554,   555,
     560,   561,   566,   571,   576,   580,   587,   592,   596,   601,
     605,   606,   607,   611,   615,   617,   619,   621,   623,   630,
     631,   635,   636,   647,   648,   649,   650,   651,   652,   653,
     654,   655,   656,   657,   658,   659,   660,   661,   665,   666,
     667,   668,   669,   670,   671,   678,   679,   680,   681,   682,
     683,   684,   685,   690,   691,   695,   696,   700,   701,   705,
     706,   707,   708,   709,   710,   711,   712,   716,   717,   721,
     722,   723,   724,   728,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   749,   750,
     751,   757,   763,   769,   775,   782,   792,   796,   797,   798,
     799,   800,   802,   804,   806,   811,   814,   815,   816,   817,
     818,   819,   823,   824,   828,   829,   830,   834,   835,   839,
     842,   844,   849,   850,   854,   856,   858,   865,   875,   885,
     895,   908,   913,   918,   926,   927,   932,   933,   935,   940,
     956,   963,   972,   980,   984,   991,   992,   997,  1002,   996,
    1027,  1033,  1040,  1046,  1052,  1063,  1069,  1062,  1101,  1105,
    1110,  1114,  1119,  1126,  1127,  1131,  1132,  1133,  1134,  1135,
    1136,  1137,  1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,
    1146,  1147,  1148,  1149,  1150,  1151,  1152,  1153,  1154,  1155,
    1156,  1160,  1161,  1162,  1163,  1164,  1165,  1166,  1167,  1168,
    1169,  1170,  1171,  1175,  1176,  1180,  1184,  1185,  1186,  1190,
    1192,  1194,  1196,  1198,  1203,  1204,  1208,  1209,  1210,  1211,
    1212,  1213,  1214,  1215,  1216,  1220,  1221,  1222,  1223,  1224,
    1225,  1229,  1230,  1234,  1235,  1236,  1237,  1238,  1239,  1243,
    1244,  1247,  1248,  1252,  1253,  1257,  1259,  1264,  1265,  1269,
    1270,  1274,  1275,  1279,  1281,  1283,  1288,  1301,  1318,  1319,
    1321,  1326,  1334,  1342,  1350,  1359,  1369,  1370,  1371,  1375,
    1376,  1384,  1386,  1391,  1393,  1395,  1400,  1402,  1404,  1411,
    1412,  1413,  1418,  1420,  1422,  1426,  1430,  1432,  1436,  1444,
    1445,  1446,  1447,  1448,  1453,  1454,  1455,  1456,  1457,  1477,
    1481,  1485,  1493,  1500,  1501,  1502,  1506,  1508,  1514,  1516,
    1518,  1523,  1524,  1525,  1526,  1527,  1533,  1534,  1535,  1536,
    1540,  1541,  1545,  1546,  1547,  1551,  1552,  1556,  1557,  1561,
    1562,  1566,  1567,  1568,  1569,  1573,  1574,  1585,  1586,  1587,
    1588,  1589,  1590,  1592,  1594,  1596,  1598,  1600,  1602,  1604,
    1606,  1608,  1610,  1615,  1617,  1619,  1621,  1623,  1625,  1627,
    1629,  1631,  1633,  1635,  1637,  1639,  1646,  1652,  1658,  1664,
    1673,  1683,  1691,  1692,  1693,  1694,  1695,  1696,  1697,  1698,
    1703,  1704,  1708,  1712,  1713,  1717,  1721,  1722,  1726,  1730,
    1734,  1741,  1742,  1743,  1744,  1745,  1746,  1750,  1751,  1756,
    1761,  1769,  1770,  1771,  1772,  1773,  1774,  1775,  1776,  1777,
    1779,  1781,  1783,  1785,  1787,  1789,  1791,  1796,  1797,  1800,
    1801,  1802,  1805,  1806,  1807,  1808,  1819,  1820,  1824,  1825,
    1826,  1830,  1831,  1832,  1840,  1841,  1842,  1843,  1847,  1848,
    1849,  1850,  1851,  1852,  1853,  1854,  1855,  1856,  1860,  1868,
    1869,  1873,  1874,  1875,  1876,  1877,  1878,  1879,  1880,  1881,
    1882,  1883,  1884,  1885,  1886,  1887,  1888,  1889,  1890,  1891,
    1892,  1893,  1894,  1895,  1899,  1900,  1901,  1902,  1903,  1904,
    1908,  1909,  1910,  1911,  1915,  1916,  1917,  1918,  1923,  1924,
    1925,  1926,  1927,  1928,  1929
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
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL",
  "TASSIGNSR", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA",
  "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER",
  "TGREATEREQUAL", "THASH", "TIO", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD",
  "TNOT", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT",
  "TSHIFTRIGHT", "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR",
  "TRSBR", "TNOELSE", "TUMINUS", "TUPLUS", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_stmt", "access_control", "opt_prototype", "block_stmt",
  "stmt_ls", "only_ls", "opt_only_ls", "except_ls", "use_stmt",
  "require_stmt", "assignment_stmt", "opt_label_ident", "ident_def",
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
     178,   178,   178,   178,   178,   178,   178,   178,   179,   179,
     179,   179,   179,   179,   179,   180,   180,   180,   180,   180,
     180,   180,   180,   181,   181,   182,   182,   183,   183,   184,
     184,   184,   184,   184,   184,   184,   184,   185,   185,   186,
     186,   186,   186,   187,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   189,   190,   190,   190,
     190,   190,   190,   190,   190,   191,   192,   192,   192,   192,
     192,   192,   193,   193,   194,   194,   194,   195,   195,   196,
     197,   197,   198,   198,   199,   199,   199,   200,   200,   200,
     200,   201,   201,   201,   202,   202,   203,   203,   203,   204,
     204,   205,   206,   206,   206,   207,   207,   209,   210,   208,
     211,   211,   211,   211,   211,   213,   214,   212,   215,   215,
     215,   215,   215,   216,   216,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   219,   219,   220,   221,   221,   221,   222,
     222,   222,   222,   222,   223,   223,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   225,   225,   225,   225,   225,
     225,   226,   226,   227,   227,   227,   227,   227,   227,   228,
     228,   229,   229,   230,   230,   231,   231,   232,   232,   233,
     233,   234,   234,   235,   235,   235,   236,   236,   237,   237,
     237,   238,   238,   238,   238,   238,   239,   239,   239,   240,
     240,   241,   241,   242,   242,   242,   243,   243,   243,   244,
     244,   244,   245,   245,   245,   245,   245,   245,   245,   246,
     246,   246,   246,   246,   247,   247,   247,   247,   247,   248,
     248,   248,   248,   249,   249,   249,   250,   250,   250,   250,
     250,   251,   251,   251,   251,   251,   252,   252,   252,   252,
     253,   253,   254,   254,   254,   255,   255,   256,   256,   257,
     257,   258,   258,   258,   258,   259,   259,   260,   260,   260,
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     2,     1,     2,     3,     1,
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
       5,     5,     3,     1,     3,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     3,     3,     0,     1,     3,     4,
       5,     4,     6,     6,     0,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     0,     1,     1,     2,     1,
       1,     1,     1,     0,     1,     2,     1,     1,     1,     0,
       1,     1,     1,     1,     1,     2,     1,     0,     1,     2,
       2,     0,     2,     3,     4,     4,     2,     4,     0,     2,
       2,     4,     4,     4,     5,     4,     0,     1,     1,     1,
       3,     3,     5,     1,     1,     3,     1,     2,     3,     0,
       2,     2,     3,     2,     4,     3,     3,     4,     3,     0,
       2,     2,     2,     1,     0,     2,     2,     2,     1,     4,
       4,     6,     3,     0,     1,     1,     3,     4,     3,     4,
       6,     0,     2,     2,     2,     2,     1,     1,     3,     3,
       1,     3,     1,     1,     1,     3,     3,     0,     1,     1,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     4,     4,     4,     5,     2,     2,     2,
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
       3,     0,     0,     1,     0,   385,   488,   489,   490,   491,
     492,     0,   430,    69,    88,     0,   181,   430,     0,    93,
     317,    69,     0,     0,     0,     0,   191,   467,   318,     0,
       0,   316,     0,    92,     0,   201,    89,     0,   197,     0,
       0,     0,     0,   421,     0,   204,     0,     0,     0,     0,
     316,   316,    91,   182,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,    90,   183,
       0,     0,     0,     0,   540,   542,     0,   543,   544,   466,
       0,     0,   541,   538,   109,   539,     0,     0,     0,     4,
       0,     5,     9,    45,    10,    11,    12,    14,   386,    22,
      13,   110,   116,    15,    17,    16,    19,    20,    21,    18,
     115,     0,   113,     0,   456,     0,   117,   114,   118,     0,
     472,   452,   453,   389,   387,     0,     0,   457,   458,     0,
     388,     0,   473,   474,   475,   451,   391,   390,   454,   455,
       0,    38,    24,   397,     0,     0,   431,    70,     0,     0,
       0,     0,     0,     0,     0,     0,   456,   472,   387,   457,
     458,   399,   388,   473,   474,     0,   430,     0,     0,   155,
       0,   370,     0,   377,   468,   202,   491,   123,     0,     0,
     203,     0,     0,     0,     0,     0,   317,   318,   122,     0,
       0,   377,    71,    73,    84,    78,    86,    77,    87,    74,
      82,    76,    83,    81,    79,    80,    75,    85,     0,    72,
       0,     0,     0,     0,   319,     0,   106,    32,     0,   526,
     449,     0,   400,   527,     7,   377,   318,   112,   111,   296,
     367,     0,   366,     0,     0,   107,   471,     0,     0,    35,
       0,   402,   392,     0,   377,    36,   398,     0,   162,   158,
       0,   388,   162,   159,     0,   308,     0,   401,     0,   366,
       0,     0,   529,   465,   525,   528,   524,     0,    47,    50,
       0,     0,   372,     0,   374,     0,     0,   373,     0,   366,
       0,     0,     6,    46,     0,   184,     0,   282,   281,   205,
       0,     0,     0,     0,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   241,   248,   249,   250,   245,   247,
       0,     0,   243,   246,   244,   242,     0,   252,   251,     0,
       0,   377,   377,     0,     0,     0,    25,    26,     0,     0,
       0,     0,     0,    27,     0,     0,     0,    28,     0,    29,
       0,   385,   383,     0,   378,   379,   384,     0,     0,     0,
       0,   132,     0,     0,   131,     0,     0,     0,   138,     0,
       0,    56,   119,     0,   148,     0,     0,    30,   264,   198,
     323,     0,   324,   326,     0,   348,     0,   329,     0,     0,
     105,    31,    33,     0,   295,     0,    63,   469,   470,   108,
       0,    34,   377,     0,   169,   160,   156,   161,   157,     0,
     306,   303,    60,     0,    56,   125,    37,    49,    48,    51,
       0,   493,     0,     0,   484,     0,   486,     0,     0,     0,
       0,     0,     0,   497,     8,     0,     0,     0,     0,   275,
       0,     0,     0,     0,     0,   429,   521,   520,   523,   531,
     530,   535,   534,   517,   514,   515,   516,   461,   462,   460,
     504,   481,   483,   482,   463,   508,   519,   513,   511,   522,
     512,   510,   502,   507,   509,   518,   501,   505,   506,   503,
       0,     0,     0,     0,     0,     0,     0,   533,   532,   537,
     536,   441,   442,   444,   446,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,   430,   430,   128,
     304,   371,     0,     0,   394,     0,   305,   184,     0,     0,
       0,   405,     0,     0,     0,   411,     0,     0,     0,   139,
     539,    59,     0,    52,    57,     0,   147,     0,     0,     0,
     393,   269,   266,   267,   268,   272,   273,   274,   264,     0,
     257,     0,   265,   283,     0,   327,     0,    99,   101,   102,
      97,    98,    96,    95,   100,     0,   347,   346,   452,     0,
     321,   450,   320,   480,   369,   368,     0,     0,     0,   395,
       0,   163,   310,   452,     0,     0,     0,   494,   459,   485,
     375,   487,   376,     0,     0,   496,   144,   415,     0,   499,
     498,     0,     0,   185,     0,     0,   195,     0,   192,   279,
     276,   277,   280,   206,     0,     0,   312,   311,   313,   315,
      67,    68,    66,    65,    64,   478,   479,   443,   445,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   124,   381,   382,   380,     0,     0,
     146,     0,     0,   130,     0,   129,     0,     0,   136,     0,
       0,   134,     0,     0,   436,     0,   120,     0,   121,     0,
       0,   150,     0,   152,   270,   271,     0,   264,   255,     0,
     361,   284,   287,   286,   288,     0,   325,   328,   329,     0,
       0,   330,   331,   171,     0,     0,   170,   173,   396,     0,
     164,   167,     0,   307,    61,    62,     0,     0,     0,     0,
     145,     0,     0,     0,   316,   190,     0,   193,   189,   278,
     283,   253,    71,   238,    73,   236,    78,    77,    74,    76,
      79,    75,   217,   220,   218,   219,   230,   221,   234,   226,
     224,   237,   240,   225,   223,   228,   233,   235,   222,   227,
     231,   232,   229,   239,     0,   215,     0,   253,   253,   213,
     314,   434,   329,   472,   472,     0,     0,     0,     0,     0,
       0,     0,     0,   127,   126,     0,   133,     0,     0,   404,
       0,   403,     0,     0,   410,   137,     0,   409,   135,     0,
     435,    54,    53,   149,   420,   151,     0,   361,   258,     0,
       0,   329,   285,   301,   322,   352,     0,   495,     0,   175,
       0,     0,     0,   165,     0,   142,   417,     0,     0,   140,
     416,     0,   500,     0,    39,     0,   179,   318,   316,   316,
     177,   316,   187,   196,   194,     0,   264,   212,   216,     0,
       0,   208,   209,   438,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   316,   408,     0,     0,   414,     0,     0,
     437,     0,     0,   154,   329,   361,     0,   364,   363,   365,
     452,   297,   261,   259,     0,     0,     0,   350,   452,   176,
     174,     0,   168,     0,   143,     0,   141,    41,    40,   188,
     343,     0,   289,     0,   214,   253,   253,     0,     0,     0,
       0,     0,     0,   180,   178,     0,     0,     0,     0,    55,
     153,   260,   329,   353,     0,   298,   300,   299,   302,   293,
     294,   199,     0,   166,     0,     0,     0,   342,   341,   452,
     290,   301,   254,   210,   211,     0,     0,     0,     0,     0,
       0,   407,   406,   413,   412,   263,   262,   355,   356,   358,
     452,     0,   495,   452,   419,   418,     0,   333,     0,     0,
       0,   357,   359,   338,   336,   452,   495,   291,   207,   292,
     353,   337,   452,   360
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   269,    90,   546,    92,    93,   284,    94,
     270,   541,   545,   542,    95,    96,    97,   148,   212,   209,
      98,   576,   179,   217,    99,   100,   101,   102,   103,   104,
     650,   105,   106,   107,   415,   591,   712,   108,   109,   587,
     707,   110,   111,   447,   724,   112,   113,   617,   618,   156,
     210,   563,   115,   116,   449,   730,   623,   766,   767,   385,
     847,   389,   559,   560,   561,   562,   624,   289,   695,   941,
     978,   931,   230,   926,   882,   885,   117,   256,   420,   118,
     119,   213,   214,   393,   394,   580,   938,   902,   397,   577,
     958,   879,   811,   271,   170,   275,   276,   363,   364,   365,
     157,   121,   122,   123,   158,   125,   145,   146,   505,   379,
     673,   506,   507,   126,   159,   160,   129,   180,   366,   162,
     131,   163,   164,   134,   135,   280,   136,   137,   138,   139,
     140
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -862
static const yytype_int16 yypact[] =
{
    -862,   146,  2839,  -862,   120,  -862,  -862,  -862,  -862,  -862,
    -862,  4055,    11,   162,  -862,  9194,  -862,    11,  9194,  -862,
     113,   162,  4055,  9194,  4055,   201,  -862,  9194,  6186,  7637,
    9194,  7789,  9194,  -862,   202,  -862,  -862, 15716,  -862,  2164,
    8328,  9194,  9194,  -862,  9194,  -862,  9194,  9194,   273,   224,
     769,   918,  -862,  -862,  8457,  7052,  9194,  8328,  9194,  9194,
     231,  -862,   256,  4055,  9194,  9323,  9323, 15716,  -862,  -862,
    9194,  8457,  9194,  9194,  -862,  -862,  9194,  -862,  -862, 11258,
    9194,  9194,  -862,  9194,  -862,  -862,  3143,  6467,  8457,  -862,
    3903,  -862,  -862,   255,  -862,  -862,  -862,  -862,  -862,  -862,
    -862,  -862,  -862,  -862,  -862,  -862,  -862,  -862,  -862,  -862,
    -862, 15716,  -862, 15716,   288,    49,  -862,  -862,  -862,   181,
     200,  -862,  -862,  -862,   261,   271,   282,   291,   294, 15375,
    2043,   -40,   304,   307,  -862,  -862,  -862,  -862,  -862,  -862,
     203,  -862,  -862, 15375,   308,  4055,  -862,  -862,   319,  9194,
    9194,  9194,  9194,  9194,  8457,  8457,    55,  -862,  -862,  -862,
    -862,   338,   309,  -862,  -862,   321, 13585, 15716,   322,  -862,
      36, 15375,   374,  7204, 15375,  -862,   174,  -862, 15716,   189,
    -862, 15716,   324,    29, 13176, 13140,  -862,  -862,  -862, 13532,
   12422,  7204,  -862,  -862,  -862,  -862,  -862,  -862,  -862,  -862,
    -862,  -862,  -862,  -862,  -862,  -862,  -862,  -862,  4055,  -862,
     327,  1134,    26,    33,  -862,  4055,  -862,  -862, 13310,   320,
     338, 13310,   338,   320,  -862,  7204,  7918,  -862,  -862, 15716,
    -862,   111, 15375,  9194,  9194,  -862, 15375,   329, 13444,  -862,
   13310,   338, 15375,   328,  7204,  -862, 15375, 13867,  -862,  -862,
   13898,  2462,  -862,  -862, 14032,   379,   341,   338,   116, 13673,
   13310, 14063,    97,   943,   320,    97,   320,   191,  -862,  -862,
    3295,   -15,  -862,  9194,  -862,   -33,    31,  -862,    47, 14112,
     -23,   480,  -862,  -862,   438,   367,   342,  -862,  -862,  -862,
    1895,  2164,  2164,  2164,  -862,  9194,  9194,  9194,  9194,  8609,
    8609,  9194,  9194,  9194,  9194,  9452,  9194,    70, 11258,  9194,
    9194,  9194,  9194,  9194,  9194,  9194,  9194,  9194,  9194,  9194,
    9194,  9194,  9194,  9194,  -862,  -862,  -862,  -862,  -862,  -862,
    8047,  8047,  -862,  -862,  -862,  -862,  8047,  -862,  -862,  8047,
    8047,  7204,  7204,  8609,  8609,  6923,  -862,  -862, 13726, 13814,
   14197,   -31,  3447,  -862,  8609,    29,   350,  -862,  9194,  -862,
    9194,   392,  -862,   345,   382,  -862,  -862,   365, 15716,   472,
    8457,  -862,  4207,  8609,  -862,  4359,  8609,   363,  -862,    29,
    9581,  9194,  -862,  4055,   485,  9194,   371,  -862,   193,  -862,
    -862,  1134,  -862,   393,   381,  -862,  9710,   415,  9194,  2164,
    -862,  -862,  -862,   385,  -862,  8457,  -862, 15375, 15375,  -862,
      25,  -862,  7204,   387,  -862,   517,  -862,   517,  -862,  9839,
     414,  -862,  -862,  9581,  9194,  -862,  -862,  -862,  -862,  -862,
    8176,  -862, 12281,  6619,  -862,  6771,  -862,  8609,  5727,  2991,
     388,  9194,  6033,  -862,  -862, 15716,  8457,   391, 15699,   254,
    2164,   128,   190,   233,   243, 14246, 15492, 15492,   287,  -862,
     287,  -862,   287, 15560,   610,   406,   599, 11600, 11600,   338,
      97,  -862,  -862,  -862,   943,  1906,   287,  2393,  2393, 15492,
    2393,  2393,  1263,    97,  1906, 15522,  1263,   320,   320,    97,
     401,   402,   404,   405,   410,   398,   403,  -862,   287,  -862,
     287,    45,  -862,  -862,  -862,    67,  -862,   466, 15443,   386,
    9968,  8609, 10097,  8609,  9194,  8609, 11788,    11, 14286,  -862,
    -862, 15375, 14331,  7356,  -862,  7204,  -862,   367,  9194,    85,
    9194, 15375,    35, 13355,  9194, 15375,     3, 13221,  6923,  -862,
     420,   441,   425, 14420,   441,   426,   541, 14473,  4055, 13391,
    -862,    60,  -862,  -862,  -862,  -862,  -862,  -862,   818,    94,
    -862,  2275,  -862,   347,   422,  1134,    26,  9194,   201,   202,
    9194,  9194,  9194,  9194,  9194,  5575,  -862,  -862,   300,  7508,
    -862, 15375,  -862,  -862,  -862, 15375,   428,    38,   423,  -862,
    2053,  -862,  -862,   258, 15716,   432,   435,  -862,  -862,  -862,
    -862,  -862,  -862,   -34, 11999,  -862,  -862, 15375,  4055, 15375,
    -862, 14554,   431,   457,  2564,   443,   491,    75,  -862,   524,
    -862,  -862,  -862,  -862,  2626,   270,  -862,  -862,  -862,  -862,
    -862,  -862,  -862,  -862,  -862,  -862,  -862,  -862,  -862,  6923,
    -862,    24,  8609,  8609,  9194,   570, 14607,  9194,   571, 14638,
     447, 12067,    29,    29,  -862,  -862,  -862,  -862,   453, 13310,
    -862, 12495,  4511,  -862,  4663,  -862, 12568,  4815,  -862,    29,
    4967,  -862,    29,   115,  -862,  9194,  -862,  9194,  -862,  4055,
    9194,  -862,  4055,   573,  -862,  -862, 15716,  1014,  -862,  1134,
     481,   534,  -862,  -862,  -862,    77,  -862,  -862,   415,   454,
      41,  -862,  -862,  -862,  5119,  8457,  -862,  -862,  -862, 15716,
    -862,   484,   321,  -862,  -862,  -862,  5271,   456,  5423,   458,
    -862,  9194,  3599,   463,   555,  -862,  9194, 15716,  -862,  -862,
     347,   464,   490,  -862,   493,  -862,   496,   498,   503,   508,
     510,   512,  -862, 15716,  -862,  -862,  -862,  -862,  -862,  -862,
    -862,  -862,  -862,  -862,  -862,  -862,  -862,  -862,  -862,  -862,
    -862,  -862,  -862,  -862,  9194,  -862,   513,   464,   464,  -862,
    -862,  -862,   415,   117,   171, 14772, 10226, 10355, 14803, 10484,
   10613, 10742, 10871,  -862,  -862, 11854,  -862,  4055,  9194, 15375,
    9194, 15375,  4055,  9194, 15375,  -862,  9194, 15375,  -862,  6923,
    -862, 14852, 15375,  -862, 15375,   587,  4055,   481,  -862,   488,
    8738,   -12,  -862,   544,  -862,  -862,  8609, 11694,  4055,  -862,
      20,   494,  9194,  -862,  9194,  -862, 15375,  4055,  9194,  -862,
   15375,  4055, 15375,   303,  -862,  3751,  -862,  6338,   317,   317,
    -862,   764,  -862, 15375,  -862,    80,   193,  -862,  -862, 12349,
   11916,  -862,  -862,  -862,  9194,  9194,  9194,  9194,  9194,  9194,
    9194,  9194,   495,  1470, 14473, 12641, 12714, 14473, 12787, 12860,
    -862,  9194,  4055,  -862,   415,   481,  5880,  -862,  -862,  -862,
     106,  8457,  -862,  -862,  9194,     4, 12140,  -862,   822,   374,
    -862,   321, 15375, 12933,  -862, 13006,  -862,  -862,  -862,  -862,
    -862, 11000,   557,   232,  -862,   464,   464, 14937, 15010, 15083,
   15156, 15229, 15302,  -862,  -862,  4055,  4055,  4055,  4055, 15375,
    -862,  -862,   -12,  8890,    57,  -862,  -862, 15375, 15375,  -862,
    -862,  -862, 10871,  -862,  4055,  4055,  2407,  -862,  -862,    99,
    -862,   544,  -862,  -862,  -862,  9194,  9194,  9194,  9194,  9194,
    9194, 14473, 14473, 14473, 14473,  -862,  -862,  -862,  -862,  -862,
     268,  8609, 11387,   858, 14473, 14473,   497, 11129,    58,   -37,
   12208,  -862,  -862,  -862,  -862,   131, 11481,  -862,  -862,  -862,
    9042,  -862,   176,  -862
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -862,  -862,  -862,    -1,  -374,  2000,  -862,  -862,  -862,  1724,
     -73,  -297,   230,   234,  -862,  -862,   301,   637,   801,  -862,
    -862,  -782,  -862,  1180,  -835,  -673,   -45,  -862,  -862,  -862,
      56,  -862,  -862,  -862,   408,  -862,   -48,  -862,  -862,  -862,
    -862,  -862,   489,   136,  -118,  -862,  -862,  -862,   -57,   891,
    -862,  -862,  -862,  -862,  -862,  -862,  -862,  -862,  -178,  -128,
    -711,  -862,  -172,   -10,   118,  -862,  -862,  -862,   -51,  -862,
    -862,  -288,   250,  -862,  -238,  -252,  -862,  -156,  -862,   659,
    -862,  -251,   293,  -862,  -367,  -683,  -861,  -862,  -532,  -410,
    -730,  -836,  -716,     9,  -862,  -187,  -862,  -154,  -862,   168,
     380,  -289,  -862,  -862,   938,  -862,    -9,  -862,  -862,  -265,
    -862,  -535,  -862,  -862,   985,  1032,   -11,   668,    50,   520,
    -862,  1254,  1395,  -862,  -862,  -862,  -862,  -862,  -862,  -862,
    -322
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -496
static const yytype_int16 yytable[] =
{
     143,    89,   340,   674,   161,   227,   228,   166,   165,   592,
     515,   356,   171,   440,   558,   814,   174,   174,   184,   185,
     189,   190,   367,   509,   564,   395,   586,   395,   877,   218,
     219,   220,   667,   221,   698,   222,   223,   386,    55,   451,
     452,   453,   454,   232,   236,   238,   240,   241,   242,   215,
     930,   842,   246,   247,   250,   254,   851,   852,   215,   257,
     259,   260,   261,   231,   662,   262,   377,  -344,   263,   264,
     265,   403,   266,   471,   398,   232,   236,   279,   395,    55,
     258,   900,   816,  -172,   544,   183,   637,   959,   437,   853,
     413,   874,   433,   579,   438,   287,   704,   278,   961,   515,
     472,   684,   442,   377,   771,   237,   974,   578,   977,   772,
     430,   144,   341,   352,   342,   981,   288,   881,   638,   937,
     434,   716,  -172,   340,   516,   298,   972,   544,   883,  -344,
     593,  -344,   443,   685,   930,   705,   431,   277,   143,   348,
     349,   350,   246,   232,   279,   405,     3,   377,   396,  -344,
     396,  -344,  -344,   352,   352,  -339,   435,   355,   399,   922,
     473,   358,   236,   813,   351,   147,   438,  -339,   899,   299,
     352,  -344,   438,   300,  -340,  -344,  -172,  -344,  -339,   352,
     236,   359,   438,   438,   436,   352,  -340,   495,   496,   706,
     842,   921,   639,  -104,   943,   944,   817,  -340,   697,   625,
     727,   396,   439,   167,   901,   290,  -332,  -477,    16,  -477,
     405,  -362,   962,   976,   236,   174,   509,   551,  -332,   687,
     640,   305,   407,   408,   307,  -339,   728,  -344,   310,  -332,
    -339,  -362,   971,   236,   552,  -362,   405,   553,   660,   956,
     799,   405,  -440,   291,  -340,  -104,   600,   688,   602,  -340,
     983,  -334,   554,   399,   292,   555,   406,    48,   588,  -362,
      53,   422,   432,  -334,   870,   141,   556,  -104,   800,   429,
    -440,   669,   672,   626,  -334,   343,  -332,   293,   619,   344,
     224,  -332,    69,   557,   455,   456,   457,   458,   460,   462,
     463,   464,   465,   466,   469,   470,  -439,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   558,   243,   399,   620,   509,  -256,   236,
     236,  -334,   809,   283,  -439,   236,  -334,   621,   236,   236,
     236,   236,   498,   500,   508,   627,   141,   274,   717,   719,
     186,  -345,   427,   518,   622,  -423,  -256,   521,   298,   522,
     841,    27,   226,   173,   191,   459,   461,   687,   399,   232,
      35,   531,   533,  -200,   535,   537,   249,   253,   399,   543,
     543,   691,   547,  -354,   549,  -345,   225,    45,   628,   529,
     490,   491,   120,  -309,  -200,   942,   492,   581,   629,   493,
     494,   120,   299,  -354,   585,   399,   300,  -354,  -345,   497,
     499,   236,   120,  -309,   120,  -345,  -422,   887,   244,   692,
     517,   305,   543,   543,   307,   770,   294,   295,   310,   585,
     693,  -354,   236,   362,   236,  -345,   604,   585,   607,   532,
     609,   611,   536,  -426,   298,   232,  -427,   694,   713,  -428,
    -477,   362,  -477,   120,   305,  -345,   306,   307,   141,  -425,
    -345,   310,  -424,  -345,   897,   613,   161,   257,   643,   317,
     345,  -476,   344,  -476,   347,   307,   120,   357,   323,     5,
     120,   352,   558,   360,   409,   362,   370,   509,   299,   388,
     412,    14,   300,   277,   419,   277,   421,   444,    19,   841,
     445,   446,   448,   603,   362,   520,   768,   523,   524,   531,
     646,   535,   649,   547,   651,   607,    33,   525,   652,   653,
     526,    36,   236,   528,   236,   538,   548,   659,   565,   661,
     579,   880,   130,   666,   550,   120,   302,   508,   888,   304,
     305,   130,   306,   307,   566,   590,    52,   310,   583,   594,
     589,   614,   130,   608,   130,   317,   630,   631,    61,   632,
     633,   635,   321,   322,   323,   634,   161,    68,   636,   222,
     241,   242,   246,   257,   279,   -58,   675,   645,   236,   648,
     676,   678,   679,   656,    16,   696,   708,   714,    20,   703,
     715,   722,   405,   130,   700,   251,   251,    26,   120,    27,
     837,   362,   362,    31,   725,   120,   726,   729,    35,   776,
     779,  -200,   781,   785,   806,   810,   130,   812,   822,   815,
     130,   827,   939,   831,   836,    45,   846,  -385,   872,    48,
     -88,   838,  -200,   -93,   839,   -92,    53,   298,   508,   702,
     -89,   460,   498,   775,   960,   -91,   778,   -94,   298,   -90,
     850,   875,   884,   963,   940,    67,   913,   891,    69,   835,
     120,   789,   973,   791,   596,   584,   794,   595,   168,   797,
     417,   821,   362,   658,   801,   130,   802,   863,   368,   804,
     844,   299,   905,   960,   903,   300,   686,   808,   975,   845,
     584,   979,   299,   274,   955,   274,   300,   982,   584,   969,
     188,   960,   582,   657,   232,   175,     0,     0,   459,   497,
      84,     0,     0,     0,     0,   826,   840,   830,     0,     0,
     832,     0,     0,     0,   820,   843,     0,     0,     0,   302,
       0,     0,   906,   305,     0,   306,   307,     0,   130,     0,
     310,     0,   120,     0,   305,   130,   306,   307,   317,     0,
       0,   310,     0,     0,     0,   321,   322,   323,     0,   317,
       0,     0,   120,   849,     0,   120,   321,   322,   323,     0,
       0,     0,     0,   120,     0,   789,   791,     0,   794,   797,
     826,   830,     0,   655,     0,   362,   864,   865,     0,   866,
       0,   867,   868,    16,     0,   869,     0,    20,   508,     0,
     130,     0,   186,   227,   228,     0,    26,     0,    27,   837,
       0,     0,    31,    27,   226,   886,   607,    35,     0,     0,
    -200,   892,    35,   893,     0,  -200,     0,   895,     0,   120,
       0,   -44,     0,     0,    45,     0,   174,     0,   281,    45,
     838,  -200,     0,   839,   429,    53,  -200,   -44,   208,     0,
       0,     0,   551,   864,   907,   908,   867,   909,   910,   911,
     912,     0,     0,     0,    67,     0,     0,    69,     0,   552,
     919,     0,   553,  -349,     0,   279,     0,     0,   255,     0,
     927,     0,   130,   928,     0,     0,     0,   554,     0,     0,
     555,     0,   281,     0,     0,   924,     0,   641,     0,     0,
       0,   556,   130,   114,     0,   130,     0,  -349,     0,  -351,
       0,     0,   114,   130,   951,   952,   953,   954,   557,    84,
       0,     0,   285,   114,   286,   114,     0,     0,     0,     0,
    -349,   830,     0,   964,   965,   279,     0,  -349,   120,     0,
       0,     0,     0,  -351,   951,   952,   953,   954,   964,   965,
     124,   186,     0,     0,     0,   968,     0,  -349,     0,   124,
     970,   607,    27,   226,   114,     0,  -351,     0,     0,   130,
     124,    35,   124,  -351,  -200,   607,     0,  -349,   255,   830,
     -43,   298,  -349,     0,     0,  -349,     0,   114,    45,   255,
       0,   114,   369,  -351,     0,  -200,   -43,   127,   120,     0,
       0,     0,     0,     0,     0,     0,   127,     0,     0,     0,
       0,   124,     0,  -351,   769,     0,     0,   127,  -351,   127,
       0,  -351,   392,     0,     0,   299,     0,     0,     0,   300,
       0,     0,   773,   774,   124,     0,     0,     0,   124,     0,
     404,     0,     0,     0,   128,     0,   114,     0,   551,     0,
       0,     0,   120,   128,   120,     0,     0,   120,   127,     0,
     120,     0,     0,     0,   128,   552,   128,     0,   553,   120,
     878,     0,   120,   302,     0,   303,   304,   305,   130,   306,
     307,   127,     0,   554,   310,   127,   555,     0,    48,     0,
       0,   316,   317,   124,   120,     0,   320,   556,     0,   321,
     322,   323,     0,     0,     0,   128,   120,     0,   120,   114,
       0,     0,   120,     0,   557,     0,   114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   128,     0,
       0,     0,   128,     0,     0,     0,     0,     0,   130,     0,
     127,   925,     0,     0,     0,     0,     0,   192,     0,     0,
       0,     0,     0,     0,     0,     0,   124,     0,     0,   193,
     194,     0,     0,   124,     0,     0,   195,     0,     0,     0,
       0,   114,     0,     0,   196,     0,     0,   120,     0,   527,
       0,     0,   120,   957,   197,     0,   198,   128,     0,   199,
       0,     0,   130,     0,   130,     0,   120,   130,     0,     0,
     130,     0,   392,   127,     0,   200,     0,     0,   120,   130,
     127,     0,   130,     0,   201,     0,     0,   120,   124,     0,
       0,   120,   957,   202,   203,   120,   204,     0,   205,     0,
       0,     0,     0,     0,   130,   206,   390,     0,   207,     0,
     957,     0,     0,     0,     0,     0,   130,   239,   130,     0,
     128,     0,   130,   114,     0,     0,   612,   128,     0,   616,
       0,     0,   120,     0,     0,   127,   132,     0,     0,     0,
       0,     0,     0,   114,     0,   132,   114,     0,     0,     0,
       0,     0,     0,     0,   114,     0,   132,     0,   132,     0,
       0,     0,     0,     0,     0,     0,   391,     0,     0,     0,
     124,   298,     0,     0,     0,   120,   120,   120,   120,     0,
       0,     0,   128,     0,     0,     0,     0,   130,     0,     0,
     124,     0,   130,   124,   120,   120,     0,   132,     0,     0,
       0,   124,     0,     0,     0,     0,   130,     0,     0,     0,
     114,     0,     0,     0,     0,   299,     0,   127,   130,   300,
     132,     0,     0,     0,   132,     0,     0,   130,     0,     0,
       0,   130,     0,     0,     0,   130,     0,   127,     0,     0,
     127,     0,   690,   371,   374,   378,   392,     0,   127,     0,
       0,     0,     0,     0,     0,     0,     0,   124,     0,     0,
       0,     0,     0,   302,   128,   303,   304,   305,     0,   306,
     307,   711,   130,     0,   310,   255,     0,   133,   401,   132,
       0,   402,   317,     0,   128,     0,   133,   128,     0,   321,
     322,   323,     0,     0,     0,   128,     0,   133,     0,   133,
     411,     0,     0,     0,   127,   765,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   130,   130,   130,   130,   114,
     425,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   130,   130,     0,     0,   133,     0,
       0,     0,   132,     0,     0,     0,     0,     0,     0,   132,
       0,   128,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   133,     0,     0,     0,   133,   124,   807,     0,    16,
     392,     0,     0,    20,     0,     0,     0,     0,     0,   114,
       0,     0,    26,     0,    27,   837,     0,     0,    31,     0,
     711,     0,     0,    35,     0,     0,  -200,     0,     0,     0,
       0,     0,     0,     0,   132,     0,     0,     0,   616,     0,
      45,     0,     0,   127,    48,   519,   838,  -200,     0,   839,
     133,    53,     0,     0,   848,     0,   124,     0,     0,     0,
       0,     0,     0,   114,     0,   114,     0,     0,   114,   539,
      67,   114,     0,    69,     0,     0,     0,     0,     0,     0,
     114,     0,     0,   114,     0,     0,     0,     0,     0,     0,
     128,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   127,     0,   114,     0,     0,     0,     0,
     124,     0,   124,   133,     0,   124,   132,   114,   124,   114,
     133,     0,     0,   114,     0,    84,     0,   124,     0,     0,
     124,   914,     0,     0,     0,     0,   132,     0,     0,   132,
       0,     0,     0,     0,     0,     0,     0,   132,     0,     0,
     128,     0,   124,     0,     0,     0,     0,   127,     0,   127,
       0,   765,   127,     0,   124,   127,   124,     0,     0,     0,
     124,     0,     0,     0,   127,   133,     0,   127,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   114,     0,
       0,     0,     0,   114,     0,     0,     0,     0,     0,   127,
       0,     0,     0,   132,   128,     0,   128,   114,     0,   128,
       0,   127,   128,   127,     0,     0,     0,   127,     0,   114,
       0,   128,   663,   665,   128,     0,   668,   671,   114,     0,
       0,     0,   114,     0,     0,   124,   114,     0,     0,     0,
     124,     0,     0,     0,     0,     0,   128,     0,     0,     0,
       0,     0,     0,     0,   124,     0,     0,   133,   128,     0,
     128,     0,     0,     0,   128,     0,   124,     0,     0,     0,
       0,     0,     0,   114,   216,   124,     0,   133,     0,   124,
     133,     0,   127,   124,     0,     0,     0,   127,   133,     0,
       0,   216,     0,     0,     0,     0,     0,     0,     0,   248,
     252,   127,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   132,   127,     0,     0,   114,   114,   114,   114,
     124,     0,   127,     0,     0,     0,   127,     0,     0,   128,
     127,     0,     0,     0,   128,   114,   114,     0,     0,     0,
       0,     0,   783,   784,   133,     0,     0,     0,   128,   786,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   795,
     128,     0,   798,   124,   124,   124,   124,   127,     0,   128,
       0,     0,   132,   128,     0,     0,     0,   128,     0,     0,
       0,     0,   124,   124,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,     0,     0,     0,     0,     0,     0,     0,   192,     0,
     127,   127,   127,   127,   128,     0,     0,   216,   216,   216,
     193,   194,     0,     0,   384,     0,   132,   195,   132,   127,
     127,   132,     0,     0,   132,   196,     0,     0,     0,     0,
       0,     0,     0,   132,   298,   197,   132,   198,     0,     0,
     199,     0,   216,   133,     0,   216,     0,   128,   128,   128,
     128,     0,     0,     0,     0,     0,   200,     0,   132,     0,
       0,     0,     0,     0,   216,   201,   128,   128,   450,     0,
     132,     0,   132,     0,   202,   203,   132,   204,   299,   205,
       0,     0,   300,     0,   216,     0,   206,     0,     0,   207,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     890,     0,    91,   133,     0,     0,     0,     0,     0,     0,
       0,   142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   169,     0,   172,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,     0,   310,   311,   312,
       0,   132,   314,   315,   316,   317,   132,   211,     0,   320,
       0,     0,   321,   322,   323,     0,   192,   133,     0,   133,
     132,     0,   133,   245,     0,   133,     0,     0,   193,   194,
       0,     0,   132,     0,   133,   195,     0,   133,     0,   216,
       0,   132,     0,   196,     0,   132,    91,     0,     0,   132,
     282,     0,     0,   197,     0,   198,     0,     0,   199,   133,
       0,     0,     0,   216,     0,     0,     0,     0,     0,     0,
       0,   133,     0,   133,   200,     0,     0,   133,     0,     0,
       0,     0,     0,   201,     0,     0,   132,     0,     0,     0,
       0,     0,   202,   203,     0,   204,     0,   205,     0,     0,
       0,     0,     0,     0,   206,   346,     0,   207,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,     0,     0,     0,     0,   192,     0,   132,
     132,   132,   132,     0,     0,     0,     0,     0,  -447,   193,
     194,     0,   133,     0,     0,     0,   195,   133,   132,   132,
       0,     0,   339,     0,   196,  -476,     0,  -476,     0,     0,
       0,   133,     0,   352,   197,   709,   198,     0,   387,   199,
       0,     0,     0,   133,     0,   400,     0,     0,     0,     0,
       0,     0,   133,     0,     0,   200,   133,     0,     0,     0,
     133,     0,     0,     0,   201,     0,     0,     0,     0,     0,
       0,     0,     0,   202,   203,     0,   204,     0,   205,     0,
       0,     0,     0,     0,     0,   206,   216,   216,   207,     0,
     216,   216,     0,     0,     0,     0,     0,   133,     0,     0,
      91,     0,     0,   683,     0,     0,     0,     0,   192,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     193,   194,     0,     0,     0,     0,     0,   195,     0,     0,
       0,     0,     0,     0,     0,   196,     0,     0,     0,     0,
     133,   133,   133,   133,   710,   197,   211,   198,     0,     0,
     199,     0,     0,     0,     0,     0,     0,     0,     0,   133,
     133,     0,     0,     0,     0,     0,   200,     0,     0,     0,
       0,     0,     0,     0,     0,   201,     0,     0,     0,     0,
       0,     0,    91,     0,   202,   203,     0,   204,     0,   205,
       0,     0,     0,     0,     0,     0,   206,     0,     0,   207,
       0,     0,   400,     0,     0,   400,   216,   216,     0,     0,
       0,     0,     0,   216,     0,   384,     0,     0,     0,     0,
     384,     0,     0,   216,     0,     0,   216,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   966,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   149,
       0,   298,    14,    15,     0,     0,     0,   689,     0,    19,
       0,     0,     0,     0,     0,     0,   823,    25,     0,   606,
       0,     0,     0,   150,   151,     0,   152,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,     0,
      42,    43,     0,     0,     0,   299,     0,     0,    46,   300,
      47,     0,    49,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,   153,     0,     0,     0,     0,     0,     0,    68,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,   302,     0,   303,   304,   305,     0,   306,
     307,   308,     0,     0,   310,     0,     0,    75,    76,    77,
      78,   316,   317,     0,     0,    79,   320,     0,     0,   321,
     322,   323,     0,     0,   216,    80,     0,    81,   681,    82,
      83,   229,     0,     0,     0,    85,     0,   154,     0,    87,
       0,   155,   967,     0,     0,   723,     0,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,     0,  -186,     0,     0,     0,  -186,  -186,   384,
     384,     0,   384,   384,     0,     0,  -186,     0,  -186,  -186,
       0,     0,  -186,     0,     0,     0,     0,  -186,   720,   929,
    -186,   339,     0,     0,  -476,   933,  -476,   384,     0,   384,
       0,     0,     0,     0,  -186,     0,  -186,   731,  -186,   732,
    -186,  -186,     0,  -186,     0,  -186,   733,  -186,     0,     0,
       0,   734,   194,   735,     0,     0,     0,     0,   736,     0,
       0,     0,     0,     0,  -186,     0,   196,  -186,     0,     0,
    -186,     0,   400,     0,   400,     0,   737,   400,   198,     0,
     400,   738,     0,     0,     0,     0,     0,     0,     0,   803,
       0,     0,   805,     0,     0,     0,     0,   200,     0,     0,
       0,     0,     0,   929,     0,     0,   739,     0,     0,     0,
       0,     0,     0,     0,   819,   202,   203,     0,   740,  -186,
     205,     0,     0,     0,     0,  -186,   825,   741,   829,     0,
     207,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,   324,   325,   326,   327,   328,   329,     0,     0,   332,
     333,   334,   335,     0,   337,   338,   742,   743,   744,   745,
       0,     0,   746,     0,     0,     0,   747,   748,   749,   750,
     751,   752,   753,   754,   755,   756,   757,   758,     0,   759,
       0,     0,   760,   761,   762,   763,     0,     0,   764,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   873,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   889,     0,
       0,     0,     0,     0,     0,     0,     0,   894,     0,     0,
       0,   896,     0,     0,     0,    91,     0,     0,     0,    -2,
       4,     0,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,  -316,    21,    22,    23,     0,    24,    25,
       0,    26,   920,    27,    28,    29,    30,    31,    32,    33,
       0,    34,    35,     0,    36,  -200,    37,    38,    39,    40,
      41,   -42,    42,    43,     0,    44,     0,     0,     0,    45,
      46,  -316,    47,    48,    49,    50,  -200,   -42,    51,    52,
      53,     0,  -316,    54,    55,     0,    56,    57,    58,    59,
      60,    61,    62,    63,     0,    64,     0,    65,    66,    67,
      68,     0,    69,    70,    71,  -316,     0,     0,    72,     0,
      73,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    80,     0,    81,
       0,    82,    83,     0,    84,     0,     0,    85,     0,    86,
       0,    87,     4,    88,     5,     6,     7,     8,     9,    10,
       0,  -495,     0,    11,    12,    13,    14,    15,  -495,     0,
      16,    17,    18,    19,    20,  -316,    21,    22,    23,  -495,
      24,    25,  -495,    26,     0,    27,    28,    29,    30,    31,
      32,    33,     0,    34,    35,     0,    36,  -200,    37,    38,
      39,    40,    41,   -42,    42,    43,     0,    44,     0,     0,
       0,    45,    46,  -316,    47,     0,    49,    50,  -200,   -42,
      51,    52,    53,  -495,  -316,    54,    55,  -495,    56,    57,
      58,    59,    60,    61,    62,    63,     0,    64,     0,    65,
      66,    67,    68,     0,    69,    70,    71,  -316,     0,     0,
      72,     0,    73,     0,     0,  -495,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -495,    76,  -495,  -495,  -495,  -495,  -495,  -495,  -495,
       0,  -495,  -495,  -495,  -495,  -495,     0,  -495,  -495,  -495,
    -495,    81,  -495,  -495,  -495,     0,    84,  -495,  -495,  -495,
       0,    86,  -495,    87,   267,    88,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,  -316,    21,    22,
      23,     0,    24,    25,     0,    26,     0,    27,    28,    29,
      30,    31,    32,    33,     0,    34,    35,     0,    36,  -200,
      37,    38,    39,    40,    41,   -42,    42,    43,     0,    44,
       0,     0,     0,    45,    46,  -316,    47,    48,    49,    50,
    -200,   -42,    51,    52,    53,     0,  -316,    54,    55,     0,
      56,    57,    58,    59,    60,    61,    62,    63,     0,    64,
       0,    65,    66,    67,    68,     0,    69,    70,    71,  -316,
       0,     0,    72,     0,    73,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,    81,     0,    82,    83,   229,    84,     0,
       0,    85,     0,    86,   268,    87,     4,    88,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,  -316,
      21,    22,    23,     0,    24,    25,     0,    26,     0,    27,
      28,    29,    30,    31,    32,    33,     0,    34,    35,     0,
      36,  -200,    37,    38,    39,    40,    41,   -42,    42,    43,
       0,    44,     0,     0,     0,    45,    46,  -316,    47,    48,
      49,    50,  -200,   -42,    51,    52,    53,     0,  -316,    54,
      55,     0,    56,    57,    58,    59,    60,    61,    62,    63,
       0,    64,     0,    65,    66,    67,    68,     0,    69,    70,
      71,  -316,     0,     0,    72,     0,    73,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    80,     0,    81,     0,    82,    83,     0,
      84,     0,     0,    85,     0,    86,   428,    87,   267,    88,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,  -316,    21,    22,    23,     0,    24,    25,     0,    26,
       0,    27,    28,    29,    30,    31,    32,    33,     0,    34,
      35,     0,    36,  -200,    37,    38,    39,    40,    41,   -42,
      42,    43,     0,    44,     0,     0,     0,    45,    46,  -316,
      47,    48,    49,    50,  -200,   -42,    51,    52,    53,     0,
    -316,    54,    55,     0,    56,    57,    58,    59,    60,    61,
      62,    63,     0,    64,     0,    65,    66,    67,    68,     0,
      69,    70,    71,  -316,     0,     0,    72,     0,    73,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    81,     0,    82,
      83,     0,    84,     0,     0,    85,     0,    86,   268,    87,
     833,    88,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,  -316,    21,    22,    23,     0,    24,    25,
       0,    26,     0,    27,    28,    29,    30,    31,    32,    33,
       0,    34,    35,     0,    36,  -200,    37,    38,    39,    40,
      41,   -42,    42,    43,     0,    44,     0,     0,     0,    45,
      46,  -316,    47,    48,    49,    50,  -200,   -42,    51,    52,
      53,     0,  -316,    54,    55,     0,    56,    57,    58,    59,
      60,    61,    62,    63,     0,    64,     0,    65,    66,    67,
      68,     0,    69,    70,    71,  -316,     0,     0,    72,     0,
      73,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    80,     0,    81,
       0,    82,    83,     0,    84,     0,     0,    85,     0,    86,
     834,    87,     4,    88,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,  -316,    21,    22,    23,     0,
      24,    25,     0,    26,     0,    27,    28,    29,    30,    31,
      32,    33,     0,    34,    35,     0,    36,  -200,    37,    38,
      39,    40,    41,   -42,    42,    43,     0,    44,     0,     0,
       0,    45,    46,  -316,    47,    48,    49,    50,  -200,   -42,
      51,    52,    53,     0,  -316,    54,    55,     0,    56,    57,
      58,    59,    60,    61,    62,    63,     0,    64,     0,    65,
      66,    67,    68,     0,    69,    70,    71,  -316,     0,     0,
      72,     0,    73,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    80,
       0,    81,     0,    82,    83,     0,    84,     0,     0,    85,
       0,    86,   898,    87,     4,    88,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,  -316,    21,    22,
      23,     0,    24,    25,     0,    26,     0,    27,    28,    29,
      30,    31,    32,    33,     0,    34,    35,     0,    36,  -200,
      37,    38,    39,    40,    41,   -42,    42,    43,     0,    44,
       0,     0,     0,    45,    46,  -316,    47,   281,    49,    50,
    -200,   -42,    51,    52,    53,     0,  -316,    54,    55,     0,
      56,    57,    58,    59,    60,    61,    62,    63,     0,    64,
       0,    65,    66,    67,    68,     0,    69,    70,    71,  -316,
       0,     0,    72,     0,    73,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,    81,     0,    82,    83,     0,    84,     0,
       0,    85,     0,    86,     0,    87,     4,    88,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,  -316,
      21,    22,    23,     0,    24,    25,     0,    26,     0,    27,
      28,    29,    30,    31,    32,    33,     0,    34,    35,     0,
      36,  -200,    37,    38,    39,    40,    41,   -42,    42,    43,
       0,    44,     0,     0,     0,    45,    46,  -316,    47,     0,
      49,    50,  -200,   -42,    51,    52,    53,     0,  -316,    54,
      55,     0,    56,    57,    58,    59,    60,    61,    62,    63,
       0,    64,     0,    65,    66,    67,    68,     0,    69,    70,
      71,  -316,     0,     0,    72,     0,    73,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    80,     0,    81,     0,    82,    83,     0,
      84,     0,     0,    85,     0,    86,     0,    87,     4,    88,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,  -316,    21,    22,    23,     0,    24,    25,     0,    26,
       0,    27,    28,    29,    30,    31,   530,    33,     0,    34,
      35,     0,    36,  -200,    37,    38,    39,    40,    41,   -42,
      42,    43,     0,    44,     0,     0,     0,    45,    46,  -316,
      47,     0,    49,    50,  -200,   -42,    51,    52,    53,     0,
    -316,    54,    55,     0,    56,    57,    58,    59,    60,    61,
      62,    63,     0,    64,     0,    65,    66,    67,    68,     0,
      69,    70,    71,  -316,     0,     0,    72,     0,    73,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    81,     0,    82,
      83,     0,    84,     0,     0,    85,     0,    86,     0,    87,
       4,    88,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,  -316,    21,    22,    23,     0,    24,    25,
       0,    26,     0,    27,    28,    29,    30,    31,   534,    33,
       0,    34,    35,     0,    36,  -200,    37,    38,    39,    40,
      41,   -42,    42,    43,     0,    44,     0,     0,     0,    45,
      46,  -316,    47,     0,    49,    50,  -200,   -42,    51,    52,
      53,     0,  -316,    54,    55,     0,    56,    57,    58,    59,
      60,    61,    62,    63,     0,    64,     0,    65,    66,    67,
      68,     0,    69,    70,    71,  -316,     0,     0,    72,     0,
      73,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    80,     0,    81,
       0,    82,    83,     0,    84,     0,     0,    85,     0,    86,
       0,    87,     4,    88,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,  -316,    21,    22,    23,     0,
      24,    25,     0,    26,     0,    27,    28,    29,    30,    31,
     788,    33,     0,    34,    35,     0,    36,  -200,    37,    38,
      39,    40,    41,   -42,    42,    43,     0,    44,     0,     0,
       0,    45,    46,  -316,    47,     0,    49,    50,  -200,   -42,
      51,    52,    53,     0,  -316,    54,    55,     0,    56,    57,
      58,    59,    60,    61,    62,    63,     0,    64,     0,    65,
      66,    67,    68,     0,    69,    70,    71,  -316,     0,     0,
      72,     0,    73,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    80,
       0,    81,     0,    82,    83,     0,    84,     0,     0,    85,
       0,    86,     0,    87,     4,    88,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,  -316,    21,    22,
      23,     0,    24,    25,     0,    26,     0,    27,    28,    29,
      30,    31,   790,    33,     0,    34,    35,     0,    36,  -200,
      37,    38,    39,    40,    41,   -42,    42,    43,     0,    44,
       0,     0,     0,    45,    46,  -316,    47,     0,    49,    50,
    -200,   -42,    51,    52,    53,     0,  -316,    54,    55,     0,
      56,    57,    58,    59,    60,    61,    62,    63,     0,    64,
       0,    65,    66,    67,    68,     0,    69,    70,    71,  -316,
       0,     0,    72,     0,    73,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,    81,     0,    82,    83,     0,    84,     0,
       0,    85,     0,    86,     0,    87,     4,    88,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,  -316,
      21,    22,    23,     0,    24,    25,     0,    26,     0,    27,
      28,    29,    30,    31,   793,    33,     0,    34,    35,     0,
      36,  -200,    37,    38,    39,    40,    41,   -42,    42,    43,
       0,    44,     0,     0,     0,    45,    46,  -316,    47,     0,
      49,    50,  -200,   -42,    51,    52,    53,     0,  -316,    54,
      55,     0,    56,    57,    58,    59,    60,    61,    62,    63,
       0,    64,     0,    65,    66,    67,    68,     0,    69,    70,
      71,  -316,     0,     0,    72,     0,    73,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    80,     0,    81,     0,    82,    83,     0,
      84,     0,     0,    85,     0,    86,     0,    87,     4,    88,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,  -316,    21,    22,    23,     0,    24,    25,     0,    26,
       0,    27,    28,    29,    30,    31,   796,    33,     0,    34,
      35,     0,    36,  -200,    37,    38,    39,    40,    41,   -42,
      42,    43,     0,    44,     0,     0,     0,    45,    46,  -316,
      47,     0,    49,    50,  -200,   -42,    51,    52,    53,     0,
    -316,    54,    55,     0,    56,    57,    58,    59,    60,    61,
      62,    63,     0,    64,     0,    65,    66,    67,    68,     0,
      69,    70,    71,  -316,     0,     0,    72,     0,    73,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    81,     0,    82,
      83,     0,    84,     0,     0,    85,     0,    86,     0,    87,
       4,    88,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,  -316,    21,    22,    23,     0,   818,    25,
       0,    26,     0,    27,    28,    29,    30,    31,    32,    33,
       0,    34,    35,     0,    36,  -200,    37,    38,    39,    40,
      41,   -42,    42,    43,     0,    44,     0,     0,     0,    45,
      46,  -316,    47,     0,    49,    50,  -200,   -42,    51,    52,
      53,     0,  -316,    54,    55,     0,    56,    57,    58,    59,
      60,    61,    62,    63,     0,    64,     0,    65,    66,    67,
      68,     0,    69,    70,    71,  -316,     0,     0,    72,     0,
      73,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    80,     0,    81,
       0,    82,    83,     0,    84,     0,     0,    85,     0,    86,
       0,    87,     4,    88,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,  -316,    21,    22,    23,     0,
      24,    25,     0,    26,     0,    27,    28,    29,    30,    31,
     824,    33,     0,    34,    35,     0,    36,  -200,    37,    38,
      39,    40,    41,   -42,    42,    43,     0,    44,     0,     0,
       0,    45,    46,  -316,    47,     0,    49,    50,  -200,   -42,
      51,    52,    53,     0,  -316,    54,    55,     0,    56,    57,
      58,    59,    60,    61,    62,    63,     0,    64,     0,    65,
      66,    67,    68,     0,    69,    70,    71,  -316,     0,     0,
      72,     0,    73,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    80,
       0,    81,     0,    82,    83,     0,    84,     0,     0,    85,
       0,    86,     0,    87,     4,    88,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,  -316,    21,    22,
      23,     0,    24,    25,     0,    26,     0,    27,    28,    29,
      30,    31,   828,    33,     0,    34,    35,     0,    36,  -200,
      37,    38,    39,    40,    41,   -42,    42,    43,     0,    44,
       0,     0,     0,    45,    46,  -316,    47,     0,    49,    50,
    -200,   -42,    51,    52,    53,     0,  -316,    54,    55,     0,
      56,    57,    58,    59,    60,    61,    62,    63,     0,    64,
       0,    65,    66,    67,    68,     0,    69,    70,    71,  -316,
       0,     0,    72,     0,    73,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,    81,     0,    82,    83,     0,    84,     0,
       0,    85,     0,    86,     0,    87,   699,    88,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   149,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,   150,   151,     0,   152,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,     0,    42,    43,
       0,     0,     0,     0,     0,     0,    46,     0,    47,     0,
      49,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,    62,   153,
       0,     0,     0,     0,     0,     0,    68,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    80,     0,    81,     0,    82,    83,   229,
       0,     0,     0,    85,     0,   154,     0,    87,     0,   155,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   149,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,   150,   151,     0,   152,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,    46,     0,
      47,     0,    49,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,   153,     0,     0,     0,     0,     0,     0,    68,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    81,     0,    82,
      83,   229,     0,     0,     0,    85,     0,   154,     0,    87,
       0,   155,   605,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   149,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,   150,   151,     0,   152,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,    46,     0,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,    62,   153,     0,     0,     0,     0,     0,
       0,    68,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,   229,     0,     0,     0,    85,     0,
     154,     0,    87,     0,   155,   923,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   149,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,   150,
     151,     0,   152,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,   153,     0,     0,
       0,     0,     0,     0,    68,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,    81,     0,    82,    83,     0,     0,     0,
       0,    85,     0,   154,     0,    87,     0,   155,   610,     5,
       6,     7,     8,   176,    10,   177,     0,     0,   149,     0,
       0,    14,    15,     0,     0,  -103,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,   150,   151,     0,   152,    33,     0,    34,     0,
       0,    36,  -467,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,    46,     0,    47,
       0,    49,     0,  -467,     0,     0,    52,  -103,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     153,     0,     0,     0,     0,     0,   178,    68,     0,  -103,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
       0,     0,     0,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
       0,     0,     0,     0,    85,     0,   154,     0,    87,     0,
     155,     5,     6,     7,     8,   176,    10,     0,     0,     0,
     149,     0,     0,    14,    15,     0,     0,  -103,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,   150,   151,     0,   152,    33,     0,
      34,     0,     0,    36,  -467,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,  -467,     0,     0,    52,  -103,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   153,     0,     0,     0,     0,     0,   178,    68,
       0,  -103,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    80,     0,    81,   149,
      82,    83,    14,    15,     0,     0,    85,     0,   154,    19,
      87,     0,   155,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,   150,   151,     0,   152,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,    46,     0,
      47,     0,    49,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,   153,     0,     0,     0,   233,   234,     0,    68,   272,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,   273,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    81,     0,    82,
      83,   229,     0,     0,     0,    85,     0,   154,     0,    87,
       0,   155,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   149,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,   150,   151,     0,   152,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
      46,     0,    47,     0,    49,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,   153,     0,     0,     0,   233,   234,     0,
      68,   272,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    80,     0,    81,
       0,    82,    83,   229,     0,     0,     0,    85,     0,   154,
       0,    87,   599,   155,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   149,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,   150,   151,     0,
     152,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,    62,   153,     0,     0,     0,   233,
     234,     0,    68,   272,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    80,
       0,    81,     0,    82,    83,   229,     0,     0,     0,    85,
       0,   154,     0,    87,   601,   155,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   149,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,   501,     0,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,   150,
     151,     0,   152,    33,   502,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
       0,     0,     0,    52,     0,     0,   503,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,   153,     0,     0,
       0,     0,     0,     0,    68,     0,     0,    70,     0,   504,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    80,     0,    81,   149,    82,    83,    14,    15,     0,
       0,    85,     0,   154,    19,    87,     0,   155,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,   150,   151,
       0,   152,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   153,     0,     0,     0,
     233,   234,     0,    68,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,     0,   235,     0,     0,
      85,     0,   154,     0,    87,     0,   155,   361,     6,     7,
       8,     9,    10,     0,     0,     0,   149,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
     150,   151,     0,   152,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,     0,    42,    43,     0,
       0,     0,     0,     0,     0,    46,     0,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,   153,     0,
       0,     0,   233,   234,     0,    68,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    80,     0,    81,     0,    82,    83,   229,     0,
       0,     0,    85,     0,   154,     0,    87,     0,   155,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   149,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,   150,   151,     0,   152,    33,     0,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,    46,     0,    47,
       0,    49,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     153,     0,     0,     0,   233,   234,     0,    68,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
       0,     0,     0,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
     229,     0,     0,     0,    85,     0,   154,     0,    87,     0,
     155,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     149,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,   150,   151,     0,   152,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
       0,    42,    43,   701,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   153,     0,     0,     0,   233,   234,     0,    68,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    80,     0,    81,   149,
      82,    83,    14,    15,     0,     0,    85,     0,   154,    19,
      87,     0,   155,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,   150,   151,     0,   152,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,    46,   181,
      47,     0,    49,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,   153,     0,     0,     0,     0,     0,     0,    68,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,   182,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    81,     0,    82,
      83,     0,     0,     0,     0,    85,     0,   154,     0,    87,
       0,   155,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   149,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,   186,     0,     0,     0,     0,     0,     0,    25,
       0,     0,     0,     0,   187,   150,   151,     0,   152,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
      46,     0,    47,     0,    49,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,   153,     0,     0,     0,     0,     0,     0,
      68,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    80,     0,    81,
     149,    82,    83,    14,    15,     0,     0,    85,     0,   154,
      19,    87,     0,   155,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,   150,   151,     0,   152,    33,     0,
      34,     0,     0,    36,  -467,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,  -467,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   153,     0,     0,     0,     0,     0,     0,    68,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    80,     0,    81,   149,
      82,    83,    14,    15,     0,     0,    85,     0,   154,    19,
      87,     0,   155,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,   150,   151,     0,   152,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,    46,     0,
      47,     0,    49,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,   153,     0,     0,     0,   233,   234,     0,    68,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    80,     0,    81,   149,    82,
      83,    14,    15,     0,     0,    85,     0,   154,    19,    87,
       0,   155,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,   150,   151,     0,   152,    33,     0,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,    46,     0,    47,
       0,    49,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     153,     0,     0,     0,     0,     0,     0,    68,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
       0,     0,     0,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
     229,     0,     0,     0,    85,     0,   154,   597,    87,     0,
     155,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     149,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,   215,    25,     0,
       0,     0,     0,     0,   150,   151,     0,   152,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   153,     0,     0,     0,     0,     0,     0,    68,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    80,     0,    81,   149,
      82,    83,    14,    15,     0,     0,    85,     0,    86,    19,
      87,     0,   155,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,   150,   151,     0,   152,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,    46,     0,
      47,     0,    49,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,   153,     0,     0,     0,     0,     0,     0,    68,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    81,     0,    82,
      83,   229,     0,     0,     0,    85,     0,   154,     0,    87,
       0,   155,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   149,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,   150,   151,     0,   152,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
      46,     0,    47,     0,    49,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,   153,     0,     0,     0,     0,     0,     0,
      68,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,   182,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    80,     0,    81,
     149,    82,    83,    14,   567,     0,     0,    85,     0,   154,
      19,    87,     0,   155,     0,     0,     0,     0,   568,     0,
       0,     0,     0,     0,   150,   151,     0,   152,    33,     0,
     569,     0,     0,    36,     0,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,   570,
       0,    47,     0,    49,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,   571,   572,    60,
      61,    62,   573,     0,     0,     0,     0,     0,     0,    68,
       0,     0,   574,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,   229,     0,     0,     0,    85,     0,   154,     0,
      87,     0,   876,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   149,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,   150,   151,     0,   152,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,    46,     0,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,    62,   153,     0,     0,     0,     0,     0,
       0,    68,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,   229,     0,     0,     0,    85,     0,
     154,     0,    87,     0,   876,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   149,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,   150,   151,
       0,   861,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   153,     0,     0,     0,
       0,     0,     0,    68,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,   229,     0,     0,     0,
      85,     0,   154,     0,    87,     0,   155,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   149,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
     150,   151,     0,   152,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,     0,    42,    43,     0,
       0,     0,     0,     0,     0,    46,     0,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,   153,     0,
       0,     0,     0,     0,     0,    68,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    80,     0,    81,   149,    82,    83,    14,    15,
       0,     0,    85,     0,   154,    19,    87,     0,   155,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,   150,
     151,     0,   152,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,   153,     0,     0,
       0,     0,     0,     0,    68,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    80,     0,    81,   149,    82,    83,    14,   467,     0,
       0,    85,     0,    86,    19,    87,     0,   155,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,   150,   151,
       0,   152,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   153,     0,     0,     0,
       0,     0,     0,    68,     0,     0,   468,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      80,     0,    81,   149,    82,    83,    14,    15,     0,     0,
      85,     0,   154,    19,    87,     0,   155,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,   150,   151,     0,
     152,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,    62,   153,     0,     0,     0,     0,
       0,     0,    68,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    80,
       0,    81,   149,    82,    83,    14,   567,     0,     0,   540,
       0,   154,    19,    87,     0,   155,     0,     0,     0,     0,
     568,     0,     0,     0,     0,     0,   150,   151,     0,   152,
      33,     0,   569,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,   570,     0,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,   571,
     572,    60,    61,    62,   573,     0,     0,     0,     0,     0,
       0,    68,     0,     0,   574,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    80,     0,
      81,   149,    82,    83,    14,    15,     0,     0,    85,     0,
     154,    19,    87,     0,   575,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,   150,   151,     0,   152,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
      46,     0,    47,     0,    49,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,   153,     0,     0,     0,     0,     0,     0,
      68,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    80,     0,    81,
     149,    82,    83,    14,    15,     0,     0,    85,     0,   154,
      19,    87,     0,   575,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,   150,   151,     0,   644,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   153,     0,     0,     0,     0,     0,     0,    68,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    80,     0,    81,   149,
      82,    83,    14,    15,     0,     0,    85,     0,   154,    19,
      87,     0,   155,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,   150,   151,     0,   647,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,    46,     0,
      47,     0,    49,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,   153,     0,     0,     0,     0,     0,     0,    68,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    80,     0,    81,   149,    82,
      83,    14,    15,     0,     0,    85,     0,   154,    19,    87,
       0,   155,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,   150,   151,     0,   855,    33,     0,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,    46,     0,    47,
       0,    49,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     153,     0,     0,     0,     0,     0,     0,    68,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
       0,     0,     0,     0,    79,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    80,     0,    81,   149,    82,    83,
      14,    15,     0,     0,    85,     0,   154,    19,    87,     0,
     155,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,   150,   151,     0,   856,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,     0,    42,    43,
       0,     0,     0,     0,     0,     0,    46,     0,    47,     0,
      49,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,    62,   153,
       0,     0,     0,     0,     0,     0,    68,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    80,     0,    81,   149,    82,    83,    14,
      15,     0,     0,    85,     0,   154,    19,    87,     0,   155,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
     150,   151,     0,   858,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,     0,    42,    43,     0,
       0,     0,     0,     0,     0,    46,     0,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,   153,     0,
       0,     0,     0,     0,     0,    68,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    80,     0,    81,   149,    82,    83,    14,    15,
       0,     0,    85,     0,   154,    19,    87,     0,   155,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,   150,
     151,     0,   859,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,   153,     0,     0,
       0,     0,     0,     0,    68,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    80,     0,    81,   149,    82,    83,    14,    15,     0,
       0,    85,     0,   154,    19,    87,     0,   155,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,   150,   151,
       0,   860,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   153,     0,     0,     0,
       0,     0,     0,    68,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      80,     0,    81,   149,    82,    83,    14,    15,     0,     0,
      85,     0,   154,    19,    87,     0,   155,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,   150,   151,     0,
     861,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,    62,   153,     0,     0,     0,     0,
       0,     0,    68,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    80,
       0,    81,   149,    82,    83,    14,   567,     0,     0,    85,
       0,   154,    19,    87,     0,   155,     0,     0,     0,     0,
     568,     0,     0,     0,     0,     0,   150,   151,     0,   152,
      33,     0,   569,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,   570,     0,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,   571,
     572,    60,    61,    62,   573,     0,     0,     0,     0,     0,
       0,    68,     0,     0,   574,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    80,     0,
      81,   149,    82,    83,    14,    15,     0,     0,    85,     0,
     154,    19,    87,     0,   936,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,   150,   151,     0,   152,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
      46,     0,    47,     0,    49,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,   153,     0,     0,     0,     0,     0,     0,
      68,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    80,     0,    81,
     149,    82,    83,    14,    15,     0,     0,    85,     0,     0,
      19,    87,     0,   936,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,   150,   151,     0,   152,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   153,     0,     0,     0,     0,     0,     0,    68,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    80,     0,    81,   149,
       0,    83,    14,    15,     0,     0,    85,     0,   154,    19,
      87,     0,   155,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,   150,   151,     0,   152,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,    46,     0,
      47,     0,    49,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,   153,     0,     0,     0,     0,     0,     0,    68,     0,
       0,    70,     0,     0,     5,     6,     7,     8,     9,    10,
       0,     0,  -353,   149,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,    76,     0,
       0,    25,  -353,     0,     0,     0,  -353,   150,   151,     0,
     152,    33,     0,    34,     0,     0,    36,    81,     0,    38,
      39,   229,    41,     0,    42,    43,     0,   154,     0,    87,
    -353,   876,    46,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,     0,     0,  -335,     0,     0,     0,
      58,    59,    60,    61,    62,   153,     0,     0,  -335,     0,
       0,     0,    68,     0,     0,    70,     0,     0,     0,  -335,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   149,     0,     0,    14,    15,     0,     0,     0,
       0,    81,    19,     0,     0,     0,  -335,     0,     0,     0,
      25,  -335,     0,    87,     0,   936,   150,   151,     0,   152,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,    46,     0,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,    62,   153,     0,     0,     0,     0,     0,
       0,    68,     0,     0,    70,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   149,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,    76,     0,     0,    25,     0,     0,     0,     0,     0,
     150,   151,     0,   152,    33,     0,    34,     0,     0,    36,
      81,     0,    38,    39,     0,    41,     0,    42,    43,     0,
     154,     0,    87,     0,   155,    46,     0,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,   153,     0,
       0,     0,     0,     0,     0,    68,     0,     0,    70,     0,
       0,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     149,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,    76,     0,     0,    25,     0,
       0,     0,     0,     0,   150,   151,     0,   152,    33,     0,
      34,     0,     0,    36,    81,     0,    38,    39,     0,    41,
       0,    42,    43,     0,   154,     0,    87,     0,   575,    46,
       0,    47,     0,    49,     0,   862,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   153,  -186,     0,     0,     0,  -186,  -186,    68,
       0,     0,    70,     0,     0,     0,  -186,     0,  -186,  -186,
       0,     0,  -186,     0,     0,     0,     0,  -186,     0,     0,
    -186,     0,     0,     0,     0,     0,     0,     0,     0,    76,
       0,     0,     0,     0,  -186,     0,  -186,     0,  -186,   192,
    -186,  -186,     0,  -186,     0,  -186,   733,  -186,    81,     0,
       0,   193,   194,   735,     0,     0,     0,     0,   195,     0,
      87,     0,   155,     0,  -186,     0,   196,  -186,     0,     0,
    -186,     0,     0,     0,     0,     0,   197,     0,   198,     0,
       0,   199,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   200,     0,     0,
       0,     0,     0,     0,     0,     0,   201,     0,     0,     0,
       0,     0,     0,     0,     0,   202,   203,     0,   204,  -186,
     205,     0,     0,     0,     0,  -186,     0,   206,     0,   296,
     207,     0,     0,     0,     0,     0,   297,     0,     0,     0,
       0,   324,   325,   326,   327,   328,   329,   298,     0,   332,
     333,   334,   335,     0,   337,   338,   742,   743,   744,   745,
       0,     0,   746,     0,     0,     0,   747,   748,   749,   750,
     751,   752,   753,   754,   755,   756,   757,   758,     0,   759,
       0,     0,   760,   761,   762,   763,     0,     0,     0,     0,
       0,   299,     0,     0,     0,   300,     0,   296,     0,     0,
       0,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,     0,   377,
       0,     0,     0,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   302,
       0,   303,   304,   305,     0,   306,   307,   308,     0,   309,
     310,   311,   312,   313,     0,   314,   315,   316,   317,   299,
     318,   319,   320,   300,     0,   321,   322,   323,     0,     0,
     296,     0,     0,     0,   718,     0,     0,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,     0,
       0,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   302,     0,   303,
     304,   305,     0,   306,   307,   308,     0,   309,   310,   311,
     312,   313,     0,   314,   315,   316,   317,     0,   318,   319,
     320,     0,   299,   321,   322,   323,   300,     0,   296,     0,
       0,     0,   782,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   298,     0,     0,     0,
       0,     0,     0,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,     0,   314,   315,   316,   317,
     299,   318,   319,   320,   300,     0,   321,   322,   323,     0,
       0,   296,     0,     0,     0,   932,     0,     0,   297,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
       0,     0,   301,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,     0,   314,   315,   316,   317,     0,   318,
     319,   320,     0,   299,   321,   322,   323,   300,     0,   296,
       0,     0,     0,   980,     0,     0,   297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,   301,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   302,     0,   303,   304,   305,     0,   306,   307,   308,
       0,   309,   310,   311,   312,   313,     0,   314,   315,   316,
     317,   299,   318,   319,   320,   300,     0,   321,   322,   323,
       0,     0,   296,     0,   598,     0,     0,     0,     0,   297,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,     0,     0,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   302,
       0,   303,   304,   305,     0,   306,   307,   308,     0,   309,
     310,   311,   312,   313,     0,   314,   315,   316,   317,     0,
     318,   319,   320,     0,   299,   321,   322,   323,   300,     0,
       0,     0,   904,     0,     0,   296,     0,   383,     0,     0,
       0,     0,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,     0,     0,   301,   324,   325,   326,
     327,   328,   329,     0,     0,   332,   333,   334,   335,     0,
     337,   338,   302,     0,   303,   304,   305,     0,   306,   307,
     308,     0,   309,   310,   311,   312,   313,     0,   314,   315,
     316,   317,     0,   318,   319,   320,     0,   299,   321,   322,
     323,   300,   352,     0,     0,     0,     0,     0,   296,     0,
     787,     0,     0,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   298,     0,     0,   301,
     324,   325,   326,   327,   328,   329,     0,     0,   332,   333,
     334,   335,     0,   337,   338,   302,     0,   303,   304,   305,
       0,   306,   307,   308,     0,   309,   310,   311,   312,   313,
       0,   314,   315,   316,   317,     0,   318,   319,   320,     0,
     299,   321,   322,   323,   300,   352,     0,     0,     0,     0,
       0,   296,     0,   792,     0,     0,     0,     0,   297,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
       0,     0,   301,   324,   325,   326,   327,   328,   329,     0,
       0,   332,   333,   334,   335,     0,   337,   338,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,     0,   314,   315,   316,   317,     0,   318,
     319,   320,     0,   299,   321,   322,   323,   300,   352,     0,
       0,     0,     0,     0,   296,     0,   915,     0,     0,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   298,     0,     0,   301,   324,   325,   326,   327,
     328,   329,     0,     0,   332,   333,   334,   335,     0,   337,
     338,   302,     0,   303,   304,   305,     0,   306,   307,   308,
       0,   309,   310,   311,   312,   313,     0,   314,   315,   316,
     317,     0,   318,   319,   320,     0,   299,   321,   322,   323,
     300,   352,     0,     0,     0,     0,     0,   296,     0,   916,
       0,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,   301,   324,
     325,   326,   327,   328,   329,     0,     0,   332,   333,   334,
     335,     0,   337,   338,   302,     0,   303,   304,   305,     0,
     306,   307,   308,     0,   309,   310,   311,   312,   313,     0,
     314,   315,   316,   317,     0,   318,   319,   320,     0,   299,
     321,   322,   323,   300,   352,     0,     0,     0,     0,     0,
     296,     0,   917,     0,     0,     0,     0,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,     0,
       0,   301,   324,   325,   326,   327,   328,   329,     0,     0,
     332,   333,   334,   335,     0,   337,   338,   302,     0,   303,
     304,   305,     0,   306,   307,   308,     0,   309,   310,   311,
     312,   313,     0,   314,   315,   316,   317,     0,   318,   319,
     320,     0,   299,   321,   322,   323,   300,   352,     0,     0,
       0,     0,     0,   296,     0,   918,     0,     0,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,     0,     0,   301,   324,   325,   326,   327,   328,
     329,     0,     0,   332,   333,   334,   335,     0,   337,   338,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,     0,   314,   315,   316,   317,
       0,   318,   319,   320,     0,   299,   321,   322,   323,   300,
     352,     0,     0,     0,     0,     0,   296,     0,   934,     0,
       0,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,     0,     0,   301,   324,   325,
     326,   327,   328,   329,     0,     0,   332,   333,   334,   335,
       0,   337,   338,   302,     0,   303,   304,   305,     0,   306,
     307,   308,     0,   309,   310,   311,   312,   313,     0,   314,
     315,   316,   317,     0,   318,   319,   320,     0,   299,   321,
     322,   323,   300,   352,     0,     0,     0,     0,     0,     0,
       0,   935,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     301,   324,   325,   326,   327,   328,   329,     0,     0,   332,
     333,   334,   335,     0,   337,   338,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,     0,   314,   315,   316,   317,     0,   318,   319,   320,
     296,     0,   321,   322,   323,     0,   352,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,   375,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   376,     0,     0,     0,     0,   296,     0,     0,     0,
       0,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,   372,     0,     0,     0,     0,
       0,     0,   299,     0,     0,     0,   300,   373,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   296,     0,     0,     0,     0,     0,     0,   297,     0,
     377,     0,     0,     0,   301,     0,     0,     0,   299,   298,
     670,     0,   300,     0,     0,     0,     0,     0,     0,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,     0,   314,   315,   316,   317,
     301,   318,   319,   320,     0,     0,   321,   322,   323,     0,
     352,     0,     0,   299,     0,     0,   302,   300,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,     0,   314,   315,   316,   317,     0,   318,   319,   320,
     296,   377,   321,   322,   323,   301,   352,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,   215,
       0,   302,     0,   303,   304,   305,     0,   306,   307,   308,
       0,   309,   310,   311,   312,   313,     0,   314,   315,   316,
     317,     0,   318,   319,   320,   296,     0,   321,   322,   323,
       0,   352,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   299,   298,   664,     0,   300,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   296,     0,     0,     0,     0,     0,     0,   297,     0,
       0,     0,     0,     0,   301,     0,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,   299,     0,     0,
     302,   300,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,     0,   314,   315,   316,   317,
       0,   318,   319,   320,   296,     0,   321,   322,   323,   301,
     352,   297,     0,   299,     0,     0,     0,   300,     0,     0,
       0,     0,   298,     0,     0,   302,   682,   303,   304,   305,
       0,   306,   307,   308,     0,   309,   310,   311,   312,   313,
       0,   314,   315,   316,   317,   301,   318,   319,   320,     0,
       0,   321,   322,   323,     0,   352,     0,     0,     0,     0,
       0,   302,     0,   303,   304,   305,   299,   306,   307,   308,
     300,   309,   310,   311,   312,   313,     0,   314,   315,   316,
     317,     0,   318,   319,   320,     0,     0,   321,   322,   323,
       0,   352,   296,     0,     0,     0,     0,     0,   301,   297,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,     0,     0,     0,   302,   380,   303,   304,   305,     0,
     306,   307,   308,     0,   309,   310,   311,   312,   313,     0,
     314,   315,   316,   317,     0,   318,   319,   320,     0,   381,
     321,   322,   323,     0,   410,   296,     0,     0,     0,     0,
       0,     0,   297,     0,   299,     0,     0,     0,   300,     0,
       0,     0,     0,   298,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   354,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   301,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   302,     0,   303,   304,   305,   299,   306,   307,
     308,   300,   309,   310,   311,   312,   313,     0,   314,   315,
     316,   317,     0,   318,   319,   320,     0,   382,   321,   322,
     323,     0,     0,   296,     0,   144,     0,     0,     0,   301,
     297,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,     0,     0,     0,   302,   423,   303,   304,   305,
       0,   306,   307,   308,     0,   309,   310,   311,   312,   313,
       0,   314,   315,   316,   317,     0,   318,   319,   320,     0,
     424,   321,   322,   323,     0,     0,   296,     0,     0,     0,
       0,     0,     0,   297,     0,   299,     0,     0,     0,   300,
       0,     0,     0,     0,   298,   510,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   511,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   301,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   302,     0,   303,   304,   305,   299,   306,
     307,   308,   300,   309,   310,   311,   312,   313,     0,   314,
     315,   316,   317,     0,   318,   319,   320,     0,     0,   321,
     322,   323,     0,     0,   296,     0,     0,     0,     0,     0,
     301,   297,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   298,   512,     0,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,   513,   309,   310,   311,   312,
     313,     0,   314,   315,   316,   317,     0,   318,   319,   320,
       0,     0,   321,   322,   323,     0,     0,   296,     0,     0,
       0,     0,     0,     0,   297,     0,   299,     0,     0,     0,
     300,     0,     0,     0,     0,   298,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   296,     0,
       0,     0,     0,     0,     0,   297,     0,     0,   301,     0,
       0,     0,     0,     0,     0,     0,   298,     0,     0,     0,
       0,     0,     0,     0,   302,     0,   303,   304,   305,   299,
     306,   307,   308,   300,   309,   310,   311,   312,   313,     0,
     314,   315,   316,   317,     0,   318,   319,   320,     0,     0,
     321,   322,   323,     0,     0,     0,     0,     0,     0,     0,
     299,   301,     0,     0,   300,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   302,     0,   303,
     304,   305,     0,   306,   307,   308,     0,   309,   310,   311,
     312,   313,   301,   314,   315,   316,   317,     0,   318,   319,
     320,     0,   414,   321,   322,   323,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,     0,   314,   315,   316,   317,     0,   318,
     319,   320,   296,   416,   321,   322,   323,     0,     0,   297,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,     0,     0,     0,     0,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   299,     0,     0,     0,   300,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   296,     0,     0,     0,     0,     0,     0,   297,
       0,     0,     0,     0,     0,   299,   301,     0,     0,   300,
     298,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   302,     0,   303,   304,   305,     0,   306,   307,
     308,     0,   309,   310,   311,   312,   313,   301,   314,   315,
     316,   317,     0,   318,   319,   320,     0,   418,   321,   322,
     323,     0,     0,   302,   299,   303,   304,   305,   300,   306,
     307,   308,     0,   309,   310,   311,   312,   313,     0,   314,
     315,   316,   317,     0,   318,   319,   320,   296,   426,   321,
     322,   323,     0,     0,   297,   441,   301,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,     0,     0,
       0,     0,   302,     0,   303,   304,   305,     0,   306,   307,
     308,     0,   309,   310,   311,   312,   313,     0,   314,   315,
     316,   317,     0,   318,   319,   320,   296,     0,   321,   322,
     323,     0,     0,   297,     0,     0,     0,     0,     0,   299,
       0,     0,     0,   300,   298,     0,     0,     0,     0,     0,
       0,     0,   514,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   296,     0,     0,     0,
       0,   301,     0,   297,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,     0,     0,   302,   299,   303,
     304,   305,   300,   306,   307,   308,     0,   309,   310,   311,
     312,   313,     0,   314,   315,   316,   317,     0,   318,   319,
     320,   296,     0,   321,   322,   323,     0,     0,   297,     0,
     301,     0,     0,     0,     0,     0,     0,     0,   299,   298,
       0,     0,   300,     0,     0,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,  -448,   314,   315,   316,   317,   144,   318,   319,   320,
     301,     0,   321,   322,   323,     0,     0,     0,     0,     0,
       0,     0,     0,   299,     0,     0,   302,   300,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,     0,   314,   315,   316,   317,     0,   318,   319,   320,
     296,   677,   321,   322,   323,   301,     0,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,     0,
       0,   302,     0,   303,   304,   305,     0,   306,   307,   308,
       0,   309,   310,   311,   312,   313,     0,   314,   315,   316,
     317,     0,   318,   319,   320,     0,   654,   321,   322,   323,
       0,     0,     0,   296,     0,     0,     0,     0,     0,     0,
     297,     0,   299,     0,     0,     0,   300,     0,     0,     0,
       0,   298,     0,     0,   680,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     302,     0,   303,   304,   305,   299,   306,   307,   308,   300,
     309,   310,   311,   312,   313,     0,   314,   315,   316,   317,
       0,   318,   319,   320,   296,     0,   321,   322,   323,     0,
       0,   297,     0,     0,     0,     0,     0,   301,     0,     0,
       0,     0,   298,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   302,     0,   303,   304,   305,     0,   306,
     307,   308,     0,   309,   310,   311,   312,   313,     0,   314,
     315,   316,   317,     0,   318,   319,   320,   296,     0,   321,
     322,   323,     0,     0,   297,     0,   299,     0,     0,     0,
     300,     0,     0,     0,     0,   298,   777,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   296,     0,
       0,     0,     0,     0,     0,   297,     0,   721,   301,     0,
       0,     0,     0,     0,     0,     0,   298,   780,     0,     0,
       0,     0,     0,     0,   302,     0,   303,   304,   305,   299,
     306,   307,   308,   300,   309,   310,   311,   312,   313,     0,
     314,   315,   316,   317,     0,   318,   319,   320,     0,     0,
     321,   322,   323,     0,     0,     0,     0,     0,     0,     0,
     299,   301,     0,     0,   300,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   302,     0,   303,
     304,   305,     0,   306,   307,   308,     0,   309,   310,   311,
     312,   313,   301,   314,   315,   316,   317,     0,   318,   319,
     320,     0,     0,   321,   322,   323,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,     0,   314,   315,   316,   317,     0,   318,
     319,   320,   296,     0,   321,   322,   323,     0,     0,   297,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,     0,     0,     0,     0,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   299,     0,     0,     0,   300,     0,
       0,     0,     0,     0,     0,     0,     0,   854,     0,     0,
       0,     0,   296,   871,     0,     0,     0,     0,     0,   297,
       0,     0,     0,     0,     0,   299,   301,     0,     0,   300,
     298,     0,     0,     0,     0,     0,     0,     0,   857,     0,
       0,     0,   302,     0,   303,   304,   305,     0,   306,   307,
     308,     0,   309,   310,   311,   312,   313,   301,   314,   315,
     316,   317,     0,   318,   319,   320,     0,     0,   321,   322,
     323,     0,     0,   302,   299,   303,   304,   305,   300,   306,
     307,   308,     0,   309,   310,   311,   312,   313,     0,   314,
     315,   316,   317,     0,   318,   319,   320,   296,     0,   321,
     322,   323,     0,     0,   297,     0,   301,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,     0,     0,
       0,     0,   302,     0,   303,   304,   305,     0,   306,   307,
     308,     0,   309,   310,   311,   312,   313,     0,   314,   315,
     316,   317,     0,   318,   319,   320,     0,     0,   321,   322,
     323,     0,     0,     0,     0,     0,     0,     0,     0,   299,
       0,     0,     0,   300,     0,     0,     0,     0,     0,     0,
     296,     0,   945,     0,     0,     0,     0,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,     0,
       0,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   302,     0,   303,
     304,   305,     0,   306,   307,   308,     0,   309,   310,   311,
     312,   313,     0,   314,   315,   316,   317,     0,   318,   319,
     320,     0,   299,   321,   322,   323,   300,     0,     0,     0,
       0,     0,     0,   296,     0,   946,     0,     0,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,     0,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,     0,   314,   315,   316,   317,
       0,   318,   319,   320,     0,   299,   321,   322,   323,   300,
       0,     0,     0,     0,     0,     0,   296,     0,   947,     0,
       0,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,     0,     0,   301,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   302,     0,   303,   304,   305,     0,   306,
     307,   308,     0,   309,   310,   311,   312,   313,     0,   314,
     315,   316,   317,     0,   318,   319,   320,     0,   299,   321,
     322,   323,   300,     0,     0,     0,     0,     0,     0,   296,
       0,   948,     0,     0,     0,     0,   297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   298,     0,     0,
     301,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,     0,   314,   315,   316,   317,     0,   318,   319,   320,
       0,   299,   321,   322,   323,   300,     0,     0,     0,     0,
       0,     0,   296,     0,   949,     0,     0,     0,     0,   297,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,     0,     0,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   302,
       0,   303,   304,   305,     0,   306,   307,   308,     0,   309,
     310,   311,   312,   313,     0,   314,   315,   316,   317,     0,
     318,   319,   320,     0,   299,   321,   322,   323,   300,     0,
       0,     0,     0,     0,     0,   296,     0,   950,     0,     0,
       0,     0,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,     0,     0,   301,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   302,     0,   303,   304,   305,     0,   306,   307,
     308,     0,   309,   310,   311,   312,   313,     0,   314,   315,
     316,   317,     0,   318,   319,   320,     0,   299,   321,   322,
     323,   300,     0,   296,     0,     0,     0,     0,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,     0,     0,     0,     0,     0,     0,     0,   301,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,   303,   304,   305,
       0,   306,   307,   308,     0,   309,   310,   311,   312,   313,
       0,   314,   315,   316,   317,   642,   318,   319,   320,   300,
     298,   321,   322,   323,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   301,     0,     0,
     298,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   302,   299,   303,   304,   305,   300,   306,
     307,   308,     0,   309,   310,   311,   312,   313,     0,   314,
     315,   316,   317,     0,   318,   319,   320,     0,   298,   321,
     322,   323,     0,     0,   299,     0,   301,     0,   300,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   302,     0,   303,   304,   305,     0,   306,   307,
     308,     0,   309,   310,   311,   312,   301,     0,   314,   315,
     316,   317,   299,   318,   319,   320,   300,     0,   321,   322,
     323,     0,   302,     0,   303,   304,   305,     0,   306,   307,
     308,     0,   309,   310,   311,   312,     0,     0,   314,   315,
     316,   317,     0,   318,     0,   320,     0,     0,   321,   322,
     323,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,     0,     0,   314,   315,   316,   317,
     615,   318,   192,   320,     0,     0,   321,   322,   323,     0,
       0,     0,     0,     0,   193,   194,     0,     0,     0,   192,
       0,   195,     0,     0,     0,     0,     0,     0,     0,   196,
       0,   193,   194,     0,     0,     0,     0,     0,   195,   197,
       0,   198,     0,     0,   199,     0,   196,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   197,     0,   198,     0,
     200,   199,     0,     0,     0,     0,     0,     0,     0,   201,
       0,     0,     0,     0,     0,     0,     0,   200,   202,   203,
       0,   204,     0,   205,     0,     0,   201,     0,     0,     0,
     206,     0,     0,   207,     0,   202,   203,     0,   204,     0,
     205,     0,     0,     0,     0,     0,     0,   206,     0,     0,
     207
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-862)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,     2,   130,   538,    15,    50,    51,    18,    17,   419,
      41,   167,    23,   278,   388,   698,    27,    28,    29,    30,
      31,    32,   178,   345,   391,     1,     1,     1,   810,    40,
      41,    42,    29,    44,   566,    46,    47,   191,    75,   290,
     291,   292,   293,    54,    55,    56,    57,    58,    59,    29,
     885,   724,    63,    64,    65,    66,   767,   768,    29,    70,
      71,    72,    73,    54,    29,    76,   100,    41,    79,    80,
      81,   225,    83,     3,    41,    86,    87,    88,     1,    75,
      71,     1,    41,    58,   381,    29,    41,   923,    41,   772,
     244,   807,   125,   105,   125,    46,    58,    88,    41,    41,
      30,    41,   125,   100,   639,    55,   967,   396,   145,   641,
     125,   100,   152,   150,   154,   976,    67,   129,    73,   901,
     153,   155,    97,   251,   155,    28,   962,   424,   811,   105,
     419,   105,   155,    73,   969,    97,   151,    87,   149,   150,
     151,   152,   153,   154,   155,   125,     0,   100,   124,   125,
     124,   125,    75,   150,   150,    75,   125,   166,   125,   875,
      90,   125,   173,   695,   155,     3,   125,    87,   841,    72,
     150,   145,   125,    76,    75,    98,   151,   153,    98,   150,
     191,   145,   125,   125,   153,   150,    87,   341,   342,   151,
     863,   874,   125,    19,   905,   906,   155,    98,   565,   450,
     125,   124,   155,    90,   124,    24,    75,   152,    19,   154,
     125,   105,   155,   155,   225,   226,   538,    24,    87,   125,
     153,   124,   233,   234,   127,   145,   151,   150,   131,    98,
     150,   125,   962,   244,    41,   129,   125,    44,   153,   922,
     125,   125,   125,    62,   145,    71,   433,   153,   435,   150,
     980,    75,    59,   125,    73,    62,   145,    64,   412,   153,
      71,   145,   273,    87,   799,   145,    73,    93,   153,   270,
     153,   536,   537,   145,    98,    72,   145,    96,    24,    76,
       7,   150,    93,    90,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   125,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   687,    83,   125,    62,   639,   125,   330,
     331,   145,   689,    68,   153,   336,   150,    73,   339,   340,
     341,   342,   343,   344,   345,   145,   145,    87,   603,   604,
      23,    41,   151,   354,    90,   145,   153,   358,    28,   360,
     724,    34,    35,   152,   152,   299,   300,   125,   125,   370,
      43,   372,   373,    46,   375,   376,    65,    66,   125,   380,
     381,    24,   383,   105,   385,    75,   152,    60,   145,   370,
     330,   331,     2,   125,    67,   153,   336,   398,   145,   339,
     340,    11,    72,   125,   405,   125,    76,   129,    98,   343,
     344,   412,    22,   145,    24,   105,   145,   817,   152,    62,
     354,   124,   423,   424,   127,   145,   145,   135,   131,   430,
      73,   153,   433,   173,   435,   125,   437,   438,   439,   373,
     441,   442,   376,   145,    28,   446,   145,    90,   594,   145,
     152,   191,   154,    63,   124,   145,   126,   127,   145,   145,
     150,   131,   145,   153,   151,   446,   467,   468,    72,   139,
     152,   152,    76,   154,   145,   127,    86,   145,   148,     3,
      90,   150,   846,    99,   145,   225,   152,   799,    72,   152,
     152,    15,    76,   433,   105,   435,   145,     7,    22,   863,
      52,   124,   150,   437,   244,   145,   624,   105,   153,   510,
     511,   512,   513,   514,   515,   516,    40,   125,   517,   518,
     145,    45,   523,    41,   525,   152,    31,   528,   125,   530,
     105,   810,     2,   534,   153,   145,   120,   538,   817,   123,
     124,    11,   126,   127,   153,    18,    70,   131,   153,   125,
     153,   150,    22,   155,    24,   139,   145,   145,    82,   145,
     145,   153,   146,   147,   148,   145,   567,    91,   155,   570,
     571,   572,   573,   574,   575,   145,   125,   511,   579,   513,
     145,   145,    31,   523,    19,   153,   153,   145,    23,   151,
     145,   150,   125,    63,   575,    65,    66,    32,   208,    34,
      35,   341,   342,    38,   151,   215,   105,    73,    43,    29,
      29,    46,   155,   150,    31,   124,    86,    73,   124,   155,
      90,   155,   901,   155,   151,    60,   152,   127,    31,    64,
     127,    66,    67,   127,    69,   127,    71,    28,   639,   579,
     127,   642,   643,   644,   923,   127,   647,   127,    28,   127,
     127,   153,    98,   932,    87,    90,   151,   153,    93,   722,
     270,   662,   155,   664,   424,   405,   667,   423,    21,   670,
     252,   709,   412,   527,   675,   145,   677,   785,   179,   680,
     727,    72,   850,   962,   846,    76,   558,   687,   967,   730,
     430,   969,    72,   433,   922,   435,    76,   976,   438,   941,
      31,   980,   399,   525,   705,    27,    -1,    -1,   642,   643,
     145,    -1,    -1,    -1,    -1,   716,   151,   718,    -1,    -1,
     721,    -1,    -1,    -1,   705,   726,    -1,    -1,    -1,   120,
      -1,    -1,   850,   124,    -1,   126,   127,    -1,   208,    -1,
     131,    -1,   352,    -1,   124,   215,   126,   127,   139,    -1,
      -1,   131,    -1,    -1,    -1,   146,   147,   148,    -1,   139,
      -1,    -1,   372,   764,    -1,   375,   146,   147,   148,    -1,
      -1,    -1,    -1,   383,    -1,   776,   777,    -1,   779,   780,
     781,   782,    -1,   523,    -1,   525,   787,   788,    -1,   790,
      -1,   792,   793,    19,    -1,   796,    -1,    23,   799,    -1,
     270,    -1,    23,   838,   839,    -1,    32,    -1,    34,    35,
      -1,    -1,    38,    34,    35,   816,   817,    43,    -1,    -1,
      46,   822,    43,   824,    -1,    46,    -1,   828,    -1,   439,
      -1,    52,    -1,    -1,    60,    -1,   837,    -1,    64,    60,
      66,    67,    -1,    69,   835,    71,    67,    68,    37,    -1,
      -1,    -1,    24,   854,   855,   856,   857,   858,   859,   860,
     861,    -1,    -1,    -1,    90,    -1,    -1,    93,    -1,    41,
     871,    -1,    44,    41,    -1,   876,    -1,    -1,    67,    -1,
     881,    -1,   352,   884,    -1,    -1,    -1,    59,    -1,    -1,
      62,    -1,    64,    -1,    -1,   876,    -1,   507,    -1,    -1,
      -1,    73,   372,     2,    -1,   375,    -1,    75,    -1,    41,
      -1,    -1,    11,   383,   915,   916,   917,   918,    90,   145,
      -1,    -1,   111,    22,   113,    24,    -1,    -1,    -1,    -1,
      98,   932,    -1,   934,   935,   936,    -1,   105,   548,    -1,
      -1,    -1,    -1,    75,   945,   946,   947,   948,   949,   950,
       2,    23,    -1,    -1,    -1,   936,    -1,   125,    -1,    11,
     961,   962,    34,    35,    63,    -1,    98,    -1,    -1,   439,
      22,    43,    24,   105,    46,   976,    -1,   145,   167,   980,
      52,    28,   150,    -1,    -1,   153,    -1,    86,    60,   178,
      -1,    90,   181,   125,    -1,    67,    68,     2,   608,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,
      -1,    63,    -1,   145,   624,    -1,    -1,    22,   150,    24,
      -1,   153,   211,    -1,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,   642,   643,    86,    -1,    -1,    -1,    90,    -1,
     229,    -1,    -1,    -1,     2,    -1,   145,    -1,    24,    -1,
      -1,    -1,   662,    11,   664,    -1,    -1,   667,    63,    -1,
     670,    -1,    -1,    -1,    22,    41,    24,    -1,    44,   679,
     810,    -1,   682,   120,    -1,   122,   123,   124,   548,   126,
     127,    86,    -1,    59,   131,    90,    62,    -1,    64,    -1,
      -1,   138,   139,   145,   704,    -1,   143,    73,    -1,   146,
     147,   148,    -1,    -1,    -1,    63,   716,    -1,   718,   208,
      -1,    -1,   722,    -1,    90,    -1,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,   608,    -1,
     145,   881,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,    15,
      16,    -1,    -1,   215,    -1,    -1,    22,    -1,    -1,    -1,
      -1,   270,    -1,    -1,    30,    -1,    -1,   787,    -1,   368,
      -1,    -1,   792,   923,    40,    -1,    42,   145,    -1,    45,
      -1,    -1,   662,    -1,   664,    -1,   806,   667,    -1,    -1,
     670,    -1,   391,   208,    -1,    61,    -1,    -1,   818,   679,
     215,    -1,   682,    -1,    70,    -1,    -1,   827,   270,    -1,
      -1,   831,   962,    79,    80,   835,    82,    -1,    84,    -1,
      -1,    -1,    -1,    -1,   704,    91,    92,    -1,    94,    -1,
     980,    -1,    -1,    -1,    -1,    -1,   716,    57,   718,    -1,
     208,    -1,   722,   352,    -1,    -1,   445,   215,    -1,   448,
      -1,    -1,   872,    -1,    -1,   270,     2,    -1,    -1,    -1,
      -1,    -1,    -1,   372,    -1,    11,   375,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   383,    -1,    22,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,
     352,    28,    -1,    -1,    -1,   915,   916,   917,   918,    -1,
      -1,    -1,   270,    -1,    -1,    -1,    -1,   787,    -1,    -1,
     372,    -1,   792,   375,   934,   935,    -1,    63,    -1,    -1,
      -1,   383,    -1,    -1,    -1,    -1,   806,    -1,    -1,    -1,
     439,    -1,    -1,    -1,    -1,    72,    -1,   352,   818,    76,
      86,    -1,    -1,    -1,    90,    -1,    -1,   827,    -1,    -1,
      -1,   831,    -1,    -1,    -1,   835,    -1,   372,    -1,    -1,
     375,    -1,   561,   183,   184,   185,   565,    -1,   383,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,    -1,
      -1,    -1,    -1,   120,   352,   122,   123,   124,    -1,   126,
     127,   590,   872,    -1,   131,   594,    -1,     2,   218,   145,
      -1,   221,   139,    -1,   372,    -1,    11,   375,    -1,   146,
     147,   148,    -1,    -1,    -1,   383,    -1,    22,    -1,    24,
     240,    -1,    -1,    -1,   439,   624,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   915,   916,   917,   918,   548,
     260,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   934,   935,    -1,    -1,    63,    -1,
      -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,   215,
      -1,   439,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    90,   548,   686,    -1,    19,
     689,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,   608,
      -1,    -1,    32,    -1,    34,    35,    -1,    -1,    38,    -1,
     709,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   270,    -1,    -1,    -1,   727,    -1,
      60,    -1,    -1,   548,    64,   355,    66,    67,    -1,    69,
     145,    71,    -1,    -1,   743,    -1,   608,    -1,    -1,    -1,
      -1,    -1,    -1,   662,    -1,   664,    -1,    -1,   667,   379,
      90,   670,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
     679,    -1,    -1,   682,    -1,    -1,    -1,    -1,    -1,    -1,
     548,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   608,    -1,   704,    -1,    -1,    -1,    -1,
     662,    -1,   664,   208,    -1,   667,   352,   716,   670,   718,
     215,    -1,    -1,   722,    -1,   145,    -1,   679,    -1,    -1,
     682,   151,    -1,    -1,    -1,    -1,   372,    -1,    -1,   375,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   383,    -1,    -1,
     608,    -1,   704,    -1,    -1,    -1,    -1,   662,    -1,   664,
      -1,   850,   667,    -1,   716,   670,   718,    -1,    -1,    -1,
     722,    -1,    -1,    -1,   679,   270,    -1,   682,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   787,    -1,
      -1,    -1,    -1,   792,    -1,    -1,    -1,    -1,    -1,   704,
      -1,    -1,    -1,   439,   662,    -1,   664,   806,    -1,   667,
      -1,   716,   670,   718,    -1,    -1,    -1,   722,    -1,   818,
      -1,   679,   532,   533,   682,    -1,   536,   537,   827,    -1,
      -1,    -1,   831,    -1,    -1,   787,   835,    -1,    -1,    -1,
     792,    -1,    -1,    -1,    -1,    -1,   704,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   806,    -1,    -1,   352,   716,    -1,
     718,    -1,    -1,    -1,   722,    -1,   818,    -1,    -1,    -1,
      -1,    -1,    -1,   872,    40,   827,    -1,   372,    -1,   831,
     375,    -1,   787,   835,    -1,    -1,    -1,   792,   383,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      66,   806,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   548,   818,    -1,    -1,   915,   916,   917,   918,
     872,    -1,   827,    -1,    -1,    -1,   831,    -1,    -1,   787,
     835,    -1,    -1,    -1,   792,   934,   935,    -1,    -1,    -1,
      -1,    -1,   652,   653,   439,    -1,    -1,    -1,   806,   659,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   669,
     818,    -1,   672,   915,   916,   917,   918,   872,    -1,   827,
      -1,    -1,   608,   831,    -1,    -1,    -1,   835,    -1,    -1,
      -1,    -1,   934,   935,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
     915,   916,   917,   918,   872,    -1,    -1,   183,   184,   185,
      15,    16,    -1,    -1,   190,    -1,   662,    22,   664,   934,
     935,   667,    -1,    -1,   670,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   679,    28,    40,   682,    42,    -1,    -1,
      45,    -1,   218,   548,    -1,   221,    -1,   915,   916,   917,
     918,    -1,    -1,    -1,    -1,    -1,    61,    -1,   704,    -1,
      -1,    -1,    -1,    -1,   240,    70,   934,   935,    73,    -1,
     716,    -1,   718,    -1,    79,    80,   722,    82,    72,    84,
      -1,    -1,    76,    -1,   260,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     820,    -1,     2,   608,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    24,    -1,   120,    -1,   122,   123,
     124,    -1,   126,   127,   128,    -1,    -1,   131,   132,   133,
      -1,   787,   136,   137,   138,   139,   792,   152,    -1,   143,
      -1,    -1,   146,   147,   148,    -1,     3,   662,    -1,   664,
     806,    -1,   667,    63,    -1,   670,    -1,    -1,    15,    16,
      -1,    -1,   818,    -1,   679,    22,    -1,   682,    -1,   355,
      -1,   827,    -1,    30,    -1,   831,    86,    -1,    -1,   835,
      90,    -1,    -1,    40,    -1,    42,    -1,    -1,    45,   704,
      -1,    -1,    -1,   379,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   716,    -1,   718,    61,    -1,    -1,   722,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    -1,   872,    -1,    -1,    -1,
      -1,    -1,    79,    80,    -1,    82,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    91,   145,    -1,    94,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    -1,    -1,    -1,    -1,     3,    -1,   915,
     916,   917,   918,    -1,    -1,    -1,    -1,    -1,   135,    15,
      16,    -1,   787,    -1,    -1,    -1,    22,   792,   934,   935,
      -1,    -1,   149,    -1,    30,   152,    -1,   154,    -1,    -1,
      -1,   806,    -1,   150,    40,   152,    42,    -1,   208,    45,
      -1,    -1,    -1,   818,    -1,   215,    -1,    -1,    -1,    -1,
      -1,    -1,   827,    -1,    -1,    61,   831,    -1,    -1,    -1,
     835,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    -1,    82,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    91,   532,   533,    94,    -1,
     536,   537,    -1,    -1,    -1,    -1,    -1,   872,    -1,    -1,
     270,    -1,    -1,   549,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
     915,   916,   917,   918,   590,    40,   152,    42,    -1,    -1,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   934,
     935,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,   352,    -1,    79,    80,    -1,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,
      -1,    -1,   372,    -1,    -1,   375,   652,   653,    -1,    -1,
      -1,    -1,    -1,   659,    -1,   661,    -1,    -1,    -1,    -1,
     666,    -1,    -1,   669,    -1,    -1,   672,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    28,    15,    16,    -1,    -1,    -1,   152,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   712,    30,    -1,   439,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    72,    -1,    -1,    61,    76,
      63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,   120,    -1,   122,   123,   124,    -1,   126,
     127,   128,    -1,    -1,   131,    -1,    -1,   120,   121,   122,
     123,   138,   139,    -1,    -1,   128,   143,    -1,    -1,   146,
     147,   148,    -1,    -1,   820,   138,    -1,   140,   548,   142,
     143,   144,    -1,    -1,    -1,   148,    -1,   150,    -1,   152,
      -1,   154,   155,    -1,    -1,     1,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,    -1,    19,    -1,    -1,    -1,    23,    24,   865,
     866,    -1,   868,   869,    -1,    -1,    32,    -1,    34,    35,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,   608,   885,
      46,   149,    -1,    -1,   152,   891,   154,   893,    -1,   895,
      -1,    -1,    -1,    -1,    60,    -1,    62,     1,    64,     3,
      66,    67,    -1,    69,    -1,    71,    10,    73,    -1,    -1,
      -1,    15,    16,    17,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    30,    93,    -1,    -1,
      96,    -1,   662,    -1,   664,    -1,    40,   667,    42,    -1,
     670,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   679,
      -1,    -1,   682,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    -1,    -1,   969,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   704,    79,    80,    -1,    82,   145,
      84,    -1,    -1,    -1,    -1,   151,   716,    91,   718,    -1,
      94,    -1,   722,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,   122,   123,
      -1,    -1,   126,    -1,    -1,    -1,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,    -1,   143,
      -1,    -1,   146,   147,   148,   149,    -1,    -1,   152,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   806,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   818,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   827,    -1,    -1,
      -1,   831,    -1,    -1,    -1,   835,    -1,    -1,    -1,     0,
       1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,   872,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    43,    -1,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    -1,    56,    -1,    -1,    -1,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    -1,    86,    -1,    88,    89,    90,
      91,    -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,   140,
      -1,   142,   143,    -1,   145,    -1,    -1,   148,    -1,   150,
      -1,   152,     1,   154,     3,     4,     5,     6,     7,     8,
      -1,    10,    -1,    12,    13,    14,    15,    16,    17,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    43,    -1,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    -1,    56,    -1,    -1,
      -1,    60,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    -1,    86,    -1,    88,
      89,    90,    91,    -1,    93,    94,    95,    96,    -1,    -1,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,   133,   134,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,    -1,   145,   146,   147,   148,
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
      -1,    -1,    -1,   120,   121,   122,   123,    -1,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,   143,   144,   145,    -1,
      -1,   148,    -1,   150,   151,   152,     1,   154,     3,     4,
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
      -1,    -1,    -1,    -1,    -1,   120,   121,   122,   123,    -1,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,   140,    -1,   142,   143,    -1,
     145,    -1,    -1,   148,    -1,   150,   151,   152,     1,   154,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,
     123,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,   140,    -1,   142,
     143,    -1,   145,    -1,    -1,   148,    -1,   150,   151,   152,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,   140,
      -1,   142,   143,    -1,   145,    -1,    -1,   148,    -1,   150,
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
      -1,   120,   121,   122,   123,    -1,    -1,    -1,    -1,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,   143,    -1,   145,    -1,    -1,   148,
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
      -1,    -1,    -1,   120,   121,   122,   123,    -1,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,   143,    -1,   145,    -1,
      -1,   148,    -1,   150,    -1,   152,     1,   154,     3,     4,
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
      -1,    -1,    -1,    -1,    -1,   120,   121,   122,   123,    -1,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,   140,    -1,   142,   143,    -1,
     145,    -1,    -1,   148,    -1,   150,    -1,   152,     1,   154,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,
     123,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,   140,    -1,   142,
     143,    -1,   145,    -1,    -1,   148,    -1,   150,    -1,   152,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,   140,
      -1,   142,   143,    -1,   145,    -1,    -1,   148,    -1,   150,
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
      -1,   120,   121,   122,   123,    -1,    -1,    -1,    -1,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,   143,    -1,   145,    -1,    -1,   148,
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
      -1,    -1,    -1,   120,   121,   122,   123,    -1,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,   143,    -1,   145,    -1,
      -1,   148,    -1,   150,    -1,   152,     1,   154,     3,     4,
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
      -1,    -1,    -1,    -1,    -1,   120,   121,   122,   123,    -1,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,   140,    -1,   142,   143,    -1,
     145,    -1,    -1,   148,    -1,   150,    -1,   152,     1,   154,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,
     123,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,   140,    -1,   142,
     143,    -1,   145,    -1,    -1,   148,    -1,   150,    -1,   152,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,   140,
      -1,   142,   143,    -1,   145,    -1,    -1,   148,    -1,   150,
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
      -1,   120,   121,   122,   123,    -1,    -1,    -1,    -1,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,   143,    -1,   145,    -1,    -1,   148,
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
      -1,    -1,    -1,   120,   121,   122,   123,    -1,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,   143,    -1,   145,    -1,
      -1,   148,    -1,   150,    -1,   152,     1,   154,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,   121,   122,   123,    -1,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,   140,    -1,   142,   143,   144,
      -1,    -1,    -1,   148,    -1,   150,    -1,   152,    -1,   154,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,
     123,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,   140,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,    -1,   150,    -1,   152,
      -1,   154,   155,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,   121,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,
     140,    -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,
     150,    -1,   152,    -1,   154,   155,     3,     4,     5,     6,
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
      -1,    -1,    -1,   120,   121,   122,   123,    -1,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,   150,    -1,   152,    -1,   154,   155,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    19,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    46,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    67,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,   123,
      -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,   140,    -1,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,   150,    -1,   152,    -1,
     154,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,
     122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   138,    -1,   140,    12,
     142,   143,    15,    16,    -1,    -1,   148,    -1,   150,    22,
     152,    -1,   154,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    88,    89,    -1,    91,    92,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,
     123,    -1,    -1,    -1,    -1,   128,   129,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,   140,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,    -1,   150,    -1,   152,
      -1,   154,     3,     4,     5,     6,     7,     8,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,   140,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,   150,
      -1,   152,   153,   154,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    -1,    88,
      89,    -1,    91,    92,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,   121,   122,   123,    -1,    -1,    -1,    -1,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
      -1,   150,    -1,   152,   153,   154,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    41,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    73,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,    -1,    -1,    -1,
      -1,   128,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,   138,    -1,   140,    12,   142,   143,    15,    16,    -1,
      -1,   148,    -1,   150,    22,   152,    -1,   154,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      88,    89,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,   121,   122,   123,    -1,    -1,    -1,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,   140,    -1,   142,   143,    -1,   145,    -1,    -1,
     148,    -1,   150,    -1,   152,    -1,   154,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    88,    89,    -1,    91,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,   121,   122,   123,    -1,    -1,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,    -1,   150,    -1,   152,    -1,   154,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    88,    89,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,   123,
      -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,   140,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,    -1,   150,    -1,   152,    -1,
     154,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    55,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    88,    89,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,
     122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   138,    -1,   140,    12,
     142,   143,    15,    16,    -1,    -1,   148,    -1,   150,    22,
     152,    -1,   154,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,
     123,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,   140,    -1,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,   150,    -1,   152,
      -1,   154,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    35,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,   138,    -1,   140,
      12,   142,   143,    15,    16,    -1,    -1,   148,    -1,   150,
      22,   152,    -1,   154,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    46,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    67,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,
     122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   138,    -1,   140,    12,
     142,   143,    15,    16,    -1,    -1,   148,    -1,   150,    22,
     152,    -1,   154,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    88,    89,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,
     123,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,   138,    -1,   140,    12,   142,
     143,    15,    16,    -1,    -1,   148,    -1,   150,    22,   152,
      -1,   154,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,   123,
      -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,   140,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,    -1,   150,   151,   152,    -1,
     154,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,
     122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   138,    -1,   140,    12,
     142,   143,    15,    16,    -1,    -1,   148,    -1,   150,    22,
     152,    -1,   154,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,
     123,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,   140,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,    -1,   150,    -1,   152,
      -1,   154,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,   138,    -1,   140,
      12,   142,   143,    15,    16,    -1,    -1,   148,    -1,   150,
      22,   152,    -1,   154,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,
     122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,   150,    -1,
     152,    -1,   154,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,   121,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,
     140,    -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,
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
      -1,    -1,   120,   121,   122,   123,    -1,    -1,    -1,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,   140,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,    -1,   150,    -1,   152,    -1,   154,     3,     4,     5,
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
      -1,    -1,    -1,    -1,   120,   121,   122,   123,    -1,    -1,
      -1,    -1,   128,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   138,    -1,   140,    12,   142,   143,    15,    16,
      -1,    -1,   148,    -1,   150,    22,   152,    -1,   154,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,    -1,    -1,    -1,
      -1,   128,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,   138,    -1,   140,    12,   142,   143,    15,    16,    -1,
      -1,   148,    -1,   150,    22,   152,    -1,   154,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,   121,   122,   123,    -1,    -1,    -1,    -1,
     128,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     138,    -1,   140,    12,   142,   143,    15,    16,    -1,    -1,
     148,    -1,   150,    22,   152,    -1,   154,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,   121,   122,   123,    -1,    -1,    -1,    -1,   128,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   138,
      -1,   140,    12,   142,   143,    15,    16,    -1,    -1,   148,
      -1,   150,    22,   152,    -1,   154,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,   121,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   138,    -1,
     140,    12,   142,   143,    15,    16,    -1,    -1,   148,    -1,
     150,    22,   152,    -1,   154,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,   138,    -1,   140,
      12,   142,   143,    15,    16,    -1,    -1,   148,    -1,   150,
      22,   152,    -1,   154,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,
     122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   138,    -1,   140,    12,
     142,   143,    15,    16,    -1,    -1,   148,    -1,   150,    22,
     152,    -1,   154,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,
     123,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,   138,    -1,   140,    12,   142,
     143,    15,    16,    -1,    -1,   148,    -1,   150,    22,   152,
      -1,   154,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,   123,
      -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   138,    -1,   140,    12,   142,   143,
      15,    16,    -1,    -1,   148,    -1,   150,    22,   152,    -1,
     154,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,   121,   122,   123,    -1,
      -1,    -1,    -1,   128,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,   138,    -1,   140,    12,   142,   143,    15,
      16,    -1,    -1,   148,    -1,   150,    22,   152,    -1,   154,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,   121,   122,   123,    -1,    -1,
      -1,    -1,   128,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   138,    -1,   140,    12,   142,   143,    15,    16,
      -1,    -1,   148,    -1,   150,    22,   152,    -1,   154,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,    -1,    -1,    -1,
      -1,   128,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,   138,    -1,   140,    12,   142,   143,    15,    16,    -1,
      -1,   148,    -1,   150,    22,   152,    -1,   154,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,   121,   122,   123,    -1,    -1,    -1,    -1,
     128,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     138,    -1,   140,    12,   142,   143,    15,    16,    -1,    -1,
     148,    -1,   150,    22,   152,    -1,   154,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,   121,   122,   123,    -1,    -1,    -1,    -1,   128,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   138,
      -1,   140,    12,   142,   143,    15,    16,    -1,    -1,   148,
      -1,   150,    22,   152,    -1,   154,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,   121,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   138,    -1,
     140,    12,   142,   143,    15,    16,    -1,    -1,   148,    -1,
     150,    22,   152,    -1,   154,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,   138,    -1,   140,
      12,   142,   143,    15,    16,    -1,    -1,   148,    -1,    -1,
      22,   152,    -1,   154,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,
     122,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   138,    -1,   140,    12,
      -1,   143,    15,    16,    -1,    -1,   148,    -1,   150,    22,
     152,    -1,   154,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    94,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,   105,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    30,   125,    -1,    -1,    -1,   129,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,   140,    -1,    48,
      49,   144,    51,    -1,    53,    54,    -1,   150,    -1,   152,
     153,   154,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    87,    -1,
      -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,   140,    22,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      30,   150,    -1,   152,    -1,   154,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
     140,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
     150,    -1,   152,    -1,   154,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,   121,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,   140,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,   150,    -1,   152,    -1,   154,    61,
      -1,    63,    -1,    65,    -1,     1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    19,    -1,    -1,    -1,    23,    24,    91,
      -1,    -1,    94,    -1,    -1,    -1,    32,    -1,    34,    35,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    60,    -1,    62,    -1,    64,     3,
      66,    67,    -1,    69,    -1,    71,    10,    73,   140,    -1,
      -1,    15,    16,    17,    -1,    -1,    -1,    -1,    22,    -1,
     152,    -1,   154,    -1,    90,    -1,    30,    93,    -1,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    40,    -1,    42,    -1,
      -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    -1,    82,   145,
      84,    -1,    -1,    -1,    -1,   151,    -1,    91,    -1,    10,
      94,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,   105,   106,   107,   108,   109,   110,    28,    -1,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,   122,   123,
      -1,    -1,   126,    -1,    -1,    -1,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,    -1,   143,
      -1,    -1,   146,   147,   148,   149,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,   100,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,   122,   123,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,   133,   134,    -1,   136,   137,   138,   139,    72,
     141,   142,   143,    76,    -1,   146,   147,   148,    -1,    -1,
      10,    -1,    -1,    -1,   155,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,
     123,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
     133,   134,    -1,   136,   137,   138,   139,    -1,   141,   142,
     143,    -1,    72,   146,   147,   148,    76,    -1,    10,    -1,
      -1,    -1,   155,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,   123,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,   133,   134,    -1,   136,   137,   138,   139,
      72,   141,   142,   143,    76,    -1,   146,   147,   148,    -1,
      -1,    10,    -1,    -1,    -1,   155,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
     122,   123,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,   133,   134,    -1,   136,   137,   138,   139,    -1,   141,
     142,   143,    -1,    72,   146,   147,   148,    76,    -1,    10,
      -1,    -1,    -1,   155,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,   122,   123,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,   133,   134,    -1,   136,   137,   138,
     139,    72,   141,   142,   143,    76,    -1,   146,   147,   148,
      -1,    -1,    10,    -1,   153,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,   122,   123,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,   133,   134,    -1,   136,   137,   138,   139,    -1,
     141,   142,   143,    -1,    72,   146,   147,   148,    76,    -1,
      -1,    -1,   153,    -1,    -1,    10,    -1,    85,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,   104,   105,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,    -1,   136,   137,
     138,   139,    -1,   141,   142,   143,    -1,    72,   146,   147,
     148,    76,   150,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      85,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,   104,
     105,   106,   107,   108,   109,   110,    -1,    -1,   113,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
      -1,   126,   127,   128,    -1,   130,   131,   132,   133,   134,
      -1,   136,   137,   138,   139,    -1,   141,   142,   143,    -1,
      72,   146,   147,   148,    76,   150,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    85,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,   104,   105,   106,   107,   108,   109,   110,    -1,
      -1,   113,   114,   115,   116,    -1,   118,   119,   120,    -1,
     122,   123,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,   133,   134,    -1,   136,   137,   138,   139,    -1,   141,
     142,   143,    -1,    72,   146,   147,   148,    76,   150,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    85,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,   104,   105,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,    -1,   122,   123,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,   133,   134,    -1,   136,   137,   138,
     139,    -1,   141,   142,   143,    -1,    72,   146,   147,   148,
      76,   150,    -1,    -1,    -1,    -1,    -1,    10,    -1,    85,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,   104,   105,
     106,   107,   108,   109,   110,    -1,    -1,   113,   114,   115,
     116,    -1,   118,   119,   120,    -1,   122,   123,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,   133,   134,    -1,
     136,   137,   138,   139,    -1,   141,   142,   143,    -1,    72,
     146,   147,   148,    76,   150,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    85,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,   104,   105,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,    -1,   122,
     123,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
     133,   134,    -1,   136,   137,   138,   139,    -1,   141,   142,
     143,    -1,    72,   146,   147,   148,    76,   150,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    85,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,   104,   105,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,    -1,   122,   123,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,   133,   134,    -1,   136,   137,   138,   139,
      -1,   141,   142,   143,    -1,    72,   146,   147,   148,    76,
     150,    -1,    -1,    -1,    -1,    -1,    10,    -1,    85,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,   104,   105,   106,
     107,   108,   109,   110,    -1,    -1,   113,   114,   115,   116,
      -1,   118,   119,   120,    -1,   122,   123,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,   133,   134,    -1,   136,
     137,   138,   139,    -1,   141,   142,   143,    -1,    72,   146,
     147,   148,    76,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,    -1,   118,   119,   120,    -1,   122,   123,
     124,    -1,   126,   127,   128,    -1,   130,   131,   132,   133,
     134,    -1,   136,   137,   138,   139,    -1,   141,   142,   143,
      10,    -1,   146,   147,   148,    -1,   150,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    76,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,    -1,    72,    28,
      29,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,   123,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,   133,   134,    -1,   136,   137,   138,   139,
     104,   141,   142,   143,    -1,    -1,   146,   147,   148,    -1,
     150,    -1,    -1,    72,    -1,    -1,   120,    76,   122,   123,
     124,    -1,   126,   127,   128,    -1,   130,   131,   132,   133,
     134,    -1,   136,   137,   138,   139,    -1,   141,   142,   143,
      10,   100,   146,   147,   148,   104,   150,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,   120,    -1,   122,   123,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,   133,   134,    -1,   136,   137,   138,
     139,    -1,   141,   142,   143,    10,    -1,   146,   147,   148,
      -1,   150,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    28,    29,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,
     120,    76,   122,   123,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,   133,   134,    -1,   136,   137,   138,   139,
      -1,   141,   142,   143,    10,    -1,   146,   147,   148,   104,
     150,    17,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    28,    -1,    -1,   120,    85,   122,   123,   124,
      -1,   126,   127,   128,    -1,   130,   131,   132,   133,   134,
      -1,   136,   137,   138,   139,   104,   141,   142,   143,    -1,
      -1,   146,   147,   148,    -1,   150,    -1,    -1,    -1,    -1,
      -1,   120,    -1,   122,   123,   124,    72,   126,   127,   128,
      76,   130,   131,   132,   133,   134,    -1,   136,   137,   138,
     139,    -1,   141,   142,   143,    -1,    -1,   146,   147,   148,
      -1,   150,    10,    -1,    -1,    -1,    -1,    -1,   104,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,   120,    33,   122,   123,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,   133,   134,    -1,
     136,   137,   138,   139,    -1,   141,   142,   143,    -1,    57,
     146,   147,   148,    -1,   150,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    72,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,   122,   123,   124,    72,   126,   127,
     128,    76,   130,   131,   132,   133,   134,    -1,   136,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,    -1,    -1,    10,    -1,   100,    -1,    -1,    -1,   104,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,   120,    33,   122,   123,   124,
      -1,   126,   127,   128,    -1,   130,   131,   132,   133,   134,
      -1,   136,   137,   138,   139,    -1,   141,   142,   143,    -1,
      57,   146,   147,   148,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,   122,   123,   124,    72,   126,
     127,   128,    76,   130,   131,   132,   133,   134,    -1,   136,
     137,   138,   139,    -1,   141,   142,   143,    -1,    -1,   146,
     147,   148,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
     104,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    29,    -1,    -1,   120,    -1,   122,   123,
     124,    -1,   126,   127,   128,    41,   130,   131,   132,   133,
     134,    -1,   136,   137,   138,   139,    -1,   141,   142,   143,
      -1,    -1,   146,   147,   148,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    72,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,   122,   123,   124,    72,
     126,   127,   128,    76,   130,   131,   132,   133,   134,    -1,
     136,   137,   138,   139,    -1,   141,   142,   143,    -1,    -1,
     146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,   104,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,
     123,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
     133,   134,   104,   136,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,    -1,    -1,   120,    -1,
     122,   123,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,   133,   134,    -1,   136,   137,   138,   139,    -1,   141,
     142,   143,    10,   145,   146,   147,   148,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    72,   104,    -1,    -1,    76,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,   122,   123,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,   104,   136,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,    -1,    -1,   120,    72,   122,   123,   124,    76,   126,
     127,   128,    -1,   130,   131,   132,   133,   134,    -1,   136,
     137,   138,   139,    -1,   141,   142,   143,    10,   145,   146,
     147,   148,    -1,    -1,    17,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,   122,   123,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,    -1,   136,   137,
     138,   139,    -1,   141,   142,   143,    10,    -1,   146,   147,
     148,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    76,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,   104,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,   120,    72,   122,
     123,   124,    76,   126,   127,   128,    -1,   130,   131,   132,
     133,   134,    -1,   136,   137,   138,   139,    -1,   141,   142,
     143,    10,    -1,   146,   147,   148,    -1,    -1,    17,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    28,
      -1,    -1,    76,    -1,    -1,    -1,   120,    -1,   122,   123,
     124,    -1,   126,   127,   128,    -1,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   100,   141,   142,   143,
     104,    -1,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    -1,   120,    76,   122,   123,
     124,    -1,   126,   127,   128,    -1,   130,   131,   132,   133,
     134,    -1,   136,   137,   138,   139,    -1,   141,   142,   143,
      10,    11,   146,   147,   148,   104,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,   120,    -1,   122,   123,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,   133,   134,    -1,   136,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,   123,   124,    72,   126,   127,   128,    76,
     130,   131,   132,   133,   134,    -1,   136,   137,   138,   139,
      -1,   141,   142,   143,    10,    -1,   146,   147,   148,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,   122,   123,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,   133,   134,    -1,   136,
     137,   138,   139,    -1,   141,   142,   143,    10,    -1,   146,
     147,   148,    -1,    -1,    17,    -1,    72,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,   122,   123,   124,    72,
     126,   127,   128,    76,   130,   131,   132,   133,   134,    -1,
     136,   137,   138,   139,    -1,   141,   142,   143,    -1,    -1,
     146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,   104,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,
     123,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
     133,   134,   104,   136,   137,   138,   139,    -1,   141,   142,
     143,    -1,    -1,   146,   147,   148,    -1,    -1,   120,    -1,
     122,   123,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,   133,   134,    -1,   136,   137,   138,   139,    -1,   141,
     142,   143,    10,    -1,   146,   147,   148,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    10,    11,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    72,   104,    -1,    -1,    76,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,   120,    -1,   122,   123,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,   104,   136,   137,
     138,   139,    -1,   141,   142,   143,    -1,    -1,   146,   147,
     148,    -1,    -1,   120,    72,   122,   123,   124,    76,   126,
     127,   128,    -1,   130,   131,   132,   133,   134,    -1,   136,
     137,   138,   139,    -1,   141,   142,   143,    10,    -1,   146,
     147,   148,    -1,    -1,    17,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,   122,   123,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,    -1,   136,   137,
     138,   139,    -1,   141,   142,   143,    -1,    -1,   146,   147,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    85,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,
     123,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
     133,   134,    -1,   136,   137,   138,   139,    -1,   141,   142,
     143,    -1,    72,   146,   147,   148,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    85,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,   123,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,   133,   134,    -1,   136,   137,   138,   139,
      -1,   141,   142,   143,    -1,    72,   146,   147,   148,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    85,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,   122,   123,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,   133,   134,    -1,   136,
     137,   138,   139,    -1,   141,   142,   143,    -1,    72,   146,
     147,   148,    76,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    85,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,   123,
     124,    -1,   126,   127,   128,    -1,   130,   131,   132,   133,
     134,    -1,   136,   137,   138,   139,    -1,   141,   142,   143,
      -1,    72,   146,   147,   148,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    85,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,   122,   123,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,   133,   134,    -1,   136,   137,   138,   139,    -1,
     141,   142,   143,    -1,    72,   146,   147,   148,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    85,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,   122,   123,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,    -1,   136,   137,
     138,   139,    -1,   141,   142,   143,    -1,    72,   146,   147,
     148,    76,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,   122,   123,   124,
      -1,   126,   127,   128,    -1,   130,   131,   132,   133,   134,
      -1,   136,   137,   138,   139,    72,   141,   142,   143,    76,
      28,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    72,   122,   123,   124,    76,   126,
     127,   128,    -1,   130,   131,   132,   133,   134,    -1,   136,
     137,   138,   139,    -1,   141,   142,   143,    -1,    28,   146,
     147,   148,    -1,    -1,    72,    -1,   104,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,   122,   123,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,   133,   104,    -1,   136,   137,
     138,   139,    72,   141,   142,   143,    76,    -1,   146,   147,
     148,    -1,   120,    -1,   122,   123,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,   133,    -1,    -1,   136,   137,
     138,   139,    -1,   141,    -1,   143,    -1,    -1,   146,   147,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,   123,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,   133,    -1,    -1,   136,   137,   138,   139,
       1,   141,     3,   143,    -1,    -1,   146,   147,   148,    -1,
      -1,    -1,    -1,    -1,    15,    16,    -1,    -1,    -1,     3,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    40,
      -1,    42,    -1,    -1,    45,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    40,    -1,    42,    -1,
      61,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    79,    80,
      -1,    82,    -1,    84,    -1,    -1,    70,    -1,    -1,    -1,
      91,    -1,    -1,    94,    -1,    79,    80,    -1,    82,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      94
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
      94,    95,    99,   101,   104,   120,   121,   122,   123,   128,
     138,   140,   142,   143,   145,   148,   150,   152,   154,   162,
     163,   164,   165,   166,   168,   173,   174,   175,   179,   183,
     184,   185,   186,   187,   188,   190,   191,   192,   196,   197,
     200,   201,   204,   205,   208,   211,   212,   235,   238,   239,
     259,   260,   261,   262,   263,   264,   272,   273,   274,   275,
     278,   279,   280,   281,   282,   283,   285,   286,   287,   288,
     289,   145,   164,   275,   100,   265,   266,     3,   176,    12,
      36,    37,    39,    84,   150,   154,   208,   259,   263,   273,
     274,   275,   278,   280,   281,   265,   275,    90,   176,   164,
     253,   275,   164,   152,   275,   276,     7,     9,    90,   181,
     276,    62,   102,   189,   275,   275,    23,    35,   238,   275,
     275,   152,     3,    15,    16,    22,    30,    40,    42,    45,
      61,    70,    79,    80,    82,    84,    91,    94,   177,   178,
     209,   152,   177,   240,   241,    29,   168,   182,   275,   275,
     275,   275,   275,   275,     7,   152,    35,   185,   185,   144,
     231,   252,   275,    88,    89,   145,   275,   277,   275,   182,
     275,   275,   275,    83,   152,   164,   275,   275,   168,   175,
     275,   278,   168,   175,   275,   177,   236,   275,   252,   275,
     275,   275,   275,   275,   275,   275,   275,     1,   151,   162,
     169,   252,    92,   129,   231,   254,   255,   277,   252,   275,
     284,    64,   164,    68,   167,   177,   177,    46,    67,   226,
      24,    62,    73,    96,   145,   135,    10,    17,    28,    72,
      76,   104,   120,   122,   123,   124,   126,   127,   128,   130,
     131,   132,   133,   134,   136,   137,   138,   139,   141,   142,
     143,   146,   147,   148,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   149,
     218,   152,   154,    72,    76,   152,   164,   145,   275,   275,
     275,   252,   150,   168,    41,   265,   236,   145,   125,   145,
      99,     3,   231,   256,   257,   258,   277,   236,   201,   177,
     152,   182,    29,    41,   182,    29,    41,   100,   182,   268,
      33,    57,   145,    85,   168,   218,   256,   164,   152,   220,
      92,   152,   177,   242,   243,     1,   124,   247,    41,   125,
     164,   182,   182,   256,   177,   125,   145,   275,   275,   145,
     150,   182,   152,   256,   145,   193,   145,   193,   145,   105,
     237,   145,   145,    33,    57,   182,   145,   151,   151,   162,
     125,   151,   275,   125,   153,   125,   153,    41,   125,   155,
     268,   103,   125,   155,     7,    52,   124,   202,   150,   213,
      73,   240,   240,   240,   240,   275,   275,   275,   275,   189,
     275,   189,   275,   275,   275,   275,   275,    16,    94,   275,
     275,     3,    30,    90,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     277,   277,   277,   277,   277,   256,   256,   189,   275,   189,
     275,    24,    41,    73,    96,   267,   270,   271,   275,   289,
      29,    41,    29,    41,    85,    41,   155,   189,   275,   182,
     145,   275,   275,   105,   153,   125,   145,   177,    41,   252,
      39,   275,   189,   275,    39,   275,   189,   275,   152,   182,
     148,   170,   172,   275,   170,   171,   164,   275,    31,   275,
     153,    24,    41,    44,    59,    62,    73,    90,   163,   221,
     222,   223,   224,   210,   243,   125,   153,    16,    30,    42,
      61,    79,    80,    84,    94,   154,   180,   248,   260,   105,
     244,   275,   241,   153,   231,   275,     1,   198,   256,   153,
      18,   194,   248,   260,   125,   172,   171,   151,   153,   153,
     254,   153,   254,   189,   275,   155,   164,   275,   155,   275,
     155,   275,   177,   252,   150,     1,   177,   206,   207,    24,
      62,    73,    90,   215,   225,   240,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   153,   155,    41,    73,   125,
     153,   259,    72,    72,    39,   189,   275,    39,   189,   275,
     189,   275,   265,   265,   145,   231,   277,   258,   202,   275,
     153,   275,    29,   182,    29,   182,   275,    29,   182,   268,
      29,   182,   268,   269,   270,   125,   145,    11,   145,    31,
      31,   164,    85,   168,    41,    73,   223,   125,   153,   152,
     177,    24,    62,    73,    90,   227,   153,   243,   247,     1,
     252,    55,   277,   151,    58,    97,   151,   199,   153,   152,
     168,   177,   195,   236,   145,   145,   155,   268,   155,   268,
     164,   103,   150,     1,   203,   151,   105,   125,   151,    73,
     214,     1,     3,    10,    15,    17,    22,    40,    45,    70,
      82,    91,   120,   121,   122,   123,   126,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   143,
     146,   147,   148,   149,   152,   177,   216,   217,   218,   259,
     145,   270,   247,   259,   259,   275,    29,    29,   275,    29,
      29,   155,   155,   182,   182,   150,   182,    85,    39,   275,
      39,   275,    85,    39,   275,   182,    39,   275,   182,   125,
     153,   275,   275,   164,   275,   164,    31,   177,   222,   243,
     124,   251,    73,   247,   244,   155,    41,   155,    29,   164,
     252,   195,   124,   168,    39,   164,   275,   155,    39,   164,
     275,   155,   275,     1,   151,   169,   151,    35,    66,    69,
     151,   163,   184,   275,   207,   227,   152,   219,   177,   275,
     127,   219,   219,   244,    85,    39,    39,    85,    39,    39,
      39,    39,     1,   203,   275,   275,   275,   275,   275,   275,
     270,    11,    31,   164,   251,   153,   154,   180,   231,   250,
     260,   129,   233,   244,    98,   234,   275,   248,   260,   164,
     182,   153,   275,   275,   164,   275,   164,   151,   151,   184,
       1,   124,   246,   221,   153,   217,   218,   275,   275,   275,
     275,   275,   275,   151,   151,    85,    85,    85,    85,   275,
     164,   244,   251,   155,   252,   231,   232,   275,   275,   168,
     183,   230,   155,   168,    85,    85,   154,   180,   245,   260,
      87,   228,   153,   219,   219,    85,    85,    85,    85,    85,
      85,   275,   275,   275,   275,   233,   244,   231,   249,   250,
     260,    41,   155,   260,   275,   275,     1,   155,   252,   234,
     275,   249,   250,   155,   245,   260,   155,   145,   229,   230,
     155,   245,   260,   249
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
#line 465 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 470 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 471 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 478 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 483 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 484 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 503 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 507 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 508 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 509 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 511 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 512 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 513 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 514 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 515 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 516 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 517 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 518 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 523 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (6)].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[(1) - (6)].b), (yyvsp[(2) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 526 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType, (yyvsp[(6) - (7)].pblockstmt), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 528 "chapel.ypp"
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 533 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 534 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 535 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 540 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 553 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 554 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 555 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 560 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 561 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 566 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 571 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 576 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 580 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 596 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 605 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 607 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 611 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 616 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 618 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 620 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 622 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 624 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 630 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 631 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 635 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 636 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 647 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 648 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 649 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 650 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 651 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 652 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 653 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 654 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 655 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 656 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 657 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 658 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 659 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 660 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 661 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 665 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 666 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 667 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 668 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 669 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 670 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 671 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 678 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 679 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 680 "chapel.ypp"
    { (yyval.pch) = "_owned"; }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 681 "chapel.ypp"
    { (yyval.pch) = "_shared"; }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 682 "chapel.ypp"
    { (yyval.pch) = "_borrowed"; }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 683 "chapel.ypp"
    { (yyval.pch) = "_unmanaged"; }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 684 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 685 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 690 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 705 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 707 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 708 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 721 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 722 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 723 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 724 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 729 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 737 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 738 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 739 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 740 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 742 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 743 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 744 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 745 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 748 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 749 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 752 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 758 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 764 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 770 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 776 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 783 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 792 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 796 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 797 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 798 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 799 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 800 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 802 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 804 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 806 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 811 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 814 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 815 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 816 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 817 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 818 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 819 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 823 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 824 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 828 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 829 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 830 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 834 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 835 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 839 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 843 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 845 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 849 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 850 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 855 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 857 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 859 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 866 "chapel.ypp"
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

  case 178:
/* Line 1792 of yacc.c  */
#line 876 "chapel.ypp"
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

  case 179:
/* Line 1792 of yacc.c  */
#line 886 "chapel.ypp"
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

  case 180:
/* Line 1792 of yacc.c  */
#line 896 "chapel.ypp"
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

  case 181:
/* Line 1792 of yacc.c  */
#line 908 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 913 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 918 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 926 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 927 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 932 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 934 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 936 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 941 "chapel.ypp"
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

  case 190:
/* Line 1792 of yacc.c  */
#line 957 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 964 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 973 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 981 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 985 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 991 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 992 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 997 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1002 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1007 "chapel.ypp"
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

  case 200:
/* Line 1792 of yacc.c  */
#line 1027 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1033 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1040 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[(2) - (2)].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1046 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[(2) - (2)].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1052 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1063 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1069 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1076 "chapel.ypp"
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

  case 208:
/* Line 1792 of yacc.c  */
#line 1102 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1106 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1111 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1115 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1120 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1127 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1132 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1133 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1134 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1135 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1136 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1137 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1139 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1140 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1141 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1142 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1143 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1144 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1145 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1146 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1147 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1148 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1149 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1150 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1151 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1152 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1153 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1154 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1155 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1156 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1160 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1161 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1162 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1163 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1164 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1165 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1166 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1167 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1168 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1169 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1170 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1171 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1175 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1176 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1180 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1184 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1185 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1186 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1191 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1193 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1195 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1197 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1199 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1203 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1204 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1208 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1209 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1210 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1213 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1214 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1215 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1216 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1220 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1221 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1225 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1230 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1234 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1235 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1236 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1237 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1238 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1239 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1243 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1244 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1247 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1253 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1258 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1260 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1264 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1269 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1270 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1284 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 306:
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

  case 307:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
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

  case 308:
/* Line 1792 of yacc.c  */
#line 1318 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1320 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1322 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1327 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1335 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1343 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1351 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1360 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1370 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1377 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1387 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1392 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1394 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1396 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1401 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1403 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1405 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1411 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1412 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1413 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1419 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1421 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1423 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1427 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1431 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1437 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1444 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1445 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1446 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1447 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1448 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1453 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1454 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1455 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1456 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1457 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1486 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1494 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1500 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1501 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1517 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1526 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1527 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1534 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1536 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1541 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1546 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1552 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1556 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1561 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1566 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1568 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1569 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1573 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1574 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1591 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1593 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1597 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1601 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1605 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1607 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1609 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1611 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1616 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1618 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1620 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1622 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1624 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1626 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1630 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1634 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1636 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1638 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1640 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1647 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1653 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1659 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1665 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1674 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1683 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1699 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1703 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1708 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1712 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1713 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1721 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1722 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1727 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1731 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1735 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1741 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1742 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1743 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1744 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1745 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1746 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1752 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1757 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1762 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1778 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1780 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1782 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1784 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1786 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1788 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1790 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1792 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1796 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1797 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1800 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1801 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1802 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1824 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1825 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1826 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1830 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1831 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1832 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1840 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1841 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1842 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1843 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1847 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1848 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1849 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1850 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1851 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1852 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1853 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1854 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1855 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1857 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1861 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1868 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1869 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1873 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1874 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1875 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1876 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1877 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1878 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1879 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 1880 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1881 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1882 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1883 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1884 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1885 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1886 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1887 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1888 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1889 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1890 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1891 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1892 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 1893 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 1894 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 523:
/* Line 1792 of yacc.c  */
#line 1895 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 1899 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 1900 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 1901 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 1902 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 1903 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 1904 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 1908 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 1909 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 1910 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 1911 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 1915 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 1916 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 1917 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 1918 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 1923 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 1924 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 1925 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 1926 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 1927 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 1928 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 1929 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 9096 "bison-chapel.cpp"
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


