/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 1

/* Pull parsers.  */
#define YYPULL 0





# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED
# define YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 33 "chapel.ypp"

  #include <string>
  extern int         captureTokens;
  extern std::string captureString;
#line 47 "chapel.ypp"

  #ifndef _BISON_CHAPEL_DEFINES_0_
  #define _BISON_CHAPEL_DEFINES_0_

  #define YYLEX_NEWLINE                  -1
  #define YYLEX_SINGLE_LINE_COMMENT      -2
  #define YYLEX_BLOCK_COMMENT            -3

  typedef void* yyscan_t;

  int processNewline(yyscan_t scanner);
  void stringBufferInit();

  #endif
#line 67 "chapel.ypp"

  #ifndef _BISON_CHAPEL_DEFINES_1_
  #define _BISON_CHAPEL_DEFINES_1_

  #include "symbol.h"

  #include "ImportStmt.h"

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

  struct PotentialRename {
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
    ImportStmt*               pimportstmt;
    ProcIter                  procIter;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<PotentialRename*>* ponlylist;
    std::set<Flag>*           pflagset;
    WhereAndLifetime          lifetimeAndWhere;
  };

  #endif
#line 151 "chapel.ypp"

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
#line 174 "chapel.ypp"

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

#line 256 "bison-chapel.cpp"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
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
    TFOREACH = 297,
    TFORWARDING = 298,
    TIF = 299,
    TIMAG = 300,
    TIMPORT = 301,
    TIN = 302,
    TINCLUDE = 303,
    TINDEX = 304,
    TINLINE = 305,
    TINOUT = 306,
    TINT = 307,
    TITER = 308,
    TINITEQUALS = 309,
    TIMPLEMENTS = 310,
    TINTERFACE = 311,
    TLABEL = 312,
    TLAMBDA = 313,
    TLET = 314,
    TLIFETIME = 315,
    TLOCAL = 316,
    TLOCALE = 317,
    TMINUSMINUS = 318,
    TMODULE = 319,
    TNEW = 320,
    TNIL = 321,
    TNOINIT = 322,
    TNONE = 323,
    TNOTHING = 324,
    TON = 325,
    TONLY = 326,
    TOTHERWISE = 327,
    TOUT = 328,
    TOVERRIDE = 329,
    TOWNED = 330,
    TPARAM = 331,
    TPLUSPLUS = 332,
    TPRAGMA = 333,
    TPRIMITIVE = 334,
    TPRIVATE = 335,
    TPROC = 336,
    TPROTOTYPE = 337,
    TPUBLIC = 338,
    TREAL = 339,
    TRECORD = 340,
    TREDUCE = 341,
    TREF = 342,
    TREQUIRE = 343,
    TRETURN = 344,
    TSCAN = 345,
    TSELECT = 346,
    TSERIAL = 347,
    TSHARED = 348,
    TSINGLE = 349,
    TSPARSE = 350,
    TSTRING = 351,
    TSUBDOMAIN = 352,
    TSYNC = 353,
    TTHEN = 354,
    TTHIS = 355,
    TTHROW = 356,
    TTHROWS = 357,
    TTRUE = 358,
    TTRY = 359,
    TTRYBANG = 360,
    TTYPE = 361,
    TUINT = 362,
    TUNDERSCORE = 363,
    TUNION = 364,
    TUNMANAGED = 365,
    TUSE = 366,
    TVAR = 367,
    TVOID = 368,
    TWHEN = 369,
    TWHERE = 370,
    TWHILE = 371,
    TWITH = 372,
    TYIELD = 373,
    TZIP = 374,
    TALIAS = 375,
    TAND = 376,
    TASSIGN = 377,
    TASSIGNBAND = 378,
    TASSIGNBOR = 379,
    TASSIGNBXOR = 380,
    TASSIGNDIVIDE = 381,
    TASSIGNEXP = 382,
    TASSIGNLAND = 383,
    TASSIGNLOR = 384,
    TASSIGNMINUS = 385,
    TASSIGNMOD = 386,
    TASSIGNMULTIPLY = 387,
    TASSIGNPLUS = 388,
    TASSIGNREDUCE = 389,
    TASSIGNSL = 390,
    TASSIGNSR = 391,
    TBANG = 392,
    TBAND = 393,
    TBNOT = 394,
    TBOR = 395,
    TBXOR = 396,
    TCOLON = 397,
    TCOMMA = 398,
    TDIVIDE = 399,
    TDOT = 400,
    TDOTDOT = 401,
    TDOTDOTDOT = 402,
    TEQUAL = 403,
    TEXP = 404,
    TGREATER = 405,
    TGREATEREQUAL = 406,
    THASH = 407,
    TIO = 408,
    TLESS = 409,
    TLESSEQUAL = 410,
    TMINUS = 411,
    TMOD = 412,
    TNOTEQUAL = 413,
    TOR = 414,
    TPLUS = 415,
    TQUESTION = 416,
    TSEMI = 417,
    TSHIFTLEFT = 418,
    TSHIFTRIGHT = 419,
    TSTAR = 420,
    TSWAP = 421,
    TLCBR = 422,
    TRCBR = 423,
    TLP = 424,
    TRP = 425,
    TLSBR = 426,
    TRSBR = 427,
    TNOELSE = 428,
    TDOTDOTOPENHIGH = 429,
    TUPLUS = 430,
    TUMINUS = 431,
    TLNOT = 432
  };
#endif

/* Value type.  */

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



#ifndef YYPUSH_MORE_DEFINED
# define YYPUSH_MORE_DEFINED
enum { YYPUSH_MORE = 4 };
#endif

typedef struct yypstate yypstate;

int yypush_parse (yypstate *ps, int pushed_char, YYSTYPE const *pushed_val, YYLTYPE *pushed_loc, ParserContext* context);

yypstate * yypstate_new (void);
void yypstate_delete (yypstate *ps);
/* "%code provides" blocks.  */
#line 205 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 479 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */


/* Unqualified %code blocks.  */
#line 39 "chapel.ypp"

  #include <string>
  int         captureTokens;
  std::string captureString;
  bool        parsingPrivate=false;
#line 213 "chapel.ypp"

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

#line 556 "bison-chapel.cpp"

#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   19686

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  178
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  160
/* YYNRULES -- Number of rules.  */
#define YYNRULES  685
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1226

#define YYUNDEFTOK  2
#define YYMAXUTOK   432


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
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
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   509,   509,   514,   515,   521,   522,   527,   528,   533,
     534,   535,   536,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,   561,   562,   563,
     564,   565,   566,   570,   583,   588,   593,   601,   602,   603,
     607,   608,   612,   613,   614,   619,   618,   639,   640,   641,
     646,   647,   652,   657,   662,   666,   675,   680,   685,   690,
     694,   698,   706,   711,   715,   720,   724,   725,   726,   730,
     731,   732,   733,   734,   735,   736,   740,   745,   746,   747,
     751,   752,   756,   760,   762,   764,   766,   768,   770,   777,
     778,   782,   783,   784,   785,   786,   787,   790,   791,   792,
     793,   794,   795,   807,   808,   819,   820,   821,   822,   823,
     824,   825,   826,   827,   828,   829,   830,   831,   832,   833,
     834,   835,   836,   837,   841,   842,   843,   844,   845,   846,
     847,   848,   849,   850,   851,   852,   859,   860,   861,   862,
     866,   867,   871,   872,   876,   877,   878,   888,   888,   893,
     894,   895,   896,   897,   898,   899,   903,   904,   905,   906,
     911,   910,   926,   925,   942,   941,   957,   956,   972,   976,
     981,   989,  1000,  1007,  1008,  1009,  1010,  1011,  1012,  1013,
    1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,
    1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,  1032,  1033,
    1039,  1045,  1051,  1057,  1065,  1073,  1077,  1084,  1088,  1089,
    1090,  1091,  1092,  1094,  1096,  1098,  1103,  1105,  1113,  1114,
    1119,  1124,  1125,  1126,  1127,  1128,  1129,  1130,  1131,  1132,
    1133,  1134,  1135,  1136,  1143,  1144,  1145,  1146,  1155,  1156,
    1160,  1162,  1165,  1171,  1173,  1176,  1182,  1185,  1186,  1187,
    1188,  1189,  1190,  1194,  1195,  1199,  1200,  1201,  1205,  1206,
    1210,  1213,  1215,  1220,  1221,  1225,  1227,  1229,  1236,  1246,
    1260,  1265,  1270,  1278,  1279,  1284,  1285,  1287,  1292,  1308,
    1315,  1324,  1332,  1336,  1343,  1344,  1349,  1354,  1348,  1381,
    1384,  1388,  1396,  1406,  1395,  1438,  1442,  1447,  1451,  1456,
    1463,  1464,  1468,  1469,  1470,  1471,  1472,  1473,  1474,  1475,
    1476,  1477,  1478,  1479,  1480,  1481,  1482,  1483,  1484,  1485,
    1486,  1487,  1488,  1489,  1490,  1491,  1492,  1493,  1494,  1498,
    1499,  1500,  1501,  1502,  1503,  1504,  1505,  1506,  1507,  1508,
    1509,  1513,  1514,  1518,  1522,  1523,  1524,  1528,  1530,  1532,
    1534,  1536,  1538,  1543,  1544,  1548,  1549,  1550,  1551,  1552,
    1553,  1554,  1555,  1556,  1560,  1561,  1562,  1563,  1564,  1565,
    1569,  1570,  1574,  1575,  1576,  1577,  1578,  1579,  1583,  1584,
    1587,  1588,  1592,  1593,  1597,  1601,  1602,  1603,  1611,  1612,
    1614,  1616,  1618,  1623,  1625,  1630,  1631,  1632,  1633,  1634,
    1635,  1636,  1640,  1642,  1647,  1649,  1651,  1656,  1669,  1686,
    1687,  1689,  1694,  1695,  1696,  1697,  1698,  1702,  1708,  1716,
    1717,  1725,  1727,  1732,  1734,  1736,  1741,  1743,  1745,  1752,
    1753,  1754,  1759,  1761,  1763,  1767,  1771,  1773,  1777,  1785,
    1786,  1787,  1788,  1789,  1794,  1795,  1796,  1797,  1798,  1818,
    1822,  1826,  1834,  1841,  1842,  1843,  1847,  1849,  1855,  1857,
    1859,  1864,  1865,  1866,  1867,  1868,  1874,  1875,  1876,  1877,
    1881,  1882,  1886,  1887,  1888,  1892,  1893,  1897,  1898,  1902,
    1903,  1907,  1908,  1909,  1910,  1914,  1915,  1926,  1928,  1930,
    1936,  1937,  1938,  1939,  1940,  1941,  1943,  1945,  1947,  1949,
    1951,  1953,  1956,  1958,  1960,  1962,  1964,  1966,  1968,  1970,
    1973,  1975,  1980,  1982,  1984,  1986,  1988,  1990,  1992,  1994,
    1996,  1998,  2000,  2002,  2004,  2011,  2017,  2023,  2029,  2038,
    2048,  2056,  2057,  2058,  2059,  2060,  2061,  2062,  2063,  2068,
    2069,  2073,  2077,  2078,  2082,  2086,  2087,  2091,  2095,  2099,
    2106,  2107,  2108,  2109,  2110,  2111,  2115,  2116,  2121,  2123,
    2127,  2131,  2135,  2143,  2148,  2154,  2160,  2167,  2177,  2185,
    2186,  2187,  2188,  2189,  2190,  2191,  2192,  2193,  2194,  2196,
    2198,  2200,  2215,  2217,  2219,  2221,  2226,  2227,  2231,  2232,
    2233,  2237,  2238,  2239,  2240,  2249,  2250,  2251,  2252,  2253,
    2257,  2258,  2259,  2263,  2264,  2265,  2266,  2267,  2275,  2276,
    2277,  2278,  2282,  2283,  2287,  2288,  2292,  2293,  2294,  2295,
    2296,  2297,  2298,  2299,  2301,  2303,  2304,  2305,  2309,  2317,
    2318,  2322,  2323,  2324,  2325,  2326,  2327,  2328,  2329,  2330,
    2331,  2332,  2333,  2334,  2335,  2336,  2337,  2338,  2339,  2340,
    2341,  2342,  2343,  2344,  2349,  2350,  2351,  2352,  2353,  2354,
    2355,  2359,  2360,  2361,  2362,  2366,  2367,  2368,  2369,  2374,
    2375,  2376,  2377,  2378,  2379,  2380
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
  "TEXCEPT", "TEXPORT", "TEXTERN", "TFALSE", "TFOR", "TFORALL", "TFOREACH",
  "TFORWARDING", "TIF", "TIMAG", "TIMPORT", "TIN", "TINCLUDE", "TINDEX",
  "TINLINE", "TINOUT", "TINT", "TITER", "TINITEQUALS", "TIMPLEMENTS",
  "TINTERFACE", "TLABEL", "TLAMBDA", "TLET", "TLIFETIME", "TLOCAL",
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
  "TRP", "TLSBR", "TRSBR", "TNOELSE", "TDOTDOTOPENHIGH", "TUPLUS",
  "TUMINUS", "TLNOT", "$accept", "program", "toplevel_stmt_ls",
  "toplevel_stmt", "pragma_ls", "stmt", "module_decl_start",
  "module_decl_stmt", "access_control", "opt_prototype",
  "include_access_control", "include_module_stmt", "$@1", "block_stmt",
  "stmt_ls", "renames_ls", "use_renames_ls", "opt_only_ls", "except_ls",
  "use_access_control", "use_stmt", "import_stmt", "import_expr",
  "import_ls", "require_stmt", "assignment_stmt", "opt_label_ident",
  "ident_fn_def", "ident_def", "ident_use", "internal_type_ident_def",
  "scalar_type", "reserved_type_ident_use", "do_stmt", "return_stmt",
  "class_level_stmt", "@2", "private_decl", "forwarding_stmt",
  "extern_export_decl_stmt", "$@3", "$@4", "$@5", "$@6",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "interface_stmt", "ifc_formal_ls", "ifc_formal", "implements_type_ident",
  "implements_type_error_ident", "implements_stmt", "ifc_constraint",
  "defer_stmt", "try_stmt", "catch_stmt_ls", "catch_stmt", "catch_expr",
  "throw_stmt", "select_stmt", "when_stmt_ls", "when_stmt",
  "class_decl_stmt", "class_tag", "opt_inherit", "class_level_stmt_ls",
  "enum_decl_stmt", "enum_header", "enum_ls", "enum_item",
  "lambda_decl_expr", "$@7", "$@8", "linkage_spec", "fn_decl_stmt", "$@9",
  "$@10", "fn_decl_stmt_inner", "fn_decl_receiver_expr", "fn_ident",
  "assignop_ident", "opt_formal_ls", "req_formal_ls", "formal_ls",
  "formal", "opt_intent_tag", "required_intent_tag", "opt_this_intent_tag",
  "proc_or_iter", "opt_ret_tag", "opt_throws_error",
  "opt_function_body_stmt", "function_body_stmt", "query_expr",
  "var_arg_expr", "opt_lifetime_where", "lifetime_components_expr",
  "lifetime_expr", "lifetime_ident", "type_alias_decl_stmt",
  "type_alias_decl_stmt_inner", "opt_init_type", "var_decl_type",
  "var_decl_stmt", "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
  "tuple_var_decl_component", "tuple_var_decl_stmt_inner_ls",
  "opt_init_expr", "ret_array_type", "opt_ret_type", "opt_type",
  "array_type", "opt_formal_array_elt_type", "formal_array_type",
  "opt_formal_type", "expr_ls", "simple_expr_ls", "tuple_component",
  "tuple_expr_ls", "opt_actual_ls", "actual_ls", "actual_expr",
  "ident_expr", "type_level_expr", "sub_type_level_expr", "for_expr",
  "cond_expr", "nil_expr", "stmt_level_expr", "opt_task_intent_ls",
  "task_intent_clause", "task_intent_ls", "forall_intent_clause",
  "forall_intent_ls", "intent_expr", "shadow_var_prefix", "io_expr",
  "new_maybe_decorated", "new_expr", "let_expr", "expr", "opt_expr",
  "opt_try_expr", "lhs_expr", "call_base_expr", "call_expr", "dot_expr",
  "parenthesized_expr", "bool_literal", "str_bytes_literal",
  "literal_expr", "assoc_expr_ls", "binary_op_expr", "unary_op_expr",
  "reduce_expr", "scan_expr", "reduce_scan_op_expr", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
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
     425,   426,   427,   428,   429,   430,   431,   432
};
# endif

