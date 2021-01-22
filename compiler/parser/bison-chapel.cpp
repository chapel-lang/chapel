/* A Bison parser, made by GNU Bison 3.6.4.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.6.4"

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

#line 249 "bison-chapel.cpp"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TIDENT = 258,                  /* TIDENT  */
    TQUERIEDIDENT = 259,           /* TQUERIEDIDENT  */
    INTLITERAL = 260,              /* INTLITERAL  */
    REALLITERAL = 261,             /* REALLITERAL  */
    IMAGLITERAL = 262,             /* IMAGLITERAL  */
    STRINGLITERAL = 263,           /* STRINGLITERAL  */
    BYTESLITERAL = 264,            /* BYTESLITERAL  */
    CSTRINGLITERAL = 265,          /* CSTRINGLITERAL  */
    EXTERNCODE = 266,              /* EXTERNCODE  */
    TALIGN = 267,                  /* TALIGN  */
    TAS = 268,                     /* TAS  */
    TATOMIC = 269,                 /* TATOMIC  */
    TBEGIN = 270,                  /* TBEGIN  */
    TBREAK = 271,                  /* TBREAK  */
    TBOOL = 272,                   /* TBOOL  */
    TBORROWED = 273,               /* TBORROWED  */
    TBY = 274,                     /* TBY  */
    TBYTES = 275,                  /* TBYTES  */
    TCATCH = 276,                  /* TCATCH  */
    TCLASS = 277,                  /* TCLASS  */
    TCOBEGIN = 278,                /* TCOBEGIN  */
    TCOFORALL = 279,               /* TCOFORALL  */
    TCOMPLEX = 280,                /* TCOMPLEX  */
    TCONFIG = 281,                 /* TCONFIG  */
    TCONST = 282,                  /* TCONST  */
    TCONTINUE = 283,               /* TCONTINUE  */
    TDEFER = 284,                  /* TDEFER  */
    TDELETE = 285,                 /* TDELETE  */
    TDMAPPED = 286,                /* TDMAPPED  */
    TDO = 287,                     /* TDO  */
    TDOMAIN = 288,                 /* TDOMAIN  */
    TELSE = 289,                   /* TELSE  */
    TENUM = 290,                   /* TENUM  */
    TEXCEPT = 291,                 /* TEXCEPT  */
    TEXPORT = 292,                 /* TEXPORT  */
    TEXTERN = 293,                 /* TEXTERN  */
    TFALSE = 294,                  /* TFALSE  */
    TFOR = 295,                    /* TFOR  */
    TFORALL = 296,                 /* TFORALL  */
    TFORWARDING = 297,             /* TFORWARDING  */
    TIF = 298,                     /* TIF  */
    TIMAG = 299,                   /* TIMAG  */
    TIMPORT = 300,                 /* TIMPORT  */
    TIN = 301,                     /* TIN  */
    TINCLUDE = 302,                /* TINCLUDE  */
    TINDEX = 303,                  /* TINDEX  */
    TINLINE = 304,                 /* TINLINE  */
    TINOUT = 305,                  /* TINOUT  */
    TINT = 306,                    /* TINT  */
    TITER = 307,                   /* TITER  */
    TINITEQUALS = 308,             /* TINITEQUALS  */
    TIMPLEMENTS = 309,             /* TIMPLEMENTS  */
    TINTERFACE = 310,              /* TINTERFACE  */
    TLABEL = 311,                  /* TLABEL  */
    TLAMBDA = 312,                 /* TLAMBDA  */
    TLET = 313,                    /* TLET  */
    TLIFETIME = 314,               /* TLIFETIME  */
    TLOCAL = 315,                  /* TLOCAL  */
    TLOCALE = 316,                 /* TLOCALE  */
    TMINUSMINUS = 317,             /* TMINUSMINUS  */
    TMODULE = 318,                 /* TMODULE  */
    TNEW = 319,                    /* TNEW  */
    TNIL = 320,                    /* TNIL  */
    TNOINIT = 321,                 /* TNOINIT  */
    TNONE = 322,                   /* TNONE  */
    TNOTHING = 323,                /* TNOTHING  */
    TON = 324,                     /* TON  */
    TONLY = 325,                   /* TONLY  */
    TOTHERWISE = 326,              /* TOTHERWISE  */
    TOUT = 327,                    /* TOUT  */
    TOVERRIDE = 328,               /* TOVERRIDE  */
    TOWNED = 329,                  /* TOWNED  */
    TPARAM = 330,                  /* TPARAM  */
    TPLUSPLUS = 331,               /* TPLUSPLUS  */
    TPRAGMA = 332,                 /* TPRAGMA  */
    TPRIMITIVE = 333,              /* TPRIMITIVE  */
    TPRIVATE = 334,                /* TPRIVATE  */
    TPROC = 335,                   /* TPROC  */
    TPROTOTYPE = 336,              /* TPROTOTYPE  */
    TPUBLIC = 337,                 /* TPUBLIC  */
    TREAL = 338,                   /* TREAL  */
    TRECORD = 339,                 /* TRECORD  */
    TREDUCE = 340,                 /* TREDUCE  */
    TREF = 341,                    /* TREF  */
    TREQUIRE = 342,                /* TREQUIRE  */
    TRETURN = 343,                 /* TRETURN  */
    TSCAN = 344,                   /* TSCAN  */
    TSELECT = 345,                 /* TSELECT  */
    TSERIAL = 346,                 /* TSERIAL  */
    TSHARED = 347,                 /* TSHARED  */
    TSINGLE = 348,                 /* TSINGLE  */
    TSPARSE = 349,                 /* TSPARSE  */
    TSTRING = 350,                 /* TSTRING  */
    TSUBDOMAIN = 351,              /* TSUBDOMAIN  */
    TSYNC = 352,                   /* TSYNC  */
    TTHEN = 353,                   /* TTHEN  */
    TTHIS = 354,                   /* TTHIS  */
    TTHROW = 355,                  /* TTHROW  */
    TTHROWS = 356,                 /* TTHROWS  */
    TTRUE = 357,                   /* TTRUE  */
    TTRY = 358,                    /* TTRY  */
    TTRYBANG = 359,                /* TTRYBANG  */
    TTYPE = 360,                   /* TTYPE  */
    TUINT = 361,                   /* TUINT  */
    TUNDERSCORE = 362,             /* TUNDERSCORE  */
    TUNION = 363,                  /* TUNION  */
    TUNMANAGED = 364,              /* TUNMANAGED  */
    TUSE = 365,                    /* TUSE  */
    TVAR = 366,                    /* TVAR  */
    TVOID = 367,                   /* TVOID  */
    TWHEN = 368,                   /* TWHEN  */
    TWHERE = 369,                  /* TWHERE  */
    TWHILE = 370,                  /* TWHILE  */
    TWITH = 371,                   /* TWITH  */
    TYIELD = 372,                  /* TYIELD  */
    TZIP = 373,                    /* TZIP  */
    TALIAS = 374,                  /* TALIAS  */
    TAND = 375,                    /* TAND  */
    TASSIGN = 376,                 /* TASSIGN  */
    TASSIGNBAND = 377,             /* TASSIGNBAND  */
    TASSIGNBOR = 378,              /* TASSIGNBOR  */
    TASSIGNBXOR = 379,             /* TASSIGNBXOR  */
    TASSIGNDIVIDE = 380,           /* TASSIGNDIVIDE  */
    TASSIGNEXP = 381,              /* TASSIGNEXP  */
    TASSIGNLAND = 382,             /* TASSIGNLAND  */
    TASSIGNLOR = 383,              /* TASSIGNLOR  */
    TASSIGNMINUS = 384,            /* TASSIGNMINUS  */
    TASSIGNMOD = 385,              /* TASSIGNMOD  */
    TASSIGNMULTIPLY = 386,         /* TASSIGNMULTIPLY  */
    TASSIGNPLUS = 387,             /* TASSIGNPLUS  */
    TASSIGNREDUCE = 388,           /* TASSIGNREDUCE  */
    TASSIGNSL = 389,               /* TASSIGNSL  */
    TASSIGNSR = 390,               /* TASSIGNSR  */
    TBANG = 391,                   /* TBANG  */
    TBAND = 392,                   /* TBAND  */
    TBNOT = 393,                   /* TBNOT  */
    TBOR = 394,                    /* TBOR  */
    TBXOR = 395,                   /* TBXOR  */
    TCOLON = 396,                  /* TCOLON  */
    TCOMMA = 397,                  /* TCOMMA  */
    TDIVIDE = 398,                 /* TDIVIDE  */
    TDOT = 399,                    /* TDOT  */
    TDOTDOT = 400,                 /* TDOTDOT  */
    TDOTDOTDOT = 401,              /* TDOTDOTDOT  */
    TEQUAL = 402,                  /* TEQUAL  */
    TEXP = 403,                    /* TEXP  */
    TGREATER = 404,                /* TGREATER  */
    TGREATEREQUAL = 405,           /* TGREATEREQUAL  */
    THASH = 406,                   /* THASH  */
    TIO = 407,                     /* TIO  */
    TLESS = 408,                   /* TLESS  */
    TLESSEQUAL = 409,              /* TLESSEQUAL  */
    TMINUS = 410,                  /* TMINUS  */
    TMOD = 411,                    /* TMOD  */
    TNOTEQUAL = 412,               /* TNOTEQUAL  */
    TOR = 413,                     /* TOR  */
    TPLUS = 414,                   /* TPLUS  */
    TQUESTION = 415,               /* TQUESTION  */
    TSEMI = 416,                   /* TSEMI  */
    TSHIFTLEFT = 417,              /* TSHIFTLEFT  */
    TSHIFTRIGHT = 418,             /* TSHIFTRIGHT  */
    TSTAR = 419,                   /* TSTAR  */
    TSWAP = 420,                   /* TSWAP  */
    TLCBR = 421,                   /* TLCBR  */
    TRCBR = 422,                   /* TRCBR  */
    TLP = 423,                     /* TLP  */
    TRP = 424,                     /* TRP  */
    TLSBR = 425,                   /* TLSBR  */
    TRSBR = 426,                   /* TRSBR  */
    TNOELSE = 427,                 /* TNOELSE  */
    TDOTDOTOPENHIGH = 428,         /* TDOTDOTOPENHIGH  */
    TUPLUS = 429,                  /* TUPLUS  */
    TUMINUS = 430,                 /* TUMINUS  */
    TLNOT = 431                    /* TLNOT  */
  };
  typedef enum yytokentype yytoken_kind_t;
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


int yypush_parse (yypstate *ps,
                  int pushed_char, YYSTYPE const *pushed_val, YYLTYPE *pushed_loc, ParserContext* context);

yypstate *yypstate_new (void);
void yypstate_delete (yypstate *ps);

/* "%code provides" blocks.  */
#line 205 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 479 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TIDENT = 3,                     /* TIDENT  */
  YYSYMBOL_TQUERIEDIDENT = 4,              /* TQUERIEDIDENT  */
  YYSYMBOL_INTLITERAL = 5,                 /* INTLITERAL  */
  YYSYMBOL_REALLITERAL = 6,                /* REALLITERAL  */
  YYSYMBOL_IMAGLITERAL = 7,                /* IMAGLITERAL  */
  YYSYMBOL_STRINGLITERAL = 8,              /* STRINGLITERAL  */
  YYSYMBOL_BYTESLITERAL = 9,               /* BYTESLITERAL  */
  YYSYMBOL_CSTRINGLITERAL = 10,            /* CSTRINGLITERAL  */
  YYSYMBOL_EXTERNCODE = 11,                /* EXTERNCODE  */
  YYSYMBOL_TALIGN = 12,                    /* TALIGN  */
  YYSYMBOL_TAS = 13,                       /* TAS  */
  YYSYMBOL_TATOMIC = 14,                   /* TATOMIC  */
  YYSYMBOL_TBEGIN = 15,                    /* TBEGIN  */
  YYSYMBOL_TBREAK = 16,                    /* TBREAK  */
  YYSYMBOL_TBOOL = 17,                     /* TBOOL  */
  YYSYMBOL_TBORROWED = 18,                 /* TBORROWED  */
  YYSYMBOL_TBY = 19,                       /* TBY  */
  YYSYMBOL_TBYTES = 20,                    /* TBYTES  */
  YYSYMBOL_TCATCH = 21,                    /* TCATCH  */
  YYSYMBOL_TCLASS = 22,                    /* TCLASS  */
  YYSYMBOL_TCOBEGIN = 23,                  /* TCOBEGIN  */
  YYSYMBOL_TCOFORALL = 24,                 /* TCOFORALL  */
  YYSYMBOL_TCOMPLEX = 25,                  /* TCOMPLEX  */
  YYSYMBOL_TCONFIG = 26,                   /* TCONFIG  */
  YYSYMBOL_TCONST = 27,                    /* TCONST  */
  YYSYMBOL_TCONTINUE = 28,                 /* TCONTINUE  */
  YYSYMBOL_TDEFER = 29,                    /* TDEFER  */
  YYSYMBOL_TDELETE = 30,                   /* TDELETE  */
  YYSYMBOL_TDMAPPED = 31,                  /* TDMAPPED  */
  YYSYMBOL_TDO = 32,                       /* TDO  */
  YYSYMBOL_TDOMAIN = 33,                   /* TDOMAIN  */
  YYSYMBOL_TELSE = 34,                     /* TELSE  */
  YYSYMBOL_TENUM = 35,                     /* TENUM  */
  YYSYMBOL_TEXCEPT = 36,                   /* TEXCEPT  */
  YYSYMBOL_TEXPORT = 37,                   /* TEXPORT  */
  YYSYMBOL_TEXTERN = 38,                   /* TEXTERN  */
  YYSYMBOL_TFALSE = 39,                    /* TFALSE  */
  YYSYMBOL_TFOR = 40,                      /* TFOR  */
  YYSYMBOL_TFORALL = 41,                   /* TFORALL  */
  YYSYMBOL_TFORWARDING = 42,               /* TFORWARDING  */
  YYSYMBOL_TIF = 43,                       /* TIF  */
  YYSYMBOL_TIMAG = 44,                     /* TIMAG  */
  YYSYMBOL_TIMPORT = 45,                   /* TIMPORT  */
  YYSYMBOL_TIN = 46,                       /* TIN  */
  YYSYMBOL_TINCLUDE = 47,                  /* TINCLUDE  */
  YYSYMBOL_TINDEX = 48,                    /* TINDEX  */
  YYSYMBOL_TINLINE = 49,                   /* TINLINE  */
  YYSYMBOL_TINOUT = 50,                    /* TINOUT  */
  YYSYMBOL_TINT = 51,                      /* TINT  */
  YYSYMBOL_TITER = 52,                     /* TITER  */
  YYSYMBOL_TINITEQUALS = 53,               /* TINITEQUALS  */
  YYSYMBOL_TIMPLEMENTS = 54,               /* TIMPLEMENTS  */
  YYSYMBOL_TINTERFACE = 55,                /* TINTERFACE  */
  YYSYMBOL_TLABEL = 56,                    /* TLABEL  */
  YYSYMBOL_TLAMBDA = 57,                   /* TLAMBDA  */
  YYSYMBOL_TLET = 58,                      /* TLET  */
  YYSYMBOL_TLIFETIME = 59,                 /* TLIFETIME  */
  YYSYMBOL_TLOCAL = 60,                    /* TLOCAL  */
  YYSYMBOL_TLOCALE = 61,                   /* TLOCALE  */
  YYSYMBOL_TMINUSMINUS = 62,               /* TMINUSMINUS  */
  YYSYMBOL_TMODULE = 63,                   /* TMODULE  */
  YYSYMBOL_TNEW = 64,                      /* TNEW  */
  YYSYMBOL_TNIL = 65,                      /* TNIL  */
  YYSYMBOL_TNOINIT = 66,                   /* TNOINIT  */
  YYSYMBOL_TNONE = 67,                     /* TNONE  */
  YYSYMBOL_TNOTHING = 68,                  /* TNOTHING  */
  YYSYMBOL_TON = 69,                       /* TON  */
  YYSYMBOL_TONLY = 70,                     /* TONLY  */
  YYSYMBOL_TOTHERWISE = 71,                /* TOTHERWISE  */
  YYSYMBOL_TOUT = 72,                      /* TOUT  */
  YYSYMBOL_TOVERRIDE = 73,                 /* TOVERRIDE  */
  YYSYMBOL_TOWNED = 74,                    /* TOWNED  */
  YYSYMBOL_TPARAM = 75,                    /* TPARAM  */
  YYSYMBOL_TPLUSPLUS = 76,                 /* TPLUSPLUS  */
  YYSYMBOL_TPRAGMA = 77,                   /* TPRAGMA  */
  YYSYMBOL_TPRIMITIVE = 78,                /* TPRIMITIVE  */
  YYSYMBOL_TPRIVATE = 79,                  /* TPRIVATE  */
  YYSYMBOL_TPROC = 80,                     /* TPROC  */
  YYSYMBOL_TPROTOTYPE = 81,                /* TPROTOTYPE  */
  YYSYMBOL_TPUBLIC = 82,                   /* TPUBLIC  */
  YYSYMBOL_TREAL = 83,                     /* TREAL  */
  YYSYMBOL_TRECORD = 84,                   /* TRECORD  */
  YYSYMBOL_TREDUCE = 85,                   /* TREDUCE  */
  YYSYMBOL_TREF = 86,                      /* TREF  */
  YYSYMBOL_TREQUIRE = 87,                  /* TREQUIRE  */
  YYSYMBOL_TRETURN = 88,                   /* TRETURN  */
  YYSYMBOL_TSCAN = 89,                     /* TSCAN  */
  YYSYMBOL_TSELECT = 90,                   /* TSELECT  */
  YYSYMBOL_TSERIAL = 91,                   /* TSERIAL  */
  YYSYMBOL_TSHARED = 92,                   /* TSHARED  */
  YYSYMBOL_TSINGLE = 93,                   /* TSINGLE  */
  YYSYMBOL_TSPARSE = 94,                   /* TSPARSE  */
  YYSYMBOL_TSTRING = 95,                   /* TSTRING  */
  YYSYMBOL_TSUBDOMAIN = 96,                /* TSUBDOMAIN  */
  YYSYMBOL_TSYNC = 97,                     /* TSYNC  */
  YYSYMBOL_TTHEN = 98,                     /* TTHEN  */
  YYSYMBOL_TTHIS = 99,                     /* TTHIS  */
  YYSYMBOL_TTHROW = 100,                   /* TTHROW  */
  YYSYMBOL_TTHROWS = 101,                  /* TTHROWS  */
  YYSYMBOL_TTRUE = 102,                    /* TTRUE  */
  YYSYMBOL_TTRY = 103,                     /* TTRY  */
  YYSYMBOL_TTRYBANG = 104,                 /* TTRYBANG  */
  YYSYMBOL_TTYPE = 105,                    /* TTYPE  */
  YYSYMBOL_TUINT = 106,                    /* TUINT  */
  YYSYMBOL_TUNDERSCORE = 107,              /* TUNDERSCORE  */
  YYSYMBOL_TUNION = 108,                   /* TUNION  */
  YYSYMBOL_TUNMANAGED = 109,               /* TUNMANAGED  */
  YYSYMBOL_TUSE = 110,                     /* TUSE  */
  YYSYMBOL_TVAR = 111,                     /* TVAR  */
  YYSYMBOL_TVOID = 112,                    /* TVOID  */
  YYSYMBOL_TWHEN = 113,                    /* TWHEN  */
  YYSYMBOL_TWHERE = 114,                   /* TWHERE  */
  YYSYMBOL_TWHILE = 115,                   /* TWHILE  */
  YYSYMBOL_TWITH = 116,                    /* TWITH  */
  YYSYMBOL_TYIELD = 117,                   /* TYIELD  */
  YYSYMBOL_TZIP = 118,                     /* TZIP  */
  YYSYMBOL_TALIAS = 119,                   /* TALIAS  */
  YYSYMBOL_TAND = 120,                     /* TAND  */
  YYSYMBOL_TASSIGN = 121,                  /* TASSIGN  */
  YYSYMBOL_TASSIGNBAND = 122,              /* TASSIGNBAND  */
  YYSYMBOL_TASSIGNBOR = 123,               /* TASSIGNBOR  */
  YYSYMBOL_TASSIGNBXOR = 124,              /* TASSIGNBXOR  */
  YYSYMBOL_TASSIGNDIVIDE = 125,            /* TASSIGNDIVIDE  */
  YYSYMBOL_TASSIGNEXP = 126,               /* TASSIGNEXP  */
  YYSYMBOL_TASSIGNLAND = 127,              /* TASSIGNLAND  */
  YYSYMBOL_TASSIGNLOR = 128,               /* TASSIGNLOR  */
  YYSYMBOL_TASSIGNMINUS = 129,             /* TASSIGNMINUS  */
  YYSYMBOL_TASSIGNMOD = 130,               /* TASSIGNMOD  */
  YYSYMBOL_TASSIGNMULTIPLY = 131,          /* TASSIGNMULTIPLY  */
  YYSYMBOL_TASSIGNPLUS = 132,              /* TASSIGNPLUS  */
  YYSYMBOL_TASSIGNREDUCE = 133,            /* TASSIGNREDUCE  */
  YYSYMBOL_TASSIGNSL = 134,                /* TASSIGNSL  */
  YYSYMBOL_TASSIGNSR = 135,                /* TASSIGNSR  */
  YYSYMBOL_TBANG = 136,                    /* TBANG  */
  YYSYMBOL_TBAND = 137,                    /* TBAND  */
  YYSYMBOL_TBNOT = 138,                    /* TBNOT  */
  YYSYMBOL_TBOR = 139,                     /* TBOR  */
  YYSYMBOL_TBXOR = 140,                    /* TBXOR  */
  YYSYMBOL_TCOLON = 141,                   /* TCOLON  */
  YYSYMBOL_TCOMMA = 142,                   /* TCOMMA  */
  YYSYMBOL_TDIVIDE = 143,                  /* TDIVIDE  */
  YYSYMBOL_TDOT = 144,                     /* TDOT  */
  YYSYMBOL_TDOTDOT = 145,                  /* TDOTDOT  */
  YYSYMBOL_TDOTDOTDOT = 146,               /* TDOTDOTDOT  */
  YYSYMBOL_TEQUAL = 147,                   /* TEQUAL  */
  YYSYMBOL_TEXP = 148,                     /* TEXP  */
  YYSYMBOL_TGREATER = 149,                 /* TGREATER  */
  YYSYMBOL_TGREATEREQUAL = 150,            /* TGREATEREQUAL  */
  YYSYMBOL_THASH = 151,                    /* THASH  */
  YYSYMBOL_TIO = 152,                      /* TIO  */
  YYSYMBOL_TLESS = 153,                    /* TLESS  */
  YYSYMBOL_TLESSEQUAL = 154,               /* TLESSEQUAL  */
  YYSYMBOL_TMINUS = 155,                   /* TMINUS  */
  YYSYMBOL_TMOD = 156,                     /* TMOD  */
  YYSYMBOL_TNOTEQUAL = 157,                /* TNOTEQUAL  */
  YYSYMBOL_TOR = 158,                      /* TOR  */
  YYSYMBOL_TPLUS = 159,                    /* TPLUS  */
  YYSYMBOL_TQUESTION = 160,                /* TQUESTION  */
  YYSYMBOL_TSEMI = 161,                    /* TSEMI  */
  YYSYMBOL_TSHIFTLEFT = 162,               /* TSHIFTLEFT  */
  YYSYMBOL_TSHIFTRIGHT = 163,              /* TSHIFTRIGHT  */
  YYSYMBOL_TSTAR = 164,                    /* TSTAR  */
  YYSYMBOL_TSWAP = 165,                    /* TSWAP  */
  YYSYMBOL_TLCBR = 166,                    /* TLCBR  */
  YYSYMBOL_TRCBR = 167,                    /* TRCBR  */
  YYSYMBOL_TLP = 168,                      /* TLP  */
  YYSYMBOL_TRP = 169,                      /* TRP  */
  YYSYMBOL_TLSBR = 170,                    /* TLSBR  */
  YYSYMBOL_TRSBR = 171,                    /* TRSBR  */
  YYSYMBOL_TNOELSE = 172,                  /* TNOELSE  */
  YYSYMBOL_TDOTDOTOPENHIGH = 173,          /* TDOTDOTOPENHIGH  */
  YYSYMBOL_TUPLUS = 174,                   /* TUPLUS  */
  YYSYMBOL_TUMINUS = 175,                  /* TUMINUS  */
  YYSYMBOL_TLNOT = 176,                    /* TLNOT  */
  YYSYMBOL_YYACCEPT = 177,                 /* $accept  */
  YYSYMBOL_program = 178,                  /* program  */
  YYSYMBOL_toplevel_stmt_ls = 179,         /* toplevel_stmt_ls  */
  YYSYMBOL_toplevel_stmt = 180,            /* toplevel_stmt  */
  YYSYMBOL_pragma_ls = 181,                /* pragma_ls  */
  YYSYMBOL_stmt = 182,                     /* stmt  */
  YYSYMBOL_module_decl_start = 183,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 184,         /* module_decl_stmt  */
  YYSYMBOL_access_control = 185,           /* access_control  */
  YYSYMBOL_opt_prototype = 186,            /* opt_prototype  */
  YYSYMBOL_include_access_control = 187,   /* include_access_control  */
  YYSYMBOL_include_module_stmt = 188,      /* include_module_stmt  */
  YYSYMBOL_189_1 = 189,                    /* $@1  */
  YYSYMBOL_block_stmt = 190,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 191,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 192,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 193,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 194,              /* opt_only_ls  */
  YYSYMBOL_except_ls = 195,                /* except_ls  */
  YYSYMBOL_use_access_control = 196,       /* use_access_control  */
  YYSYMBOL_use_stmt = 197,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 198,              /* import_stmt  */
  YYSYMBOL_import_expr = 199,              /* import_expr  */
  YYSYMBOL_import_ls = 200,                /* import_ls  */
  YYSYMBOL_require_stmt = 201,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 202,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 203,          /* opt_label_ident  */
  YYSYMBOL_ident_fn_def = 204,             /* ident_fn_def  */
  YYSYMBOL_ident_def = 205,                /* ident_def  */
  YYSYMBOL_ident_use = 206,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 207,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 208,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 209,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 210,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 211,              /* return_stmt  */
  YYSYMBOL_class_level_stmt = 212,         /* class_level_stmt  */
  YYSYMBOL_213_2 = 213,                    /* @2  */
  YYSYMBOL_private_decl = 214,             /* private_decl  */
  YYSYMBOL_forwarding_stmt = 215,          /* forwarding_stmt  */
  YYSYMBOL_extern_export_decl_stmt = 216,  /* extern_export_decl_stmt  */
  YYSYMBOL_217_3 = 217,                    /* $@3  */
  YYSYMBOL_218_4 = 218,                    /* $@4  */
  YYSYMBOL_219_5 = 219,                    /* $@5  */
  YYSYMBOL_220_6 = 220,                    /* $@6  */
  YYSYMBOL_extern_block_stmt = 221,        /* extern_block_stmt  */
  YYSYMBOL_loop_stmt = 222,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 223,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 224,                  /* if_stmt  */
  YYSYMBOL_interface_stmt = 225,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 226,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 227,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 228,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 229, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 230,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 231,           /* ifc_constraint  */
  YYSYMBOL_defer_stmt = 232,               /* defer_stmt  */
  YYSYMBOL_try_stmt = 233,                 /* try_stmt  */
  YYSYMBOL_catch_stmt_ls = 234,            /* catch_stmt_ls  */
  YYSYMBOL_catch_stmt = 235,               /* catch_stmt  */
  YYSYMBOL_catch_expr = 236,               /* catch_expr  */
  YYSYMBOL_throw_stmt = 237,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 238,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 239,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 240,                /* when_stmt  */
  YYSYMBOL_class_decl_stmt = 241,          /* class_decl_stmt  */
  YYSYMBOL_class_tag = 242,                /* class_tag  */
  YYSYMBOL_opt_inherit = 243,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 244,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 245,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header = 246,              /* enum_header  */
  YYSYMBOL_enum_ls = 247,                  /* enum_ls  */
  YYSYMBOL_enum_item = 248,                /* enum_item  */
  YYSYMBOL_lambda_decl_expr = 249,         /* lambda_decl_expr  */
  YYSYMBOL_250_7 = 250,                    /* $@7  */
  YYSYMBOL_251_8 = 251,                    /* $@8  */
  YYSYMBOL_linkage_spec = 252,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt = 253,             /* fn_decl_stmt  */
  YYSYMBOL_254_9 = 254,                    /* $@9  */
  YYSYMBOL_255_10 = 255,                   /* $@10  */
  YYSYMBOL_fn_decl_stmt_inner = 256,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_receiver_expr = 257,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 258,                 /* fn_ident  */
  YYSYMBOL_assignop_ident = 259,           /* assignop_ident  */
  YYSYMBOL_opt_formal_ls = 260,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 261,            /* req_formal_ls  */
  YYSYMBOL_formal_ls = 262,                /* formal_ls  */
  YYSYMBOL_formal = 263,                   /* formal  */
  YYSYMBOL_opt_intent_tag = 264,           /* opt_intent_tag  */
  YYSYMBOL_required_intent_tag = 265,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 266,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_or_iter = 267,             /* proc_or_iter  */
  YYSYMBOL_opt_ret_tag = 268,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 269,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 270,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 271,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 272,               /* query_expr  */
  YYSYMBOL_var_arg_expr = 273,             /* var_arg_expr  */
  YYSYMBOL_opt_lifetime_where = 274,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 275, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 276,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 277,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 278,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_inner = 279, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 280,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 281,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 282,            /* var_decl_stmt  */
  YYSYMBOL_var_decl_stmt_inner_ls = 283,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 284,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 285, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 286, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 287,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 288,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 289,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 290,                 /* opt_type  */
  YYSYMBOL_array_type = 291,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 292, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 293,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 294,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 295,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 296,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 297,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 298,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 299,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 300,                /* actual_ls  */
  YYSYMBOL_actual_expr = 301,              /* actual_expr  */
  YYSYMBOL_ident_expr = 302,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 303,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 304,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 305,                 /* for_expr  */
  YYSYMBOL_cond_expr = 306,                /* cond_expr  */
  YYSYMBOL_nil_expr = 307,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 308,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 309,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 310,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 311,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 312,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 313,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 314,              /* intent_expr  */
  YYSYMBOL_shadow_var_prefix = 315,        /* shadow_var_prefix  */
  YYSYMBOL_io_expr = 316,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 317,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 318,                 /* new_expr  */
  YYSYMBOL_let_expr = 319,                 /* let_expr  */
  YYSYMBOL_expr = 320,                     /* expr  */
  YYSYMBOL_opt_expr = 321,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 322,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 323,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 324,           /* call_base_expr  */
  YYSYMBOL_call_expr = 325,                /* call_expr  */
  YYSYMBOL_dot_expr = 326,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 327,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 328,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 329,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 330,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 331,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 332,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 333,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 334,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 335,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 336       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



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

