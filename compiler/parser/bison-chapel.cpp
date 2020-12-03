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
    TLABEL = 309,                  /* TLABEL  */
    TLAMBDA = 310,                 /* TLAMBDA  */
    TLET = 311,                    /* TLET  */
    TLIFETIME = 312,               /* TLIFETIME  */
    TLOCAL = 313,                  /* TLOCAL  */
    TLOCALE = 314,                 /* TLOCALE  */
    TMINUSMINUS = 315,             /* TMINUSMINUS  */
    TMODULE = 316,                 /* TMODULE  */
    TNEW = 317,                    /* TNEW  */
    TNIL = 318,                    /* TNIL  */
    TNOINIT = 319,                 /* TNOINIT  */
    TNONE = 320,                   /* TNONE  */
    TNOTHING = 321,                /* TNOTHING  */
    TON = 322,                     /* TON  */
    TONLY = 323,                   /* TONLY  */
    TOTHERWISE = 324,              /* TOTHERWISE  */
    TOUT = 325,                    /* TOUT  */
    TOVERRIDE = 326,               /* TOVERRIDE  */
    TOWNED = 327,                  /* TOWNED  */
    TPARAM = 328,                  /* TPARAM  */
    TPLUSPLUS = 329,               /* TPLUSPLUS  */
    TPRAGMA = 330,                 /* TPRAGMA  */
    TPRIMITIVE = 331,              /* TPRIMITIVE  */
    TPRIVATE = 332,                /* TPRIVATE  */
    TPROC = 333,                   /* TPROC  */
    TPROTOTYPE = 334,              /* TPROTOTYPE  */
    TPUBLIC = 335,                 /* TPUBLIC  */
    TREAL = 336,                   /* TREAL  */
    TRECORD = 337,                 /* TRECORD  */
    TREDUCE = 338,                 /* TREDUCE  */
    TREF = 339,                    /* TREF  */
    TREQUIRE = 340,                /* TREQUIRE  */
    TRETURN = 341,                 /* TRETURN  */
    TSCAN = 342,                   /* TSCAN  */
    TSELECT = 343,                 /* TSELECT  */
    TSERIAL = 344,                 /* TSERIAL  */
    TSHARED = 345,                 /* TSHARED  */
    TSINGLE = 346,                 /* TSINGLE  */
    TSPARSE = 347,                 /* TSPARSE  */
    TSTRING = 348,                 /* TSTRING  */
    TSUBDOMAIN = 349,              /* TSUBDOMAIN  */
    TSYNC = 350,                   /* TSYNC  */
    TTHEN = 351,                   /* TTHEN  */
    TTHIS = 352,                   /* TTHIS  */
    TTHROW = 353,                  /* TTHROW  */
    TTHROWS = 354,                 /* TTHROWS  */
    TTRUE = 355,                   /* TTRUE  */
    TTRY = 356,                    /* TTRY  */
    TTRYBANG = 357,                /* TTRYBANG  */
    TTYPE = 358,                   /* TTYPE  */
    TUINT = 359,                   /* TUINT  */
    TUNDERSCORE = 360,             /* TUNDERSCORE  */
    TUNION = 361,                  /* TUNION  */
    TUNMANAGED = 362,              /* TUNMANAGED  */
    TUSE = 363,                    /* TUSE  */
    TVAR = 364,                    /* TVAR  */
    TVOID = 365,                   /* TVOID  */
    TWHEN = 366,                   /* TWHEN  */
    TWHERE = 367,                  /* TWHERE  */
    TWHILE = 368,                  /* TWHILE  */
    TWITH = 369,                   /* TWITH  */
    TYIELD = 370,                  /* TYIELD  */
    TZIP = 371,                    /* TZIP  */
    TALIAS = 372,                  /* TALIAS  */
    TAND = 373,                    /* TAND  */
    TASSIGN = 374,                 /* TASSIGN  */
    TASSIGNBAND = 375,             /* TASSIGNBAND  */
    TASSIGNBOR = 376,              /* TASSIGNBOR  */
    TASSIGNBXOR = 377,             /* TASSIGNBXOR  */
    TASSIGNDIVIDE = 378,           /* TASSIGNDIVIDE  */
    TASSIGNEXP = 379,              /* TASSIGNEXP  */
    TASSIGNLAND = 380,             /* TASSIGNLAND  */
    TASSIGNLOR = 381,              /* TASSIGNLOR  */
    TASSIGNMINUS = 382,            /* TASSIGNMINUS  */
    TASSIGNMOD = 383,              /* TASSIGNMOD  */
    TASSIGNMULTIPLY = 384,         /* TASSIGNMULTIPLY  */
    TASSIGNPLUS = 385,             /* TASSIGNPLUS  */
    TASSIGNREDUCE = 386,           /* TASSIGNREDUCE  */
    TASSIGNSL = 387,               /* TASSIGNSL  */
    TASSIGNSR = 388,               /* TASSIGNSR  */
    TBANG = 389,                   /* TBANG  */
    TBAND = 390,                   /* TBAND  */
    TBNOT = 391,                   /* TBNOT  */
    TBOR = 392,                    /* TBOR  */
    TBXOR = 393,                   /* TBXOR  */
    TCOLON = 394,                  /* TCOLON  */
    TCOMMA = 395,                  /* TCOMMA  */
    TDIVIDE = 396,                 /* TDIVIDE  */
    TDOT = 397,                    /* TDOT  */
    TDOTDOT = 398,                 /* TDOTDOT  */
    TDOTDOTDOT = 399,              /* TDOTDOTDOT  */
    TEQUAL = 400,                  /* TEQUAL  */
    TEXP = 401,                    /* TEXP  */
    TGREATER = 402,                /* TGREATER  */
    TGREATEREQUAL = 403,           /* TGREATEREQUAL  */
    THASH = 404,                   /* THASH  */
    TIO = 405,                     /* TIO  */
    TLESS = 406,                   /* TLESS  */
    TLESSEQUAL = 407,              /* TLESSEQUAL  */
    TMINUS = 408,                  /* TMINUS  */
    TMOD = 409,                    /* TMOD  */
    TNOTEQUAL = 410,               /* TNOTEQUAL  */
    TOR = 411,                     /* TOR  */
    TPLUS = 412,                   /* TPLUS  */
    TQUESTION = 413,               /* TQUESTION  */
    TSEMI = 414,                   /* TSEMI  */
    TSHIFTLEFT = 415,              /* TSHIFTLEFT  */
    TSHIFTRIGHT = 416,             /* TSHIFTRIGHT  */
    TSTAR = 417,                   /* TSTAR  */
    TSWAP = 418,                   /* TSWAP  */
    TLCBR = 419,                   /* TLCBR  */
    TRCBR = 420,                   /* TRCBR  */
    TLP = 421,                     /* TLP  */
    TRP = 422,                     /* TRP  */
    TLSBR = 423,                   /* TLSBR  */
    TRSBR = 424,                   /* TRSBR  */
    TNOELSE = 425,                 /* TNOELSE  */
    TDOTDOTOPENHIGH = 426,         /* TDOTDOTOPENHIGH  */
    TUPLUS = 427,                  /* TUPLUS  */
    TUMINUS = 428,                 /* TUMINUS  */
    TLNOT = 429                    /* TLNOT  */
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

#line 477 "bison-chapel.cpp"

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
  YYSYMBOL_TLABEL = 54,                    /* TLABEL  */
  YYSYMBOL_TLAMBDA = 55,                   /* TLAMBDA  */
  YYSYMBOL_TLET = 56,                      /* TLET  */
  YYSYMBOL_TLIFETIME = 57,                 /* TLIFETIME  */
  YYSYMBOL_TLOCAL = 58,                    /* TLOCAL  */
  YYSYMBOL_TLOCALE = 59,                   /* TLOCALE  */
  YYSYMBOL_TMINUSMINUS = 60,               /* TMINUSMINUS  */
  YYSYMBOL_TMODULE = 61,                   /* TMODULE  */
  YYSYMBOL_TNEW = 62,                      /* TNEW  */
  YYSYMBOL_TNIL = 63,                      /* TNIL  */
  YYSYMBOL_TNOINIT = 64,                   /* TNOINIT  */
  YYSYMBOL_TNONE = 65,                     /* TNONE  */
  YYSYMBOL_TNOTHING = 66,                  /* TNOTHING  */
  YYSYMBOL_TON = 67,                       /* TON  */
  YYSYMBOL_TONLY = 68,                     /* TONLY  */
  YYSYMBOL_TOTHERWISE = 69,                /* TOTHERWISE  */
  YYSYMBOL_TOUT = 70,                      /* TOUT  */
  YYSYMBOL_TOVERRIDE = 71,                 /* TOVERRIDE  */
  YYSYMBOL_TOWNED = 72,                    /* TOWNED  */
  YYSYMBOL_TPARAM = 73,                    /* TPARAM  */
  YYSYMBOL_TPLUSPLUS = 74,                 /* TPLUSPLUS  */
  YYSYMBOL_TPRAGMA = 75,                   /* TPRAGMA  */
  YYSYMBOL_TPRIMITIVE = 76,                /* TPRIMITIVE  */
  YYSYMBOL_TPRIVATE = 77,                  /* TPRIVATE  */
  YYSYMBOL_TPROC = 78,                     /* TPROC  */
  YYSYMBOL_TPROTOTYPE = 79,                /* TPROTOTYPE  */
  YYSYMBOL_TPUBLIC = 80,                   /* TPUBLIC  */
  YYSYMBOL_TREAL = 81,                     /* TREAL  */
  YYSYMBOL_TRECORD = 82,                   /* TRECORD  */
  YYSYMBOL_TREDUCE = 83,                   /* TREDUCE  */
  YYSYMBOL_TREF = 84,                      /* TREF  */
  YYSYMBOL_TREQUIRE = 85,                  /* TREQUIRE  */
  YYSYMBOL_TRETURN = 86,                   /* TRETURN  */
  YYSYMBOL_TSCAN = 87,                     /* TSCAN  */
  YYSYMBOL_TSELECT = 88,                   /* TSELECT  */
  YYSYMBOL_TSERIAL = 89,                   /* TSERIAL  */
  YYSYMBOL_TSHARED = 90,                   /* TSHARED  */
  YYSYMBOL_TSINGLE = 91,                   /* TSINGLE  */
  YYSYMBOL_TSPARSE = 92,                   /* TSPARSE  */
  YYSYMBOL_TSTRING = 93,                   /* TSTRING  */
  YYSYMBOL_TSUBDOMAIN = 94,                /* TSUBDOMAIN  */
  YYSYMBOL_TSYNC = 95,                     /* TSYNC  */
  YYSYMBOL_TTHEN = 96,                     /* TTHEN  */
  YYSYMBOL_TTHIS = 97,                     /* TTHIS  */
  YYSYMBOL_TTHROW = 98,                    /* TTHROW  */
  YYSYMBOL_TTHROWS = 99,                   /* TTHROWS  */
  YYSYMBOL_TTRUE = 100,                    /* TTRUE  */
  YYSYMBOL_TTRY = 101,                     /* TTRY  */
  YYSYMBOL_TTRYBANG = 102,                 /* TTRYBANG  */
  YYSYMBOL_TTYPE = 103,                    /* TTYPE  */
  YYSYMBOL_TUINT = 104,                    /* TUINT  */
  YYSYMBOL_TUNDERSCORE = 105,              /* TUNDERSCORE  */
  YYSYMBOL_TUNION = 106,                   /* TUNION  */
  YYSYMBOL_TUNMANAGED = 107,               /* TUNMANAGED  */
  YYSYMBOL_TUSE = 108,                     /* TUSE  */
  YYSYMBOL_TVAR = 109,                     /* TVAR  */
  YYSYMBOL_TVOID = 110,                    /* TVOID  */
  YYSYMBOL_TWHEN = 111,                    /* TWHEN  */
  YYSYMBOL_TWHERE = 112,                   /* TWHERE  */
  YYSYMBOL_TWHILE = 113,                   /* TWHILE  */
  YYSYMBOL_TWITH = 114,                    /* TWITH  */
  YYSYMBOL_TYIELD = 115,                   /* TYIELD  */
  YYSYMBOL_TZIP = 116,                     /* TZIP  */
  YYSYMBOL_TALIAS = 117,                   /* TALIAS  */
  YYSYMBOL_TAND = 118,                     /* TAND  */
  YYSYMBOL_TASSIGN = 119,                  /* TASSIGN  */
  YYSYMBOL_TASSIGNBAND = 120,              /* TASSIGNBAND  */
  YYSYMBOL_TASSIGNBOR = 121,               /* TASSIGNBOR  */
  YYSYMBOL_TASSIGNBXOR = 122,              /* TASSIGNBXOR  */
  YYSYMBOL_TASSIGNDIVIDE = 123,            /* TASSIGNDIVIDE  */
  YYSYMBOL_TASSIGNEXP = 124,               /* TASSIGNEXP  */
  YYSYMBOL_TASSIGNLAND = 125,              /* TASSIGNLAND  */
  YYSYMBOL_TASSIGNLOR = 126,               /* TASSIGNLOR  */
  YYSYMBOL_TASSIGNMINUS = 127,             /* TASSIGNMINUS  */
  YYSYMBOL_TASSIGNMOD = 128,               /* TASSIGNMOD  */
  YYSYMBOL_TASSIGNMULTIPLY = 129,          /* TASSIGNMULTIPLY  */
  YYSYMBOL_TASSIGNPLUS = 130,              /* TASSIGNPLUS  */
  YYSYMBOL_TASSIGNREDUCE = 131,            /* TASSIGNREDUCE  */
  YYSYMBOL_TASSIGNSL = 132,                /* TASSIGNSL  */
  YYSYMBOL_TASSIGNSR = 133,                /* TASSIGNSR  */
  YYSYMBOL_TBANG = 134,                    /* TBANG  */
  YYSYMBOL_TBAND = 135,                    /* TBAND  */
  YYSYMBOL_TBNOT = 136,                    /* TBNOT  */
  YYSYMBOL_TBOR = 137,                     /* TBOR  */
  YYSYMBOL_TBXOR = 138,                    /* TBXOR  */
  YYSYMBOL_TCOLON = 139,                   /* TCOLON  */
  YYSYMBOL_TCOMMA = 140,                   /* TCOMMA  */
  YYSYMBOL_TDIVIDE = 141,                  /* TDIVIDE  */
  YYSYMBOL_TDOT = 142,                     /* TDOT  */
  YYSYMBOL_TDOTDOT = 143,                  /* TDOTDOT  */
  YYSYMBOL_TDOTDOTDOT = 144,               /* TDOTDOTDOT  */
  YYSYMBOL_TEQUAL = 145,                   /* TEQUAL  */
  YYSYMBOL_TEXP = 146,                     /* TEXP  */
  YYSYMBOL_TGREATER = 147,                 /* TGREATER  */
  YYSYMBOL_TGREATEREQUAL = 148,            /* TGREATEREQUAL  */
  YYSYMBOL_THASH = 149,                    /* THASH  */
  YYSYMBOL_TIO = 150,                      /* TIO  */
  YYSYMBOL_TLESS = 151,                    /* TLESS  */
  YYSYMBOL_TLESSEQUAL = 152,               /* TLESSEQUAL  */
  YYSYMBOL_TMINUS = 153,                   /* TMINUS  */
  YYSYMBOL_TMOD = 154,                     /* TMOD  */
  YYSYMBOL_TNOTEQUAL = 155,                /* TNOTEQUAL  */
  YYSYMBOL_TOR = 156,                      /* TOR  */
  YYSYMBOL_TPLUS = 157,                    /* TPLUS  */
  YYSYMBOL_TQUESTION = 158,                /* TQUESTION  */
  YYSYMBOL_TSEMI = 159,                    /* TSEMI  */
  YYSYMBOL_TSHIFTLEFT = 160,               /* TSHIFTLEFT  */
  YYSYMBOL_TSHIFTRIGHT = 161,              /* TSHIFTRIGHT  */
  YYSYMBOL_TSTAR = 162,                    /* TSTAR  */
  YYSYMBOL_TSWAP = 163,                    /* TSWAP  */
  YYSYMBOL_TLCBR = 164,                    /* TLCBR  */
  YYSYMBOL_TRCBR = 165,                    /* TRCBR  */
  YYSYMBOL_TLP = 166,                      /* TLP  */
  YYSYMBOL_TRP = 167,                      /* TRP  */
  YYSYMBOL_TLSBR = 168,                    /* TLSBR  */
  YYSYMBOL_TRSBR = 169,                    /* TRSBR  */
  YYSYMBOL_TNOELSE = 170,                  /* TNOELSE  */
  YYSYMBOL_TDOTDOTOPENHIGH = 171,          /* TDOTDOTOPENHIGH  */
  YYSYMBOL_TUPLUS = 172,                   /* TUPLUS  */
  YYSYMBOL_TUMINUS = 173,                  /* TUMINUS  */
  YYSYMBOL_TLNOT = 174,                    /* TLNOT  */
  YYSYMBOL_YYACCEPT = 175,                 /* $accept  */
  YYSYMBOL_program = 176,                  /* program  */
  YYSYMBOL_toplevel_stmt_ls = 177,         /* toplevel_stmt_ls  */
  YYSYMBOL_toplevel_stmt = 178,            /* toplevel_stmt  */
  YYSYMBOL_pragma_ls = 179,                /* pragma_ls  */
  YYSYMBOL_stmt = 180,                     /* stmt  */
  YYSYMBOL_module_decl_start = 181,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 182,         /* module_decl_stmt  */
  YYSYMBOL_access_control = 183,           /* access_control  */
  YYSYMBOL_opt_prototype = 184,            /* opt_prototype  */
  YYSYMBOL_include_access_control = 185,   /* include_access_control  */
  YYSYMBOL_include_module_stmt = 186,      /* include_module_stmt  */
  YYSYMBOL_187_1 = 187,                    /* $@1  */
  YYSYMBOL_block_stmt = 188,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 189,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 190,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 191,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 192,              /* opt_only_ls  */
  YYSYMBOL_except_ls = 193,                /* except_ls  */
  YYSYMBOL_use_access_control = 194,       /* use_access_control  */
  YYSYMBOL_use_stmt = 195,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 196,              /* import_stmt  */
  YYSYMBOL_import_expr = 197,              /* import_expr  */
  YYSYMBOL_import_ls = 198,                /* import_ls  */
  YYSYMBOL_require_stmt = 199,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 200,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 201,          /* opt_label_ident  */
  YYSYMBOL_ident_fn_def = 202,             /* ident_fn_def  */
  YYSYMBOL_ident_def = 203,                /* ident_def  */
  YYSYMBOL_ident_use = 204,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 205,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 206,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 207,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 208,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 209,              /* return_stmt  */
  YYSYMBOL_class_level_stmt = 210,         /* class_level_stmt  */
  YYSYMBOL_211_2 = 211,                    /* @2  */
  YYSYMBOL_private_decl = 212,             /* private_decl  */
  YYSYMBOL_forwarding_stmt = 213,          /* forwarding_stmt  */
  YYSYMBOL_extern_export_decl_stmt = 214,  /* extern_export_decl_stmt  */
  YYSYMBOL_215_3 = 215,                    /* $@3  */
  YYSYMBOL_216_4 = 216,                    /* $@4  */
  YYSYMBOL_217_5 = 217,                    /* $@5  */
  YYSYMBOL_218_6 = 218,                    /* $@6  */
  YYSYMBOL_extern_block_stmt = 219,        /* extern_block_stmt  */
  YYSYMBOL_loop_stmt = 220,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 221,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 222,                  /* if_stmt  */
  YYSYMBOL_defer_stmt = 223,               /* defer_stmt  */
  YYSYMBOL_try_stmt = 224,                 /* try_stmt  */
  YYSYMBOL_catch_stmt_ls = 225,            /* catch_stmt_ls  */
  YYSYMBOL_catch_stmt = 226,               /* catch_stmt  */
  YYSYMBOL_catch_expr = 227,               /* catch_expr  */
  YYSYMBOL_throw_stmt = 228,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 229,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 230,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 231,                /* when_stmt  */
  YYSYMBOL_class_decl_stmt = 232,          /* class_decl_stmt  */
  YYSYMBOL_class_tag = 233,                /* class_tag  */
  YYSYMBOL_opt_inherit = 234,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 235,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 236,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header = 237,              /* enum_header  */
  YYSYMBOL_enum_ls = 238,                  /* enum_ls  */
  YYSYMBOL_enum_item = 239,                /* enum_item  */
  YYSYMBOL_lambda_decl_expr = 240,         /* lambda_decl_expr  */
  YYSYMBOL_241_7 = 241,                    /* $@7  */
  YYSYMBOL_242_8 = 242,                    /* $@8  */
  YYSYMBOL_linkage_spec = 243,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt = 244,             /* fn_decl_stmt  */
  YYSYMBOL_245_9 = 245,                    /* $@9  */
  YYSYMBOL_246_10 = 246,                   /* $@10  */
  YYSYMBOL_fn_decl_stmt_inner = 247,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_receiver_expr = 248,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 249,                 /* fn_ident  */
  YYSYMBOL_assignop_ident = 250,           /* assignop_ident  */
  YYSYMBOL_opt_formal_ls = 251,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 252,            /* req_formal_ls  */
  YYSYMBOL_formal_ls = 253,                /* formal_ls  */
  YYSYMBOL_formal = 254,                   /* formal  */
  YYSYMBOL_opt_intent_tag = 255,           /* opt_intent_tag  */
  YYSYMBOL_required_intent_tag = 256,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 257,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_or_iter = 258,             /* proc_or_iter  */
  YYSYMBOL_opt_ret_tag = 259,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 260,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 261,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 262,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 263,               /* query_expr  */
  YYSYMBOL_var_arg_expr = 264,             /* var_arg_expr  */
  YYSYMBOL_opt_lifetime_where = 265,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 266, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 267,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 268,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 269,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_inner = 270, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 271,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 272,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 273,            /* var_decl_stmt  */
  YYSYMBOL_var_decl_stmt_inner_ls = 274,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 275,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 276, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 277, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 278,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 279,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 280,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 281,                 /* opt_type  */
  YYSYMBOL_array_type = 282,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 283, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 284,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 285,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 286,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 287,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 288,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 289,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 290,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 291,                /* actual_ls  */
  YYSYMBOL_actual_expr = 292,              /* actual_expr  */
  YYSYMBOL_ident_expr = 293,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 294,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 295,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 296,                 /* for_expr  */
  YYSYMBOL_cond_expr = 297,                /* cond_expr  */
  YYSYMBOL_nil_expr = 298,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 299,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 300,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 301,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 302,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 303,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 304,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 305,              /* intent_expr  */
  YYSYMBOL_shadow_var_prefix = 306,        /* shadow_var_prefix  */
  YYSYMBOL_io_expr = 307,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 308,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 309,                 /* new_expr  */
  YYSYMBOL_let_expr = 310,                 /* let_expr  */
  YYSYMBOL_expr = 311,                     /* expr  */
  YYSYMBOL_opt_expr = 312,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 313,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 314,                 /* lhs_expr  */
  YYSYMBOL_fun_expr = 315,                 /* fun_expr  */
  YYSYMBOL_call_expr = 316,                /* call_expr  */
  YYSYMBOL_dot_expr = 317,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 318,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 319,             /* bool_literal  */
  YYSYMBOL_literal_expr = 320,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 321,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 322,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 323,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 324,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 325,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 326       /* reduce_scan_op_expr  */
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

#line 888 "bison-chapel.cpp"

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
#define YYLAST   19039

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  175
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  152
/* YYNRULES -- Number of rules.  */
#define YYNRULES  642
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1154

#define YYMAXUTOK   429


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
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   506,   506,   511,   512,   518,   519,   524,   525,   530,
     531,   532,   533,   534,   535,   536,   537,   538,   539,   540,
     541,   542,   543,   544,   545,   546,   547,   548,   549,   550,
     551,   552,   553,   554,   555,   556,   557,   558,   559,   560,
     561,   565,   578,   583,   588,   596,   597,   598,   602,   603,
     607,   608,   609,   614,   613,   634,   635,   636,   641,   642,
     647,   652,   657,   661,   670,   675,   680,   685,   689,   693,
     701,   706,   710,   715,   719,   720,   721,   725,   726,   727,
     728,   729,   730,   731,   735,   740,   741,   742,   746,   747,
     751,   755,   757,   759,   761,   763,   765,   772,   773,   777,
     778,   779,   780,   781,   782,   785,   786,   787,   788,   789,
     790,   802,   803,   814,   815,   816,   817,   818,   819,   820,
     821,   822,   823,   824,   825,   826,   827,   828,   829,   830,
     831,   832,   836,   837,   838,   839,   840,   841,   842,   843,
     844,   845,   846,   847,   854,   855,   856,   857,   861,   862,
     866,   867,   871,   872,   873,   883,   883,   888,   889,   890,
     891,   892,   893,   894,   898,   899,   900,   901,   906,   905,
     921,   920,   937,   936,   952,   951,   967,   971,   976,   984,
     995,  1002,  1003,  1004,  1005,  1006,  1007,  1008,  1009,  1010,
    1011,  1012,  1013,  1014,  1015,  1016,  1017,  1018,  1019,  1020,
    1026,  1032,  1038,  1044,  1051,  1058,  1062,  1069,  1073,  1074,
    1075,  1076,  1077,  1079,  1081,  1083,  1088,  1091,  1092,  1093,
    1094,  1095,  1096,  1100,  1101,  1105,  1106,  1107,  1111,  1112,
    1116,  1119,  1121,  1126,  1127,  1131,  1133,  1135,  1142,  1152,
    1166,  1171,  1176,  1184,  1185,  1190,  1191,  1193,  1198,  1214,
    1221,  1230,  1238,  1242,  1249,  1250,  1255,  1260,  1254,  1287,
    1290,  1294,  1302,  1312,  1301,  1344,  1348,  1353,  1357,  1362,
    1369,  1370,  1374,  1375,  1376,  1377,  1378,  1379,  1380,  1381,
    1382,  1383,  1384,  1385,  1386,  1387,  1388,  1389,  1390,  1391,
    1392,  1393,  1394,  1395,  1396,  1397,  1398,  1399,  1400,  1404,
    1405,  1406,  1407,  1408,  1409,  1410,  1411,  1412,  1413,  1414,
    1415,  1419,  1420,  1424,  1428,  1429,  1430,  1434,  1436,  1438,
    1440,  1442,  1444,  1449,  1450,  1454,  1455,  1456,  1457,  1458,
    1459,  1460,  1461,  1462,  1466,  1467,  1468,  1469,  1470,  1471,
    1475,  1476,  1480,  1481,  1482,  1483,  1484,  1485,  1489,  1490,
    1493,  1494,  1498,  1499,  1503,  1507,  1508,  1509,  1517,  1518,
    1520,  1522,  1524,  1529,  1531,  1536,  1537,  1538,  1539,  1540,
    1541,  1542,  1546,  1548,  1553,  1555,  1557,  1562,  1575,  1592,
    1593,  1595,  1600,  1601,  1602,  1603,  1604,  1608,  1614,  1622,
    1623,  1631,  1633,  1638,  1640,  1642,  1647,  1649,  1651,  1658,
    1659,  1660,  1665,  1667,  1669,  1673,  1677,  1679,  1683,  1691,
    1692,  1693,  1694,  1695,  1700,  1701,  1702,  1703,  1704,  1724,
    1728,  1732,  1740,  1747,  1748,  1749,  1753,  1755,  1761,  1763,
    1765,  1770,  1771,  1772,  1773,  1774,  1780,  1781,  1782,  1783,
    1787,  1788,  1792,  1793,  1794,  1798,  1799,  1803,  1804,  1808,
    1809,  1813,  1814,  1815,  1816,  1820,  1821,  1832,  1834,  1836,
    1842,  1843,  1844,  1845,  1846,  1847,  1849,  1851,  1853,  1855,
    1857,  1859,  1862,  1864,  1866,  1868,  1870,  1872,  1874,  1876,
    1879,  1881,  1886,  1888,  1890,  1892,  1894,  1896,  1898,  1900,
    1902,  1904,  1906,  1908,  1910,  1917,  1923,  1929,  1935,  1944,
    1954,  1962,  1963,  1964,  1965,  1966,  1967,  1968,  1969,  1974,
    1975,  1979,  1983,  1984,  1988,  1992,  1993,  1997,  2001,  2005,
    2012,  2013,  2014,  2015,  2016,  2017,  2021,  2022,  2027,  2029,
    2033,  2037,  2041,  2049,  2054,  2060,  2066,  2073,  2083,  2091,
    2092,  2093,  2094,  2095,  2096,  2097,  2098,  2099,  2101,  2103,
    2105,  2120,  2122,  2124,  2126,  2131,  2132,  2136,  2137,  2138,
    2142,  2143,  2144,  2145,  2156,  2157,  2158,  2159,  2163,  2164,
    2165,  2169,  2170,  2171,  2172,  2173,  2181,  2182,  2183,  2184,
    2188,  2189,  2193,  2194,  2195,  2196,  2197,  2198,  2199,  2200,
    2201,  2203,  2205,  2206,  2207,  2211,  2219,  2220,  2224,  2225,
    2226,  2227,  2228,  2229,  2230,  2231,  2232,  2233,  2234,  2235,
    2236,  2237,  2238,  2239,  2240,  2241,  2242,  2243,  2244,  2245,
    2246,  2251,  2252,  2253,  2254,  2255,  2256,  2257,  2261,  2262,
    2263,  2264,  2268,  2269,  2270,  2271,  2276,  2277,  2278,  2279,
    2280,  2281,  2282
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
  "TINITEQUALS", "TLABEL", "TLAMBDA", "TLET", "TLIFETIME", "TLOCAL",
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
  "opt_try_expr", "lhs_expr", "fun_expr", "call_expr", "dot_expr",
  "parenthesized_expr", "bool_literal", "literal_expr", "assoc_expr_ls",
  "binary_op_expr", "unary_op_expr", "reduce_expr", "scan_expr",
  "reduce_scan_op_expr", YY_NULLPTR
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
     425,   426,   427,   428,   429
};
#endif

