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

#line 422 "../include/bison-chapel.h"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