#line 900 "bison-chapel.cpp"

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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYLAST   20035

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  177
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  160
/* YYNRULES -- Number of rules.  */
#define YYNRULES  677
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1208

#define YYMAXUTOK   431


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

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
     175,   176
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
    1024,  1025,  1031,  1037,  1043,  1049,  1056,  1063,  1067,  1074,
    1078,  1079,  1080,  1081,  1082,  1084,  1086,  1088,  1093,  1095,
    1103,  1104,  1109,  1114,  1115,  1116,  1117,  1118,  1119,  1120,
    1121,  1122,  1123,  1124,  1125,  1126,  1133,  1134,  1135,  1136,
    1145,  1146,  1150,  1152,  1155,  1161,  1163,  1166,  1172,  1175,
    1176,  1177,  1178,  1179,  1180,  1184,  1185,  1189,  1190,  1191,
    1195,  1196,  1200,  1203,  1205,  1210,  1211,  1215,  1217,  1219,
    1226,  1236,  1250,  1255,  1260,  1268,  1269,  1274,  1275,  1277,
    1282,  1298,  1305,  1314,  1322,  1326,  1333,  1334,  1339,  1344,
    1338,  1371,  1374,  1378,  1386,  1396,  1385,  1428,  1432,  1437,
    1441,  1446,  1453,  1454,  1458,  1459,  1460,  1461,  1462,  1463,
    1464,  1465,  1466,  1467,  1468,  1469,  1470,  1471,  1472,  1473,
    1474,  1475,  1476,  1477,  1478,  1479,  1480,  1481,  1482,  1483,
    1484,  1488,  1489,  1490,  1491,  1492,  1493,  1494,  1495,  1496,
    1497,  1498,  1499,  1503,  1504,  1508,  1512,  1513,  1514,  1518,
    1520,  1522,  1524,  1526,  1528,  1533,  1534,  1538,  1539,  1540,
    1541,  1542,  1543,  1544,  1545,  1546,  1550,  1551,  1552,  1553,
    1554,  1555,  1559,  1560,  1564,  1565,  1566,  1567,  1568,  1569,
    1573,  1574,  1577,  1578,  1582,  1583,  1587,  1591,  1592,  1593,
    1601,  1602,  1604,  1606,  1608,  1613,  1615,  1620,  1621,  1622,
    1623,  1624,  1625,  1626,  1630,  1632,  1637,  1639,  1641,  1646,
    1659,  1676,  1677,  1679,  1684,  1685,  1686,  1687,  1688,  1692,
    1698,  1706,  1707,  1715,  1717,  1722,  1724,  1726,  1731,  1733,
    1735,  1742,  1743,  1744,  1749,  1751,  1753,  1757,  1761,  1763,
    1767,  1775,  1776,  1777,  1778,  1779,  1784,  1785,  1786,  1787,
    1788,  1808,  1812,  1816,  1824,  1831,  1832,  1833,  1837,  1839,
    1845,  1847,  1849,  1854,  1855,  1856,  1857,  1858,  1864,  1865,
    1866,  1867,  1871,  1872,  1876,  1877,  1878,  1882,  1883,  1887,
    1888,  1892,  1893,  1897,  1898,  1899,  1900,  1904,  1905,  1916,
    1918,  1920,  1926,  1927,  1928,  1929,  1930,  1931,  1933,  1935,
    1937,  1939,  1941,  1943,  1946,  1948,  1950,  1952,  1954,  1956,
    1958,  1960,  1963,  1965,  1970,  1972,  1974,  1976,  1978,  1980,
    1982,  1984,  1986,  1988,  1990,  1992,  1994,  2001,  2007,  2013,
    2019,  2028,  2038,  2046,  2047,  2048,  2049,  2050,  2051,  2052,
    2053,  2058,  2059,  2063,  2067,  2068,  2072,  2076,  2077,  2081,
    2085,  2089,  2096,  2097,  2098,  2099,  2100,  2101,  2105,  2106,
    2111,  2113,  2117,  2121,  2125,  2133,  2138,  2144,  2150,  2157,
    2167,  2175,  2176,  2177,  2178,  2179,  2180,  2181,  2182,  2183,
    2184,  2186,  2188,  2190,  2205,  2207,  2209,  2211,  2216,  2217,
    2221,  2222,  2223,  2227,  2228,  2229,  2230,  2239,  2240,  2241,
    2242,  2243,  2247,  2248,  2249,  2253,  2254,  2255,  2256,  2257,
    2265,  2266,  2267,  2268,  2272,  2273,  2277,  2278,  2282,  2283,
    2284,  2285,  2286,  2287,  2288,  2289,  2291,  2293,  2294,  2295,
    2299,  2307,  2308,  2312,  2313,  2314,  2315,  2316,  2317,  2318,
    2319,  2320,  2321,  2322,  2323,  2324,  2325,  2326,  2327,  2328,
    2329,  2330,  2331,  2332,  2333,  2334,  2339,  2340,  2341,  2342,
    2343,  2344,  2345,  2349,  2350,  2351,  2352,  2356,  2357,  2358,
    2359,  2364,  2365,  2366,  2367,  2368,  2369,  2370
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TIDENT",
  "TQUERIEDIDENT", "INTLITERAL", "REALLITERAL", "IMAGLITERAL",
  "STRINGLITERAL", "BYTESLITERAL", "CSTRINGLITERAL", "EXTERNCODE",
  "TALIGN", "TAS", "TATOMIC", "TBEGIN", "TBREAK", "TBOOL", "TBORROWED",
  "TBY", "TBYTES", "TCATCH", "TCLASS", "TCOBEGIN", "TCOFORALL", "TCOMPLEX",
  "TCONFIG", "TCONST", "TCONTINUE", "TDEFER", "TDELETE", "TDMAPPED", "TDO",
  "TDOMAIN", "TELSE", "TENUM", "TEXCEPT", "TEXPORT", "TEXTERN", "TFALSE",
  "TFOR", "TFORALL", "TFORWARDING", "TIF", "TIMAG", "TIMPORT", "TIN",
  "TINCLUDE", "TINDEX", "TINLINE", "TINOUT", "TINT", "TITER",
  "TINITEQUALS", "TIMPLEMENTS", "TINTERFACE", "TLABEL", "TLAMBDA", "TLET",
  "TLIFETIME", "TLOCAL", "TLOCALE", "TMINUSMINUS", "TMODULE", "TNEW",
  "TNIL", "TNOINIT", "TNONE", "TNOTHING", "TON", "TONLY", "TOTHERWISE",
  "TOUT", "TOVERRIDE", "TOWNED", "TPARAM", "TPLUSPLUS", "TPRAGMA",
  "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC", "TREAL",
  "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT",
  "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE", "TSTRING", "TSUBDOMAIN",
  "TSYNC", "TTHEN", "TTHIS", "TTHROW", "TTHROWS", "TTRUE", "TTRY",
  "TTRYBANG", "TTYPE", "TUINT", "TUNDERSCORE", "TUNION", "TUNMANAGED",
  "TUSE", "TVAR", "TVOID", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD",
  "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR",
  "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND",
  "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY",
  "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL", "TASSIGNSR", "TBANG",
  "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT",
  "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL",
  "THASH", "TIO", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOTEQUAL",
  "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT",
  "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR", "TRSBR",
  "TNOELSE", "TDOTDOTOPENHIGH", "TUPLUS", "TUMINUS", "TLNOT", "$accept",
  "program", "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_start", "module_decl_stmt", "access_control",
  "opt_prototype", "include_access_control", "include_module_stmt", "$@1",
  "block_stmt", "stmt_ls", "renames_ls", "use_renames_ls", "opt_only_ls",
  "except_ls", "use_access_control", "use_stmt", "import_stmt",
  "import_expr", "import_ls", "require_stmt", "assignment_stmt",
  "opt_label_ident", "ident_fn_def", "ident_def", "ident_use",
  "internal_type_ident_def", "scalar_type", "reserved_type_ident_use",
  "do_stmt", "return_stmt", "class_level_stmt", "@2", "private_decl",
  "forwarding_stmt", "extern_export_decl_stmt", "$@3", "$@4", "$@5", "$@6",
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

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
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
     425,   426,   427,   428,   429,   430,   431
};
#endif

