/* A Bison parser, made by GNU Bison 3.6.4.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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

#line 194 "../include/bison-chapel.h"

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

#line 424 "../include/bison-chapel.h"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