#define YYPACT_NINF (-1068)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-636)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1068,   108,  3528, -1068,    66,   135, -1068, -1068, -1068, -1068,
   -1068, -1068,  4904,   149,   292,   197, 14561,   253, 19255,   149,
   11240,   270,   359,   251,   292,  4904, 11240,  4904,   214, 19339,
   11410,  8334,   336,  8504,  9532,  9532,  6966, 11240,   406, -1068,
     295, -1068,   433, 19372, 19372, 19372, -1068, 14915,  9702,   438,
   11240,   229, -1068,   444,   469, 11240, -1068, 14561, -1068, 11240,
     550,   308,   376,  2174,   515, 19456, -1068,  9874,  7822, 11240,
    9702, 14561, 11240,   476,   530,   421,  4904,   541, 11240,   542,
   11580, 11580, 19372,   556, -1068, 14561, -1068,   557, 11240, 11240,
   -1068, 11240, -1068, 11240, -1068, -1068, 14088, 11240, -1068, 11240,
   -1068, -1068, -1068,  3872,  7138,  8676, 11240, -1068,  4732, -1068,
     448, -1068,   553, -1068, -1068,    60, -1068, -1068, -1068, -1068,
   -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068,
   -1068,   568, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068,
   19372, -1068, 19372,   392,   196, -1068, -1068, 14915, -1068,   477,
   -1068,   481, -1068, -1068,   482,   483,   490, 11240,   484,   485,
   19012,  2513,   236,   489,   494, -1068, -1068,   375, -1068, -1068,
   -1068, -1068, -1068,   449, -1068, -1068, 19012,   495,  4904, -1068,
   -1068,   503, 11240, -1068, -1068, 11240, 11240, 11240, 19372, -1068,
   11240,  9874,  9874,   597,   398, -1068, -1068, -1068, -1068,   -20,
     412, -1068, -1068,   499, 16373, 19372, 14915, -1068,   505, -1068,
     -40, 19012,   552,  7994,   585, 19489, 19012,   456,   586, -1068,
   19573, 19372,   456, 19372,   509,     2, 15983,    -2, 15827,    -2,
   15903,   267, -1068, 16059,  3328,   184,  7994, -1068, -1068, -1068,
   -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068,
   -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068,
   -1068,   510, -1068,   419,  4904,   511,  3033,   117,    88, -1068,
    4904, -1068, -1068, 16453,   751, -1068,   513,   514, -1068, 16453,
     -20,   751, -1068,  7994,  2363, -1068, -1068, 10044, -1068, -1068,
   -1068, -1068,   -14, 19012, 11240, 11240, -1068, 19012,   522, 16921,
   -1068, 16453,   -20, 19012,   516,  7994, -1068, 19012, 16997, -1068,
   -1068, 17037,  2764, -1068, -1068, 17077,   565,   526,   -20, 16453,
   17193,   113,   113,  1037,   751,   751,   166, -1068, -1068,  4044,
     163, -1068, 11240, -1068,   -29,   126, -1068,   -21,    25, 17242,
     -42,  1037,   664, -1068,  4216, -1068,   625, 11240, 11240, 19372,
     549,   527, -1068, -1068, -1068,   162,   420, -1068, 11240,   554,
   11240, 11240, 11240,  9532,  9532, 11240,   458, 11240, 11240, 11240,
   11240, 11240,   223, 14088, 11240, 11240, 11240, 11240, 11240, 11240,
   11240, 11240, 11240, 11240, 11240, 11240, 11240, 11240, 11240, 11240,
     628, -1068, -1068, -1068, -1068, -1068,  8846,  8846, -1068, -1068,
   -1068, -1068,  8846, -1068, -1068,  8846,  8846,  7994,  7994,  9532,
    9532,  7652, -1068, -1068, 16529, 16609, 17282,   524,    33, 19372,
    4388, -1068,  9532,     2,   535,   282, -1068, 11240, -1068, 11240,
     579, -1068,   533,   561, -1068, -1068, -1068, 19372, -1068, 14915,
   -1068, 19372,   543, -1068, 14915,   665,  9874, -1068,  5076,  9532,
   -1068,   539, -1068,     2,  5248,  9532, -1068,     2, -1068,     2,
    9532, -1068,     2, 11750, 11240, -1068,  4904, -1068,   679, 11240,
   -1068, -1068,   553,   546,  7994, 19372, -1068, -1068,   422, -1068,
   -1068,  3033, -1068,   575,   551, -1068, 11920,   598, 11240, 14915,
   -1068, -1068, 11240, 11240, -1068,   560, -1068,  9874, -1068, 19012,
   19012, -1068,    38, -1068,  7994,   562, -1068,   701, -1068,   701,
   -1068, 12090,   588, -1068, -1068, -1068, -1068, -1068, -1068,  9018,
   -1068, 17358,  7310, -1068,  7482, -1068,  4904,   564,  9532,  9190,
    3700,   567, 11240, 10214, -1068, -1068,   259, -1068,  4560, 19372,
   -1068,   316, 17398,   317, 16141,    92,  9874,   566, 19222,   254,
   -1068, 17439, 15943, 15943,   377, -1068,   377, -1068,   377,  1844,
     600,   661,  1076,   -20,   113, -1068,   572, -1068, -1068, -1068,
   -1068, -1068,  1037, 16569,   377,  1586,  1586, 15943,  1586,  1586,
    1449,   113, 16569, 17677,  1449,   751,   751,   113,  1037,   581,
     584,   592,   593,   596,   599,   578,   587, -1068,   377, -1068,
     377,    51, -1068, -1068, -1068,   129, -1068,  2281, 19057,   455,
   12260,  9532, 12430,  9532, 11240,  7994,  9532, 14831,   591,   149,
   17547, -1068, -1068, -1068, 19012, 17588,  7994, -1068,  7994, 19372,
     549,   327, 19372,   549, -1068,   351, 11240,   133, 11240, 19012,
      17, 16685,  7652, -1068, 11240, 19012,    10, 16217, -1068, -1068,
      -2, 16297, -1068,   604,   607,   608, 17628,   607,   611,   700,
   17777,  4904,  3026,   698, -1068,   141, -1068,   143, -1068,    52,
   -1068, -1068, -1068, -1068, -1068, -1068,   785,   157, -1068, 15013,
   -1068,   399,   614,  3033,   117,    45,    49, 11240, 11240,  6796,
   -1068, -1068,   341,  8164, -1068, 19012, -1068, 17817, 17857, -1068,
   -1068, 19012,   606,    -1,   617, -1068,  3239, -1068, -1068,   361,
   19372, -1068, -1068, -1068, -1068, -1068, -1068, -1068,  4904,   120,
   16765, -1068, -1068, 19012,  4904, 19012, -1068, 17933, -1068, -1068,
   -1068, 11240, -1068,    47,    50, 11240, -1068, 10384, 11750, 11240,
   -1068,  7994,   634,  1327,   620,   659,   176, -1068,   702, -1068,
   -1068, -1068, -1068, 13923,   621, -1068, -1068, -1068, -1068, -1068,
   -1068, -1068, -1068, -1068, -1068,  7652, -1068, -1068, -1068, -1068,
   -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068,    15,  9532,
    9532, 11240,   758, 18013, 11240,   760, 18089,   167,   636, 18129,
    7994,     2,     2, -1068, -1068, -1068, -1068,   549,   633, -1068,
     549,   642, -1068, 16453, -1068, 15195,  5420, -1068,  5592, -1068,
     175, -1068, 15271,  5764, -1068,     2,  5936, -1068,     2, -1068,
       2, -1068,     2, 11240, -1068, 11240, -1068,  4904, 11240, -1068,
    4904,   780, 19372,   653, 19372,   499, -1068, -1068, 19372,   899,
   -1068,  3033,   675,   732, -1068, -1068, -1068,   123, -1068, -1068,
     598,   649,    39, -1068, -1068,   654,   658, -1068,  6108,  9874,
   -1068, -1068, -1068, 19372, -1068,   686,   499, -1068, -1068,  6280,
     662,  6452,   663, -1068, 11240, -1068, -1068, 11240, 18169,   145,
   16841,   668,   671,   238,   670,  1447, -1068, 11240, 19372, -1068,
   -1068,   399,   673,   310, -1068,   699, -1068,   704,   705,   706,
     719,   722, -1068,   723,   708,   724,   728,   729,   403,   709,
     731,   733, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068,
   -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068,
   -1068, -1068, -1068, -1068, 11240, -1068,   740,   742,   735,   673,
     673, -1068, -1068, -1068,   598,   246,   249, 18245, 12600, 12770,
   18285, 12940, 13110, -1068, 13280, 13450,   268, -1068, -1068,   715,
   -1068,   716, -1068, -1068,  4904, 11240, 19012, 11240, 19012,  7652,
   -1068,  4904, 11240, 19012, -1068, 11240, 19012, -1068, -1068, -1068,
   18326, 19012, -1068, 19012,   850,  4904,   730, -1068, -1068, -1068,
     675, -1068,   720, 10556,   284, -1068,   182, -1068, -1068,  9532,
   14696,  7994,  7994,  4904, -1068,    46,   727, 11240, -1068, 11240,
   -1068, 19012,  4904, 11240, -1068, 19012,  4904, 19012,   271, 10726,
   11750, 11240, 11750, 11240, -1068, -1068,   739, -1068, -1068,  2363,
   -1068,  1263, -1068, 19012, -1068,   106,   422, -1068, 18434, -1068,
   15112, -1068, -1068, -1068, 11240, 11240, 11240, 11240, 11240, 11240,
   11240, 11240, -1068, -1068,  1800, -1068,  2070, 17777, 15353, 15429,
   -1068, 17777, 15511, 15587, 11240,  4904, -1068, -1068,   284,   675,
    9362, -1068, -1068, -1068,   131,  9874, -1068, -1068,   151, 11240,
     -41, 18475, -1068,   491,   734,   741,   552, -1068,   499, 19012,
   15669, -1068, 15745, -1068, -1068, -1068, 19012,   747,   748,   750,
     752, -1068, -1068, -1068, 13620,   783,   280, -1068,   749,   757,
     673,   673, 18515, 18591, 18631, 18707, 18747, 18823,  2545, -1068,
    2712, -1068,  4904,  4904,  4904,  4904, 19012, -1068, -1068, -1068,
     284, 10898,    43, -1068, 19012, -1068,   102, -1068,   148, -1068,
     405, 18863, -1068, -1068, -1068, 13450,   754,   756, -1068,  4904,
    4904, -1068, -1068, -1068, -1068,  6624, -1068, -1068,   547, -1068,
     182, -1068, -1068, -1068, 11240, 11240, 11240, 11240, 11240, 11240,
   -1068, -1068, 17777, 17777, 17777, 17777, -1068, -1068, -1068, -1068,
   -1068,   298,  9532, 14257, -1068, 11240,   151,   102,   102,   102,
     102,   102,   102,   151,   613, -1068, -1068, 17777, 17777,   746,
   13790,    55,    74, 18908, -1068, -1068, 19012, -1068, -1068, -1068,
   -1068, -1068, -1068, -1068,   759, -1068, -1068,   548, 14426, -1068,
   -1068, -1068, 11070, -1068,   609, -1068
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   113,   628,   629,   630,   624,
     625,   631,     0,   549,    99,   134,   518,   141,   520,   549,
       0,   140,     0,   423,    99,     0,     0,     0,   248,   135,
     596,   596,   622,     0,     0,     0,     0,     0,   139,    55,
     249,   300,   136,     0,     0,     0,   296,     0,     0,   143,
       0,   568,   540,   632,   144,     0,   301,   512,   422,     0,
       0,     0,   157,    48,   138,   521,   424,     0,     0,     0,
       0,   516,     0,     0,   142,     0,     0,   114,     0,   623,
       0,     0,     0,   137,   282,   514,   426,   145,     0,     0,
     681,     0,   683,     0,   684,   685,   595,     0,   682,   679,
     499,   154,   680,     0,     0,     0,     0,     4,     0,     5,
       0,     9,    50,    10,    11,     0,    12,    13,    14,    16,
     495,   496,    26,    15,   155,   164,   165,    17,    21,    18,
      20,     0,   243,    19,   587,    23,    24,    25,    22,   163,
       0,   161,     0,   584,     0,   159,   162,     0,   160,   601,
     580,   497,   581,   502,   500,     0,     0,     0,   585,   586,
       0,   501,     0,   602,   603,   604,   626,   627,   579,   504,
     503,   582,   583,     0,    42,    28,   510,     0,     0,   550,
     100,     0,     0,   520,   135,     0,     0,     0,     0,   521,
       0,     0,     0,     0,   584,   601,   500,   585,   586,   519,
     501,   602,   603,     0,   549,     0,     0,   425,     0,   256,
       0,   480,     0,   487,   624,   521,   597,   299,   624,   182,
     521,     0,   299,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   169,     0,     0,    52,   487,   107,   115,   127,
     121,   120,   129,   110,   119,   130,   116,   131,   108,   132,
     125,   118,   126,   124,   122,   123,   109,   111,   117,   128,
     133,     0,   112,     0,     0,     0,     0,     0,     0,   429,
       0,   151,    36,     0,   666,   572,   569,   570,   571,     0,
     513,   667,     7,   487,   299,   280,   290,   596,   281,   156,
     394,   477,     0,   476,     0,     0,   152,   600,     0,     0,
      39,     0,   517,   505,     0,   487,    40,   511,     0,   263,
     259,     0,   501,   263,   260,     0,   419,     0,   515,     0,
       0,   668,   670,   593,   665,   664,     0,    57,    60,     0,
       0,   482,     0,   484,     0,     0,   483,     0,     0,   476,
       0,   594,     0,     6,     0,    51,     0,     0,     0,     0,
     283,     0,   381,   380,   302,     0,   498,    27,     0,   573,
       0,     0,     0,     0,     0,     0,   669,     0,     0,     0,
       0,     0,     0,   592,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     339,   346,   347,   348,   343,   345,     0,     0,   341,   344,
     342,   340,     0,   350,   349,     0,     0,   487,   487,     0,
       0,     0,    29,    30,     0,     0,     0,     0,     0,     0,
       0,    31,     0,     0,     0,     0,    32,     0,    33,     0,
     495,   493,     0,   488,   489,   494,   176,     0,   179,     0,
     172,     0,     0,   178,     0,     0,     0,   191,     0,     0,
     190,     0,   199,     0,     0,     0,   197,     0,   207,     0,
       0,   205,     0,     0,    72,   166,     0,   339,   219,     0,
      54,    53,    50,     0,     0,     0,   227,    34,   363,   297,
     433,     0,   434,   436,     0,   458,     0,   439,     0,     0,
     150,    35,     0,     0,    37,     0,   158,     0,    92,   598,
     599,   153,     0,    38,   487,     0,   270,   261,   257,   262,
     258,     0,   417,   414,   184,    41,    59,    58,    61,     0,
     633,     0,     0,   618,     0,   620,     0,     0,     0,     0,
       0,     0,     0,     0,   637,     8,     0,    44,     0,     0,
      90,     0,    87,     0,    66,   254,     0,     0,     0,   374,
     428,   548,   661,   660,   663,   672,   671,   676,   675,   657,
     654,   655,   656,   589,   644,   113,     0,   615,   616,   114,
     614,   613,   590,   648,   659,   653,   651,   662,   652,   650,
     642,   647,   649,   658,   641,   645,   646,   643,   591,     0,
       0,     0,     0,     0,     0,     0,     0,   674,   673,   678,
     677,   560,   561,   563,   565,     0,   552,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   635,   254,   549,
     549,   187,   415,   427,   481,     0,     0,   507,     0,     0,
     283,     0,     0,   283,   416,     0,     0,     0,     0,   524,
       0,     0,     0,   200,     0,   530,     0,     0,   198,   208,
       0,     0,   206,   680,    75,     0,    62,    73,     0,   218,
       0,     0,     0,     0,   506,     0,   230,     0,   228,   368,
     365,   366,   367,   371,   372,   373,   363,     0,   355,     0,
     364,   382,     0,   437,     0,   148,   149,   147,   146,     0,
     457,   456,   580,     0,   431,   578,   430,     0,     0,   612,
     479,   478,     0,     0,     0,   508,     0,   264,   421,   580,
       0,   634,   588,   619,   485,   621,   486,   215,     0,     0,
       0,   636,   213,   534,     0,   639,   638,     0,    46,    45,
      43,     0,    86,     0,     0,     0,    79,     0,     0,    72,
     251,     0,   284,     0,     0,   294,     0,   291,   378,   375,
     376,   379,   303,     0,     0,    98,    96,    97,    95,    94,
      93,   610,   611,   562,   564,     0,   551,   134,   141,   140,
     139,   136,   143,   144,   138,   142,   137,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   183,   491,   492,   490,   283,     0,   181,
     283,     0,   180,     0,   217,     0,     0,   189,     0,   188,
       0,   555,     0,     0,   195,     0,     0,   193,     0,   203,
       0,   201,     0,     0,   167,     0,   168,     0,     0,   221,
       0,   223,     0,   253,     0,     0,   369,   370,     0,   363,
     353,     0,   471,   383,   386,   385,   387,     0,   435,   438,
     439,     0,     0,   440,   441,     0,     0,   272,     0,     0,
     271,   274,   509,     0,   265,   268,     0,   418,   216,     0,
       0,     0,     0,   214,     0,    91,    88,     0,    69,    68,
      67,     0,     0,     0,     0,   299,   289,     0,   292,   288,
     377,   382,   351,   101,   334,   115,   332,   121,   120,   104,
     119,   116,   337,   131,   102,   132,   118,   122,   103,   105,
     117,   133,   331,   313,   316,   314,   315,   326,   317,   330,
     322,   320,   333,   336,   321,   319,   324,   329,   318,   323,
     327,   328,   325,   335,     0,   312,     0,   106,     0,   351,
     351,   310,   617,   553,   439,   601,   601,     0,     0,     0,
       0,     0,     0,   253,     0,     0,     0,   186,   185,     0,
     285,     0,   285,   192,     0,     0,   523,     0,   522,     0,
     554,     0,     0,   529,   196,     0,   528,   194,   204,   202,
      64,    63,   220,   539,   222,     0,     0,   250,   229,   226,
     471,   356,     0,     0,   439,   384,   398,   432,   462,     0,
     635,   487,   487,     0,   276,     0,     0,     0,   266,     0,
     211,   536,     0,     0,   209,   535,     0,   640,     0,     0,
       0,    72,     0,    72,    80,    83,   255,   279,   157,   299,
     278,   299,   286,   295,   293,     0,   363,   309,     0,   338,
       0,   305,   306,   557,     0,     0,     0,     0,     0,     0,
       0,     0,   255,   285,   299,   285,   299,   527,     0,     0,
     556,   533,     0,     0,     0,     0,   225,    56,   439,   471,
       0,   474,   473,   475,   580,   395,   359,   357,     0,     0,
       0,     0,   460,   580,     0,     0,   277,   275,     0,   269,
       0,   212,     0,   210,    89,    71,    70,     0,     0,     0,
       0,   252,   287,   453,     0,   388,     0,   311,   101,   103,
     351,   351,     0,     0,     0,     0,     0,     0,   299,   175,
     299,   171,     0,     0,     0,     0,    65,   224,   360,   358,
     439,   463,     0,   397,   396,   412,     0,   413,   400,   403,
       0,   399,   392,   393,   298,     0,   574,   575,   267,     0,
       0,    82,    85,    81,    84,     0,   452,   451,   580,   389,
     398,   352,   307,   308,     0,     0,     0,     0,     0,     0,
     177,   173,   526,   525,   532,   531,   362,   361,   465,   466,
     468,   580,     0,   635,   411,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   580,   576,   577,   538,   537,     0,
     443,     0,     0,     0,   467,   469,   402,   404,   405,   408,
     409,   410,   406,   407,   401,   448,   446,   580,   635,   390,
     304,   391,   463,   447,   580,   470
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1068, -1068, -1068,     4,  -461,  2431, -1068, -1068, -1068,   441,
   -1068, -1068, -1068,   626,   576,  -459, -1068,  -707,  -705, -1068,
   -1068, -1068,   188, -1068, -1068,   497,   897, -1068,  2074,  -162,
    -738, -1068,  -911,  1001, -1015,  -831, -1068,   -62, -1068, -1068,
   -1068, -1068, -1068, -1068, -1068, -1068,   127, -1068, -1068, -1068,
      89,  1207, -1068, -1068, -1068, -1068, -1068,   612, -1068,    61,
   -1068, -1068, -1068, -1068, -1068, -1068,  -593,  -653, -1068, -1068,
   -1068,    40,  1364, -1068, -1068, -1068,   250, -1068, -1068, -1068,
   -1068,  -111,  -159,  -877, -1068,  -106,    93,   255, -1068, -1068,
   -1068,    42, -1068, -1068,  -268,    28, -1007,  -225,  -255,  -245,
    -570, -1068,  -177, -1068,    13,   908,  -118,   459, -1068,  -454,
    -798,  -703, -1068,  -638,  -504, -1067, -1034,  -881,   -64, -1068,
     116, -1068,  -216,  -443,   319,   795,  -352, -1068, -1068, -1068,
    1441, -1068,   -10, -1068, -1068,  -218, -1068,  -629, -1068, -1068,
   -1068,  1861,  1938,   -12,   919,     7,  1039, -1068,  2263,  2391,
   -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068,  -385
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   328,   108,   659,   110,   111,   112,   346,
     472,   113,   235,   114,   329,   654,   543,   658,   655,   115,
     116,   117,   540,   541,   118,   119,   181,   935,   267,   120,
     262,   121,   690,   272,   122,   123,   284,   124,   125,   126,
     441,   632,   437,   629,   127,   128,   788,   129,   130,   667,
     668,   193,   132,   133,   134,   135,   136,   507,   707,   866,
     137,   138,   703,   861,   139,   140,   547,   885,   141,   142,
     746,   747,   194,   265,   681,   144,   145,   549,   891,   752,
     938,   939,   469,  1037,   479,   677,   678,   679,   680,   753,
     354,   847,  1160,  1220,  1144,   431,  1076,  1080,  1138,  1139,
    1140,   146,   317,   512,   147,   148,   268,   269,   483,   484,
     694,  1157,  1105,   487,   691,  1179,  1073,   994,   330,   210,
     334,   335,   432,   433,   434,   195,   150,   151,   152,   153,
     196,   155,   178,   179,   605,   453,   810,   606,   607,   156,
     157,   197,   198,   160,   222,   435,   200,   162,   201,   202,
     165,   166,   167,   168,   340,   169,   170,   171,   172,   173
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     176,   289,   406,   292,   199,   657,   107,   708,   204,   203,
     457,   459,   462,   811,   211,   937,   485,   676,   216,   216,
     473,   226,   228,   230,   233,   234,   609,   682,   424,   355,
     270,   665,   882,   881,   270,   206,   273,   798,   274,   702,
     801,   338,   813,   279,   442,   280,   850,   281,    68,   806,
     565,   430,   997,   565,  1032,   293,   297,   299,   301,   302,
     303,  1128,  1041,  1042,   307,  1143,   308,   495,   311,   315,
     566,   858,   528,   318,   430,   298,   319,   320,   270,   321,
     616,   322,  1071,   567,   323,   324,   999,   325,   425,   505,
    1182,   293,   297,   339,   341,   291,   451,  1180,   763,   836,
    -248,   533,   616,   427,  -249,  1135,   347,  1103,     3,  1068,
    -273,   336,   568,   859,   522,   451,  1204,   366,   485,   527,
     531,   430,   428,  1176,   485,   372,   420,   451,   418,   497,
     534,   291,   333,   291,   692,   488,   943,  -454,   764,   837,
     944,   523,   451,   430,   362,   359,  1043,   569,   498,  1205,
     569,   526,  -273,   406,  1135,  1225,   570,   486,  -454,   709,
     225,   227,   229,    68,  -454,   420,  -449,   860,   529,   420,
     176,   348,   787,   414,   415,   416,   529,   420,   307,   293,
     339,  1020,   529,  -454,   420,  -454,   529,  1143,  1130,   497,
    -231,   595,   596,  1156,   423,  -449,  1077,   530,   529,   363,
    1102,   297,  1137,   364,   959,   617,  -273,   961,  -449,   996,
     571,  1000,  -454,   420,   213,  1183,  1021,   877,   236,   291,
     291,  -449,   496,  1032,   297,  1032,   565,  1218,   174,   849,
     439,   489,   337,  1162,  1163,   444,  1219,   451,  -454,  -454,
    1136,   420,  1078,   566,   206,   430,   430,   275,  1104,   352,
     366,  1137,  -232,  -472,   740,   370,   567,   609,   372,   486,
    -454,   741,   375,  1185,   470,   486,   177,   471,  -449,   524,
    1129,   297,   765,  -449,  -472,   216,   497,   353,  -472,  -454,
     657,   748,   499,   500,   628,   568,   834,  1032,   704,  1032,
    -454,  1186,   869,   297,    23,   180,   525,  1079,   883,   766,
     839,  -472,   937,   804,   276,   489,   519,  1054,  -238,  1056,
     628,   833,   430,   835,  1098,  1097,  1100,  1099,   969,   888,
     521,   631,   277,   569,   550,  -237,   635,   840,   174,   570,
     749,   520,  1177,   518,   516,   542,   544,   953,   207,   278,
    1060,   750,   430,    58,   889,   970,   551,   956,   552,   553,
     554,   556,   558,   559,    66,   560,   561,   562,   563,   564,
     751,   572,   573,   574,   575,   576,   577,   578,   579,   580,
     581,   582,   583,   584,   585,   586,   587,   588,   676,    86,
     609,   628,   637,   213,   297,   297,    23,   992,  -455,  -559,
     297,  -246,  -558,   297,   297,   297,   297,   598,   600,   608,
    1118,  -455,  1120,   590,   591,   407,   693,   408,  1026,   592,
     620,   628,   593,   594,   823,   624,  -559,   625,  1018,  -558,
    -464,   174,   -78,   839,  1031,   489,   843,   728,   815,   818,
    -455,  1075,   820,   822,   293,    58,   639,   641,  1052,  1094,
     -49,  -464,   645,   647,   623,  -464,    66,  -107,   651,   669,
    1161,   656,   656,   430,   660,  -113,  -455,   662,   -49,   731,
     735,  -236,   297,  -455,   236,   205,   430,   438,  -464,   670,
     489,    86,   443,   671,   291,   844,   695,   283,   732,   736,
     697,   698,   742,    23,  -455,   701,   845,   -78,  -233,   799,
     555,   557,   297,  -240,   489,   672,  1082,  1216,   673,  -244,
      60,   870,   872,  -455,  -420,   846,    41,   701,  -455,   674,
     297,  -455,   297,   802,   366,  1223,   720,   701,   723,   370,
     725,   727,   372,  -420,  -241,   700,   375,  1187,   675,   336,
      56,   336,    58,   867,   293,   409,   597,   599,  -459,   410,
    -109,   780,   518,    66,  -609,   410,  -609,   700,  -114,   619,
     333,  -459,   333,  1188,  -545,  1189,  1190,   700,   282,  1191,
    1192,  -608,   657,  -608,   657,  -354,  1184,  -608,    86,  -608,
    -235,   876,   571,   304,   291,   676,   640,   310,   314,   430,
    -459,  -605,   646,  -605,   609,  -239,   420,   650,   475,  -607,
     305,  -607,  -354,  1031,   940,  1031,  -245,  -247,   639,   783,
     645,   786,   660,   297,   789,   723,  -459,  -450,  -442,   791,
     792,  -234,  -242,  -459,   297,   344,   297,  1208,  1209,  1210,
    1211,  1212,  1213,   349,   803,   852,   805,  -606,   430,  -606,
     608,   362,   812,   795,  -459,   345,  -450,  -442,   714,  -542,
     716,  1074,   356,   358,  -541,   357,  -546,  -547,  1083,  -450,
    -442,  -544,   419,  -459,   794,   719,  -543,  1031,  -459,  1031,
    -461,  -459,  -450,  -442,   411,   413,   420,   426,   429,  -444,
     436,   440,   535,  -461,   271,   303,   307,   339,   446,   474,
     478,   297,   492,   493,   501,   504,   363,   511,   513,   539,
     364,   546,   362,   615,   548,   589,   271,   622,  -444,   372,
     854,   626,  -461,   627,   628,   634,   309,   313,   642,  -450,
    -442,  -444,   636,   661,  -450,  -442,   664,   291,   683,   542,
     693,   684,   706,   878,  -444,   880,   656,   656,  -461,   297,
     699,   710,   705,   743,   827,  -461,   718,   366,   782,   724,
     785,   754,   370,   755,   371,   372,   756,   363,   761,   375,
     823,   364,  1158,   608,   757,   758,  -461,   382,   759,   762,
     790,   760,   832,   386,   387,   388,   -74,   556,   598,   947,
     824,  -444,   950,   826,   857,  -461,  -444,   497,   297,  1181,
    -461,   887,   362,  -461,   848,  1084,  1085,   862,   886,   890,
     948,   942,   951,  1194,   966,  1005,   968,   149,   366,   367,
     960,   973,   369,   370,   976,   371,   372,   149,   954,   962,
     375,   980,   669,   981,   985,   987,   983,   993,   382,   995,
     149,   998,   149,  1001,   386,   387,   388,  1002,  1007,   421,
    1024,  1181,   670,  1025,  1012,  1016,   671,   363,  1027,   430,
     430,   364,  1036,  -110,  -134,  -108,  -111,   293,  1217,  -141,
    -140,   271,   271,   271,   271,   271,   271,  1011,   672,  1015,
     468,   673,  1017,   342,  -139,   656,  1224,  -136,  -143,  -144,
    1181,   149,   674,  -138,  -142,  1033,  -137,  1039,  -145,  -112,
    1040,  1111,  1053,  1055,  1065,  1159,  -107,   291,   366,   476,
    1069,   675,  1067,   370,  -109,   371,   372,  1088,   149,   271,
     375,  1101,  1186,   149,  1146,   271,   555,   597,   382,  1151,
    1152,  1147,  1153,   663,  1154,  1195,   388,  1196,  1215,   875,
     538,   208,  1038,   988,  1006,   509,   669,   271,  1034,  1110,
    1106,   838,   991,  1035,  1221,  1202,   966,   968,  1214,   973,
     976,  1207,  1011,  1015,   232,   271,   670,   796,   696,   217,
     671,     0,  1057,  1058,     0,  1059,     0,   608,     0,  1061,
    1062,     0,     0,  1063,     0,     0,     0,   289,     0,     0,
       0,     0,   672,   149,     0,   673,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,   674,  1081,   723,   297,
     297,     0,     0,     0,     0,  1089,     0,  1090,     0,     0,
       0,  1092,     0,     0,     0,   675,  1132,  1096,   656,   656,
     656,   656,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1072,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1057,  1112,  1113,  1061,  1114,  1115,  1116,  1117,
       0,   161,     0,     0,     0,     0,     0,     0,     0,   271,
       0,   161,  1126,     0,     0,     0,     0,     0,   339,   149,
       0,     0,     0,  1134,   161,   149,   161,  1141,   362,     0,
       0,   300,     0,     0,     0,     0,     0,     0,     0,   271,
       0,     0,     0,   271,     0,   271,     0,     0,   271,     0,
       0,  1201,     0,     0,     0,     0,     0,     0,   291,     0,
       0,     0,     0,  1133,     0,     0,     0,   362,     0,     0,
    1172,  1173,  1174,  1175,     0,   161,     0,     0,     0,   312,
     312,     0,     0,   363,   149,     0,     0,   364,     0,     0,
       0,     0,     0,  1015,     0,     0,     0,  1197,  1198,   149,
       0,     0,   161,   339,     0,     0,     0,   161,     0,     0,
       0,     0,  1172,  1173,  1174,  1175,  1197,  1198,     0,  1178,
       0,     0,   363,     0,     0,     0,   364,     0,     0,     0,
    1203,   723,     0,  1206,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   291,     0,     0,   375,     0,     0,     0,
       0,     0,     0,   381,   382,     0,     0,   385,     0,     0,
     386,   387,   388,     0,     0,     0,   723,     0,     0,   131,
    1015,  1178,     0,   366,   367,   149,     0,   161,   370,   131,
     371,   372,     0,     0,     0,   375,   447,   450,   452,   456,
     458,   461,   131,   382,   131,     0,     0,     0,     0,   386,
     387,   388,     0,   149,     0,     0,     0,     0,     0,   149,
    1178,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   149,     0,     0,     0,     0,   271,   271,     0,     0,
       0,     0,   271,   271,   491,     0,   271,   271,     0,     0,
     494,     0,     0,   131,     0,   285,     0,     0,   831,    22,
      23,     0,     0,     0,     0,     0,     0,     0,   286,     0,
      30,   287,   503,   161,     0,     0,    36,     0,     0,   161,
     131,     0,     0,    41,     0,   131,     0,     0,     0,     0,
     514,   149,     0,     0,     0,   149,     0,     0,   884,     0,
       0,     0,   864,   149,     0,     0,     0,    56,     0,    58,
       0,   342,     0,  1028,     0,     0,  1029,     0,   288,  -285,
      66,     0,     0,  -285,  -285,     0,     0,     0,     0,     0,
       0,     0,  -285,     0,  -285,  -285,   143,     0,   161,    82,
    -285,     0,    84,     0,     0,    86,   143,  -285,     0,     0,
    -285,     0,     0,   161,     0,   131,     0,     0,     0,   143,
       0,   143,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -285,   778,  -285,     0,  -285,     0,  -285,  -285,     0,
    -285,     0,  -285,     0,  -285,     0,     0,   271,   271,     0,
       0,     0,     0,     0,   621,   101,     0,     0,     0,   271,
       0,   468,     0,  -285,     0,     0,  -285,     0,   468,  -285,
     143,   271,     0,   154,   271,     0,   271,     0,   271,     0,
       0,     0,     0,   154,   643,     0,   149,     0,   648,   161,
     649,   989,     0,   652,     0,     0,   154,   143,   154,   285,
       0,   131,   143,    22,    23,     0,     0,   131,     0,     0,
     362,     0,   286,     0,    30,   287,     0,   161,     0,  -285,
      36,     0,  1008,   161,     0,  -285,     0,    41,     0,     0,
       0,     0,     0,     0,     0,   161,     0,     0,     0,     0,
       0,     0,     0,   149,     0,     0,     0,   154,     0,   149,
       0,    56,     0,    58,     0,    60,     0,  1028,     0,     0,
    1029,     0,   288,     0,    66,   363,   131,     0,     0,   364,
       0,     0,   143,     0,   154,     0,     0,     0,   941,   154,
       0,   131,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,   161,     0,     0,     0,   161,
       0,     0,     0,     0,   945,   946,     0,   161,     0,     0,
       0,     0,     0,     0,     0,     0,   366,   367,     0,   368,
     369,   370,     0,   371,   372,     0,     0,     0,   375,     0,
       0,   149,     0,   149,     0,     0,   382,     0,   149,   101,
       0,   149,   386,   387,   388,  1030,     0,   362,     0,   154,
       0,     0,   149,     0,     0,   149,     0,   131,   143,     0,
       0,   271,     0,     0,   143,     0,     0,     0,     0,     0,
       0,   807,   809,     0,     0,     0,     0,   814,   817,     0,
       0,   819,   821,   149,     0,   131,     0,     0,     0,     0,
       0,   131,     0,     0,   149,     0,   149,     0,     0,     0,
       0,     0,   363,   131,     0,     0,   364,     0,     0,     0,
       0,     0,     0,     0,   468,   468,     0,     0,   468,   468,
       0,     0,     0,   143,     0,     0,     0,     0,     0,     0,
     161,     0,     0,     0,     0,   154,  1142,     0,   143,     0,
       0,   154,     0,     0,  1148,     0,   468,     0,   468,     0,
       0,     0,     0,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   131,     0,   375,     0,   131,     0,     0,
       0,     0,   381,   382,     0,   131,   385,     0,     0,   386,
     387,   388,     0,     0,     0,     0,     0,   161,     0,   149,
     389,     0,     0,   161,     0,     0,   149,     0,     0,     0,
     154,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     149,     0,     0,     0,   143,   154,     0,     0,     0,     0,
       0,     0,   957,   958,     0,     0,     0,     0,   149,     0,
       0,     0,     0,     0,   963,     0,     0,   149,     0,     0,
       0,   149,   143,     0,     0,     0,   974,     0,   143,   977,
       0,   978,   285,   979,     0,     0,    22,    23,  1142,     0,
     143,     0,     0,     0,     0,   286,     0,    30,   287,     0,
       0,     0,     0,    36,     0,   161,     0,   161,     0,     0,
      41,     0,   161,     0,     0,   161,     0,     0,     0,     0,
     149,   154,     0,   158,     0,     0,   161,     0,   131,   161,
       0,     0,     0,   158,    56,   362,    58,     0,    60,     0,
    1028,     0,     0,  1029,     0,   288,   158,    66,   158,   154,
     143,     0,     0,     0,   143,   154,     0,   161,     0,     0,
       0,     0,   143,     0,     0,     0,    82,   154,   161,    84,
     161,     0,    86,     0,     0,     0,     0,   149,   149,   149,
     149,     0,     0,     0,     0,   131,     0,     0,     0,     0,
     363,   131,     0,     0,   364,     0,     0,   158,     0,     0,
     159,     0,     0,     0,   149,   149,     0,     0,     0,     0,
     159,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   101,   159,   158,   159,     0,   154,  1119,   158,
       0,   154,     0,     0,     0,     0,     0,     0,     0,   154,
       0,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,     0,   374,   375,   376,   377,     0,     0,   379,   380,
     381,   382,   383,   161,   385,     0,  1087,   386,   387,   388,
     161,     0,     0,   131,   159,   131,     0,     0,   389,     0,
     131,     0,     0,   131,   161,   143,     0,     0,     0,     0,
       0,     0,     0,     0,   131,     0,     0,   131,     0,   158,
       0,   159,   161,     0,     0,     0,   159,     0,     0,     0,
       0,   161,     0,     0,     0,   161,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   131,     0,   131,     0,
       0,     0,   143,     0,     0,     0,     0,     0,   143,     0,
       0,     0,   285,     0,     0,     0,    22,    23,     0,     0,
       0,     0,   154,     0,   161,   286,     0,    30,   287,     0,
       0,     0,     0,    36,     0,     0,   159,   261,   263,   264,
      41,     0,     0,     0,     0,   158,     0,     0,     0,     0,
       0,   158,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
    1028,     0,     0,  1029,     0,   288,   316,    66,     0,   154,
       0,   161,   161,   161,   161,   154,     0,     0,     0,     0,
     143,   131,   143,     0,     0,     0,    82,   143,   131,    84,
     143,     0,    86,     0,     0,     0,     0,     0,   161,   161,
     158,   143,   131,     0,   143,     0,   285,     0,     0,     0,
      22,    23,   159,     0,     0,   158,     0,     0,   159,   286,
     131,    30,   287,     0,   350,     0,   351,    36,     0,   131,
     -77,     0,   143,   131,    41,     0,     0,  -299,     0,     0,
       0,     0,   101,   143,     0,   143,     0,     0,  1121,     0,
       0,     0,     0,     0,     0,     0,     0,   154,    56,   154,
      58,     0,     0,     0,   154,  -299,     0,   154,     0,   288,
       0,    66,   417,     0,     0,   163,     0,   159,   154,     0,
       0,   154,   131,     0,     0,   163,     0,     0,     0,   316,
      82,   158,   159,    84,   565,   -77,    86,     0,   163,     0,
     163,     0,     0,     0,     0,   316,     0,   445,   767,   154,
       0,   768,     0,     0,     0,     0,   769,     0,     0,   158,
     154,     0,   154,     0,     0,   158,   184,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   770,   158,   143,   131,
     131,   131,   131,   771,     0,   143,     0,     0,     0,   163,
     482,     0,     0,   772,     0,     0,     0,     0,     0,   143,
     773,     0,     0,     0,     0,     0,   131,   131,   159,     0,
       0,     0,     0,     0,     0,   774,   163,   143,     0,     0,
       0,   163,     0,     0,     0,     0,   143,   775,     0,     0,
     143,   569,     0,     0,     0,   285,   159,   158,   776,    22,
      23,   158,   159,   164,   777,     0,     0,     0,   286,   158,
      30,   287,     0,   164,   159,   154,    36,     0,     0,     0,
       0,     0,   154,    41,     0,     0,   164,     0,   164,     0,
       0,     0,     0,   545,     0,     0,   154,     0,     0,   143,
       0,     0,     0,   109,     0,     0,     0,    56,     0,    58,
       0,   163,     0,   175,   154,     0,     0,     0,   288,     0,
      66,     0,     0,   154,     0,     0,   209,   154,   212,     0,
       0,     0,     0,     0,   159,     0,     0,   164,   159,    82,
       0,     0,    84,     0,     0,    86,   159,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   143,   143,   143,   143,
       0,     0,     0,   618,   164,     0,     0,     0,     0,   164,
       0,     0,     0,     0,     0,     0,   154,   306,     0,     0,
       0,   630,     0,   143,   143,   633,     0,     0,     0,     0,
       0,     0,   158,     0,     0,     0,     0,   163,     0,     0,
       0,     0,     0,   163,   109,     0,     0,     0,     0,   343,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   666,
       0,     0,     0,     0,     0,   482,     0,     0,     0,     0,
       0,     0,     0,   154,   154,   154,   154,   285,     0,   164,
       0,    22,    23,     0,     0,     0,     0,     0,     0,   158,
     286,     0,    30,   287,     0,   158,     0,     0,    36,     0,
     154,   154,   163,     0,     0,    41,     0,     0,     0,   159,
       0,     0,     0,     0,     0,     0,     0,   163,     0,   412,
       0,     0,     0,   730,     0,     0,     0,     0,     0,    56,
       0,    58,   745,    60,     0,  1028,     0,     0,  1029,     0,
     288,     0,    66,     0,     0,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
       0,    82,     0,     0,    84,   164,   159,    86,     0,     0,
       0,   164,   159,     0,     0,     0,  -566,   158,     0,   158,
       0,     0,     0,     0,   158,     0,     0,   158,     0,   405,
       0,     0,  -605,   163,  -605,     0,     0,     0,   158,     0,
       0,   158,     0,     0,     0,   477,     0,     0,     0,     0,
       0,   490,     0,   797,     0,     0,   800,   101,     0,     0,
       0,   163,     0,  1170,     0,     0,     0,   163,     0,   158,
     164,     0,     0,     0,     0,     0,     0,     0,     0,   163,
     158,     0,   158,     0,   285,   164,     0,     0,    22,    23,
       0,     0,     0,     0,   159,     0,   159,   286,     0,    30,
     287,   159,     0,   842,   159,    36,     0,   482,     0,     0,
     109,     0,    41,     0,     0,   159,     0,     0,   159,     0,
       0,     0,     0,     0,     0,   109,     0,     0,     0,     0,
     865,     0,     0,     0,   316,     0,    56,     0,    58,   163,
      60,     0,  1028,   163,     0,  1029,   159,   288,     0,    66,
       0,   163,     0,     0,     0,     0,     0,   159,     0,   159,
       0,   164,     0,     0,     0,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,   158,     0,   936,     0,     0,
       0,     0,   158,     0,     0,     0,     0,     0,     0,   164,
       0,     0,     0,     0,     0,   164,   158,     0,     0,     0,
       0,   109,     0,     0,     0,     0,     0,   164,     0,     0,
       0,     0,     0,     0,   158,     0,     0,     0,     0,     0,
       0,     0,     0,   158,   101,     0,     0,   158,     0,   490,
    1171,     0,     0,     0,     0,   490,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     404,     0,   159,     0,     0,     0,   986,     0,   666,   159,
       0,     0,   990,     0,     0,   482,     0,   164,     0,     0,
       0,   164,     0,   159,   163,     0,   158,     0,     0,   164,
     405,     0,     0,  -605,     0,  -605,     0,   865,     0,     0,
       0,   159,     0,     0,     0,     0,     0,     0,     0,     0,
     159,     0,     0,     0,   159,     0,     0,   717,     0,     0,
       0,   722,   745,     0,     0,     0,     0,     0,     0,   109,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   163,     0,   158,   158,   158,   158,   163,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   159,     0,     0,     0,     0,     0,     0,
     158,   158,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,   360,     0,
       0,     0,     0,     0,     0,   361,     0,     0,     0,     0,
     238,   239,   164,   240,     0,     0,     0,   362,   241,     0,
     159,   159,   159,   159,     0,     0,   242,     0,     0,   163,
       0,   163,   243,     0,     0,     0,   163,     0,   244,   163,
       0,     0,   245,     0,     0,   246,     0,   159,   159,     0,
     163,     0,   829,   163,     0,   247,     0,     0,     0,     0,
       0,   248,   249,     0,     0,     0,     0,     0,   250,   164,
       0,     0,   363,     0,   936,   164,   364,   251,     0,     0,
       0,   163,     0,     0,     0,   830,   252,   253,     0,   254,
       0,   255,   163,   256,   163,     0,   257,     0,     0,     0,
     258,   480,     0,   259,     0,     0,   260,   365,     0,   868,
       0,     0,     0,     0,     0,   873,     0,     0,     0,     0,
       0,     0,     0,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,     0,     0,   386,
     387,   388,     0,   420,     0,     0,     0,   164,     0,   164,
     389,     0,   481,     0,   164,     0,     0,   164,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   164,     0,
       0,   164,     0,     0,     0,     0,     0,   163,     0,     0,
       0,     0,     0,     0,   163,     0,     0,   490,     0,   490,
       0,     0,   237,     0,   490,     0,     0,   490,   163,   164,
       0,     0,     0,     0,     0,     0,   238,   239,   982,   240,
     164,   984,   164,     0,   241,     0,   163,     0,     0,     0,
       0,     0,   242,     0,     0,   163,     0,     0,   243,   163,
       0,     0,     0,     0,   244,     0,     0,     0,   245,  1004,
       0,   246,     0,     0,     0,     0,     0,     0,     0,     0,
    1010,   247,  1014,     0,     0,     0,     0,   248,   249,     0,
       0,     0,     0,     0,   250,     0,     0,     0,     0,     0,
       0,     0,     0,   251,     0,     0,     0,     0,   163,     0,
       0,     0,   252,   253,     0,   254,     0,   255,     0,   256,
     360,     0,   257,     0,     0,     0,   258,   361,     0,   259,
       0,     0,   260,     0,     0,   164,     0,     0,     0,   362,
       0,     0,   164,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   164,     0,     0,     0,
       0,     0,     0,     0,     0,   163,   163,   163,   163,     0,
       0,     0,     0,     0,   164,     0,     0,     0,     0,     0,
       0,     0,     0,   164,     0,     0,   420,   164,   863,     0,
       0,     0,   163,   163,   363,     0,  1066,     0,   364,     0,
       0,     0,     0,     0,     0,     0,     0,   466,     0,     0,
       0,     0,     0,     0,  1086,     0,     0,     0,     0,     0,
       0,     0,     0,  1091,     0,     0,     0,  1093,     0,   365,
     467,   391,   392,   393,   394,   395,   164,     0,   398,   399,
     400,   401,     0,   403,   404,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,     0,   374,   375,   376,   377,
     378,     0,   379,   380,   381,   382,   383,   384,   385,     0,
       0,   386,   387,   388,     0,   420,  1127,     0,     0,     0,
       0,     0,   389,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   164,   164,   164,   164,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    -2,     4,
       0,     5,     0,     6,     7,     8,     9,    10,    11,     0,
     164,   164,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,   -76,     0,    39,    40,    41,     0,
      42,  -299,     0,    43,    44,    45,    46,    47,     0,    48,
      49,    50,   -47,    51,    52,     0,    53,    54,    55,     0,
       0,     0,    56,    57,    58,    59,    60,    61,    62,  -299,
     -47,    63,    64,    65,     0,    66,    67,    68,     0,    69,
      70,    71,    72,    73,    74,    75,    76,     0,    77,    78,
       0,    79,    80,    81,    82,    83,     0,    84,    85,   -76,
      86,    87,     0,     0,    88,     0,    89,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
     101,     0,     0,   102,     0,   103,     0,   104,     0,   105,
       0,     4,   106,     5,     0,     6,     7,     8,     9,    10,
      11,     0,  -635,     0,    12,    13,    14,    15,    16,  -635,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,  -635,    27,    28,  -635,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,    38,   -76,     0,    39,    40,
      41,     0,    42,  -299,     0,    43,    44,    45,    46,    47,
       0,    48,    49,    50,   -47,    51,    52,     0,    53,    54,
      55,     0,     0,     0,    56,    57,    58,    59,     0,    61,
      62,  -299,   -47,    63,    64,    65,  -635,    66,    67,    68,
    -635,    69,    70,    71,    72,    73,    74,    75,    76,     0,
      77,    78,     0,    79,    80,    81,    82,    83,     0,    84,
      85,   -76,    86,    87,     0,     0,    88,     0,    89,     0,
       0,  -635,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -635,  -635,    93,
    -635,  -635,  -635,  -635,  -635,  -635,  -635,     0,  -635,  -635,
    -635,  -635,  -635,     0,  -635,  -635,  -635,  -635,  -635,  -635,
    -635,  -635,   101,  -635,  -635,  -635,     0,   103,  -635,   104,
       0,   105,     0,   326,  -635,     5,   290,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,    38,   -76,     0,
      39,    40,    41,     0,    42,  -299,     0,    43,    44,    45,
      46,    47,     0,    48,    49,    50,   -47,    51,    52,     0,
      53,    54,    55,     0,     0,     0,    56,    57,    58,    59,
      60,    61,    62,  -299,   -47,    63,    64,    65,     0,    66,
      67,    68,     0,    69,    70,    71,    72,    73,    74,    75,
      76,     0,    77,    78,     0,    79,    80,    81,    82,    83,
       0,    84,    85,   -76,    86,    87,     0,     0,    88,     0,
      89,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,   101,     0,     0,   102,     0,   103,
     327,   104,     0,   105,     0,     4,   106,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,    38,
     -76,     0,    39,    40,    41,     0,    42,  -299,     0,    43,
      44,    45,    46,    47,     0,    48,    49,    50,   -47,    51,
      52,     0,    53,    54,    55,     0,     0,     0,    56,    57,
      58,    59,    60,    61,    62,  -299,   -47,    63,    64,    65,
       0,    66,    67,    68,     0,    69,    70,    71,    72,    73,
      74,    75,    76,     0,    77,    78,     0,    79,    80,    81,
      82,    83,     0,    84,    85,   -76,    86,    87,     0,     0,
      88,     0,    89,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,   101,     0,     0,   102,
       0,   103,   517,   104,     0,   105,     0,   536,   106,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,    38,   -76,     0,    39,    40,    41,     0,    42,  -299,
       0,    43,    44,    45,    46,    47,     0,    48,    49,    50,
     -47,    51,    52,     0,    53,    54,    55,     0,     0,     0,
      56,    57,    58,    59,    60,    61,    62,  -299,   -47,    63,
      64,    65,     0,    66,    67,    68,     0,    69,    70,    71,
      72,    73,    74,    75,    76,     0,    77,    78,     0,    79,
      80,    81,    82,    83,     0,    84,    85,   -76,    86,    87,
       0,     0,    88,     0,    89,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,   101,     0,
       0,   102,     0,   103,   537,   104,     0,   105,     0,   326,
     106,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,   -76,     0,    39,    40,    41,     0,
      42,  -299,     0,    43,    44,    45,    46,    47,     0,    48,
      49,    50,   -47,    51,    52,     0,    53,    54,    55,     0,
       0,     0,    56,    57,    58,    59,    60,    61,    62,  -299,
     -47,    63,    64,    65,     0,    66,    67,    68,     0,    69,
      70,    71,    72,    73,    74,    75,    76,     0,    77,    78,
       0,    79,    80,    81,    82,    83,     0,    84,    85,   -76,
      86,    87,     0,     0,    88,     0,    89,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
     101,     0,     0,   102,     0,   103,   327,   104,     0,   105,
       0,     4,   106,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,    38,   -76,     0,    39,    40,
      41,     0,    42,  -299,     0,    43,    44,    45,    46,    47,
       0,    48,    49,    50,   -47,    51,    52,     0,    53,    54,
      55,     0,     0,     0,    56,    57,    58,    59,    60,    61,
      62,  -299,   -47,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,    76,     0,
      77,    78,     0,    79,    80,    81,    82,    83,     0,    84,
      85,   -76,    86,    87,     0,     0,    88,     0,    89,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,   101,     0,     0,   102,     0,   103,   729,   104,
       0,   105,     0,     4,   106,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,    38,   -76,     0,
      39,    40,    41,     0,    42,  -299,     0,    43,    44,    45,
      46,    47,     0,    48,    49,    50,   -47,    51,    52,     0,
      53,    54,    55,     0,     0,     0,    56,    57,    58,    59,
     342,    61,    62,  -299,   -47,    63,    64,    65,     0,    66,
      67,    68,     0,    69,    70,    71,    72,    73,    74,    75,
      76,     0,    77,    78,     0,    79,    80,    81,    82,    83,
       0,    84,    85,   -76,    86,    87,     0,     0,    88,     0,
      89,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,   101,     0,     0,   102,     0,   103,
       0,   104,     0,   105,     0,     4,   106,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,    38,
     -76,     0,    39,    40,    41,     0,    42,  -299,     0,    43,
      44,    45,    46,    47,     0,    48,    49,    50,   -47,    51,
      52,     0,    53,    54,    55,     0,     0,     0,    56,    57,
      58,    59,     0,    61,    62,  -299,   -47,    63,    64,    65,
       0,    66,    67,    68,     0,    69,    70,    71,    72,    73,
      74,    75,    76,     0,    77,    78,     0,    79,    80,    81,
      82,    83,     0,    84,    85,   -76,    86,    87,     0,     0,
      88,     0,    89,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,   101,     0,     0,   102,
       0,   103,     0,   104,     0,   105,     0,     4,   106,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
     638,    38,   -76,     0,    39,    40,    41,     0,    42,  -299,
       0,    43,    44,    45,    46,    47,     0,    48,    49,    50,
     -47,    51,    52,     0,    53,    54,    55,     0,     0,     0,
      56,    57,    58,    59,     0,    61,    62,  -299,   -47,    63,
      64,    65,     0,    66,    67,    68,     0,    69,    70,    71,
      72,    73,    74,    75,    76,     0,    77,    78,     0,    79,
      80,    81,    82,    83,     0,    84,    85,   -76,    86,    87,
       0,     0,    88,     0,    89,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,   101,     0,
       0,   102,     0,   103,     0,   104,     0,   105,     0,     4,
     106,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,   644,    38,   -76,     0,    39,    40,    41,     0,
      42,  -299,     0,    43,    44,    45,    46,    47,     0,    48,
      49,    50,   -47,    51,    52,     0,    53,    54,    55,     0,
       0,     0,    56,    57,    58,    59,     0,    61,    62,  -299,
     -47,    63,    64,    65,     0,    66,    67,    68,     0,    69,
      70,    71,    72,    73,    74,    75,    76,     0,    77,    78,
       0,    79,    80,    81,    82,    83,     0,    84,    85,   -76,
      86,    87,     0,     0,    88,     0,    89,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
     101,     0,     0,   102,     0,   103,     0,   104,     0,   105,
       0,     4,   106,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,   965,    38,   -76,     0,    39,    40,
      41,     0,    42,  -299,     0,    43,    44,    45,    46,    47,
       0,    48,    49,    50,   -47,    51,    52,     0,    53,    54,
      55,     0,     0,     0,    56,    57,    58,    59,     0,    61,
      62,  -299,   -47,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,    76,     0,
      77,    78,     0,    79,    80,    81,    82,    83,     0,    84,
      85,   -76,    86,    87,     0,     0,    88,     0,    89,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,   101,     0,     0,   102,     0,   103,     0,   104,
       0,   105,     0,     4,   106,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,   967,    38,   -76,     0,
      39,    40,    41,     0,    42,  -299,     0,    43,    44,    45,
      46,    47,     0,    48,    49,    50,   -47,    51,    52,     0,
      53,    54,    55,     0,     0,     0,    56,    57,    58,    59,
       0,    61,    62,  -299,   -47,    63,    64,    65,     0,    66,
      67,    68,     0,    69,    70,    71,    72,    73,    74,    75,
      76,     0,    77,    78,     0,    79,    80,    81,    82,    83,
       0,    84,    85,   -76,    86,    87,     0,     0,    88,     0,
      89,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,   101,     0,     0,   102,     0,   103,
       0,   104,     0,   105,     0,     4,   106,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,   972,    38,
     -76,     0,    39,    40,    41,     0,    42,  -299,     0,    43,
      44,    45,    46,    47,     0,    48,    49,    50,   -47,    51,
      52,     0,    53,    54,    55,     0,     0,     0,    56,    57,
      58,    59,     0,    61,    62,  -299,   -47,    63,    64,    65,
       0,    66,    67,    68,     0,    69,    70,    71,    72,    73,
      74,    75,    76,     0,    77,    78,     0,    79,    80,    81,
      82,    83,     0,    84,    85,   -76,    86,    87,     0,     0,
      88,     0,    89,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,   101,     0,     0,   102,
       0,   103,     0,   104,     0,   105,     0,     4,   106,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
     975,    38,   -76,     0,    39,    40,    41,     0,    42,  -299,
       0,    43,    44,    45,    46,    47,     0,    48,    49,    50,
     -47,    51,    52,     0,    53,    54,    55,     0,     0,     0,
      56,    57,    58,    59,     0,    61,    62,  -299,   -47,    63,
      64,    65,     0,    66,    67,    68,     0,    69,    70,    71,
      72,    73,    74,    75,    76,     0,    77,    78,     0,    79,
      80,    81,    82,    83,     0,    84,    85,   -76,    86,    87,
       0,     0,    88,     0,    89,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,   101,     0,
       0,   102,     0,   103,     0,   104,     0,   105,     0,     4,
     106,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
    1003,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,   -76,     0,    39,    40,    41,     0,
      42,  -299,     0,    43,    44,    45,    46,    47,     0,    48,
      49,    50,   -47,    51,    52,     0,    53,    54,    55,     0,
       0,     0,    56,    57,    58,    59,     0,    61,    62,  -299,
     -47,    63,    64,    65,     0,    66,    67,    68,     0,    69,
      70,    71,    72,    73,    74,    75,    76,     0,    77,    78,
       0,    79,    80,    81,    82,    83,     0,    84,    85,   -76,
      86,    87,     0,     0,    88,     0,    89,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
     101,     0,     0,   102,     0,   103,     0,   104,     0,   105,
       0,     4,   106,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,  1009,    38,   -76,     0,    39,    40,
      41,     0,    42,  -299,     0,    43,    44,    45,    46,    47,
       0,    48,    49,    50,   -47,    51,    52,     0,    53,    54,
      55,     0,     0,     0,    56,    57,    58,    59,     0,    61,
      62,  -299,   -47,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,    76,     0,
      77,    78,     0,    79,    80,    81,    82,    83,     0,    84,
      85,   -76,    86,    87,     0,     0,    88,     0,    89,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,   101,     0,     0,   102,     0,   103,     0,   104,
       0,   105,     0,     4,   106,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,  1013,    38,   -76,     0,
      39,    40,    41,     0,    42,  -299,     0,    43,    44,    45,
      46,    47,     0,    48,    49,    50,   -47,    51,    52,     0,
      53,    54,    55,     0,     0,     0,    56,    57,    58,    59,
       0,    61,    62,  -299,   -47,    63,    64,    65,     0,    66,
      67,    68,     0,    69,    70,    71,    72,    73,    74,    75,
      76,     0,    77,    78,     0,    79,    80,    81,    82,    83,
       0,    84,    85,   -76,    86,    87,     0,     0,    88,     0,
      89,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,   101,     0,     0,   102,     0,   103,
       0,   104,     0,   105,     0,  1199,   106,     5,   290,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   192,  1200,   851,   106,     5,
     290,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,     0,     0,
       0,   102,     0,   191,     0,   104,     0,   192,     0,     5,
     106,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,   231,    23,     0,     0,     0,     0,     0,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,    57,    58,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,    66,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,    86,    87,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,     0,     0,
       0,   102,     0,   191,     0,   104,     0,   192,     0,     0,
     106,     5,   290,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,     0,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,     0,
       0,     0,    64,   189,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   190,     0,    77,     0,
       0,    79,   294,   295,     0,    83,   331,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,   332,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
       0,     0,     0,   102,     0,   191,     0,   104,     0,   192,
       0,     0,   106,     5,   290,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,   294,   295,     0,    83,   331,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
     713,   192,     0,     0,   106,     5,   290,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   182,     0,     0,    15,
      16,     0,    17,     0,   183,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   184,     0,     0,
       0,    32,   185,   186,     0,     0,   187,    38,     0,     0,
       0,    40,     0,     0,    42,     0,     0,   188,     0,     0,
      46,    47,     0,     0,    49,    50,     0,    51,    52,     0,
      53,    54,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,   294,   295,     0,    83,
     331,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,     0,     0,     0,   102,     0,   191,
       0,   104,   715,   192,     0,     5,   106,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   182,     0,     0,    15,
      16,     0,    17,     0,   183,     0,     0,    21,     0,   601,
       0,     0,     0,     0,     0,    28,     0,   184,     0,     0,
       0,    32,   185,   186,     0,     0,   187,    38,     0,   602,
       0,    40,     0,     0,    42,     0,     0,   188,     0,     0,
      46,    47,     0,     0,    49,    50,     0,    51,    52,     0,
      53,    54,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,   603,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,   604,    87,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,     0,     0,     0,   102,     0,   191,
       0,   104,     0,   192,     0,     5,   106,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   182,     0,     0,    15,
      16,     0,    17,     0,   183,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   184,     0,     0,
       0,    32,   185,   186,     0,     0,   187,    38,     0,     0,
       0,    40,     0,     0,    42,     0,     0,   188,     0,     0,
      46,    47,     0,     0,    49,    50,     0,    51,    52,     0,
      53,    54,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,   294,   295,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,   296,     0,     0,   102,     0,   191,
       0,   104,     0,   192,     0,     0,   106,     5,   290,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,   294,   295,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   192,     0,     5,   106,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,   853,    53,    54,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,   294,   295,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   192,     0,     5,   106,     6,
       7,     8,   218,    10,    11,   219,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   220,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
     221,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   192,     0,     5,   106,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,    57,
     223,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,   224,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   192,     0,     0,   106,     5,
     290,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,   224,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,     0,     0,
       0,   102,     0,   191,     0,   104,     0,   192,     0,     5,
     106,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,     0,    79,
     294,   295,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,     0,     0,
       0,   102,     0,   191,     0,   104,     0,   192,     0,     0,
     106,     5,   290,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,     0,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,     0,
       0,     0,    64,   189,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   190,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
       0,     0,     0,   102,     0,   191,   711,   104,     0,   192,
       0,     0,   106,     5,   290,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,   721,     0,   106,     5,   290,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   182,     0,     0,    15,
      16,     0,    17,     0,   183,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   184,     0,     0,
       0,    32,   185,   186,     0,     0,   187,    38,     0,     0,
       0,    40,     0,     0,    42,     0,     0,   188,     0,     0,
      46,    47,     0,     0,    49,    50,     0,    51,    52,     0,
      53,    54,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,     0,     0,     0,   102,     0,   191,
       0,   104,     0,   192,  1131,     5,   106,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   182,     0,     0,    15,
      16,     0,    17,     0,   183,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   184,     0,     0,
       0,    32,   185,   186,     0,     0,   187,    38,     0,     0,
       0,    40,     0,     0,    42,     0,     0,   188,     0,     0,
      46,    47,     0,     0,    49,    50,     0,    51,    52,     0,
      53,    54,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,   224,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,     0,     0,     0,   102,     0,   191,
       0,   104,     0,   192,     0,     5,   106,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   182,     0,     0,    15,
      16,     0,    17,     0,   183,     0,     0,    21,     0,     0,
       0,     0,     0,     0,   270,    28,     0,   184,     0,     0,
       0,    32,   185,   186,     0,     0,   187,    38,     0,     0,
       0,    40,     0,     0,    42,     0,     0,   188,     0,     0,
      46,    47,     0,     0,    49,    50,     0,    51,    52,     0,
      53,    54,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,     0,     0,     0,   102,     0,   103,
       0,   104,     0,   192,     0,     0,   106,     5,   290,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   192,     0,     5,   106,     6,
       7,     8,   218,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   220,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
     221,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   192,     0,     5,   106,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   192,   726,     5,   106,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
       0,    83,   879,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   192,     0,     0,   106,     5,
     290,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,   685,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,   686,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,     0,     0,     0,     0,    71,
     687,    73,    74,    75,   688,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,     0,     0,
       0,   102,     0,   191,     0,   104,     0,  1070,     0,     5,
     106,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,     0,    79,
       0,     0,     0,    83,  1095,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,     0,     0,
       0,   102,     0,   191,     0,   104,     0,   192,     0,     0,
     106,     5,   290,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,     0,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,     0,
       0,     0,    64,   189,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   190,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
       0,     0,     0,   102,     0,   191,     0,   104,     0,  1070,
       0,     0,   106,     5,   290,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,  1051,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,   214,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   215,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   103,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   653,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   685,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,   686,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,   687,    73,    74,    75,   688,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   689,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   689,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   781,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   784,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,  1045,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,  1046,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,  1048,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,  1049,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,  1050,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,  1051,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   685,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,   686,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,   687,    73,    74,    75,   688,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,  1155,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   892,     0,   893,    91,    92,    93,
      94,    95,     0,     0,     0,   894,    96,     0,     0,     0,
     895,   239,   896,   897,     0,     0,    97,     0,   898,    98,
      99,   100,     0,     0,     0,   102,   242,     0,   184,   104,
       0,  1155,   899,     0,   106,     0,     0,     0,   900,     0,
       0,     0,   245,     0,     0,   901,     0,   902,     0,     0,
       0,     0,     0,     0,     0,   903,     0,     0,     0,     0,
       0,   904,   905,     0,     0,     0,     0,     0,   250,     0,
       0,     0,     0,     0,     0,     0,     0,   906,     0,     0,
       0,     0,     0,     0,     0,     0,   252,   253,     0,   907,
       0,   255,     0,   908,     0,     0,   909,     0,     0,     0,
     910,     0,     0,   259,     0,     0,   911,     0,     0,     0,
       0,     0,     0,     0,     0,   467,   391,   392,   393,   394,
     395,     0,     0,   398,   399,   400,   401,     0,   403,   404,
     912,   913,   914,   915,   916,     0,     0,   917,     0,     0,
       0,   918,   919,   920,   921,   922,   923,   924,   925,   926,
     927,   928,     0,   929,     0,     0,   930,   931,   932,   933,
       0,     5,   934,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,     0,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,     0,
       0,     0,    64,   189,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   190,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,     0,    99,   100,
       0,     0,     0,   102,     0,   191,     0,   104,     0,   192,
       5,   290,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   182,     0,     0,    15,    16,     0,    17,     0,   183,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   184,     0,     0,     0,    32,   185,   186,     0,
       0,   187,    38,     0,     0,     0,    40,     0,     0,    42,
       0,     0,   188,     0,     0,    46,    47,     0,     0,    49,
      50,     0,    51,    52,     0,    53,    54,     0,     0,     0,
       0,     0,    57,     0,    59,     0,    61,     0,     0,     0,
       0,    64,   189,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   190,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,  -463,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
    -463,     0,     0,     0,  -463,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,   191,     0,   104,  -463,  1070,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,  -445,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,  -445,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,  -445,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,  -445,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,    93,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   182,     0,     0,    15,    16,
       0,    17,     0,   183,     0,     0,    21,   100,  -445,     0,
       0,     0,     0,  -445,    28,   104,   184,  1155,     0,     0,
      32,   185,   186,     0,     0,   187,    38,     0,     0,     0,
      40,     0,     0,    42,     0,     0,   188,     0,     0,    46,
      47,     0,     0,    49,    50,     0,    51,    52,     0,    53,
      54,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,     0,     0,     0,    64,   189,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   190,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,     5,
      93,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,   100,     0,     0,     0,     0,     0,   191,    28,
     104,   184,   192,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,    93,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   182,     0,     0,    15,    16,
       0,    17,     0,   183,     0,     0,    21,   100,     0,     0,
       0,     0,     0,   191,    28,   104,   184,   689,     0,     0,
      32,   185,   186,     0,     0,   187,    38,     0,     0,     0,
      40,     0,     0,    42,     0,     0,   188,     0,     0,    46,
      47,     0,     0,    49,    50,     0,    51,    52,     0,    53,
      54,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,     0,     0,     0,    64,   189,     0,   237,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   190,
       0,    77,   238,   239,    79,   240,     0,     0,    83,     0,
     241,    85,     0,     0,    87,     0,     0,     0,   242,     0,
       0,     0,     0,     0,   243,     0,     0,     0,     0,     0,
     244,     0,     0,     0,   245,     0,     0,   246,     0,     0,
      93,     0,     0,     0,     0,     0,     0,   247,     0,     0,
       0,     0,     0,   248,   249,     0,     0,     0,     0,     0,
     250,     0,     0,     0,     0,     0,     0,     0,     0,   251,
     104,     0,   192,     0,     0,     0,     0,     0,   252,   253,
       0,   254,     0,   255,     0,   256,   237,     0,   257,     0,
       0,     0,   258,     0,     0,   259,     0,     0,   260,     0,
     238,   239,     0,   240,     0,     0,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,     0,
       0,     0,   243,     0,     0,     0,     0,     0,   244,     0,
       0,     0,   245,     0,     0,   246,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   247,     0,     0,     0,     0,
       0,   248,   249,     0,   266,     0,     0,     0,   250,     0,
       0,     0,     0,     0,     0,     0,     0,   251,     0,     0,
       0,     0,     0,     0,     0,     0,   252,   253,     0,   254,
       0,   255,     0,   256,     0,  1108,   257,     0,     0,     0,
     258,     0,     0,   259,   894,     0,   260,     0,     0,   238,
     239,   896,   240,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,   242,     0,     0,     0,     0,
       0,   899,     0,     0,     0,     0,     0,   244,     0,     0,
       0,   245,     0,     0,   246,     0,   902,     0,     0,     0,
       0,     0,     0,     0,   247,     0,     0,     0,     0,     0,
     904,   249,   841,     0,     0,     0,     0,   250,     0,     0,
       0,     0,     0,     0,     0,     0,   251,     0,     0,     0,
       0,     0,     0,     0,     0,   252,   253,   360,   254,     0,
     255,     0,  1109,     0,   361,   909,     0,     0,     0,   258,
       0,     0,   259,     0,     0,   260,   362,     0,     0,     0,
       0,     0,     0,     0,   467,   391,   392,   393,   394,   395,
       0,     0,   398,   399,   400,   401,     0,   403,   404,   912,
     913,   914,   915,   916,     0,     0,   917,     0,     0,     0,
     918,   919,   920,   921,   922,   923,   924,   925,   926,   927,
     928,     0,   929,     0,     0,   930,   931,   932,   933,     0,
       0,   363,     0,   360,     0,   364,     0,     0,     0,     0,
     361,     0,     0,     0,   964,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   365,   467,   391,   392,
     393,   394,   395,     0,     0,   398,   399,   400,   401,     0,
     403,   404,   366,   367,     0,   368,   369,   370,     0,   371,
     372,   373,     0,   374,   375,   376,   377,   378,     0,   379,
     380,   381,   382,   383,   384,   385,     0,   363,   386,   387,
     388,   364,   420,     0,     0,   360,     0,     0,     0,   389,
     971,     0,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,     0,     0,     0,     0,     0,
       0,     0,   365,   467,   391,   392,   393,   394,   395,     0,
       0,   398,   399,   400,   401,     0,   403,   404,   366,   367,
       0,   368,   369,   370,     0,   371,   372,   373,     0,   374,
     375,   376,   377,   378,     0,   379,   380,   381,   382,   383,
     384,   385,     0,     0,   386,   387,   388,     0,   420,   363,
       0,   360,     0,   364,     0,   389,     0,     0,   361,     0,
       0,     0,  1122,     0,     0,     0,     0,     0,     0,     0,
     362,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   365,   467,   391,   392,   393,   394,
     395,     0,     0,   398,   399,   400,   401,     0,   403,   404,
     366,   367,     0,   368,   369,   370,     0,   371,   372,   373,
       0,   374,   375,   376,   377,   378,     0,   379,   380,   381,
     382,   383,   384,   385,     0,   363,   386,   387,   388,   364,
     420,     0,     0,   360,     0,     0,     0,   389,  1123,     0,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
     365,   467,   391,   392,   393,   394,   395,     0,     0,   398,
     399,   400,   401,     0,   403,   404,   366,   367,     0,   368,
     369,   370,     0,   371,   372,   373,     0,   374,   375,   376,
     377,   378,     0,   379,   380,   381,   382,   383,   384,   385,
       0,     0,   386,   387,   388,     0,   420,   363,     0,   360,
       0,   364,     0,   389,     0,     0,   361,     0,     0,     0,
    1124,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   365,   467,   391,   392,   393,   394,   395,     0,
       0,   398,   399,   400,   401,     0,   403,   404,   366,   367,
       0,   368,   369,   370,     0,   371,   372,   373,     0,   374,
     375,   376,   377,   378,     0,   379,   380,   381,   382,   383,
     384,   385,     0,   363,   386,   387,   388,   364,   420,     0,
       0,   360,     0,     0,     0,   389,  1125,     0,   361,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     362,     0,     0,     0,     0,     0,     0,     0,   365,   467,
     391,   392,   393,   394,   395,     0,     0,   398,   399,   400,
     401,     0,   403,   404,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,     0,   374,   375,   376,   377,   378,
       0,   379,   380,   381,   382,   383,   384,   385,     0,     0,
     386,   387,   388,     0,   420,   363,     0,   360,     0,   364,
       0,   389,     0,     0,   361,     0,     0,     0,  1149,     0,
       0,     0,     0,     0,     0,     0,   362,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     365,   467,   391,   392,   393,   394,   395,     0,     0,   398,
     399,   400,   401,     0,   403,   404,   366,   367,     0,   368,
     369,   370,     0,   371,   372,   373,     0,   374,   375,   376,
     377,   378,     0,   379,   380,   381,   382,   383,   384,   385,
       0,   363,   386,   387,   388,   364,   420,     0,     0,   360,
       0,     0,     0,   389,  1150,     0,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,   454,
       0,     0,     0,     0,     0,     0,   365,   467,   391,   392,
     393,   394,   395,     0,   455,   398,   399,   400,   401,     0,
     403,   404,   366,   367,     0,   368,   369,   370,     0,   371,
     372,   373,     0,   374,   375,   376,   377,   378,     0,   379,
     380,   381,   382,   383,   384,   385,     0,     0,   386,   387,
     388,     0,   420,   363,     0,   360,     0,   364,     0,   389,
       0,     0,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,   270,     0,     0,     0,     0,
       0,     0,     0,     0,   451,     0,     0,     0,   365,     0,
     460,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,   362,   374,   375,   376,   377,   378,
       0,   379,   380,   381,   382,   383,   384,   385,     0,   363,
     386,   387,   388,   364,   420,   360,     0,     0,     0,     0,
       0,   389,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,   448,     0,     0,     0,     0,
     451,     0,     0,     0,   365,     0,     0,     0,     0,   363,
     449,     0,     0,   364,     0,     0,     0,     0,     0,     0,
     366,   367,     0,   368,   369,   370,     0,   371,   372,   373,
       0,   374,   375,   376,   377,   378,     0,   379,   380,   381,
     382,   383,   384,   385,   365,     0,   386,   387,   388,   363,
     420,   360,     0,   364,     0,     0,     0,   389,   361,     0,
     366,   367,     0,   368,   369,   370,     0,   371,   372,   373,
     362,   374,   375,   376,   377,   463,     0,   379,   380,   381,
     382,   383,   384,   385,   365,     0,   386,   387,   388,     0,
       0,     0,     0,     0,     0,     0,     0,   389,     0,     0,
     366,   367,     0,   368,   369,   370,     0,   371,   372,   373,
     464,   374,   375,   376,   377,   378,     0,   379,   380,   381,
     382,   383,   384,   385,     0,   363,   386,   387,   388,   364,
     420,     0,     0,   360,   737,     0,     0,   389,     0,     0,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,   738,     0,     0,
     365,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   366,   367,     0,   368,
     369,   370,     0,   371,   372,   373,     0,   374,   375,   376,
     377,   378,   739,   379,   380,   381,   382,   383,   384,   385,
       0,   465,   386,   387,   388,     0,     0,   363,     0,   360,
       0,   364,     0,   389,     0,     0,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,   816,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   365,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,   367,
       0,   368,   369,   370,     0,   371,   372,   373,     0,   374,
     375,   376,   377,   378,     0,   379,   380,   381,   382,   383,
     384,   385,     0,   363,   386,   387,   388,   364,     0,   360,
       0,     0,     0,     0,     0,   389,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,   270,
       0,     0,     0,     0,   451,     0,     0,     0,   365,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,     0,   374,   375,   376,   377,   378,
       0,   379,   380,   381,   382,   383,   384,   385,     0,     0,
     386,   387,   388,   363,   420,   360,     0,   364,     0,     0,
       0,   389,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   451,     0,     0,     0,   365,     0,
     422,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,     0,   374,   375,   376,   377,   378,
       0,   379,   380,   381,   382,   383,   384,   385,     0,   363,
     386,   387,   388,   364,   420,   360,     0,     0,     0,     0,
       0,   389,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,   270,     0,     0,     0,     0,
     177,     0,     0,     0,   365,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     366,   367,     0,   368,   369,   370,     0,   371,   372,   373,
       0,   374,   375,   376,   377,   378,     0,   379,   380,   381,
     382,   383,   384,   385,     0,     0,   386,   387,   388,   363,
       0,   360,     0,   364,     0,     0,     0,   389,   361,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     362,   610,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   365,     0,   611,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     366,   367,     0,   368,   369,   370,     0,   371,   372,   373,
     362,   374,   375,   376,   377,   378,     0,   379,   380,   381,
     382,   383,   384,   385,     0,   363,   386,   387,   388,   364,
     420,   360,     0,     0,     0,     0,     0,   389,   361,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     362,   612,     0,     0,     0,     0,     0,     0,     0,     0,
     365,     0,     0,     0,     0,   363,   613,     0,     0,   364,
       0,     0,     0,     0,     0,     0,   366,   367,     0,   368,
     369,   370,     0,   371,   372,   373,     0,   374,   375,   376,
     377,   378,     0,   379,   380,   381,   382,   383,   384,   385,
       0,     0,   386,   387,   388,   363,     0,   360,     0,   364,
       0,     0,     0,   389,   361,     0,   366,   367,     0,   368,
     369,   370,     0,   371,   372,   373,   362,   808,   375,   376,
     377,     0,     0,   379,   380,   381,   382,     0,     0,   385,
     365,     0,   386,   387,   388,     0,     0,     0,     0,     0,
       0,     0,     0,   389,     0,     0,   366,   367,     0,   368,
     369,   370,     0,   371,   372,   373,     0,   374,   375,   376,
     377,   378,     0,   379,   380,   381,   382,   383,   384,   385,
       0,   363,   386,   387,   388,   364,     0,   360,     0,     0,
       0,     0,     0,   389,   361,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   362,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   365,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   366,   367,     0,   368,   369,   370,     0,   371,
     372,   373,     0,   374,   375,   376,   377,   378,     0,   379,
     380,   381,   382,   383,   384,   385,     0,     0,   386,   387,
     388,   363,   420,   360,     0,   364,     0,     0,     0,   389,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,  1022,     0,     0,
       0,     0,   451,     0,     0,     0,   365,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   366,   367,     0,   368,   369,   370,     0,   371,
     372,   373,  1023,   374,   375,   376,   377,   378,     0,   379,
     380,   381,   382,   383,   384,   385,     0,   363,   386,   387,
     388,   364,     0,   360,     0,     0,     0,   871,     0,   389,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   365,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,   367,
       0,   368,   369,   370,     0,   371,   372,   373,     0,   374,
     375,   376,   377,   378,     0,   379,   380,   381,   382,   383,
     384,   385,     0,     0,   386,   387,   388,   363,     0,   360,
       0,   364,     0,     0,     0,   389,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   365,     0,     0,     0,     0,     0,     0,   360,
       0,     0,     0,     0,     0,     0,   361,     0,   366,   367,
       0,   368,   369,   370,     0,   371,   372,   373,   362,   374,
     375,   376,   377,   378,     0,   379,   380,   381,   382,   383,
     384,   385,     0,   363,   386,   387,   388,   364,   502,   360,
       0,     0,     0,     0,     0,   389,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   365,     0,
       0,     0,     0,   363,     0,     0,     0,   364,     0,     0,
       0,     0,     0,     0,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,     0,   374,   375,   376,   377,   378,
       0,   379,   380,   381,   382,   383,   384,   385,   365,   506,
     386,   387,   388,   363,     0,     0,     0,   364,     0,     0,
       0,   389,     0,     0,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,     0,   374,   375,   376,   377,   378,
       0,   379,   380,   381,   382,   383,   384,   385,   365,   508,
     386,   387,   388,     0,     0,   360,     0,     0,     0,     0,
       0,   389,   361,     0,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,   362,   374,   375,   376,   377,   378,
       0,   379,   380,   381,   382,   383,   384,   385,     0,   510,
     386,   387,   388,     0,     0,     0,     0,     0,     0,     0,
       0,   389,     0,     0,   360,     0,     0,     0,     0,     0,
       0,   361,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,   363,
       0,     0,     0,   364,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   360,     0,     0,     0,     0,     0,
       0,   361,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   362,   365,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   363,     0,
     366,   367,   364,   368,   369,   370,     0,   371,   372,   373,
       0,   374,   375,   376,   377,   378,     0,   379,   380,   381,
     382,   383,   384,   385,     0,   515,   386,   387,   388,     0,
       0,     0,   532,   365,     0,     0,     0,   389,   363,     0,
     360,     0,   364,     0,     0,     0,     0,   361,     0,   366,
     367,   614,   368,   369,   370,     0,   371,   372,   373,   362,
     374,   375,   376,   377,   378,     0,   379,   380,   381,   382,
     383,   384,   385,   365,     0,   386,   387,   388,     0,     0,
     360,   733,     0,     0,     0,     0,   389,   361,     0,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   362,
     374,   375,   376,   377,   378,     0,   379,   380,   381,   382,
     383,   384,   385,     0,   363,   386,   387,   388,   364,     0,
       0,   360,     0,     0,     0,     0,   389,     0,   361,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     362,     0,     0,     0,     0,     0,     0,     0,     0,   365,
       0,     0,     0,     0,   363,     0,     0,     0,   364,     0,
       0,     0,     0,     0,     0,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,     0,   374,   375,   376,   377,
     378,     0,   379,   380,   381,   382,   383,   384,   385,   365,
       0,   386,   387,   388,     0,   363,     0,     0,   712,   364,
       0,     0,   389,     0,     0,   366,   367,     0,   368,   369,
     370,     0,   371,   734,   373,     0,   374,   375,   376,   377,
     378,     0,   379,   380,   381,   382,   383,   384,   385,   360,
     365,   386,   387,   388,     0,     0,   361,     0,     0,     0,
       0,     0,   389,     0,     0,     0,   366,   367,   362,   368,
     369,   370,     0,   371,   372,   373,     0,   374,   375,   376,
     377,   378,  -567,   379,   380,   381,   382,   383,   384,   385,
     360,     0,   386,   387,   388,     0,     0,   361,     0,     0,
       0,     0,     0,   389,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,     0,     0,     0,   364,     0,     0,
     360,   825,     0,     0,     0,     0,     0,   361,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,   177,     0,     0,     0,   365,     0,
       0,     0,     0,     0,   363,     0,     0,     0,   364,     0,
       0,     0,     0,     0,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,     0,   374,   375,   376,   377,   378,
       0,   379,   380,   381,   382,   383,   384,   385,   362,   365,
     386,   387,   388,     0,   363,     0,     0,     0,   364,     0,
       0,   389,     0,     0,     0,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,     0,   374,   375,   376,   377,
     378,     0,   379,   380,   381,   382,   383,   384,   385,   365,
     793,   386,   387,   388,     0,     0,     0,     0,     0,     0,
       0,     0,   389,   363,     0,   366,   367,   364,   368,   369,
     370,     0,   371,   372,   373,     0,   374,   375,   376,   377,
     378,     0,   379,   380,   381,   382,   383,   384,   385,   360,
       0,   386,   387,   388,     0,     0,   361,     0,   365,     0,
       0,     0,   389,     0,     0,     0,     0,     0,   362,     0,
       0,   828,     0,     0,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,     0,   374,   375,   376,   377,   360,
       0,   379,   380,   381,   382,   383,   361,   385,     0,     0,
     386,   387,   388,     0,     0,     0,     0,     0,   362,     0,
       0,   389,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,     0,     0,     0,   364,     0,   360,
       0,     0,     0,     0,     0,     0,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   365,     0,
       0,     0,     0,   363,     0,     0,     0,   364,     0,     0,
       0,     0,     0,     0,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,     0,   374,   375,   376,   377,   378,
       0,   379,   380,   381,   382,   383,   384,   385,   365,     0,
     386,   387,   388,   363,     0,   360,     0,   364,     0,     0,
       0,   389,   361,     0,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,   362,   374,   375,   376,   377,   378,
       0,   379,   380,   381,   382,   383,   384,   385,   365,     0,
     386,   387,   388,     0,     0,     0,     0,   855,     0,     0,
       0,   389,     0,     0,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,     0,   374,   375,   376,   377,   378,
       0,   379,   380,   381,   382,   383,   384,   385,     0,   363,
     386,   387,   388,   364,     0,   360,     0,   856,     0,     0,
       0,   389,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,   949,     0,     0,     0,     0,
       0,     0,     0,   874,   365,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     366,   367,     0,   368,   369,   370,     0,   371,   372,   373,
       0,   374,   375,   376,   377,   378,     0,   379,   380,   381,
     382,   383,   384,   385,     0,     0,   386,   387,   388,   363,
       0,   360,     0,   364,     0,     0,     0,   389,   361,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     362,   952,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   365,     0,     0,     0,     0,     0,
       0,   360,     0,     0,     0,     0,     0,     0,   361,     0,
     366,   367,     0,   368,   369,   370,     0,   371,   372,   373,
     362,   374,   375,   376,   377,   378,     0,   379,   380,   381,
     382,   383,   384,   385,     0,   363,   386,   387,   388,   364,
       0,   360,  1019,     0,     0,     0,     0,   389,   361,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     362,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     365,     0,     0,     0,     0,   363,     0,     0,     0,   364,
       0,     0,     0,     0,     0,     0,   366,   367,     0,   368,
     369,   370,     0,   371,   372,   373,     0,   374,   375,   376,
     377,   378,     0,   379,   380,   381,   382,   383,   384,   385,
     365,     0,   386,   387,   388,   363,     0,   360,     0,   364,
       0,     0,     0,   389,   361,     0,   366,   367,     0,   368,
     369,   370,     0,   371,   372,   373,   362,   374,   375,   376,
     377,   378,     0,   379,   380,   381,   382,   383,   384,   385,
     365,     0,   386,   387,   388,     0,     0,   360,     0,     0,
       0,   955,     0,   389,   361,     0,   366,   367,     0,   368,
     369,   370,     0,   371,   372,   373,   362,   374,   375,   376,
     377,   378,     0,   379,   380,   381,   382,   383,   384,   385,
       0,   363,   386,   387,   388,   364,     0,     0,   360,  1064,
       0,     0,     0,   389,  1044,   361,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,   365,     0,     0,     0,
       0,   363,     0,     0,     0,   364,     0,     0,     0,     0,
       0,     0,   366,   367,  1047,   368,   369,   370,     0,   371,
     372,   373,     0,   374,   375,   376,   377,   378,     0,   379,
     380,   381,   382,   383,   384,   385,   365,     0,   386,   387,
     388,     0,   363,     0,     0,     0,   364,     0,     0,   389,
       0,     0,   366,   367,     0,   368,   369,   370,     0,   371,
     372,   373,     0,   374,   375,   376,   377,   378,     0,   379,
     380,   381,   382,   383,   384,   385,   360,   365,   386,   387,
     388,     0,     0,   361,     0,     0,     0,     0,     0,   389,
       0,     0,     0,   366,   367,   362,   368,   369,   370,     0,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,   360,     0,   386,
     387,   388,     0,     0,   361,     0,     0,     0,     0,     0,
     389,     0,     0,     0,     0,     0,   362,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     363,     0,     0,     0,   364,     0,     0,   360,     0,     0,
       0,     0,     0,     0,   361,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   362,     0,     0,     0,
       0,     0,     0,     0,     0,   365,     0,     0,     0,     0,
       0,   363,     0,     0,     0,   364,     0,     0,     0,     0,
       0,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,     0,   374,   375,   376,   377,   378,     0,   379,   380,
     381,   382,   383,   384,   385,     0,   365,   386,   387,   388,
       0,   363,     0,   360,  1107,   364,     0,     0,   389,     0,
     361,     0,   366,   367,  1164,   368,   369,   370,     0,   371,
     372,   373,   362,   374,   375,   376,   377,   378,     0,   379,
     380,   381,   382,   383,   384,   385,   365,     0,   386,   387,
     388,     0,     0,   360,     0,     0,     0,  1145,     0,   389,
     361,     0,   366,   367,     0,   368,   369,   370,     0,   371,
     372,   373,   362,   374,   375,   376,   377,   378,     0,   379,
     380,   381,   382,   383,   384,   385,     0,   363,   386,   387,
     388,   364,     0,     0,     0,     0,     0,     0,     0,   389,
    1165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   365,     0,     0,     0,     0,   363,     0,   360,
       0,   364,     0,     0,     0,     0,   361,     0,   366,   367,
    1166,   368,   369,   370,     0,   371,   372,   373,   362,   374,
     375,   376,   377,   378,     0,   379,   380,   381,   382,   383,
     384,   385,   365,     0,   386,   387,   388,     0,     0,   360,
       0,     0,     0,     0,     0,   389,   361,     0,   366,   367,
       0,   368,   369,   370,     0,   371,   372,   373,   362,   374,
     375,   376,   377,   378,     0,   379,   380,   381,   382,   383,
     384,   385,     0,   363,   386,   387,   388,   364,     0,     0,
       0,     0,     0,     0,     0,   389,  1167,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   365,     0,
       0,     0,     0,   363,     0,   360,     0,   364,     0,     0,
       0,     0,   361,     0,   366,   367,  1168,   368,   369,   370,
       0,   371,   372,   373,   362,   374,   375,   376,   377,   378,
       0,   379,   380,   381,   382,   383,   384,   385,   365,     0,
     386,   387,   388,     0,     0,   360,     0,     0,     0,     0,
       0,   389,   361,     0,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,   362,   374,   375,   376,   377,   378,
       0,   379,   380,   381,   382,   383,   384,   385,     0,   363,
     386,   387,   388,   364,     0,     0,     0,     0,     0,     0,
     360,   389,  1169,  1193,     0,     0,     0,   361,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,   365,     0,     0,     0,     0,   363,
       0,     0,     0,   364,     0,     0,     0,     0,     0,     0,
     366,   367,     0,   368,   369,   370,     0,   371,   372,   373,
       0,   374,   375,   376,   377,   378,     0,   379,   380,   381,
     382,   383,   384,   385,   365,     0,   386,   387,   388,     0,
       0,     0,     0,     0,   363,     0,     0,   389,   364,     0,
     366,   367,     0,   368,   369,   370,     0,   371,   372,   373,
       0,   374,   375,   376,   377,   378,     0,   379,   380,   381,
     382,   383,   384,   385,   360,     0,   386,   387,   388,   365,
       0,   361,     0,     0,     0,     0,     0,   389,     0,     0,
       0,     0,     0,   362,     0,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,     0,   374,   375,   376,   377,
     378,     0,   379,   380,   381,   382,   383,   384,   385,   360,
       0,   386,   387,   388,     0,     0,   361,     0,     0,     0,
    1222,     0,   389,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   363,     0,
       0,     0,   364,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   779,     0,     0,     0,   364,     0,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,     0,
     374,   375,   376,   377,   378,     0,   379,   380,   381,   382,
     383,   384,   385,     0,     0,   386,   387,   388,   365,     0,
       0,     0,     0,     0,     0,     0,   389,     0,     0,     0,
       0,     0,     0,     0,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,     0,   374,   375,   376,   377,   378,
       0,   379,   380,   381,   382,   383,   384,   385,     0,     0,
     386,   387,   388,   744,     0,   237,     0,     0,     0,     0,
       0,   389,     0,     0,     0,     0,     0,     0,     0,   238,
     239,     0,   240,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,   242,     0,     0,  -280,     0,
       0,   243,     0,     0,     0,     0,     0,   244,     0,     0,
       0,   245,  -280,  -280,   246,  -280,     0,     0,     0,     0,
    -280,     0,     0,     0,   247,     0,     0,     0,  -280,     0,
     248,   249,     0,     0,  -280,     0,     0,   250,     0,     0,
    -280,     0,     0,     0,  -280,     0,   251,  -280,     0,     0,
       0,     0,     0,     0,     0,   252,   253,  -280,   254,     0,
     255,     0,   256,  -280,  -280,   257,     0,     0,     0,   258,
    -280,     0,   259,     0,     0,   260,     0,     0,     0,  -280,
       0,     0,  -290,     0,     0,     0,     0,     0,  -280,  -280,
       0,  -280,     0,  -280,     0,  -280,  -290,  -290,  -280,  -290,
       0,     0,  -280,     0,  -290,  -280,     0,     0,  -280,     0,
       0,     0,  -290,     0,     0,   237,     0,     0,  -290,     0,
       0,     0,     0,     0,  -290,     0,     0,     0,  -290,   238,
     239,  -290,   240,     0,     0,     0,     0,   241,     0,     0,
       0,  -290,     0,     0,     0,   242,     0,  -290,  -290,     0,
       0,   243,     0,     0,  -290,     0,     0,   244,     0,     0,
       0,   245,     0,  -290,   246,     0,     0,     0,     0,     0,
       0,     0,  -290,  -290,   247,  -290,     0,  -290,     0,  -290,
     248,   249,  -290,     0,     0,     0,  -290,   250,     0,  -290,
       0,     0,  -290,     0,     0,     0,   251,     0,     0,  -281,
       0,     0,     0,     0,     0,   252,   253,     0,   254,     0,
     255,     0,   256,  -281,  -281,   257,  -281,     0,     0,   258,
       0,  -281,   259,     0,     0,   260,     0,     0,     0,  -281,
       0,     0,  -174,     0,     0,  -281,     0,     0,     0,     0,
       0,  -281,     0,     0,     0,  -281,  -174,  -174,  -281,  -174,
       0,     0,     0,     0,  -174,     0,     0,     0,  -281,     0,
       0,     0,  -174,     0,  -281,  -281,     0,     0,  -174,     0,
       0,  -281,     0,     0,  -174,     0,     0,     0,  -174,     0,
    -281,  -174,     0,     0,     0,     0,     0,     0,     0,  -281,
    -281,  -174,  -281,     0,  -281,     0,  -281,  -174,  -174,  -281,
       0,     0,     0,  -281,  -174,     0,  -281,     0,     0,  -281,
       0,     0,     0,  -174,     0,     0,  -170,     0,     0,     0,
       0,     0,  -174,  -174,     0,  -174,     0,  -174,     0,  -174,
    -170,  -170,  -174,  -170,     0,     0,  -174,     0,  -170,  -174,
       0,     0,  -174,     0,     0,     0,  -170,     0,     0,     0,
       0,     0,  -170,     0,     0,     0,     0,     0,  -170,     0,
       0,     0,  -170,     0,     0,  -170,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -170,     0,     0,     0,     0,
       0,  -170,  -170,     0,     0,     0,     0,     0,  -170,     0,
       0,     0,     0,     0,     0,     0,     0,  -170,     0,     0,
       0,     0,     0,     0,     0,     0,  -170,  -170,     0,  -170,
       0,  -170,     0,  -170,     0,     0,  -170,     0,     0,     0,
    -170,     0,     0,  -170,     0,     0,  -170
};