#define YYPACT_NINF (-1093)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-628)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1093,   183,  3341, -1093,    82,    46, -1093, -1093, -1093, -1093,
   -1093, -1093,  4709,   204,   268,   290, 14309,   345, 19663,   204,
   11008,   367,   478,   344,   268,  4709, 11008,  4709,   273, 19715,
   11177,  8119,   388,  8288,  9310,  6759, 11008,   401, -1093,   310,
   -1093,   413, 19767, 19767, 19767, -1093,  3037,  9479,   427, 11008,
     309, -1093,   436,   453, 11008, -1093, 14309, -1093, 11008,   515,
     366,   524,  2065,   504, 19819, -1093,  9650,  7610, 11008,  9479,
   14309, 11008,   472,   525,   420,  4709,   546, 11008,   556, 11346,
   11346, 19767,   563, -1093, 14309, -1093,   572, 11008, 11008, -1093,
   11008, -1093, 11008, -1093, -1093, 13839, 11008, -1093, 11008, -1093,
   -1093, -1093,  3683,  6930,  8459, 11008, -1093,  4538, -1093,   469,
   -1093,   557, -1093, -1093,    61, -1093, -1093, -1093, -1093, -1093,
   -1093, -1093, -1093, -1093, -1093, -1093, -1093, -1093, -1093, -1093,
     585, -1093, -1093, -1093, -1093, -1093, -1093, -1093, -1093, 19767,
   -1093, 19767,   357,    29, -1093, -1093,  3037, -1093,   482, -1093,
     491, -1093, -1093,   495,   497,   507, 11008,   500,   506, 19078,
    2466,   -54,   511,   512, -1093, -1093,   397, -1093, -1093, -1093,
   -1093, -1093,   461, -1093, -1093, 19078,   494,  4709, -1093, -1093,
     513, 11008, -1093, -1093, 11008, 11008, 11008, 19767, -1093, 11008,
    9650,  9650,   622,   412, -1093, -1093, -1093, -1093,   283,   433,
   -1093, -1093,   514, 15779, 19767,  3037, -1093,   516, -1093,    93,
   19078,   564,  7781,   594, 19871, 19078,   351,   599, -1093, 19923,
   19767,   351, 19767,   518,    30, 15459,    -3, 15383,   425, -1093,
   15541,  3157,    69,  7781, -1093, -1093, -1093, -1093, -1093, -1093,
   -1093, -1093, -1093, -1093, -1093, -1093, -1093, -1093, -1093, -1093,
   -1093, -1093, -1093, -1093, -1093, -1093, -1093, -1093,   521, -1093,
     443,  4709,   522,  2849,    95,   109, -1093,  4709, -1093, -1093,
   15855,  1082, -1093,   523,   532, -1093, 15855,   283,  1082, -1093,
    7781,  1717, -1093, -1093,  9819, -1093, -1093, -1093, -1093,   116,
   19078, 11008, 11008, -1093, 19078,   536, 16403, -1093, 15855,   283,
   19078,   533,  7781, -1093, 19078, 16479, -1093, -1093, 16559, 13832,
   -1093, -1093, 16639,   581,   542,   283, 15855, 16719,   380,   380,
    1449,  1082,  1082,   132, -1093, -1093,  3854,   -65, -1093, 11008,
   -1093,    84,   145, -1093,   -31,    23, 16799,   -37,  1449,   676,
   -1093,  4025, -1093,   645, 11008, 11008, 19767,   568,   544, -1093,
   -1093, -1093,   117,   455, -1093, 11008,   574, 11008, 11008, 11008,
    9310,  9310, 11008,   459, 11008, 11008, 11008, 11008, 11008,   377,
   13839, 11008, 11008, 11008, 11008, 11008, 11008, 11008, 11008, 11008,
   11008, 11008, 11008, 11008, 11008, 11008, 11008,   653, -1093, -1093,
   -1093, -1093, -1093,  8628,  8628, -1093, -1093, -1093, -1093,  8628,
   -1093, -1093,  8628,  8628,  7781,  7781,  9310,  9310,  7441, -1093,
   -1093, 15935, 16011, 16875,   552,     4, 19767,  4196, -1093,  9310,
      30,   560,   243, -1093, 11008, -1093, 11008,   601, -1093,   555,
     586, -1093, -1093, -1093, 19767, -1093,  3037, -1093, 19767,   566,
   -1093,  3037,   683,  9650, -1093,  4880,  9310, -1093,   562, -1093,
      30,  5051,  9310, -1093,    30, 11515, 11008, -1093,  4709, -1093,
     697, 11008, -1093, -1093,   557,   567,  7781, 19767, -1093, -1093,
     120, -1093, -1093,  2849, -1093,   591,   573, -1093, 11684,   616,
   11008,  3037, -1093, -1093, 11008, 11008, -1093,   576, -1093,  9650,
   -1093, 19078, 19078, -1093,    44, -1093,  7781,   577, -1093,   722,
   -1093,   722, -1093, 11853,   605, -1093, -1093, -1093, -1093, -1093,
   -1093,  8799, -1093, 16955,  7101, -1093,  7272, -1093,  4709,   578,
    9310,  8970,  3512,   579, 11008,  9988, -1093, -1093,   394, -1093,
    4367, 19767, -1093,   264, 17037,   333, 15623,   155,  9650,   582,
   19611,   225, -1093, 17113, 19366, 19366,   501, -1093,   501, -1093,
     501, 16608,  1041,  1710,   650,   283,   380, -1093,   583, -1093,
   -1093, -1093, -1093, -1093,  1449,  1224,   501,  1774,  1774, 19366,
    1774,  1774,  1300,   380,  1224, 19442,  1300,  1082,  1082,   380,
    1449,   593,   598,   600,   604,   606,   607,   602,   603, -1093,
     501, -1093,   501,    25, -1093, -1093, -1093,   150, -1093,  1192,
   19154,   492, 12022,  9310, 12191,  9310, 11008,  7781,  9310, 14577,
     584,   204, 17193, -1093, -1093, -1093, 19078, 17269,  7781, -1093,
    7781, 19767,   568,   337, 19767,   568, -1093,   354, 11008,   152,
   11008, 19078,    65, 16091,  7441, -1093, 11008, 19078,     1, 15699,
   -1093,   608,   628,   612, 17349,   628,   614,   742, 17425,  4709,
   16167,   715, -1093,   153, -1093,   159, -1093,    43, -1093, -1093,
   -1093, -1093, -1093, -1093,   758,   160, -1093,  3135, -1093,   361,
     619,  2849,    95,    12,    45, 11008, 11008,  6590, -1093, -1093,
     526,  7950, -1093, 19078, -1093, 17505, 17581, -1093, -1093, 19078,
     632,   169,   633, -1093,   836, -1093, -1093,   359, 19767, -1093,
   -1093, -1093, -1093, -1093, -1093, -1093,  4709,    22, 16247, -1093,
   -1093, 19078,  4709, 19078, -1093, 17663, -1093, -1093, -1093, 11008,
   -1093,   161,   125, 11008, -1093, 10157, 11515, 11008, -1093,  7781,
     639,  1460,   634,   662,   -44, -1093,   717, -1093, -1093, -1093,
   -1093, 13675,   636, -1093, -1093, -1093, -1093, -1093, -1093, -1093,
   -1093, -1093, -1093,  7441, -1093, -1093, -1093, -1093, -1093, -1093,
   -1093, -1093, -1093, -1093, -1093, -1093,    40,  9310,  9310, 11008,
     775, 17739, 11008,   777, 17819,   171,   640, 17895,  7781,    30,
      30, -1093, -1093, -1093, -1093,   568,   644, -1093,   568,   649,
   -1093, 15855, -1093, 14757,  5222, -1093,  5393, -1093,   244, -1093,
   14833,  5564, -1093,    30,  5735, -1093,    30, 11008, -1093, 11008,
   -1093,  4709, 11008, -1093,  4709,   782, 19767,   657, 19767,   514,
   -1093, -1093, 19767,  1213, -1093,  2849,   679,   735, -1093, -1093,
   -1093,   100, -1093, -1093,   616,   651,    90, -1093, -1093,   655,
     658, -1093,  5906,  9650, -1093, -1093, -1093, 19767, -1093,   686,
     514, -1093, -1093,  6077,   661,  6248,   665, -1093, 11008, -1093,
   -1093, 11008, 17975,    52, 16323,   667,   668,   247,   671,  1617,
   -1093, 11008, 19767, -1093, -1093,   361,   672,   370, -1093,   698,
   -1093,   701,   706,   716,   707,   713, -1093,   714,   724,   718,
     720,   723,   393,   730,   728,   729, -1093, -1093, -1093, -1093,
   -1093, -1093, -1093, -1093, -1093, -1093, -1093, -1093, -1093, -1093,
   -1093, -1093, -1093, -1093, -1093, -1093, -1093, -1093, 11008, -1093,
     732,   738,   733,   672,   672, -1093, -1093, -1093,   616,   260,
     266, 18051, 12360, 12529, 18131, 12698, 12867, -1093, 13036, 13205,
     311, -1093, -1093,   719, -1093,   739, -1093, -1093,  4709, 11008,
   19078, 11008, 19078,  7441, -1093,  4709, 11008, 19078, -1093, 11008,
   19078, -1093, 18213, 19078, -1093, 19078,   845,  4709,   725, -1093,
   -1093, -1093,   679, -1093,   712, 10328,   299, -1093,   174, -1093,
   -1093,  9310, 14443,  7781,  7781,  4709, -1093,    63,   727, 11008,
   -1093, 11008, -1093, 19078,  4709, 11008, -1093, 19078,  4709, 19078,
     -34, 10497, 11515, 11008, 11515, 11008, -1093, -1093,   737, -1093,
   -1093,  1717, -1093, 19581, -1093, 19078, -1093,   103,   120, -1093,
   18289, -1093, 14675, -1093, -1093, -1093, 11008, 11008, 11008, 11008,
   11008, 11008, 11008, 11008, -1093, -1093,  2746, -1093, 19293, 17425,
   14913, 14989, -1093, 17425, 15069, 15145, 11008,  4709, -1093, -1093,
     299,   679,  9141, -1093, -1093, -1093,   128,  9650, -1093, -1093,
     151, 11008,   -41, 18370, -1093,   611,   744,   745,   564, -1093,
     514, 19078, 15225, -1093, 15301, -1093, -1093, -1093, 19078,   746,
     747,   750,   754, -1093, -1093, -1093, 13374,   798,   316, -1093,
     780,   781,   672,   672, 18451, 18527, 18607, 18683, 18763, 18839,
   19310, -1093, 19390, -1093,  4709,  4709,  4709,  4709, 19078, -1093,
   -1093, -1093,   299, 10668,    96, -1093, 19078, -1093,   186, -1093,
      -7, -1093,   398, 18922, -1093, -1093, -1093, 13205,   762,   765,
   -1093,  4709,  4709, -1093, -1093, -1093, -1093,  6419, -1093, -1093,
     237, -1093,   174, -1093, -1093, -1093, 11008, 11008, 11008, 11008,
   11008, 11008, -1093, -1093, 17425, 17425, 17425, 17425, -1093, -1093,
   -1093, -1093, -1093,   263,  9310, 14007, -1093, 11008,   151,   186,
     186,   186,   186,   186,   186,   151,   788, -1093, -1093, 17425,
   17425,   761, 13543,   141,    62, 18998, -1093, -1093, 19078, -1093,
   -1093, -1093, -1093, -1093, -1093, -1093,   741, -1093, -1093,   369,
   14175, -1093, -1093, -1093, 10839, -1093,   527, -1093
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   113,   620,   621,   622,   616,
     617,   623,     0,   541,    99,   134,   510,   141,   512,   541,
       0,   140,     0,   415,    99,     0,     0,     0,   240,   135,
     588,   588,   614,     0,     0,     0,     0,   139,    55,   241,
     292,   136,     0,     0,     0,   288,     0,     0,   143,     0,
     560,   532,   624,   144,     0,   293,   504,   414,     0,     0,
       0,   157,    48,   138,   513,   416,     0,     0,     0,     0,
     508,     0,     0,   142,     0,     0,   114,     0,   615,     0,
       0,     0,   137,   274,   506,   418,   145,     0,     0,   673,
       0,   675,     0,   676,   677,   587,     0,   674,   671,   491,
     154,   672,     0,     0,     0,     0,     4,     0,     5,     0,
       9,    50,    10,    11,     0,    12,    13,    14,    16,   487,
     488,    26,    15,   155,   164,   165,    17,    21,    18,    20,
       0,   235,    19,   579,    23,    24,    25,    22,   163,     0,
     161,     0,   576,     0,   159,   162,     0,   160,   593,   572,
     489,   573,   494,   492,     0,     0,     0,   577,   578,     0,
     493,     0,   594,   595,   596,   618,   619,   571,   496,   495,
     574,   575,     0,    42,    28,   502,     0,     0,   542,   100,
       0,     0,   512,   135,     0,     0,     0,     0,   513,     0,
       0,     0,     0,   576,   593,   492,   577,   578,   511,   493,
     594,   595,     0,   541,     0,     0,   417,     0,   248,     0,
     472,     0,   479,   616,   513,   589,   291,   616,   182,   513,
       0,   291,     0,     0,     0,     0,     0,     0,     0,   169,
       0,     0,    52,   479,   107,   115,   127,   121,   120,   129,
     110,   119,   130,   116,   131,   108,   132,   125,   118,   126,
     124,   122,   123,   109,   111,   117,   128,   133,     0,   112,
       0,     0,     0,     0,     0,     0,   421,     0,   151,    36,
       0,   658,   564,   561,   562,   563,     0,   505,   659,     7,
     479,   291,   272,   282,   588,   273,   156,   386,   469,     0,
     468,     0,     0,   152,   592,     0,     0,    39,     0,   509,
     497,     0,   479,    40,   503,     0,   255,   251,     0,   493,
     255,   252,     0,   411,     0,   507,     0,     0,   660,   662,
     585,   657,   656,     0,    57,    60,     0,     0,   474,     0,
     476,     0,     0,   475,     0,     0,   468,     0,   586,     0,
       6,     0,    51,     0,     0,     0,     0,   275,     0,   373,
     372,   294,     0,   490,    27,     0,   565,     0,     0,     0,
       0,     0,     0,   661,     0,     0,     0,     0,     0,     0,
     584,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   331,   338,   339,
     340,   335,   337,     0,     0,   333,   336,   334,   332,     0,
     342,   341,     0,     0,   479,   479,     0,     0,     0,    29,
      30,     0,     0,     0,     0,     0,     0,     0,    31,     0,
       0,     0,     0,    32,     0,    33,     0,   487,   485,     0,
     480,   481,   486,   176,     0,   179,     0,   172,     0,     0,
     178,     0,     0,     0,   191,     0,     0,   190,     0,   199,
       0,     0,     0,   197,     0,     0,    72,   166,     0,   331,
     211,     0,    54,    53,    50,     0,     0,     0,   219,    34,
     355,   289,   425,     0,   426,   428,     0,   450,     0,   431,
       0,     0,   150,    35,     0,     0,    37,     0,   158,     0,
      92,   590,   591,   153,     0,    38,   479,     0,   262,   253,
     249,   254,   250,     0,   409,   406,   184,    41,    59,    58,
      61,     0,   625,     0,     0,   610,     0,   612,     0,     0,
       0,     0,     0,     0,     0,     0,   629,     8,     0,    44,
       0,     0,    90,     0,    87,     0,    66,   246,     0,     0,
       0,   366,   420,   540,   653,   652,   655,   664,   663,   668,
     667,   649,   646,   647,   648,   581,   636,   113,     0,   607,
     608,   114,   606,   605,   582,   640,   651,   645,   643,   654,
     644,   642,   634,   639,   641,   650,   633,   637,   638,   635,
     583,     0,     0,     0,     0,     0,     0,     0,     0,   666,
     665,   670,   669,   552,   553,   555,   557,     0,   544,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   627,
     246,   541,   541,   187,   407,   419,   473,     0,     0,   499,
       0,     0,   275,     0,     0,   275,   408,     0,     0,     0,
       0,   516,     0,     0,     0,   200,     0,   522,     0,     0,
     198,   672,    75,     0,    62,    73,     0,   210,     0,     0,
       0,     0,   498,     0,   222,     0,   220,   360,   357,   358,
     359,   363,   364,   365,   355,     0,   347,     0,   356,   374,
       0,   429,     0,   148,   149,   147,   146,     0,   449,   448,
     572,     0,   423,   570,   422,     0,     0,   604,   471,   470,
       0,     0,     0,   500,     0,   256,   413,   572,     0,   626,
     580,   611,   477,   613,   478,   207,     0,     0,     0,   628,
     205,   526,     0,   631,   630,     0,    46,    45,    43,     0,
      86,     0,     0,     0,    79,     0,     0,    72,   243,     0,
     276,     0,     0,   286,     0,   283,   370,   367,   368,   371,
     295,     0,     0,    98,    96,    97,    95,    94,    93,   602,
     603,   554,   556,     0,   543,   134,   141,   140,   139,   136,
     143,   144,   138,   142,   137,   145,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   183,   483,   484,   482,   275,     0,   181,   275,     0,
     180,     0,   209,     0,     0,   189,     0,   188,     0,   547,
       0,     0,   195,     0,     0,   193,     0,     0,   167,     0,
     168,     0,     0,   213,     0,   215,     0,   245,     0,     0,
     361,   362,     0,   355,   345,     0,   463,   375,   378,   377,
     379,     0,   427,   430,   431,     0,     0,   432,   433,     0,
       0,   264,     0,     0,   263,   266,   501,     0,   257,   260,
       0,   410,   208,     0,     0,     0,     0,   206,     0,    91,
      88,     0,    69,    68,    67,     0,     0,     0,     0,   291,
     281,     0,   284,   280,   369,   374,   343,   101,   326,   115,
     324,   121,   120,   104,   119,   116,   329,   131,   102,   132,
     118,   122,   103,   105,   117,   133,   323,   305,   308,   306,
     307,   318,   309,   322,   314,   312,   325,   328,   313,   311,
     316,   321,   310,   315,   319,   320,   317,   327,     0,   304,
       0,   106,     0,   343,   343,   302,   609,   545,   431,   593,
     593,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,   186,   185,     0,   277,     0,   277,   192,     0,     0,
     515,     0,   514,     0,   546,     0,     0,   521,   196,     0,
     520,   194,    64,    63,   212,   531,   214,     0,     0,   242,
     221,   218,   463,   348,     0,     0,   431,   376,   390,   424,
     454,     0,   627,   479,   479,     0,   268,     0,     0,     0,
     258,     0,   203,   528,     0,     0,   201,   527,     0,   632,
       0,     0,     0,    72,     0,    72,    80,    83,   247,   271,
     157,   291,   270,   291,   278,   287,   285,     0,   355,   301,
       0,   330,     0,   297,   298,   549,     0,     0,     0,     0,
       0,     0,     0,     0,   247,   277,   291,   277,   291,   519,
       0,     0,   548,   525,     0,     0,     0,     0,   217,    56,
     431,   463,     0,   466,   465,   467,   572,   387,   351,   349,
       0,     0,     0,     0,   452,   572,     0,     0,   269,   267,
       0,   261,     0,   204,     0,   202,    89,    71,    70,     0,
       0,     0,     0,   244,   279,   445,     0,   380,     0,   303,
     101,   103,   343,   343,     0,     0,     0,     0,     0,     0,
     291,   175,   291,   171,     0,     0,     0,     0,    65,   216,
     352,   350,   431,   455,     0,   389,   388,   404,     0,   405,
     392,   395,     0,   391,   384,   385,   290,     0,   566,   567,
     259,     0,     0,    82,    85,    81,    84,     0,   444,   443,
     572,   381,   390,   344,   299,   300,     0,     0,     0,     0,
       0,     0,   177,   173,   518,   517,   524,   523,   354,   353,
     457,   458,   460,   572,     0,   627,   403,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   572,   568,   569,   530,
     529,     0,   435,     0,     0,     0,   459,   461,   394,   396,
     397,   400,   401,   402,   398,   399,   393,   440,   438,   572,
     627,   382,   296,   383,   455,   439,   572,   462
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1093, -1093, -1093,    13,  -406,  2196, -1093, -1093, -1093,   454,
   -1093, -1093, -1093,   262,   610,  -446, -1093,  -700,  -696, -1093,
   -1093, -1093,   215, -1093, -1093,   255,   916, -1093,  2268,  -159,
    -724, -1093,  -888,   428, -1028,  -837, -1093,   -61, -1093, -1093,
   -1093, -1093, -1093, -1093, -1093, -1093,   214, -1093, -1093, -1093,
     135,  1149, -1093, -1093, -1093, -1093, -1093,   642, -1093,   108,
   -1093, -1093, -1093, -1093, -1093, -1093,  -573,  -660, -1093, -1093,
   -1093,    86,  1246, -1093, -1093, -1093,   386, -1093, -1093, -1093,
   -1093,   -63,  -157,  -885, -1093,   -58,   138,   298, -1093, -1093,
   -1093,    88, -1093, -1093,  -219,    28, -1002,  -178,  -209,  -201,
    -281, -1093,  -195, -1093,     6,   933,  -126,   503, -1093,  -468,
    -808,  -927, -1093,  -629,  -487, -1092, -1062,  -902,   -64, -1093,
     134, -1093,  -220,  -454,   353,   529,  -442, -1093, -1093, -1093,
    1447, -1093,   -13, -1093, -1093,  -216, -1093,  -627, -1093, -1093,
   -1093,  1660,  1707,   -12,   945,    57,  1025, -1093,  1874,  2222,
   -1093, -1093, -1093, -1093, -1093, -1093, -1093, -1093, -1093,  -377
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   325,   107,   647,   109,   110,   111,   343,
     464,   112,   232,   113,   326,   642,   535,   646,   643,   114,
     115,   116,   532,   533,   117,   118,   180,   919,   264,   119,
     259,   120,   678,   269,   121,   122,   281,   123,   124,   125,
     438,   624,   434,   621,   126,   127,   776,   128,   129,   655,
     656,   192,   131,   132,   133,   134,   135,   499,   695,   850,
     136,   137,   691,   845,   138,   139,   539,   869,   140,   141,
     734,   735,   193,   262,   669,   143,   144,   541,   875,   740,
     922,   923,   461,  1019,   471,   665,   666,   667,   668,   741,
     351,   831,  1142,  1202,  1126,   428,  1058,  1062,  1120,  1121,
    1122,   145,   314,   504,   146,   147,   265,   266,   475,   476,
     682,  1139,  1087,   479,   679,  1161,  1055,   976,   327,   209,
     331,   332,   429,   430,   431,   194,   149,   150,   151,   152,
     195,   154,   177,   178,   597,   450,   798,   598,   599,   155,
     156,   196,   197,   159,   221,   432,   199,   161,   200,   201,
     164,   165,   166,   167,   337,   168,   169,   170,   171,   172
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     175,   286,   289,   403,   198,   670,   202,   799,   203,   421,
     645,   454,   653,   465,   210,   106,   696,   921,   215,   215,
     352,   225,   227,   230,   231,   439,   979,   866,   205,   267,
     865,   601,  1014,   801,  1125,   270,   680,   271,  1023,  1024,
     335,   477,   276,   834,   277,   690,   278,    67,  1110,   786,
     608,  1162,   789,   427,   290,   294,   296,   298,   299,   300,
     487,   697,   267,   304,   664,   305,  -240,   308,   312,   520,
    1050,   751,   315,  1186,   427,   316,   317,   511,   318,   422,
     319,   349,   497,   320,   321,   448,   322,  1053,  1002,   820,
     290,   294,   336,   338,   288,   267,   477,   794,   872,  -241,
    -223,   477,   512,  1187,  1085,   525,   344,  1167,   807,   350,
    1158,   752,  1207,   448,   404,  -265,   405,   448,   519,   523,
    1025,   427,  1003,   873,   295,   417,   927,   415,   557,   821,
     288,   330,   288,  1076,   526,  1168,   981,   928,   448,   448,
     518,  -446,  1164,   427,   356,   558,   521,   657,   462,  1112,
      67,   463,   403,   775,  1117,   480,  1125,  -265,   559,  -446,
     333,  -446,  -441,   417,   557,   521,   658,   417,  1059,   175,
     659,   345,   411,   412,   413,   609,  1084,   304,   290,   336,
     212,   478,  -446,     3,   587,   588,   560,   608,  -446,  1117,
     420,  -441,   660,   853,   522,   661,   417,    59,  1138,  1014,
     294,  1014,   978,   833,  -441,   489,   662,  1144,  1145,  -446,
     563,  -265,   943,   233,  -446,   945,  -446,  -441,   288,   288,
     488,   294,   436,  1201,   561,   663,   514,   441,   417,   417,
     562,   417,   521,  1060,   205,   424,   478,  -446,   521,  1118,
     842,   478,  1111,   173,  1086,   427,   427,   224,   226,  -464,
    1119,   481,   736,   515,   425,  1198,  -446,   601,   489,   481,
     561,   982,  -346,  1014,  -441,  1014,  -446,  1165,   294,  -441,
    -464,   179,   215,  1205,  -464,   867,   692,   490,   542,   491,
     492,   645,   843,   521,  1036,  1119,  1038,   516,  1061,  -346,
     294,   861,   753,   173,   489,   620,  -442,  -464,   921,   508,
     737,   818,   823,  1080,  1159,  1082,  1079,   427,  1081,   268,
     623,   738,  1200,   620,   517,   627,   728,   513,   334,   754,
     176,   792,   817,   729,   940,  -442,  1042,   272,   819,   824,
     739,   268,   534,   536,   307,   311,   844,   427,  -442,   510,
     937,   306,   310,   543,  -224,   544,   545,   546,   548,   550,
     551,  -442,   552,   553,   554,   555,   556,   974,   564,   565,
     566,   567,   568,   569,   570,   571,   572,   573,   574,   575,
     576,   577,   578,   579,   580,  1100,   601,  1102,    23,   629,
     557,   294,   294,   273,  -456,   481,   953,   294,   827,   620,
     294,   294,   294,   294,   590,   592,   600,   558,  -442,  -230,
      40,   274,  -551,  -442,   615,  -456,   719,   612,  -550,  -456,
     559,   359,   616,   954,   617,  1000,  1008,   664,   275,   363,
     681,  -229,   803,   806,    55,   720,    57,   369,  -434,  -551,
     206,   290,  -456,   631,   633,  -550,   828,    65,   560,   637,
     639,   212,  -238,   644,   644,  1057,   648,   829,   427,   650,
     582,   583,    23,   620,   294,  -228,   584,  -434,   823,   585,
     586,   427,    85,  1013,   418,   360,   830,  -225,   683,   361,
    -434,   288,   685,   686,   730,   723,   561,   689,   233,   481,
    1034,  -232,   562,  -434,   294,  1143,   268,   268,   268,   268,
    -236,   854,   856,   460,   724,  1064,   481,   297,   787,   689,
      57,  -412,   294,   851,   294,    23,  -107,  -233,   708,   689,
     711,    65,   713,   715,  -113,   790,   363,   688,  -537,  1169,
    -412,   367,   468,   279,   369,  -600,   290,  -600,   372,  -109,
    -434,   148,   268,  1056,   280,  -434,    85,  -114,   268,   688,
    1065,   148,   330,   510,   330,  1170,   406,  1171,  1172,   688,
     407,  1173,  1174,    57,   148,   173,   148,   645,  -227,   645,
     268,   716,   860,   563,    65,  -601,   288,  -601,   301,   -78,
     427,   333,  -447,   333,   547,   549,   601,   768,   268,  -231,
    -600,   407,  -600,   204,   924,  -447,  -436,   -49,   302,    85,
     631,   771,   637,   774,   648,   294,   777,   711,   779,   780,
    -237,  -597,   435,  -597,   148,   -49,   294,   440,   294,   417,
    -239,   467,   664,   836,  -447,  -436,   791,  -226,   793,   427,
     589,   591,   600,  -599,   800,  -599,  -234,  -598,  -436,  -598,
    1013,   148,  1013,   611,   -78,   341,   148,   363,   342,   346,
    -447,  -436,   367,  -534,  1140,   369,   782,  -447,   702,   372,
     704,   353,   444,   447,   449,   453,  -533,  -451,   354,   355,
     632,  -538,   408,   300,   304,   336,   638,  -539,  -447,   294,
    -451,  1163,  -536,  -535,   410,   783,   416,   423,   433,   426,
     417,   359,   268,   437,   527,  1176,   443,  -447,  -436,   466,
     470,   484,  -447,  -436,  1013,  -447,  1013,   493,   483,  -451,
     485,   496,   503,   505,   486,   288,   148,   534,   531,   538,
     540,   862,   268,   864,   644,   644,   268,   294,   369,   581,
     607,   614,   618,  1163,   619,  -451,   495,   626,   620,   628,
     634,   649,  -451,   671,   707,   360,   652,   681,   838,   361,
    1199,   600,   672,   694,   506,   687,   693,   698,   731,   706,
     712,   742,   778,  -451,   743,   548,   590,   931,  1206,   744,
     934,   745,  1163,  1066,  1067,   746,   294,   747,   748,   -74,
     807,   749,  -451,   808,   750,   810,   811,  -451,   816,   987,
    -451,   489,   950,   871,   952,   657,   363,   364,   832,   957,
     148,   367,   960,   368,   369,   962,   148,   963,   372,   841,
     965,   870,   846,   874,   658,   926,   379,   932,   659,   935,
     944,   938,   383,   384,   385,   946,   967,   770,   969,   773,
     975,   977,   980,   983,   427,   427,   984,   989,  1006,  1007,
     660,   290,   994,   661,  -453,   339,   998,  1166,  1009,   234,
    1018,   993,  -134,   997,   662,  -141,   999,  -453,   613,   644,
    -140,  -139,  -110,   235,   236,   148,   237,  -136,  -143,  1015,
    -108,   238,  -144,   663,  -138,  1093,  -111,  -142,  1021,   239,
     148,   288,  -137,  -145,  -112,   240,  -453,  1022,   635,  1047,
     241,  1051,   640,  1168,   242,  1035,  1049,   243,  1190,  1191,
    1192,  1193,  1194,  1195,   268,   268,  1070,   244,  1083,  1141,
     268,   268,  -453,   245,   246,  1037,  1020,  1133,  1134,  -453,
     247,  1135,   815,  1128,  1129,  1136,  -107,  -109,   651,   248,
     950,   952,  1177,   957,   960,  1178,   993,   997,   249,   250,
    -453,   251,  1197,   252,   859,   253,  1039,  1040,   254,  1041,
     207,   600,   255,  1043,  1044,   256,   148,  1045,   257,  -453,
     286,   530,   501,   970,  -453,   988,   848,  -453,  1016,  1092,
    1088,   973,   822,  1017,  1184,  1203,  1196,  1189,   229,  1063,
     711,   294,   294,   784,   148,   216,     0,  1071,     0,  1072,
     148,   547,   589,  1074,   684,     0,     0,   148,  1114,  1078,
     644,   644,   644,   644,     0,     0,     0,     0,     0,     0,
       0,     0,   417,  1054,   847,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1039,  1094,  1095,  1043,  1096,  1097,
    1098,  1099,     0,     0,     0,     0,     0,   160,     0,     0,
       0,     0,     0,     0,  1108,     0,     0,   160,     0,     0,
     336,   268,   268,     0,     0,  1116,     0,   148,     0,  1123,
     160,   148,   160,   268,     0,   460,     0,     0,     0,   148,
     795,   797,   460,     0,     0,   268,   802,   805,   268,     0,
       0,     0,   359,  1183,     0,     0,     0,     0,     0,     0,
     288,   971,     0,     0,     0,  1115,     0,     0,     0,     0,
       0,     0,  1154,  1155,  1156,  1157,     0,     0,     0,     0,
     160,     0,     0,     0,   309,   309,     0,     0,     0,     0,
       0,     0,   990,   359,     0,   997,     0,     0,     0,  1179,
    1180,     0,     0,     0,     0,   336,   360,   160,   766,     0,
     361,     0,   160,     0,  1154,  1155,  1156,  1157,  1179,  1180,
       0,  1160,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   130,  1185,   711,     0,  1188,     0,     0,     0,     0,
       0,   130,     0,     0,     0,   288,     0,   360,     0,     0,
       0,   361,     0,     0,   130,     0,   130,   363,   148,     0,
       0,     0,   367,     0,   368,   369,     0,     0,   711,   372,
       0,     0,   997,  1160,     0,   557,     0,   379,     0,     0,
       0,     0,   160,   383,   384,   385,     0,   941,   942,   755,
       0,     0,   756,     0,     0,     0,     0,   757,   363,   947,
       0,     0,     0,   367,   130,   368,   369,   183,     0,     0,
     372,   958,  1160,     0,   961,   148,   758,     0,   379,     0,
     657,   148,     0,   759,     0,     0,   385,     0,   142,   268,
       0,   130,     0,   760,     0,   359,   130,     0,   142,   658,
     761,     0,     0,   659,     0,     0,     0,     0,     0,     0,
     925,   142,     0,   142,     0,   762,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   660,   160,   763,   661,     0,
      59,   561,   160,     0,     0,     0,   929,   930,   764,   662,
       0,     0,   460,   460,   765,     0,   460,   460,     0,   360,
       0,     0,     0,   361,     0,     0,     0,     0,   663,     0,
       0,   142,     0,   148,  1124,   148,   130,     0,     0,     0,
     148,   359,  1130,   148,   460,     0,   460,     0,     0,     0,
     148,     0,     0,   148,     0,     0,     0,     0,   142,     0,
       0,   160,     0,   142,     0,     0,     0,     0,     0,     0,
     363,   364,     0,   365,   366,   367,   160,   368,   369,   370,
       0,   148,   372,   373,   374,     0,     0,   376,   377,   378,
     379,     0,   148,   382,   148,   360,   383,   384,   385,   361,
       0,     0,     0,     0,     0,     0,     0,   386,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     130,     0,     0,     0,     0,  1069,   130,     0,     0,     0,
       0,     0,     0,   142,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   363,   364,     0,   365,
     366,   367,   160,   368,   369,     0,  1124,     0,   372,   153,
       0,     0,     0,     0,     0,     0,   379,     0,     0,   153,
       0,   868,   383,   384,   385,     0,     0,     0,     0,     0,
     160,     0,   153,     0,   153,   130,   160,   148,     0,     0,
     359,     0,  -277,   160,   148,     0,  -277,  -277,     0,     0,
     130,     0,     0,     0,     0,  -277,   148,  -277,  -277,     0,
       0,     0,  -277,     0,     0,     0,     0,   142,     0,  -277,
       0,     0,  -277,   142,   148,     0,     0,     0,     0,     0,
       0,     0,   153,   148,     0,     0,     0,   148,     0,     0,
       0,     0,     0,  -277,   360,  -277,     0,  -277,   361,  -277,
    -277,     0,  -277,   160,  -277,     0,  -277,   160,     0,   153,
       0,     0,     0,     0,   153,   160,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -277,   130,     0,  -277,     0,
       0,  -277,   142,     0,     0,     0,   148,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,   142,   365,   366,
     367,     0,   368,   369,   130,     0,     0,   372,     0,     0,
     130,     0,     0,     0,   378,   379,     0,   130,   382,     0,
       0,   383,   384,   385,     0,     0,     0,     0,     0,     0,
       0,  -277,     0,     0,   153,     0,     0,  -277,     0,     0,
       0,     0,     0,   148,   148,   148,   148,     0,     0,   282,
       0,     0,     0,    22,    23,     0,     0,     0,     0,     0,
       0,     0,   283,     0,    30,   284,     0,     0,     0,    35,
     148,   148,   157,   142,     0,     0,    40,   130,     0,     0,
       0,   130,   157,     0,   160,     0,     0,     0,     0,   130,
       0,     0,     0,     0,     0,   157,     0,   157,     0,     0,
      55,   142,    57,     0,    59,     0,  1010,   142,     0,  1011,
       0,   285,     0,    65,   142,     0,     0,     0,   153,   158,
       0,     0,     0,     0,   153,     0,     0,     0,     0,   158,
       0,     0,    81,     0,     0,    83,     0,     0,    85,     0,
       0,   160,   158,     0,   158,   157,     0,   160,     0,   282,
       0,   359,     0,    22,    23,     0,     0,     0,     0,     0,
       0,     0,   283,     0,    30,   284,     0,     0,     0,    35,
       0,     0,   157,     0,   142,     0,    40,   157,   142,     0,
       0,     0,     0,   153,     0,     0,   142,     0,   100,     0,
       0,     0,   158,     0,  1012,     0,     0,     0,   153,     0,
      55,     0,    57,     0,     0,   360,     0,     0,   130,   361,
       0,   285,     0,    65,     0,   359,     0,     0,     0,   158,
       0,     0,     0,     0,   158,     0,     0,     0,     0,   160,
       0,   160,    81,     0,     0,    83,   160,     0,    85,   160,
       0,     0,     0,     0,     0,     0,   160,   157,     0,   160,
       0,     0,     0,     0,     0,     0,   363,   364,     0,     0,
     366,   367,     0,   368,   369,   130,     0,     0,   372,   360,
       0,   130,     0,   361,   153,     0,   379,   160,     0,     0,
       0,     0,   383,   384,   385,     0,   162,     0,   160,     0,
     160,     0,     0,     0,   158,     0,   162,     0,     0,     0,
       0,     0,   153,     0,     0,   142,     0,     0,   153,   162,
       0,   162,     0,     0,     0,   153,     0,     0,     0,     0,
     363,   364,     0,   365,   366,   367,     0,   368,   369,   370,
       0,   157,   372,     0,     0,     0,     0,   157,     0,   378,
     379,     0,     0,   382,     0,     0,   383,   384,   385,     0,
       0,     0,     0,   130,     0,   130,     0,   386,     0,   162,
     130,     0,   142,   130,     0,     0,     0,     0,   142,     0,
     130,     0,     0,   130,     0,   153,     0,     0,   158,   153,
       0,     0,     0,   160,   158,     0,   162,   153,     0,     0,
     160,   162,     0,     0,     0,     0,   157,     0,     0,     0,
       0,   130,   160,     0,     0,     0,     0,     0,     0,     0,
       0,   157,   130,     0,   130,     0,     0,     0,     0,     0,
     160,     0,     0,     0,     0,     0,     0,     0,     0,   160,
       0,     0,     0,   160,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   158,     0,     0,     0,     0,     0,     0,
     142,     0,   142,     0,     0,     0,     0,   142,   158,     0,
     142,   162,     0,     0,     0,     0,     0,   142,     0,     0,
     142,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   160,     0,     0,     0,     0,   157,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   282,   142,     0,
       0,    22,    23,     0,     0,     0,   153,   130,     0,   142,
     283,   142,    30,   284,   130,   157,     0,    35,     0,     0,
     -77,   157,     0,     0,    40,     0,   130,  -291,   157,     0,
       0,     0,     0,     0,   158,     0,     0,     0,     0,   160,
     160,   160,   160,     0,   130,   162,     0,     0,    55,     0,
      57,   162,     0,   130,     0,  -291,     0,   130,     0,   285,
       0,    65,   158,   153,     0,     0,   160,   160,   158,   153,
       0,     0,     0,     0,     0,   158,     0,     0,     0,     0,
      81,     0,     0,    83,     0,   -77,    85,     0,   157,     0,
       0,     0,   157,     0,     0,     0,     0,     0,     0,     0,
     157,     0,     0,     0,   142,     0,   130,     0,   108,     0,
     162,   142,     0,     0,     0,     0,     0,     0,   174,     0,
       0,     0,     0,   142,     0,   162,     0,     0,     0,     0,
       0,   208,     0,   211,   163,   158,     0,     0,     0,   158,
       0,   142,     0,     0,   163,     0,     0,   158,     0,     0,
     142,   153,     0,   153,   142,     0,     0,   163,   153,   163,
       0,   153,     0,   130,   130,   130,   130,     0,   153,     0,
       0,   153,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   303,     0,     0,     0,     0,     0,     0,     0,     0,
     130,   130,     0,     0,     0,     0,     0,     0,     0,   153,
       0,   162,     0,   142,     0,     0,     0,   163,   108,     0,
     153,     0,   153,   340,     0,     0,     0,     0,     0,   157,
     258,   260,   261,     0,     0,     0,     0,     0,     0,   162,
       0,     0,     0,     0,   163,   162,     0,     0,     0,   163,
       0,     0,   162,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   313,
     142,   142,   142,   142,     0,     0,   158,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   157,     0,     0,     0,
       0,     0,   157,   409,     0,     0,     0,   142,   142,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   162,     0,     0,   153,   162,     0,     0,   163,
       0,     0,   153,     0,   162,     0,     0,   347,     0,   348,
       0,     0,     0,   158,   153,     0,     0,     0,     0,   158,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   153,     0,     0,     0,     0,     0,     0,     0,
       0,   153,     0,     0,     0,   153,     0,     0,     0,     0,
       0,     0,     0,     0,   157,   414,   157,   469,     0,     0,
       0,   157,     0,   482,   157,     0,     0,     0,     0,     0,
       0,   157,   313,     0,   157,     0,     0,     0,     0,     0,
       0,     0,     0,   163,     0,     0,     0,     0,   313,   163,
     442,     0,     0,     0,   153,     0,     0,     0,     0,     0,
       0,   158,   157,   158,     0,     0,     0,     0,   158,     0,
       0,   158,     0,   157,     0,   157,     0,     0,   158,     0,
       0,   158,   108,   162,     0,     0,     0,     0,     0,     0,
       0,   474,     0,     0,     0,     0,     0,   108,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   163,   158,
       0,   153,   153,   153,   153,     0,     0,     0,     0,     0,
     158,     0,   158,   163,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   153,   153,
     162,     0,     0,     0,     0,     0,   162,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,     0,     0,     0,     0,     0,     0,   157,     0,
       0,     0,     0,   108,   537,   157,     0,     0,  -558,     0,
       0,     0,     0,     0,     0,     0,     0,   157,     0,     0,
       0,   402,     0,     0,  -597,     0,  -597,     0,     0,   163,
       0,   482,     0,     0,     0,   157,     0,   482,     0,     0,
       0,     0,     0,     0,   157,   158,     0,     0,   157,     0,
       0,     0,   158,     0,     0,     0,     0,   163,   162,     0,
     162,     0,     0,   163,   158,   162,     0,     0,   162,     0,
     163,     0,     0,     0,   610,   162,     0,     0,   162,     0,
       0,     0,   158,     0,     0,     0,     0,     0,     0,     0,
       0,   158,   622,     0,     0,   158,   625,   157,     0,     0,
       0,     0,     0,     0,   705,     0,   162,     0,   710,     0,
       0,     0,     0,     0,     0,     0,   108,   162,     0,   162,
       0,     0,     0,     0,     0,   654,     0,     0,     0,     0,
     163,   474,     0,     0,   163,     0,     0,     0,     0,     0,
       0,     0,   163,     0,   158,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   157,   157,   157,   157,   282,     0,
       0,     0,    22,    23,     0,     0,     0,     0,     0,     0,
       0,   283,     0,    30,   284,     0,     0,     0,    35,     0,
       0,   157,   157,     0,     0,    40,     0,     0,     0,   718,
       0,     0,     0,     0,     0,     0,     0,     0,   733,     0,
       0,   158,   158,   158,   158,     0,     0,     0,     0,    55,
       0,    57,   162,    59,     0,  1010,     0,     0,  1011,   162,
     285,     0,    65,     0,     0,     0,     0,     0,   158,   158,
       0,   162,     0,     0,     0,   813,     0,     0,     0,     0,
       0,    81,   234,     0,    83,     0,     0,    85,     0,   162,
       0,     0,     0,     0,     0,     0,   235,   236,   162,   237,
       0,   163,   162,     0,   238,     0,     0,     0,     0,     0,
       0,     0,   239,     0,     0,     0,     0,     0,   240,   785,
       0,     0,   788,   241,     0,     0,     0,   242,     0,     0,
     243,     0,   852,     0,     0,     0,     0,   100,   857,     0,
     244,     0,     0,  1101,     0,     0,   245,   246,     0,     0,
       0,   162,     0,   247,     0,     0,     0,     0,   163,     0,
       0,     0,   248,     0,   163,   826,     0,     0,     0,   474,
       0,   249,   250,     0,   251,     0,   252,     0,   253,     0,
       0,   254,     0,     0,     0,   255,   472,     0,   256,     0,
       0,   257,   849,     0,     0,     0,   313,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   162,   162,
     162,   162,     0,     0,     0,     0,     0,     0,     0,     0,
     482,     0,   482,     0,     0,     0,     0,   482,     0,     0,
     482,     0,     0,     0,     0,   162,   162,   964,     0,   920,
     966,     0,     0,     0,     0,     0,   163,   473,   163,     0,
       0,     0,     0,   163,     0,     0,   163,     0,     0,     0,
       0,     0,     0,   163,     0,     0,   163,     0,   986,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,   992,
       0,   996,     0,     0,   235,   236,     0,   237,     0,     0,
       0,     0,   238,     0,   163,     0,     0,     0,     0,     0,
     239,     0,     0,     0,     0,   163,   240,   163,     0,     0,
       0,   241,     0,     0,   968,   242,   654,     0,   243,     0,
     972,     0,     0,   474,     0,     0,     0,     0,   244,     0,
       0,     0,     0,     0,   245,   246,     0,     0,     0,     0,
       0,   247,     0,     0,     0,   849,     0,     0,     0,     0,
     248,     0,     0,     0,     0,     0,     0,     0,     0,   249,
     250,     0,   251,     0,   252,     0,   253,     0,   234,   254,
     733,     0,     0,   255,     0,     0,   256,     0,     0,   257,
       0,     0,   235,   236,     0,   237,     0,     0,     0,     0,
     238,     0,     0,  1048,     0,     0,     0,     0,   239,   357,
     163,     0,     0,     0,   240,     0,   358,   163,     0,   241,
       0,  1068,     0,   242,     0,     0,   243,     0,   359,   163,
    1073,     0,     0,     0,  1075,     0,   244,     0,     0,     0,
       0,     0,   245,   246,     0,   263,     0,   163,     0,   247,
       0,     0,     0,     0,     0,     0,   163,     0,   248,     0,
     163,     0,     0,     0,     0,     0,     0,   249,   250,     0,
     251,     0,   252,     0,   253,     0,     0,   254,     0,     0,
       0,   255,   360,  1109,   256,     0,   361,   257,     0,     0,
       0,     0,     0,     0,     0,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   163,
       0,     0,     0,     0,     0,     0,     0,   362,   459,   388,
     389,   390,   391,   392,     0,     0,   395,   396,   397,   398,
     920,   400,   401,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,   825,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,     0,   383,
     384,   385,     0,   417,     0,     0,   163,   163,   163,   163,
     386,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    -2,     4,     0,     5,     0,     6,     7,     8,     9,
      10,    11,     0,   163,   163,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,   -76,     0,    38,    39,
      40,     0,    41,  -291,     0,    42,    43,    44,    45,    46,
       0,    47,    48,    49,   -47,    50,    51,     0,    52,    53,
      54,     0,     0,     0,    55,    56,    57,    58,    59,    60,
      61,  -291,   -47,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,     0,    83,
      84,   -76,    85,    86,     0,     0,    87,     0,    88,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,     0,   103,
       0,   104,     0,     4,   105,     5,     0,     6,     7,     8,
       9,    10,    11,     0,  -627,     0,    12,    13,    14,    15,
      16,  -627,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,  -627,    27,    28,  -627,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,   -76,     0,    38,
      39,    40,     0,    41,  -291,     0,    42,    43,    44,    45,
      46,     0,    47,    48,    49,   -47,    50,    51,     0,    52,
      53,    54,     0,     0,     0,    55,    56,    57,    58,     0,
      60,    61,  -291,   -47,    62,    63,    64,  -627,    65,    66,
      67,  -627,    68,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,     0,
      83,    84,   -76,    85,    86,     0,     0,    87,     0,    88,
       0,     0,  -627,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -627,  -627,
      92,  -627,  -627,  -627,  -627,  -627,  -627,  -627,     0,  -627,
    -627,  -627,  -627,  -627,     0,  -627,  -627,  -627,  -627,  -627,
    -627,  -627,  -627,   100,  -627,  -627,  -627,     0,   102,  -627,
     103,     0,   104,     0,   323,  -627,     5,   287,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,   -76,     0,
      38,    39,    40,     0,    41,  -291,     0,    42,    43,    44,
      45,    46,     0,    47,    48,    49,   -47,    50,    51,     0,
      52,    53,    54,     0,     0,     0,    55,    56,    57,    58,
      59,    60,    61,  -291,   -47,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -76,    85,    86,     0,     0,    87,     0,
      88,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
     324,   103,     0,   104,     0,     4,   105,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,   -76,
       0,    38,    39,    40,     0,    41,  -291,     0,    42,    43,
      44,    45,    46,     0,    47,    48,    49,   -47,    50,    51,
       0,    52,    53,    54,     0,     0,     0,    55,    56,    57,
      58,    59,    60,    61,  -291,   -47,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,    75,     0,    76,    77,     0,    78,    79,    80,    81,
      82,     0,    83,    84,   -76,    85,    86,     0,     0,    87,
       0,    88,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   100,     0,     0,   101,     0,
     102,   509,   103,     0,   104,     0,   528,   105,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
     -76,     0,    38,    39,    40,     0,    41,  -291,     0,    42,
      43,    44,    45,    46,     0,    47,    48,    49,   -47,    50,
      51,     0,    52,    53,    54,     0,     0,     0,    55,    56,
      57,    58,    59,    60,    61,  -291,   -47,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -76,    85,    86,     0,     0,
      87,     0,    88,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,   529,   103,     0,   104,     0,   323,   105,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,   -76,     0,    38,    39,    40,     0,    41,  -291,     0,
      42,    43,    44,    45,    46,     0,    47,    48,    49,   -47,
      50,    51,     0,    52,    53,    54,     0,     0,     0,    55,
      56,    57,    58,    59,    60,    61,  -291,   -47,    62,    63,
      64,     0,    65,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,     0,    78,    79,
      80,    81,    82,     0,    83,    84,   -76,    85,    86,     0,
       0,    87,     0,    88,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,   100,     0,     0,
     101,     0,   102,   324,   103,     0,   104,     0,     4,   105,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,   -76,     0,    38,    39,    40,     0,    41,  -291,
       0,    42,    43,    44,    45,    46,     0,    47,    48,    49,
     -47,    50,    51,     0,    52,    53,    54,     0,     0,     0,
      55,    56,    57,    58,    59,    60,    61,  -291,   -47,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -76,    85,    86,
       0,     0,    87,     0,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,   717,   103,     0,   104,     0,     4,
     105,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,   -76,     0,    38,    39,    40,     0,    41,
    -291,     0,    42,    43,    44,    45,    46,     0,    47,    48,
      49,   -47,    50,    51,     0,    52,    53,    54,     0,     0,
       0,    55,    56,    57,    58,   339,    60,    61,  -291,   -47,
      62,    63,    64,     0,    65,    66,    67,     0,    68,    69,
      70,    71,    72,    73,    74,    75,     0,    76,    77,     0,
      78,    79,    80,    81,    82,     0,    83,    84,   -76,    85,
      86,     0,     0,    87,     0,    88,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,   100,
       0,     0,   101,     0,   102,     0,   103,     0,   104,     0,
       4,   105,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,   -76,     0,    38,    39,    40,     0,
      41,  -291,     0,    42,    43,    44,    45,    46,     0,    47,
      48,    49,   -47,    50,    51,     0,    52,    53,    54,     0,
       0,     0,    55,    56,    57,    58,     0,    60,    61,  -291,
     -47,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,    75,     0,    76,    77,
       0,    78,    79,    80,    81,    82,     0,    83,    84,   -76,
      85,    86,     0,     0,    87,     0,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,     0,   103,     0,   104,
       0,     4,   105,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,   630,    37,   -76,     0,    38,    39,    40,
       0,    41,  -291,     0,    42,    43,    44,    45,    46,     0,
      47,    48,    49,   -47,    50,    51,     0,    52,    53,    54,
       0,     0,     0,    55,    56,    57,    58,     0,    60,    61,
    -291,   -47,    62,    63,    64,     0,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,     0,    83,    84,
     -76,    85,    86,     0,     0,    87,     0,    88,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,   100,     0,     0,   101,     0,   102,     0,   103,     0,
     104,     0,     4,   105,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,   636,    37,   -76,     0,    38,    39,
      40,     0,    41,  -291,     0,    42,    43,    44,    45,    46,
       0,    47,    48,    49,   -47,    50,    51,     0,    52,    53,
      54,     0,     0,     0,    55,    56,    57,    58,     0,    60,
      61,  -291,   -47,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,     0,    83,
      84,   -76,    85,    86,     0,     0,    87,     0,    88,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,     0,   103,
       0,   104,     0,     4,   105,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,   949,    37,   -76,     0,    38,
      39,    40,     0,    41,  -291,     0,    42,    43,    44,    45,
      46,     0,    47,    48,    49,   -47,    50,    51,     0,    52,
      53,    54,     0,     0,     0,    55,    56,    57,    58,     0,
      60,    61,  -291,   -47,    62,    63,    64,     0,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,     0,
      83,    84,   -76,    85,    86,     0,     0,    87,     0,    88,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,   100,     0,     0,   101,     0,   102,     0,
     103,     0,   104,     0,     4,   105,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,   951,    37,   -76,     0,
      38,    39,    40,     0,    41,  -291,     0,    42,    43,    44,
      45,    46,     0,    47,    48,    49,   -47,    50,    51,     0,
      52,    53,    54,     0,     0,     0,    55,    56,    57,    58,
       0,    60,    61,  -291,   -47,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -76,    85,    86,     0,     0,    87,     0,
      88,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
       0,   103,     0,   104,     0,     4,   105,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,   956,    37,   -76,
       0,    38,    39,    40,     0,    41,  -291,     0,    42,    43,
      44,    45,    46,     0,    47,    48,    49,   -47,    50,    51,
       0,    52,    53,    54,     0,     0,     0,    55,    56,    57,
      58,     0,    60,    61,  -291,   -47,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,    75,     0,    76,    77,     0,    78,    79,    80,    81,
      82,     0,    83,    84,   -76,    85,    86,     0,     0,    87,
       0,    88,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   100,     0,     0,   101,     0,
     102,     0,   103,     0,   104,     0,     4,   105,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,   959,    37,
     -76,     0,    38,    39,    40,     0,    41,  -291,     0,    42,
      43,    44,    45,    46,     0,    47,    48,    49,   -47,    50,
      51,     0,    52,    53,    54,     0,     0,     0,    55,    56,
      57,    58,     0,    60,    61,  -291,   -47,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -76,    85,    86,     0,     0,
      87,     0,    88,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,     0,   103,     0,   104,     0,     4,   105,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,   985,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,   -76,     0,    38,    39,    40,     0,    41,  -291,     0,
      42,    43,    44,    45,    46,     0,    47,    48,    49,   -47,
      50,    51,     0,    52,    53,    54,     0,     0,     0,    55,
      56,    57,    58,     0,    60,    61,  -291,   -47,    62,    63,
      64,     0,    65,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,     0,    78,    79,
      80,    81,    82,     0,    83,    84,   -76,    85,    86,     0,
       0,    87,     0,    88,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,   100,     0,     0,
     101,     0,   102,     0,   103,     0,   104,     0,     4,   105,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
     991,    37,   -76,     0,    38,    39,    40,     0,    41,  -291,
       0,    42,    43,    44,    45,    46,     0,    47,    48,    49,
     -47,    50,    51,     0,    52,    53,    54,     0,     0,     0,
      55,    56,    57,    58,     0,    60,    61,  -291,   -47,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -76,    85,    86,
       0,     0,    87,     0,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,     0,   103,     0,   104,     0,     4,
     105,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,   995,    37,   -76,     0,    38,    39,    40,     0,    41,
    -291,     0,    42,    43,    44,    45,    46,     0,    47,    48,
      49,   -47,    50,    51,     0,    52,    53,    54,     0,     0,
       0,    55,    56,    57,    58,     0,    60,    61,  -291,   -47,
      62,    63,    64,     0,    65,    66,    67,     0,    68,    69,
      70,    71,    72,    73,    74,    75,     0,    76,    77,     0,
      78,    79,    80,    81,    82,     0,    83,    84,   -76,    85,
      86,     0,     0,    87,     0,    88,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,   100,
       0,     0,   101,     0,   102,     0,   103,     0,   104,     0,
    1181,   105,     5,   287,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   186,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,     0,   191,
    1182,   835,   105,     5,   287,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   190,     0,   103,     0,
     191,     0,     5,   105,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,   228,    23,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   186,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,    57,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,    65,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
      85,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,     0,   191,
       0,     0,   105,     5,   287,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,     0,    78,   291,   292,     0,    82,   328,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,   329,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   190,     0,   103,     0,
     191,     0,     0,   105,     5,   287,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,   291,   292,     0,    82,   328,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
     701,   191,     0,     0,   105,     5,   287,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   183,     0,     0,
       0,    32,   184,   185,     0,   186,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,     0,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,   291,   292,     0,    82,   328,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,   703,   191,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,   593,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,   594,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,   595,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,   596,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,   191,     0,     5,   105,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,     0,    78,   291,   292,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,   293,     0,     0,   101,     0,   190,     0,   103,     0,
     191,     0,     0,   105,     5,   287,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,   291,   292,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,   191,     0,     5,   105,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,   837,    52,    53,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,     0,    78,   291,   292,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   190,     0,   103,     0,
     191,     0,     5,   105,     6,     7,     8,   217,    10,    11,
     218,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   186,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   219,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,   220,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,     0,   191,
       0,     5,   105,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,   222,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,   223,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,     0,   191,     0,
       0,   105,     5,   287,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   186,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,   223,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,     0,   191,
       0,     5,   105,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,   291,   292,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,     0,   191,     0,
       0,   105,     5,   287,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   186,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,   699,   103,     0,   191,
       0,     0,   105,     5,   287,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   190,     0,   103,     0,
     191,   709,     0,   105,     5,   287,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,   191,  1113,     5,   105,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,   223,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   190,     0,   103,     0,
     191,     0,     5,   105,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,   267,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   186,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   102,     0,   103,     0,   191,
       0,     0,   105,     5,   287,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   190,     0,   103,     0,
     191,     0,     5,   105,     6,     7,     8,   217,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   186,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   219,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,   220,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,     0,   191,
       0,     5,   105,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,     0,   191,   714,
       5,   105,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,   863,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     0,
     105,     5,   287,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,   673,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,   674,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,   675,    72,    73,    74,   676,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,     0,  1052,     0,
       5,   105,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,  1077,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     0,
     105,     5,   287,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,     0,  1052,     0,
       0,   105,     5,   287,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,  1033,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,     0,   191,
       0,     5,   105,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,     0,   191,     0,
       5,   105,     6,     7,     8,   213,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   214,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   102,     0,   103,     0,   191,     0,     5,   105,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   641,
       0,   190,     0,   103,     0,   191,     0,     5,   105,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   181,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   673,     0,   183,
       0,     0,     0,    32,   184,   185,     0,   186,    37,     0,
       0,     0,   674,     0,     0,    41,     0,     0,   187,     0,
       0,    45,    46,     0,     0,    48,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,   188,     0,
       0,     0,     0,     0,     0,     0,    70,   675,    72,    73,
      74,   676,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     190,     0,   103,     0,   677,     0,     5,   105,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   181,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   183,     0,
       0,     0,    32,   184,   185,     0,   186,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,   187,     0,     0,
      45,    46,     0,     0,    48,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   190,
       0,   103,     0,   677,     0,     5,   105,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   183,     0,     0,
       0,    32,   184,   185,     0,   769,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,     0,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,   191,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   772,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,   191,     0,     5,   105,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,  1027,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   190,     0,   103,     0,
     191,     0,     5,   105,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,  1028,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,     0,   191,
       0,     5,   105,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,  1030,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,     0,   191,     0,
       5,   105,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
    1031,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,  1032,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,   191,     0,     5,   105,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,  1033,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   190,     0,   103,     0,   191,     0,     5,   105,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   181,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   673,     0,   183,
       0,     0,     0,    32,   184,   185,     0,   186,    37,     0,
       0,     0,   674,     0,     0,    41,     0,     0,   187,     0,
       0,    45,    46,     0,     0,    48,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,   188,     0,
       0,     0,     0,     0,     0,     0,    70,   675,    72,    73,
      74,   676,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     190,     0,   103,     0,  1137,     0,     5,   105,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   181,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   183,     0,
       0,     0,    32,   184,   185,     0,   186,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,   187,     0,     0,
      45,    46,     0,     0,    48,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   876,     0,   877,    90,
      91,    92,    93,    94,     0,     0,     0,   878,    95,     0,
       0,     0,   879,   236,   880,   881,     0,     0,    96,     0,
     882,    97,    98,    99,     0,     0,     0,   101,   239,     0,
     183,   103,     0,  1137,   883,     0,   105,     0,     0,   884,
       0,     0,     0,   242,     0,     0,   885,     0,   886,     0,
       0,     0,     0,     0,     0,     0,   887,     0,     0,     0,
       0,     0,   888,   889,     0,     0,     0,     0,     0,   247,
       0,     0,     0,     0,     0,     0,     0,     0,   890,     0,
       0,     0,     0,     0,     0,     0,     0,   249,   250,     0,
     891,     0,   252,     0,   892,     0,     0,   893,     0,     0,
       0,   894,     0,     0,   256,     0,     0,   895,     0,     0,
       0,     0,     0,     0,     0,     0,   459,   388,   389,   390,
     391,   392,     0,     0,   395,   396,   397,   398,     0,   400,
     401,   896,   897,   898,   899,   900,     0,     0,   901,     0,
       0,     0,   902,   903,   904,   905,   906,   907,   908,   909,
     910,   911,   912,     0,   913,     0,     0,   914,   915,   916,
     917,     0,     5,   918,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   186,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,   387,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,   402,    98,    99,
    -597,     0,  -597,   101,     0,   190,     0,   103,     0,   191,
       5,   287,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,  -455,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,  -455,
       0,     0,     0,  -455,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,   190,     0,   103,  -455,  1052,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,  -437,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,  -437,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,  -437,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,  -437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,    92,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,    99,  -437,     0,     0,     0,
       0,  -437,    28,   103,   183,  1137,     0,     0,    32,   184,
     185,     0,   186,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,     5,    92,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   181,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,    99,
       0,     0,     0,     0,     0,   190,    28,   103,   183,   191,
       0,     0,    32,   184,   185,     0,   186,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,   187,     0,     0,
      45,    46,     0,     0,    48,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,    92,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,    99,     0,     0,     0,     0,     0,   190,
      28,   103,   183,   677,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,  1090,    78,
       0,     0,     0,    82,     0,     0,    84,   878,     0,    86,
       0,     0,   235,   236,   880,   237,     0,     0,     0,     0,
     238,     0,     0,     0,     0,     0,     0,     0,   239,     0,
       0,     0,     0,     0,   883,    92,     0,     0,     0,   241,
       0,     0,     0,   242,     0,     0,   243,     0,   886,     0,
       0,     0,     0,     0,     0,     0,   244,     0,     0,     0,
       0,     0,   888,   246,     0,   103,     0,   191,     0,   247,
       0,     0,     0,     0,     0,     0,     0,     0,   248,     0,
       0,     0,     0,     0,     0,     0,     0,   249,   250,   357,
     251,     0,   252,     0,  1091,     0,   358,   893,     0,     0,
       0,   255,     0,     0,   256,     0,     0,   257,   359,     0,
       0,     0,     0,     0,     0,     0,   459,   388,   389,   390,
     391,   392,     0,     0,   395,   396,   397,   398,     0,   400,
     401,   896,   897,   898,   899,   900,     0,     0,   901,     0,
       0,     0,   902,   903,   904,   905,   906,   907,   908,   909,
     910,   911,   912,     0,   913,     0,     0,   914,   915,   916,
     917,     0,   360,     0,     0,   357,   361,     0,     0,     0,
       0,     0,   358,     0,     0,   948,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   362,   459,   388,
     389,   390,   391,   392,     0,     0,   395,   396,   397,   398,
       0,   400,   401,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,   360,   383,
     384,   385,   361,   417,     0,   357,     0,     0,     0,     0,
     386,   955,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,     0,
       0,     0,     0,   362,   459,   388,   389,   390,   391,   392,
       0,     0,   395,   396,   397,   398,     0,   400,   401,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,     0,   383,   384,   385,   360,   417,
       0,   357,   361,     0,     0,     0,   386,     0,   358,     0,
       0,  1104,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   362,   459,   388,   389,   390,   391,   392,
       0,     0,   395,   396,   397,   398,     0,   400,   401,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,   360,   383,   384,   385,   361,   417,
       0,   357,     0,     0,     0,     0,   386,  1105,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,     0,   362,
     459,   388,   389,   390,   391,   392,     0,     0,   395,   396,
     397,   398,     0,   400,   401,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
       0,   383,   384,   385,   360,   417,     0,   357,   361,     0,
       0,     0,   386,     0,   358,     0,     0,  1106,     0,     0,
       0,     0,     0,     0,     0,     0,   359,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   362,
     459,   388,   389,   390,   391,   392,     0,     0,   395,   396,
     397,   398,     0,   400,   401,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
     360,   383,   384,   385,   361,   417,     0,   357,     0,     0,
       0,     0,   386,  1107,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,     0,     0,     0,
       0,     0,     0,     0,     0,   362,   459,   388,   389,   390,
     391,   392,     0,     0,   395,   396,   397,   398,     0,   400,
     401,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,     0,   383,   384,   385,
     360,   417,     0,   357,   361,     0,     0,     0,   386,     0,
     358,     0,     0,  1131,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   362,   459,   388,   389,   390,
     391,   392,     0,     0,   395,   396,   397,   398,     0,   400,
     401,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,   360,   383,   384,   385,
     361,   417,     0,     0,     0,   357,     0,     0,   386,  1132,
       0,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,   451,     0,     0,     0,     0,
       0,   362,   459,   388,   389,   390,   391,   392,     0,   452,
     395,   396,   397,   398,     0,   400,   401,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,     0,   383,   384,   385,     0,   417,   360,     0,
       0,   357,   361,     0,   386,     0,     0,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,   445,     0,     0,     0,     0,     0,     0,     0,   448,
       0,     0,     0,   362,     0,   446,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,   360,   383,   384,   385,   361,   417,
       0,     0,     0,   357,     0,     0,   386,     0,     0,     0,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,   455,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,   456,   376,   377,   378,   379,   380,   381,   382,     0,
       0,   383,   384,   385,     0,   417,   360,     0,     0,     0,
     361,     0,   386,     0,     0,   357,   725,     0,     0,     0,
       0,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,   726,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,   727,   376,   377,   378,   379,   380,   381,
     382,     0,   457,   383,   384,   385,     0,     0,   360,     0,
       0,   357,   361,     0,   386,     0,     0,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,   804,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,   360,   383,   384,   385,   361,     0,
       0,   357,     0,     0,     0,     0,   386,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,   448,     0,     0,     0,   362,
       0,     0,     0,     0,     0,   419,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
       0,   383,   384,   385,   360,   417,     0,   357,   361,     0,
       0,     0,   386,     0,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,   267,     0,     0,
       0,     0,     0,     0,     0,   176,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
     360,   383,   384,   385,   361,     0,     0,   357,     0,     0,
       0,     0,   386,     0,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,   602,     0,     0,
       0,     0,     0,     0,     0,   362,     0,     0,     0,     0,
       0,   603,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,     0,   383,   384,   385,
     360,   417,     0,   357,   361,     0,     0,     0,   386,     0,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,   604,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   362,     0,   605,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,   360,   383,   384,   385,
     361,     0,     0,   357,     0,     0,     0,     0,   386,     0,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,   796,     0,     0,     0,     0,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,     0,   383,   384,   385,   360,     0,     0,   357,
     361,     0,     0,     0,   386,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,   360,   383,   384,   385,   361,   417,     0,   357,
       0,     0,     0,     0,   386,   814,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,     0,   383,
     384,   385,   360,   417,     0,   357,   361,     0,     0,     0,
     386,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,  1004,
       0,     0,     0,   448,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,  1005,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,   360,   383,
     384,   385,   361,     0,     0,   357,     0,     0,   855,     0,
     386,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,     0,   383,   384,   385,   360,     0,
       0,   357,   361,     0,     0,     0,   386,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,   360,   383,   384,   385,   361,   494,
       0,   357,     0,     0,     0,     0,   386,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,   359,
     498,   383,   384,   385,   360,     0,     0,     0,   361,     0,
       0,   357,   386,     0,     0,     0,     0,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   360,     0,   363,   364,   361,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
     500,   383,   384,   385,   360,     0,     0,     0,   361,     0,
       0,   357,   386,     0,     0,     0,     0,     0,   358,     0,
       0,     0,     0,     0,   363,   364,     0,   365,   366,   367,
     359,   368,   369,   370,     0,   371,   372,   373,   374,   362,
       0,   376,   377,   378,   379,   380,     0,   382,     0,     0,
     383,   384,   385,     0,     0,   363,   364,     0,   365,   366,
     367,   386,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
     502,   383,   384,   385,   360,     0,     0,     0,   361,     0,
       0,   357,   386,     0,     0,     0,     0,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
     507,   383,   384,   385,   360,     0,     0,   357,   361,     0,
       0,     0,   386,     0,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   524,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
     360,   383,   384,   385,   361,     0,     0,   357,     0,     0,
       0,     0,   386,   606,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,     0,     0,     0,
       0,     0,     0,     0,     0,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,     0,   383,   384,   385,
     360,     0,     0,     0,   361,     0,     0,     0,   386,   357,
     721,     0,     0,     0,     0,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,     0,
       0,     0,     0,     0,     0,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,     0,   383,   384,   385,
       0,     0,   360,     0,   700,   357,   361,     0,   386,     0,
       0,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   722,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,   360,   383,
     384,   385,   361,     0,     0,   357,     0,     0,     0,     0,
     386,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,  -559,   376,   377,   378,   379,
     380,   381,   382,     0,     0,   383,   384,   385,   360,     0,
       0,   357,   361,     0,     0,     0,   386,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,     0,   176,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,   360,   383,   384,   385,   361,     0,
       0,   357,   809,     0,     0,     0,   386,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
     781,   383,   384,   385,   360,     0,     0,   357,   361,     0,
       0,     0,   386,     0,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,     0,     0,   812,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
     360,   383,   384,   385,   361,     0,     0,   357,     0,     0,
       0,     0,   386,     0,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,     0,     0,     0,
       0,     0,     0,     0,     0,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,     0,   383,   384,   385,
     360,     0,     0,   357,   361,     0,     0,     0,   386,     0,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,   360,   383,   384,   385,
     361,     0,     0,     0,   839,   357,     0,     0,   386,     0,
       0,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,     0,   383,   384,   385,     0,     0,   360,     0,
     840,   357,   361,     0,   386,     0,     0,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,   933,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   858,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,   360,   383,   384,   385,   361,     0,
       0,   357,     0,     0,     0,     0,   386,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,   936,     0,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
       0,   383,   384,   385,   360,     0,     0,   357,   361,     0,
       0,     0,   386,     0,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
     360,   383,   384,   385,   361,     0,     0,   357,  1001,     0,
       0,     0,   386,     0,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,     0,     0,     0,
       0,     0,     0,     0,     0,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,     0,   383,   384,   385,
     360,     0,     0,   357,   361,     0,   939,     0,   386,     0,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,   364,     0,   365,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,   374,   375,     0,   376,   377,
     378,   379,   380,   381,   382,     0,   360,   383,   384,   385,
     361,     0,     0,   357,     0,     0,     0,     0,   386,  1026,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,     0,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,     0,   383,   384,   385,   360,     0,     0,     0,
     361,     0,     0,     0,   386,   357,  1046,     0,     0,  1029,
       0,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,     0,   383,   384,   385,     0,     0,   360,     0,
       0,   357,   361,     0,   386,     0,     0,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,   360,   383,   384,   385,   361,     0,
       0,     0,   357,     0,     0,     0,   386,     0,     0,   358,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   359,     0,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
       0,   383,   384,   385,     0,   360,     0,     0,  1089,   361,
       0,     0,   386,   357,     0,     0,     0,     0,     0,     0,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,     0,     0,     0,
     362,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   363,   364,     0,   365,
     366,   367,     0,   368,   369,   370,     0,   371,   372,   373,
     374,   375,     0,   376,   377,   378,   379,   380,   381,   382,
       0,     0,   383,   384,   385,     0,   360,     0,     0,   357,
     361,  1127,     0,   386,     0,     0,   358,     0,     0,  1146,
       0,     0,     0,     0,     0,     0,     0,     0,   359,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   364,     0,
     365,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,   374,   375,     0,   376,   377,   378,   379,   380,   381,
     382,     0,   360,   383,   384,   385,   361,     0,     0,   357,
       0,     0,     0,     0,   386,  1147,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,     0,   383,
     384,   385,   360,     0,     0,   357,   361,     0,     0,     0,
     386,     0,   358,     0,     0,  1148,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   364,     0,   365,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,   374,   375,     0,
     376,   377,   378,   379,   380,   381,   382,     0,   360,   383,
     384,   385,   361,     0,     0,   357,     0,     0,     0,     0,
     386,  1149,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,     0,   383,   384,   385,   360,     0,
       0,   357,   361,     0,     0,     0,   386,     0,   358,     0,
       0,  1150,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     364,     0,   365,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,   374,   375,     0,   376,   377,   378,   379,
     380,   381,   382,     0,   360,   383,   384,   385,   361,     0,
       0,     0,     0,     0,   357,     0,   386,  1151,     0,     0,
       0,   358,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   359,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,     0,   365,   366,
     367,  1175,   368,   369,   370,     0,   371,   372,   373,   374,
     375,     0,   376,   377,   378,   379,   380,   381,   382,     0,
       0,   383,   384,   385,     0,     0,     0,   360,     0,     0,
     357,   361,   386,     0,     0,     0,     0,   358,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   359,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   363,   364,
       0,   365,   366,   367,     0,   368,   369,   370,     0,   371,
     372,   373,   374,   375,     0,   376,   377,   378,   379,   380,
     381,   382,     0,   360,   383,   384,   385,   361,     0,     0,
     357,     0,     0,     0,     0,   386,     0,   358,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   359,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   363,   364,     0,   365,   366,   367,
       0,   368,   369,   370,     0,   371,   372,   373,   374,   375,
       0,   376,   377,   378,   379,   380,   381,   382,     0,     0,
     383,   384,   385,   360,     0,     0,   357,   361,     0,  1204,
       0,   386,     0,   358,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   359,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   363,   364,     0,   365,   366,   367,
       0,   368,   369,   370,     0,   371,   372,   373,   374,   375,
       0,   376,   377,   378,   379,   380,   381,   382,     0,   767,
     383,   384,   385,   361,     0,     0,     0,     0,     0,     0,
       0,   386,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     363,   364,     0,   365,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,   374,   375,     0,   376,   377,   378,
     379,   380,   381,   382,     0,   282,   383,   384,   385,    22,
      23,     0,     0,     0,     0,     0,     0,   386,   283,     0,
      30,   284,   282,     0,     0,    35,    22,    23,     0,     0,
       0,     0,    40,     0,     0,   283,     0,    30,   284,     0,
       0,     0,    35,     0,     0,     0,     0,     0,     0,    40,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,  1010,     0,     0,  1011,     0,   285,     0,    65,
       0,     0,     0,    55,     0,    57,     0,    59,     0,  1010,
       0,     0,  1011,     0,   285,     0,    65,   359,    81,     0,
       0,    83,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,   282,     0,     0,    81,    22,    23,    83,     0,
       0,    85,     0,     0,     0,   283,     0,    30,   284,     0,
       0,     0,    35,     0,     0,     0,     0,     0,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   360,     0,     0,   100,   361,     0,     0,     0,     0,
    1103,     0,     0,    55,     0,    57,     0,    59,     0,  1010,
       0,   100,  1011,   359,   285,     0,    65,  1152,     0,     0,
       0,     0,     0,     0,     0,     0,   362,     0,     0,     0,
       0,     0,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,   363,   364,     0,   365,   366,   367,     0,   368,
     369,   370,     0,   371,   372,   373,   374,     0,     0,   376,
     377,   378,   379,   380,   381,   382,     0,   360,   383,   384,
     385,   361,     0,     0,     0,     0,     0,     0,     0,   386,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,  1153,     0,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   363,   364,
       0,   365,   366,   367,     0,   368,   369,   370,     0,   371,
     372,   373,   374,     0,     0,   376,   377,   378,   379,   380,
       0,   382,     0,   282,   383,   384,   385,    22,    23,     0,
       0,     0,   732,     0,   234,   386,   283,     0,    30,   284,
       0,     0,     0,    35,     0,     0,     0,     0,   235,   236,
      40,   237,     0,     0,     0,     0,   238,     0,     0,     0,
       0,     0,     0,     0,   239,     0,     0,     0,     0,     0,
     240,     0,     0,     0,    55,   241,    57,     0,   339,   242,
    1010,     0,   243,  1011,     0,   285,  -272,    65,     0,     0,
       0,     0,   244,     0,     0,     0,     0,     0,   245,   246,
    -272,  -272,     0,  -272,     0,   247,    81,     0,  -272,    83,
       0,     0,    85,     0,   248,     0,  -272,     0,     0,     0,
       0,     0,  -272,   249,   250,     0,   251,  -272,   252,     0,
     253,  -272,     0,   254,  -272,     0,     0,   255,  -282,     0,
     256,     0,     0,   257,  -272,     0,     0,     0,     0,     0,
    -272,  -272,  -282,  -282,     0,  -282,     0,  -272,     0,     0,
    -282,     0,   100,     0,     0,     0,  -272,     0,  -282,     0,
       0,     0,     0,     0,  -282,  -272,  -272,     0,  -272,  -282,
    -272,     0,  -272,  -282,     0,  -272,  -282,     0,     0,  -272,
     234,     0,  -272,     0,     0,  -272,  -282,     0,     0,     0,
       0,     0,  -282,  -282,   235,   236,     0,   237,     0,  -282,
       0,     0,   238,     0,     0,     0,     0,     0,  -282,     0,
     239,     0,     0,     0,     0,     0,   240,  -282,  -282,     0,
    -282,   241,  -282,     0,  -282,   242,     0,  -282,   243,     0,
       0,  -282,  -273,     0,  -282,     0,     0,  -282,   244,     0,
       0,     0,     0,     0,   245,   246,  -273,  -273,     0,  -273,
       0,   247,     0,     0,  -273,     0,     0,     0,     0,     0,
     248,     0,  -273,     0,     0,     0,     0,     0,  -273,   249,
     250,     0,   251,  -273,   252,     0,   253,  -273,     0,   254,
    -273,     0,     0,   255,  -174,     0,   256,     0,     0,   257,
    -273,     0,     0,     0,     0,     0,  -273,  -273,  -174,  -174,
       0,  -174,     0,  -273,     0,     0,  -174,     0,     0,     0,
       0,     0,  -273,     0,  -174,     0,     0,     0,     0,     0,
    -174,  -273,  -273,     0,  -273,  -174,  -273,     0,  -273,  -174,
       0,  -273,  -174,     0,     0,  -273,  -170,     0,  -273,     0,
       0,  -273,  -174,     0,     0,     0,     0,     0,  -174,  -174,
    -170,  -170,     0,  -170,     0,  -174,     0,     0,  -170,     0,
       0,     0,     0,     0,  -174,     0,  -170,     0,     0,     0,
       0,     0,  -170,  -174,  -174,     0,  -174,  -170,  -174,     0,
    -174,  -170,     0,  -174,  -170,     0,     0,  -174,     0,     0,
    -174,     0,     0,  -174,  -170,     0,     0,     0,     0,     0,
    -170,  -170,     0,     0,     0,     0,     0,  -170,     0,     0,
       0,     0,     0,     0,     0,     0,  -170,     0,     0,     0,
       0,     0,     0,     0,     0,  -170,  -170,     0,  -170,     0,
    -170,     0,  -170,     0,     0,  -170,     0,     0,     0,  -170,
       0,     0,  -170,     0,     0,  -170
};

