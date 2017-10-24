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

  struct IntentExpr {
    Expr*     iVar;
    IntentTag tfIntent; // undefined for a reduce intent
    Expr*     riExp;    // non-NULL for a reduce intent
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
    IntentExpr                pIntentExpr;
    std::vector<OnlyRename*>* ponlylist;
  };

  #endif

/* Line 387 of yacc.c  */
#line 136 "chapel.ypp"

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
#line 158 "chapel.ypp"

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
#line 240 "bison-chapel.cpp"

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
     TPARAM = 310,
     TPLUSPLUS = 311,
     TPRAGMA = 312,
     TPRIMITIVE = 313,
     TPRIVATE = 314,
     TPROC = 315,
     TPROTOTYPE = 316,
     TPUBLIC = 317,
     TRECORD = 318,
     TREDUCE = 319,
     TREF = 320,
     TREQUIRE = 321,
     TRETURN = 322,
     TSCAN = 323,
     TSELECT = 324,
     TSERIAL = 325,
     TSINGLE = 326,
     TSPARSE = 327,
     TSUBDOMAIN = 328,
     TSYNC = 329,
     TTHEN = 330,
     TTHROW = 331,
     TTHROWS = 332,
     TTRY = 333,
     TTRYBANG = 334,
     TTYPE = 335,
     TUNDERSCORE = 336,
     TUNION = 337,
     TUSE = 338,
     TVAR = 339,
     TWHEN = 340,
     TWHERE = 341,
     TWHILE = 342,
     TWITH = 343,
     TYIELD = 344,
     TZIP = 345,
     TALIAS = 346,
     TAND = 347,
     TASSIGN = 348,
     TASSIGNBAND = 349,
     TASSIGNBOR = 350,
     TASSIGNBXOR = 351,
     TASSIGNDIVIDE = 352,
     TASSIGNEXP = 353,
     TASSIGNLAND = 354,
     TASSIGNLOR = 355,
     TASSIGNMINUS = 356,
     TASSIGNMOD = 357,
     TASSIGNMULTIPLY = 358,
     TASSIGNPLUS = 359,
     TASSIGNSL = 360,
     TASSIGNSR = 361,
     TBAND = 362,
     TBNOT = 363,
     TBOR = 364,
     TBXOR = 365,
     TCOLON = 366,
     TCOMMA = 367,
     TDIVIDE = 368,
     TDOT = 369,
     TDOTDOT = 370,
     TDOTDOTDOT = 371,
     TEQUAL = 372,
     TEXP = 373,
     TGREATER = 374,
     TGREATEREQUAL = 375,
     THASH = 376,
     TLESS = 377,
     TLESSEQUAL = 378,
     TMINUS = 379,
     TMOD = 380,
     TNOT = 381,
     TNOTEQUAL = 382,
     TOR = 383,
     TPLUS = 384,
     TQUESTION = 385,
     TSEMI = 386,
     TSHIFTLEFT = 387,
     TSHIFTRIGHT = 388,
     TSTAR = 389,
     TSWAP = 390,
     TASSIGNREDUCE = 391,
     TIO = 392,
     TLCBR = 393,
     TRCBR = 394,
     TLP = 395,
     TRP = 396,
     TLSBR = 397,
     TRSBR = 398,
     TNOELSE = 399,
     TUMINUS = 400,
     TUPLUS = 401
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
#line 189 "chapel.ypp"

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
#line 197 "chapel.ypp"

  #include "build.h"
  #include "CatchStmt.h"
  #include "DeferStmt.h"
  #include "DoWhileStmt.h"
  #include "driver.h"
  #include "flex-chapel.h"
  #include "ForallStmt.h"
  #include "ForLoop.h"
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

    clean_exit(1);
  }