static const yytype_int16 yycheck[] =
{
      12,    63,   161,    67,    16,   464,     2,   511,    20,    19,
     228,   229,   230,   642,    26,   753,     1,   478,    30,    31,
     236,    33,    34,    35,    36,    37,   411,   481,   205,   147,
      32,   474,   739,   738,    32,    22,    48,   630,    50,     1,
     633,   105,    32,    55,   221,    57,   684,    59,    89,    32,
       3,   213,   850,     3,   885,    67,    68,    69,    70,    71,
      72,  1068,   939,   940,    76,  1080,    78,   283,    80,    81,
      20,    72,    47,    85,   236,    68,    88,    89,    32,    91,
      47,    93,   993,    33,    96,    97,    47,    99,   206,   305,
      47,   103,   104,   105,   106,    67,   117,  1131,    47,    47,
      55,   143,    47,   143,    55,     3,    46,     1,     0,   990,
      72,   104,    62,   114,   143,   117,  1183,   137,     1,   337,
     338,   283,   162,  1130,     1,   145,   167,   117,   192,   143,
     172,   103,   104,   105,   486,    47,   765,   122,    87,    87,
     778,   170,   117,   305,    31,   157,   944,   100,   162,  1183,
     100,   172,   114,   312,     3,  1222,   106,   142,   143,   511,
      33,    34,    35,    89,    47,   167,    60,   168,   143,   167,
     182,   111,   615,   185,   186,   187,   143,   167,   190,   191,
     192,    36,   143,    60,   167,   170,   143,  1202,  1069,   143,
      55,   407,   408,  1104,   204,    89,   994,   172,   143,    86,
    1031,   213,   100,    90,   797,   172,   168,   800,   102,   847,
     372,   172,    89,   167,   169,   172,    71,   167,   169,   191,
     192,   115,   284,  1054,   236,  1056,     3,   172,   162,   683,
     217,   143,   105,  1110,  1111,   222,   162,   117,   115,   122,
      89,   167,    60,    20,   231,   407,   408,    18,   142,    53,
     137,   100,    55,   122,   162,   142,    33,   642,   145,   142,
     143,   169,   149,   115,    80,   142,   117,    83,   162,   143,
    1068,   283,   143,   167,   143,   287,   143,    81,   147,   162,
     739,    27,   294,   295,   143,    62,   143,  1118,   504,  1120,
     167,   143,   172,   305,    27,     3,   170,   115,   741,   170,
     143,   170,  1040,   170,    75,   143,   143,   960,    55,   962,
     143,   170,   474,   170,  1021,  1020,  1023,  1022,   143,   143,
     332,   439,    93,   100,   162,    55,   444,   170,   162,   106,
      76,   168,  1130,   329,   168,   347,   348,   170,    87,   110,
     969,    87,   504,    76,   168,   170,   358,   790,   360,   361,
     362,   363,   364,   365,    87,   367,   368,   369,   370,   371,
     106,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   839,   112,
     765,   143,   446,   169,   396,   397,    27,   841,    47,   143,
     402,    55,   143,   405,   406,   407,   408,   409,   410,   411,
    1053,    60,  1055,   396,   397,   169,   122,   171,   170,   402,
     422,   143,   405,   406,   143,   427,   170,   429,   877,   170,
     122,   162,    46,   143,   885,   143,    27,   168,   646,   647,
      89,   147,   650,   651,   446,    76,   448,   449,   170,   168,
      64,   143,   454,   455,   162,   147,    87,   137,   460,    27,
     170,   463,   464,   615,   466,   145,   115,   469,    82,   143,
     143,    55,   474,   122,   169,   106,   628,   217,   170,    47,
     143,   112,   222,    51,   446,    76,   488,   169,   162,   162,
     492,   493,   546,    27,   143,   497,    87,   111,    55,   162,
     363,   364,   504,    55,   143,    73,  1000,  1200,    76,    55,
      78,   719,   720,   162,   143,   106,    50,   519,   167,    87,
     522,   170,   524,   162,   137,  1218,   528,   529,   530,   142,
     532,   533,   145,   162,    55,   497,   149,   122,   106,   522,
      74,   524,    76,   710,   546,    86,   409,   410,    47,    90,
     137,    86,   538,    87,   169,    90,   171,   519,   145,   422,
     522,    60,   524,   148,   162,   150,   151,   529,     8,   154,
     155,   169,  1021,   171,  1023,   143,  1136,   169,   112,   171,
      55,   733,   734,    97,   546,  1036,   449,    80,    81,   741,
      89,   169,   455,   171,   969,    55,   167,   460,   169,   169,
     169,   171,   170,  1054,   753,  1056,    55,    55,   610,   611,
     612,   613,   614,   615,   616,   617,   115,    60,    60,   619,
     620,    55,    55,   122,   626,   167,   628,  1187,  1188,  1189,
    1190,  1191,  1192,    55,   636,   689,   638,   169,   790,   171,
     642,    31,   644,   626,   143,    82,    89,    89,   522,   162,
     524,   993,   161,   153,   162,   162,   162,   162,  1000,   102,
     102,   162,    55,   162,   626,   528,   162,  1118,   167,  1120,
      47,   170,   115,   115,   169,   162,   167,   162,   116,    60,
      85,    85,     8,    60,    48,   687,   688,   689,   169,   169,
     169,   693,   169,   169,   162,   169,    86,   122,   162,    64,
      90,   142,    31,   169,   167,    67,    70,   162,    89,   145,
     693,   122,    89,   170,   143,   162,    80,    81,   169,   162,
     162,   102,    47,    34,   167,   167,   170,   689,   143,   731,
     122,   170,    21,   735,   115,   737,   738,   739,   115,   741,
     170,   143,   170,   167,    34,   122,   172,   137,   611,   172,
     613,   169,   142,   162,   144,   145,   162,    86,   170,   149,
     143,    90,  1104,   765,   162,   162,   143,   157,   162,   172,
     169,   162,    64,   163,   164,   165,   162,   779,   780,   781,
     162,   162,   784,   162,   168,   162,   167,   143,   790,  1131,
     167,   122,    31,   170,   170,  1001,  1002,   170,   168,    87,
      32,   170,    32,  1145,   806,   859,   808,     2,   137,   138,
     167,   813,   141,   142,   816,   144,   145,    12,   172,   167,
     149,   823,    27,   825,    34,   162,   828,   142,   157,    87,
      25,   172,    27,   169,   163,   164,   165,   169,   142,   203,
     162,  1183,    47,   162,   172,   172,    51,    86,   168,  1001,
    1002,    90,   169,   137,   145,   137,   137,   859,  1200,   145,
     145,   225,   226,   227,   228,   229,   230,   869,    73,   871,
     234,    76,   874,    78,   145,   877,  1218,   145,   145,   145,
    1222,    76,    87,   145,   145,   887,   145,   137,   145,   137,
     145,  1040,   167,   167,    34,   102,   137,   859,   137,   263,
     170,   106,   162,   142,   137,   144,   145,   170,   103,   273,
     149,   162,   143,   108,   170,   279,   779,   780,   157,   162,
     162,   170,   162,   472,   162,   161,   165,   161,   172,   731,
     344,    24,   934,   834,   863,   313,    27,   301,   888,  1040,
    1036,   676,   839,   891,  1202,  1160,   948,   949,  1193,   951,
     952,  1186,   954,   955,    36,   319,    47,   628,   489,    30,
      51,    -1,   964,   965,    -1,   967,    -1,   969,    -1,   971,
     972,    -1,    -1,   975,    -1,    -1,    -1,  1029,    -1,    -1,
      -1,    -1,    73,   178,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,   999,  1000,  1001,
    1002,    -1,    -1,    -1,    -1,  1007,    -1,  1009,    -1,    -1,
      -1,  1013,    -1,    -1,    -1,   106,  1070,  1019,  1020,  1021,
    1022,  1023,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   993,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,
      -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   423,
      -1,    12,  1064,    -1,    -1,    -1,    -1,    -1,  1070,   264,
      -1,    -1,    -1,  1075,    25,   270,    27,  1079,    31,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   453,
      -1,    -1,    -1,   457,    -1,   459,    -1,    -1,   462,    -1,
      -1,  1155,    -1,    -1,    -1,    -1,    -1,    -1,  1070,    -1,
      -1,    -1,    -1,  1075,    -1,    -1,    -1,    31,    -1,    -1,
    1122,  1123,  1124,  1125,    -1,    76,    -1,    -1,    -1,    80,
      81,    -1,    -1,    86,   329,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,  1145,    -1,    -1,    -1,  1149,  1150,   344,
      -1,    -1,   103,  1155,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,  1164,  1165,  1166,  1167,  1168,  1169,    -1,  1131,
      -1,    -1,    86,    -1,    -1,    -1,    90,    -1,    -1,    -1,
    1182,  1183,    -1,  1185,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,  1155,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,   156,   157,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,  1218,    -1,    -1,     2,
    1222,  1183,    -1,   137,   138,   420,    -1,   178,   142,    12,
     144,   145,    -1,    -1,    -1,   149,   225,   226,   227,   228,
     229,   230,    25,   157,    27,    -1,    -1,    -1,    -1,   163,
     164,   165,    -1,   448,    -1,    -1,    -1,    -1,    -1,   454,
    1222,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   466,    -1,    -1,    -1,    -1,   640,   641,    -1,    -1,
      -1,    -1,   646,   647,   273,    -1,   650,   651,    -1,    -1,
     279,    -1,    -1,    76,    -1,    22,    -1,    -1,   662,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,    38,   301,   264,    -1,    -1,    43,    -1,    -1,   270,
     103,    -1,    -1,    50,    -1,   108,    -1,    -1,    -1,    -1,
     319,   526,    -1,    -1,    -1,   530,    -1,    -1,     1,    -1,
      -1,    -1,   706,   538,    -1,    -1,    -1,    74,    -1,    76,
      -1,    78,    -1,    80,    -1,    -1,    83,    -1,    85,    22,
      87,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    37,    38,     2,    -1,   329,   106,
      43,    -1,   109,    -1,    -1,   112,    12,    50,    -1,    -1,
      53,    -1,    -1,   344,    -1,   178,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,   607,    76,    -1,    78,    -1,    80,    81,    -1,
      83,    -1,    85,    -1,    87,    -1,    -1,   791,   792,    -1,
      -1,    -1,    -1,    -1,   423,   162,    -1,    -1,    -1,   803,
      -1,   805,    -1,   106,    -1,    -1,   109,    -1,   812,   112,
      76,   815,    -1,     2,   818,    -1,   820,    -1,   822,    -1,
      -1,    -1,    -1,    12,   453,    -1,   661,    -1,   457,   420,
     459,   835,    -1,   462,    -1,    -1,    25,   103,    27,    22,
      -1,   264,   108,    26,    27,    -1,    -1,   270,    -1,    -1,
      31,    -1,    35,    -1,    37,    38,    -1,   448,    -1,   162,
      43,    -1,   866,   454,    -1,   168,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   466,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   718,    -1,    -1,    -1,    76,    -1,   724,
      -1,    74,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,
      83,    -1,    85,    -1,    87,    86,   329,    -1,    -1,    90,
      -1,    -1,   178,    -1,   103,    -1,    -1,    -1,   753,   108,
      -1,   344,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,   526,    -1,    -1,    -1,   530,
      -1,    -1,    -1,    -1,   779,   780,    -1,   538,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,    -1,    -1,    -1,   149,    -1,
      -1,   806,    -1,   808,    -1,    -1,   157,    -1,   813,   162,
      -1,   816,   163,   164,   165,   168,    -1,    31,    -1,   178,
      -1,    -1,   827,    -1,    -1,   830,    -1,   420,   264,    -1,
      -1,  1005,    -1,    -1,   270,    -1,    -1,    -1,    -1,    -1,
      -1,   640,   641,    -1,    -1,    -1,    -1,   646,   647,    -1,
      -1,   650,   651,   858,    -1,   448,    -1,    -1,    -1,    -1,
      -1,   454,    -1,    -1,   869,    -1,   871,    -1,    -1,    -1,
      -1,    -1,    86,   466,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1058,  1059,    -1,    -1,  1062,  1063,
      -1,    -1,    -1,   329,    -1,    -1,    -1,    -1,    -1,    -1,
     661,    -1,    -1,    -1,    -1,   264,  1080,    -1,   344,    -1,
      -1,   270,    -1,    -1,  1088,    -1,  1090,    -1,  1092,    -1,
      -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,   526,    -1,   149,    -1,   530,    -1,    -1,
      -1,    -1,   156,   157,    -1,   538,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,   718,    -1,   964,
     174,    -1,    -1,   724,    -1,    -1,   971,    -1,    -1,    -1,
     329,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     985,    -1,    -1,    -1,   420,   344,    -1,    -1,    -1,    -1,
      -1,    -1,   791,   792,    -1,    -1,    -1,    -1,  1003,    -1,
      -1,    -1,    -1,    -1,   803,    -1,    -1,  1012,    -1,    -1,
      -1,  1016,   448,    -1,    -1,    -1,   815,    -1,   454,   818,
      -1,   820,    22,   822,    -1,    -1,    26,    27,  1202,    -1,
     466,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,   806,    -1,   808,    -1,    -1,
      50,    -1,   813,    -1,    -1,   816,    -1,    -1,    -1,    -1,
    1065,   420,    -1,     2,    -1,    -1,   827,    -1,   661,   830,
      -1,    -1,    -1,    12,    74,    31,    76,    -1,    78,    -1,
      80,    -1,    -1,    83,    -1,    85,    25,    87,    27,   448,
     526,    -1,    -1,    -1,   530,   454,    -1,   858,    -1,    -1,
      -1,    -1,   538,    -1,    -1,    -1,   106,   466,   869,   109,
     871,    -1,   112,    -1,    -1,    -1,    -1,  1122,  1123,  1124,
    1125,    -1,    -1,    -1,    -1,   718,    -1,    -1,    -1,    -1,
      86,   724,    -1,    -1,    90,    -1,    -1,    76,    -1,    -1,
       2,    -1,    -1,    -1,  1149,  1150,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   162,    25,   103,    27,    -1,   526,   168,   108,
      -1,   530,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   538,
      -1,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,   151,    -1,    -1,   154,   155,
     156,   157,   158,   964,   160,    -1,  1005,   163,   164,   165,
     971,    -1,    -1,   806,    76,   808,    -1,    -1,   174,    -1,
     813,    -1,    -1,   816,   985,   661,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   827,    -1,    -1,   830,    -1,   178,
      -1,   103,  1003,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,  1012,    -1,    -1,    -1,  1016,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   858,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   869,    -1,   871,    -1,
      -1,    -1,   718,    -1,    -1,    -1,    -1,    -1,   724,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,
      -1,    -1,   661,    -1,  1065,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,   178,    43,    44,    45,
      50,    -1,    -1,    -1,    -1,   264,    -1,    -1,    -1,    -1,
      -1,   270,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      80,    -1,    -1,    83,    -1,    85,    82,    87,    -1,   718,
      -1,  1122,  1123,  1124,  1125,   724,    -1,    -1,    -1,    -1,
     806,   964,   808,    -1,    -1,    -1,   106,   813,   971,   109,
     816,    -1,   112,    -1,    -1,    -1,    -1,    -1,  1149,  1150,
     329,   827,   985,    -1,   830,    -1,    22,    -1,    -1,    -1,
      26,    27,   264,    -1,    -1,   344,    -1,    -1,   270,    35,
    1003,    37,    38,    -1,   140,    -1,   142,    43,    -1,  1012,
      46,    -1,   858,  1016,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,   162,   869,    -1,   871,    -1,    -1,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   806,    74,   808,
      76,    -1,    -1,    -1,   813,    81,    -1,   816,    -1,    85,
      -1,    87,   188,    -1,    -1,     2,    -1,   329,   827,    -1,
      -1,   830,  1065,    -1,    -1,    12,    -1,    -1,    -1,   205,
     106,   420,   344,   109,     3,   111,   112,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,   221,    -1,   223,    17,   858,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,   448,
     869,    -1,   871,    -1,    -1,   454,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,   466,   964,  1122,
    1123,  1124,  1125,    52,    -1,   971,    -1,    -1,    -1,    76,
     266,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,   985,
      69,    -1,    -1,    -1,    -1,    -1,  1149,  1150,   420,    -1,
      -1,    -1,    -1,    -1,    -1,    84,   103,  1003,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,  1012,    96,    -1,    -1,
    1016,   100,    -1,    -1,    -1,    22,   448,   526,   107,    26,
      27,   530,   454,     2,   113,    -1,    -1,    -1,    35,   538,
      37,    38,    -1,    12,   466,   964,    43,    -1,    -1,    -1,
      -1,    -1,   971,    50,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,   349,    -1,    -1,   985,    -1,    -1,  1065,
      -1,    -1,    -1,     2,    -1,    -1,    -1,    74,    -1,    76,
      -1,   178,    -1,    12,  1003,    -1,    -1,    -1,    85,    -1,
      87,    -1,    -1,  1012,    -1,    -1,    25,  1016,    27,    -1,
      -1,    -1,    -1,    -1,   526,    -1,    -1,    76,   530,   106,
      -1,    -1,   109,    -1,    -1,   112,   538,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1122,  1123,  1124,  1125,
      -1,    -1,    -1,   419,   103,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,  1065,    76,    -1,    -1,
      -1,   437,    -1,  1149,  1150,   441,    -1,    -1,    -1,    -1,
      -1,    -1,   661,    -1,    -1,    -1,    -1,   264,    -1,    -1,
      -1,    -1,    -1,   270,   103,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   475,
      -1,    -1,    -1,    -1,    -1,   481,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1122,  1123,  1124,  1125,    22,    -1,   178,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,   718,
      35,    -1,    37,    38,    -1,   724,    -1,    -1,    43,    -1,
    1149,  1150,   329,    -1,    -1,    50,    -1,    -1,    -1,   661,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   344,    -1,   178,
      -1,    -1,    -1,   539,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,   548,    78,    -1,    80,    -1,    -1,    83,    -1,
      85,    -1,    87,    -1,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
      -1,   106,    -1,    -1,   109,   264,   718,   112,    -1,    -1,
      -1,   270,   724,    -1,    -1,    -1,   153,   806,    -1,   808,
      -1,    -1,    -1,    -1,   813,    -1,    -1,   816,    -1,   166,
      -1,    -1,   169,   420,   171,    -1,    -1,    -1,   827,    -1,
      -1,   830,    -1,    -1,    -1,   264,    -1,    -1,    -1,    -1,
      -1,   270,    -1,   629,    -1,    -1,   632,   162,    -1,    -1,
      -1,   448,    -1,   168,    -1,    -1,    -1,   454,    -1,   858,
     329,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   466,
     869,    -1,   871,    -1,    22,   344,    -1,    -1,    26,    27,
      -1,    -1,    -1,    -1,   806,    -1,   808,    35,    -1,    37,
      38,   813,    -1,   679,   816,    43,    -1,   683,    -1,    -1,
     329,    -1,    50,    -1,    -1,   827,    -1,    -1,   830,    -1,
      -1,    -1,    -1,    -1,    -1,   344,    -1,    -1,    -1,    -1,
     706,    -1,    -1,    -1,   710,    -1,    74,    -1,    76,   526,
      78,    -1,    80,   530,    -1,    83,   858,    85,    -1,    87,
      -1,   538,    -1,    -1,    -1,    -1,    -1,   869,    -1,   871,
      -1,   420,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,   112,   964,    -1,   753,    -1,    -1,
      -1,    -1,   971,    -1,    -1,    -1,    -1,    -1,    -1,   448,
      -1,    -1,    -1,    -1,    -1,   454,   985,    -1,    -1,    -1,
      -1,   420,    -1,    -1,    -1,    -1,    -1,   466,    -1,    -1,
      -1,    -1,    -1,    -1,  1003,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1012,   162,    -1,    -1,  1016,    -1,   448,
     168,    -1,    -1,    -1,    -1,   454,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,    -1,   964,    -1,    -1,    -1,   832,    -1,   834,   971,
      -1,    -1,   838,    -1,    -1,   841,    -1,   526,    -1,    -1,
      -1,   530,    -1,   985,   661,    -1,  1065,    -1,    -1,   538,
     166,    -1,    -1,   169,    -1,   171,    -1,   863,    -1,    -1,
      -1,  1003,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1012,    -1,    -1,    -1,  1016,    -1,    -1,   526,    -1,    -1,
      -1,   530,   888,    -1,    -1,    -1,    -1,    -1,    -1,   538,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   718,    -1,  1122,  1123,  1124,  1125,   724,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1065,    -1,    -1,    -1,    -1,    -1,    -1,
    1149,  1150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      17,    18,   661,    20,    -1,    -1,    -1,    31,    25,    -1,
    1122,  1123,  1124,  1125,    -1,    -1,    33,    -1,    -1,   806,
      -1,   808,    39,    -1,    -1,    -1,   813,    -1,    45,   816,
      -1,    -1,    49,    -1,    -1,    52,    -1,  1149,  1150,    -1,
     827,    -1,   661,   830,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    75,   718,
      -1,    -1,    86,    -1,  1040,   724,    90,    84,    -1,    -1,
      -1,   858,    -1,    -1,    -1,    99,    93,    94,    -1,    96,
      -1,    98,   869,   100,   871,    -1,   103,    -1,    -1,    -1,
     107,   108,    -1,   110,    -1,    -1,   113,   121,    -1,   718,
      -1,    -1,    -1,    -1,    -1,   724,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,    -1,    -1,   163,
     164,   165,    -1,   167,    -1,    -1,    -1,   806,    -1,   808,
     174,    -1,   169,    -1,   813,    -1,    -1,   816,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   827,    -1,
      -1,   830,    -1,    -1,    -1,    -1,    -1,   964,    -1,    -1,
      -1,    -1,    -1,    -1,   971,    -1,    -1,   806,    -1,   808,
      -1,    -1,     3,    -1,   813,    -1,    -1,   816,   985,   858,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,   827,    20,
     869,   830,   871,    -1,    25,    -1,  1003,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,  1012,    -1,    -1,    39,  1016,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,   858,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     869,    62,   871,    -1,    -1,    -1,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,  1065,    -1,
      -1,    -1,    93,    94,    -1,    96,    -1,    98,    -1,   100,
      12,    -1,   103,    -1,    -1,    -1,   107,    19,    -1,   110,
      -1,    -1,   113,    -1,    -1,   964,    -1,    -1,    -1,    31,
      -1,    -1,   971,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   985,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1122,  1123,  1124,  1125,    -1,
      -1,    -1,    -1,    -1,  1003,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1012,    -1,    -1,   167,  1016,   169,    -1,
      -1,    -1,  1149,  1150,    86,    -1,   985,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,  1003,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1012,    -1,    -1,    -1,  1016,    -1,   121,
     122,   123,   124,   125,   126,   127,  1065,    -1,   130,   131,
     132,   133,    -1,   135,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,   163,   164,   165,    -1,   167,  1065,    -1,    -1,    -1,
      -1,    -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1122,  1123,  1124,  1125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,     1,
      -1,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
    1149,  1150,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      -1,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    91,
      92,    93,    94,    95,    96,    97,    98,    -1,   100,   101,
      -1,   103,   104,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,    -1,    -1,   116,    -1,   118,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
     162,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,
      -1,     1,   174,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    19,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    -1,    68,    69,
      70,    -1,    -1,    -1,    74,    75,    76,    77,    -1,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
     100,   101,    -1,   103,   104,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,    -1,    -1,   116,    -1,   118,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,   167,   168,   169,
      -1,   171,    -1,     1,   174,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    -1,
      68,    69,    70,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    91,    92,    93,    94,    95,    96,    97,
      98,    -1,   100,   101,    -1,   103,   104,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,    -1,    -1,   116,    -1,
     118,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,   162,    -1,    -1,   165,    -1,   167,
     168,   169,    -1,   171,    -1,     1,   174,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    -1,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    91,    92,    93,    94,    95,
      96,    97,    98,    -1,   100,   101,    -1,   103,   104,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,    -1,    -1,
     116,    -1,   118,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,   162,    -1,    -1,   165,
      -1,   167,   168,   169,    -1,   171,    -1,     1,   174,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    -1,    68,    69,    70,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    91,    92,    93,
      94,    95,    96,    97,    98,    -1,   100,   101,    -1,   103,
     104,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,    -1,   118,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,   162,    -1,
      -1,   165,    -1,   167,   168,   169,    -1,   171,    -1,     1,
     174,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      -1,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    91,
      92,    93,    94,    95,    96,    97,    98,    -1,   100,   101,
      -1,   103,   104,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,    -1,    -1,   116,    -1,   118,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
     162,    -1,    -1,   165,    -1,   167,   168,   169,    -1,   171,
      -1,     1,   174,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    -1,    68,    69,
      70,    -1,    -1,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
     100,   101,    -1,   103,   104,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,    -1,    -1,   116,    -1,   118,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,   162,    -1,    -1,   165,    -1,   167,   168,   169,
      -1,   171,    -1,     1,   174,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    -1,
      68,    69,    70,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    91,    92,    93,    94,    95,    96,    97,
      98,    -1,   100,   101,    -1,   103,   104,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,    -1,    -1,   116,    -1,
     118,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,   162,    -1,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,     1,   174,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    -1,    -1,    74,    75,
      76,    77,    -1,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    91,    92,    93,    94,    95,
      96,    97,    98,    -1,   100,   101,    -1,   103,   104,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,    -1,    -1,
     116,    -1,   118,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,   162,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,    -1,     1,   174,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    -1,    68,    69,    70,    -1,    -1,    -1,
      74,    75,    76,    77,    -1,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    91,    92,    93,
      94,    95,    96,    97,    98,    -1,   100,   101,    -1,   103,
     104,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,    -1,   118,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,   162,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     1,
     174,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      -1,    -1,    74,    75,    76,    77,    -1,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    91,
      92,    93,    94,    95,    96,    97,    98,    -1,   100,   101,
      -1,   103,   104,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,    -1,    -1,   116,    -1,   118,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
     162,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,
      -1,     1,   174,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    -1,    68,    69,
      70,    -1,    -1,    -1,    74,    75,    76,    77,    -1,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
     100,   101,    -1,   103,   104,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,    -1,    -1,   116,    -1,   118,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,   162,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     1,   174,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    -1,
      68,    69,    70,    -1,    -1,    -1,    74,    75,    76,    77,
      -1,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    91,    92,    93,    94,    95,    96,    97,
      98,    -1,   100,   101,    -1,   103,   104,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,    -1,    -1,   116,    -1,
     118,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,   162,    -1,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,     1,   174,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    -1,    -1,    74,    75,
      76,    77,    -1,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    91,    92,    93,    94,    95,
      96,    97,    98,    -1,   100,   101,    -1,   103,   104,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,    -1,    -1,
     116,    -1,   118,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,   162,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,    -1,     1,   174,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    -1,    68,    69,    70,    -1,    -1,    -1,
      74,    75,    76,    77,    -1,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    91,    92,    93,
      94,    95,    96,    97,    98,    -1,   100,   101,    -1,   103,
     104,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,    -1,   118,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,   162,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     1,
     174,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      -1,    -1,    74,    75,    76,    77,    -1,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    91,
      92,    93,    94,    95,    96,    97,    98,    -1,   100,   101,
      -1,   103,   104,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,    -1,    -1,   116,    -1,   118,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
     162,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,
      -1,     1,   174,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    -1,    68,    69,
      70,    -1,    -1,    -1,    74,    75,    76,    77,    -1,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
     100,   101,    -1,   103,   104,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,    -1,    -1,   116,    -1,   118,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,   162,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     1,   174,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    -1,
      68,    69,    70,    -1,    -1,    -1,    74,    75,    76,    77,
      -1,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    91,    92,    93,    94,    95,    96,    97,
      98,    -1,   100,   101,    -1,   103,   104,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,    -1,    -1,   116,    -1,
     118,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,   162,    -1,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,     1,   174,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,   172,     1,   174,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    87,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,   112,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,
     174,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    97,    98,    -1,   100,    -1,
      -1,   103,   104,   105,    -1,   107,   108,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
      -1,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,
      -1,    -1,   174,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,   104,   105,    -1,   107,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
     170,   171,    -1,    -1,   174,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,   103,   104,   105,    -1,   107,
     108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,
      -1,   169,   170,   171,    -1,     3,   174,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,
      -1,    -1,   110,    -1,   112,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,     3,   174,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,   103,   104,   105,    -1,   107,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,   162,    -1,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,    -1,   174,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    -1,   100,    -1,    -1,   103,   104,   105,
      -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,    -1,     3,   174,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    -1,   100,    -1,    -1,   103,   104,   105,
      -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,    -1,     3,   174,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,
     106,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,    -1,     3,   174,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,    -1,    -1,   174,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
     104,   105,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,
     174,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    97,    98,    -1,   100,    -1,
      -1,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
      -1,    -1,    -1,   165,    -1,   167,   168,   169,    -1,   171,
      -1,    -1,   174,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,   172,    -1,   174,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,   172,     3,   174,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,   119,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,     3,   174,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,    -1,   174,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,    -1,     3,   174,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,
     106,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,    -1,     3,   174,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,   172,     3,   174,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,
      -1,   107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,    -1,    -1,   174,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,
     174,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    97,    98,    -1,   100,    -1,
      -1,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
      -1,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,
      -1,    -1,   174,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,     3,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    12,   146,    -1,    -1,    -1,
      17,    18,    19,    20,    -1,    -1,   156,    -1,    25,   159,
     160,   161,    -1,    -1,    -1,   165,    33,    -1,    35,   169,
      -1,   171,    39,    -1,   174,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    96,
      -1,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,
     127,    -1,    -1,   130,   131,   132,   133,    -1,   135,   136,
     137,   138,   139,   140,   141,    -1,    -1,   144,    -1,    -1,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,   160,    -1,    -1,   163,   164,   165,   166,
      -1,     3,   169,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    97,    98,    -1,   100,    -1,
      -1,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,   160,   161,
      -1,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    97,    98,    -1,   100,    -1,    -1,
     103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
      -1,    -1,    -1,    -1,   167,    -1,   169,   170,   171,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    60,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,   102,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,   139,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,   161,   162,    -1,
      -1,    -1,    -1,   167,    33,   169,    35,   171,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,     3,
     139,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,   161,    -1,    -1,    -1,    -1,    -1,   167,    33,
     169,    35,   171,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,   139,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,   161,    -1,    -1,
      -1,    -1,    -1,   167,    33,   169,    35,   171,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    17,    18,   103,    20,    -1,    -1,   107,    -1,
      25,   110,    -1,    -1,   113,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
     169,    -1,   171,    -1,    -1,    -1,    -1,    -1,    93,    94,
      -1,    96,    -1,    98,    -1,   100,     3,    -1,   103,    -1,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    68,    69,    -1,   169,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    96,
      -1,    98,    -1,   100,    -1,     3,   103,    -1,    -1,    -1,
     107,    -1,    -1,   110,    12,    -1,   113,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,
      68,    69,   169,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    12,    96,    -1,
      98,    -1,   100,    -1,    19,   103,    -1,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,   113,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
      -1,    -1,   130,   131,   132,   133,    -1,   135,   136,   137,
     138,   139,   140,   141,    -1,    -1,   144,    -1,    -1,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,    -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,
      -1,    86,    -1,    12,    -1,    90,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,   122,   123,   124,
     125,   126,   127,    -1,    -1,   130,   131,   132,   133,    -1,
     135,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,    -1,    86,   163,   164,
     165,    90,   167,    -1,    -1,    12,    -1,    -1,    -1,   174,
      99,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,   122,   123,   124,   125,   126,   127,    -1,
      -1,   130,   131,   132,   133,    -1,   135,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,    -1,    -1,   163,   164,   165,    -1,   167,    86,
      -1,    12,    -1,    90,    -1,   174,    -1,    -1,    19,    -1,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,   122,   123,   124,   125,   126,
     127,    -1,    -1,   130,   131,   132,   133,    -1,   135,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,    -1,    86,   163,   164,   165,    90,
     167,    -1,    -1,    12,    -1,    -1,    -1,   174,    99,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,   122,   123,   124,   125,   126,   127,    -1,    -1,   130,
     131,   132,   133,    -1,   135,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
      -1,    -1,   163,   164,   165,    -1,   167,    86,    -1,    12,
      -1,    90,    -1,   174,    -1,    -1,    19,    -1,    -1,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,   122,   123,   124,   125,   126,   127,    -1,
      -1,   130,   131,   132,   133,    -1,   135,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,    -1,    86,   163,   164,   165,    90,   167,    -1,
      -1,    12,    -1,    -1,    -1,   174,    99,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,   122,
     123,   124,   125,   126,   127,    -1,    -1,   130,   131,   132,
     133,    -1,   135,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,    -1,    -1,
     163,   164,   165,    -1,   167,    86,    -1,    12,    -1,    90,
      -1,   174,    -1,    -1,    19,    -1,    -1,    -1,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,   122,   123,   124,   125,   126,   127,    -1,    -1,   130,
     131,   132,   133,    -1,   135,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
      -1,    86,   163,   164,   165,    90,   167,    -1,    -1,    12,
      -1,    -1,    -1,   174,    99,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   121,   122,   123,   124,
     125,   126,   127,    -1,    47,   130,   131,   132,   133,    -1,
     135,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,   163,   164,
     165,    -1,   167,    86,    -1,    12,    -1,    90,    -1,   174,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,   121,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    31,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,    -1,    86,
     163,   164,   165,    90,   167,    12,    -1,    -1,    -1,    -1,
      -1,   174,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    86,
      47,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   121,    -1,   163,   164,   165,    86,
     167,    12,    -1,    90,    -1,    -1,    -1,   174,    19,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      31,   148,   149,   150,   151,    36,    -1,   154,   155,   156,
     157,   158,   159,   160,   121,    -1,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      71,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,    -1,    86,   163,   164,   165,    90,
     167,    -1,    -1,    12,    13,    -1,    -1,   174,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,    71,   154,   155,   156,   157,   158,   159,   160,
      -1,   162,   163,   164,   165,    -1,    -1,    86,    -1,    12,
      -1,    90,    -1,   174,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,    -1,    86,   163,   164,   165,    90,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   174,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,    -1,    -1,
     163,   164,   165,    86,   167,    12,    -1,    90,    -1,    -1,
      -1,   174,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,   121,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,    -1,    86,
     163,   164,   165,    90,   167,    12,    -1,    -1,    -1,    -1,
      -1,   174,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,    -1,    -1,   163,   164,   165,    86,
      -1,    12,    -1,    90,    -1,    -1,    -1,   174,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      31,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,    -1,    86,   163,   164,   165,    90,
     167,    12,    -1,    -1,    -1,    -1,    -1,   174,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    86,    47,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
      -1,    -1,   163,   164,   165,    86,    -1,    12,    -1,    90,
      -1,    -1,    -1,   174,    19,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    31,    32,   149,   150,
     151,    -1,    -1,   154,   155,   156,   157,    -1,    -1,   160,
     121,    -1,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,    -1,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
      -1,    86,   163,   164,   165,    90,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   174,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,   163,   164,
     165,    86,   167,    12,    -1,    90,    -1,    -1,    -1,   174,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    71,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,    -1,    86,   163,   164,
     165,    90,    -1,    12,    -1,    -1,    -1,   172,    -1,   174,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,    -1,    -1,   163,   164,   165,    86,    -1,    12,
      -1,    90,    -1,    -1,    -1,   174,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    31,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,    -1,    86,   163,   164,   165,    90,   167,    12,
      -1,    -1,    -1,    -1,    -1,   174,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   121,   162,
     163,   164,   165,    86,    -1,    -1,    -1,    90,    -1,    -1,
      -1,   174,    -1,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   121,   162,
     163,   164,   165,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   174,    19,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    31,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,    -1,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
     137,   138,    90,   140,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,    -1,   162,   163,   164,   165,    -1,
      -1,    -1,   120,   121,    -1,    -1,    -1,   174,    86,    -1,
      12,    -1,    90,    -1,    -1,    -1,    -1,    19,    -1,   137,
     138,    99,   140,   141,   142,    -1,   144,   145,   146,    31,
     148,   149,   150,   151,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   121,    -1,   163,   164,   165,    -1,    -1,
      12,    13,    -1,    -1,    -1,    -1,   174,    19,    -1,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,    31,
     148,   149,   150,   151,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,    -1,    86,   163,   164,   165,    90,    -1,
      -1,    12,    -1,    -1,    -1,    -1,   174,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   121,
      -1,   163,   164,   165,    -1,    86,    -1,    -1,   170,    90,
      -1,    -1,   174,    -1,    -1,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,    12,
     121,   163,   164,   165,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   174,    -1,    -1,    -1,   137,   138,    31,   140,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      12,    -1,   163,   164,   165,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    90,    -1,    -1,
      12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,    31,   121,
     163,   164,   165,    -1,    86,    -1,    -1,    -1,    90,    -1,
      -1,   174,    -1,    -1,    -1,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   121,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    86,    -1,   137,   138,    90,   140,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,    12,
      -1,   163,   164,   165,    -1,    -1,    19,    -1,   121,    -1,
      -1,    -1,   174,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    34,    -1,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,   151,    12,
      -1,   154,   155,   156,   157,   158,    19,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    90,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   121,    -1,
     163,   164,   165,    86,    -1,    12,    -1,    90,    -1,    -1,
      -1,   174,    19,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    31,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   121,    -1,
     163,   164,   165,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,   174,    -1,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,    -1,    86,
     163,   164,   165,    90,    -1,    12,    -1,   170,    -1,    -1,
      -1,   174,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,    -1,    -1,   163,   164,   165,    86,
      -1,    12,    -1,    90,    -1,    -1,    -1,   174,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      31,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,    -1,    86,   163,   164,   165,    90,
      -1,    12,    13,    -1,    -1,    -1,    -1,   174,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     121,    -1,   163,   164,   165,    86,    -1,    12,    -1,    90,
      -1,    -1,    -1,   174,    19,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     121,    -1,   163,   164,   165,    -1,    -1,    12,    -1,    -1,
      -1,   172,    -1,   174,    19,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
      -1,    86,   163,   164,   165,    90,    -1,    -1,    12,    13,
      -1,    -1,    -1,   174,    99,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,    99,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   121,    -1,   163,   164,
     165,    -1,    86,    -1,    -1,    -1,    90,    -1,    -1,   174,
      -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,    12,   121,   163,   164,
     165,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   174,
      -1,    -1,    -1,   137,   138,    31,   140,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,    12,    -1,   163,
     164,   165,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     174,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    90,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,    -1,   121,   163,   164,   165,
      -1,    86,    -1,    12,   170,    90,    -1,    -1,   174,    -1,
      19,    -1,   137,   138,    99,   140,   141,   142,    -1,   144,
     145,   146,    31,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   121,    -1,   163,   164,
     165,    -1,    -1,    12,    -1,    -1,    -1,   172,    -1,   174,
      19,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    31,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,    -1,    86,   163,   164,
     165,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    86,    -1,    12,
      -1,    90,    -1,    -1,    -1,    -1,    19,    -1,   137,   138,
      99,   140,   141,   142,    -1,   144,   145,   146,    31,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   121,    -1,   163,   164,   165,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   174,    19,    -1,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    31,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,    -1,    86,   163,   164,   165,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    86,    -1,    12,    -1,    90,    -1,    -1,
      -1,    -1,    19,    -1,   137,   138,    99,   140,   141,   142,
      -1,   144,   145,   146,    31,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   121,    -1,
     163,   164,   165,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   174,    19,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    31,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,    -1,    86,
     163,   164,   165,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      12,   174,    99,    60,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   121,    -1,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,   174,    90,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,    12,    -1,   163,   164,   165,   121,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,
      -1,    -1,    -1,    31,    -1,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,    12,
      -1,   163,   164,   165,    -1,    -1,    19,    -1,    -1,    -1,
     172,    -1,   174,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    90,    -1,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,    -1,    -1,   163,   164,   165,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,    -1,    -1,
     163,   164,   165,     1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,     3,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    17,    18,    52,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    62,    -1,    -1,    -1,    33,    -1,
      68,    69,    -1,    -1,    39,    -1,    -1,    75,    -1,    -1,
      45,    -1,    -1,    -1,    49,    -1,    84,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    62,    96,    -1,
      98,    -1,   100,    68,    69,   103,    -1,    -1,    -1,   107,
      75,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    84,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    93,    94,
      -1,    96,    -1,    98,    -1,   100,    17,    18,   103,    20,
      -1,    -1,   107,    -1,    25,   110,    -1,    -1,   113,    -1,
      -1,    -1,    33,    -1,    -1,     3,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    17,
      18,    52,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    62,    -1,    -1,    -1,    33,    -1,    68,    69,    -1,
      -1,    39,    -1,    -1,    75,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    84,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    62,    96,    -1,    98,    -1,   100,
      68,    69,   103,    -1,    -1,    -1,   107,    75,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    84,    -1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    96,    -1,
      98,    -1,   100,    17,    18,   103,    20,    -1,    -1,   107,
      -1,    25,   110,    -1,    -1,   113,    -1,    -1,    -1,    33,
      -1,    -1,     3,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    49,    17,    18,    52,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    33,    -1,    68,    69,    -1,    -1,    39,    -1,
      -1,    75,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,
      84,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    62,    96,    -1,    98,    -1,   100,    68,    69,   103,
      -1,    -1,    -1,   107,    75,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    84,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    93,    94,    -1,    96,    -1,    98,    -1,   100,
      17,    18,   103,    20,    -1,    -1,   107,    -1,    25,   110,
      -1,    -1,   113,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    96,
      -1,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,   113
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   179,   180,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,    35,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    48,
      49,    50,    52,    55,    56,    57,    58,    59,    61,    62,
      63,    65,    66,    68,    69,    70,    74,    75,    76,    77,
      78,    79,    80,    83,    84,    85,    87,    88,    89,    91,
      92,    93,    94,    95,    96,    97,    98,   100,   101,   103,
     104,   105,   106,   107,   109,   110,   112,   113,   116,   118,
     121,   137,   138,   139,   140,   141,   146,   156,   159,   160,
     161,   162,   165,   167,   169,   171,   174,   181,   182,   183,
     184,   185,   186,   189,   191,   197,   198,   199,   202,   203,
     207,   209,   212,   213,   215,   216,   217,   222,   223,   225,
     226,   229,   230,   231,   232,   233,   234,   238,   239,   242,
     243,   246,   247,   250,   253,   254,   279,   282,   283,   303,
     304,   305,   306,   307,   308,   309,   317,   318,   319,   320,
     321,   324,   325,   326,   327,   328,   329,   330,   331,   333,
     334,   335,   336,   337,   162,   183,   321,   117,   310,   311,
       3,   204,    14,    22,    35,    40,    41,    44,    55,    85,
      98,   167,   171,   229,   250,   303,   308,   319,   320,   321,
     324,   326,   327,   310,   321,   106,   282,    87,   204,   183,
     297,   321,   183,   169,     8,    85,   321,   322,     8,    11,
      85,   106,   322,    76,   119,   224,   321,   224,   321,   224,
     321,    26,   283,   321,   321,   190,   169,     3,    17,    18,
      20,    25,    33,    39,    45,    49,    52,    62,    68,    69,
      75,    84,    93,    94,    96,    98,   100,   103,   107,   110,
     113,   206,   208,   206,   206,   251,   169,   206,   284,   285,
      32,   191,   211,   321,   321,    18,    75,    93,   110,   321,
     321,   321,     8,   169,   214,    22,    35,    38,    85,   215,
       4,   273,   296,   321,   104,   105,   162,   321,   323,   321,
     211,   321,   321,   321,    97,   169,   183,   321,   321,   191,
     203,   321,   324,   191,   203,   321,   206,   280,   321,   321,
     321,   321,   321,   321,   321,   321,     1,   168,   181,   192,
     296,   108,   147,   273,   298,   299,   323,   224,   296,   321,
     332,   321,    78,   183,   167,    82,   187,    46,   111,    55,
     206,   206,    53,    81,   268,   284,   161,   162,   153,   321,
      12,    19,    31,    86,    90,   121,   137,   138,   140,   141,
     142,   144,   145,   146,   148,   149,   150,   151,   152,   154,
     155,   156,   157,   158,   159,   160,   163,   164,   165,   174,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   166,   260,   169,   171,    86,
      90,   169,   183,   162,   321,   321,   321,   206,   296,    55,
     167,   191,    47,   310,   280,   284,   162,   143,   162,   116,
     207,   273,   300,   301,   302,   323,    85,   220,   254,   282,
      85,   218,   280,   254,   282,   206,   169,   211,    32,    47,
     211,   117,   211,   313,    32,    47,   211,   313,   211,   313,
      47,   211,   313,    36,    71,   162,    99,   122,   191,   260,
      80,    83,   188,   300,   169,   169,   191,   183,   169,   262,
     108,   169,   206,   286,   287,     1,   142,   291,    47,   143,
     183,   211,   169,   169,   211,   300,   215,   143,   162,   321,
     321,   162,   167,   211,   169,   300,   162,   235,   162,   235,
     162,   122,   281,   162,   211,   162,   168,   168,   181,   143,
     168,   321,   143,   170,   143,   170,   172,   313,    47,   143,
     172,   313,   120,   143,   172,     8,     1,   168,   192,    64,
     200,   201,   321,   194,   321,   206,   142,   244,   167,   255,
     162,   321,   321,   321,   321,   224,   321,   224,   321,   321,
     321,   321,   321,   321,   321,     3,    20,    33,    62,   100,
     106,   207,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,    67,
     323,   323,   323,   323,   323,   300,   300,   224,   321,   224,
     321,    27,    47,    87,   112,   312,   315,   316,   321,   337,
      32,    47,    32,    47,    99,   169,    47,   172,   206,   224,
     321,   211,   162,   162,   321,   321,   122,   170,   143,   221,
     206,   284,   219,   206,   162,   284,    47,   296,    44,   321,
     224,   321,   169,   211,    44,   321,   224,   321,   211,   211,
     224,   321,   211,   165,   193,   196,   321,   193,   195,   183,
     321,    34,   321,   187,   170,   301,   206,   227,   228,    27,
      47,    51,    73,    76,    87,   106,   182,   263,   264,   265,
     266,   252,   287,   143,   170,    33,    49,    94,    98,   171,
     210,   292,   304,   122,   288,   321,   285,   321,   321,   170,
     273,   321,     1,   240,   300,   170,    21,   236,   292,   304,
     143,   168,   170,   170,   298,   170,   298,   183,   172,   224,
     321,   172,   183,   321,   172,   321,   172,   321,   168,   168,
     206,   143,   162,    13,   145,   143,   162,    13,    36,    71,
     162,   169,   296,   167,     1,   206,   248,   249,    27,    76,
      87,   106,   257,   267,   169,   162,   162,   162,   162,   162,
     162,   170,   172,    47,    87,   143,   170,    17,    20,    25,
      45,    52,    62,    69,    84,    96,   107,   113,   303,    86,
      86,    44,   224,   321,    44,   224,   321,   301,   224,   321,
     169,   310,   310,   162,   273,   323,   302,   206,   244,   162,
     206,   244,   162,   321,   170,   321,    32,   211,    32,   211,
     314,   315,   321,    32,   211,   313,    32,   211,   313,   211,
     313,   211,   313,   143,   162,    13,   162,    34,    34,   183,
      99,   191,    64,   170,   143,   170,    47,    87,   265,   143,
     170,   169,   206,    27,    76,    87,   106,   269,   170,   287,
     291,     1,   296,    67,   323,   170,   170,   168,    72,   114,
     168,   241,   170,   169,   191,   206,   237,   280,   183,   172,
     313,   172,   313,   183,   120,   200,   207,   167,   321,   108,
     321,   196,   195,   301,     1,   245,   168,   122,   143,   168,
      87,   256,     1,     3,    12,    17,    19,    20,    25,    39,
      45,    52,    54,    62,    68,    69,    84,    96,   100,   103,
     107,   113,   137,   138,   139,   140,   141,   144,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   160,
     163,   164,   165,   166,   169,   205,   206,   208,   258,   259,
     260,   303,   170,   315,   291,   303,   303,   321,    32,    32,
     321,    32,    32,   170,   172,   172,   301,   211,   211,   244,
     167,   244,   167,   211,    99,    44,   321,    44,   321,   143,
     170,    99,    44,   321,   211,    44,   321,   211,   211,   211,
     321,   321,   183,   321,   183,    34,   206,   162,   228,   191,
     206,   264,   287,   142,   295,    87,   291,   288,   172,    47,
     172,   169,   169,    32,   183,   296,   237,   142,   191,    44,
     183,   321,   172,    44,   183,   321,   172,   321,   193,    13,
      36,    71,    36,    71,   162,   162,   170,   168,    80,    83,
     168,   182,   213,   321,   249,   269,   169,   261,   321,   137,
     145,   261,   261,   288,    99,    44,    44,    99,    44,    44,
      44,    44,   170,   167,   245,   167,   245,   321,   321,   321,
     315,   321,   321,   321,    13,    34,   183,   162,   295,   170,
     171,   210,   273,   294,   304,   147,   274,   288,    60,   115,
     275,   321,   292,   304,   300,   300,   183,   211,   170,   321,
     321,   183,   321,   183,   168,   108,   321,   196,   195,   196,
     195,   162,   213,     1,   142,   290,   263,   170,     3,   100,
     259,   260,   321,   321,   321,   321,   321,   321,   245,   168,
     245,   168,    99,    99,    99,    99,   321,   183,   274,   288,
     295,   172,   296,   273,   321,     3,    89,   100,   276,   277,
     278,   321,   191,   212,   272,   172,   170,   170,   191,    99,
      99,   162,   162,   162,   162,   171,   210,   289,   304,   102,
     270,   170,   261,   261,    99,    99,    99,    99,    99,    99,
     168,   168,   321,   321,   321,   321,   274,   288,   273,   293,
     294,   304,    47,   172,   278,   115,   143,   122,   148,   150,
     151,   154,   155,    60,   304,   161,   161,   321,   321,     1,
     172,   296,   275,   321,   293,   294,   321,   277,   278,   278,
     278,   278,   278,   278,   276,   172,   289,   304,   172,   162,
     271,   272,   172,   289,   304,   293
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   178,   179,   180,   180,   181,   181,   182,   182,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   184,   185,   185,   185,   186,   186,   186,
     187,   187,   188,   188,   188,   190,   189,   191,   191,   191,
     192,   192,   193,   193,   193,   193,   194,   194,   194,   194,
     194,   194,   195,   195,   196,   196,   197,   197,   197,   198,
     198,   198,   198,   198,   198,   198,   199,   200,   200,   200,
     201,   201,   202,   203,   203,   203,   203,   203,   203,   204,
     204,   205,   205,   205,   205,   205,   205,   206,   206,   206,
     206,   206,   206,   207,   207,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   210,   210,   210,   210,
     211,   211,   212,   212,   213,   213,   213,   214,   213,   215,
     215,   215,   215,   215,   215,   215,   216,   216,   216,   216,
     218,   217,   219,   217,   220,   217,   221,   217,   217,   217,
     217,   217,   222,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   224,   225,   225,
     225,   225,   225,   225,   225,   225,   226,   226,   227,   227,
     228,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   230,   230,   230,   230,   230,   230,
     231,   231,   231,   232,   232,   232,   233,   234,   234,   234,
     234,   234,   234,   235,   235,   236,   236,   236,   237,   237,
     238,   239,   239,   240,   240,   241,   241,   241,   242,   242,
     243,   243,   243,   244,   244,   245,   245,   245,   246,   246,
     247,   248,   248,   248,   249,   249,   251,   252,   250,   253,
     253,   253,   255,   256,   254,   257,   257,   257,   257,   257,
     258,   258,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   261,   261,   262,   263,   263,   263,   264,   264,   264,
     264,   264,   264,   265,   265,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   267,   267,   267,   267,   267,   267,
     268,   268,   269,   269,   269,   269,   269,   269,   270,   270,
     271,   271,   272,   272,   273,   274,   274,   274,   275,   275,
     275,   275,   275,   276,   276,   277,   277,   277,   277,   277,
     277,   277,   278,   278,   279,   279,   279,   280,   280,   281,
     281,   281,   282,   282,   282,   282,   282,   283,   283,   284,
     284,   285,   285,   286,   286,   286,   287,   287,   287,   288,
     288,   288,   289,   289,   289,   289,   289,   289,   289,   290,
     290,   290,   290,   290,   291,   291,   291,   291,   291,   292,
     292,   292,   292,   293,   293,   293,   294,   294,   294,   294,
     294,   295,   295,   295,   295,   295,   296,   296,   296,   296,
     297,   297,   298,   298,   298,   299,   299,   300,   300,   301,
     301,   302,   302,   302,   302,   303,   303,   304,   304,   304,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   307,
     308,   309,   309,   309,   309,   309,   309,   309,   309,   310,
     310,   311,   312,   312,   313,   314,   314,   315,   315,   315,
     316,   316,   316,   316,   316,   316,   317,   317,   318,   318,
     318,   318,   318,   319,   319,   319,   319,   319,   320,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   322,   322,   323,   323,
     323,   324,   324,   324,   324,   325,   325,   325,   325,   325,
     326,   326,   326,   327,   327,   327,   327,   327,   328,   328,
     328,   328,   329,   329,   330,   330,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   331,   331,   331,   332,
     332,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   334,   334,   334,   334,   334,   334,
     334,   335,   335,   335,   335,   336,   336,   336,   336,   337,
     337,   337,   337,   337,   337,   337
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     3,     2,
       2,     3,     2,     4,     3,     4,     4,     0,     1,     1,
       0,     1,     0,     1,     1,     0,     7,     2,     3,     3,
       1,     2,     1,     3,     3,     5,     1,     3,     3,     3,
       5,     5,     0,     1,     1,     1,     0,     1,     1,     4,
       6,     8,     8,     6,     8,     8,     4,     1,     3,     5,
       1,     3,     3,     4,     4,     4,     4,     4,     4,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     3,     1,     1,     2,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     3,     5,     5,     2,
       0,     8,     0,     9,     0,     8,     0,     9,     3,     3,
       5,     5,     2,     5,     3,     6,     6,     4,     5,     5,
       3,     3,     6,     5,     6,     5,     6,     3,     4,     3,
       4,     5,     6,     5,     6,     3,     4,     3,     4,     6,
       7,     6,     7,     4,     5,     4,     5,     4,     4,     3,
       6,     5,     6,     5,     8,     7,     6,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       6,     4,     7,     5,     3,     6,     2,     3,     3,     2,
       2,     3,     3,     0,     2,     2,     3,     5,     1,     3,
       3,     5,     5,     0,     2,     3,     2,     3,     6,     6,
       1,     1,     1,     0,     2,     0,     2,     3,     5,     5,
       1,     1,     2,     3,     1,     3,     0,     0,     8,     0,
       1,     1,     0,     0,    10,     3,     3,     5,     5,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     3,     0,     1,     3,     4,     5,     4,
       5,     6,     6,     0,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     0,     1,     1,     2,     1,     1,
       1,     1,     0,     1,     2,     1,     1,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     0,     2,
       2,     4,     4,     1,     3,     3,     3,     3,     3,     3,
       3,     2,     1,     1,     3,     4,     4,     2,     4,     0,
       2,     2,     1,     1,     1,     2,     1,     4,     3,     1,
       3,     3,     5,     1,     1,     3,     1,     2,     3,     0,
       2,     2,     3,     2,     4,     3,     3,     4,     3,     0,
       2,     2,     2,     1,     0,     2,     2,     2,     1,     4,
       4,     6,     3,     0,     1,     1,     3,     4,     3,     4,
       6,     0,     2,     2,     2,     2,     1,     1,     3,     3,
       1,     3,     1,     1,     1,     3,     3,     0,     1,     1,
       3,     3,     3,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     2,     4,     4,     4,     5,
       2,     2,     1,     2,     1,     2,     1,     2,     1,     2,
       1,     1,     6,     6,     4,     9,     9,     7,     6,     6,
       4,     9,     9,     7,     4,     6,     6,     9,     9,     6,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       1,     4,     1,     3,     4,     1,     3,     4,     3,     3,
       1,     1,     2,     1,     2,     1,     1,     3,     1,     2,
       2,     2,     2,     2,     8,     8,     9,     9,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     3,
       3,     3,     2,     2,     2,     1,     0,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       4,     4,     4,     3,     3,     3,     3,     5,     3,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     4,     3,     4,     3,     4,     3,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
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
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, context); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParserContext* context)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (context);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParserContext* context)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp, context);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, ParserContext* context)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , context);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, context); \
} while (0)

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
#ifndef YYINITDEPTH
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
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
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
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
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
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
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
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
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
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, ParserContext* context)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (context);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}