static const yytype_int16 yycheck[] =
{
      12,    62,    66,   160,    16,   473,    19,   634,    20,   204,
     456,   227,   466,   233,    26,     2,   503,   741,    30,    31,
     146,    33,    34,    35,    36,   220,   834,   727,    22,    32,
     726,   408,   869,    32,  1062,    47,   478,    49,   923,   924,
     104,     1,    54,   672,    56,     1,    58,    88,  1050,   622,
      46,  1113,   625,   212,    66,    67,    68,    69,    70,    71,
     280,   503,    32,    75,   470,    77,    54,    79,    80,    46,
     972,    46,    84,  1165,   233,    87,    88,   142,    90,   205,
      92,    52,   302,    95,    96,   116,    98,   975,    36,    46,
     102,   103,   104,   105,    66,    32,     1,    32,   142,    54,
      54,     1,   167,  1165,     1,   142,    45,   114,   142,    80,
    1112,    86,  1204,   116,   168,    71,   170,   116,   334,   335,
     928,   280,    70,   167,    67,   166,   753,   191,     3,    86,
     102,   103,   104,   167,   171,   142,    46,   766,   116,   116,
     171,    46,    46,   302,   156,    20,   142,    27,    79,  1051,
      88,    82,   309,   607,     3,    46,  1184,   113,    33,    59,
     103,   121,    59,   166,     3,   142,    46,   166,   976,   181,
      50,   110,   184,   185,   186,   171,  1013,   189,   190,   191,
     168,   141,   142,     0,   404,   405,    61,    46,    88,     3,
     203,    88,    72,   171,   171,    75,   166,    77,  1086,  1036,
     212,  1038,   831,   671,   101,   142,    86,  1092,  1093,   169,
     369,   167,   785,   168,   114,   788,   121,   114,   190,   191,
     281,   233,   216,   161,    99,   105,   142,   221,   166,   166,
     105,   166,   142,    59,   228,   142,   141,   142,   142,    88,
      71,   141,  1050,   161,   141,   404,   405,    33,    34,   121,
      99,   142,    27,   169,   161,  1182,   161,   634,   142,   142,
      99,   171,   142,  1100,   161,  1102,   166,   171,   280,   166,
     142,     3,   284,  1200,   146,   729,   496,   161,   161,   291,
     292,   727,   113,   142,   944,    99,   946,   142,   114,   169,
     302,   166,   142,   161,   142,   142,    59,   169,  1022,   167,
      75,   142,   142,  1003,  1112,  1005,  1002,   466,  1004,    47,
     436,    86,   171,   142,   169,   441,   161,   329,   104,   169,
     116,   169,   169,   168,   778,    88,   953,    18,   169,   169,
     105,    69,   344,   345,    79,    80,   167,   496,   101,   326,
     169,    79,    80,   355,    54,   357,   358,   359,   360,   361,
     362,   114,   364,   365,   366,   367,   368,   825,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,  1035,   753,  1037,    27,   443,
       3,   393,   394,    74,   121,   142,   142,   399,    27,   142,
     402,   403,   404,   405,   406,   407,   408,    20,   161,    54,
      49,    92,   142,   166,   161,   142,   142,   419,   142,   146,
      33,    31,   424,   169,   426,   861,   169,   823,   109,   136,
     121,    54,   638,   639,    73,   161,    75,   144,    59,   169,
      86,   443,   169,   445,   446,   169,    75,    86,    61,   451,
     452,   168,    54,   455,   456,   146,   458,    86,   607,   461,
     393,   394,    27,   142,   466,    54,   399,    88,   142,   402,
     403,   620,   111,   869,   202,    85,   105,    54,   480,    89,
     101,   443,   484,   485,   538,   142,    99,   489,   168,   142,
     169,    54,   105,   114,   496,   169,   224,   225,   226,   227,
      54,   707,   708,   231,   161,   982,   142,    69,   161,   511,
      75,   142,   514,   698,   516,    27,   136,    54,   520,   521,
     522,    86,   524,   525,   144,   161,   136,   489,   161,   121,
     161,   141,   260,     8,   144,   168,   538,   170,   148,   136,
     161,     2,   270,   975,   168,   166,   111,   144,   276,   511,
     982,    12,   514,   530,   516,   147,    85,   149,   150,   521,
      89,   153,   154,    75,    25,   161,    27,  1003,    54,  1005,
     298,   167,   721,   722,    86,   168,   538,   170,    96,    45,
     729,   514,    46,   516,   360,   361,   953,    85,   316,    54,
     168,    89,   170,   105,   741,    59,    59,    63,   168,   111,
     602,   603,   604,   605,   606,   607,   608,   609,   611,   612,
      54,   168,   216,   170,    75,    81,   618,   221,   620,   166,
      54,   168,  1018,   677,    88,    88,   628,    54,   630,   778,
     406,   407,   634,   168,   636,   170,    54,   168,   101,   170,
    1036,   102,  1038,   419,   110,   166,   107,   136,    81,    54,
     114,   114,   141,   161,  1086,   144,   618,   121,   514,   148,
     516,   160,   224,   225,   226,   227,   161,    46,   161,   152,
     446,   161,   168,   675,   676,   677,   452,   161,   142,   681,
      59,  1113,   161,   161,   161,   618,    54,   161,    84,   115,
     166,    31,   420,    84,     8,  1127,   168,   161,   161,   168,
     168,   168,   166,   166,  1100,   169,  1102,   161,   270,    88,
     168,   168,   121,   161,   276,   677,   177,   719,    63,   141,
     166,   723,   450,   725,   726,   727,   454,   729,   144,    66,
     168,   161,   121,  1165,   169,   114,   298,   161,   142,    46,
     168,    34,   121,   142,   520,    85,   169,   121,   681,    89,
    1182,   753,   169,    21,   316,   169,   169,   142,   166,   171,
     171,   168,   168,   142,   161,   767,   768,   769,  1200,   161,
     772,   161,  1204,   983,   984,   161,   778,   161,   161,   161,
     142,   169,   161,   161,   171,   161,    34,   166,    63,   843,
     169,   142,   794,   121,   796,    27,   136,   137,   169,   801,
     261,   141,   804,   143,   144,   807,   267,   809,   148,   167,
     812,   167,   169,    86,    46,   169,   156,    32,    50,    32,
     166,   171,   162,   163,   164,   166,    34,   603,   161,   605,
     141,    86,   171,   168,   983,   984,   168,   141,   161,   161,
      72,   843,   171,    75,    46,    77,   171,  1118,   167,     3,
     168,   853,   144,   855,    86,   144,   858,    59,   420,   861,
     144,   144,   136,    17,    18,   326,    20,   144,   144,   871,
     136,    25,   144,   105,   144,  1022,   136,   144,   136,    33,
     341,   843,   144,   144,   136,    39,    88,   144,   450,    34,
      44,   169,   454,   142,    48,   166,   161,    51,  1169,  1170,
    1171,  1172,  1173,  1174,   632,   633,   169,    61,   161,   101,
     638,   639,   114,    67,    68,   166,   918,   161,   161,   121,
      74,   161,   650,   169,   169,   161,   136,   136,   464,    83,
     932,   933,   160,   935,   936,   160,   938,   939,    92,    93,
     142,    95,   171,    97,   719,    99,   948,   949,   102,   951,
      24,   953,   106,   955,   956,   109,   417,   959,   112,   161,
    1011,   341,   310,   818,   166,   847,   694,   169,   872,  1022,
    1018,   823,   664,   875,  1142,  1184,  1175,  1168,    35,   981,
     982,   983,   984,   620,   445,    30,    -1,   989,    -1,   991,
     451,   767,   768,   995,   481,    -1,    -1,   458,  1052,  1001,
    1002,  1003,  1004,  1005,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,   975,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1026,  1027,  1028,  1029,  1030,  1031,
    1032,  1033,    -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,
      -1,    -1,    -1,    -1,  1046,    -1,    -1,    12,    -1,    -1,
    1052,   779,   780,    -1,    -1,  1057,    -1,   518,    -1,  1061,
      25,   522,    27,   791,    -1,   793,    -1,    -1,    -1,   530,
     632,   633,   800,    -1,    -1,   803,   638,   639,   806,    -1,
      -1,    -1,    31,  1137,    -1,    -1,    -1,    -1,    -1,    -1,
    1052,   819,    -1,    -1,    -1,  1057,    -1,    -1,    -1,    -1,
      -1,    -1,  1104,  1105,  1106,  1107,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,   850,    31,    -1,  1127,    -1,    -1,    -1,  1131,
    1132,    -1,    -1,    -1,    -1,  1137,    85,   102,   599,    -1,
      89,    -1,   107,    -1,  1146,  1147,  1148,  1149,  1150,  1151,
      -1,  1113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     2,  1164,  1165,    -1,  1167,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,  1137,    -1,    85,    -1,    -1,
      -1,    89,    -1,    -1,    25,    -1,    27,   136,   649,    -1,
      -1,    -1,   141,    -1,   143,   144,    -1,    -1,  1200,   148,
      -1,    -1,  1204,  1165,    -1,     3,    -1,   156,    -1,    -1,
      -1,    -1,   177,   162,   163,   164,    -1,   779,   780,    17,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    25,   136,   791,
      -1,    -1,    -1,   141,    75,   143,   144,    35,    -1,    -1,
     148,   803,  1204,    -1,   806,   706,    44,    -1,   156,    -1,
      27,   712,    -1,    51,    -1,    -1,   164,    -1,     2,   987,
      -1,   102,    -1,    61,    -1,    31,   107,    -1,    12,    46,
      68,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
     741,    25,    -1,    27,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,   261,    95,    75,    -1,
      77,    99,   267,    -1,    -1,    -1,   767,   768,   106,    86,
      -1,    -1,  1040,  1041,   112,    -1,  1044,  1045,    -1,    85,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,   105,    -1,
      -1,    75,    -1,   794,  1062,   796,   177,    -1,    -1,    -1,
     801,    31,  1070,   804,  1072,    -1,  1074,    -1,    -1,    -1,
     811,    -1,    -1,   814,    -1,    -1,    -1,    -1,   102,    -1,
      -1,   326,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,    -1,   139,   140,   141,   341,   143,   144,   145,
      -1,   842,   148,   149,   150,    -1,    -1,   153,   154,   155,
     156,    -1,   853,   159,   855,    85,   162,   163,   164,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     261,    -1,    -1,    -1,    -1,   987,   267,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,
     140,   141,   417,   143,   144,    -1,  1184,    -1,   148,     2,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,    12,
      -1,     1,   162,   163,   164,    -1,    -1,    -1,    -1,    -1,
     445,    -1,    25,    -1,    27,   326,   451,   948,    -1,    -1,
      31,    -1,    22,   458,   955,    -1,    26,    27,    -1,    -1,
     341,    -1,    -1,    -1,    -1,    35,   967,    37,    38,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    -1,   261,    -1,    49,
      -1,    -1,    52,   267,   985,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,   994,    -1,    -1,    -1,   998,    -1,    -1,
      -1,    -1,    -1,    73,    85,    75,    -1,    77,    89,    79,
      80,    -1,    82,   518,    84,    -1,    86,   522,    -1,   102,
      -1,    -1,    -1,    -1,   107,   530,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   417,    -1,   108,    -1,
      -1,   111,   326,    -1,    -1,    -1,  1047,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   341,   139,   140,
     141,    -1,   143,   144,   445,    -1,    -1,   148,    -1,    -1,
     451,    -1,    -1,    -1,   155,   156,    -1,   458,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,    -1,   177,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,  1104,  1105,  1106,  1107,    -1,    -1,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,    42,
    1131,  1132,     2,   417,    -1,    -1,    49,   518,    -1,    -1,
      -1,   522,    12,    -1,   649,    -1,    -1,    -1,    -1,   530,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      73,   445,    75,    -1,    77,    -1,    79,   451,    -1,    82,
      -1,    84,    -1,    86,   458,    -1,    -1,    -1,   261,     2,
      -1,    -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,    12,
      -1,    -1,   105,    -1,    -1,   108,    -1,    -1,   111,    -1,
      -1,   706,    25,    -1,    27,    75,    -1,   712,    -1,    22,
      -1,    31,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,    42,
      -1,    -1,   102,    -1,   518,    -1,    49,   107,   522,    -1,
      -1,    -1,    -1,   326,    -1,    -1,   530,    -1,   161,    -1,
      -1,    -1,    75,    -1,   167,    -1,    -1,    -1,   341,    -1,
      73,    -1,    75,    -1,    -1,    85,    -1,    -1,   649,    89,
      -1,    84,    -1,    86,    -1,    31,    -1,    -1,    -1,   102,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,   794,
      -1,   796,   105,    -1,    -1,   108,   801,    -1,   111,   804,
      -1,    -1,    -1,    -1,    -1,    -1,   811,   177,    -1,   814,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,    -1,
     140,   141,    -1,   143,   144,   706,    -1,    -1,   148,    85,
      -1,   712,    -1,    89,   417,    -1,   156,   842,    -1,    -1,
      -1,    -1,   162,   163,   164,    -1,     2,    -1,   853,    -1,
     855,    -1,    -1,    -1,   177,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   445,    -1,    -1,   649,    -1,    -1,   451,    25,
      -1,    27,    -1,    -1,    -1,   458,    -1,    -1,    -1,    -1,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
      -1,   261,   148,    -1,    -1,    -1,    -1,   267,    -1,   155,
     156,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,    -1,   794,    -1,   796,    -1,   173,    -1,    75,
     801,    -1,   706,   804,    -1,    -1,    -1,    -1,   712,    -1,
     811,    -1,    -1,   814,    -1,   518,    -1,    -1,   261,   522,
      -1,    -1,    -1,   948,   267,    -1,   102,   530,    -1,    -1,
     955,   107,    -1,    -1,    -1,    -1,   326,    -1,    -1,    -1,
      -1,   842,   967,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   341,   853,    -1,   855,    -1,    -1,    -1,    -1,    -1,
     985,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   994,
      -1,    -1,    -1,   998,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   326,    -1,    -1,    -1,    -1,    -1,    -1,
     794,    -1,   796,    -1,    -1,    -1,    -1,   801,   341,    -1,
     804,   177,    -1,    -1,    -1,    -1,    -1,   811,    -1,    -1,
     814,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1047,    -1,    -1,    -1,    -1,   417,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,   842,    -1,
      -1,    26,    27,    -1,    -1,    -1,   649,   948,    -1,   853,
      35,   855,    37,    38,   955,   445,    -1,    42,    -1,    -1,
      45,   451,    -1,    -1,    49,    -1,   967,    52,   458,    -1,
      -1,    -1,    -1,    -1,   417,    -1,    -1,    -1,    -1,  1104,
    1105,  1106,  1107,    -1,   985,   261,    -1,    -1,    73,    -1,
      75,   267,    -1,   994,    -1,    80,    -1,   998,    -1,    84,
      -1,    86,   445,   706,    -1,    -1,  1131,  1132,   451,   712,
      -1,    -1,    -1,    -1,    -1,   458,    -1,    -1,    -1,    -1,
     105,    -1,    -1,   108,    -1,   110,   111,    -1,   518,    -1,
      -1,    -1,   522,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     530,    -1,    -1,    -1,   948,    -1,  1047,    -1,     2,    -1,
     326,   955,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,   967,    -1,   341,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,     2,   518,    -1,    -1,    -1,   522,
      -1,   985,    -1,    -1,    12,    -1,    -1,   530,    -1,    -1,
     994,   794,    -1,   796,   998,    -1,    -1,    25,   801,    27,
      -1,   804,    -1,  1104,  1105,  1106,  1107,    -1,   811,    -1,
      -1,   814,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1131,  1132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   842,
      -1,   417,    -1,  1047,    -1,    -1,    -1,    75,   102,    -1,
     853,    -1,   855,   107,    -1,    -1,    -1,    -1,    -1,   649,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,   445,
      -1,    -1,    -1,    -1,   102,   451,    -1,    -1,    -1,   107,
      -1,    -1,   458,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
    1104,  1105,  1106,  1107,    -1,    -1,   649,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   706,    -1,    -1,    -1,
      -1,    -1,   712,   177,    -1,    -1,    -1,  1131,  1132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   518,    -1,    -1,   948,   522,    -1,    -1,   177,
      -1,    -1,   955,    -1,   530,    -1,    -1,   139,    -1,   141,
      -1,    -1,    -1,   706,   967,    -1,    -1,    -1,    -1,   712,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   985,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   994,    -1,    -1,    -1,   998,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   794,   187,   796,   261,    -1,    -1,
      -1,   801,    -1,   267,   804,    -1,    -1,    -1,    -1,    -1,
      -1,   811,   204,    -1,   814,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   261,    -1,    -1,    -1,    -1,   220,   267,
     222,    -1,    -1,    -1,  1047,    -1,    -1,    -1,    -1,    -1,
      -1,   794,   842,   796,    -1,    -1,    -1,    -1,   801,    -1,
      -1,   804,    -1,   853,    -1,   855,    -1,    -1,   811,    -1,
      -1,   814,   326,   649,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   263,    -1,    -1,    -1,    -1,    -1,   341,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   326,   842,
      -1,  1104,  1105,  1106,  1107,    -1,    -1,    -1,    -1,    -1,
     853,    -1,   855,   341,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1131,  1132,
     706,    -1,    -1,    -1,    -1,    -1,   712,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,   948,    -1,
      -1,    -1,    -1,   417,   346,   955,    -1,    -1,   152,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   967,    -1,    -1,
      -1,   165,    -1,    -1,   168,    -1,   170,    -1,    -1,   417,
      -1,   445,    -1,    -1,    -1,   985,    -1,   451,    -1,    -1,
      -1,    -1,    -1,    -1,   994,   948,    -1,    -1,   998,    -1,
      -1,    -1,   955,    -1,    -1,    -1,    -1,   445,   794,    -1,
     796,    -1,    -1,   451,   967,   801,    -1,    -1,   804,    -1,
     458,    -1,    -1,    -1,   416,   811,    -1,    -1,   814,    -1,
      -1,    -1,   985,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   994,   434,    -1,    -1,   998,   438,  1047,    -1,    -1,
      -1,    -1,    -1,    -1,   518,    -1,   842,    -1,   522,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   530,   853,    -1,   855,
      -1,    -1,    -1,    -1,    -1,   467,    -1,    -1,    -1,    -1,
     518,   473,    -1,    -1,   522,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   530,    -1,  1047,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1104,  1105,  1106,  1107,    22,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    37,    38,    -1,    -1,    -1,    42,    -1,
      -1,  1131,  1132,    -1,    -1,    49,    -1,    -1,    -1,   531,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   540,    -1,
      -1,  1104,  1105,  1106,  1107,    -1,    -1,    -1,    -1,    73,
      -1,    75,   948,    77,    -1,    79,    -1,    -1,    82,   955,
      84,    -1,    86,    -1,    -1,    -1,    -1,    -1,  1131,  1132,
      -1,   967,    -1,    -1,    -1,   649,    -1,    -1,    -1,    -1,
      -1,   105,     3,    -1,   108,    -1,    -1,   111,    -1,   985,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,   994,    20,
      -1,   649,   998,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,   621,
      -1,    -1,   624,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,   706,    -1,    -1,    -1,    -1,   161,   712,    -1,
      61,    -1,    -1,   167,    -1,    -1,    67,    68,    -1,    -1,
      -1,  1047,    -1,    74,    -1,    -1,    -1,    -1,   706,    -1,
      -1,    -1,    83,    -1,   712,   667,    -1,    -1,    -1,   671,
      -1,    92,    93,    -1,    95,    -1,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,    -1,   106,   107,    -1,   109,    -1,
      -1,   112,   694,    -1,    -1,    -1,   698,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1104,  1105,
    1106,  1107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     794,    -1,   796,    -1,    -1,    -1,    -1,   801,    -1,    -1,
     804,    -1,    -1,    -1,    -1,  1131,  1132,   811,    -1,   741,
     814,    -1,    -1,    -1,    -1,    -1,   794,   168,   796,    -1,
      -1,    -1,    -1,   801,    -1,    -1,   804,    -1,    -1,    -1,
      -1,    -1,    -1,   811,    -1,    -1,   814,    -1,   842,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   853,
      -1,   855,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,   842,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,   853,    39,   855,    -1,    -1,
      -1,    44,    -1,    -1,   816,    48,   818,    -1,    51,    -1,
     822,    -1,    -1,   825,    -1,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,   847,    -1,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    -1,    95,    -1,    97,    -1,    99,    -1,     3,   102,
     872,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,   967,    -1,    -1,    -1,    -1,    33,    12,
     948,    -1,    -1,    -1,    39,    -1,    19,   955,    -1,    44,
      -1,   985,    -1,    48,    -1,    -1,    51,    -1,    31,   967,
     994,    -1,    -1,    -1,   998,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    67,    68,    -1,   168,    -1,   985,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,   994,    -1,    83,    -1,
     998,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,
      95,    -1,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
      -1,   106,    85,  1047,   109,    -1,    89,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1047,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,
     123,   124,   125,   126,    -1,    -1,   129,   130,   131,   132,
    1022,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,   168,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    -1,   162,
     163,   164,    -1,   166,    -1,    -1,  1104,  1105,  1106,  1107,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     0,     1,    -1,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,  1131,  1132,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    -1,    51,    52,    -1,    54,    55,    56,    57,    58,
      -1,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    -1,    -1,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    -1,    86,    87,    88,
      -1,    90,    91,    92,    93,    94,    95,    96,    97,    -1,
      99,   100,    -1,   102,   103,   104,   105,   106,    -1,   108,
     109,   110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,
     159,   160,   161,    -1,    -1,   164,    -1,   166,    -1,   168,
      -1,   170,    -1,     1,   173,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    -1,    51,    52,    -1,    54,    55,    56,    57,
      58,    -1,    60,    61,    62,    63,    64,    65,    -1,    67,
      68,    69,    -1,    -1,    -1,    73,    74,    75,    76,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    99,   100,    -1,   102,   103,   104,   105,   106,    -1,
     108,   109,   110,   111,   112,    -1,    -1,   115,    -1,   117,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,   147,
     148,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,   166,   167,
     168,    -1,   170,    -1,     1,   173,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    54,    55,    56,
      57,    58,    -1,    60,    61,    62,    63,    64,    65,    -1,
      67,    68,    69,    -1,    -1,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    -1,    86,
      87,    88,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    -1,    99,   100,    -1,   102,   103,   104,   105,   106,
      -1,   108,   109,   110,   111,   112,    -1,    -1,   115,    -1,
     117,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,    -1,   166,
     167,   168,    -1,   170,    -1,     1,   173,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    51,    52,    -1,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    63,    64,    65,
      -1,    67,    68,    69,    -1,    -1,    -1,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    -1,
      86,    87,    88,    -1,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    99,   100,    -1,   102,   103,   104,   105,
     106,    -1,   108,   109,   110,   111,   112,    -1,    -1,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,    -1,
     166,   167,   168,    -1,   170,    -1,     1,   173,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    -1,    51,    52,    -1,    54,
      55,    56,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    -1,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      -1,    86,    87,    88,    -1,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    99,   100,    -1,   102,   103,   104,
     105,   106,    -1,   108,   109,   110,   111,   112,    -1,    -1,
     115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
      -1,   166,   167,   168,    -1,   170,    -1,     1,   173,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    49,    -1,    51,    52,    -1,
      54,    55,    56,    57,    58,    -1,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    -1,    -1,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    -1,    86,    87,    88,    -1,    90,    91,    92,    93,
      94,    95,    96,    97,    -1,    99,   100,    -1,   102,   103,
     104,   105,   106,    -1,   108,   109,   110,   111,   112,    -1,
      -1,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,   161,    -1,    -1,
     164,    -1,   166,   167,   168,    -1,   170,    -1,     1,   173,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    49,    -1,    51,    52,
      -1,    54,    55,    56,    57,    58,    -1,    60,    61,    62,
      63,    64,    65,    -1,    67,    68,    69,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    -1,    86,    87,    88,    -1,    90,    91,    92,
      93,    94,    95,    96,    97,    -1,    99,   100,    -1,   102,
     103,   104,   105,   106,    -1,   108,   109,   110,   111,   112,
      -1,    -1,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,   161,    -1,
      -1,   164,    -1,   166,   167,   168,    -1,   170,    -1,     1,
     173,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    49,    -1,    51,
      52,    -1,    54,    55,    56,    57,    58,    -1,    60,    61,
      62,    63,    64,    65,    -1,    67,    68,    69,    -1,    -1,
      -1,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    -1,    86,    87,    88,    -1,    90,    91,
      92,    93,    94,    95,    96,    97,    -1,    99,   100,    -1,
     102,   103,   104,   105,   106,    -1,   108,   109,   110,   111,
     112,    -1,    -1,   115,    -1,   117,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
       1,   173,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    49,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    -1,    67,    68,    69,    -1,
      -1,    -1,    73,    74,    75,    76,    -1,    78,    79,    80,
      81,    82,    83,    84,    -1,    86,    87,    88,    -1,    90,
      91,    92,    93,    94,    95,    96,    97,    -1,    99,   100,
      -1,   102,   103,   104,   105,   106,    -1,   108,   109,   110,
     111,   112,    -1,    -1,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,     1,   173,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    49,
      -1,    51,    52,    -1,    54,    55,    56,    57,    58,    -1,
      60,    61,    62,    63,    64,    65,    -1,    67,    68,    69,
      -1,    -1,    -1,    73,    74,    75,    76,    -1,    78,    79,
      80,    81,    82,    83,    84,    -1,    86,    87,    88,    -1,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,    99,
     100,    -1,   102,   103,   104,   105,   106,    -1,   108,   109,
     110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,   161,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,
     170,    -1,     1,   173,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    -1,    51,    52,    -1,    54,    55,    56,    57,    58,
      -1,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    -1,    -1,    73,    74,    75,    76,    -1,    78,
      79,    80,    81,    82,    83,    84,    -1,    86,    87,    88,
      -1,    90,    91,    92,    93,    94,    95,    96,    97,    -1,
      99,   100,    -1,   102,   103,   104,   105,   106,    -1,   108,
     109,   110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,
     159,   160,   161,    -1,    -1,   164,    -1,   166,    -1,   168,
      -1,   170,    -1,     1,   173,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    -1,    51,    52,    -1,    54,    55,    56,    57,
      58,    -1,    60,    61,    62,    63,    64,    65,    -1,    67,
      68,    69,    -1,    -1,    -1,    73,    74,    75,    76,    -1,
      78,    79,    80,    81,    82,    83,    84,    -1,    86,    87,
      88,    -1,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    99,   100,    -1,   102,   103,   104,   105,   106,    -1,
     108,   109,   110,   111,   112,    -1,    -1,   115,    -1,   117,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
     158,   159,   160,   161,    -1,    -1,   164,    -1,   166,    -1,
     168,    -1,   170,    -1,     1,   173,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    54,    55,    56,
      57,    58,    -1,    60,    61,    62,    63,    64,    65,    -1,
      67,    68,    69,    -1,    -1,    -1,    73,    74,    75,    76,
      -1,    78,    79,    80,    81,    82,    83,    84,    -1,    86,
      87,    88,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    -1,    99,   100,    -1,   102,   103,   104,   105,   106,
      -1,   108,   109,   110,   111,   112,    -1,    -1,   115,    -1,
     117,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,    -1,   166,
      -1,   168,    -1,   170,    -1,     1,   173,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    51,    52,    -1,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    63,    64,    65,
      -1,    67,    68,    69,    -1,    -1,    -1,    73,    74,    75,
      76,    -1,    78,    79,    80,    81,    82,    83,    84,    -1,
      86,    87,    88,    -1,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    99,   100,    -1,   102,   103,   104,   105,
     106,    -1,   108,   109,   110,   111,   112,    -1,    -1,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,    -1,
     166,    -1,   168,    -1,   170,    -1,     1,   173,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    -1,    51,    52,    -1,    54,
      55,    56,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    -1,    -1,    73,    74,
      75,    76,    -1,    78,    79,    80,    81,    82,    83,    84,
      -1,    86,    87,    88,    -1,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    99,   100,    -1,   102,   103,   104,
     105,   106,    -1,   108,   109,   110,   111,   112,    -1,    -1,
     115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,     1,   173,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    49,    -1,    51,    52,    -1,
      54,    55,    56,    57,    58,    -1,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    -1,    -1,    73,
      74,    75,    76,    -1,    78,    79,    80,    81,    82,    83,
      84,    -1,    86,    87,    88,    -1,    90,    91,    92,    93,
      94,    95,    96,    97,    -1,    99,   100,    -1,   102,   103,
     104,   105,   106,    -1,   108,   109,   110,   111,   112,    -1,
      -1,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,   161,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,     1,   173,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    49,    -1,    51,    52,
      -1,    54,    55,    56,    57,    58,    -1,    60,    61,    62,
      63,    64,    65,    -1,    67,    68,    69,    -1,    -1,    -1,
      73,    74,    75,    76,    -1,    78,    79,    80,    81,    82,
      83,    84,    -1,    86,    87,    88,    -1,    90,    91,    92,
      93,    94,    95,    96,    97,    -1,    99,   100,    -1,   102,
     103,   104,   105,   106,    -1,   108,   109,   110,   111,   112,
      -1,    -1,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,   161,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,     1,
     173,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    49,    -1,    51,
      52,    -1,    54,    55,    56,    57,    58,    -1,    60,    61,
      62,    63,    64,    65,    -1,    67,    68,    69,    -1,    -1,
      -1,    73,    74,    75,    76,    -1,    78,    79,    80,    81,
      82,    83,    84,    -1,    86,    87,    88,    -1,    90,    91,
      92,    93,    94,    95,    96,    97,    -1,    99,   100,    -1,
     102,   103,   104,   105,   106,    -1,   108,   109,   110,   111,
     112,    -1,    -1,   115,    -1,   117,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
       1,   173,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
     171,     1,   173,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,
     170,    -1,     3,   173,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    75,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,    -1,   173,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,    -1,   102,   103,   104,    -1,   106,   107,    -1,   109,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,
     170,    -1,    -1,   173,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,    97,    -1,
      99,    -1,    -1,   102,   103,   104,    -1,   106,   107,    -1,
     109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,
     159,   160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,
     169,   170,    -1,    -1,   173,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,
      78,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,    97,
      -1,    99,    -1,    -1,   102,   103,   104,    -1,   106,   107,
      -1,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
     158,   159,   160,    -1,    -1,    -1,   164,    -1,   166,    -1,
     168,   169,   170,    -1,     3,   173,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    46,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    83,    84,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,    97,    -1,
      99,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,
     109,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,
     159,   160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,
      -1,   170,    -1,     3,   173,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,    -1,   102,   103,   104,    -1,   106,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,   161,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,
     170,    -1,    -1,   173,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,    97,    -1,
      99,    -1,    -1,   102,   103,   104,    -1,   106,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,
     159,   160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,
      -1,   170,    -1,     3,   173,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,    -1,   102,   103,   104,    -1,   106,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,
     170,    -1,     3,   173,     5,     6,     7,     8,     9,    10,
      11,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,   105,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,     3,   173,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    75,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,   118,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
      -1,   173,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,   118,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,     3,   173,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,   103,   104,    -1,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
      -1,   173,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,   167,   168,    -1,   170,
      -1,    -1,   173,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,
     170,   171,    -1,   173,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,    97,    -1,
      99,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,
     159,   160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,
      -1,   170,   171,     3,   173,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,   118,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,
     170,    -1,     3,   173,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,    -1,   173,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,
     170,    -1,     3,   173,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,   105,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,     3,   173,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,   171,
       3,   173,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,   107,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,
     173,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
       3,   173,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,   107,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,
     173,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
      -1,   173,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,     3,   173,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
       3,   173,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,
     173,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,    65,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,    -1,
     166,    -1,   168,    -1,   170,    -1,     3,   173,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    76,
      -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
      97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,   158,   159,   160,    -1,    -1,    -1,   164,    -1,   166,
      -1,   168,    -1,   170,    -1,     3,   173,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,
      78,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,    97,
      -1,    99,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
     158,   159,   160,    -1,    -1,    -1,   164,    -1,   166,    -1,
     168,    -1,   170,    -1,     3,   173,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,    97,    -1,
      99,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,
     159,   160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,
      -1,   170,    -1,     3,   173,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,
     170,    -1,     3,   173,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,     3,   173,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
       3,   173,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,
     173,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,    65,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,    -1,
     166,    -1,   168,    -1,   170,    -1,     3,   173,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    76,
      -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
      97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,   136,
     137,   138,   139,   140,    -1,    -1,    -1,    12,   145,    -1,
      -1,    -1,    17,    18,    19,    20,    -1,    -1,   155,    -1,
      25,   158,   159,   160,    -1,    -1,    -1,   164,    33,    -1,
      35,   168,    -1,   170,    39,    -1,   173,    -1,    -1,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,
      95,    -1,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,   122,   123,   124,
     125,   126,    -1,    -1,   129,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,   139,   140,    -1,    -1,   143,    -1,
      -1,    -1,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,     3,   168,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   165,   159,   160,
     168,    -1,   170,   164,    -1,   166,    -1,   168,    -1,   170,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
      -1,    -1,    -1,   166,    -1,   168,   169,   170,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    59,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,   101,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,   138,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,   160,   161,    -1,    -1,    -1,
      -1,   166,    33,   168,    35,   170,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,     3,   138,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,   160,
      -1,    -1,    -1,    -1,    -1,   166,    33,   168,    35,   170,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    76,
      -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
      97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,   138,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,   160,    -1,    -1,    -1,    -1,    -1,   166,
      33,   168,    35,   170,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,     3,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    12,    -1,   112,
      -1,    -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,   138,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    67,    68,    -1,   168,    -1,   170,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    12,
      95,    -1,    97,    -1,    99,    -1,    19,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,   122,   123,   124,
     125,   126,    -1,    -1,   129,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,   139,   140,    -1,    -1,   143,    -1,
      -1,    -1,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    85,    -1,    -1,    12,    89,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,
     123,   124,   125,   126,    -1,    -1,   129,   130,   131,   132,
      -1,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    85,   162,
     163,   164,    89,   166,    -1,    12,    -1,    -1,    -1,    -1,
     173,    98,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,   126,
      -1,    -1,   129,   130,   131,   132,    -1,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,   162,   163,   164,    85,   166,
      -1,    12,    89,    -1,    -1,    -1,   173,    -1,    19,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,   126,
      -1,    -1,   129,   130,   131,   132,    -1,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,   166,
      -1,    12,    -1,    -1,    -1,    -1,   173,    98,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,   124,   125,   126,    -1,    -1,   129,   130,
     131,   132,    -1,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    85,   166,    -1,    12,    89,    -1,
      -1,    -1,   173,    -1,    19,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,   124,   125,   126,    -1,    -1,   129,   130,
     131,   132,    -1,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      85,   162,   163,   164,    89,   166,    -1,    12,    -1,    -1,
      -1,    -1,   173,    98,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,   121,   122,   123,   124,
     125,   126,    -1,    -1,   129,   130,   131,   132,    -1,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    -1,   162,   163,   164,
      85,   166,    -1,    12,    89,    -1,    -1,    -1,   173,    -1,
      19,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,   121,   122,   123,   124,
     125,   126,    -1,    -1,   129,   130,   131,   132,    -1,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    85,   162,   163,   164,
      89,   166,    -1,    -1,    -1,    12,    -1,    -1,   173,    98,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,   120,   121,   122,   123,   124,   125,   126,    -1,    46,
     129,   130,   131,   132,    -1,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    -1,   162,   163,   164,    -1,   166,    85,    -1,
      -1,    12,    89,    -1,   173,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,   120,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,   166,
      -1,    -1,    -1,    12,    -1,    -1,   173,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    70,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    -1,   166,    85,    -1,    -1,    -1,
      89,    -1,   173,    -1,    -1,    12,    13,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    70,   153,   154,   155,   156,   157,   158,
     159,    -1,   161,   162,   163,   164,    -1,    -1,    85,    -1,
      -1,    12,    89,    -1,   173,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,    -1,
      -1,    12,    -1,    -1,    -1,    -1,   173,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    85,   166,    -1,    12,    89,    -1,
      -1,    -1,   173,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      85,   162,   163,   164,    89,    -1,    -1,    12,    -1,    -1,
      -1,    -1,   173,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    -1,   162,   163,   164,
      85,   166,    -1,    12,    89,    -1,    -1,    -1,   173,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    85,   162,   163,   164,
      89,    -1,    -1,    12,    -1,    -1,    -1,    -1,   173,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    -1,   162,   163,   164,    85,    -1,    -1,    12,
      89,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    85,   162,   163,   164,    89,   166,    -1,    12,
      -1,    -1,    -1,    -1,   173,    98,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    -1,   162,
     163,   164,    85,   166,    -1,    12,    89,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    -1,    -1,   116,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    70,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    85,   162,
     163,   164,    89,    -1,    -1,    12,    -1,    -1,   171,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,   162,   163,   164,    85,    -1,
      -1,    12,    89,    -1,    -1,    -1,   173,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,   166,
      -1,    12,    -1,    -1,    -1,    -1,   173,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    31,
     161,   162,   163,   164,    85,    -1,    -1,    -1,    89,    -1,
      -1,    12,   173,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,   136,   137,    89,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
     161,   162,   163,   164,    85,    -1,    -1,    -1,    89,    -1,
      -1,    12,   173,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,
      31,   143,   144,   145,    -1,   147,   148,   149,   150,   120,
      -1,   153,   154,   155,   156,   157,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,   136,   137,    -1,   139,   140,
     141,   173,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
     161,   162,   163,   164,    85,    -1,    -1,    -1,    89,    -1,
      -1,    12,   173,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
     161,   162,   163,   164,    85,    -1,    -1,    12,    89,    -1,
      -1,    -1,   173,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      85,   162,   163,   164,    89,    -1,    -1,    12,    -1,    -1,
      -1,    -1,   173,    98,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    -1,   162,   163,   164,
      85,    -1,    -1,    -1,    89,    -1,    -1,    -1,   173,    12,
      13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    85,    -1,   169,    12,    89,    -1,   173,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    85,   162,
     163,   164,    89,    -1,    -1,    12,    -1,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,   162,   163,   164,    85,    -1,
      -1,    12,    89,    -1,    -1,    -1,   173,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,    -1,
      -1,    12,    13,    -1,    -1,    -1,   173,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
     161,   162,   163,   164,    85,    -1,    -1,    12,    89,    -1,
      -1,    -1,   173,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      85,   162,   163,   164,    89,    -1,    -1,    12,    -1,    -1,
      -1,    -1,   173,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    -1,   162,   163,   164,
      85,    -1,    -1,    12,    89,    -1,    -1,    -1,   173,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    85,   162,   163,   164,
      89,    -1,    -1,    -1,   169,    12,    -1,    -1,   173,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    85,    -1,
     169,    12,    89,    -1,   173,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,    -1,
      -1,    12,    -1,    -1,    -1,    -1,   173,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    85,    -1,    -1,    12,    89,    -1,
      -1,    -1,   173,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      85,   162,   163,   164,    89,    -1,    -1,    12,    13,    -1,
      -1,    -1,   173,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    -1,   162,   163,   164,
      85,    -1,    -1,    12,    89,    -1,   171,    -1,   173,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    85,   162,   163,   164,
      89,    -1,    -1,    12,    -1,    -1,    -1,    -1,   173,    98,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    -1,   162,   163,   164,    85,    -1,    -1,    -1,
      89,    -1,    -1,    -1,   173,    12,    13,    -1,    -1,    98,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    85,    -1,
      -1,    12,    89,    -1,   173,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,    -1,
      -1,    -1,    12,    -1,    -1,    -1,   173,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    -1,    85,    -1,    -1,   169,    89,
      -1,    -1,   173,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
      -1,    -1,   162,   163,   164,    -1,    85,    -1,    -1,    12,
      89,   171,    -1,   173,    -1,    -1,    19,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    85,   162,   163,   164,    89,    -1,    -1,    12,
      -1,    -1,    -1,    -1,   173,    98,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    -1,   162,
     163,   164,    85,    -1,    -1,    12,    89,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    85,   162,
     163,   164,    89,    -1,    -1,    12,    -1,    -1,    -1,    -1,
     173,    98,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,   162,   163,   164,    85,    -1,
      -1,    12,    89,    -1,    -1,    -1,   173,    -1,    19,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,    -1,
      -1,    -1,    -1,    -1,    12,    -1,   173,    98,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    59,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,    85,    -1,    -1,
      12,    89,   173,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
      -1,   139,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,    -1,    85,   162,   163,   164,    89,    -1,    -1,
      12,    -1,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,    -1,    -1,
     162,   163,   164,    85,    -1,    -1,    12,    89,    -1,   171,
      -1,   173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,    -1,    85,
     162,   163,   164,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,    -1,    22,   162,   163,   164,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,   173,    35,    -1,
      37,    38,    22,    -1,    -1,    42,    26,    27,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    75,    -1,
      77,    -1,    79,    -1,    -1,    82,    -1,    84,    -1,    86,
      -1,    -1,    -1,    73,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    82,    -1,    84,    -1,    86,    31,   105,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,   105,    26,    27,   108,    -1,
      -1,   111,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,   161,    89,    -1,    -1,    -1,    -1,
     167,    -1,    -1,    73,    -1,    75,    -1,    77,    -1,    79,
      -1,   161,    82,    31,    84,    -1,    86,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,    -1,   108,    -1,
      -1,   111,   136,   137,    -1,   139,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,   150,    -1,    -1,   153,
     154,   155,   156,   157,   158,   159,    -1,    85,   162,   163,
     164,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
      -1,   139,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,   150,    -1,    -1,   153,   154,   155,   156,   157,
      -1,   159,    -1,    22,   162,   163,   164,    26,    27,    -1,
      -1,    -1,     1,    -1,     3,   173,    35,    -1,    37,    38,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    17,    18,
      49,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    73,    44,    75,    -1,    77,    48,
      79,    -1,    51,    82,    -1,    84,     3,    86,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,
      17,    18,    -1,    20,    -1,    74,   105,    -1,    25,   108,
      -1,    -1,   111,    -1,    83,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    92,    93,    -1,    95,    44,    97,    -1,
      99,    48,    -1,   102,    51,    -1,    -1,   106,     3,    -1,
     109,    -1,    -1,   112,    61,    -1,    -1,    -1,    -1,    -1,
      67,    68,    17,    18,    -1,    20,    -1,    74,    -1,    -1,
      25,    -1,   161,    -1,    -1,    -1,    83,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    92,    93,    -1,    95,    44,
      97,    -1,    99,    48,    -1,   102,    51,    -1,    -1,   106,
       3,    -1,   109,    -1,    -1,   112,    61,    -1,    -1,    -1,
      -1,    -1,    67,    68,    17,    18,    -1,    20,    -1,    74,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,    92,    93,    -1,
      95,    44,    97,    -1,    99,    48,    -1,   102,    51,    -1,
      -1,   106,     3,    -1,   109,    -1,    -1,   112,    61,    -1,
      -1,    -1,    -1,    -1,    67,    68,    17,    18,    -1,    20,
      -1,    74,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    92,
      93,    -1,    95,    44,    97,    -1,    99,    48,    -1,   102,
      51,    -1,    -1,   106,     3,    -1,   109,    -1,    -1,   112,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    17,    18,
      -1,    20,    -1,    74,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    92,    93,    -1,    95,    44,    97,    -1,    99,    48,
      -1,   102,    51,    -1,    -1,   106,     3,    -1,   109,    -1,
      -1,   112,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,
      17,    18,    -1,    20,    -1,    74,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    83,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    92,    93,    -1,    95,    44,    97,    -1,
      99,    48,    -1,   102,    51,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,   112,    61,    -1,    -1,    -1,    -1,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    95,    -1,
      97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,   112
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   178,   179,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,    35,
      37,    38,    39,    40,    41,    42,    43,    44,    47,    48,
      49,    51,    54,    55,    56,    57,    58,    60,    61,    62,
      64,    65,    67,    68,    69,    73,    74,    75,    76,    77,
      78,    79,    82,    83,    84,    86,    87,    88,    90,    91,
      92,    93,    94,    95,    96,    97,    99,   100,   102,   103,
     104,   105,   106,   108,   109,   111,   112,   115,   117,   120,
     136,   137,   138,   139,   140,   145,   155,   158,   159,   160,
     161,   164,   166,   168,   170,   173,   180,   181,   182,   183,
     184,   185,   188,   190,   196,   197,   198,   201,   202,   206,
     208,   211,   212,   214,   215,   216,   221,   222,   224,   225,
     228,   229,   230,   231,   232,   233,   237,   238,   241,   242,
     245,   246,   249,   252,   253,   278,   281,   282,   302,   303,
     304,   305,   306,   307,   308,   316,   317,   318,   319,   320,
     323,   324,   325,   326,   327,   328,   329,   330,   332,   333,
     334,   335,   336,   161,   182,   320,   116,   309,   310,     3,
     203,    14,    22,    35,    40,    41,    43,    54,    84,    97,
     166,   170,   228,   249,   302,   307,   318,   319,   320,   323,
     325,   326,   309,   320,   105,   281,    86,   203,   182,   296,
     320,   182,   168,     8,    84,   320,   321,     8,    11,    84,
     105,   321,    75,   118,   223,   320,   223,   320,    26,   282,
     320,   320,   189,   168,     3,    17,    18,    20,    25,    33,
      39,    44,    48,    51,    61,    67,    68,    74,    83,    92,
      93,    95,    97,    99,   102,   106,   109,   112,   205,   207,
     205,   205,   250,   168,   205,   283,   284,    32,   190,   210,
     320,   320,    18,    74,    92,   109,   320,   320,   320,     8,
     168,   213,    22,    35,    38,    84,   214,     4,   272,   295,
     320,   103,   104,   161,   320,   322,   320,   210,   320,   320,
     320,    96,   168,   182,   320,   320,   190,   202,   320,   323,
     190,   202,   320,   205,   279,   320,   320,   320,   320,   320,
     320,   320,   320,     1,   167,   180,   191,   295,   107,   146,
     272,   297,   298,   322,   223,   295,   320,   331,   320,    77,
     182,   166,    81,   186,    45,   110,    54,   205,   205,    52,
      80,   267,   283,   160,   161,   152,   320,    12,    19,    31,
      85,    89,   120,   136,   137,   139,   140,   141,   143,   144,
     145,   147,   148,   149,   150,   151,   153,   154,   155,   156,
     157,   158,   159,   162,   163,   164,   173,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   165,   259,   168,   170,    85,    89,   168,   182,
     161,   320,   320,   320,   205,   295,    54,   166,   190,    46,
     309,   279,   283,   161,   142,   161,   115,   206,   272,   299,
     300,   301,   322,    84,   219,   253,   281,    84,   217,   279,
     253,   281,   205,   168,   210,    32,    46,   210,   116,   210,
     312,    32,    46,   210,   312,    36,    70,   161,    98,   121,
     190,   259,    79,    82,   187,   299,   168,   168,   190,   182,
     168,   261,   107,   168,   205,   285,   286,     1,   141,   290,
      46,   142,   182,   210,   168,   168,   210,   299,   214,   142,
     161,   320,   320,   161,   166,   210,   168,   299,   161,   234,
     161,   234,   161,   121,   280,   161,   210,   161,   167,   167,
     180,   142,   167,   320,   142,   169,   142,   169,   171,   312,
      46,   142,   171,   312,   119,   142,   171,     8,     1,   167,
     191,    63,   199,   200,   320,   193,   320,   205,   141,   243,
     166,   254,   161,   320,   320,   320,   320,   223,   320,   223,
     320,   320,   320,   320,   320,   320,   320,     3,    20,    33,
      61,    99,   105,   206,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,    66,   322,   322,   322,   322,   322,   299,   299,   223,
     320,   223,   320,    27,    46,    86,   111,   311,   314,   315,
     320,   336,    32,    46,    32,    46,    98,   168,    46,   171,
     205,   223,   320,   210,   161,   161,   320,   320,   121,   169,
     142,   220,   205,   283,   218,   205,   161,   283,    46,   295,
      43,   320,   223,   320,   168,   210,    43,   320,   223,   320,
     210,   164,   192,   195,   320,   192,   194,   182,   320,    34,
     320,   186,   169,   300,   205,   226,   227,    27,    46,    50,
      72,    75,    86,   105,   181,   262,   263,   264,   265,   251,
     286,   142,   169,    33,    48,    93,    97,   170,   209,   291,
     303,   121,   287,   320,   284,   320,   320,   169,   272,   320,
       1,   239,   299,   169,    21,   235,   291,   303,   142,   167,
     169,   169,   297,   169,   297,   182,   171,   223,   320,   171,
     182,   320,   171,   320,   171,   320,   167,   167,   205,   142,
     161,    13,   144,   142,   161,    13,    36,    70,   161,   168,
     295,   166,     1,   205,   247,   248,    27,    75,    86,   105,
     256,   266,   168,   161,   161,   161,   161,   161,   161,   169,
     171,    46,    86,   142,   169,    17,    20,    25,    44,    51,
      61,    68,    83,    95,   106,   112,   302,    85,    85,    43,
     223,   320,    43,   223,   320,   300,   223,   320,   168,   309,
     309,   161,   272,   322,   301,   205,   243,   161,   205,   243,
     161,   320,   169,   320,    32,   210,    32,   210,   313,   314,
     320,    32,   210,   312,    32,   210,   312,   142,   161,    13,
     161,    34,    34,   182,    98,   190,    63,   169,   142,   169,
      46,    86,   264,   142,   169,   168,   205,    27,    75,    86,
     105,   268,   169,   286,   290,     1,   295,    66,   322,   169,
     169,   167,    71,   113,   167,   240,   169,   168,   190,   205,
     236,   279,   182,   171,   312,   171,   312,   182,   119,   199,
     206,   166,   320,   107,   320,   195,   194,   300,     1,   244,
     167,   121,   142,   167,    86,   255,     1,     3,    12,    17,
      19,    20,    25,    39,    44,    51,    53,    61,    67,    68,
      83,    95,    99,   102,   106,   112,   136,   137,   138,   139,
     140,   143,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   159,   162,   163,   164,   165,   168,   204,
     205,   207,   257,   258,   259,   302,   169,   314,   290,   302,
     302,   320,    32,    32,   320,    32,    32,   169,   171,   171,
     300,   210,   210,   243,   166,   243,   166,   210,    98,    43,
     320,    43,   320,   142,   169,    98,    43,   320,   210,    43,
     320,   210,   320,   320,   182,   320,   182,    34,   205,   161,
     227,   190,   205,   263,   286,   141,   294,    86,   290,   287,
     171,    46,   171,   168,   168,    32,   182,   295,   236,   141,
     190,    43,   182,   320,   171,    43,   182,   320,   171,   320,
     192,    13,    36,    70,    36,    70,   161,   161,   169,   167,
      79,    82,   167,   181,   212,   320,   248,   268,   168,   260,
     320,   136,   144,   260,   260,   287,    98,    43,    43,    98,
      43,    43,    43,    43,   169,   166,   244,   166,   244,   320,
     320,   320,   314,   320,   320,   320,    13,    34,   182,   161,
     294,   169,   170,   209,   272,   293,   303,   146,   273,   287,
      59,   114,   274,   320,   291,   303,   299,   299,   182,   210,
     169,   320,   320,   182,   320,   182,   167,   107,   320,   195,
     194,   195,   194,   161,   212,     1,   141,   289,   262,   169,
       3,    99,   258,   259,   320,   320,   320,   320,   320,   320,
     244,   167,   244,   167,    98,    98,    98,    98,   320,   182,
     273,   287,   294,   171,   295,   272,   320,     3,    88,    99,
     275,   276,   277,   320,   190,   211,   271,   171,   169,   169,
     190,    98,    98,   161,   161,   161,   161,   170,   209,   288,
     303,   101,   269,   169,   260,   260,    98,    98,    98,    98,
      98,    98,   167,   167,   320,   320,   320,   320,   273,   287,
     272,   292,   293,   303,    46,   171,   277,   114,   142,   121,
     147,   149,   150,   153,   154,    59,   303,   160,   160,   320,
     320,     1,   171,   295,   274,   320,   292,   293,   320,   276,
     277,   277,   277,   277,   277,   277,   275,   171,   288,   303,
     171,   161,   270,   271,   171,   288,   303,   292
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   177,   178,   179,   179,   180,   180,   181,   181,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   183,   184,   184,   184,   185,   185,   185,
     186,   186,   187,   187,   187,   189,   188,   190,   190,   190,
     191,   191,   192,   192,   192,   192,   193,   193,   193,   193,
     193,   193,   194,   194,   195,   195,   196,   196,   196,   197,
     197,   197,   197,   197,   197,   197,   198,   199,   199,   199,
     200,   200,   201,   202,   202,   202,   202,   202,   202,   203,
     203,   204,   204,   204,   204,   204,   204,   205,   205,   205,
     205,   205,   205,   206,   206,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   209,   209,   209,   209,
     210,   210,   211,   211,   212,   212,   212,   213,   212,   214,
     214,   214,   214,   214,   214,   214,   215,   215,   215,   215,
     217,   216,   218,   216,   219,   216,   220,   216,   216,   216,
     216,   216,   221,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   223,
     224,   224,   224,   224,   224,   224,   224,   224,   225,   225,
     226,   226,   227,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   229,   229,   229,   229,
     229,   229,   230,   230,   230,   231,   231,   231,   232,   233,
     233,   233,   233,   233,   233,   234,   234,   235,   235,   235,
     236,   236,   237,   238,   238,   239,   239,   240,   240,   240,
     241,   241,   242,   242,   242,   243,   243,   244,   244,   244,
     245,   245,   246,   247,   247,   247,   248,   248,   250,   251,
     249,   252,   252,   252,   254,   255,   253,   256,   256,   256,
     256,   256,   257,   257,   258,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   260,   260,   261,   262,   262,   262,   263,
     263,   263,   263,   263,   263,   264,   264,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   266,   266,   266,   266,
     266,   266,   267,   267,   268,   268,   268,   268,   268,   268,
     269,   269,   270,   270,   271,   271,   272,   273,   273,   273,
     274,   274,   274,   274,   274,   275,   275,   276,   276,   276,
     276,   276,   276,   276,   277,   277,   278,   278,   278,   279,
     279,   280,   280,   280,   281,   281,   281,   281,   281,   282,
     282,   283,   283,   284,   284,   285,   285,   285,   286,   286,
     286,   287,   287,   287,   288,   288,   288,   288,   288,   288,
     288,   289,   289,   289,   289,   289,   290,   290,   290,   290,
     290,   291,   291,   291,   291,   292,   292,   292,   293,   293,
     293,   293,   293,   294,   294,   294,   294,   294,   295,   295,
     295,   295,   296,   296,   297,   297,   297,   298,   298,   299,
     299,   300,   300,   301,   301,   301,   301,   302,   302,   303,
     303,   303,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   306,   307,   308,   308,   308,   308,   308,   308,   308,
     308,   309,   309,   310,   311,   311,   312,   313,   313,   314,
     314,   314,   315,   315,   315,   315,   315,   315,   316,   316,
     317,   317,   317,   317,   317,   318,   318,   318,   318,   318,
     319,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   321,   321,
     322,   322,   322,   323,   323,   323,   323,   324,   324,   324,
     324,   324,   325,   325,   325,   326,   326,   326,   326,   326,
     327,   327,   327,   327,   328,   328,   329,   329,   330,   330,
     330,   330,   330,   330,   330,   330,   330,   330,   330,   330,
     330,   331,   331,   332,   332,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   333,   333,   333,   333,
     333,   333,   333,   334,   334,   334,   334,   335,   335,   335,
     335,   336,   336,   336,   336,   336,   336,   336
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
       4,     6,     7,     6,     7,     4,     5,     4,     5,     4,
       4,     3,     6,     5,     6,     5,     8,     7,     6,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     6,     4,     7,     5,     3,     6,     2,     3,
       3,     2,     2,     3,     3,     0,     2,     2,     3,     5,
       1,     3,     3,     5,     5,     0,     2,     3,     2,     3,
       6,     6,     1,     1,     1,     0,     2,     0,     2,     3,
       5,     5,     1,     1,     2,     3,     1,     3,     0,     0,
       8,     0,     1,     1,     0,     0,    10,     3,     3,     5,
       5,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     3,     3,     0,     1,     3,     4,
       5,     4,     5,     6,     6,     0,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     0,     1,     1,     2,
       1,     1,     1,     1,     0,     1,     2,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       0,     2,     2,     4,     4,     1,     3,     3,     3,     3,
       3,     3,     3,     2,     1,     1,     3,     4,     4,     2,
       4,     0,     2,     2,     1,     1,     1,     2,     1,     4,
       3,     1,     3,     3,     5,     1,     1,     3,     1,     2,
       3,     0,     2,     2,     3,     2,     4,     3,     3,     4,
       3,     0,     2,     2,     2,     1,     0,     2,     2,     2,
       1,     4,     4,     6,     3,     0,     1,     1,     3,     4,
       3,     4,     6,     0,     2,     2,     2,     2,     1,     1,
       3,     3,     1,     3,     1,     1,     1,     3,     3,     0,
       1,     1,     3,     3,     3,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     2,     4,     4,
       4,     5,     2,     2,     1,     2,     1,     2,     1,     2,
       1,     2,     1,     1,     6,     6,     4,     9,     9,     7,
       6,     6,     4,     9,     9,     7,     4,     6,     6,     9,
       9,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     1,     4,     1,     3,     4,     1,     3,     4,
       3,     3,     1,     1,     2,     1,     2,     1,     1,     3,
       1,     2,     2,     2,     2,     2,     8,     8,     9,     9,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     3,     3,     3,     2,     2,     2,     1,     0,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     4,     4,     4,     3,     3,     3,     3,     5,
       3,     4,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     4,     3,     4,     3,
       4,     3,     5,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

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

# ifndef YY_LOCATION_PRINT
#  if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

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

#   define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

#  else
#   define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined YY_LOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, context); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParserContext* context)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (context);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParserContext* context)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, context);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, ParserContext* context)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), context);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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
/* Parser data structure.  */
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

    /* Their size.  */
    YYPTRDIFF_T yystacksize;

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
    /* Whether this instance has not started parsing yet.  */
    int yynew;
  };






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, ParserContext* context)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (context);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
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
#define yystacksize yyps->yystacksize