/* Line 391 of yacc.c  */
#line 534 "bison-chapel.cpp"

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
#define YYLAST   13064

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  147
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  124
/* YYNRULES -- Number of rules.  */
#define YYNRULES  485
/* YYNRULES -- Number of states.  */
#define YYNSTATES  915

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   401

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
     145,   146
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    14,    17,    21,
      23,    25,    27,    29,    31,    33,    35,    37,    39,    41,
      43,    45,    47,    49,    52,    55,    59,    63,    67,    71,
      75,    79,    83,    86,    90,    94,    97,   100,   104,   106,
     113,   121,   122,   124,   126,   127,   129,   132,   136,   138,
     141,   143,   147,   151,   157,   158,   160,   162,   164,   168,
     174,   180,   184,   189,   194,   199,   204,   209,   214,   219,
     224,   229,   234,   239,   244,   249,   254,   259,   264,   265,
     267,   269,   271,   273,   276,   278,   281,   285,   287,   289,
     292,   295,   297,   299,   301,   303,   305,   307,   311,   317,
     323,   326,   329,   335,   339,   346,   353,   358,   364,   370,
     374,   378,   385,   391,   398,   404,   411,   415,   420,   427,
     435,   442,   450,   455,   461,   466,   471,   475,   482,   488,
     491,   495,   499,   502,   505,   509,   513,   514,   517,   520,
     524,   530,   532,   536,   540,   546,   547,   550,   554,   557,
     561,   568,   577,   579,   581,   583,   584,   587,   588,   591,
     595,   601,   603,   605,   608,   612,   614,   618,   619,   620,
     629,   630,   632,   635,   638,   639,   640,   651,   655,   659,
     665,   671,   673,   677,   679,   682,   684,   686,   688,   690,
     692,   694,   696,   698,   700,   702,   704,   706,   708,   710,
     712,   714,   716,   718,   720,   722,   724,   726,   728,   730,
     732,   734,   736,   738,   740,   742,   744,   746,   748,   750,
     752,   754,   755,   759,   763,   764,   766,   770,   775,   780,
     787,   794,   795,   797,   799,   801,   803,   805,   808,   811,
     813,   815,   817,   818,   820,   822,   825,   827,   829,   831,
     833,   834,   836,   839,   841,   843,   845,   846,   848,   850,
     852,   854,   856,   859,   861,   862,   864,   867,   870,   871,
     874,   878,   883,   888,   891,   896,   897,   900,   903,   908,
     913,   918,   924,   929,   930,   932,   934,   936,   940,   944,
     950,   952,   954,   958,   960,   963,   967,   968,   971,   974,
     975,   978,   981,   983,   988,   993,  1000,  1002,  1003,  1005,
    1007,  1011,  1016,  1020,  1025,  1032,  1033,  1036,  1039,  1042,
    1045,  1048,  1051,  1053,  1055,  1059,  1063,  1065,  1069,  1071,
    1073,  1075,  1079,  1083,  1084,  1086,  1088,  1092,  1096,  1100,
    1102,  1104,  1106,  1108,  1110,  1112,  1114,  1116,  1119,  1124,
    1129,  1134,  1140,  1143,  1146,  1153,  1160,  1165,  1175,  1185,
    1193,  1200,  1207,  1212,  1222,  1232,  1240,  1245,  1252,  1259,
    1269,  1279,  1286,  1288,  1290,  1292,  1294,  1296,  1298,  1300,
    1302,  1306,  1307,  1309,  1314,  1316,  1320,  1325,  1327,  1331,
    1334,  1338,  1342,  1344,  1348,  1351,  1356,  1358,  1360,  1362,
    1364,  1366,  1368,  1370,  1372,  1377,  1381,  1385,  1388,  1391,
    1393,  1396,  1399,  1401,  1403,  1405,  1407,  1409,  1411,  1413,
    1418,  1423,  1428,  1432,  1436,  1440,  1444,  1449,  1453,  1458,
    1460,  1462,  1464,  1466,  1468,  1472,  1477,  1481,  1486,  1490,
    1495,  1499,  1505,  1509,  1513,  1517,  1521,  1525,  1529,  1533,
    1537,  1541,  1545,  1549,  1553,  1557,  1561,  1565,  1569,  1573,
    1577,  1581,  1585,  1589,  1593,  1597,  1600,  1603,  1606,  1609,
    1612,  1615,  1619,  1623,  1627,  1631,  1635,  1639,  1643,  1647,
    1649,  1651,  1653,  1655,  1657,  1659
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     148,     0,    -1,   149,    -1,    -1,   149,   150,    -1,   152,
      -1,   151,   152,    -1,    57,     7,    -1,   151,    57,     7,
      -1,   153,    -1,   156,    -1,   161,    -1,   162,    -1,   169,
      -1,   163,    -1,   172,    -1,   175,    -1,   173,    -1,   182,
      -1,   176,    -1,   177,    -1,   181,    -1,   168,    -1,   247,
     131,    -1,    12,   152,    -1,    13,   248,   152,    -1,    14,
     164,   131,    -1,    18,   248,   156,    -1,    22,   164,   131,
      -1,    24,   236,   131,    -1,    42,   165,   152,    -1,    45,
     257,   167,    -1,    45,   167,    -1,    51,   257,   167,    -1,
      70,   257,   167,    -1,    70,   167,    -1,    74,   152,    -1,
      89,   257,   131,    -1,     1,    -1,   154,   155,    47,   165,
     138,   139,    -1,   154,   155,    47,   165,   138,   157,   139,
      -1,    -1,    62,    -1,    59,    -1,    -1,    61,    -1,   138,
     139,    -1,   138,   157,   139,    -1,   150,    -1,   157,   150,
      -1,   257,    -1,   257,    11,   257,    -1,   158,   112,   257,
      -1,   158,   112,   257,    11,   257,    -1,    -1,   158,    -1,
     134,    -1,   158,    -1,    83,   235,   131,    -1,    83,   257,
      30,   160,   131,    -1,    83,   257,    52,   159,   131,    -1,
      66,   235,   131,    -1,   259,    93,   258,   131,    -1,   259,
     104,   258,   131,    -1,   259,   101,   258,   131,    -1,   259,
     103,   258,   131,    -1,   259,    97,   258,   131,    -1,   259,
     102,   258,   131,    -1,   259,    98,   258,   131,    -1,   259,
      94,   258,   131,    -1,   259,    95,   258,   131,    -1,   259,
      96,   258,   131,    -1,   259,   106,   258,   131,    -1,   259,
     105,   258,   131,    -1,   259,   135,   258,   131,    -1,   259,
     136,   258,   131,    -1,   259,    99,   258,   131,    -1,   259,
     100,   258,   131,    -1,    -1,   165,    -1,     3,    -1,   164,
      -1,     7,    -1,    26,   152,    -1,   156,    -1,    67,   131,
      -1,    67,   258,   131,    -1,   131,    -1,   170,    -1,    62,
     170,    -1,    59,   170,    -1,   189,    -1,   220,    -1,   185,
      -1,   171,    -1,   197,    -1,   223,    -1,    35,   257,   131,
      -1,    35,   257,    30,   160,   131,    -1,    35,   257,    52,
     159,   131,    -1,    35,   223,    -1,    32,     9,    -1,    26,
     152,    87,   257,   131,    -1,    87,   257,   167,    -1,    19,
     257,    37,   257,   248,   167,    -1,    19,   257,    37,   174,
     248,   167,    -1,    19,   257,   248,   167,    -1,    33,   257,
      37,   257,   167,    -1,    33,   257,    37,   174,   167,    -1,
      33,   257,   167,    -1,    33,   174,   167,    -1,    33,    55,
     165,    37,   257,   167,    -1,    34,   257,    37,   257,   167,
      -1,    34,   257,    37,   257,   251,   167,    -1,    34,   257,
      37,   174,   167,    -1,    34,   257,    37,   174,   251,   167,
      -1,    34,   257,   167,    -1,    34,   257,   251,   167,    -1,
     142,   235,    37,   257,   143,   152,    -1,   142,   235,    37,
     257,   251,   143,   152,    -1,   142,   235,    37,   174,   143,
     152,    -1,   142,   235,    37,   174,   251,   143,   152,    -1,
     142,   235,   143,   152,    -1,   142,   235,   251,   143,   152,
      -1,    90,   140,   235,   141,    -1,    36,   257,    75,   152,
      -1,    36,   257,   156,    -1,    36,   257,    75,   152,    28,
     152,    -1,    36,   257,   156,    28,   152,    -1,    23,   152,
      -1,    78,   257,   131,    -1,    79,   257,   131,    -1,    78,
     163,    -1,    79,   163,    -1,    78,   156,   178,    -1,    79,
     156,   178,    -1,    -1,   178,   179,    -1,    16,   156,    -1,
      16,   180,   156,    -1,    16,   140,   180,   141,   156,    -1,
     165,    -1,   165,   111,   257,    -1,    76,   257,   131,    -1,
      69,   257,   138,   183,   139,    -1,    -1,   183,   184,    -1,
      85,   235,   167,    -1,    53,   152,    -1,    53,    26,   152,
      -1,   186,   165,   187,   138,   188,   139,    -1,    32,   166,
     186,   165,   187,   138,   188,   139,    -1,    17,    -1,    63,
      -1,    82,    -1,    -1,   111,   235,    -1,    -1,   188,   169,
      -1,   188,   151,   169,    -1,   190,   165,   138,   191,   139,
      -1,    29,    -1,   192,    -1,   191,   112,    -1,   191,   112,
     192,    -1,   165,    -1,   165,    93,   257,    -1,    -1,    -1,
      43,   194,   205,   195,   212,   230,   219,   215,    -1,    -1,
      39,    -1,    31,   166,    -1,    32,   166,    -1,    -1,    -1,
     196,   211,   198,   200,   199,   212,   230,   213,   219,   214,
      -1,   210,   202,   204,    -1,   210,   203,   204,    -1,   210,
     201,   114,   202,   204,    -1,   210,   201,   114,   203,   204,
      -1,   242,    -1,   140,   257,   141,    -1,   165,    -1,   108,
     165,    -1,   107,    -1,   109,    -1,   110,    -1,   108,    -1,
     117,    -1,   127,    -1,   123,    -1,   120,    -1,   122,    -1,
     119,    -1,   129,    -1,   124,    -1,   134,    -1,   113,    -1,
     132,    -1,   133,    -1,   125,    -1,   118,    -1,   126,    -1,
      15,    -1,   121,    -1,    10,    -1,   135,    -1,   137,    -1,
      93,    -1,   104,    -1,   101,    -1,   103,    -1,    97,    -1,
     102,    -1,    98,    -1,    94,    -1,    95,    -1,    96,    -1,
     106,    -1,   105,    -1,    -1,   140,   206,   141,    -1,   140,
     206,   141,    -1,    -1,   207,    -1,   206,   112,   207,    -1,
     208,   165,   234,   229,    -1,   208,   165,   234,   218,    -1,
     208,   140,   228,   141,   234,   229,    -1,   208,   140,   228,
     141,   234,   218,    -1,    -1,   209,    -1,    37,    -1,    40,
      -1,    54,    -1,    21,    -1,    21,    37,    -1,    21,    65,
      -1,    55,    -1,    65,    -1,    80,    -1,    -1,    55,    -1,
      65,    -1,    21,    65,    -1,    21,    -1,    80,    -1,    60,
      -1,    41,    -1,    -1,    21,    -1,    21,    65,    -1,    65,
      -1,    55,    -1,    80,    -1,    -1,    77,    -1,   131,    -1,
     215,    -1,   156,    -1,   168,    -1,   130,   165,    -1,   130,
      -1,    -1,   216,    -1,   116,   257,    -1,   116,   217,    -1,
      -1,    86,   257,    -1,    80,   221,   131,    -1,    20,    80,
     221,   131,    -1,    32,    80,   221,   131,    -1,   165,   222,
      -1,   165,   222,   112,   221,    -1,    -1,    93,   243,    -1,
      93,   231,    -1,   224,    55,   225,   131,    -1,   224,    21,
     225,   131,    -1,   224,    65,   225,   131,    -1,   224,    21,
      65,   225,   131,    -1,   224,    84,   225,   131,    -1,    -1,
      20,    -1,    32,    -1,   226,    -1,   225,   112,   226,    -1,
     165,   230,   229,    -1,   140,   228,   141,   230,   229,    -1,
      81,    -1,   165,    -1,   140,   228,   141,    -1,   227,    -1,
     227,   112,    -1,   227,   112,   228,    -1,    -1,    93,    50,
      -1,    93,   258,    -1,    -1,   111,   243,    -1,   111,   231,
      -1,     1,    -1,   142,   235,   143,   243,    -1,   142,   235,
     143,   231,    -1,   142,   235,    37,   257,   143,   243,    -1,
       1,    -1,    -1,   243,    -1,   216,    -1,   142,   143,   232,
      -1,   142,   235,   143,   232,    -1,   142,   143,   233,    -1,
     142,   235,   143,   233,    -1,   142,   235,    37,   257,   143,
     232,    -1,    -1,   111,   243,    -1,   111,   216,    -1,   111,
      27,    -1,   111,    71,    -1,   111,    74,    -1,   111,   233,
      -1,   257,    -1,   216,    -1,   235,   112,   257,    -1,   235,
     112,   216,    -1,   257,    -1,   236,   112,   257,    -1,    81,
      -1,   258,    -1,   216,    -1,   237,   112,   237,    -1,   238,
     112,   237,    -1,    -1,   240,    -1,   241,    -1,   240,   112,
     241,    -1,   165,    93,   216,    -1,   165,    93,   258,    -1,
     216,    -1,   258,    -1,   165,    -1,   246,    -1,   259,    -1,
     245,    -1,   267,    -1,   266,    -1,    71,   257,    -1,    38,
     140,   239,   141,    -1,    27,   140,   239,   141,    -1,    73,
     140,   239,   141,    -1,    72,    73,   140,   239,   141,    -1,
      12,   257,    -1,    74,   257,    -1,    33,   257,    37,   257,
      26,   257,    -1,    33,   257,    37,   174,    26,   257,    -1,
      33,   257,    26,   257,    -1,    33,   257,    37,   257,    26,
      36,   257,    75,   257,    -1,    33,   257,    37,   174,    26,
      36,   257,    75,   257,    -1,    33,   257,    26,    36,   257,
      75,   257,    -1,    34,   257,    37,   257,    26,   257,    -1,
      34,   257,    37,   174,    26,   257,    -1,    34,   257,    26,
     257,    -1,    34,   257,    37,   257,    26,    36,   257,    75,
     257,    -1,    34,   257,    37,   174,    26,    36,   257,    75,
     257,    -1,    34,   257,    26,    36,   257,    75,   257,    -1,
     142,   235,   143,   257,    -1,   142,   235,    37,   257,   143,
     257,    -1,   142,   235,    37,   174,   143,   257,    -1,   142,
     235,    37,   257,   143,    36,   257,    75,   257,    -1,   142,
     235,    37,   174,   143,    36,   257,    75,   257,    -1,    36,
     257,    75,   257,    28,   257,    -1,    49,    -1,   246,    -1,
     242,    -1,   262,    -1,   261,    -1,   193,    -1,   255,    -1,
     256,    -1,   254,   137,   257,    -1,    -1,   249,    -1,    88,
     140,   250,   141,    -1,   253,    -1,   250,   112,   253,    -1,
      88,   140,   252,   141,    -1,   253,    -1,   252,   112,   253,
      -1,   209,   242,    -1,   257,    64,   242,    -1,   270,    64,
     242,    -1,   259,    -1,   254,   137,   257,    -1,    48,   257,
      -1,    44,   225,    37,   257,    -1,   264,    -1,   243,    -1,
     244,    -1,   268,    -1,   269,    -1,   193,    -1,   255,    -1,
     256,    -1,   140,   116,   257,   141,    -1,   257,   111,   257,
      -1,   257,   115,   257,    -1,   257,   115,    -1,   115,   257,
      -1,   115,    -1,    78,   257,    -1,    79,   257,    -1,   257,
      -1,   242,    -1,   261,    -1,   262,    -1,   263,    -1,   259,
      -1,   193,    -1,   260,   140,   239,   141,    -1,   260,   142,
     239,   143,    -1,    58,   140,   239,   141,    -1,   257,   114,
     165,    -1,   257,   114,    80,    -1,   257,   114,    27,    -1,
     140,   237,   141,    -1,   140,   237,   112,   141,    -1,   140,
     238,   141,    -1,   140,   238,   112,   141,    -1,     4,    -1,
       5,    -1,     6,    -1,     7,    -1,     8,    -1,   138,   235,
     139,    -1,   138,   235,   112,   139,    -1,   142,   235,   143,
      -1,   142,   235,   112,   143,    -1,   142,   265,   143,    -1,
     142,   265,   112,   143,    -1,   257,    91,   257,    -1,   265,
     112,   257,    91,   257,    -1,   257,   129,   257,    -1,   257,
     124,   257,    -1,   257,   134,   257,    -1,   257,   113,   257,
      -1,   257,   132,   257,    -1,   257,   133,   257,    -1,   257,
     125,   257,    -1,   257,   117,   257,    -1,   257,   127,   257,
      -1,   257,   123,   257,    -1,   257,   120,   257,    -1,   257,
     122,   257,    -1,   257,   119,   257,    -1,   257,   107,   257,
      -1,   257,   109,   257,    -1,   257,   110,   257,    -1,   257,
      92,   257,    -1,   257,   128,   257,    -1,   257,   118,   257,
      -1,   257,    15,   257,    -1,   257,    10,   257,    -1,   257,
     121,   257,    -1,   257,    25,   257,    -1,   129,   257,    -1,
     124,   257,    -1,    46,   257,    -1,    56,   257,    -1,   126,
     257,    -1,   108,   257,    -1,   257,    64,   257,    -1,   257,
      64,   174,    -1,   270,    64,   257,    -1,   270,    64,   174,
      -1,   257,    68,   257,    -1,   257,    68,   174,    -1,   270,
      68,   257,    -1,   270,    68,   174,    -1,   129,    -1,   134,
      -1,    92,    -1,   128,    -1,   107,    -1,   109,    -1,   110,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   441,   441,   446,   447,   453,   454,   459,   460,   465,
     466,   467,   468,   469,   470,   471,   472,   473,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   498,
     501,   506,   507,   508,   512,   513,   526,   527,   532,   533,
     538,   543,   548,   552,   559,   564,   568,   573,   577,   578,
     579,   583,   587,   589,   591,   593,   595,   597,   599,   601,
     603,   605,   607,   609,   611,   613,   615,   617,   622,   623,
     627,   631,   632,   636,   637,   641,   642,   646,   647,   648,
     649,   650,   651,   652,   653,   657,   658,   662,   663,   664,
     665,   669,   684,   685,   686,   687,   688,   689,   690,   691,
     692,   693,   694,   695,   696,   697,   698,   699,   700,   706,
     712,   718,   724,   731,   741,   745,   746,   747,   748,   752,
     755,   756,   757,   758,   759,   760,   764,   765,   769,   770,
     771,   775,   776,   780,   783,   788,   789,   793,   795,   797,
     804,   814,   827,   832,   837,   845,   846,   851,   852,   854,
     859,   878,   887,   895,   899,   906,   907,   912,   917,   911,
     942,   948,   955,   963,   975,   981,   974,  1009,  1013,  1018,
    1022,  1030,  1031,  1035,  1036,  1037,  1038,  1039,  1040,  1041,
    1042,  1043,  1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,
    1052,  1053,  1054,  1055,  1056,  1057,  1058,  1059,  1060,  1064,
    1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,
    1075,  1079,  1080,  1084,  1088,  1089,  1090,  1094,  1096,  1098,
    1100,  1105,  1106,  1110,  1111,  1112,  1113,  1114,  1115,  1116,
    1117,  1118,  1122,  1123,  1124,  1125,  1126,  1127,  1131,  1132,
    1136,  1137,  1138,  1139,  1140,  1141,  1145,  1146,  1149,  1150,
    1154,  1155,  1159,  1161,  1166,  1167,  1171,  1172,  1176,  1177,
    1181,  1183,  1185,  1190,  1203,  1220,  1221,  1223,  1228,  1236,
    1244,  1252,  1261,  1271,  1272,  1273,  1277,  1278,  1286,  1288,
    1293,  1295,  1297,  1302,  1304,  1306,  1313,  1314,  1315,  1319,
    1320,  1321,  1322,  1342,  1346,  1350,  1358,  1362,  1363,  1364,
    1368,  1370,  1376,  1378,  1380,  1385,  1386,  1387,  1388,  1389,
    1390,  1391,  1397,  1398,  1399,  1400,  1404,  1405,  1409,  1410,
    1411,  1415,  1416,  1420,  1421,  1425,  1426,  1430,  1431,  1432,
    1433,  1437,  1448,  1449,  1450,  1451,  1452,  1453,  1455,  1457,
    1459,  1461,  1463,  1465,  1470,  1472,  1474,  1476,  1478,  1480,
    1482,  1484,  1486,  1488,  1490,  1492,  1494,  1501,  1507,  1513,
    1519,  1528,  1538,  1546,  1547,  1548,  1549,  1550,  1551,  1552,
    1553,  1558,  1559,  1563,  1567,  1570,  1575,  1579,  1582,  1587,
    1591,  1592,  1596,  1597,  1602,  1607,  1615,  1616,  1617,  1618,
    1619,  1620,  1621,  1622,  1623,  1625,  1627,  1629,  1631,  1633,
    1638,  1639,  1640,  1643,  1644,  1645,  1646,  1657,  1658,  1662,
    1663,  1664,  1668,  1669,  1670,  1678,  1679,  1680,  1681,  1685,
    1686,  1687,  1688,  1689,  1690,  1691,  1692,  1693,  1694,  1698,
    1706,  1707,  1711,  1712,  1713,  1714,  1715,  1716,  1717,  1718,
    1719,  1720,  1721,  1722,  1723,  1724,  1725,  1726,  1727,  1728,
    1729,  1730,  1731,  1732,  1733,  1737,  1738,  1739,  1740,  1741,
    1742,  1746,  1747,  1748,  1749,  1753,  1754,  1755,  1756,  1761,
    1762,  1763,  1764,  1765,  1766,  1767
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
  "TONLY", "TOTHERWISE", "TOUT", "TPARAM", "TPLUSPLUS", "TPRAGMA",
  "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC", "TRECORD",
  "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT", "TSERIAL",
  "TSINGLE", "TSPARSE", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHROW",
  "TTHROWS", "TTRY", "TTRYBANG", "TTYPE", "TUNDERSCORE", "TUNION", "TUSE",
  "TVAR", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS",
  "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR",
  "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR",
  "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY", "TASSIGNPLUS",
  "TASSIGNSL", "TASSIGNSR", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON",
  "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP",
  "TGREATER", "TGREATEREQUAL", "THASH", "TLESS", "TLESSEQUAL", "TMINUS",
  "TMOD", "TNOT", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI",
  "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP", "TASSIGNREDUCE", "TIO",
  "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR", "TRSBR", "TNOELSE", "TUMINUS",
  "TUPLUS", "$accept", "program", "toplevel_stmt_ls", "toplevel_stmt",
  "pragma_ls", "stmt", "module_decl_stmt", "access_control",
  "opt_prototype", "block_stmt", "stmt_ls", "only_ls", "opt_only_ls",
  "except_ls", "use_stmt", "require_stmt", "assignment_stmt", "opt_ident",
  "ident", "opt_ident_or_string", "do_stmt", "return_stmt",
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
     395,   396,   397,   398,   399,   400,   401
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   147,   148,   149,   149,   150,   150,   151,   151,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   153,
     153,   154,   154,   154,   155,   155,   156,   156,   157,   157,
     158,   158,   158,   158,   159,   159,   160,   160,   161,   161,
     161,   162,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   164,   164,
     165,   166,   166,   167,   167,   168,   168,   169,   169,   169,
     169,   169,   169,   169,   169,   170,   170,   171,   171,   171,
     171,   172,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   174,   175,   175,   175,   175,   176,
     177,   177,   177,   177,   177,   177,   178,   178,   179,   179,
     179,   180,   180,   181,   182,   183,   183,   184,   184,   184,
     185,   185,   186,   186,   186,   187,   187,   188,   188,   188,
     189,   190,   191,   191,   191,   192,   192,   194,   195,   193,
     196,   196,   196,   196,   198,   199,   197,   200,   200,   200,
     200,   201,   201,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   204,   204,   205,   206,   206,   206,   207,   207,   207,
     207,   208,   208,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   210,   210,   210,   210,   210,   210,   211,   211,
     212,   212,   212,   212,   212,   212,   213,   213,   214,   214,
     215,   215,   216,   216,   217,   217,   218,   218,   219,   219,
     220,   220,   220,   221,   221,   222,   222,   222,   223,   223,
     223,   223,   223,   224,   224,   224,   225,   225,   226,   226,
     227,   227,   227,   228,   228,   228,   229,   229,   229,   230,
     230,   230,   230,   231,   231,   231,   231,   232,   232,   232,
     233,   233,   233,   233,   233,   234,   234,   234,   234,   234,
     234,   234,   235,   235,   235,   235,   236,   236,   237,   237,
     237,   238,   238,   239,   239,   240,   240,   241,   241,   241,
     241,   242,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   245,   246,   247,   247,   247,   247,   247,   247,   247,
     247,   248,   248,   249,   250,   250,   251,   252,   252,   253,
     253,   253,   254,   254,   255,   256,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     258,   258,   258,   259,   259,   259,   259,   260,   260,   261,
     261,   261,   262,   262,   262,   263,   263,   263,   263,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     265,   265,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   267,   267,   267,   267,   267,
     267,   268,   268,   268,   268,   269,   269,   269,   269,   270,
     270,   270,   270,   270,   270,   270
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     3,     2,     2,     3,     1,     6,
       7,     0,     1,     1,     0,     1,     2,     3,     1,     2,
       1,     3,     3,     5,     0,     1,     1,     1,     3,     5,
       5,     3,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     0,     1,
       1,     1,     1,     2,     1,     2,     3,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     3,     5,     5,
       2,     2,     5,     3,     6,     6,     4,     5,     5,     3,
       3,     6,     5,     6,     5,     6,     3,     4,     6,     7,
       6,     7,     4,     5,     4,     4,     3,     6,     5,     2,
       3,     3,     2,     2,     3,     3,     0,     2,     2,     3,
       5,     1,     3,     3,     5,     0,     2,     3,     2,     3,
       6,     8,     1,     1,     1,     0,     2,     0,     2,     3,
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
       2,     2,     1,     4,     4,     6,     1,     0,     1,     1,
       3,     4,     3,     4,     6,     0,     2,     2,     2,     2,
       2,     2,     1,     1,     3,     3,     1,     3,     1,     1,
       1,     3,     3,     0,     1,     1,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     4,     4,
       4,     5,     2,     2,     6,     6,     4,     9,     9,     7,
       6,     6,     4,     9,     9,     7,     4,     6,     6,     9,
       9,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     1,     4,     1,     3,     4,     1,     3,     2,
       3,     3,     1,     3,     2,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     3,     3,     2,     2,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     4,     3,     3,     3,     3,     4,     3,     4,     1,
       1,     1,     1,     1,     3,     4,     3,     4,     3,     4,
       3,     5,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    38,    80,   429,   430,   431,   432,
     433,     0,   381,    78,   152,   381,     0,   284,    78,     0,
       0,     0,     0,   161,    78,    78,     0,     0,   283,     0,
       0,   171,     0,   167,     0,     0,     0,     0,   372,     0,
       0,     0,     0,   283,   283,   153,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   154,     0,
       0,     0,   481,   483,     0,   484,   485,   409,     0,     0,
     482,   479,    87,   480,     0,     0,     0,     4,     0,     5,
       9,    44,    10,    11,    12,    14,   341,    22,    13,    88,
      94,    15,    17,    16,    19,    20,    21,    18,    93,     0,
      91,     0,   401,     0,    95,    92,    96,     0,   413,   397,
     398,   344,   342,     0,     0,   402,   403,     0,   343,     0,
     414,   415,   416,   396,   346,   345,   399,   400,     0,    24,
     352,     0,     0,   382,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,   401,   413,   342,   402,   403,   381,
     343,   414,   415,     0,     0,   129,     0,   326,     0,   333,
      82,    81,   172,   101,     0,   173,     0,     0,     0,     0,
       0,   284,   285,   100,     0,     0,   333,     0,     0,     0,
       0,     0,   286,     0,    84,    32,     0,   467,   394,     0,
     468,     7,   333,   285,    90,    89,   263,   323,     0,   322,
       0,     0,    85,   412,     0,     0,    35,     0,   347,     0,
     333,    36,   353,     0,   136,   132,     0,   343,   136,   133,
       0,   275,     0,     0,   322,     0,     0,   470,   408,   466,
     469,   465,    46,    48,     0,     0,   328,     0,   330,     0,
       0,   329,     0,   322,     0,     0,     6,    45,     0,   155,
       0,   249,   248,   174,     0,     0,     0,     0,    23,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   407,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   333,   333,     0,     0,     0,    25,
      26,     0,    27,     0,     0,     0,     0,     0,     0,     0,
      28,     0,    29,     0,   341,   339,     0,   334,   335,   340,
       0,     0,     0,     0,   110,     0,     0,   109,     0,     0,
       0,   116,     0,     0,    54,    97,     0,   126,     0,    30,
     224,   168,   290,     0,   291,   293,     0,   302,     0,   296,
       0,     0,    83,    31,    33,     0,   173,   262,     0,    61,
     410,   411,    86,   145,    34,   333,     0,   143,   134,   130,
     135,   131,     0,   273,   270,    58,     0,    54,   103,    37,
      47,    49,     0,   434,     0,     0,   425,     0,   427,     0,
       0,     0,     0,     0,     0,   438,     8,     0,     0,     0,
       0,   242,     0,     0,     0,     0,     0,   380,   462,   461,
     464,   472,   471,   476,   475,   458,   455,   456,   457,   405,
     445,   424,   423,   422,   406,   449,   460,   454,   452,   463,
     453,   451,   443,   448,   450,   459,   442,   446,   447,   444,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   474,   473,
     478,   477,   236,   233,   234,   235,   239,   240,   241,     0,
       0,   384,     0,     0,     0,     0,     0,     0,     0,     0,
     436,   381,   381,   106,   271,   327,     0,     0,   349,     0,
     272,   155,     0,     0,     0,   356,     0,     0,     0,   362,
       0,     0,     0,   117,   480,    57,     0,    50,    55,     0,
     125,     0,     0,   348,     0,   225,     0,   232,   250,     0,
     294,     0,   306,     0,   301,   397,     0,   288,   395,   287,
     421,   325,   324,     0,     0,   350,     0,   137,   277,   397,
       0,     0,     0,   435,   404,   426,   331,   428,   332,     0,
       0,   437,   122,   366,     0,   440,   439,     0,     0,   156,
     157,   165,     0,   162,   246,   243,   244,   247,   175,     0,
       0,   279,   278,   280,   282,    62,    69,    70,    71,    66,
      68,    76,    77,    64,    67,    65,    63,    73,    72,    74,
      75,   419,   420,   237,   238,   389,     0,   383,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     102,   337,   338,   336,     0,     0,   124,     0,     0,   108,
       0,   107,     0,     0,   114,     0,     0,   112,     0,     0,
     387,     0,    98,     0,    99,     0,     0,   128,   231,   223,
       0,   315,   251,   254,   253,   255,     0,   292,   295,   296,
       0,   297,   298,     0,     0,   144,   146,   351,     0,   138,
     141,     0,   274,    59,    60,     0,     0,     0,     0,   123,
       0,     0,   283,     0,   163,   160,   245,   250,   206,   204,
     209,   216,   217,   218,   213,   215,   211,   214,   212,   210,
     220,   219,   185,   188,   186,   187,   198,   189,   202,   194,
     192,   205,   193,   191,   196,   201,   203,   190,   195,   199,
     200,   197,   207,   208,     0,   183,     0,   221,   221,   181,
     281,   385,   413,   413,     0,     0,     0,     0,     0,     0,
       0,     0,   105,   104,   157,   111,     0,     0,   355,     0,
     354,     0,     0,   361,   115,     0,   360,   113,     0,   386,
      52,    51,   127,   371,   226,     0,     0,   296,   252,   268,
     289,     0,     0,     0,   148,     0,     0,     0,   139,     0,
     120,   368,     0,     0,   118,   367,     0,   441,    39,     0,
      78,   283,   283,   150,   283,   158,   166,   164,     0,   184,
       0,     0,   224,   177,   178,     0,     0,     0,     0,     0,
       0,     0,     0,   283,   359,     0,     0,   365,     0,     0,
     388,     0,   315,   318,   319,   320,     0,   317,   321,   397,
     264,   228,   227,     0,     0,     0,   304,   397,   149,   147,
       0,   142,     0,   121,     0,   119,    40,   159,   256,   182,
     183,   221,   221,     0,     0,     0,     0,     0,     0,     0,
     151,     0,     0,     0,     0,    53,   296,   307,     0,   265,
     267,   266,   269,   260,   261,   169,     0,   140,     0,     0,
     257,   268,   179,   180,   222,     0,     0,     0,     0,     0,
       0,   358,   357,   364,   363,   230,   229,   309,   310,   312,
     397,     0,   436,   397,   370,   369,     0,     0,   311,   313,
     258,   176,   259,   307,   314
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   233,    78,   520,    80,    81,   248,    82,
     234,   515,   519,   516,    83,    84,    85,   161,    86,   165,
     185,    87,    88,    89,    90,    91,    92,   616,    93,    94,
      95,   378,   547,   671,    96,    97,   543,   666,    98,    99,
     409,   682,   100,   101,   572,   573,   144,   178,   528,   103,
     104,   411,   687,   578,   726,   727,   728,   803,   351,   524,
     525,   526,   479,   579,   253,   656,   881,   911,   875,   197,
     870,   831,   834,   105,   222,   383,   106,   107,   181,   182,
     355,   356,   537,   359,   534,   898,   828,   767,   235,   156,
     239,   240,   326,   327,   328,   145,   109,   110,   111,   146,
     113,   132,   133,   480,   342,   639,   481,   114,   147,   148,
     203,   329,   150,   119,   151,   152,   122,   123,   244,   124,
     125,   126,   127,   128
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -805
static const yytype_int16 yypact[] =
{
    -805,   100,  2649,  -805,  -805,  -805,  -805,  -805,  -805,  -805,
    -805,  3769,    26,   161,  -805,    26,  7971,   113,   161,  3769,
    7971,  3769,    70,  -805,   289,   765,  6571,  7971,  6711,  7971,
      91,  -805,   161,  -805,    25,  7131,  7971,  7971,  -805,  7971,
    7971,   189,   111,  1367,  1436,  -805,  7271,  6151,  7971,  7131,
    7971,   213,   162,  3769,  7971,  8111,  8111,   161,  -805,  7271,
    7971,  7971,  -805,  -805,  7971,  -805,  -805,  1957,  7971,  7971,
    -805,  7971,  -805,  -805,  2929,  5731,  7271,  -805,  3629,  -805,
    -805,   178,  -805,  -805,  -805,  -805,  -805,  -805,  -805,  -805,
    -805,  -805,  -805,  -805,  -805,  -805,  -805,  -805,  -805,   161,
    -805,   161,   103,   196,  -805,  -805,  -805,   165,   199,  -805,
    -805,  -805,   243,   250,   117,   263,   264, 12859,  5292,    16,
     283,   286,  -805,  -805,  -805,  -805,  -805,  -805,   319,  -805,
   12859,   187,  3769,  -805,   291,  -805,   202,  7971,  7971,  7971,
    7971,  7971,  7271,  7271,   278,  -805,  -805,  -805,  -805, 11127,
     281,  -805,  -805,   161,   296,  -805,   181, 12859,   341,  6291,
    -805,  -805,  -805,  -805,   161,    23,   161,   225,    44, 10077,
   10043,  -805,  -805,  -805, 11065, 10255,  6291,  3769,   290,    29,
      96,     6,  -805,  3769,  -805,  -805, 10326,   445,   342, 10326,
     445,  -805,  6291,   209,  -805,  -805,   161,  -805,   183, 12859,
    7971,  7971,  -805, 12859,   302, 10115,  -805, 10326, 12859,   315,
    6291,  -805, 12859, 11396,  -805,  -805, 11467,  5433,  -805,  -805,
   11529,   364,   327,   201, 11201, 10326, 11600,   518,   591,   445,
     518,   445,  -805,  -805,  3069,    89,  -805,  7971,  -805,   -51,
     101,  -805,    59, 11662,   -21,   452,  -805,  -805,   413,   350,
     329,  -805,  -805,  -805,    20,    25,    25,    25,  -805,  7971,
    7971,  7971,  7971,  7411,  7411,  7971,  7971,  7971,  7971,  7971,
    7971,   123,  1957,  7971,  7971,  7971,  7971,  7971,  7971,  7971,
    7971,  7971,  7971,  7971,  7971,  7971,  7971,  7971,  6851,  6851,
    6851,  6851,  6851,  6851,  6851,  6851,  6851,  6851,  6851,  6851,
    6851,  6851,  6851,  6851,  6291,  6291,  7411,  7411,  5591,  -805,
    -805,  3209,  -805, 11263, 11334, 11733,    47,  7411,    44,   338,
    -805,  7971,  -805,  7971,   378,  -805,   333,   366,  -805,  -805,
     345,   161,   442,  7271,  -805,  3909,  7411,  -805,  4049,  7411,
     340,  -805,    44,  8251,  7971,  -805,  3769,   453,   355,  -805,
      52,  -805,  -805,    29,  -805,   386,   367,  -805,  2343,   406,
    7971,    25,  -805,  -805,  -805,   375,  -805,  -805,  7271,  -805,
   12859, 12859,  -805,  -805,  -805,  6291,   380,  -805,   489,  -805,
     489,  -805,  2343,   407,  -805,  -805,  8251,  7971,  -805,  -805,
    -805,  -805,  6991,  -805,  9885,  5871,  -805,  6011,  -805,  7411,
    2499,  2789,   381,  7971,  5450,  -805,  -805,   161,  7271,   387,
     161,    54,    25,   217,   241,   242,   251, 10991,  1762,  1762,
     298,  -805,   298,  -805,   298,  5140,   439,  1223,   404,   342,
     518,  -805,  -805,  -805,   591,  2470,   298,   982,   982,  1762,
     982,   982,   757,   518,  2470,  2314,   757,   445,   445,   518,
     395,   396,   397,   399,   401,   402,   408,   411,   415,   417,
     423,   424,   430,   435,   437,   438,   403,   422,  -805,   298,
    -805,   298,    50,  -805,  -805,  -805,  -805,  -805,  -805,   161,
     106,  -805, 12930,   339,  8391,  7411,  8531,  7411,  7971,  7411,
    1130,    26, 11795,  -805,  -805, 12859, 11866,  6291,  -805,  6291,
    -805,   350,  7971,   114,  7971, 12859,    57, 10388,  7971, 12859,
      56, 10193,  5591,  -805,   443,   464,   446, 11928,   464,   449,
     553, 11999,  3769,  -805,   176,  -805,    27,  -805,   149,   447,
      29,    96,  -805,  7271,  -805,   867,  6431,  -805, 12859,  -805,
    -805,  -805, 12859,     3,   454,  -805,    13,  -805,  -805,   255,
     161,   463,   465,  -805,  -805,  -805,  -805,  -805,  -805,   -39,
    2121,  -805,  -805, 12859,  3769, 12859,  -805, 12061,   460,   472,
    -805,   507,    99,  -805,   536,  -805,  -805,  -805,  -805,  9561,
     258,  -805,  -805,  -805,  -805,  -805,  -805,  -805,  -805,  -805,
    -805,  -805,  -805,  -805,  -805,  -805,  -805,  -805,  -805,  -805,
    -805,  -805,  -805,  -805,  -805,  -805,  5591,  -805,  7411,  7411,
    7971,   576, 12132,  7971,   577, 12194,   462,  9731,    44,    44,
    -805,  -805,  -805,  -805,   471, 10326,  -805, 10459,  4189,  -805,
    4329,  -805, 10521,  4469,  -805,    44,  4609,  -805,    44,   211,
    -805,  7971,  -805,  7971,  -805,  3769,  7971,  -805,   270,  -805,
      29,   499,   547,  -805,  -805,  -805,    41,  -805,  -805,   406,
      62,  -805,  -805,  4749,  7271,  -805,  -805,  -805,   161,  -805,
     502,   202,  -805,  -805,  -805,  4889,   476,  5029,   479,  -805,
    7971,  3349,   558,  7971,   161,  -805,  -805,   149,  -805,  -805,
    -805,  -805,  -805,  -805,  -805,  -805,  -805,  -805,  -805,  -805,
    -805,  -805,  -805,   161,  -805,  -805,  -805,  -805,  -805,  -805,
    -805,  -805,  -805,  -805,  -805,  -805,  -805,  -805,  -805,  -805,
    -805,  -805,  -805,  -805,  7971,   500,   509,   484,   484,  -805,
    -805,  -805,   214,   216, 12265,  8671,  8811, 12327,  8951,  9091,
    9231,  9371,  -805,  -805,  -805,  -805,  3769,  7971, 12859,  7971,
   12859,  3769,  7971, 12859,  -805,  7971, 12859,  -805,  5591,  -805,
   12398, 12859,  -805, 12859,  -805,   486,  7551,    32,  -805,   539,
    -805,  7411,  5169,  3769,  -805,    42,   492,  7971,  -805,  7971,
    -805, 12859,  3769,  7971,  -805, 12859,  3769, 12859,  -805,  3489,
     994,   434,   434,  -805,  1311,  -805, 12859,  -805,    35,  -805,
    9923,  9610,    52,  -805,  -805,  7971,  7971,  7971,  7971,  7971,
    7971,  7971,  7971,  1245, 11999, 10592, 10654, 11999, 10725, 10787,
    -805,  7971,   499,    70,  7971,  7971,  5309,  -805,  -805,   204,
    7271,  -805,  -805,  7971,    -7,  9769,  -805,  1113,   341,  -805,
     202, 12859, 10858,  -805, 10920,  -805,  -805,  -805,   560,  -805,
    -805,   484,   484,   234, 12460, 12531, 12593, 12664, 12726, 12797,
    -805,  3769,  3769,  3769,  3769, 12859,    32,  7691,    66,  -805,
    -805, 12859, 12859,  -805,  -805,  -805,  9371,  -805,  3769,  3769,
    -805,   539,  -805,  -805,  -805,  7971,  7971,  7971,  7971,  7971,
    7971, 11999, 11999, 11999, 11999,  -805,  -805,  -805,  -805,  -805,
     206,  7411,  9511,  1114, 11999, 11999,    53,  9807,  -805,  -805,
    -805,  -805,  -805,  7831,  -805
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -805,  -805,  -805,    -1,  -670,   358,  -805,  -805,  -805,  1681,
     -46,  -294,   259,   244,  -805,  -805,   369,   110,  1962,     7,
     854,  -793,  -637,   -40,  -805,  -805,  -805,    98,  -805,  -805,
    -805,   427,  -805,   -26,  -805,  -805,  -805,  -805,  -805,   483,
     148,   -94,  -805,  -805,  -805,   -33,   774,  -805,  -805,  -805,
    -805,  -805,  -805,  -805,  -805,  -148,  -147,  -689,  -805,  -150,
       8,  -805,  -340,  -805,  -805,   -30,  -805,  -805,  -248,    -4,
    -805,  -206,  -218,  -805,  -142,  -805,   636,  -805,  -176,   304,
    -805,  -347,  -651,  -511,  -380,  -661,  -804,  -156,    18,  -805,
    -149,  -805,   -81,  -805,   168,   357,  -353,  -805,  -805,   894,
    -805,     0,  -805,  -805,  -221,  -805,  -505,  -805,   922,  1051,
      -2,   150,   166,  -805,  1271,  1413,  -805,  -805,  -805,  -805,
    -805,  -805,  -805,  -295
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -437
static const yytype_int16 yytable[] =
{
     117,    77,   548,   194,   195,   535,   529,   640,   770,   130,
     527,   319,   794,   483,   149,   136,     5,   117,   157,   117,
     659,   402,   330,     5,   169,   170,   174,   175,     5,   549,
       5,   162,     5,   186,   187,   188,   357,   189,   190,   804,
      14,   874,   357,   360,   199,   795,   205,   207,   208,   340,
     518,   212,   213,   216,   220,  -231,   663,   224,   225,   226,
      47,   395,   227,   899,   198,   228,   229,   230,   183,   231,
     183,   238,   199,   472,   243,   574,   117,   223,   413,   414,
     415,   416,   633,   628,   489,   412,    45,   603,   664,   473,
     396,   404,   474,   518,   242,   348,   399,   357,   909,   771,
       3,   731,  -299,   901,   675,    58,   475,   476,  -299,   575,
     352,   365,  -299,   874,   131,   604,   832,   477,   361,   576,
      47,  -299,   405,   134,   168,   536,     5,  -299,   154,   376,
     117,   311,   478,  -299,   577,   130,   313,   314,   315,   212,
     199,   243,   665,   794,   340,   769,   358,   340,   830,   318,
     431,   311,   358,   668,   368,   325,   304,   847,   305,   400,
     179,   316,   882,   883,     5,   179,  -299,   650,   118,   353,
     652,   400,   325,  -299,   400,   117,   795,   118,   400,  -299,
     311,   117,   311,   658,   910,   118,   254,   118,   325,  -299,
     490,   311,  -231,   153,   311,   311,   191,   204,   370,   371,
     366,   392,   401,   432,   653,   772,   325,   358,  -299,   902,
     159,   684,     5,   397,   654,   896,   160,   483,   606,   118,
     255,   217,   217,   466,   467,   241,   368,  -299,   393,   655,
     256,   176,   117,   391,  -377,   394,   580,   251,   685,   247,
     118,   908,   398,  -418,   118,  -418,   556,   607,   558,   257,
     -78,   192,   914,   820,   259,   626,   252,   417,   418,   419,
     420,   422,   424,   425,   426,   427,   428,   429,   430,   -78,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   209,   848,   648,   635,
     638,   472,     5,   321,   544,   368,   160,  -316,   118,  -308,
     325,   325,   210,   765,   469,   471,   482,   473,   527,   117,
     474,   483,   322,   368,   369,   492,  -316,   649,  -308,   495,
    -316,   496,  -308,   758,   475,   476,  -390,   308,  -391,   361,
    -374,   199,   385,   505,   507,   477,   509,   511,   676,   678,
     311,   517,   517,   118,   521,  -316,   648,  -308,   581,   118,
     478,   503,   759,   361,   361,  -390,   117,  -391,   538,   108,
      79,   421,   423,   361,   541,   333,   542,  -276,   108,   129,
     361,   325,   582,   583,  -373,   884,   108,   155,   108,   158,
     117,   258,   584,   306,   517,   517,  -276,   307,   541,   730,
     542,   238,   836,   238,  -378,  -379,   541,   560,   542,   563,
     118,   565,   567,   609,   468,   470,   199,   307,   672,   269,
     108,   211,   271,   829,  -376,   491,   274,  -375,  -418,   837,
    -418,  -417,   310,  -417,   215,   219,   569,   320,   323,   262,
     350,   108,    79,   372,   506,   108,   246,   510,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   171,   375,   271,   382,   384,   406,
     407,   408,   527,   483,   262,    24,   193,   410,   263,   494,
     262,   497,   264,    31,   498,  -170,   500,   118,   499,   502,
     512,   522,   505,   612,   509,   615,   521,   617,   563,   108,
     309,   618,   619,   621,  -170,   325,   523,   559,   530,   536,
     625,   118,   627,   263,   118,   546,   632,   264,   531,   263,
     482,   266,   118,   264,   900,   269,   540,   270,   271,   550,
     117,   545,   274,   903,   564,   570,   585,   586,   587,   281,
     588,   243,   589,   590,   108,   349,   285,   286,   287,   591,
     108,   362,   592,   262,   601,   241,   593,   241,   594,   900,
     269,   660,   270,   271,   595,   596,   269,   274,   270,   271,
     900,   597,   117,   274,   281,   602,   598,   118,   599,   600,
     281,   285,   286,   287,   -56,    14,   641,   642,    17,   287,
     644,   645,   263,   611,   368,   614,   264,    23,   657,    24,
     790,   108,    79,    28,   673,   667,   674,    31,   681,  -170,
     683,   686,   735,   738,   482,   740,   422,   469,   734,   744,
     766,   737,   768,   777,  -341,    41,   262,   791,  -170,   782,
     792,    45,   786,   801,   802,   833,   748,   822,   750,   269,
     551,   753,   271,   840,   756,   789,   274,   880,    57,   760,
      58,   761,   776,   117,   763,   380,   552,   622,   331,   624,
     813,   797,   853,   851,   852,   263,   764,   798,   912,   264,
     895,   117,   199,   906,   173,   539,   866,   623,   108,    79,
       0,     0,     0,   781,     0,   785,     0,     0,   787,   117,
       0,   796,   775,     0,     0,     0,   662,     0,   118,    72,
       0,     0,   108,   362,     0,   108,   362,   793,   266,     0,
     267,   268,   269,   108,   270,   271,   421,   468,     0,   274,
       0,     0,     0,     0,     0,   280,   281,     0,     0,     0,
     284,     0,   800,   285,   286,   287,     0,     0,     0,     0,
     118,     0,     0,   748,   750,     0,   753,   756,   781,   785,
       0,     0,     0,     0,   814,   815,     0,   816,     0,   817,
     818,   194,   195,   819,     0,     0,   482,     0,   108,   562,
       0,     0,   827,     0,   117,     0,     0,     0,     5,   835,
     563,   117,   160,     0,   163,   841,   102,   842,     0,     0,
     117,   844,   262,     0,   117,   102,  -285,   117,   391,     0,
       0,     0,     0,   102,   118,   102,   118,     0,     0,   118,
       0,     0,   118,   814,   854,   855,   817,   856,   857,   858,
     859,   118,     0,     0,     0,     0,     0,     0,     0,   865,
    -285,   263,   208,   212,   243,   264,   869,   102,   871,   118,
    -285,   872,     0,     0,     0,     0,   605,     0,     0,     0,
       0,   118,     0,   118,   868,   164,     0,   118,   102,  -285,
       0,     0,   102,     0,     0,     0,     0,     0,     0,   891,
     892,   893,   894,   897,   266,   117,   267,   268,   269,     0,
     270,   271,     0,     0,   785,   274,   904,   905,     0,   108,
     647,     0,   281,   891,   892,   893,   894,   904,   905,   285,
     286,   287,     0,     0,     0,     0,   112,     0,   897,   907,
     563,     0,     0,   206,  -300,   112,   102,     0,     0,   897,
       0,   785,   118,   112,     0,   112,     0,   118,     0,     0,
       0,   108,   679,     0,   115,     0,     0,     0,     0,     0,
       0,     0,     0,   115,  -300,     0,   729,     0,     0,   118,
       0,   115,     0,   115,  -300,     0,     0,   112,   118,     0,
       0,   102,   118,  -300,     0,   118,     0,   102,     0,     0,
    -300,     0,     0,     0,     0,   732,   733,     0,   112,     0,
       0,     0,   112,     0,     0,   115,     0,     0,     0,  -300,
       0,     0,     0,     0,     0,   108,   362,   108,   362,     0,
     108,   362,     0,   108,   362,     0,   115,     5,  -300,     0,
     115,   160,   108,   762,     0,  -300,     0,   262,   102,     0,
       0,     0,     0,     0,     0,  -285,     0,     0,     0,     0,
     108,   774,   334,   337,   341,     0,   112,   118,   118,   118,
     118,     0,   108,   780,   108,   784,     0,     0,   108,    79,
     363,     0,     0,   364,   118,   118,   263,     0,     0,  -285,
     264,     0,     0,   116,   115,     0,     0,     0,     0,  -285,
       0,   374,   116,     0,     0,     0,     0,     0,     0,     0,
     116,   112,   116,     0,   164,     0,     0,   112,  -285,   388,
       0,     0,     0,     0,     0,   102,     0,     0,     0,   266,
       0,   267,   268,   269,     0,   270,   271,   272,     0,   115,
     274,     0,     0,   108,   116,   115,   280,   281,   108,   102,
       0,   284,   102,     0,   285,   286,   287,     0,     0,     0,
     102,     0,     0,     0,     0,   116,     0,     0,   112,   116,
     108,   838,     0,     5,     6,     7,     8,     9,    10,   108,
     843,     0,   137,   108,   845,     0,   108,    79,     0,     0,
    -303,  -305,     0,     0,     0,     0,   115,    22,     0,     0,
       0,     0,     0,   138,   139,     0,   140,     0,    30,     0,
       0,     0,   493,    33,    34,   102,    36,     0,    37,    38,
    -303,  -305,     0,   116,     0,     0,    40,     0,    42,     0,
    -303,  -305,     0,     0,     0,     0,   513,     0,     0,  -303,
    -305,    50,    51,    52,   141,   112,  -303,  -305,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   108,   108,
     108,   108,     0,     0,     0,  -303,  -305,     0,   116,   112,
       0,     0,   112,   115,   116,   108,   108,     0,    64,     0,
     112,     0,     0,     0,  -303,  -305,     0,     0,   262,     0,
       0,  -303,  -305,     0,     0,     0,    69,   115,     0,     0,
     115,     0,    14,     0,     0,    17,     0,     0,   115,     0,
      75,     0,   143,   120,    23,     0,    24,   790,     0,     0,
      28,     0,   120,     0,    31,   116,  -170,   263,     0,     0,
     120,   264,   120,     0,     0,   112,   102,     0,     0,     0,
       0,     0,    41,     0,   791,  -170,     0,   792,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   115,   120,    57,     0,    58,    14,     0,
     266,    17,     0,   268,   269,     0,   270,   271,   102,     0,
      23,   274,    24,   790,     0,   120,    28,     0,   281,   120,
      31,     0,  -170,     0,     0,   285,   286,   287,     0,     0,
     629,   631,   116,     0,   634,   637,     0,     0,   245,     0,
     791,  -170,     0,   792,    45,     0,    72,     0,     0,     0,
       0,     0,     0,     0,   860,     0,   116,   171,     0,   116,
       0,    57,     0,    58,     0,     0,     0,   116,    24,   193,
       0,     0,   102,   120,   102,     0,    31,   102,  -170,     0,
     102,     0,     0,     0,   -43,   121,   112,     0,     0,   102,
       0,     0,     0,     0,   121,     0,     0,  -170,   -43,     0,
       0,     0,   121,     0,   121,     0,     0,   102,     0,     0,
       0,     0,    72,     0,   115,     0,     0,     0,   120,   102,
       0,   102,   116,     0,   120,   102,   171,     0,   112,     0,
       0,     0,     0,     0,     0,     0,   121,    24,   193,     0,
       0,     0,   742,   743,     0,    31,     0,  -170,     0,   745,
       0,     0,     0,   -42,     0,     0,   115,   121,     0,   754,
       0,   121,   757,     0,     0,     0,  -170,   -42,     0,     0,
       0,     0,     0,     0,     0,   120,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     102,     0,   112,     0,   112,   102,     0,   112,     0,     0,
     112,     0,     0,     0,     0,     0,     0,     0,     0,   112,
       0,     0,     0,     0,     0,   121,     0,   102,     0,     0,
     115,     0,   115,     0,     0,   115,   102,   112,   115,     0,
     102,     0,     0,   102,     0,     0,     0,   115,     0,   112,
       0,   112,     0,   116,     0,   112,     0,     0,     0,     0,
       0,     0,   120,     0,     0,   115,     0,     0,     0,     0,
     121,     0,     0,     0,     0,     0,   121,   115,     0,   115,
       0,     0,     0,   115,     0,     0,   120,     0,     0,   120,
       0,     0,     0,     0,     0,   116,     0,   120,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   839,
       0,     0,     0,     0,     0,   102,   102,   102,   102,     0,
     112,     0,     0,     0,     0,   112,     0,   121,     0,     0,
       0,     0,   102,   102,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   112,   115,     0,
       0,     0,   120,   115,     0,     0,   112,     0,     0,   116,
     112,   116,     0,   112,   116,     0,     0,   116,     0,     0,
       0,     0,     0,     0,     0,   115,   116,     0,     0,     0,
       0,     0,     0,     0,   115,     0,     0,     0,   115,     0,
       0,   115,     0,     0,   116,     0,   184,     0,     0,     0,
       0,     0,     0,     0,   121,     0,   116,     0,   116,     0,
     184,     0,   116,     0,     0,     0,   214,   218,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   121,     0,
       0,   121,     0,     0,     0,   112,   112,   112,   112,   121,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   112,   112,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   115,   115,   115,   115,   262,     0,     0,
       0,     0,     0,   120,     0,     0,     0,   116,     0,     0,
     115,   115,   116,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   121,     0,     0,   312,     0,     0,
       0,     0,     0,     0,   116,     0,   263,     0,     0,     0,
     264,     0,     0,   116,     0,   120,     0,   116,     0,     0,
     116,     0,     0,     0,     0,     0,     0,     0,     0,   184,
     184,   184,     0,     0,   265,     0,   347,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   184,     0,   266,
     184,   267,   268,   269,     0,   270,   271,   272,     0,   273,
     274,   275,   276,     0,   278,   279,   280,   281,   184,   282,
     283,   284,     0,     0,   285,   286,   287,     0,     0,   120,
       0,   120,     0,     0,   120,     0,   184,   120,     0,     0,
       0,     0,   116,   116,   116,   116,   120,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   116,
     116,     0,     0,     0,   120,   121,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   120,     0,   120,     0,
       0,     0,   120,     0,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   137,
       0,     0,     0,     0,     0,   135,     0,   121,     0,     0,
     135,     0,     0,     0,    22,     0,   135,   135,     0,     0,
     138,   139,     0,   140,   177,    30,   180,     0,     0,   184,
      33,    34,     0,    36,     0,    37,    38,     0,     0,     0,
       0,     0,     0,    40,     0,    42,     0,   120,     0,   221,
       0,     0,   120,   184,     0,     0,     0,     0,    50,    51,
      52,   141,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   121,     0,   121,   120,     0,   121,     0,     0,   121,
       0,     0,     0,   120,     0,     0,     0,   120,   121,     0,
     120,   249,     0,   250,    63,    64,    65,    66,     0,     0,
       0,     0,     0,     0,     0,     0,   121,     0,     0,     0,
       0,    68,     0,    69,     0,     0,    71,     0,   121,     0,
     121,    73,     0,     0,   121,   142,     0,    75,     0,   143,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   221,     0,     0,     0,     0,
       0,   324,     0,     0,     0,     0,   221,     0,   332,     0,
       0,   260,   120,   120,   120,   120,   261,     0,   324,     0,
       0,   354,     0,     0,     0,     0,   262,     0,     0,   120,
     120,     0,     0,     0,   324,   135,     0,     0,   367,   121,
       0,     0,     0,     0,   121,     0,     0,     0,     0,     0,
       0,     0,   324,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   263,   121,   184,   184,   264,
       0,   184,   184,     0,     0,   121,     0,     0,     0,   121,
       0,     0,   121,     0,     0,     0,     0,     0,     0,   340,
       0,     0,     0,   265,     0,     0,   180,   180,   180,   180,
       0,     0,     0,     0,     0,     0,     0,   669,   266,     0,
     267,   268,   269,   433,   270,   271,   272,     0,   273,   274,
     275,   276,   277,   278,   279,   280,   281,     0,   282,   283,
     284,     0,     0,   285,   286,   287,     0,     0,     0,     0,
       0,     0,     0,     0,   677,     0,   324,   324,     0,     0,
       0,     0,     0,     0,   121,   121,   121,   121,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   121,   121,   501,     0,     0,     0,     0,     0,   184,
     184,     0,     0,     0,     0,     0,   184,     0,   347,     0,
       0,     0,     0,   347,     0,   354,   184,     0,     0,   184,
       0,     0,     0,   180,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   324,     0,   262,
       0,     0,     0,     0,   532,     0,     5,     6,     7,     8,
       9,    10,   778,     0,     0,   137,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   568,
      22,     0,   571,     0,   180,     0,   138,   139,   263,   140,
       0,    30,   264,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,     0,    40,
       0,    42,     0,     0,     0,     0,   265,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    52,   141,     0,     0,
       0,   266,     0,   267,   268,   269,     0,   270,   271,   272,
       0,   273,   274,   275,   276,    62,   278,   279,   280,   281,
       0,   282,     0,   284,     0,     0,   285,   286,   287,     0,
      63,    64,    65,    66,     0,     0,   184,     0,    67,     0,
       0,   324,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,     0,     0,     0,     0,    73,     0,     0,
       0,   142,     0,    75,     0,   533,     0,     0,   651,     0,
       0,     0,   354,     0,     0,   262,   347,   347,     0,   347,
     347,     0,     5,     6,     7,     8,     9,    10,   670,     0,
       0,   137,   221,     0,     0,   873,     0,     0,     0,     0,
       0,   877,     0,   347,     0,   347,    22,     0,     0,     0,
       0,     0,   138,   139,   263,   140,     0,    30,   264,     0,
       0,   725,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,     0,    40,     0,    42,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,    52,   141,     0,     0,     0,   266,     0,   267,
     268,   269,     0,   270,   271,   272,     0,   873,   274,   275,
     276,    62,   278,   279,   280,   281,     0,     0,     0,   284,
       0,     0,   285,   286,   287,     0,    63,    64,    65,    66,
       0,     0,   354,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,   196,
     670,     0,     0,    73,     0,     0,     0,   142,     0,    75,
       0,   143,   561,     0,     0,     0,   571,     0,     0,    -2,
       4,     0,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,   799,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,    41,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   135,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,   850,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,     0,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,  -436,
       0,    11,    12,    13,  -436,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,  -436,    21,    22,  -436,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,     0,    42,    43,  -170,
     -41,    44,    45,  -436,  -283,    46,    47,  -436,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,  -436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -436,    64,  -436,  -436,
    -436,  -436,  -436,  -436,  -436,     0,  -436,  -436,  -436,  -436,
    -436,  -436,  -436,  -436,  -436,    69,  -436,  -436,  -436,     0,
      72,  -436,  -436,  -436,     0,     0,     0,    74,  -436,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,    41,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,   196,
      72,     0,     0,    73,     0,     0,     0,    74,   232,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,    41,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,   390,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,    41,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,   232,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,    41,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,   788,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,    41,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,   846,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,   245,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,     0,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,     0,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,     0,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,   504,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,     0,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,     0,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,   508,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,     0,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,     0,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,   747,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,     0,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,     0,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,   749,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,     0,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,     0,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,   752,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,     0,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,     0,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,   755,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,     0,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,     0,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,   773,    22,     0,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,     0,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,     0,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,   779,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,     0,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,     0,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -283,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,   783,     0,    30,    31,     0,
    -170,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -283,    40,     0,    42,    43,  -170,
     -41,    44,    45,     0,  -283,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -283,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,   262,     0,    74,     0,    75,
     532,    76,     5,     6,     7,     8,     9,    10,     0,  -436,
       0,   137,     0,     0,  -436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -436,     0,    22,     0,     0,     0,
       0,     0,   138,   139,   263,   140,     0,    30,   264,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,     0,    40,     0,    42,     0,     0,
       0,     0,     0,  -436,     0,     0,     0,  -436,     0,     0,
      50,    51,    52,   141,     0,     0,     0,   266,     0,   267,
     268,   269,     0,   270,   271,   272,     0,   273,   274,   275,
     276,  -436,   278,   279,   280,   281,     0,   282,     0,   284,
       0,     0,   285,   286,   287,     0,  -436,    64,  -436,  -436,
    -436,     0,  -436,  -436,  -436,     0,  -436,  -436,  -436,  -436,
    -436,  -436,  -436,  -436,  -436,    69,  -436,  -436,  -436,     0,
       0,  -436,  -436,  -436,     0,     0,     0,   142,     0,    75,
       0,   533,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   137,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,   138,   139,     0,   140,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,     0,    40,     0,    42,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,    52,   141,     0,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,   302,   303,  -392,
       0,     0,  -417,    68,  -417,    69,     0,    70,    71,   196,
       0,     0,     0,    73,     0,     0,     0,   142,     0,    75,
       0,   143,   867,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   137,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,   138,   139,     0,   140,     0,    30,     0,
       0,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,     0,    40,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,    51,    52,   141,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,   302,   303,
       0,     0,     0,  -417,    68,  -417,    69,     0,    70,    71,
       0,     0,     0,     0,    73,     0,     0,     0,   142,     0,
      75,     0,   143,   566,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,   472,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,   473,    30,
       0,   474,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,   475,   476,    40,     0,    42,
       0,     0,     0,     0,     0,     0,   477,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,   478,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,   200,
     201,     0,   236,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,   237,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   196,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,   200,
     201,     0,   236,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   196,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,   555,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,   200,
     201,     0,   236,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   196,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,   557,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,   200,
     201,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,   202,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,   200,
     201,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   196,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,   661,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,   200,
     201,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,   166,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   167,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,   171,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,   172,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,   200,
     201,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   196,     0,     0,     0,    73,     0,     0,     0,   142,
     553,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   183,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   196,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   167,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   823,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   824,    51,    52,   825,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   196,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   826,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   196,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   826,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   812,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   196,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,   514,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   610,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   613,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   806,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   807,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   809,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   810,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   811,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   812,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   142,
       0,    75,     0,   143,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   138,   139,     0,   140,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     5,     0,     0,    40,     0,    42,
       0,   688,     0,     0,     0,     0,   689,     0,     0,     0,
       0,     0,    50,    51,    52,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -307,     0,     0,     0,     0,     0,
       0,     0,     0,     5,     0,     0,     0,     0,     0,    64,
     688,     0,     0,  -307,     0,   689,     0,  -307,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,     0,     0,
       0,   196,     0,     0,     0,     0,     0,     0,     0,   142,
       0,    75,  -307,   826,   690,   691,   692,   693,   694,   695,
       0,     0,   696,   697,   698,   699,   700,   701,   702,   703,
     704,   705,     0,     0,   706,     0,     0,     0,   707,   708,
     709,   710,   711,   712,   713,   714,   715,   716,   717,     0,
     718,     0,     0,   719,   720,   721,   722,     0,   723,     0,
       0,   724,     0,   690,   691,   692,   693,   694,   695,     0,
       0,   696,   697,   698,   699,   700,   701,   702,   703,   704,
     705,     0,     0,   706,     0,     0,     0,   707,   708,   709,
     710,   711,   712,   713,   714,   715,   716,   717,     0,   718,
       0,   260,   719,   720,   721,   722,   261,   723,     0,     0,
       0,     0,     0,     0,     0,     0,   262,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   260,
       0,     0,     0,     0,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   262,   263,     0,     0,     0,   264,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   260,     0,     0,
       0,     0,   261,   265,     0,     0,     0,     0,     0,     0,
       0,     0,   262,   263,     0,     0,     0,   264,   266,     0,
     267,   268,   269,     0,   270,   271,   272,     0,   273,   274,
     275,   276,   277,   278,   279,   280,   281,     0,   282,   283,
     284,   265,     0,   285,   286,   287,     0,     0,     0,     0,
       0,   263,     0,     0,   741,   264,   266,     0,   267,   268,
     269,     0,   270,   271,   272,     0,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   260,   282,   283,   284,   265,
     261,   285,   286,   287,     0,     0,     0,     0,     0,     0,
     262,     0,   876,     0,   266,     0,   267,   268,   269,     0,
     270,   271,   272,     0,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   260,   282,   283,   284,     0,   261,   285,
     286,   287,     0,     0,     0,     0,     0,     0,   262,   263,
     913,     0,     0,   264,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   263,     0,     0,
       0,   264,   266,     0,   267,   268,   269,     0,   270,   271,
     272,     0,   273,   274,   275,   276,   277,   278,   279,   280,
     281,     0,   282,   283,   284,   265,     0,   285,   286,   287,
       0,     0,     0,     0,     0,     0,   554,     0,     0,     0,
     266,     0,   267,   268,   269,     0,   270,   271,   272,     0,
     273,   274,   275,   276,   277,   278,   279,   280,   281,     0,
     282,   283,   284,   260,     0,   285,   286,   287,   261,     0,
       0,     0,     0,     0,   849,     0,     0,     0,   262,   338,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     339,     0,     0,     0,     0,     0,     0,   260,     0,     0,
       0,     0,   261,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   262,   335,     0,     0,     0,   263,     0,     0,
       0,   264,     0,     0,   336,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   260,     0,     0,     0,     0,
     261,   340,     0,     0,     0,   265,     0,     0,     0,     0,
     262,   263,     0,     0,     0,   264,     0,     0,     0,     0,
     266,     0,   267,   268,   269,     0,   270,   271,   272,     0,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   265,
     282,   283,   284,     0,     0,   285,   286,   287,     0,   263,
       0,   311,     0,   264,   266,     0,   267,   268,   269,     0,
     270,   271,   272,     0,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   260,   282,   283,   284,   265,   261,   285,
     286,   287,     0,     0,     0,   311,     0,     0,   262,   636,
       0,     0,   266,     0,   267,   268,   269,     0,   270,   271,
     272,     0,   273,   274,   275,   276,   277,   278,   279,   280,
     281,     0,   282,   283,   284,     0,     0,   285,   286,   287,
       0,     0,     0,   373,     0,     0,     0,   263,     0,     0,
       0,   264,     0,     0,     0,   260,     0,     0,     0,     0,
     261,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   340,     0,     0,     0,   265,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     266,     0,   267,   268,   269,     0,   270,   271,   272,     0,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   263,
     282,   283,   284,   264,     0,   285,   286,   287,     0,     0,
     346,   311,     0,     0,     0,     0,   260,     0,     0,     0,
       0,   261,     0,     0,     0,     0,     0,   265,     0,     0,
       0,   262,   183,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   266,     0,   267,   268,   269,     0,   270,   271,
     272,     0,   273,   274,   275,   276,   277,   278,   279,   280,
     281,     0,   282,   283,   284,     0,     0,   285,   286,   287,
     263,     0,     0,   311,   264,     0,     0,     0,   260,     0,
       0,     0,     0,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   262,   630,     0,     0,     0,   265,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   266,     0,   267,   268,   269,     0,   270,
     271,   272,     0,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   263,   282,   283,   284,   264,     0,   285,   286,
     287,     0,     0,     0,   311,     0,     0,     0,     0,   260,
       0,     0,     0,     0,   261,     0,     0,     0,     0,     0,
     265,     0,     0,     0,   262,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   266,     0,   267,   268,   269,
       0,   270,   271,   272,     0,   273,   274,   275,   276,   277,
     278,   279,   280,   281,     0,   282,   283,   284,     0,     0,
     285,   286,   287,   263,     0,     0,   311,   264,     0,     0,
       0,   260,     0,     0,   746,     0,   261,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   262,     0,     0,     0,
       0,   265,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   266,     0,   267,   268,
     269,     0,   270,   271,   272,     0,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   263,   282,   283,   284,   264,
       0,   285,   286,   287,     0,     0,   751,   311,     0,     0,
       0,     0,   260,     0,     0,     0,     0,   261,     0,     0,
       0,     0,     0,   265,     0,     0,     0,   262,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   266,     0,
     267,   268,   269,     0,   270,   271,   272,     0,   273,   274,
     275,   276,   277,   278,   279,   280,   281,     0,   282,   283,
     284,     0,     0,   285,   286,   287,   263,     0,     0,   311,
     264,     0,     0,     0,   260,     0,     0,   861,     0,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
       0,     0,     0,     0,   265,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   266,
       0,   267,   268,   269,     0,   270,   271,   272,     0,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   263,   282,
     283,   284,   264,     0,   285,   286,   287,     0,     0,   862,
     311,     0,     0,     0,     0,   260,     0,     0,     0,     0,
     261,     0,     0,     0,     0,     0,   265,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   266,     0,   267,   268,   269,     0,   270,   271,   272,
       0,   273,   274,   275,   276,   277,   278,   279,   280,   281,
       0,   282,   283,   284,     0,     0,   285,   286,   287,   263,
       0,     0,   311,   264,     0,     0,     0,   260,     0,     0,
     863,     0,   261,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   262,     0,     0,     0,     0,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   266,     0,   267,   268,   269,     0,   270,   271,
     272,     0,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   263,   282,   283,   284,   264,     0,   285,   286,   287,
       0,     0,   864,   311,     0,     0,     0,     0,   260,     0,
       0,     0,     0,   261,     0,     0,     0,     0,     0,   265,
       0,     0,     0,   262,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   266,     0,   267,   268,   269,     0,
     270,   271,   272,     0,   273,   274,   275,   276,   277,   278,
     279,   280,   281,     0,   282,   283,   284,     0,     0,   285,
     286,   287,   263,     0,     0,   311,   264,     0,     0,     0,
     260,     0,     0,   878,     0,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
     265,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   266,     0,   267,   268,   269,
       0,   270,   271,   272,     0,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   263,   282,   283,   284,   264,     0,
     285,   286,   287,     0,     0,   879,   311,     0,     0,     0,
       0,   260,     0,     0,     0,     0,   261,     0,     0,     0,
       0,     0,   265,     0,     0,     0,   262,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   266,     0,   267,
     268,   269,     0,   270,   271,   272,     0,   273,   274,   275,
     276,   277,   278,   279,   280,   281,     0,   282,   283,   284,
       0,     0,   285,   286,   287,   263,     0,     0,   311,   264,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   260,     0,     0,     0,     0,
     261,     0,     0,   265,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,   343,     0,     0,   266,     0,
     267,   268,   269,     0,   270,   271,   272,     0,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   344,   282,   283,
     284,     0,     0,   285,   286,   287,     0,     0,  -393,   263,
       0,     0,     0,   264,     0,     0,     0,   260,     0,     0,
       0,     0,   261,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   262,     0,     0,     0,     0,   265,     0,     0,
       0,     0,     0,     0,   317,     0,     0,     0,     0,     0,
       0,     0,   266,     0,   267,   268,   269,     0,   270,   271,
     272,     0,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   263,   282,   283,   284,   264,   345,   285,   286,   287,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   260,     0,     0,     0,   131,   261,     0,     0,   265,
       0,     0,     0,     0,     0,     0,   262,     0,     0,     0,
       0,   386,     0,     0,   266,     0,   267,   268,   269,     0,
     270,   271,   272,     0,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   387,   282,   283,   284,     0,     0,   285,
     286,   287,     0,     0,     0,   263,     0,     0,     0,   264,
       0,     0,     0,   260,     0,     0,     0,     0,   261,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   262,   484,
       0,     0,     0,   265,     0,     0,     0,     0,     0,     0,
     485,     0,     0,     0,     0,     0,     0,     0,   266,     0,
     267,   268,   269,     0,   270,   271,   272,     0,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   263,   282,   283,
     284,   264,     0,   285,   286,   287,     0,     0,     0,     0,
       0,     0,     0,     0,   260,     0,     0,     0,     0,   261,
       0,     0,     0,     0,     0,   265,     0,     0,     0,   262,
     486,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     266,   487,   267,   268,   269,     0,   270,   271,   272,     0,
     273,   274,   275,   276,   277,   278,   279,   280,   281,     0,
     282,   283,   284,     0,     0,   285,   286,   287,   263,     0,
       0,     0,   264,     0,     0,     0,   260,     0,     0,     0,
       0,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   262,     0,     0,     0,     0,   265,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   266,     0,   267,   268,   269,     0,   270,   271,   272,
       0,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     263,   282,   283,   284,   264,     0,   285,   286,   287,     0,
       0,     0,     0,     0,     0,     0,     0,   260,     0,     0,
       0,     0,   261,     0,     0,     0,     0,     0,   265,     0,
       0,     0,   262,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   266,     0,   267,   268,   269,     0,   270,
     271,   272,     0,   273,   274,   275,   276,   277,   278,   279,
     280,   281,     0,   282,   283,   284,     0,   377,   285,   286,
     287,   263,     0,     0,     0,   264,     0,     0,     0,   260,
       0,     0,     0,     0,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   262,     0,     0,     0,     0,   265,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   266,     0,   267,   268,   269,     0,
     270,   271,   272,     0,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   263,   282,   283,   284,   264,   379,   285,
     286,   287,     0,     0,     0,     0,     0,     0,     0,     0,
     260,     0,     0,     0,     0,   261,     0,     0,     0,     0,
       0,   265,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   266,     0,   267,   268,
     269,     0,   270,   271,   272,     0,   273,   274,   275,   276,
     277,   278,   279,   280,   281,     0,   282,   283,   284,     0,
     381,   285,   286,   287,   263,     0,     0,     0,   264,     0,
       0,     0,   260,     0,     0,     0,     0,   261,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   262,     0,     0,
       0,     0,   265,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   266,     0,   267,
     268,   269,     0,   270,   271,   272,     0,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   263,   282,   283,   284,
     264,   389,   285,   286,   287,     0,     0,     0,     0,     0,
       0,     0,     0,   260,     0,     0,     0,     0,   261,     0,
       0,     0,     0,   403,   265,     0,     0,     0,   262,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   266,
       0,   267,   268,   269,     0,   270,   271,   272,     0,   273,
     274,   275,   276,   277,   278,   279,   280,   281,     0,   282,
     283,   284,     0,     0,   285,   286,   287,   263,     0,     0,
       0,   264,     0,     0,     0,   260,     0,     0,   488,     0,
     261,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,   265,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     266,     0,   267,   268,   269,     0,   270,   271,   272,     0,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   263,
     282,   283,   284,   264,     0,   285,   286,   287,     0,     0,
       0,     0,     0,     0,     0,     0,   260,     0,     0,     0,
       0,   261,     0,   131,     0,     0,     0,   265,     0,     0,
       0,   262,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   266,     0,   267,   268,   269,     0,   270,   271,
     272,     0,   273,   274,   275,   276,   277,   278,   279,   280,
     281,     0,   282,   283,   284,     0,     0,   285,   286,   287,
     263,     0,     0,     0,   264,     0,     0,     0,   260,   643,
       0,     0,     0,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   262,     0,     0,     0,     0,   265,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   266,     0,   267,   268,   269,     0,   270,
     271,   272,     0,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   263,   282,   283,   284,   264,   620,   285,   286,
     287,     0,     0,     0,     0,     0,     0,     0,     0,   260,
       0,     0,     0,     0,   261,     0,     0,     0,     0,     0,
     265,     0,     0,     0,   262,     0,     0,   646,     0,     0,
       0,     0,     0,     0,     0,   266,     0,   267,   268,   269,
       0,   270,   271,   272,     0,   273,   274,   275,   276,   277,
     278,   279,   280,   281,     0,   282,   283,   284,     0,     0,
     285,   286,   287,   263,     0,     0,     0,   264,     0,     0,
       0,   260,     0,     0,     0,     0,   261,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   262,     0,     0,     0,
       0,   265,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   266,     0,   267,   268,
     269,     0,   270,   271,   272,     0,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   263,   282,   283,   284,   264,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,   260,     0,     0,     0,     0,   261,     0,     0,
       0,     0,   680,   265,     0,     0,     0,   262,   736,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   266,     0,
     267,   268,   269,     0,   270,   271,   272,     0,   273,   274,
     275,   276,   277,   278,   279,   280,   281,     0,   282,   283,
     284,     0,     0,   285,   286,   287,   263,     0,     0,     0,
     264,     0,     0,     0,   260,     0,     0,     0,     0,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     739,     0,     0,     0,   265,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   266,
       0,   267,   268,   269,     0,   270,   271,   272,     0,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   263,   282,
     283,   284,   264,     0,   285,   286,   287,     0,     0,     0,
       0,     0,     0,     0,     0,   260,     0,     0,     0,     0,
     261,     0,     0,     0,     0,     0,   265,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   266,     0,   267,   268,   269,     0,   270,   271,   272,
       0,   273,   274,   275,   276,   277,   278,   279,   280,   281,
       0,   282,   283,   284,     0,     0,   285,   286,   287,   263,
       0,     0,     0,   264,     0,     0,     0,   260,     0,     0,
     805,     0,   261,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   262,     0,     0,     0,     0,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   266,     0,   267,   268,   269,     0,   270,   271,
     272,     0,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   263,   282,   283,   284,   264,     0,   285,   286,   287,
       0,     0,   808,     0,     0,     0,     0,     0,   260,   821,
       0,     0,     0,   261,     0,     0,     0,     0,     0,   265,
       0,     0,     0,   262,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   266,     0,   267,   268,   269,     0,
     270,   271,   272,     0,   273,   274,   275,   276,   277,   278,
     279,   280,   281,     0,   282,   283,   284,     0,     0,   285,
     286,   287,   263,     0,     0,     0,   264,     0,     0,     0,
     260,     0,     0,     0,     0,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
     265,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   266,     0,   267,   268,   269,
       0,   270,   271,   272,     0,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   263,   282,   283,   284,   264,     0,
     285,   286,   287,     0,     0,   885,     0,     0,     0,     0,
       0,   260,     0,     0,     0,     0,   261,     0,     0,     0,
       0,     0,   265,     0,     0,     0,   262,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   266,     0,   267,
     268,   269,     0,   270,   271,   272,     0,   273,   274,   275,
     276,   277,   278,   279,   280,   281,     0,   282,   283,   284,
       0,     0,   285,   286,   287,   263,     0,     0,     0,   264,
       0,     0,     0,   260,     0,     0,   886,     0,   261,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   262,     0,
       0,     0,     0,   265,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   266,     0,
     267,   268,   269,     0,   270,   271,   272,     0,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   263,   282,   283,
     284,   264,     0,   285,   286,   287,     0,     0,   887,     0,
       0,     0,     0,     0,   260,     0,     0,     0,     0,   261,
       0,     0,     0,     0,     0,   265,     0,     0,     0,   262,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     266,     0,   267,   268,   269,     0,   270,   271,   272,     0,
     273,   274,   275,   276,   277,   278,   279,   280,   281,     0,
     282,   283,   284,     0,     0,   285,   286,   287,   263,     0,
       0,     0,   264,     0,     0,     0,   260,     0,     0,   888,
       0,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   262,     0,     0,     0,     0,   265,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   266,     0,   267,   268,   269,     0,   270,   271,   272,
       0,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     263,   282,   283,   284,   264,     0,   285,   286,   287,     0,
       0,   889,     0,     0,     0,     0,     0,   260,     0,     0,
       0,     0,   261,     0,     0,     0,     0,     0,   265,     0,
       0,     0,   262,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   266,     0,   267,   268,   269,     0,   270,
     271,   272,     0,   273,   274,   275,   276,   277,   278,   279,
     280,   281,     0,   282,   283,   284,     0,     0,   285,   286,
     287,   263,     0,     0,     0,   264,     0,     0,     0,   260,
       0,     0,   890,     0,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   262,     0,     0,     0,     0,   265,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   266,     0,   267,   268,   269,     0,
     270,   271,   272,     0,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   263,   282,   283,   284,   264,     0,   285,
     286,   287,     0,     0,     0,     0,     0,     0,     0,     0,
     260,     0,     0,     0,     0,   261,     0,     0,     0,     0,
       0,   265,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   266,     0,   267,   268,
     269,     0,   270,   271,   272,     0,   273,   274,   275,   276,
     277,   278,   279,   280,   281,     0,   282,   283,   284,     0,
       0,   285,   286,   287,   608,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   265,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   266,     0,   267,
     268,   269,     0,   270,   271,   272,     0,   273,   274,   275,
     276,   277,   278,   279,   280,   281,     0,   282,   283,   284,
       0,     0,   285,   286,   287
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-805)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       2,     2,   382,    43,    44,   358,   353,   512,   659,    11,
     350,   153,   682,   308,    16,    15,     3,    19,    20,    21,
     531,   242,   164,     3,    26,    27,    28,    29,     3,   382,
       3,    24,     3,    35,    36,    37,     1,    39,    40,   728,
      17,   834,     1,    37,    46,   682,    48,    49,    50,    88,
     344,    53,    54,    55,    56,     3,    53,    59,    60,    61,
      67,   112,    64,   867,    46,    67,    68,    69,    26,    71,
      26,    75,    74,    21,    76,    21,    78,    59,   254,   255,
     256,   257,    26,    26,    37,    65,    63,    37,    85,    37,
     141,   112,    40,   387,    76,   176,    37,     1,   902,    37,
       0,   606,    67,    37,   143,    82,    54,    55,    67,    55,
      81,   192,    77,   906,    88,    65,   767,    65,   112,    65,
      67,    86,   143,    13,    26,    93,     3,    86,    18,   210,
     132,   138,    80,    37,    80,   137,   138,   139,   140,   141,
     142,   143,   139,   813,    88,   656,   111,    88,   116,   149,
      27,   138,   111,   140,   112,   159,   140,   794,   142,   112,
     140,   143,   851,   852,     3,   140,   131,   140,     2,   140,
      21,   112,   176,   138,   112,   177,   813,    11,   112,   138,
     138,   183,   138,   530,   131,    19,    21,    21,   192,    93,
     143,   138,   140,    80,   138,   138,     7,    47,   200,   201,
     193,   112,   143,    80,    55,   143,   210,   111,   112,   143,
     140,   112,     3,   112,    65,   866,     7,   512,   112,    53,
      55,    55,    56,   304,   305,    75,   112,   131,   139,    80,
      65,   140,   234,   234,   131,   237,   412,    41,   139,    61,
      74,   902,   141,   140,    78,   142,   395,   141,   397,    84,
      41,   140,   913,   758,   137,   141,    60,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,    60,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,    73,   798,   112,   510,
     511,    21,     3,   112,   375,   112,     7,    93,   132,    93,
     304,   305,   140,   650,   306,   307,   308,    37,   648,   311,
      40,   606,   131,   112,   131,   317,   112,   141,   112,   321,
     116,   323,   116,   112,    54,    55,   112,   140,   112,   112,
     131,   333,   131,   335,   336,    65,   338,   339,   559,   560,
     138,   343,   344,   177,   346,   141,   112,   141,   131,   183,
      80,   333,   141,   112,   112,   141,   358,   141,   360,     2,
       2,   263,   264,   112,   368,   140,   368,   112,    11,    11,
     112,   375,   131,   131,   131,   141,    19,    19,    21,    21,
     382,   131,   131,    64,   386,   387,   131,    68,   392,   131,
     392,   395,   772,   397,   131,   131,   400,   399,   400,   401,
     234,   403,   404,    64,   306,   307,   408,    68,   550,   111,
      53,    53,   114,   766,   131,   317,   118,   131,   140,   772,
     142,   140,   131,   142,    55,    56,   408,   131,    87,    25,
     140,    74,    74,   131,   336,    78,    78,   339,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,    20,   140,   114,    93,   131,     7,
      47,   111,   802,   758,    25,    31,    32,   138,    64,   131,
      25,    93,    68,    39,   141,    41,   131,   311,   112,    37,
     140,    28,   484,   485,   486,   487,   488,   489,   490,   132,
     132,   491,   492,   497,    60,   499,   141,   399,   112,    93,
     502,   335,   504,    64,   338,    16,   508,    68,   141,    64,
     512,   107,   346,    68,   867,   111,   141,   113,   114,   112,
     522,   141,   118,   876,   143,   138,   131,   131,   131,   125,
     131,   533,   131,   131,   177,   177,   132,   133,   134,   131,
     183,   183,   131,    25,   141,   395,   131,   397,   131,   902,
     111,   533,   113,   114,   131,   131,   111,   118,   113,   114,
     913,   131,   564,   118,   125,   143,   131,   401,   131,   131,
     125,   132,   133,   134,   131,    17,   112,   131,    20,   134,
     131,    28,    64,   485,   112,   487,    68,    29,   141,    31,
      32,   234,   234,    35,   131,   141,   131,    39,   138,    41,
      93,    65,    26,    26,   606,   143,   608,   609,   610,   138,
     111,   613,    65,   111,   114,    57,    25,    59,    60,   143,
      62,    63,   143,   114,   140,    86,   628,   141,   630,   111,
     386,   633,   114,   141,   636,   681,   118,    77,    80,   641,
      82,   643,   668,   645,   646,   218,   387,   497,   165,   501,
     744,   684,   802,   801,   801,    64,   648,   687,   906,    68,
     866,   663,   664,   881,    28,   361,   822,   499,   311,   311,
      -1,    -1,    -1,   675,    -1,   677,    -1,    -1,   680,   681,
      -1,   683,   664,    -1,    -1,    -1,   536,    -1,   522,   131,
      -1,    -1,   335,   335,    -1,   338,   338,   139,   107,    -1,
     109,   110,   111,   346,   113,   114,   608,   609,    -1,   118,
      -1,    -1,    -1,    -1,    -1,   124,   125,    -1,    -1,    -1,
     129,    -1,   724,   132,   133,   134,    -1,    -1,    -1,    -1,
     564,    -1,    -1,   735,   736,    -1,   738,   739,   740,   741,
      -1,    -1,    -1,    -1,   746,   747,    -1,   749,    -1,   751,
     752,   791,   792,   755,    -1,    -1,   758,    -1,   401,   401,
      -1,    -1,   766,    -1,   766,    -1,    -1,    -1,     3,   771,
     772,   773,     7,    -1,     9,   777,     2,   779,    -1,    -1,
     782,   783,    25,    -1,   786,    11,    21,   789,   789,    -1,
      -1,    -1,    -1,    19,   628,    21,   630,    -1,    -1,   633,
      -1,    -1,   636,   805,   806,   807,   808,   809,   810,   811,
     812,   645,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   821,
      55,    64,   824,   825,   826,    68,   830,    53,   830,   663,
      65,   833,    -1,    -1,    -1,    -1,   479,    -1,    -1,    -1,
      -1,   675,    -1,   677,   826,    80,    -1,   681,    74,    84,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,   861,
     862,   863,   864,   867,   107,   867,   109,   110,   111,    -1,
     113,   114,    -1,    -1,   876,   118,   878,   879,    -1,   522,
     522,    -1,   125,   885,   886,   887,   888,   889,   890,   132,
     133,   134,    -1,    -1,    -1,    -1,     2,    -1,   902,   901,
     902,    -1,    -1,    49,    37,    11,   132,    -1,    -1,   913,
      -1,   913,   746,    19,    -1,    21,    -1,   751,    -1,    -1,
      -1,   564,   564,    -1,     2,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    11,    67,    -1,   579,    -1,    -1,   773,
      -1,    19,    -1,    21,    77,    -1,    -1,    53,   782,    -1,
      -1,   177,   786,    86,    -1,   789,    -1,   183,    -1,    -1,
      93,    -1,    -1,    -1,    -1,   608,   609,    -1,    74,    -1,
      -1,    -1,    78,    -1,    -1,    53,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,   628,   628,   630,   630,    -1,
     633,   633,    -1,   636,   636,    -1,    74,     3,   131,    -1,
      78,     7,   645,   645,    -1,   138,    -1,    25,   234,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
     663,   663,   168,   169,   170,    -1,   132,   861,   862,   863,
     864,    -1,   675,   675,   677,   677,    -1,    -1,   681,   681,
     186,    -1,    -1,   189,   878,   879,    64,    -1,    -1,    55,
      68,    -1,    -1,     2,   132,    -1,    -1,    -1,    -1,    65,
      -1,   207,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,   177,    21,    -1,    80,    -1,    -1,   183,    84,   225,
      -1,    -1,    -1,    -1,    -1,   311,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   177,
     118,    -1,    -1,   746,    53,   183,   124,   125,   751,   335,
      -1,   129,   338,    -1,   132,   133,   134,    -1,    -1,    -1,
     346,    -1,    -1,    -1,    -1,    74,    -1,    -1,   234,    78,
     773,   773,    -1,     3,     4,     5,     6,     7,     8,   782,
     782,    -1,    12,   786,   786,    -1,   789,   789,    -1,    -1,
      37,    37,    -1,    -1,    -1,    -1,   234,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,
      -1,    -1,   318,    43,    44,   401,    46,    -1,    48,    49,
      67,    67,    -1,   132,    -1,    -1,    56,    -1,    58,    -1,
      77,    77,    -1,    -1,    -1,    -1,   342,    -1,    -1,    86,
      86,    71,    72,    73,    74,   311,    93,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   861,   862,
     863,   864,    -1,    -1,    -1,   112,   112,    -1,   177,   335,
      -1,    -1,   338,   311,   183,   878,   879,    -1,   108,    -1,
     346,    -1,    -1,    -1,   131,   131,    -1,    -1,    25,    -1,
      -1,   138,   138,    -1,    -1,    -1,   126,   335,    -1,    -1,
     338,    -1,    17,    -1,    -1,    20,    -1,    -1,   346,    -1,
     140,    -1,   142,     2,    29,    -1,    31,    32,    -1,    -1,
      35,    -1,    11,    -1,    39,   234,    41,    64,    -1,    -1,
      19,    68,    21,    -1,    -1,   401,   522,    -1,    -1,    -1,
      -1,    -1,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   401,    53,    80,    -1,    82,    17,    -1,
     107,    20,    -1,   110,   111,    -1,   113,   114,   564,    -1,
      29,   118,    31,    32,    -1,    74,    35,    -1,   125,    78,
      39,    -1,    41,    -1,    -1,   132,   133,   134,    -1,    -1,
     506,   507,   311,    -1,   510,   511,    -1,    -1,    57,    -1,
      59,    60,    -1,    62,    63,    -1,   131,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,   335,    20,    -1,   338,
      -1,    80,    -1,    82,    -1,    -1,    -1,   346,    31,    32,
      -1,    -1,   628,   132,   630,    -1,    39,   633,    41,    -1,
     636,    -1,    -1,    -1,    47,     2,   522,    -1,    -1,   645,
      -1,    -1,    -1,    -1,    11,    -1,    -1,    60,    61,    -1,
      -1,    -1,    19,    -1,    21,    -1,    -1,   663,    -1,    -1,
      -1,    -1,   131,    -1,   522,    -1,    -1,    -1,   177,   675,
      -1,   677,   401,    -1,   183,   681,    20,    -1,   564,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    31,    32,    -1,
      -1,    -1,   618,   619,    -1,    39,    -1,    41,    -1,   625,
      -1,    -1,    -1,    47,    -1,    -1,   564,    74,    -1,   635,
      -1,    78,   638,    -1,    -1,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   234,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     746,    -1,   628,    -1,   630,   751,    -1,   633,    -1,    -1,
     636,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   645,
      -1,    -1,    -1,    -1,    -1,   132,    -1,   773,    -1,    -1,
     628,    -1,   630,    -1,    -1,   633,   782,   663,   636,    -1,
     786,    -1,    -1,   789,    -1,    -1,    -1,   645,    -1,   675,
      -1,   677,    -1,   522,    -1,   681,    -1,    -1,    -1,    -1,
      -1,    -1,   311,    -1,    -1,   663,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,   183,   675,    -1,   677,
      -1,    -1,    -1,   681,    -1,    -1,   335,    -1,    -1,   338,
      -1,    -1,    -1,    -1,    -1,   564,    -1,   346,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   775,
      -1,    -1,    -1,    -1,    -1,   861,   862,   863,   864,    -1,
     746,    -1,    -1,    -1,    -1,   751,    -1,   234,    -1,    -1,
      -1,    -1,   878,   879,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   773,   746,    -1,
      -1,    -1,   401,   751,    -1,    -1,   782,    -1,    -1,   628,
     786,   630,    -1,   789,   633,    -1,    -1,   636,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   773,   645,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   782,    -1,    -1,    -1,   786,    -1,
      -1,   789,    -1,    -1,   663,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   311,    -1,   675,    -1,   677,    -1,
      49,    -1,   681,    -1,    -1,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   335,    -1,
      -1,   338,    -1,    -1,    -1,   861,   862,   863,   864,   346,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   878,   879,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   861,   862,   863,   864,    25,    -1,    -1,
      -1,    -1,    -1,   522,    -1,    -1,    -1,   746,    -1,    -1,
     878,   879,   751,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   401,    -1,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    -1,   773,    -1,    64,    -1,    -1,    -1,
      68,    -1,    -1,   782,    -1,   564,    -1,   786,    -1,    -1,
     789,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,
     169,   170,    -1,    -1,    92,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,   107,
     189,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,   207,   127,
     128,   129,    -1,    -1,   132,   133,   134,    -1,    -1,   628,
      -1,   630,    -1,    -1,   633,    -1,   225,   636,    -1,    -1,
      -1,    -1,   861,   862,   863,   864,   645,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   878,
     879,    -1,    -1,    -1,   663,   522,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   675,    -1,   677,    -1,
      -1,    -1,   681,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    13,    -1,   564,    -1,    -1,
      18,    -1,    -1,    -1,    27,    -1,    24,    25,    -1,    -1,
      33,    34,    -1,    36,    32,    38,    34,    -1,    -1,   318,
      43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    58,    -1,   746,    -1,    57,
      -1,    -1,   751,   342,    -1,    -1,    -1,    -1,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   628,    -1,   630,   773,    -1,   633,    -1,    -1,   636,
      -1,    -1,    -1,   782,    -1,    -1,    -1,   786,   645,    -1,
     789,    99,    -1,   101,   107,   108,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   663,    -1,    -1,    -1,
      -1,   124,    -1,   126,    -1,    -1,   129,    -1,   675,    -1,
     677,   134,    -1,    -1,   681,   138,    -1,   140,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,    -1,    -1,   164,    -1,   166,    -1,
      -1,    10,   861,   862,   863,   864,    15,    -1,   176,    -1,
      -1,   179,    -1,    -1,    -1,    -1,    25,    -1,    -1,   878,
     879,    -1,    -1,    -1,   192,   193,    -1,    -1,   196,   746,
      -1,    -1,    -1,    -1,   751,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,   773,   506,   507,    68,
      -1,   510,   511,    -1,    -1,   782,    -1,    -1,    -1,   786,
      -1,    -1,   789,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    92,    -1,    -1,   254,   255,   256,   257,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   546,   107,    -1,
     109,   110,   111,   271,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,    -1,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,   304,   305,    -1,    -1,
      -1,    -1,    -1,    -1,   861,   862,   863,   864,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   878,   879,   331,    -1,    -1,    -1,    -1,    -1,   618,
     619,    -1,    -1,    -1,    -1,    -1,   625,    -1,   627,    -1,
      -1,    -1,    -1,   632,    -1,   353,   635,    -1,    -1,   638,
      -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,    25,
      -1,    -1,    -1,    -1,     1,    -1,     3,     4,     5,     6,
       7,     8,   671,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   407,
      27,    -1,   410,    -1,   412,    -1,    33,    34,    64,    36,
      -1,    38,    68,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,    92,   122,   123,   124,   125,
      -1,   127,    -1,   129,    -1,    -1,   132,   133,   134,    -1,
     107,   108,   109,   110,    -1,    -1,   775,    -1,   115,    -1,
      -1,   499,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,    -1,    -1,   526,    -1,
      -1,    -1,   530,    -1,    -1,    25,   815,   816,    -1,   818,
     819,    -1,     3,     4,     5,     6,     7,     8,   546,    -1,
      -1,    12,   550,    -1,    -1,   834,    -1,    -1,    -1,    -1,
      -1,   840,    -1,   842,    -1,   844,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    64,    36,    -1,    38,    68,    -1,
      -1,   579,    43,    44,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,    74,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   906,   118,   119,
     120,    92,   122,   123,   124,   125,    -1,    -1,    -1,   129,
      -1,    -1,   132,   133,   134,    -1,   107,   108,   109,   110,
      -1,    -1,   650,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,
     668,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,
      -1,   142,   143,    -1,    -1,    -1,   684,    -1,    -1,     0,
       1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,   703,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   790,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,   801,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    10,
      -1,    12,    13,    14,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
     131,   132,   133,   134,    -1,    -1,    -1,   138,   139,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,
     131,    -1,    -1,   134,    -1,    -1,    -1,   138,   139,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    -1,    -1,   134,    -1,    -1,    -1,   138,   139,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    -1,    -1,   134,    -1,    -1,    -1,   138,   139,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    -1,    -1,   134,    -1,    -1,    -1,   138,   139,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    -1,    -1,   134,    -1,    -1,    -1,   138,   139,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    -1,    -1,   134,    -1,    25,    -1,   138,    -1,   140,
       1,   142,     3,     4,     5,     6,     7,     8,    -1,    10,
      -1,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    64,    36,    -1,    38,    68,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    68,    -1,    -1,
      71,    72,    73,    74,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,    92,   122,   123,   124,   125,    -1,   127,    -1,   129,
      -1,    -1,   132,   133,   134,    -1,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
      -1,   132,   133,   134,    -1,    -1,    -1,   138,    -1,   140,
      -1,   142,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,    74,    -1,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,   135,   136,   137,
      -1,    -1,   140,   124,   142,   126,    -1,   128,   129,   130,
      -1,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,
      -1,   142,   143,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,
      -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    73,    74,    -1,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,   135,   136,
      -1,    -1,    -1,   140,   124,   142,   126,    -1,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,
     140,    -1,   142,   143,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    37,    38,
      -1,    40,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    54,    55,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      79,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      79,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,   141,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      79,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,   141,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    55,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    32,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
     139,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,     3,    -1,    -1,    56,    -1,    58,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,   108,
      10,    -1,    -1,   112,    -1,    15,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,   140,   141,   142,    93,    94,    95,    96,    97,    98,
      -1,    -1,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,    -1,
     129,    -1,    -1,   132,   133,   134,   135,    -1,   137,    -1,
      -1,   140,    -1,    93,    94,    95,    96,    97,    98,    -1,
      -1,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,    -1,    -1,   113,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,    -1,   129,
      -1,    10,   132,   133,   134,   135,    15,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    64,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    64,    -1,    -1,    -1,    68,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,    92,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,   143,    68,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    10,   127,   128,   129,    92,
      15,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,   143,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    10,   127,   128,   129,    -1,    15,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    25,    64,
     143,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    68,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    92,    -1,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    10,    -1,   132,   133,   134,    15,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    25,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    26,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    68,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      25,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    92,
     127,   128,   129,    -1,    -1,   132,   133,   134,    -1,    64,
      -1,   138,    -1,    68,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    10,   127,   128,   129,    92,    15,   132,
     133,   134,    -1,    -1,    -1,   138,    -1,    -1,    25,    26,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    64,
     127,   128,   129,    68,    -1,   132,   133,   134,    -1,    -1,
      75,   138,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134,
      64,    -1,    -1,   138,    68,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    64,   127,   128,   129,    68,    -1,   132,   133,
     134,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,    -1,    -1,
     132,   133,   134,    64,    -1,    -1,   138,    68,    -1,    -1,
      -1,    10,    -1,    -1,    75,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    64,   127,   128,   129,    68,
      -1,   132,   133,   134,    -1,    -1,    75,   138,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,    -1,    -1,   132,   133,   134,    64,    -1,    -1,   138,
      68,    -1,    -1,    -1,    10,    -1,    -1,    75,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    64,   127,
     128,   129,    68,    -1,   132,   133,   134,    -1,    -1,    75,
     138,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,    -1,    -1,   132,   133,   134,    64,
      -1,    -1,   138,    68,    -1,    -1,    -1,    10,    -1,    -1,
      75,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    64,   127,   128,   129,    68,    -1,   132,   133,   134,
      -1,    -1,    75,   138,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,    -1,    -1,   132,
     133,   134,    64,    -1,    -1,   138,    68,    -1,    -1,    -1,
      10,    -1,    -1,    75,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    64,   127,   128,   129,    68,    -1,
     132,   133,   134,    -1,    -1,    75,   138,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
      -1,    -1,   132,   133,   134,    64,    -1,    -1,   138,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    30,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    52,   127,   128,
     129,    -1,    -1,   132,   133,   134,    -1,    -1,   137,    64,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    64,   127,   128,   129,    68,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    88,    15,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    30,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    52,   127,   128,   129,    -1,    -1,   132,
     133,   134,    -1,    -1,    -1,    64,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    64,   127,   128,
     129,    68,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    37,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,    -1,   132,   133,   134,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      64,   127,   128,   129,    68,    -1,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,    -1,   131,   132,   133,
     134,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    64,   127,   128,   129,    68,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,    -1,
     131,   132,   133,   134,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    64,   127,   128,   129,
      68,   131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    91,    92,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,    -1,    -1,   132,   133,   134,    64,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    75,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    64,
     127,   128,   129,    68,    -1,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    64,   127,   128,   129,    68,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    25,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,    -1,    -1,
     132,   133,   134,    64,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    64,   127,   128,   129,    68,
      -1,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    91,    92,    -1,    -1,    -1,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,    -1,    -1,   132,   133,   134,    64,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    64,   127,
     128,   129,    68,    -1,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,    -1,    -1,   132,   133,   134,    64,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,
      75,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    64,   127,   128,   129,    68,    -1,   132,   133,   134,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,    -1,    -1,   132,
     133,   134,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    64,   127,   128,   129,    68,    -1,
     132,   133,   134,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
      -1,    -1,   132,   133,   134,    64,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    10,    -1,    -1,    75,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    64,   127,   128,
     129,    68,    -1,   132,   133,   134,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,    -1,   132,   133,   134,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    75,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      64,   127,   128,   129,    68,    -1,   132,   133,   134,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,    -1,    -1,   132,   133,
     134,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,
      -1,    -1,    75,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    64,   127,   128,   129,    68,    -1,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,    -1,
      -1,   132,   133,   134,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
      -1,    -1,   132,   133,   134
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   148,   149,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    17,    18,    19,    20,    22,    23,
      24,    26,    27,    29,    31,    32,    33,    34,    35,    36,
      38,    39,    42,    43,    44,    45,    46,    48,    49,    51,
      56,    57,    58,    59,    62,    63,    66,    67,    69,    70,
      71,    72,    73,    74,    76,    78,    79,    80,    82,    83,
      87,    89,    92,   107,   108,   109,   110,   115,   124,   126,
     128,   129,   131,   134,   138,   140,   142,   150,   151,   152,
     153,   154,   156,   161,   162,   163,   165,   168,   169,   170,
     171,   172,   173,   175,   176,   177,   181,   182,   185,   186,
     189,   190,   193,   196,   197,   220,   223,   224,   242,   243,
     244,   245,   246,   247,   254,   255,   256,   257,   259,   260,
     261,   262,   263,   264,   266,   267,   268,   269,   270,   152,
     257,    88,   248,   249,   164,   165,   248,    12,    33,    34,
      36,    74,   138,   142,   193,   242,   246,   255,   256,   257,
     259,   261,   262,    80,   164,   152,   236,   257,   152,   140,
       7,   164,   166,     9,    80,   166,    55,    90,   174,   257,
     257,    20,    32,   223,   257,   257,   140,   165,   194,   140,
     165,   225,   226,    26,   156,   167,   257,   257,   257,   257,
     257,     7,   140,    32,   170,   170,   130,   216,   235,   257,
      78,    79,   131,   257,   258,   257,   167,   257,   257,    73,
     140,   152,   257,   257,   156,   163,   257,   259,   156,   163,
     257,   165,   221,   235,   257,   257,   257,   257,   257,   257,
     257,   257,   139,   150,   157,   235,    81,   116,   216,   237,
     238,   258,   235,   257,   265,    57,   152,    61,   155,   165,
     165,    41,    60,   211,    21,    55,    65,    84,   131,   137,
      10,    15,    25,    64,    68,    92,   107,   109,   110,   111,
     113,   114,   115,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   127,   128,   129,   132,   133,   134,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   135,   136,   140,   142,    64,    68,   140,   152,
     131,   138,   156,   257,   257,   257,   235,    37,   248,   221,
     131,   112,   131,    87,   165,   216,   239,   240,   241,   258,
     221,   186,   165,   140,   167,    26,    37,   167,    26,    37,
      88,   167,   251,    30,    52,   131,    75,   156,   239,   152,
     140,   205,    81,   140,   165,   227,   228,     1,   111,   230,
      37,   112,   152,   167,   167,   239,   166,   165,   112,   131,
     257,   257,   131,   138,   167,   140,   239,   131,   178,   131,
     178,   131,    93,   222,   131,   131,    30,    52,   167,   131,
     139,   150,   112,   139,   257,   112,   141,   112,   141,    37,
     112,   143,   251,    91,   112,   143,     7,    47,   111,   187,
     138,   198,    65,   225,   225,   225,   225,   257,   257,   257,
     257,   174,   257,   174,   257,   257,   257,   257,   257,   257,
     257,    27,    80,   165,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   239,   239,   174,   257,
     174,   257,    21,    37,    40,    54,    55,    65,    80,   209,
     250,   253,   257,   270,    26,    37,    26,    37,    75,    37,
     143,   174,   257,   167,   131,   257,   257,    93,   141,   112,
     131,   165,    37,   235,    36,   257,   174,   257,    36,   257,
     174,   257,   140,   167,   134,   158,   160,   257,   158,   159,
     152,   257,    28,   141,   206,   207,   208,   209,   195,   228,
     112,   141,     1,   142,   231,   243,    93,   229,   257,   226,
     141,   216,   257,   183,   239,   141,    16,   179,   231,   243,
     112,   160,   159,   139,   141,   141,   237,   141,   237,   174,
     257,   143,   152,   257,   143,   257,   143,   257,   165,   235,
     138,   165,   191,   192,    21,    55,    65,    80,   200,   210,
     225,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   141,   143,    37,    65,   242,   112,   141,    64,    64,
      36,   174,   257,    36,   174,   257,   174,   257,   248,   248,
     131,   216,   258,   241,   187,   257,   141,   257,    26,   167,
      26,   167,   257,    26,   167,   251,    26,   167,   251,   252,
     253,   112,   131,    11,   131,    28,    28,   152,   112,   141,
     140,   165,    21,    55,    65,    80,   212,   141,   228,   230,
     235,    50,   258,    53,    85,   139,   184,   141,   140,   156,
     165,   180,   221,   131,   131,   143,   251,   143,   251,   152,
      91,   138,   188,    93,   112,   139,    65,   199,    10,    15,
      93,    94,    95,    96,    97,    98,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   113,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   129,   132,
     133,   134,   135,   137,   140,   165,   201,   202,   203,   242,
     131,   253,   242,   242,   257,    26,    26,   257,    26,    26,
     143,   143,   167,   167,   138,   167,    75,    36,   257,    36,
     257,    75,    36,   257,   167,    36,   257,   167,   112,   141,
     257,   257,   152,   257,   207,   228,   111,   234,    65,   230,
     229,    37,   143,    26,   152,   235,   180,   111,   156,    36,
     152,   257,   143,    36,   152,   257,   143,   257,   139,   157,
      32,    59,    62,   139,   151,   169,   257,   192,   212,   165,
     257,   114,   140,   204,   204,    75,    36,    36,    75,    36,
      36,    36,    36,   188,   257,   257,   257,   257,   257,   257,
     253,    11,   141,    27,    71,    74,   142,   216,   233,   243,
     116,   218,   229,    86,   219,   257,   231,   243,   152,   167,
     141,   257,   257,   152,   257,   152,   139,   169,   230,   141,
     165,   202,   203,   206,   257,   257,   257,   257,   257,   257,
     139,    75,    75,    75,    75,   257,   234,   143,   235,   216,
     217,   257,   257,   156,   168,   215,   143,   156,    75,    75,
      77,   213,   204,   204,   141,    75,    75,    75,    75,    75,
      75,   257,   257,   257,   257,   218,   229,   216,   232,   233,
     243,    37,   143,   243,   257,   257,   219,   257,   232,   233,
     131,   214,   215,   143,   232
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
#line 441 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 446 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 447 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 454 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 459 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 460 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 479 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 480 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 481 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 482 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 483 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 484 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 485 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 486 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 487 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 488 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 489 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 490 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 491 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 492 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 493 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 494 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 499 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (6)].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[(1) - (6)].b), (yyvsp[(2) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 502 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType, (yyvsp[(6) - (7)].pblockstmt), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 507 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 508 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 512 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 513 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 526 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 527 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 532 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 533 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 543 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 548 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 552 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 559 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
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
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 578 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 579 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 583 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 588 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 590 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 592 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 594 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 596 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 598 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 600 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 602 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 610 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 616 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 618 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 622 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 627 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 631 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 632 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 636 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 637 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 646 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 648 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 649 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 662 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 663 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 664 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 665 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 670 "chapel.ypp"
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

  case 102:
/* Line 1792 of yacc.c  */
#line 684 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 685 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 686 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 687 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 690 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 694 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 698 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 701 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 707 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 713 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 719 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 725 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 732 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 745 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 748 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 758 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 759 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 760 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 764 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 765 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 770 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 771 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 775 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 776 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 780 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 784 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 788 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 789 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 794 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 796 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 798 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 805 "chapel.ypp"
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

  case 151:
/* Line 1792 of yacc.c  */
#line 815 "chapel.ypp"
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

  case 152:
/* Line 1792 of yacc.c  */
#line 827 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 832 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 837 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 845 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 846 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 851 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 853 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 855 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 860 "chapel.ypp"
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

  case 161:
/* Line 1792 of yacc.c  */
#line 879 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 888 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 896 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 900 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 906 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 907 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 912 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 917 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 922 "chapel.ypp"
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

  case 170:
/* Line 1792 of yacc.c  */
#line 942 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 948 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 955 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 963 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 975 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 981 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 988 "chapel.ypp"
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

  case 177:
/* Line 1792 of yacc.c  */
#line 1010 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 1014 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 1019 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 1023 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 1031 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 1036 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 1037 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 1038 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 1039 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 1040 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 1041 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 1042 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 1043 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1044 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 1045 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1046 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1047 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1048 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1049 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1050 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1051 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1052 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1053 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1054 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1055 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1056 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1057 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1058 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1059 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1060 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1064 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1065 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1066 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1067 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1068 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1069 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1070 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1071 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1072 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1073 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1074 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1075 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1079 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1080 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1084 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1089 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1090 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1097 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1099 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1101 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1105 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1106 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1111 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1112 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1113 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1114 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1115 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1116 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1117 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1118 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1122 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1123 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1124 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1125 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1126 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1127 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1132 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1136 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1137 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1139 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1140 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1141 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1145 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1146 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1149 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1155 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1160 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1162 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1166 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1171 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1172 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1176 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1177 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1182 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1184 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1186 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1191 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1204 "chapel.ypp"
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

  case 275:
/* Line 1792 of yacc.c  */
#line 1220 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1229 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1237 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1245 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1253 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1262 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1271 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1272 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1273 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1279 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1298 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1307 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1313 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1314 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1319 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1320 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1321 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1322 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1343 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1347 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1351 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1358 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1363 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1364 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1377 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1379 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1381 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1386 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1387 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1388 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1389 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1390 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1391 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1397 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1398 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1399 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1400 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1404 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1405 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1409 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1410 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1411 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1415 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1416 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1420 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1425 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1426 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1430 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1431 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1432 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1437 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1454 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1456 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1458 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1460 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1462 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1464 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1466 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1471 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1473 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1479 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1481 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1487 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1493 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1495 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1502 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1508 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1514 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1520 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1554 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1563 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1568 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1571 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1575 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1580 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1583 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1588 "chapel.ypp"
    {
      (yyval.pIntentExpr).tfIntent = (yyvsp[(1) - (2)].pt); (yyval.pIntentExpr).iVar = (yyvsp[(2) - (2)].pexpr); (yyval.pIntentExpr).riExp = NULL;
    }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1591 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1592 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1598 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1624 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1626 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1630 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1634 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1638 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1639 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1640 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1662 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1663 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1664 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1668 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1669 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1670 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1678 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1679 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1680 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1681 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1685 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1686 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1687 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1688 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1690 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1691 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1692 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1693 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1695 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1699 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1707 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1711 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1712 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1713 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1715 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1719 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1720 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1721 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1722 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1724 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1725 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1726 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1727 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1728 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1729 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1730 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1731 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1732 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1733 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1737 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1738 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1739 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1740 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1741 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1742 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1746 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1747 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1748 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1749 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1753 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1754 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1755 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1756 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1761 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1762 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1763 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1764 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1765 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1766 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1767 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 8066 "bison-chapel.cpp"
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