struct yypstate
  {
    /* Number of syntax errors so far.  */
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

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

    YYPTRDIFF_T yystacksize;
    /* Used to determine if this is the first time this instance has
       been used.  */
    int yynew;
  };

/* Initialize the parser data structure.  */
yypstate *
yypstate_new (void)
{
  yypstate *yyps;
  yyps = YY_CAST (yypstate *, malloc (sizeof *yyps));
  if (!yyps)
    return YY_NULLPTR;
  yyps->yynew = 1;
  return yyps;
}

void
yypstate_delete (yypstate *yyps)
{
  if (yyps)
    {
#ifndef yyoverflow
      /* If the stack was reallocated but the parse did not complete, then the
         stack still needs to be freed.  */
      if (!yyps->yynew && yyps->yyss != yyps->yyssa)
        YYSTACK_FREE (yyps->yyss);
#endif
      free (yyps);
    }
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

int
yypush_parse (yypstate *yyps, int yypushed_char, YYSTYPE const *yypushed_val, YYLTYPE *yypushed_loc, ParserContext* context)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
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
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 509 "chapel.ypp"
                                       { yyblock = (yyval.pblockstmt); }
#line 6490 "bison-chapel.cpp"
    break;

  case 3:
#line 514 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6496 "bison-chapel.cpp"
    break;

  case 4:
#line 515 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6502 "bison-chapel.cpp"
    break;

  case 6:
#line 522 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6508 "bison-chapel.cpp"
    break;

  case 7:
#line 527 "chapel.ypp"
                                       { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6514 "bison-chapel.cpp"
    break;

  case 8:
#line 528 "chapel.ypp"
                                       { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6520 "bison-chapel.cpp"
    break;

  case 27:
#line 551 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6526 "bison-chapel.cpp"
    break;

  case 28:
#line 552 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6532 "bison-chapel.cpp"
    break;

  case 29:
#line 553 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6538 "bison-chapel.cpp"
    break;

  case 30:
#line 554 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6544 "bison-chapel.cpp"
    break;

  case 31:
#line 555 "chapel.ypp"
                                         { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6550 "bison-chapel.cpp"
    break;

  case 32:
#line 556 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6556 "bison-chapel.cpp"
    break;

  case 33:
#line 557 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6562 "bison-chapel.cpp"
    break;

  case 34:
#line 558 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6568 "bison-chapel.cpp"
    break;

  case 35:
#line 559 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6574 "bison-chapel.cpp"
    break;

  case 36:
#line 560 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6580 "bison-chapel.cpp"
    break;

  case 37:
#line 561 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6586 "bison-chapel.cpp"
    break;

  case 38:
#line 562 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6592 "bison-chapel.cpp"
    break;

  case 39:
#line 563 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6598 "bison-chapel.cpp"
    break;

  case 40:
#line 564 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6604 "bison-chapel.cpp"
    break;

  case 41:
#line 565 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6610 "bison-chapel.cpp"
    break;

  case 42:
#line 566 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6616 "bison-chapel.cpp"
    break;

  case 43:
#line 571 "chapel.ypp"
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[0].pch), currentModuleType, NULL, yyfilename, (yyvsp[-3].b), (yyvsp[-2].b), (yylsp[-3]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[0].pch);
    }
#line 6630 "bison-chapel.cpp"
    break;

  case 44:
#line 584 "chapel.ypp"
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6639 "bison-chapel.cpp"
    break;

  case 45:
#line 589 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6648 "bison-chapel.cpp"
    break;

  case 46:
#line 594 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6657 "bison-chapel.cpp"
    break;

  case 47:
#line 601 "chapel.ypp"
        { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6663 "bison-chapel.cpp"
    break;

  case 48:
#line 602 "chapel.ypp"
          { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6669 "bison-chapel.cpp"
    break;

  case 49:
#line 603 "chapel.ypp"
           { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6675 "bison-chapel.cpp"
    break;

  case 50:
#line 607 "chapel.ypp"
             { (yyval.b) = false; }
#line 6681 "bison-chapel.cpp"
    break;

  case 51:
#line 608 "chapel.ypp"
             { (yyval.b) = true;  }
#line 6687 "bison-chapel.cpp"
    break;

  case 52:
#line 612 "chapel.ypp"
           { (yyval.b) = false; }
#line 6693 "bison-chapel.cpp"
    break;

  case 53:
#line 613 "chapel.ypp"
           { (yyval.b) = false; }
#line 6699 "bison-chapel.cpp"
    break;

  case 54:
#line 614 "chapel.ypp"
           { (yyval.b) = true; }
#line 6705 "bison-chapel.cpp"
    break;

  case 55:
#line 619 "chapel.ypp"
  {
    (yylsp[0]).comment = context->latestComment;
    context->latestComment = NULL;
  }
#line 6714 "bison-chapel.cpp"
    break;

  case 56:
#line 624 "chapel.ypp"
 {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[-1].pch), (yyvsp[-4].b), (yyvsp[-3].b), (yylsp[-6]).comment);
 }
#line 6722 "bison-chapel.cpp"
    break;

  case 57:
#line 639 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); }
#line 6728 "bison-chapel.cpp"
    break;

  case 58:
#line 640 "chapel.ypp"
                                       { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6734 "bison-chapel.cpp"
    break;

  case 59:
#line 641 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6740 "bison-chapel.cpp"
    break;

  case 60:
#line 646 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6746 "bison-chapel.cpp"
    break;

  case 61:
#line 647 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6752 "bison-chapel.cpp"
    break;

  case 62:
#line 652 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6762 "bison-chapel.cpp"
    break;

  case 63:
#line 657 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6772 "bison-chapel.cpp"
    break;

  case 64:
#line 662 "chapel.ypp"
                                          { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6781 "bison-chapel.cpp"
    break;

  case 65:
#line 666 "chapel.ypp"
                                          { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6790 "bison-chapel.cpp"
    break;

  case 66:
#line 675 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6800 "bison-chapel.cpp"
    break;

  case 67:
#line 680 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6810 "bison-chapel.cpp"
    break;

  case 68:
#line 685 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6820 "bison-chapel.cpp"
    break;

  case 69:
#line 690 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6829 "bison-chapel.cpp"
    break;

  case 70:
#line 694 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6838 "bison-chapel.cpp"
    break;

  case 71:
#line 698 "chapel.ypp"
                                             { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6847 "bison-chapel.cpp"
    break;

  case 72:
#line 706 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6857 "bison-chapel.cpp"
    break;

  case 74:
#line 715 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6867 "bison-chapel.cpp"
    break;

  case 76:
#line 724 "chapel.ypp"
           { (yyval.b) = true; }
#line 6873 "bison-chapel.cpp"
    break;

  case 77:
#line 725 "chapel.ypp"
           { (yyval.b) = false; }
#line 6879 "bison-chapel.cpp"
    break;

  case 78:
#line 726 "chapel.ypp"
           { (yyval.b) = true; }
#line 6885 "bison-chapel.cpp"
    break;

  case 79:
#line 730 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 6891 "bison-chapel.cpp"
    break;

  case 80:
#line 731 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 6897 "bison-chapel.cpp"
    break;

  case 81:
#line 732 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6903 "bison-chapel.cpp"
    break;

  case 82:
#line 733 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6909 "bison-chapel.cpp"
    break;

  case 83:
#line 734 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 6915 "bison-chapel.cpp"
    break;

  case 84:
#line 735 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6921 "bison-chapel.cpp"
    break;

  case 85:
#line 736 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6927 "bison-chapel.cpp"
    break;

  case 86:
#line 740 "chapel.ypp"
                                             { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[-3].b));}
#line 6934 "bison-chapel.cpp"
    break;

  case 87:
#line 745 "chapel.ypp"
       { (yyval.pimportstmt) = buildImportStmt((yyvsp[0].pexpr)); }
#line 6940 "bison-chapel.cpp"
    break;

  case 88:
#line 746 "chapel.ypp"
                     { (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 6946 "bison-chapel.cpp"
    break;

  case 89:
#line 747 "chapel.ypp"
                                   { (yyval.pimportstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist)); }
#line 6952 "bison-chapel.cpp"
    break;

  case 90:
#line 751 "chapel.ypp"
              { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pimportstmt)); }
#line 6958 "bison-chapel.cpp"
    break;

  case 91:
#line 752 "chapel.ypp"
                               { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pimportstmt)); }
#line 6964 "bison-chapel.cpp"
    break;

  case 92:
#line 756 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 6970 "bison-chapel.cpp"
    break;

  case 93:
#line 761 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 6976 "bison-chapel.cpp"
    break;

  case 94:
#line 763 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 6982 "bison-chapel.cpp"
    break;

  case 95:
#line 765 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 6988 "bison-chapel.cpp"
    break;

  case 96:
#line 767 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 6994 "bison-chapel.cpp"
    break;

  case 97:
#line 769 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 7000 "bison-chapel.cpp"
    break;

  case 98:
#line 771 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), new SymExpr(gNoInit), "="); }
#line 7006 "bison-chapel.cpp"
    break;

  case 99:
#line 777 "chapel.ypp"
         { (yyval.pch) = NULL; }
#line 7012 "bison-chapel.cpp"
    break;

  case 100:
#line 778 "chapel.ypp"
         { (yyval.pch) = (yyvsp[0].pch); }
#line 7018 "bison-chapel.cpp"
    break;

  case 101:
#line 782 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7024 "bison-chapel.cpp"
    break;

  case 102:
#line 783 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7030 "bison-chapel.cpp"
    break;

  case 103:
#line 784 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7036 "bison-chapel.cpp"
    break;

  case 104:
#line 785 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7042 "bison-chapel.cpp"
    break;

  case 105:
#line 786 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7048 "bison-chapel.cpp"
    break;

  case 106:
#line 787 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7054 "bison-chapel.cpp"
    break;

  case 107:
#line 790 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7060 "bison-chapel.cpp"
    break;

  case 108:
#line 791 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7066 "bison-chapel.cpp"
    break;

  case 109:
#line 792 "chapel.ypp"
                           { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 7072 "bison-chapel.cpp"
    break;

  case 110:
#line 793 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7078 "bison-chapel.cpp"
    break;

  case 111:
#line 794 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7084 "bison-chapel.cpp"
    break;

  case 112:
#line 795 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7090 "bison-chapel.cpp"
    break;

  case 113:
#line 807 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7096 "bison-chapel.cpp"
    break;

  case 114:
#line 808 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7102 "bison-chapel.cpp"
    break;

  case 115:
#line 819 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 7108 "bison-chapel.cpp"
    break;

  case 116:
#line 820 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 7114 "bison-chapel.cpp"
    break;

  case 117:
#line 821 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 7120 "bison-chapel.cpp"
    break;

  case 118:
#line 822 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 7126 "bison-chapel.cpp"
    break;

  case 119:
#line 823 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 7132 "bison-chapel.cpp"
    break;

  case 120:
#line 824 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 7138 "bison-chapel.cpp"
    break;

  case 121:
#line 825 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 7144 "bison-chapel.cpp"
    break;

  case 122:
#line 826 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 7150 "bison-chapel.cpp"
    break;

  case 123:
#line 827 "chapel.ypp"
             { (yyval.pch) = "sync"; }
#line 7156 "bison-chapel.cpp"
    break;

  case 124:
#line 828 "chapel.ypp"
             { (yyval.pch) = "single"; }
#line 7162 "bison-chapel.cpp"
    break;

  case 125:
#line 829 "chapel.ypp"
             { (yyval.pch) = "owned"; }
#line 7168 "bison-chapel.cpp"
    break;

  case 126:
#line 830 "chapel.ypp"
             { (yyval.pch) = "shared"; }
#line 7174 "bison-chapel.cpp"
    break;

  case 127:
#line 831 "chapel.ypp"
             { (yyval.pch) = "borrowed"; }
#line 7180 "bison-chapel.cpp"
    break;

  case 128:
#line 832 "chapel.ypp"
             { (yyval.pch) = "unmanaged"; }
#line 7186 "bison-chapel.cpp"
    break;

  case 129:
#line 833 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 7192 "bison-chapel.cpp"
    break;

  case 130:
#line 834 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 7198 "bison-chapel.cpp"
    break;

  case 131:
#line 835 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 7204 "bison-chapel.cpp"
    break;

  case 132:
#line 836 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 7210 "bison-chapel.cpp"
    break;

  case 133:
#line 837 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 7216 "bison-chapel.cpp"
    break;

  case 134:
#line 841 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 7222 "bison-chapel.cpp"
    break;

  case 135:
#line 842 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 7228 "bison-chapel.cpp"
    break;

  case 136:
#line 843 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 7234 "bison-chapel.cpp"
    break;

  case 137:
#line 844 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 7240 "bison-chapel.cpp"
    break;

  case 138:
#line 845 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7246 "bison-chapel.cpp"
    break;

  case 139:
#line 846 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7252 "bison-chapel.cpp"
    break;

  case 140:
#line 847 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 7258 "bison-chapel.cpp"
    break;

  case 141:
#line 848 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 7264 "bison-chapel.cpp"
    break;

  case 142:
#line 849 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 7270 "bison-chapel.cpp"
    break;

  case 143:
#line 850 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 7276 "bison-chapel.cpp"
    break;

  case 144:
#line 851 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 7282 "bison-chapel.cpp"
    break;

  case 145:
#line 852 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 7288 "bison-chapel.cpp"
    break;

  case 146:
#line 859 "chapel.ypp"
             { (yyval.pch) = "_syncvar"; }
#line 7294 "bison-chapel.cpp"
    break;

  case 147:
#line 860 "chapel.ypp"
             { (yyval.pch) = "_singlevar"; }
#line 7300 "bison-chapel.cpp"
    break;

  case 148:
#line 861 "chapel.ypp"
             { (yyval.pch) = "_domain"; }
#line 7306 "bison-chapel.cpp"
    break;

  case 149:
#line 862 "chapel.ypp"
             { (yyval.pch) = "_index"; }
#line 7312 "bison-chapel.cpp"
    break;

  case 150:
#line 866 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7318 "bison-chapel.cpp"
    break;

  case 151:
#line 867 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7324 "bison-chapel.cpp"
    break;

  case 152:
#line 871 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 7330 "bison-chapel.cpp"
    break;

  case 153:
#line 872 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 7336 "bison-chapel.cpp"
    break;

  case 154:
#line 876 "chapel.ypp"
                        { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 7342 "bison-chapel.cpp"
    break;

  case 156:
#line 878 "chapel.ypp"
                        { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7348 "bison-chapel.cpp"
    break;

  case 157:
#line 888 "chapel.ypp"
           { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 7354 "bison-chapel.cpp"
    break;

  case 158:
#line 889 "chapel.ypp"
                 { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7360 "bison-chapel.cpp"
    break;

  case 166:
#line 903 "chapel.ypp"
                          { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 7366 "bison-chapel.cpp"
    break;

  case 167:
#line 904 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 7372 "bison-chapel.cpp"
    break;

  case 168:
#line 905 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 7378 "bison-chapel.cpp"
    break;

  case 169:
#line 906 "chapel.ypp"
                            { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 7384 "bison-chapel.cpp"
    break;

  case 170:
#line 911 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7393 "bison-chapel.cpp"
    break;

  case 171:
#line 916 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7407 "bison-chapel.cpp"
    break;

  case 172:
#line 926 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7416 "bison-chapel.cpp"
    break;

  case 173:
#line 931 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7431 "bison-chapel.cpp"
    break;

  case 174:
#line 942 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7440 "bison-chapel.cpp"
    break;

  case 175:
#line 947 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7454 "bison-chapel.cpp"
    break;

  case 176:
#line 957 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7463 "bison-chapel.cpp"
    break;

  case 177:
#line 962 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7477 "bison-chapel.cpp"
    break;

  case 178:
#line 973 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7485 "bison-chapel.cpp"
    break;

  case 179:
#line 977 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7493 "bison-chapel.cpp"
    break;

  case 180:
#line 982 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7505 "bison-chapel.cpp"
    break;

  case 181:
#line 990 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7517 "bison-chapel.cpp"
    break;

  case 182:
#line 1001 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 7525 "bison-chapel.cpp"
    break;

  case 183:
#line 1007 "chapel.ypp"
                                                { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 7531 "bison-chapel.cpp"
    break;

  case 184:
#line 1008 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7537 "bison-chapel.cpp"
    break;

  case 185:
#line 1009 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 7543 "bison-chapel.cpp"
    break;

  case 186:
#line 1010 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 7549 "bison-chapel.cpp"
    break;

  case 187:
#line 1011 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 7555 "bison-chapel.cpp"
    break;

  case 188:
#line 1012 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7561 "bison-chapel.cpp"
    break;

  case 189:
#line 1013 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7567 "bison-chapel.cpp"
    break;

  case 190:
#line 1014 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7573 "bison-chapel.cpp"
    break;

  case 191:
#line 1015 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7579 "bison-chapel.cpp"
    break;

  case 192:
#line 1016 "chapel.ypp"
                                                { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7585 "bison-chapel.cpp"
    break;

  case 193:
#line 1017 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, FORALL_LOOP); }
#line 7591 "bison-chapel.cpp"
    break;

  case 194:
#line 1018 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, FORALL_LOOP); }
#line 7597 "bison-chapel.cpp"
    break;

  case 195:
#line 1019 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  FORALL_LOOP); }
#line 7603 "bison-chapel.cpp"
    break;

  case 196:
#line 1020 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  FORALL_LOOP); }
#line 7609 "bison-chapel.cpp"
    break;

  case 197:
#line 1021 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, FORALL_LOOP); }
#line 7615 "bison-chapel.cpp"
    break;

  case 198:
#line 1022 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, FORALL_LOOP); }
#line 7621 "bison-chapel.cpp"
    break;

  case 199:
#line 1023 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  FORALL_LOOP); }
#line 7627 "bison-chapel.cpp"
    break;

  case 200:
#line 1024 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  FORALL_LOOP); }
#line 7633 "bison-chapel.cpp"
    break;

  case 201:
#line 1025 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, FOREACH_LOOP); }
#line 7639 "bison-chapel.cpp"
    break;

  case 202:
#line 1026 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, FOREACH_LOOP); }
#line 7645 "bison-chapel.cpp"
    break;

  case 203:
#line 1027 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  FOREACH_LOOP); }
#line 7651 "bison-chapel.cpp"
    break;

  case 204:
#line 1028 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  FOREACH_LOOP); }
#line 7657 "bison-chapel.cpp"
    break;

  case 205:
#line 1029 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, FOREACH_LOOP); }
#line 7663 "bison-chapel.cpp"
    break;

  case 206:
#line 1030 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, FOREACH_LOOP); }
#line 7669 "bison-chapel.cpp"
    break;

  case 207:
#line 1031 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  FOREACH_LOOP); }
#line 7675 "bison-chapel.cpp"
    break;

  case 208:
#line 1032 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  FOREACH_LOOP); }
#line 7681 "bison-chapel.cpp"
    break;

  case 209:
#line 1034 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, BRACKET_LOOP);
    }
#line 7691 "bison-chapel.cpp"
    break;

  case 210:
#line 1040 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, BRACKET_LOOP);
    }
#line 7701 "bison-chapel.cpp"
    break;

  case 211:
#line 1046 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  BRACKET_LOOP);
    }
#line 7711 "bison-chapel.cpp"
    break;

  case 212:
#line 1052 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  BRACKET_LOOP);
    }
#line 7721 "bison-chapel.cpp"
    break;

  case 213:
#line 1058 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new
        CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, BRACKET_LOOP);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, BRACKET_LOOP);
    }
#line 7733 "bison-chapel.cpp"
    break;

  case 214:
#line 1066 "chapel.ypp"
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new
        CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, BRACKET_LOOP);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, BRACKET_LOOP);
    }
#line 7745 "bison-chapel.cpp"
    break;

  case 215:
#line 1074 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true, BRACKET_LOOP);
    }
