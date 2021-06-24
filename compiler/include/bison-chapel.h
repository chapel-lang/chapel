/* A Bison parser, made by GNU Bison 3.7.6.  */

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
    TMINUSMINUS = 319,             /* TMINUSMINUS  */
    TMODULE = 320,                 /* TMODULE  */
    TNEW = 321,                    /* TNEW  */
    TNIL = 322,                    /* TNIL  */
    TNOINIT = 323,                 /* TNOINIT  */
    TNONE = 324,                   /* TNONE  */
    TNOTHING = 325,                /* TNOTHING  */
    TON = 326,                     /* TON  */
    TONLY = 327,                   /* TONLY  */
    TOPERATOR = 328,               /* TOPERATOR  */
    TOTHERWISE = 329,              /* TOTHERWISE  */
    TOUT = 330,                    /* TOUT  */
    TOVERRIDE = 331,               /* TOVERRIDE  */
    TOWNED = 332,                  /* TOWNED  */
    TPARAM = 333,                  /* TPARAM  */
    TPLUSPLUS = 334,               /* TPLUSPLUS  */
    TPRAGMA = 335,                 /* TPRAGMA  */
    TPRIMITIVE = 336,              /* TPRIMITIVE  */
    TPRIVATE = 337,                /* TPRIVATE  */
    TPROC = 338,                   /* TPROC  */
    TPROTOTYPE = 339,              /* TPROTOTYPE  */
    TPUBLIC = 340,                 /* TPUBLIC  */
    TREAL = 341,                   /* TREAL  */
    TRECORD = 342,                 /* TRECORD  */
    TREDUCE = 343,                 /* TREDUCE  */
    TREF = 344,                    /* TREF  */
    TREQUIRE = 345,                /* TREQUIRE  */
    TRETURN = 346,                 /* TRETURN  */
    TSCAN = 347,                   /* TSCAN  */
    TSELECT = 348,                 /* TSELECT  */
    TSERIAL = 349,                 /* TSERIAL  */
    TSHARED = 350,                 /* TSHARED  */
    TSINGLE = 351,                 /* TSINGLE  */
    TSPARSE = 352,                 /* TSPARSE  */
    TSTRING = 353,                 /* TSTRING  */
    TSUBDOMAIN = 354,              /* TSUBDOMAIN  */
    TSYNC = 355,                   /* TSYNC  */
    TTHEN = 356,                   /* TTHEN  */
    TTHIS = 357,                   /* TTHIS  */
    TTHROW = 358,                  /* TTHROW  */
    TTHROWS = 359,                 /* TTHROWS  */
    TTRUE = 360,                   /* TTRUE  */
    TTRY = 361,                    /* TTRY  */
    TTRYBANG = 362,                /* TTRYBANG  */
    TTYPE = 363,                   /* TTYPE  */
    TUINT = 364,                   /* TUINT  */
    TUNDERSCORE = 365,             /* TUNDERSCORE  */
    TUNION = 366,                  /* TUNION  */
    TUNMANAGED = 367,              /* TUNMANAGED  */
    TUSE = 368,                    /* TUSE  */
    TVAR = 369,                    /* TVAR  */
    TVOID = 370,                   /* TVOID  */
    TWHEN = 371,                   /* TWHEN  */
    TWHERE = 372,                  /* TWHERE  */
    TWHILE = 373,                  /* TWHILE  */
    TWITH = 374,                   /* TWITH  */
    TYIELD = 375,                  /* TYIELD  */
    TZIP = 376,                    /* TZIP  */
    TALIAS = 377,                  /* TALIAS  */
    TAND = 378,                    /* TAND  */
    TASSIGN = 379,                 /* TASSIGN  */
    TASSIGNBAND = 380,             /* TASSIGNBAND  */
    TASSIGNBOR = 381,              /* TASSIGNBOR  */
    TASSIGNBXOR = 382,             /* TASSIGNBXOR  */
    TASSIGNDIVIDE = 383,           /* TASSIGNDIVIDE  */
    TASSIGNEXP = 384,              /* TASSIGNEXP  */
    TASSIGNLAND = 385,             /* TASSIGNLAND  */
    TASSIGNLOR = 386,              /* TASSIGNLOR  */
    TASSIGNMINUS = 387,            /* TASSIGNMINUS  */
    TASSIGNMOD = 388,              /* TASSIGNMOD  */
    TASSIGNMULTIPLY = 389,         /* TASSIGNMULTIPLY  */
    TASSIGNPLUS = 390,             /* TASSIGNPLUS  */
    TASSIGNREDUCE = 391,           /* TASSIGNREDUCE  */
    TASSIGNSL = 392,               /* TASSIGNSL  */
    TASSIGNSR = 393,               /* TASSIGNSR  */
    TBANG = 394,                   /* TBANG  */
    TBAND = 395,                   /* TBAND  */
    TBNOT = 396,                   /* TBNOT  */
    TBOR = 397,                    /* TBOR  */
    TBXOR = 398,                   /* TBXOR  */
    TCOLON = 399,                  /* TCOLON  */
    TCOMMA = 400,                  /* TCOMMA  */
    TDIVIDE = 401,                 /* TDIVIDE  */
    TDOT = 402,                    /* TDOT  */
    TDOTDOT = 403,                 /* TDOTDOT  */
    TDOTDOTDOT = 404,              /* TDOTDOTDOT  */
    TEQUAL = 405,                  /* TEQUAL  */
    TEXP = 406,                    /* TEXP  */
    TGREATER = 407,                /* TGREATER  */
    TGREATEREQUAL = 408,           /* TGREATEREQUAL  */
    THASH = 409,                   /* THASH  */
    TIO = 410,                     /* TIO  */
    TLESS = 411,                   /* TLESS  */
    TLESSEQUAL = 412,              /* TLESSEQUAL  */
    TMINUS = 413,                  /* TMINUS  */
    TMOD = 414,                    /* TMOD  */
    TNOTEQUAL = 415,               /* TNOTEQUAL  */
    TOR = 416,                     /* TOR  */
    TPLUS = 417,                   /* TPLUS  */
    TQUESTION = 418,               /* TQUESTION  */
    TSEMI = 419,                   /* TSEMI  */
    TSHIFTLEFT = 420,              /* TSHIFTLEFT  */
    TSHIFTRIGHT = 421,             /* TSHIFTRIGHT  */
    TSTAR = 422,                   /* TSTAR  */
    TSWAP = 423,                   /* TSWAP  */
    TLCBR = 424,                   /* TLCBR  */
    TRCBR = 425,                   /* TRCBR  */
    TLP = 426,                     /* TLP  */
    TRP = 427,                     /* TRP  */
    TLSBR = 428,                   /* TLSBR  */
    TRSBR = 429,                   /* TRSBR  */
    TNOELSE = 430,                 /* TNOELSE  */
    TDOTDOTOPENHIGH = 431,         /* TDOTDOTOPENHIGH  */
    TUPLUS = 432,                  /* TUPLUS  */
    TUMINUS = 433,                 /* TUMINUS  */
    TLNOT = 434                    /* TLNOT  */
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

#line 428 "../include/bison-chapel.h"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
