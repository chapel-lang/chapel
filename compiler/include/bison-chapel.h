/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

  enum   ProcIterOp {
    ProcIterOp_PROC,
    ProcIterOp_ITER,
    ProcIterOp_OP
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
    ProcIterOp                procIterOp;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<PotentialRename*>* ponlylist;
    std::set<Flag>*           pflagset;
    WhereAndLifetime          lifetimeAndWhere;
  };

  // Communicate to Bison that our YYSTYPE only contains simple scalars
  // and can be bitcopied. We do this because we do not use the '%union'
  // directive.
  #define YYSTYPE_IS_TRIVIAL 1

  #endif
#line 157 "chapel.ypp"

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
#line 180 "chapel.ypp"

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

#line 200 "../include/bison-chapel.h"

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
    TDEPRECATED = 286,             /* TDEPRECATED  */
    TDMAPPED = 287,                /* TDMAPPED  */
    TDO = 288,                     /* TDO  */
    TDOMAIN = 289,                 /* TDOMAIN  */
    TELSE = 290,                   /* TELSE  */
    TENUM = 291,                   /* TENUM  */
    TEXCEPT = 292,                 /* TEXCEPT  */
    TEXPORT = 293,                 /* TEXPORT  */
    TEXTERN = 294,                 /* TEXTERN  */
    TFALSE = 295,                  /* TFALSE  */
    TFOR = 296,                    /* TFOR  */
    TFORALL = 297,                 /* TFORALL  */
    TFOREACH = 298,                /* TFOREACH  */
    TFORWARDING = 299,             /* TFORWARDING  */
    TIF = 300,                     /* TIF  */
    TIMAG = 301,                   /* TIMAG  */
    TIMPORT = 302,                 /* TIMPORT  */
    TIN = 303,                     /* TIN  */
    TINCLUDE = 304,                /* TINCLUDE  */
    TINDEX = 305,                  /* TINDEX  */
    TINLINE = 306,                 /* TINLINE  */
    TINOUT = 307,                  /* TINOUT  */
    TINT = 308,                    /* TINT  */
    TITER = 309,                   /* TITER  */
    TINITEQUALS = 310,             /* TINITEQUALS  */
    TIMPLEMENTS = 311,             /* TIMPLEMENTS  */
    TINTERFACE = 312,              /* TINTERFACE  */
    TLABEL = 313,                  /* TLABEL  */
    TLAMBDA = 314,                 /* TLAMBDA  */
    TLET = 315,                    /* TLET  */
    TLIFETIME = 316,               /* TLIFETIME  */
    TLOCAL = 317,                  /* TLOCAL  */
    TLOCALE = 318,                 /* TLOCALE  */
    TMANAGE = 319,                 /* TMANAGE  */
    TMINUSMINUS = 320,             /* TMINUSMINUS  */
    TMODULE = 321,                 /* TMODULE  */
    TNEW = 322,                    /* TNEW  */
    TNIL = 323,                    /* TNIL  */
    TNOINIT = 324,                 /* TNOINIT  */
    TNONE = 325,                   /* TNONE  */
    TNOTHING = 326,                /* TNOTHING  */
    TON = 327,                     /* TON  */
    TONLY = 328,                   /* TONLY  */
    TOPERATOR = 329,               /* TOPERATOR  */
    TOTHERWISE = 330,              /* TOTHERWISE  */
    TOUT = 331,                    /* TOUT  */
    TOVERRIDE = 332,               /* TOVERRIDE  */
    TOWNED = 333,                  /* TOWNED  */
    TPARAM = 334,                  /* TPARAM  */
    TPLUSPLUS = 335,               /* TPLUSPLUS  */
    TPRAGMA = 336,                 /* TPRAGMA  */
    TPRIMITIVE = 337,              /* TPRIMITIVE  */
    TPRIVATE = 338,                /* TPRIVATE  */
    TPROC = 339,                   /* TPROC  */
    TPROTOTYPE = 340,              /* TPROTOTYPE  */
    TPUBLIC = 341,                 /* TPUBLIC  */
    TREAL = 342,                   /* TREAL  */
    TRECORD = 343,                 /* TRECORD  */
    TREDUCE = 344,                 /* TREDUCE  */
    TREF = 345,                    /* TREF  */
    TREQUIRE = 346,                /* TREQUIRE  */
    TRETURN = 347,                 /* TRETURN  */
    TSCAN = 348,                   /* TSCAN  */
    TSELECT = 349,                 /* TSELECT  */
    TSERIAL = 350,                 /* TSERIAL  */
    TSHARED = 351,                 /* TSHARED  */
    TSINGLE = 352,                 /* TSINGLE  */
    TSPARSE = 353,                 /* TSPARSE  */
    TSTRING = 354,                 /* TSTRING  */
    TSUBDOMAIN = 355,              /* TSUBDOMAIN  */
    TSYNC = 356,                   /* TSYNC  */
    TTHEN = 357,                   /* TTHEN  */
    TTHIS = 358,                   /* TTHIS  */
    TTHROW = 359,                  /* TTHROW  */
    TTHROWS = 360,                 /* TTHROWS  */
    TTRUE = 361,                   /* TTRUE  */
    TTRY = 362,                    /* TTRY  */
    TTRYBANG = 363,                /* TTRYBANG  */
    TTYPE = 364,                   /* TTYPE  */
    TUINT = 365,                   /* TUINT  */
    TUNDERSCORE = 366,             /* TUNDERSCORE  */
    TUNION = 367,                  /* TUNION  */
    TUNMANAGED = 368,              /* TUNMANAGED  */
    TUSE = 369,                    /* TUSE  */
    TVAR = 370,                    /* TVAR  */
    TVOID = 371,                   /* TVOID  */
    TWHEN = 372,                   /* TWHEN  */
    TWHERE = 373,                  /* TWHERE  */
    TWHILE = 374,                  /* TWHILE  */
    TWITH = 375,                   /* TWITH  */
    TYIELD = 376,                  /* TYIELD  */
    TZIP = 377,                    /* TZIP  */
    TALIAS = 378,                  /* TALIAS  */
    TAND = 379,                    /* TAND  */
    TASSIGN = 380,                 /* TASSIGN  */
    TASSIGNBAND = 381,             /* TASSIGNBAND  */
    TASSIGNBOR = 382,              /* TASSIGNBOR  */
    TASSIGNBXOR = 383,             /* TASSIGNBXOR  */
    TASSIGNDIVIDE = 384,           /* TASSIGNDIVIDE  */
    TASSIGNEXP = 385,              /* TASSIGNEXP  */
    TASSIGNLAND = 386,             /* TASSIGNLAND  */
    TASSIGNLOR = 387,              /* TASSIGNLOR  */
    TASSIGNMINUS = 388,            /* TASSIGNMINUS  */
    TASSIGNMOD = 389,              /* TASSIGNMOD  */
    TASSIGNMULTIPLY = 390,         /* TASSIGNMULTIPLY  */
    TASSIGNPLUS = 391,             /* TASSIGNPLUS  */
    TASSIGNREDUCE = 392,           /* TASSIGNREDUCE  */
    TASSIGNSL = 393,               /* TASSIGNSL  */
    TASSIGNSR = 394,               /* TASSIGNSR  */
    TBANG = 395,                   /* TBANG  */
    TBAND = 396,                   /* TBAND  */
    TBNOT = 397,                   /* TBNOT  */
    TBOR = 398,                    /* TBOR  */
    TBXOR = 399,                   /* TBXOR  */
    TCOLON = 400,                  /* TCOLON  */
    TCOMMA = 401,                  /* TCOMMA  */
    TDIVIDE = 402,                 /* TDIVIDE  */
    TDOT = 403,                    /* TDOT  */
    TDOTDOT = 404,                 /* TDOTDOT  */
    TDOTDOTDOT = 405,              /* TDOTDOTDOT  */
    TEQUAL = 406,                  /* TEQUAL  */
    TEXP = 407,                    /* TEXP  */
    TGREATER = 408,                /* TGREATER  */
    TGREATEREQUAL = 409,           /* TGREATEREQUAL  */
    THASH = 410,                   /* THASH  */
    TIO = 411,                     /* TIO  */
    TLESS = 412,                   /* TLESS  */
    TLESSEQUAL = 413,              /* TLESSEQUAL  */
    TMINUS = 414,                  /* TMINUS  */
    TMOD = 415,                    /* TMOD  */
    TNOTEQUAL = 416,               /* TNOTEQUAL  */
    TOR = 417,                     /* TOR  */
    TPLUS = 418,                   /* TPLUS  */
    TQUESTION = 419,               /* TQUESTION  */
    TSEMI = 420,                   /* TSEMI  */
    TSHIFTLEFT = 421,              /* TSHIFTLEFT  */
    TSHIFTRIGHT = 422,             /* TSHIFTRIGHT  */
    TSTAR = 423,                   /* TSTAR  */
    TSWAP = 424,                   /* TSWAP  */
    TLCBR = 425,                   /* TLCBR  */
    TRCBR = 426,                   /* TRCBR  */
    TLP = 427,                     /* TLP  */
    TRP = 428,                     /* TRP  */
    TLSBR = 429,                   /* TLSBR  */
    TRSBR = 430,                   /* TRSBR  */
    TNOELSE = 431,                 /* TNOELSE  */
    TDOTDOTOPENHIGH = 432,         /* TDOTDOTOPENHIGH  */
    TUPLUS = 433,                  /* TUPLUS  */
    TUMINUS = 434,                 /* TUMINUS  */
    TLNOT = 435                    /* TLNOT  */
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
#line 211 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 435 "../include/bison-chapel.h"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