#line 7753 "bison-chapel.cpp"
    break;

  case 216:
#line 1078 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true, BRACKET_LOOP);
    }
#line 7761 "bison-chapel.cpp"
    break;

  case 217:
#line 1084 "chapel.ypp"
                            { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 7767 "bison-chapel.cpp"
    break;

  case 218:
#line 1088 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7773 "bison-chapel.cpp"
    break;

  case 219:
#line 1089 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7779 "bison-chapel.cpp"
    break;

  case 220:
#line 1090 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7785 "bison-chapel.cpp"
    break;

  case 221:
#line 1091 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7791 "bison-chapel.cpp"
    break;

  case 222:
#line 1092 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7798 "bison-chapel.cpp"
    break;

  case 223:
#line 1094 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7805 "bison-chapel.cpp"
    break;

  case 224:
#line 1096 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7812 "bison-chapel.cpp"
    break;

  case 225:
#line 1098 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7819 "bison-chapel.cpp"
    break;

  case 226:
#line 1104 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), (yyvsp[0].pblockstmt))); }
#line 7825 "bison-chapel.cpp"
    break;

  case 227:
#line 1106 "chapel.ypp"
  { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-1].pch), ls, (yyvsp[0].pblockstmt))); }
#line 7834 "bison-chapel.cpp"
    break;

  case 228:
#line 1113 "chapel.ypp"
                                  { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 7840 "bison-chapel.cpp"
    break;

  case 229:
#line 1114 "chapel.ypp"
                                  { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 7846 "bison-chapel.cpp"
    break;

  case 230:
#line 1119 "chapel.ypp"
             { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[0].pch), INTENT_TYPE); }
#line 7852 "bison-chapel.cpp"
    break;

  case 231:
#line 1124 "chapel.ypp"
             { (yyval.pch) = (yyvsp[0].pch); }
#line 7858 "bison-chapel.cpp"
    break;

  case 232:
#line 1125 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 7864 "bison-chapel.cpp"
    break;

  case 233:
#line 1126 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 7870 "bison-chapel.cpp"
    break;

  case 234:
#line 1127 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 7876 "bison-chapel.cpp"
    break;

  case 235:
#line 1128 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 7882 "bison-chapel.cpp"
    break;

  case 236:
#line 1129 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 7888 "bison-chapel.cpp"
    break;

  case 237:
#line 1130 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 7894 "bison-chapel.cpp"
    break;

  case 238:
#line 1131 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 7900 "bison-chapel.cpp"
    break;

  case 239:
#line 1132 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 7906 "bison-chapel.cpp"
    break;

  case 240:
#line 1133 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 7912 "bison-chapel.cpp"
    break;

  case 241:
#line 1134 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 7918 "bison-chapel.cpp"
    break;

  case 242:
#line 1135 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 7924 "bison-chapel.cpp"
    break;

  case 243:
#line 1137 "chapel.ypp"
  { (yyval.pch) = (yyvsp[0].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[0].pch)); }
#line 7931 "bison-chapel.cpp"
    break;

  case 244:
#line 1143 "chapel.ypp"
             { (yyval.pch) = "none"; }
#line 7937 "bison-chapel.cpp"
    break;

  case 245:
#line 1144 "chapel.ypp"
             { (yyval.pch) = "this"; }
#line 7943 "bison-chapel.cpp"
    break;

  case 246:
#line 1145 "chapel.ypp"
             { (yyval.pch) = "false"; }
#line 7949 "bison-chapel.cpp"
    break;

  case 247:
#line 1146 "chapel.ypp"
             { (yyval.pch) = "true"; }
#line 7955 "bison-chapel.cpp"
    break;

  case 248:
#line 1155 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 7961 "bison-chapel.cpp"
    break;

  case 249:
#line 1156 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 7967 "bison-chapel.cpp"
    break;

  case 250:
#line 1161 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 7973 "bison-chapel.cpp"
    break;

  case 251:
#line 1163 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-3].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-1].pch), act, NULL)); }
#line 7980 "bison-chapel.cpp"
    break;

  case 252:
#line 1166 "chapel.ypp"
  { (yyvsp[-2].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-6].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 7987 "bison-chapel.cpp"
    break;

  case 253:
#line 1172 "chapel.ypp"
  { (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 7993 "bison-chapel.cpp"
    break;

  case 254:
#line 1174 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-2].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[0].pch), act); }
#line 8000 "bison-chapel.cpp"
    break;

  case 255:
#line 1177 "chapel.ypp"
  { (yyvsp[-1].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-5].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8007 "bison-chapel.cpp"
    break;

  case 256:
#line 1182 "chapel.ypp"
                          { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 8013 "bison-chapel.cpp"
    break;

  case 257:
#line 1185 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 8019 "bison-chapel.cpp"
    break;

  case 258:
#line 1186 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 8025 "bison-chapel.cpp"
    break;

  case 259:
#line 1187 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 8031 "bison-chapel.cpp"
    break;

  case 260:
#line 1188 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 8037 "bison-chapel.cpp"
    break;

  case 261:
#line 1189 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8043 "bison-chapel.cpp"
    break;

  case 262:
#line 1190 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8049 "bison-chapel.cpp"
    break;

  case 263:
#line 1194 "chapel.ypp"
                           { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8055 "bison-chapel.cpp"
    break;

  case 264:
#line 1195 "chapel.ypp"
                           { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8061 "bison-chapel.cpp"
    break;

  case 265:
#line 1199 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 8067 "bison-chapel.cpp"
    break;

  case 266:
#line 1200 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8073 "bison-chapel.cpp"
    break;

  case 267:
#line 1201 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8079 "bison-chapel.cpp"
    break;

  case 268:
#line 1205 "chapel.ypp"
                      { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 8085 "bison-chapel.cpp"
    break;

  case 269:
#line 1206 "chapel.ypp"
                        { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 8091 "bison-chapel.cpp"
    break;

  case 270:
#line 1210 "chapel.ypp"
                    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 8097 "bison-chapel.cpp"
    break;

  case 271:
#line 1214 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 8103 "bison-chapel.cpp"
    break;

  case 272:
#line 1216 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 8109 "bison-chapel.cpp"
    break;

  case 273:
#line 1220 "chapel.ypp"
                          { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8115 "bison-chapel.cpp"
    break;

  case 274:
#line 1221 "chapel.ypp"
                          { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8121 "bison-chapel.cpp"
    break;

  case 275:
#line 1226 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 8127 "bison-chapel.cpp"
    break;

  case 276:
#line 1228 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8133 "bison-chapel.cpp"
    break;

  case 277:
#line 1230 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8139 "bison-chapel.cpp"
    break;

  case 278:
#line 1237 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8153 "bison-chapel.cpp"
    break;

  case 279:
#line 1247 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8167 "bison-chapel.cpp"
    break;

  case 280:
#line 1260 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8177 "bison-chapel.cpp"
    break;

  case 281:
#line 1265 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8187 "bison-chapel.cpp"
    break;

  case 282:
#line 1270 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8197 "bison-chapel.cpp"
    break;

  case 283:
#line 1278 "chapel.ypp"
                  { (yyval.pcallexpr) = NULL; }
#line 8203 "bison-chapel.cpp"
    break;

  case 284:
#line 1279 "chapel.ypp"
                  { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 8209 "bison-chapel.cpp"
    break;

  case 285:
#line 1284 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 8215 "bison-chapel.cpp"
    break;

  case 286:
#line 1286 "chapel.ypp"
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 8221 "bison-chapel.cpp"
    break;

  case 287:
#line 1288 "chapel.ypp"
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 8227 "bison-chapel.cpp"
    break;

  case 288:
#line 1293 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[-4].penumtype);
      for_vector(DefExpr, ec, *(yyvsp[-1].pvecOfDefs)) {
        ec->sym->type = pdt;
        pdt->constants.insertAtTail(ec);
        if (pdt->defaultValue == NULL) {
          pdt->defaultValue = ec->sym;
        }
      }
      delete (yyvsp[-1].pvecOfDefs);
      pdt->doc = (yylsp[-4]).comment;
      TypeSymbol* pst = new TypeSymbol((yyvsp[-3].pch), pdt);
      (yyvsp[-4].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
#line 8247 "bison-chapel.cpp"
    break;

  case 289:
#line 1309 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 8255 "bison-chapel.cpp"
    break;

  case 290:
#line 1316 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 8265 "bison-chapel.cpp"
    break;

  case 291:
#line 1325 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 8277 "bison-chapel.cpp"
    break;

  case 292:
#line 1333 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 8285 "bison-chapel.cpp"
    break;

  case 293:
#line 1337 "chapel.ypp"
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 8293 "bison-chapel.cpp"
    break;

  case 294:
#line 1343 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 8299 "bison-chapel.cpp"
    break;

  case 295:
#line 1344 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 8305 "bison-chapel.cpp"
    break;

  case 296:
#line 1349 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 8314 "bison-chapel.cpp"
    break;

  case 297:
#line 1354 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8323 "bison-chapel.cpp"
    break;

  case 298:
#line 1359 "chapel.ypp"
    {
      (yyvsp[-5].pfnsymbol)->retTag = (yyvsp[-3].retTag);
      if ((yyvsp[-3].retTag) == RET_REF || (yyvsp[-3].retTag) == RET_CONST_REF)
        USR_FATAL("'ref' return types are not allowed in lambdas");
      if ((yyvsp[-3].retTag) == RET_PARAM)
        USR_FATAL("'param' return types are not allowed in lambdas");
      if ((yyvsp[-3].retTag) == RET_TYPE)
        USR_FATAL("'type' return types are not allowed in lambdas");
      if ((yyvsp[-2].pexpr))
        (yyvsp[-5].pfnsymbol)->retExprType = new BlockStmt((yyvsp[-2].pexpr), BLOCK_SCOPELESS);
      if ((yyvsp[-1].lifetimeAndWhere).where)
        (yyvsp[-5].pfnsymbol)->where = new BlockStmt((yyvsp[-1].lifetimeAndWhere).where);
      if ((yyvsp[-1].lifetimeAndWhere).lifetime)
        (yyvsp[-5].pfnsymbol)->lifetimeConstraints = new BlockStmt((yyvsp[-1].lifetimeAndWhere).lifetime);
      (yyvsp[-5].pfnsymbol)->insertAtTail((yyvsp[0].pblockstmt));
      (yyval.pexpr) = new DefExpr(buildLambda((yyvsp[-5].pfnsymbol)));
    }
#line 8345 "bison-chapel.cpp"
    break;

  case 299:
#line 1381 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 8353 "bison-chapel.cpp"
    break;

  case 300:
#line 1384 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 8362 "bison-chapel.cpp"
    break;

  case 301:
#line 1388 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 8371 "bison-chapel.cpp"
    break;

  case 302:
#line 1396 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 8385 "bison-chapel.cpp"
    break;

  case 303:
#line 1406 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8396 "bison-chapel.cpp"
    break;

  case 304:
#line 1413 "chapel.ypp"
    {
      FnSymbol* fn = (yyvsp[-6].pfnsymbol);
      FnSymbol* linkageFn = (yyvsp[-9].pfnsymbol);

      fn->copyFlags((yyvsp[-9].pfnsymbol));
      if (*linkageFn->name)
        // The user explicitly named this function (controls mangling).
        fn->cname = linkageFn->name;
      else if (linkageFn->numFormals() == 1)
        // cname should be set based upon param
        fn->insertFormalAtTail(linkageFn->getFormal(1));

      if ((yyvsp[-8].procIter) == ProcIter_ITER)
      {
        if (fn->hasFlag(FLAG_EXTERN))
          USR_FATAL_CONT(fn, "'iter' is not legal with 'extern'");
        fn->addFlag(FLAG_ITERATOR_FN);
      }

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[-6].pfnsymbol), (yyvsp[-4].retTag), (yyvsp[-3].pexpr), (yyvsp[-2].b), (yyvsp[-1].lifetimeAndWhere).where, (yyvsp[-1].lifetimeAndWhere).lifetime, (yyvsp[0].pblockstmt), (yylsp[-8]).comment);
      context->latestComment = NULL;
    }
#line 8423 "bison-chapel.cpp"
    break;

  case 305:
#line 1439 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 8431 "bison-chapel.cpp"
    break;

  case 306:
#line 1443 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8440 "bison-chapel.cpp"
    break;

  case 307:
#line 1448 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 8448 "bison-chapel.cpp"
    break;

  case 308:
#line 1452 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8457 "bison-chapel.cpp"
    break;

  case 309:
#line 1457 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 8465 "bison-chapel.cpp"
    break;

  case 311:
#line 1464 "chapel.ypp"
                      { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8471 "bison-chapel.cpp"
    break;

  case 312:
#line 1468 "chapel.ypp"
                  { (yyval.pch) = (yyvsp[0].pch); }
#line 8477 "bison-chapel.cpp"
    break;

  case 313:
#line 1469 "chapel.ypp"
                 { (yyval.pch) = "&"; }
#line 8483 "bison-chapel.cpp"
    break;

  case 314:
#line 1470 "chapel.ypp"
                 { (yyval.pch) = "|"; }
#line 8489 "bison-chapel.cpp"
    break;

  case 315:
#line 1471 "chapel.ypp"
                 { (yyval.pch) = "^"; }
#line 8495 "bison-chapel.cpp"
    break;

  case 316:
#line 1472 "chapel.ypp"
                 { (yyval.pch) = "~"; }
#line 8501 "bison-chapel.cpp"
    break;

  case 317:
#line 1473 "chapel.ypp"
                 { (yyval.pch) = "=="; }
#line 8507 "bison-chapel.cpp"
    break;

  case 318:
#line 1474 "chapel.ypp"
                 { (yyval.pch) = "!="; }
#line 8513 "bison-chapel.cpp"
    break;

  case 319:
#line 1475 "chapel.ypp"
                 { (yyval.pch) = "<="; }
#line 8519 "bison-chapel.cpp"
    break;

  case 320:
#line 1476 "chapel.ypp"
                 { (yyval.pch) = ">="; }
#line 8525 "bison-chapel.cpp"
    break;

  case 321:
#line 1477 "chapel.ypp"
                 { (yyval.pch) = "<"; }
#line 8531 "bison-chapel.cpp"
    break;

  case 322:
#line 1478 "chapel.ypp"
                 { (yyval.pch) = ">"; }
#line 8537 "bison-chapel.cpp"
    break;

  case 323:
#line 1479 "chapel.ypp"
                 { (yyval.pch) = "+"; }
#line 8543 "bison-chapel.cpp"
    break;

  case 324:
#line 1480 "chapel.ypp"
                 { (yyval.pch) = "-"; }
#line 8549 "bison-chapel.cpp"
    break;

  case 325:
#line 1481 "chapel.ypp"
                 { (yyval.pch) = "*"; }
#line 8555 "bison-chapel.cpp"
    break;

  case 326:
#line 1482 "chapel.ypp"
                 { (yyval.pch) = "/"; }
#line 8561 "bison-chapel.cpp"
    break;

  case 327:
#line 1483 "chapel.ypp"
                 { (yyval.pch) = "<<"; }
#line 8567 "bison-chapel.cpp"
    break;

  case 328:
#line 1484 "chapel.ypp"
                 { (yyval.pch) = ">>"; }
#line 8573 "bison-chapel.cpp"
    break;

  case 329:
#line 1485 "chapel.ypp"
                 { (yyval.pch) = "%"; }
#line 8579 "bison-chapel.cpp"
    break;

  case 330:
#line 1486 "chapel.ypp"
                 { (yyval.pch) = "**"; }
#line 8585 "bison-chapel.cpp"
    break;

  case 331:
#line 1487 "chapel.ypp"
                 { (yyval.pch) = "!"; }
#line 8591 "bison-chapel.cpp"
    break;

  case 332:
#line 1488 "chapel.ypp"
                 { (yyval.pch) = "chpl_by"; }
#line 8597 "bison-chapel.cpp"
    break;

  case 333:
#line 1489 "chapel.ypp"
                 { (yyval.pch) = "#"; }
#line 8603 "bison-chapel.cpp"
    break;

  case 334:
#line 1490 "chapel.ypp"
                 { (yyval.pch) = "chpl_align"; }
#line 8609 "bison-chapel.cpp"
    break;

  case 335:
#line 1491 "chapel.ypp"
                 { (yyval.pch) = "<=>"; }
#line 8615 "bison-chapel.cpp"
    break;

  case 336:
#line 1492 "chapel.ypp"
                 { (yyval.pch) = "<~>"; }
#line 8621 "bison-chapel.cpp"
    break;

  case 337:
#line 1493 "chapel.ypp"
                 { (yyval.pch) = "init="; }
#line 8627 "bison-chapel.cpp"
    break;

  case 338:
#line 1494 "chapel.ypp"
                  { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 8633 "bison-chapel.cpp"
    break;

  case 339:
#line 1498 "chapel.ypp"
                 { (yyval.pch) = "="; }
#line 8639 "bison-chapel.cpp"
    break;

  case 340:
#line 1499 "chapel.ypp"
                 { (yyval.pch) = "+="; }
#line 8645 "bison-chapel.cpp"
    break;

  case 341:
#line 1500 "chapel.ypp"
                 { (yyval.pch) = "-="; }
#line 8651 "bison-chapel.cpp"
    break;

  case 342:
#line 1501 "chapel.ypp"
                  { (yyval.pch) = "*="; }
#line 8657 "bison-chapel.cpp"
    break;

  case 343:
#line 1502 "chapel.ypp"
                 { (yyval.pch) = "/="; }
#line 8663 "bison-chapel.cpp"
    break;

  case 344:
#line 1503 "chapel.ypp"
                 { (yyval.pch) = "%="; }
#line 8669 "bison-chapel.cpp"
    break;

  case 345:
#line 1504 "chapel.ypp"
                 { (yyval.pch) = "**="; }
#line 8675 "bison-chapel.cpp"
    break;

  case 346:
#line 1505 "chapel.ypp"
                 { (yyval.pch) = "&="; }
#line 8681 "bison-chapel.cpp"
    break;

  case 347:
#line 1506 "chapel.ypp"
                 { (yyval.pch) = "|="; }
#line 8687 "bison-chapel.cpp"
    break;

  case 348:
#line 1507 "chapel.ypp"
                 { (yyval.pch) = "^="; }
#line 8693 "bison-chapel.cpp"
    break;

  case 349:
#line 1508 "chapel.ypp"
                 { (yyval.pch) = ">>="; }
#line 8699 "bison-chapel.cpp"
    break;

  case 350:
#line 1509 "chapel.ypp"
                 { (yyval.pch) = "<<="; }
#line 8705 "bison-chapel.cpp"
    break;

  case 351:
#line 1513 "chapel.ypp"
                     { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 8711 "bison-chapel.cpp"
    break;

  case 352:
#line 1514 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8717 "bison-chapel.cpp"
    break;

  case 353:
#line 1518 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8723 "bison-chapel.cpp"
    break;

  case 354:
#line 1522 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 8729 "bison-chapel.cpp"
    break;

  case 355:
#line 1523 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 8735 "bison-chapel.cpp"
    break;

  case 356:
#line 1524 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 8741 "bison-chapel.cpp"
    break;

  case 357:
#line 1529 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 8747 "bison-chapel.cpp"
    break;

  case 358:
#line 1531 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 8753 "bison-chapel.cpp"
    break;

  case 359:
#line 1533 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 8759 "bison-chapel.cpp"
    break;

  case 360:
#line 1535 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr))); }
#line 8765 "bison-chapel.cpp"
    break;

  case 361:
#line 1537 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8771 "bison-chapel.cpp"
    break;

  case 362:
#line 1539 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 8777 "bison-chapel.cpp"
    break;

  case 363:
#line 1543 "chapel.ypp"
                      { (yyval.pt) = INTENT_BLANK; }
#line 8783 "bison-chapel.cpp"
    break;

  case 364:
#line 1544 "chapel.ypp"
                      { (yyval.pt) = (yyvsp[0].pt); }
#line 8789 "bison-chapel.cpp"
    break;

  case 365:
#line 1548 "chapel.ypp"
          { (yyval.pt) = INTENT_IN; }
#line 8795 "bison-chapel.cpp"
    break;

  case 366:
#line 1549 "chapel.ypp"
          { (yyval.pt) = INTENT_INOUT; }
#line 8801 "bison-chapel.cpp"
    break;

  case 367:
#line 1550 "chapel.ypp"
          { (yyval.pt) = INTENT_OUT; }
#line 8807 "bison-chapel.cpp"
    break;

  case 368:
#line 1551 "chapel.ypp"
          { (yyval.pt) = INTENT_CONST; }
#line 8813 "bison-chapel.cpp"
    break;

  case 369:
#line 1552 "chapel.ypp"
             { (yyval.pt) = INTENT_CONST_IN; }
#line 8819 "bison-chapel.cpp"
    break;

  case 370:
#line 1553 "chapel.ypp"
              { (yyval.pt) = INTENT_CONST_REF; }
#line 8825 "bison-chapel.cpp"
    break;

  case 371:
#line 1554 "chapel.ypp"
          { (yyval.pt) = INTENT_PARAM; }
#line 8831 "bison-chapel.cpp"
    break;

  case 372:
#line 1555 "chapel.ypp"
          { (yyval.pt) = INTENT_REF; }
#line 8837 "bison-chapel.cpp"
    break;

  case 373:
#line 1556 "chapel.ypp"
          { (yyval.pt) = INTENT_TYPE; }
#line 8843 "bison-chapel.cpp"
    break;

  case 374:
#line 1560 "chapel.ypp"
         { (yyval.pt) = INTENT_BLANK; }
#line 8849 "bison-chapel.cpp"
    break;

  case 375:
#line 1561 "chapel.ypp"
         { (yyval.pt) = INTENT_PARAM; }
#line 8855 "bison-chapel.cpp"
    break;

  case 376:
#line 1562 "chapel.ypp"
         { (yyval.pt) = INTENT_REF;   }
#line 8861 "bison-chapel.cpp"
    break;

  case 377:
#line 1563 "chapel.ypp"
                { (yyval.pt) = INTENT_CONST_REF;   }
#line 8867 "bison-chapel.cpp"
    break;

  case 378:
#line 1564 "chapel.ypp"
         { (yyval.pt) = INTENT_CONST;   }
#line 8873 "bison-chapel.cpp"
    break;

  case 379:
#line 1565 "chapel.ypp"
         { (yyval.pt) = INTENT_TYPE;  }
#line 8879 "bison-chapel.cpp"
    break;

  case 380:
#line 1569 "chapel.ypp"
         { (yyval.procIter) = ProcIter_PROC; }
#line 8885 "bison-chapel.cpp"
    break;

  case 381:
#line 1570 "chapel.ypp"
         { (yyval.procIter) = ProcIter_ITER; }
#line 8891 "bison-chapel.cpp"
    break;

  case 382:
#line 1574 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 8897 "bison-chapel.cpp"
    break;

  case 383:
#line 1575 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 8903 "bison-chapel.cpp"
    break;

  case 384:
#line 1576 "chapel.ypp"
              { (yyval.retTag) = RET_CONST_REF; }
#line 8909 "bison-chapel.cpp"
    break;

  case 385:
#line 1577 "chapel.ypp"
              { (yyval.retTag) = RET_REF; }
#line 8915 "bison-chapel.cpp"
    break;

  case 386:
#line 1578 "chapel.ypp"
              { (yyval.retTag) = RET_PARAM; }
#line 8921 "bison-chapel.cpp"
    break;

  case 387:
#line 1579 "chapel.ypp"
              { (yyval.retTag) = RET_TYPE; }
#line 8927 "bison-chapel.cpp"
    break;

  case 388:
#line 1583 "chapel.ypp"
          { (yyval.b) = false; }
#line 8933 "bison-chapel.cpp"
    break;

  case 389:
#line 1584 "chapel.ypp"
          { (yyval.b) = true;  }
#line 8939 "bison-chapel.cpp"
    break;

  case 390:
#line 1587 "chapel.ypp"
            { (yyval.pblockstmt) = NULL; }
#line 8945 "bison-chapel.cpp"
    break;

  case 393:
#line 1593 "chapel.ypp"
               { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 8951 "bison-chapel.cpp"
    break;

  case 394:
#line 1597 "chapel.ypp"
                      { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 8957 "bison-chapel.cpp"
    break;

  case 395:
#line 1601 "chapel.ypp"
                         { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 8963 "bison-chapel.cpp"
    break;

  case 396:
#line 1602 "chapel.ypp"
                         { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8969 "bison-chapel.cpp"
    break;

  case 397:
#line 1603 "chapel.ypp"
                         { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 8979 "bison-chapel.cpp"
    break;

  case 398:
#line 1611 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 8985 "bison-chapel.cpp"
    break;

  case 399:
#line 1613 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 8991 "bison-chapel.cpp"
    break;

  case 400:
#line 1615 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 8997 "bison-chapel.cpp"
    break;

  case 401:
#line 1617 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9003 "bison-chapel.cpp"
    break;

  case 402:
#line 1619 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9009 "bison-chapel.cpp"
    break;

  case 403:
#line 1624 "chapel.ypp"
  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9015 "bison-chapel.cpp"
    break;

  case 404:
#line 1626 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9021 "bison-chapel.cpp"
    break;

  case 405:
#line 1630 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9027 "bison-chapel.cpp"
    break;

  case 406:
#line 1631 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9033 "bison-chapel.cpp"
    break;

  case 407:
#line 1632 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9039 "bison-chapel.cpp"
    break;

  case 408:
#line 1633 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9045 "bison-chapel.cpp"
    break;

  case 409:
#line 1634 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9051 "bison-chapel.cpp"
    break;

  case 410:
#line 1635 "chapel.ypp"
                                              {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9057 "bison-chapel.cpp"
    break;

  case 411:
#line 1636 "chapel.ypp"
                         { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 9063 "bison-chapel.cpp"
    break;

  case 412:
#line 1641 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 9069 "bison-chapel.cpp"
    break;

  case 413:
#line 1643 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 9075 "bison-chapel.cpp"
    break;

  case 414:
#line 1648 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 9081 "bison-chapel.cpp"
    break;

  case 415:
#line 1650 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 9087 "bison-chapel.cpp"
    break;

  case 416:
#line 1652 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 9093 "bison-chapel.cpp"
    break;

  case 417:
#line 1657 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 9110 "bison-chapel.cpp"
    break;

  case 418:
#line 1670 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 9128 "bison-chapel.cpp"
    break;

  case 419:
#line 1686 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
#line 9134 "bison-chapel.cpp"
    break;

  case 420:
#line 1688 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9140 "bison-chapel.cpp"
    break;

  case 421:
#line 1690 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 9146 "bison-chapel.cpp"
    break;

  case 422:
#line 1694 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 9152 "bison-chapel.cpp"
    break;

  case 423:
#line 1695 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 9158 "bison-chapel.cpp"
    break;

  case 424:
#line 1696 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 9164 "bison-chapel.cpp"
    break;

  case 425:
#line 1697 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 9170 "bison-chapel.cpp"
    break;

  case 426:
#line 1698 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(); }
#line 9176 "bison-chapel.cpp"
    break;

  case 427:
#line 1703 "chapel.ypp"
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9186 "bison-chapel.cpp"
    break;

  case 428:
#line 1709 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9195 "bison-chapel.cpp"
    break;

  case 430:
#line 1718 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 9204 "bison-chapel.cpp"
    break;

  case 431:
#line 1726 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 9210 "bison-chapel.cpp"
    break;

  case 432:
#line 1728 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9216 "bison-chapel.cpp"
    break;

  case 433:
#line 1733 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 9222 "bison-chapel.cpp"
    break;

  case 434:
#line 1735 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 9228 "bison-chapel.cpp"
    break;

  case 435:
#line 1737 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 9234 "bison-chapel.cpp"
    break;

  case 436:
#line 1742 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 9240 "bison-chapel.cpp"
    break;

  case 437:
#line 1744 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 9246 "bison-chapel.cpp"
    break;

  case 438:
#line 1746 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 9252 "bison-chapel.cpp"
    break;

  case 439:
#line 1752 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9258 "bison-chapel.cpp"
    break;

  case 440:
#line 1753 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 9264 "bison-chapel.cpp"
    break;

  case 441:
#line 1754 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9270 "bison-chapel.cpp"
    break;

  case 442:
#line 1760 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9276 "bison-chapel.cpp"
    break;

  case 443:
#line 1762 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 9282 "bison-chapel.cpp"
    break;

  case 444:
#line 1764 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9290 "bison-chapel.cpp"
    break;

  case 445:
#line 1768 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 9298 "bison-chapel.cpp"
    break;

  case 446:
#line 1772 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9304 "bison-chapel.cpp"
    break;

  case 447:
#line 1774 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9312 "bison-chapel.cpp"
    break;

  case 448:
#line 1778 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9320 "bison-chapel.cpp"
    break;

  case 449:
#line 1785 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9326 "bison-chapel.cpp"
    break;

  case 450:
#line 1786 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9332 "bison-chapel.cpp"
    break;

  case 451:
#line 1787 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9338 "bison-chapel.cpp"
    break;

  case 452:
#line 1788 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9344 "bison-chapel.cpp"
    break;

  case 453:
#line 1789 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9350 "bison-chapel.cpp"
    break;

  case 454:
#line 1794 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9356 "bison-chapel.cpp"
    break;

  case 455:
#line 1795 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9362 "bison-chapel.cpp"
    break;

  case 456:
#line 1796 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 9368 "bison-chapel.cpp"
    break;

  case 457:
#line 1797 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9374 "bison-chapel.cpp"
    break;

  case 458:
#line 1798 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9380 "bison-chapel.cpp"
    break;

  case 459:
#line 1819 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9388 "bison-chapel.cpp"
    break;

  case 460:
#line 1823 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 9396 "bison-chapel.cpp"
    break;

  case 461:
#line 1827 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 9408 "bison-chapel.cpp"
    break;

  case 462:
#line 1835 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9416 "bison-chapel.cpp"
    break;

  case 463:
#line 1841 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9422 "bison-chapel.cpp"
    break;

  case 464:
#line 1842 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9428 "bison-chapel.cpp"
    break;

  case 465:
#line 1843 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9434 "bison-chapel.cpp"
    break;

  case 466:
#line 1848 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9440 "bison-chapel.cpp"
    break;

  case 467:
#line 1850 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9446 "bison-chapel.cpp"
    break;

  case 468:
#line 1856 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9452 "bison-chapel.cpp"
    break;

  case 469:
#line 1858 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9458 "bison-chapel.cpp"
    break;

  case 470:
#line 1860 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 9464 "bison-chapel.cpp"
    break;

  case 471:
#line 1864 "chapel.ypp"
                            { (yyval.pexpr) = NULL; }
#line 9470 "bison-chapel.cpp"
    break;

  case 472:
#line 1865 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9476 "bison-chapel.cpp"
    break;

  case 473:
#line 1866 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9482 "bison-chapel.cpp"
    break;

  case 474:
#line 1867 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9488 "bison-chapel.cpp"
    break;

  case 475:
#line 1868 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9494 "bison-chapel.cpp"
    break;

  case 476:
#line 1874 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9500 "bison-chapel.cpp"
    break;

  case 477:
#line 1875 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9506 "bison-chapel.cpp"
    break;

  case 478:
#line 1876 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9512 "bison-chapel.cpp"
    break;

  case 479:
#line 1877 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9518 "bison-chapel.cpp"
    break;

  case 480:
#line 1881 "chapel.ypp"
                                   { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 9524 "bison-chapel.cpp"
    break;

  case 481:
#line 1882 "chapel.ypp"
                                   { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9530 "bison-chapel.cpp"
    break;

  case 482:
#line 1886 "chapel.ypp"
                { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 9536 "bison-chapel.cpp"
    break;

  case 483:
#line 1887 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9542 "bison-chapel.cpp"
    break;

  case 484:
#line 1888 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9548 "bison-chapel.cpp"
    break;

  case 485:
#line 1892 "chapel.ypp"
                                         { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9554 "bison-chapel.cpp"
    break;

  case 486:
#line 1893 "chapel.ypp"
                                       { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9560 "bison-chapel.cpp"
    break;

  case 487:
#line 1897 "chapel.ypp"
             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 9566 "bison-chapel.cpp"
    break;

  case 489:
#line 1902 "chapel.ypp"
                                { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9572 "bison-chapel.cpp"
    break;

  case 490:
#line 1903 "chapel.ypp"
                                { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9578 "bison-chapel.cpp"
    break;

  case 491:
#line 1907 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9584 "bison-chapel.cpp"
    break;

  case 492:
#line 1908 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9590 "bison-chapel.cpp"
    break;

  case 493:
#line 1909 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9596 "bison-chapel.cpp"
    break;

  case 494:
#line 1910 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9602 "bison-chapel.cpp"
    break;

  case 495:
#line 1914 "chapel.ypp"
                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9608 "bison-chapel.cpp"
    break;

  case 496:
#line 1915 "chapel.ypp"
                 { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9614 "bison-chapel.cpp"
    break;

  case 497:
#line 1927 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9620 "bison-chapel.cpp"
    break;

  case 498:
#line 1929 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 9626 "bison-chapel.cpp"
    break;

  case 499:
#line 1931 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9632 "bison-chapel.cpp"
    break;

  case 505:
#line 1942 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 9638 "bison-chapel.cpp"
    break;

  case 506:
#line 1944 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 9644 "bison-chapel.cpp"
    break;

  case 507:
#line 1946 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 9650 "bison-chapel.cpp"
    break;

  case 508:
#line 1948 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 9656 "bison-chapel.cpp"
    break;

  case 509:
#line 1950 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 9662 "bison-chapel.cpp"
    break;

  case 510:
#line 1952 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 9668 "bison-chapel.cpp"
    break;

  case 511:
#line 1954 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 9674 "bison-chapel.cpp"
    break;

  case 512:
#line 1957 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 9680 "bison-chapel.cpp"
    break;

  case 513:
#line 1959 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 9686 "bison-chapel.cpp"
    break;

  case 514:
#line 1961 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 9692 "bison-chapel.cpp"
    break;

  case 515:
#line 1963 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9698 "bison-chapel.cpp"
    break;

  case 516:
#line 1965 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 9704 "bison-chapel.cpp"
    break;

  case 517:
#line 1967 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 9710 "bison-chapel.cpp"
    break;

  case 518:
#line 1969 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 9716 "bison-chapel.cpp"
    break;

  case 519:
#line 1971 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9722 "bison-chapel.cpp"
    break;

  case 520:
#line 1974 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 9728 "bison-chapel.cpp"
    break;

  case 521:
#line 1976 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 9734 "bison-chapel.cpp"
    break;

  case 522:
#line 1981 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9740 "bison-chapel.cpp"
    break;

  case 523:
#line 1983 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 9746 "bison-chapel.cpp"
    break;

  case 524:
#line 1985 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9752 "bison-chapel.cpp"
    break;

  case 525:
#line 1987 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9758 "bison-chapel.cpp"
    break;

  case 526:
#line 1989 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 9764 "bison-chapel.cpp"
    break;

  case 527:
#line 1991 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9770 "bison-chapel.cpp"
    break;

  case 528:
#line 1993 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9776 "bison-chapel.cpp"
    break;

  case 529:
#line 1995 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 9782 "bison-chapel.cpp"
    break;

  case 530:
#line 1997 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9788 "bison-chapel.cpp"
    break;

  case 531:
#line 1999 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9794 "bison-chapel.cpp"
    break;

  case 532:
#line 2001 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 9800 "bison-chapel.cpp"
    break;

  case 533:
#line 2003 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9806 "bison-chapel.cpp"
    break;

  case 534:
#line 2005 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 9817 "bison-chapel.cpp"
    break;

  case 535:
#line 2012 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 9827 "bison-chapel.cpp"
    break;

  case 536:
#line 2018 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 9837 "bison-chapel.cpp"
    break;

  case 537:
#line 2024 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9847 "bison-chapel.cpp"
    break;

  case 538:
#line 2030 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 9857 "bison-chapel.cpp"
    break;

  case 539:
#line 2039 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9863 "bison-chapel.cpp"
    break;

  case 540:
#line 2048 "chapel.ypp"
            { (yyval.pexpr) = new SymExpr(gNil); }
#line 9869 "bison-chapel.cpp"
    break;

  case 548:
#line 2064 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9875 "bison-chapel.cpp"
    break;

  case 549:
#line 2068 "chapel.ypp"
                                { (yyval.pcallexpr) = NULL; }
#line 9881 "bison-chapel.cpp"
    break;

  case 551:
#line 2073 "chapel.ypp"
                                { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9887 "bison-chapel.cpp"
    break;

  case 552:
#line 2077 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9893 "bison-chapel.cpp"
    break;

  case 553:
#line 2078 "chapel.ypp"
                                                    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9899 "bison-chapel.cpp"
    break;

  case 554:
#line 2082 "chapel.ypp"
                                  { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9905 "bison-chapel.cpp"
    break;

  case 555:
#line 2086 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9911 "bison-chapel.cpp"
    break;

  case 556:
#line 2087 "chapel.ypp"
                                                    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9917 "bison-chapel.cpp"
    break;

  case 557:
#line 2092 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 9925 "bison-chapel.cpp"
    break;

  case 558:
#line 2096 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9933 "bison-chapel.cpp"
    break;

  case 559:
#line 2100 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9941 "bison-chapel.cpp"
    break;

  case 560:
#line 2106 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 9947 "bison-chapel.cpp"
    break;

  case 561:
#line 2107 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_IN;        }
#line 9953 "bison-chapel.cpp"
    break;

  case 562:
#line 2108 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 9959 "bison-chapel.cpp"
    break;

  case 563:
#line 2109 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_REF;       }
#line 9965 "bison-chapel.cpp"
    break;

  case 564:
#line 2110 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 9971 "bison-chapel.cpp"
    break;

  case 565:
#line 2111 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 9977 "bison-chapel.cpp"
    break;

  case 567:
#line 2117 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9983 "bison-chapel.cpp"
    break;

  case 568:
#line 2122 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 9989 "bison-chapel.cpp"
    break;

  case 569:
#line 2124 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 9997 "bison-chapel.cpp"
    break;

  case 570:
#line 2128 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 10005 "bison-chapel.cpp"
    break;

  case 571:
#line 2132 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 10013 "bison-chapel.cpp"
    break;

  case 572:
#line 2136 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 10021 "bison-chapel.cpp"
    break;

  case 573:
#line 2144 "chapel.ypp"
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 10028 "bison-chapel.cpp"
    break;

  case 574:
#line 2149 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10038 "bison-chapel.cpp"
    break;

  case 575:
#line 2155 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10048 "bison-chapel.cpp"
    break;

  case 576:
#line 2161 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10059 "bison-chapel.cpp"
    break;

  case 577:
#line 2168 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10070 "bison-chapel.cpp"
    break;

  case 578:
#line 2178 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 10076 "bison-chapel.cpp"
    break;

  case 588:
#line 2195 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 10082 "bison-chapel.cpp"
    break;

  case 589:
#line 2197 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10088 "bison-chapel.cpp"
    break;

  case 590:
#line 2199 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10094 "bison-chapel.cpp"
    break;

  case 591:
#line 2201 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr), false, true); }
#line 10100 "bison-chapel.cpp"
    break;

  case 592:
#line 2216 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[-1].pexpr)); }
#line 10106 "bison-chapel.cpp"
    break;

  case 593:
#line 2218 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr)); }
#line 10112 "bison-chapel.cpp"
    break;

  case 594:
#line 2220 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr), true); }
#line 10118 "bison-chapel.cpp"
    break;

  case 595:
#line 2222 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
#line 10124 "bison-chapel.cpp"
    break;

  case 596:
#line 2226 "chapel.ypp"
                  { (yyval.pexpr) = NULL; }
#line 10130 "bison-chapel.cpp"
    break;

  case 597:
#line 2227 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10136 "bison-chapel.cpp"
    break;

  case 598:
#line 2231 "chapel.ypp"
                  { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 10142 "bison-chapel.cpp"
    break;

  case 599:
#line 2232 "chapel.ypp"
                  { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 10148 "bison-chapel.cpp"
    break;

  case 600:
#line 2233 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10154 "bison-chapel.cpp"
    break;

  case 605:
#line 2249 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10160 "bison-chapel.cpp"
    break;

  case 606:
#line 2250 "chapel.ypp"
                     { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10166 "bison-chapel.cpp"
    break;

  case 607:
#line 2251 "chapel.ypp"
                                {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 10172 "bison-chapel.cpp"
    break;

  case 608:
#line 2252 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10178 "bison-chapel.cpp"
    break;

  case 610:
#line 2257 "chapel.ypp"
                                              { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10184 "bison-chapel.cpp"
    break;

  case 611:
#line 2258 "chapel.ypp"
                                              { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10190 "bison-chapel.cpp"
    break;

  case 612:
#line 2259 "chapel.ypp"
                                        { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 10196 "bison-chapel.cpp"
    break;

  case 613:
#line 2263 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 10202 "bison-chapel.cpp"
    break;

  case 614:
#line 2264 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 10208 "bison-chapel.cpp"
    break;

  case 615:
#line 2265 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 10214 "bison-chapel.cpp"
    break;

  case 616:
#line 2266 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 10220 "bison-chapel.cpp"
    break;

  case 617:
#line 2267 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 10226 "bison-chapel.cpp"
    break;

  case 618:
#line 2275 "chapel.ypp"
                                    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 10232 "bison-chapel.cpp"
    break;

  case 619:
#line 2276 "chapel.ypp"
                                    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 10238 "bison-chapel.cpp"
    break;

  case 620:
#line 2277 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 10244 "bison-chapel.cpp"
    break;

  case 621:
#line 2278 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 10250 "bison-chapel.cpp"
    break;

  case 622:
#line 2282 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gFalse); }
#line 10256 "bison-chapel.cpp"
    break;

  case 623:
#line 2283 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gTrue); }
#line 10262 "bison-chapel.cpp"
    break;

  case 624:
#line 2287 "chapel.ypp"
                  { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 10268 "bison-chapel.cpp"
    break;

  case 625:
#line 2288 "chapel.ypp"
                  { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 10274 "bison-chapel.cpp"
    break;

  case 628:
#line 2294 "chapel.ypp"
                        { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 10280 "bison-chapel.cpp"
    break;

  case 629:
#line 2295 "chapel.ypp"
                        { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 10286 "bison-chapel.cpp"
    break;

  case 630:
#line 2296 "chapel.ypp"
                        { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 10292 "bison-chapel.cpp"
    break;

  case 631:
#line 2297 "chapel.ypp"
                        { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 10298 "bison-chapel.cpp"
    break;

  case 632:
#line 2298 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNone); }
#line 10304 "bison-chapel.cpp"
    break;

  case 633:
#line 2299 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr),
                                            new SymExpr(gTrue)); }
#line 10311 "bison-chapel.cpp"
    break;

  case 634:
#line 2301 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr),
                                                   new SymExpr(gTrue)); }
#line 10318 "bison-chapel.cpp"
    break;

  case 635:
#line 2303 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 10324 "bison-chapel.cpp"
    break;

  case 636:
#line 2304 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 10330 "bison-chapel.cpp"
    break;

  case 637:
#line 2306 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 10338 "bison-chapel.cpp"
    break;

  case 638:
#line 2310 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 10346 "bison-chapel.cpp"
    break;

  case 639:
#line 2317 "chapel.ypp"
                                        { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10352 "bison-chapel.cpp"
    break;

  case 640:
#line 2318 "chapel.ypp"
                                        { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 10358 "bison-chapel.cpp"
    break;

  case 641:
#line 2322 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10364 "bison-chapel.cpp"
    break;

  case 642:
#line 2323 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10370 "bison-chapel.cpp"
    break;

  case 643:
#line 2324 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10376 "bison-chapel.cpp"
    break;

  case 644:
#line 2325 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10382 "bison-chapel.cpp"
    break;

  case 645:
#line 2326 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10388 "bison-chapel.cpp"
    break;

  case 646:
#line 2327 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10394 "bison-chapel.cpp"
    break;

  case 647:
#line 2328 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10400 "bison-chapel.cpp"
    break;

  case 648:
#line 2329 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10406 "bison-chapel.cpp"
    break;

  case 649:
#line 2330 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10412 "bison-chapel.cpp"
    break;

  case 650:
#line 2331 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10418 "bison-chapel.cpp"
    break;

  case 651:
#line 2332 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10424 "bison-chapel.cpp"
    break;

  case 652:
#line 2333 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10430 "bison-chapel.cpp"
    break;

  case 653:
#line 2334 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10436 "bison-chapel.cpp"
    break;

  case 654:
#line 2335 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10442 "bison-chapel.cpp"
    break;

  case 655:
#line 2336 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10448 "bison-chapel.cpp"
    break;

  case 656:
#line 2337 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10454 "bison-chapel.cpp"
    break;

  case 657:
#line 2338 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10460 "bison-chapel.cpp"
    break;

  case 658:
#line 2339 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10466 "bison-chapel.cpp"
    break;

  case 659:
#line 2340 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10472 "bison-chapel.cpp"
    break;

  case 660:
#line 2341 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10478 "bison-chapel.cpp"
    break;

  case 661:
#line 2342 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10484 "bison-chapel.cpp"
    break;

  case 662:
#line 2343 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10490 "bison-chapel.cpp"
    break;

  case 663:
#line 2344 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr),
                                               new SymExpr(gTrue)); }
#line 10497 "bison-chapel.cpp"
    break;

  case 664:
#line 2349 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 10503 "bison-chapel.cpp"
    break;

  case 665:
#line 2350 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 10509 "bison-chapel.cpp"
    break;

  case 666:
#line 2351 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 10515 "bison-chapel.cpp"
    break;

  case 667:
#line 2352 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 10521 "bison-chapel.cpp"
    break;

  case 668:
#line 2353 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 10527 "bison-chapel.cpp"
    break;

  case 669:
#line 2354 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10533 "bison-chapel.cpp"
    break;

  case 670:
#line 2355 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 10539 "bison-chapel.cpp"
    break;

  case 671:
#line 2359 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10545 "bison-chapel.cpp"
    break;

  case 672:
#line 2360 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10551 "bison-chapel.cpp"
    break;

  case 673:
#line 2361 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10557 "bison-chapel.cpp"
    break;

  case 674:
#line 2362 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10563 "bison-chapel.cpp"
    break;

  case 675:
#line 2366 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10569 "bison-chapel.cpp"
    break;

  case 676:
#line 2367 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10575 "bison-chapel.cpp"
    break;

  case 677:
#line 2368 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10581 "bison-chapel.cpp"
    break;

  case 678:
#line 2369 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10587 "bison-chapel.cpp"
    break;

  case 679:
#line 2374 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 10593 "bison-chapel.cpp"
    break;

  case 680:
#line 2375 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 10599 "bison-chapel.cpp"
    break;

  case 681:
#line 2376 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 10605 "bison-chapel.cpp"
    break;

  case 682:
#line 2377 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 10611 "bison-chapel.cpp"
    break;

  case 683:
#line 2378 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 10617 "bison-chapel.cpp"
    break;

  case 684:
#line 2379 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 10623 "bison-chapel.cpp"
    break;

  case 685:
#line 2380 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 10629 "bison-chapel.cpp"
    break;


#line 10633 "bison-chapel.cpp"

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

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
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
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

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


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, context);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, yylsp, context);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  yyps->yynew = 1;


/*-----------------------------------------.
| yypushreturn -- ask for the next token.  |
`-----------------------------------------*/
yypushreturn:
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
