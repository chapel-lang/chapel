/* A Bison parser, made by GNU Bison 3.7.2.  */

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

  #endif
#line 152 "chapel.ypp"

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
#line 175 "chapel.ypp"

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

#line 195 "../include/bison-chapel.h"

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
    TOPERATOR = 326,               /* TOPERATOR  */
    TOTHERWISE = 327,              /* TOTHERWISE  */
    TOUT = 328,                    /* TOUT  */
    TOVERRIDE = 329,               /* TOVERRIDE  */
    TOWNED = 330,                  /* TOWNED  */
    TPARAM = 331,                  /* TPARAM  */
    TPLUSPLUS = 332,               /* TPLUSPLUS  */
    TPRAGMA = 333,                 /* TPRAGMA  */
    TPRIMITIVE = 334,              /* TPRIMITIVE  */
    TPRIVATE = 335,                /* TPRIVATE  */
    TPROC = 336,                   /* TPROC  */
    TPROTOTYPE = 337,              /* TPROTOTYPE  */
    TPUBLIC = 338,                 /* TPUBLIC  */
    TREAL = 339,                   /* TREAL  */
    TRECORD = 340,                 /* TRECORD  */
    TREDUCE = 341,                 /* TREDUCE  */
    TREF = 342,                    /* TREF  */
    TREQUIRE = 343,                /* TREQUIRE  */
    TRETURN = 344,                 /* TRETURN  */
    TSCAN = 345,                   /* TSCAN  */
    TSELECT = 346,                 /* TSELECT  */
    TSERIAL = 347,                 /* TSERIAL  */
    TSHARED = 348,                 /* TSHARED  */
    TSINGLE = 349,                 /* TSINGLE  */
    TSPARSE = 350,                 /* TSPARSE  */
    TSTRING = 351,                 /* TSTRING  */
    TSUBDOMAIN = 352,              /* TSUBDOMAIN  */
    TSYNC = 353,                   /* TSYNC  */
    TTHEN = 354,                   /* TTHEN  */
    TTHIS = 355,                   /* TTHIS  */
    TTHROW = 356,                  /* TTHROW  */
    TTHROWS = 357,                 /* TTHROWS  */
    TTRUE = 358,                   /* TTRUE  */
    TTRY = 359,                    /* TTRY  */
    TTRYBANG = 360,                /* TTRYBANG  */
    TTYPE = 361,                   /* TTYPE  */
    TUINT = 362,                   /* TUINT  */
    TUNDERSCORE = 363,             /* TUNDERSCORE  */
    TUNION = 364,                  /* TUNION  */
    TUNMANAGED = 365,              /* TUNMANAGED  */
    TUSE = 366,                    /* TUSE  */
    TVAR = 367,                    /* TVAR  */
    TVOID = 368,                   /* TVOID  */
    TWHEN = 369,                   /* TWHEN  */
    TWHERE = 370,                  /* TWHERE  */
    TWHILE = 371,                  /* TWHILE  */
    TWITH = 372,                   /* TWITH  */
    TYIELD = 373,                  /* TYIELD  */
    TZIP = 374,                    /* TZIP  */
    TALIAS = 375,                  /* TALIAS  */
    TAND = 376,                    /* TAND  */
    TASSIGN = 377,                 /* TASSIGN  */
    TASSIGNBAND = 378,             /* TASSIGNBAND  */
    TASSIGNBOR = 379,              /* TASSIGNBOR  */
    TASSIGNBXOR = 380,             /* TASSIGNBXOR  */
    TASSIGNDIVIDE = 381,           /* TASSIGNDIVIDE  */
    TASSIGNEXP = 382,              /* TASSIGNEXP  */
    TASSIGNLAND = 383,             /* TASSIGNLAND  */
    TASSIGNLOR = 384,              /* TASSIGNLOR  */
    TASSIGNMINUS = 385,            /* TASSIGNMINUS  */
    TASSIGNMOD = 386,              /* TASSIGNMOD  */
    TASSIGNMULTIPLY = 387,         /* TASSIGNMULTIPLY  */
    TASSIGNPLUS = 388,             /* TASSIGNPLUS  */
    TASSIGNREDUCE = 389,           /* TASSIGNREDUCE  */
    TASSIGNSL = 390,               /* TASSIGNSL  */
    TASSIGNSR = 391,               /* TASSIGNSR  */
    TBANG = 392,                   /* TBANG  */
    TBAND = 393,                   /* TBAND  */
    TBNOT = 394,                   /* TBNOT  */
    TBOR = 395,                    /* TBOR  */
    TBXOR = 396,                   /* TBXOR  */
    TCOLON = 397,                  /* TCOLON  */
    TCOMMA = 398,                  /* TCOMMA  */
    TDIVIDE = 399,                 /* TDIVIDE  */
    TDOT = 400,                    /* TDOT  */
    TDOTDOT = 401,                 /* TDOTDOT  */
    TDOTDOTDOT = 402,              /* TDOTDOTDOT  */
    TEQUAL = 403,                  /* TEQUAL  */
    TEXP = 404,                    /* TEXP  */
    TGREATER = 405,                /* TGREATER  */
    TGREATEREQUAL = 406,           /* TGREATEREQUAL  */
    THASH = 407,                   /* THASH  */
    TIO = 408,                     /* TIO  */
    TLESS = 409,                   /* TLESS  */
    TLESSEQUAL = 410,              /* TLESSEQUAL  */
    TMINUS = 411,                  /* TMINUS  */
    TMOD = 412,                    /* TMOD  */
    TNOTEQUAL = 413,               /* TNOTEQUAL  */
    TOR = 414,                     /* TOR  */
    TPLUS = 415,                   /* TPLUS  */
    TQUESTION = 416,               /* TQUESTION  */
    TSEMI = 417,                   /* TSEMI  */
    TSHIFTLEFT = 418,              /* TSHIFTLEFT  */
    TSHIFTRIGHT = 419,             /* TSHIFTRIGHT  */
    TSTAR = 420,                   /* TSTAR  */
    TSWAP = 421,                   /* TSWAP  */
    TLCBR = 422,                   /* TLCBR  */
    TRCBR = 423,                   /* TRCBR  */
    TLP = 424,                     /* TLP  */
    TRP = 425,                     /* TRP  */
    TLSBR = 426,                   /* TLSBR  */
    TRSBR = 427,                   /* TRSBR  */
    TNOELSE = 428,                 /* TNOELSE  */
    TDOTDOTOPENHIGH = 429,         /* TDOTDOTOPENHIGH  */
    TUPLUS = 430,                  /* TUPLUS  */
    TUMINUS = 431,                 /* TUMINUS  */
    TLNOT = 432                    /* TLNOT  */
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
#line 206 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 426 "../include/bison-chapel.h"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