#define YYPACT_NINF (-1048)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-593)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1048,   100,  2996, -1048,   -60, -1048, -1048, -1048, -1048, -1048,
   -1048, -1048,  4348,    -5,   136, -1048, 13834, -1048, 17170,    -5,
   10573, -1048,   449,    72,   136,  4348, 10573,  4348,    40, 18729,
   10740,  7718, -1048,  7885,  8895,  6374, 10573, -1048, -1048,    43,
   -1048, -1048, 18779, -1048,  2866,  9062, -1048, 10573,   440, -1048,
   -1048, -1048, 10573, -1048, 13834, -1048, 10573,   307,   200,   283,
    2175, -1048, 18829, -1048,  9231,  7215, 10573,  9062, 13834, 10573,
     292, -1048,   278,  4348, -1048, 10573, -1048, 10907, 10907, 18779,
   -1048, -1048, 13834, -1048, -1048, 10573, 10573, -1048, 10573, -1048,
   10573, -1048, -1048, 13370, 10573, -1048, 10573, -1048, -1048, -1048,
    3334,  6543,  8054, 10573, -1048,  4179, -1048,   249, -1048,   386,
   -1048, -1048,   190, -1048, -1048, -1048, -1048, -1048, -1048, -1048,
   -1048, -1048, -1048, -1048, -1048, -1048, -1048, -1048, -1048, -1048,
   -1048, -1048, 18779, -1048, 18779,   464,    65, -1048, -1048,  2866,
   -1048,   326, -1048,   317, -1048, -1048,   344,   354,   369, 10573,
     361,   392, 18165,  1609,   436,   397,   398, -1048, -1048, -1048,
   -1048, -1048, -1048, -1048,   343, -1048, -1048, 18165,   362,  4348,
   -1048, -1048,   408, 10573, -1048, -1048, 10573, 10573, 10573, -1048,
   10573,  9231,  9231,   473, -1048, -1048, -1048, -1048,   117,   480,
   -1048, -1048,   412,  2672, 18779,  2866, -1048,   418, -1048,   -53,
   18165,   468,  7384,   531, 18879, 18165,   584,   535, -1048, 18929,
   18779,   584, 18779,   455,    27, 15091,    28, 15015,   198, -1048,
   15181, 14311,   465,  7384, -1048, -1048, -1048, -1048, -1048, -1048,
   -1048, -1048, -1048, -1048, -1048, -1048, -1048, -1048, -1048, -1048,
   -1048, -1048, -1048, -1048, -1048, -1048, -1048, -1048,  4348, -1048,
     456,  2583,   153,    22, -1048,  4348, -1048, -1048, 15427,   466,
   -1048,   461,   469, -1048, 15427,   117,   466, -1048,  7384,  1890,
   -1048, -1048,  9398, -1048, -1048, -1048, -1048,   163, 18165, 10573,
   10573, -1048, 18165,   475, 15962, -1048, 15427,   117, 18165,   484,
    7384, -1048, 18165, 16002, -1048, -1048, 16051, 13363, -1048, -1048,
   16148,   528,   494,   117, 15427, 16197,   557,   557,   970,   466,
     466,   151, -1048, -1048,  3503,   233, -1048, 10573, -1048,    83,
      99, -1048,   127,     5, 16243,   107,   970,   651, -1048,  3672,
   -1048,   599, 10573, 10573,   536,   515, -1048, -1048, -1048,   256,
     498, -1048, 10573,   547, 10573, 10573, 10573,  8895,  8895, 10573,
     499, 10573, 10573, 10573, 10573, 10573,   267, 13370, 10573, 10573,
   10573, 10573, 10573, 10573, 10573, 10573, 10573, 10573, 10573, 10573,
   10573, 10573, 10573, 10573,   626, -1048, -1048, -1048, -1048, -1048,
    8221,  8221, -1048, -1048, -1048, -1048,  8221, -1048, -1048,  8221,
    8221,  7384,  7384,  8895,  8895,  7048, -1048, -1048, 15503, 15583,
   16343,    12,  3841, -1048,  8895,    27,   533,   375, -1048, 10573,
   -1048, 10573,   575, -1048,   530,   555, -1048, -1048, -1048, 18779,
   -1048,  2866, -1048, 18779,   541, -1048,  2866,   655,  9231, -1048,
    4517,  8895, -1048,   538, -1048,    27,  4686,  8895, -1048,    27,
   11074, 10573, -1048,  4348, -1048,   668, 10573, -1048, -1048,   386,
     539, -1048,   579, -1048, -1048,  2583, -1048,   565,   540, -1048,
   11241,   589, 10573,  2866, -1048, -1048, 10573, 10573, -1048,   542,
   -1048,  9231, -1048, 18165, 18165, -1048,    29, -1048,  7384,   543,
   -1048,   690, -1048,   690, -1048, 11408,   573, -1048, -1048, -1048,
   -1048, -1048, -1048,  8390, -1048, 16389,  6712, -1048,  6881, -1048,
    4348,   545,  8895,  8559,  3165,   552, 10573,  9565, -1048, -1048,
     230, -1048,  4010, 18779, -1048,   389, 16429,   395, 15271,  9231,
     560, 18679,   222, -1048, 16542, 18391, 18391,   452, -1048,   452,
   -1048,   452, 18486,  1229,   691,  1089,   117,   557,   561, -1048,
   -1048, -1048, -1048,   970, 18524,   452,   646,   646, 18391,   646,
     646,  1198,   557, 18524, 18448,  1198,   466,   466,   557,   970,
     567,   569,   571,   572,   576,   586,   585,   563, -1048,   452,
   -1048,   452,    46, -1048, -1048, -1048,   144, -1048,  1537, 18278,
     481, 11575,  8895, 11742,  8895, 10573,  8895, 14098,    -5, 16587,
   -1048, -1048, -1048, 18165, 16627,  7384, -1048,  7384, 18779,   536,
     396, 18779,   536, -1048,   400, 10573,   250, 10573, 18165,    37,
   15659,  7048, -1048, 10573, 18165,    62, 15347, -1048,   592,   614,
     597, 16740,   614,   598,   725, 16780,  4348, 15739,   699, -1048,
      51, -1048, -1048, -1048, -1048, -1048, -1048,   215,   297, -1048,
   14179, -1048,   349,   600,  2583,   153,    40,    43, 10573, 10573,
    6207, -1048, -1048,   281,  7551, -1048, 18165, -1048, 16820, 16933,
   -1048, -1048, 18165,   605,   129,   609, -1048,  2692, -1048, -1048,
     403, 18779, -1048, -1048, -1048, -1048, -1048, -1048, -1048,  4348,
     139, 15815, -1048, -1048, 18165,  4348, 18165, -1048, 16974, -1048,
   -1048, -1048, 10573, -1048,   135,    82, 10573, -1048,  9732, 11074,
   10573,   625,  1443,   628,   671,   247, -1048,   711, -1048, -1048,
   -1048, -1048, 13208,   630, -1048, -1048, -1048, -1048, -1048, -1048,
   -1048, -1048, -1048, -1048,  7048, -1048,    38,  8895,  8895, 10573,
     773, 17014, 10573,   777, 17160,   641, 17200,    27,    27, -1048,
   -1048, -1048, -1048,   536,   647, -1048,   536,   650, -1048, 15427,
   -1048, 14387,  4855, -1048,  5024, -1048,   303, -1048, 14467,  5193,
   -1048,    27,  5362, -1048,    27, 10573, -1048, 10573, -1048,  4348,
   10573, -1048,  4348,   781, 18779, -1048, -1048, 18779,   959, -1048,
    2583,   679,   736, -1048, -1048, -1048,    36, -1048, -1048,   589,
     654,    49, -1048, -1048,   658,   661, -1048,  5531,  9231, -1048,
   -1048, -1048, 18779, -1048,   695,   412, -1048, -1048,  5700,   666,
    5869,   667, -1048, 10573, -1048, -1048, 10573, 17240,    50, 15905,
     680,   681,   673,  1642, -1048, 10573, 18779, -1048, -1048,   349,
     675,   121, -1048,   700, -1048,   702,   704,   713,   706,   707,
   -1048,   708,   720,   716,   717,   721,   268,   732,   726,   727,
   -1048, -1048, -1048, -1048, -1048, -1048, -1048, -1048, -1048, -1048,
   -1048, -1048, -1048, -1048, -1048, -1048, -1048, -1048, -1048, -1048,
   -1048, -1048, 10573, -1048,   737,   738,   728,   675,   675, -1048,
   -1048, -1048,   589,   306,   311, 17353, 11909, 12076, 17393, 12243,
   12410, 12577, 12744, -1048, -1048,   712, -1048,   718, -1048, -1048,
    4348, 10573, 18165, 10573, 18165,  7048, -1048,  4348, 10573, 18165,
   -1048, 10573, 18165, -1048, 17433, 18165, -1048, 18165,   847,  4348,
     724,   679, -1048,   719,  9901,   358, -1048,   197, -1048, -1048,
    8895, 13966,  7384,  7384,  4348, -1048,    64,   723, 10573, -1048,
   10573, -1048, 18165,  4348, 10573, -1048, 18165,  4348, 18165,   359,
   10068, 11074, 10573, 11074, 10573, -1048, -1048, -1048, -1048,  1890,
   -1048,  2090, -1048, 18165, -1048,    74,   579, -1048, 17546, -1048,
   14229, -1048, -1048, -1048, 10573, 10573, 10573, 10573, 10573, 10573,
   10573, 10573, -1048,  2294, -1048,  2456, 16780, 14543, 14623, -1048,
   16780, 14699, 14779, 10573,  4348, -1048, -1048,   358,   679,  8728,
   -1048, -1048, -1048,   244,  9231, -1048, -1048,    47, 10573,   -39,
   17586, -1048,   425,   730,   731,   468, -1048,   412, 18165, 14855,
   -1048, 14935, -1048, -1048, -1048, 18165,   733,   735,   744,   746,
   -1048, -1048, 12911,   785,   316, -1048,   753,   772,   675,   675,
   17699, 17739, 17779, 17892, 17932, 17972, 18302, -1048, 18415, -1048,
    4348,  4348,  4348,  4348, 18165, -1048, -1048, -1048,   358, 10237,
      67, -1048, 18165, -1048,   147, -1048,    -4, -1048,   782, 18085,
   -1048, -1048, -1048, 12744,   749,   750, -1048,  4348,  4348, -1048,
   -1048, -1048, -1048,  6038, -1048, -1048,    98, -1048,   197, -1048,
   -1048, -1048, 10573, 10573, 10573, 10573, 10573, 10573, -1048, -1048,
   16780, 16780, 16780, 16780, -1048, -1048, -1048, -1048, -1048,   313,
    8895, 13536, -1048, 10573,    47,   147,   147,   147,   147,   147,
     147,    47,   745, -1048, -1048, 16780, 16780,   742, 13078,    68,
     -43, 18125, -1048, -1048, 18165, -1048, -1048, -1048, -1048, -1048,
   -1048, -1048,   774, -1048, -1048,   315, 13702, -1048, -1048, -1048,
   10406, -1048,   337, -1048
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   111,   583,   584,   585,   586,
     587,   588,     0,   509,    97,   132,   478,   139,   480,   509,
       0,   138,     0,   383,    97,     0,     0,     0,     0,   133,
     555,   555,   580,     0,     0,     0,     0,   137,    53,     0,
     260,   134,     0,   256,     0,     0,   141,     0,   528,   500,
     589,   142,     0,   261,   472,   382,     0,     0,     0,   155,
      46,   136,   481,   384,     0,     0,     0,     0,   476,     0,
       0,   140,     0,     0,   112,     0,   581,     0,     0,     0,
     135,   242,   474,   386,   143,     0,     0,   638,     0,   640,
       0,   641,   642,   554,     0,   639,   636,   459,   152,   637,
       0,     0,     0,     0,     4,     0,     5,     0,     9,    48,
      10,    11,     0,    12,    13,    14,    16,   455,   456,    24,
      15,   153,   162,   163,    17,    19,    18,    21,    22,    23,
      20,   161,     0,   159,     0,   544,     0,   157,   160,     0,
     158,   560,   540,   457,   541,   462,   460,     0,     0,     0,
     545,   546,     0,   461,     0,   561,   562,   563,   582,   539,
     464,   463,   542,   543,     0,    40,    26,   470,     0,     0,
     510,    98,     0,     0,   480,   133,     0,     0,     0,   481,
       0,     0,     0,   544,   560,   460,   545,   546,   479,   461,
     561,   562,     0,   509,     0,     0,   385,     0,   216,     0,
     440,     0,   447,   586,   481,   556,   259,   586,   180,   481,
       0,   259,     0,     0,     0,     0,     0,     0,     0,   167,
       0,     0,    50,   447,   105,   113,   125,   119,   118,   127,
     108,   117,   128,   114,   129,   106,   130,   123,   116,   124,
     122,   120,   121,   107,   109,   115,   126,   131,     0,   110,
       0,     0,     0,     0,   389,     0,   149,    34,     0,   623,
     532,   529,   530,   531,     0,   473,   624,     7,   447,   259,
     240,   250,   555,   241,   154,   354,   437,     0,   436,     0,
       0,   150,   559,     0,     0,    37,     0,   477,   465,     0,
     447,    38,   471,     0,   223,   219,     0,   461,   223,   220,
       0,   379,     0,   475,     0,     0,   625,   627,   552,   622,
     621,     0,    55,    58,     0,     0,   442,     0,   444,     0,
       0,   443,     0,     0,   436,     0,   553,     0,     6,     0,
      49,     0,     0,     0,   243,     0,   341,   340,   262,     0,
     458,    25,     0,   533,     0,     0,     0,     0,     0,     0,
     626,     0,     0,     0,     0,     0,     0,   551,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   299,   306,   307,   308,   303,   305,
       0,     0,   301,   304,   302,   300,     0,   310,   309,     0,
       0,   447,   447,     0,     0,     0,    27,    28,     0,     0,
       0,     0,     0,    29,     0,     0,     0,     0,    30,     0,
      31,     0,   455,   453,     0,   448,   449,   454,   174,     0,
     177,     0,   170,     0,     0,   176,     0,     0,     0,   189,
       0,     0,   188,     0,   197,     0,     0,     0,   195,     0,
       0,    70,   164,     0,   299,   209,     0,    52,    51,    48,
       0,    32,   323,   257,   393,     0,   394,   396,     0,   418,
       0,   399,     0,     0,   148,    33,     0,     0,    35,     0,
     156,     0,    90,   557,   558,   151,     0,    36,   447,     0,
     230,   221,   217,   222,   218,     0,   377,   374,   182,    39,
      57,    56,    59,     0,   590,     0,     0,   576,     0,   578,
       0,     0,     0,     0,     0,     0,     0,     0,   594,     8,
       0,    42,     0,     0,    88,     0,    85,     0,    64,     0,
       0,     0,   334,   388,   508,   618,   617,   620,   629,   628,
     633,   632,   614,   611,   612,   613,   548,   601,     0,   573,
     574,   572,   571,   549,   605,   616,   610,   608,   619,   609,
     607,   599,   604,   606,   615,   598,   602,   603,   600,   550,
       0,     0,     0,     0,     0,     0,     0,     0,   631,   630,
     635,   634,   520,   521,   523,   525,     0,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   592,   509,   509,
     185,   375,   387,   441,     0,     0,   467,     0,     0,   243,
       0,     0,   243,   376,     0,     0,     0,     0,   484,     0,
       0,     0,   198,     0,   490,     0,     0,   196,   637,    73,
       0,    60,    71,     0,   208,     0,     0,     0,     0,   466,
     328,   325,   326,   327,   331,   332,   333,   323,     0,   315,
       0,   324,   342,     0,   397,     0,   146,   147,   145,   144,
       0,   417,   416,   540,     0,   391,   538,   390,     0,     0,
     570,   439,   438,     0,     0,     0,   468,     0,   224,   381,
     540,     0,   591,   547,   577,   445,   579,   446,   205,     0,
       0,     0,   593,   203,   494,     0,   596,   595,     0,    44,
      43,    41,     0,    84,     0,     0,     0,    77,     0,     0,
      70,   244,     0,     0,   254,     0,   251,   338,   335,   336,
     339,   263,     0,     0,    96,    94,    95,    93,    92,    91,
     568,   569,   522,   524,     0,   511,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   181,
     451,   452,   450,   243,     0,   179,   243,     0,   178,     0,
     207,     0,     0,   187,     0,   186,     0,   515,     0,     0,
     193,     0,     0,   191,     0,     0,   165,     0,   166,     0,
       0,   211,     0,   213,     0,   329,   330,     0,   323,   313,
       0,   431,   343,   346,   345,   347,     0,   395,   398,   399,
       0,     0,   400,   401,     0,     0,   232,     0,     0,   231,
     234,   469,     0,   225,   228,     0,   378,   206,     0,     0,
       0,     0,   204,     0,    89,    86,     0,    67,    66,    65,
       0,     0,     0,   259,   249,     0,   252,   248,   337,   342,
     311,    99,   294,   113,   292,   119,   118,   102,   117,   114,
     297,   129,   100,   130,   116,   120,   101,   103,   115,   131,
     291,   273,   276,   274,   275,   286,   277,   290,   282,   280,
     293,   296,   281,   279,   284,   289,   278,   283,   287,   288,
     285,   295,     0,   272,     0,   104,     0,   311,   311,   270,
     575,   513,   399,   560,   560,     0,     0,     0,     0,     0,
       0,     0,     0,   184,   183,     0,   245,     0,   245,   190,
       0,     0,   483,     0,   482,     0,   514,     0,     0,   489,
     194,     0,   488,   192,    62,    61,   210,   499,   212,     0,
       0,   431,   316,     0,     0,   399,   344,   358,   392,   422,
       0,   592,   447,   447,     0,   236,     0,     0,     0,   226,
       0,   201,   496,     0,     0,   199,   495,     0,   597,     0,
       0,     0,    70,     0,    70,    78,    81,   239,   155,   259,
     238,   259,   246,   255,   253,     0,   323,   269,     0,   298,
       0,   265,   266,   517,     0,     0,     0,     0,     0,     0,
       0,     0,   245,   259,   245,   259,   487,     0,     0,   516,
     493,     0,     0,     0,     0,   215,    54,   399,   431,     0,
     434,   433,   435,   540,   355,   319,   317,     0,     0,     0,
       0,   420,   540,     0,     0,   237,   235,     0,   229,     0,
     202,     0,   200,    87,    69,    68,     0,     0,     0,     0,
     247,   413,     0,   348,     0,   271,    99,   101,   311,   311,
       0,     0,     0,     0,     0,     0,   259,   173,   259,   169,
       0,     0,     0,     0,    63,   214,   320,   318,   399,   423,
       0,   357,   356,   372,     0,   373,   360,   363,     0,   359,
     352,   353,   258,     0,   534,   535,   227,     0,     0,    80,
      83,    79,    82,     0,   412,   411,   540,   349,   358,   312,
     267,   268,     0,     0,     0,     0,     0,     0,   175,   171,
     486,   485,   492,   491,   322,   321,   425,   426,   428,   540,
       0,   592,   371,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   540,   536,   537,   498,   497,     0,   403,     0,
       0,     0,   427,   429,   362,   364,   365,   368,   369,   370,
     366,   367,   361,   408,   406,   540,   592,   350,   264,   351,
     423,   407,   540,   430
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1048, -1048, -1048,    15,  -403,  2182, -1048, -1048, -1048,   482,
   -1048, -1048, -1048,   246,   588,  -431, -1048,  -675,  -670, -1048,
   -1048, -1048,   223, -1048, -1048,   329,   900, -1048,   524,  -189,
    -696, -1048,  -883,     0,  -977,  -803, -1048,   -57, -1048, -1048,
   -1048, -1048, -1048, -1048, -1048, -1048,    94, -1048, -1048, -1048,
     624, -1048,   133, -1048, -1048, -1048, -1048, -1048, -1048,  -522,
    -850, -1048, -1048, -1048,   111,  1101, -1048, -1048, -1048,   108,
   -1048, -1048, -1048, -1048,   -25,  -151,  -851, -1048,   -20,   170,
     312, -1048, -1048, -1048,   122, -1048, -1048,  -178,   270,  -838,
    -135,  -167,  -159,  -446, -1048,  -182, -1048,    16,   921,  -124,
     495, -1048,  -449,  -778, -1023, -1048,  -614,  -484, -1047,  -987,
    -750,   -19, -1048,  -223, -1048,  -161, -1048,   363,   514,  -334,
   -1048, -1048, -1048,  1257, -1048,   -10, -1048, -1048,  -212, -1048,
    -604, -1048, -1048, -1048,  1372,  1585,   -12,   929,   128,   898,
   -1048,  1785,  1801, -1048, -1048, -1048, -1048, -1048, -1048, -1048,
   -1048,  -359
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   313,   105,   624,   107,   108,   109,   331,
     449,   110,   222,   111,   314,   619,   517,   623,   620,   112,
     113,   114,   514,   515,   115,   116,   172,   873,   252,   117,
     249,   118,   651,   257,   119,   120,   269,   121,   122,   123,
     423,   601,   419,   598,   124,   125,   735,   126,   127,   128,
     481,   668,   805,   129,   130,   664,   800,   131,   132,   520,
     823,   133,   134,   705,   706,   183,   250,   642,   136,   137,
     522,   829,   711,   876,   877,   446,   967,   453,   638,   639,
     640,   641,   712,   338,   786,  1088,  1148,  1072,   276,  1005,
    1009,  1066,  1067,  1068,   138,   302,   486,   139,   140,   253,
     254,   457,   458,   655,  1085,  1033,   461,   652,  1107,  1002,
     925,   315,   199,   319,   320,   414,   415,   416,   184,   142,
     143,   144,   145,   185,   147,   169,   170,   576,   435,   756,
     577,   578,   148,   149,   186,   187,   152,   211,   417,   189,
     154,   190,   191,   157,   158,   159,   325,   160,   161,   162,
     163,   164
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   669,   390,   274,   188,   439,   643,   757,   193,   192,
     622,   928,   406,   412,   200,   339,   875,   104,   205,   205,
     962,   215,   217,   220,   221,   821,   971,   972,   424,   820,
     663,   789,  1071,   258,   412,   259,   580,   459,   195,   459,
     264,  1000,   265,    65,   266,   277,   983,    65,   985,   637,
    1063,   502,   278,   282,   284,   286,   287,   288,   586,   255,
     255,   292,   450,   293,  1132,   296,   300,   285,   462,   752,
     303,   407,  1108,   304,   305,  1031,   306,   744,   307,   412,
     747,   308,   309,   323,   310,     5,   951,   409,   278,   282,
     324,   326,   722,  -414,   759,   930,   255,   775,  -233,   165,
       3,   412,   538,  1153,   973,  1144,   410,   469,  1113,   168,
     501,   505,   882,  1110,   586,   539,  1147,   336,   952,   433,
     881,   402,  -414,  1151,  1133,   402,   653,   214,   216,   479,
     723,  -409,  1046,  1064,  1048,   776,  1114,   343,     5,   171,
    -233,   540,   433,   337,  1065,   503,   390,  1006,  -414,  1084,
    1063,   670,   503,  1071,   459,  -410,   196,  -414,  1030,  1056,
    -409,   167,   463,   401,   398,   399,   400,   542,   292,   278,
     324,   997,   927,  -409,   504,   460,   433,   460,  -414,    74,
     962,   587,   962,   405,  -410,   541,  -409,  1090,  1091,   503,
     282,   402,   402,   283,  -233,   788,   322,  -410,   797,  -414,
    -414,   402,   412,   412,   471,  -414,   202,   503,   503,   223,
    -410,   282,   470,  1032,   429,   432,   434,   438,   931,  1057,
    1104,   895,   421,   496,   897,    23,   402,   426,   402,   321,
     566,   567,    74,  -409,   195,   332,  1111,  1146,  -409,   498,
     798,   433,   630,   962,  1065,   962,   816,   507,  1058,   707,
     497,   350,   580,   433,  1007,  -105,   282,  -410,   465,   356,
     205,   631,  -410,  -111,   468,   632,   499,   473,   474,   622,
       5,    55,  -414,   675,   875,   677,   508,  1027,   282,  1029,
    1105,  1026,    63,  1028,   724,   633,   477,   538,   634,   412,
     327,   256,   460,  -414,   799,   708,   500,   600,   333,   635,
     539,   989,   604,   471,   488,   495,   709,    83,   808,  1008,
     165,   725,  -414,   256,   420,   267,   490,   665,   636,   425,
     516,   518,   472,   294,   298,   710,   540,  -415,   -76,   492,
     524,   923,   525,   526,   527,   529,   531,   532,  -415,   533,
     534,   535,   536,   537,   -47,   543,   544,   545,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   -47,  -432,    74,   580,   268,  -415,   282,   282,
     541,   318,  -402,   493,   282,   637,   782,   282,   282,   282,
     282,   569,   571,   579,  -432,   949,   289,   826,  -432,   165,
     471,   -76,   589,  -415,  -404,   689,   463,   593,   494,   594,
    -415,  -402,  -107,   761,   764,   590,   295,   299,   412,   606,
    -112,  -432,   827,   329,  -402,   523,   278,   750,   608,   610,
     961,  -415,   783,  -404,   614,   616,   393,  -402,   621,   621,
     394,   625,  -424,   784,   627,   612,  -404,   778,   403,   617,
    -415,   528,   530,   905,   290,  -415,  -519,  1011,  -415,  -404,
     656,  -518,   785,  -424,   658,   659,   778,  -424,   260,   662,
     256,   256,   256,   256,   779,   330,   282,   445,   809,   811,
     906,  -419,   413,  -519,  -402,   340,    23,   654,  -518,  -402,
    -424,   662,  -419,  1089,   282,  -502,   282,   568,   570,   806,
     681,   662,   684,   413,   686,   688,  -404,   346,   588,   765,
     701,  -404,  1004,  -501,   256,   815,   542,   278,   561,   562,
     256,  -419,   261,   341,   563,   463,   141,   564,   565,   342,
    -506,   622,    55,   622,  1023,   609,   141,   492,   395,   692,
     262,   615,   256,    63,   592,   696,   463,  -419,   413,   141,
     463,   141,   447,  -380,  -419,   448,   580,   263,   693,   347,
     256,  -507,   194,   348,   697,   745,  -504,  -503,    83,   748,
     413,   878,  -380,   637,   728,  -419,   248,   397,   394,   608,
     731,   614,   734,   625,   736,   684,   402,   408,   737,   738,
     961,   411,   961,   282,  -419,   282,   350,   141,   346,  -419,
    1003,   354,  -419,   749,   356,   751,   680,  1012,   359,   579,
     350,   758,   391,   301,   392,   354,   630,   355,   356,   753,
     755,    23,   359,   418,   141,   760,   763,   422,  1112,   141,
     366,   428,   452,  -505,   321,   631,   321,   466,   372,   632,
    -567,   791,  -567,    40,   475,   467,   288,   292,   324,  -567,
     347,  -567,   282,   961,   348,   961,  -564,   485,  -564,   633,
     478,   256,   634,   487,    57,    53,   334,    55,   335,   509,
     513,   413,   413,   635,  -566,  -565,  -566,  -565,    63,  1136,
    1137,  1138,  1139,  1140,  1141,   519,   730,   346,   733,   521,
     516,   256,   636,   141,   817,   256,   819,   621,   621,   356,
     560,   350,   591,    83,   595,   597,   354,   596,  1086,   356,
     603,   605,   626,   359,   611,   644,   629,   645,   654,   660,
     666,   667,   579,   671,   679,   529,   569,   885,   301,  -314,
     888,   685,   346,   741,   702,  1109,   714,   713,   715,   347,
     716,   717,   721,   348,   301,   718,   427,   893,   894,  1122,
     902,   661,   904,   412,   412,   719,  -314,   909,   413,   899,
     912,   -72,   720,   914,   765,   915,   766,   768,   917,   769,
     774,   910,   141,   661,   913,   471,   318,   787,   318,   141,
     796,  1013,  1014,   661,   347,   456,   801,  1109,   348,   936,
     350,   351,   793,   352,   353,   354,   278,   355,   356,   357,
     825,  -421,   359,   824,  1145,   828,   942,   880,   946,   365,
     366,   948,  -421,   369,   621,   886,   370,   371,   372,   889,
     891,   896,  1152,   963,   898,   919,  1109,   373,   924,  1039,
     926,   528,   568,   929,   932,   350,   351,   933,   141,   353,
     354,  -421,   355,   356,   938,   943,   947,   359,   957,   955,
     956,   966,  -132,   141,  -139,   366,  -138,  -108,  -137,  -134,
    -141,   370,   371,   372,  -106,   256,   256,  -421,  -142,  -136,
     968,   256,   256,  -140,  -421,   740,  -109,   413,  -135,  -143,
     970,   969,  -110,   773,   902,   904,   982,   909,   912,   942,
     946,   994,   984,   996,  1087,  -421,   998,  -105,   986,   987,
    1017,   988,  1079,   579,  1080,   990,   991,  1074,  1075,   992,
     153,  1115,   274,  1081,  -421,  1082,  -107,  1123,  1124,  -421,
     153,  1143,  -421,   803,  1114,   814,   141,   512,  1010,   684,
     282,   282,   483,   153,   197,   153,  1018,  1116,  1019,  1117,
    1118,   628,  1021,  1119,  1120,   937,  1016,   964,  1025,   621,
     621,   621,   621,   599,   141,  1038,  1034,   602,   922,   777,
     141,   965,  1149,  1130,  1142,  1135,   219,   141,   657,   206,
     742,     0,   986,  1040,  1041,   990,  1042,  1043,  1044,  1045,
       0,   153,     0,     0,     0,   297,   297,     0,     0,   456,
    1060,  1054,     0,   256,   256,     0,   630,   324,     0,     0,
       0,     0,  1062,     0,     0,   256,  1069,   445,   153,     0,
       0,   346,     0,   153,   445,   631,     0,   256,     0,   632,
     256,     0,     0,     0,   141,     0,     0,     0,   141,     0,
       0,     0,     0,     0,     0,     0,   141,     0,     0,   633,
       0,     0,   634,     0,    57,     0,     0,   691,  1100,  1101,
    1102,  1103,     0,   635,     0,   704,     0,     0,     0,     0,
       0,   939,     0,   347,     0,     0,     0,   348,     0,     0,
       0,   946,   636,     0,  1129,  1125,  1126,   153,     0,     0,
       0,   324,     0,     0,     0,     0,     0,     0,     0,     0,
    1100,  1101,  1102,  1103,  1125,  1126,     0,     0,     0,     0,
       0,     0,   726,     0,     0,     0,     0,     0,  1131,   684,
       0,  1134,     0,   135,   350,   351,     0,   352,   353,   354,
       0,   355,   356,   135,     0,     0,   359,     0,     0,     0,
     346,     0,   743,   365,   366,   746,   135,   369,   135,     0,
     370,   371,   372,     0,   684,     0,     0,     0,   946,     0,
     141,     0,     0,     0,     0,     0,   153,     0,     0,     0,
       0,     0,     0,   153,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   781,     0,     0,     0,   456,     0,
       0,     0,   347,     0,   135,     0,   348,     0,     0,     0,
       0,     0,   256,     0,     0,     0,     0,     0,     0,     0,
       0,   804,     0,   141,  1001,   301,     0,     0,     0,   141,
       0,   135,   413,   413,     0,     0,   135,     0,     0,     0,
       0,     0,   153,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   350,   351,     0,   879,   153,   354,   346,
     355,   356,     0,   445,   445,   359,   874,   445,   445,     0,
       0,   883,   884,   366,     0,     0,     0,     0,     0,   370,
     371,   372,     0,     0,     0,  1070,     0,     0,     0,   146,
     346,     0,     0,  1076,     0,   445,   141,   445,   141,   146,
     135,     0,     0,   141,  1061,     0,   141,     0,     0,     0,
       0,   347,   146,   141,   146,   348,   141,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   920,     0,
     153,   921,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   141,   347,     0,     0,     0,   348,     0,     0,     0,
       0,     0,   141,     0,   141,     0,   804,     0,   153,  1106,
     146,     0,   350,   351,   153,   352,   353,   354,     0,   355,
     356,   153,     0,     0,   359,     0,     0,     0,     0,   135,
     704,     0,   366,     0,     0,     0,   135,   146,   370,   371,
     372,     0,   146,   350,     0,     0,     0,     0,   354,     0,
     355,   356,     0,     0,   150,   359,  1070,     0,     0,     0,
       0,  1106,     0,   366,   150,     0,     0,     0,     0,   370,
     371,   372,     0,     0,     0,     0,     0,   150,   153,   150,
       0,     0,   153,     0,     0,     0,     0,     0,     0,     0,
     153,     0,     0,     0,   141,   135,     0,     0,     0,     0,
    1106,   141,     0,     0,     0,     0,   146,     0,     0,     0,
     135,     0,     0,   141,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   822,   150,     0,     0,   141,     0,
       0,     0,     0,     0,     0,     0,     0,   141,     0,     0,
       0,   141,     0,     0,     0,  -245,     0,     0,     0,  -245,
    -245,     0,   150,     0,     0,     0,     0,   150,  -245,     0,
    -245,  -245,     0,     0,     0,  -245,     0,     0,     0,     0,
       0,     0,  -245,     0,   874,  -245,     0,     0,     0,     0,
       0,     0,     0,   135,     0,   146,     0,     0,   141,     0,
       0,     0,   146,     0,  -245,     0,  -245,     0,  -245,     0,
    -245,  -245,     0,  -245,   153,  -245,     0,  -245,     0,     0,
       0,   135,     0,     0,     0,     0,     0,   135,     0,     0,
       5,   150,     0,     0,   135,     0,  -245,     0,     0,  -245,
       0,     0,  -245,     0,    15,     0,     0,    17,     0,     0,
       0,     0,    21,     0,   141,   141,   141,   141,     0,     0,
       0,   146,   175,     0,     0,     0,     0,   153,     0,     0,
       0,    37,     0,   153,     0,     0,   146,   151,    41,     0,
       0,   141,   141,     0,     0,     0,    46,   151,     0,     0,
       0,   135,  -245,    51,     0,   135,     0,     0,  -245,     0,
     151,     0,   151,   135,     0,     0,     0,     0,    61,     0,
     150,     0,     0,     0,     0,     0,     0,   150,     0,     0,
      71,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,    80,     0,     0,     0,     0,     0,    84,     0,     0,
     153,     0,   153,     0,     0,     0,     0,   153,   151,   146,
     153,     0,     0,     0,   270,     0,     0,   153,    22,    23,
     153,     0,     0,     0,     0,     0,     0,   271,     0,    30,
     272,     0,     0,     0,    35,   151,   150,   146,     0,     0,
     151,    40,     0,   146,     0,   153,     0,     0,     0,     0,
     146,   150,     0,     0,     0,     0,   153,     0,   153,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,   958,
       0,     0,   959,     0,   273,     0,    63,   135,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,   151,     0,     0,   146,     0,  -526,
       0,   146,     0,     0,     0,     0,     0,     0,     0,   146,
       0,     0,   389,     0,   150,  -564,     0,  -564,     0,     0,
     135,     0,     0,     0,     0,     0,   135,   155,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   155,   153,     0,
       0,    98,   150,   156,     0,   153,     0,   960,   150,     0,
     155,     0,   155,   156,     0,   150,     0,   153,     0,     0,
       0,     0,     0,     0,     0,     0,   156,     0,   156,     0,
       0,     0,   153,   151,     0,     0,     0,     0,     0,     0,
     151,   153,     0,     0,     0,   153,     0,     0,     0,     0,
       0,     0,     0,   135,     0,   135,     0,     0,   155,     0,
     135,     0,     0,   135,     0,     0,     0,     0,     0,     0,
     135,     0,   150,   135,   156,     0,   150,     0,     0,     0,
       0,     0,     0,   146,   150,   155,     0,     0,     0,     0,
     155,     0,   153,     0,     0,     0,     0,     0,   135,   151,
       0,   156,     0,     0,     0,     0,   156,     0,     0,   135,
       0,   135,   270,     0,   151,     0,    22,    23,     0,     0,
       0,     0,     0,     0,     0,   271,     0,    30,   272,     0,
       0,     0,    35,     0,     0,     0,   146,     0,     0,    40,
       0,     0,   146,     0,     0,     0,     0,     0,   153,   153,
     153,   153,     0,     0,   155,     0,     0,     0,     0,     0,
       0,    53,     0,    55,     0,     0,     0,     0,     0,     0,
     156,     0,   273,     0,    63,   153,   153,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   151,     0,     0,
       0,     0,     0,    79,     0,     0,    81,     0,   150,    83,
       0,   135,     0,     0,     0,     0,     0,     0,   135,   146,
       0,   146,     0,     0,     0,   151,   146,     0,     0,   146,
     135,   151,     0,     0,     0,     0,   146,     0,   151,   146,
       0,     0,     0,   155,     0,   135,     0,     0,     0,     0,
     155,     0,     0,     0,   135,     0,     0,     0,   135,   156,
       0,   150,     0,     0,   146,     0,   156,   150,     0,     0,
       0,     0,     0,     0,     0,   146,     0,   146,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   151,     0,     0,     0,   151,
       0,     0,     0,     0,     0,   135,     0,   151,     0,   155,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,   155,   156,    22,    23,     0,     0,
       0,     0,     0,     0,   150,   271,   150,    30,   272,     0,
     156,   150,    35,     0,   150,     0,     0,     0,     0,    40,
       0,   150,     0,     0,   150,     0,     0,     0,     0,     0,
       0,   135,   135,   135,   135,     0,     0,   146,     0,     0,
       0,    53,     0,    55,   146,   327,     0,   958,     0,   150,
     959,     0,   273,     0,    63,     0,   146,     0,   135,   135,
     150,     0,   150,     0,   106,     0,     0,   155,     0,     0,
       0,   146,     0,    79,   166,     0,    81,   270,     0,    83,
     146,    22,    23,   156,   146,     0,     0,   198,     0,   201,
     271,   151,    30,   272,     0,   155,     0,    35,     0,     0,
     -75,   155,     0,     0,    40,     0,     0,  -259,   155,     0,
       0,   156,     0,     0,     0,     0,     0,   156,     0,     0,
       0,     0,     0,     0,   156,     0,    53,     0,    55,    98,
       0,   146,     0,  -259,     0,   291,     0,   273,     0,    63,
       0,     0,     0,     0,   151,     0,     0,     0,     0,     0,
     151,     0,   150,     0,     0,     0,     0,     0,    79,   150,
       0,    81,   106,   -75,    83,   155,     0,   328,     0,   155,
       0,   150,     0,     0,     0,     0,     0,   155,     0,     0,
       0,   156,     0,     0,     0,   156,   150,   146,   146,   146,
     146,     0,     0,   156,     0,   150,   270,     0,     0,   150,
      22,    23,     0,     0,     0,     0,     0,     0,     0,   271,
       0,    30,   272,     0,   146,   146,    35,   151,     0,   151,
       0,     0,     0,    40,   151,     0,     0,   151,     0,     0,
       0,   396,     0,     0,   151,     0,     0,   151,     0,     0,
       0,     0,     0,     0,     0,    53,   150,    55,     0,    57,
       0,   958,     0,     0,   959,     0,   273,     0,    63,     0,
       0,     0,   151,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   151,     0,   151,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,   155,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   150,   150,   150,   150,     0,   156,     0,     0,
     451,     0,     0,     0,     0,     0,     0,   464,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   150,
     150,     0,     0,    98,     0,     0,     0,     0,     0,  1047,
       0,     0,     0,     0,   155,     0,     0,     0,     0,     0,
     155,     0,     0,     0,     0,     0,     0,     0,   270,     0,
     156,     0,    22,    23,     0,   151,   156,     0,     0,     0,
       0,   271,   151,    30,   272,     0,   106,     0,    35,     0,
       0,     0,     0,     0,   151,    40,     0,     0,     0,     0,
       0,   106,     0,     0,     0,     0,     0,     0,     0,   151,
       0,     0,     0,     0,     0,     0,     0,    53,   151,    55,
       0,    57,   151,   958,     0,     0,   959,   155,   273,   155,
      63,     0,     0,     0,   155,     0,     0,   155,     0,     0,
       0,     0,     0,   156,   155,   156,     0,   155,     0,    79,
     156,     0,    81,   156,     0,    83,     0,     0,     0,     0,
     156,     0,     0,   156,     0,     0,     0,     0,     0,   151,
       0,     0,   155,     0,   106,     0,   224,     0,     0,     0,
       0,     0,     0,   155,     0,   155,     0,     0,   156,     0,
     225,   226,     0,   227,     0,     0,     0,     0,   228,   156,
       0,   156,   464,     0,     0,    98,   229,     0,   464,     0,
       0,  1049,   230,     0,     0,     0,     0,   231,     0,     0,
       0,   232,     0,     0,   233,   151,   151,   151,   151,     0,
       0,     0,   234,     0,     0,     0,     0,     0,   235,   236,
       0,     0,     0,     0,     0,   237,     0,     0,     0,     0,
       0,     0,   151,   151,   238,     0,     0,     0,     0,     0,
       0,     0,     0,   239,   240,     0,   241,     0,   242,     0,
     243,     0,   678,   244,   344,   155,   683,   245,   454,     0,
     246,   345,   155,   247,   106,   224,     0,     0,     0,     0,
       0,   156,     0,   346,   155,     0,     0,     0,   156,   225,
     226,     0,   227,     0,     0,     0,     0,   228,   404,   155,
     156,     0,     0,     0,     0,   229,     0,     0,   155,     0,
       0,   230,   155,     0,     0,   156,   231,     0,     0,     0,
     232,     0,     0,   233,   156,     0,     0,     0,   156,   455,
       0,   234,     0,     0,     0,   347,     0,   235,   236,   348,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,     0,     0,     0,     0,   155,
       0,     0,   239,   240,     0,   241,   168,   242,     0,   243,
     349,     0,   244,     0,     0,   156,   245,     0,     0,   246,
       0,     0,   247,     0,     0,     0,   350,   351,   771,   352,
     353,   354,     0,   355,   356,   357,     0,   358,   359,   360,
     361,   362,     0,   363,   364,   365,   366,   367,   368,   369,
       0,     0,   370,   371,   372,   155,   155,   155,   155,     0,
       0,     0,     0,   373,     0,     0,     0,     0,     0,     0,
       0,   156,   156,   156,   156,     0,   402,     0,   802,     0,
       0,   807,   155,   155,     0,     0,     0,   812,     0,   224,
       0,     0,     0,     0,     0,     0,     0,     0,   156,   156,
       0,     0,     0,   225,   226,     0,   227,     0,     0,     0,
       0,   228,     0,     0,     0,     0,     0,     0,     0,   229,
       0,     0,     0,     0,     0,   230,     0,     0,     0,     0,
     231,     0,     0,     0,   232,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,   235,   236,     0,   464,     0,   464,     0,   237,     0,
       0,   464,     0,     0,   464,     0,     0,   238,     0,     0,
       0,   916,     0,     0,   918,     0,   239,   240,     0,   241,
       0,   242,     0,   243,     0,     0,   244,     0,     0,     0,
     245,     0,     0,   246,     0,     0,   247,     0,     0,   935,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     941,     0,   945,     0,     0,     0,    -2,     4,     0,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,   251,    30,    31,    32,    33,    34,    35,    36,
      37,   -74,     0,    38,    39,    40,     0,    41,  -259,     0,
      42,    43,    44,     0,    45,    46,    47,   -45,    48,    49,
       0,    50,    51,    52,     0,     0,     0,    53,    54,    55,
      56,    57,    58,    59,  -259,   -45,    60,    61,    62,     0,
      63,    64,    65,     0,    66,    67,    68,    69,    70,    71,
      72,    73,     0,    74,    75,     0,    76,    77,    78,    79,
      80,   995,    81,    82,   -74,    83,    84,     0,     0,    85,
       0,    86,     0,     0,    87,     0,  1015,     0,     0,     0,
       0,     0,     0,     0,     0,  1020,     0,     0,     0,  1022,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,    98,     0,     0,    99,     0,
     100,     0,   101,     0,   102,     0,     4,   103,     5,     0,
       6,     7,     8,     9,    10,    11,  1055,  -592,     0,    12,
      13,    14,    15,    16,  -592,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,  -592,    27,    28,  -592,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
     -74,     0,    38,    39,    40,     0,    41,  -259,     0,    42,
      43,    44,     0,    45,    46,    47,   -45,    48,    49,     0,
      50,    51,    52,     0,     0,     0,    53,    54,    55,    56,
       0,    58,    59,  -259,   -45,    60,    61,    62,  -592,    63,
      64,    65,  -592,    66,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
       0,    81,    82,   -74,    83,    84,     0,     0,    85,     0,
      86,     0,     0,  -592,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -592,
    -592,    90,  -592,  -592,  -592,  -592,  -592,  -592,  -592,     0,
    -592,  -592,  -592,  -592,  -592,     0,  -592,  -592,  -592,  -592,
    -592,  -592,  -592,  -592,    98,  -592,  -592,  -592,     0,   100,
    -592,   101,     0,   102,     0,   311,  -592,     5,   275,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,   -74,
       0,    38,    39,    40,     0,    41,  -259,     0,    42,    43,
      44,     0,    45,    46,    47,   -45,    48,    49,     0,    50,
      51,    52,     0,     0,     0,    53,    54,    55,    56,    57,
      58,    59,  -259,   -45,    60,    61,    62,     0,    63,    64,
      65,     0,    66,    67,    68,    69,    70,    71,    72,    73,
       0,    74,    75,     0,    76,    77,    78,    79,    80,     0,
      81,    82,   -74,    83,    84,     0,     0,    85,     0,    86,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,    98,     0,     0,    99,     0,   100,   312,
     101,     0,   102,     0,     4,   103,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,   -74,     0,
      38,    39,    40,     0,    41,  -259,     0,    42,    43,    44,
       0,    45,    46,    47,   -45,    48,    49,     0,    50,    51,
      52,     0,     0,     0,    53,    54,    55,    56,    57,    58,
      59,  -259,   -45,    60,    61,    62,     0,    63,    64,    65,
       0,    66,    67,    68,    69,    70,    71,    72,    73,     0,
      74,    75,     0,    76,    77,    78,    79,    80,     0,    81,
      82,   -74,    83,    84,     0,     0,    85,     0,    86,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,    98,     0,     0,    99,     0,   100,   491,   101,
       0,   102,     0,   510,   103,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,   -74,     0,    38,
      39,    40,     0,    41,  -259,     0,    42,    43,    44,     0,
      45,    46,    47,   -45,    48,    49,     0,    50,    51,    52,
       0,     0,     0,    53,    54,    55,    56,    57,    58,    59,
    -259,   -45,    60,    61,    62,     0,    63,    64,    65,     0,
      66,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,     0,    81,    82,
     -74,    83,    84,     0,     0,    85,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,     0,   100,   511,   101,     0,
     102,     0,   311,   103,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,   -74,     0,    38,    39,
      40,     0,    41,  -259,     0,    42,    43,    44,     0,    45,
      46,    47,   -45,    48,    49,     0,    50,    51,    52,     0,
       0,     0,    53,    54,    55,    56,    57,    58,    59,  -259,
     -45,    60,    61,    62,     0,    63,    64,    65,     0,    66,
      67,    68,    69,    70,    71,    72,    73,     0,    74,    75,
       0,    76,    77,    78,    79,    80,     0,    81,    82,   -74,
      83,    84,     0,     0,    85,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
      98,     0,     0,    99,     0,   100,   312,   101,     0,   102,
       0,     4,   103,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,   -74,     0,    38,    39,    40,
       0,    41,  -259,     0,    42,    43,    44,     0,    45,    46,
      47,   -45,    48,    49,     0,    50,    51,    52,     0,     0,
       0,    53,    54,    55,    56,    57,    58,    59,  -259,   -45,
      60,    61,    62,     0,    63,    64,    65,     0,    66,    67,
      68,    69,    70,    71,    72,    73,     0,    74,    75,     0,
      76,    77,    78,    79,    80,     0,    81,    82,   -74,    83,
      84,     0,     0,    85,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,    98,
       0,     0,    99,     0,   100,   690,   101,     0,   102,     0,
       4,   103,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,   -74,     0,    38,    39,    40,     0,
      41,  -259,     0,    42,    43,    44,     0,    45,    46,    47,
     -45,    48,    49,     0,    50,    51,    52,     0,     0,     0,
      53,    54,    55,    56,   327,    58,    59,  -259,   -45,    60,
      61,    62,     0,    63,    64,    65,     0,    66,    67,    68,
      69,    70,    71,    72,    73,     0,    74,    75,     0,    76,
      77,    78,    79,    80,     0,    81,    82,   -74,    83,    84,
       0,     0,    85,     0,    86,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,    98,     0,
       0,    99,     0,   100,     0,   101,     0,   102,     0,     4,
     103,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,   -74,     0,    38,    39,    40,     0,    41,
    -259,     0,    42,    43,    44,     0,    45,    46,    47,   -45,
      48,    49,     0,    50,    51,    52,     0,     0,     0,    53,
      54,    55,    56,     0,    58,    59,  -259,   -45,    60,    61,
      62,     0,    63,    64,    65,     0,    66,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,     0,    81,    82,   -74,    83,    84,     0,
       0,    85,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,    98,     0,     0,
      99,     0,   100,     0,   101,     0,   102,     0,     4,   103,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
     607,    37,   -74,     0,    38,    39,    40,     0,    41,  -259,
       0,    42,    43,    44,     0,    45,    46,    47,   -45,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,     0,    58,    59,  -259,   -45,    60,    61,    62,
       0,    63,    64,    65,     0,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,     0,    81,    82,   -74,    83,    84,     0,     0,
      85,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,    98,     0,     0,    99,
       0,   100,     0,   101,     0,   102,     0,     4,   103,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,   613,
      37,   -74,     0,    38,    39,    40,     0,    41,  -259,     0,
      42,    43,    44,     0,    45,    46,    47,   -45,    48,    49,
       0,    50,    51,    52,     0,     0,     0,    53,    54,    55,
      56,     0,    58,    59,  -259,   -45,    60,    61,    62,     0,
      63,    64,    65,     0,    66,    67,    68,    69,    70,    71,
      72,    73,     0,    74,    75,     0,    76,    77,    78,    79,
      80,     0,    81,    82,   -74,    83,    84,     0,     0,    85,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,    98,     0,     0,    99,     0,
     100,     0,   101,     0,   102,     0,     4,   103,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,   901,    37,
     -74,     0,    38,    39,    40,     0,    41,  -259,     0,    42,
      43,    44,     0,    45,    46,    47,   -45,    48,    49,     0,
      50,    51,    52,     0,     0,     0,    53,    54,    55,    56,
       0,    58,    59,  -259,   -45,    60,    61,    62,     0,    63,
      64,    65,     0,    66,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
       0,    81,    82,   -74,    83,    84,     0,     0,    85,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,    98,     0,     0,    99,     0,   100,
       0,   101,     0,   102,     0,     4,   103,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,   903,    37,   -74,
       0,    38,    39,    40,     0,    41,  -259,     0,    42,    43,
      44,     0,    45,    46,    47,   -45,    48,    49,     0,    50,
      51,    52,     0,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -259,   -45,    60,    61,    62,     0,    63,    64,
      65,     0,    66,    67,    68,    69,    70,    71,    72,    73,
       0,    74,    75,     0,    76,    77,    78,    79,    80,     0,
      81,    82,   -74,    83,    84,     0,     0,    85,     0,    86,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,    98,     0,     0,    99,     0,   100,     0,
     101,     0,   102,     0,     4,   103,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,   908,    37,   -74,     0,
      38,    39,    40,     0,    41,  -259,     0,    42,    43,    44,
       0,    45,    46,    47,   -45,    48,    49,     0,    50,    51,
      52,     0,     0,     0,    53,    54,    55,    56,     0,    58,
      59,  -259,   -45,    60,    61,    62,     0,    63,    64,    65,
       0,    66,    67,    68,    69,    70,    71,    72,    73,     0,
      74,    75,     0,    76,    77,    78,    79,    80,     0,    81,
      82,   -74,    83,    84,     0,     0,    85,     0,    86,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,    98,     0,     0,    99,     0,   100,     0,   101,
       0,   102,     0,     4,   103,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,   911,    37,   -74,     0,    38,
      39,    40,     0,    41,  -259,     0,    42,    43,    44,     0,
      45,    46,    47,   -45,    48,    49,     0,    50,    51,    52,
       0,     0,     0,    53,    54,    55,    56,     0,    58,    59,
    -259,   -45,    60,    61,    62,     0,    63,    64,    65,     0,
      66,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,     0,    81,    82,
     -74,    83,    84,     0,     0,    85,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,     0,   100,     0,   101,     0,
     102,     0,     4,   103,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,   934,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,   -74,     0,    38,    39,
      40,     0,    41,  -259,     0,    42,    43,    44,     0,    45,
      46,    47,   -45,    48,    49,     0,    50,    51,    52,     0,
       0,     0,    53,    54,    55,    56,     0,    58,    59,  -259,
     -45,    60,    61,    62,     0,    63,    64,    65,     0,    66,
      67,    68,    69,    70,    71,    72,    73,     0,    74,    75,
       0,    76,    77,    78,    79,    80,     0,    81,    82,   -74,
      83,    84,     0,     0,    85,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
      98,     0,     0,    99,     0,   100,     0,   101,     0,   102,
       0,     4,   103,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,   940,    37,   -74,     0,    38,    39,    40,
       0,    41,  -259,     0,    42,    43,    44,     0,    45,    46,
      47,   -45,    48,    49,     0,    50,    51,    52,     0,     0,
       0,    53,    54,    55,    56,     0,    58,    59,  -259,   -45,
      60,    61,    62,     0,    63,    64,    65,     0,    66,    67,
      68,    69,    70,    71,    72,    73,     0,    74,    75,     0,
      76,    77,    78,    79,    80,     0,    81,    82,   -74,    83,
      84,     0,     0,    85,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,    98,
       0,     0,    99,     0,   100,     0,   101,     0,   102,     0,
       4,   103,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,   944,    37,   -74,     0,    38,    39,    40,     0,
      41,  -259,     0,    42,    43,    44,     0,    45,    46,    47,
     -45,    48,    49,     0,    50,    51,    52,     0,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -259,   -45,    60,
      61,    62,     0,    63,    64,    65,     0,    66,    67,    68,
      69,    70,    71,    72,    73,     0,    74,    75,     0,    76,
      77,    78,    79,    80,     0,    81,    82,   -74,    83,    84,
       0,     0,    85,     0,    86,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,    98,     0,
       0,    99,     0,   100,     0,   101,     0,   102,     0,  1127,
     103,     5,   275,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   173,     0,     0,    15,    16,     0,    17,     0,
     174,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   175,     0,     0,     0,    32,   176,   177,
       0,   178,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     179,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   180,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   181,     0,   101,     0,   182,  1128,   790,   103,
       5,   275,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   173,     0,     0,    15,    16,     0,    17,     0,   174,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   175,     0,     0,     0,    32,   176,   177,     0,
     178,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   179,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   180,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   181,     0,   101,     0,   182,     0,     5,   103,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   173,     0,
       0,    15,    16,     0,    17,     0,   174,     0,     0,    21,
     218,    23,     0,     0,     0,     0,     0,    28,     0,   175,
       0,     0,     0,    32,   176,   177,     0,   178,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,    55,    56,     0,
      58,     0,     0,     0,     0,    61,   179,     0,    63,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   180,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,    83,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   181,     0,
     101,     0,   182,     0,     0,   103,     5,   275,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   173,     0,     0,
      15,    16,     0,    17,     0,   174,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   175,     0,
       0,     0,    32,   176,   177,     0,   178,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   179,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   180,     0,
      74,     0,     0,    76,   279,   280,     0,    80,   316,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,   317,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   181,     0,   101,
       0,   182,     0,     0,   103,     5,   275,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   173,     0,     0,    15,
      16,     0,    17,     0,   174,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   175,     0,     0,
       0,    32,   176,   177,     0,   178,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   179,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   180,     0,    74,
       0,     0,    76,   279,   280,     0,    80,   316,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   181,     0,   101,   674,
     182,     0,     0,   103,     5,   275,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   173,     0,     0,    15,    16,
       0,    17,     0,   174,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   175,     0,     0,     0,
      32,   176,   177,     0,   178,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   179,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   180,     0,    74,     0,
       0,    76,   279,   280,     0,    80,   316,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   181,     0,   101,   676,   182,
       0,     5,   103,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   173,     0,     0,    15,    16,     0,    17,     0,
     174,     0,     0,    21,     0,   572,     0,     0,     0,     0,
       0,    28,     0,   175,     0,     0,     0,    32,   176,   177,
       0,   178,    37,     0,   573,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     179,     0,   574,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   180,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,   575,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   181,     0,   101,     0,   182,     0,     5,   103,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   173,
       0,     0,    15,    16,     0,    17,     0,   174,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     175,     0,     0,     0,    32,   176,   177,     0,   178,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   179,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     180,     0,    74,     0,     0,    76,   279,   280,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,   281,     0,     0,    99,     0,   181,
       0,   101,     0,   182,     0,     0,   103,     5,   275,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   173,     0,
       0,    15,    16,     0,    17,     0,   174,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   175,
       0,     0,     0,    32,   176,   177,     0,   178,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   179,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   180,
       0,    74,     0,     0,    76,   279,   280,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   181,     0,
     101,     0,   182,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   173,     0,     0,    15,    16,
       0,    17,     0,   174,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   175,     0,     0,     0,
      32,   176,   177,     0,   178,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,   792,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   179,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   180,     0,    74,     0,
       0,    76,   279,   280,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   181,     0,   101,     0,   182,
       0,     5,   103,     6,     7,     8,   207,    10,    11,   208,
       0,     0,   173,     0,     0,    15,    16,     0,    17,     0,
     174,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   175,     0,     0,     0,    32,   176,   177,
       0,   178,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     209,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   180,     0,    74,     0,     0,    76,     0,
       0,   210,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   181,     0,   101,     0,   182,     0,     5,   103,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   173,
       0,     0,    15,    16,     0,    17,     0,   174,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     175,     0,     0,     0,    32,   176,   177,     0,   178,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,   212,    56,
       0,    58,     0,     0,     0,     0,    61,   179,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     180,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,   213,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   181,
       0,   101,     0,   182,     0,     0,   103,     5,   275,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   173,     0,
       0,    15,    16,     0,    17,     0,   174,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   175,
       0,     0,     0,    32,   176,   177,     0,   178,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   179,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   180,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
     213,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   181,     0,
     101,     0,   182,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   173,     0,     0,    15,    16,
       0,    17,     0,   174,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   175,     0,     0,     0,
      32,   176,   177,     0,   178,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   179,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   180,     0,    74,     0,
       0,    76,   279,   280,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   181,     0,   101,     0,   182,
       0,     0,   103,     5,   275,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   173,     0,     0,    15,    16,     0,
      17,     0,   174,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   175,     0,     0,     0,    32,
     176,   177,     0,   178,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   179,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   180,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   181,   672,   101,     0,   182,     0,
       0,   103,     5,   275,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   173,     0,     0,    15,    16,     0,    17,
       0,   174,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   175,     0,     0,     0,    32,   176,
     177,     0,   178,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   179,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   180,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   181,     0,   101,     0,   182,   682,     0,
     103,     5,   275,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   173,     0,     0,    15,    16,     0,    17,     0,
     174,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   175,     0,     0,     0,    32,   176,   177,
       0,   178,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     179,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   180,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   181,     0,   101,     0,   182,  1059,     5,   103,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   173,
       0,     0,    15,    16,     0,    17,     0,   174,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     175,     0,     0,     0,    32,   176,   177,     0,   178,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   179,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     180,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,   213,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   181,
       0,   101,     0,   182,     0,     5,   103,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   173,     0,     0,    15,
      16,     0,    17,     0,   174,     0,     0,    21,     0,     0,
       0,     0,     0,     0,   255,    28,     0,   175,     0,     0,
       0,    32,   176,   177,     0,   178,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   179,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   180,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   100,     0,   101,     0,
     182,     0,     0,   103,     5,   275,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   173,     0,     0,    15,    16,
       0,    17,     0,   174,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   175,     0,     0,     0,
      32,   176,   177,     0,   178,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   179,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   180,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   181,     0,   101,     0,   182,
       0,     5,   103,     6,     7,     8,   207,    10,    11,     0,
       0,     0,   173,     0,     0,    15,    16,     0,    17,     0,
     174,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   175,     0,     0,     0,    32,   176,   177,
       0,   178,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     209,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   180,     0,    74,     0,     0,    76,     0,
       0,   210,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   181,     0,   101,     0,   182,     0,     5,   103,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   173,
       0,     0,    15,    16,     0,    17,     0,   174,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     175,     0,     0,     0,    32,   176,   177,     0,   178,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   179,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     180,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   181,
       0,   101,     0,   182,   687,     5,   103,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   173,     0,     0,    15,
      16,     0,    17,     0,   174,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   175,     0,     0,
       0,    32,   176,   177,     0,   178,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   179,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   180,     0,    74,
       0,     0,    76,     0,     0,     0,    80,   818,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   181,     0,   101,     0,
     182,     0,     0,   103,     5,   275,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   173,     0,     0,    15,    16,
       0,    17,     0,   174,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   646,     0,   175,     0,     0,     0,
      32,   176,   177,     0,   178,    37,     0,     0,     0,   647,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   179,     0,     0,     0,     0,     0,     0,
       0,    68,   648,    70,    71,    72,   649,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   181,     0,   101,     0,   999,
       0,     5,   103,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   173,     0,     0,    15,    16,     0,    17,     0,
     174,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   175,     0,     0,     0,    32,   176,   177,
       0,   178,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     179,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   180,     0,    74,     0,     0,    76,     0,
       0,     0,    80,  1024,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   181,     0,   101,     0,   182,     0,     0,   103,
       5,   275,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   173,     0,     0,    15,    16,     0,    17,     0,   174,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   175,     0,     0,     0,    32,   176,   177,     0,
     178,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   179,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   180,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   181,     0,   101,     0,   999,     0,     0,   103,     5,
     275,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     173,     0,     0,    15,    16,     0,    17,     0,   174,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   175,     0,     0,     0,    32,   176,   177,     0,   981,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   179,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   180,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     181,     0,   101,     0,   182,     0,     5,   103,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   173,     0,     0,
      15,    16,     0,    17,     0,   174,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   175,     0,
       0,     0,    32,   176,   177,     0,   178,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   179,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   180,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   181,     0,   101,
       0,   182,     0,     5,   103,     6,     7,     8,   203,    10,
      11,     0,     0,     0,   173,     0,     0,    15,    16,     0,
      17,     0,   174,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   175,     0,     0,     0,    32,
     176,   177,     0,   178,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   204,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   180,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   181,     0,   101,     0,   182,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   173,     0,     0,    15,    16,     0,    17,     0,   174,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   175,     0,     0,     0,    32,   176,   177,     0,
     178,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   179,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   180,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   100,     0,   101,     0,   182,     0,     5,   103,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   173,     0,
       0,    15,    16,     0,    17,     0,   174,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   175,
       0,     0,     0,    32,   176,   177,     0,   178,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   179,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   180,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,   618,     0,   181,     0,
     101,     0,   182,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   173,     0,     0,    15,    16,
       0,    17,     0,   174,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   646,     0,   175,     0,     0,     0,
      32,   176,   177,     0,   178,    37,     0,     0,     0,   647,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   179,     0,     0,     0,     0,     0,     0,
       0,    68,   648,    70,    71,    72,   649,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   181,     0,   101,     0,   650,
       0,     5,   103,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   173,     0,     0,    15,    16,     0,    17,     0,
     174,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   175,     0,     0,     0,    32,   176,   177,
       0,   178,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     179,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   180,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   181,     0,   101,     0,   650,     0,     5,   103,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   173,
       0,     0,    15,    16,     0,    17,     0,   174,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     175,     0,     0,     0,    32,   176,   177,     0,   729,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   179,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     180,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   181,
       0,   101,     0,   182,     0,     5,   103,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   173,     0,     0,    15,
      16,     0,    17,     0,   174,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   175,     0,     0,
       0,    32,   176,   177,     0,   732,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   179,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   180,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   181,     0,   101,     0,
     182,     0,     5,   103,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   173,     0,     0,    15,    16,     0,    17,
       0,   174,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   175,     0,     0,     0,    32,   176,
     177,     0,   975,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   179,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   180,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   181,     0,   101,     0,   182,     0,     5,
     103,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     173,     0,     0,    15,    16,     0,    17,     0,   174,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   175,     0,     0,     0,    32,   176,   177,     0,   976,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   179,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   180,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     181,     0,   101,     0,   182,     0,     5,   103,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   173,     0,     0,
      15,    16,     0,    17,     0,   174,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   175,     0,
       0,     0,    32,   176,   177,     0,   978,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   179,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   180,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   181,     0,   101,
       0,   182,     0,     5,   103,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   173,     0,     0,    15,    16,     0,
      17,     0,   174,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   175,     0,     0,     0,    32,
     176,   177,     0,   979,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   179,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   180,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   181,     0,   101,     0,   182,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   173,     0,     0,    15,    16,     0,    17,     0,   174,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   175,     0,     0,     0,    32,   176,   177,     0,
     980,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   179,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   180,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   181,     0,   101,     0,   182,     0,     5,   103,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   173,     0,
       0,    15,    16,     0,    17,     0,   174,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   175,
       0,     0,     0,    32,   176,   177,     0,   981,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   179,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   180,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   181,     0,
     101,     0,   182,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   173,     0,     0,    15,    16,
       0,    17,     0,   174,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   646,     0,   175,     0,     0,     0,
      32,   176,   177,     0,   178,    37,     0,     0,     0,   647,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   179,     0,     0,     0,     0,     0,     0,
       0,    68,   648,    70,    71,    72,   649,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   181,     0,   101,     0,  1083,
       0,     5,   103,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   173,     0,     0,    15,    16,     0,    17,     0,
     174,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   175,     0,     0,     0,    32,   176,   177,
       0,   178,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     179,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   180,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   830,
       0,   831,    88,    89,    90,    91,    92,     0,     0,     0,
     832,    93,     0,     0,     0,   833,   226,   834,   835,     0,
       0,    94,     0,   836,    95,    96,    97,     0,     0,     0,
      99,   229,     0,   175,   101,     0,  1083,   837,     0,   103,
       0,     0,   838,     0,     0,     0,   232,     0,     0,   839,
       0,   840,     0,     0,     0,     0,     0,   841,     0,     0,
       0,     0,     0,   842,   843,     0,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,   844,
       0,     0,     0,     0,     0,     0,     0,     0,   239,   240,
       0,   845,     0,   242,     0,   846,     0,     0,   847,     0,
       0,     0,   848,     0,     0,   246,     0,     0,   849,     0,
       0,     0,     0,     0,     0,     0,     0,   444,   375,   376,
     377,   378,   379,     0,     0,   382,   383,   384,   385,     0,
     387,   388,   850,   851,   852,   853,   854,     0,     0,   855,
       0,     0,     0,   856,   857,   858,   859,   860,   861,   862,
     863,   864,   865,   866,     0,   867,     0,     0,   868,   869,
     870,   871,     0,     5,   872,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   173,     0,     0,    15,    16,     0,
      17,     0,   174,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   175,     0,     0,     0,    32,
     176,   177,     0,   178,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   179,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   180,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,   389,    96,    97,  -564,
       0,  -564,    99,     0,   181,     0,   101,     0,   182,     5,
     275,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     173,     0,     0,    15,    16,     0,    17,     0,   174,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   175,     0,     0,     0,    32,   176,   177,     0,   178,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   179,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   180,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,     0,  -423,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,     0,  -423,     0,     0,     0,
    -423,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
     181,     0,   101,  -423,   999,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   173,     0,     0,    15,
      16,     0,    17,     0,   174,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   175,     0,     0,
       0,    32,   176,   177,     0,   178,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,  -405,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   179,     0,     0,     0,  -405,     0,
       0,     0,    68,    69,    70,    71,    72,   180,     0,    74,
       0,  -405,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,  -405,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,    90,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   173,     0,
       0,    15,    16,     0,    17,     0,   174,     0,     0,    21,
      97,  -405,     0,     0,     0,     0,  -405,    28,   101,   175,
    1083,     0,     0,    32,   176,   177,     0,   178,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   179,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   180,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     5,
      90,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     173,     0,     0,    15,    16,     0,    17,     0,   174,     0,
       0,    21,    97,     0,     0,     0,     0,     0,   181,    28,
     101,   175,   182,     0,     0,    32,   176,   177,     0,   178,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   179,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   180,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     5,    90,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   173,     0,     0,    15,    16,     0,    17,     0,
     174,     0,     0,    21,    97,     0,     0,     0,     0,     0,
     181,    28,   101,   175,   650,     0,     0,    32,   176,   177,
       0,   178,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     179,     0,   224,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   180,     0,    74,   225,   226,    76,   227,
       0,     0,    80,     0,   228,    82,     0,     0,    84,     0,
       0,     0,   229,     0,     0,     0,     0,     0,   230,     0,
       0,     0,     0,   231,     0,     0,     0,   232,     0,     0,
     233,     0,  1036,     0,    90,     0,     0,     0,   234,     0,
       0,   832,     0,     0,   235,   236,   225,   226,   834,   227,
       0,   237,     0,     0,   228,     0,     0,     0,     0,     0,
     238,     0,   229,     0,   101,     0,   182,     0,   837,   239,
     240,     0,   241,   231,   242,     0,   243,   232,     0,   244,
     233,     0,   840,   245,     0,     0,   246,     0,   234,   247,
       0,     0,     0,     0,   842,   236,     0,     0,     0,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,     0,     0,     0,     0,     0,     0,   239,
     240,     0,   241,   344,   242,     0,  1037,     0,     0,   847,
     345,     0,     0,   245,     0,     0,   246,     0,     0,   247,
       0,     0,   346,     0,     0,   780,     0,     0,   444,   375,
     376,   377,   378,   379,     0,     0,   382,   383,   384,   385,
       0,   387,   388,   850,   851,   852,   853,   854,     0,     0,
     855,     0,     0,     0,   856,   857,   858,   859,   860,   861,
     862,   863,   864,   865,   866,     0,   867,     0,     0,   868,
     869,   870,   871,     0,   347,     0,     0,     0,   348,   344,
       0,     0,     0,     0,     0,     0,   345,   443,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   346,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   349,
     444,   375,   376,   377,   378,   379,     0,     0,   382,   383,
     384,   385,     0,   387,   388,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,     0,   358,   359,   360,   361,
     362,     0,   363,   364,   365,   366,   367,   368,   369,     0,
     347,   370,   371,   372,   348,   402,     0,     0,     0,   344,
       0,     0,   373,   900,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   346,     0,
       0,     0,     0,     0,     0,   349,   444,   375,   376,   377,
     378,   379,     0,     0,   382,   383,   384,   385,     0,   387,
     388,   350,   351,     0,   352,   353,   354,     0,   355,   356,
     357,     0,   358,   359,   360,   361,   362,     0,   363,   364,
     365,   366,   367,   368,   369,     0,     0,   370,   371,   372,
     347,   402,     0,     0,   348,   344,     0,     0,   373,     0,
       0,     0,   345,   907,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   349,   444,   375,   376,   377,
     378,   379,     0,     0,   382,   383,   384,   385,     0,   387,
     388,   350,   351,     0,   352,   353,   354,     0,   355,   356,
     357,     0,   358,   359,   360,   361,   362,     0,   363,   364,
     365,   366,   367,   368,   369,     0,   347,   370,   371,   372,
     348,   402,     0,     0,     0,   344,     0,     0,   373,  1050,
       0,     0,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,     0,     0,     0,     0,     0,
       0,   349,   444,   375,   376,   377,   378,   379,     0,     0,
     382,   383,   384,   385,     0,   387,   388,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,     0,   358,   359,
     360,   361,   362,     0,   363,   364,   365,   366,   367,   368,
     369,     0,     0,   370,   371,   372,   347,   402,     0,     0,
     348,   344,     0,     0,   373,     0,     0,     0,   345,  1051,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     346,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   349,   444,   375,   376,   377,   378,   379,     0,     0,
     382,   383,   384,   385,     0,   387,   388,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,     0,   358,   359,
     360,   361,   362,     0,   363,   364,   365,   366,   367,   368,
     369,     0,   347,   370,   371,   372,   348,   402,     0,     0,
       0,   344,     0,     0,   373,  1052,     0,     0,   345,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     346,     0,     0,     0,     0,     0,     0,   349,   444,   375,
     376,   377,   378,   379,     0,     0,   382,   383,   384,   385,
       0,   387,   388,   350,   351,     0,   352,   353,   354,     0,
     355,   356,   357,     0,   358,   359,   360,   361,   362,     0,
     363,   364,   365,   366,   367,   368,   369,     0,     0,   370,
     371,   372,   347,   402,     0,     0,   348,   344,     0,     0,
     373,     0,     0,     0,   345,  1053,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   349,   444,   375,
     376,   377,   378,   379,     0,     0,   382,   383,   384,   385,
       0,   387,   388,   350,   351,     0,   352,   353,   354,     0,
     355,   356,   357,     0,   358,   359,   360,   361,   362,     0,
     363,   364,   365,   366,   367,   368,   369,     0,   347,   370,
     371,   372,   348,   402,     0,     0,     0,   344,     0,     0,
     373,  1077,     0,     0,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   346,     0,     0,     0,
       0,     0,     0,   349,   444,   375,   376,   377,   378,   379,
       0,     0,   382,   383,   384,   385,     0,   387,   388,   350,
     351,     0,   352,   353,   354,     0,   355,   356,   357,     0,
     358,   359,   360,   361,   362,     0,   363,   364,   365,   366,
     367,   368,   369,     0,     0,   370,   371,   372,   347,   402,
       0,     0,   348,     0,     0,     0,   373,   344,     0,     0,
       0,  1078,     0,     0,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   346,   436,     0,     0,
       0,     0,     0,   349,   444,   375,   376,   377,   378,   379,
       0,   437,   382,   383,   384,   385,     0,   387,   388,   350,
     351,     0,   352,   353,   354,     0,   355,   356,   357,     0,
     358,   359,   360,   361,   362,     0,   363,   364,   365,   366,
     367,   368,   369,     0,     0,   370,   371,   372,   347,   402,
       0,     0,   348,   344,     0,     0,   373,     0,     0,     0,
     345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   346,   430,     0,     0,     0,     0,     0,   433,
       0,     0,     0,   349,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   350,
     351,     0,   352,   353,   354,     0,   355,   356,   357,     0,
     358,   359,   360,   361,   362,     0,   363,   364,   365,   366,
     367,   368,   369,     0,   347,   370,   371,   372,   348,   402,
       0,     0,     0,     0,     0,     0,   373,     0,     0,     0,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
     345,     0,     0,     0,     0,     0,     0,     0,     0,   349,
       0,     0,   346,     0,     0,     0,     0,   440,     0,     0,
       0,     0,     0,     0,     0,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,     0,   358,   359,   360,   361,
     362,     0,   363,   364,   365,   366,   367,   368,   369,   441,
       0,   370,   371,   372,     0,   402,     0,     0,     0,     0,
       0,     0,   373,     0,   347,     0,     0,     0,   348,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   344,   698,     0,     0,     0,     0,     0,
     345,     0,     0,     0,     0,     0,     0,     0,     0,   349,
       0,     0,   346,     0,     0,     0,     0,   699,     0,     0,
       0,     0,     0,     0,     0,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,     0,   358,   359,   360,   361,
     362,     0,   363,   364,   365,   366,   367,   368,   369,   700,
     442,   370,   371,   372,     0,     0,     0,     0,     0,     0,
       0,     0,   373,     0,   347,     0,     0,     0,   348,   344,
       0,     0,     0,     0,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   346,   762,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   349,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,     0,   358,   359,   360,   361,
     362,     0,   363,   364,   365,   366,   367,   368,   369,     0,
     347,   370,   371,   372,   348,     0,     0,     0,     0,   344,
       0,     0,   373,     0,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   346,   255,
       0,   433,     0,     0,     0,   349,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   350,   351,     0,   352,   353,   354,     0,   355,   356,
     357,     0,   358,   359,   360,   361,   362,     0,   363,   364,
     365,   366,   367,   368,   369,     0,     0,   370,   371,   372,
     347,   402,     0,     0,   348,   344,     0,     0,   373,     0,
       0,     0,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   581,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   349,     0,     0,     0,   582,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   350,   351,     0,   352,   353,   354,     0,   355,   356,
     357,     0,   358,   359,   360,   361,   362,     0,   363,   364,
     365,   366,   367,   368,   369,     0,   347,   370,   371,   372,
     348,   402,     0,     0,     0,   344,     0,     0,   373,     0,
       0,     0,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   583,     0,     0,     0,     0,
       0,   349,     0,     0,     0,     0,     0,     0,     0,   584,
       0,     0,     0,     0,     0,     0,     0,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,     0,   358,   359,
     360,   361,   362,     0,   363,   364,   365,   366,   367,   368,
     369,     0,     0,   370,   371,   372,   347,     0,     0,     0,
     348,   344,     0,     0,   373,     0,     0,     0,   345,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     346,   754,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   349,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,     0,   358,   359,
     360,   361,   362,     0,   363,   364,   365,   366,   367,   368,
     369,     0,   347,   370,   371,   372,   348,     0,     0,     0,
       0,   344,     0,     0,   373,     0,     0,     0,   345,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     346,     0,     0,     0,     0,     0,     0,   349,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   350,   351,     0,   352,   353,   354,     0,
     355,   356,   357,     0,   358,   359,   360,   361,   362,     0,
     363,   364,   365,   366,   367,   368,   369,     0,     0,   370,
     371,   372,   347,   402,     0,     0,   348,   344,     0,     0,
     373,     0,     0,     0,   345,   772,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   349,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   350,   351,     0,   352,   353,   354,     0,
     355,   356,   357,     0,   358,   359,   360,   361,   362,     0,
     363,   364,   365,   366,   367,   368,   369,     0,   347,   370,
     371,   372,   348,   402,     0,     0,     0,     0,     0,     0,
     373,     0,     0,     0,     0,     0,     0,   344,     0,     0,
       0,     0,     0,     0,   345,     0,     0,     0,     0,   433,
       0,     0,     0,   349,     0,     0,   346,     0,     0,     0,
       0,   953,     0,     0,     0,     0,     0,     0,     0,   350,
     351,     0,   352,   353,   354,     0,   355,   356,   357,     0,
     358,   359,   360,   361,   362,     0,   363,   364,   365,   366,
     367,   368,   369,   954,   344,   370,   371,   372,     0,     0,
       0,   345,     0,     0,   810,     0,   373,     0,   347,     0,
       0,     0,   348,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   344,     0,     0,     0,     0,     0,
       0,   345,     0,   349,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   346,     0,     0,     0,     0,     0,   350,
     351,     0,   352,   353,   354,   347,   355,   356,   357,   348,
     358,   359,   360,   361,   362,     0,   363,   364,   365,   366,
     367,   368,   369,   344,     0,   370,   371,   372,     0,     0,
     345,     0,     0,     0,     0,     0,   373,     0,     0,     0,
     349,     0,   346,     0,     0,   347,     0,     0,     0,   348,
       0,     0,     0,     0,     0,     0,   350,   351,     0,   352,
     353,   354,     0,   355,   356,   357,     0,   358,   359,   360,
     361,   362,     0,   363,   364,   365,   366,   367,   368,   369,
     349,     0,   370,   371,   372,     0,   476,     0,     0,     0,
       0,     0,     0,   373,   347,     0,   350,   351,   348,   352,
     353,   354,     0,   355,   356,   357,     0,   358,   359,   360,
     361,   362,     0,   363,   364,   365,   366,   367,   368,   369,
     344,   480,   370,   371,   372,     0,     0,   345,     0,   349,
       0,     0,     0,   373,     0,     0,     0,     0,     0,   346,
       0,     0,     0,     0,     0,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,     0,   358,   359,   360,   361,
     362,     0,   363,   364,   365,   366,   367,   368,   369,   344,
     482,   370,   371,   372,     0,     0,   345,     0,     0,     0,
       0,     0,   373,     0,     0,     0,     0,     0,   346,     0,
       0,   347,     0,     0,     0,   348,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,     0,     0,     0,     0,
       0,     0,   345,     0,     0,     0,   349,     0,     0,     0,
       0,     0,     0,     0,   346,     0,     0,     0,     0,     0,
     347,     0,   350,   351,   348,   352,   353,   354,     0,   355,
     356,   357,     0,   358,   359,   360,   361,   362,     0,   363,
     364,   365,   366,   367,   368,   369,     0,   484,   370,   371,
     372,     0,     0,     0,     0,   349,     0,     0,     0,   373,
       0,     0,     0,     0,     0,     0,   347,     0,     0,     0,
     348,   350,   351,     0,   352,   353,   354,     0,   355,   356,
     357,     0,   358,   359,   360,   361,   362,     0,   363,   364,
     365,   366,   367,   368,   369,   344,   489,   370,   371,   372,
     506,   349,   345,     0,     0,     0,     0,     0,   373,     0,
       0,     0,     0,     0,   346,     0,     0,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,     0,   358,   359,
     360,   361,   362,     0,   363,   364,   365,   366,   367,   368,
     369,   344,     0,   370,   371,   372,     0,     0,   345,     0,
       0,     0,     0,     0,   373,     0,     0,     0,     0,     0,
     346,     0,     0,     0,     0,     0,   347,     0,     0,     0,
     348,     0,     0,     0,     0,     0,     0,     0,     0,   585,
       0,   344,   694,     0,     0,     0,     0,     0,   345,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     346,   349,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   347,     0,     0,     0,   348,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,     0,   358,   359,
     360,   361,   362,     0,   363,   364,   365,   366,   367,   368,
     369,     0,     0,   370,   371,   372,     0,   349,     0,     0,
       0,     0,   347,     0,   373,     0,   348,     0,     0,     0,
       0,     0,     0,   350,   351,     0,   352,   353,   354,     0,
     355,   356,   357,     0,   358,   359,   360,   361,   362,     0,
     363,   364,   365,   366,   367,   368,   369,   349,     0,   370,
     371,   372,     0,     0,   344,     0,   673,     0,     0,     0,
     373,   345,     0,   350,   351,     0,   352,   353,   354,     0,
     355,   695,   357,   346,   358,   359,   360,   361,   362,     0,
     363,   364,   365,   366,   367,   368,   369,     0,     0,   370,
     371,   372,     0,     0,     0,     0,     0,     0,     0,   344,
     373,     0,     0,     0,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   346,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,   348,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   344,
       0,     0,     0,     0,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   346,     0,
     349,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,     0,   348,     0,   350,   351,     0,   352,
     353,   354,     0,   355,   356,   357,     0,   358,   359,   360,
     361,   362,  -527,   363,   364,   365,   366,   367,   368,   369,
       0,   168,   370,   371,   372,   349,     0,     0,     0,     0,
     347,     0,     0,   373,   348,     0,     0,     0,     0,     0,
       0,   350,   351,     0,   352,   353,   354,     0,   355,   356,
     357,     0,   358,   359,   360,   361,   362,     0,   363,   364,
     365,   366,   367,   368,   369,   349,     0,   370,   371,   372,
       0,     0,   344,   767,     0,     0,     0,     0,   373,   345,
       0,   350,   351,     0,   352,   353,   354,     0,   355,   356,
     357,   346,   358,   359,   360,   361,   362,     0,   363,   364,
     365,   366,   367,   368,   369,     0,   739,   370,   371,   372,
       0,     0,   344,     0,     0,     0,     0,     0,   373,   345,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   346,     0,     0,   770,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,   348,     0,     0,
       0,     0,   344,     0,     0,     0,     0,     0,     0,   345,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   346,     0,     0,     0,     0,     0,     0,   349,     0,
       0,     0,     0,   347,     0,     0,     0,   348,     0,     0,
       0,     0,     0,     0,   350,   351,     0,   352,   353,   354,
       0,   355,   356,   357,     0,   358,   359,   360,   361,   362,
       0,   363,   364,   365,   366,   367,   368,   369,   349,     0,
     370,   371,   372,   347,     0,     0,     0,   348,     0,     0,
       0,   373,     0,     0,   350,   351,     0,   352,   353,   354,
       0,   355,   356,   357,     0,   358,   359,   360,   361,   362,
       0,   363,   364,   365,   366,   367,   368,   369,   349,     0,
     370,   371,   372,     0,     0,   344,     0,     0,     0,     0,
       0,   373,   345,     0,   350,   351,     0,   352,   353,   354,
       0,   355,   356,   357,   346,   358,   359,   360,   361,   362,
       0,   363,   364,   365,   366,   367,   368,   369,     0,     0,
     370,   371,   372,     0,     0,     0,   344,   794,     0,     0,
       0,   373,     0,   345,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   347,     0,     0,     0,
     348,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,     0,   345,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   346,   887,     0,     0,     0,
       0,   349,     0,     0,     0,     0,     0,   347,     0,     0,
       0,   348,     0,     0,     0,     0,     0,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,     0,   358,   359,
     360,   361,   362,     0,   363,   364,   365,   366,   367,   368,
     369,   813,   349,   370,   371,   372,     0,   347,     0,     0,
     795,   348,     0,     0,   373,     0,     0,     0,   350,   351,
       0,   352,   353,   354,     0,   355,   356,   357,     0,   358,
     359,   360,   361,   362,     0,   363,   364,   365,   366,   367,
     368,   369,   349,     0,   370,   371,   372,     0,     0,     0,
       0,     0,     0,     0,     0,   373,     0,     0,   350,   351,
       0,   352,   353,   354,     0,   355,   356,   357,     0,   358,
     359,   360,   361,   362,     0,   363,   364,   365,   366,   367,
     368,   369,   344,  -240,   370,   371,   372,     0,     0,   345,
       0,     0,     0,     0,     0,   373,     0,  -240,  -240,     0,
    -240,   346,   890,     0,     0,  -240,     0,     0,     0,     0,
       0,     0,     0,  -240,     0,     0,     0,     0,     0,  -240,
       0,     0,   344,     0,  -240,     0,     0,     0,  -240,   345,
       0,  -240,     0,     0,     0,     0,     0,     0,     0,  -240,
       0,   346,     0,     0,     0,  -240,  -240,     0,     0,     0,
       0,     0,  -240,   347,     0,     0,     0,   348,     0,     0,
       0,  -240,   344,   950,     0,     0,     0,     0,     0,   345,
    -240,  -240,     0,  -240,     0,  -240,     0,  -240,     0,     0,
    -240,   346,     0,     0,  -240,     0,     0,  -240,   349,     0,
    -240,     0,     0,   347,     0,     0,     0,   348,     0,     0,
       0,     0,     0,     0,   350,   351,     0,   352,   353,   354,
       0,   355,   356,   357,     0,   358,   359,   360,   361,   362,
       0,   363,   364,   365,   366,   367,   368,   369,   349,     0,
     370,   371,   372,   347,     0,     0,     0,   348,     0,     0,
       0,   373,     0,     0,   350,   351,     0,   352,   353,   354,
       0,   355,   356,   357,     0,   358,   359,   360,   361,   362,
       0,   363,   364,   365,   366,   367,   368,   369,   349,     0,
     370,   371,   372,     0,     0,   344,     0,     0,     0,   892,
       0,   373,   345,     0,   350,   351,     0,   352,   353,   354,
       0,   355,   356,   357,   346,   358,   359,   360,   361,   362,
       0,   363,   364,   365,   366,   367,   368,   369,     0,     0,
     370,   371,   372,     0,     0,   344,     0,     0,     0,     0,
       0,   373,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   347,     0,     0,     0,
     348,     0,     0,     0,     0,   344,   993,     0,     0,   974,
       0,     0,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,     0,     0,     0,     0,     0,
       0,   349,     0,     0,     0,     0,   347,     0,     0,     0,
     348,     0,     0,     0,     0,     0,     0,   350,   351,   977,
     352,   353,   354,     0,   355,   356,   357,     0,   358,   359,
     360,   361,   362,     0,   363,   364,   365,   366,   367,   368,
     369,   349,     0,   370,   371,   372,   347,     0,     0,     0,
     348,     0,     0,     0,   373,     0,     0,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,     0,   358,   359,
     360,   361,   362,     0,   363,   364,   365,   366,   367,   368,
     369,   349,     0,   370,   371,   372,     0,     0,   344,     0,
       0,     0,     0,     0,   373,   345,     0,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   346,   358,   359,
     360,   361,   362,     0,   363,   364,   365,   366,   367,   368,
     369,     0,     0,   370,   371,   372,     0,     0,   344,     0,
       0,     0,     0,     0,   373,   345,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,   348,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   349,     0,     0,     0,     0,   347,
       0,     0,     0,   348,     0,     0,     0,     0,     0,     0,
     350,   351,     0,   352,   353,   354,     0,   355,   356,   357,
       0,   358,   359,   360,   361,   362,     0,   363,   364,   365,
     366,   367,   368,   369,   349,     0,   370,   371,   372,     0,
       0,   344,     0,  1035,     0,     0,     0,   373,   345,     0,
     350,   351,     0,   352,   353,   354,     0,   355,   356,   357,
     346,   358,   359,   360,   361,   362,     0,   363,   364,   365,
     366,   367,   368,   369,     0,     0,   370,   371,   372,     0,
       0,   344,     0,     0,     0,  1073,     0,   373,   345,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     346,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   347,     0,     0,     0,   348,     0,     0,     0,
       0,   344,     0,     0,     0,  1092,     0,     0,   345,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     346,     0,     0,     0,     0,     0,     0,   349,     0,     0,
       0,     0,   347,     0,     0,     0,   348,     0,     0,     0,
       0,     0,     0,   350,   351,  1093,   352,   353,   354,     0,
     355,   356,   357,     0,   358,   359,   360,   361,   362,     0,
     363,   364,   365,   366,   367,   368,   369,   349,     0,   370,
     371,   372,   347,     0,     0,     0,   348,     0,     0,     0,
     373,     0,     0,   350,   351,  1094,   352,   353,   354,     0,
     355,   356,   357,     0,   358,   359,   360,   361,   362,     0,
     363,   364,   365,   366,   367,   368,   369,   349,     0,   370,
     371,   372,     0,     0,   344,     0,     0,     0,     0,     0,
     373,   345,     0,   350,   351,     0,   352,   353,   354,     0,
     355,   356,   357,   346,   358,   359,   360,   361,   362,     0,
     363,   364,   365,   366,   367,   368,   369,     0,     0,   370,
     371,   372,     0,     0,   344,     0,     0,     0,     0,     0,
     373,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,   348,
       0,     0,     0,     0,   344,     0,     0,     0,  1095,     0,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   346,     0,     0,     0,     0,     0,     0,
     349,     0,     0,     0,     0,   347,     0,     0,     0,   348,
       0,     0,     0,     0,     0,     0,   350,   351,  1096,   352,
     353,   354,     0,   355,   356,   357,     0,   358,   359,   360,
     361,   362,     0,   363,   364,   365,   366,   367,   368,   369,
     349,     0,   370,   371,   372,   347,     0,     0,     0,   348,
       0,     0,     0,   373,     0,     0,   350,   351,  1097,   352,
     353,   354,     0,   355,   356,   357,     0,   358,   359,   360,
     361,   362,     0,   363,   364,   365,   366,   367,   368,   369,
     349,     0,   370,   371,   372,     0,     0,   344,     0,     0,
       0,     0,     0,   373,   345,     0,   350,   351,     0,   352,
     353,   354,     0,   355,   356,   357,   346,   358,   359,   360,
     361,   362,     0,   363,   364,   365,   366,   367,   368,   369,
       0,     0,   370,   371,   372,     0,     0,   344,     0,     0,
       0,     0,  1121,   373,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,     0,
       0,     0,   348,     0,     0,     0,     0,   344,     0,     0,
       0,     0,     0,     0,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   346,     0,     0,     0,
       0,     0,     0,   349,     0,     0,     0,     0,   347,     0,
       0,     0,   348,     0,     0,     0,     0,     0,     0,   350,
     351,     0,   352,   353,   354,     0,   355,   356,   357,     0,
     358,   359,   360,   361,   362,     0,   363,   364,   365,   366,
     367,   368,   369,   349,     0,   370,   371,   372,   347,     0,
       0,     0,   348,     0,     0,     0,   373,     0,     0,   350,
     351,     0,   352,   353,   354,     0,   355,   356,   357,     0,
     358,   359,   360,   361,   362,     0,   363,   364,   365,   366,
     367,   368,   369,   349,     0,   370,   371,   372,     0,     0,
     344,     0,     0,     0,  1150,     0,   373,   345,     0,   350,
     351,     0,   352,   353,   354,     0,   355,   356,   357,   346,
     358,   359,   360,   361,   362,     0,   363,   364,   365,   366,
     367,   368,   369,     0,   270,   370,   371,   372,    22,    23,
       0,     0,     0,     0,     0,     0,   373,   271,     0,    30,
     272,     0,     0,     0,    35,     0,     0,     0,     0,     0,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   727,     0,     0,     0,   348,     0,     0,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,   958,
       0,     0,   959,     0,   273,     0,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   349,     0,     0,     0,
       0,     0,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,   350,   351,     0,   352,   353,   354,     0,   355,
     356,   357,   346,   358,   359,   360,   361,   362,     0,   363,
     364,   365,   366,   367,   368,   369,     0,   270,   370,   371,
     372,    22,    23,     0,     0,     0,     0,     0,     0,   373,
     271,     0,    30,   272,     0,     0,     0,    35,     0,     0,
       0,    98,     0,     0,    40,     0,     0,  1098,     0,     0,
       0,     0,     0,     0,   347,     0,     0,     0,   348,   346,
       0,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,   958,     0,     0,   959,     0,   273,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   349,
       0,     0,     0,     0,     0,     0,     0,   346,    79,     0,
       0,    81,     0,     0,    83,   350,   351,     0,   352,   353,
     354,   347,   355,   356,   357,   348,   358,   359,   360,   361,
       0,     0,   363,   364,   365,   366,   367,   368,   369,     0,
       0,   370,   371,   372,     0,   346,     0,     0,     0,     0,
       0,     0,   373,     0,     0,     0,   349,     0,     0,   347,
       0,     0,     0,   348,    98,     0,     0,     0,     0,     0,
    1099,     0,   350,   351,     0,   352,   353,   354,     0,   355,
     356,   357,     0,   358,   359,   360,   361,     0,     0,   363,
     364,   365,   366,   367,     0,   369,     0,   347,   370,   371,
     372,   348,     0,     0,     0,     0,     0,     0,     0,   373,
     350,   351,     0,   352,   353,   354,     0,   355,   356,   357,
       0,   358,   359,   360,   361,     0,     0,   363,   364,   365,
     366,   367,     0,   369,     0,     0,   370,   371,   372,     0,
       0,     0,     0,     0,     0,     0,     0,   373,   350,   351,
       0,   352,   353,   354,     0,   355,   356,   357,     0,     0,
     359,   360,   361,     0,     0,   363,   364,   365,   366,     0,
     703,   369,   224,     0,   370,   371,   372,     0,     0,     0,
       0,     0,     0,     0,     0,   373,   225,   226,     0,   227,
       0,     0,     0,     0,   228,     0,     0,     0,     0,     0,
       0,     0,   229,     0,     0,     0,     0,     0,   230,     0,
       0,     0,     0,   231,     0,     0,     0,   232,     0,     0,
     233,     0,  -250,     0,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,   235,   236,  -250,  -250,     0,  -250,
       0,   237,     0,     0,  -250,     0,     0,     0,     0,     0,
     238,     0,  -250,     0,     0,     0,     0,     0,  -250,   239,
     240,     0,   241,  -250,   242,     0,   243,  -250,     0,   244,
    -250,     0,   224,   245,     0,     0,   246,     0,  -250,   247,
       0,     0,     0,     0,  -250,  -250,   225,   226,     0,   227,
       0,  -250,     0,     0,   228,     0,     0,     0,     0,     0,
    -250,     0,   229,     0,     0,     0,     0,     0,   230,  -250,
    -250,     0,  -250,   231,  -250,     0,  -250,   232,     0,  -250,
     233,     0,  -241,  -250,     0,     0,  -250,     0,   234,  -250,
       0,     0,     0,     0,   235,   236,  -241,  -241,     0,  -241,
       0,   237,     0,     0,  -241,     0,     0,     0,     0,     0,
     238,     0,  -241,     0,     0,     0,     0,     0,  -241,   239,
     240,     0,   241,  -241,   242,     0,   243,  -241,     0,   244,
    -241,     0,  -172,   245,     0,     0,   246,     0,  -241,   247,
       0,     0,     0,     0,  -241,  -241,  -172,  -172,     0,  -172,
       0,  -241,     0,     0,  -172,     0,     0,     0,     0,     0,
    -241,     0,  -172,     0,     0,     0,     0,     0,  -172,  -241,
    -241,     0,  -241,  -172,  -241,     0,  -241,  -172,     0,  -241,
    -172,     0,  -168,  -241,     0,     0,  -241,     0,  -172,  -241,
       0,     0,     0,     0,  -172,  -172,  -168,  -168,     0,  -168,
       0,  -172,     0,     0,  -168,     0,     0,     0,     0,     0,
    -172,     0,  -168,     0,     0,     0,     0,     0,  -168,  -172,
    -172,     0,  -172,  -168,  -172,     0,  -172,  -168,     0,  -172,
    -168,     0,     0,  -172,     0,     0,  -172,     0,  -168,  -172,
       0,     0,     0,     0,  -168,  -168,     0,     0,     0,     0,
       0,  -168,     0,     0,     0,     0,     0,     0,     0,     0,
    -168,     0,     0,     0,     0,     0,     0,     0,     0,  -168,
    -168,     0,  -168,     0,  -168,     0,  -168,     0,     0,  -168,
       0,     0,     0,  -168,     0,     0,  -168,     0,     0,  -168
};

