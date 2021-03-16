/* A Bison parser, made by GNU Bison 3.7.5.  */

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
      scanner       = nullptr;
      latestComment = nullptr;
      generatedStmt = nullptr;
    }

    ParserContext(yyscan_t scannerIn)
    {
      scanner       = scannerIn;
      latestComment = nullptr;
      generatedStmt = nullptr;
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
    TFOREACH = 297,                /* TFOREACH  */
    TFORWARDING = 298,             /* TFORWARDING  */
    TIF = 299,                     /* TIF  */
    TIMAG = 300,                   /* TIMAG  */
    TIMPORT = 301,                 /* TIMPORT  */
    TIN = 302,                     /* TIN  */
    TINCLUDE = 303,                /* TINCLUDE  */
    TINDEX = 304,                  /* TINDEX  */
    TINLINE = 305,                 /* TINLINE  */
    TINOUT = 306,                  /* TINOUT  */
    TINT = 307,                    /* TINT  */
    TITER = 308,                   /* TITER  */
    TINITEQUALS = 309,             /* TINITEQUALS  */
    TIMPLEMENTS = 310,             /* TIMPLEMENTS  */
    TINTERFACE = 311,              /* TINTERFACE  */
    TLABEL = 312,                  /* TLABEL  */
    TLAMBDA = 313,                 /* TLAMBDA  */
    TLET = 314,                    /* TLET  */
    TLIFETIME = 315,               /* TLIFETIME  */
    TLOCAL = 316,                  /* TLOCAL  */
    TLOCALE = 317,                 /* TLOCALE  */
    TMINUSMINUS = 318,             /* TMINUSMINUS  */
    TMODULE = 319,                 /* TMODULE  */
    TNEW = 320,                    /* TNEW  */
    TNIL = 321,                    /* TNIL  */
    TNOINIT = 322,                 /* TNOINIT  */
    TNONE = 323,                   /* TNONE  */
    TNOTHING = 324,                /* TNOTHING  */
    TON = 325,                     /* TON  */
    TONLY = 326,                   /* TONLY  */
    TOPERATOR = 327,               /* TOPERATOR  */
    TOTHERWISE = 328,              /* TOTHERWISE  */
    TOUT = 329,                    /* TOUT  */
    TOVERRIDE = 330,               /* TOVERRIDE  */
    TOWNED = 331,                  /* TOWNED  */
    TPARAM = 332,                  /* TPARAM  */
    TPLUSPLUS = 333,               /* TPLUSPLUS  */
    TPRAGMA = 334,                 /* TPRAGMA  */
    TPRIMITIVE = 335,              /* TPRIMITIVE  */
    TPRIVATE = 336,                /* TPRIVATE  */
    TPROC = 337,                   /* TPROC  */
    TPROTOTYPE = 338,              /* TPROTOTYPE  */
    TPUBLIC = 339,                 /* TPUBLIC  */
    TREAL = 340,                   /* TREAL  */
    TRECORD = 341,                 /* TRECORD  */
    TREDUCE = 342,                 /* TREDUCE  */
    TREF = 343,                    /* TREF  */
    TREQUIRE = 344,                /* TREQUIRE  */
    TRETURN = 345,                 /* TRETURN  */
    TSCAN = 346,                   /* TSCAN  */
    TSELECT = 347,                 /* TSELECT  */
    TSERIAL = 348,                 /* TSERIAL  */
    TSHARED = 349,                 /* TSHARED  */
    TSINGLE = 350,                 /* TSINGLE  */
    TSPARSE = 351,                 /* TSPARSE  */
    TSTRING = 352,                 /* TSTRING  */
    TSUBDOMAIN = 353,              /* TSUBDOMAIN  */
    TSYNC = 354,                   /* TSYNC  */
    TTHEN = 355,                   /* TTHEN  */
    TTHIS = 356,                   /* TTHIS  */
    TTHROW = 357,                  /* TTHROW  */
    TTHROWS = 358,                 /* TTHROWS  */
    TTRUE = 359,                   /* TTRUE  */
    TTRY = 360,                    /* TTRY  */
    TTRYBANG = 361,                /* TTRYBANG  */
    TTYPE = 362,                   /* TTYPE  */
    TUINT = 363,                   /* TUINT  */
    TUNDERSCORE = 364,             /* TUNDERSCORE  */
    TUNION = 365,                  /* TUNION  */
    TUNMANAGED = 366,              /* TUNMANAGED  */
    TUSE = 367,                    /* TUSE  */
    TVAR = 368,                    /* TVAR  */
    TVOID = 369,                   /* TVOID  */
    TWHEN = 370,                   /* TWHEN  */
    TWHERE = 371,                  /* TWHERE  */
    TWHILE = 372,                  /* TWHILE  */
    TWITH = 373,                   /* TWITH  */
    TYIELD = 374,                  /* TYIELD  */
    TZIP = 375,                    /* TZIP  */
    TALIAS = 376,                  /* TALIAS  */
    TAND = 377,                    /* TAND  */
    TASSIGN = 378,                 /* TASSIGN  */
    TASSIGNBAND = 379,             /* TASSIGNBAND  */
    TASSIGNBOR = 380,              /* TASSIGNBOR  */
    TASSIGNBXOR = 381,             /* TASSIGNBXOR  */
    TASSIGNDIVIDE = 382,           /* TASSIGNDIVIDE  */
    TASSIGNEXP = 383,              /* TASSIGNEXP  */
    TASSIGNLAND = 384,             /* TASSIGNLAND  */
    TASSIGNLOR = 385,              /* TASSIGNLOR  */
    TASSIGNMINUS = 386,            /* TASSIGNMINUS  */
    TASSIGNMOD = 387,              /* TASSIGNMOD  */
    TASSIGNMULTIPLY = 388,         /* TASSIGNMULTIPLY  */
    TASSIGNPLUS = 389,             /* TASSIGNPLUS  */
    TASSIGNREDUCE = 390,           /* TASSIGNREDUCE  */
    TASSIGNSL = 391,               /* TASSIGNSL  */
    TASSIGNSR = 392,               /* TASSIGNSR  */
    TBANG = 393,                   /* TBANG  */
    TBAND = 394,                   /* TBAND  */
    TBNOT = 395,                   /* TBNOT  */
    TBOR = 396,                    /* TBOR  */
    TBXOR = 397,                   /* TBXOR  */
    TCOLON = 398,                  /* TCOLON  */
    TCOMMA = 399,                  /* TCOMMA  */
    TDIVIDE = 400,                 /* TDIVIDE  */
    TDOT = 401,                    /* TDOT  */
    TDOTDOT = 402,                 /* TDOTDOT  */
    TDOTDOTDOT = 403,              /* TDOTDOTDOT  */
    TEQUAL = 404,                  /* TEQUAL  */
    TEXP = 405,                    /* TEXP  */
    TGREATER = 406,                /* TGREATER  */
    TGREATEREQUAL = 407,           /* TGREATEREQUAL  */
    THASH = 408,                   /* THASH  */
    TIO = 409,                     /* TIO  */
    TLESS = 410,                   /* TLESS  */
    TLESSEQUAL = 411,              /* TLESSEQUAL  */
    TMINUS = 412,                  /* TMINUS  */
    TMOD = 413,                    /* TMOD  */
    TNOTEQUAL = 414,               /* TNOTEQUAL  */
    TOR = 415,                     /* TOR  */
    TPLUS = 416,                   /* TPLUS  */
    TQUESTION = 417,               /* TQUESTION  */
    TSEMI = 418,                   /* TSEMI  */
    TSHIFTLEFT = 419,              /* TSHIFTLEFT  */
    TSHIFTRIGHT = 420,             /* TSHIFTRIGHT  */
    TSTAR = 421,                   /* TSTAR  */
    TSWAP = 422,                   /* TSWAP  */
    TLCBR = 423,                   /* TLCBR  */
    TRCBR = 424,                   /* TRCBR  */
    TLP = 425,                     /* TLP  */
    TRP = 426,                     /* TRP  */
    TLSBR = 427,                   /* TLSBR  */
    TRSBR = 428,                   /* TRSBR  */
    TNOELSE = 429,                 /* TNOELSE  */
    TDOTDOTOPENHIGH = 430,         /* TDOTDOTOPENHIGH  */
    TUPLUS = 431,                  /* TUPLUS  */
    TUMINUS = 432,                 /* TUMINUS  */
    TLNOT = 433                    /* TLNOT  */
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

#line 427 "../include/bison-chapel.h"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