/* Initialize the parser data structure.  */
static void
yypstate_clear (yypstate *yyps)
{
  yynerrs = 0;
  yystate = 0;
  yyerrstatus = 0;

  yystacksize = YYINITDEPTH;
  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;

  /* Initialize the state stack, in case yypcontext_expected_tokens is
     called before the first call to yyparse. */
  *yyssp = 0;
}

/* Initialize the parser data structure.  */
yypstate *
yypstate_new (void)
{
  yypstate *yyps;
  yyps = YY_CAST (yypstate *, malloc (sizeof *yyps));
  if (!yyps)
    return YY_NULLPTR;
  yyps->yynew = 1;
  yypstate_clear (yyps);
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
      if (!yyps->yynew && yyss != yyssa)
        YYSTACK_FREE (yyss);
#endif
      free (yyps);
    }
}



/*---------------.
| yypush_parse.  |
`---------------*/

int
yypush_parse (yypstate *yyps,
              int yypushed_char, YYSTYPE const *yypushed_val, YYLTYPE *yypushed_loc, ParserContext* context)
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
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  if (!yyps->yynew)
    {
      yyn = yypact[yystate];
      goto yyread_pushed_token;
    }

  YYDPRINTF ((stderr, "Starting parse\n"));

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
  YY_STACK_PRINT (yyss, yyssp);

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
#  undef YYSTACK_RELOCATE
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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
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
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yypushed_char;
      if (yypushed_val)
        yylval = *yypushed_val;
      if (yypushed_loc)
        yylloc = *yypushed_loc;
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
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
#line 6708 "bison-chapel.cpp"
    break;

  case 3:
#line 514 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6714 "bison-chapel.cpp"
    break;

  case 4:
#line 515 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6720 "bison-chapel.cpp"
    break;

  case 6:
#line 522 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6726 "bison-chapel.cpp"
    break;

  case 7:
#line 527 "chapel.ypp"
                                       { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6732 "bison-chapel.cpp"
    break;

  case 8:
#line 528 "chapel.ypp"
                                       { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6738 "bison-chapel.cpp"
    break;

  case 27:
#line 551 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6744 "bison-chapel.cpp"
    break;

  case 28:
#line 552 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6750 "bison-chapel.cpp"
    break;

  case 29:
#line 553 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6756 "bison-chapel.cpp"
    break;

  case 30:
#line 554 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6762 "bison-chapel.cpp"
    break;

  case 31:
#line 555 "chapel.ypp"
                                         { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6768 "bison-chapel.cpp"
    break;

  case 32:
#line 556 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6774 "bison-chapel.cpp"
    break;

  case 33:
#line 557 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6780 "bison-chapel.cpp"
    break;

  case 34:
#line 558 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6786 "bison-chapel.cpp"
    break;

  case 35:
#line 559 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6792 "bison-chapel.cpp"
    break;

  case 36:
#line 560 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6798 "bison-chapel.cpp"
    break;

  case 37:
#line 561 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6804 "bison-chapel.cpp"
    break;

  case 38:
#line 562 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6810 "bison-chapel.cpp"
    break;

  case 39:
#line 563 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6816 "bison-chapel.cpp"
    break;

  case 40:
#line 564 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6822 "bison-chapel.cpp"
    break;

  case 41:
#line 565 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6828 "bison-chapel.cpp"
    break;

  case 42:
#line 566 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6834 "bison-chapel.cpp"
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
#line 6848 "bison-chapel.cpp"
    break;

  case 44:
#line 584 "chapel.ypp"
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6857 "bison-chapel.cpp"
    break;

  case 45:
#line 589 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6866 "bison-chapel.cpp"
    break;

  case 46:
#line 594 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6875 "bison-chapel.cpp"
    break;

  case 47:
#line 601 "chapel.ypp"
        { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6881 "bison-chapel.cpp"
    break;

  case 48:
#line 602 "chapel.ypp"
          { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6887 "bison-chapel.cpp"
    break;

  case 49:
#line 603 "chapel.ypp"
           { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6893 "bison-chapel.cpp"
    break;

  case 50:
#line 607 "chapel.ypp"
             { (yyval.b) = false; }
#line 6899 "bison-chapel.cpp"
    break;

  case 51:
#line 608 "chapel.ypp"
             { (yyval.b) = true;  }
#line 6905 "bison-chapel.cpp"
    break;

  case 52:
#line 612 "chapel.ypp"
           { (yyval.b) = false; }
#line 6911 "bison-chapel.cpp"
    break;

  case 53:
#line 613 "chapel.ypp"
           { (yyval.b) = false; }
#line 6917 "bison-chapel.cpp"
    break;

  case 54:
#line 614 "chapel.ypp"
           { (yyval.b) = true; }
#line 6923 "bison-chapel.cpp"
    break;

  case 55:
#line 619 "chapel.ypp"
  {
    (yylsp[0]).comment = context->latestComment;
    context->latestComment = NULL;
  }
#line 6932 "bison-chapel.cpp"
    break;

  case 56:
#line 624 "chapel.ypp"
 {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[-1].pch), (yyvsp[-4].b), (yyvsp[-3].b), (yylsp[-6]).comment);
 }
#line 6940 "bison-chapel.cpp"
    break;

  case 57:
#line 639 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); }
#line 6946 "bison-chapel.cpp"
    break;

  case 58:
#line 640 "chapel.ypp"
                                       { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6952 "bison-chapel.cpp"
    break;

  case 59:
#line 641 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6958 "bison-chapel.cpp"
    break;

  case 60:
#line 646 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6964 "bison-chapel.cpp"
    break;

  case 61:
#line 647 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6970 "bison-chapel.cpp"
    break;

  case 62:
#line 652 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6980 "bison-chapel.cpp"
    break;

  case 63:
#line 657 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6990 "bison-chapel.cpp"
    break;

  case 64:
#line 662 "chapel.ypp"
                                          { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6999 "bison-chapel.cpp"
    break;

  case 65:
#line 666 "chapel.ypp"
                                          { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7008 "bison-chapel.cpp"
    break;

  case 66:
#line 675 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 7018 "bison-chapel.cpp"
    break;

  case 67:
#line 680 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 7028 "bison-chapel.cpp"
    break;

  case 68:
#line 685 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
#line 7038 "bison-chapel.cpp"
    break;

  case 69:
#line 690 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 7047 "bison-chapel.cpp"
    break;

  case 70:
#line 694 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7056 "bison-chapel.cpp"
    break;

  case 71:
#line 698 "chapel.ypp"
                                             { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7065 "bison-chapel.cpp"
    break;

  case 72:
#line 706 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 7075 "bison-chapel.cpp"
    break;

  case 74:
#line 715 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 7085 "bison-chapel.cpp"
    break;

  case 76:
#line 724 "chapel.ypp"
           { (yyval.b) = true; }
#line 7091 "bison-chapel.cpp"
    break;

  case 77:
#line 725 "chapel.ypp"
           { (yyval.b) = false; }
#line 7097 "bison-chapel.cpp"
    break;

  case 78:
#line 726 "chapel.ypp"
           { (yyval.b) = true; }
#line 7103 "bison-chapel.cpp"
    break;

  case 79:
#line 730 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 7109 "bison-chapel.cpp"
    break;

  case 80:
#line 731 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 7115 "bison-chapel.cpp"
    break;

  case 81:
#line 732 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7121 "bison-chapel.cpp"
    break;

  case 82:
#line 733 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7127 "bison-chapel.cpp"
    break;

  case 83:
#line 734 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 7133 "bison-chapel.cpp"
    break;

  case 84:
#line 735 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7139 "bison-chapel.cpp"
    break;

  case 85:
#line 736 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7145 "bison-chapel.cpp"
    break;

  case 86:
#line 740 "chapel.ypp"
                                             { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[-3].b));}
#line 7152 "bison-chapel.cpp"
    break;

  case 87:
#line 745 "chapel.ypp"
       { (yyval.pimportstmt) = buildImportStmt((yyvsp[0].pexpr)); }
#line 7158 "bison-chapel.cpp"
    break;

  case 88:
#line 746 "chapel.ypp"
                     { (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7164 "bison-chapel.cpp"
    break;

  case 89:
#line 747 "chapel.ypp"
                                   { (yyval.pimportstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist)); }
#line 7170 "bison-chapel.cpp"
    break;

  case 90:
#line 751 "chapel.ypp"
              { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pimportstmt)); }
#line 7176 "bison-chapel.cpp"
    break;

  case 91:
#line 752 "chapel.ypp"
                               { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pimportstmt)); }
#line 7182 "bison-chapel.cpp"
    break;

  case 92:
#line 756 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 7188 "bison-chapel.cpp"
    break;

  case 93:
#line 761 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 7194 "bison-chapel.cpp"
    break;

  case 94:
#line 763 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 7200 "bison-chapel.cpp"
    break;

  case 95:
#line 765 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 7206 "bison-chapel.cpp"
    break;

  case 96:
#line 767 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 7212 "bison-chapel.cpp"
    break;

  case 97:
#line 769 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 7218 "bison-chapel.cpp"
    break;

  case 98:
#line 771 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), new SymExpr(gNoInit), "="); }
#line 7224 "bison-chapel.cpp"
    break;

  case 99:
#line 777 "chapel.ypp"
         { (yyval.pch) = NULL; }
#line 7230 "bison-chapel.cpp"
    break;

  case 100:
#line 778 "chapel.ypp"
         { (yyval.pch) = (yyvsp[0].pch); }
#line 7236 "bison-chapel.cpp"
    break;

  case 101:
#line 782 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7242 "bison-chapel.cpp"
    break;

  case 102:
#line 783 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7248 "bison-chapel.cpp"
    break;

  case 103:
#line 784 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7254 "bison-chapel.cpp"
    break;

  case 104:
#line 785 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7260 "bison-chapel.cpp"
    break;

  case 105:
#line 786 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7266 "bison-chapel.cpp"
    break;

  case 106:
#line 787 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7272 "bison-chapel.cpp"
    break;

  case 107:
#line 790 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7278 "bison-chapel.cpp"
    break;

  case 108:
#line 791 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7284 "bison-chapel.cpp"
    break;

  case 109:
#line 792 "chapel.ypp"
                           { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 7290 "bison-chapel.cpp"
    break;

  case 110:
#line 793 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7296 "bison-chapel.cpp"
    break;

  case 111:
#line 794 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7302 "bison-chapel.cpp"
    break;

  case 112:
#line 795 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7308 "bison-chapel.cpp"
    break;

  case 113:
#line 807 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7314 "bison-chapel.cpp"
    break;

  case 114:
#line 808 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7320 "bison-chapel.cpp"
    break;

  case 115:
#line 819 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 7326 "bison-chapel.cpp"
    break;

  case 116:
#line 820 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 7332 "bison-chapel.cpp"
    break;

  case 117:
#line 821 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 7338 "bison-chapel.cpp"
    break;

  case 118:
#line 822 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 7344 "bison-chapel.cpp"
    break;

  case 119:
#line 823 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 7350 "bison-chapel.cpp"
    break;

  case 120:
#line 824 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 7356 "bison-chapel.cpp"
    break;

  case 121:
#line 825 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 7362 "bison-chapel.cpp"
    break;

  case 122:
#line 826 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 7368 "bison-chapel.cpp"
    break;

  case 123:
#line 827 "chapel.ypp"
             { (yyval.pch) = "sync"; }
#line 7374 "bison-chapel.cpp"
    break;

  case 124:
#line 828 "chapel.ypp"
             { (yyval.pch) = "single"; }
#line 7380 "bison-chapel.cpp"
    break;

  case 125:
#line 829 "chapel.ypp"
             { (yyval.pch) = "owned"; }
#line 7386 "bison-chapel.cpp"
    break;

  case 126:
#line 830 "chapel.ypp"
             { (yyval.pch) = "shared"; }
#line 7392 "bison-chapel.cpp"
    break;

  case 127:
#line 831 "chapel.ypp"
             { (yyval.pch) = "borrowed"; }
#line 7398 "bison-chapel.cpp"
    break;

  case 128:
#line 832 "chapel.ypp"
             { (yyval.pch) = "unmanaged"; }
#line 7404 "bison-chapel.cpp"
    break;

  case 129:
#line 833 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 7410 "bison-chapel.cpp"
    break;

  case 130:
#line 834 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 7416 "bison-chapel.cpp"
    break;

  case 131:
#line 835 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 7422 "bison-chapel.cpp"
    break;

  case 132:
#line 836 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 7428 "bison-chapel.cpp"
    break;

  case 133:
#line 837 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 7434 "bison-chapel.cpp"
    break;

  case 134:
#line 841 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 7440 "bison-chapel.cpp"
    break;

  case 135:
#line 842 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 7446 "bison-chapel.cpp"
    break;

  case 136:
#line 843 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 7452 "bison-chapel.cpp"
    break;

  case 137:
#line 844 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 7458 "bison-chapel.cpp"
    break;

  case 138:
#line 845 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7464 "bison-chapel.cpp"
    break;

  case 139:
#line 846 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7470 "bison-chapel.cpp"
    break;

  case 140:
#line 847 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 7476 "bison-chapel.cpp"
    break;

  case 141:
#line 848 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 7482 "bison-chapel.cpp"
    break;

  case 142:
#line 849 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 7488 "bison-chapel.cpp"
    break;

  case 143:
#line 850 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 7494 "bison-chapel.cpp"
    break;

  case 144:
#line 851 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 7500 "bison-chapel.cpp"
    break;

  case 145:
#line 852 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 7506 "bison-chapel.cpp"
    break;

  case 146:
#line 859 "chapel.ypp"
             { (yyval.pch) = "_syncvar"; }
#line 7512 "bison-chapel.cpp"
    break;

  case 147:
#line 860 "chapel.ypp"
             { (yyval.pch) = "_singlevar"; }
#line 7518 "bison-chapel.cpp"
    break;

  case 148:
#line 861 "chapel.ypp"
             { (yyval.pch) = "_domain"; }
#line 7524 "bison-chapel.cpp"
    break;

  case 149:
#line 862 "chapel.ypp"
             { (yyval.pch) = "_index"; }
#line 7530 "bison-chapel.cpp"
    break;

  case 150:
#line 866 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7536 "bison-chapel.cpp"
    break;

  case 151:
#line 867 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7542 "bison-chapel.cpp"
    break;

  case 152:
#line 871 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 7548 "bison-chapel.cpp"
    break;

  case 153:
#line 872 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 7554 "bison-chapel.cpp"
    break;

  case 154:
#line 876 "chapel.ypp"
                        { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 7560 "bison-chapel.cpp"
    break;

  case 156:
#line 878 "chapel.ypp"
                        { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7566 "bison-chapel.cpp"
    break;

  case 157:
#line 888 "chapel.ypp"
           { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 7572 "bison-chapel.cpp"
    break;

  case 158:
#line 889 "chapel.ypp"
                 { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7578 "bison-chapel.cpp"
    break;

  case 166:
#line 903 "chapel.ypp"
                          { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 7584 "bison-chapel.cpp"
    break;

  case 167:
#line 904 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 7590 "bison-chapel.cpp"
    break;

  case 168:
#line 905 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 7596 "bison-chapel.cpp"
    break;

  case 169:
#line 906 "chapel.ypp"
                            { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 7602 "bison-chapel.cpp"
    break;

  case 170:
#line 911 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7611 "bison-chapel.cpp"
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
#line 7625 "bison-chapel.cpp"
    break;

  case 172:
#line 926 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7634 "bison-chapel.cpp"
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
#line 7649 "bison-chapel.cpp"
    break;

  case 174:
#line 942 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7658 "bison-chapel.cpp"
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
#line 7672 "bison-chapel.cpp"
    break;

  case 176:
#line 957 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7681 "bison-chapel.cpp"
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
#line 7695 "bison-chapel.cpp"
    break;

  case 178:
#line 973 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7703 "bison-chapel.cpp"
    break;

  case 179:
#line 977 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7711 "bison-chapel.cpp"
    break;

  case 180:
#line 982 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7723 "bison-chapel.cpp"
    break;

  case 181:
#line 990 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7735 "bison-chapel.cpp"
    break;

  case 182:
#line 1001 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 7743 "bison-chapel.cpp"
    break;

  case 183:
#line 1007 "chapel.ypp"
                                                { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 7749 "bison-chapel.cpp"
    break;

  case 184:
#line 1008 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7755 "bison-chapel.cpp"
    break;

  case 185:
#line 1009 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 7761 "bison-chapel.cpp"
    break;

  case 186:
#line 1010 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 7767 "bison-chapel.cpp"
    break;

  case 187:
#line 1011 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 7773 "bison-chapel.cpp"
    break;

  case 188:
#line 1012 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7779 "bison-chapel.cpp"
    break;

  case 189:
#line 1013 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7785 "bison-chapel.cpp"
    break;

  case 190:
#line 1014 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7791 "bison-chapel.cpp"
    break;

  case 191:
#line 1015 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7797 "bison-chapel.cpp"
    break;

  case 192:
#line 1016 "chapel.ypp"
                                                { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7803 "bison-chapel.cpp"
    break;

  case 193:
#line 1017 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7809 "bison-chapel.cpp"
    break;

  case 194:
#line 1018 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7815 "bison-chapel.cpp"
    break;

  case 195:
#line 1019 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7821 "bison-chapel.cpp"
    break;

  case 196:
#line 1020 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7827 "bison-chapel.cpp"
    break;

  case 197:
#line 1021 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7833 "bison-chapel.cpp"
    break;

  case 198:
#line 1022 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7839 "bison-chapel.cpp"
    break;

  case 199:
#line 1023 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7845 "bison-chapel.cpp"
    break;

  case 200:
#line 1024 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7851 "bison-chapel.cpp"
    break;

  case 201:
#line 1026 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7861 "bison-chapel.cpp"
    break;

  case 202:
#line 1032 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7871 "bison-chapel.cpp"
    break;

  case 203:
#line 1038 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7881 "bison-chapel.cpp"
    break;

  case 204:
#line 1044 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7891 "bison-chapel.cpp"
    break;

  case 205:
#line 1050 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7902 "bison-chapel.cpp"
    break;

  case 206:
#line 1057 "chapel.ypp"
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7913 "bison-chapel.cpp"
    break;

  case 207:
#line 1064 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7921 "bison-chapel.cpp"
    break;

  case 208:
#line 1068 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7929 "bison-chapel.cpp"
    break;

  case 209:
#line 1074 "chapel.ypp"
                            { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 7935 "bison-chapel.cpp"
    break;

  case 210:
#line 1078 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7941 "bison-chapel.cpp"
    break;

  case 211:
#line 1079 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7947 "bison-chapel.cpp"
    break;

  case 212:
#line 1080 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7953 "bison-chapel.cpp"
    break;

  case 213:
#line 1081 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7959 "bison-chapel.cpp"
    break;

  case 214:
#line 1082 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7966 "bison-chapel.cpp"
    break;

  case 215:
#line 1084 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7973 "bison-chapel.cpp"
    break;

  case 216:
#line 1086 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7980 "bison-chapel.cpp"
    break;

  case 217:
#line 1088 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7987 "bison-chapel.cpp"
    break;

  case 218:
#line 1094 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), (yyvsp[0].pblockstmt))); }
#line 7993 "bison-chapel.cpp"
    break;

  case 219:
#line 1096 "chapel.ypp"
  { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-1].pch), ls, (yyvsp[0].pblockstmt))); }
#line 8002 "bison-chapel.cpp"
    break;

  case 220:
#line 1103 "chapel.ypp"
                                  { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 8008 "bison-chapel.cpp"
    break;

  case 221:
#line 1104 "chapel.ypp"
                                  { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 8014 "bison-chapel.cpp"
    break;

  case 222:
#line 1109 "chapel.ypp"
             { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[0].pch), INTENT_TYPE); }
#line 8020 "bison-chapel.cpp"
    break;

  case 223:
#line 1114 "chapel.ypp"
             { (yyval.pch) = (yyvsp[0].pch); }
#line 8026 "bison-chapel.cpp"
    break;

  case 224:
#line 1115 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 8032 "bison-chapel.cpp"
    break;

  case 225:
#line 1116 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 8038 "bison-chapel.cpp"
    break;

  case 226:
#line 1117 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 8044 "bison-chapel.cpp"
    break;

  case 227:
#line 1118 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 8050 "bison-chapel.cpp"
    break;

  case 228:
#line 1119 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 8056 "bison-chapel.cpp"
    break;

  case 229:
#line 1120 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 8062 "bison-chapel.cpp"
    break;

  case 230:
#line 1121 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 8068 "bison-chapel.cpp"
    break;

  case 231:
#line 1122 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 8074 "bison-chapel.cpp"
    break;

  case 232:
#line 1123 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 8080 "bison-chapel.cpp"
    break;

  case 233:
#line 1124 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 8086 "bison-chapel.cpp"
    break;

  case 234:
#line 1125 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 8092 "bison-chapel.cpp"
    break;

  case 235:
#line 1127 "chapel.ypp"
  { (yyval.pch) = (yyvsp[0].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[0].pch)); }
#line 8099 "bison-chapel.cpp"
    break;

  case 236:
#line 1133 "chapel.ypp"
             { (yyval.pch) = "none"; }
#line 8105 "bison-chapel.cpp"
    break;

  case 237:
#line 1134 "chapel.ypp"
             { (yyval.pch) = "this"; }
#line 8111 "bison-chapel.cpp"
    break;

  case 238:
#line 1135 "chapel.ypp"
             { (yyval.pch) = "false"; }
#line 8117 "bison-chapel.cpp"
    break;

  case 239:
#line 1136 "chapel.ypp"
             { (yyval.pch) = "true"; }
#line 8123 "bison-chapel.cpp"
    break;

  case 240:
#line 1145 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 8129 "bison-chapel.cpp"
    break;

  case 241:
#line 1146 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 8135 "bison-chapel.cpp"
    break;

  case 242:
#line 1151 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8141 "bison-chapel.cpp"
    break;

  case 243:
#line 1153 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-3].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-1].pch), act, NULL)); }
#line 8148 "bison-chapel.cpp"
    break;

  case 244:
#line 1156 "chapel.ypp"
  { (yyvsp[-2].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-6].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8155 "bison-chapel.cpp"
    break;

  case 245:
#line 1162 "chapel.ypp"
  { (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8161 "bison-chapel.cpp"
    break;

  case 246:
#line 1164 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-2].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[0].pch), act); }
#line 8168 "bison-chapel.cpp"
    break;

  case 247:
#line 1167 "chapel.ypp"
  { (yyvsp[-1].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-5].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8175 "bison-chapel.cpp"
    break;

  case 248:
#line 1172 "chapel.ypp"
                          { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 8181 "bison-chapel.cpp"
    break;

  case 249:
#line 1175 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 8187 "bison-chapel.cpp"
    break;

  case 250:
#line 1176 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 8193 "bison-chapel.cpp"
    break;

  case 251:
#line 1177 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 8199 "bison-chapel.cpp"
    break;

  case 252:
#line 1178 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 8205 "bison-chapel.cpp"
    break;

  case 253:
#line 1179 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8211 "bison-chapel.cpp"
    break;

  case 254:
#line 1180 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8217 "bison-chapel.cpp"
    break;

  case 255:
#line 1184 "chapel.ypp"
                           { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8223 "bison-chapel.cpp"
    break;

  case 256:
#line 1185 "chapel.ypp"
                           { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8229 "bison-chapel.cpp"
    break;

  case 257:
#line 1189 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 8235 "bison-chapel.cpp"
    break;

  case 258:
#line 1190 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8241 "bison-chapel.cpp"
    break;

  case 259:
#line 1191 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8247 "bison-chapel.cpp"
    break;

  case 260:
#line 1195 "chapel.ypp"
                      { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 8253 "bison-chapel.cpp"
    break;

  case 261:
#line 1196 "chapel.ypp"
                        { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 8259 "bison-chapel.cpp"
    break;

  case 262:
#line 1200 "chapel.ypp"
                    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 8265 "bison-chapel.cpp"
    break;

  case 263:
#line 1204 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 8271 "bison-chapel.cpp"
    break;

  case 264:
#line 1206 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 8277 "bison-chapel.cpp"
    break;

  case 265:
#line 1210 "chapel.ypp"
                          { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8283 "bison-chapel.cpp"
    break;

  case 266:
#line 1211 "chapel.ypp"
                          { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8289 "bison-chapel.cpp"
    break;

  case 267:
#line 1216 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 8295 "bison-chapel.cpp"
    break;

  case 268:
#line 1218 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8301 "bison-chapel.cpp"
    break;

  case 269:
#line 1220 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8307 "bison-chapel.cpp"
    break;

  case 270:
#line 1227 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8321 "bison-chapel.cpp"
    break;

  case 271:
#line 1237 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8335 "bison-chapel.cpp"
    break;

  case 272:
#line 1250 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8345 "bison-chapel.cpp"
    break;

  case 273:
#line 1255 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8355 "bison-chapel.cpp"
    break;

  case 274:
#line 1260 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8365 "bison-chapel.cpp"
    break;

  case 275:
#line 1268 "chapel.ypp"
                  { (yyval.pcallexpr) = NULL; }
#line 8371 "bison-chapel.cpp"
    break;

  case 276:
#line 1269 "chapel.ypp"
                  { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 8377 "bison-chapel.cpp"
    break;

  case 277:
#line 1274 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 8383 "bison-chapel.cpp"
    break;

  case 278:
#line 1276 "chapel.ypp"
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 8389 "bison-chapel.cpp"
    break;

  case 279:
#line 1278 "chapel.ypp"
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 8395 "bison-chapel.cpp"
    break;

  case 280:
#line 1283 "chapel.ypp"
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
#line 8415 "bison-chapel.cpp"
    break;

  case 281:
#line 1299 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 8423 "bison-chapel.cpp"
    break;

  case 282:
#line 1306 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 8433 "bison-chapel.cpp"
    break;

  case 283:
#line 1315 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 8445 "bison-chapel.cpp"
    break;

  case 284:
#line 1323 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 8453 "bison-chapel.cpp"
    break;

  case 285:
#line 1327 "chapel.ypp"
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 8461 "bison-chapel.cpp"
    break;

  case 286:
#line 1333 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 8467 "bison-chapel.cpp"
    break;

  case 287:
#line 1334 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 8473 "bison-chapel.cpp"
    break;

  case 288:
#line 1339 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 8482 "bison-chapel.cpp"
    break;

  case 289:
#line 1344 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8491 "bison-chapel.cpp"
    break;

  case 290:
#line 1349 "chapel.ypp"
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
#line 8513 "bison-chapel.cpp"
    break;

  case 291:
#line 1371 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 8521 "bison-chapel.cpp"
    break;

  case 292:
#line 1374 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 8530 "bison-chapel.cpp"
    break;

  case 293:
#line 1378 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 8539 "bison-chapel.cpp"
    break;

  case 294:
#line 1386 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 8553 "bison-chapel.cpp"
    break;

  case 295:
#line 1396 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8564 "bison-chapel.cpp"
    break;

  case 296:
#line 1403 "chapel.ypp"
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
#line 8591 "bison-chapel.cpp"
    break;

  case 297:
#line 1429 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 8599 "bison-chapel.cpp"
    break;

  case 298:
#line 1433 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8608 "bison-chapel.cpp"
    break;

  case 299:
#line 1438 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 8616 "bison-chapel.cpp"
    break;

  case 300:
#line 1442 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8625 "bison-chapel.cpp"
    break;

  case 301:
#line 1447 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 8633 "bison-chapel.cpp"
    break;

  case 303:
#line 1454 "chapel.ypp"
                      { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8639 "bison-chapel.cpp"
    break;

  case 304:
#line 1458 "chapel.ypp"
                  { (yyval.pch) = (yyvsp[0].pch); }
#line 8645 "bison-chapel.cpp"
    break;

  case 305:
#line 1459 "chapel.ypp"
                 { (yyval.pch) = "&"; }
#line 8651 "bison-chapel.cpp"
    break;

  case 306:
#line 1460 "chapel.ypp"
                 { (yyval.pch) = "|"; }
#line 8657 "bison-chapel.cpp"
    break;

  case 307:
#line 1461 "chapel.ypp"
                 { (yyval.pch) = "^"; }
#line 8663 "bison-chapel.cpp"
    break;

  case 308:
#line 1462 "chapel.ypp"
                 { (yyval.pch) = "~"; }
#line 8669 "bison-chapel.cpp"
    break;

  case 309:
#line 1463 "chapel.ypp"
                 { (yyval.pch) = "=="; }
#line 8675 "bison-chapel.cpp"
    break;

  case 310:
#line 1464 "chapel.ypp"
                 { (yyval.pch) = "!="; }
#line 8681 "bison-chapel.cpp"
    break;

  case 311:
#line 1465 "chapel.ypp"
                 { (yyval.pch) = "<="; }
#line 8687 "bison-chapel.cpp"
    break;

  case 312:
#line 1466 "chapel.ypp"
                 { (yyval.pch) = ">="; }
#line 8693 "bison-chapel.cpp"
    break;

  case 313:
#line 1467 "chapel.ypp"
                 { (yyval.pch) = "<"; }
#line 8699 "bison-chapel.cpp"
    break;

  case 314:
#line 1468 "chapel.ypp"
                 { (yyval.pch) = ">"; }
#line 8705 "bison-chapel.cpp"
    break;

  case 315:
#line 1469 "chapel.ypp"
                 { (yyval.pch) = "+"; }
#line 8711 "bison-chapel.cpp"
    break;

  case 316:
#line 1470 "chapel.ypp"
                 { (yyval.pch) = "-"; }
#line 8717 "bison-chapel.cpp"
    break;

  case 317:
#line 1471 "chapel.ypp"
                 { (yyval.pch) = "*"; }
#line 8723 "bison-chapel.cpp"
    break;

  case 318:
#line 1472 "chapel.ypp"
                 { (yyval.pch) = "/"; }
#line 8729 "bison-chapel.cpp"
    break;

  case 319:
#line 1473 "chapel.ypp"
                 { (yyval.pch) = "<<"; }
#line 8735 "bison-chapel.cpp"
    break;

  case 320:
#line 1474 "chapel.ypp"
                 { (yyval.pch) = ">>"; }
#line 8741 "bison-chapel.cpp"
    break;

  case 321:
#line 1475 "chapel.ypp"
                 { (yyval.pch) = "%"; }
#line 8747 "bison-chapel.cpp"
    break;

  case 322:
#line 1476 "chapel.ypp"
                 { (yyval.pch) = "**"; }
#line 8753 "bison-chapel.cpp"
    break;

  case 323:
#line 1477 "chapel.ypp"
                 { (yyval.pch) = "!"; }
#line 8759 "bison-chapel.cpp"
    break;

  case 324:
#line 1478 "chapel.ypp"
                 { (yyval.pch) = "chpl_by"; }
#line 8765 "bison-chapel.cpp"
    break;

  case 325:
#line 1479 "chapel.ypp"
                 { (yyval.pch) = "#"; }
#line 8771 "bison-chapel.cpp"
    break;

  case 326:
#line 1480 "chapel.ypp"
                 { (yyval.pch) = "chpl_align"; }
#line 8777 "bison-chapel.cpp"
    break;

  case 327:
#line 1481 "chapel.ypp"
                 { (yyval.pch) = "<=>"; }
#line 8783 "bison-chapel.cpp"
    break;

  case 328:
#line 1482 "chapel.ypp"
                 { (yyval.pch) = "<~>"; }
#line 8789 "bison-chapel.cpp"
    break;

  case 329:
#line 1483 "chapel.ypp"
                 { (yyval.pch) = "init="; }
#line 8795 "bison-chapel.cpp"
    break;

  case 330:
#line 1484 "chapel.ypp"
                  { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 8801 "bison-chapel.cpp"
    break;

  case 331:
#line 1488 "chapel.ypp"
                 { (yyval.pch) = "="; }
#line 8807 "bison-chapel.cpp"
    break;

  case 332:
#line 1489 "chapel.ypp"
                 { (yyval.pch) = "+="; }
#line 8813 "bison-chapel.cpp"
    break;

  case 333:
#line 1490 "chapel.ypp"
                 { (yyval.pch) = "-="; }
#line 8819 "bison-chapel.cpp"
    break;

  case 334:
#line 1491 "chapel.ypp"
                  { (yyval.pch) = "*="; }
#line 8825 "bison-chapel.cpp"
    break;

  case 335:
#line 1492 "chapel.ypp"
                 { (yyval.pch) = "/="; }
#line 8831 "bison-chapel.cpp"
    break;

  case 336:
#line 1493 "chapel.ypp"
                 { (yyval.pch) = "%="; }
#line 8837 "bison-chapel.cpp"
    break;

  case 337:
#line 1494 "chapel.ypp"
                 { (yyval.pch) = "**="; }
#line 8843 "bison-chapel.cpp"
    break;

  case 338:
#line 1495 "chapel.ypp"
                 { (yyval.pch) = "&="; }
#line 8849 "bison-chapel.cpp"
    break;

  case 339:
#line 1496 "chapel.ypp"
                 { (yyval.pch) = "|="; }
#line 8855 "bison-chapel.cpp"
    break;

  case 340:
#line 1497 "chapel.ypp"
                 { (yyval.pch) = "^="; }
#line 8861 "bison-chapel.cpp"
    break;

  case 341:
#line 1498 "chapel.ypp"
                 { (yyval.pch) = ">>="; }
#line 8867 "bison-chapel.cpp"
    break;

  case 342:
#line 1499 "chapel.ypp"
                 { (yyval.pch) = "<<="; }
#line 8873 "bison-chapel.cpp"
    break;

  case 343:
#line 1503 "chapel.ypp"
                     { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 8879 "bison-chapel.cpp"
    break;

  case 344:
#line 1504 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8885 "bison-chapel.cpp"
    break;

  case 345:
#line 1508 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8891 "bison-chapel.cpp"
    break;

  case 346:
#line 1512 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 8897 "bison-chapel.cpp"
    break;

  case 347:
#line 1513 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 8903 "bison-chapel.cpp"
    break;

  case 348:
#line 1514 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 8909 "bison-chapel.cpp"
    break;

  case 349:
#line 1519 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 8915 "bison-chapel.cpp"
    break;

  case 350:
#line 1521 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 8921 "bison-chapel.cpp"
    break;

  case 351:
#line 1523 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 8927 "bison-chapel.cpp"
    break;

  case 352:
#line 1525 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr))); }
#line 8933 "bison-chapel.cpp"
    break;

  case 353:
#line 1527 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8939 "bison-chapel.cpp"
    break;

  case 354:
#line 1529 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 8945 "bison-chapel.cpp"
    break;

  case 355:
#line 1533 "chapel.ypp"
                      { (yyval.pt) = INTENT_BLANK; }
#line 8951 "bison-chapel.cpp"
    break;

  case 356:
#line 1534 "chapel.ypp"
                      { (yyval.pt) = (yyvsp[0].pt); }
#line 8957 "bison-chapel.cpp"
    break;

  case 357:
#line 1538 "chapel.ypp"
          { (yyval.pt) = INTENT_IN; }
#line 8963 "bison-chapel.cpp"
    break;

  case 358:
#line 1539 "chapel.ypp"
          { (yyval.pt) = INTENT_INOUT; }
#line 8969 "bison-chapel.cpp"
    break;

  case 359:
#line 1540 "chapel.ypp"
          { (yyval.pt) = INTENT_OUT; }
#line 8975 "bison-chapel.cpp"
    break;

  case 360:
#line 1541 "chapel.ypp"
          { (yyval.pt) = INTENT_CONST; }
#line 8981 "bison-chapel.cpp"
    break;

  case 361:
#line 1542 "chapel.ypp"
             { (yyval.pt) = INTENT_CONST_IN; }
#line 8987 "bison-chapel.cpp"
    break;

  case 362:
#line 1543 "chapel.ypp"
              { (yyval.pt) = INTENT_CONST_REF; }
#line 8993 "bison-chapel.cpp"
    break;

  case 363:
#line 1544 "chapel.ypp"
          { (yyval.pt) = INTENT_PARAM; }
#line 8999 "bison-chapel.cpp"
    break;

  case 364:
#line 1545 "chapel.ypp"
          { (yyval.pt) = INTENT_REF; }
#line 9005 "bison-chapel.cpp"
    break;

  case 365:
#line 1546 "chapel.ypp"
          { (yyval.pt) = INTENT_TYPE; }
#line 9011 "bison-chapel.cpp"
    break;

  case 366:
#line 1550 "chapel.ypp"
         { (yyval.pt) = INTENT_BLANK; }
#line 9017 "bison-chapel.cpp"
    break;

  case 367:
#line 1551 "chapel.ypp"
         { (yyval.pt) = INTENT_PARAM; }
#line 9023 "bison-chapel.cpp"
    break;

  case 368:
#line 1552 "chapel.ypp"
         { (yyval.pt) = INTENT_REF;   }
#line 9029 "bison-chapel.cpp"
    break;

  case 369:
#line 1553 "chapel.ypp"
                { (yyval.pt) = INTENT_CONST_REF;   }
#line 9035 "bison-chapel.cpp"
    break;

  case 370:
#line 1554 "chapel.ypp"
         { (yyval.pt) = INTENT_CONST;   }
#line 9041 "bison-chapel.cpp"
    break;

  case 371:
#line 1555 "chapel.ypp"
         { (yyval.pt) = INTENT_TYPE;  }
#line 9047 "bison-chapel.cpp"
    break;

  case 372:
#line 1559 "chapel.ypp"
         { (yyval.procIter) = ProcIter_PROC; }
#line 9053 "bison-chapel.cpp"
    break;

  case 373:
#line 1560 "chapel.ypp"
         { (yyval.procIter) = ProcIter_ITER; }
#line 9059 "bison-chapel.cpp"
    break;

  case 374:
#line 1564 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 9065 "bison-chapel.cpp"
    break;

  case 375:
#line 1565 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 9071 "bison-chapel.cpp"
    break;

  case 376:
#line 1566 "chapel.ypp"
              { (yyval.retTag) = RET_CONST_REF; }
#line 9077 "bison-chapel.cpp"
    break;

  case 377:
#line 1567 "chapel.ypp"
              { (yyval.retTag) = RET_REF; }
#line 9083 "bison-chapel.cpp"
    break;

  case 378:
#line 1568 "chapel.ypp"
              { (yyval.retTag) = RET_PARAM; }
#line 9089 "bison-chapel.cpp"
    break;

  case 379:
#line 1569 "chapel.ypp"
              { (yyval.retTag) = RET_TYPE; }
#line 9095 "bison-chapel.cpp"
    break;

  case 380:
#line 1573 "chapel.ypp"
          { (yyval.b) = false; }
#line 9101 "bison-chapel.cpp"
    break;

  case 381:
#line 1574 "chapel.ypp"
          { (yyval.b) = true;  }
#line 9107 "bison-chapel.cpp"
    break;

  case 382:
#line 1577 "chapel.ypp"
            { (yyval.pblockstmt) = NULL; }
#line 9113 "bison-chapel.cpp"
    break;

  case 385:
#line 1583 "chapel.ypp"
               { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 9119 "bison-chapel.cpp"
    break;

  case 386:
#line 1587 "chapel.ypp"
                      { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 9125 "bison-chapel.cpp"
    break;

  case 387:
#line 1591 "chapel.ypp"
                         { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9131 "bison-chapel.cpp"
    break;

  case 388:
#line 1592 "chapel.ypp"
                         { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9137 "bison-chapel.cpp"
    break;

  case 389:
#line 1593 "chapel.ypp"
                         { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 9147 "bison-chapel.cpp"
    break;

  case 390:
#line 1601 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 9153 "bison-chapel.cpp"
    break;

  case 391:
#line 1603 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 9159 "bison-chapel.cpp"
    break;

  case 392:
#line 1605 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 9165 "bison-chapel.cpp"
    break;

  case 393:
#line 1607 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9171 "bison-chapel.cpp"
    break;

  case 394:
#line 1609 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9177 "bison-chapel.cpp"
    break;

  case 395:
#line 1614 "chapel.ypp"
  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9183 "bison-chapel.cpp"
    break;

  case 396:
#line 1616 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9189 "bison-chapel.cpp"
    break;

  case 397:
#line 1620 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9195 "bison-chapel.cpp"
    break;

  case 398:
#line 1621 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9201 "bison-chapel.cpp"
    break;

  case 399:
#line 1622 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9207 "bison-chapel.cpp"
    break;

  case 400:
#line 1623 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9213 "bison-chapel.cpp"
    break;

  case 401:
#line 1624 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9219 "bison-chapel.cpp"
    break;

  case 402:
#line 1625 "chapel.ypp"
                                              {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9225 "bison-chapel.cpp"
    break;

  case 403:
#line 1626 "chapel.ypp"
                         { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 9231 "bison-chapel.cpp"
    break;

  case 404:
#line 1631 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 9237 "bison-chapel.cpp"
    break;

  case 405:
#line 1633 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 9243 "bison-chapel.cpp"
    break;

  case 406:
#line 1638 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 9249 "bison-chapel.cpp"
    break;

  case 407:
#line 1640 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 9255 "bison-chapel.cpp"
    break;

  case 408:
#line 1642 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 9261 "bison-chapel.cpp"
    break;

  case 409:
#line 1647 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 9278 "bison-chapel.cpp"
    break;

  case 410:
#line 1660 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 9296 "bison-chapel.cpp"
    break;

  case 411:
#line 1676 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
#line 9302 "bison-chapel.cpp"
    break;

  case 412:
#line 1678 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9308 "bison-chapel.cpp"
    break;

  case 413:
#line 1680 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 9314 "bison-chapel.cpp"
    break;

  case 414:
#line 1684 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 9320 "bison-chapel.cpp"
    break;

  case 415:
#line 1685 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 9326 "bison-chapel.cpp"
    break;

  case 416:
#line 1686 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 9332 "bison-chapel.cpp"
    break;

  case 417:
#line 1687 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 9338 "bison-chapel.cpp"
    break;

  case 418:
#line 1688 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(); }
#line 9344 "bison-chapel.cpp"
    break;

  case 419:
#line 1693 "chapel.ypp"
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9354 "bison-chapel.cpp"
    break;

  case 420:
#line 1699 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9363 "bison-chapel.cpp"
    break;

  case 422:
#line 1708 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 9372 "bison-chapel.cpp"
    break;

  case 423:
#line 1716 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 9378 "bison-chapel.cpp"
    break;

  case 424:
#line 1718 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9384 "bison-chapel.cpp"
    break;

  case 425:
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 9390 "bison-chapel.cpp"
    break;

  case 426:
#line 1725 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 9396 "bison-chapel.cpp"
    break;

  case 427:
#line 1727 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 9402 "bison-chapel.cpp"
    break;

  case 428:
#line 1732 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 9408 "bison-chapel.cpp"
    break;

  case 429:
#line 1734 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 9414 "bison-chapel.cpp"
    break;

  case 430:
#line 1736 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 9420 "bison-chapel.cpp"
    break;

  case 431:
#line 1742 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9426 "bison-chapel.cpp"
    break;

  case 432:
#line 1743 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 9432 "bison-chapel.cpp"
    break;

  case 433:
#line 1744 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9438 "bison-chapel.cpp"
    break;

  case 434:
#line 1750 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9444 "bison-chapel.cpp"
    break;

  case 435:
#line 1752 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 9450 "bison-chapel.cpp"
    break;

  case 436:
#line 1754 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9458 "bison-chapel.cpp"
    break;

  case 437:
#line 1758 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 9466 "bison-chapel.cpp"
    break;

  case 438:
#line 1762 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9472 "bison-chapel.cpp"
    break;

  case 439:
#line 1764 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9480 "bison-chapel.cpp"
    break;

  case 440:
#line 1768 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9488 "bison-chapel.cpp"
    break;

  case 441:
#line 1775 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9494 "bison-chapel.cpp"
    break;

  case 442:
#line 1776 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9500 "bison-chapel.cpp"
    break;

  case 443:
#line 1777 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9506 "bison-chapel.cpp"
    break;

  case 444:
#line 1778 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9512 "bison-chapel.cpp"
    break;

  case 445:
#line 1779 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9518 "bison-chapel.cpp"
    break;

  case 446:
#line 1784 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9524 "bison-chapel.cpp"
    break;

  case 447:
#line 1785 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9530 "bison-chapel.cpp"
    break;

  case 448:
#line 1786 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 9536 "bison-chapel.cpp"
    break;

  case 449:
#line 1787 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9542 "bison-chapel.cpp"
    break;

  case 450:
#line 1788 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9548 "bison-chapel.cpp"
    break;

  case 451:
#line 1809 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9556 "bison-chapel.cpp"
    break;

  case 452:
#line 1813 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 9564 "bison-chapel.cpp"
    break;

  case 453:
#line 1817 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 9576 "bison-chapel.cpp"
    break;

  case 454:
#line 1825 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9584 "bison-chapel.cpp"
    break;

  case 455:
#line 1831 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9590 "bison-chapel.cpp"
    break;

  case 456:
#line 1832 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9596 "bison-chapel.cpp"
    break;

  case 457:
#line 1833 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9602 "bison-chapel.cpp"
    break;

  case 458:
#line 1838 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9608 "bison-chapel.cpp"
    break;

  case 459:
#line 1840 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9614 "bison-chapel.cpp"
    break;

  case 460:
#line 1846 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9620 "bison-chapel.cpp"
    break;

  case 461:
#line 1848 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9626 "bison-chapel.cpp"
    break;

  case 462:
#line 1850 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 9632 "bison-chapel.cpp"
    break;

  case 463:
#line 1854 "chapel.ypp"
                            { (yyval.pexpr) = NULL; }
#line 9638 "bison-chapel.cpp"
    break;

  case 464:
#line 1855 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9644 "bison-chapel.cpp"
    break;

  case 465:
#line 1856 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9650 "bison-chapel.cpp"
    break;

  case 466:
#line 1857 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9656 "bison-chapel.cpp"
    break;

  case 467:
#line 1858 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9662 "bison-chapel.cpp"
    break;

  case 468:
#line 1864 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9668 "bison-chapel.cpp"
    break;

  case 469:
#line 1865 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9674 "bison-chapel.cpp"
    break;

  case 470:
#line 1866 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9680 "bison-chapel.cpp"
    break;

  case 471:
#line 1867 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9686 "bison-chapel.cpp"
    break;

  case 472:
#line 1871 "chapel.ypp"
                                   { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 9692 "bison-chapel.cpp"
    break;

  case 473:
#line 1872 "chapel.ypp"
                                   { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9698 "bison-chapel.cpp"
    break;

  case 474:
#line 1876 "chapel.ypp"
                { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 9704 "bison-chapel.cpp"
    break;

  case 475:
#line 1877 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9710 "bison-chapel.cpp"
    break;

  case 476:
#line 1878 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9716 "bison-chapel.cpp"
    break;

  case 477:
#line 1882 "chapel.ypp"
                                         { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9722 "bison-chapel.cpp"
    break;

  case 478:
#line 1883 "chapel.ypp"
                                       { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9728 "bison-chapel.cpp"
    break;

  case 479:
#line 1887 "chapel.ypp"
             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 9734 "bison-chapel.cpp"
    break;

  case 481:
#line 1892 "chapel.ypp"
                                { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9740 "bison-chapel.cpp"
    break;

  case 482:
#line 1893 "chapel.ypp"
                                { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9746 "bison-chapel.cpp"
    break;

  case 483:
#line 1897 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9752 "bison-chapel.cpp"
    break;

  case 484:
#line 1898 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9758 "bison-chapel.cpp"
    break;

  case 485:
#line 1899 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9764 "bison-chapel.cpp"
    break;

  case 486:
#line 1900 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9770 "bison-chapel.cpp"
    break;

  case 487:
#line 1904 "chapel.ypp"
                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9776 "bison-chapel.cpp"
    break;

  case 488:
#line 1905 "chapel.ypp"
                 { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9782 "bison-chapel.cpp"
    break;

  case 489:
#line 1917 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9788 "bison-chapel.cpp"
    break;

  case 490:
#line 1919 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 9794 "bison-chapel.cpp"
    break;

  case 491:
#line 1921 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9800 "bison-chapel.cpp"
    break;

  case 497:
#line 1932 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 9806 "bison-chapel.cpp"
    break;

  case 498:
#line 1934 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 9812 "bison-chapel.cpp"
    break;

  case 499:
#line 1936 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 9818 "bison-chapel.cpp"
    break;

  case 500:
#line 1938 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 9824 "bison-chapel.cpp"
    break;

  case 501:
#line 1940 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 9830 "bison-chapel.cpp"
    break;

  case 502:
#line 1942 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 9836 "bison-chapel.cpp"
    break;

  case 503:
#line 1944 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 9842 "bison-chapel.cpp"
    break;

  case 504:
#line 1947 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 9848 "bison-chapel.cpp"
    break;

  case 505:
#line 1949 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 9854 "bison-chapel.cpp"
    break;

  case 506:
#line 1951 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 9860 "bison-chapel.cpp"
    break;

  case 507:
#line 1953 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9866 "bison-chapel.cpp"
    break;

  case 508:
#line 1955 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 9872 "bison-chapel.cpp"
    break;

  case 509:
#line 1957 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 9878 "bison-chapel.cpp"
    break;

  case 510:
#line 1959 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 9884 "bison-chapel.cpp"
    break;

  case 511:
#line 1961 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9890 "bison-chapel.cpp"
    break;

  case 512:
#line 1964 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 9896 "bison-chapel.cpp"
    break;

  case 513:
#line 1966 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 9902 "bison-chapel.cpp"
    break;

  case 514:
#line 1971 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9908 "bison-chapel.cpp"
    break;

  case 515:
#line 1973 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 9914 "bison-chapel.cpp"
    break;

  case 516:
#line 1975 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9920 "bison-chapel.cpp"
    break;

  case 517:
#line 1977 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9926 "bison-chapel.cpp"
    break;

  case 518:
#line 1979 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 9932 "bison-chapel.cpp"
    break;

  case 519:
#line 1981 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9938 "bison-chapel.cpp"
    break;

  case 520:
#line 1983 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9944 "bison-chapel.cpp"
    break;

  case 521:
#line 1985 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 9950 "bison-chapel.cpp"
    break;

  case 522:
#line 1987 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9956 "bison-chapel.cpp"
    break;

  case 523:
#line 1989 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9962 "bison-chapel.cpp"
    break;

  case 524:
#line 1991 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 9968 "bison-chapel.cpp"
    break;

  case 525:
#line 1993 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9974 "bison-chapel.cpp"
    break;

  case 526:
#line 1995 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 9985 "bison-chapel.cpp"
    break;

  case 527:
#line 2002 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 9995 "bison-chapel.cpp"
    break;

  case 528:
#line 2008 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 10005 "bison-chapel.cpp"
    break;

  case 529:
#line 2014 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10015 "bison-chapel.cpp"
    break;

  case 530:
#line 2020 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 10025 "bison-chapel.cpp"
    break;

  case 531:
#line 2029 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10031 "bison-chapel.cpp"
    break;

  case 532:
#line 2038 "chapel.ypp"
            { (yyval.pexpr) = new SymExpr(gNil); }
#line 10037 "bison-chapel.cpp"
    break;

  case 540:
#line 2054 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10043 "bison-chapel.cpp"
    break;

  case 541:
#line 2058 "chapel.ypp"
                                { (yyval.pcallexpr) = NULL; }
#line 10049 "bison-chapel.cpp"
    break;

  case 543:
#line 2063 "chapel.ypp"
                                { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10055 "bison-chapel.cpp"
    break;

  case 544:
#line 2067 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10061 "bison-chapel.cpp"
    break;

  case 545:
#line 2068 "chapel.ypp"
                                                    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10067 "bison-chapel.cpp"
    break;

  case 546:
#line 2072 "chapel.ypp"
                                  { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10073 "bison-chapel.cpp"
    break;

  case 547:
#line 2076 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10079 "bison-chapel.cpp"
    break;

  case 548:
#line 2077 "chapel.ypp"
                                                    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10085 "bison-chapel.cpp"
    break;

  case 549:
#line 2082 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 10093 "bison-chapel.cpp"
    break;

  case 550:
#line 2086 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10101 "bison-chapel.cpp"
    break;

  case 551:
#line 2090 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10109 "bison-chapel.cpp"
    break;

  case 552:
#line 2096 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 10115 "bison-chapel.cpp"
    break;

  case 553:
#line 2097 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_IN;        }
#line 10121 "bison-chapel.cpp"
    break;

  case 554:
#line 2098 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 10127 "bison-chapel.cpp"
    break;

  case 555:
#line 2099 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_REF;       }
#line 10133 "bison-chapel.cpp"
    break;

  case 556:
#line 2100 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 10139 "bison-chapel.cpp"
    break;

  case 557:
#line 2101 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 10145 "bison-chapel.cpp"
    break;

  case 559:
#line 2107 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10151 "bison-chapel.cpp"
    break;

  case 560:
#line 2112 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 10157 "bison-chapel.cpp"
    break;

  case 561:
#line 2114 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 10165 "bison-chapel.cpp"
    break;

  case 562:
#line 2118 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 10173 "bison-chapel.cpp"
    break;

  case 563:
#line 2122 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 10181 "bison-chapel.cpp"
    break;

  case 564:
#line 2126 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 10189 "bison-chapel.cpp"
    break;

  case 565:
#line 2134 "chapel.ypp"
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 10196 "bison-chapel.cpp"
    break;

  case 566:
#line 2139 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10206 "bison-chapel.cpp"
    break;

  case 567:
#line 2145 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10216 "bison-chapel.cpp"
    break;

  case 568:
#line 2151 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10227 "bison-chapel.cpp"
    break;

  case 569:
#line 2158 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10238 "bison-chapel.cpp"
    break;

  case 570:
#line 2168 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 10244 "bison-chapel.cpp"
    break;

  case 580:
#line 2185 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 10250 "bison-chapel.cpp"
    break;

  case 581:
#line 2187 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10256 "bison-chapel.cpp"
    break;

  case 582:
#line 2189 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10262 "bison-chapel.cpp"
    break;

  case 583:
#line 2191 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr), false, true); }
#line 10268 "bison-chapel.cpp"
    break;

  case 584:
#line 2206 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[-1].pexpr)); }
#line 10274 "bison-chapel.cpp"
    break;

  case 585:
#line 2208 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr)); }
#line 10280 "bison-chapel.cpp"
    break;

  case 586:
#line 2210 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr), true); }
#line 10286 "bison-chapel.cpp"
    break;

  case 587:
#line 2212 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
#line 10292 "bison-chapel.cpp"
    break;

  case 588:
#line 2216 "chapel.ypp"
                  { (yyval.pexpr) = NULL; }
#line 10298 "bison-chapel.cpp"
    break;

  case 589:
#line 2217 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10304 "bison-chapel.cpp"
    break;

  case 590:
#line 2221 "chapel.ypp"
                  { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 10310 "bison-chapel.cpp"
    break;

  case 591:
#line 2222 "chapel.ypp"
                  { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 10316 "bison-chapel.cpp"
    break;

  case 592:
#line 2223 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10322 "bison-chapel.cpp"
    break;

  case 597:
#line 2239 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10328 "bison-chapel.cpp"
    break;

  case 598:
#line 2240 "chapel.ypp"
                     { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10334 "bison-chapel.cpp"
    break;

  case 599:
#line 2241 "chapel.ypp"
                                {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 10340 "bison-chapel.cpp"
    break;

  case 600:
#line 2242 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10346 "bison-chapel.cpp"
    break;

  case 602:
#line 2247 "chapel.ypp"
                                              { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10352 "bison-chapel.cpp"
    break;

  case 603:
#line 2248 "chapel.ypp"
                                              { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10358 "bison-chapel.cpp"
    break;

  case 604:
#line 2249 "chapel.ypp"
                                        { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 10364 "bison-chapel.cpp"
    break;

  case 605:
#line 2253 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 10370 "bison-chapel.cpp"
    break;

  case 606:
#line 2254 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 10376 "bison-chapel.cpp"
    break;

  case 607:
#line 2255 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 10382 "bison-chapel.cpp"
    break;

  case 608:
#line 2256 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 10388 "bison-chapel.cpp"
    break;

  case 609:
#line 2257 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 10394 "bison-chapel.cpp"
    break;

  case 610:
#line 2265 "chapel.ypp"
                                    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 10400 "bison-chapel.cpp"
    break;

  case 611:
#line 2266 "chapel.ypp"
                                    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 10406 "bison-chapel.cpp"
    break;

  case 612:
#line 2267 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 10412 "bison-chapel.cpp"
    break;

  case 613:
#line 2268 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 10418 "bison-chapel.cpp"
    break;

  case 614:
#line 2272 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gFalse); }
#line 10424 "bison-chapel.cpp"
    break;

  case 615:
#line 2273 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gTrue); }
#line 10430 "bison-chapel.cpp"
    break;

  case 616:
#line 2277 "chapel.ypp"
                  { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 10436 "bison-chapel.cpp"
    break;

  case 617:
#line 2278 "chapel.ypp"
                  { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 10442 "bison-chapel.cpp"
    break;

  case 620:
#line 2284 "chapel.ypp"
                        { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 10448 "bison-chapel.cpp"
    break;

  case 621:
#line 2285 "chapel.ypp"
                        { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 10454 "bison-chapel.cpp"
    break;

  case 622:
#line 2286 "chapel.ypp"
                        { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 10460 "bison-chapel.cpp"
    break;

  case 623:
#line 2287 "chapel.ypp"
                        { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 10466 "bison-chapel.cpp"
    break;

  case 624:
#line 2288 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNone); }
#line 10472 "bison-chapel.cpp"
    break;

  case 625:
#line 2289 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr),
                                            new SymExpr(gTrue)); }
#line 10479 "bison-chapel.cpp"
    break;

  case 626:
#line 2291 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr),
                                                   new SymExpr(gTrue)); }
#line 10486 "bison-chapel.cpp"
    break;

  case 627:
#line 2293 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 10492 "bison-chapel.cpp"
    break;

  case 628:
#line 2294 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 10498 "bison-chapel.cpp"
    break;

  case 629:
#line 2296 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 10506 "bison-chapel.cpp"
    break;

  case 630:
#line 2300 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 10514 "bison-chapel.cpp"
    break;

  case 631:
#line 2307 "chapel.ypp"
                                        { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10520 "bison-chapel.cpp"
    break;

  case 632:
#line 2308 "chapel.ypp"
                                        { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 10526 "bison-chapel.cpp"
    break;

  case 633:
#line 2312 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10532 "bison-chapel.cpp"
    break;

  case 634:
#line 2313 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10538 "bison-chapel.cpp"
    break;

  case 635:
#line 2314 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10544 "bison-chapel.cpp"
    break;

  case 636:
#line 2315 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10550 "bison-chapel.cpp"
    break;

  case 637:
#line 2316 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10556 "bison-chapel.cpp"
    break;

  case 638:
#line 2317 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10562 "bison-chapel.cpp"
    break;

  case 639:
#line 2318 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10568 "bison-chapel.cpp"
    break;

  case 640:
#line 2319 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10574 "bison-chapel.cpp"
    break;

  case 641:
#line 2320 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10580 "bison-chapel.cpp"
    break;

  case 642:
#line 2321 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10586 "bison-chapel.cpp"
    break;

  case 643:
#line 2322 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10592 "bison-chapel.cpp"
    break;

  case 644:
#line 2323 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10598 "bison-chapel.cpp"
    break;

  case 645:
#line 2324 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10604 "bison-chapel.cpp"
    break;

  case 646:
#line 2325 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10610 "bison-chapel.cpp"
    break;

  case 647:
#line 2326 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10616 "bison-chapel.cpp"
    break;

  case 648:
#line 2327 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10622 "bison-chapel.cpp"
    break;

  case 649:
#line 2328 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10628 "bison-chapel.cpp"
    break;

  case 650:
#line 2329 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10634 "bison-chapel.cpp"
    break;

  case 651:
#line 2330 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10640 "bison-chapel.cpp"
    break;

  case 652:
#line 2331 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10646 "bison-chapel.cpp"
    break;

  case 653:
#line 2332 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10652 "bison-chapel.cpp"
    break;

  case 654:
#line 2333 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10658 "bison-chapel.cpp"
    break;

  case 655:
#line 2334 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr),
                                               new SymExpr(gTrue)); }
#line 10665 "bison-chapel.cpp"
    break;

  case 656:
#line 2339 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 10671 "bison-chapel.cpp"
    break;

  case 657:
#line 2340 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 10677 "bison-chapel.cpp"
    break;

  case 658:
#line 2341 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 10683 "bison-chapel.cpp"
    break;

  case 659:
#line 2342 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 10689 "bison-chapel.cpp"
    break;

  case 660:
#line 2343 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 10695 "bison-chapel.cpp"
    break;

  case 661:
#line 2344 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10701 "bison-chapel.cpp"
    break;

  case 662:
#line 2345 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 10707 "bison-chapel.cpp"
    break;

  case 663:
#line 2349 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10713 "bison-chapel.cpp"
    break;

  case 664:
#line 2350 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10719 "bison-chapel.cpp"
    break;

  case 665:
#line 2351 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10725 "bison-chapel.cpp"
    break;

  case 666:
#line 2352 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10731 "bison-chapel.cpp"
    break;

  case 667:
#line 2356 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10737 "bison-chapel.cpp"
    break;

  case 668:
#line 2357 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10743 "bison-chapel.cpp"
    break;

  case 669:
#line 2358 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10749 "bison-chapel.cpp"
    break;

  case 670:
#line 2359 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10755 "bison-chapel.cpp"
    break;

  case 671:
#line 2364 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 10761 "bison-chapel.cpp"
    break;

  case 672:
#line 2365 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 10767 "bison-chapel.cpp"
    break;

  case 673:
#line 2366 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 10773 "bison-chapel.cpp"
    break;

  case 674:
#line 2367 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 10779 "bison-chapel.cpp"
    break;

  case 675:
#line 2368 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 10785 "bison-chapel.cpp"
    break;

  case 676:
#line 2369 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 10791 "bison-chapel.cpp"
    break;

  case 677:
#line 2370 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 10797 "bison-chapel.cpp"
    break;


#line 10801 "bison-chapel.cpp"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (&yylloc, context, YY_("syntax error"));
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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, context);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

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


#if !defined yyoverflow
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, context);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  yypstate_clear (yyps);
  yyps->yynew = 1;


/*-----------------------------------------.
| yypushreturn -- ask for the next token.  |
`-----------------------------------------*/
yypushreturn:

  return yyresult;
}
#undef yynerrs
#undef yystate
#undef yyerrstatus
#undef yyssa
#undef yyss
#undef yyssp
#undef yyvsa
#undef yyvs
#undef yyvsp
#undef yylsa
#undef yyls
#undef yylsp
#undef yystacksize