static const yytype_int16 yycheck[] =
{
      12,   485,   153,    60,    16,   217,   455,   611,    20,    19,
     441,   789,   194,   202,    26,   139,   712,     2,    30,    31,
     823,    33,    34,    35,    36,   700,   877,   878,   210,   699,
       1,   645,  1009,    45,   223,    47,   395,     1,    22,     1,
      52,   924,    54,    86,    56,    64,   896,    86,   898,   452,
       3,    46,    64,    65,    66,    67,    68,    69,    46,    32,
      32,    73,   223,    75,  1111,    77,    78,    67,    46,    32,
      82,   195,  1059,    85,    86,     1,    88,   599,    90,   268,
     602,    93,    94,   102,    96,     3,    36,   140,   100,   101,
     102,   103,    46,    57,    32,    46,    32,    46,    69,   159,
       0,   290,    20,  1150,   882,  1128,   159,   268,   112,   114,
     322,   323,   726,    46,    46,    33,   159,    52,    68,   114,
     724,   164,    86,  1146,  1111,   164,   460,    33,    34,   290,
      84,    57,   982,    86,   984,    84,   140,   149,     3,     3,
     111,    59,   114,    78,    97,   140,   297,   925,   112,  1032,
       3,   485,   140,  1130,     1,    57,    84,   119,   961,   997,
      86,   173,   140,   182,   176,   177,   178,   356,   180,   181,
     182,   921,   786,    99,   169,   139,   114,   139,   140,    97,
     983,   169,   985,   193,    86,   103,   112,  1038,  1039,   140,
     202,   164,   164,    65,   165,   644,   102,    99,    69,    46,
     164,   164,   391,   392,   140,   167,   166,   140,   140,   166,
     112,   223,   269,   139,   214,   215,   216,   217,   169,   997,
    1058,   743,   206,   140,   746,    27,   164,   211,   164,   101,
     391,   392,    97,   159,   218,    45,   169,   169,   164,   140,
     111,   114,    27,  1046,    97,  1048,   164,   140,   998,    27,
     167,   134,   611,   114,    57,   134,   268,   159,   258,   142,
     272,    46,   164,   142,   264,    50,   167,   279,   280,   700,
       3,    73,   119,   496,   970,   498,   169,   952,   290,   954,
    1058,   951,    84,   953,   140,    70,   286,    20,    73,   478,
      75,    45,   139,   140,   165,    73,   169,   421,   108,    84,
      33,   905,   426,   140,   304,   317,    84,   109,   169,   112,
     159,   167,   159,    67,   206,     8,   165,   478,   103,   211,
     332,   333,   159,    77,    78,   103,    59,    46,    45,   314,
     342,   780,   344,   345,   346,   347,   348,   349,    57,   351,
     352,   353,   354,   355,    61,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,    79,   119,    97,   724,   166,    86,   380,   381,
     103,   101,    57,   140,   386,   778,    27,   389,   390,   391,
     392,   393,   394,   395,   140,   816,    94,   140,   144,   159,
     140,   108,   404,   112,    57,   165,   140,   409,   165,   411,
     119,    86,   134,   615,   616,   405,    77,    78,   597,   428,
     142,   167,   165,   164,    99,   159,   428,   167,   430,   431,
     823,   140,    73,    86,   436,   437,    83,   112,   440,   441,
      87,   443,   119,    84,   446,   435,    99,   140,   192,   439,
     159,   347,   348,   140,   166,   164,   140,   931,   167,   112,
     462,   140,   103,   140,   466,   467,   140,   144,    18,   471,
     214,   215,   216,   217,   167,    79,   478,   221,   680,   681,
     167,    46,   202,   167,   159,   158,    27,   119,   167,   164,
     167,   493,    57,   167,   496,   159,   498,   393,   394,   671,
     502,   503,   504,   223,   506,   507,   159,    31,   404,   140,
     519,   164,   144,   159,   258,   694,   695,   519,   380,   381,
     264,    86,    72,   159,   386,   140,     2,   389,   390,   150,
     159,   952,    73,   954,   165,   431,    12,   512,   166,   140,
      90,   437,   286,    84,   159,   140,   140,   112,   268,    25,
     140,    27,    77,   140,   119,    80,   905,   107,   159,    83,
     304,   159,   103,    87,   159,   159,   159,   159,   109,   159,
     290,   712,   159,   966,    83,   140,    42,   159,    87,   581,
     582,   583,   584,   585,   586,   587,   164,   159,   588,   589,
     983,   113,   985,   595,   159,   597,   134,    73,    31,   164,
     924,   139,   167,   605,   142,   607,   502,   931,   146,   611,
     134,   613,   166,    79,   168,   139,    27,   141,   142,   609,
     610,    27,   146,    82,   100,   615,   616,    82,  1064,   105,
     154,   166,   166,   159,   496,    46,   498,   166,   162,    50,
     166,   650,   168,    49,   159,   166,   648,   649,   650,   166,
      83,   168,   654,  1046,    87,  1048,   166,   119,   168,    70,
     166,   405,    73,   159,    75,    71,   132,    73,   134,     8,
      61,   391,   392,    84,   166,   166,   168,   168,    84,  1115,
    1116,  1117,  1118,  1119,  1120,   139,   582,    31,   584,   164,
     692,   435,   103,   169,   696,   439,   698,   699,   700,   142,
      64,   134,   159,   109,   119,   140,   139,   167,  1032,   142,
     159,    46,    34,   146,   166,   140,   167,   167,   119,   167,
     167,    21,   724,   140,   169,   727,   728,   729,   194,   140,
     732,   169,    31,   595,   164,  1059,   159,   166,   159,    83,
     159,   159,   169,    87,   210,   159,   212,   737,   738,  1073,
     752,   471,   754,   932,   933,   159,   167,   759,   478,   749,
     762,   159,   167,   765,   140,   767,   159,   159,   770,    34,
      61,   761,   248,   493,   764,   140,   496,   167,   498,   255,
     165,   932,   933,   503,    83,   251,   167,  1111,    87,   798,
     134,   135,   654,   137,   138,   139,   798,   141,   142,   143,
     119,    46,   146,   165,  1128,    84,   808,   167,   810,   153,
     154,   813,    57,   157,   816,    32,   160,   161,   162,    32,
     169,   164,  1146,   825,   164,    34,  1150,   171,   139,   970,
      84,   727,   728,   169,   166,   134,   135,   166,   314,   138,
     139,    86,   141,   142,   139,   169,   169,   146,   165,   159,
     159,   166,   142,   329,   142,   154,   142,   134,   142,   142,
     142,   160,   161,   162,   134,   609,   610,   112,   142,   142,
     872,   615,   616,   142,   119,   595,   134,   597,   142,   142,
     142,   134,   134,   627,   886,   887,   164,   889,   890,   891,
     892,    34,   164,   159,    99,   140,   167,   134,   900,   901,
     167,   903,   159,   905,   159,   907,   908,   167,   167,   911,
       2,   119,   959,   159,   159,   159,   134,   158,   158,   164,
      12,   169,   167,   667,   140,   692,   402,   329,   930,   931,
     932,   933,   298,    25,    24,    27,   938,   145,   940,   147,
     148,   449,   944,   151,   152,   802,   936,   826,   950,   951,
     952,   953,   954,   419,   430,   970,   966,   423,   778,   637,
     436,   829,  1130,  1088,  1121,  1114,    35,   443,   463,    30,
     597,    -1,   974,   975,   976,   977,   978,   979,   980,   981,
      -1,    73,    -1,    -1,    -1,    77,    78,    -1,    -1,   455,
     999,   993,    -1,   737,   738,    -1,    27,   999,    -1,    -1,
      -1,    -1,  1004,    -1,    -1,   749,  1008,   751,   100,    -1,
      -1,    31,    -1,   105,   758,    46,    -1,   761,    -1,    50,
     764,    -1,    -1,    -1,   500,    -1,    -1,    -1,   504,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   512,    -1,    -1,    70,
      -1,    -1,    73,    -1,    75,    -1,    -1,   513,  1050,  1051,
    1052,  1053,    -1,    84,    -1,   521,    -1,    -1,    -1,    -1,
      -1,   805,    -1,    83,    -1,    -1,    -1,    87,    -1,    -1,
      -1,  1073,   103,    -1,  1083,  1077,  1078,   169,    -1,    -1,
      -1,  1083,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1092,  1093,  1094,  1095,  1096,  1097,    -1,    -1,    -1,    -1,
      -1,    -1,   578,    -1,    -1,    -1,    -1,    -1,  1110,  1111,
      -1,  1113,    -1,     2,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,    12,    -1,    -1,   146,    -1,    -1,    -1,
      31,    -1,   598,   153,   154,   601,    25,   157,    27,    -1,
     160,   161,   162,    -1,  1146,    -1,    -1,    -1,  1150,    -1,
     626,    -1,    -1,    -1,    -1,    -1,   248,    -1,    -1,    -1,
      -1,    -1,    -1,   255,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   640,    -1,    -1,    -1,   644,    -1,
      -1,    -1,    83,    -1,    73,    -1,    87,    -1,    -1,    -1,
      -1,    -1,   936,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   667,    -1,   679,   924,   671,    -1,    -1,    -1,   685,
      -1,   100,   932,   933,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,   314,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,    -1,   712,   329,   139,    31,
     141,   142,    -1,   987,   988,   146,   712,   991,   992,    -1,
      -1,   727,   728,   154,    -1,    -1,    -1,    -1,    -1,   160,
     161,   162,    -1,    -1,    -1,  1009,    -1,    -1,    -1,     2,
      31,    -1,    -1,  1017,    -1,  1019,   752,  1021,   754,    12,
     169,    -1,    -1,   759,  1004,    -1,   762,    -1,    -1,    -1,
      -1,    83,    25,   769,    27,    87,   772,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   774,    -1,
     402,   777,    -1,    -1,   780,    -1,    -1,    -1,    -1,    -1,
      -1,   797,    83,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,   808,    -1,   810,    -1,   802,    -1,   430,  1059,
      73,    -1,   134,   135,   436,   137,   138,   139,    -1,   141,
     142,   443,    -1,    -1,   146,    -1,    -1,    -1,    -1,   248,
     826,    -1,   154,    -1,    -1,    -1,   255,   100,   160,   161,
     162,    -1,   105,   134,    -1,    -1,    -1,    -1,   139,    -1,
     141,   142,    -1,    -1,     2,   146,  1130,    -1,    -1,    -1,
      -1,  1111,    -1,   154,    12,    -1,    -1,    -1,    -1,   160,
     161,   162,    -1,    -1,    -1,    -1,    -1,    25,   500,    27,
      -1,    -1,   504,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     512,    -1,    -1,    -1,   900,   314,    -1,    -1,    -1,    -1,
    1150,   907,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,
     329,    -1,    -1,   919,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    73,    -1,    -1,   934,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   943,    -1,    -1,
      -1,   947,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,   100,    -1,    -1,    -1,    -1,   105,    35,    -1,
      37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    -1,   970,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   402,    -1,   248,    -1,    -1,   994,    -1,
      -1,    -1,   255,    -1,    71,    -1,    73,    -1,    75,    -1,
      77,    78,    -1,    80,   626,    82,    -1,    84,    -1,    -1,
      -1,   430,    -1,    -1,    -1,    -1,    -1,   436,    -1,    -1,
       3,   169,    -1,    -1,   443,    -1,   103,    -1,    -1,   106,
      -1,    -1,   109,    -1,    17,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,  1050,  1051,  1052,  1053,    -1,    -1,
      -1,   314,    35,    -1,    -1,    -1,    -1,   679,    -1,    -1,
      -1,    44,    -1,   685,    -1,    -1,   329,     2,    51,    -1,
      -1,  1077,  1078,    -1,    -1,    -1,    59,    12,    -1,    -1,
      -1,   500,   159,    66,    -1,   504,    -1,    -1,   165,    -1,
      25,    -1,    27,   512,    -1,    -1,    -1,    -1,    81,    -1,
     248,    -1,    -1,    -1,    -1,    -1,    -1,   255,    -1,    -1,
      93,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
     752,    -1,   754,    -1,    -1,    -1,    -1,   759,    73,   402,
     762,    -1,    -1,    -1,    22,    -1,    -1,   769,    26,    27,
     772,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,
      38,    -1,    -1,    -1,    42,   100,   314,   430,    -1,    -1,
     105,    49,    -1,   436,    -1,   797,    -1,    -1,    -1,    -1,
     443,   329,    -1,    -1,    -1,    -1,   808,    -1,   810,    -1,
      -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,    77,
      -1,    -1,    80,    -1,    82,    -1,    84,   626,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,   169,    -1,    -1,   500,    -1,   150,
      -1,   504,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   512,
      -1,    -1,   163,    -1,   402,   166,    -1,   168,    -1,    -1,
     679,    -1,    -1,    -1,    -1,    -1,   685,     2,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,   900,    -1,
      -1,   159,   430,     2,    -1,   907,    -1,   165,   436,    -1,
      25,    -1,    27,    12,    -1,   443,    -1,   919,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,   934,   248,    -1,    -1,    -1,    -1,    -1,    -1,
     255,   943,    -1,    -1,    -1,   947,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   752,    -1,   754,    -1,    -1,    73,    -1,
     759,    -1,    -1,   762,    -1,    -1,    -1,    -1,    -1,    -1,
     769,    -1,   500,   772,    73,    -1,   504,    -1,    -1,    -1,
      -1,    -1,    -1,   626,   512,   100,    -1,    -1,    -1,    -1,
     105,    -1,   994,    -1,    -1,    -1,    -1,    -1,   797,   314,
      -1,   100,    -1,    -1,    -1,    -1,   105,    -1,    -1,   808,
      -1,   810,    22,    -1,   329,    -1,    26,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    42,    -1,    -1,    -1,   679,    -1,    -1,    49,
      -1,    -1,   685,    -1,    -1,    -1,    -1,    -1,  1050,  1051,
    1052,  1053,    -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    -1,    82,    -1,    84,  1077,  1078,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   402,    -1,    -1,
      -1,    -1,    -1,   103,    -1,    -1,   106,    -1,   626,   109,
      -1,   900,    -1,    -1,    -1,    -1,    -1,    -1,   907,   752,
      -1,   754,    -1,    -1,    -1,   430,   759,    -1,    -1,   762,
     919,   436,    -1,    -1,    -1,    -1,   769,    -1,   443,   772,
      -1,    -1,    -1,   248,    -1,   934,    -1,    -1,    -1,    -1,
     255,    -1,    -1,    -1,   943,    -1,    -1,    -1,   947,   248,
      -1,   679,    -1,    -1,   797,    -1,   255,   685,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   808,    -1,   810,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   500,    -1,    -1,    -1,   504,
      -1,    -1,    -1,    -1,    -1,   994,    -1,   512,    -1,   314,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,   329,   314,    26,    27,    -1,    -1,
      -1,    -1,    -1,    -1,   752,    35,   754,    37,    38,    -1,
     329,   759,    42,    -1,   762,    -1,    -1,    -1,    -1,    49,
      -1,   769,    -1,    -1,   772,    -1,    -1,    -1,    -1,    -1,
      -1,  1050,  1051,  1052,  1053,    -1,    -1,   900,    -1,    -1,
      -1,    71,    -1,    73,   907,    75,    -1,    77,    -1,   797,
      80,    -1,    82,    -1,    84,    -1,   919,    -1,  1077,  1078,
     808,    -1,   810,    -1,     2,    -1,    -1,   402,    -1,    -1,
      -1,   934,    -1,   103,    12,    -1,   106,    22,    -1,   109,
     943,    26,    27,   402,   947,    -1,    -1,    25,    -1,    27,
      35,   626,    37,    38,    -1,   430,    -1,    42,    -1,    -1,
      45,   436,    -1,    -1,    49,    -1,    -1,    52,   443,    -1,
      -1,   430,    -1,    -1,    -1,    -1,    -1,   436,    -1,    -1,
      -1,    -1,    -1,    -1,   443,    -1,    71,    -1,    73,   159,
      -1,   994,    -1,    78,    -1,    73,    -1,    82,    -1,    84,
      -1,    -1,    -1,    -1,   679,    -1,    -1,    -1,    -1,    -1,
     685,    -1,   900,    -1,    -1,    -1,    -1,    -1,   103,   907,
      -1,   106,   100,   108,   109,   500,    -1,   105,    -1,   504,
      -1,   919,    -1,    -1,    -1,    -1,    -1,   512,    -1,    -1,
      -1,   500,    -1,    -1,    -1,   504,   934,  1050,  1051,  1052,
    1053,    -1,    -1,   512,    -1,   943,    22,    -1,    -1,   947,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    37,    38,    -1,  1077,  1078,    42,   752,    -1,   754,
      -1,    -1,    -1,    49,   759,    -1,    -1,   762,    -1,    -1,
      -1,   169,    -1,    -1,   769,    -1,    -1,   772,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,   994,    73,    -1,    75,
      -1,    77,    -1,    -1,    80,    -1,    82,    -1,    84,    -1,
      -1,    -1,   797,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   808,    -1,   810,    -1,   103,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   626,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1050,  1051,  1052,  1053,    -1,   626,    -1,    -1,
     248,    -1,    -1,    -1,    -1,    -1,    -1,   255,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1077,
    1078,    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,   679,    -1,    -1,    -1,    -1,    -1,
     685,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
     679,    -1,    26,    27,    -1,   900,   685,    -1,    -1,    -1,
      -1,    35,   907,    37,    38,    -1,   314,    -1,    42,    -1,
      -1,    -1,    -1,    -1,   919,    49,    -1,    -1,    -1,    -1,
      -1,   329,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   934,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,   943,    73,
      -1,    75,   947,    77,    -1,    -1,    80,   752,    82,   754,
      84,    -1,    -1,    -1,   759,    -1,    -1,   762,    -1,    -1,
      -1,    -1,    -1,   752,   769,   754,    -1,   772,    -1,   103,
     759,    -1,   106,   762,    -1,   109,    -1,    -1,    -1,    -1,
     769,    -1,    -1,   772,    -1,    -1,    -1,    -1,    -1,   994,
      -1,    -1,   797,    -1,   402,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,   808,    -1,   810,    -1,    -1,   797,    -1,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,   808,
      -1,   810,   430,    -1,    -1,   159,    33,    -1,   436,    -1,
      -1,   165,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,  1050,  1051,  1052,  1053,    -1,
      -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,  1077,  1078,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    -1,    93,    -1,    95,    -1,
      97,    -1,   500,   100,    12,   900,   504,   104,   105,    -1,
     107,    19,   907,   110,   512,     3,    -1,    -1,    -1,    -1,
      -1,   900,    -1,    31,   919,    -1,    -1,    -1,   907,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    46,   934,
     919,    -1,    -1,    -1,    -1,    33,    -1,    -1,   943,    -1,
      -1,    39,   947,    -1,    -1,   934,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,   943,    -1,    -1,    -1,   947,   166,
      -1,    59,    -1,    -1,    -1,    83,    -1,    65,    66,    87,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,   994,
      -1,    -1,    90,    91,    -1,    93,   114,    95,    -1,    97,
     118,    -1,   100,    -1,    -1,   994,   104,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,    -1,   134,   135,   626,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,   160,   161,   162,  1050,  1051,  1052,  1053,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1050,  1051,  1052,  1053,    -1,   164,    -1,   166,    -1,
      -1,   679,  1077,  1078,    -1,    -1,    -1,   685,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1077,  1078,
      -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    65,    66,    -1,   752,    -1,   754,    -1,    72,    -1,
      -1,   759,    -1,    -1,   762,    -1,    -1,    81,    -1,    -1,
      -1,   769,    -1,    -1,   772,    -1,    90,    91,    -1,    93,
      -1,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,
     104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   797,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     808,    -1,   810,    -1,    -1,    -1,     0,     1,    -1,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,   166,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    49,    -1,    51,    52,    -1,
      54,    55,    56,    -1,    58,    59,    60,    61,    62,    63,
      -1,    65,    66,    67,    -1,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    88,    89,    90,    91,    92,    93,
      94,    95,    -1,    97,    98,    -1,   100,   101,   102,   103,
     104,   919,   106,   107,   108,   109,   110,    -1,    -1,   113,
      -1,   115,    -1,    -1,   118,    -1,   934,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   943,    -1,    -1,    -1,   947,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,   159,    -1,    -1,   162,    -1,
     164,    -1,   166,    -1,   168,    -1,     1,   171,     3,    -1,
       5,     6,     7,     8,     9,    10,   994,    12,    -1,    14,
      15,    16,    17,    18,    19,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    -1,    51,    52,    -1,    54,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    -1,    -1,    71,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    -1,    97,    98,    -1,   100,   101,   102,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,    -1,   113,    -1,
     115,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    -1,   164,
     165,   166,    -1,   168,    -1,     1,   171,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    51,    52,    -1,    54,    55,
      56,    -1,    58,    59,    60,    61,    62,    63,    -1,    65,
      66,    67,    -1,    -1,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    88,    89,    90,    91,    92,    93,    94,    95,
      -1,    97,    98,    -1,   100,   101,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,    -1,   113,    -1,   115,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,   159,    -1,    -1,   162,    -1,   164,   165,
     166,    -1,   168,    -1,     1,   171,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    54,    55,    56,
      -1,    58,    59,    60,    61,    62,    63,    -1,    65,    66,
      67,    -1,    -1,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    88,    89,    90,    91,    92,    93,    94,    95,    -1,
      97,    98,    -1,   100,   101,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,    -1,   113,    -1,   115,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,   159,    -1,    -1,   162,    -1,   164,   165,   166,
      -1,   168,    -1,     1,   171,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    -1,    51,    52,    -1,    54,    55,    56,    -1,
      58,    59,    60,    61,    62,    63,    -1,    65,    66,    67,
      -1,    -1,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      88,    89,    90,    91,    92,    93,    94,    95,    -1,    97,
      98,    -1,   100,   101,   102,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,    -1,   113,    -1,   115,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,
     158,   159,    -1,    -1,   162,    -1,   164,   165,   166,    -1,
     168,    -1,     1,   171,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    -1,    51,    52,    -1,    54,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      -1,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    88,
      89,    90,    91,    92,    93,    94,    95,    -1,    97,    98,
      -1,   100,   101,   102,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,    -1,   113,    -1,   115,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
     159,    -1,    -1,   162,    -1,   164,   165,   166,    -1,   168,
      -1,     1,   171,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    49,
      -1,    51,    52,    -1,    54,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    -1,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    88,    89,
      90,    91,    92,    93,    94,    95,    -1,    97,    98,    -1,
     100,   101,   102,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,    -1,   115,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,   159,
      -1,    -1,   162,    -1,   164,   165,   166,    -1,   168,    -1,
       1,   171,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    49,    -1,
      51,    52,    -1,    54,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    -1,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    88,    89,    90,
      91,    92,    93,    94,    95,    -1,    97,    98,    -1,   100,
     101,   102,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,    -1,   113,    -1,   115,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,   159,    -1,
      -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,     1,
     171,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    49,    -1,    51,
      52,    -1,    54,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    88,    89,    90,    91,
      92,    93,    94,    95,    -1,    97,    98,    -1,   100,   101,
     102,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
      -1,   113,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,   159,    -1,    -1,
     162,    -1,   164,    -1,   166,    -1,   168,    -1,     1,   171,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    49,    -1,    51,    52,
      -1,    54,    55,    56,    -1,    58,    59,    60,    61,    62,
      63,    -1,    65,    66,    67,    -1,    -1,    -1,    71,    72,
      73,    74,    -1,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    88,    89,    90,    91,    92,
      93,    94,    95,    -1,    97,    98,    -1,   100,   101,   102,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,    -1,
     113,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,   159,    -1,    -1,   162,
      -1,   164,    -1,   166,    -1,   168,    -1,     1,   171,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    49,    -1,    51,    52,    -1,
      54,    55,    56,    -1,    58,    59,    60,    61,    62,    63,
      -1,    65,    66,    67,    -1,    -1,    -1,    71,    72,    73,
      74,    -1,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    88,    89,    90,    91,    92,    93,
      94,    95,    -1,    97,    98,    -1,   100,   101,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,    -1,   113,
      -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,   159,    -1,    -1,   162,    -1,
     164,    -1,   166,    -1,   168,    -1,     1,   171,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    -1,    51,    52,    -1,    54,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    -1,    -1,    71,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    88,    89,    90,    91,    92,    93,    94,
      95,    -1,    97,    98,    -1,   100,   101,   102,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,    -1,   113,    -1,
     115,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,   159,    -1,    -1,   162,    -1,   164,
      -1,   166,    -1,   168,    -1,     1,   171,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    51,    52,    -1,    54,    55,
      56,    -1,    58,    59,    60,    61,    62,    63,    -1,    65,
      66,    67,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    88,    89,    90,    91,    92,    93,    94,    95,
      -1,    97,    98,    -1,   100,   101,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,    -1,   113,    -1,   115,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,   159,    -1,    -1,   162,    -1,   164,    -1,
     166,    -1,   168,    -1,     1,   171,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    54,    55,    56,
      -1,    58,    59,    60,    61,    62,    63,    -1,    65,    66,
      67,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    88,    89,    90,    91,    92,    93,    94,    95,    -1,
      97,    98,    -1,   100,   101,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,    -1,   113,    -1,   115,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,   159,    -1,    -1,   162,    -1,   164,    -1,   166,
      -1,   168,    -1,     1,   171,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    -1,    51,    52,    -1,    54,    55,    56,    -1,
      58,    59,    60,    61,    62,    63,    -1,    65,    66,    67,
      -1,    -1,    -1,    71,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      88,    89,    90,    91,    92,    93,    94,    95,    -1,    97,
      98,    -1,   100,   101,   102,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,    -1,   113,    -1,   115,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,
     158,   159,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,
     168,    -1,     1,   171,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    -1,    51,    52,    -1,    54,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    88,
      89,    90,    91,    92,    93,    94,    95,    -1,    97,    98,
      -1,   100,   101,   102,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,    -1,   113,    -1,   115,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
     159,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,
      -1,     1,   171,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    49,
      -1,    51,    52,    -1,    54,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    -1,
      -1,    71,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    88,    89,
      90,    91,    92,    93,    94,    95,    -1,    97,    98,    -1,
     100,   101,   102,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,    -1,   115,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,   159,
      -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,
       1,   171,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    49,    -1,
      51,    52,    -1,    54,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    -1,    -1,
      71,    72,    73,    74,    -1,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    88,    89,    90,
      91,    92,    93,    94,    95,    -1,    97,    98,    -1,   100,
     101,   102,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,    -1,   113,    -1,   115,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,   159,    -1,
      -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,     1,
     171,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
     162,    -1,   164,    -1,   166,    -1,   168,   169,     1,   171,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,
      -1,   164,    -1,   166,    -1,   168,    -1,     3,   171,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      26,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,
      66,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,
      76,    -1,    -1,    -1,    -1,    81,    82,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
      -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,    -1,
      -1,   107,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,
     166,    -1,   168,    -1,    -1,   171,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,
      -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,    66,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,
      -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    -1,
      97,    -1,    -1,   100,   101,   102,    -1,   104,   105,    -1,
     107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,   143,   144,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,
      -1,   168,    -1,    -1,   171,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,
      -1,    59,    60,    -1,    62,    63,    -1,    65,    66,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    94,    95,    -1,    97,
      -1,    -1,   100,   101,   102,    -1,   104,   105,    -1,   107,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,
     158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,   167,
     168,    -1,    -1,   171,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,
      -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    -1,    97,    -1,
      -1,   100,   101,   102,    -1,   104,   105,    -1,   107,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
      -1,    -1,    -1,   162,    -1,   164,    -1,   166,   167,   168,
      -1,     3,   171,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    46,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,
      82,    -1,    84,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,   104,    -1,    -1,   107,    -1,   109,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
     162,    -1,   164,    -1,   166,    -1,   168,    -1,     3,   171,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,
      -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    -1,    97,    -1,    -1,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,   159,    -1,    -1,   162,    -1,   164,
      -1,   166,    -1,   168,    -1,    -1,   171,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,
      66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,
      76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
      -1,    97,    -1,    -1,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,
     166,    -1,   168,    -1,     3,   171,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    62,    63,    64,    65,    66,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,
      -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    -1,    97,    -1,
      -1,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
      -1,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,
      -1,     3,   171,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,
      -1,   103,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
     162,    -1,   164,    -1,   166,    -1,   168,    -1,     3,   171,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,   116,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,
      -1,   166,    -1,   168,    -1,    -1,   171,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,
      66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,
      76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
      -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,
     166,    -1,   168,    -1,     3,   171,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,
      -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    -1,    97,    -1,
      -1,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
      -1,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,
      -1,    -1,   171,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,
      60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,
      -1,    -1,   162,    -1,   164,   165,   166,    -1,   168,    -1,
      -1,   171,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,
      -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,
      81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,
      -1,   162,    -1,   164,    -1,   166,    -1,   168,   169,    -1,
     171,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
     162,    -1,   164,    -1,   166,    -1,   168,   169,     3,   171,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,
      -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,   116,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,
      -1,   166,    -1,   168,    -1,     3,   171,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,
      -1,    59,    60,    -1,    62,    63,    -1,    65,    66,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    94,    95,    -1,    97,
      -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,
     158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,
     168,    -1,    -1,   171,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,
      -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    -1,    97,    -1,
      -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
      -1,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,
      -1,     3,   171,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,
      -1,   103,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
     162,    -1,   164,    -1,   166,    -1,   168,    -1,     3,   171,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,
      -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,
      -1,   166,    -1,   168,   169,     3,   171,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,
      -1,    59,    60,    -1,    62,    63,    -1,    65,    66,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    94,    95,    -1,    97,
      -1,    -1,   100,    -1,    -1,    -1,   104,   105,    -1,   107,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,
     158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,
     168,    -1,    -1,   171,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,
      -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    -1,    97,    -1,
      -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
      -1,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,
      -1,     3,   171,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,   104,   105,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
     162,    -1,   164,    -1,   166,    -1,   168,    -1,    -1,   171,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,
      -1,   164,    -1,   166,    -1,   168,    -1,    -1,   171,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,    63,
      -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,
     104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,
     164,    -1,   166,    -1,   168,    -1,     3,   171,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,
      -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,    66,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,
      -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    -1,
      97,    -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,
      -1,   168,    -1,     3,   171,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,
      60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,
      -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,
       3,   171,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,
      -1,   164,    -1,   166,    -1,   168,    -1,     3,   171,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,
      66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,
      76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
      -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,
     166,    -1,   168,    -1,     3,   171,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,
      -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    -1,    97,    -1,
      -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
      -1,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,
      -1,     3,   171,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
     162,    -1,   164,    -1,   166,    -1,   168,    -1,     3,   171,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,
      -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,
      -1,   166,    -1,   168,    -1,     3,   171,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,
      -1,    59,    60,    -1,    62,    63,    -1,    65,    66,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    94,    95,    -1,    97,
      -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,
     158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,
     168,    -1,     3,   171,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,
      -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,
      81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,
      -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,     3,
     171,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,    63,
      -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,
     104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,
     164,    -1,   166,    -1,   168,    -1,     3,   171,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,
      -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,    66,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,
      -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    -1,
      97,    -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,
      -1,   168,    -1,     3,   171,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,
      60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,
      -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,
       3,   171,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,
      -1,   164,    -1,   166,    -1,   168,    -1,     3,   171,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,
      66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,
      76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
      -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,
     166,    -1,   168,    -1,     3,   171,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,
      -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    -1,    97,    -1,
      -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
      -1,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,
      -1,     3,   171,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      12,   143,    -1,    -1,    -1,    17,    18,    19,    20,    -1,
      -1,   153,    -1,    25,   156,   157,   158,    -1,    -1,    -1,
     162,    33,    -1,    35,   166,    -1,   168,    39,    -1,   171,
      -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    93,    -1,    95,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,   123,   124,    -1,    -1,   127,   128,   129,   130,    -1,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,   141,
      -1,    -1,    -1,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,    -1,   157,    -1,    -1,   160,   161,
     162,   163,    -1,     3,   166,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,
      60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
     110,    -1,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,   163,   157,   158,   166,
      -1,   168,   162,    -1,   164,    -1,   166,    -1,   168,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,    63,
      -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,
     104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,    -1,    -1,   140,    -1,    -1,    -1,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,
     164,    -1,   166,   167,   168,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,    57,
      -1,    59,    60,    -1,    62,    63,    -1,    65,    66,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    90,    91,    92,    93,    94,    95,    -1,    97,
      -1,    99,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,
      -1,    -1,   110,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,   136,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
     158,   159,    -1,    -1,    -1,    -1,   164,    33,   166,    35,
     168,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,
      66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,
      76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
      -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,     3,
     136,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,   158,    -1,    -1,    -1,    -1,    -1,   164,    33,
     166,    35,   168,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,    63,
      -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,
     104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,   136,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,   158,    -1,    -1,    -1,    -1,    -1,
     164,    33,   166,    35,   168,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,
      82,    -1,     3,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    -1,    97,    17,    18,   100,    20,
      -1,    -1,   104,    -1,    25,   107,    -1,    -1,   110,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,     3,    -1,   136,    -1,    -1,    -1,    59,    -1,
      -1,    12,    -1,    -1,    65,    66,    17,    18,    19,    20,
      -1,    72,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    33,    -1,   166,    -1,   168,    -1,    39,    90,
      91,    -1,    93,    44,    95,    -1,    97,    48,    -1,   100,
      51,    -1,    53,   104,    -1,    -1,   107,    -1,    59,   110,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    93,    12,    95,    -1,    97,    -1,    -1,   100,
      19,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,
      -1,    -1,    31,    -1,    -1,   166,    -1,    -1,   119,   120,
     121,   122,   123,   124,    -1,    -1,   127,   128,   129,   130,
      -1,   132,   133,   134,   135,   136,   137,   138,    -1,    -1,
     141,    -1,    -1,    -1,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,    -1,   157,    -1,    -1,   160,
     161,   162,   163,    -1,    83,    -1,    -1,    -1,    87,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,   120,   121,   122,   123,   124,    -1,    -1,   127,   128,
     129,   130,    -1,   132,   133,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,    -1,
      83,   160,   161,   162,    87,   164,    -1,    -1,    -1,    12,
      -1,    -1,   171,    96,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,   120,   121,   122,
     123,   124,    -1,    -1,   127,   128,   129,   130,    -1,   132,
     133,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,    -1,   160,   161,   162,
      83,   164,    -1,    -1,    87,    12,    -1,    -1,   171,    -1,
      -1,    -1,    19,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,   120,   121,   122,
     123,   124,    -1,    -1,   127,   128,   129,   130,    -1,   132,
     133,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,    83,   160,   161,   162,
      87,   164,    -1,    -1,    -1,    12,    -1,    -1,   171,    96,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,   120,   121,   122,   123,   124,    -1,    -1,
     127,   128,   129,   130,    -1,   132,   133,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    -1,    -1,   160,   161,   162,    83,   164,    -1,    -1,
      87,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,   120,   121,   122,   123,   124,    -1,    -1,
     127,   128,   129,   130,    -1,   132,   133,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    -1,    83,   160,   161,   162,    87,   164,    -1,    -1,
      -1,    12,    -1,    -1,   171,    96,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,   120,
     121,   122,   123,   124,    -1,    -1,   127,   128,   129,   130,
      -1,   132,   133,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,    -1,   160,
     161,   162,    83,   164,    -1,    -1,    87,    12,    -1,    -1,
     171,    -1,    -1,    -1,    19,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,   120,
     121,   122,   123,   124,    -1,    -1,   127,   128,   129,   130,
      -1,   132,   133,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,    83,   160,
     161,   162,    87,   164,    -1,    -1,    -1,    12,    -1,    -1,
     171,    96,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,   120,   121,   122,   123,   124,
      -1,    -1,   127,   128,   129,   130,    -1,   132,   133,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,    -1,    -1,   160,   161,   162,    83,   164,
      -1,    -1,    87,    -1,    -1,    -1,   171,    12,    -1,    -1,
      -1,    96,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,   118,   119,   120,   121,   122,   123,   124,
      -1,    46,   127,   128,   129,   130,    -1,   132,   133,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,    -1,    -1,   160,   161,   162,    83,   164,
      -1,    -1,    87,    12,    -1,    -1,   171,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,    -1,    83,   160,   161,   162,    87,   164,
      -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,    68,
      -1,   160,   161,   162,    -1,   164,    -1,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    83,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,    68,
     159,   160,   161,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    83,    -1,    -1,    -1,    87,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,    -1,
      83,   160,   161,   162,    87,    -1,    -1,    -1,    -1,    12,
      -1,    -1,   171,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,    -1,   160,   161,   162,
      83,   164,    -1,    -1,    87,    12,    -1,    -1,   171,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,    83,   160,   161,   162,
      87,   164,    -1,    -1,    -1,    12,    -1,    -1,   171,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    -1,    -1,   160,   161,   162,    83,    -1,    -1,    -1,
      87,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    -1,    83,   160,   161,   162,    87,    -1,    -1,    -1,
      -1,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,    -1,   160,
     161,   162,    83,   164,    -1,    -1,    87,    12,    -1,    -1,
     171,    -1,    -1,    -1,    19,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,    83,   160,
     161,   162,    87,   164,    -1,    -1,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,   118,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,    68,    12,   160,   161,   162,    -1,    -1,
      -1,    19,    -1,    -1,   169,    -1,   171,    -1,    83,    -1,
      -1,    -1,    87,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   134,
     135,    -1,   137,   138,   139,    83,   141,   142,   143,    87,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,    12,    -1,   160,   161,   162,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
     118,    -1,    31,    -1,    -1,    83,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     118,    -1,   160,   161,   162,    -1,   164,    -1,    -1,    -1,
      -1,    -1,    -1,   171,    83,    -1,   134,   135,    87,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      12,   159,   160,   161,   162,    -1,    -1,    19,    -1,   118,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,    12,
     159,   160,   161,   162,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    83,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      83,    -1,   134,   135,    87,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,    -1,   159,   160,   161,
     162,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      87,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    12,   159,   160,   161,   162,
     117,   118,    19,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    12,    -1,   160,   161,   162,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    87,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    -1,    -1,   160,   161,   162,    -1,   118,    -1,    -1,
      -1,    -1,    83,    -1,   171,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   118,    -1,   160,
     161,   162,    -1,    -1,    12,    -1,   167,    -1,    -1,    -1,
     171,    19,    -1,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    31,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,    -1,   160,
     161,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
     171,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    87,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,   114,   160,   161,   162,   118,    -1,    -1,    -1,    -1,
      83,    -1,    -1,   171,    87,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   118,    -1,   160,   161,   162,
      -1,    -1,    12,    13,    -1,    -1,    -1,    -1,   171,    19,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    31,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,   159,   160,   161,   162,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   171,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    83,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   118,    -1,
     160,   161,   162,    83,    -1,    -1,    -1,    87,    -1,    -1,
      -1,   171,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   118,    -1,
     160,   161,   162,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   171,    19,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    31,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,    12,   167,    -1,    -1,
      -1,   171,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   117,   118,   160,   161,   162,    -1,    83,    -1,    -1,
     167,    87,    -1,    -1,   171,    -1,    -1,    -1,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   118,    -1,   160,   161,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    12,     3,   160,   161,   162,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    17,    18,    -1,
      20,    31,    32,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    12,    -1,    44,    -1,    -1,    -1,    48,    19,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      -1,    31,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    72,    83,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    81,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      90,    91,    -1,    93,    -1,    95,    -1,    97,    -1,    -1,
     100,    31,    -1,    -1,   104,    -1,    -1,   107,   118,    -1,
     110,    -1,    -1,    83,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   118,    -1,
     160,   161,   162,    83,    -1,    -1,    -1,    87,    -1,    -1,
      -1,   171,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   118,    -1,
     160,   161,   162,    -1,    -1,    12,    -1,    -1,    -1,   169,
      -1,   171,    19,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    31,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   171,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    12,    13,    -1,    -1,    96,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    96,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   118,    -1,   160,   161,   162,    83,    -1,    -1,    -1,
      87,    -1,    -1,    -1,   171,    -1,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   118,    -1,   160,   161,   162,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,   171,    19,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    31,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,   171,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    83,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   118,    -1,   160,   161,   162,    -1,
      -1,    12,    -1,   167,    -1,    -1,    -1,   171,    19,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      31,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    12,    -1,    -1,    -1,   169,    -1,   171,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    96,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,    96,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   118,    -1,   160,
     161,   162,    83,    -1,    -1,    -1,    87,    -1,    -1,    -1,
     171,    -1,    -1,   134,   135,    96,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   118,    -1,   160,
     161,   162,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     171,    19,    -1,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    31,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,    -1,   160,
     161,   162,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     171,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    96,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    96,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     118,    -1,   160,   161,   162,    83,    -1,    -1,    -1,    87,
      -1,    -1,    -1,   171,    -1,    -1,   134,   135,    96,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     118,    -1,   160,   161,   162,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   171,    19,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    31,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    57,   171,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    83,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   118,    -1,   160,   161,   162,    83,    -1,
      -1,    -1,    87,    -1,    -1,    -1,   171,    -1,    -1,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   118,    -1,   160,   161,   162,    -1,    -1,
      12,    -1,    -1,    -1,   169,    -1,   171,    19,    -1,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    31,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,    -1,    22,   160,   161,   162,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,   171,    35,    -1,    37,
      38,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,    77,
      -1,    -1,    80,    -1,    82,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,   109,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    31,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,    -1,    22,   160,   161,
     162,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,   171,
      35,    -1,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,
      -1,   159,    -1,    -1,    49,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    87,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,
      75,    -1,    77,    -1,    -1,    80,    -1,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,   103,    -1,
      -1,   106,    -1,    -1,   109,   134,   135,    -1,   137,   138,
     139,    83,   141,   142,   143,    87,   145,   146,   147,   148,
      -1,    -1,   151,   152,   153,   154,   155,   156,   157,    -1,
      -1,   160,   161,   162,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,   118,    -1,    -1,    83,
      -1,    -1,    -1,    87,   159,    -1,    -1,    -1,    -1,    -1,
     165,    -1,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,    -1,    83,   160,   161,
     162,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,    -1,
     146,   147,   148,    -1,    -1,   151,   152,   153,   154,    -1,
       1,   157,     3,    -1,   160,   161,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   171,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,     3,    -1,    -1,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    65,    66,    17,    18,    -1,    20,
      -1,    72,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    90,
      91,    -1,    93,    44,    95,    -1,    97,    48,    -1,   100,
      51,    -1,     3,   104,    -1,    -1,   107,    -1,    59,   110,
      -1,    -1,    -1,    -1,    65,    66,    17,    18,    -1,    20,
      -1,    72,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    90,
      91,    -1,    93,    44,    95,    -1,    97,    48,    -1,   100,
      51,    -1,     3,   104,    -1,    -1,   107,    -1,    59,   110,
      -1,    -1,    -1,    -1,    65,    66,    17,    18,    -1,    20,
      -1,    72,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    90,
      91,    -1,    93,    44,    95,    -1,    97,    48,    -1,   100,
      51,    -1,     3,   104,    -1,    -1,   107,    -1,    59,   110,
      -1,    -1,    -1,    -1,    65,    66,    17,    18,    -1,    20,
      -1,    72,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    90,
      91,    -1,    93,    44,    95,    -1,    97,    48,    -1,   100,
      51,    -1,     3,   104,    -1,    -1,   107,    -1,    59,   110,
      -1,    -1,    -1,    -1,    65,    66,    17,    18,    -1,    20,
      -1,    72,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    90,
      91,    -1,    93,    44,    95,    -1,    97,    48,    -1,   100,
      51,    -1,    -1,   104,    -1,    -1,   107,    -1,    59,   110,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    93,    -1,    95,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   176,   177,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,    35,
      37,    38,    39,    40,    41,    42,    43,    44,    47,    48,
      49,    51,    54,    55,    56,    58,    59,    60,    62,    63,
      65,    66,    67,    71,    72,    73,    74,    75,    76,    77,
      80,    81,    82,    84,    85,    86,    88,    89,    90,    91,
      92,    93,    94,    95,    97,    98,   100,   101,   102,   103,
     104,   106,   107,   109,   110,   113,   115,   118,   134,   135,
     136,   137,   138,   143,   153,   156,   157,   158,   159,   162,
     164,   166,   168,   171,   178,   179,   180,   181,   182,   183,
     186,   188,   194,   195,   196,   199,   200,   204,   206,   209,
     210,   212,   213,   214,   219,   220,   222,   223,   224,   228,
     229,   232,   233,   236,   237,   240,   243,   244,   269,   272,
     273,   293,   294,   295,   296,   297,   298,   299,   307,   308,
     309,   310,   311,   314,   315,   316,   317,   318,   319,   320,
     322,   323,   324,   325,   326,   159,   180,   311,   114,   300,
     301,     3,   201,    14,    22,    35,    40,    41,    43,    82,
      95,   164,   168,   240,   293,   298,   309,   310,   311,   314,
     316,   317,   300,   311,   103,   272,    84,   201,   180,   287,
     311,   180,   166,     8,    82,   311,   312,     8,    11,    82,
     103,   312,    73,   116,   221,   311,   221,   311,    26,   273,
     311,   311,   187,   166,     3,    17,    18,    20,    25,    33,
      39,    44,    48,    51,    59,    65,    66,    72,    81,    90,
      91,    93,    95,    97,   100,   104,   107,   110,   203,   205,
     241,   166,   203,   274,   275,    32,   188,   208,   311,   311,
      18,    72,    90,   107,   311,   311,   311,     8,   166,   211,
      22,    35,    38,    82,   212,     4,   263,   286,   311,   101,
     102,   159,   311,   313,   311,   208,   311,   311,   311,    94,
     166,   180,   311,   311,   188,   200,   311,   314,   188,   200,
     311,   203,   270,   311,   311,   311,   311,   311,   311,   311,
     311,     1,   165,   178,   189,   286,   105,   144,   263,   288,
     289,   313,   221,   286,   311,   321,   311,    75,   180,   164,
      79,   184,    45,   108,   203,   203,    52,    78,   258,   274,
     158,   159,   150,   311,    12,    19,    31,    83,    87,   118,
     134,   135,   137,   138,   139,   141,   142,   143,   145,   146,
     147,   148,   149,   151,   152,   153,   154,   155,   156,   157,
     160,   161,   162,   171,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   163,
     250,   166,   168,    83,    87,   166,   180,   159,   311,   311,
     311,   286,   164,   188,    46,   300,   270,   274,   159,   140,
     159,   113,   204,   263,   290,   291,   292,   313,    82,   217,
     244,   272,    82,   215,   270,   244,   272,   203,   166,   208,
      32,    46,   208,   114,   208,   303,    32,    46,   208,   303,
      36,    68,   159,    96,   119,   188,   250,    77,    80,   185,
     290,   180,   166,   252,   105,   166,   203,   276,   277,     1,
     139,   281,    46,   140,   180,   208,   166,   166,   208,   290,
     212,   140,   159,   311,   311,   159,   164,   208,   166,   290,
     159,   225,   159,   225,   159,   119,   271,   159,   208,   159,
     165,   165,   178,   140,   165,   311,   140,   167,   140,   167,
     169,   303,    46,   140,   169,   303,   117,   140,   169,     8,
       1,   165,   189,    61,   197,   198,   311,   191,   311,   139,
     234,   164,   245,   159,   311,   311,   311,   311,   221,   311,
     221,   311,   311,   311,   311,   311,   311,   311,    20,    33,
      59,   103,   204,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
      64,   313,   313,   313,   313,   313,   290,   290,   221,   311,
     221,   311,    27,    46,    84,   109,   302,   305,   306,   311,
     326,    32,    46,    32,    46,    96,    46,   169,   221,   311,
     208,   159,   159,   311,   311,   119,   167,   140,   218,   203,
     274,   216,   203,   159,   274,    46,   286,    43,   311,   221,
     311,   166,   208,    43,   311,   221,   311,   208,   162,   190,
     193,   311,   190,   192,   180,   311,    34,   311,   184,   167,
      27,    46,    50,    70,    73,    84,   103,   179,   253,   254,
     255,   256,   242,   277,   140,   167,    33,    48,    91,    95,
     168,   207,   282,   294,   119,   278,   311,   275,   311,   311,
     167,   263,   311,     1,   230,   290,   167,    21,   226,   282,
     294,   140,   165,   167,   167,   288,   167,   288,   180,   169,
     221,   311,   169,   180,   311,   169,   311,   169,   311,   165,
     165,   203,   140,   159,    13,   142,   140,   159,    13,    36,
      68,   286,   164,     1,   203,   238,   239,    27,    73,    84,
     103,   247,   257,   166,   159,   159,   159,   159,   159,   159,
     167,   169,    46,    84,   140,   167,   293,    83,    83,    43,
     221,   311,    43,   221,   311,   221,   311,   300,   300,   159,
     263,   313,   292,   203,   234,   159,   203,   234,   159,   311,
     167,   311,    32,   208,    32,   208,   304,   305,   311,    32,
     208,   303,    32,   208,   303,   140,   159,    13,   159,    34,
      34,   180,    96,   188,    61,    46,    84,   255,   140,   167,
     166,   203,    27,    73,    84,   103,   259,   167,   277,   281,
       1,   286,    64,   313,   167,   167,   165,    69,   111,   165,
     231,   167,   166,   188,   203,   227,   270,   180,   169,   303,
     169,   303,   180,   117,   197,   204,   164,   311,   105,   311,
     193,   192,     1,   235,   165,   119,   140,   165,    84,   246,
       1,     3,    12,    17,    19,    20,    25,    39,    44,    51,
      53,    59,    65,    66,    81,    93,    97,   100,   104,   110,
     134,   135,   136,   137,   138,   141,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   157,   160,   161,
     162,   163,   166,   202,   203,   205,   248,   249,   250,   293,
     167,   305,   281,   293,   293,   311,    32,    32,   311,    32,
      32,   169,   169,   208,   208,   234,   164,   234,   164,   208,
      96,    43,   311,    43,   311,   140,   167,    96,    43,   311,
     208,    43,   311,   208,   311,   311,   180,   311,   180,    34,
     203,   203,   254,   277,   139,   285,    84,   281,   278,   169,
      46,   169,   166,   166,    32,   180,   286,   227,   139,   188,
      43,   180,   311,   169,    43,   180,   311,   169,   311,   190,
      13,    36,    68,    36,    68,   159,   159,   165,    77,    80,
     165,   179,   210,   311,   239,   259,   166,   251,   311,   134,
     142,   251,   251,   278,    96,    43,    43,    96,    43,    43,
      43,    43,   164,   235,   164,   235,   311,   311,   311,   305,
     311,   311,   311,    13,    34,   180,   159,   285,   167,   168,
     207,   263,   284,   294,   144,   264,   278,    57,   112,   265,
     311,   282,   294,   290,   290,   180,   208,   167,   311,   311,
     180,   311,   180,   165,   105,   311,   193,   192,   193,   192,
     210,     1,   139,   280,   253,   167,     3,    97,   249,   250,
     311,   311,   311,   311,   311,   311,   235,   165,   235,   165,
      96,    96,    96,    96,   311,   180,   264,   278,   285,   169,
     286,   263,   311,     3,    86,    97,   266,   267,   268,   311,
     188,   209,   262,   169,   167,   167,   188,    96,    96,   159,
     159,   159,   159,   168,   207,   279,   294,    99,   260,   167,
     251,   251,    96,    96,    96,    96,    96,    96,   165,   165,
     311,   311,   311,   311,   264,   278,   263,   283,   284,   294,
      46,   169,   268,   112,   140,   119,   145,   147,   148,   151,
     152,    57,   294,   158,   158,   311,   311,     1,   169,   286,
     265,   311,   283,   284,   311,   267,   268,   268,   268,   268,
     268,   268,   266,   169,   279,   294,   169,   159,   261,   262,
     169,   279,   294,   283
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   175,   176,   177,   177,   178,   178,   179,   179,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   181,   182,   182,   182,   183,   183,   183,   184,   184,
     185,   185,   185,   187,   186,   188,   188,   188,   189,   189,
     190,   190,   190,   190,   191,   191,   191,   191,   191,   191,
     192,   192,   193,   193,   194,   194,   194,   195,   195,   195,
     195,   195,   195,   195,   196,   197,   197,   197,   198,   198,
     199,   200,   200,   200,   200,   200,   200,   201,   201,   202,
     202,   202,   202,   202,   202,   203,   203,   203,   203,   203,
     203,   204,   204,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   207,   207,   207,   207,   208,   208,
     209,   209,   210,   210,   210,   211,   210,   212,   212,   212,
     212,   212,   212,   212,   213,   213,   213,   213,   215,   214,
     216,   214,   217,   214,   218,   214,   214,   214,   214,   214,
     219,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   221,   222,   222,
     222,   222,   222,   222,   222,   222,   223,   224,   224,   224,
     224,   224,   224,   225,   225,   226,   226,   226,   227,   227,
     228,   229,   229,   230,   230,   231,   231,   231,   232,   232,
     233,   233,   233,   234,   234,   235,   235,   235,   236,   236,
     237,   238,   238,   238,   239,   239,   241,   242,   240,   243,
     243,   243,   245,   246,   244,   247,   247,   247,   247,   247,
     248,   248,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   251,   251,   252,   253,   253,   253,   254,   254,   254,
     254,   254,   254,   255,   255,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   257,   257,   257,   257,   257,   257,
     258,   258,   259,   259,   259,   259,   259,   259,   260,   260,
     261,   261,   262,   262,   263,   264,   264,   264,   265,   265,
     265,   265,   265,   266,   266,   267,   267,   267,   267,   267,
     267,   267,   268,   268,   269,   269,   269,   270,   270,   271,
     271,   271,   272,   272,   272,   272,   272,   273,   273,   274,
     274,   275,   275,   276,   276,   276,   277,   277,   277,   278,
     278,   278,   279,   279,   279,   279,   279,   279,   279,   280,
     280,   280,   280,   280,   281,   281,   281,   281,   281,   282,
     282,   282,   282,   283,   283,   283,   284,   284,   284,   284,
     284,   285,   285,   285,   285,   285,   286,   286,   286,   286,
     287,   287,   288,   288,   288,   289,   289,   290,   290,   291,
     291,   292,   292,   292,   292,   293,   293,   294,   294,   294,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   297,
     298,   299,   299,   299,   299,   299,   299,   299,   299,   300,
     300,   301,   302,   302,   303,   304,   304,   305,   305,   305,
     306,   306,   306,   306,   306,   306,   307,   307,   308,   308,
     308,   308,   308,   309,   309,   309,   309,   309,   310,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   312,   312,   313,   313,   313,
     314,   314,   314,   314,   315,   315,   315,   315,   316,   316,
     316,   317,   317,   317,   317,   317,   318,   318,   318,   318,
     319,   319,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   321,   321,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   323,   323,   323,   323,   323,   323,   323,   324,   324,
     324,   324,   325,   325,   325,   325,   326,   326,   326,   326,
     326,   326,   326
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     2,     3,     3,     2,     2,     3,
       2,     4,     3,     4,     4,     0,     1,     1,     0,     1,
       0,     1,     1,     0,     7,     2,     3,     3,     1,     2,
       1,     3,     3,     5,     1,     3,     3,     3,     5,     5,
       0,     1,     1,     1,     0,     1,     1,     4,     6,     8,
       8,     6,     8,     8,     4,     1,     3,     5,     1,     3,
       3,     4,     4,     4,     4,     4,     4,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       2,     3,     1,     1,     2,     0,     3,     1,     1,     1,
       1,     1,     1,     1,     3,     5,     5,     2,     0,     8,
       0,     9,     0,     8,     0,     9,     3,     3,     5,     5,
       2,     5,     3,     6,     6,     4,     5,     5,     3,     3,
       6,     5,     6,     5,     6,     3,     4,     3,     4,     6,
       7,     6,     7,     4,     5,     4,     5,     4,     4,     3,
       6,     5,     6,     5,     8,     7,     2,     3,     3,     2,
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
       1,     1,     1,     1,     1,     1,     1,     4,     3,     3,
       3,     2,     2,     2,     1,     0,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     4,     4,
       4,     3,     3,     3,     3,     5,     3,     4,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     4,     3,     4,     3,     4,     3,     5,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     1,     1
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
#line 506 "chapel.ypp"
                                       { yyblock = (yyval.pblockstmt); }
#line 6469 "bison-chapel.cpp"
    break;

  case 3:
#line 511 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6475 "bison-chapel.cpp"
    break;

  case 4:
#line 512 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6481 "bison-chapel.cpp"
    break;

  case 6:
#line 519 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6487 "bison-chapel.cpp"
    break;

  case 7:
#line 524 "chapel.ypp"
                                       { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6493 "bison-chapel.cpp"
    break;

  case 8:
#line 525 "chapel.ypp"
                                       { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6499 "bison-chapel.cpp"
    break;

  case 25:
#line 546 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6505 "bison-chapel.cpp"
    break;

  case 26:
#line 547 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6511 "bison-chapel.cpp"
    break;

  case 27:
#line 548 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6517 "bison-chapel.cpp"
    break;

  case 28:
#line 549 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6523 "bison-chapel.cpp"
    break;

  case 29:
#line 550 "chapel.ypp"
                                         { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6529 "bison-chapel.cpp"
    break;

  case 30:
#line 551 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6535 "bison-chapel.cpp"
    break;

  case 31:
#line 552 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6541 "bison-chapel.cpp"
    break;

  case 32:
#line 553 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6547 "bison-chapel.cpp"
    break;

  case 33:
#line 554 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6553 "bison-chapel.cpp"
    break;

  case 34:
#line 555 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6559 "bison-chapel.cpp"
    break;

  case 35:
#line 556 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6565 "bison-chapel.cpp"
    break;

  case 36:
#line 557 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6571 "bison-chapel.cpp"
    break;

  case 37:
#line 558 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6577 "bison-chapel.cpp"
    break;

  case 38:
#line 559 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6583 "bison-chapel.cpp"
    break;

  case 39:
#line 560 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6589 "bison-chapel.cpp"
    break;

  case 40:
#line 561 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6595 "bison-chapel.cpp"
    break;

  case 41:
#line 566 "chapel.ypp"
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[0].pch), currentModuleType, NULL, yyfilename, (yyvsp[-3].b), (yyvsp[-2].b), (yylsp[-3]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[0].pch);
    }
#line 6609 "bison-chapel.cpp"
    break;

  case 42:
#line 579 "chapel.ypp"
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6618 "bison-chapel.cpp"
    break;

  case 43:
#line 584 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6627 "bison-chapel.cpp"
    break;

  case 44:
#line 589 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6636 "bison-chapel.cpp"
    break;

  case 45:
#line 596 "chapel.ypp"
        { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6642 "bison-chapel.cpp"
    break;

  case 46:
#line 597 "chapel.ypp"
          { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6648 "bison-chapel.cpp"
    break;

  case 47:
#line 598 "chapel.ypp"
           { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6654 "bison-chapel.cpp"
    break;

  case 48:
#line 602 "chapel.ypp"
             { (yyval.b) = false; }
#line 6660 "bison-chapel.cpp"
    break;

  case 49:
#line 603 "chapel.ypp"
             { (yyval.b) = true;  }
#line 6666 "bison-chapel.cpp"
    break;

  case 50:
#line 607 "chapel.ypp"
           { (yyval.b) = false; }
#line 6672 "bison-chapel.cpp"
    break;

  case 51:
#line 608 "chapel.ypp"
           { (yyval.b) = false; }
#line 6678 "bison-chapel.cpp"
    break;

  case 52:
#line 609 "chapel.ypp"
           { (yyval.b) = true; }
#line 6684 "bison-chapel.cpp"
    break;

  case 53:
#line 614 "chapel.ypp"
  {
    (yylsp[0]).comment = context->latestComment;
    context->latestComment = NULL;
  }
#line 6693 "bison-chapel.cpp"
    break;

  case 54:
#line 619 "chapel.ypp"
 {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[-1].pch), (yyvsp[-4].b), (yyvsp[-3].b), (yylsp[-6]).comment);
 }
#line 6701 "bison-chapel.cpp"
    break;

  case 55:
#line 634 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); }
#line 6707 "bison-chapel.cpp"
    break;

  case 56:
#line 635 "chapel.ypp"
                                       { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6713 "bison-chapel.cpp"
    break;

  case 57:
#line 636 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6719 "bison-chapel.cpp"
    break;

  case 58:
#line 641 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6725 "bison-chapel.cpp"
    break;

  case 59:
#line 642 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6731 "bison-chapel.cpp"
    break;

  case 60:
#line 647 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6741 "bison-chapel.cpp"
    break;

  case 61:
#line 652 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6751 "bison-chapel.cpp"
    break;

  case 62:
#line 657 "chapel.ypp"
                                          { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6760 "bison-chapel.cpp"
    break;

  case 63:
#line 661 "chapel.ypp"
                                          { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6769 "bison-chapel.cpp"
    break;

  case 64:
#line 670 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6779 "bison-chapel.cpp"
    break;

  case 65:
#line 675 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6789 "bison-chapel.cpp"
    break;

  case 66:
#line 680 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6799 "bison-chapel.cpp"
    break;

  case 67:
#line 685 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6808 "bison-chapel.cpp"
    break;

  case 68:
#line 689 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6817 "bison-chapel.cpp"
    break;

  case 69:
#line 693 "chapel.ypp"
                                             { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6826 "bison-chapel.cpp"
    break;

  case 70:
#line 701 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6836 "bison-chapel.cpp"
    break;

  case 72:
#line 710 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6846 "bison-chapel.cpp"
    break;

  case 74:
#line 719 "chapel.ypp"
           { (yyval.b) = true; }
#line 6852 "bison-chapel.cpp"
    break;

  case 75:
#line 720 "chapel.ypp"
           { (yyval.b) = false; }
#line 6858 "bison-chapel.cpp"
    break;

  case 76:
#line 721 "chapel.ypp"
           { (yyval.b) = true; }
#line 6864 "bison-chapel.cpp"
    break;

  case 77:
#line 725 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 6870 "bison-chapel.cpp"
    break;

  case 78:
#line 726 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 6876 "bison-chapel.cpp"
    break;

  case 79:
#line 727 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6882 "bison-chapel.cpp"
    break;

  case 80:
#line 728 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6888 "bison-chapel.cpp"
    break;

  case 81:
#line 729 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 6894 "bison-chapel.cpp"
    break;

  case 82:
#line 730 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6900 "bison-chapel.cpp"
    break;

  case 83:
#line 731 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6906 "bison-chapel.cpp"
    break;

  case 84:
#line 735 "chapel.ypp"
                                             { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[-3].b));}
#line 6913 "bison-chapel.cpp"
    break;

  case 85:
#line 740 "chapel.ypp"
       { (yyval.pimportstmt) = buildImportStmt((yyvsp[0].pexpr)); }
#line 6919 "bison-chapel.cpp"
    break;

  case 86:
#line 741 "chapel.ypp"
                     { (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 6925 "bison-chapel.cpp"
    break;

  case 87:
#line 742 "chapel.ypp"
                                   { (yyval.pimportstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist)); }
#line 6931 "bison-chapel.cpp"
    break;

  case 88:
#line 746 "chapel.ypp"
              { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pimportstmt)); }
#line 6937 "bison-chapel.cpp"
    break;

  case 89:
#line 747 "chapel.ypp"
                               { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pimportstmt)); }
#line 6943 "bison-chapel.cpp"
    break;

  case 90:
#line 751 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 6949 "bison-chapel.cpp"
    break;

  case 91:
#line 756 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 6955 "bison-chapel.cpp"
    break;

  case 92:
#line 758 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 6961 "bison-chapel.cpp"
    break;

  case 93:
#line 760 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 6967 "bison-chapel.cpp"
    break;

  case 94:
#line 762 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 6973 "bison-chapel.cpp"
    break;

  case 95:
#line 764 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 6979 "bison-chapel.cpp"
    break;

  case 96:
#line 766 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), new SymExpr(gNoInit), "="); }
#line 6985 "bison-chapel.cpp"
    break;

  case 97:
#line 772 "chapel.ypp"
         { (yyval.pch) = NULL; }
#line 6991 "bison-chapel.cpp"
    break;

  case 98:
#line 773 "chapel.ypp"
         { (yyval.pch) = (yyvsp[0].pch); }
#line 6997 "bison-chapel.cpp"
    break;

  case 99:
#line 777 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7003 "bison-chapel.cpp"
    break;

  case 100:
#line 778 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7009 "bison-chapel.cpp"
    break;

  case 101:
#line 779 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7015 "bison-chapel.cpp"
    break;

  case 102:
#line 780 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7021 "bison-chapel.cpp"
    break;

  case 103:
#line 781 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7027 "bison-chapel.cpp"
    break;

  case 104:
#line 782 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7033 "bison-chapel.cpp"
    break;

  case 105:
#line 785 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7039 "bison-chapel.cpp"
    break;

  case 106:
#line 786 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7045 "bison-chapel.cpp"
    break;

  case 107:
#line 787 "chapel.ypp"
                           { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 7051 "bison-chapel.cpp"
    break;

  case 108:
#line 788 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7057 "bison-chapel.cpp"
    break;

  case 109:
#line 789 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7063 "bison-chapel.cpp"
    break;

  case 110:
#line 790 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7069 "bison-chapel.cpp"
    break;

  case 111:
#line 802 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7075 "bison-chapel.cpp"
    break;

  case 112:
#line 803 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7081 "bison-chapel.cpp"
    break;

  case 113:
#line 814 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 7087 "bison-chapel.cpp"
    break;

  case 114:
#line 815 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 7093 "bison-chapel.cpp"
    break;

  case 115:
#line 816 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 7099 "bison-chapel.cpp"
    break;

  case 116:
#line 817 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 7105 "bison-chapel.cpp"
    break;

  case 117:
#line 818 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 7111 "bison-chapel.cpp"
    break;

  case 118:
#line 819 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 7117 "bison-chapel.cpp"
    break;

  case 119:
#line 820 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 7123 "bison-chapel.cpp"
    break;

  case 120:
#line 821 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 7129 "bison-chapel.cpp"
    break;

  case 121:
#line 822 "chapel.ypp"
             { (yyval.pch) = "sync"; }
#line 7135 "bison-chapel.cpp"
    break;

  case 122:
#line 823 "chapel.ypp"
             { (yyval.pch) = "single"; }
#line 7141 "bison-chapel.cpp"
    break;

  case 123:
#line 824 "chapel.ypp"
             { (yyval.pch) = "owned"; }
#line 7147 "bison-chapel.cpp"
    break;

  case 124:
#line 825 "chapel.ypp"
             { (yyval.pch) = "shared"; }
#line 7153 "bison-chapel.cpp"
    break;

  case 125:
#line 826 "chapel.ypp"
             { (yyval.pch) = "borrowed"; }
#line 7159 "bison-chapel.cpp"
    break;

  case 126:
#line 827 "chapel.ypp"
             { (yyval.pch) = "unmanaged"; }
#line 7165 "bison-chapel.cpp"
    break;

  case 127:
#line 828 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 7171 "bison-chapel.cpp"
    break;

  case 128:
#line 829 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 7177 "bison-chapel.cpp"
    break;

  case 129:
#line 830 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 7183 "bison-chapel.cpp"
    break;

  case 130:
#line 831 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 7189 "bison-chapel.cpp"
    break;

  case 131:
#line 832 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 7195 "bison-chapel.cpp"
    break;

  case 132:
#line 836 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 7201 "bison-chapel.cpp"
    break;

  case 133:
#line 837 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 7207 "bison-chapel.cpp"
    break;

  case 134:
#line 838 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 7213 "bison-chapel.cpp"
    break;

  case 135:
#line 839 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 7219 "bison-chapel.cpp"
    break;

  case 136:
#line 840 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7225 "bison-chapel.cpp"
    break;

  case 137:
#line 841 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7231 "bison-chapel.cpp"
    break;

  case 138:
#line 842 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 7237 "bison-chapel.cpp"
    break;

  case 139:
#line 843 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 7243 "bison-chapel.cpp"
    break;

  case 140:
#line 844 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 7249 "bison-chapel.cpp"
    break;

  case 141:
#line 845 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 7255 "bison-chapel.cpp"
    break;

  case 142:
#line 846 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 7261 "bison-chapel.cpp"
    break;

  case 143:
#line 847 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 7267 "bison-chapel.cpp"
    break;

  case 144:
#line 854 "chapel.ypp"
             { (yyval.pch) = "_syncvar"; }
#line 7273 "bison-chapel.cpp"
    break;

  case 145:
#line 855 "chapel.ypp"
             { (yyval.pch) = "_singlevar"; }
#line 7279 "bison-chapel.cpp"
    break;

  case 146:
#line 856 "chapel.ypp"
             { (yyval.pch) = "_domain"; }
#line 7285 "bison-chapel.cpp"
    break;

  case 147:
#line 857 "chapel.ypp"
             { (yyval.pch) = "_index"; }
#line 7291 "bison-chapel.cpp"
    break;

  case 148:
#line 861 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7297 "bison-chapel.cpp"
    break;

  case 149:
#line 862 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7303 "bison-chapel.cpp"
    break;

  case 150:
#line 866 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 7309 "bison-chapel.cpp"
    break;

  case 151:
#line 867 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 7315 "bison-chapel.cpp"
    break;

  case 152:
#line 871 "chapel.ypp"
                        { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 7321 "bison-chapel.cpp"
    break;

  case 154:
#line 873 "chapel.ypp"
                        { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7327 "bison-chapel.cpp"
    break;

  case 155:
#line 883 "chapel.ypp"
           { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 7333 "bison-chapel.cpp"
    break;

  case 156:
#line 884 "chapel.ypp"
                 { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7339 "bison-chapel.cpp"
    break;

  case 164:
#line 898 "chapel.ypp"
                          { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 7345 "bison-chapel.cpp"
    break;

  case 165:
#line 899 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 7351 "bison-chapel.cpp"
    break;

  case 166:
#line 900 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 7357 "bison-chapel.cpp"
    break;

  case 167:
#line 901 "chapel.ypp"
                            { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 7363 "bison-chapel.cpp"
    break;

  case 168:
#line 906 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7372 "bison-chapel.cpp"
    break;

  case 169:
#line 911 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7386 "bison-chapel.cpp"
    break;

  case 170:
#line 921 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7395 "bison-chapel.cpp"
    break;

  case 171:
#line 926 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7410 "bison-chapel.cpp"
    break;

  case 172:
#line 937 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7419 "bison-chapel.cpp"
    break;

  case 173:
#line 942 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7433 "bison-chapel.cpp"
    break;

  case 174:
#line 952 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7442 "bison-chapel.cpp"
    break;

  case 175:
#line 957 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7456 "bison-chapel.cpp"
    break;

  case 176:
#line 968 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7464 "bison-chapel.cpp"
    break;

  case 177:
#line 972 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7472 "bison-chapel.cpp"
    break;

  case 178:
#line 977 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7484 "bison-chapel.cpp"
    break;

  case 179:
#line 985 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7496 "bison-chapel.cpp"
    break;

  case 180:
#line 996 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 7504 "bison-chapel.cpp"
    break;

  case 181:
#line 1002 "chapel.ypp"
                                                { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 7510 "bison-chapel.cpp"
    break;

  case 182:
#line 1003 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7516 "bison-chapel.cpp"
    break;

  case 183:
#line 1004 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 7522 "bison-chapel.cpp"
    break;

  case 184:
#line 1005 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 7528 "bison-chapel.cpp"
    break;

  case 185:
#line 1006 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 7534 "bison-chapel.cpp"
    break;

  case 186:
#line 1007 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7540 "bison-chapel.cpp"
    break;

  case 187:
#line 1008 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7546 "bison-chapel.cpp"
    break;

  case 188:
#line 1009 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7552 "bison-chapel.cpp"
    break;

  case 189:
#line 1010 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7558 "bison-chapel.cpp"
    break;

  case 190:
#line 1011 "chapel.ypp"
                                                { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7564 "bison-chapel.cpp"
    break;

  case 191:
#line 1012 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7570 "bison-chapel.cpp"
    break;

  case 192:
#line 1013 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7576 "bison-chapel.cpp"
    break;

  case 193:
#line 1014 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7582 "bison-chapel.cpp"
    break;

  case 194:
#line 1015 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7588 "bison-chapel.cpp"
    break;

  case 195:
#line 1016 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7594 "bison-chapel.cpp"
    break;

  case 196:
#line 1017 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7600 "bison-chapel.cpp"
    break;

  case 197:
#line 1018 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7606 "bison-chapel.cpp"
    break;

  case 198:
#line 1019 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7612 "bison-chapel.cpp"
    break;

  case 199:
#line 1021 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7622 "bison-chapel.cpp"
    break;

  case 200:
#line 1027 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7632 "bison-chapel.cpp"
    break;

  case 201:
#line 1033 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7642 "bison-chapel.cpp"
    break;

  case 202:
#line 1039 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7652 "bison-chapel.cpp"
    break;

  case 203:
#line 1045 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7663 "bison-chapel.cpp"
    break;

  case 204:
#line 1052 "chapel.ypp"
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7674 "bison-chapel.cpp"
    break;

  case 205:
#line 1059 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7682 "bison-chapel.cpp"
    break;

  case 206:
#line 1063 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7690 "bison-chapel.cpp"
    break;

  case 207:
#line 1069 "chapel.ypp"
                            { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 7696 "bison-chapel.cpp"
    break;

  case 208:
#line 1073 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7702 "bison-chapel.cpp"
    break;

  case 209:
#line 1074 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7708 "bison-chapel.cpp"
    break;

  case 210:
#line 1075 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7714 "bison-chapel.cpp"
    break;

  case 211:
#line 1076 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7720 "bison-chapel.cpp"
    break;

  case 212:
#line 1077 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7727 "bison-chapel.cpp"
    break;

  case 213:
#line 1079 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7734 "bison-chapel.cpp"
    break;

  case 214:
#line 1081 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7741 "bison-chapel.cpp"
    break;

  case 215:
#line 1083 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7748 "bison-chapel.cpp"
    break;

  case 216:
#line 1088 "chapel.ypp"
                          { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 7754 "bison-chapel.cpp"
    break;

  case 217:
#line 1091 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 7760 "bison-chapel.cpp"
    break;

  case 218:
#line 1092 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 7766 "bison-chapel.cpp"
    break;

  case 219:
#line 1093 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 7772 "bison-chapel.cpp"
    break;

  case 220:
#line 1094 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 7778 "bison-chapel.cpp"
    break;

  case 221:
#line 1095 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7784 "bison-chapel.cpp"
    break;

  case 222:
#line 1096 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7790 "bison-chapel.cpp"
    break;

  case 223:
#line 1100 "chapel.ypp"
                           { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7796 "bison-chapel.cpp"
    break;

  case 224:
#line 1101 "chapel.ypp"
                           { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7802 "bison-chapel.cpp"
    break;

  case 225:
#line 1105 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 7808 "bison-chapel.cpp"
    break;

  case 226:
#line 1106 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7814 "bison-chapel.cpp"
    break;

  case 227:
#line 1107 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7820 "bison-chapel.cpp"
    break;

  case 228:
#line 1111 "chapel.ypp"
                      { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 7826 "bison-chapel.cpp"
    break;

  case 229:
#line 1112 "chapel.ypp"
                        { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 7832 "bison-chapel.cpp"
    break;

  case 230:
#line 1116 "chapel.ypp"
                    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 7838 "bison-chapel.cpp"
    break;

  case 231:
#line 1120 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 7844 "bison-chapel.cpp"
    break;

  case 232:
#line 1122 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 7850 "bison-chapel.cpp"
    break;

  case 233:
#line 1126 "chapel.ypp"
                          { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7856 "bison-chapel.cpp"
    break;

  case 234:
#line 1127 "chapel.ypp"
                          { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7862 "bison-chapel.cpp"
    break;

  case 235:
#line 1132 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 7868 "bison-chapel.cpp"
    break;

  case 236:
#line 1134 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7874 "bison-chapel.cpp"
    break;

  case 237:
#line 1136 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7880 "bison-chapel.cpp"
    break;

  case 238:
#line 1143 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 7894 "bison-chapel.cpp"
    break;

  case 239:
#line 1153 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 7908 "bison-chapel.cpp"
    break;

  case 240:
#line 1166 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7918 "bison-chapel.cpp"
    break;

  case 241:
#line 1171 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7928 "bison-chapel.cpp"
    break;

  case 242:
#line 1176 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7938 "bison-chapel.cpp"
    break;

  case 243:
#line 1184 "chapel.ypp"
                  { (yyval.pcallexpr) = NULL; }
#line 7944 "bison-chapel.cpp"
    break;

  case 244:
#line 1185 "chapel.ypp"
                  { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 7950 "bison-chapel.cpp"
    break;

  case 245:
#line 1190 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 7956 "bison-chapel.cpp"
    break;

  case 246:
#line 1192 "chapel.ypp"
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7962 "bison-chapel.cpp"
    break;

  case 247:
#line 1194 "chapel.ypp"
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 7968 "bison-chapel.cpp"
    break;

  case 248:
#line 1199 "chapel.ypp"
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
#line 7988 "bison-chapel.cpp"
    break;

  case 249:
#line 1215 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 7996 "bison-chapel.cpp"
    break;

  case 250:
#line 1222 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 8006 "bison-chapel.cpp"
    break;

  case 251:
#line 1231 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 8018 "bison-chapel.cpp"
    break;

  case 252:
#line 1239 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 8026 "bison-chapel.cpp"
    break;

  case 253:
#line 1243 "chapel.ypp"
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 8034 "bison-chapel.cpp"
    break;

  case 254:
#line 1249 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 8040 "bison-chapel.cpp"
    break;

  case 255:
#line 1250 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 8046 "bison-chapel.cpp"
    break;

  case 256:
#line 1255 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 8055 "bison-chapel.cpp"
    break;

  case 257:
#line 1260 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8064 "bison-chapel.cpp"
    break;

  case 258:
#line 1265 "chapel.ypp"
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
#line 8086 "bison-chapel.cpp"
    break;

  case 259:
#line 1287 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 8094 "bison-chapel.cpp"
    break;

  case 260:
#line 1290 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 8103 "bison-chapel.cpp"
    break;

  case 261:
#line 1294 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 8112 "bison-chapel.cpp"
    break;

  case 262:
#line 1302 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 8126 "bison-chapel.cpp"
    break;

  case 263:
#line 1312 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8137 "bison-chapel.cpp"
    break;

  case 264:
#line 1319 "chapel.ypp"
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
#line 8164 "bison-chapel.cpp"
    break;

  case 265:
#line 1345 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 8172 "bison-chapel.cpp"
    break;

  case 266:
#line 1349 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8181 "bison-chapel.cpp"
    break;

  case 267:
#line 1354 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 8189 "bison-chapel.cpp"
    break;

  case 268:
#line 1358 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8198 "bison-chapel.cpp"
    break;

  case 269:
#line 1363 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 8206 "bison-chapel.cpp"
    break;

  case 271:
#line 1370 "chapel.ypp"
                      { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8212 "bison-chapel.cpp"
    break;

  case 272:
#line 1374 "chapel.ypp"
                  { (yyval.pch) = (yyvsp[0].pch); }
#line 8218 "bison-chapel.cpp"
    break;

  case 273:
#line 1375 "chapel.ypp"
                 { (yyval.pch) = "&"; }
#line 8224 "bison-chapel.cpp"
    break;

  case 274:
#line 1376 "chapel.ypp"
                 { (yyval.pch) = "|"; }
#line 8230 "bison-chapel.cpp"
    break;

  case 275:
#line 1377 "chapel.ypp"
                 { (yyval.pch) = "^"; }
#line 8236 "bison-chapel.cpp"
    break;

  case 276:
#line 1378 "chapel.ypp"
                 { (yyval.pch) = "~"; }
#line 8242 "bison-chapel.cpp"
    break;

  case 277:
#line 1379 "chapel.ypp"
                 { (yyval.pch) = "=="; }
#line 8248 "bison-chapel.cpp"
    break;

  case 278:
#line 1380 "chapel.ypp"
                 { (yyval.pch) = "!="; }
#line 8254 "bison-chapel.cpp"
    break;

  case 279:
#line 1381 "chapel.ypp"
                 { (yyval.pch) = "<="; }
#line 8260 "bison-chapel.cpp"
    break;

  case 280:
#line 1382 "chapel.ypp"
                 { (yyval.pch) = ">="; }
#line 8266 "bison-chapel.cpp"
    break;

  case 281:
#line 1383 "chapel.ypp"
                 { (yyval.pch) = "<"; }
#line 8272 "bison-chapel.cpp"
    break;

  case 282:
#line 1384 "chapel.ypp"
                 { (yyval.pch) = ">"; }
#line 8278 "bison-chapel.cpp"
    break;

  case 283:
#line 1385 "chapel.ypp"
                 { (yyval.pch) = "+"; }
#line 8284 "bison-chapel.cpp"
    break;

  case 284:
#line 1386 "chapel.ypp"
                 { (yyval.pch) = "-"; }
#line 8290 "bison-chapel.cpp"
    break;

  case 285:
#line 1387 "chapel.ypp"
                 { (yyval.pch) = "*"; }
#line 8296 "bison-chapel.cpp"
    break;

  case 286:
#line 1388 "chapel.ypp"
                 { (yyval.pch) = "/"; }
#line 8302 "bison-chapel.cpp"
    break;

  case 287:
#line 1389 "chapel.ypp"
                 { (yyval.pch) = "<<"; }
#line 8308 "bison-chapel.cpp"
    break;

  case 288:
#line 1390 "chapel.ypp"
                 { (yyval.pch) = ">>"; }
#line 8314 "bison-chapel.cpp"
    break;

  case 289:
#line 1391 "chapel.ypp"
                 { (yyval.pch) = "%"; }
#line 8320 "bison-chapel.cpp"
    break;

  case 290:
#line 1392 "chapel.ypp"
                 { (yyval.pch) = "**"; }
#line 8326 "bison-chapel.cpp"
    break;

  case 291:
#line 1393 "chapel.ypp"
                 { (yyval.pch) = "!"; }
#line 8332 "bison-chapel.cpp"
    break;

  case 292:
#line 1394 "chapel.ypp"
                 { (yyval.pch) = "chpl_by"; }
#line 8338 "bison-chapel.cpp"
    break;

  case 293:
#line 1395 "chapel.ypp"
                 { (yyval.pch) = "#"; }
#line 8344 "bison-chapel.cpp"
    break;

  case 294:
#line 1396 "chapel.ypp"
                 { (yyval.pch) = "chpl_align"; }
#line 8350 "bison-chapel.cpp"
    break;

  case 295:
#line 1397 "chapel.ypp"
                 { (yyval.pch) = "<=>"; }
#line 8356 "bison-chapel.cpp"
    break;

  case 296:
#line 1398 "chapel.ypp"
                 { (yyval.pch) = "<~>"; }
#line 8362 "bison-chapel.cpp"
    break;

  case 297:
#line 1399 "chapel.ypp"
                 { (yyval.pch) = "init="; }
#line 8368 "bison-chapel.cpp"
    break;

  case 298:
#line 1400 "chapel.ypp"
                  { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 8374 "bison-chapel.cpp"
    break;

  case 299:
#line 1404 "chapel.ypp"
                 { (yyval.pch) = "="; }
#line 8380 "bison-chapel.cpp"
    break;

  case 300:
#line 1405 "chapel.ypp"
                 { (yyval.pch) = "+="; }
#line 8386 "bison-chapel.cpp"
    break;

  case 301:
#line 1406 "chapel.ypp"
                 { (yyval.pch) = "-="; }
#line 8392 "bison-chapel.cpp"
    break;

  case 302:
#line 1407 "chapel.ypp"
                  { (yyval.pch) = "*="; }
#line 8398 "bison-chapel.cpp"
    break;

  case 303:
#line 1408 "chapel.ypp"
                 { (yyval.pch) = "/="; }
#line 8404 "bison-chapel.cpp"
    break;

  case 304:
#line 1409 "chapel.ypp"
                 { (yyval.pch) = "%="; }
#line 8410 "bison-chapel.cpp"
    break;

  case 305:
#line 1410 "chapel.ypp"
                 { (yyval.pch) = "**="; }
#line 8416 "bison-chapel.cpp"
    break;

  case 306:
#line 1411 "chapel.ypp"
                 { (yyval.pch) = "&="; }
#line 8422 "bison-chapel.cpp"
    break;

  case 307:
#line 1412 "chapel.ypp"
                 { (yyval.pch) = "|="; }
#line 8428 "bison-chapel.cpp"
    break;

  case 308:
#line 1413 "chapel.ypp"
                 { (yyval.pch) = "^="; }
#line 8434 "bison-chapel.cpp"
    break;

  case 309:
#line 1414 "chapel.ypp"
                 { (yyval.pch) = ">>="; }
#line 8440 "bison-chapel.cpp"
    break;

  case 310:
#line 1415 "chapel.ypp"
                 { (yyval.pch) = "<<="; }
#line 8446 "bison-chapel.cpp"
    break;

  case 311:
#line 1419 "chapel.ypp"
                     { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 8452 "bison-chapel.cpp"
    break;

  case 312:
#line 1420 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8458 "bison-chapel.cpp"
    break;

  case 313:
#line 1424 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8464 "bison-chapel.cpp"
    break;

  case 314:
#line 1428 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 8470 "bison-chapel.cpp"
    break;

  case 315:
#line 1429 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 8476 "bison-chapel.cpp"
    break;

  case 316:
#line 1430 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 8482 "bison-chapel.cpp"
    break;

  case 317:
#line 1435 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 8488 "bison-chapel.cpp"
    break;

  case 318:
#line 1437 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 8494 "bison-chapel.cpp"
    break;

  case 319:
#line 1439 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 8500 "bison-chapel.cpp"
    break;

  case 320:
#line 1441 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr))); }
#line 8506 "bison-chapel.cpp"
    break;

  case 321:
#line 1443 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8512 "bison-chapel.cpp"
    break;

  case 322:
#line 1445 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 8518 "bison-chapel.cpp"
    break;

  case 323:
#line 1449 "chapel.ypp"
                      { (yyval.pt) = INTENT_BLANK; }
#line 8524 "bison-chapel.cpp"
    break;

  case 324:
#line 1450 "chapel.ypp"
                      { (yyval.pt) = (yyvsp[0].pt); }
#line 8530 "bison-chapel.cpp"
    break;

  case 325:
#line 1454 "chapel.ypp"
          { (yyval.pt) = INTENT_IN; }
#line 8536 "bison-chapel.cpp"
    break;

  case 326:
#line 1455 "chapel.ypp"
          { (yyval.pt) = INTENT_INOUT; }
#line 8542 "bison-chapel.cpp"
    break;

  case 327:
#line 1456 "chapel.ypp"
          { (yyval.pt) = INTENT_OUT; }
#line 8548 "bison-chapel.cpp"
    break;

  case 328:
#line 1457 "chapel.ypp"
          { (yyval.pt) = INTENT_CONST; }
#line 8554 "bison-chapel.cpp"
    break;

  case 329:
#line 1458 "chapel.ypp"
             { (yyval.pt) = INTENT_CONST_IN; }
#line 8560 "bison-chapel.cpp"
    break;

  case 330:
#line 1459 "chapel.ypp"
              { (yyval.pt) = INTENT_CONST_REF; }
#line 8566 "bison-chapel.cpp"
    break;

  case 331:
#line 1460 "chapel.ypp"
          { (yyval.pt) = INTENT_PARAM; }
#line 8572 "bison-chapel.cpp"
    break;

  case 332:
#line 1461 "chapel.ypp"
          { (yyval.pt) = INTENT_REF; }
#line 8578 "bison-chapel.cpp"
    break;

  case 333:
#line 1462 "chapel.ypp"
          { (yyval.pt) = INTENT_TYPE; }
#line 8584 "bison-chapel.cpp"
    break;

  case 334:
#line 1466 "chapel.ypp"
         { (yyval.pt) = INTENT_BLANK; }
#line 8590 "bison-chapel.cpp"
    break;

  case 335:
#line 1467 "chapel.ypp"
         { (yyval.pt) = INTENT_PARAM; }
#line 8596 "bison-chapel.cpp"
    break;

  case 336:
#line 1468 "chapel.ypp"
         { (yyval.pt) = INTENT_REF;   }
#line 8602 "bison-chapel.cpp"
    break;

  case 337:
#line 1469 "chapel.ypp"
                { (yyval.pt) = INTENT_CONST_REF;   }
#line 8608 "bison-chapel.cpp"
    break;

  case 338:
#line 1470 "chapel.ypp"
         { (yyval.pt) = INTENT_CONST;   }
#line 8614 "bison-chapel.cpp"
    break;

  case 339:
#line 1471 "chapel.ypp"
         { (yyval.pt) = INTENT_TYPE;  }
#line 8620 "bison-chapel.cpp"
    break;

  case 340:
#line 1475 "chapel.ypp"
         { (yyval.procIter) = ProcIter_PROC; }
#line 8626 "bison-chapel.cpp"
    break;

  case 341:
#line 1476 "chapel.ypp"
         { (yyval.procIter) = ProcIter_ITER; }
#line 8632 "bison-chapel.cpp"
    break;

  case 342:
#line 1480 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 8638 "bison-chapel.cpp"
    break;

  case 343:
#line 1481 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 8644 "bison-chapel.cpp"
    break;

  case 344:
#line 1482 "chapel.ypp"
              { (yyval.retTag) = RET_CONST_REF; }
#line 8650 "bison-chapel.cpp"
    break;

  case 345:
#line 1483 "chapel.ypp"
              { (yyval.retTag) = RET_REF; }
#line 8656 "bison-chapel.cpp"
    break;

  case 346:
#line 1484 "chapel.ypp"
              { (yyval.retTag) = RET_PARAM; }
#line 8662 "bison-chapel.cpp"
    break;

  case 347:
#line 1485 "chapel.ypp"
              { (yyval.retTag) = RET_TYPE; }
#line 8668 "bison-chapel.cpp"
    break;

  case 348:
#line 1489 "chapel.ypp"
          { (yyval.b) = false; }
#line 8674 "bison-chapel.cpp"
    break;

  case 349:
#line 1490 "chapel.ypp"
          { (yyval.b) = true;  }
#line 8680 "bison-chapel.cpp"
    break;

  case 350:
#line 1493 "chapel.ypp"
            { (yyval.pblockstmt) = NULL; }
#line 8686 "bison-chapel.cpp"
    break;

  case 353:
#line 1499 "chapel.ypp"
               { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 8692 "bison-chapel.cpp"
    break;

  case 354:
#line 1503 "chapel.ypp"
                      { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 8698 "bison-chapel.cpp"
    break;

  case 355:
#line 1507 "chapel.ypp"
                         { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 8704 "bison-chapel.cpp"
    break;

  case 356:
#line 1508 "chapel.ypp"
                         { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8710 "bison-chapel.cpp"
    break;

  case 357:
#line 1509 "chapel.ypp"
                         { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 8720 "bison-chapel.cpp"
    break;

  case 358:
#line 1517 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 8726 "bison-chapel.cpp"
    break;

  case 359:
#line 1519 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 8732 "bison-chapel.cpp"
    break;

  case 360:
#line 1521 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 8738 "bison-chapel.cpp"
    break;

  case 361:
#line 1523 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8744 "bison-chapel.cpp"
    break;

  case 362:
#line 1525 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8750 "bison-chapel.cpp"
    break;

  case 363:
#line 1530 "chapel.ypp"
  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8756 "bison-chapel.cpp"
    break;

  case 364:
#line 1532 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8762 "bison-chapel.cpp"
    break;

  case 365:
#line 1536 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8768 "bison-chapel.cpp"
    break;

  case 366:
#line 1537 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8774 "bison-chapel.cpp"
    break;

  case 367:
#line 1538 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8780 "bison-chapel.cpp"
    break;

  case 368:
#line 1539 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8786 "bison-chapel.cpp"
    break;

  case 369:
#line 1540 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8792 "bison-chapel.cpp"
    break;

  case 370:
#line 1541 "chapel.ypp"
                                              {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8798 "bison-chapel.cpp"
    break;

  case 371:
#line 1542 "chapel.ypp"
                         { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 8804 "bison-chapel.cpp"
    break;

  case 372:
#line 1547 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 8810 "bison-chapel.cpp"
    break;

  case 373:
#line 1549 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 8816 "bison-chapel.cpp"
    break;

  case 374:
#line 1554 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 8822 "bison-chapel.cpp"
    break;

  case 375:
#line 1556 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 8828 "bison-chapel.cpp"
    break;

  case 376:
#line 1558 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 8834 "bison-chapel.cpp"
    break;

  case 377:
#line 1563 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 8851 "bison-chapel.cpp"
    break;

  case 378:
#line 1576 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 8869 "bison-chapel.cpp"
    break;

  case 379:
#line 1592 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
#line 8875 "bison-chapel.cpp"
    break;

  case 380:
#line 1594 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8881 "bison-chapel.cpp"
    break;

  case 381:
#line 1596 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 8887 "bison-chapel.cpp"
    break;

  case 382:
#line 1600 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 8893 "bison-chapel.cpp"
    break;

  case 383:
#line 1601 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 8899 "bison-chapel.cpp"
    break;

  case 384:
#line 1602 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 8905 "bison-chapel.cpp"
    break;

  case 385:
#line 1603 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 8911 "bison-chapel.cpp"
    break;

  case 386:
#line 1604 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(); }
#line 8917 "bison-chapel.cpp"
    break;

  case 387:
#line 1609 "chapel.ypp"
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8927 "bison-chapel.cpp"
    break;

  case 388:
#line 1615 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8936 "bison-chapel.cpp"
    break;

  case 390:
#line 1624 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 8945 "bison-chapel.cpp"
    break;

  case 391:
#line 1632 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 8951 "bison-chapel.cpp"
    break;

  case 392:
#line 1634 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8957 "bison-chapel.cpp"
    break;

  case 393:
#line 1639 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 8963 "bison-chapel.cpp"
    break;

  case 394:
#line 1641 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 8969 "bison-chapel.cpp"
    break;

  case 395:
#line 1643 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 8975 "bison-chapel.cpp"
    break;

  case 396:
#line 1648 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 8981 "bison-chapel.cpp"
    break;

  case 397:
#line 1650 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 8987 "bison-chapel.cpp"
    break;

  case 398:
#line 1652 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8993 "bison-chapel.cpp"
    break;

  case 399:
#line 1658 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 8999 "bison-chapel.cpp"
    break;

  case 400:
#line 1659 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 9005 "bison-chapel.cpp"
    break;

  case 401:
#line 1660 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9011 "bison-chapel.cpp"
    break;

  case 402:
#line 1666 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9017 "bison-chapel.cpp"
    break;

  case 403:
#line 1668 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 9023 "bison-chapel.cpp"
    break;

  case 404:
#line 1670 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9031 "bison-chapel.cpp"
    break;

  case 405:
#line 1674 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 9039 "bison-chapel.cpp"
    break;

  case 406:
#line 1678 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9045 "bison-chapel.cpp"
    break;

  case 407:
#line 1680 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9053 "bison-chapel.cpp"
    break;

  case 408:
#line 1684 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9061 "bison-chapel.cpp"
    break;

  case 409:
#line 1691 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9067 "bison-chapel.cpp"
    break;

  case 410:
#line 1692 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9073 "bison-chapel.cpp"
    break;

  case 411:
#line 1693 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9079 "bison-chapel.cpp"
    break;

  case 412:
#line 1694 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9085 "bison-chapel.cpp"
    break;

  case 413:
#line 1695 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9091 "bison-chapel.cpp"
    break;

  case 414:
#line 1700 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9097 "bison-chapel.cpp"
    break;

  case 415:
#line 1701 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9103 "bison-chapel.cpp"
    break;

  case 416:
#line 1702 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 9109 "bison-chapel.cpp"
    break;

  case 417:
#line 1703 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9115 "bison-chapel.cpp"
    break;

  case 418:
#line 1704 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9121 "bison-chapel.cpp"
    break;

  case 419:
#line 1725 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9129 "bison-chapel.cpp"
    break;

  case 420:
#line 1729 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 9137 "bison-chapel.cpp"
    break;

  case 421:
#line 1733 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 9149 "bison-chapel.cpp"
    break;

  case 422:
#line 1741 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9157 "bison-chapel.cpp"
    break;

  case 423:
#line 1747 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9163 "bison-chapel.cpp"
    break;

  case 424:
#line 1748 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9169 "bison-chapel.cpp"
    break;

  case 425:
#line 1749 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9175 "bison-chapel.cpp"
    break;

  case 426:
#line 1754 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9181 "bison-chapel.cpp"
    break;

  case 427:
#line 1756 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9187 "bison-chapel.cpp"
    break;

  case 428:
#line 1762 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9193 "bison-chapel.cpp"
    break;

  case 429:
#line 1764 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9199 "bison-chapel.cpp"
    break;

  case 430:
#line 1766 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 9205 "bison-chapel.cpp"
    break;

  case 431:
#line 1770 "chapel.ypp"
                            { (yyval.pexpr) = NULL; }
#line 9211 "bison-chapel.cpp"
    break;

  case 432:
#line 1771 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9217 "bison-chapel.cpp"
    break;

  case 433:
#line 1772 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9223 "bison-chapel.cpp"
    break;

  case 434:
#line 1773 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9229 "bison-chapel.cpp"
    break;

  case 435:
#line 1774 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9235 "bison-chapel.cpp"
    break;

  case 436:
#line 1780 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9241 "bison-chapel.cpp"
    break;

  case 437:
#line 1781 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9247 "bison-chapel.cpp"
    break;

  case 438:
#line 1782 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9253 "bison-chapel.cpp"
    break;

  case 439:
#line 1783 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9259 "bison-chapel.cpp"
    break;

  case 440:
#line 1787 "chapel.ypp"
                                   { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 9265 "bison-chapel.cpp"
    break;

  case 441:
#line 1788 "chapel.ypp"
                                   { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9271 "bison-chapel.cpp"
    break;

  case 442:
#line 1792 "chapel.ypp"
                { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 9277 "bison-chapel.cpp"
    break;

  case 443:
#line 1793 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9283 "bison-chapel.cpp"
    break;

  case 444:
#line 1794 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9289 "bison-chapel.cpp"
    break;

  case 445:
#line 1798 "chapel.ypp"
                                         { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9295 "bison-chapel.cpp"
    break;

  case 446:
#line 1799 "chapel.ypp"
                                       { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9301 "bison-chapel.cpp"
    break;

  case 447:
#line 1803 "chapel.ypp"
             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 9307 "bison-chapel.cpp"
    break;

  case 449:
#line 1808 "chapel.ypp"
                                { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9313 "bison-chapel.cpp"
    break;

  case 450:
#line 1809 "chapel.ypp"
                                { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9319 "bison-chapel.cpp"
    break;

  case 451:
#line 1813 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9325 "bison-chapel.cpp"
    break;

  case 452:
#line 1814 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9331 "bison-chapel.cpp"
    break;

  case 453:
#line 1815 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9337 "bison-chapel.cpp"
    break;

  case 454:
#line 1816 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9343 "bison-chapel.cpp"
    break;

  case 455:
#line 1820 "chapel.ypp"
                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9349 "bison-chapel.cpp"
    break;

  case 456:
#line 1821 "chapel.ypp"
                 { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9355 "bison-chapel.cpp"
    break;

  case 457:
#line 1833 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9361 "bison-chapel.cpp"
    break;

  case 458:
#line 1835 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 9367 "bison-chapel.cpp"
    break;

  case 459:
#line 1837 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9373 "bison-chapel.cpp"
    break;

  case 465:
#line 1848 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 9379 "bison-chapel.cpp"
    break;

  case 466:
#line 1850 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 9385 "bison-chapel.cpp"
    break;

  case 467:
#line 1852 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 9391 "bison-chapel.cpp"
    break;

  case 468:
#line 1854 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 9397 "bison-chapel.cpp"
    break;

  case 469:
#line 1856 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 9403 "bison-chapel.cpp"
    break;

  case 470:
#line 1858 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 9409 "bison-chapel.cpp"
    break;

  case 471:
#line 1860 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 9415 "bison-chapel.cpp"
    break;

  case 472:
#line 1863 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 9421 "bison-chapel.cpp"
    break;

  case 473:
#line 1865 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 9427 "bison-chapel.cpp"
    break;

  case 474:
#line 1867 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 9433 "bison-chapel.cpp"
    break;

  case 475:
#line 1869 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9439 "bison-chapel.cpp"
    break;

  case 476:
#line 1871 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 9445 "bison-chapel.cpp"
    break;

  case 477:
#line 1873 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 9451 "bison-chapel.cpp"
    break;

  case 478:
#line 1875 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 9457 "bison-chapel.cpp"
    break;

  case 479:
#line 1877 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9463 "bison-chapel.cpp"
    break;

  case 480:
#line 1880 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 9469 "bison-chapel.cpp"
    break;

  case 481:
#line 1882 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 9475 "bison-chapel.cpp"
    break;

  case 482:
#line 1887 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9481 "bison-chapel.cpp"
    break;

  case 483:
#line 1889 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 9487 "bison-chapel.cpp"
    break;

  case 484:
#line 1891 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9493 "bison-chapel.cpp"
    break;

  case 485:
#line 1893 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9499 "bison-chapel.cpp"
    break;

  case 486:
#line 1895 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 9505 "bison-chapel.cpp"
    break;

  case 487:
#line 1897 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9511 "bison-chapel.cpp"
    break;

  case 488:
#line 1899 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9517 "bison-chapel.cpp"
    break;

  case 489:
#line 1901 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 9523 "bison-chapel.cpp"
    break;

  case 490:
#line 1903 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9529 "bison-chapel.cpp"
    break;

  case 491:
#line 1905 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9535 "bison-chapel.cpp"
    break;

  case 492:
#line 1907 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 9541 "bison-chapel.cpp"
    break;

  case 493:
#line 1909 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9547 "bison-chapel.cpp"
    break;

  case 494:
#line 1911 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 9558 "bison-chapel.cpp"
    break;

  case 495:
#line 1918 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 9568 "bison-chapel.cpp"
    break;

  case 496:
#line 1924 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 9578 "bison-chapel.cpp"
    break;

  case 497:
#line 1930 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9588 "bison-chapel.cpp"
    break;

  case 498:
#line 1936 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 9598 "bison-chapel.cpp"
    break;

  case 499:
#line 1945 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9604 "bison-chapel.cpp"
    break;

  case 500:
#line 1954 "chapel.ypp"
            { (yyval.pexpr) = new SymExpr(gNil); }
#line 9610 "bison-chapel.cpp"
    break;

  case 508:
#line 1970 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9616 "bison-chapel.cpp"
    break;

  case 509:
#line 1974 "chapel.ypp"
                                { (yyval.pcallexpr) = NULL; }
#line 9622 "bison-chapel.cpp"
    break;

  case 511:
#line 1979 "chapel.ypp"
                                { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9628 "bison-chapel.cpp"
    break;

  case 512:
#line 1983 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9634 "bison-chapel.cpp"
    break;

  case 513:
#line 1984 "chapel.ypp"
                                                    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9640 "bison-chapel.cpp"
    break;

  case 514:
#line 1988 "chapel.ypp"
                                  { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9646 "bison-chapel.cpp"
    break;

  case 515:
#line 1992 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9652 "bison-chapel.cpp"
    break;

  case 516:
#line 1993 "chapel.ypp"
                                                    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9658 "bison-chapel.cpp"
    break;

  case 517:
#line 1998 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 9666 "bison-chapel.cpp"
    break;

  case 518:
#line 2002 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9674 "bison-chapel.cpp"
    break;

  case 519:
#line 2006 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9682 "bison-chapel.cpp"
    break;

  case 520:
#line 2012 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 9688 "bison-chapel.cpp"
    break;

  case 521:
#line 2013 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_IN;        }
#line 9694 "bison-chapel.cpp"
    break;

  case 522:
#line 2014 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 9700 "bison-chapel.cpp"
    break;

  case 523:
#line 2015 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_REF;       }
#line 9706 "bison-chapel.cpp"
    break;

  case 524:
#line 2016 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 9712 "bison-chapel.cpp"
    break;

  case 525:
#line 2017 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 9718 "bison-chapel.cpp"
    break;

  case 527:
#line 2023 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9724 "bison-chapel.cpp"
    break;

  case 528:
#line 2028 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 9730 "bison-chapel.cpp"
    break;

  case 529:
#line 2030 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 9738 "bison-chapel.cpp"
    break;

  case 530:
#line 2034 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 9746 "bison-chapel.cpp"
    break;

  case 531:
#line 2038 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 9754 "bison-chapel.cpp"
    break;

  case 532:
#line 2042 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 9762 "bison-chapel.cpp"
    break;

  case 533:
#line 2050 "chapel.ypp"
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 9769 "bison-chapel.cpp"
    break;

  case 534:
#line 2055 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9779 "bison-chapel.cpp"
    break;

  case 535:
#line 2061 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9789 "bison-chapel.cpp"
    break;

  case 536:
#line 2067 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9800 "bison-chapel.cpp"
    break;

  case 537:
#line 2074 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9811 "bison-chapel.cpp"
    break;

  case 538:
#line 2084 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 9817 "bison-chapel.cpp"
    break;

  case 547:
#line 2100 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 9823 "bison-chapel.cpp"
    break;

  case 548:
#line 2102 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9829 "bison-chapel.cpp"
    break;

  case 549:
#line 2104 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9835 "bison-chapel.cpp"
    break;

  case 550:
#line 2106 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr), false, true); }
#line 9841 "bison-chapel.cpp"
    break;

  case 551:
#line 2121 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[-1].pexpr)); }
#line 9847 "bison-chapel.cpp"
    break;

  case 552:
#line 2123 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr)); }
#line 9853 "bison-chapel.cpp"
    break;

  case 553:
#line 2125 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr), true); }
#line 9859 "bison-chapel.cpp"
    break;

  case 554:
#line 2127 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
#line 9865 "bison-chapel.cpp"
    break;

  case 555:
#line 2131 "chapel.ypp"
                  { (yyval.pexpr) = NULL; }
#line 9871 "bison-chapel.cpp"
    break;

  case 556:
#line 2132 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9877 "bison-chapel.cpp"
    break;

  case 557:
#line 2136 "chapel.ypp"
                  { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 9883 "bison-chapel.cpp"
    break;

  case 558:
#line 2137 "chapel.ypp"
                  { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 9889 "bison-chapel.cpp"
    break;

  case 559:
#line 2138 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9895 "bison-chapel.cpp"
    break;

  case 564:
#line 2156 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9901 "bison-chapel.cpp"
    break;

  case 565:
#line 2157 "chapel.ypp"
                     { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9907 "bison-chapel.cpp"
    break;

  case 566:
#line 2158 "chapel.ypp"
                                {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 9913 "bison-chapel.cpp"
    break;

  case 567:
#line 2159 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9919 "bison-chapel.cpp"
    break;

  case 568:
#line 2163 "chapel.ypp"
                                        { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9925 "bison-chapel.cpp"
    break;

  case 569:
#line 2164 "chapel.ypp"
                                        { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9931 "bison-chapel.cpp"
    break;

  case 570:
#line 2165 "chapel.ypp"
                                        { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 9937 "bison-chapel.cpp"
    break;

  case 571:
#line 2169 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 9943 "bison-chapel.cpp"
    break;

  case 572:
#line 2170 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 9949 "bison-chapel.cpp"
    break;

  case 573:
#line 2171 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 9955 "bison-chapel.cpp"
    break;

  case 574:
#line 2172 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 9961 "bison-chapel.cpp"
    break;

  case 575:
#line 2173 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 9967 "bison-chapel.cpp"
    break;

  case 576:
#line 2181 "chapel.ypp"
                                    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 9973 "bison-chapel.cpp"
    break;

  case 577:
#line 2182 "chapel.ypp"
                                    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 9979 "bison-chapel.cpp"
    break;

  case 578:
#line 2183 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 9985 "bison-chapel.cpp"
    break;

  case 579:
#line 2184 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 9991 "bison-chapel.cpp"
    break;

  case 580:
#line 2188 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gFalse); }
#line 9997 "bison-chapel.cpp"
    break;

  case 581:
#line 2189 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gTrue); }
#line 10003 "bison-chapel.cpp"
    break;

  case 583:
#line 2194 "chapel.ypp"
                        { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 10009 "bison-chapel.cpp"
    break;

  case 584:
#line 2195 "chapel.ypp"
                        { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 10015 "bison-chapel.cpp"
    break;

  case 585:
#line 2196 "chapel.ypp"
                        { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 10021 "bison-chapel.cpp"
    break;

  case 586:
#line 2197 "chapel.ypp"
                        { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 10027 "bison-chapel.cpp"
    break;

  case 587:
#line 2198 "chapel.ypp"
                        { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 10033 "bison-chapel.cpp"
    break;

  case 588:
#line 2199 "chapel.ypp"
                        { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 10039 "bison-chapel.cpp"
    break;

  case 589:
#line 2200 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNone); }
#line 10045 "bison-chapel.cpp"
    break;

  case 590:
#line 2201 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr),
                                            new SymExpr(gTrue)); }
#line 10052 "bison-chapel.cpp"
    break;

  case 591:
#line 2203 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr),
                                                   new SymExpr(gTrue)); }
#line 10059 "bison-chapel.cpp"
    break;

  case 592:
#line 2205 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 10065 "bison-chapel.cpp"
    break;

  case 593:
#line 2206 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 10071 "bison-chapel.cpp"
    break;

  case 594:
#line 2208 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 10079 "bison-chapel.cpp"
    break;

  case 595:
#line 2212 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 10087 "bison-chapel.cpp"
    break;

  case 596:
#line 2219 "chapel.ypp"
                                        { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10093 "bison-chapel.cpp"
    break;

  case 597:
#line 2220 "chapel.ypp"
                                        { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 10099 "bison-chapel.cpp"
    break;

  case 598:
#line 2224 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10105 "bison-chapel.cpp"
    break;

  case 599:
#line 2225 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10111 "bison-chapel.cpp"
    break;

  case 600:
#line 2226 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10117 "bison-chapel.cpp"
    break;

  case 601:
#line 2227 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10123 "bison-chapel.cpp"
    break;

  case 602:
#line 2228 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10129 "bison-chapel.cpp"
    break;

  case 603:
#line 2229 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10135 "bison-chapel.cpp"
    break;

  case 604:
#line 2230 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10141 "bison-chapel.cpp"
    break;

  case 605:
#line 2231 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10147 "bison-chapel.cpp"
    break;

  case 606:
#line 2232 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10153 "bison-chapel.cpp"
    break;

  case 607:
#line 2233 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10159 "bison-chapel.cpp"
    break;

  case 608:
#line 2234 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10165 "bison-chapel.cpp"
    break;

  case 609:
#line 2235 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10171 "bison-chapel.cpp"
    break;

  case 610:
#line 2236 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10177 "bison-chapel.cpp"
    break;

  case 611:
#line 2237 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10183 "bison-chapel.cpp"
    break;

  case 612:
#line 2238 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10189 "bison-chapel.cpp"
    break;

  case 613:
#line 2239 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10195 "bison-chapel.cpp"
    break;

  case 614:
#line 2240 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10201 "bison-chapel.cpp"
    break;

  case 615:
#line 2241 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10207 "bison-chapel.cpp"
    break;

  case 616:
#line 2242 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10213 "bison-chapel.cpp"
    break;

  case 617:
#line 2243 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10219 "bison-chapel.cpp"
    break;

  case 618:
#line 2244 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10225 "bison-chapel.cpp"
    break;

  case 619:
#line 2245 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10231 "bison-chapel.cpp"
    break;

  case 620:
#line 2246 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr),
                                               new SymExpr(gTrue)); }
#line 10238 "bison-chapel.cpp"
    break;

  case 621:
#line 2251 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 10244 "bison-chapel.cpp"
    break;

  case 622:
#line 2252 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 10250 "bison-chapel.cpp"
    break;

  case 623:
#line 2253 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 10256 "bison-chapel.cpp"
    break;

  case 624:
#line 2254 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 10262 "bison-chapel.cpp"
    break;

  case 625:
#line 2255 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 10268 "bison-chapel.cpp"
    break;

  case 626:
#line 2256 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10274 "bison-chapel.cpp"
    break;

  case 627:
#line 2257 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 10280 "bison-chapel.cpp"
    break;

  case 628:
#line 2261 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10286 "bison-chapel.cpp"
    break;

  case 629:
#line 2262 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10292 "bison-chapel.cpp"
    break;

  case 630:
#line 2263 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10298 "bison-chapel.cpp"
    break;

  case 631:
#line 2264 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10304 "bison-chapel.cpp"
    break;

  case 632:
#line 2268 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10310 "bison-chapel.cpp"
    break;

  case 633:
#line 2269 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10316 "bison-chapel.cpp"
    break;

  case 634:
#line 2270 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10322 "bison-chapel.cpp"
    break;

  case 635:
#line 2271 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10328 "bison-chapel.cpp"
    break;

  case 636:
#line 2276 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 10334 "bison-chapel.cpp"
    break;

  case 637:
#line 2277 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 10340 "bison-chapel.cpp"
    break;

  case 638:
#line 2278 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 10346 "bison-chapel.cpp"
    break;

  case 639:
#line 2279 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 10352 "bison-chapel.cpp"
    break;

  case 640:
#line 2280 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 10358 "bison-chapel.cpp"
    break;

  case 641:
#line 2281 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 10364 "bison-chapel.cpp"
    break;

  case 642:
#line 2282 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 10370 "bison-chapel.cpp"
    break;


#line 10374 "bison-chapel.cpp"

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
